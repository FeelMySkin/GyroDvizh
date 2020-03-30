#ifndef PERIPH_USE_H
#define PERIPH_USE_H

#include "stdint.h"
#include "stdio.h"
#include "stm32g071xx.h"
#include "stm32g0xx.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_tim.h"
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_adc.h"
#include "stm32g0xx_ll_dac.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_spi.h"
#include "stm32g0xx_ll_rcc.h"

#ifdef __cplusplus
extern "C"
{
#endif
	
	void EnableExtiIRQn(uint32_t exti_line,uint8_t priority);
	void EnableTimIRQn(TIM_TypeDef* tim,uint8_t priority);
	void EnableDmaIRQn(DMA_TypeDef* dma, uint32_t channel,uint8_t priority);
	void DisableExtiIRQn(uint32_t exti_line);
	
	//void SetExtiSource(GPIO_TypeDef* gpio,uint32_t exti_line);
	uint32_t GetExtiLine(uint32_t pin);
	
#ifdef __cplusplus
}
#endif

/*GPIO*/
#define DAC_CH1_GPIO			GPIOA
#define DAC_CH2_GPIO			GPIOA
#define DAC_CH1_PIN				LL_GPIO_PIN_4
#define DAC_CH2_PIN				LL_GPIO_PIN_5

#define SPI1_MISO_GPIO		GPIOB
#define SPI1_MOSI_GPIO		GPIOB
#define SPI1_SCK_GPIO			GPIOB
#define SPI1_MISO_PIN			LL_GPIO_PIN_4
#define SPI1_MOSI_PIN			LL_GPIO_PIN_5
#define SPI1_SCK_PIN			LL_GPIO_PIN_3
#define SPI1_AF						LL_GPIO_AF_0

#define ADC_CH1_GPIO			GPIOA
#define ADC_CH2_GPIO			GPIOA
#define ADC_CH1_PIN				LL_GPIO_PIN_6
#define ADC_CH2_PIN				LL_GPIO_PIN_7

#define MPU_CS_GPIO				GPIOB
#define MPU_CS_PIN				LL_GPIO_PIN_6

#define GYRO_ON_GPIO			GPIOA
#define GYRO_ON_PIN				LL_GPIO_PIN_10 //OUT 4

#define BRAKE_BUTTON_GPIO	GPIOC
#define BRAKE_BUTTON_PIN	LL_GPIO_PIN_7 //OUT 3
#define BRAKE_ACTIVE_GPIO	GPIOC
#define BRAKE_ACTIVE_PIN	LL_GPIO_PIN_6 //OUT 2

/*DAC*/
#define DAC_CH1_CHANNEL		LL_DAC_CHANNEL_1
#define DAC_CH2_CHANNEL		LL_DAC_CHANNEL_2

/*TIM*/
#define MPU_UPDATE_TIM		TIM3
#define MPU_UPDATE_PERIOD	10	//ms

#define BRAKE_PAUSE_TIM		TIM6
#define BRAKE_PAUSE_WAIT	100	//ms

/*ADC*/
#define ADC_CH1_CHANNEL		LL_ADC_CHANNEL_6
#define ADC_CH2_CHANNEL		LL_ADC_CHANNEL_7

/*IDs*/
#define DAC_CH1_ID				"DAC1"
#define DAC_CH2_ID				"DAC2"

#define ADC_CH1_ID				"ADC1"
#define ADC_CH2_ID				"ADC2"

/*SPI*/

/*DMA*/
#define ADC_DMA_CH				LL_DMA_CHANNEL_1
#define ADC_DMA						DMA1


#endif
