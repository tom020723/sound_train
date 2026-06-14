/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_x-cube-ai.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#ifndef ARM_MATH_CM4
#define ARM_MATH_CM4
#endif

#include "rain_detector_model.h"
#include "rain_detector_model_data.h"
#include "mfcc_params.h"
#include "arm_math.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    STATE_WINDOW_OPENING,   /* motor driving window open after boot  */
    STATE_WINDOW_OPEN,      /* window open, monitoring for rain      */
    STATE_WINDOW_CLOSING,   /* rain detected, motor closing window   */
    STATE_WINDOW_CLOSED     /* window closed, waiting for manual open */
} WindowState;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define THRESHOLD_RAIN_DETECT    5
#define THRESHOLD_NOT_RAIN       30

/* H-bridge motor pins (IN1/IN2) */
#define MOTOR_IN1_PIN            GPIO_PIN_5
#define MOTOR_IN1_PORT           GPIOA
#define MOTOR_IN2_PIN            GPIO_PIN_6
#define MOTOR_IN2_PORT           GPIOA

/* HC-SR04 ultrasonic sensor pins */
#define SONAR_TRIG_PIN           GPIO_PIN_0
#define SONAR_TRIG_PORT          GPIOB
#define SONAR_ECHO_PIN           GPIO_PIN_1
#define SONAR_ECHO_PORT          GPIOB

/* Distance threshold (cm): >= OPEN, < CLOSED */
#define WINDOW_CLOSED_CM         8U

/* Motor run limit; force state transition if HC-SR04 never confirms */
#define MOTOR_TIMEOUT_MS         6000U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

I2S_HandleTypeDef hi2s2;
DMA_HandleTypeDef hdma_spi2_rx;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
WindowState system_state          = STATE_WINDOW_OPENING;
int         rain_strike_count     = 0;
int         not_rain_strike_count = 0;
uint32_t    motor_start_time      = 0;

/* ICS43434 I2S DMA circular buffer
 * 160 frames x 2 (stereo) x 2 (double-buffer halves) = 640 half-words (1.28 KB)
 * ICS43434 L/R=GND -> left channel only; right channel words are 0
 * Left channel: buf[0], buf[2], buf[4], ... (even indices) */
#define I2S_CHUNK_FRAMES  160U
#define I2S_DMA_BUF_LEN  (I2S_CHUNK_FRAMES * 2U * 2U)   /* 640 */

