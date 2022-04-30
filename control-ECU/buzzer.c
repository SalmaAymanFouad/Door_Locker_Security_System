/*******************************************************************************
 *                                                                             *
 * Module: Buzzer                                                              *
 *                                                                             *
 * Description: Buzzer driver                                                  *
 *                                                                             *
 * Author: Salma Ayman                                                         *
 *                                                                             *
 *******************************************************************************/


#include "gpio.h"
#include "buzzer.h"
/*
 * Description :
 * Set the direction of pin of the buzzer connected to the MC
 */
void Buzzer_init(void){
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
}

/*
 * Description :
 * Set the pin to be on
 */
void Buzzer_on(void){

}
/*
 * Description :
 * Clear the pin to be on
 */
void Buzzer_off(void){
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);

}
