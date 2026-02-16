# AGENTS.md

本仓库是面向 MSPM0G3507 的 TI Code Composer Studio (CCS) 工程。
供自动化编码代理在此仓库内执行改动时参考。

## 基本信息

- 语言：C（嵌入式）
- MCU：Texas Instruments MSPM0G3507
- 构建系统：CCS 工程文件 + `Debug/` 自动生成 makefile
- DSP：CMSIS-DSP（`arm_math.h`）

## Build / Lint / Test

### 构建（推荐方式）

- 使用 CCS 导入并构建：
  - 打开 CCS，导入项目根目录，然后 Build Project。
  - 这是官方/常见流程（工具链 + SysConfig 集成）。

### 构建（CLI，已安装工具链时）

- `Debug/` 目录包含自动生成的 makefile。
- 仓库根目录下构建示例：

```bash
make -C Debug
```

- 该方式依赖 `Debug/makefile` 中的 TI ARM LLVM 工具链路径有效。

### 清理

```bash
make -C Debug clean
```

### 代码格式化

- 格式由 `.clang-format` 定义（Google 风格基础，4 空格缩进，100 列）。
- 格式化单个文件：

```bash
clang-format -i main.c
```

### 测试

- 当前仓库没有单元/集成测试。
- 目前没有“单测运行”命令。
- 若后续新增测试，请在此补充单测命令。

## 自动生成文件

- `Debug/` 包含构建产物与自动生成文件。
- `Debug/ti_msp_dl_config.c` 与 `Debug/ti_msp_dl_config.h` 由 SysConfig 生成。
- 不要手改自动生成文件；需通过 SysConfig 修改后再构建。

## 代码风格规范

### 格式

- 遵循 `.clang-format`：
  - 缩进宽度：4 空格
  - 行宽限制：100
  - 对齐连续赋值与尾随注释
  - `#include` 排序
  - 函数花括号换行（自定义 brace 风格）

### Includes

- 工程内头文件使用双引号 `"header.h"`。
- `#include` 排序交给 clang-format 处理。
- 本地头文件与系统头文件同时存在时，优先本地头文件。

### 命名

- 函数：`lower_snake_case`（如 `calculate_thd`, `set_sample_freq`）。
- 全局变量：
  - 使用 `g` 前缀（如 `gADCSamples`, `gCheckADC`）。
  - ISR/主循环共享的标志应为 `volatile`。
- 宏/常量：`UPPER_SNAKE_CASE`（如 `ADC_SAMPLE_SIZE`）。
- 类型：`CamelCase`（如 `Peak_info` 结构体）。

### 类型与数据

- MCU 寄存器与计数逻辑使用定宽整数（`uint32_t`, `uint16_t`）。
- DSP 缓冲使用 `float32_t` 以适配 CMSIS-DSP。
- 避免动态分配；数组多为静态/全局固定大小。

### 函数与文件组织

- ISR 保持精简：置标志后尽快退出。
- 多步骤流程拆分为辅助函数（如 `process_once`, `clear_sampling_data`）。
- 相关功能聚合在 `USER/` 模块中（ADC/FFT/THD/UART/clock）。

### 错误处理

- 计算类函数倾向返回显式状态（如失败返回 `-1`）。
- 显式保护无效状态（如 `peak_num == 0`）。
- 实时路径避免在 ISR 内进行重型错误处理。

### 并发与中断

- ISR 与主循环共享数据必须 `volatile`。
- 使用标志位进行同步（`gCheckADC`, `process_flag`）。
- 标志使用后立即清零，避免重复触发。

### 注释与文档

- C 模块函数使用 Doxygen 风格块注释。
- 文件已有中文注释时保持中文。
- 仅在逻辑不直观时添加注释。

### 数学/DSP 约定

- FFT 输入需执行直流分量移除与窗函数（Hanning）。
- FFT 输出保持单边谱归一化（参见 `USER/fft.c`）。
- 调整 THD 逻辑时保持谐波阶数逻辑清晰并注释。

## 仓库结构（概要）

- `main.c`：主循环与流程组织
- `USER/`：ADC/FFT/THD/UART/clock 等模块
- `Debug/`：构建产物与自动生成配置
- `signal_distortion_measurement.syscfg`：SysConfig 源文件

## CCS / SysConfig 注意事项

- SysConfig 修改会重新生成 `Debug/ti_msp_dl_config.*`。
- 外设/时钟调整应在 SysConfig 中完成，并重新构建。
- 避免手改 `Debug/` 输出。

## Cursor / Copilot 规则

- 未发现 `.cursor/rules/` 或 `.cursorrules` 规则。
- 未发现 `.github/copilot-instructions.md` 规则。

## 避免事项

- 不要修改 `Debug/` 内生成文件。
- 不要在 ISR 内加入大量日志。
- 不要改动 DSP 缓冲大小而未同步调整相关常量/逻辑。

## 新增模块约定

- 新模块放在 `USER/`，配对 `.c/.h`。
- 仅在确实需要全局访问时才在 `USER/global.h` 加 extern。
- 命名与格式保持与现有代码一致。
