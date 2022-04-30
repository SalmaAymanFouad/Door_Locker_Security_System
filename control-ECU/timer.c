/*******************************************************************************
 *                                                                             *
 * Module: TIMER                                                               *
 *                                                                             *
 * Description: Source file for the TIMER driver for AVR                       *
 *                                                                             *
 * Author: Salma Ayman                                                         *
 *                                                                             *
 *******************************************************************************/

#include "timer.h"
#include "avr/io.h"
#include "common_macros.h"
#include <avr/interrupt.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)();
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the TIMER device by:
 * 1. Non PWM mode FOC0=1
 * 2. Set the mood
 * 3. Normal Mode COM00=0 & COM01=0
 * 4. Set the Prescaler
 * 5. Set the initial value of the timer
 * 6. Set the compare value of the timer if working in compare mode
 * 7. Enable Timer0 Compare Interrupt if in compare mode
 * 8. Enable Timer0 Overflow Interrupt if in normal mode
 */

void Timer0_Init(const TIMER0_ConfigType * Config_Ptr){
	TCNT0 = Config_Ptr->initial_value;

	SET_BIT(TCCR0,FOC0);//this bit is for either normal or compare mode

	TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->prescaler); //set and clear prescaler bits as desired

	if(Config_Ptr->mode == COMPARE_MODE){

		SET_BIT(TCCR0,WGM01);
		OCR0 = Config_Ptr->compare_value;
		SET_BIT(TIMSK,OCIE0);
	}
	else{
		SET_BIT(TIMSK,TOIE0);
	}
}

/*
 * Description: Function to disable the Timer0 to stop the TIMER Driver
 */
void Timer0_DeInit(void){

	/*Clear all Timer0 registers*/
	TCCR0=0;
	TCNT0=0;
	OCR0=0;
	TIMSK=0;
}

/*
 * Description: Function to set the Call Back function address.
 */
void Timer0_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}
