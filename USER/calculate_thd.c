#include "calculate_thd.h"

Peak_info peaks[MAX_HARMONIC_PEAKS] = {0};
uint16_t peak_num = 0;
float fundamental_freq; // 基波频率
float thd = 0;
float normalized_ampl[MAX_HARMONIC_PEAKS] = {
    1}; // 归一化幅值数组，将第一个元素置1，剩下元素置0

/**
 * @brief 检测并提取信号频谱中的峰值信息
 *
 * 该函数通过分析FFT输出结果，找到频谱中的局部峰值点，使用插值法对频率和幅值进行校正，
 * 然后根据幅值大小排序并过滤掉幅值较小的峰值，最终保留显著的峰值信息。
 *
 * 函数处理流程：
 * 1. 遍历FFT输出，找到满足阈值条件的局部峰值
 * 2. 使用双峰谱线插值法对频率和幅值进行精确校正
 * 3. 按幅值从大到小对检测到的峰值进行排序
 * 4. 以最大幅值峰值作为基波，过滤掉幅值低于基波5%的峰值
 * 5. 将处理后的峰值信息保存到全局数组peaks中，并更新峰值数量peak_num
 *
 * @param void 无参数
 * @return void 无返回值
 */
static void find_peak_info(void) {
  // 创建临时数组用于分析频谱
  Peak_info temp_peaks[FFT_LENGTH / 2];
  uint16_t temp_peaks_count = 0;

  // 找局部峰值（已经按频率从小到大排好）
  for (uint16_t i = 1; i < FFT_LENGTH / 2; i++) {
    float amplitude_mv = fft_outputbuf[i];
    // 忽略幅值过小点，判断是否为极值
    if (amplitude_mv >= 10.0f && amplitude_mv > fft_outputbuf[i - 1] &&
        amplitude_mv > fft_outputbuf[i + 1]) {
      // 使用双峰谱线插值法进行更精确的频率和幅值校正
      float k = get_pianyik(i); // 获取校正系数k

      // 根据校正系数计算精确频率位置
      float precise_bin = (float)i + k; // k是偏移量

      // 计算校正后的幅值
      float corrected_amplitude =
          (PI * k * amplitude_mv * 2 * (1 - k * k) / arm_sin_f32(PI * k));

      temp_peaks[temp_peaks_count].freq =
          precise_bin * sample_freq / FFT_LENGTH;
      temp_peaks[temp_peaks_count].amplitude_mv = corrected_amplitude;
      temp_peaks_count++;
    }
  }

  // 对检测到的峰值进行排序，按幅值从大到小排列，以便后续处理
  for (int i = 0; i < temp_peaks_count - 1; i++) {
    for (int j = 0; j < temp_peaks_count - 1 - i; j++) {
      if (temp_peaks[j].amplitude_mv < temp_peaks[j + 1].amplitude_mv) {
        Peak_info temp = temp_peaks[j];
        temp_peaks[j] = temp_peaks[j + 1];
        temp_peaks[j + 1] = temp;
      }
    }
  }

  // 输出结果 - 只保留显著的峰值，排除幅值过小的峰值
  // 首先确定基波（幅值最大的峰值）
  if (temp_peaks_count > 0) {
    float fundamental_ampl = temp_peaks[0].amplitude_mv;
    uint16_t valid_peak_count = 0;

    // 过滤掉幅值小于基波一定比例的峰值
    for (uint16_t i = 0;
         i < temp_peaks_count && valid_peak_count < MAX_HARMONIC_PEAKS; i++) {
      // 只保留幅值大于基波幅值5%的峰值（可根据需要调整比例）
      if (temp_peaks[i].amplitude_mv >= fundamental_ampl * 0.05f) {
        peaks[valid_peak_count] = temp_peaks[i];
        valid_peak_count++;
      }
    }
    peak_num = valid_peak_count;
  } else {
    peak_num = 0;
  }

  __BKPT(0);
}

/**
 * @brief 计算总谐波失真(THD)
 *
 * 该函数根据频谱分析得到的峰值信息计算总谐波失真。
 * 以基波为参考，计算2到5次谐波的归一化幅值，并根据这些谐波分量计算THD值。
 * THD是衡量信号失真的重要指标，表示谐波分量相对于基波的幅度。
 *
 * 依赖全局变量：
 * - peaks[]: 存储频谱峰值信息的数组，其中第一个元素为基波
 * - peak_num: 检测到的峰值数量
 * - fundamental_freq: 用于存储基波频率的全局变量
 * - thd: 用于存储计算得到的THD值的全局变量
 * - normalized_ampl[]: 存储各次谐波归一化幅值的数组
 */
void calculate_thd(void) {
  find_peak_info();

  if (peak_num == 0)
    return;

  // peaks数组第一个元素为基波
  float fundamental_ampl = peaks[0].amplitude_mv;
  fundamental_freq = peaks[0].freq;

  // 计算归一化幅值
  float normalized_ampl_sum = 0;
  for (uint16_t i = 1; i < peak_num; i++) {
    float ratio = peaks[i].freq / fundamental_freq;
    // 只考虑2~5次谐波（可调整）
    int harmonic_order = (int)(ratio + 0.5f); // 四舍五入
    if (harmonic_order >= 2 && harmonic_order <= 5) {
      normalized_ampl[harmonic_order - 1] =
          peaks[i].amplitude_mv / fundamental_ampl; // 计算归一化幅值
      normalized_ampl_sum += normalized_ampl[harmonic_order - 1] *
                             normalized_ampl[harmonic_order - 1];
    }
  }

  // 计算thd
  thd = sqrtf(normalized_ampl_sum);
}

/**
 * @brief 计算频谱峰值的偏移校正系数
 *
 * 该函数用于计算频谱分析中峰值位置的偏移校正系数k，
 * 通过比较峰值点与其相邻点的幅值关系，使用不同的公式计算校正系数，
 * 以提高频率和幅值的测量精度。
 *
 * @param time FFT输出数据中的索引位置，表示待校正的峰值位置
 * @return float 返回计算得到的偏移校正系数k，用于后续的频率和幅值校正
 */
static float get_pianyik(uint16_t time) {
  float k;
  if (fft_outputbuf[time + 1] >= fft_outputbuf[time - 1]) {
    k = (fft_outputbuf[time + 1] * 2 - fft_outputbuf[time]) /
        (fft_outputbuf[time + 1] + fft_outputbuf[time]);
  } else {
    k = (fft_outputbuf[time] - fft_outputbuf[time - 1] * 2) /
        (fft_outputbuf[time] + fft_outputbuf[time - 1]);
  }
  return k;
}