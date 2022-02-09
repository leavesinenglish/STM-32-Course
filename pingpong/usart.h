#pragma once
#include "SPI.h"

typedef struct _transfer{
	unsigned char dataT;
	unsigned char dataR;
	bool isTransmit;
}Transfer;

static volatile Transfer transfer;

void init_usart(void);
void transfer_init(bool isTransmit);
bool transmit(void);
bool receive(void);


void init_usart(){
	if (transfer.isTransmit){
		USART3->CR1 &= ~USART_CR1_UE;
		USART3->ICR = 0;
		USART3->CR1 &= ~USART_CR1_RE;
		USART3->BRR = 480000 / 96;
		USART3->CR1 = USART_CR1_TXEIE | USART_CR1_TE | USART_CR1_UE;
	}
	else{
		USART3->CR1 &= ~USART_CR1_UE;
		USART3->ICR = 0;
		USART3->CR1 &= ~USART_CR1_TE;
		USART3->BRR = 480000 / 96;
		USART3->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_UE;
	}
}

void transfer_init(bool isTransmit){
	RCC->APB1ENR|= RCC_APB1ENR_USART3EN;
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIODEN;
	
	GPIOC->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;
	GPIOD->MODER |= GPIO_MODER_MODER2_1;
	
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR10_1 | GPIO_PUPDR_PUPDR11_1;
	GPIOD->PUPDR |= GPIO_PUPDR_PUPDR2_1;
	
	USART3->CR3 |= USART_CR3_DEM;
	GPIOC->AFR[1] |= 1 << (10 - 8) * 4;
	GPIOC->AFR[1] |= 1 << (11 - 8) * 4;
	GPIOD->AFR[0] |= 1 << (2  - 0) * 4;
	NVIC_EnableIRQ(USART3_4_IRQn);
	transfer.dataR = 0;
	transfer.dataT = 0;
	transfer.isTransmit = isTransmit;
	init_usart();
}

bool transmit(){
		if ((USART3->ISR & USART_ISR_TC) == USART_ISR_TC){
			/* clear transfer complete flag and fill TDR with a new char */
			USART3->TDR = transfer.dataT;
			USART3->ICR |= USART_ICR_TCCF;
			return true;
		}
		return false;
}

bool receive(){
		if ((USART3->ISR & USART_ISR_RXNE) == USART_ISR_RXNE){
			 /* Receive data, clear flag */
			transfer.dataR = (uint8_t)(USART3->RDR);
			return true;
		}
		return false;
}
