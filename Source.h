#ifndef SOURCE_H
#define SOURCE_H

#include "defines.h"
#include "dac_controller.h"
#include "spi_controller.h"
#include "mpu_controller.h"
#include "adc_controller.h"

DACController dac;
SPIController spi1;
MPUController mpu1;
ADCController adc;

double DutyVoltage();

#endif
