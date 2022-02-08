#include <stm32f0xx.h>
#include <stdbool.h>
#include "buttons.h"
volatile static int x = 1;
volatile static int y = 1;
volatile static int matrix[8] = {0};

void init_all(void);
void Sys_Tick(void);
void SPI2_IRQHandler(void);
void SysTick_Handler(void);
void draw_line(int y);
void draw_start_cross(void);

void init_all(){
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN|RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
	GPIOA->MODER &= ~GPIO_MODER_MODER0;
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0;
	
	GPIOC->MODER &= ~GPIO_MODER_MODER12;
	GPIOC->MODER |=	GPIO_MODER_MODER12_0;
	
	GPIOA->MODER &= ~GPIO_MODER_MODER15;
	GPIOA->MODER |= GPIO_MODER_MODER15_0;
	
	GPIOA->MODER &= ~GPIO_MODER_MODER4;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR5_1;
	GPIOA->MODER &= ~GPIO_MODER_MODER5;
	
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	
	SPI2->CR1 = SPI_CR1_SSM | SPI_CR1_SSI	| SPI_CR1_BR | SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA;
	SPI2->CR2 = SPI_CR2_DS | SPI_CR2_RXNEIE;
	//NVIC_SetPriority(SPI2_IRQn, 1);
	NVIC_EnableIRQ(SPI2_IRQn);
	
	GPIOB->MODER |= GPIO_MODER_MODER13_1 | GPIO_MODER_MODER15_1;
	GPIOA->MODER |= GPIO_MODER_MODER8_0;
	
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR13_1 | GPIO_PUPDR_PUPDR15_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR8_1;
	GPIOB->AFR[1] = 0 << (15 - 8) * 4;
  GPIOB->AFR[1] = 0 << (13 - 8) * 4;
	
	SPI2->CR1 |= SPI_CR1_SPE;
	SPI2->DR = 0;
}

void Sys_Tick(void){
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
	SystemCoreClockUpdate();
	SysTick->LOAD = SystemCoreClock / 1000 - 1;//10ms
}

void SPI2_IRQHandler(void){
	static int dy = 0;
	if (SPI2->SR & SPI_SR_RXNE){
		GPIOA->BSRR = GPIO_BSRR_BS_8;
		SPI2->DR;
		draw_line(dy);
		if (dy <= 7){
			dy++;
		}
		else dy = 0;
	}
}

void SysTick_Handler(void){	
  up_down();
  right_left();
}

void draw_line(int _y){
		GPIOA->BSRR = GPIO_BSRR_BR_8;
		SPI2->DR = (matrix[_y]) | (1 << (_y)<<8);
}

void draw_start_cross(){
	matrix[x-1] = 1<<y;
	matrix[x] = 1<<y | 1<<(y-1) | 1<<(y+1);
	matrix[x+1] = 1<<y;
}

int main(void){
	init_all();
	Sys_Tick();
	draw_start_cross();
	while(1){
		if (b_states.left_button){
			if(x > 1) {
				matrix[x+1] = 0;
				matrix[x-2] = 1<<y;
				matrix[x] = 1<<y;
				matrix[x-1] = 1<<y | 1<<(y-1) | 1<<(y+1);
				--x;		
			}
			b_states.left_button = false;
	  }
		else if (b_states.right_button){
			if(x < 6) {
				matrix[x-1] = 0;
				matrix[x+2] = 1<<y;
				matrix[x] = 1<<y;
				matrix[x+1] = (1 << y| 1<<(y-1) | 1<<(y+1));
				++x;	
				
			}
			b_states.right_button = false;
		}
		else if (b_states.down_button){
			if(y < 6) {
				matrix[x+1] = matrix[x+1] << 1;
				matrix[x] = matrix[x] << 1;
				matrix[x-1] = matrix[x-1] << 1;
				++y;	
			}
			b_states.down_button = false;
		}
		else if (b_states.up_button){
			if(y > 1) {
				matrix[x+1] = matrix[x+1] >> 1;
				matrix[x] = matrix[x] >> 1;
				matrix[x-1] = matrix[x-1] >> 1;
				--y;
			}
			b_states.up_button = false;
		}
	}
}
