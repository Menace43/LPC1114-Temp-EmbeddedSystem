/*

Written by: Steven Kamm
11/11/2018

This program has 2 main purposes
	-- Demonstrate how to setup a timer with a timer-interrupt
	-- Demonstrate how to configure UART and use it to send/receive strings of data
	
For the timer configuration, reference timer.c

For the UART configuration, reference UART.c


Currently, the timer is setup to use a 32 bit timers, 32B0
When this timer matches with MR0, it will trigger an interrupt. 
The interrupt is currently empty, with the exception of clearing the flag and reseting the timer


UART INFORMATION

	Connect a terminal window to your UART device
	
	To transmit from terminal to microcontroller:
		-- type in what you would like, and hit the Enter key
		
	When the Enter key is hit, the UART receiving handler will convert the char array
	   into a "string" using memcpy. This will allow for the use of strcmp() to compare 
		 to any string you would like. Strcmp() is used in main() to decide what to do based
		 on the user input
		 
	 To transmit a string from microcontroller to terminal:
		 -- use the sprintf() function to place your wanted string into a char array[]
		 -- then call the UART_send_string(string) function to send each character one-by-one
		 
		
*/





#include <system_LPC11xx.h>
#include <LPC11xx.h>
#include "timer.h"
#include "UART.h"
#include <stdio.h> // for sprintf()
#include <string.h> // for memcpy()
#include <stdlib.h> // for atoi()
#include "ADC.h"
#include "LCD_driver.h"
#include "keypad_scanner.h"

#define BUFFER_SIZE 20


//--Global Variable for ADC
  // My internal take helps display the particular temp to the UART after every 10,000 samples
	int counter=0;
	float average=0.0;
	int average_Srate;
	char degree_symbol = 0xDF;  // 0xDF is the degree symbol on Hitatchi HD44780 driver
	int LED;  //Flag to turn on LED
	
	//Global variable for keypad 
	//int Sfrequency;
	int Sfrequency_num=0;    // tracks the number
	float Srate_num=0;
	int Average_num=0;
  int interrupt = 0;
	int timer_interrupt;
	int change_me=0;      // Global variable  for changing from F to C
	char pressed_letter;
  char key;
  char keyvalue;
  int increase=0;
	int number1;int number2;int number3;
	int count_num=0;
	int sample=0;
  int count_D=0;
	int entered_number=0;           // Display in the UART the Enter Sampling frequency or Average length
	float entered_number_float=0;    // Display in the UART the Enter display period
	char print_letter_flag;           // Flag that Display input from UART terminal on the LCD
	
	int Sfrequency_Displayfrom_UART=0;
	float Srate_Displayfrom_UART=0.0;
	int Average_Num_Displayfrom_UART=0;
	int double_srate;
	float received_number2=0.0;
	     
	
	
	float ADC_value=0.0;
	float temp;
  float running_sum=0.0;
	int delay;
  int count=0;
	int  average_1;

int ADC_value_Srate=0;
int running_sum_Srate=0;
int temp_Srate;
int counter2=0;   //Counts for Timer handler 1

// global variables
char transmit_string_buffer[BUFFER_SIZE]; // no strings in c language, so use a char array
char UART_received_buffer[BUFFER_SIZE];
char received_string[BUFFER_SIZE];
char UART_received_flag = 0;
char UART_received_index = 0; 
char UART_received_byte = 0;




/******** Timer Interrupt Handler ***********************************/

