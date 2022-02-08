// if ( SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk ){...} //if time run out
#include "traffic_lights.h"
#include "sys_tick.h"
#define TIMELIMIT 200

static int CLOCK = 0;

void SysTick_Handler(void);
void SysTick_Handler(void){
	CLOCK += 10;
}

int main(void){
  init_leds();
  init_button();
  Sys_Tick();
  int flash_count = 0;
  enum STATE state = BEGIN;
  while(1){
			if (!(GPIOA->IDR &= GPIO_IDR_0)) {
				switch(state){
						case BEGIN:
							if (CLOCK > 0){
								CLOCK = 0;
								red_on();
								state = STATE_RED;
							}
							break;
						case STATE_RED:
							if (CLOCK >= TIMELIMIT & CLOCK < TIMELIMIT * 2)
								yellow_on();          
							else if (CLOCK >= TIMELIMIT * 2){
								CLOCK = 0;
								red_off();
								yellow_off();
								green_on();
								state = STATE_GREEN;
							}
							break;
						case STATE_GREEN:
							if (CLOCK >= TIMELIMIT){
								CLOCK = 0;
								green_off();
								state = PAUSE;
							}
							break;
						case PAUSE:
							if (CLOCK >= TIMELIMIT / 5){
								CLOCK = 0;
								state = FLASHING;
							}
							break;
						case FLASHING:
							if (CLOCK < TIMELIMIT / 5)
								green_on();
							else if (CLOCK >= TIMELIMIT / 5 & flash_count < 2){
								CLOCK = 0;
								green_off();
								state = PAUSE;
								flash_count++;
							}
							else if (CLOCK >= TIMELIMIT / 5 & flash_count >= 2){
								CLOCK = 0;
								green_off();
								state = STATE_YELLOW;
								yellow_on();
								flash_count = 0;
							}
							break;
						case STATE_YELLOW:
							if (CLOCK >= TIMELIMIT){
								CLOCK = 0;
								yellow_off();
								red_on();
								state = STATE_RED;
							}
							break;
					}
			} else
					switch(state){
							case STATE_RED:
								CLOCK = 0;
								red_off();
								yellow_off();
								green_on();
								state = STATE_GREEN;
								break;
							default:
								break;
					}
  }
}


