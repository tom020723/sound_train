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
#include "audio_data.h"
#include "mfcc_params.h"
#include "arm_math.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum{
	STATE_WINDOW_OPEN,
	STATE_WINDOW_CLOSING,
	STATE_WINDOW_CLOSED,
	STATE_LOW_POWER_IDLE
}WindowState;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define THRESHOLD_RAIN_DETECT 5
#define THRESHOLD_NOT_RAIN 30
#define WINDOW_CLOSE_DURATION 5000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
WindowState system_state = STATE_WINDOW_OPEN;

int rain_strike_count = 0;
int not_rain_strike_count = 0;

uint32_t motor_start_time = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CRC_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void AI_Run_Microphone_Inference(int8_t* mic_data, int8_t* out_not_rain, int8_t* out_rain);
void Hardware_MFCC_Process(const short* p_mic_signal, int8_t* p_out_mfcc_int8);
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
  MX_CRC_Init();
  MX_USART2_UART_Init();
  MX_X_CUBE_AI_Init();
  /* USER CODE BEGIN 2 */
  setvbuf(stdout, NULL, _IONBF, 0);
  printf("\r\n\r\nSTM32 BOARD BOOTING SUCCESS!!\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  static uint32_t last_tick = 0;
  static uint32_t total_seconds = 0;
  static int p_idx = 0;
  static int n_idx = 0;

  static short mic_raw_signal[16000];
  static int8_t test_buffer[1274];  /* 98 frames × 13 coefs */

  const char* event_log = "System Init Complete.";

  while (1)
  {
    /* USER CODE END WHILE */

  MX_X_CUBE_AI_Process();
    /* USER CODE BEGIN 3 */

      if (HAL_GetTick() - last_tick >= 1000)
      {
      	last_tick = HAL_GetTick();
      	total_seconds++;

      	if (total_seconds <= 10) {
      		memcpy(mic_raw_signal, test_rain_audio[p_idx], 16000 * sizeof(short));
      		p_idx = (p_idx + 1) % 2;
      	}
      	else {
      		memcpy(mic_raw_signal, test_not_rain_audio[n_idx], 16000 * sizeof(short));
      		n_idx = (n_idx + 1) % 2;
      	}

      	Hardware_MFCC_Process(mic_raw_signal, test_buffer);

      	/* MFCC ?��버그: �? ?��?��?�� 3�? 계수 출력 (값이 �??��?���? ?��?��?��) */
      	printf("  [MFCC dbg] f0: %4d %4d %4d\r\n",
      	       (int)test_buffer[0], (int)test_buffer[1], (int)test_buffer[2]);

      	int8_t out_not_rain = 0;
      	int8_t out_rain     = 0;

      	AI_Run_Microphone_Inference(test_buffer, &out_not_rain, &out_rain);


      	if (out_rain > out_not_rain) {
      		if(rain_strike_count < THRESHOLD_RAIN_DETECT) rain_strike_count++;
      	}
      	else {
      		if(not_rain_strike_count < THRESHOLD_NOT_RAIN) not_rain_strike_count++;
      		if(rain_strike_count > 0) rain_strike_count--;
      	}

      	int percent_rain     = (int)(((out_rain + 128) * 100) / 255);
      	int percent_not_rain = (int)(((out_not_rain + 128) * 100) / 255);

      	switch (system_state) {
  			case STATE_WINDOW_OPEN:
  				if (rain_strike_count >= THRESHOLD_RAIN_DETECT) {
  					event_log = "RAIN DETECTED! Starting Motor to Close Window.";
  					motor_start_time = HAL_GetTick();
  					system_state = STATE_WINDOW_CLOSING;
  				}
  				else {
  					event_log = "Weather Clear. Window remains OPEN.";
  				}
  				break;

  			case STATE_WINDOW_CLOSING:
  				if ((HAL_GetTick() - motor_start_time) >= WINDOW_CLOSE_DURATION){
  					event_log = "Window completely CLOSED. Switching to monitoring mode.";

  					not_rain_strike_count = 0;
  					system_state = STATE_WINDOW_CLOSED;
  				}
  				else {
  					event_log = "Window Closing... Motor is active.";
  				}
  				break;

  			case STATE_WINDOW_CLOSED:
  				if (not_rain_strike_count >= THRESHOLD_NOT_RAIN) {
  					event_log = "30 seconds of continuous clear weather. Entering Low Power Mode.";

  					rain_strike_count = 0;
  					system_state = STATE_LOW_POWER_IDLE;
  				}
  				else {
  					event_log = "Raining or waiting stabilization... Keeping window CLOSED.";
  				}
  				break;

  			case STATE_LOW_POWER_IDLE:
  				if (rain_strike_count >= THRESHOLD_RAIN_DETECT) {
  					event_log = "Sudden rain detected during IDLE! Waking up system.";
  					system_state = STATE_WINDOW_CLOSED;
  				}
  				else {
  					event_log = "System is in Low Power Idle Mode (zzz...) Monitoring...";
  				}
  				break;
      	}

      	printf("\033[2J\033[H");
      	printf("==================================================\r\n");
      	printf("           AI Smart Window Dashboard              \r\n");
      	printf("==================================================\r\n");
      	printf("  Uptime        : %lu sec\r\n", total_seconds);

      	const char* state_str = "UNKNOWN";
      	switch(system_state) {
  			case STATE_WINDOW_OPEN:    state_str = "OPEN (Monitoring)"; break;
  			case STATE_WINDOW_CLOSING: state_str = "CLOSING (Motor Running)"; break;
  			case STATE_WINDOW_CLOSED:  state_str = "CLOSED (Rainy State)"; break;
  			case STATE_LOW_POWER_IDLE: state_str = "IDLE (Power Saving Mode)"; break;
  		}
      	printf("  System State  : %s\r\n", state_str);
      	printf("--------------------------------------------------\r\n");
      	printf("         Neural Network Output Data\r\n");
      	printf("  - Raw Outputs : Not Rain [%4d] | Rain [%4d]\r\n", out_not_rain, out_rain);
      	printf("  - Not Rain Probability : %3d %%\r\n", percent_not_rain);
      	printf("  - Rain Probability     : %3d %%\r\n", percent_rain);
      	printf("--------------------------------------------------\r\n");
      	printf("         Confidence Threshold Counters\r\n");
      	printf("  - Rain Strikes  : %2d / %d\r\n", rain_strike_count, THRESHOLD_RAIN_DETECT);
      	printf("  - Clear Strikes : %2d / %d\r\n", not_rain_strike_count, THRESHOLD_NOT_RAIN);
      	printf("==================================================\r\n");
      	printf("  Event Log     : %s\r\n", event_log);
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
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
