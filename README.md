# Signal Distortion Measurement（信号失真测量）

基于 **Texas Instruments MSPM0G3507** 的信号失真度测量项目。项目基于21年电赛A题完成。

> 语言：C
> 平台：TI MSPM0G3507（MSPM0 系列 MCU）  
> 开发方式：TI Code Composer Studio（CCS）

---

## 1. 软件流程说明

主流程位于 `main.c`，总体顺序如下：

1. `SYSCFG_DL_init()`：初始化时钟、GPIO、定时器、UART、ADC、DMA 等。
2. `adc_init()`：初始化ADC。
3. 设置采样频率并启动定时器进行ADC采样：默认 `sample_freq = 102400`，通过 `set_sample_freq()` 配置采样频率。
4. 等待采样完成触发中断。
5. `fft_calculate()`：
	- 去直流分量
	- 加汉宁窗（Hanning Window）
	- 调用 CMSIS-DSP FFT
	- 频谱归一化
6. `calculate_thd()`：提取峰值、计算 THD。

## 2. 关键参数

- 采样点数：`ADC_SAMPLE_SIZE = 1024`（见 `USER/adc.h`）
- 默认采样率：`sample_freq = 102400 Hz`
- CPU 主频：`CPUCLK_FREQ = 80 MHz`
- 峰值检测阈值：幅值 `>= 10 mV`（见 `USER/calculate_thd.c`）
- 有效峰值筛选：保留幅值 ≥ 基波 5% 的峰值，最多 5 个峰（可按需要扩展）
- THD 谐波阶次：仅统计 2~5 次（可按需要扩展）

---

## 3. 项目结构 

```
.
├─ main.c                                # 主流程
├─ signal_distortion_measurement.syscfg  # SysConfig 外设配置
├─ USER/
│  ├─ adc.c / adc.h                      # ADC + DMA 采样、采样频率设置
│  ├─ fft.c / fft.h                      # FFT + 窗函数 + 频谱输出（CMSIS-DSP）
│  ├─ calculate_thd.c / .h               # 峰值检测、谐波提取、THD 计算
│  ├─ uart.c / uart.h                    # printf 串口重定向
│  └─ delay.h                            # 延时函数
└─ Debug/
	├─ ti_msp_dl_config.c/.h             # SysConfig 生成的 DriverLib 初始化代码
	└─ ...                               # 构建产物与链接脚本等
```

---