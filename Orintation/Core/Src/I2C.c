/*
 * I2C.c
 *
 *  Created on: Oct 25, 2025
 *      Author: CptFrog
 */

#include "I2C.h"

void I2C_ReadReg(I2C_TypeDef* i2c, uint8_t Addr, uint8_t Reg, uint8_t* data, uint8_t n)
{
	volatile int temp;

	while (i2c->SR2 & I2C_SR2_BUSY); 	//ожидание пока не будет свободен i2c
	i2c->CR1|=I2C_CR1_START;			//генерация старт сигнала
	while(!(i2c->SR1 & I2C_SR1_SB)); 	//ожидание пока не будет запущен i2c
	i2c->DR=Addr<<1;					//Адресс на запись
	while(!(i2c->SR1 & I2C_SR1_ADDR));	//ожидание ответа на запрос i2c
	temp=i2c->SR2;						//освобождение чтением ??
	while(!(i2c->SR1&I2C_SR1_TXE));		//ожидание разрешения на отправку
	i2c->DR = Reg;						//запись регистра чтения
	while(!(i2c->SR1&I2C_SR1_TXE));		//ожидание разрешения на отправку
	i2c->CR1|=I2C_CR1_START;			//генерация старт сигнала
	while(!(i2c->SR1 & I2C_SR1_SB));	//ожидание пока не будет запущен i2c
	i2c->DR=Addr<<1|1;					//адресс на чтение
	while(!(i2c->SR1 & I2C_SR1_ADDR));	//ожидание ответа на запрос i2c
	temp=i2c->SR2;						//освобождение чтением ??
	i2c->CR1|=I2C_CR1_ACK;
	while(n>0)
	{
		if(n==1)
		{
			i2c->CR1&=~I2C_CR1_ACK;
			while(!(i2c->SR1&I2C_SR1_RXNE));
			*data++=i2c->DR;
			i2c->CR1|=I2C_CR1_STOP;
			break;
		}
		else
		{
			while(!(i2c->SR1&I2C_SR1_RXNE));
			(*data++)=i2c->DR;
			n--;
		}
	}
}
void I2C_write2reg(I2C_TypeDef* i2c, uint8_t Addr, uint8_t Reg, uint8_t data)
{
volatile int Temp;

while(i2c->SR2&I2C_SR2_BUSY){;}			//ожидание пока не будет свободен i2c
i2c->CR1|=I2C_CR1_START;                //генерация старт сигнала
while(!(I2C1->SR1&I2C_SR1_SB)){;}       //ожидание пока не будет запущен i2c
i2c->DR = Addr<< 1;                 	//Адресс на запись
while(!(i2c->SR1&I2C_SR1_ADDR)){;}      //ожидание ответа на запрос i2c
Temp = i2c->SR2; 						//освобождение чтением ??
while(!(i2c->SR1&I2C_SR1_TXE)){;}      	//ожидание разрешения на отправку
i2c->DR = Reg;                        	//запись регистра чтения
while(!(i2c->SR1&I2C_SR1_TXE)){;}      	//ожидание разрешения на отправку
i2c->DR = data;						//отправка данных
while (!(i2c->SR1 & I2C_SR1_BTF));     //ожидание завершения отправки
i2c->CR1 |=I2C_CR1_STOP;				//генерация стоп сигнала
}


/*
 * void i2c_ReadReg(I2C_TypeDef* i2c, uint8_t Addr, uint8_t Reg, uint8_t* data)
{
	volatile int temp;

	while (i2c->SR2 & I2C_SR2_BUSY); 	//ожидание пока не будет свободен i2c
	i2c->CR1|=I2C_CR1_START;			//генерация старт сигнала
	while(!(i2c->SR1 & I2C_SR1_SB)); 	//ожидание пока не будет запущен i2c
	i2c->DR=Addr<<1;					//Адресс на запись
	while(!(i2c->SR1 & I2C_SR1_ADDR));	//ожидание ответа на запрос i2c
	temp=i2c->SR2;						//освобождение чтением ??
	while(!(i2c->SR1&I2C_SR1_TXE));		//ожидание разрешения на отправку
	i2c->DR = Reg;						//запись регистра чтения
	while(!(i2c->SR1&I2C_SR1_TXE));		//ожидание разрешения на отправку
	i2c->CR1|=I2C_CR1_START;			//генерация старт сигнала
	while(!(i2c->SR1 & I2C_SR1_SB));	//ожидание пока не будет запущен i2c
	i2c->DR=Addr<<1|1;					//адресс на чтение
	while(!(i2c->SR1 & I2C_SR1_ADDR));	//ожидание ответа на запрос i2c
	temp=i2c->SR2;						//освобождение чтением ??

	while(!(i2c->SR1&I2C_SR1_RXNE));	//ожидание приема
	*data=i2c->DR;						//прием данных

	i2c->CR1|=I2C_CR1_STOP;				//генерация стоп сигнала
}
void i2c_ReadRegs(I2C_TypeDef* i2c, uint8_t Addr, uint8_t Reg, uint8_t* data)
{
	volatile int temp;

	while (i2c->SR2 & I2C_SR2_BUSY); 	//ожидание пока не будет свободен i2c
	i2c->CR1|=I2C_CR1_START;			//генерация старт сигнала
	while(!(i2c->SR1 & I2C_SR1_SB)); 	//ожидание пока не будет запущен i2c
	i2c->DR=Addr<<1;					//Адресс на запись
	while(!(i2c->SR1 & I2C_SR1_ADDR));	//ожидание ответа на запрос i2c
	temp=i2c->SR2;						//освобождение чтением ??
	while(!(i2c->SR1&I2C_SR1_TXE));		//ожидание разрешения на отправку
	i2c->DR = Reg;						//запись регистра чтения
	while(!(i2c->SR1&I2C_SR1_TXE));		//ожидание разрешения на отправку
	i2c->CR1|=I2C_CR1_START;			//генерация старт сигнала
	while(!(i2c->SR1 & I2C_SR1_SB));	//ожидание пока не будет запущен i2c
	i2c->DR=Addr<<1|1;					//адресс на чтение
	while(!(i2c->SR1 & I2C_SR1_ADDR));	//ожидание ответа на запрос i2c
	temp=i2c->SR2;						//освобождение чтением ??

	while(!(i2c->SR1&I2C_SR1_RXNE));	//ожидание приема
	*data=i2c->DR;						//прием данных

	i2c->CR1|=I2C_CR1_STOP;				//генерация стоп сигнала
}
*/
