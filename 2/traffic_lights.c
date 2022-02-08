#include "traffic_lights.h"

void init_leds(void){
  RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  GPIOC -> MODER &=~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER8 |GPIO_MODER_MODER9);
  GPIOC -> MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER7_0|GPIO_MODER_MODER8_0;  
}

void init_button(void){
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  GPIOA -> MODER &=~ GPIO_MODER_MODER0;
}

void green_on(void){
  GPIOC->BSRR = GPIO_BSRR_BS_9;
}
void red_on(void){
  GPIOC->BSRR = GPIO_BSRR_BS_6;
}
void yellow_on(void){
  GPIOC->BSRR = GPIO_BSRR_BS_8;
}
void green_off(void){
  GPIOC->BSRR = GPIO_BSRR_BR_9;
}
void red_off(void){
  GPIOC->BSRR = GPIO_BSRR_BR_6;
}
void yellow_off(void){
  GPIOC->BSRR = GPIO_BSRR_BR_8;
}
