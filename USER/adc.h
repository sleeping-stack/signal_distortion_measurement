#ifndef ADC_H
#define ADC_H

#include "global.h"

// 定义基波频率检测范围，用于确定合适的采样率
#define FUNDAMENTAL_FREQ_MIN 1000   // 基波最小频率 (Hz)
#define FUNDAMENTAL_FREQ_MAX 100000 // 基波最大频率 (Hz)

void adc_init(void);
void set_sample_freq(uint32_t sample_freq, GPTIMER_Regs *gptimer);
uint32_t calculate_optimal_sampling_rate(float fundamental_freq);

#endif // ADC_H