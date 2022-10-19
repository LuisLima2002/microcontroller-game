/*
 * nokia5110.h
 *
 *  Created on: 5 de mar de 2022
 *      Author: luise
       3.3V          (VCC, pin 1) power
       Ground        (GND, pin 2) ground
       SSI0Fss       (SCE, pin 3) connected to PA3
       Reset         (RST, pin 4) connected to PA7
       Data/Command  (D/C, pin 5) connected to PA6
       SSI0Tx        (DN,  pin 6) connected to PA4
       SSI0Clk       (SCLK, pin 7) connected to PA2
       back light    (LED, pin 8) connected to PA6
 */

#ifndef NOKIA5110_H_
#define NOKIA5110_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"

void enableSSI0         (void);
void lcdWrite           (uint8_t dc,uint8_t message);
void nokia5510init      (uint32_t SysClockFreq);
void nokia5110_SetCursor(uint8_t newX, uint8_t newY);
void nokia5110_ClrPxl   (uint32_t j, uint32_t i);
void nokia5110_SetPxl   (uint32_t j, uint32_t i);
void nokia5110_DrawFullImage(const uint8_t *ptr);
void nokia5110_displayScreen(void);
void nokia5110_OutChar  (char data);
void nokia5110_OutString(char *ptr);
void nokia5110_OutDec   (uint16_t n);
void nokia5110_Clear    (void);

#endif /* NOKIA5110_H_ */
