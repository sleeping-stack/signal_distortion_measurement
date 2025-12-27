#include "uart.h"

// 串口重定向
int fputc(int c, FILE *stream) {
  DL_UART_Main_transmitDataBlocking(UART_0_INST, c);
  return c;
}

int fputs(const char *restrict s, FILE *restrict stream) {
  uint16_t i, len;
  len = strlen(s);
  for (i = 0; i < len; i++) {
    DL_UART_Main_transmitDataBlocking(UART_0_INST, s[i]);
  }
  return len;
}

int puts(const char *_ptr) {
  int count = fputs(_ptr, stdout);
  count += fputs("\n", stdout);
  return count;
}