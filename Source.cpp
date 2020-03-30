#include "Source.h"

bool read_mpu = false;
bool rdy = false;
bool brake_on = false, brake_trig = false;
uint32_t dac1_old_v = 0, dac1_new_v = 0;
uint32_t dac2_old_v = 0, dac2_new_v = 0;
double duty,interval = 0;

#define LOW_ANGLE			10
#define HIGH_ANGLE		60
#define MAX_VOLTAGE		4095
#define MIN_VOLTAGE		0

#define Kp						0.05
#define Ki						40
#define Kd						40

#define REF						0.0

#define a							-0.5026 //-0.42
#define b							98.6750 //94.3
#define c							-31.6144 //-51

#define LOW_HANDLE		0x460
#define HIGH_HANDLE		0xCB0


extern "C"
{
	void DMA1_Channel1_IRQHandler()
	{
		LL_DMA_ClearFlag_TC1(DMA1);
		LL_DMA_ClearFlag_GI1(DMA1);
		LL_DMA_ClearFlag_HT1(DMA1);
		adc.Process();
	}
	//
	
	void TIM3_IRQHandler()
	{
		LL_TIM_ClearFlag_UPDATE(MPU_UPDATE_TIM);
		mpu1.ReadAccelData();
		mpu1.ReadGyroData();
		mpu1.FilterAccelData();
		mpu1.FilterGyroData();
		mpu1.CountRP();
		DutyVoltage();
	}
	//
	
	void TIM6_DAC_LPTIM1_IRQHandler()
	{
		LL_TIM_ClearFlag_UPDATE(BRAKE_PAUSE_TIM);
		if(brake_trig && !brake_on)
		{
			LL_GPIO_SetOutputPin(BRAKE_ACTIVE_GPIO,BRAKE_ACTIVE_PIN);
			brake_trig = false;
			brake_on = true;
		}
		else if (brake_trig && brake_on)
		{
			LL_GPIO_ResetOutputPin(BRAKE_ACTIVE_GPIO,BRAKE_ACTIVE_PIN);
			brake_trig = false;
			brake_on = false;
		}
		LL_TIM_DisableCounter(BRAKE_PAUSE_TIM);
	}
	//
	
	
}
//

void InitRCC()
{
	LL_RCC_HSI_Enable();
	while(!LL_RCC_HSI_IsReady()) ;
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI) ;
	
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
	LL_RCC_PLL_Disable();
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI,LL_RCC_PLLM_DIV_1,8,LL_RCC_PLLR_DIV_2);
	LL_RCC_PLL_EnableDomain_SYS();
	LL_RCC_PLL_Enable();
	while(!LL_RCC_PLL_IsReady()) ;
	
	
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
	//while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) ;
	
	SystemCoreClockUpdate();
}
//

void InitDAC()
{
	DAC_Struct *str = new DAC_Struct();
	
	str->dac_channel = DAC_CH1_CHANNEL;
	str->dac_gpio = DAC_CH1_GPIO;
	str->dac_pin = DAC_CH1_PIN;
	str->id = DAC_CH1_ID;
	dac.AddLine(str);
	
	str = new DAC_Struct();
	str->dac_channel = DAC_CH2_CHANNEL;
	str->dac_gpio = DAC_CH2_GPIO;
	str->dac_pin = DAC_CH2_PIN;
	str->id = DAC_CH2_ID;
	dac.AddLine(str);
	
	dac.Init();
}
//

void InitSPI1()
{
	SPI_Struct spi;
	spi.af = SPI1_AF;
	spi.miso_gpio = SPI1_MISO_GPIO;
	spi.miso_pin 	= SPI1_MISO_PIN;
	spi.mosi_gpio = SPI1_MOSI_GPIO;
	spi.mosi_pin 	= SPI1_MOSI_PIN;
	spi.sck_gpio 	= SPI1_SCK_GPIO;
	spi.sck_pin 	= SPI1_SCK_PIN;
	spi.spi				= SPI1;
	spi1.Init(spi);
}
//

void InitADC()
{
	ADC_Struct *str = new ADC_Struct();
	str->adc_ch = ADC_CH1_CHANNEL;
	str->coeff = 1;
	str->gpio = ADC_CH1_GPIO;
	str->id = ADC_CH1_ID;
	str->next = NULL;
	str->offset = 0;
	str->pin = ADC_CH1_PIN;
	str->type = ADC_TYPE_VOLTAGE;
	adc.AddLine(str);
	
	str = new ADC_Struct();
	str->adc_ch = ADC_CH2_CHANNEL;
	str->coeff = 1;
	str->gpio = ADC_CH2_GPIO;
	str->id = ADC_CH2_ID;
	str->next = NULL;
	str->offset = 0;
	str->pin = ADC_CH2_PIN;
	str->type = ADC_TYPE_VOLTAGE;
	adc.AddLine(str);
	
	adc.Init();
}
//

void InitGPIO()
{
	LL_GPIO_InitTypeDef gpio;
	gpio.Mode = LL_GPIO_MODE_INPUT;
	gpio.Pin = GYRO_ON_PIN;
	gpio.Pull = LL_GPIO_PULL_UP;
	gpio.Speed = LL_GPIO_SPEED_FAST;
	LL_GPIO_Init(GYRO_ON_GPIO,&gpio);
	
	gpio.Pin = BRAKE_BUTTON_PIN;
	LL_GPIO_Init(BRAKE_BUTTON_GPIO,&gpio);
	
	gpio.Mode = LL_GPIO_MODE_OUTPUT;
	gpio.Pin = BRAKE_ACTIVE_PIN;
	LL_GPIO_Init(BRAKE_ACTIVE_GPIO,&gpio);
	LL_GPIO_ResetOutputPin(BRAKE_ACTIVE_GPIO,BRAKE_ACTIVE_PIN);
	
	
}
//

