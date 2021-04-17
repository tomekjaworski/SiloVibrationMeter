//
// arp -s 169.254.225.1 00-1d-72-ca-9e-e0
//
#include <p33fj128mc706.h>
#include <generic.h>
#include <string.h>
#include "spi_sw.h"
#include "defs.h"
#include "enc.h"
#include "delay.h"
#include "net.h"

void InitCPU(void);
void InitENC(void);
void InitSPI(void);

//static BYTE ENC_ReadRegister(BYTE reg_addr);



unsigned char bufor[1600] __attribute__((near));


struct ETH
{
	IP_ADDR ip;
	ETH_ADDR mac;
	WORD ip_id;
	
} eth0;

unsigned char __mac[6] = {0x00, 0x1d, 0x72, 0xCA, 0x9E, 0xe0};


unsigned short s1[5] __attribute__((space(dma))) = {};
unsigned short s2[5] __attribute__((space(dma))) = {};

WORD __attribute__((far)) tab[6000];
WORD __attribute__((far)) *tab_ptr;
WORD __attribute__((far)) *tab_end;


union __DAC_COMMAND
{
	WORD command;
	struct
	{
		unsigned value	: 12;
		unsigned SHDN	: 1;
		unsigned GA		: 1;
		unsigned BUF	: 1;
		unsigned AB		: 1;
	};	
};	

		union XXX
		{
			struct // A0 = 0 - gain register
			{
				unsigned char GAIN		: 3;
				unsigned char __reserved : 5;
			};
			
			struct // A0 = 1 - channel register
			{
				unsigned char CHANNEL	: 3;
				unsigned char __reserved	: 5;
			};	
		};	

union __AMP_COMMAND
{
	WORD command;
	
	struct
	{
		unsigned char A0			: 1;
		unsigned char __reserved	: 4;
		unsigned char MODE		: 3;
		
		union
		{
			struct // A0 = 0 - gain register
			{
				unsigned char GAIN		: 3;
				unsigned char __reserved : 5;
			};
			
			struct // A0 = 1 - channel register
			{
				unsigned char CHANNEL	: 3;
				unsigned char __reserved	: 5;
			};	
		};	
	};	
};	

union __DAC_COMMAND dac;
union __AMP_COMMAND amp;
WORD dummy;

