#ifndef ADC_H
#define ADC_H

#include "ti_msp_dl_config.h"

#define ADC_SAMPLE_SIZE (1024)
extern uint16_t gADCSamples[ADC_SAMPLE_SIZE];
extern volatile bool gCheckADC;
extern volatile uint32_t sample_freq;

void adc_init(void);
void set_sample_freq(uint32_t sample_freq, GPTIMER_Regs *gptimer);

#endif // ADC_H