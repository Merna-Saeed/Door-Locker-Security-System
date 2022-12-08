/******************************************************************************
 * Module: Timer
 *
 * File Name: timer.h
 *
 * Description: header file for the timer driver
 *
 * Author: Merna Saeed Habib
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"

/*******************************************************************************
 *                        Types Declaration                                    *
 *******************************************************************************/
typedef enum
{
	TIMER_0,TIMER_1,TIMER_2
}Timer_ID;
typedef enum
{
	NORMAL,CTC
}Timer_Mode;
typedef enum
{
	NOCLOCK,PRESCALER_1,PRESCALER_8,PRESCALER_64,PRESCALER_256,PRESCALER_1024,EXTERNAL_FALLING,EXTERNAL_RISING
}Timer0_1_clock;

typedef enum
{
	NOCLOCK2,NO_PRESCALER2,PRESCALER2_8,PRESCALER2_32,PRESCALER2_64,PRESCALER2_128,PRESCALER2_256,PRESCALER2_1024
}Timer2_clock;

typedef enum
{
	NORMAL_OC_DISCONNECT,TOGGLE_OC,CLEAR_OC,SET_OC
}Timer_CompareMatchMode;


typedef struct
{
	Timer_ID timer_id;
	Timer_Mode  mode;
	Timer0_1_clock prescaler;
	Timer2_clock prescaler2;
	Timer_CompareMatchMode CompareOutputMode;
	uint32 initialValue;
	uint32 comparelValue;
}Timer_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description: initialize the selected timer driver in the AVR
 *  according to the conifig-type structure
 */
void TIMER_init(const Timer_ConfigType * Config_Ptr);

/*
 * Description:
 * Save the address of the Call back function for timer 0 in a global variable
 */

void TIMER0_setCallBack(void(*a_ptr0)(void));

/*
 * Description:
 * Save the address of the Call back function for timer 1 in a global variable
 */
void TIMER1_setCallBack(void(*a_ptr1)(void));

/*
 * Description:
 * Save the address of the Call back function for timer 2 in a global variable
 */
void TIMER2_setCallBack(void(*a_ptr2)(void));

/*
 * Description:
 * closing the selected timer driver by clearing the clock
 */
void TIMER_deinit();

#endif /* TIMER_H_ */
