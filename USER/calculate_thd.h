#ifndef CALCULATE_THD_H
#define CALCULATE_THD_H

typedef struct {
  float freq;         // 频谱中峰值频率
  float amplitude_mv; // 频谱中峰值幅值
} Peak_info;

#include "global.h"
#include <string.h>

void find_peak_info(void);
float calculate_thd(void);
static float get_peak_offset_correction_factor(uint16_t time);
void clear_peaks_data(void);

#endif // CALCULATE_THD_H