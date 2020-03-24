#include "USART.h"
#include "stm32f10x.h"

void Usart_SendByte(USART_TypeDef * pUSARTx, uint8_t ch){
	USART_SendData(pUSARTx,ch);		
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}//发送一个字节数据到USART

void Usart_SendString(USART_TypeDef * pUSARTx, char *str){
	do{
		Usart_SendByte(pUSARTx, *str++);;
	} while(*str != '\0');
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}//发送一个字符串数据到USART

static void USART_GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//打开USARTx的GPIO时钟
	USART_GPIO_APBxClkCmd(USART_GPIO_CLK, ENABLE);
	
	//配置USARTx TX的GPIO为复用推挽输出模式
	GPIO_InitStruct.GPIO_Pin = USART_TX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitStruct);
	
	//配置USARTx RX的GPIO为浮空输入模式
	GPIO_InitStruct.GPIO_Pin = USART_RX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitStruct);
	
}

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

static void USART_USART_Init(void){
	USART_InitTypeDef USART_InitStruct;
	
	//打开USARTx的时钟
	USART_APBxClkCmd(USART_CLK, ENABLE);
	
	USART_InitStruct.USART_BaudRate = USART_BAUDRATE;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx, &USART_InitStruct);
	
	//串口中断优先级配置
	NVIC_Configuration();
	
	// 使能串口接收中断
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(USARTx, ENABLE);	
	
}



void USART3_Init(void){
	USART_GPIO_Init();
	USART_USART_Init();
}