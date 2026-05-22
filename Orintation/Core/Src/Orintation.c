/*
 * ITG.c
 *
 *  Created on: Nov 2, 2025
 *      Author: CptFrog
 */

#include "Orintation.h"

struct	AdxlReg AdxlReg = {0x00, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22,
	0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,
	0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

Status	ADXL_init(OrintationData* OriData, uint8_t Range, uint8_t BRate){
	Status Status=HAL_OK;
	uint8_t id;
	/* ID */
	I2C_ReadReg(OriData->I2Cx, ADXL_ADR, AdxlReg.DEVID, &id,1);

	if(id != 0xE5){
		Status = O_ID_ERROR;
		return Status;
	}

	switch(Range)
	{
	case ADXL_RANGE_2G:
		OriData->Acce_Scalefactor=ADXL_SCALE_FACTOR_2G;
		break;
	case ADXL_RANGE_4G:
		OriData->Acce_Scalefactor=ADXL_SCALE_FACTOR_4G;
		break;
	case ADXL_RANGE_8G:
		OriData->Acce_Scalefactor=ADXL_SCALE_FACTOR_8G;
		break;
	case ADXL_RANGE_16G:
		OriData->Acce_Scalefactor=ADXL_SCALE_FACTOR_16G;
		break;
	}

	I2C_write2reg(OriData->I2Cx, ADXL_ADR, AdxlReg.BW_RATE, BRate);
	I2C_write2reg(OriData->I2Cx, ADXL_ADR, AdxlReg.POWER_CTL,ADXL_POWER_CTL_Measure);
	I2C_write2reg(OriData->I2Cx, ADXL_ADR, AdxlReg.DATA_FORMAT, Range);

	return Status;
}

Status	Orintation_init(OrintationData* OriData,I2C_TypeDef* I2Cx,float Kf_a,float Kf_Om,uint8_t ADXL_RANGE){
	Status Status=HAL_OK;
	uint8_t i=0;
	OriData->I2Cx=I2Cx;
	OriData->Kf_a=Kf_a;
	OriData->Kf_Om=Kf_Om;
	uint8_t BRate=ADXL_BWRATE_6_25;
	Status=ADXL_init(OriData, ADXL_RANGE, BRate);

	for(i=0;i<100;i++){
		Status=Orintation_Acc_Get(OriData);
	}
	return Status;
}

Status	ADXL_GetDataRaw(OrintationData* OriData, uint8_t* data){
	Status Status=HAL_OK;
	/* ID */
	I2C_ReadReg(OriData->I2Cx,ADXL_ADR,AdxlReg.DATAX0,data,6);

	return Status;
}

Status	ADXL_GetData16b(OrintationData* OriData, int16_t* data){
	Status Status=HAL_OK;
	uint8_t buff[6];
	/* ID */
	if(ADXL_GetDataRaw(OriData, buff)){
		Status = HAL_ERROR;
		return Status;
	}
	data[0] = ((int16_t)(buff[1] << 8) | buff[0]);
	data[1] = ((int16_t)(buff[3] << 8) | buff[2]);
	data[2] = ((int16_t)(buff[5] << 8) | buff[4]);

	return Status;
}

Status	ADXL_GetDataF(OrintationData* OriData, float* data){
	Status Status=HAL_OK;
	int16_t buff[3];
	/* ID */
	if(ADXL_GetData16b(OriData, buff)){
		Status = HAL_ERROR;
		return Status;
	}
	data[0] = OriData->Acce_Scalefactor * buff[0]/1000;
	data[1] = OriData->Acce_Scalefactor * buff[1]/1000;
	data[2] = OriData->Acce_Scalefactor * buff[2]/1000;

	return Status;
}

Status	Orintation_Acc_Get(OrintationData* OriData){
	Status Status;
	float dataAXYZ[3];
	float Kf_a=0.2;
	if(OriData->Kf_a==0){
		OriData->Kf_a=Kf_a;
	}
	Status=ADXL_GetDataF(OriData,dataAXYZ);
	Filtration(OriData->AcceXYZ_raw,dataAXYZ,OriData->Kf_a);
	return Status;
}

void	Filtration(float* FiltredData, float* NewData,float Kf){
	FiltredData[0]+=(NewData[0]-FiltredData[0])*Kf;
	FiltredData[1]+=(NewData[1]-FiltredData[1])*Kf;
	FiltredData[2]+=(NewData[2]-FiltredData[2])*Kf;
}

void	Orintation_Horizon_Calc(OrintationData* OriData){
	float g;
	g=sqrt(pow(OriData->AcceXYZ_raw[0],2)+pow(OriData->AcceXYZ_raw[1],2)+pow(OriData->AcceXYZ_raw[2],2));
	OriData->AnglPYR[0]=(asin(OriData->AcceXYZ_raw[0]/g)*180/3.14);
	OriData->AnglPYR[2]=(asin(OriData->AcceXYZ_raw[1]/g)*180/3.14);
}

void	Orintation_Horizon_AcclCorr(OrintationData* OriData){
	float g;
	g=sqrt(pow(OriData->AcceXYZ_raw[0],2)+pow(OriData->AcceXYZ_raw[1],2)+pow(OriData->AcceXYZ_raw[2],2));
	OriData->AnglPYR[0]+=(asin(OriData->AcceXYZ_cor[0]/g)*180/3.14);
	OriData->AnglPYR[2]+=(asin(OriData->AcceXYZ_cor[1]/g)*180/3.14);
}

void	Orintation_Accl_HorizonCorr(OrintationData* OriData){
	//float g;
	//g=sqrt(pow(OriData->AcceXYZ[0],2)+pow(OriData->AcceXYZ[1],2)+pow(OriData->AcceXYZ[2],2));
	OriData->AcceXYZ_cor[0]=0.9*(OriData->AcceXYZ_raw[0]-sin(OriData->AnglPYR[0]*3.14/180));
	OriData->AcceXYZ_cor[1]=0.9*(OriData->AcceXYZ_raw[1]-sin(OriData->AnglPYR[2]*3.14/180));
}

void	Orintation_Velocity_Calc(OrintationData* OriData, float Time){
	//float g;
	//g=sqrt(pow(OriData->AcceXYZ[0],2)+pow(OriData->AcceXYZ[1],2)+pow(OriData->AcceXYZ[2],2));
	OriData->AcceXYZ_cor[0]=OriData->AcceXYZ_raw[0]-sin(OriData->AnglPYR[0]*3.14/180);
	OriData->AcceXYZ_cor[1]=OriData->AcceXYZ_raw[1]-sin(OriData->AnglPYR[2]*3.14/180);
}

