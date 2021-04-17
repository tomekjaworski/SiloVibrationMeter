#if !defined(_NET_H_)
#define _NET_H_

//#include <stddef.h>
#include <generic.h>

#define PACKED __attribute__((packed))

#define IPADDR_ANY			0x00000000
#define IPADDR_BROADCAST	0xFFFFFFFF

#define NTOHS(x) (((x) >> 8) | ((x) << 8))
#define HTONS(x) (((x) >> 8) | ((x) << 8))

struct __ETH_ADDR
{
	unsigned char byte[6];
} PACKED;
typedef struct __ETH_ADDR ETH_ADDR;

union __IP_ADDR
{
	unsigned long int address;
	BYTE byte[4];
};
typedef union __IP_ADDR IP_ADDR;	

struct __ETH_HEADER
{
	ETH_ADDR destination;
	ETH_ADDR source;
	WORD type;
} PACKED;
typedef struct __ETH_HEADER ETH_HEADER;

struct __IP_HEADER
{
	unsigned char ihl : 4;
	unsigned char version : 4;
	unsigned char type;
	unsigned int length;
	unsigned int id;
	unsigned int flags : 3;
	unsigned int frag_offset : 13;
	unsigned char ttl;
	unsigned char protocol;
	unsigned int checksum;
	
	DWORD source_addr;
	DWORD destination_addr;
	
	//unsigned long int options : 24;
	//unsigned char padding;
} PACKED;
typedef struct __IP_HEADER IP_HEADER;

struct __UDP_HEADER
{
	WORD source_port;
	WORD destination_port;
	WORD length;
	WORD checksum;
} PACKED;
typedef struct __UDP_HEADER UDP_HEADER;	


struct __ICMP_HEADER
{
	unsigned char type;
	unsigned char code;
	unsigned short checksum;
	unsigned short identifier;
	unsigned short sequence_number;
};
typedef struct __ICMP_HEADER ICMP_HEADER;	

struct __ARP_HEADER
{
	unsigned short hardware_type;
	unsigned short protocol_type;
	
	unsigned char hardware_length;
	unsigned char protocol_length;
	
	unsigned short opcode;
	
	ETH_ADDR sender_mac;
	DWORD sender_ip;
	ETH_ADDR target_mac;
	DWORD target_ip;
};
typedef struct __ARP_HEADER	ARP_HEADER;


#define ETH_TYPE_IP4			0x0008
#define ETH_TYPE_ARP			0x0608

#define IP_TYPE_ICMP			0x01
#define IP_TYPE_TCP				0x06
#define IP_TYPE_UDP				0x11

#define ARP_HARDWARE_ETHERNET	0x0100
#define ARP_PROTOCOL_IP			0x0008

#define ARP_OPCODE_REQUEST		0x0100
#define ARP_OPCODE_REPLY		0x0200

#define ICMP_TYPE_REQUEST		0x08
#define ICMP_TYPE_REPLY			0x00


void CalculateChecksumUDP(IP_HEADER* ip_header);

void CalculateChecksumIP(IP_HEADER* header);
void CalculateChecksumICMP(ICMP_HEADER* header, unsigned short icmp_load_length);

int inet_aton(const char *cp, unsigned long int *addr);
unsigned long int inet_addr(const char* cp);

void SwapHardwareAddressess(ETH_ADDR* addr1, ETH_ADDR* addr2);
void SwapHardwareAddressessETH(ETH_HEADER* ethernet_header);


#endif // _NET_H_

