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


