#include "stm32f0xx.h"
#include <stdbool.h>
#define MAX_TIME 150

typedef struct states{
  bool up_button;
  bool down_button;
  bool right_button;
  bool left_button;
} states;

typedef struct time{
  int up_button;
  int down_button;
  int right_button;
  int left_button;
} time;
  
volatile static states b_states = {false, false, false, false};
volatile static time b_time = {0, 0, 0, 0};

void up_down(void);
void up_down(void){
  GPIOA->BSRR = GPIO_BSRR_BS_15;
  
  if (GPIOA->IDR & GPIO_IDR_4){
    if (b_time.up_button < MAX_TIME){
			b_time.up_button++;
		} 
		else {
			b_states.up_button = true;
			b_time.up_button = 0;
		}
	}
  else if (GPIOA->IDR & GPIO_IDR_5){
    if (b_time.down_button < MAX_TIME){
			b_time.down_button++;
		} 
		else {
			b_states.down_button = true;
			b_time.down_button = 0;
		}
	}
  GPIOA->BRR = GPIO_BRR_BR_15;
}

void right_left(void);
void right_left(void){
  GPIOC->BSRR = GPIO_BSRR_BS_12;

  if (GPIOA->IDR & GPIO_IDR_4){
    if (b_time.right_button < MAX_TIME){
			b_time.right_button++;
		} 
		else {
			b_states.right_button = true;
			b_time.right_button = 0;
			}
		}
  else if (GPIOA->IDR & GPIO_IDR_5){
    if (b_time.left_button < MAX_TIME){
			b_time.left_button++;
		}
		else {
			b_states.left_button = true;
			b_time.left_button = 0;
		}
	}
  GPIOC->BRR = GPIO_BRR_BR_12;
}
