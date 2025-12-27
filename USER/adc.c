#include "adc.h"

uint16_t gADCSamples[ADC_SAMPLE_SIZE];
volatile bool gCheckADC = false;
volatile uint32_t sample_freq;

void adc_init(void) {
  NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);
  DL_DMA_setSrcAddr(
      DMA, DMA_CH0_CHAN_ID,
      (uint32_t)DL_ADC12_getMemResultAddress(ADC12_0_INST, ADC12_0_ADCMEM_0));
  DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)&gADCSamples[0]);
  DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
}

/**
 * @brief ADC12_0_INST中断处理函数
 *
 * 该函数处理ADC12_0_INST的中断事件，主要处理DMA传输完成中断，
 * 设置全局标志并禁用ADC转换
 *
 * @param void 无参数
 * @return void 无返回值
 */
void ADC12_0_INST_IRQHandler(void) {
  switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
  case DL_ADC12_IIDX_DMA_DONE:
    gCheckADC = true;
    DL_Timer_stopCounter(TIMER_0_INST);
    break;
  default:
    break;
  }
}

/**
 * @brief 设置采样频率
 *
 * 该函数根据指定的采样频率和ADC采样大小计算定时器的周期计数值，
 * 并将其设置到GPTIMER寄存器中，以控制ADC采样频率。
 *
 * @param sample_freq 期望设置的采样频率（单位：Hz）
 * @param gptimer 指向GPTIMER寄存器的指针，用于配置定时器
 *
 * @return 无返回值
 */
void set_sample_freq(uint32_t sample_freq, GPTIMER_Regs *gptimer) {
  DL_Timer_setLoadValue(gptimer, CPUCLK_FREQ / sample_freq);
}

/**
 * 计算最优采样率
 * 
 * 根据基波频率计算满足奈奎斯特采样定理的最优采样率，并考虑谐波分量
 * 通过调整采样率为ADC_SAMPLE_SIZE的倍数，以满足系统要求
 * 
 * @param fundamental_freq 基波频率（Hz）
 * @return 返回计算得出的最优采样率（Hz）
 */
uint32_t calculate_optimal_sampling_rate(float fundamental_freq) {
  uint32_t optimal_rate;
  uint32_t highest_freq_to_process; // 需要考虑的最高频率（5次谐波）
  uint32_t nyquist_rate;            // 奈奎斯特采样率（最高频率的2倍）
  uint32_t multiplier;              // 采样率倍数

  // 计算需要处理的最高频率（5次谐波）
  highest_freq_to_process = (uint32_t)(fundamental_freq * 5);
  // 根据奈奎斯特采样定理，采样率至少是最高频率的2倍
  nyquist_rate = highest_freq_to_process * 2;

  // 为了获得更好的频率分辨率和精度，使用高于奈奎斯特率的采样率
  // 使用2-4倍的奈奎斯特率，具体倍数根据基波频率调整
  if (fundamental_freq <= 10000) {
    // 对于较低频率，使用较高倍数以获得更好的频率分辨率
    multiplier = 4; // 4倍奈奎斯特率
  } else if (fundamental_freq <= 50000) {
    // 中等频率，使用适中倍数
    multiplier = 3; // 3倍奈奎斯特率
  } else {
    // 较高频率，使用稍低倍数以避免采样率过高
    multiplier = 2; // 2倍奈奎斯特率（最低要求）
  }
  // 计算初步的采样率
  optimal_rate = nyquist_rate * multiplier;

  // 调整采样率为ADC_SAMPLE_SIZE的倍数
  // ADC_SAMPLE_SIZE = 1024，使采样率尽可能接近1024的倍数
  uint32_t adjusted_rate =
      ((optimal_rate + ADC_SAMPLE_SIZE / 2) / ADC_SAMPLE_SIZE) *
      ADC_SAMPLE_SIZE;
  // 确保调整后的采样率仍满足最小要求
  if (adjusted_rate < nyquist_rate * multiplier) {
    adjusted_rate += ADC_SAMPLE_SIZE;
  }
  optimal_rate = adjusted_rate;

  return optimal_rate;
}