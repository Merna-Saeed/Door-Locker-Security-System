/******************************************************************************
 *
 * File Name: MC_ECU.c
 *
 * Description: door locker security system micro2 source file
 *
 * Author: Merna Saeed Habib
 *
 *******************************************************************************/

#include"MC_ECU.h"
#include"dc_motor.h"
#include"buzzer.h"
#include"twi.h"
#include"uart.h"
#include"external_eeprom.h"
#include "timer.h"
#include<util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/******************************************************************************
 *								GLOBAL VARIABLES                              *
 *******************************************************************************/
/*set the dynamic configuration for uart, twi and timer */
Uart_ConfigType g_UartMc2 ={_8_BITS,_1_BIT,DISABLED,9600};
Twi_ConfigType  g_Twi={BITRATE_2,TWI_PRESCALER_1,TWI_SLAVE_ADDRESS};
Timer_ConfigType g_timer={TIMER_1,CTC,PRESCALER_1024,NOCLOCK2,NORMAL_OC_DISCONNECT,0,8388};


/******************************************************************************
 *								Main                                          *
 *******************************************************************************/
int main()
{
	/*enable the global interrupt enable bit*/
	SREG|=(1<<7);
	UART_init(&g_UartMc2);/*initiate the UART driver of micro 2*/
	TWI_init(&g_Twi);/*initiate the i2c driver*/
	BUZZER_init();/*initiate the buzzer driver*/
	DcMotor_Init();/*initiate the DC motor driver*/
	TIMER1_setCallBack(timerCounter);/*set the call-back function to handle the interrupt*/
	while(UART_recieveByte() != MC2_READY){}/*receive the byte and insure the synchronization*/
	checkNewPassword();/*check the send password from micro 1*/


	/******************************************************************************
	 *								super loop                                    *
	 *******************************************************************************/
	while(1)
	{
		/*switch case to execute depending on the current state*/

		switch (g_state)
		{
		case MAIN_OPTIONS:
			/*receive the chosen option from the mc1 */
			KEY=UART_recieveByte();
			g_state=ENTER_YOUR_PASSWORD;
			UART_sendByte(g_state);
			break;
		case ENTER_YOUR_PASSWORD:
			/*check on the entered password*/
			checkPassword();
			break;
		case MATCH:
			/*chose the option using the received key*/
			mainOptionsChosen();
			break;
		case DOOR_OPENING:
			/*open the door(motor)*/
			DoorMechanism();
			break;
		case ENTER_NEW_PASSWORD:
			/*check on the password if the user want to set it */
			checkNewPassword();
			break;
		case WARNING:
			/*password is set wrong for three times*/
			buzzerAlarm();
			break;
			/* mismatch in case of entered wrong password */
		case MISMATCH:
			checkPassword();
			break;
		}
	}
}


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * function take the first password sending by the uart from micro1 and store it in an array
 * then take the second array in the same way
 * compared two password if they are the same store it in the eeprom
 * if wrong put the g_state =mismatch to ask the user to set the password once again
 */
uint8  checkNewPassword()
{
	for(uint8 i=0;i<5;i++)
	{
		/*receiving password for the first time*/
		g_recivedIntialPassword[i]=UART_recieveByte();
	}
	for(uint8 i =0;i<PASS_LENGTH;i++)
	{
		/*receiving password for the second time*/
		g_recivedReenterdPassword[i]=UART_recieveByte();
	}
	for(uint8 i=0;i<PASS_LENGTH;i++)
	{
		/*check if both arrays are the same*/
		if(g_recivedIntialPassword[i]!=g_recivedReenterdPassword[i])
		{
			g_state=MISMATCH;/*wrong password */
			UART_sendByte(g_state);/*send the current state to micro 1*/
			checkNewPassword();/*call same function to try again*/
			return 0;/*terminate the function*/
		}
	}
	savePasswordInEEPROM((uint8*)&g_recivedIntialPassword);/*save the new pass in memmory*/
	g_state=MATCH;/*correct password */
	UART_sendByte(g_state);/*send the current state to micro 1*/
	g_state=MAIN_OPTIONS;/*after setting the password enter the main options*/
	UART_sendByte(g_state);/*send the current state to micro 1*/
	return 0;
}


/*
 * Description:
 * function take the password sending by the uart from micro1 and store it in an array
 * then get the password stored in the eeprom
 * check if the two password are the same
 * execute the action which user has been chosen
 * if wrong ask him to enter the password again
 * if wrong for three times put the state = warning
 */
uint8 checkPassword(void)
{
	uint8 recievedPassword[PASS_LENGTH];/*array to save the entered password in it*/
	uint8 Password[PASS_LENGTH];/*array to get the current password in it from eeprom*/
	for(uint8 j=0;j<PASS_LENGTH;j++)
	{
		/*receiving password */
		recievedPassword[j]=UART_recieveByte();/*storing the entered password*/
	}
	readPasswordFromEEPROM((uint8*)&Password);/*read password from eeprom to compare it with entered one*/

	for(uint8 j=0;j<PASS_LENGTH;j++)
	{/*compare the two arrays*/
		if(recievedPassword[j]==Password[j])
		{
			continue;/*dont complete the current iteration of the for loop*/
		}
		else if(recievedPassword[j]!=Password[j])
		{/*if they are not the same increment the counter by 1*/
			if(mismatchCounter<2)
			{
				mismatchCounter++; /*increment the flag of the wrong password */
				g_state=MISMATCH; /*wrong password */
				UART_sendByte(g_state);/*send the mismatch flag to MC1_mainOptions*/
				return 0;
			}
			else if(mismatchCounter==2)
			{
				/*if the password was entered wrong for 3 times*/
				g_state=WARNING;
				UART_sendByte(g_state);
				return 0;
			}
		}
	}
	g_state=MATCH;
	UART_sendByte(g_state);/*send the match flag to MC1_mainOptions*/
	return 0;
}

