#ifndef UART_H
#define UART_H

#include "ti_msp_dl_config.h"
#include "stdio.h"
#include "string.h"

int fputc(int c, FILE *stream);
int fputs(const char *restrict s, FILE *restrict stream);
int puts(const char *_ptr);

#endif // UART_H