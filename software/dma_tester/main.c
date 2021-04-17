#include <p33fj128mc706.h>
#include <stddef.h>

#include <generic.h>
#include "comm.h"

#define UxSTA	U1STAbits
#define UxTXREG	U1TXREG
#define UxRXREG U1RXREG

unsigned char rs_tx[32] __attribute__((far, space(dma))) = {'a', 'b', 'c', 'd', 'e'};


void putc(char c)
{
	while(UxSTA.UTXBF);
    UxTXREG = c;
}

void puts(const char* str)
{
	while(*str)
		putc(*str++);
}	

char getc(void)
{
	while(!DataRdyUART1());
	return UxRXREG;
}	

char* gets(char* str)
{
	char* ptr = str;
	char c;
	
	while(DataRdyUART1())
		c = UxRXREG;

	while(TRUE)
	{
		while(!DataRdyUART1());
		c = UxRXREG;
		
		if ((c == '\n') || (c == '\r'))
		{
			*ptr = '\x0';
			putc('\n');
			return str;
		}
		
		putc(c);
			
		*ptr = c;
		ptr++;
	}
}



int main(void)
{
	// UART 2
	// Fosc = 20MHz
	// Fcy = Fosc/2;
	
	// ### BRGH = 0
	// UxBRG = ((Fcy / BaudRate) / 16 - 1)
	// 9600 = 64
	// 19200 = 32
	
	// ### BRGH = 1
	// UxBRG = Fcy / (4xBaudRate) - 1
	// 9600 = 259
	// 19200 = 129
	// 57600 = 42
	U1MODE = 0x0000;
	U1BRG = 129;
	U1MODEbits.BRGH = TRUE;

	
//	IEC0bits.U1TXIE = FALSE;
//	IEC0bits.U1RXIE = FALSE;
//	IEC4bits.U1EIE = FALSE;
//	
//	IFS0bits.U1TXIF = FALSE;
//	IFS0bits.U1RXIF = FALSE;
//	IFS4bits.U1EIF = FALSE;
	
//	__C30_UART = 2;
	
	
	//puts("1 Ala ma kota\n2 Ala ma kota\n3 Ala ma kota\n");
	
	//putc('q');

	DMA0CONbits.SIZE = 1;
	DMA0CONbits.DIR = 1;
	DMA0CONbits.MODE = 1; // one-shot, pingpong disabled
	DMA0REQbits.IRQSEL = 0b0001100;
	
	DMA0PAD = (volatile unsigned int) &U1TXREG;
	DMA0STA = __builtin_dmaoffset(rs_tx);
	DMA0STB = __builtin_dmaoffset(rs_tx);
	DMA0CNT = 4;
	
	IFS0bits.DMA0IF  = 0;
	IEC0bits.DMA0IE  = 1;
	DMA0CONbits.CHEN = 1;
	
	
	
	U1MODEbits.UARTEN = TRUE;
	U1STAbits.UTXEN = TRUE;

	while(1);

	DMA0REQbits.FORCE = 1;


	return 0;	
}


void __attribute__((interrupt,no_auto_psv)) _DMA0Interrupt(void)
{
	IFS0bits.DMA0IF = 0;  //Clear the DMA0 Interrupt Flag
}

