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
#include "delay.h"
#include "fft.h"
#include "ti_msp_dl_config.h"
#include "uart.h"
#include "calculate_thd.h"

int main(void) {
  SYSCFG_DL_init();
  adc_init();

  // 进行第一次ADC采样，确认基波频率用于调整采样率，默认采样频率为102400Hz
  sample_freq = 102400;
  set_sample_freq(sample_freq, TIMER_0_INST);
  DL_Timer_startCounter(TIMER_0_INST);
  while (false == gCheckADC) {
    __WFE();
  }
  
  gCheckADC = true;
  for (uint32_t i = 0; i < ADC_SAMPLE_SIZE; i++) {
    printf("Voltage:%d\r\n", gADCSamples[i]);
  }

  __BKPT(0);

  fft_calculate();
  calculate_thd();

  __BKPT(0);

  while (1) {
  }
}