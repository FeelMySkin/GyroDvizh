#ifndef SPI_CONTROLLER_H
#define SPI_CONTROLLER_H

#include "defines.h"

struct SPI_Struct
{
	GPIO_TypeDef*	miso_gpio;
	GPIO_TypeDef*	mosi_gpio;
	GPIO_TypeDef*	sck_gpio;
	uint32_t			miso_pin;
	uint32_t			mosi_pin;
	uint32_t			sck_pin;
	uint32_t			af;
	SPI_TypeDef*	spi;
};

class SPIController
{
	public:
		SPIController();
		~SPIController();
		void Init(SPI_Struct);
		uint8_t Transmit(uint8_t byte);
		
		
	private:
		void InitGPIO();
		void InitSPI();
	
		SPI_Struct spi;
};
//

#endif
