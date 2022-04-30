/*******************************************************************************
 *                                                                             *
 * Module: Buzzer                                                              *
 *                                                                             *
 * Description: Buzzer driver                                                  *
 *                                                                             *
 * Author: Salma Ayman                                                         *
 *                                                                             *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define BUZZER_PORT_ID                   PORTC_ID
#define BUZZER_PIN_ID                    PIN2_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Set the direction of pin of the buzzer connected to the MC
 */
void Buzzer_init(void);

/*
 * Description :
 * Set the pin to be on
 */
void Buzzer_on(void);
/*
 * Description :
 * Clear the pin to be on
 */
void Buzzer_off(void);
#endif /* BUZZER_H_ */
