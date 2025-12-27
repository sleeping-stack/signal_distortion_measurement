#include "fft.h"

// 定义全局变量
float fft_outputbuf[FFT_LENGTH] = {0};
float fft_inputbuf[2 * FFT_LENGTH] = {0};
arm_cfft_instance_f32 scfft;

/**
 * @brief 执行FFT计算并输出频谱幅值
 * @note 该函数对ADC采样数据进行FFT变换，计算信号的频谱幅值
 *       1. 提取直流分量并从采样数据中减去
 *       2. 使用Hanning窗减少频谱泄漏
 *       3. 进行FFT变换和幅值计算
 *       4. 对频谱进行归一化处理（单边谱）
 *       5. 输出幅值
 */
void fft_calculate(void) {
  // 提取直流分量
  float32_t dc_component = 0.0f;
  for (uint16_t i = 0; i < FFT_LENGTH; i++) {
    dc_component += gADCSamples[i];
  }
  dc_component /= FFT_LENGTH;

  // 使用Hanning窗提高fft精度
  float32_t hanning_window[FFT_LENGTH];
  Hanningwindow(hanning_window);

  // 生成信号序列
  for (uint32_t i = 0; i < FFT_LENGTH; i++) {
    float32_t temp = (gADCSamples[i] - dc_component) * 3300.0 /
                     4095.0; // 先去除直流分量再转换为电压值
    fft_inputbuf[2 * i] = temp * hanning_window[i]; // 实部，单位为mV
    fft_inputbuf[2 * i + 1] = 0.0f;                 // 虚部
  }

  arm_cfft_init_f32(&scfft, FFT_LENGTH);    // 初始化scfft结构体,设定FFT参数
  arm_cfft_f32(&scfft, fft_inputbuf, 0, 1); // FFT计算
  arm_cmplx_mag_f32(fft_inputbuf, fft_outputbuf,
                    FFT_LENGTH); // 把运算结果复数求模得幅值

  // 对非直流分量进行频谱归一化（单边谱）
  for (uint16_t i = 1; i < FFT_LENGTH / 2; i++) {
    fft_outputbuf[i] = fft_outputbuf[i] / (FFT_LENGTH / 2);
  }
  // 直流分量单独归一化
  fft_outputbuf[0] = fft_outputbuf[0] / FFT_LENGTH;

  // 抹除右半部分
  for (uint16_t i = FFT_LENGTH / 2; i < FFT_LENGTH; i++) {
    fft_outputbuf[i] = 0.0f;
  }
}

/**
 * @brief 应用汉宁窗函数到输入数组
 *
 * 该函数计算并应用汉宁窗，用于减少FFT分析中的频谱泄漏
 * 汉宁窗公式为: w(n) = 0.5 * (1 - cos(2πn/N-1)), 其中n是样本索引，N是FFT长度
 *
 * @param hanning_window
 * 指向存储汉宁窗系数的浮点数组的指针，数组长度为FFT_LENGTH
 * @return 无返回值
 */
static void Hanningwindow(float32_t hanning_window[]) // 汉宁窗
{
  for (uint16_t i = 0; i < FFT_LENGTH; i++) {
    hanning_window[i] =
        0.5f * (1.0f - arm_cos_f32(2.0f * PI * i / (FFT_LENGTH - 1)));
  }
}