// if you choose to use an interrupt based keypad scanner, use the function below as the interrupt handler
void PIOINT0_IRQHandler(void)  // interrupt handler for Port 0  (from LPC11xx.h)
{ 
	
	// this interrupt handler will be entered hundereds of times each time a button is pressed
	// so use "interrupt" as a flag so that the scan only happens once
	if(!interrupt)  
	{
		 
			 int delay;
		   int char_num;
	    	NVIC_DisableIRQ(EINT0_IRQn);
			  NVIC_ClearPendingIRQ(EINT0_IRQn);
	     // groundRows();

			  key = scanner();
		    debounce();
        keyvalue =key; 
	

				if((keyvalue>=0x30&& keyvalue<=0x39)||(keyvalue>=0x41&& keyvalue<=0x44)||(keyvalue =='*')||(keyvalue=='#'))
			  {
					
					//Set the Port0 to be output 
					//prepare for LCD display
			   LPC_GPIO0 -> DIR  |=(0x9E);
			   LPC_GPIO1-> DIR  |=(0x100);
			
				
		    //LCD_print_char(keyvalue);
					
					
					//Conversion on the numbers entered//
					if(keyvalue>=0x30&& keyvalue<=0x39)
					{
						if(pressed_letter=='A')
						{
						  LCD_print_char(keyvalue);
						  char_num=(keyvalue-0x30);
						  
						 

						switch(char_num)
						{
							 case 0x0:   
                 Sfrequency_num *=10;
							   break;
				       case 0x1:
								 Sfrequency_num *=10;
								 Sfrequency_num +=1;
							   break;
							 case 0x2:
								 Sfrequency_num *=10;
								 Sfrequency_num +=2;
							   break;
							 case 0x3:
								Sfrequency_num *=10;
								 Sfrequency_num +=3;
							   break;
							 case 0x4:
								Sfrequency_num *=10;
								 Sfrequency_num +=4;
							   break;
							 case 0x5:
								Sfrequency_num *=10;
								 Sfrequency_num +=5;
							   break;
							 case 0x6:
								 Sfrequency_num *=10;
								 Sfrequency_num +=6;
							   break;
							 case 0x7:
								 Sfrequency_num *=10;
								 Sfrequency_num +=7;
							   break;
							 case 0x8:
								 Sfrequency_num *=10;
								 Sfrequency_num +=8;
							   break;
							 case 0x9:
								 Sfrequency_num *=10;
								 Sfrequency_num +=9;
				          break;
				    }
						
					}
						else if(pressed_letter=='B')   //Sample Rate (from 0.1 to 10 seconds)
						{
							 LCD_print_char(keyvalue);
							 char_num=(keyvalue-0x30);
							 count_num++;
						  
							
						    	switch(char_num)
						   { 
							   case 0x0:   
                 Srate_num *=10;
							   break;
				       case 0x1:
								 Srate_num *=10;
								 Srate_num +=1;
							   break;
							 case 0x2:
								 Srate_num *=10;
								 Srate_num +=2;
							   break;
							 case 0x3:
								Srate_num *=10;
								 Srate_num +=3;
							   break;
							 case 0x4:
								Srate_num *=10;
								 Srate_num +=4;
							   break;
							 case 0x5:
								Srate_num *=10;
								 Srate_num +=5;
							   break;
							 case 0x6:
								 Srate_num *=10;
								 Srate_num +=6;
							   break;
							 case 0x7:
								 Srate_num *=10;
								 Srate_num +=7;
							   break;
							 case 0x8:
								 Srate_num *=10;
								 Srate_num +=8;
							   break;
							 case 0x9:
								 Srate_num *=10;
								 Srate_num +=9;
				          break;
				      }
						    
						}
						
						else if(pressed_letter=='C')   //Average_num (from 1 to 10000)
						{
							 LCD_print_char(keyvalue);
							 char_num=(keyvalue-0x30);
							// count_num++;
						  
							
						    	switch(char_num)
						   { 
							   case 0x0:   
                 Average_num *=10;
							   break;
				       case 0x1:
								 Average_num *=10;
								 Average_num +=1;
							   break;
							 case 0x2:
								 Average_num *=10;
								 Average_num +=2;
							   break;
							 case 0x3:
								Average_num *=10;
								 Average_num +=3;
							   break;
							 case 0x4:
								Average_num *=10;
								 Average_num +=4;
							   break;
							 case 0x5:
								Average_num *=10;
								 Average_num +=5;
							   break;
							 case 0x6:
								 Average_num *=10;
								 Average_num +=6;
							   break;
							 case 0x7:
								 Average_num *=10;
								 Average_num +=7;
							   break;
							 case 0x8:
								 Average_num *=10;
								 Average_num +=8;
							   break;
							 case 0x9:
								 Average_num *=10;
								 Average_num +=9;
				          break;
				      }
						    
						}
			 }
				else if (keyvalue=='#')
				{
             // -- Entering Each value
					  if (pressed_letter=='A')
						{
							  timer_init(Sfrequency_num);
								//Send the number also to the UART
						     entered_number=Sfrequency_num;
								sprintf(transmit_string_buffer, ": %d\n\r",entered_number);
			          UART_send_string(transmit_string_buffer);
                LCD_clear();								
						 }
				      else if (pressed_letter=='B')
				    	{
						        if (count_num==3)
					         {
						           Srate_num *=0.1;
										   double_srate =Srate_num;
							         if(Srate_num>=0.1&& Srate_num<=10)
							           {
												  timer_init1(Srate_num);    //Send the value to the 32B1MR0 but dont start
								          count_num=0;
												
												//Send the Number entered to UART terminal
												 entered_number_float=Srate_num;
								        sprintf(transmit_string_buffer, ": %d\n\r",double_srate);
			                  UART_send_string(transmit_string_buffer);
												   LCD_clear();		
								       }
										else {LCD_print_string("Not in range");}
						
					       }
							 }
				
				   	else if (pressed_letter=='C')
					  {
						      if (Average_num>=0x1 && Average_num<=0x10000)
						      {
							       Average_num=Average_num;
										
										//Send the Number entered to UART terminal
											entered_number_float=Srate_num;
								       sprintf(transmit_string_buffer, ": %d\n\r",entered_number);
			                 UART_send_string(transmit_string_buffer);
										
									//	pressed_letter='D';
										 LCD_clear();	
						      } 		
                 else {LCD_print_string("Not in Range");}									
				       }
						 }
				  else if (keyvalue=='A')
				   { 
				    	//-- Enter the Sampling frequency 
					     pressed_letter='A';
					     Sfrequency_num =0; // Reset Sampling frequency
					     count_D=0;
					    // count_D=1;
					       LCD_init();
					       Enter_Sfrequency();
						   //prepare for keypad detection
			          LPC_GPIO0 -> DIR   &=(0x0);  // set direction of port 0 as input
			          LPC_GPIO1 -> DIR  |= (0x17);  // set direction of Port 1 as output	   
					   
           //change_me= 'A';
           sprintf(transmit_string_buffer, "Enter sampling frequency \n\r");
				   UART_send_string(transmit_string_buffer);					
					
				}
				
				else if (keyvalue=='B')
				{ 
					//--Enter Sampling period
					   pressed_letter='B';
					
					//Display Sampling frequency on the LCD
					   LCD_init();
					  LCD_print_string("Enter DisplayRate");
					  LCD_command(0xC0,0);
					 
					   Srate_num=0.0;  //Reset the Display rate
					   count_num=0;
					
					//Display Sampling Rate on Terminal
					 sprintf(transmit_string_buffer, "Enter Display Rate \n\r");
				   UART_send_string(transmit_string_buffer);		
					
					  //prepare for keypad detection
			          LPC_GPIO0 -> DIR   &=(0x0);  // set direction of port 0 as input
			          LPC_GPIO1 -> DIR  |= (0x17);  // set direction of Port 1 as output	
					
					  
            // count_D=2;					
					   //  LCD_clear();
					  // LCD_command(0xC0,0);
					         	
				}
				
					else if (keyvalue=='C')
				{ 
					//--Enter Sampling period
					   pressed_letter='C';
					
					//Display Average Number on the LCD
					  LCD_init();
					  LCD_print_string("Enter Average Number");
					  LCD_command(0xC0,0);
					
					//Display Sampling Rate on Terminal
					 sprintf(transmit_string_buffer, "Enter Average Number \n\r");
				   UART_send_string(transmit_string_buffer);		
					
					  //prepare for keypad detection
			          LPC_GPIO0 -> DIR   &=(0x0);  // set direction of port 0 as input
			          LPC_GPIO1 -> DIR  |= (0x17);  // set direction of Port 1 as output	
					
					    Average_num=0;  //Reset the Average Num
					    //count_D=3;
					
				}
				
				else if (keyvalue=='D')
				{ 
					          count_D +=1;
										pressed_letter='D';
										LCD_clear();		
										
						      	//Now start the timer
						    	LPC_TMR32B0->TCR = 1;				 // start timer  for the Sfrequency
								  LPC_TMR32B1->TCR = 1;				 // start timer    for DisplayRate		
					
					
					  //prepare for keypad detection
			          LPC_GPIO0 -> DIR   &=(0x0);  // set direction of port 0 as input
			          LPC_GPIO1 -> DIR  |= (0x17);  // set direction of Port 1 as output	
						}
						
			
				
				else if (keyvalue=='*')	
				{  //Stop timer first
					  
					  timer_init(0);
					  timer_init1(0);
					  //Main Menu
					  LCD_clear();
					  Instruction_Display();
					
					// clear the counter so that when a new value is entered it starts again 
					  counter=0;
					  running_sum=0;
				}
			
				//
				increase++;
			  
					 LPC_GPIO0 -> DIR   =(0x0);  // set direction of port 0 as input
			}
			//prepare for keypad detection
			 LPC_GPIO0 -> DIR   &=(0x0);  // set direction of port 0 as input
			 LPC_GPIO1 -> DIR  |= (0x17);  // set direction of Port 1 as output
			interrupt = 0;	
		   NVIC_EnableIRQ(EINT0_IRQn);   //
			 NVIC_ClearPendingIRQ(EINT0_IRQn); // Clear pending interrupt
				

		// Reground rows 
	     groundRows();
	}

	
	// IMPORTANT: don't forget to reset interrupt in main
	
}



