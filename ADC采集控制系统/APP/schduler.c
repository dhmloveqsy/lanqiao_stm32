#include "schduler.h"
//获取任务数量
uint8_t task_num=0;

//任务结构体
typedef struct{
	void (*task_func)(void);
	uint32_t rate_ms;
	uint32_t last_run;
}schduler_t;

//任务数组 初始化任务的轮询周期和上一次运行的时间
static schduler_t task_t[]={
	{led_proc,10,0},
	{key_proc,10,0},
	{lcd_proc,100,0},
	{usart_task,10,0},
	{adc_proc,100,0}
};

void schduler_init()
{
	task_num=sizeof(task_t)/sizeof(schduler_t);
}

//任务调度
void schduler_run()
{
	for(uint8_t i=0;i<task_num;i++)//遍历任务列表
	{
	uint32_t  now_time=HAL_GetTick ();//获取当前时间
	if(now_time>=task_t[i].rate_ms+task_t[i].last_run)//判断是否到达任务执行时间
	{
		task_t[i].last_run=now_time;//更新任务上次执行时间
		task_t[i].task_func();//执行任务
	}
	}
}



