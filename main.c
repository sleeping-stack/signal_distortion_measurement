/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "adc.h"
#include "calculate_thd.h"
#include "delay.h"
#include "fft.h"
#include "ti_msp_dl_config.h"
#include "uart.h"

void clear_sampling_data(void);
int main(void) {
  SYSCFG_DL_init();
  adc_init();

  // 进行第一次ADC采样，确认基波频率用于调整采样率，默认采样频率为262144（1024 *
  // 256）Hz，确保能正确采集到基波频率（1kHz到100kHz）
  sample_freq = 262144;
  set_sample_freq(sample_freq, TIMER_0_INST);
  DL_Timer_startCounter(TIMER_0_INST);
  while (false == gCheckADC) {
    __WFE();
  }
  for (uint16_t i = 0; i < ADC_SAMPLE_SIZE; i++) {
    printf("V:%d\r\n", gADCSamples[i]);
  }
  gCheckADC = false;

  fft_calculate();
  find_peak_info();

  __BKPT(0);

  // 根据第一次采样得到的基波频率调整采样频率
  sample_freq = calculate_optimal_sampling_rate(fundamental_freq);
  set_sample_freq(sample_freq, TIMER_0_INST);
  clear_sampling_data();
  DL_Timer_startCounter(TIMER_0_INST);
  while (false == gCheckADC) {
    __WFE();
  }
  for (uint16_t i = 0; i < ADC_SAMPLE_SIZE; i++) {
    printf("V:%d\r\n", gADCSamples[i]);
  }
  // 计算thd
  fft_calculate();
  find_peak_info();
  calculate_thd();

  __BKPT(0);

  while (1) {
  }
}

/**
 * @brief 清零用于第二次采样的数组
 *
 * 该函数清零在第二次采样前需要重置的所有数组，包括ADC采样数组、
 * FFT输入输出数组以及与峰值检测相关的全局变量。
 *
 * @param void 无参数
 * @return void 无返回值
 */
void clear_sampling_data(void) {
  // 清零ADC采样数组
  memset(gADCSamples, 0, sizeof(gADCSamples));

  // 清零FFT相关数组
  memset(fft_inputbuf, 0, sizeof(fft_inputbuf));
  memset(fft_outputbuf, 0, sizeof(fft_outputbuf));

  // 清零峰值信息相关变量
  clear_peaks_data();
}