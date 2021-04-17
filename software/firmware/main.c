//
// arp -s 192.168.0.100 00-1d-72-ca-9e-e0 192.168.0.200
//
#include <p33fj128mc706.h>
#include <generic.h>
#include <string.h>
#include <stdlib.h>
#include <dsp.h>
#include "defs.h"
#include "spi_sw.h"
#include "enc.h"
#include "delay.h"
#include "net.h"
#include "analog.h"
#include "rand_seed.h"



void InitADCDMA(BOOL turn_off, WORD divider1, WORD divider2, WORD measures_per_packet);

unsigned char packet[1700] __attribute__((near));


#define DEVICE_ID 9


struct ETH
{
	DWORD addr;		// adres IP interfejsu
	DWORD netmask;	// maska sieciowa
	DWORD gateway;	// adres IP bramy wyjsciowej
	DWORD dns1;		// pierwszy DNS
	DWORD dns2;		// drugi DNS
	
	DWORD netaddr;	// adres sieci
	ETH_ADDR mac;
	WORD ip_id;
	
} eth0;

#define MAX_MEASURES	127

struct OUTPUT_PACKET
{
	ETH_HEADER eth;
	IP_HEADER ip;
	UDP_HEADER udp;
} output_packet;	

struct DMA_STRUCT
{
	WORD A_timestamp;
	WORD A[MAX_MEASURES*2 + MAX_MEASURES*2];
	//WORD A_XY[MAX_MEASURES*2];
	//WORD A_Z [MAX_MEASURES*2];
	
	WORD B_timestamp;
	WORD B[MAX_MEASURES*2 + MAX_MEASURES*2];
	//WORD B_XY[MAX_MEASURES*2];
	//WORD B_Z [MAX_MEASURES*2];
} data __attribute__((space(dma))) = {};



struct DEVICE
{
	DWORD random;
	WORD deviceid;
	
	DWORD dest_ip;
	WORD dest_port;
	
	BOOL send_data;
	WORD measures_per_packet;
	WORD dma_selector;
	WORD timestamp;
	
	float measure_frequency;
	
	float data_timeout;
	WORD packets_until_timeout;
	WORD packet_timeout_counter;
	
} device = {};	


const char* timestamp = __DATE__ " " __TIME__;
const char* version = "v1.0";

void CalcRandomSeed(void)
{
	int i;
	device.random = DEVICE_ID;
	for(i = 0; i < sizeof(__rand_seed); i++)
	{
		device.random += (unsigned long)__rand_seed[i];	
		device.random ^= 0x80000001;
	}	
}	

