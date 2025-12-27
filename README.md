# Signal Distortion Measurement（信号失真测量）

基于 **Texas Instruments MSPM0G3507** 的信号失真度测量项目。项目基于21年电赛A题完成。

> 语言：C
> 平台：TI MSPM0G3507（MSPM0 系列 MCU）  
> 开发方式：TI Code Composer Studio（CCS）

---

## 1. 项目结构 

```
.
├─ main.c                                # 主流程
├─ signal_distortion_measurement.syscfg  # SysConfig 外设配置
├─ USER/
│  ├─ adc.c / adc.h                      # ADC + DMA 采样、采样频率设置
│  ├─ fft.c / fft.h                      # FFT + 窗函数 + 频谱输出（CMSIS-DSP）
│  ├─ calculate_thd.c / calculate_thd.h  # 峰值检测、谐波提取、THD 计算
│  ├─ data_packet.c / data_packet.h      # 数据打包（short/float）用于蓝牙模块发送数据
│  ├─ uart.c / uart.h                    # printf 串口重定向
│  └─ delay.h                            # 延时函数
├─ targetConfigs/                        
├─ Debug/                                # 构建产物（由 CCS 自动生成）
│  ├─ ti_msp_dl_config.c/.h              # SysConfig 生成的 DriverLib 初始化代码
│  └─ ...                                # 目标文件、链接脚本、makefile 等
└─ .project/.cproject/.ccsproject        
```

---

## 2. 如何构建该项目

-   确保你使用的是 **Texas Instruments MSPM0G3507** 芯片
-   下载整个文件夹，使用CCS的导入功能导入该项目（使用Keil MDK请自行移植）