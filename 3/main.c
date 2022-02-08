#include "buttons.h"

static int CLOCK = 0;

void init_all(void);
void Sys_Tick(void);
void SysTick_Handler(void);

void init_all(void){
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5);
	GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR4_1 | GPIO_PUPDR_PUPDR5_1);
	GPIOC->MODER &= ~GPIO_MODER_MODER12;
	GPIOC->MODER |= GPIO_MODER_MODER12_0;
	GPIOA->MODER &= ~GPIO_MODER_MODER4;
	GPIOA->MODER &= ~GPIO_MODER_MODER5;
	GPIOA->MODER &= ~GPIO_MODER_MODER15;
	GPIOA->MODER |= GPIO_MODER_MODER15_0;
	GPIOC->MODER &=~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8 |GPIO_MODER_MODER9);
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER7_0|GPIO_MODER_MODER8_0;
}

void Sys_Tick(void){
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
  SystemCoreClockUpdate();
  SysTick->LOAD = SystemCoreClock / 1000 - 1;//1ms
}

void SysTick_Handler(void){
  CLOCK += 1;
  up_down(CLOCK);
  right_left(CLOCK);
}

int main(void){
	init_all();
	Sys_Tick();
	while(1){
		GPIOC->BRR = GPIO_BRR_BR_6;
		GPIOC->BRR = GPIO_BRR_BR_7;
		GPIOC->BRR = GPIO_BRR_BR_8;
		GPIOC->BRR = GPIO_BRR_BR_9;
		if (b_states.up_button)
			GPIOC->BSRR = GPIO_BSRR_BS_6;
		if (b_states.down_button)
			GPIOC->BSRR = GPIO_BSRR_BS_7;
		if (b_states.right_button)
			GPIOC->BSRR = GPIO_BSRR_BS_9;
		if (b_states.left_button)
			GPIOC->BSRR = GPIO_BSRR_BS_8;
	}
}
