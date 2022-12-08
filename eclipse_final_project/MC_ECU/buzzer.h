/******************************************************************************
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the BUZZER  driver
 *
 * Author: Merna Saeed Habib
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"
/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

#define BUZZER_PORT_ID  PORTC_ID
#define BUZZER_PIN_ID	PIN7_ID


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/* Description :
 * initiating the buzzer driver according to the connection port and pin
 */
void BUZZER_init();

/* Description :
 * enabling the buzzer
 */
void BUZZER_on();

/* Description :
 * disabling the buzzer
 */
void BUZZER_off();

#endif /* BUZZER_H_ */
