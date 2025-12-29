#ifndef DATA_PACKET_H
#define DATA_PACKET_H

#include <stdint.h>
#include <string.h>

// 打包 float 类型数据
// value: 要打包的 float 值
// packet: 输出缓冲区，至少需要 7 字节空间
// 返回值: 成功返回 7（数据包总长度），失败返回 0
int pack_float(float value, uint8_t *packet);
// 打包 short 类型数据
// value: 要打包的 short 值
// packet: 输出缓冲区，至少需要 5 字节空间
// 返回值: 成功返回 5（数据包总长度），失败返回 0
int pack_short(short value, uint8_t *packet);
// short_value: 要打包的 short 值
// float_value: 要打包的 float 值
// packet: 输出缓冲区，至少需要 8 字节空间
// 返回值: 成功返回 8（数据包总长度），失败返回 0
int pack_short_and_float(short short_value, float float_value, uint8_t *packet);
int pack_short_and_6floats(short short_value, const float *float_values,
                           uint8_t *packet);

#endif // DATA_PACK_H
