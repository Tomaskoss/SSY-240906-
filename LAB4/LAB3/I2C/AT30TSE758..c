/*
 * AT30TSE758.c
 *
 * Created: 3/10/2025 13:19:44
 *  Author: Student
 */ 
#include "I2C.h"
#include "AT30TSE758.h"

float AT30TSE758_read_temperature() {
    uint8_t temp_msb, temp_lsb;
    int16_t raw_temp;

    i2cStart();
    i2cWrite((AT30TSE758_ADDR << 1) | 0); // Zápis na I2C
    i2cWrite(0x00);  // Registr teploty
    i2cStart();
    i2cWrite((AT30TSE758_ADDR << 1) | 1); // ?tení z I2C
    temp_msb = i2cReadACK();
    temp_lsb = i2cReadNACK();
    i2cStop();

    raw_temp = (temp_msb << 8) | temp_lsb;
    return raw_temp / 256.0; // P?evod na °C
    }