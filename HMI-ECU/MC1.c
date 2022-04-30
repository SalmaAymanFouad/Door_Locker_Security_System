/******************************************************************************
 *                                                                            *
 * Module: Application                                                        *
 *                                                                            *
 * Description: microcontroller 1 used to take the password of door locker    *
 *              system and display some info on the LCD                       *
 *                                                                            *
 * Author: Salma Ayman                                                        *
 *                                                                            *
 ******************************************************************************/

#include "keypad.h"
#include "lcd.h"
#include "uart.h"
#include "timer.h"
#include "util/delay.h"
#include "gpio.h"
#include "avr/io.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define PASSWORD_SIZE         5
#define MC2_READY            0x10
#define MC1_READY            0x30
#define SEND_PASSWORD        0x20
#define PASSWORD_WRONG       0x50
#define PASSWORD_STATUS      0x60
#define CHANGE_PASS          0X90
#define USE_MOTOR            0x70
#define USE_BUZZER           0x80

#define MOVING_MOTOR_SEC      15
#define HOLDING_MOTOR_SEC      3

#define NUMBER_OF_OVERFLOWS_PER_SECOND 32
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint8 g_firstPassword[PASSWORD_SIZE];/* Array to store the first entered password*/
uint8 g_secondPassword[PASSWORD_SIZE];/* Array to store the second entered password*/
uint8 g_password[PASSWORD_SIZE];
uint8 g_countMismatch = 0;
uint8 g_operation = 0;

uint8 g_tick_motor = 0;
uint8 g_seconds_motor = 0;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 * Ask the user who use the system for the first time to enter a password of 5 numbers
 * and then re-enter that password and display the password on the screen in the '*' form
 */
void enter_password_first_time(void);

/*
 * Description:
 * take the first entered password and check it with the second attempt, if both are matched
 * then send the password to MC2 and display operation list. In the mismatch case, go back to
 * display "Enter a password"
 */
void check_password(void);

/*
 * Description:
 * display available options and return the user's choice
 */
void display_main_options(void);

/*
 * Description:
 * After choosing an operation he will be asked again about his password and send it to MC2 to
 * check it with the old one stored in EEPROM
 */
void checking_password_after_operation(void);

/*
 * Description:
 * check whether the sent password to MC2 is correct or wrong
 */
void mismatch_3_or_less(void);


void motor_CW_15sec(void);


void motor_holding_3sec(void);



void motor_ACW_15sec(void);

