#include "stm32f0xx.h"
#include <stdbool.h>

typedef struct states{
	bool up_button;
	bool down_button;
	bool right_button;
	bool left_button;
} states;

typedef struct pressed{
	bool up_button;
	bool down_button;
	bool right_button;
	bool left_button;
} pressed;

typedef struct time{
	int up_button;
	int down_button;
	int right_button;
	int left_button;
} time;
	
volatile states b_states = {false, false, false, false};
volatile pressed b_pressed = {true, true, true, true};
volatile time b_time = {0, 0, 0, 0};

void up_down(int time);
void up_down(int time){
	GPIOA->BSRR = GPIO_BSRR_BS_15;
	
	if (GPIOA->IDR & GPIO_IDR_4){
		if (b_time.up_button + 80 <= time ){
			if (b_pressed.up_button){
				b_states.up_button = true;
				b_pressed.up_button = false;
			} else {
				b_states.up_button = false;
				b_pressed.up_button = true;
			}
		}
		b_time.up_button = time;
	}
	
	else if (GPIOA->IDR & GPIO_IDR_5){
		if (b_time.down_button + 80 <= time){
			if (b_pressed.down_button){
				b_states.down_button = true;
				b_pressed.down_button = false;
			} else {
				b_states.down_button = false;
				b_pressed.down_button = true;
			}
		}
		b_time.down_button = time;
	}
	
	GPIOA->BRR = GPIO_BRR_BR_15;
}

void right_left(int time);
void right_left(int time){
	GPIOC->BSRR = GPIO_BSRR_BS_12;

	if (GPIOA->IDR & GPIO_IDR_4){
		if (b_time.right_button + 80 <= time){
			if (b_pressed.right_button){
				b_states.right_button = true;
				b_pressed.right_button = false;
			} else {
				b_states.right_button = false;
				b_pressed.right_button = true;
			}
		}
		b_time.right_button = time;
	}
	
	else if (GPIOA->IDR & GPIO_IDR_5){
		if (b_time.left_button + 80 <= time){
			if (b_pressed.left_button){
				b_states.left_button = true;
				b_pressed.left_button = false;
			} else {
				b_states.left_button = false;
				b_pressed.left_button = true;
			}
		}
		b_time.left_button = time;
	}

	GPIOC->BRR = GPIO_BRR_BR_12;
}
