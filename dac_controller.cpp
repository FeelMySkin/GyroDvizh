#include "dac_controller.h"

DACController::DACController()
{
	cnt = 0;
}
//

DACController::~DACController()
{
	
}
//

void DACController::Init()
{
	InitGPIO();
	InitDAC();
}
//

void DACController::InitGPIO()
{
	DAC_Struct * buf_dac = dac;
	
	LL_GPIO_InitTypeDef gpio;
	gpio.Mode = LL_GPIO_MODE_ANALOG;
	gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	gpio.Pull = LL_GPIO_PULL_NO;
	gpio.Speed = LL_GPIO_SPEED_FAST;
	
	for(int i = 0;i<cnt;++i)
	{
		gpio.Pin = buf_dac->dac_pin;
		LL_GPIO_Init(buf_dac->dac_gpio,&gpio);
		buf_dac = buf_dac->next;
	}
}
//

void DACController::InitDAC()
{
	DAC_Struct * buf_dac = this->dac;
	
	LL_DAC_InitTypeDef dac;
	dac.OutputMode = LL_DAC_OUTPUT_MODE_NORMAL;
	dac.WaveAutoGeneration = LL_DAC_WAVE_AUTO_GENERATION_NONE;
	dac.TriggerSource = LL_DAC_TRIG_SOFTWARE;
	dac.OutputBuffer = LL_DAC_OUTPUT_BUFFER_DISABLE;
	dac.OutputConnection = LL_DAC_OUTPUT_CONNECT_GPIO;
	
	for(int i = 0;i<cnt;++i)
	{
		LL_DAC_Init(DAC1,buf_dac->dac_channel,&dac);
		LL_DAC_Enable(DAC1,buf_dac->dac_channel);
		buf_dac = buf_dac->next;
	}
	
	
}
//

void DACController::AddLine(DAC_Struct* new_dac)
{
	if(cnt == 0)
	{
		dac = new_dac;
		cnt++;
		return;
	}
	
	DAC_Struct *nxt = dac;
	for(int i = 0;i<cnt-1;++i) nxt = dac->next;
	nxt->next = new_dac;
	cnt++;
	
}
//

void DACController::SetVoltage(const char* id, double voltage)
{
	DAC_Struct *buf_str = dac;
	for(int i = 0;i<cnt;++i)
	{
		if(strcmp(id,buf_str->id) == 0) break;
		buf_str = buf_str->next;
	}
	
	if(buf_str == NULL) return;
	LL_DAC_ConvertData12RightAligned(DAC1,buf_str->dac_channel,voltage*4095.0/3.4);
}
//

void DACController::SetBinding(const char* id,uint16_t data)
{
	DAC_Struct *buf_str = dac;
	for(int i = 0;i<cnt;++i)
	{
		if(strcmp(id,buf_str->id) == 0) break;
		buf_str = buf_str->next;
	}
	
	if(buf_str == NULL) return;
	
	LL_DAC_ConvertData12RightAligned(DAC1,buf_str->dac_channel,data);
}
//
