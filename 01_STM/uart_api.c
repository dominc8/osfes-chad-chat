#include "uart_api.h"
#include <string.h>

static void MX_USART1_UART_Init(void);
UART_HandleTypeDef huart1;
volatile uint8_t recv_data;
volatile uint8_t uart_buf[100];
volatile uint8_t uart_buff_cnt;
volatile bool recv_command;


void UartInit(void){
	//__USART2_CLK_ENABLE;

	MX_USART1_UART_Init();

	HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

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

static void MX_USART1_UART_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;


  __HAL_RCC_USART1_CLK_ENABLE();

  __HAL_RCC_GPIOA_CLK_ENABLE();
  /**USART1 GPIO Configuration
  PA10     ------> USART1_RX
  PA9     ------> USART1_TX
  */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(VCP_RX_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(VCP_TX_GPIO_Port, &GPIO_InitStruct);

  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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

void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}