int main(void){

	/*Configuration of UART and initialization*/
	Usart_ConfigType USART_Config = {NO_PARITY, ONE_BIT, BIT_8, 9600};
	UART_init(&USART_Config);

	/* Wait until MC2 is ready to receive the string */
	//while(UART_recieveByte() != MC2_READY){}

	LCD_init();

	//GPIO_setupPinDirection(PORTC_ID, PIN5_ID, PIN_OUTPUT);

	while(1){
		enter_password_first_time();
		check_password();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                   *
 *******************************************************************************/
/*
 * Description:
 * take the first entered password and check it with the second attempt, if both are matched
 * then send the password to MC2 and display operation list. In the mismatch case, go back to
 * display "Enter a password"
 */
void check_password(void){
	uint8 i=0;
	uint8 unmatched = 0;
	UART_sendByte(CHANGE_PASS);
	for(i=0; i<PASSWORD_SIZE; i++){
		if(g_firstPassword[i] == g_secondPassword[i]){

		}
		else{
			unmatched++;
		}
	}
	//while(UART_recieveByte() != MC2_READY){}
	if(unmatched == 0){

		/*UART_sendByte(MC1_READY);
		while(UART_recieveByte() != MC2_READY){}*/
		LCD_clearScreen();
		LCD_displayString("Matched!");

		UART_sendByte(MC1_READY);
		while(UART_recieveByte() != MC2_READY){}
		for(i=0; i<PASSWORD_SIZE; i++){

			UART_sendByte(g_firstPassword[i]);/* Send each number of the password alone to MC2 */
			_delay_ms(3000);
		}
		LCD_clearScreen();
		unmatched = 0;
		display_main_options();
		checking_password_after_operation();

	}
	else{
		LCD_clearScreen();
		unmatched = 0;
		enter_password_first_time();
		check_password();
	}

}


/*
 * Description:
 * Ask the user who use the system for the first time to enter a password of 5 numbers
 * and then re-enter that password and display the password on the screen in the '*' form
 */
void enter_password_first_time(void){
	uint8 i=0;
	uint8 key;
	LCD_clearScreen();
	LCD_displayString("Enter Password");
	LCD_moveCursor(1,0);

	for(i=0; i<PASSWORD_SIZE; i++){

		key = KEYPAD_getPressedKey();/* read the pressed key on the keypad*/
		_delay_ms(2000);
		//UART_sendByte(key);

		if((key <= 9) && (key >= 0))
		{
			g_firstPassword[i] = key;/* store every pressed key to store the whole password*/
			LCD_displayCharacter('*'); /* display '*' on the LCD */
			//LCD_intgerToString(key); /* display the pressed keypad switch */
		}

	}
	key = 0;
	LCD_clearScreen();
	LCD_displayString("ReEnter Password");
	LCD_moveCursor(1,0);
	for(i=0; i<PASSWORD_SIZE; i++){

		key = KEYPAD_getPressedKey();/* read the pressed key on the keypad*/
		_delay_ms(2000);


		if((key <= 9) && (key >= 0))
		{
			g_secondPassword[i] = key;/* store every pressed key to store the whole re entered password*/
			LCD_displayCharacter('*');/* display '*' on the LCD */
			//LCD_intgerToString(key); /* display the pressed keypad switch */
		}
	}
}
/*
 * Description:
 * display available options and return the user's choice
 */
void display_main_options(void){

	uint8 key;
	while(1){
		LCD_clearScreen();
		LCD_displayString("+: open the door");
		LCD_moveCursor(1,0);
		LCD_displayString("-: change pass  ");

		key = KEYPAD_getPressedKey();/* read the pressed key on the keypad*/

		_delay_ms(1000);
		LCD_moveCursor(1,15);
		LCD_displayCharacter(key);
		_delay_ms(1000);
		LCD_clearScreen();
		if((key == '+') || (key == '-')){
			g_operation = key;
			checking_password_after_operation();
			break;
		}


	}

}

/*
 * Description:
 * After choosing an operation he will be asked again about his password and send it to MC2 to
 * check it with the old one stored in EEPROM
 */
void checking_password_after_operation(void){
	uint8 i=0;
	uint8 key;
	LCD_clearScreen();
	LCD_displayString("Enter Password:");
	LCD_moveCursor(1,0);

	for(i=0; i<PASSWORD_SIZE; i++){

		key = KEYPAD_getPressedKey();/* read the pressed key on the keypad*/
		_delay_ms(2000);


		if((key <= 9) && (key >= 0))
		{
			g_password[i] = key;/* store every pressed key to store the whole password*/
			LCD_displayCharacter('*'); /* display '*' on the LCD */

			//LCD_intgerToString(key); /* display the pressed keypad switch */
		}
	}
	UART_sendByte(SEND_PASSWORD);
	while(UART_recieveByte() != MC2_READY){}
	for(i=0; i<PASSWORD_SIZE; i++){
		UART_sendByte(g_password[i]);/* Send each number of the password alone to MC2 */
		_delay_ms(3000);
	}


	mismatch_3_or_less();
}
/*
 * Description:
 * check whether the sent password to MC2 is correct or wrong
 */
void mismatch_3_or_less(void){

	while(UART_recieveByte() != PASSWORD_STATUS){}

	if(g_operation == '+'){
		TIMER0_ConfigType Timer_Config = {OVF_MODE,CLK_1024, 0, 0};
		SREG  |= (1<<7);    //Enable global interrupts in MC by setting the I-Bit.
		LCD_clearScreen();
		UART_sendByte(USE_MOTOR);
		g_countMismatch=0;

		Timer0_setCallBack(motor_CW_15sec);
		LCD_displayString("Door is opening ");

		Timer0_Init(&Timer_Config);

	}
	else if(g_operation == '-'){


		LCD_clearScreen();
		g_countMismatch=0;
		enter_password_first_time();
		UART_sendByte(CHANGE_PASS);
		_delay_ms(3000);
		check_password();
	}
	/*else if (UART_recieveByte() == PASSWORD_WRONG ){
		g_countMismatch++;
		if(g_countMismatch == 3){
			g_countMismatch=0;
			UART_sendByte(USE_BUZZER );

		}

		checking_password_after_operation();*/

		/*while(UART_recieveByte() != MC2_ORDER){}

		UART_sendByte(MC2_ORDER);
		g_countMismatch++;
		checking_password_after_operation();
		if(g_countMismatch==3){
			g_countMismatch=0;
			UART_sendByte(USE_BUZZER);
		}

		else if(g_operation == '-'){
			g_countMismatch=0;
			enter_password_first_time();
			check_password();
		}*/
	//}

}

void motor_CW_15sec(void){
	/*TIMER0_ConfigType Timer_Config = {OVF_MODE,CLK_1024, 0, 0};
	Timer0_Init(&Timer_Config);*/

	g_tick_motor++;
		if(g_tick_motor == NUMBER_OF_OVERFLOWS_PER_SECOND)
		{
			/*GPIO_writePin(PORTC_ID, PIN5_ID, LOGIC_HIGH);
			_delay_ms(2000);
			GPIO_writePin(PORTC_ID, PIN5_ID, LOGIC_LOW);*/
			g_tick_motor = 0;
			g_seconds_motor++;
		}

		if(g_seconds_motor == MOVING_MOTOR_SEC){
			LCD_clearScreen();
			g_seconds_motor=0;
			Timer0_setCallBack(motor_holding_3sec);
			LCD_displayString("Door is Open!");


		}
}

void motor_holding_3sec(void){

	g_tick_motor++;
	if(g_tick_motor == NUMBER_OF_OVERFLOWS_PER_SECOND)
	{
		/*GPIO_writePin(PORTC_ID, PIN5_ID, LOGIC_HIGH);
		_delay_ms(2000);
		GPIO_writePin(PORTC_ID, PIN5_ID, LOGIC_LOW);*/
		g_tick_motor = 0;
		g_seconds_motor++;
	}

	if(g_seconds_motor == HOLDING_MOTOR_SEC){
		LCD_clearScreen();
		g_seconds_motor=0;
		Timer0_setCallBack(motor_ACW_15sec);
		LCD_displayString("Door is Closing");

	}
}
void motor_ACW_15sec(void){
	/*TIMER0_ConfigType Timer_Config = {OVF_MODE,CLK_1024, 0, 0};
	Timer0_Init(&Timer_Config);*/

	g_tick_motor++;
	if(g_tick_motor == NUMBER_OF_OVERFLOWS_PER_SECOND)
	{
		/*GPIO_writePin(PORTA_ID, PIN5_ID, LOGIC_HIGH);
			_delay_ms(2000);
			GPIO_writePin(PORTA_ID, PIN5_ID, LOGIC_LOW);*/
		g_tick_motor = 0;
		g_seconds_motor++;
	}

	if(g_seconds_motor == MOVING_MOTOR_SEC){

		g_seconds_motor=0;
		Timer0_DeInit();
		LCD_clearScreen();
		display_main_options();

	}
}

