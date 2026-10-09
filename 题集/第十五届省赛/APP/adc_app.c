#include "adc_app.h"

uint32_t dma_buff[2][30]; // DMA接收缓存
float adc_value[2]; // ADC采样值数组

void adc_proc()
{
    uint32_t sum0 = 0, sum1 = 0;
    for(uint8_t i=0;i<30;i++)
    {
        sum0 += dma_buff[0][i];
        sum1 += dma_buff[1][i];
    }
    adc_value[0] = sum0 / 30.0f * 3.3f / 4095;
    adc_value[1] = sum1 / 30.0f * 3.3f / 4095;
}

