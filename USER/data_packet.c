#include "data_packet.h"
#include <string.h>

// 打包 float 类型（4 字节）
int pack_float(float value, uint8_t *packet) {
  if (!packet)
    return 0;

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
  packet[1 + sizeof(float)] = checksum;
  packet[1 + sizeof(float) + 1] = 0x5A;

  return 1 + sizeof(float) + 1 + 1; // head + data + checksum + tail = 7
}

// 打包 short 类型（2 字节）
int pack_short(short value, uint8_t *packet) {
  if (!packet)
    return 0;

  packet[0] = 0xA5;
  memcpy(&packet[1], &value, sizeof(short));

  uint8_t checksum = 0;
  for (int i = 0; i < sizeof(short); i++) {
    checksum += packet[1 + i];
  }

  packet[1 + sizeof(short)] = checksum;
  packet[1 + sizeof(short) + 1] = 0x5A;

  return 1 + sizeof(short) + 1 + 1; // = 5
}
int pack_short_and_float(short short_value, float float_value,
                         uint8_t *packet) {
  if (!packet)
    return 0;

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
  packet[1 + sizeof(short) + sizeof(float)] = checksum; // checksum 位置
  packet[1 + sizeof(short) + sizeof(float) + 1] = 0x5A; // 包尾

  // 返回总长度：1 (包头) + 6 (原数据) + 1 (校验) + 1 (包尾) = 9
  return 1 + sizeof(short) + sizeof(float) + 1 + 1;
}
int pack_short_and_6floats(short short_value, const float *float_values,
                           uint8_t *packet) {
  if (!packet || !float_values)
    return 0;

  // 包头
  packet[0] = 0xA5;

  // 拷贝 short 数据（2 字节）
  memcpy(&packet[1], &short_value, sizeof(short));

  // 拷贝 6 个 float 数据（6 * 4 = 24 字节）
  for (int i = 0; i < 6; i++) {
    memcpy(&packet[1 + sizeof(short) + (i * sizeof(float))], &float_values[i],
           sizeof(float));
  }

  // 计算校验和（原数据部分：2 + 24 = 26 字节）
  uint8_t checksum = 0;
  for (int i = 0; i < (sizeof(short) + 6 * sizeof(float)); i++) {
    checksum += packet[1 + i];
  }

  // 校验和 & 包尾
  packet[1 + sizeof(short) + 6 * sizeof(float)] = checksum; // checksum 位置
  packet[1 + sizeof(short) + 6 * sizeof(float) + 1] = 0x5A; // 包尾

  // 返回总长度：1 (包头) + 26 (原数据) + 1 (校验) + 1 (包尾) = 29
  return 1 + sizeof(short) + 6 * sizeof(float) + 1 + 1;
}
