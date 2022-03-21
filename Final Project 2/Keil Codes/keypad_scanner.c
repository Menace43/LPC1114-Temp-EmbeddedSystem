#include "keypad_scanner.h"
#include "LPC11xx.h"
#include "LCD_driver.h"
#include <stdio.h> // for sprintf()
#include <string.h> // for memcpy()
#include <stdlib.h> // for atoi()
#include "UART.h"




/*
Columns: 
				 P0.1
				 P0.2
				 P0.3
				 P0.4
				 
Rows:
				 P1.0
				 P1.1
				 P1.2
				 P1.4
*/




#define rows 0x17
#define columns 0x1E

// function that will ground all the rows
void groundRows()
{
	LPC_GPIO1->DATA  &= ~(rows);
}



/****************************************************************************************
  This function will set the pins as GPIO, INPUTS/OUTPUTS, and will ground the rows
****************************************************************************************/
void keypad_init(void)
{
	// First, declare the 8 pins as GPIO (LPC_IOCON-> .... )
	  LPC_IOCON-> PIO0_1  |=(0x0);
	  LPC_IOCON-> PIO0_2  |=(0x0);
	  LPC_IOCON-> PIO0_3  |=(0x0);
	  LPC_IOCON-> PIO0_4  |=(0x0);
	  
  	LPC_IOCON-> R_PIO1_0  |=(0x81);
	  LPC_IOCON-> R_PIO1_1  |=(0x81);
	  LPC_IOCON-> R_PIO1_2  |=(0x81);
	  LPC_IOCON-> PIO1_4  |=(0x80);
	
    	
	// Next, set the columns as inputs (NOTE: pins default to input)P0.1,P0.2,P0.3,P0.4 
	   LPC_GPIO0 -> DIR  =(0x0);
	
	// Set the rows as outputs (LPC_GPIO1-> .... )P1.0,P1.1,P1.2,P1.4 
	   LPC_GPIO1->DIR    |=(0x17)  ;  
	
	// Finally, ground the four rows (LPC_GPIO1-> ...)
	    groundRows() ; 

	// If you choose to use an interrupt, set it up here
	//    modify the LPC_GPIO0->IS register to set as a level sensitve interrupt as to bee 0x1E
	   LPC_GPIO0 -> IS |=(0x1E);    
	//    modify the LPC_GPIO0->IEV register to set as active low
	    LPC_GPIO0->IE |= (0x1E);
     LPC_GPIO0->IEV &= ~(0x1E);
			
	
			  groundRows();
				LPC_GPIO0->DATA  = (0x0);
			
		  NVIC_EnableIRQ(EINT0_IRQn);
			 
			
}

char scanner(void)
{
	// perform the scanning proccess here by reading from the columns and 
	//    grounding the various rows
	
	char letterArray[4][4] = {
		{'1','2','3','A'},
		{'4','5','6','B'},
		{'7','8','9','C'},
		{'*','0','#','D'}
	};
	int scanCodes[4] = {0x7,0x13,0x15,0x16};
	int rowd;
			int i;
		for (i=0;i<10000;i++); //Delay
	for (rowd = 0;rowd < 4; rowd++)
	{
		LPC_GPIO1 ->DATA &= ~(0x17);  //ground rows 
		LPC_GPIO1 ->DATA |= (scanCodes[rowd]);  // load the first value of the rows
		// ((LPC_GPIO0 ->DATA>>1)&(0xF))

		switch(((LPC_GPIO0 ->DATA) & (0x1E))>> 1)
		{
			case 0x07 :
			 groundRows();
			
				return letterArray[rowd][0];
			case 0xB:
			 groundRows();
				return letterArray[rowd][1];
			case 0xD :
			  groundRows();
				return letterArray[rowd][2]; 
			case 0x0E:
			  groundRows();
				return letterArray[rowd][3];

		}
	}
	//LPC_GPIO1 ->DATA &= ~(0xF<<1);
	groundRows();
	return 'x';
}
	//return '3';  // an example 

void Enter_Sfrequency(void)
{
	int i;
	LCD_print_string("Enter SampleFreq");
	LCD_command(0xC0,0);
}
	

void Display_screen(void)
{
	int i;
	
		LCD_print_string("ECE 323 F2020");
		LCD_command(0xC0,0);  //Move cursor to the next line
		LCD_print_string("Adeyemo Joel");
	
}


void Display_instruct(void)
{
	int m;
	
	  LCD_print_string("A:Sampling frequency");
		LCD_command(0xC0,0);  //Move cursor to the next line
	 LCD_print_string("B: Display period");
	  LCD_command(0xC0,0);
	
	 //Delay for 2 seconds
	  for (m=0;m<12000000;m++);  
	
	  LCD_clear();
	  LCD_print_string("C: Average Length");
		LCD_command(0xC0,0);  //Move cursor to the next line
	  LCD_print_string("D:Start, *:Return");

	
	//	Display_screen(); 		//Intial Display Note
//	
//	sprintf(transmit_string_buffer, "ECE 323 F2020 \n\r");
//	UART_send_string(transmit_string_buffer);
//	sprintf(transmit_string_buffer, "Adeyemo Joel\n\r");
//	UART_send_string(transmit_string_buffer);
//	
//	for (m=0;m<22000000;m++);  //delay of 5 seconds
//	LCD_clear();
//	
//	
//	sprintf(transmit_string_buffer, "A: Sampling Frequency \n\r");
//	UART_send_string(transmit_string_buffer);
//	sprintf(transmit_string_buffer, "B: Display period\n\r");
//	UART_send_string(transmit_string_buffer);
//	
//	Display_instruct();       //Intial Display Instruction
//	sprintf(transmit_string_buffer, "C: Average length \n\r");
//	UART_send_string(transmit_string_buffer);
//	sprintf(transmit_string_buffer, "D:Start, *: Return n\r");
//	UART_send_string(transmit_string_buffer);
//	
//	for (m=0;m<22000000;m++);  //delay of 5 seconds
//	LCD_clear();
	//Enter_Sfrequency();
}


void debounce(void)
{
  while((((LPC_GPIO0 ->DATA) & (0x1E))>> 1) != 0xF)
		{}
}