static uint16_t         i2s_dma_buf[I2S_DMA_BUF_LEN];
static volatile uint8_t i2s_half        = 0;     /* 0=first half, 1=second half ready */
static volatile bool    i2s_chunk_rdy   = false;
static volatile bool    mic_accumulating = true;
static int              mic_sample_count = 0;
static volatile bool    mic_data_ready   = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_CRC_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2S2_Init(void);
/* USER CODE BEGIN PFP */
void        AI_Run_Microphone_Inference(int8_t* mic_data, int8_t* out_not_rain, int8_t* out_rain);
void        Hardware_MFCC_Process(const short* p_mic_signal, int8_t* p_out_mfcc_int8);
static void     Motor_Close(void);
static void     Motor_Open(void);
static void     Motor_Stop(void);
static void     DWT_DelayUs(uint32_t us);
static uint32_t HC_SR04_ReadCm(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_CRC_Init();
  MX_USART2_UART_Init();
  MX_I2S2_Init();
  MX_X_CUBE_AI_Init();
  /* USER CODE BEGIN 2 */
  /* Enable DWT cycle counter for HC-SR04 microsecond timing */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;

  setvbuf(stdout, NULL, _IONBF, 0);
  printf("\r\n\r\nSTM32 BOARD BOOTING SUCCESS!!\r\n");

  /* Start I2S DMA; callbacks fill mic buffer continuously */
  printf("  [DBG] Calling HAL_I2S_Receive_DMA...\r\n");
  HAL_StatusTypeDef i2s_ret = HAL_I2S_Receive_DMA(&hi2s2, i2s_dma_buf, I2S_DMA_BUF_LEN);
  printf("  [DBG] HAL_I2S_Receive_DMA returned: %d\r\n", (int)i2s_ret);
  if (i2s_ret != HAL_OK) {
      printf("  [I2S] DMA start FAILED (err=%d, i2s_err=0x%lx)\r\n",
             (int)i2s_ret, hi2s2.ErrorCode);
  } else {
      printf("  [I2S] DMA RX started (ICS43434 @ 16kHz)\r\n");
  }

  /* Open window on boot */
  printf("  [STATE] BOOT -> OPENING\r\n");
  Motor_Open();
  motor_start_time = HAL_GetTick();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  static uint32_t total_seconds = 0;
  static short    mic_raw_signal[16000];
  static int8_t   test_buffer[1274];   /* 98 frames x 13 coefs */

  const char* event_log = "System Init. Opening window...";

  while (1)
  {
    /* USER CODE END WHILE */

  MX_X_CUBE_AI_Process();
    /* USER CODE BEGIN 3 */

    /* Accumulate I2S chunks into mic_raw_signal (non-blocking, callback-driven) */
    if (i2s_chunk_rdy && mic_accumulating) {
        i2s_chunk_rdy = false;
        uint16_t *src = i2s_dma_buf +
                        (i2s_half ? I2S_CHUNK_FRAMES * 2U : 0U);
        /* Extract left channel (even indices) into mic_raw_signal */
        for (int i = 0; i < (int)I2S_CHUNK_FRAMES; i++) {
            mic_raw_signal[mic_sample_count++] = (int16_t)src[i * 2];
        }
        if (mic_sample_count >= 16000) {
            mic_sample_count = 0;
            mic_data_ready   = true;
            mic_accumulating = false;   /* hold off until MFCC processing completes */
        }
    }

    /* 1 second of audio ready: run MFCC + inference + state machine */
    if (mic_data_ready) {
        mic_data_ready = false;
        total_seconds++;

        /* Peak-normalize mic signal to match training data amplitude (~10000 peak).
         * Training audio peaks at 7000-10000; ICS43434 pickup is much quieter.
         * Skip normalization if signal is below noise floor (basically silence). */
        #define MIC_NOISE_FLOOR   300
        #define MIC_TARGET_PEAK   10000
        int16_t mic_max = 0;
        for (int i = 0; i < 16000; i++) {
            int16_t v = mic_raw_signal[i] < 0 ? -mic_raw_signal[i] : mic_raw_signal[i];
            if (v > mic_max) mic_max = v;
        }
        printf("  [MIC] max_amp: %d\r\n", mic_max);
        if (mic_max > MIC_NOISE_FLOOR) {
            float norm_scale = (float)MIC_TARGET_PEAK / (float)mic_max;
            for (int i = 0; i < 16000; i++) {
                int32_t s = (int32_t)((float)mic_raw_signal[i] * norm_scale);
                if (s >  32767) s =  32767;
                if (s < -32768) s = -32768;
                mic_raw_signal[i] = (int16_t)s;
            }
        }

        Hardware_MFCC_Process(mic_raw_signal, test_buffer);
        printf("  [MFCC dbg] f0: %4d %4d %4d\r\n",
               (int)test_buffer[0], (int)test_buffer[1], (int)test_buffer[2]);

        int8_t out_not_rain = 0;
        int8_t out_rain     = 0;
        AI_Run_Microphone_Inference(test_buffer, &out_not_rain, &out_rain);

        /* Bidirectional counter: increment rain or clear strike toward threshold */
        if (out_rain > out_not_rain) {
            if (rain_strike_count     < THRESHOLD_RAIN_DETECT) rain_strike_count++;
            if (not_rain_strike_count > 0)                     not_rain_strike_count--;
        } else {
            if (not_rain_strike_count < THRESHOLD_NOT_RAIN)    not_rain_strike_count++;
            if (rain_strike_count     > 0)                     rain_strike_count--;
        }

        /* HC-SR04 distance measurement (once per second) */
        uint32_t distance_cm = HC_SR04_ReadCm();
        bool     window_open = (distance_cm >= WINDOW_CLOSED_CM);
        printf("  [SONAR] %u cm -> %s\r\n",
               (unsigned)distance_cm, window_open ? "OPEN" : "CLOSED");

        /* State machine */
        switch (system_state) {

            case STATE_WINDOW_OPENING:
                /* Confirm OPEN via HC-SR04 or motor timeout */
                if (window_open ||
                    (HAL_GetTick() - motor_start_time) >= MOTOR_TIMEOUT_MS) {
                    printf("  [STATE] OPENING -> OPEN%s\r\n",
                           window_open ? "" : " (timeout)");
                    Motor_Stop();
                    rain_strike_count     = 0;
                    not_rain_strike_count = 0;
                    system_state = STATE_WINDOW_OPEN;
                    event_log    = window_open
                                   ? "Window OPEN confirmed. Monitoring for rain."
                                   : "Window OPEN (timeout). Monitoring for rain.";
                } else {
                    event_log = "Window Opening... Motor is active.";
                }
                break;

            case STATE_WINDOW_OPEN:
                /* Close window when rain_strike reaches threshold */
                if (rain_strike_count >= THRESHOLD_RAIN_DETECT) {
                    printf("  [STATE] OPEN -> CLOSING\r\n");
                    Motor_Close();
                    motor_start_time      = HAL_GetTick();
                    not_rain_strike_count = 0;
                    system_state = STATE_WINDOW_CLOSING;
                    event_log    = "RAIN DETECTED! Motor ON. Window closing.";
                } else {
                    event_log = "Weather Clear. Window remains OPEN.";
                }
                break;

            case STATE_WINDOW_CLOSING:
                /* Confirm CLOSED via HC-SR04 or motor timeout */
                if (!window_open ||
                    (HAL_GetTick() - motor_start_time) >= MOTOR_TIMEOUT_MS) {
                    printf("  [STATE] CLOSING -> CLOSED%s\r\n",
                           !window_open ? "" : " (timeout)");
                    Motor_Stop();
                    not_rain_strike_count = 0;
                    system_state = STATE_WINDOW_CLOSED;
                    event_log    = !window_open
                                   ? "Window CLOSED confirmed. Waiting for manual open."
                                   : "Window CLOSED (timeout). Waiting for manual open.";
                } else {
                    event_log = "Window Closing... Motor is active.";
                }
                break;

            case STATE_WINDOW_CLOSED:
                /* Manual open detected via HC-SR04; resume monitoring */
                if (window_open) {
                    printf("  [STATE] CLOSED -> OPEN (manual open detected)\r\n");
                    rain_strike_count     = 0;
                    not_rain_strike_count = 0;
                    system_state = STATE_WINDOW_OPEN;
                    event_log    = "Manual open detected. Monitoring for rain.";
                } else {
                    event_log = "Window CLOSED. Waiting for manual open...";
                }
                break;
        }
        int percent_rain     = (int)(((out_rain     + 128) * 100) / 255);
        int percent_not_rain = (int)(((out_not_rain + 128) * 100) / 255);

        /* Dashboard output */
        /* printf("\033[2J\033[H"); */ /* screen clear - disabled during debug */
        printf("==================================================\r\n");
        printf("           AI Smart Window Dashboard              \r\n");
        printf("==================================================\r\n");
        printf("  Uptime        : %lu sec\r\n", total_seconds);

        const char* state_str = "UNKNOWN";
        switch (system_state) {
            case STATE_WINDOW_OPENING: state_str = "OPENING (Motor ON)";  break;
            case STATE_WINDOW_OPEN:    state_str = "OPEN    (Monitoring)"; break;
            case STATE_WINDOW_CLOSING: state_str = "CLOSING (Motor ON)";  break;
            case STATE_WINDOW_CLOSED:  state_str = "CLOSED  (Monitoring)"; break;
        }
        printf("  System State  : %s\r\n", state_str);
        printf("  Window Pos    : %u cm (%s)\r\n",
               (unsigned)distance_cm, window_open ? "OPEN" : "CLOSED");
        printf("--------------------------------------------------\r\n");
        printf("         Neural Network Output Data\r\n");
        printf("  - Raw Outputs : Not Rain [%4d] | Rain [%4d]\r\n", out_not_rain, out_rain);
        printf("  - Not Rain Probability : %3d %%\r\n", percent_not_rain);
        printf("  - Rain    Probability  : %3d %%\r\n", percent_rain);
        printf("--------------------------------------------------\r\n");
        printf("         Confidence Threshold Counters\r\n");
        printf("  - Rain   Strikes : %2d / %d\r\n", rain_strike_count,     THRESHOLD_RAIN_DETECT);
        printf("  - Clear  Strikes : %2d / %d\r\n", not_rain_strike_count, THRESHOLD_NOT_RAIN);
        printf("==================================================\r\n");
        printf("  Event Log     : %s\r\n", event_log);

        /* Re-enable accumulation for next 1-second window */
        mic_accumulating = true;
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief I2S2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S2_Init(void)
{

  /* USER CODE BEGIN I2S2_Init 0 */

  /* USER CODE END I2S2_Init 0 */

  /* USER CODE BEGIN I2S2_Init 1 */

  /* USER CODE END I2S2_Init 1 */
  hi2s2.Instance = SPI2;
  hi2s2.Init.Mode = I2S_MODE_MASTER_RX;
  hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B_EXTENDED;
  hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
  hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_16K;
  hi2s2.Init.CPOL = I2S_CPOL_LOW;
  hi2s2.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S2_Init 2 */

  /* USER CODE END I2S2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, MOTOR_IN1_Pin|MOTOR_IN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SONAR_TRIG_GPIO_Port, SONAR_TRIG_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : MOTOR_IN1_Pin MOTOR_IN2_Pin */
  GPIO_InitStruct.Pin = MOTOR_IN1_Pin|MOTOR_IN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SONAR_TRIG_Pin */
  GPIO_InitStruct.Pin = SONAR_TRIG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SONAR_TRIG_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SONAR_ECHO_Pin */
  GPIO_InitStruct.Pin = SONAR_ECHO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SONAR_ECHO_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}

/* DMA half-complete callback: first half of i2s_dma_buf[0..319] ready */
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    if (hi2s->Instance == SPI2) {
        i2s_half      = 0;
        i2s_chunk_rdy = true;
    }
}

/* DMA full-complete callback: second half of i2s_dma_buf[320..639] ready */
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    if (hi2s->Instance == SPI2) {
        i2s_half      = 1;
        i2s_chunk_rdy = true;
    }
}

/* DWT cycle-counter based microsecond delay (used for HC-SR04 TRIG pulse) */
static void DWT_DelayUs(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000U);
    while ((DWT->CYCCNT - start) < ticks);
}

