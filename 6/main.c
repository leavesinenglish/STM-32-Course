#include "stm32f0xx.h"
#include "adc.h"
#include "spi.h"

const uint8_t update_time = 10; // in millisecs
const uint16_t scanTime = 100; //in millisecs time of scan
uint8_t sweep_flag = 0;
uint32_t sweep_interrupt_count = 0;
uint32_t max_voltage = 1024;
uint32_t cur_voltage = 0;
uint32_t voltage_arr[8];
uint32_t prev_col = 4;
const uint8_t first_part = 0;
const uint8_t second_part = 1;
uint8_t part_for_analysis = first_part;

void SysTick_Handler(void);
void SPI2_IRQHandler(void);
void DMA1_Channel1_IRQHandler(void);
void update_voltage_arr(void);
uint32_t set_bit(uint32_t value, uint32_t position);
uint32_t get_pattern(void);

void SysTick_Handler(){
  sweep_interrupt_count += 1;
  uint32_t sweep_update_count = scanTime/update_time;
  if(sweep_interrupt_count == sweep_update_count){
    sweep_interrupt_count = 0;
    sweep_flag = 1;
  }
}

void SPI2_IRQHandler(void){
  uint32_t status = SPI2->SR;
  if((status && SPI_SR_RXNE) == SPI_SR_RXNE){
    GPIOA->BSRR = GPIO_BSRR_BS_8;
    SPI2->DR;
    uint32_t pattern = get_pattern();
    prev_col = ((prev_col + 1) == 8) ? 0 : (prev_col + 1);
    SPI2->DR = pattern;
    GPIOA->BSRR = GPIO_BSRR_BR_8;
  }
}

void DMA1_Channel1_IRQHandler(){
  if((DMA1->ISR & DMA_ISR_HTIF1)){
    part_for_analysis = second_part;
    DMA_flag = 1;
  }else if(DMA1->ISR & DMA_ISR_TCIF1){
    part_for_analysis = first_part;
    DMA_flag = 1;
  }
  DMA1->IFCR |= DMA_IFCR_CGIF1;
}

void update_voltage_arr(){
  for(int i = 0; i < 7; ++i){
    voltage_arr[7-i] = voltage_arr[6-i];
  }
  voltage_arr[0] = cur_voltage;
}

uint32_t set_bit(uint32_t value, uint32_t position){
  return (value | (1 << position));
}

uint32_t get_pattern(){
  uint32_t cur_col = (prev_col + 1) == 8 ? 0 : (prev_col + 1);
  uint32_t pattern = 0;
  pattern = set_bit(pattern,8 + cur_col);
  uint32_t line_to_show = voltage_arr[cur_col]*8/max_voltage;
  line_to_show = 7 - line_to_show;
  pattern = set_bit(pattern,line_to_show);
  return pattern;
}

int main(void){
  spi_init();
  Sys_Tick();
  adc_init();
	dma_init();
  while(1){
    if(sweep_flag == 1){
      update_voltage_arr();
      sweep_flag = 0;
    }
    if(DMA_flag == 1){
      uint32_t sum = 0;
      if(part_for_analysis == second_part){
        for (uint16_t i = 0; i < DMA_buf_size/2; ++i) sum = (sum + DMA_buf[i]);
      }else{
        uint16_t init_point = DMA_buf_size/2;
        for (uint16_t i = init_point; i < DMA_buf_size; ++i) sum = (sum + DMA_buf[i]);
      }
      cur_voltage = (sum/(DMA_buf_size/2));
      DMA_flag = 0;
    }
  }
}