/*******************************************************************
******** Timer Interrupt Handler ***********************************
*******************************************************************/
void TIMER32_0_IRQHandler(void)
{
	// in this example, the interrupt does nothing but clear the flag and reset TC
	// put a break point on line 27 and run the code. If the interrupt is set up correctly, it should stop at the breakpoint
	// clear the interrupt flag
	 LPC_TMR32B0->IR |= 1;  // do this first
	  

				
	  // sample = Sfrequency_num*Srate_num;	
   //		Prepare LCD
   //		LPC_GPIO0-> DIR |=(0x9E);
   //		LPC_GPIO1-> DIR |=(0x100);
		  
		
 	    	if(change_me==0)    // Pressing A gives the voltage in F
				{
					ADC_value = (LPC_ADC->DR[0] >> 6) & (0x3ff);
					temp = 3.33 / (1023 / ADC_value) *100; // The temperature is now in celsius
		      temp = temp * 1.8 + 32; // converted into farenheit
					counter = counter + 1;
		      running_sum = running_sum + temp;
				}
				
	      	else if(change_me==1)    // Pressing A gives the voltage in F
				{
					ADC_value = (LPC_ADC->DR[0] >> 6) & (0x3ff);
					temp = 3.33 / (1023 / ADC_value) *100; // The temperature is now in celsius
		      //temp = temp * 1.8 + 32; // converted into farenheit
					counter = counter + 1;
		      running_sum = running_sum + temp;
				}
				
			
				
				  if(counter == Average_num)
	        {  
			         average = running_sum / Average_num;
						    LPC_GPIO0-> DIR |=(0x9E);
          		  LPC_GPIO1-> DIR |=(0x100);
			         running_sum = 0;
						   counter=0;
			         //count = 0;
						
//					      LCD_clear();			
//		            LCD_print_number(average);
//			          LCD_print_char(degree_symbol);  // print degree symbol
						
		          if (change_me ==0)
			        {
				     
						 		// Prepare for Keypad after input	
		              	LPC_GPIO0->DIR &= (0x00);  
			               LPC_GPIO1->DIR |= (0x017);
	              }
							}
	
			
		
			else{}
			
				// Prepare for Keypad after input	
			LPC_GPIO0->DIR &= (0x00);  
			LPC_GPIO1->DIR |= (0x017);
		   
	// if you have modified the value in the match register (MR0), then you should reset the timer
	// If you have not, then the option to reset the timer is up to you 
	LPC_TMR32B0->TCR = 2;			   // reset timer
	LPC_TMR32B0->TCR = 1;				 // start timer
	
		 NVIC_DisableIRQ(EINT0_IRQn);
     NVIC_ClearPendingIRQ(EINT0_IRQn);
				
	 	
	   NVIC_EnableIRQ(EINT0_IRQn); 
	   NVIC_ClearPendingIRQ(EINT0_IRQn);
}