/*
 * Description:
 *for loop looping on the elements of the password's array to store it
 *in the eeprom
 */
void  savePasswordInEEPROM(uint8 *a_passwordArr)
{
	uint8 counter;
	for(counter=0 ; counter < PASS_LENGTH ; counter++)
	{
		EEPROM_writeByte(PASSWORD_ADDRESS+counter,a_passwordArr[counter]);
		_delay_ms(100);
	}
}

/*
 * Description:
 *for loop looping on the elements of the array to get the password
 *from the eeprom
 */
void  readPasswordFromEEPROM(const uint8 *a_passwordSavedInEeprom)
{
	uint8 counter;
	for(counter=0 ; counter < PASS_LENGTH ; counter++)
	{
		EEPROM_readByte(PASSWORD_ADDRESS+counter,a_passwordSavedInEeprom+counter);
		_delay_ms(100);
	}
}

/*
 * Description:
 *switch case on the key if it was + put the g_state=DOOR_OPENING
 *if it was - put the g_state=ENTER_NEW_PASSWORD
 */
void  mainOptionsChosen(void)
{
	switch(KEY)
	{
	case '+':
		g_state=DOOR_OPENING;
		UART_sendByte(g_state);/*send the DOOR_OPENING flag to mainOptions*/
		break;
	case'-':

		g_state=ENTER_NEW_PASSWORD;
		UART_sendByte(g_state);/*send the ENTER_NEW_PASSWORD flag to mainOptions*/
		break;
	}
}

/*
 * Description:
 * first check of the timer init flag
 * then rotate the motor clockwise for 15 seconds and send the g_state
 * for the other mc
 * stop the motor for 3 seconds and send the g_state
 * for the other mc
 * then rotate the motor anti clockwise for 15 seconds and send the g_state
 * for the other mc
 */

void  DoorMechanism(void)
{
	if(g_init_timer_motor==1)
	{
		TIMER_init(&g_timer); //Timer starts
		g_init_timer_motor=0;
	}
	/*
		 * state that opening
		 */
	DcMotor_Rotate(CLOCKWISE); // motor rotates clockwise (opens door)
	/*
	 * this while loop makes sure that this state is entered once at a time
	 * for 15 seconds
	 */
	while(g_secondes_counter <=15);

	/*
	 * state that stops motor
	 */
	DcMotor_Rotate(STOP); //stop motor
	g_state = DOOR_IS_OPENED;
	UART_sendByte(g_state);/*send the DOOR_IS_OPENED flag to mainOptions*/
	/*
	 * state = door is opened
	 * so in the while(1) loop the program will keep entering this state
	 * until it changes, so this while loop prevents program
	 * from entering the state more than once
	 */
	while(g_secondes_counter <=18);/*15+3 seconds*/

	/*
	 * state that closes the door
	 */
	DcMotor_Rotate(ANTI_CLOCKWISE);
	g_state = DOOR_CLOSING;
	UART_sendByte(g_state);/*send the match flag to MC1_mainOptions*/
	/*
	 * state = DOOR_CLOSING
	 * so in the while(1) loop the program will keep entering this state
	 * until it changes, so this while loop prevents program
	 * from entering the state more than once
	 */
	while(g_secondes_counter <=33);
	DcMotor_Rotate(STOP); //stop motor
	g_state = MAIN_OPTIONS;
	UART_sendByte(g_state);/*send the MAIN_OPTIONS flag to mainOptions*/

	/*deinit the timer and put the seconds counter equal zero and set the flag*/
	TIMER_deinit(&g_timer);
	g_secondes_counter=0;
	g_init_timer_motor=1;
}

/*
 * Description:
 * first check of the timer init flag
 * enable buzzer for one minute if the seconds counter is less than 60
 * else disable buzzer deinit timer send the g_state by uart
 */
void buzzerAlarm()
{
	/*check on the timer flag */
	if(g_init_timer_buzzer== 1 )
	{
		TIMER_init(&g_timer); //Timer starts
		g_init_timer_buzzer=0;
	}
	/*enable buzzer for one minute */
	if(g_secondes_counter <=60)
	{
		BUZZER_on();
	}
	else
	{
		/*set the flag, deinit the timer ,clear counter , send the current state and clear the mismatch counter*/
		g_init_timer_buzzer=1;
		TIMER_deinit(&g_timer);
		g_secondes_counter=0;
		g_state = MAIN_OPTIONS;
		UART_sendByte(g_state);
		mismatchCounter=0;
	}
}


/*
 * Description:
 * the set call back function for the timer increment the counter every second
 */
void timerCounter()
{
	g_secondes_counter++;
}


