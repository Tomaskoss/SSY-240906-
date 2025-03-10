/*
 * I2C.c
 *
 * Created: 3/10/2025 12:50:08
 *  Author: Student
 */ 

// i2c.c - Implementace I2C knihovny
#include "i2c.h"

void i2cInit(void) {
	// Nastaveni SCL na 400kHz
	TWSR = 0x00;
	TWBR = 0x02;
	// Povolit TWI
	TWCR = (1 << TWEN);
}

void i2cStart(void) {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0);
}

void i2cStop(void) {
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void i2cWrite(uint8_t byte) {
	TWDR = byte;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0);
}

uint8_t i2cReadACK(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while ((TWCR & (1 << TWINT)) == 0);
	return TWDR;
}

uint8_t i2cReadNACK(void) {
	TWCR = (1 << TWINT) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0);
	return TWDR;
}

uint8_t i2cGetStatus(void) {
	return TWSR & 0xF8;
}
