/******************************************************************************
 * Module: Buzzer
 *
 * File Name: buzzer.c
 *
 * Description: source file for the BUZZER  driver
 *
 * Author: Merna Saeed Habib
 *******************************************************************************/

#include "buzzer.h"
#include "gpio.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/* Description :
 * initiating the buzzer driver according to the connection port and pin
 */
void BUZZER_init()
{
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID,LOGIC_LOW);
}

/* Description :
 * enabling the buzzer
 */
void BUZZER_on()
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID,LOGIC_HIGH);
}

/* Description :
 * disabling the buzzer
 */
void BUZZER_off()
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID,LOGIC_LOW);
}
