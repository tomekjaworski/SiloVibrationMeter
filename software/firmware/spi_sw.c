#include <p33fj128mc706.h>
#include <generic.h>
#include <string.h>
#include "defs.h"

#define NOPx4 {__delay32(5000+2000);}


void InitSPI(void)
{
	// inicjowanie SPI2
	SPI2CON1 = 0x0000;
	SPI2CON1bits.MODE16 = 0; // 8 bitów
	
	//SPI2CON1bits.MODE16 = 1; // 8 bitów
	
	SPI2CON1bits.SSEN = 0; // ?
	SPI2CON1bits.MSTEN = 1; // master
	
	// preskaler
	//SPI2CON1bits.PPRE = 0b00;
	//SPI2CON1bits.SPRE = 0b000;
	SPI2CON1bits.PPRE = 0b11; // 1:1
	SPI2CON1bits.SPRE = 0b110; // 2:1
	// 40mhz/2 = 20mhz
	
	// ramki spi??
	SPI2CON2bits.FRMEN = 0;
	SPI2CON2bits.FRMPOL = 1;
	

	IEC2bits.SPI2IE = 0;
	IFS2bits.SPI2IF = 0;
	
	SPI2STATbits.SPIROV = 0;
	

	// uruchomienie SPI		
	SPI2STATbits.SPIEN = 1;

	while(SPI2STATbits.SPITBF);	

	// wszystkie linie CS s¹ niekatywne	
	CS_ENC = 1;
	
	CS_REF1 = 1;
	CS_REF2 = 1;
	CS_REF3 = 1;
	
	NEG_SET(CS_AMP1, 1);
	NEG_SET(CS_AMP2, 1);
	NEG_SET(CS_AMP3, 1);

}	


void DEV_ResetSPI(unsigned char devid)
{		
	CS_REF1 = 1;
	CS_REF2 = 1;
	CS_REF3 = 1;

	NEG_SET(CS_AMP1, 1);
	NEG_SET(CS_AMP2, 1);
	NEG_SET(CS_AMP3, 1);
		
/*		
	if (devid == 0x11)
		CS_REF1 = 1;
	if (devid == 0x12)
		CS_REF2 = 1;
	if (devid == 0x13)
		CS_REF3 = 1;

	if (devid == 0x21)
		NEG_SET(CS_AMP1, 1);
	if (devid == 0x22)
		NEG_SET(CS_AMP2, 1);
	if (devid == 0x23)
		NEG_SET(CS_AMP3, 1);
*/		
		
	if ((devid & 0xF0) == 0x10)
	{
		SDO = 0;
		SCK = 0;
	}
	
	if ((devid & 0xF0) == 0x20)
	{
		NEG_SET(SDO, 0);
		NEG_SET(SCK, 0);
	}
	
	NOPx4;
}

void DEV_WriteSPI16(unsigned char devid, WORD value)
{
//	BYTE temp;
	WORD_VAL w;
	w.Val = value;

//	if ((devid & 0xF0) == 0x20)
//	{
//		// zamiana kolejnosci bajtow
//		temp = w.byte.HB;
//		w.byte.HB = w.byte.LB;
//		w.byte.LB = temp;
//	}
		
	if (devid == 0x11)
		CS_REF1 = 0;
	if (devid == 0x12)
		CS_REF2 = 0;
	if (devid == 0x13)
		CS_REF3 = 0;
		
	if (devid == 0x21)
		NEG_SET(CS_AMP1, 0);
	if (devid == 0x22)
		NEG_SET(CS_AMP2, 0);
	if (devid == 0x23)
		NEG_SET(CS_AMP3, 0);
				
	NOPx4;
	int i;
	
	// wersja wersja prosta
	if ((devid & 0xF0) == 0x10)
		for (i = 0; i < 16; i++)
		{
			if (w.bits.b15)
				SDO = 1;
			else
				SDO = 0;
				
			NOPx4;
			SCK = 1;
			NOPx4;
			SCK = 0;
			
			w.Val <<= 1;
		}
	
	// wersja zanegowana
	if ((devid & 0xF0) == 0x20)
		for (i = 0; i < 16; i++)
		{
			if (w.bits.b15)
				NEG_SET(SDO, 1)
			else
				NEG_SET(SDO, 0);
				
			NOPx4;
			NEG_SET(SCK, 1);
			NOPx4;
			NEG_SET(SCK, 0);
			
			w.Val <<= 1;
		}
	
	NOPx4;
	if (devid == 0x11)
		CS_REF1 = 1;
	if (devid == 0x12)
		CS_REF2 = 1;
	if (devid == 0x13)
		CS_REF3 = 1;

	if (devid == 0x21)
		NEG_SET(CS_AMP1, 1);
	if (devid == 0x22)
		NEG_SET(CS_AMP2, 1);
	if (devid == 0x23)
		NEG_SET(CS_AMP3, 1);

	if ((devid & 0xF0) == 0x10)
		SDO = 0;
	if ((devid & 0xF0) == 0x20)
		NEG_SET(SDO, 0);
		
	NOPx4;
}	

