/******************************************************************************
 *
 * Module: DC motor
 *
 * File Name: dc_motor.h
 *
 * Description: header file for a dc motor driver
 *
 * Author: Merna Saeed Habib
 *
 *******************************************************************************/


#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* DC_motor HW Ports and Pins Ids */
#define MOTOR1_IN_PORT_ID      PORTD_ID
#define MOTOR1_IN1_PIN_ID      PIN6_ID
#define MOTOR1_IN2_PIN_ID	   PIN7_ID
#define MOTOR1_ENABLE_PIN_ID

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	STOP,CLOCKWISE,ANTI_CLOCKWISE
}DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Function Name:DcMotor_Init
 * Description: initialize the DC motor driver selected pins in the AVR
 * 				to output pins and keep the motor off at start
 */
void DcMotor_Init(void);

/* Function Name:DcMotor_Rotate
 * description: update the motor state and speed according to its selected configuration structure
 * Arguments:the members of the config_type structure state and speed
 *
 */

void DcMotor_Rotate(DcMotor_State a_state);

#endif /* DC_MOTOR_H_ */
