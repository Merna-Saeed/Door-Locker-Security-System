/******************************************************************************
 *
 * File Name: MC_ECU.h
 *
 * Description: door locker security system micro2 header file
 *
 * Author: Merna Saeed Habib
 *
 *******************************************************************************/

#ifndef MC2_H_
#define MC2_H_

#include"std_types.h"
/*******************************************************************************
 *                              Definitions                                    *
 *******************************************************************************/

#define MC2_READY 0x01
#define PASS_LENGTH 5 /*password composed of 5 numbers*/
/*address of the first place in the eeprom memory which save password on it */
#define PASSWORD_ADDRESS 0x006
/*address of the 12c when use it as a slave*/
#define TWI_SLAVE_ADDRESS 0b0010001

/*definitions to change the state variable */
#define MATCH 0xF1
#define ENTER_NEW_PASSWORD 0xF2
#define MAIN_OPTIONS 0xF3
#define ENTER_YOUR_PASSWORD 0xF4
#define DOOR_OPENING 0xF5
#define DOOR_IS_OPENED 0xF6
#define MISMATCH 0xF7
#define DOOR_CLOSING 0xF8
#define WARNING 0xF9

/*flag to make sure before starting the motor and buzzer functions */
#define MOTOR_FLAG 0xC0
#define BUZZER_FLAG 0xC1

/*variable to store the current state to check on it in while (1)*/
volatile uint8 g_state;

uint8 g_recivedIntialPassword[PASS_LENGTH];
uint8 g_recivedReenterdPassword[PASS_LENGTH];
uint8 g_Password[PASS_LENGTH];
uint8 mismatchCounter=0;/*variable to count the times of false enters*/

/*flags to check if the timer started or not for motor or buzzer */
uint8 g_init_timer_motor=1;
uint8 g_init_timer_buzzer=1;

/*counter for numbers of seconds*/
uint8 g_secondes_counter=0;

/*variable to get the selected option if '+' or '-'*/
uint8 KEY;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * function take the first password sending by the uart from micro1 and store it in an array
 * then take the second array in the same way
 * compared two password if they are the same store it in the eeprom
 * if wrong put the g_state =mismatch to ask the user to set the password once again
 */
uint8   checkNewPassword(void);
/*
 * Description:
 * function take the password sending by the uart from micro1 and store it in an array
 * then get the password stored in the eeprom
 * check if the two password are the same
 * execute the action which user has been chosen
 * if wrong ask him to enter the password again
 * if wrong for three times put the state = warning
 */
uint8   checkPassword(void);

/*
 * Description:
 *switch case on the key if it was + put the g_state=DOOR_OPENING
 *if it was - put the g_state=ENTER_NEW_PASSWORD
 */
void    mainOptionsChosen(void);

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

/*
 * Description:
 *for loop looping on the elements of the password's array to store it
 *in the eeprom
 */
void  	savePasswordInEEPROM(uint8 *a_passwordArr);

/*
 * Description:
 *for loop looping on the elements of thearray to get the password
 *from the eeprom
 */
void    readPasswordFromEEPROM(const uint8 *a_passwordSavedInEeprom);

void    DoorMechanism(void);

/*
 * Description:
 * first check of the timer init flag
 * enable buzzer for one minute if the seconds counter is less than 60
 * else disable buzzer deinit timer send the g_state by uart
 */
void  	buzzerAlarm(void);

/*
 * Description:
 * the set call back function for the timer increment the counter every second
 */
void    timerCounter();
#endif /* MC2_H_ */
