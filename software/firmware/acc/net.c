#include <generic.h>
#include <string.h>
#include "net.h"



unsigned int NetworkChecksum(const void* buffer, unsigned int len)
{
	unsigned int *ptr = (unsigned int*)buffer;
	unsigned long int chk = 0;
	unsigned int* chk_upper = (unsigned int*)&chk + 1;
	for(; len > 1; len -= 2, ptr++)
		chk += *ptr;
	if (len)
		chk += *((unsigned char*)ptr) << 8;

	while(*chk_upper)
		chk = (chk & 0x0000FFFF) + *chk_upper;
	return chk ^ 0xFFFF;
}

void CalculateChecksumIP(IP_HEADER* header)
{
	header->checksum = 0x0000;
	header->checksum = NetworkChecksum(header, header->ihl * 4);
}	

void CalculateChecksumICMP(ICMP_HEADER* header, unsigned short icmp_load_length)
{
	header->checksum = 0x0000;
	header->checksum = NetworkChecksum(header, sizeof(ICMP_HEADER) + icmp_load_length);
}	

void CalculateChecksumUDP(IP_HEADER* ip_header)
{
//	unsigned int *ptr = (unsigned int*)((BYTE*)ip_header + sizeof(IP_HEADER) - 2*sizeof(DWORD));// + sizeof(UDP_HEADER));
//	unsigned long int chk = 0;
//	unsigned int* chk_upper = (unsigned int*)&chk + 1;
//	UDP_HEADER * udp = (UDP_HEADER*)((BYTE*)ip_header + sizeof(IP_HEADER));
//	WORD len = NTOHS(ip_header->length) - sizeof(IP_HEADER) + 2*sizeof(DWORD);
//	
//	chk += ip_header->protocol;
//	for(; len > 1; len -= 2, ptr++)
//		chk += *ptr;
//	if (len)
//		chk += *((unsigned char*)ptr) << 8;
//
//	while(*chk_upper)
//		chk = (chk & 0x0000FFFF) + *chk_upper;
//		
//	udp->checksum = chk ^ 0xFFFF;

	UDP_HEADER * udp = (UDP_HEADER*)((BYTE*)ip_header + sizeof(IP_HEADER));
	udp->checksum = 0x0000;
	
	
//		
//u16 udp_sum_calc(u16 len_udp, u16 src_addr[],u16 dest_addr[], BOOL padding, u16 buff[])
//{
//u16 prot_udp=17;
//u16 padd=0;
//u16 word16;
//u32 sum;	
//	
//	// Find out if the length of data is even or odd number. If odd,
//	// add a padding byte = 0 at the end of packet
//	if (padding&1==1){
//		padd=1;
//		buff[len_udp]=0;
//	}
//	
//	//initialize sum to zero
//	sum=0;
//	
//	// make 16 bit words out of every two adjacent 8 bit words and 
//	// calculate the sum of all 16 vit words
//	for (i=0;i<len_udp+padd;i=i+2){
//		word16 =((buff[i]<<8)&0xFF00)+(buff[i+1]&0xFF);
//		sum = sum + (unsigned long)word16;
//	}	
//	// add the UDP pseudo header which contains the IP source and destinationn addresses
//	for (i=0;i<4;i=i+2){
//		word16 =((src_addr[i]<<8)&0xFF00)+(src_addr[i+1]&0xFF);
//		sum=sum+word16;	
//	}
//	for (i=0;i<4;i=i+2){
//		word16 =((dest_addr[i]<<8)&0xFF00)+(dest_addr[i+1]&0xFF);
//		sum=sum+word16; 	
//	}
//	// the protocol number and the length of the UDP packet
//	sum = sum + prot_udp + len_udp;
//
//	// keep only the last 16 bits of the 32 bit calculated sum and add the carries
//    	while (sum>>16)
//		sum = (sum & 0xFFFF)+(sum >> 16);
//		
//	// Take the one's complement of sum
//	sum = ~sum;
//
//return ((u16) sum);
//}	
}	


void SwapHardwareAddressess(ETH_ADDR* addr1, ETH_ADDR* addr2)
{
	ETH_ADDR temp;

	memcpy(&temp, addr1, sizeof(ETH_ADDR));
	memcpy(addr1, addr2, sizeof(ETH_ADDR));
	memcpy(addr2, &temp, sizeof(ETH_ADDR));
}	

void SwapHardwareAddressessETH(ETH_HEADER* ethernet_header)
{
	ETH_ADDR temp;

	memcpy(&temp, &(ethernet_header->destination), sizeof(ETH_ADDR));
	memcpy(&(ethernet_header->destination), &(ethernet_header->source), sizeof(ETH_ADDR));
	memcpy(&(ethernet_header->source), &temp, sizeof(ETH_ADDR));
}	


