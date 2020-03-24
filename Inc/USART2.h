#ifndef _USART_H_
#define _USART_H_

#include "stm32f10x.h"

void Usart_SendByte(USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString(USART_TypeDef * pUSARTx, char *str);


#define	USARTx                   		USART3
#define	USART_CLK                		RCC_APB2Periph_USART3
#define USART_APBxClkCmd         		RCC_APB2PeriphClockCmd
#define USART_BAUDRATE           		9600

// USART GPIO Òý½Åºê¶¨Òå
#define USART_GPIO_CLK           		(RCC_APB2Periph_GPIOB)
#define USART_GPIO_APBxClkCmd    		RCC_APB2PeriphClockCmd
    
#define USART_TX_GPIO_PORT       		GPIOB   
#define USART_TX_GPIO_PIN        		GPIO_Pin_10
#define USART_RX_GPIO_PORT       		GPIOB
#define USART_RX_GPIO_PIN        		GPIO_Pin_11

#define USART_IRQ                		USART3_IRQn
#define USART_IRQHandler         		USART3_IRQHandler

void USART3_Init(void);


#endif
