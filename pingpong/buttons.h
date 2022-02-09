#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <stm32f0xx.h>

#define DELAY 15
#define Key_Up 2
#define Key_Down 3
#define Key_Right 1
#define Key_Left 0

typedef struct {
	bool prev_state;
	bool state;
	bool clicked;
	uint32_t last_change_time;
} key_state;

static volatile key_state key_states[4] = {0}; 

void reset_buttons(void);
void set_button(volatile key_state* key, bool button_state, volatile uint32_t timestamp);
void ask_buttons(volatile uint32_t timestamp);
void init_buttons(void);

void reset_buttons(){
	for(int i=0;i<4;++i){
		key_states[i].last_change_time = 0;
		key_states[i].state = false;
		key_states[i].prev_state = false;
		key_states[i].clicked = false;
	}
}

void set_button(volatile key_state* key, bool button_state, volatile uint32_t timestamp){
	if(timestamp > key->last_change_time + DELAY){
		key->prev_state = key->state;
		key->state = button_state;
		key->clicked = key->state != key->prev_state;
		key->last_change_time = timestamp;
	}
}

void ask_buttons(volatile uint32_t timestamp){
	GPIOA->ODR &= ~GPIO_ODR_15;
	GPIOC->ODR &= ~GPIO_ODR_12;
	GPIOA->IDR &= ~GPIO_IDR_4;
	GPIOA->IDR &= ~GPIO_IDR_5;
	
	GPIOA->ODR |= GPIO_ODR_15;
	GPIOC->ODR &= ~GPIO_ODR_12;
	
	set_button(&key_states[0], GPIOA->IDR & GPIO_IDR_4, timestamp);
	set_button(&key_states[1], GPIOA->IDR & GPIO_IDR_5, timestamp);
	
	GPIOA->ODR &= ~GPIO_ODR_15;
	GPIOC->ODR &= ~GPIO_ODR_12;
	GPIOA->IDR &= ~GPIO_IDR_4;
	GPIOA->IDR &= ~GPIO_IDR_5;
	
	GPIOA->ODR &= ~GPIO_ODR_15;
	GPIOC->ODR |= GPIO_ODR_12;

	set_button(&key_states[2], GPIOA->IDR & GPIO_IDR_4, timestamp);
	set_button(&key_states[3], GPIOA->IDR & GPIO_IDR_5, timestamp);
	
  GPIOA->ODR &= ~GPIO_ODR_15;
	GPIOC->ODR &= ~GPIO_ODR_12;
	GPIOA->IDR &= ~GPIO_IDR_4;
	GPIOA->IDR &= ~GPIO_IDR_5;
}

void init_buttons(){
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN;
	
	GPIOC->MODER &= ~GPIO_MODER_MODER0;
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0;//enable ports
	
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | ~GPIO_OTYPER_OT_9);
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);


	GPIOA->MODER |= GPIO_MODER_MODER15_0;
	GPIOA->MODER &= ~GPIO_MODER_MODER4;
	GPIOA->MODER &= ~GPIO_MODER_MODER5;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_1 | GPIO_PUPDR_PUPDR5_1;
	GPIOC->MODER |= GPIO_MODER_MODER12_0;
}