void TIMER32_1_IRQHandler(void)
{
	// in this example, the interrupt does nothing but clear the flag and reset TC
	// put a break point on line 27 and run the code. If the interrupt is set up correctly, it should stop at the breakpoint
	// clear the interrupt flag
	 LPC_TMR32B1->IR |= 1;  // do this first
	  
    //This timer Handler just deals with the Sample rate (Number of sample collected per second )
    //We want the Display Rate to Come inside and display the average gotten from the Frequency 
	  //if there is no value at the beginning just take the current temperature and display it 
	
	    if (average>0)
			{
				//Put in LCD Mode First
				   LPC_GPIO0-> DIR |=(0x9E);
           LPC_GPIO1-> DIR |=(0x100);
				
				//Display on the LCD
				   LCD_clear();			
				   average_1 =average*100;
				   LCD_print_number2(average_1);
	         //LCD_print_number(average);
		       LCD_print_char(degree_symbol);  // print degree symbol
				   LCD_print_string("F");
				
				//Display in the terminal 
				   if (change_me ==0)
			     {
				   	sprintf(transmit_string_buffer, "Temp from Frequency in Fahrenhiet:");
			     	UART_send_string(transmit_string_buffer);
						sprintf(transmit_string_buffer, " %d.\n\r", average_1);
						transmit_string_buffer[3]=0x2E;
			     	UART_send_string(transmit_string_buffer);
						//counter=0;
			      }
		        else if (change_me ==1)
		     	{
					   sprintf(transmit_string_buffer, "Degress in Celcuis %.1f.\n\r",average);
				      UART_send_string(transmit_string_buffer);
						  //counter=0;
					}
				
			}
			
			else 
			{
				//just take the current temperature from the Measured
				
				  if(change_me==0)    // Pressing A gives the voltage in F
				{
					ADC_value_Srate = (LPC_ADC->DR[0] >> 6) & (0x3ff);
					temp_Srate = 3.33 / (1023 / ADC_value) *100; // The temperature is now in celsius
		      temp_Srate = temp * 1.8 + 32; // converted into farenheit
					counter2 = counter2 + 1;
		      running_sum_Srate = running_sum_Srate + temp_Srate;
				}
				
	      	 else if(change_me==1)    // Pressing A gives the voltage in F
				  {
					ADC_value_Srate = (LPC_ADC->DR[0] >> 6) & (0x3ff);
					temp_Srate = 3.33 / (1023 / ADC_value) *100; // The temperature is now in celsius
		      //temp = temp * 1.8 + 32; // converted into farenheit
					counter2 = counter2 + 1;
		      running_sum_Srate = running_sum_Srate + temp;
				 }
					
					//Put in LCD Mode First
				     LPC_GPIO0-> DIR |=(0x9E);
          	LPC_GPIO1-> DIR |=(0x100);
				 
				   //Display the Temprature on LCD
					   LCD_clear();			
		         LCD_print_number(temp_Srate);
			       LCD_print_char(degree_symbol);  // print degree symbol
				     LCD_print_string("F");
				 
				 //Display on terminal

             sprintf(transmit_string_buffer, "Temp from Frequency in Fahrenhiet:");
			     	UART_send_string(transmit_string_buffer);
						sprintf(transmit_string_buffer, " %d.\n\r", temp_Srate);
						transmit_string_buffer[4]=0x2E;
			     	UART_send_string(transmit_string_buffer);
				 
				    // sprintf(transmit_string_buffer, "Temp from Rate in Fahrenhiet: %d.\n\r", temp_Srate);
			      // UART_send_string(transmit_string_buffer);
						counter2=0;
			}
				
				 //Blink the LCD to tell the Display Rate
			
				    if (LED==1)    //LPC_GPIO1-> DATA ==(0xEBF)
		           {
		          	LPC_GPIO1-> DATA &=~(0x20); // Make low
		            LED=23;
		            }
		
		          else if (LED==23)    //LPC_GPIO1-> DATA ==(0xE3F
		         {
		            	LPC_GPIO1-> DATA |=(0x20);   // Make High
			            LED=1;
		         }  
			
//            if (counter2==Srate_num)
//						{
//			         average_Srate = running_sum /Srate_num;
//						    LPC_GPIO0-> DIR |=(0x9E);
//          		  LPC_GPIO1-> DIR |=(0x100);
//				
//			          running_sum = 0;
//			     
//					      LCD_clear();			
//		            LCD_print_number(average_Srate);
//			          LCD_print_char(degree_symbol);  // print degree symbol
//				        LCD_print_string("F");
//		       
//				      sprintf(transmit_string_buffer, "Temp from Rate in Fahrenhiet: %d.\n\r", average_Srate);
//			      	UART_send_string(transmit_string_buffer);
//						  counter2=0;
//			      }
			
				// Prepare for Keypad after input	
			LPC_GPIO0->DIR &= (0x00);  
			LPC_GPIO1->DIR |= (0x017);
		   
	// if you have modified the value in the match register (MR0), then you should reset the timer
	// If you have not, then the option to reset the timer is up to you 
	LPC_TMR32B1->TCR = 2;			   // reset timer
	LPC_TMR32B1->TCR = 1;				 // start timer
	
		 NVIC_DisableIRQ(EINT0_IRQn);
     NVIC_ClearPendingIRQ(EINT0_IRQn);
				
	 	
	   NVIC_EnableIRQ(EINT0_IRQn); 
	   NVIC_ClearPendingIRQ(EINT0_IRQn);
}



