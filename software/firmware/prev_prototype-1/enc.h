#if !defined(__ENC_H__)
#define __ENC_H__

#include "net.h"

// ##################################################################
// ## Konfiguracja uk³adu ENC28J60
// ##################################################################


#define RX_START	0x0000
#define RX_END		0x0FFF
#define TX_START	0x1000
#define TX_END		0x1FFF

// ##################################################################
// ##################################################################
// ##################################################################
// ##################################################################

// mapy rejestrów

#define BANK0			0b00000000
#define BANK1			0b00100000
#define BANK2			0b01000000
#define BANK3			0b01100000
#define DUMMY_TRANSFER	0b10000000

#define REG_MASK		0b00011111
#define BANK_MASK		0b01100000
#define DUMMY_MASK		0b10000000

// dbbrrrrr
// r - rejestr
// b - bank
// d - dummy read


// ----------------------------
#define ERDPTL		BANK0 | 0x00
#define ERDPTH		BANK0 | 0x01
#define EWRPTL		BANK0 | 0x02
#define EWRPTH		BANK0 | 0x03
#define ETXSTL		BANK0 | 0x04
#define ETXSTH		BANK0 | 0x05
#define ETXNDL		BANK0 | 0x06
#define ETXNDH		BANK0 | 0x07
#define ERXSTL		BANK0 | 0x08
#define ERXSTH		BANK0 | 0x09
#define ERXNDL		BANK0 | 0x0A
#define ERXNDH		BANK0 | 0x0B
#define ERXRDPTL	BANK0 | 0x0C
#define ERXRDPTH	BANK0 | 0x0D
#define ERXWRPTL	BANK0 | 0x0E
#define ERXWRPTH	BANK0 | 0x0F
#define EDMASTL 	BANK0 | 0x10
#define EDMASTH 	BANK0 | 0x11
#define EDMANDL 	BANK0 | 0x12
#define EDMANDH 	BANK0 | 0x13
#define EDMADSTL	BANK0 | 0x14
#define EDMADSTH	BANK0 | 0x15
#define EDMACSL 	BANK0 | 0x16
#define EDMACSH 	BANK0 | 0x17

#define EHT0		BANK1 | 0x00
#define EHT1 		BANK1 | 0x01
#define EHT2 		BANK1 | 0x02
#define EHT3 		BANK1 | 0x03
#define EHT4		BANK1 | 0x04
#define EHT5 		BANK1 | 0x05
#define EHT6 		BANK1 | 0x06
#define EHT7 		BANK1 | 0x07
#define EPMM0 		BANK1 | 0x08
#define EPMM1	 	BANK1 | 0x09
#define EPMM2 		BANK1 | 0x0A
#define EPMM3 		BANK1 | 0x0B
#define EPMM4 		BANK1 | 0x0C
#define EPMM5 		BANK1 | 0x0D
#define EPMM6 		BANK1 | 0x0E
#define EPMM7 		BANK1 | 0x0F
#define EPMCSL 		BANK1 | 0x10
#define EPMCSH 		BANK1 | 0x11
#define EPMOL 		BANK1 | 0x14
#define EPMOH 		BANK1 | 0x15
#define EWOLIE 		BANK1 | 0x16
#define EWOLIR 		BANK1 | 0x17
#define ERXFCON	 	BANK1 | 0x18
#define EPKTCNT 	BANK1 | 0x19

