#ifndef GLOBAL_H
#define GLOBAL_H

#include "ti_msp_dl_config.h"
#include "arm_math.h"

// 所有全局变量的extern声明
// 与ADC采样有关
#define ADC_SAMPLE_SIZE 1024
extern uint16_t gADCSamples[ADC_SAMPLE_SIZE];
extern volatile bool gCheckADC;
extern volatile uint32_t sample_freq;
// 有thd计算有关
#define MAX_HARMONIC_PEAKS 5
extern float normalized_ampl[MAX_HARMONIC_PEAKS];
extern float fundamental_freq; // 基波频率
// 与FFT计算有关
#define FFT_LENGTH 1024
extern float32_t fft_outputbuf[FFT_LENGTH];
extern float32_t fft_inputbuf[2 * FFT_LENGTH];

#endif // GLOBAL_H 