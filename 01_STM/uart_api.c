#include "uart_api.h"
#include <string.h>

extern UART_HandleTypeDef huart1;
volatile uint8_t recv_data;
volatile uint8_t uart_buf[100];
volatile uint8_t uart_buff_cnt;
volatile bool recv_command;


void UartInit(void){
	HAL_UART_Receive_IT(&huart1, &recv_data, 1);
}

void UartWriteCommand(uint8_t *data,uint8_t size){
	 HAL_UART_Transmit(&huart1, data, size, 100);
}

bool UartReadCommand(uint8_t *data){
	if(data){
		if(!recv_command){
			return false;
		}
		strcpy(data, uart_buf);
		recv_command = false;
		uart_buff_cnt = 0;
		memset(uart_buf, 0x00, 100);
		return true;
	}
	return false;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//ascii 0x0D - line feed
	if(recv_data == 0x0D){
		recv_command = true;
	}else{
		uart_buf[uart_buff_cnt] = recv_data;
		++uart_buff_cnt;
	}

	HAL_UART_Transmit_IT(&huart1, &recv_data, 1);

	HAL_UART_Receive_IT(&huart1, &recv_data, 1);
}
