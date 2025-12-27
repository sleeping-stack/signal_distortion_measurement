#ifndef DELAY_H
#define DELAY_H

#include "ti_msp_dl_config.h"

/**
 * @brief 延时毫秒函数
 * 
 * 该宏通过调用delay_cycles函数实现毫秒级延时。
 * 根据CPU时钟频率计算出对应的时钟周期数进行精确延时。
 * 
 * @param X 指定要延时的毫秒数
 * 
 * @note 延时时间取决于CPUCLK_FREQ的值，该值在ti_msp_dl_config.h中定义
 */
#define delay_ms(X) delay_cycles((CPUCLK_FREQ / 1000) * (X))

#endif // DELAY_H