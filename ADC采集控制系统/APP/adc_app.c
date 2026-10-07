#include "adc_app.h"

uint32_t dma_buff[2][30]; // DMA接收缓存
float adc_value[2]; // ADC采样值数组

uint16_t VH;
uint16_t VD;
bool adc_vh_flag;
bool adc_vd_flag;

/*ringbuffer的写法*/
#define BUFFER_SIZE 30
#define BUFFER_TIMEOUT 3000
typedef struct{
	uint32_t time_stamp;
	uint32_t adc;
}adc_data_t;

adc_data_t adc[30];
int buffer_start=0;
int buffer_end=0;

void add_adc_data(uint32_t adc,uint32_t current_time,adc_data_t *buffer)
{
		buffer[buffer_end].time_stamp=current_time;
		buffer[buffer_end].adc=adc;
		buffer_end=(buffer_end+1)%BUFFER_SIZE;
		if(buffer_end==buffer_start)
		{
			buffer_start=(buffer_start+1)%BUFFER_SIZE;
		}
		while(current_time-buffer[buffer_start].time_stamp>=BUFFER_TIMEOUT)
		{
			buffer_start=(buffer_start+1)%BUFFER_SIZE;
		}
}
uint32_t adc_check;
void check_adc_sudden_change(uint16_t *sudden_change_count,adc_data_t* buffer)
{
	uint32_t adc_max=buffer[buffer_start].adc;
	uint32_t adc_min=buffer[buffer_start].adc;
	int adc_index=buffer_start;
	
	while(adc_index!=buffer_end)
	{
		if(buffer[adc_index].adc>adc_max)
		{
			adc_max=buffer[adc_index].adc;
		}
		else if(buffer[adc_index].adc<adc_min)
		{
			adc_min=buffer[adc_index].adc;
		}
		adc_index=(adc_index+1)%BUFFER_SIZE;
	}
	
	 adc_check=adc_max-adc_min;
	
	if(adc_check<PD)
	{
		adc_vd_flag=1;
	}
	else if(adc_vd_flag)
	{
		adc_vd_flag=0;
		(*sudden_change_count)++;
	}
}

void adc_proc()
{
		uint32_t adc_time=HAL_GetTick();
		adc_value[0]=0;
		adc_value[1]=0;
		for(uint8_t i=0;i<30;i++)
		{
			adc_value[0] += (float)dma_buff[0][i];
			adc_value[1] += (float)dma_buff[1][i];
		}
			adc_value[0] = adc_value[0] / 30;
			adc_value[1] = adc_value[1] / 30 ;
		
		add_adc_data(adc_value[0],adc_time,adc);
		check_adc_sudden_change(&VD,adc);
		
		if((int)adc_value[1]>PH&&adc_vh_flag)
		{
				VH+=1;
				adc_vh_flag=0;
		}
		else if((int)adc_value[1]<PH)
		{
				adc_vh_flag=1;
		}
}

