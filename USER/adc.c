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