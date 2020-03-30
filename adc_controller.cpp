#include "adc_controller.h"

ADCController::ADCController()
{
	cnt = 0;
	for(int i = 0;i<30;++i)
	{
		max[i] = 0;
		min[i] = 0xFFFF;
	}
}
//

ADCController::~ADCController()
{
	
}
//

void ADCController::AddLine(ADC_Struct* str)
{
	if(cnt == 0)
	{		
		adc = str;
		cnt++;
		return;
	}
	
	ADC_Struct* new_adc = adc;
	for(int i = 0;i<cnt-1;++i)
	{
		new_adc=new_adc->next;
	}
	new_adc->next = str;

	cnt++;
}
//

void ADCController::Init()
{
	if(cnt == 0) return;
	InitGPIO();
	InitDMA();
	InitLines();
	
	tst_cnt = 0;
	LL_ADC_REG_StartConversion(ADC1);
}
//

void ADCController::InitGPIO()
{
	ADC_Struct* buf_adc = adc;
	
	LL_GPIO_InitTypeDef gpio;
	gpio.Mode = LL_GPIO_MODE_ANALOG;
	gpio.Pull = LL_GPIO_PULL_NO;
	gpio.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	for(int i = 0;i<cnt;++i)
	{
		gpio.Pin = buf_adc->pin;
		LL_GPIO_Init(buf_adc->gpio,&gpio);
		buf_adc = buf_adc->next;
	}
}
//

void ADCController::InitLines()
{
	ADC_Struct* buf_adc = adc;
	
	//LL_ADC_StartCalibration(ADC1);
	//LL_ADC_Enable(ADC1);
	//while(LL_ADC_IsCalibrationOnGoing(ADC1)) ;
	
	LL_ADC_InitTypeDef adc_ini;
	adc_ini.Clock = LL_ADC_CLOCK_ASYNC_DIV2;
	adc_ini.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
	adc_ini.Resolution = LL_ADC_RESOLUTION_12B;
	LL_ADC_Init(ADC1,&adc_ini);
	
	LL_ADC_REG_InitTypeDef reg;
	reg.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
	reg.Overrun = LL_ADC_REG_OVR_DATA_PRESERVED;
	reg.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
	reg.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
	reg.SequencerLength = RanksCounter();
	reg.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
	LL_ADC_REG_Init(ADC1,&reg);
	LL_ADC_REG_SetSequencerScanDirection(ADC1,LL_ADC_REG_SEQ_SCAN_DIR_FORWARD);
	LL_ADC_REG_SetSequencerConfigurable(ADC1,LL_ADC_REG_SEQ_CONFIGURABLE);
	LL_ADC_SetSamplingTimeCommonChannels(ADC1,LL_ADC_SAMPLINGTIME_COMMON_1,LL_ADC_SAMPLINGTIME_160CYCLES_5);
	
	for(int i = 0;i<cnt;++i)
	{
		LL_ADC_REG_SetSequencerRanks(ADC1,GetRank(i),buf_adc->adc_ch);
		LL_ADC_SetChannelSamplingTime(ADC1,buf_adc->adc_ch,LL_ADC_SAMPLINGTIME_COMMON_1);
		buf_adc = buf_adc->next;
	}
	
	LL_ADC_Enable(ADC1);
}
//

void ADCController::InitDMA()
{	
	LL_DMA_InitTypeDef dma;
	dma.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
	dma.MemoryOrM2MDstAddress = (uint32_t)&meas;
	dma.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_WORD;
	dma.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	dma.PeriphOrM2MSrcAddress = (uint32_t)&ADC1->DR;
	dma.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_WORD;
	dma.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
	dma.Mode = LL_DMA_MODE_CIRCULAR;
	dma.NbData = cnt; //ADC_CHANNELS;
	dma.PeriphRequest = LL_DMAMUX_REQ_ADC1;
	dma.Priority = LL_DMA_PRIORITY_MEDIUM;
	LL_DMA_Init(ADC_DMA,ADC_DMA_CH,&dma);
	
	LL_DMA_EnableChannel(ADC_DMA,ADC_DMA_CH);
	
	LL_DMA_EnableIT_TC(ADC_DMA,ADC_DMA_CH);
	EnableDmaIRQn(ADC_DMA,ADC_DMA_CH,1);
}
//

uint32_t ADCController::RanksCounter()
{
	if(cnt == 1) return LL_ADC_REG_SEQ_SCAN_DISABLE;
	if(cnt == 2) return LL_ADC_REG_SEQ_SCAN_ENABLE_2RANKS;
	if(cnt == 3) return LL_ADC_REG_SEQ_SCAN_ENABLE_3RANKS;
	if(cnt == 4) return LL_ADC_REG_SEQ_SCAN_ENABLE_4RANKS;
	if(cnt == 5) return LL_ADC_REG_SEQ_SCAN_ENABLE_5RANKS;
	if(cnt == 6) return LL_ADC_REG_SEQ_SCAN_ENABLE_6RANKS;
	if(cnt == 7) return LL_ADC_REG_SEQ_SCAN_ENABLE_7RANKS;
	if(cnt == 8) return LL_ADC_REG_SEQ_SCAN_ENABLE_8RANKS;
	
	return 0;
}
//

uint32_t ADCController::GetRank(uint8_t c)
{
	c=c+1;
	if(c == 1) return LL_ADC_REG_RANK_1;
	if(c == 2) return LL_ADC_REG_RANK_2;
	if(c == 3) return LL_ADC_REG_RANK_3;
	if(c == 4) return LL_ADC_REG_RANK_4;
	if(c == 5) return LL_ADC_REG_RANK_5;
	if(c == 6) return LL_ADC_REG_RANK_6;
	if(c == 7) return LL_ADC_REG_RANK_7;
	if(c == 8) return LL_ADC_REG_RANK_8;
	
	return 0;
}
//

void ADCController::Process()
{
	ADC_Struct * buf_adc = adc;
	for(int i = 0;i<cnt;++i)
	{
		if(meas[i]>=max[i]) max[i] = meas[i];
		if(meas[i]<=min[i]) min[i] = meas[i];
		switch(buf_adc->type)
		{
			case ADC_TYPE_CURRENT:
				results[i] = ((meas[i]*3.3/4095 + buf_adc->offset)*buf_adc->coeff)*1000.0;
				break;
			
			case ADC_TYPE_RESISTANCE:
				results[i] = (buf_adc->coeff*4095.0  + buf_adc->offset)/meas[i];
				break;
			
			case ADC_TYPE_VOLTAGE:
				results[i] = (meas[i]*3.3/4095  + buf_adc->offset)*buf_adc->coeff;
				break;
		}
		
		buf_adc = buf_adc->next;
	}
	
	
	LL_ADC_REG_StartConversion(ADC1);
}
//

double ADCController::getMeasure(const char* id)
{
	ADC_Struct* str_buf = adc;
	for(int i = 0;i<cnt;++i)
	{
		
		if(strcmp(id,str_buf->id) == 0) return results[i];
		str_buf = str_buf->next;
	}
	
	return 0xFF;
}
//

uint16_t ADCController::GetRawMeasure(const char* id)
{
	ADC_Struct* str_buf = adc;
	for(int i = 0;i<cnt;++i)
	{
		
		if(strcmp(id,str_buf->id) == 0) return meas[i];
		str_buf = str_buf->next;
	}
	
	return 0x00;
}
//