#define MACON1 		DUMMY_TRANSFER | BANK2 | 0x00
#define MACON2 		DUMMY_TRANSFER | BANK2 | 0x01
#define MACON3 		DUMMY_TRANSFER | BANK2 | 0x02
#define MACON4 		DUMMY_TRANSFER | BANK2 | 0x03
#define MABBIPG 	DUMMY_TRANSFER | BANK2 | 0x04
#define MAIPGL		DUMMY_TRANSFER | BANK2 | 0x06
#define MAIPGH		DUMMY_TRANSFER | BANK2 | 0x07
#define MACLCON1	DUMMY_TRANSFER | BANK2 | 0x08
#define MACLCON2	DUMMY_TRANSFER | BANK2 | 0x09
#define MAMXFLL 	DUMMY_TRANSFER | BANK2 | 0x0A
#define MAMXFLH 	DUMMY_TRANSFER | BANK2 | 0x0B
#define MAPHSUP 	DUMMY_TRANSFER | BANK2 | 0x0D
#define MICON 		DUMMY_TRANSFER | BANK2 | 0x11
#define MICMD 		DUMMY_TRANSFER | BANK2 | 0x12
#define MIREGADR 	DUMMY_TRANSFER | BANK2 | 0x14
#define MIWRL 		DUMMY_TRANSFER | BANK2 | 0x16
#define MIWRH 		DUMMY_TRANSFER | BANK2 | 0x17
#define MIRDL 		DUMMY_TRANSFER | BANK2 | 0x18
#define MIRDH 		DUMMY_TRANSFER | BANK2 | 0x19

#define MAADR5 		DUMMY_TRANSFER | BANK3 | 0x00
#define MAADR6 		DUMMY_TRANSFER | BANK3 | 0x01
#define MAADR3 		DUMMY_TRANSFER | BANK3 | 0x02
#define MAADR4 		DUMMY_TRANSFER | BANK3 | 0x03
#define MAADR1 		DUMMY_TRANSFER | BANK3 | 0x04
#define MAADR2 		DUMMY_TRANSFER | BANK3 | 0x05
#define EBSTSD 		BANK3 | 0x06
#define EBSTCON 	BANK3 | 0x07
#define EBSTCSL 	BANK3 | 0x08
#define EBSTCSH 	BANK3 | 0x09
#define MISTAT 		DUMMY_TRANSFER | BANK3 | 0x0A
#define EREVID 		BANK3 | 0x12
#define ECOCON 		BANK3 | 0x15
#define EFLOCON 	BANK3 | 0x17
#define EPAUSL 		BANK3 | 0x18
#define EPAUSH 		BANK3 | 0x19

// --------------------------------
// Rejestry wspólne, niezaleznie od banku
#define EIE			0x1B
#define EIR			0x1C
#define ESTAT		0x1D
#define ECON2		0x1E
#define ECON1		0x1F


#define PHCON1		0x00
#define PHSTAT1		0x01
#define PHID1		0x02
#define PHID2		0x03
#define PHCON2		0x10
#define PHSTAT2		0x11
#define PHIE		0x12
#define PHIR		0x13
#define PHLCON		0x14


// ##################################################################
// ##################################################################
// ##################################################################
// ##################################################################
// Flagi dla poszczegolnych pol rejestrow

#define ECON1_TXRST		0b10000000
#define ECON1_RXRST		0b01000000
#define ECON1_DMAST		0b00100000
#define ECON1_CSUMEN	0b00010000
#define ECON1_TXRTS		0b00001000
#define ECON1_RXEN		0b00000100
#define ECON1_BSEL1		0b00000010
#define ECON1_BSEL0		0b00000001

#define ECON2_VRPS		0b00001000
#define ECON2_PWRSV		0b00100000
#define ECON2_PKTDEC	0b01000000
#define ECON2_AUTOINC	0b10000000

#define EIE_INTIE		0b10000000
#define EIE_PKTIE		0b01000000
#define EIE_DMAIE		0b00100000
#define EIE_LINKIE		0b00010000
#define EIE_TXIE		0b00001000
#define EIE_TXERIE		0b00000010
#define EIE_RXERIE		0b00000001
	
#define MACON1_TXPAUS	0b00001000
#define MACON1_RXPAUS	0b00000100
#define MACON1_PASALL	0b00000010
#define MACON1_MARXEN	0b00000001
	
