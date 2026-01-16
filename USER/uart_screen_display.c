#include "uart_screen_display.h"

uint8_t rx_buffer[7];
uint16_t rx_data_cnt = 0;
bool process_flag    = false;

void uart_init(void)
{
    NVIC_ClearPendingIRQ(UART_SCREEN_DISPLAY_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_SCREEN_DISPLAY_INST_INT_IRQN);
}

int button_cmd(void)
{
    if (rx_buffer[4] == 0xFF && rx_buffer[5] == 0xFF && rx_buffer[6] == 0xFF) {
        switch (rx_buffer[0]) {
            case 0x65:
                if (rx_buffer[2] == 0x03) {
                    return 1;
                }
                break;
            default:
                break;
        }
    }

    return 0;
}

// 串口发送字符串
void uart_send_string(char* str)
{
    // 当前字符串地址不在结尾并且字符串首地址不为空
    while (*str != 0 && str != 0) {
        // 发送字符串首地址中的字符，并且在发送完成之后首地址自增
        DL_UART_Main_transmitDataBlocking(UART_SCREEN_DISPLAY_INST, *str++);
    }
}

// 串口的中断服务函数
void UART_SCREEN_DISPLAY_INST_IRQHandler(void)
{
    // 如果产生了串口中断
    switch (DL_UART_getPendingInterrupt(UART_SCREEN_DISPLAY_INST)) {
        case DL_UART_IIDX_RX:  // 如果是接收中断
            // 接发送过来的数据保存在变量中
            rx_buffer[rx_data_cnt] = DL_UART_Main_receiveData(UART_SCREEN_DISPLAY_INST);
            rx_data_cnt++;
            if (rx_data_cnt == 7) {
                if (button_cmd() == 1) {
                    process_flag = true;
                }
                rx_data_cnt = 0;
                memset(rx_buffer, 0, sizeof(rx_buffer));
            }
            break;
        default:  // 其他的串口中断
            break;
    }
}

void uart_send_display_data(float thd, uint32_t start_time)
{
    char tx_buffer[20] = {0};

    for (uint16_t i = 0; i <= 4; i++) {
        snprintf(tx_buffer, sizeof(tx_buffer), "t%d.txt=\"%.3f\"\xff\xff\xff", i,
                 normalized_ampl[i]);
        uart_send_string(tx_buffer);
    }

    snprintf(tx_buffer, sizeof(tx_buffer), "t6.txt=\"%.3f\"\xff\xff\xff", thd);
    uart_send_string(tx_buffer);

    memset(tx_buffer, 0, sizeof(tx_buffer));
    for (int i = 0; i < sample_freq / fundamental_freq * 10 && i < ADC_SAMPLE_SIZE ; i++) {
        // 向曲线s0的通道0传输1个数据,add指令不支持跨页面
        snprintf(tx_buffer, sizeof(tx_buffer), "add s0.id,0,%d\xff\xff\xff",
                 (int)(gADCSamples[i] / 4095.0 * 255.0));
        uart_send_string(tx_buffer);
    }

    uint32_t end_time  = my_clock();
    float duration_sec = (float)(end_time - start_time) / 1000.0f;
    snprintf(tx_buffer, sizeof(tx_buffer), "t14.txt=\"%.1fs\"\xff\xff\xff", duration_sec);
    uart_send_string(tx_buffer);
}