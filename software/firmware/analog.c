#include <p33fj128mc706.h>
#include <generic.h>
#include <string.h>
#include "defs.h"
#include "spi_sw.h"
#include "analog.h"

struct __ANALOG_CONFIG analog_config;

void Analog_Init(void)
{
	memset(&analog_config, 0, sizeof(struct __ANALOG_CONFIG));
	
	// domyslne napiecie referencyjne
	analog_config.dac1.value = 1364; // 1.66V
	analog_config.dac2.value = 1364; // 1.66V
	analog_config.dac3.value = 1364; // 1.66V
}


void Analog_Update(void)
{
	
	// wylacz sprzêtowe SPI
	SPI2STATbits.SPIEN = 0;
	while(SPI2STATbits.SPITBF);	
	
	//config.dac1.value = 1024; // 1.25V
	//config.dac3.value = 1638; // 2.00V
	//config.dac2.value = 2048; // 2.50V
	//config.dac3.value = 2457; // 3.00V
	
	//analog_config.amp1.GAIN = 3;
	//config.amp3.GAIN = 3;
	//config.amp2.GAIN = 1;
	//config.amp3.GAIN = 1;
	/*
	0 = Gain of +1 (Default)
	1 = Gain of +2
	2 = Gain of +4
	3 = Gain of +5
	4 = Gain of +8
	5 = Gain of +10
	6 = Gain of +16
	7 = Gain of +32
	*/
	
	
	
	// dac.value = ???
	analog_config.dac1.SHDN = 1;
	analog_config.dac1.GA = 1;
	analog_config.dac2.SHDN = 1;
	analog_config.dac2.GA = 1;
	analog_config.dac3.SHDN = 1;
	analog_config.dac3.GA = 1;

	// amp.GAIN = ??	
	analog_config.amp1.A0 = 0; // rejestr wzmocnienia
	analog_config.amp1.MODE = 0b010; // zapis do rejestru
	analog_config.amp2.A0 = 0; // rejestr wzmocnienia
	analog_config.amp2.MODE = 0b010; // zapis do rejestru
	analog_config.amp3.A0 = 0; // rejestr wzmocnienia
	analog_config.amp3.MODE = 0b010; // zapis do rejestru
	analog_config.amp_zero.A0 = 0; // rejestr wzmocnienia
	analog_config.amp_zero.MODE = 0b010; // zapis do rejestru


	// ustawienie wzmocnien ZEROWYCH
	DEV_ResetSPI(0x21);
	DEV_WriteSPI16(0x21, analog_config.amp_zero.command);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	DEV_ResetSPI(0x22);
	DEV_WriteSPI16(0x22, analog_config.amp_zero.command);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	DEV_ResetSPI(0x23);
	DEV_WriteSPI16(0x23, analog_config.amp_zero.command);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	
	
	// ustawienie napiec referencyjnych
	DEV_ResetSPI(0x11);
	DEV_WriteSPI16(0x11, analog_config.dac1.command);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	DEV_ResetSPI(0x12);
	DEV_WriteSPI16(0x12, analog_config.dac2.command);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	DEV_ResetSPI(0x13);
	DEV_WriteSPI16(0x13, analog_config.dac3.command);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	
	// ustawienie wzmocnien 
	DEV_ResetSPI(0x21);
	DEV_WriteSPI16(0x21, analog_config.amp1.command);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	DEV_ResetSPI(0x22);
	DEV_WriteSPI16(0x22, analog_config.amp2.command);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	DEV_ResetSPI(0x23);
	DEV_WriteSPI16(0x23, analog_config.amp3.command);
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	
	
	// wlacz sprzetowe SPI
	SPI2STATbits.SPIEN = 1;
	while(SPI2STATbits.SPITBF);	

}
