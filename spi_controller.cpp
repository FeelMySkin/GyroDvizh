#include "spi_controller.h"

SPIController::SPIController()
{
	
}
//

SPIController::~SPIController()
{
	
}
//

void SPIController::Init(SPI_Struct spi_info)
{
	spi = spi_info;
	InitGPIO();
	InitSPI();
}
//

void SPIController::InitGPIO()
{
	LL_GPIO_InitTypeDef gpio;
	gpio.Alternate = spi.af;
	gpio.Mode = LL_GPIO_MODE_ALTERNATE;
	gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	gpio.Pull = LL_GPIO_PULL_NO;
	gpio.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	
	gpio.Pin = spi.miso_pin;
	LL_GPIO_Init(spi.miso_gpio,&gpio);
	gpio.Pin = spi.mosi_pin;
	LL_GPIO_Init(spi.mosi_gpio,&gpio);
	gpio.Pin = spi.sck_pin;
	LL_GPIO_Init(spi.sck_gpio,&gpio);
	
}
//

void SPIController::InitSPI()
{
	LL_SPI_SetRxFIFOThreshold(this->spi.spi,LL_SPI_RX_FIFO_TH_QUARTER);
	
	LL_SPI_InitTypeDef spi;
	spi.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV16;
	spi.BitOrder = LL_SPI_MSB_FIRST;
	spi.ClockPhase = LL_SPI_PHASE_2EDGE;
	spi.ClockPolarity = LL_SPI_POLARITY_HIGH;
	spi.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
	spi.DataWidth = LL_SPI_DATAWIDTH_8BIT;
	spi.Mode = LL_SPI_MODE_MASTER;
	spi.NSS = LL_SPI_NSS_SOFT;
	spi.TransferDirection = LL_SPI_FULL_DUPLEX;
	LL_SPI_Init(this->spi.spi,&spi);
	
	LL_SPI_Enable(this->spi.spi);
	
}
//

uint8_t SPIController::Transmit(uint8_t byte)
{
	LL_SPI_TransmitData8(this->spi.spi,byte);
	while(!LL_SPI_IsActiveFlag_RXNE(this->spi.spi)) ;
	
	return LL_SPI_ReceiveData8(this->spi.spi);
}
//
