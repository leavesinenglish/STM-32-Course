#include "stm32f0xx.h"

//PC6..PC9
void init_leds(void);
void init_leds(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC -> MODER &=~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8 |GPIO_MODER_MODER9);
	// ne ochen udobno:GPIOC -> MODER |= 0x01 << (6*2);
	// R |= M; bitset, m = mask
	// R &= ~M; bitclear
	GPIOC -> MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER7_0|GPIO_MODER_MODER8_0;	
}

//PA0
void init_button(void);
void init_button(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA -> MODER &=~ GPIO_MODER_MODER0;
}


void from_6_to_9(void);
void from_6_to_9(void){
	for (int time = 0; time<100000;time++){
			GPIOC->ODR |= GPIO_ODR_6; // up, red
		}
		GPIOC->ODR &=~GPIO_ODR_6;
		for (int time = 0; time<100000;time++){
			GPIOC->ODR |= GPIO_ODR_7; // down, blue
		}
		GPIOC->ODR &=~GPIO_ODR_7;
		for (int time = 0; time<100000;time++){
			GPIOC->ODR |= GPIO_ODR_8; // left, orange
		}
		GPIOC->ODR &=~GPIO_ODR_8;
		for (int time = 0; time<100000;time++){
			GPIOC->ODR |= GPIO_ODR_9; // right, green
		}
		GPIOC->ODR &=~GPIO_ODR_9;
}

void clockwise(void);
void clockwise(void){
		for (int time = 0; time<50000;time++){
			GPIOC->ODR |= GPIO_ODR_6; // up, red
		}
		GPIOC->ODR &=~GPIO_ODR_6;
		for (int time = 0; time<50000;time++){
			GPIOC->ODR |= GPIO_ODR_9; // right, green
		}
		GPIOC->ODR &=~GPIO_ODR_9;
		for (int time = 0; time<50000;time++){
			GPIOC->ODR |= GPIO_ODR_7; // down, blue
		}
		GPIOC->ODR &=~GPIO_ODR_7;
		for (int time = 0; time<50000;time++){
			GPIOC->ODR |= GPIO_ODR_8; // left, orange
		}
		GPIOC->ODR &=~GPIO_ODR_8;
}

void counterclockwise(void);
void counterclockwise(void){
	for (int time = 0; time<50000;time++){
			GPIOC->ODR |= GPIO_ODR_6; // up, red
		}
		GPIOC->ODR &=~GPIO_ODR_6;
	for (int time = 0; time<50000;time++){
			GPIOC->ODR |= GPIO_ODR_8; // left, orange
		}
		GPIOC->ODR &=~GPIO_ODR_8;
	for (int time = 0; time<50000;time++){
			GPIOC->ODR |= GPIO_ODR_7; // down, blue
		}
		GPIOC->ODR &=~GPIO_ODR_7;
	for (int time = 0; time<50000;time++){
			GPIOC->ODR |= GPIO_ODR_9; // right, green
		}
		GPIOC->ODR &=~GPIO_ODR_9;
		
}
int main(void)
{
	init_button();
	init_leds();
	while(1){
		if (!(GPIOA->IDR &= GPIO_IDR_0)){
			clockwise();
		}
		else {
			counterclockwise();
		}
	}
}