int main(void)
{
	int c = sizeof(union __AMP_COMMAND);

	InitCPU();
	InitSPI();
	//InitENC();
	
	memcpy(eth0.mac.byte, __mac, 6);
	eth0.ip.address = inet_addr("212.191.89.67");
	//eth0.ip.address = inet_addr("169.254.1.1");
	ENC_SetHardwareAddress(&eth0.mac);
	eth0.ip_id = 0x1000;

	dac.command = 0x0000;
	dac.SHDN = 1;
	dac.GA = 1;
	
	//CS_REF = 1;
	//CS_AMP = 1;
	
	__delay32(1000);
	
	CS_REF = 0;
	dummy = SPI2BUF;
	SPI_SEND_AND_WAIT(dac.command);
	CS_REF = 1;
	
	asm("nop");
	asm("nop");
	asm("nop");

	dac.value = 1310;	
	CS_REF = 0;
	dummy = SPI2BUF;
	SPI_SEND_AND_WAIT(dac.command);
	CS_REF = 1;	
	
//	amp.command = 0x0000;
//	amp.A0 = 0; // rejestr wzmocnienia
//	amp.MODE = 0b010; // zapis do rejestru
//	amp.GAIN = 0x00;
//	
//	CS_AMP = 0;
//	dummy = SPI2BUF;
//	SPI_SEND_AND_WAIT(amp.command);
//	CS_AMP = 1;	



	SPI2STATbits.SPIEN = 0;
	while(SPI2STATbits.SPITBF);	





AMP_ResetSPI();
	
while(1)
{
	asm("nop");
	asm("nop");
	asm("nop");

	__delay32(100);
	
	
	amp.command = 0x0000;
	amp.A0 = 0; // rejestr wzmocnienia
	amp.MODE = 0b010; // zapis do rejestru
	amp.GAIN = 0x03;

	AMP_WriteSPI16(amp.command);

//	
//	CS_AMP = 0;
//	dummy = SPI2BUF;
//	SPI_SEND_AND_WAIT(amp.command);
//	CS_AMP = 1;	

	
	asm("nop");
	asm("nop");
	asm("nop");
}

//	AD1CON1bits.ADON = 0;
//
//
//	// kana³ 0 to Ÿród³o referencyjne
//	AD1CHS0bits.CH0SA = 1;
//	AD1CHS0bits.CH0SB = 2;
//	AD2CHS0bits.CH0SA = 3;
//	AD2CHS0bits.CH0SB = 4;
//
//	AD1CON2bits.ALTS = 1;
//	AD1CON2bits.SMPI = 1;
//	AD1CON1bits.ADDMABM = 1;
//
//	AD1CON1bits.ADON = 1;
//
//	DMA0CONbits.AMODE = 0; // Configure DMA for Register indirect with post increment
//	DMA0CONbits.MODE = 2; // Configure DMA for Continuous Ping-Pong mode
//	
//	DMA0PAD = (unsigned short)&ADC1BUF0;
//	DMA0CNT = 1;
//	DMA0REQ = 13; // adc1
//	
//	DMA0STA = __builtin_dmaoffset(s1);
//	DMA0STB = __builtin_dmaoffset(s2);
//	
//	IFS0bits.DMA0IF = 0; 
//	IEC0bits.DMA0IE = 1; 
//	DMA0CONbits.CHEN = 1;
	

	
	long i = 0;
	tab_ptr = tab;
	tab_end = tab + sizeof(tab) / sizeof(WORD);
	while(1)
	{
		LED0 = 0;
		__delay32(15000);
		
		//AD1CHS0bits.CH0SB = 4;
		
		AD1CON1bits.DONE = 0;
//		AD2CON1bits.DONE = 0;
		while(!AD1CON1bits.DONE);
//		while(!AD2CON1bits.DONE);
		
		unsigned short a1;
		a1 = ADC1BUF0;
//		a2 = ADC2BUF0;

		
		
		*tab_ptr = a1;
		tab_ptr++;
		if (tab_ptr == tab_end)
		{
			LED0 = 1;
			//i = 0;
			tab_ptr = tab;
			asm("nop");
			asm("nop");
			asm("nop");
		}	
		
	}	
	
//	while(1);


	unsigned char q3;
	unsigned short len;
	
	ETH_HEADER *eth_header;
	IP_HEADER *ip_header;
	UDP_HEADER *udp_header;
	ICMP_HEADER *icmp_header;

//	long i;
	LED2 = 0;
	for(i = 0; i < 100000; i++)
	if ((i % 10000) == 0) LED2 = !LED2;
	LED2 = 1;


	while(TRUE)
	{
		if (!!PORTFbits.RF6)
			continue;
		
		
		while(TRUE)
		{
			LED1 = !LED1;
	
			
			//for(i = 0; i < 30000; i++);
	
			len = ENC_ReceivePacket(bufor, sizeof(bufor));
			if (len == 0)
				break;
				
			LED0 = !LED0;
	
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			
			eth_header = (ETH_HEADER*)bufor;
	
	
			if (eth_header->type == ETH_TYPE_ARP)
			{
				ARP_HEADER* arp = (ARP_HEADER*)(bufor + sizeof(ETH_HEADER));
				if (arp->hardware_type != ARP_HARDWARE_ETHERNET)
					continue;
				if (arp->protocol_type != ARP_PROTOCOL_IP)
					continue;
				if (arp->hardware_length != 6)
					continue;
				if (arp->protocol_length != 4)
					continue;
				if (arp->opcode != ARP_OPCODE_REQUEST)
					continue;
					
				if (arp->target_ip.address != eth0.ip.address)
					continue;
					
					
				// ok, teraz na tego arp trzeba odpowiedziec :)
				arp->opcode = ARP_OPCODE_REPLY;
				arp->target_ip = arp->sender_ip;
				arp->target_mac = arp->sender_mac;
				arp->sender_ip = eth0.ip;
				arp->sender_mac = eth0.mac;
				
				eth_header->source = eth0.mac;
				eth_header->destination = arp->target_mac;
				
				ENC_SendPacket((void*)eth_header, sizeof(ETH_HEADER) + sizeof(ARP_HEADER));
				continue;
			}	
	
			if (eth_header->type == ETH_TYPE_IP4)
			{
				ip_header = (IP_HEADER*)(bufor + sizeof(ETH_HEADER));
				if (ip_header->destination_ip.address != eth0.ip.address)
					continue; // ³eee.. to nie do mnie.
				
				if (ip_header->protocol == IP_TYPE_ICMP)
				{
					icmp_header = (ICMP_HEADER*)(bufor + sizeof(ETH_HEADER) + sizeof(IP_HEADER));
					if (icmp_header->type != ICMP_TYPE_REQUEST)
						continue;
					
					icmp_header->type = ICMP_TYPE_REPLY;
					CalculateChecksumICMP(icmp_header, len - sizeof(ETH_HEADER) - sizeof(IP_HEADER) - sizeof(ICMP_HEADER));
					
					DWORD temp_ip;
					temp_ip = ip_header->destination_ip.address;
					ip_header->destination_ip = ip_header->source_ip;
					ip_header->source_ip.address = temp_ip;
					ip_header->id = eth0.ip_id++;
					CalculateChecksumIP(ip_header);
					SwapHardwareAddressessETH(eth_header);
					
					ENC_SendPacket((void*)eth_header, len);
				}	
				
				
				if (ip_header->protocol == IP_TYPE_UDP)
				{
				}	
			}	
				
				
		
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
		}	
	}
}


