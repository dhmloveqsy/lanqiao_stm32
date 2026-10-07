#ifndef BSP_APP_H
#define BSP_APP_H

#include <stdlib.h>
#include <stdint.h>
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include <stdbool.h>

#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "dma.h"
#include "adc.h"
#include "system.h"
#include "schduler.h"
#include "led_app.h"
#include "key_app.h"
#include "lcd.h"
#include "lcd_app.h"
#include "usart_app.h"
#include "adc_app.h"
#include "ringbuffer.h"
#include "filter.h"

extern uint8_t ucLed[8];
extern uint8_t uart_rx_buffer[128];
extern uint32_t uart_rx_ticks;
extern uint8_t uart_rx_index;
extern uint8_t uart_rx_dma_buffer[128];
extern uint8_t uart_rx_dma_index;
extern uint32_t dma_buff[2][30];
extern float adc_value[2];
extern uint8_t lcd_mode_disp;
extern uint16_t PD;
extern uint16_t PH;
extern uint16_t VH;
extern uint16_t VD;
extern uint32_t adc_check;
#endif




