#include "adc.h"
#include "calculate_thd.h"
#include "data_packet.h"
#include "fft.h"
#include "global.h"
#include "uart_screen_display.h"
#include "clock.h"

void clear_sampling_data(void);
void bluteeth_transmit_data(float thd);
void process_once(void);

volatile bool if_process = false;

int main(void)
{
    SYSCFG_DL_init();
    adc_init();
    uart_init();

    while (1) {
        if (process_flag) {
            process_once();
            process_flag = false;
        }
    }
}

/**
 * @brief 清零用于第二次采样的数组
 *
 * 该函数清零在第二次采样前需要重置的所有数组，包括ADC采样数组、
 * FFT输入输出数组以及与峰值检测相关的全局变量。
 *
 * @param void 无参数
 * @return void 无返回值
 */
void clear_sampling_data(void)
{
    // 清零ADC采样数组
    memset(gADCSamples, 0, sizeof(gADCSamples));

    // 清零FFT相关数组
    memset(fft_inputbuf, 0, sizeof(fft_inputbuf));
    memset(fft_outputbuf, 0, sizeof(fft_outputbuf));

    // 清零峰值信息相关变量
    clear_peaks_data();
}

void bluteeth_transmit_data(float thd)
{
    uint8_t buffer[29]  = {0};
    int16_t short_data  = 0;
    float float_data[6] = {0};
    float_data[0]       = thd;
    for (uint16_t i = 1; i < 6; i++) {
        float_data[i] = normalized_ampl[i - 1];
    }

    for (uint16_t i = 0; i < ADC_SAMPLE_SIZE; i++) {
        short_data = (int16_t)gADCSamples[i];
        pack_short_and_6floats(short_data, float_data, buffer);
        for (uint16_t i = 0; i < 29; i++) {
            DL_UART_Main_transmitDataBlocking(UART_BIUTEETH_INST, buffer[i]);
        }
    }
}

void process_once(void)
{
    uint32_t start_time = my_clock();
    clear_sampling_data();
    // 进行第一次ADC采样，确认基波频率用于调整采样率，默认采样频率为262144（1024 *
    // 256）Hz，确保能正确采集到基波频率（1kHz到100kHz）
    sample_freq = 262144;
    set_sample_freq(sample_freq, TIMER_0_INST);
    DL_Timer_startCounter(TIMER_0_INST);
    while (false == gCheckADC) {
        __WFE();
    }
    gCheckADC = false;

    fft_calculate();
    find_peak_info();

    // 根据第一次采样得到的基波频率调整采样频率
    sample_freq = calculate_optimal_sampling_rate(fundamental_freq);
    set_sample_freq(sample_freq, TIMER_0_INST);
    clear_sampling_data();
    DL_Timer_startCounter(TIMER_0_INST);
    while (false == gCheckADC) {
        __WFE();
    }

    // 计算thd
    fft_calculate();
    find_peak_info();
    float thd = calculate_thd();
    // bluteeth_transmit_data(thd);
    uart_send_display_data(thd,start_time);
}