/******************************************************************************
 *
 * File Name: HMI_ECU.h
 *
 * Description: human machine interface (micro-controller one) header file
 *
 * Author: Merna Saeed Habib
 *
 *******************************************************************************/


#ifndef HMI_ECU_H_
#define HMI_ECU_H_


#include"std_types.h"
/*******************************************************************************
 *                      Definitions & static configuration                     *
 *******************************************************************************/
#define PASS_LENGTH 5
#define PRINTED_STAR '*'
#define MC2_READY 0x01

/*definitions to change the state variable depending on the current action */
#define MATCH 0xF1
#define ENTER_NEW_PASSWORD 0xF2
#define MAIN_OPTIONS 0xF3
#define ENTER_YOUR_PASSWORD 0xF4
#define DOOR_OPENING 0xF5
#define DOOR_IS_OPENED 0xF6
#define MISMATCH 0xF7
#define DOOR_CLOSING 0xF8
#define WARNING 0xF9

/*variable to receive data form MC2 */
uint8 g_currentState;
/* three differents arrays
 * 1- to get the initial password
 * 2- to get the second time of entering the password
 * 3- to get the password saved in the eeprom
 */
uint8 g_intialPassword[PASS_LENGTH+1];
uint8 g_reenterdPassword[PASS_LENGTH+1];
uint8 g_Password[PASS_LENGTH+1];


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/* Description:
 * the first function to call
 * using for set a new password from the user
 *  by getting 5 numbers from him using the keypad two times
 */
void setPassword(void);
/* Description:
 * function to display on the screen the menu for the user to choose
 * and get its option and send it to the other micro-controller
 */
void mainOptions(void);
/* Description:
 * function to enter the password before doing any action store it in an array
 * send it by UART to the another micro-controller to check if the password correct or wrong
 */
void enterPassword();
#endif /* HMI_ECU_H_ */
