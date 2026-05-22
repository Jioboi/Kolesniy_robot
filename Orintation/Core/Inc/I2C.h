/*
 * I2C.h
 *
 *  Created on: Oct 25, 2025
 *      Author: CptFrog
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "stm32f1xx.h"

void 	I2C_ReadReg(I2C_TypeDef* i2c, uint8_t Addr, uint8_t Reg, uint8_t *data,uint8_t n);
void 	I2C_write2reg(I2C_TypeDef* i2c, uint8_t Addr, uint8_t Reg, uint8_t data);

#endif /* INC_I2C_H_ */
