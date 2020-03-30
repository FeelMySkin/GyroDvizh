#ifndef ADC_CONTROLLER_H
#define ADC_CONTROLLER_H

#include "defines.h"

enum ADC_TYPE
{
	ADC_TYPE_VOLTAGE,
	ADC_TYPE_CURRENT,
	ADC_TYPE_RESISTANCE
};

struct ADC_Struct
{
	ADC_TYPE 				type;
	GPIO_TypeDef* 	gpio;
	uint32_t				pin;
	double					coeff;
	const char*			id;
	uint32_t				adc_ch;
	ADC_Struct*			next;
	double					offset;
};

class ADCController
{
	public:
		ADCController();
		~ADCController();
		void Init();
		void AddLine(ADC_Struct*);
		double getMeasure(const char* id);
		void Process();
		uint16_t GetRawMeasure(const char* id);
	
	
	private:
		void InitGPIO();
		void InitLines();
		void InitADC();
		void InitDMA();
		uint32_t RanksCounter();
		uint32_t GetRank(uint8_t);
		ADC_Struct*	adc;
		uint32_t meas[30];
		double results[30];
		uint32_t min[30];
		uint32_t max[30];
		uint8_t cnt;
		uint32_t tst_cnt;
};


#endif
