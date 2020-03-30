#include "periph_use.h"

void EnableExtiIRQn(uint32_t exti, uint8_t prio)
{
	switch(exti)
	{
		case LL_EXTI_LINE_0:
		case LL_EXTI_LINE_1:
			NVIC_DisableIRQ(EXTI0_1_IRQn);
			NVIC_SetPriority(EXTI0_1_IRQn,prio);
		break;
		
		case LL_EXTI_LINE_2:
		case LL_EXTI_LINE_3:
			NVIC_DisableIRQ(EXTI2_3_IRQn);
			NVIC_SetPriority(EXTI2_3_IRQn,prio);
		break;
		
		case LL_EXTI_LINE_4:
		case LL_EXTI_LINE_5:
		case LL_EXTI_LINE_6:
		case LL_EXTI_LINE_7:
		case LL_EXTI_LINE_8:
		case LL_EXTI_LINE_9:
		case LL_EXTI_LINE_10:
		case LL_EXTI_LINE_11:
		case LL_EXTI_LINE_12:
		case LL_EXTI_LINE_13:
		case LL_EXTI_LINE_14:
		case LL_EXTI_LINE_15:
			NVIC_DisableIRQ(EXTI4_15_IRQn);
			NVIC_SetPriority(EXTI4_15_IRQn,prio);
		break;
	}
}
//

/*void SetExtiSource(GPIO_TypeDef* gpio,uint32_t exti_line)
{
	uint16_t syscfg_exti_source;
	uint32_t syscfg_exti_line;
	
	if(gpio == GPIOA) syscfg_exti_source = LL_SYSCFG_EXTI_PORTA;
	else if(gpio == GPIOB) syscfg_exti_source = LL_SYSCFG_EXTI_PORTB;
	else if(gpio == GPIOC) syscfg_exti_source = LL_SYSCFG_EXTI_PORTC;
	else if(gpio == GPIOD) syscfg_exti_source = LL_SYSCFG_EXTI_PORTD;
	
	switch(exti_line)
	{
		case LL_EXTI_LINE_0:
			syscfg_exti_line = LL_SYSCFG_EXTI_LINE0;
		break;
		
		case LL_EXTI_LINE_1:
			syscfg_exti_line = LL_SYSCFG_EXTI_LINE1;
		break;
		
		case LL_EXTI_LINE_2:
			syscfg_exti_line = LL_SYSCFG_EXTI_LINE2;
		break;
		
		case LL_EXTI_LINE_3:
			syscfg_exti_line = LL_SYSCFG_EXTI_LINE3;
		break;
		
		case LL_EXTI_LINE_4:
			syscfg_exti_line = LL_SYSCFG_EXTI_LINE4;
		break;
		
		case LL_EXTI_LINE_5:
			syscfg_exti_line = LL_SYSCFG_EXTI_LINE5;
		break;
		
		case LL_EXTI_LINE_6:
			syscfg_exti_line = LL_SYSCFG_EXTI_LINE6;
		break;
		
		case LL_EXTI_LINE_7:
			syscfg_exti_line = LL_SYSCFG_EXTI_LINE7;
		break;
		
		case LL_EXTI_LINE_8:
			syscfg_exti_line = LL_SYSCFG_EXTI_LINE8;
		break;
		
		case LL_EXTI_LINE_9:
			syscfg_exti_line = LL_SYSCFG_EXTI_LINE9;
		break;
		
		case LL_EXTI_LINE_10:
			syscfg_exti_line = LL_SYSCFG_EXTI_LINE10;
		break;
		
		case LL_EXTI_LINE_11:
			syscfg_exti_line = LL_SYSCFG_EXTI_LINE11;
		break;
		
		case LL_EXTI_LINE_12:
			syscfg_exti_line = LL_SYSCFG_EXTI_LINE12;
		break;
		
		case LL_EXTI_LINE_13:
			syscfg_exti_line = LL_SYSCFG_EXTI_LINE13;
		break;
		
		case LL_EXTI_LINE_14:
			syscfg_exti_line = LL_SYSCFG_EXTI_LINE14;
		break;
		
		case LL_EXTI_LINE_15:
			syscfg_exti_line = LL_SYSCFG_EXTI_LINE15;
		break;
	}
	
	LL_SYSCFG_SetEXTISource(syscfg_exti_source,syscfg_exti_line);
}*/
//

