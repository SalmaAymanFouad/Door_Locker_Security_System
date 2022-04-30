/*******************************************************************************
 *                                                                             *
 * Module: TIMER                                                               *
 *                                                                             *
 * Description: TIMER driver for AVR                                           *
 *                                                                             *
 * Author: Salma Ayman                                                         *
 *                                                                             *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"


/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	OVF_MODE, COMPARE_MODE
}TIMER0_Mode;

typedef enum
{
	NO_CLK, CLK, CLK_8,CLK_64, CLK_256, CLK_1024, EXT_CLK_FALLING_EDGE, EXT_CLK_RISING_EDGE
}TIMER0_Prescaler;

typedef struct
{
	TIMER0_Mode mode;
	TIMER0_Prescaler prescaler;
	uint8 initial_value;
	uint8 compare_value;
}TIMER0_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
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
void Timer0_Init(const TIMER0_ConfigType * Config_Ptr);

/*
 * Description: Function to disable the Timer0 to stop the TIMER Driver
 */
void Timer0_DeInit(void);

/*
 * Description: Function to set the Call Back function address.
 */
void Timer0_setCallBack(void(*a_ptr)(void));

#endif /* TIMER_H_ */
