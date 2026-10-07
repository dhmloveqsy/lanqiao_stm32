#include "usart_app.h"

/*超时解析的方案*/

uint8_t uart_rx_buffer[128];
uint32_t uart_rx_ticks;
uint8_t uart_rx_index;

////串口中断函数
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance==USART1)
//	{
//			uart_rx_ticks=HAL_GetTick();
//			uart_rx_index++;
//		  HAL_UART_Receive_IT(&huart1,&uart_rx_buffer[uart_rx_index],1);
////			printf("uart_rx is doing\r\n");
//	}
//}

/**
 * @brief  处理UART接收缓冲区中的数据。
 *         如果在100ms内没有接收到新的数据，将清空缓冲区。
 * @param  None
 * @retval None
 */
// 处理UART接收缓冲区中的数据
void uart_proc(void)
{
    // 如果接收索引为0，说明没有数据需要处理，直接返回
     if(uart_rx_index == 0) return;

    // 如果从最后一次接收到数据到现在已经超过100ms
     if(uwTick - uart_rx_ticks > 100) // 100ms内没有收到新的数据
     {
       printf("uart data: %s\n", uart_rx_buffer);

         // 清空接收缓冲区，将接收索引置零
       memset(uart_rx_buffer, 0, uart_rx_index);
       uart_rx_index = 0;
        
        // 将UART接收缓冲区指针重置为接收缓冲区的起始位置
       huart1.pRxBuffPtr = uart_rx_buffer;
     }
}

/*DMA+空闲中断+ringbuffer的方案*/
uint8_t uart_rx_dma_buffer[128];
uint8_t uart_rx_dma_index;
ringbuffer_t usart_ring;
uint8_t uart_readbuffer[128];

//空闲中断回调函数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size)
{
//	 printf("uart_rx_dma receive data:%s\r\n",uart_rx_dma_buffer);
	if(!ringbuffer_is_full (&usart_ring))
	{
		ringbuffer_write(&usart_ring,uart_rx_dma_buffer,Size);
	}
	 memset(uart_rx_dma_buffer,0,sizeof(uart_rx_dma_buffer));
}
/*
sscanf的用途是从一个字符串里按格式读数据 
sscanf 会在输入字符串中按照 "PH(%d)" 的格式进行匹配，
%d表示当前位置应该存在一个整数，并把读取到的整数保存到指定变量地址 &num。
收到PH(1)则将1赋给num 
成功匹配几个 就返回几
sprintf的用途是将把格式化的数据写入字符串
sprintf 会按照格式字符串 "PH(%d)" 创建新的字符串，
%d表示这里需要插入一个整数变量的值。
得到PH(1) 是将a=1这个数据替换%d
*/
//解析代码区域
void usart_task()
{
	int num;//int有符号是%d 无符号是%hu
	if(ringbuffer_is_empty(&usart_ring))
	return;
	//ringbuffer的特点是从一个数组读到另一个数组时 原先的数组会被清空
	ringbuffer_read(&usart_ring,uart_readbuffer,usart_ring.itemCount);
	
	/*数据处理区域*/
	printf("uart receive data:%s\r\n",uart_readbuffer);
 //查询 VH
    if(strcmp((const char *)uart_readbuffer, "#VH") == 0)
    {
        printf("VH:%d\r\n", VH);
    }

    //查询 VD
    else if(strcmp((const char *)uart_readbuffer, "#VD") == 0)
    {
        printf("VD:%d\r\n", VD);
    }

    //设置 PD，例如上位机发送：$PD(100)
    else if(sscanf((const char *)uart_readbuffer, "$PD(%d)", &num) == 1)
    {
				if(num<=4096)
				{
        PD = num;
        printf("PD set:%d\r\n", PD);
				}
    }

    //设置 PH，例如上位机发送：$PH(200)
    else if(sscanf((const char *)uart_readbuffer, "$PH(%d)", &num) == 1)
    {
        if(num<=4096)
				{
        PH = num;
        printf("PD set:%d\r\n", PH);
				}
    }

	memset(uart_readbuffer,0,sizeof(uint8_t)*128);
}


