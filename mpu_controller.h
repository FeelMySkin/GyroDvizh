#ifndef MPU_CONTROLLER_H
#define MPU_CONTROLLER_H

#include "defines.h"
#include "spi_controller.h"

#define MPU_WHO_AM_I				0x75
#define MPU_CONFIG					0x1A
#define MPU_GYRO_CONF				0x1B
#define MPU_ACC_CONF				0x1C
#define MPU_ACC_CONF2				0x1D
#define MPU_ACC_OUT_X_H			0x3B
#define MPU_ACC_OUT_X_L			0x3C
#define MPU_ACC_OUT_Y_H			0x3D
#define MPU_ACC_OUT_Y_L			0x3E
#define MPU_ACC_OUT_Z_H			0x3F
#define MPU_ACC_OUT_Z_L			0x40
#define MPU_GYRO_OUT_X_H		0x43
#define MPU_GYRO_OUT_X_L		0x44
#define MPU_GYRO_OUT_Y_H		0x45
#define MPU_GYRO_OUT_Y_L		0x46
#define MPU_GYRO_OUT_Z_H		0x47
#define MPU_GYRO_OUT_Z_L		0x48
#define MPU_PWR_MGMT1				0x6B
#define MPU_PWR_MGMT2				0x6C

#define alpha								0.5
#define DT									MPU_UPDATE_PERIOD/1000.0
#define M_PI								3.14159265359

struct MPU_Datas
{
	double acc_x,acc_y,acc_z;
	double gyro_x,gyro_y,gyro_z;
};

class MPUController
{
	public:
		MPUController();
		~MPUController();
		void Init(SPIController* spi, GPIO_TypeDef* cs_gpio, uint32_t cs_pin);
		void InitializeMPU();
		void Process();
		void ReadAccelData();
		void ReadGyroData();
		void FilterGyroData();
		void FilterAccelData();
		void CountRP();
	
		double roll,pitch;
	
	private:
		void InitGPIO();
		uint8_t ReadRegister(uint8_t);
		void WriteRegister(uint8_t,uint8_t);
		void SetupChip();
		void EnableChip();
		void DisableChip();
	
		SPIController*	spi;
		GPIO_TypeDef*		cs_gpio;
		uint32_t 				cs_pin;
	
		MPU_Datas raw_data[2];
		MPU_Datas norm_data[2];
		double atan_tst_roll,atan_tst_pitch;
	
		double vector_len;
		bool founded;
		uint8_t dat;
};
//



#endif
