#include "stm32f0xx.h"

//PC6..PC9
void init_leds(void);
//PA0
void init_button(void);

enum STATE {
  BEGIN,
	STATE_RED,
  STATE_GREEN,
	PAUSE,
  FLASHING,
  STATE_YELLOW
};

void green_on(void);
void red_on(void);
void yellow_on(void);

void green_off(void);
void red_off(void);
void yellow_off(void);
