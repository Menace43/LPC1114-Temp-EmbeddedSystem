#include "timer.h"
void timer_init(int frequency)
{
	 int frequency_init;
//	 // Initialize Pin as match register
//	  LPC_IOCON ->PIO1_5 |=(0x0);
     	LPC_TMR32B0->TCR = 2;			   // reset timer
	
			LPC_TMR32B0->TCR = 2;
		// Enables clock for 32 bit timer (CT32B0)  Page 34
		LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9);  
		
		// timer mode not counter mode  (Page 367)
		LPC_TMR32B0->CTCR = 0; 
	
		// make the TC trigger an interrupt when MR0 matches with TC
		LPC_TMR32B0->MCR = 0x3;   // page 368
	 
	    frequency_init =(48000000/frequency);   // MR0 Value that causes the number of Hz. was 48000000
		// place value in match register MR0
		LPC_TMR32B0->MR0 =frequency_init; // random value for an example
	
		// enable the timer interrupt
		NVIC_EnableIRQ(TIMER_32_0_IRQn);
	
		LPC_TMR32B0->TCR = 2;			   // reset timer
	//	LPC_TMR32B0->TCR = 1;				 // start timer

}

void timer_init1(float Rate)
{
	 float Rate_init;
	 float Rate1;
//	 // Initialize Pin as match register
//	  LPC_IOCON ->PIO1_5 |=(0x0);
     	LPC_TMR32B1->TCR = 2;			   // reset timer
	
			LPC_TMR32B1->TCR = 2;
		// Enables clock for 32 bit timer (CT32B0)  Page 34
		LPC_SYSCON->SYSAHBCLKCTRL |= (1<<10);  
		
		// timer mode not counter mode  (Page 367)
		LPC_TMR32B1->CTCR = 0; 
	
		// make the TC trigger an interrupt when MR0 matches with TC
		LPC_TMR32B1->MCR = 0x3;   // page 368   (1 1000)
	   
	     Rate1=(1/Rate);
	    Rate_init =(48000000/Rate1);   // MR0 Value that causes the number of Hz. was 48000000
		// place value in match register MR0
		LPC_TMR32B1->MR0 =Rate_init; // random value for an example
	
		// enable the timer interrupt
		NVIC_EnableIRQ(TIMER_32_1_IRQn);
	
		LPC_TMR32B1->TCR = 2;			   // reset timer
	//	LPC_TMR32B0->TCR = 1;				 // start timer

}
