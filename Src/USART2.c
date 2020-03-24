#include "USART.h"
#include "stm32f10x.h"

void Usart_SendByte(USART_TypeDef * pUSARTx, uint8_t ch){
	USART_SendData(pUSARTx,ch);		
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}//����һ���ֽ����ݵ�USART

void Usart_SendString(USART_TypeDef * pUSARTx, char *str){
	do{
		Usart_SendByte(pUSARTx, *str++);;
	} while(*str != '\0');
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}//����һ���ַ������ݵ�USART

static void USART_GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//��USARTx��GPIOʱ��
	USART_GPIO_APBxClkCmd(USART_GPIO_CLK, ENABLE);
	
	//����USARTx TX��GPIOΪ�����������ģʽ
	GPIO_InitStruct.GPIO_Pin = USART_TX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitStruct);
	
	//����USARTx RX��GPIOΪ��������ģʽ
	GPIO_InitStruct.GPIO_Pin = USART_RX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitStruct);
	
}

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

static void USART_USART_Init(void){
	USART_InitTypeDef USART_InitStruct;
	
	//��USARTx��ʱ��
	USART_APBxClkCmd(USART_CLK, ENABLE);
	
	USART_InitStruct.USART_BaudRate = USART_BAUDRATE;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx, &USART_InitStruct);
	
	//�����ж����ȼ�����
	NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(USARTx, ENABLE);	
	
}



void USART3_Init(void){
	USART_GPIO_Init();
	USART_USART_Init();
}