/*
 * Orintation.h
 *
 *  Created on: Nov 2, 2025
 *      Author: CptFrog
 */

#ifndef INC_ORINTATION_H_
#define INC_ORINTATION_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "math.h"
#include "I2C.h"

#define ADXL_ADR            (0x53)
#define ADXL_ERR            0
//Скорость передачи выходных данных (Гц)
#define ADXL_BWRATE_6_25 	 0x06
#define ADXL_BWRATE_12_5 	 0x07
#define ADXL_BWRATE_25 		 0x08
#define ADXL_BWRATE_50 		 0x09
#define ADXL_BWRATE_100		 0x0A
#define ADXL_BWRATE_200		 0x0B
#define ADXL_BWRATE_400		 0x0C
#define ADXL_BWRATE_800		 0x0D
#define ADXL_BWRATE_1600  	 0x0E
#define ADXL_BWRATE_3200  	 0x0F
//Формата выходных данных
#define ADXL_RANGE_2G       0x00
#define ADXL_RANGE_4G       0x01
#define ADXL_RANGE_8G       0x02
#define ADXL_RANGE_16G      0x03
//перевод от формата выходных данных
#define ADXL_SCALE_FACTOR_2G         3.9
#define ADXL_SCALE_FACTOR_4G         7.8
#define ADXL_SCALE_FACTOR_8G         15.6
#define ADXL_SCALE_FACTOR_16G        31.2

#define ADXL_POWER_CTL_Wakeup        0x01
#define ADXL_POWER_CTL_Sleep         0x04
#define ADXL_POWER_CTL_Measure       0x08
#define ADXL_POWER_CTL_AUTO_SLEEP    0x10
#define ADXL_POWER_CTL_Link          0x20

typedef enum
{
  O_OK       = 0x00U,
  O_ERROR    = 0x01U,
  O_BUSY     = 0x02U,
  O_TIMEOUT  = 0x03U,
  O_ID_ERROR = 0x04U,
} Status;

struct AdxlReg
{
	uint8_t DEVID;	           //Номер ID
	uint8_t	THRESH_TAP;        //Значение уставки для толчка
	uint8_t	OFSX;	             //Смещение для оси X
	uint8_t	OFSY;	             //Смещение для оси Y
	uint8_t	OFSZ;	             //Смещение для оси Z
	uint8_t	DUR;	             //Длительность толчка
	uint8_t	LATENT;	           //Задержка перед ожиданием второго толчка
	uint8_t	WINDOW;	           //Время для обнаружения второго толчка
	uint8_t	THRESH_ACT;	       //Значение уставки функции активности
	uint8_t	THRESH_INACT;	     //Значение уставки функции бездействия
	uint8_t	TIME_INACT;	       //Время бездействия
	uint8_t	ACT_INACT_CTL;	   //Привязка событий активности/бездействия на оси
	uint8_t	THRESH_FF;	       //Значение уставки свободного падения
	uint8_t	TIME_FF;	         //Время свободного падения
	uint8_t	TAP_AXES;	         //Привязка событий толчка на оси
	uint8_t	ACT_TAP_STATUS;	   //Регистр флагов событий толчка или активности
	uint8_t	BW_RATE;	         //Настройка частоты дискретизации
	uint8_t	POWER_CTL;	       //Настройка режима энергосбережения
	uint8_t	INT_ENABLE;	       //Регистр управления прерываниями
	uint8_t	INT_MAP;	         //Назначение выходных линий для прерываний
	uint8_t	INT_SOURCE;	       //Регистр флагов прерываний
	uint8_t	DATA_FORMAT;	     //Настройка формата выходных данных
	uint8_t	DATAX0;	           //Выходные данные для оси X (младший байт)
	uint8_t	DATAX1;	           //Выходные данные для оси X (старший байт)
	uint8_t	DATAY0;	           //Выходные данные для оси Y (старший байт)
	uint8_t	DATAY1;            //Выходные данные для оси Y (старший байт)
	uint8_t	DATAZ0;            //Выходные данные для оси Z (старший байт)
	uint8_t	DATAZ1;	           //Выходные данные для оси Z (старший байт)
	uint8_t	FIFO_CTL;	         //Настройка буфера FIFO
	uint8_t	FIFO_STATUS;	     //Регистр состояния буфера FIFO
};

typedef struct
{
	I2C_TypeDef* I2Cx;
	float Acce_Scalefactor;
	float Kf_a;
	float Kf_Om;
	float AcceXYZ_raw[3];
	float AcceXYZ_cor[3];
	float GyroXYZ[3];
	float AnglPYR[3];
}OrintationData;

//Status	ADXL_init(OrintationData* OriData, uint8_t Range, uint8_t BRate);
//Status	ADXL_GetDataRaw(OrintationData* OriData, uint8_t* data);
//Status	ADXL_GetData16b(OrintationData* OriData, int16_t* data);
Status	Orintation_init(OrintationData* OriData,I2C_TypeDef* I2Cx,float Kf_a,float Kf_Om,uint8_t ADXL_RANGE);
Status	Orintation_Acc_Get(OrintationData* OriData);
void	Orintation_Horizon_Calc(OrintationData* OriData);
void	Orintation_Horizon_AcclCorr(OrintationData* OriData);
void	Orintation_Accl_HorizonCorr(OrintationData* OriData);
void	Orintation_Velocity_Calc(OrintationData* OriData, float Time);
Status	ADXL_GetDataF(OrintationData* OriData, float* data);
void	Filtration(float* FiltredData, float* NewData, float Kf);
#endif /* INC_ORINTATION_H_ */