#define MACON3_PADCFG2	0b10000000
#define MACON3_PADCFG1	0b01000000
#define MACON3_PADCFG0	0b00100000
#define MACON3_TXCRCEN	0b00010000
#define MACON3_PHDREN	0b00001000
#define MACON3_HFRMEN	0b00000100
#define MACON3_FRMLNEN	0b00000010
#define MACON3_FULDPX	0b00000001

#define MACON4_DEFER	0b01000000
#define MACON4_BPEN		0b00100000
#define MACON4_NOBKOFF	0b00010000
	
#define MICMD_MIISCAN	0b00000010
#define MICMD_MIIRD		0b00000001

#define MISTAT_NVALID	0b00000100
#define MISTAT_SCAN		0b00000010
#define MISTAT_BUSY		0b00000001


#define PHCON1_PRST		0b1000000000000000
#define PHCON1_PLOOPBK	0b0100000000000000
#define PHCON1_PPWRSV	0b0000100000000000
#define PHCON1_PDPXMD	0b0000000100000000
	
#define PHCON2_FRCLNK	0b0100000000000000
#define PHCON2_TXDIS	0b0010000000000000
#define PHCON2_JABBER	0b0000010000000000
#define PHCON2_HDLDIS	0b0000000100000000



// ##################################################################
// ##################################################################
// ##################################################################
// ##################################################################

	
typedef struct 
{
	unsigned short phcon1;
	unsigned short phstat1;
	unsigned short phid1;
	unsigned short phid2;
	unsigned short phcon2;
	unsigned short phstat2;
	unsigned short phie;
	unsigned short phir;
	unsigned short phlcon;
	
} ENC_PHY;	
	
union REG_ECON1
{
	struct {
		unsigned char BSEL	: 2;
		unsigned char RXEN	: 1;
		unsigned char TXRTS	: 1;
		unsigned char CSUMEN	: 1;
		unsigned char DMAST	: 1;
		unsigned char RXRST	: 1;
		unsigned char TXRST	: 1;
	};
	unsigned char value;	
};	
//
//union REG_ECON2
//{
//	struct {
//		unsigned char __reserved1 : 3;
//		unsigned char VRPS : 1;
//		unsigned char __reserved2 : 1;
//		unsigned char PWRSV : 1;
//		unsigned char PKTDEC : 1;
//		unsigned char AUTOINC : 1;
//	};
//	unsigned char value;
//};	
//
//union REG_ESTAT
//{
//	struct {
//		unsigned char CLKRDY : 1;
//		unsigned char TXABRT : 1;
//		unsigned char RXBUSY : 1;
//		unsigned char __reserved1 : 1;
//		unsigned char LATECOL : 1;
//		unsigned char __reserved2 : 1;
//		unsigned char BUFER : 1;
//		unsigned char INT : 1;
//	};
//	unsigned char value;
//};	
//
//union REG_EIR
//{
//	struct {
//		unsigned char RXERIF : 1;
//		unsigned char TXERIF : 1;
//		unsigned char __reserved1 : 1;
//		unsigned char TXIF : 1;
//		unsigned char LINKIF : 1;
//		unsigned char DMAIF : 1;
//		unsigned char PKTIF : 1;
//		unsigned char __reserved2 : 1;
//	};
//	unsigned char value;	
//};	
//
//union REG_EIE
//{
//	struct {
//		unsigned char RXERIE : 1;
//		unsigned char TXERIE : 1;
//		unsigned char __reserved1 : 1;
//		unsigned char TXIE : 1;
//		unsigned char LINKIE : 1;
//		unsigned char DMAIE : 1;
//		unsigned char PKTIE : 1;
//		unsigned char INTIE : 1;
//	};	
//	unsigned char value;
//};	
//
//union REG_MIREGADR
//{
//	struct
//	{
//		unsigned char BUSY : 1;
//		unsigned char SCAN : 1;
//		unsigned char NVALID : 1;
//		unsigned char __reserved1 : 1;
//		unsigned char __reserved2 : 4;
//	};
//	unsigned char value;	
//};	