int main(void)
{
	SET_CPU_IPL(0x07);
	CPU_Init();
	SPI_Init();
	ENC_Init();
	InitADCDMA(TRUE, 0, 0, 0);
	SET_CPU_IPL(0x00);


	memset(&data, 0xABCD, sizeof(struct DMA_STRUCT));
	
#if defined(FULL_SPEED)
	long i; LED2 = 0; for(i = 0; i < 100000; i++) if ((i % 10000) == 0) LED2 = !LED2;	LED2 = 1;
#else
	long i; LED2 = 0; for(i = 0; i < 10000; i++) if ((i % 1000) == 0) LED2 = !LED2;	LED2 = 1;
#endif

	device.deviceid = DEVICE_ID;
	CalcRandomSeed();

	//unsigned char __mac[6] = {0x00, 0x1d, 0x72, 0xCA, 0x9E, 0xe0}; // z mojego kompa
	unsigned char __mac[6] = {0x00, 0x04, 0xA3, 0xCA, 0x9E, 0x80};
	__mac[4] = __rand_seed[DEVICE_ID * 2 + 0];
	__mac[5] = __rand_seed[DEVICE_ID * 2 + 1];
	
	memcpy(eth0.mac.byte, __mac, 6);
	//eth0.addr = inet_addr("192.168.0.100");
	//eth0.netmask = inet_addr("255.255.255.0");
	eth0.addr = inet_addr("212.191.89.65");
	eth0.netmask = inet_addr("255.255.255.128");
	
	eth0.addr = inet_addr("192.168.25.0");
	eth0.netmask = inet_addr("255.255.255.0");	
	eth0.addr = (eth0.addr & 0x00FFFFFF) | (DWORD)(100 + (BYTE)DEVICE_ID) << 24;
	
	eth0.netaddr = 0xFFFFFFFF ^ eth0.netmask;
	eth0.netaddr |= eth0.addr;
	ENC_SetHardwareAddress(&eth0.mac);
	eth0.ip_id = (WORD)device.random;
	device.data_timeout = 6.0f;

	// przestrzen konfiguracyjna
	Analog_Init();
	Analog_Update();


	unsigned char q3;
	unsigned short len;
	
	ETH_HEADER *eth_header;
	IP_HEADER *ip_header;
	UDP_HEADER *udp_header;
	ICMP_HEADER *icmp_header;




	while(TRUE)
	{
		if (device.send_data)
		{
			device.send_data = FALSE;
			DWORD len = 0;
			
			len += sizeof(WORD); // timestamp
			len += /*MAX_MEASURES*/ device.measures_per_packet * 2 * sizeof(WORD); // XY
			len += /*MAX_MEASURES*/ device.measures_per_packet * 2 * sizeof(WORD); // Zp   p-padding
			
			output_packet.ip.id = eth0.ip_id++;
			CalculateChecksumUDP(&output_packet.ip);
			CalculateChecksumIP(&output_packet.ip);

			if (device.dma_selector == 0)
			{
				data.B_timestamp = device.timestamp++;
				ENC_SendPacket2B((BYTE*)&output_packet, sizeof(struct OUTPUT_PACKET), (BYTE*)&data.B_timestamp, len);
			} else
			{
				data.A_timestamp = device.timestamp++;
				ENC_SendPacket2B((BYTE*)&output_packet, sizeof(struct OUTPUT_PACKET), (BYTE*)&data.A_timestamp, len);
			}	
			

		}	
		
		if (!!PORTFbits.RF6)
			continue;
		
		
		while(TRUE)
		{
			
			len = ENC_ReceivePacket(packet, sizeof(packet));
			if (len == 0)
				break;
				
			LED0 = !LED0;

			eth_header = (ETH_HEADER*)packet;

	
			// czy to zapytanie ARP?
			if (eth_header->type == ETH_TYPE_ARP)
			{
				ARP_HEADER* arp = (ARP_HEADER*)(packet + sizeof(ETH_HEADER));
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
					
				if (arp->target_ip != eth0.addr)
					continue;
					
					
				// ok, teraz na tego arp trzeba odpowiedziec :)
				arp->opcode = ARP_OPCODE_REPLY;
				arp->target_ip = arp->sender_ip;
				arp->target_mac = arp->sender_mac;
				arp->sender_ip = eth0.addr;
				arp->sender_mac = eth0.mac;
				
				eth_header->source = eth0.mac;
				eth_header->destination = arp->target_mac;
				
				ENC_SendPacket((void*)eth_header, sizeof(ETH_HEADER) + sizeof(ARP_HEADER));
				continue;
			}	
	
			// czy to pakiet IPv4?
			if (eth_header->type == ETH_TYPE_IP4)
			{
				ip_header = (IP_HEADER*)(packet + sizeof(ETH_HEADER));
				if ((ip_header->destination_addr != eth0.addr) && 
					(ip_header->destination_addr != eth0.netaddr) &&
					(ip_header->destination_addr != IPADDR_BROADCAST))
					continue; // ³eee.. to nie do mnie.
				
				// czy to zapytanie ICMP?
				if (ip_header->protocol == IP_TYPE_ICMP)
				{
					icmp_header = (ICMP_HEADER*)(packet + sizeof(ETH_HEADER) + sizeof(IP_HEADER));
					if (icmp_header->type != ICMP_TYPE_REQUEST)
						continue;
					
					icmp_header->type = ICMP_TYPE_REPLY;
					CalculateChecksumICMP(icmp_header, len - sizeof(ETH_HEADER) - sizeof(IP_HEADER) - sizeof(ICMP_HEADER));
					
					DWORD temp_ip;
					temp_ip = ip_header->destination_addr;
					ip_header->destination_addr = ip_header->source_addr;
					ip_header->source_addr = temp_ip;
					ip_header->id = eth0.ip_id++;
					CalculateChecksumIP(ip_header);
					SwapHardwareAddressessETH(eth_header);
				
					ENC_SendPacket((void*)eth_header, len);
					continue;
				}	
				
				// czy to pakiet UDP?
				if (ip_header->protocol == IP_TYPE_UDP)
				{
					udp_header = (UDP_HEADER*)(packet + sizeof(ETH_HEADER) + sizeof(IP_HEADER));					
					//WORD udp_len = NTOHS(udp_header->length) - sizeof(UDP_HEADER);
					WORD dport = NTOHS(udp_header->destination_port);
				
					if (dport == 7000) // port konfiguracyjne
					{
						WORD temp_port;
						BYTE* data = (BYTE*)(packet + sizeof(ETH_HEADER) + sizeof(IP_HEADER) + sizeof(UDP_HEADER));	
						WORD command = *((WORD*)data);
											
						len = sizeof(UDP_HEADER);
						// 
						temp_port = udp_header->destination_port;
						udp_header->destination_port = udp_header->source_port;
						udp_header->source_port = temp_port;						

						DWORD source_ip = ip_header->source_addr;
						//temp_ip = ip_header->destination_addr;
						ip_header->destination_addr = ip_header->source_addr;
						ip_header->source_addr = eth0.addr;
						//ip_header->source_addr = temp_ip;
						ip_header->id = eth0.ip_id++;

						SwapHardwareAddressessETH(eth_header);

						// ping po broadcascie - opowiedz mi swoj¹ historie ;-)
						if (command == '!') // 'ABCD'
						{
							// odpowiedziec adresem IP i numerem MAC
							*((DWORD*)data) = 0x01CAFE02; data += sizeof(DWORD);
							*((WORD*)data) = command; data += sizeof(WORD);
							*((DWORD*)data) = device.random; data += sizeof(DWORD);
							*((WORD*)data) = device.deviceid; data += sizeof(WORD);
							*((DWORD*)data) = source_ip; data += sizeof(DWORD);
							*((DWORD*)data) = eth0.addr; data += sizeof(DWORD);
							*((DWORD*)data) = eth0.gateway; data += sizeof(DWORD);
							*((DWORD*)data) = eth0.netmask; data += sizeof(DWORD);
							memcpy(data, &eth0.mac, 6); data += 6;
							strcpy((char*)data, timestamp); data += strlen(timestamp) + 1;
							strcpy((char*)data, version); data += strlen(version) + 1;
							len += sizeof(DWORD) + sizeof(WORD) + sizeof(DWORD)+ sizeof(WORD) + 4*sizeof(DWORD) + 6 + strlen(timestamp) + 1 + strlen(version) + 1;
							
							eth_header->source = eth0.mac;
						}
						
						// pobierz informacje o poziomach odniesienia i wzmocnieniach stopni analogowych
						if (command == 'C')
						{
							*((DWORD*)data) = 0x01CAFE02; data += sizeof(DWORD);
							*((WORD*)data) = command; data += sizeof(WORD);
							*((WORD*)data) = sizeof(struct __ANALOG_CONFIG); data += sizeof(WORD);
							memcpy(data, &analog_config, sizeof(struct __ANALOG_CONFIG)); data += sizeof(struct __ANALOG_CONFIG);
							len += 4 + 2 + 2 + sizeof(struct __ANALOG_CONFIG); 
						}	
						
						// ustaw poziomy odniesienia i wzmocnienia stopni analogowych
						if (command == 'D')
						{
							//BYTE* data = (BYTE*)(packet + sizeof(ETH_HEADER) + sizeof(IP_HEADER) + sizeof(UDP_HEADER));	
							data += sizeof(WORD); // command
							WORD size = *((WORD*)data); data += sizeof(WORD);
							
							if (size != sizeof(struct __ANALOG_CONFIG))
								goto send_empty;

							// ustaw konfiguracje							
							memcpy(&analog_config, data, sizeof(struct __ANALOG_CONFIG)); data += sizeof(struct __ANALOG_CONFIG);
							Analog_Update();

							data = (BYTE*)(packet + sizeof(ETH_HEADER) + sizeof(IP_HEADER) + sizeof(UDP_HEADER));
							*((DWORD*)data) = 0x01CAFE02; data += sizeof(DWORD);
							*((WORD*)data) = command; data += sizeof(WORD);
							len += 4 + 2;
						}
						
						// rozpocznij pomiar z zdanymi parametrami
						// i wysylaj dane na podane namiary IP:port
						if (command == 'E')
						{
							data += sizeof(WORD); // command		
							device.dest_ip = *((DWORD*)data); data += sizeof(DWORD);
							device.dest_port = *((WORD*)data); data += sizeof(WORD);
							
							WORD divider1 = *((WORD*)data); data += sizeof(WORD);
							WORD divider2 = *((WORD*)data); data += sizeof(WORD);
							float freq = *((float*)data); data += sizeof(float);
							WORD mpp = *((WORD*)data); data += sizeof(WORD); // measures per packet
							divider1 &= 0x0003;
							
							device.measures_per_packet = mpp;
							device.measure_frequency = freq;
							
							device.packets_until_timeout = (WORD)((freq * device.data_timeout) / (float)mpp);
							device.packet_timeout_counter = 0;
							

							memset(&output_packet, 0, sizeof(struct OUTPUT_PACKET));
							DWORD _len = sizeof(UDP_HEADER);
							_len += sizeof(WORD); // timestamp
							_len += mpp /*MAX_MEASURES*/ * 2 * sizeof(WORD); // XY
							_len += mpp /*MAX_MEASURES*/ * 2 * sizeof(WORD); // Zp   p-padding
							output_packet.udp.length = HTONS(_len); 
							output_packet.udp.checksum = 0x0000;
							_len += sizeof(IP_HEADER);

							memcpy(&output_packet.eth.destination, &(eth_header->destination), sizeof(ETH_ADDR));
							memcpy(&output_packet.eth.source, &eth0.mac, sizeof(ETH_ADDR));
							output_packet.eth.type = ETH_TYPE_IP4;
							output_packet.ip.ihl = 5;
							output_packet.ip.version = 4;
							output_packet.ip.length = HTONS(_len);
							output_packet.ip.id = eth0.ip_id++;
							output_packet.ip.ttl = 8; // a co mi tam ;-)
							output_packet.ip.protocol = IP_TYPE_UDP;
							output_packet.ip.source_addr = eth0.addr;
							output_packet.ip.destination_addr = device.dest_ip;
							
							output_packet.udp.source_port = HTONS(8000);
							output_packet.udp.destination_port = device.dest_port;
							
							//device.send_data = TRUE;
					
							InitADCDMA(FALSE, divider1, divider2, mpp); // uruchom blok pomiarowy...
				
						}	
						
						// zakoncz pomiar i wysylanie danych
						if (command == 'F')
						{
							 // stop!
							InitADCDMA(TRUE, 0, 0, 0);
						}	
						
						// ping
						if (command == 'G')
						{
							device.packet_timeout_counter = 0;
							LED1 = !LED1;
							
							*((WORD*)data) = T3CONbits.TON; data += sizeof(WORD);
							len += 2; 
							//continue; // niczego nie odsylaj...
						}						
send_empty:
						udp_header->length = HTONS(len); 
						udp_header->checksum = 0x0000;
						len += sizeof(IP_HEADER);
						ip_header->length = HTONS(len);
						ip_header->id = eth0.ip_id++;
						CalculateChecksumUDP(ip_header);
							
						CalculateChecksumIP(ip_header);
						//SwapHardwareAddressessETH(eth_header);

						len += sizeof(ETH_HEADER);
						ENC_SendPacket((void*)eth_header, len);						
						continue;
					}	
					
					asm("nop");
					asm("nop");
					asm("nop");
					asm("nop");
					asm("nop");
					asm("nop");
					asm("nop");
				}	
			}	
		}	
	}
}




