/******************************************************************************
 *
 * File Name: HMI_ECU.c
 *
 * Description: human machine interface (micro-controller one) source file
 *
 * Author: Merna Saeed Habib
 *
 *******************************************************************************/
#include"HMI_ECU.h"
#include"lcd.h"
#include"uart.h"
#include"keypad.h"
#include<util/delay.h>

/******************************************************************************
*								global variables                               *
********************************************************************************/
/*set the dynamic configurations for the uart driver*/
Uart_ConfigType g_UartHMI_ECU ={_8_BITS,_1_BIT,DISABLED,9600};


/******************************************************************************
 *								Main                                          *
 *******************************************************************************/
int main()
{
	LCD_init();/*initiate the LCD driver*/
	UART_init(&g_UartHMI_ECU);/*initiate the UART driver*/
	/*send byte from micro1 to micro 2 to check if UART in micro 2 ready or not*/
	UART_sendByte(MC2_READY);
	setPassword();/*set the password for the first time*/

/******************************************************************************
*								super loop                                    *
********************************************************************************/
	while(1)
	{
		g_currentState=UART_recieveByte();/*receive the current state from micro2 */
		if(g_currentState==MAIN_OPTIONS) /*display the main option for the user on the lcd*/
		{
			mainOptions(); /*go to the menu to choose specified action */
		}
		else if (g_currentState==ENTER_YOUR_PASSWORD)
		{
			enterPassword(); /*get the password from user */
		}
		else if(g_currentState==MATCH) /*if match then the password is right */
		{
			LCD_clearScreen();
			LCD_moveCursor(0,0);
			LCD_displayString("HEY");
			_delay_ms(500);
		}
		else if(g_currentState==MISMATCH) /*if mismatch then the password is wrong */
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"WRONG PASSWORD");
			_delay_ms(2000);
			enterPassword(); /*enter password once again*/
		}

		else if(g_currentState==DOOR_OPENING) /*display on the lcd when the door is opening*/
		{
			LCD_clearScreen();
			LCD_moveCursor(0,0);
			LCD_displayString("Opening...");
		}
		else if(g_currentState==DOOR_IS_OPENED) /*display on the lcd when the door is opened*/
		{
			LCD_clearScreen();
			LCD_moveCursor(0,0);
			LCD_displayString("Door opened");
		}
		else if(g_currentState==DOOR_CLOSING) /*display on the lcd when the door is closing*/
		{
			LCD_clearScreen();
			LCD_moveCursor(0,0);
			LCD_displayString("Closing...");
		}
		else if(g_currentState==WARNING) /*when enter the password wrong three times*/
		{
			LCD_clearScreen();
			LCD_moveCursor(0,0);
			LCD_displayString("CATCH A THIEF!!!");
		}
		else if(g_currentState==ENTER_NEW_PASSWORD) /*when the user choose to change the password*/
		{
			setPassword();
		}
	}
}
/* Description:
 * the first function to call
 * using for set a new password from the user
 *  by getting 5 numbers from him using the keypad two times
 */
void setPassword(void)
{
	/*display on the screen*/
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Enter pass: ");
	LCD_moveCursor(1,0);
	/*store the first time password in an array (g_intialPassword) */
	for(uint8 i =0;i<PASS_LENGTH+1;i++)
	{
		g_intialPassword[i]=KEYPAD_getPressedKey(); /*get the pressed key from the keypad*/
		if(i<PASS_LENGTH)/*to print star 5 times only*/
		{
			LCD_displayCharacter(PRINTED_STAR); /*display * for each entered character*/
			UART_sendByte(g_intialPassword[i]);/*send the first password to micro2*/
		}
		_delay_ms(3000); /* Press time */
	}
	/*display on the screen for the second time */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Reenter pass: ");
	LCD_moveCursor(1,0);
	/*store the second time password in an array (g_reenterdPassword) */
	for(uint8 i =0;i<PASS_LENGTH+1;i++)
	{
		g_reenterdPassword[i]=KEYPAD_getPressedKey();/*get the pressed key from the keypad*/
		if(i<PASS_LENGTH)/*to print star 5 times only*/
		{
			LCD_displayCharacter(PRINTED_STAR); /*display * for each entered character*/
			UART_sendByte(g_reenterdPassword[i]);/*send the second password to micro2*/
		}
		_delay_ms(3000); /* Press time */
	}
	g_currentState=UART_recieveByte();/*receive the current state from micro2 */
	if(MATCH==g_currentState)
	{/*if match terminate the function*/
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"PASSWORD IS SET  ");
		_delay_ms(3000);
	}
	else if(MISMATCH==g_currentState)
	{/*if mismatch call the same function to try again*/
		setPassword();
	}
}

/* Description:
 * function to display on the screen the menu for the user to choose
 * and get its option and send it to the other micro-controller
 */
void mainOptions(void)
{
	uint8 key; /*variable to store the selected option + or - */
	/*display on the screen*/
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"+: Open Door");
	LCD_displayStringRowColumn(1,0,"-: Change Pass");
	key=KEYPAD_getPressedKey();
	_delay_ms(3000); /*press time*/
	if(!(key=='+'||key=='-')) /*choose + or - only*/
	{
		mainOptions(); /*display the main options once again*/
	}
	else
	{
		UART_sendByte(key);/*if the password is correct then send the character to MC2*/
	}
}

/* Description:
 * function to enter the password before doing any action store it in an array
 * send it by UART to the another micro-controller to check if the password correct or wrong
 */
void enterPassword()
{
	/*display on the lcd screen*/
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"ENTER YOUR PASSWORD");
	LCD_moveCursor(1,0);
	/*store the second time password in an array (g_Password) */
	for(uint8 i =0;i<PASS_LENGTH;i++)
	{
		/*get the password from the user and save it in an array*/
		g_Password[i]=KEYPAD_getPressedKey();
		if(i<PASS_LENGTH)
		{
			LCD_displayCharacter(PRINTED_STAR); /*display * for each entered character*/
			UART_sendByte(g_Password[i]);/*send the password to micro2*/
		}
		_delay_ms(3000); /* Press time */
	}
}

