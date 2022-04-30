/*******************************************************************************
 *                                                                             *
 * Module: I2C                                                                 *
 *                                                                             *
 * Description: Source file for the TWI(I2C) AVR driver                        *
 *                                                                             *
 * Author: Salma Ayman                                                         *
 *                                                                             *
 *******************************************************************************/

#include "common_macros.h"
#include <avr/io.h>
#include "twi.h"

void TWI_init(const TWI_ConfigType * Config_Ptr)
{

	uint8 i,j;
	uint8 bit_reg=0;
	uint8 power=0;
	for(i=0; i<4; i++){
		if(i==0){
			power =1;
		}
		for(j=1; j<i; j++){
			power *=4;
		}
		bit_reg = (uint8)(((F_CPU/Config_Ptr->bit_rate)-16)/(2*power));
		if (bit_reg>0){
			TWBR = bit_reg;
			TWSR = i;
			break;
		}
	}


    /* Two Wire Bus address my address if any master device want to call me (used in case this MC is a slave device)
       General Call Recognition: Off */
	TWAR = ((Config_Ptr->master_address)<<1); // my address

	TWCR = (1<<TWEN); /* enable TWI */
}

void TWI_start(void)
{
    /*
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

void TWI_stop(void)
{
    /*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void TWI_writeByte(uint8 data)
{
    /* Put data On TWI data Register */
    TWDR = data;
    /*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

uint8 TWI_readByteWithACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}

uint8 TWI_readByteWithNACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}

uint8 TWI_getStatus(void)
{
    uint8 status;
    /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    status = TWSR & 0xF8;
    return status;
}


