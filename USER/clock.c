#include "clock.h"

volatile uint32_t g_system_ticks = 0;

// 在 SysTick 中断中调用此函数（每 1ms 触发一次）
void SysTick_Handler(void) { g_system_ticks++; }

// 模拟 C 标准库风格的获取时间函数
uint32_t my_clock(void) { return g_system_ticks; }