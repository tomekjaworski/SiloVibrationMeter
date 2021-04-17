#include <p33fj128mc706.h>
#include <generic.h>
#include <string.h>
#include "defs.h"

void InitCPU(void)
{
#if defined(FULL_SPEED)
	// konfiguracja petli PLL na 40MIPSów
	CLKDIVbits.PLLPRE = 5-2; // N1 = 5
	PLLFBDbits.PLLDIV = 40-2; // M = 40
	CLKDIVbits.PLLPOST = 0;// N2 = 2
	
	__builtin_write_OSCCONH(0x03);
	__builtin_write_OSCCONL(0x01);
	
	while(OSCCONbits.COSC != 0b011);
	while(OSCCONbits.LOCK != 1);		
#endif

	// porty
	//TRISA = 0b0000000000011111;
	LATB = 0;
	LATC = 0;
	LATD = 0;
	LATE = 0;
	LATF = 0;
	LATG = 0;
	
	TRISB = 0b0000000000001111;
	TRISD = 0b0000000000000000;
	TRISE = 0b0000000000000000;
	TRISF = 0b0000000001000000;  // INT
	TRISG = 0b0000000010000000; // SDI
	
	// startowe stany diod LED
	LED0 = 1;
	LED1 = 1;
	LED2 = 1;


	// inicjowanie przetwornika ADC
	AD1CON1bits.AD12B = 1; // 12 bitów
	AD2CON1bits.AD12B = AD1CON1bits.AD12B;
	
	// AN0 = Vref+
	// AN1 = X
	// AN2 = Y
	// AN3 = Z
	// AN4 = temperatura
	AD1PCFGL = 0b1111111111100000; // 1-digit; 0-anal
	AD2PCFGL = 0b1111111111100000;
	
	// sterowanie samplowaniem i konwersja
	AD1CON1bits.ASAM = 1;
	AD2CON1bits.ASAM = AD1CON1bits.ASAM;
	
	// wyzwalanie konwersji
	// 000 - recznie
	AD1CON1bits.SSRC = 0b111; 
	AD2CON1bits.SSRC = AD1CON1bits.SSRC; 
	
	// format wypluwania danych
	AD1CON1bits.FORM = 0b00;
	AD2CON1bits.FORM = AD1CON1bits.FORM;	

	// Ÿródlo napiecia referencyjnego
	AD1CON2bits.VCFG = 0b001; // Vref+=port; Vref-=Vss
	AD1CON2bits.VCFG = AD1CON2bits.VCFG;

	// czas Tad
	//AD1CON3bits.ADCS = 64-1; // Tad = x * Tcy; Tcy=25ns(40MHz), Tad=64*25=1.6us
	AD1CON3bits.ADCS = 5; // Tad = x * Tcy; Tcy=25ns (40MHz), Tad=4*25=100ns
	AD2CON3bits.ADCS = AD1CON3bits.ADCS;


	// Ÿród³o zegara taktuj¹cego
	AD1CON3bits.ADRC = 0; // wewnêtrzna magistrala zegara
	AD2CON3bits.ADRC = AD1CON3bits.ADRC;
	
	// czas samplowania,
	AD1CON3bits.SAMC = 4; // x * Tad; 4*100ns=400ns
	AD2CON3bits.SAMC = AD1CON3bits.SAMC;

	// wybor kanalow. dla 12 bitow jest tylko ch0
	AD1CON2bits.CHPS = 0;
	AD2CON2bits.CHPS = AD1CON2bits.CHPS;
//	
//#if (defined(__DEBUG) && defined(__MPLAB_DEBUGGER_ICD3)) || !defined(__DEBUG)
//	// start przetwornika
//	//AD1CON1bits.ADON = 1;
//	//AD2CON1bits.ADON = AD1CON1bits.ADON;
//#endif	
	
	
	SDO = 0;	
	SCK = 0;	
	
}