void InitPeriph()
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_ALL);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DAC1 | LL_APB1_GRP1_PERIPH_TIM3 | LL_APB1_GRP1_PERIPH_TIM6);
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1 | LL_APB2_GRP1_PERIPH_ADC);
	
}
//

void InitTimer()
{
	LL_TIM_InitTypeDef tim;
	tim.Autoreload = MPU_UPDATE_PERIOD;
	tim.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	tim.CounterMode = LL_TIM_COUNTERMODE_UP;
	tim.Prescaler = SystemCoreClock/1000-1;
	tim.RepetitionCounter = 0;
	
	LL_TIM_Init(MPU_UPDATE_TIM,&tim);
	
	LL_TIM_ClearFlag_UPDATE(MPU_UPDATE_TIM);
	LL_TIM_EnableIT_UPDATE(MPU_UPDATE_TIM);
	
	EnableTimIRQn(MPU_UPDATE_TIM,0);
	
	LL_TIM_EnableCounter(MPU_UPDATE_TIM);
	
	tim.Autoreload = BRAKE_PAUSE_WAIT;
	LL_TIM_Init(BRAKE_PAUSE_TIM,&tim);
	LL_TIM_ClearFlag_UPDATE(BRAKE_PAUSE_TIM);
	LL_TIM_EnableIT_UPDATE(BRAKE_PAUSE_TIM);
	
	EnableTimIRQn(BRAKE_PAUSE_TIM,1);
}
//

double ExpanentialVoltage()
{
	if(mpu1.pitch<=LOW_ANGLE) return 0;
	else if(mpu1.pitch>=HIGH_ANGLE) return MAX_VOLTAGE;
	else return a*mpu1.pitch*mpu1.pitch + b*mpu1.pitch + c;
}
//

double LinearVoltage()
{
	if(mpu1.pitch<=LOW_ANGLE) return 0;
	else if(mpu1.pitch>=HIGH_ANGLE) return MAX_VOLTAGE;
	else return MIN_VOLTAGE+(MAX_VOLTAGE - MIN_VOLTAGE)*(mpu1.pitch-LOW_ANGLE)/(HIGH_ANGLE-LOW_ANGLE);
}
//

double DutyVoltage()
{
	double error = REF-mpu1.pitch;
	interval+=error*DT;
	duty=(Kp*error+(Kd*1000*(error)/DT)+Ki*interval/1000);
	return duty;
}
//

uint16_t ADCVoltage()
{
	uint16_t dat = adc.GetRawMeasure(ADC_CH1_ID);
	if(dat<=LOW_HANDLE) return 0;
	else if(dat>= HIGH_HANDLE) return 0xFFF;
	else return MIN_VOLTAGE+(MAX_VOLTAGE - MIN_VOLTAGE)*(dat-LOW_HANDLE)/(HIGH_HANDLE-LOW_HANDLE);
}
//

int main()
{
	//while(!rdy) ;
	InitRCC();
	InitPeriph();
	InitGPIO();
	
	InitDAC();
	InitADC();
	
	InitSPI1();
	mpu1.Init(&spi1,MPU_CS_GPIO,MPU_CS_PIN);
	for(int i = 0;i<10000;++i) ;
	mpu1.InitializeMPU();
	
	InitTimer();
	
	while(1)
	{
		
		dac1_new_v = ((LL_GPIO_ReadInputPort(GYRO_ON_GPIO)&GYRO_ON_PIN)==0?LinearVoltage():0) + ADCVoltage();
		dac2_new_v = ((LL_GPIO_ReadInputPort(GYRO_ON_GPIO)&GYRO_ON_PIN)==0?LinearVoltage():0) + ADCVoltage();
		
		dac1_new_v=dac1_new_v<=4095?dac1_new_v:4095;
		dac2_new_v=dac2_new_v<=4095?dac2_new_v:4095;
		
		/*if(!(LL_GPIO_ReadInputPort(BRAKE_BUTTON_GPIO)&BRAKE_BUTTON_PIN) && !brake_trig && !brake_on)
		{
			brake_trig = true;
			LL_TIM_EnableCounter(BRAKE_PAUSE_TIM);
		}
		else if((LL_GPIO_ReadInputPort(BRAKE_BUTTON_GPIO)&BRAKE_BUTTON_PIN) && !brake_trig && brake_on)
		{
			brake_trig = true;
			LL_TIM_EnableCounter(BRAKE_PAUSE_TIM);
		}*/
		
		if(brake_trig || brake_on)
		{
			dac1_new_v = 0;
			dac2_new_v = 0;
		}
		
		if(dac1_old_v != dac1_new_v)
		{
			dac1_old_v = dac1_new_v;
			dac.SetBinding(DAC_CH1_ID,dac1_new_v);
		}
		
		if(dac2_old_v != dac2_new_v)
		{
			dac2_old_v = dac2_new_v;
			dac.SetBinding(DAC_CH2_ID,dac2_new_v);
		}
	}
}
//
