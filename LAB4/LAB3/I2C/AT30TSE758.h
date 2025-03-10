/*
 * AT30TSE758.h
 *
 * Created: 3/10/2025 13:18:24
 *  Author: Student
 */

#ifndef AT30TSE758_H_
#define AT30TSE758_H_

#include <stdint.h>

#define AT30TSE758_ADDR 0b1001011  // Adresa senzoru na I2C

float AT30TSE758_read_temperature();  // Prototyp funkce

#endif /* AT30TSE758_H_ */
