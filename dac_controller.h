#ifndef DAC_CONTROLLER_H
#define DAC_CONTROLLER_H

#include "defines.h"

struct DAC_Struct
{
	GPIO_TypeDef*		dac_gpio;
	uint32_t				dac_pin;
	uint32_t				dac_channel;
	DAC_Struct*			next;
	const char*			id;
};

class DACController
{
	public:
		DACController();
		~DACController();
		void Init();
		void AddLine(DAC_Struct*);
		void SetVoltage(const char*,double);
		void SetBinding(const char*,uint16_t);
	
	private:
		void InitGPIO();
		void InitDAC();
		DAC_Struct* dac;
		uint8_t cnt;
	
};

#endif
