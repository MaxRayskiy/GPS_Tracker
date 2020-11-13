#include "main.h"
#include "stm32f0xx_it.h"


//void NMI_Handler(void){}
//void SVC_Handler(void){}
//void PendSV_Handler(void) {}

void HardFault_Handler(void)
{
  while (1)
  {}
}

void SysTick_Handler(void)
{
  HAL_IncTick();
}

void USART1_IRQHandler(void) {
	  if(LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1)) {
		  GPS_UART_callback();
	  }
}

void USART3_4_IRQHandler(void) {
	GPS_UART_debug_callback();
}