/*******************************************************************
******** UART Receiving Interrupt Handler **************************
*******************************************************************/
// For the receiving handler, I have implemented it so that in your teminal window, 
//   type all the characters you want in your message, then hit "Enter" on keyboard
// When the "Enter" char is received (0xD0), the UART received handler will set the 
//    UART_received_flag, and in the main() while(1) loop, the logic will begin
void UART_IRQHandler(void)
{
	
	UART_received_byte = LPC_UART -> RBR; // read in the byte
	
	if(UART_received_byte == 0x0D) // if "Enter" key was read in, set the flag since all data is received
	{
		UART_received_flag = 1; 
		
		// now use memcpy() function to convert the char array into a "string" (included in string.h)
		// this is usefull for comparing the string as shown in main()
		memcpy(received_string, UART_received_buffer, UART_received_index);
		received_string[UART_received_index] = '\0'; // place null character at end of string 
	}
	else // place the char into the string
	{
		UART_received_buffer[UART_received_index] = UART_received_byte;
		UART_received_index++; // increment the index by 1
	}
		
}
	

void Instruction_Display(void)
{
	int m;
	  //Initial Display
		LCD_print_string("ECE 323 F2020");
		LCD_command(0xC0,0);  //Move cursor to the next line
		LCD_print_string("Adeyemo Joel");
	  //initial Display in Terminal
	sprintf(transmit_string_buffer, "ECE 323 F2020. \n\r");
	UART_send_string(transmit_string_buffer);
	sprintf(transmit_string_buffer, "Adeyemo Joel.\n\r");
	UART_send_string(transmit_string_buffer);
	
	 for (m=0;m<22000000;m++);  //delay of 5 seconds
	 LCD_clear();
	
	
	
	//After 5 Second this is the display Instruction
	  LCD_print_string("A:Sampling frequency");
		LCD_command(0xC0,0);  //Move cursor to the next line
	 LCD_print_string("B: Display period");
	  LCD_command(0xC0,0);
		
		//Display in Terminal
	sprintf(transmit_string_buffer, "A: Sampling Frequency. \n\r");
	UART_send_string(transmit_string_buffer);
	sprintf(transmit_string_buffer, "B: Display period.\n\r");
	UART_send_string(transmit_string_buffer);
	
	
	 //Delay for 2 seconds
	  for (m=0;m<12000000;m++);  
	
	  LCD_clear();
	  LCD_print_string("C: Average Length");
		LCD_command(0xC0,0);  //Move cursor to the next line
	  LCD_print_string("D:Start, *:Return");
	
	sprintf(transmit_string_buffer, "C: Average length. \n\r");
	UART_send_string(transmit_string_buffer);
	sprintf(transmit_string_buffer, "D:Start, *: Return. n\r");
	UART_send_string(transmit_string_buffer);
	
	for (m=0;m<22000000;m++);  //delay of 5 seconds
	LCD_clear();
}

