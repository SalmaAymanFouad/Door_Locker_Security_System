/*******************************************************************************
 *                                                                             *
 * Module: motor                                                               *
 *                                                                             *
 * Description: motor driver                                                   *
 *                                                                             *
 * Author: Salma Ayman                                                         *
 *                                                                             *
 *******************************************************************************/

#include "motor.h"
#include "gpio.h"

/*
 * Description :
 * Function responsible for DC motor initialization.
 */
void DcMotor_Init(void){
	GPIO_setupPinDirection(H_BRIDGE_PIN1_PORT_ID, H_BRIDGE_PIN1_PIN_ID, PIN_OUTPUT); /*H-bridge input pin1*/
	GPIO_setupPinDirection(H_BRIDGE_PIN2_PORT_ID, H_BRIDGE_PIN2_PIN_ID, PIN_OUTPUT);/*H-bridge input pin2*/
	GPIO_setupPinDirection(H_BRIDGE_EN_PORT_ID, H_BRIDGE_EN_PIN_ID, PIN_OUTPUT);/*H-bridge enable pin*/

	GPIO_writePin(H_BRIDGE_EN_PORT_ID, H_BRIDGE_EN_PIN_ID, LOGIC_LOW);

}


/*
 * Description :
 * Function responsible for DC motor rotation direction and speed.
 */
void DcMotor_Rotate(DcMotor_State state) {

	GPIO_writePin(H_BRIDGE_EN_PORT_ID, H_BRIDGE_EN_PIN_ID, LOGIC_HIGH);

	switch(state){
	case CLK_WISE:
		GPIO_writePin(H_BRIDGE_PIN1_PORT_ID, H_BRIDGE_PIN1_PIN_ID, LOGIC_HIGH);
		GPIO_writePin(H_BRIDGE_PIN2_PORT_ID, H_BRIDGE_PIN2_PIN_ID, LOGIC_LOW);
		break;
	case A_CLK_WISE:
		GPIO_writePin(H_BRIDGE_PIN1_PORT_ID, H_BRIDGE_PIN1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(H_BRIDGE_PIN2_PORT_ID, H_BRIDGE_PIN2_PIN_ID, LOGIC_HIGH);
		break;
	case STOP:
		GPIO_writePin(H_BRIDGE_PIN1_PORT_ID, H_BRIDGE_PIN1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(H_BRIDGE_PIN2_PORT_ID, H_BRIDGE_PIN2_PIN_ID, LOGIC_LOW);
		break;
	}

}

