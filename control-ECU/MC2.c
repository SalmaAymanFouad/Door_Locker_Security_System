/******************************************************************************
 *                                                                            *
 * Module: Application                                                        *
 *                                                                            *
 * Description: microcontroller 2 used to take the password of door locker    *
 *              system and check if it the same in EEPROM                     *
 *                                                                            *
 * Author: Salma Ayman                                                        *
 *                                                                            *
 ******************************************************************************/

#include "uart.h"
#include "twi.h"
#include "external_eeprom.h"
#include "buzzer.h"
#include "motor.h"
#include "gpio.h"
#include "timer.h"
#include "avr/io.h"
#include "util/delay.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define PASSWORD_SIZE         5
#define ADDRESS_CORRECT_PASSWORD  0x0300

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
//uint8 g_password[PASSWORD_SIZE]; /* array of the correct password saved in the EEPROM*/
//uint8 g_received[PASSWORD_SIZE]; /* array to store any received password that need to be checked*/
uint8 g_countMismatch = 0;
uint8 g_tick_motor = 0;
uint8 g_seconds_motor = 0;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description:
 * Store the correct password in EEPROM
 */
void store_password(void);

/*
 * Description:
 * Get the stored password and receive entered password from MC1 and
 */
void check_password(void);


void motor_CW_15sec(void);


void motor_holding_3sec(void);



void motor_ACW_15sec(void);

int main(void){

	SREG  |= (1<<7);    //Enable global interrupts in MC by setting the I-Bit.
	/*Configuration of UART and initialization*/
	Usart_ConfigType USART_Config = {NO_PARITY, ONE_BIT, BIT_8, 9600};
	UART_init(&USART_Config);

	/* Configuration of TWI/I2C Driver and initialization*/
	TWI_ConfigType Twi_Config = {FAST_MODE,0x01};
	TWI_init(&Twi_Config);

	/* Send to MC1 that MC2 is ready to receive the string */
	//UART_sendByte(MC2_READY);

	Buzzer_init();
	DcMotor_Init();

	/*GPIO_setupPinDirection(PORTC_ID, PIN5_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTA_ID, PIN5_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID, PIN5_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID, PIN7_ID, PIN_OUTPUT);*/
	while(1){
		while(UART_recieveByte() != CHANGE_PASS){}
		store_password();

/*		GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_HIGH);
		_delay_ms(2000);
		GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);

		while(UART_recieveByte() != SEND_PASSWORD ){}

		GPIO_writePin(PORTA_ID, PIN5_ID, LOGIC_HIGH);
		_delay_ms(2000);
		GPIO_writePin(PORTA_ID, PIN5_ID, LOGIC_LOW);

		UART_sendByte(MC2_READY);*/
		while(UART_recieveByte() != SEND_PASSWORD ){}
		check_password();

		//while(UART_recieveByte() != MC2_ORDER){}
	/*	if(UART_recieveByte() != USE_MOTOR){

		}
		else if(UART_recieveByte() != USE_BUZZER){
			Buzzer_on();
			_delay_ms(5000);
			Buzzer_off();
		}*/

	}

}

/*******************************************************************************
 *                      Functions Definitions                                   *
 *******************************************************************************/
/*
 * Description:
 * Store the correct password in EEPROM
 */
void store_password(void){

	uint8 password[PASSWORD_SIZE]; /* array of the correct password saved in the EEPROM*/
	uint8 i=0;

	uint16 address = ADDRESS_CORRECT_PASSWORD;
	//UART_sendByte(MC2_READY);
	while(UART_recieveByte() != MC1_READY){}
	UART_sendByte(MC2_READY);
	//_delay_ms(2000);
	/*GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_HIGH);
	_delay_ms(2000);
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);
	_delay_ms(2000);*/
	for(i=0; i<PASSWORD_SIZE; i++){
		password[i] = UART_recieveByte();
		_delay_ms(3000);
		/*if (letter == 7){
			GPIO_writePin(PORTC_ID, PIN5_ID, LOGIC_HIGH);
			_delay_ms(2000);
			GPIO_writePin(PORTC_ID, PIN5_ID, LOGIC_LOW);
			_delay_ms(2000);
		}*/
	}
	for(i=0; i<PASSWORD_SIZE; i++){
		EEPROM_writeByte(address, password[i]);/* save the password into the memory*/
		_delay_ms(100);
		address ++;
	}
	/*GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_HIGH);
	_delay_ms(2000);
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);*/


}
/*
 * Description:
 * Get the stored password and receive entered password from MC1 and
 */
