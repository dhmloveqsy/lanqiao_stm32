#include "lcd_app.h"

/**
 * @brief  格式化字符串并显示在指定的LCD行上。
 * 
 * 该函数接受一个行号和一个格式化字符串（类似于printf），
 * 格式化字符串后，将其显示在LCD的指定行上。
 *
 * @param  Line    要显示字符串的LCD行号。
 * @param  format  格式化字符串，后跟要格式化的参数。
 *
 * 该函数内部使用 `vsprintf` 来格式化字符串，然后
 * 调用 `LCD_DisplayStringLine` 在LCD上显示格式化后的字符串。
 *
 * 示例用法:
 * @code
 * LcdSprintf(0, "Temperature: %d C", temperature);
 * @endcode
 */
void LcdSprintf(uint8_t Line, char *format,...)
{
    char String[21];  // 缓冲区用于存储格式化后的字符串 每行20个字符加一个结束符
    va_list arg;      // 参数列表用于存储可变参数
    va_start(arg, format);  // 使用格式化字符串初始化参数列表
    vsprintf(String, format, arg);  // 格式化字符串并存储在缓冲区中
    va_end(arg);  // 清理参数列表
    LCD_DisplayStringLine(Line,(char *)String);  // 在LCD的指定行显示格式化后的字符串
}

void lcd_proc(void)
{
	int freq_a1=0;
	int freq_b1=0;
	int freq_a2=0;
	int freq_b2=0;
	int freq_time_b1=0;
	int freq_time_b2=0;
	

	float freq_time_a1=0;
	float freq_time_a2=0;
	
	freq_time_a1 = 1000.0f / tim3_ic_final;
	freq_time_a2 = 1000.0f / tim8_ic_final;
	
	freq_a1=tim3_ic_final/1000;
	freq_b1=tim3_ic_final/10%100;
	freq_a2=tim8_ic_final/1000;
	freq_b2=tim8_ic_final/10%100;
	
	freq_time_b1=1000000/tim3_ic_final;
	freq_time_b2=1000000/tim8_ic_final;
	
	switch(lcd_disp_mode)
	{
		case 0:
		if(tim3_ic_final>=0&&tim8_ic_final>=0)
		{
		if(!freq_mode)
		{
			LcdSprintf(Line1,"        DATA");
			LcdSprintf(Line5,"                     ");
			LcdSprintf(Line6,"                     ");
			if(freq_a2==0)
			{
				LcdSprintf(Line3,"     A=%dHz           ",tim8_ic_final);
			}
			else	
			{	
			if(freq_a2>0&&freq_b2<10)
				LcdSprintf(Line3,"     A=%d.0%dKHz      ",freq_a2,freq_b2);
			else if(freq_a2>0&&freq_b2>=10)
			{
				LcdSprintf(Line3,"     A=%d.%dKHz       ",freq_a2,freq_b2);
			}
			}
			if(freq_a1==0)
			{
				LcdSprintf(Line4,"     B=%dHz           ",tim3_ic_final);
			}
			else	
			{	
			if(freq_a1>0&&freq_b1<10)
				LcdSprintf(Line4,"     B=%d.0%dKHz      ",freq_a1,freq_b1);
			else if(freq_a1>0&&freq_b1>=10)
			{
				LcdSprintf(Line4,"     B=%d.%dKHz       ",freq_a1,freq_b1);
			}
			}
		}
		else
		{
			if(tim8_ic_val<=1000)
			{
				LcdSprintf(Line3,"     A=%.2fmS         ",freq_time_a2);
			}
			else
			{
				LcdSprintf(Line3,"     A=%duS           ",freq_time_b2);
			}
			if(tim3_ic_val<=1000)
			{
				LcdSprintf(Line4,"     B=%.2fmS         ",freq_time_a1);
			}
			else
			{
				LcdSprintf(Line4,"     B=%duS           ",freq_time_b1);
			}
		}
	}
		else
		{
				LcdSprintf(Line1,"        DATA");
				LcdSprintf(Line3,"     A=NULL           ");
				LcdSprintf(Line4,"     B=NULL           ");	
				LcdSprintf(Line5,"                      ");
				LcdSprintf(Line6,"                      ");
		}
		break;
		case 1:
				LcdSprintf(Line1,"        PARA");
				LcdSprintf(Line3,"     PD=%dHz         ",PD);
				LcdSprintf(Line4,"     PH=%dHz         ",PH);
				LcdSprintf(Line5,"     PX=%dHz         ",PX);
				LcdSprintf(Line6,"                       ");
				break;
		case 2:
				LcdSprintf(Line1,"        RECD");
				LcdSprintf(Line2,"                   ");
				LcdSprintf(Line3,"     NDA=%hu        ",NDA);
				LcdSprintf(Line4,"     NDB=%hu        ",NDB);
				LcdSprintf(Line5,"     NHA=%hu        ",NHA);
				LcdSprintf(Line6,"     NHB=%hu        ",NHB);
				break;
	}
}


