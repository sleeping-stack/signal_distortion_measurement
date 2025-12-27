#ifndef CALCULATE_THD_H
#define CALCULATE_THD_H

#define MAX_HARMONIC_PEAKS 5

typedef struct{
    float freq;         // 频谱中峰值频率
    float amplitude_mv; // 频谱中峰值幅值
}Peak_info;

extern float normalized_ampl[MAX_HARMONIC_PEAKS];
extern float fundamental_freq; // 基波频率
#include "fft.h"
#include "adc.h"
#include <string.h>

void find_peak_info(void); 
void calculate_thd(void);
static float get_pianyik(uint16_t time);
void clear_peaks_data(void);

#endif // CALCULATE_THD_H