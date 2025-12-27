#ifndef FFT_H
#define FFT_H

#include "adc.h"
#include "arm_math.h"
#include "uart.h"

#define FFT_LENGTH ADC_SAMPLE_SIZE
extern float32_t fft_outputbuf[FFT_LENGTH];
extern float32_t fft_inputbuf[2 * FFT_LENGTH];

void fft_calculate(void);
static void Hanningwindow(float32_t hanning_window[]);

#endif // FFT_H