// ----------------------------
// ----------------------------


void InitADCDMA(BOOL turn_off, WORD divider1, WORD divider2, WORD measures_per_packet)
{

	T3CONbits.TON = 0;
	T5CONbits.TON = 0;
	
	AD1CON1bits.ADON = 0;
	AD2CON1bits.ADON = 0;
	DMA0CONbits.CHEN = 0;
	DMA1CONbits.CHEN = 0;
	
	if (turn_off)
		return;
	
	AD1CON1bits.AD12B = 1;
	AD2CON1bits.AD12B = 1;
	
	AD1CON2bits.CSCNA = 0; // bez skanowania wejsc
	AD1CHS0bits.CH0SA = 1;
	AD1CHS0bits.CH0NA = 0;
	AD1CHS0bits.CH0SB = 2;
	AD1CHS0bits.CH0NB = 0;
	

	AD2CON2bits.CSCNA = 0; // bez skanowania wejsc
	AD2CHS0bits.CH0SA = 3;
	AD2CHS0bits.CH0NA = 0;
	AD2CHS0bits.CH0SB = 4; // nieuzywane
	AD2CHS0bits.CH0NB = 0;
	

	AD1CON2bits.CHPS = 0; // skanowanie tylko CH0
	AD1CON1bits.ASAM = 1; // automatyczne wyzwalanie samplowania
	AD1CON2bits.ALTS = 1; // sample a i sample b
	AD1CON2bits.SMPI = 2-1; // po dwie probki
	AD1CON1bits.ADDMABM = 1;  // ADC uzywa dma w trybie Conversion Order Build
	AD1CON1bits.SSRC = 0b010; // timer 3 dla adc1
	
	AD2CON2bits.CHPS = 0; // skanowanie tylko CH0
	AD2CON1bits.ASAM = 1; // automatyczne wyzwalanie samplowania
	//AD2CON2bits.ALTS = 1; // sample a i sample b
	AD2CON2bits.ALTS = 1; // tylko sample a
	//AD2CON2bits.SMPI = 2-1; // po dwie probki
	AD2CON2bits.SMPI = 2-1; // po jednej próbce
	AD2CON1bits.ADDMABM = 1;  // ADC uzywa dma w trybie Conversion Order Build
	AD2CON1bits.SSRC = 0b010; // timer 5 dla adc2

	IFS0bits.AD1IF = 0; 
	IFS1bits.AD2IF = 0;
	IEC0bits.AD1IE = 0; 
	IEC1bits.AD2IE = 0;

	IFS0bits.DMA0IF = 0; 
	IFS0bits.DMA1IF = 0; 

	IEC0bits.DMA0IE = 1; 
	IEC0bits.DMA1IE = 1; 


	DMA0CONbits.SIZE = 0; // WORD
	DMA0CONbits.AMODE = 0b00; // Configure DMA for Register indirect with post increment
	DMA0CONbits.MODE = 2; // tryb ping-pong (dwa bufory)
	
	DMA0PAD = (unsigned short)&ADC1BUF0;
	DMA0CNT = ((/*MAX_MEASURES*/measures_per_packet))*2-1;
	DMA0REQ = 13; // adc1
	
	DMA0STA = (unsigned int)data.A - (unsigned int)&_DMA_BASE;
	DMA0STB = (unsigned int)data.B - (unsigned int)&_DMA_BASE;
	//DMA0STA = __builtin_dmaoffset(s1);
	//DMA0STB = __builtin_dmaoffset(s2);	


	DMA1CONbits.AMODE = 0; // Configure DMA for Register indirect with post increment
	DMA1CONbits.MODE = 2; // Configure DMA for Continuous Ping-Pong mode
	
	DMA1PAD = (unsigned short)&ADC2BUF0;
	DMA1CNT = ((/*MAX_MEASURES*/measures_per_packet))*2-1;
	DMA1REQ = 21; // adc2
	
	//DMA1STA = (unsigned int)data.A_Z - (unsigned int)&_DMA_BASE;
	//DMA1STB = (unsigned int)data.B_Z - (unsigned int)&_DMA_BASE;
	DMA1STA = measures_per_packet * 2 * sizeof(WORD) + ((unsigned int)data.A - (unsigned int)&_DMA_BASE);
	DMA1STB = measures_per_packet * 2 * sizeof(WORD) + ((unsigned int)data.B - (unsigned int)&_DMA_BASE);
	//DMA1STA = __builtin_dmaoffset(s3);
	//DMA1STB = __builtin_dmaoffset(s4);	
	
	device.dma_selector = 0x0000;
	
	DMA0CONbits.CHEN = 1;
	DMA1CONbits.CHEN = 1;

	AD1CON1bits.ADON = 1;
	AD2CON1bits.ADON = 1;
	
	
	// timery
	
	T3CONbits.TGATE = 0;
	T3CONbits.TCKPS = divider1; //TCKPS: 00=1:1 01=1:8 10=1:64 11=1:256
	T3CONbits.TCS = 0; // zegar Fosc/2
	TMR3 = 0;
	PR3 = divider2;
	IEC0bits.T3IE = 0;
	IFS0bits.T3IF = 0;
	

	T5CONbits.TGATE = 0;
	T5CONbits.TCKPS = divider1; //TCKPS: 00=1:1 01=1:8 10=1:64 11=1:256
	T5CONbits.TCS = 0; // zegar Fosc/2
	TMR5 = 0;
	PR5 = divider2;
	IEC1bits.T5IE = 0;
	IFS1bits.T5IF = 0;


	T3CONbits.TON = 1;	
	T5CONbits.TON = 1;
}	

void _ISR _NOAUTOPSV _DMA0Interrupt(void)
{
	device.dma_selector = !device.dma_selector;
	device.send_data = TRUE;
	IFS0bits.DMA0IF = 0;
}

void _ISR _NOAUTOPSV _DMA1Interrupt(void)
{
	//device.send_data = TRUE;
	IFS0bits.DMA1IF = 0;
}