void InitCPU(void)
{
	// konfiguracja oscylatora na 40MIPSów
	CLKDIVbits.PLLPRE = 5-2; // N1 = 5
	PLLFBD = 40-2; // M = 40
	CLKDIVbits.PLLPOST = 0;// N2 = 2
	
	__builtin_write_OSCCONH(0x03);
	__builtin_write_OSCCONL(0x01);
	
	while(OSCCONbits.COSC != 0b011);
	while(OSCCONbits.LOCK != 1);		

	// porty
	TRISB = 0b0000000000011111;
	TRISF = 0b0000000001000000;  // INT
	TRISG = 0b0000000010000000;
	TRISE = 0b0000000000000000;

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

	
	// ### czasy, do doregulowania!!
	//TODO :sprawdzic min czas Tad

	// Ÿród³o zegara taktuj¹cego
	AD1CON3bits.ADRC = 0; // wewnêtrzna magistrala zegara
	AD2CON3bits.ADRC = AD1CON3bits.ADRC;
	
	// czas Tad
	//AD1CON3bits.ADCS = 64-1; // Tad = x * Tcy; Tcy=25ns(40MHz), Tad=64*25=1.6us
	AD1CON3bits.ADCS = 5-1; // Tad = x * Tcy; Tcy=25ns, Tad=5*25=125ns (>117,6)
	AD2CON3bits.ADCS = AD1CON3bits.ADCS;

	// czas samplowania,
	//AD1CON3bits.SAMC = 31; // x * Tad; 31*125ns=3.875us
	AD1CON3bits.SAMC = 4; // x * Tad; 4*125ns=500ns
	AD2CON3bits.SAMC = AD1CON3bits.SAMC;

	// wybor kanalow. dla 12 bitow jest tylko ch0
	AD1CON2bits.CHPS = 0;
	AD2CON2bits.CHPS = AD1CON2bits.CHPS;
	
	// start przetwornika
	AD1CON1bits.ADON = 1;
	AD2CON1bits.ADON = AD1CON1bits.ADON;
	
	SDO = 0;	
	SCK = 0;	
	
	CS_ENC = 1;
	//CS_
}

void InitSPI(void)
{
	// inicjowanie SPI2
	SPI2CON1 = 0x0000;
	SPI2CON1bits.MODE16 = 0; // 8 bitów
	
	SPI2CON1bits.MODE16 = 1; // 8 bitów
	
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
}	


// ----------------------------
// ----------------------------