/* HC-SR04 distance measurement. Returns cm, 999 on timeout.
 * Sends 10us TRIG pulse, measures ECHO HIGH duration. */
static uint32_t HC_SR04_ReadCm(void)
{
    HAL_GPIO_WritePin(SONAR_TRIG_PORT, SONAR_TRIG_PIN, GPIO_PIN_SET);
    DWT_DelayUs(10);
    HAL_GPIO_WritePin(SONAR_TRIG_PORT, SONAR_TRIG_PIN, GPIO_PIN_RESET);

    uint32_t t_start = HAL_GetTick();
    while (HAL_GPIO_ReadPin(SONAR_ECHO_PORT, SONAR_ECHO_PIN) == GPIO_PIN_RESET) {
        if (HAL_GetTick() - t_start > 30U) return 999U;
    }
    uint32_t echo_start = DWT->CYCCNT;

    t_start = HAL_GetTick();
    while (HAL_GPIO_ReadPin(SONAR_ECHO_PORT, SONAR_ECHO_PIN) == GPIO_PIN_SET) {
        if (HAL_GetTick() - t_start > 30U) return 999U;
    }
    uint32_t echo_end = DWT->CYCCNT;

    uint32_t elapsed_us = (echo_end - echo_start) / (SystemCoreClock / 1000000U);
    return elapsed_us / 58U;
}