// ##################################################################
// ##################################################################
// ##################################################################
// ##################################################################

#define SPI_SEND_AND_WAIT(value) { IFS2bits.SPI2IF = 0; SPI2BUF = (value); while(!IFS2bits.SPI2IF); }


#define READ_CONTROL_REGISTER	0b00000000
#define READ_BUFFER_MEMORY		0b00111010
#define WRITE_CONTROL_REGISTER	0b01000000
#define WRITE_BUFFER_MEMORY		0b01111010

#define BIT_FIELD_SET			0b10000000
#define BIT_FIELD_CLEAR			0b10100000
#define SOFT_RESET				0b11111111


// ##################################################################
// ##################################################################
// ##################################################################
// ##################################################################

typedef union
{
	BYTE bytes[7];
	struct
	{
		unsigned short byte_count;
		unsigned char collision_count : 4;
		unsigned char crc_error : 1;
		unsigned char length_check_error : 1;
		unsigned char length_out_of_range : 1;
		unsigned char done : 1;
		unsigned char multicast : 1;
		unsigned char broadcast : 1;
		unsigned char defer : 1;
		unsigned char excessive_defer : 1;
		unsigned char excessive_collision : 1;
		unsigned char late_collision : 1;
		unsigned char giant : 1;
		unsigned char __reserved1 : 1;
		unsigned short bytes_on_wire;
		unsigned char control_frame : 1;
		unsigned char pause_control_frame : 1;
		unsigned char backpressure : 1;
		unsigned char vlan : 1;
		unsigned short __reserved2 : 12;
	};	
} SENT_STATUS;



typedef union
{
	BYTE bytes[6];
	struct
	{
		unsigned short next_packet_address;
		
		unsigned short frame_length;
		unsigned char long_or_drop : 1;
		unsigned char __reserved1 : 1;
		unsigned char carrier_seen : 1;
		unsigned char __reserved2 : 1;
		unsigned char crc_error : 1;
		unsigned char length_check_error : 1;
		unsigned char length_out_of_range : 1;
		unsigned char recv_ok : 1;
		unsigned char multicast : 1;
		unsigned char broadcast : 1;
		unsigned char dribble_nibble : 1;
		unsigned char control_frame : 1;
		unsigned char pause_control_frame : 1;
		unsigned char unknown_opcode : 1;
		unsigned char vlan : 1;
		unsigned char __reserved3 : 1;
	};	
} RECEIVED_STATUS;	



// ##################################################################
// ##################################################################
// ##################################################################
// ##################################################################

void InitENC(void);

BYTE ENC_ReadControlRegister(BYTE banked_reg);
void ENC_WriteControlRegister(BYTE banked_reg, BYTE value);

void ENC_WritePHYRegister(unsigned char reg, unsigned short data);
unsigned short ENC_ReadPHYRegister(unsigned char reg);
void ENC_ReadPHYRegisters(ENC_PHY* phy);

void ENC_Reset(void);

void ENC_ResetBit(unsigned char banked_reg, unsigned char mask);
void ENC_SetBit(unsigned char banked_reg, unsigned char mask);

void ENC_SetHardwareAddress(const ETH_ADDR *address);

unsigned short ENC_ReceivePacket(unsigned char* buffer, unsigned short capacity);
void ENC_SendPacket(const unsigned char* packet, unsigned int length);

// private:
void ENC_SetCurrentBank(BYTE banked_reg);
void ENC_WriteRegister(BYTE reg_addr, BYTE value);
BYTE ENC_ReadRegister(BYTE reg_addr);


//
// ####################################################
// ## Funkcje inline
// ####################################################


static inline BYTE ENC_GetReceivedPacketCount(void)
{
	return ENC_ReadControlRegister(EPKTCNT);	
}	


#endif // __ENC_H__ 
