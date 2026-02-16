#ifndef UART_SEND_DISPLAY_DATA_H
#define UART_SEND_DISPLAY_DATA_H

#include <stdio.h>

#include "clock.h"
#include "global.h"

void uart_screen_display_init(void);
int button_cmd(void);
void uart_send_string(char* str);
void uart_send_display_data(float thd, uint32_t start_time);

#endif  // UART_SEND_DISPLAY_DATA_H