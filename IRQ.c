#include "LPC17xx.H"                         /* LPC17xx definitions           */

uint8_t  clock_1ms = 0;
extern int interrupt;
/*----------------------------------------------------------------------------
  Systick Interrupt Handler
  SysTick interrupt happens every 10 ms
 *----------------------------------------------------------------------------*/
void SysTick_Handler (void) {
  clock_1ms = 1;
}

void EINT3_IRQHandler()
{
  LPC_GPIOINT->IO2IntClr |= (1 << 10);     /* clear pending interrupt         */
	interrupt++;
}
