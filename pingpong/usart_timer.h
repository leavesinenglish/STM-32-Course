#pragma once
#include <stm32f0xx.h>
#include <stdbool.h>
#include "SPI.h"

static volatile int timer;

void usart_timer_init(void);
void TIM15_IRQHandler(void);

void usart_timer_init(){
	RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
	TIM15->PSC = 6000;
	TIM15->ARR = 1;
	TIM15->CR1 |= TIM_CR1_ARPE | TIM_CR1_DIR | TIM_CR1_CEN; 
	TIM15->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM15_IRQn);
	timer = 0;
}

void TIM15_IRQHandler(void){
	timer++;
  TIM15->SR &=(~TIM_SR_UIF);
}