void check_password(void){



	/*GPIO_writePin(PORTA_ID, PIN5_ID, LOGIC_HIGH);
	_delay_ms(2000);
	GPIO_writePin(PORTA_ID, PIN5_ID, LOGIC_LOW);*/

	UART_sendByte(MC2_READY);
	_delay_ms(2000);
	uint8 received[PASSWORD_SIZE]; /* array to store any received password that need to be checked*/
	uint8 pass_out_eeprom[PASSWORD_SIZE];
	uint8 i=0;
	uint16 address = ADDRESS_CORRECT_PASSWORD;
	uint8 error=0;

	/*receive the entered password*/
	for(i=0; i<PASSWORD_SIZE; i++){
		received[i] = UART_recieveByte();

		_delay_ms(3000);
		/*if (letter == 7){
			GPIO_writePin(PORTC_ID, PIN5_ID, LOGIC_HIGH);
			_delay_ms(2000);
			GPIO_writePin(PORTC_ID, PIN5_ID, LOGIC_LOW);
			_delay_ms(2000);
		}*/
	}

	for(i=0; i<PASSWORD_SIZE; i++){
		EEPROM_readByte(address,pass_out_eeprom+i);
		_delay_ms(100);
		address ++;

		/*if(pass_out_eeprom[i] == 7){
			GPIO_writePin(PORTB_ID, PIN7_ID, LOGIC_HIGH);
			_delay_ms(2000);
			GPIO_writePin(PORTB_ID, PIN7_ID, LOGIC_LOW);
		}*/
	}

	for(i=0; i<PASSWORD_SIZE; i++){
		if(received[i] == pass_out_eeprom[i]){
			GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_HIGH);
			_delay_ms(2000);
			GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);
			_delay_ms(2000);
		}
		else{
			error++;

		}
	}

	UART_sendByte(PASSWORD_STATUS);

	if(error == 0){
		if(UART_recieveByte() == USE_MOTOR){
			/* Send to MC1 that MC2 is ready to receive the string */
			TIMER0_ConfigType Timer_Config = {OVF_MODE,CLK_1024, 0, 0};
			Timer0_setCallBack(motor_CW_15sec);
			DcMotor_Rotate(CLK_WISE);
			Timer0_Init(&Timer_Config);
		}
		/*if(UART_recieveByte() == CHANGE_PASS){

			store_password();
		}*/
	}
	else{
		UART_sendByte(PASSWORD_WRONG);
		if(UART_recieveByte() == USE_BUZZER ){
			Buzzer_on();
			_delay_ms(5000);
			Buzzer_off();
		}

	}
}

void motor_CW_15sec(void){
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
			Timer0_setCallBack(motor_holding_3sec);
			DcMotor_Rotate(STOP);

		}
}

void motor_holding_3sec(void){

	g_tick_motor++;
	if(g_tick_motor == NUMBER_OF_OVERFLOWS_PER_SECOND)
	{
		/*GPIO_writePin(PORTA_ID, PIN5_ID, LOGIC_HIGH);
		_delay_ms(2000);
		GPIO_writePin(PORTA_ID, PIN5_ID, LOGIC_LOW);*/
		g_tick_motor = 0;
		g_seconds_motor++;
	}

	if(g_seconds_motor == HOLDING_MOTOR_SEC){

		g_seconds_motor=0;
		Timer0_setCallBack(motor_ACW_15sec);
		DcMotor_Rotate(A_CLK_WISE);

		//DcMotor_Rotate(STOP);
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
			DcMotor_Rotate(STOP);
			Timer0_DeInit();
			check_password();

		}
}