//
//void AMP_ResetSPI(unsigned char ampid)
//{		
//	if (ampid == 1)
//		NEG_SET(CS_AMP1, 1);
//	if (ampid == 2)
//		NEG_SET(CS_AMP2, 1);
//	if (ampid == 3)
//		NEG_SET(CS_AMP3, 1);
//		
//	NEG_SET(SDO, 0);
//	NEG_SET(SCK, 0);
//}
//
//void AMP_WriteSPI16(unsigned char ampid, WORD value)
//{
//	BYTE temp;
//	WORD_VAL w;
//	w.Val = value;
//
//	temp = w.byte.HB;
//	w.byte.HB = w.byte.LB;
//	w.byte.LB = temp;
//	
//	
//	
//	
//	if (ampid == 1)
//		NEG_SET(CS_AMP1, 0);
//	if (ampid == 2)
//		NEG_SET(CS_AMP2, 0);
//	if (ampid == 3)
//		NEG_SET(CS_AMP3, 0);
////		NOPx4;
////		NOPx4;
////		NOPx4;
////		NOPx4;
////		NOPx4;
////		NOPx4;
//		NOPx4;
//	int i;
//	for (i = 0; i < 16; i++)
//	{
//		if (w.bits.b15)
//			NEG_SET(SDO, 1)
//		else
//			NEG_SET(SDO, 0);
//			
//	//	NOPx4;
////		NOPx4;
//		NOPx4;
//		NEG_SET(SCK, 1);
//		NOPx4;
//	//	NOPx4;
//	//	NOPx4;
//	//	NOPx4;
//		NEG_SET(SCK, 0);
////		NOPx4;
////		NOPx4;
////		NOPx4;
//		
//		w.Val <<= 1;
//	}
//	
//	if (ampid == 1)
//		NEG_SET(CS_AMP1, 1);
//	if (ampid == 2)
//		NEG_SET(CS_AMP2, 1);
//	if (ampid == 3)
//		NEG_SET(CS_AMP3, 1);
////	NOPx4;
//	NEG_SET(SDO, 0);
////		NOPx4;
////		NOPx4;
////		NOPx4;
////		NOPx4;
////		NOPx4;
////		NOPx4;		NOPx4;
////		NOPx4;
////		NOPx4;
////		NOPx4;
////		NOPx4;
//		NOPx4;
//}	
//

//
//

//void OpenSPI(void)
//{		
//	SPI_TRIS_CS = 0;
//	SPI_TRIS_SDO = 0;
//	SPI_TRIS_SCK = 0;
//
//	CS = 1;
//	CLK = 0;
//	DO = 0;
//	
//	__delay32(5000); // 500us
//		
//}
//
//void WriteSPI(WORD value)
//{
//	WORD_VAL w;
//	w.Val = value;
//	
//	CS = 0;
//	int i;
//	for (i = 0; i < 16; i++)
//	{
//		if (w.bits.b15)
//			DO = 1;
//		else
//			DO = 0;
//			
//		nop4;
//		CLK = 1;
//		nop4;
//		CLK = 0;
//		
//		w.Val <<= 1;
//	}
//	CS = 1;
//	nop4;
//	DO = 0;
//}
//
//
//

