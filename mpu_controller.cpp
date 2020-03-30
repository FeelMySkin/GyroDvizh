#include "mpu_controller.h"

MPUController::MPUController()
{
	founded = false;
	for(int i = 0;i<2;++i)
	{
		norm_data[i].acc_x = 0;
		norm_data[i].acc_y = 0;
		norm_data[i].acc_z = 0;
		norm_data[i].gyro_x = 0;
		norm_data[i].gyro_y = 0;
		norm_data[i].gyro_z = 0;
		
		raw_data[i].acc_x = 0;
		raw_data[i].acc_y = 0;
		raw_data[i].acc_z = 0;
		raw_data[i].gyro_x = 0;
		raw_data[i].gyro_y = 0;
		raw_data[i].gyro_z = 0;
	}
	roll = 0;
	pitch = 0;
}
//

MPUController::~MPUController()
{
	
}
//

void MPUController::Init(SPIController* spi, GPIO_TypeDef* cs_gpio, uint32_t cs_pin)
{
	this->spi = spi;
	this->cs_gpio = cs_gpio;
	this->cs_pin = cs_pin;
	InitGPIO();
}
//

void MPUController::InitGPIO()
{
	LL_GPIO_InitTypeDef gpio;
	gpio.Mode = LL_GPIO_MODE_OUTPUT;
	gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	gpio.Pin = cs_pin;
	gpio.Pull = LL_GPIO_PULL_NO;
	gpio.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	LL_GPIO_Init(cs_gpio,&gpio);
	
	LL_GPIO_SetOutputPin(cs_gpio,cs_pin);
}
//

void MPUController::InitializeMPU()
{
	SetupChip();
}
//

void MPUController::SetupChip()
{
	dat = ReadRegister(MPU_WHO_AM_I);
	if(dat != 0x71) return;
	
	WriteRegister(MPU_GYRO_CONF,0);
	dat = ReadRegister(MPU_GYRO_CONF);
	if(dat != 0) return;
	
	WriteRegister(MPU_ACC_CONF,(0<<3));
	dat = ReadRegister(MPU_ACC_CONF);
	if(dat != (0<<3)) return;
	
	founded = true;
}
//

void MPUController::EnableChip()
{
	LL_GPIO_ResetOutputPin(cs_gpio,cs_pin);
}
//

void MPUController::DisableChip()
{
	LL_GPIO_SetOutputPin(cs_gpio,cs_pin);
}
//

uint8_t MPUController::ReadRegister(uint8_t reg)
{
	reg |= 0x80;
	uint8_t ret = 0x00;
	EnableChip();
	spi->Transmit(reg);
	ret = spi->Transmit(0xFF);
	DisableChip();
	return ret;
}
//

void MPUController::WriteRegister(uint8_t reg, uint8_t byte)
{
	reg &= ~0x80;
	EnableChip();
	spi->Transmit(reg);
	spi->Transmit(byte);
	DisableChip();
}
//

void MPUController::Process()
{

	vector_len = sqrt(raw_data[1].acc_x*raw_data[1].acc_x + raw_data[1].acc_y*raw_data[1].acc_y + raw_data[1].acc_z*raw_data[1].acc_z);
}
//

void MPUController::ReadAccelData()
{
	if(!founded)
	{		
		InitializeMPU();
		return;
	}
	
	raw_data[0].acc_x = raw_data[1].acc_x;
	raw_data[0].acc_y = raw_data[1].acc_y;
	raw_data[0].acc_z = raw_data[1].acc_z;
	
	raw_data[1].acc_x = (int16_t)((ReadRegister(MPU_ACC_OUT_X_H)<<8) | ReadRegister(MPU_ACC_OUT_X_L))/(32768.0/2);
	raw_data[1].acc_y = (int16_t)((ReadRegister(MPU_ACC_OUT_Y_H)<<8) | ReadRegister(MPU_ACC_OUT_Y_L))/(32768.0/2);
	raw_data[1].acc_z = (int16_t)((ReadRegister(MPU_ACC_OUT_Z_H)<<8) | ReadRegister(MPU_ACC_OUT_Z_L))/(32768.0/2);
}
//

void MPUController::ReadGyroData()
{
	if(!founded)
	{		
		InitializeMPU();
		return;
	}
	
	raw_data[0].gyro_x = raw_data[1].gyro_x;
	raw_data[0].gyro_y = raw_data[1].gyro_y;
	raw_data[0].gyro_z = raw_data[1].gyro_z;
	
	raw_data[1].gyro_x = (int16_t)((ReadRegister(MPU_GYRO_OUT_X_H)<<8) | ReadRegister(MPU_GYRO_OUT_X_L))/131.0;
	raw_data[1].gyro_y = (int16_t)((ReadRegister(MPU_GYRO_OUT_Y_H)<<8) | ReadRegister(MPU_GYRO_OUT_Y_L))/131.0;
	raw_data[1].gyro_z = (int16_t)((ReadRegister(MPU_GYRO_OUT_Z_H)<<8) | ReadRegister(MPU_GYRO_OUT_Z_L))/131.0;
}
//

/*Low-Pass Filter*/
/*y[n] = (1-alpha)*x[n]+alpha*y[n-1]*/
void MPUController::FilterAccelData()
{
	norm_data[0].acc_x = norm_data[1].acc_x;
	norm_data[0].acc_y = norm_data[1].acc_y;
	norm_data[0].acc_z = norm_data[1].acc_z;
	
	norm_data[1].acc_x = (1-alpha)*raw_data[1].acc_x+alpha*norm_data[0].acc_x;
	norm_data[1].acc_y = (1-alpha)*raw_data[1].acc_y+alpha*norm_data[0].acc_y;
	norm_data[1].acc_z = (1-alpha)*raw_data[1].acc_z+alpha*norm_data[0].acc_z;
}
//

/*High-Pass Filter*/
/*y[n] = (1-alpha)*y[n-1]+(1-alpha)*(x[n]-x[n-1])*/
void MPUController::FilterGyroData()
{
	norm_data[0].gyro_x = norm_data[1].gyro_x;
	norm_data[0].gyro_y = norm_data[1].gyro_y;
	norm_data[0].gyro_z = norm_data[1].gyro_z;
	
	norm_data[1].gyro_x = (1-alpha)*norm_data[0].gyro_x + (1-alpha)*(raw_data[1].gyro_x - raw_data[0].gyro_x);
	norm_data[1].gyro_y = (1-alpha)*norm_data[0].gyro_y + (1-alpha)*(raw_data[1].gyro_y - raw_data[0].gyro_y);
	norm_data[1].gyro_z = (1-alpha)*norm_data[0].gyro_z + (1-alpha)*(raw_data[1].gyro_z - raw_data[0].gyro_z);
}
//

void MPUController::CountRP()
{
	MPU_Datas *used_data = &norm_data[1];
	
	atan_tst_roll = atan2f(used_data->acc_x,used_data->acc_z);
	atan_tst_pitch = atan2f(used_data->acc_y,used_data->acc_z);
	roll = (1-alpha)*(roll-used_data->gyro_y*DT) + alpha*atan_tst_roll*180/M_PI;
	pitch = (1-alpha)*(pitch+used_data->gyro_x*DT) + alpha*atan_tst_pitch*180/M_PI;
}
//
