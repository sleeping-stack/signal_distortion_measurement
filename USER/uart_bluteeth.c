#include "uart_bluteeth.h"

volatile uint8_t uart_tx_complete_flag = 1;  // 初始标志为已完成（允许新任务）

// 打包 float 类型（4 字节）
int pack_float(float value, uint8_t *packet)
{
    if (!packet) return 0;

    // 包头
    packet[0] = 0xA5;

    // 原数据（按内存布局直接拷贝，小端）
    memcpy(&packet[1], &value, sizeof(float));

    // 计算校验和（仅原数据部分）
    uint8_t checksum = 0;
    for (int i = 0; i < sizeof(float); i++) {
        checksum += packet[1 + i];
    }

    // 校验和 & 包尾
    packet[1 + sizeof(float)]     = checksum;
    packet[1 + sizeof(float) + 1] = 0x5A;

    return 1 + sizeof(float) + 1 + 1;  // head + data + checksum + tail = 7
}

// 打包 short 类型（2 字节）
int pack_short(short value, uint8_t *packet)
{
    if (!packet) return 0;

    packet[0] = 0xA5;
    memcpy(&packet[1], &value, sizeof(short));

    uint8_t checksum = 0;
    for (int i = 0; i < sizeof(short); i++) {
        checksum += packet[1 + i];
    }

    packet[1 + sizeof(short)]     = checksum;
    packet[1 + sizeof(short) + 1] = 0x5A;

    return 1 + sizeof(short) + 1 + 1;  // = 5
}
int pack_short_and_float(short short_value, float float_value, uint8_t *packet)
{
    if (!packet) return 0;

    // 包头
    packet[0] = 0xA5;

    // 拷贝 short 数据（2 字节）
    memcpy(&packet[1], &short_value, sizeof(short));

    // 拷贝 float 数据（4 字节）
    memcpy(&packet[1 + sizeof(short)], &float_value, sizeof(float));

    // 计算校验和（原数据部分：2 + 4 = 6 字节）
    uint8_t checksum = 0;
    for (int i = 0; i < (sizeof(short) + sizeof(float)); i++) {
        checksum += packet[1 + i];
    }

    // 校验和 & 包尾
    packet[1 + sizeof(short) + sizeof(float)]     = checksum;  // checksum 位置
    packet[1 + sizeof(short) + sizeof(float) + 1] = 0x5A;      // 包尾

    // 返回总长度：1 (包头) + 6 (原数据) + 1 (校验) + 1 (包尾) = 9
    return 1 + sizeof(short) + sizeof(float) + 1 + 1;
}
int pack_short_and_6floats(short short_value, const float *float_values, uint8_t *packet)
{
    if (!packet || !float_values) return 0;

    // 包头
    packet[0] = 0xA5;

    // 拷贝 short 数据（2 字节）
    memcpy(&packet[1], &short_value, sizeof(short));

    // 拷贝 6 个 float 数据（6 * 4 = 24 字节）
    for (int i = 0; i < 6; i++) {
        memcpy(&packet[1 + sizeof(short) + (i * sizeof(float))], &float_values[i], sizeof(float));
    }

    // 计算校验和（原数据部分：2 + 24 = 26 字节）
    uint8_t checksum = 0;
    for (int i = 0; i < (sizeof(short) + 6 * sizeof(float)); i++) {
        checksum += packet[1 + i];
    }

    // 校验和 & 包尾
    packet[1 + sizeof(short) + 6 * sizeof(float)]     = checksum;  // checksum 位置
    packet[1 + sizeof(short) + 6 * sizeof(float) + 1] = 0x5A;      // 包尾

    // 返回总长度：1 (包头) + 26 (原数据) + 1 (校验) + 1 (包尾) = 29
    return 1 + sizeof(short) + 6 * sizeof(float) + 1 + 1;
}

void uart_bluteeth_init(void)
{
    NVIC_ClearPendingIRQ(UART_BLUTEETH_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_BLUTEETH_INST_INT_IRQN);
}

void uart_bluteeth_send(uint8_t tx_buff[], uint16_t tx_size)
{
    // 设置源地址
    DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)(tx_buff));
    // 设置目标地址
    DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t)(&UART_BLUTEETH_INST->TXDATA));
    // 设置要搬运的字节数
    DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, tx_size);
    // 使能DMA通道
    DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);

    uart_tx_complete_flag = 0;
}

void bluteeth_transmit_data(float thd)
{
    uint8_t buffer[30]  = {0};
    int16_t short_data  = 0;
    float float_data[6] = {0};
    float_data[0]       = thd;
    for (uint16_t i = 1; i < 6; i++) {
        float_data[i] = normalized_ampl[i - 1];
    }

    for (uint16_t i = 0; i < ADC_SAMPLE_SIZE; i++) {
        short_data = (int16_t)gADCSamples[i];
        pack_short_and_6floats(short_data, float_data, buffer);
        uint16_t tx_size = sizeof(buffer);
        uart_bluteeth_send(buffer, tx_size);
    }
}

void UART_BLUTEETH_INST_IRQHandler(void)
{
    volatile uint32_t flag = DL_UART_Main_getPendingInterrupt(UART_BLUTEETH_INST);
    switch (flag) {
        case DL_UART_IIDX_EOT_DONE: {
            uart_tx_complete_flag = 1;
        } break;
    }
}