#pragma once
#include "buttons.h"
#include <math.h>
#include "usart.h"

#define GAME 1
#define MENU 0

typedef struct {
	uint8_t x;
	uint8_t y;
	uint8_t width;
} Platform;

typedef struct {
	float x;
	float y;
	float x_velocity;
	float y_velocity;
} Ball;

typedef struct{
	bool mode;
	bool host;
} Game_state;

volatile static Game_state gameState;
static volatile Platform platform_down;
static volatile Platform platform_up;
static volatile Ball ball;
static volatile Platform* controlled_platform;
static volatile int cursorY = 6;
static volatile uint32_t lastUpdate = 0;
static volatile bool transmit_ball = false;

void init_game(void);
void draw_platform(volatile Platform* player);
void update_ball(void);
void move_platform(void);
void draw_menu(void);
uint8_t get_bit(uint8_t pos, uint8_t src);
void set_bit(uint8_t pos, uint8_t* dst);
uint8_t load_sequence(uint8_t src, uint8_t offset);
uint8_t write_sequence(uint8_t src, uint8_t offset);
void set_transmission(void);
void receive_transmission(void);
void update_game(volatile uint32_t timestamp);

void init_game(){
	platform_down.x = 0;
	platform_up.x = 7;
	
	platform_down.y = platform_up.y = 1;
	platform_down.width = platform_up.width = 3;
	
	controlled_platform = &platform_down;
	
	ball.x = 3;
	ball.y = 3;
	ball.x_velocity = 0.2f;
	ball.y_velocity = 0.1f;
	
	gameState.mode = MENU;
}

void draw_platform(volatile Platform* player){
	for(int i=-player->width/2;i<=player->width/2; ++i)
	{
		draw_dot(player->x, player->y+i);
	}
}

void update_ball(){
	ball.x += ball.x_velocity;
	ball.y += ball.y_velocity;
	if(ball.x>7 || ball.x<0){
		if ((round(ball.y)<=platform_down.y+platform_down.width/2 && round(ball.y)>=platform_down.y-platform_down.width/2 && ball.x<0) ||
			(round(ball.y)<=platform_up.y+platform_up.width/2 && round(ball.y)>=platform_up.y-platform_up.width/2 && ball.x>7)){
				ball.x_velocity *= -1.0f;
		}
		else{			
			ball.x = 3;
			ball.y = 3;
		}
	}
	if(ball.y>7 || ball.y<0){
		ball.y_velocity *= -1.0f;
	}
}

void move_platform(){
	if(key_states[Key_Up].state && controlled_platform->y+controlled_platform->width/2+1<8){
		controlled_platform->y++;
	}
	if(key_states[Key_Down].state && controlled_platform->y-controlled_platform->width/2-1>=0){
		controlled_platform->y--;
	}
}

void draw_menu(){
	//draw 1
	draw_dot(3,6);
	draw_dot(4,6);
	draw_dot(5,6);
	draw_dot(6,6);
	draw_dot(7,6);
	//draw 2
	draw_dot(7,1);
	draw_dot(7,2);
	draw_dot(7,3);
	draw_dot(6,1);
	draw_dot(5,1);
	draw_dot(5,2);
	draw_dot(5,3);
	draw_dot(4,3);
	draw_dot(3,1);
	draw_dot(3,2);
	draw_dot(3,3);
}

uint8_t get_bit(uint8_t pos, uint8_t src){
	return (src & (1 << pos)) >> pos;
}

void set_bit(uint8_t pos, uint8_t* dst){
	*dst |= 1UL << pos;
}

uint8_t load_sequence(uint8_t src, uint8_t offset){
	uint8_t result=0;
	if(get_bit(2+offset, src)){
		set_bit(0, &result);
	}
	if(get_bit(3+offset, src)){
		set_bit(1, &result);
	}
	if(get_bit(4+offset, src)){
		set_bit(2, &result);
	}
	return result;
}

uint8_t write_sequence(uint8_t src, uint8_t offset){
	uint8_t result=0;
	if(get_bit(0, src)){
		set_bit(2+offset, &result);
	}
	if(get_bit(1, src)){
		set_bit(3+offset, &result);
	}
	if(get_bit(2, src)){
		set_bit(4+offset, &result);
	}
	return result;
}

void set_transmission(){
	if(gameState.mode == GAME){
		if(gameState.host){
			if(!transmit_ball){
				uint8_t out = write_sequence(platform_down.y, 0);
				set_bit(1, &out);//01
				transfer.dataT = out;
			}
			if(transmit_ball){
				uint8_t out = write_sequence((int)round(ball.x), 0) | write_sequence((int)round(ball.y), 3);
				set_bit(0, &out);//10
				transfer.dataT = out;
			}
		}
		else{
			uint8_t out = write_sequence(platform_up.y, 0);
			set_bit(1, &out);//01
			transfer.dataT = out;
		}
		transmit_ball = !transmit_ball;
	}
}

void receive_transmission(){
	if(gameState.mode == GAME){
		if(!gameState.host){
			if(get_bit(0, transfer.dataR) == 0 && get_bit(1, transfer.dataR) == 1){
				platform_down.y = load_sequence(transfer.dataR, 0);
			}
			if(get_bit(0, transfer.dataR) == 1 && get_bit(1, transfer.dataR) == 0){
				ball.x = (float)load_sequence(transfer.dataR, 0);
				ball.y = (float)load_sequence(transfer.dataR, 3);
			}
		}
		else{
			if(get_bit(0, transfer.dataR) == 0 && get_bit(1, transfer.dataR) == 1){
				platform_up.y = load_sequence(transfer.dataR, 0);
			}
		}
	}
}

void update_game(volatile uint32_t time){
	if(gameState.mode == GAME){
		if(time-lastUpdate>30){
			if(gameState.host){
				update_ball();
			}
			move_platform();
			lastUpdate = time;
		}
		draw_platform(&platform_down);
		draw_platform(&platform_up);
		draw_dot((int)round(ball.x), (int)round(ball.y));
	}
	else if(gameState.mode == MENU){
		if(time-lastUpdate>30){
			if(key_states[Key_Up].state){
				cursorY = 6;
			}
			if(key_states[Key_Down].state){
				cursorY = 2;
			}
			lastUpdate = time;
		}
		draw_dot(1, cursorY);// draw cursor
		draw_menu();
		if(key_states[Key_Right].state && (cursorY == 1 || cursorY == 2 || cursorY == 3)){//connect to host machine
			gameState.mode = GAME;
			gameState.host = false;
			controlled_platform = &platform_up;
			transfer.isTransmit = false;
			init_usart();
		}
		if(key_states[Key_Right].state && cursorY == 6){//set as host
			gameState.mode = GAME;
			gameState.host = true;
			controlled_platform = &platform_down;
			transfer.isTransmit = true;
			init_usart();
		}
	}
}
