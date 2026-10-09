#include "tim_app.h"

/**
 * @brief 设置PWM占空比
 * 
 * 此函数用于设置TIM2的CH1通道的PWM占空比。占空比的范围为0%到100%。
 * 
 * @param Duty 占空比，范围为0.0到100.0。
 */
typedef struct{
	 uint32_t timestamp;
	 int freq;
 }ringbuffer_freq;

#define BUFFER_SIZE 30
#define BUFFER_TIMEOUT 3000
ringbuffer_freq freqa[BUFFER_SIZE];
ringbuffer_freq freqb[BUFFER_SIZE];
uint8_t read_afreq;
uint8_t write_afreq; 
uint8_t read_bfreq;
uint8_t write_bfreq;
bool freq_aflag;
bool freq_bflag;
 
void freq_data(int freq,uint32_t current_time,ringbuffer_freq *buffer,uint8_t* read_freq,uint8_t* write_freq)
{
		buffer[*write_freq].timestamp=current_time;
		buffer[*write_freq].freq=freq;
		*write_freq=(*write_freq+1)%BUFFER_SIZE;
		if(*write_freq==*read_freq)
		{
			*read_freq=(*read_freq+1)%BUFFER_SIZE;
		}
		while(current_time-buffer[*read_freq].timestamp>=BUFFER_TIMEOUT)
		{
			*read_freq=(*read_freq+1)%BUFFER_SIZE;
		}
}

uint32_t freq_check;

void check_freq_sudden_change(uint16_t *sudden_change_count,ringbuffer_freq* buffer,uint8_t* read_freq,uint8_t* write_freq,bool* freq_flag)
{
	uint32_t freq_max=buffer[*read_freq].freq;
	uint32_t freq_min=buffer[*read_freq].freq;
	int freq_index=*read_freq;
	
	while(freq_index!=*write_freq)
	{
		if(buffer[freq_index].freq>freq_max)
		{
			freq_max=buffer[freq_index].freq;
		}
		else if(buffer[freq_index].freq<freq_min)
		{
			freq_min=buffer[freq_index].freq;
		}
		freq_index=(freq_index+1)%BUFFER_SIZE;
	}
	
	 freq_check=freq_max-freq_min;
	
	if(freq_check<PD)
	{
		*freq_flag=1;
	}
	else if(*freq_flag)
	{
		*freq_flag=0;
		(*sudden_change_count)++;
	}
}
 
void pwm_set_duty(float Duty)
{
    // 根据占空比计算捕获/比较寄存器的值
    TIM2->CCR2 = (TIM2->ARR + 1) * (Duty / 100.0f);
}

/**
 * @brief 设置PWM频率
 * 
 * 此函数用于设置TIM2的PWM频率。频率的单位为Hz。
 * 
 * @param Frequency 频率，单位为Hz。
 */
void pwm_set_frequency(int Frequency)
{
    // 获取定时器的时钟频率（假设TIM2使用的时钟频率为TIM2_CLK）
    uint32_t TIM2_CLK = 8000000;  // 例如72MHz, 需要根据实际情况调整

    // 根据输入的频率计算自动重装载寄存器的值
    uint32_t ARR_Value = (TIM2_CLK / Frequency) - 1;

    // 设置自动重装载寄存器
    TIM2->ARR = ARR_Value;

    // 更新捕获/比较寄存器CCR2，保持当前占空比不变
    TIM2->CCR2 = (ARR_Value + 1) * (TIM2->CCR2 / (float)(TIM2->ARR + 1));
    
    // 触发更新事件，刷新寄存器
    TIM2->EGR = TIM_EGR_UG;
}



uint32_t tim3_ic_buffer[64];// 定义存储输入捕获值的缓冲区
uint32_t tim3_ic_val = 0;// 最终计算得到的输入捕获值
uint32_t tim3_ic_temp = 0;// 临时存储输入捕获计算的中间值

uint32_t tim8_ic_buffer[64];// 定义存储输入捕获值的缓冲区
uint32_t tim8_ic_val = 0;// 最终计算得到的输入捕获值
uint32_t tim8_ic_temp = 0;// 临时存储输入捕获计算的中间值

int tim3_ic_final;
int tim8_ic_final;

bool freq_flag_a=0;
bool freq_flag_b=0;
/**
 * @brief 处理输入捕获数据
 * 
 * 此函数用于处理输入捕获数据。它计算tim_ic_buffer中所有捕获值的平均值，
 * 然后将其转换为对应的频率值（单位为Hz），并存储在tim_ic_val中。
 * 最后，调用limit_value函数对tim_ic_val进行限制，确保其在1到20000之间。
 */
void ic_proc(void)
{
		uint32_t freq_time=HAL_GetTick();
    tim3_ic_temp = 0;
		tim8_ic_temp = 0;
    
    // 对缓冲区中的捕获值求和
    for (int i = 0; i < 64; i++)
    {
        tim3_ic_temp += tim3_ic_buffer[i];
				tim8_ic_temp += tim8_ic_buffer[i];
    }

    // 计算平均值
    tim3_ic_temp /= 64;
		tim8_ic_temp /= 64;

    // 计算频率值，单位为Hz
    tim3_ic_val = (int)((float)(1000.0f * 1000.0f) / (float)tim3_ic_temp);
		tim8_ic_val = (int)((float)(1000.0f * 1000.0f) / (float)tim8_ic_temp);

    // 将计算得到的频率值限制在1到20000之间
    limit_value(&tim3_ic_val, 1, 400, 20000);
		limit_value(&tim8_ic_val, 1, 400, 20000);
		
		tim3_ic_final=(int)tim3_ic_val+PX;
		tim8_ic_final=(int)tim8_ic_val+PX;
		
		if(tim3_ic_final>PH&&freq_flag_b)
		{
			freq_flag_b=0;
			NHB+=1;
		}
		else if(tim3_ic_final<PH)
		{
			freq_flag_b=1;
		}
		if(tim8_ic_final>PH&&freq_flag_a)
		{
			freq_flag_a=0;
			NHA+=1;
		}
		else if(tim8_ic_final<PH)
		{
			freq_flag_a=1;
		}
		
		freq_data(tim3_ic_final,freq_time,freqb,&read_bfreq,&write_bfreq);
		freq_data(tim8_ic_final,freq_time,freqa,&read_afreq,&write_afreq);
		check_freq_sudden_change(&NDB,freqb,&read_bfreq,&write_bfreq,&freq_bflag);
		check_freq_sudden_change(&NDA,freqa,&read_afreq,&write_afreq,&freq_aflag);
}




