#include "mfcc_debug.h"

#include <stdio.h>
#include <string.h>

#include "main.h"
#include "arm_math.h"
#include "mfcc_golden_pcm.h"
#include "mfcc_params.h"

extern UART_HandleTypeDef huart2;

static float32_t mfcc_input[MFCC_FFT_LENGTH];
static float32_t mfcc_output[MFCC_NUM_COEFS];
static float32_t mfcc_tmp[MFCC_FFT_LENGTH + 2U];

static void uart_write(const char *text)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)text, strlen(text), HAL_MAX_DELAY);
}

static void uart_printf_float(float value)
{
  char buffer[32];
  const char *sign = "";
  if (value < 0.0f)
  {
    sign = "-";
    value = -value;
  }

  uint32_t whole = (uint32_t)value;
  uint32_t fraction = (uint32_t)((value - (float32_t)whole) * 1000000.0f + 0.5f);
  if (fraction >= 1000000U)
  {
    whole += 1U;
    fraction -= 1000000U;
  }

  snprintf(buffer, sizeof(buffer), "%s%lu.%06lu", sign, (unsigned long)whole, (unsigned long)fraction);
  uart_write(buffer);
}

void MFCC_Debug_Run(void)
{
  arm_mfcc_instance_f32 mfcc;
  arm_status status = arm_mfcc_init_f32(
      &mfcc,
      MFCC_FFT_LENGTH,
      MFCC_NUM_MEL_FILTERS,
      MFCC_NUM_COEFS,
      mfcc_dct_coefs,
      mfcc_filter_pos,
      mfcc_filter_lengths,
      mfcc_filter_coefs,
      mfcc_window_coefs);

  if (status != ARM_MATH_SUCCESS)
  {
    uart_write("MFCC init failed\r\n");
    return;
  }

  uart_write("MFCC_BEGIN\r\n");

  uint32_t frame_count = 0;
  for (uint32_t start = 0;
       start + MFCC_FRAME_LENGTH <= MFCC_GOLDEN_PCM_LENGTH;
       start += MFCC_FRAME_STEP)
  {
    memset(mfcc_input, 0, sizeof(mfcc_input));
    for (uint32_t i = 0; i < MFCC_FRAME_LENGTH; ++i)
    {
      mfcc_input[i] = ((float32_t)mfcc_golden_pcm_i16[start + i]) / 32768.0f;
    }

    arm_mfcc_f32(&mfcc, mfcc_input, mfcc_output, mfcc_tmp);

    for (uint32_t i = 0; i < MFCC_NUM_COEFS; ++i)
    {
      uart_printf_float(mfcc_output[i]);
      uart_write(i + 1U == MFCC_NUM_COEFS ? "\r\n" : ",");
    }
    frame_count++;
  }

  uart_write("MFCC_END ");
  char count_buffer[24];
  snprintf(count_buffer, sizeof(count_buffer), "%lu\r\n", (unsigned long)frame_count);
  uart_write(count_buffer);
}
