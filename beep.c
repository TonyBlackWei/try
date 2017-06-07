#include "beep.h"
#include "LPC11xx.h"
#include "my_gpio.h"
//#include "timer32.h"
//#include "tools.h"

volatile uint32_t period_beep = 2000;

void BEEPInit(void)
{
	SysTick_Config(48000); //set systick timer
  
  LPC_IOCON->R_PIO1_1 &= ~0x07;
  LPC_IOCON->R_PIO1_1 |= 0x01;		
  /*
	GPIOSetDir(PORT1, 1, 1);			
  GPIOSetValue(PORT1, 1, 1);
	*/
	
	LPC_GPIO[1]->DIR |= 1<<1;
	LPC_GPIO[1]->MASKED_ACCESS[(1<<1)] = 1<<1;
}
/*
void BEEPOn(void)
{
	init_timer32PWM(1, period_beep, MATCH0);
	LPC_TMR32B1->PR = 0x7f;
	enable_timer32(1);
}
*/

void BEEPOn(void)
{
	LPC_TMR32B1->TCR = 0;		
	
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<10);

	LPC_IOCON->R_PIO1_1  &= ~0x07;	
  LPC_IOCON->R_PIO1_1  |= 0x03;
	
	LPC_TMR32B1->PWMC |= (1<<3)|(1<<0);

  LPC_TMR32B1->MR3 = period_beep;
  LPC_TMR32B1->MR0 = period_beep/2;
 
	LPC_TMR32B1->MCR = 1<<10;				
	
	LPC_TMR32B1->PR = 0x7f;
	
	LPC_TMR32B1->TCR = 1;
}

void BEEPOff(void)
{
	LPC_IOCON->R_PIO1_1  &= ~0x07;	
  LPC_IOCON->R_PIO1_1  |= 0x02;
	
	LPC_TMR32B1->TCR = 0;	
}

void SETPeriod(uint32_t period)
{
	period_beep = period;
}
