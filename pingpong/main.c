#include <stm32f0xx.h>
#include <system_stm32f0xx.h>
#include "usart_connection.h"

void init(void);
void SysTick_Handler (void);
void SPI2_IRQHandler(void);
void USART3_4_IRQHandler(void);

static volatile uint32_t TIME = 0;

void SysTick_Handler (void){
   TIME++;
	 ask_buttons(TIME);	
}

void SPI2_IRQHandler(void){
	if (SPI2->SR & SPI_SR_RXNE) {	
		GPIOA->BSRR = GPIO_BSRR_BS_8;
		SPI2->DR;
		process_image();
		GPIOA->BSRR = GPIO_BSRR_BR_8;
	}
}

void USART3_4_IRQHandler(void) {
	if ((USART3->ISR & USART_ISR_RXNE) == USART_ISR_RXNE) {
		transfer.dataR = (uint8_t)(USART3->RDR);
	}
	if ((USART3->ISR & USART_ISR_TXE) == USART_ISR_TXE) {
		USART3->TDR = transfer.dataT;
		//USART3->ICR |= USART_ICR_TCCF;
	}
}

void init(void){
	for(unsigned long i =0;i<15;++i){
		GPIOC->MODER |= GPIO_MODER_MODER0+i;
	}
	SystemInit();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/500);
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
	
	init_buttons();
	reset_buttons();
	initSPI();
	init_game();
	usart_timer_init();
	transfer_init(false);
}

int main(void){
	init();
	while(1){
		update_game(TIME);
		sync_connection();
		clear_screen();
	}
}