void Instruction_Display_Terminal(void)
{
	//Displays the value from UART the Sampling frequency shown on the UART Terminal
	
					//Disable keypad Detection mode
	 				   NVIC_DisableIRQ(EINT0_IRQn);
            NVIC_ClearPendingIRQ(EINT0_IRQn);

				  //Put in LCD Mode First
				   LPC_GPIO0-> DIR |=(0x9E);
           LPC_GPIO1-> DIR |=(0x100);
	
	int m;
	  //Initial Display
		LCD_print_string("ECE 323 F2020");
		LCD_command(0xC0,0);  //Move cursor to the next line
		LCD_print_string("Adeyemo Joel");
	  //initial Display in Terminal
	sprintf(transmit_string_buffer, "ECE 323 F2020. \n\r");
	UART_send_string(transmit_string_buffer);
	sprintf(transmit_string_buffer, "Adeyemo Joel.\n\r");
	UART_send_string(transmit_string_buffer);
	
	 for (m=0;m<22000000;m++);  //delay of 5 seconds
	 LCD_clear();
	
	
	
	//After 5 Second this is the display Instruction
	  LCD_print_string("A:Sampling frequency");
		LCD_command(0xC0,0);  //Move cursor to the next line
	 LCD_print_string("B: Display period");
	  LCD_command(0xC0,0);
		
		//Display in Terminal
	sprintf(transmit_string_buffer, "A: Sampling Frequency. \n\r");
	UART_send_string(transmit_string_buffer);
	sprintf(transmit_string_buffer, "B: Display period.\n\r");
	UART_send_string(transmit_string_buffer);
	
	
	 //Delay for 2 seconds
	  for (m=0;m<12000000;m++);  
	
	  LCD_clear();
	  LCD_print_string("C: Average Length");
		LCD_command(0xC0,0);  //Move cursor to the next line
	  LCD_print_string("D:Start, *:Return");
	
	sprintf(transmit_string_buffer, "C: Average length \n\r");
	UART_send_string(transmit_string_buffer);
	sprintf(transmit_string_buffer, "D:Start, *: Return n\r");
	UART_send_string(transmit_string_buffer);
	
	for (m=0;m<22000000;m++);  //delay of 5 seconds
	LCD_clear();
	
	
	 // Enable Keypad Detection
					 // Prepare for Keypad after input	
		       	LPC_GPIO0->DIR &= (0x00);  
			       LPC_GPIO1->DIR |= (0x017);
	 
	        NVIC_DisableIRQ(EINT0_IRQn);
          NVIC_ClearPendingIRQ(EINT0_IRQn);
				
						NVIC_EnableIRQ(EINT0_IRQn); 
	          NVIC_ClearPendingIRQ(EINT0_IRQn);
}

void Display_From_UART_To_LCD(void)
{
		//Displays the value from UART the Sampling frequency shown on the UART Terminal
	
					//Disable keypad Detection mode
	 				   NVIC_DisableIRQ(EINT0_IRQn);
            NVIC_ClearPendingIRQ(EINT0_IRQn);

				  //Put in LCD Mode First
				   LPC_GPIO0-> DIR |=(0x9E);
           LPC_GPIO1-> DIR |=(0x100);
					
					 
   if (print_letter_flag=='A')
	 {
		   LCD_clear();
		  LCD_print_string("Enter Sampling Freq");
		   LCD_command(0xC0,0);   // Cursor to next Line
		 
	 }
	 
	 else if (print_letter_flag=='B')
	 {
		   LCD_clear();
		  LCD_print_string("Enter Display Period");
		  LCD_command(0xC0,0);   // Cursor to next Line
	 }
	  else if (print_letter_flag=='C')
	 {
		  LCD_clear();
		  LCD_print_string("Enter Average Num");
		   LCD_command(0xC0,0);   // Cursor to next Line
		 
	 }
	 
					 // Enable Keypad Detection
					 // Prepare for Keypad after input	
		       	LPC_GPIO0->DIR &= (0x00);  
			       LPC_GPIO1->DIR |= (0x017);
	 
	        NVIC_DisableIRQ(EINT0_IRQn);
          NVIC_ClearPendingIRQ(EINT0_IRQn);
				
						NVIC_EnableIRQ(EINT0_IRQn); 
	          NVIC_ClearPendingIRQ(EINT0_IRQn);
							
	
}

void Display_From_number_UART_To_LCD(void)
{
		//Displays the value from UART the Sampling frequency shown on the UART Terminal
	
					//Disable keypad Detection mode
	 				   NVIC_DisableIRQ(EINT0_IRQn);
            NVIC_ClearPendingIRQ(EINT0_IRQn);

				  //Put in LCD Mode First
				   LPC_GPIO0-> DIR |=(0x9E);
           LPC_GPIO1-> DIR |=(0x100);
					
					 
   if (print_letter_flag=='A')
	 {
		   
		  LCD_command(0xC0,0);   // Cursor to next Line
		  if (Sfrequency_Displayfrom_UART>=0)
			{
				 LCD_print_number(Sfrequency_Displayfrom_UART);
			}
	 }
	 
	 else if (print_letter_flag=='B')
	 {
		  LCD_command(0xC0,0); // Curssor to the next line
		  if(Srate_Displayfrom_UART>0.0)
			{
				 LCD_print_float(Srate_Displayfrom_UART);
			}
	 }
	  else if (print_letter_flag=='C')
	 {
		  LCD_command(0xC0,0); // Curssor to the next line
		   if (Average_Num_Displayfrom_UART>0)
			 {
				  LCD_print_number(Average_Num_Displayfrom_UART);
			 }
	 }
	 
					 // Enable Keypad Detection
					 // Prepare for Keypad after input	
		       	LPC_GPIO0->DIR &= (0x00);  
			       LPC_GPIO1->DIR |= (0x017);
	 
	        NVIC_DisableIRQ(EINT0_IRQn);
          NVIC_ClearPendingIRQ(EINT0_IRQn);
				
						NVIC_EnableIRQ(EINT0_IRQn); 
	          NVIC_ClearPendingIRQ(EINT0_IRQn);
							
	
}



