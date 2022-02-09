#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <stm32f0xx.h>
#include <string.h>

typedef struct {
	int lines[8];
} Matrix;

static volatile Matrix buffers[2];
static volatile Matrix* front_buffer;
static volatile Matrix* back_buffer;
static volatile bool is_rendered = true;
static volatile int rendering_line = 0;
static volatile bool is_ready = false;

void initSPI(void);
void draw_line(int row, volatile Matrix* renderBuffer);
void draw_dot(int x, int y);
void clear_screen(void);
void process_image(void);

void initSPI(){
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; 
	GPIOB->AFR[1] |= 0 << (15 - 8) * 4;
	GPIOB->AFR[1] |= 0 << (13 - 8) * 4;
	SPI2->CR1 = 
		  SPI_CR1_SSM 
		| SPI_CR1_SSI 
		| SPI_CR1_BR 
		| SPI_CR1_MSTR
		| SPI_CR1_CPOL
		| SPI_CR1_CPHA;
	SPI2->CR2 = SPI_CR2_DS | SPI_CR2_RXNEIE;
	GPIOB->MODER |= GPIO_MODER_MODER13_1 | GPIO_MODER_MODER15_1;
	GPIOA->MODER |= GPIO_MODER_MODER8_0;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR13_1 | GPIO_PUPDR_PUPDR15_1;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR8_1;	
	NVIC_EnableIRQ(SPI2_IRQn);
	SPI2->CR1 |= SPI_CR1_SPE;
	SPI2->DR = 0;
	front_buffer = &buffers[0];
	back_buffer = &buffers[1];
}

void draw_line(int row, volatile Matrix* renderBuffer){
	int result = 0;
	result |= renderBuffer->lines[row];
	SPI2->DR |= result;
}

void draw_dot(int x, int y){
	if(y<8 && y>=0 && x>=0 && x<8){
		front_buffer->lines[y] |= (0x1U << y) << 8 | (0x1U << x);
	}
}

void clear_screen(){
	is_ready = true;
	volatile Matrix* tmp = front_buffer;
	front_buffer = back_buffer;
	back_buffer = tmp;
	memset((void*)front_buffer, 0, sizeof(Matrix));
}

void process_image(void){
	if(rendering_line >= 8){
		is_rendered = true;
	}
	if(is_rendered){
		if(!is_ready){
			return;
		}
		rendering_line = 0;
		is_rendered = false;
	}
	draw_line(rendering_line, back_buffer);
	rendering_line++;
}
