#include "sys_tick.h"

void Sys_Tick(void) {
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
  SystemCoreClockUpdate();
  SysTick->LOAD = SystemCoreClock / 10 - 1;//100ms
}
