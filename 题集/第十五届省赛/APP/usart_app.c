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

//解析代码区域
void usart_task()
{
	if(ringbuffer_is_empty(&usart_ring))
	return;
	//ringbuffer的特点是从一个数组读到另一个数组时 原先的数组会被清空
	ringbuffer_read(&usart_ring,uart_readbuffer,usart_ring.itemCount);
	
	/*数据处理区域*/
	printf("uart receive data:%s\r\n",uart_readbuffer);
	if(strcmp((char*)uart_readbuffer,"led_on")==0)
		ucLed[0]^=1;
	
	memset(uart_readbuffer,0,sizeof(uint8_t)*128);
}