uint32_t GetExtiLine(uint32_t pin)
{
	switch(pin)
	{
		case LL_GPIO_PIN_0:
			return LL_EXTI_LINE_0;
		
		case LL_GPIO_PIN_1:
			return LL_EXTI_LINE_1;
		
		case LL_GPIO_PIN_2:
			return LL_EXTI_LINE_2;
		
		case LL_GPIO_PIN_3:
			return LL_EXTI_LINE_3;
		
		case LL_GPIO_PIN_4:
			return LL_EXTI_LINE_4;
		
		case LL_GPIO_PIN_5:
			return LL_EXTI_LINE_5;
		
		case LL_GPIO_PIN_6:
			return LL_EXTI_LINE_6;
		
		case LL_GPIO_PIN_7:
			return LL_EXTI_LINE_7;
		
		case LL_GPIO_PIN_8:
			return LL_EXTI_LINE_8;
		
		case LL_GPIO_PIN_9:
			return LL_EXTI_LINE_9;
		
		case LL_GPIO_PIN_10:
			return LL_EXTI_LINE_10;
		
		case LL_GPIO_PIN_11:
			return LL_EXTI_LINE_11;
		
		case LL_GPIO_PIN_12:
			return LL_EXTI_LINE_12;
		
		case LL_GPIO_PIN_13:
			return LL_EXTI_LINE_13;
		
		case LL_GPIO_PIN_14:
			return LL_EXTI_LINE_14;
		
		case LL_GPIO_PIN_15:
			return LL_EXTI_LINE_15;
	}
	
	return 0;
}
//

void EnableTimIRQn(TIM_TypeDef* tim,uint8_t priority)
{
	if(tim == TIM1)
	{
		NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
		NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn,priority);
	}
	else if(tim == TIM2)
	{
		NVIC_EnableIRQ(TIM2_IRQn);
		NVIC_SetPriority(TIM2_IRQn,priority);
	}
	else if(tim == TIM3)
	{
		NVIC_EnableIRQ(TIM3_IRQn);
		NVIC_SetPriority(TIM3_IRQn,priority);
	}
	else if(tim == TIM6)
	{
		NVIC_EnableIRQ(TIM6_DAC_LPTIM1_IRQn);
		NVIC_SetPriority(TIM6_DAC_LPTIM1_IRQn,priority);
	}

}
//

void DisableExtiIRQn(uint32_t exti_line)
{
	switch(exti_line)
	{
		case LL_EXTI_LINE_0:
		case LL_EXTI_LINE_1:
			NVIC_DisableIRQ(EXTI0_1_IRQn);
		break;
		
		case LL_EXTI_LINE_2:
		case LL_EXTI_LINE_3:
			NVIC_DisableIRQ(EXTI2_3_IRQn);
		break;
		
		case LL_EXTI_LINE_4:
		case LL_EXTI_LINE_5:
		case LL_EXTI_LINE_6:
		case LL_EXTI_LINE_7:
		case LL_EXTI_LINE_8:
		case LL_EXTI_LINE_9:
		case LL_EXTI_LINE_10:
		case LL_EXTI_LINE_11:
		case LL_EXTI_LINE_12:
		case LL_EXTI_LINE_13:
		case LL_EXTI_LINE_14:
		case LL_EXTI_LINE_15:
			NVIC_DisableIRQ(EXTI4_15_IRQn);
		break;
	}
}
//

void EnableDmaIRQn(DMA_TypeDef* dma, uint32_t channel,uint8_t priority)
{
	
	switch(channel)
	{
		case LL_DMA_CHANNEL_1:
			NVIC_EnableIRQ(DMA1_Channel1_IRQn);
			NVIC_SetPriority(DMA1_Channel1_IRQn,priority);
			return;
		
		case LL_DMA_CHANNEL_2:
		case LL_DMA_CHANNEL_3:
			NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
			NVIC_SetPriority(DMA1_Channel2_3_IRQn,priority);
		return;
		
		case LL_DMA_CHANNEL_4:
		case LL_DMA_CHANNEL_5:
		case LL_DMA_CHANNEL_6:
		case LL_DMA_CHANNEL_7:
			NVIC_EnableIRQ(DMA1_Ch4_7_DMAMUX1_OVR_IRQn);
			NVIC_SetPriority(DMA1_Ch4_7_DMAMUX1_OVR_IRQn,priority);
			return;
	}

	
}
//
