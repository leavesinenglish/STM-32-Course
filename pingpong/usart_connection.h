#pragma once
#include "ping_pong_game.h"
#include "usart_timer.h"
#define LOWER_DELAY_BOUND 10
#define UPPER_DELAY_BOUND 20

static volatile int sends_amount = 0;

void sync_connection(void);

void sync_connection(){
	if (transfer.isTransmit){
		timer = 0;
		set_transmission();
		transfer.isTransmit = false;
		init_usart();
		/*if (transmit()){
			sends_amount++;
			if (sends_amount > 5){
				transfer.isTransmit = false;
				init_usart();
				sends_amount = 0;
			}
		}*/
	}
	else{
		/*if (receive()){
			
		}*/
		timer = (rand() % (UPPER_DELAY_BOUND - LOWER_DELAY_BOUND + 1)) + LOWER_DELAY_BOUND;
		if (timer > 30){
			transfer.isTransmit = true;
			init_usart();
		}
		receive_transmission();
	}
}