int main()
{
	
	NVIC_DisableIRQ(EINT0_IRQn);
  NVIC_ClearPendingIRQ(EINT0_IRQn);
	
	NVIC_EnableIRQ(EINT0_IRQn); 
	NVIC_ClearPendingIRQ(EINT0_IRQn);
	
	// local variables
	char c = 'B';
	int number = 15; 
	int received_number = 0;
	change_me=0;
	
	//--keypad Initialization//
	 //LCD_clear();
	int m;
	LCD_init();
	UART_init();
	
	//Displays Initial Instructions
	Instruction_Display();
	
	keypad_init(); 
	
	//--ADC initialization//
	//LCD_init();
	adc_init();
	
//	int ADC_value;
//	int temp;
//	int running_sum=0;
//	int delay;
//  int count=0;
	int Sfrequency=0;  //default Sampling frequency is 1hz and sampling rate is 1 second (affects the keypad)
	float Srate=0.0;
	int Averagenum=0;
  
	// initializations   
	timer_init(Sfrequency);
	//timer_init(48000);
	//keypad_init();
    UART_init();
	//adc_init();
	LED=1;   // Flag for LED
	
	
	  // Blink LED PIO1_5 on (0010 0000) 
       // Initialize Pin as match register
	     LPC_IOCON ->PIO1_5 |=(0x0);
	     LPC_GPIO1-> DIR |=(0x20);
			 LPC_GPIO1-> DATA |=(0x20);
   
	/***************************************************************************
	**************** UART Function Examples ************************************
	***************************************************************************/
	
	//// to send a char, simply use UART_send_char(char)
//	UART_send_char('A'); // can place char directly
//	UART_send_char('\n'); // new-line
//	UART_send_char('\r'); // carriage_return
//	
//	UART_send_char(c);   // can place char in a variable then pass the variable to the function
//	UART_send_char('\n'); // new-line
//	UART_send_char('\r'); // carriage_return
//	
	// to send a string, use sprintf() to place your string into a char buffer
//	sprintf(transmit_string_buffer, "STRING\n\r");  
//	UART_send_string(transmit_string_buffer);
//	
//	// can also include a variable in your string as follows
//	sprintf(transmit_string_buffer, "variable:%d\n\r", number); // %d is "place holder" for an int
//	UART_send_string(transmit_string_buffer);
	
	
	 // Begining Instructions
	 //For the first 5 seconds
//	int i;
//	
//	 sprintf(transmit_string_buffer, "ECE 323 Adeyemo Joel \n\r");
//	UART_send_string(transmit_string_buffer);
//	sprintf(transmit_string_buffer, "Push * at any time to reset\n\r");
//	UART_send_string(transmit_string_buffer);
//	 
//	 
//	sprintf(transmit_string_buffer, "Enter sampling frequency by push A \n\r");
//	UART_send_string(transmit_string_buffer);
//	sprintf(transmit_string_buffer, "Enter sampling frequency by push B\n\r");
//	UART_send_string(transmit_string_buffer);
//	
	
//	
//	sprintf(transmit_string_buffer, "***********************************\n\r");
//	UART_send_string(transmit_string_buffer);
//	sprintf(transmit_string_buffer, "Constraint For code: \n\r");
//	UART_send_string(transmit_string_buffer);
//	sprintf(transmit_string_buffer, "Input the 'Sample frequency(number of samples per second)' \n\r");
//	UART_send_string(transmit_string_buffer);
// 	sprintf(transmit_string_buffer, "Starting from Hz and in order 0f 10 after words  \n\r");
//	UART_send_string(transmit_string_buffer);
//	sprintf(transmit_string_buffer, "'Sample Rate(Display per second)' would be shown after every  sample taken \n\r");
//	UART_send_string(transmit_string_buffer);
//   sprintf(transmit_string_buffer, "***********************************\n\r");
//	UART_send_string(transmit_string_buffer);
	
	//timer_init(48000000);

	while(1)
	{

		
		while(UART_received_flag) // loop until data is received
		{
			// to compare your received string, use strcmp() function
			// This function returns 0 when the strings are equal
			
			if(strcmp(received_string, "test") == 0) // in terminal, type "test" then hit "Enter" key
			{
				sprintf(transmit_string_buffer, "'Test' received\n\r");
				UART_send_string(transmit_string_buffer);
			}
			else if(strcmp(received_string, "good") == 0)
			{
				sprintf(transmit_string_buffer, "'good' received\n\r");
				UART_send_string(transmit_string_buffer);					
			}
			else if(strcmp(received_string,"T")==0)    // Pressing end  stops the timer.90
				{
					
				  sprintf(transmit_string_buffer, "Stopped Timer\n\r");
				  UART_send_string(transmit_string_buffer);			
				  timer_init(0);   // stops the timer
					timer_init1(0);  // stops timer for Srate
				}
		  else if(strcmp(received_string,"A")==0)    // Pressing A enter sample frequency
				{
					  sprintf(transmit_string_buffer, "Enter Sampling frequency \n\r");
				    UART_send_string(transmit_string_buffer);	
				    pressed_letter ='A'; // Acts like a flag
					  print_letter_flag='A';
            Display_From_UART_To_LCD();					
					 
				}
				else if(strcmp(received_string,"B")==0)    // Pressing B enter sample rate 
				{
					  sprintf(transmit_string_buffer, "Enter Display Period \n\r");
				    UART_send_string(transmit_string_buffer);	
				    pressed_letter ='B'; // Acts like a flag
					  print_letter_flag='B';
            Display_From_UART_To_LCD();	 // Display also on the LCD
				}
				else if(strcmp(received_string,"C")==0)    // Pressing C Average Length
				{
					 sprintf(transmit_string_buffer, "Enter Average Number \n\r");
				    UART_send_string(transmit_string_buffer);	
				    pressed_letter ='C'; // Acts like a flag
					  print_letter_flag='C';
            Display_From_UART_To_LCD();		//Display also on the LCD
				}
					else if(strcmp(received_string,"Graph")==0)    //Start Graphingh
				{
				    sprintf(transmit_string_buffer, "& Graphing \n\r");
				   UART_send_string(transmit_string_buffer);
				}
	    else if(strcmp(received_string,"D")==0)    // Start the Whole sampling process
				{
					// Start the timer for both the Srate and Sfrequency 
				  	//Now start the timer
						    	LPC_TMR32B0->TCR = 1;				 // start timer  for the Sfrequency
								  LPC_TMR32B1->TCR = 1;				 // start timer    for SRate
					   sprintf(transmit_string_buffer, "'D' received\n\r");
			      	UART_send_string(transmit_string_buffer);
				} 
		 else if(strcmp(received_string,"*")==0)    // Going back to Main Menu
				{
					//Stop timer first
					  timer_init(0);
					  timer_init1(0);
					
				    //Main Menu
					Instruction_Display_Terminal();
				} 
			else // enter in a number and hit enter, and we will convert the number from a string to an integer
			{
				received_number = atoi(received_string); // atoi() will convert from a string to an integer
				received_number2= atof(received_string);
				
	        if (pressed_letter=='B')
				 {
					   print_letter_flag='B';
		         Srate= received_number2;
					   timer_init1(Srate);                           // Send to the MR0
					   
					 
					  
					   Srate_num=received_number2;
					   Srate_Displayfrom_UART=received_number2;		
             sprintf(transmit_string_buffer,": %f\n\r",received_number2);
             UART_send_string(transmit_string_buffer);					 
					   Display_From_number_UART_To_LCD();                // Display the number on the LCD
					   received_number2=0.0;
									
				 }

				else if (pressed_letter=='A')
				 {
					   print_letter_flag='A';  
					  received_number = received_number;
		        Sfrequency= received_number;
					   timer_init(Sfrequency);	 //Send to Match Register for the Frequency
					 
					 
					   Sfrequency_Displayfrom_UART= received_number;
					  sprintf(transmit_string_buffer, ": %d\n\r",Sfrequency_Displayfrom_UART);
				    UART_send_string(transmit_string_buffer);	
					   Display_From_number_UART_To_LCD();                         //Display the number on the LCD
					  received_number=0;
					   
				 }
				 
				 else if (pressed_letter=='C')
				 {
					   print_letter_flag='C';
					  received_number = received_number;
					  Average_num= received_number;
					 
					  Average_Num_Displayfrom_UART=received_number;
					  sprintf(transmit_string_buffer,": %d\n\r",received_number);
					 UART_send_string(transmit_string_buffer);	
					 Display_From_number_UART_To_LCD();
					 received_number=0;
					   
					  
				 }
				 else if (pressed_letter=='D')
				 {
					       // Start the timer for both the Srate and Sfrequency 
						      	//Now start the timer
						    	LPC_TMR32B0->TCR = 1;				 // start timer  for the Sfrequency
								  LPC_TMR32B1->TCR = 1;				 // start timer    for SRate
					   sprintf(transmit_string_buffer, "'D' received\n\r");
			     	UART_send_string(transmit_string_buffer);
				 }
			else
				{
//				// for now, I am just doing a simple equation with the number
//				// do whatever you want to with the nubmer since its just a normal integer variable
//				 received_number = received_number + 1;
//		     Sfrequency= received_number;
//				 timer_init(Sfrequency);
				
				// can now send the updated number back to the terminal for fun
				sprintf(transmit_string_buffer, "%d\n\r", received_number);
				UART_send_string(transmit_string_buffer);
				
				sprintf(transmit_string_buffer, "%d\n\r",Sfrequency);
				UART_send_string(transmit_string_buffer);
				
			//	sprintf(transmit_string_buffer, " Temp is:%d\n\r",average);
			//	UART_send_string(transmit_string_buffer);
	   		}
		
			}
			
			// be sure to clear the flag and reset the UART_received_index
			UART_received_flag = 0;
			UART_received_index = 0;
		}
		
	
	} // end infinite loop	
	return 0;
}

