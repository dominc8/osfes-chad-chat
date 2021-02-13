#ifndef _UART_API_h_
#define _UART_API_h_

#include "main.h"

#include <stdbool.h>
void USART1_IRQHandler(void);

void UartInit(void);
void UartWriteCommand(uint8_t *data,uint8_t size);
bool UartReadCommand(uint8_t *data);



#endif
