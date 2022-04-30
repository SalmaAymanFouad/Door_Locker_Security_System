/*******************************************************************************
 *                                                                             *
 * Module: motor                                                               *
 *                                                                             *
 * Description: motor driver                                                   *
 *                                                                             *
 * Author: Salma Ayman                                                         *
 *                                                                             *
 *******************************************************************************/

#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* H-bridge HW Ports and Pins Ids */
#define H_BRIDGE_EN_PORT_ID                   PORTD_ID
#define H_BRIDGE_EN_PIN_ID                    PIN7_ID

#define H_BRIDGE_PIN1_PORT_ID                 PORTD_ID
#define H_BRIDGE_PIN1_PIN_ID                  PIN5_ID

#define H_BRIDGE_PIN2_PORT_ID                 PORTD_ID
#define H_BRIDGE_PIN2_PIN_ID                  PIN6_ID
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum{
	STOP,CLK_WISE,A_CLK_WISE
}DcMotor_State;


/*
 * Description :
 * Function responsible for DC motor initialization.
 */
void DcMotor_Init(void);


/*
 * Description :
 * Function responsible for DC motor rotation direction and speed.
 */
void DcMotor_Rotate(DcMotor_State state);
#endif /* MOTOR_H_ */