/* Drive IN2 LOW then IN1 HIGH to avoid shoot-through */
static void Motor_Close(void)
{
    HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_SET);
    printf("  [MOTOR] Motor ON  (CLOSE) PA5=H PA6=L\r\n");
}

/* Drive IN1 LOW then IN2 HIGH */
static void Motor_Open(void)
{
    HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_SET);
    printf("  [MOTOR] Motor ON  (OPEN)  PA5=L PA6=H\r\n");
}

static void Motor_Stop(void)
{
    HAL_GPIO_WritePin(MOTOR_IN1_PORT, MOTOR_IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_IN2_PORT, MOTOR_IN2_PIN, GPIO_PIN_RESET);
    printf("  [MOTOR] Motor OFF         PA5=L PA6=L\r\n");
}

void Hardware_MFCC_Process(const short* p_mic_signal, int8_t* p_out_mfcc_int8)
{
    #define _NUM_COEFS       MFCC_NUM_COEFS     /* 13  */
    #define _NUM_FRAMES      98                 /* (16000-480)/160 + 1 */
    #define _FFT_LEN         MFCC_FFT_LENGTH    /* 512 */
    #define _FRAME_LEN       MFCC_FRAME_LENGTH  /* 480 */
    #define _HOP_LEN         MFCC_FRAME_STEP    /* 160 */

    const float AI_SCALE      = 0.35635978f;
    const int   AI_ZERO_POINT = 70;

    static arm_mfcc_instance_f32 mfcc_inst;
    static float32_t frame_buffer_f32[_FFT_LEN];
    static float32_t mfcc_out_f32[_NUM_COEFS];
    static float32_t mfcc_tmp[_FFT_LEN + 2U];

    static bool is_initialized = false;
    int out_idx = 0;

    if (!is_initialized) {
        arm_status status = arm_mfcc_init_f32(
            &mfcc_inst,
            _FFT_LEN,
            MFCC_NUM_MEL_FILTERS,
            _NUM_COEFS,
            mfcc_dct_coefs,
            mfcc_filter_pos,
            mfcc_filter_lengths,
            mfcc_filter_coefs,
            mfcc_window_coefs);

        if (status != ARM_MATH_SUCCESS) {
            printf("\r\nCMSIS-DSP MFCC Init Failed!!\r\n");
        }
        is_initialized = true;
    }

    for (int f = 0; f < _NUM_FRAMES; f++)
    {
        int frame_start = f * _HOP_LEN;

        memset(frame_buffer_f32, 0, sizeof(frame_buffer_f32));

        for (int i = 0; i < _FRAME_LEN; i++)
        {
            frame_buffer_f32[i] = (float32_t)p_mic_signal[frame_start + i] / 32768.0f;
        }

        arm_mfcc_f32(&mfcc_inst, frame_buffer_f32, mfcc_out_f32, mfcc_tmp);

        for (int c = 0; c < _NUM_COEFS; c++)
        {
            float32_t quantized_val = (mfcc_out_f32[c] / AI_SCALE) + AI_ZERO_POINT;
            int32_t rounded_val = (quantized_val >= 0.0f)
                ? (int32_t)(quantized_val + 0.5f)
                : (int32_t)(quantized_val - 0.5f);

            if (rounded_val >  127) rounded_val =  127;
            if (rounded_val < -128) rounded_val = -128;

            p_out_mfcc_int8[out_idx++] = (int8_t)rounded_val;
        }
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
