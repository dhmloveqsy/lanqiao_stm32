#ifndef BSP_APP_H
#define BSP_APP_H
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "dma.h"
#include "adc.h"
#include "rtc.h"
#include "tim.h"
#include "system.h"
#include "schduler.h"
#include "led_app.h"
#include "key_app.h"
#include "lcd.h"
#include "lcd_app.h"
#include "usart_app.h"
#include "adc_app.h"
#include "myi2c_app.h"
#include "rtc_app.h"
#include "tim_app.h"
#include "ringbuffer.h"
#include "i2c_hal.h"
#include "filter.h"
#include <stdlib.h>
#include <stdint.h>
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "stdbool.h"
extern uint8_t ucLed[8];
extern uint8_t uart_rx_buffer[128];
extern uint32_t uart_rx_ticks;
extern uint8_t uart_rx_index;
extern uint8_t uart_rx_dma_buffer[128];
extern uint8_t uart_rx_dma_index;
extern uint32_t dma_buff[2][30];
extern float adc_value[2];
extern uint32_t tim3_ic_buffer[64];
extern uint32_t tim8_ic_buffer[64];
extern uint8_t lcd_disp_mode;
extern bool freq_mode;
extern int PD;
extern int PH;
extern int PX;
extern uint16_t NDA;
extern uint16_t NDB;
extern uint16_t NHA;
extern uint16_t NHB;
extern uint32_t tim3_ic_val;
extern uint32_t tim8_ic_val;
extern int tim3_ic_final;
extern int tim8_ic_final;
#endif




