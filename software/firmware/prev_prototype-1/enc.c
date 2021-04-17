#include <p33fj128mc706.h>
#include <generic.h>
#include "defs.h"
#include "enc.h"
#include "net.h"
#include "delay.h"

static BYTE current_bank = 0xFF;
static WORD dummy __attribute__((near));
static WORD next_packet_ptr;



//
void InitENC(void)
{
	
	ENC_Reset();
	
	unsigned char srev;
	srev = ENC_ReadControlRegister(EREVID);
	
	while(srev != 0x06); // wymagana wersja krzemu: B7
	
	// programowanie layoutu pamieci
	
	next_packet_ptr = RX_START;
	
	// poczatek i koniec bloku odbioru danych
	ENC_WriteControlRegister(ERXSTL, RX_START & 0xFF);
	ENC_WriteControlRegister(ERXSTH, RX_START >> 8);
	ENC_WriteControlRegister(ERXNDL, RX_END & 0xFF);
	ENC_WriteControlRegister(ERXNDH, RX_END >> 8);
	
	// punkt rozpoczecia odbierania danych
	ENC_WriteControlRegister(ERXRDPTL, RX_START & 0xFF);
	ENC_WriteControlRegister(ERXRDPTH, RX_START >> 8);

	ENC_WriteControlRegister(ETXSTL, TX_START & 0xFF);
	ENC_WriteControlRegister(ETXSTH, TX_START >> 8);
	ENC_WriteControlRegister(ETXNDL, TX_END & 0xFF);
	ENC_WriteControlRegister(ETXNDH, TX_END >> 8);
		
	// konfiguracja nadajnika, odbiornika; tryb full-duplex
	ENC_WriteControlRegister(MACON1, MACON1_TXPAUS | MACON1_RXPAUS | MACON1_MARXEN);
	ENC_WriteControlRegister(MACON3, MACON3_PADCFG0 | MACON3_TXCRCEN | MACON3_FRMLNEN | MACON3_FULDPX);
	ENC_WriteControlRegister(MACON4, MACON4_DEFER);
	
	// na podstawie dokumentacji ENC28J60	
	ENC_WriteControlRegister(MAMXFLL, 0xEE);
	ENC_WriteControlRegister(MAMXFLH, 0x05);
	ENC_WriteControlRegister(MABBIPG, 0x15);
	ENC_WriteControlRegister(MAIPGL, 0x12);
	ENC_WriteControlRegister(MAIPGH, 0x0C);
	
	ENC_WritePHYRegister(PHCON1, PHCON1_PDPXMD);
	ENC_WritePHYRegister(PHCON2, PHCON2_HDLDIS);
	
	// przerwania dla: odebrany pakiet
	ENC_WriteControlRegister(EIE, EIE_INTIE | EIE_PKTIE);

	ENC_PHY phy;
	ENC_ReadPHYRegisters(&phy);

	// wylacz CLKOUT
	ENC_WriteControlRegister(ECOCON, 0x00);

	// wlaczenie odbioru
	ENC_SetBit(ECON1, ECON1_RXEN);
	
}	


BYTE ENC_ReadRegister(BYTE reg_addr)
{

	CS_ENC = 0;
	dummy = SPI2BUF;
	SPI_SEND_AND_WAIT(READ_CONTROL_REGISTER | reg_addr);

	dummy = SPI2BUF;
	SPI_SEND_AND_WAIT(0x00);
	CS_ENC = 1;

	return SPI2BUF;
}	

void ENC_WriteRegister(BYTE reg_addr, BYTE value)
{
	CS_ENC = 0;
	
	dummy = SPI2BUF;
	SPI_SEND_AND_WAIT(WRITE_CONTROL_REGISTER | reg_addr);

	dummy = SPI2BUF;
	SPI_SEND_AND_WAIT(value);

	CS_ENC = 1;
}	


void ENC_SetCurrentBank(BYTE banked_reg)
{
	BYTE bank = ((banked_reg & BANK_MASK) >> 5);
	if (bank == current_bank)
		return;
		
	union REG_ECON1 econ1;
	econ1.value = ENC_ReadRegister(ECON1);
	econ1.BSEL = bank;
	
	ENC_WriteRegister(ECON1, econ1.value);
	current_bank = bank;
}	

BYTE ENC_ReadControlRegister(BYTE banked_reg)
{
	BYTE reg = banked_reg & REG_MASK;
	
	if (reg < EIE)
		ENC_SetCurrentBank(banked_reg);

	if (banked_reg & DUMMY_MASK)
	{
		// jeden bajt do olania
		CS_ENC = 0;
		
		dummy = SPI2BUF;
		SPI_SEND_AND_WAIT(reg);
		
		// bajt dummy
		dummy = SPI2BUF;
		SPI_SEND_AND_WAIT(0x00);
		
		// bajt danych
		dummy = SPI2BUF;
		SPI_SEND_AND_WAIT(0x00);
		
		CS_ENC = 1;
		
		return (BYTE)dummy;
	}
	return ENC_ReadRegister(reg);
}	

void ENC_WriteControlRegister(BYTE banked_reg, BYTE value)
{
	BYTE reg = banked_reg & REG_MASK;
	
	if (reg < EIE)
		ENC_SetCurrentBank(banked_reg);
	
	ENC_WriteRegister(reg, value);
}


void ENC_Reset(void)
{
	
	CS_ENC = 0;
	
	dummy = SPI2BUF;
	SPI_SEND_AND_WAIT(SOFT_RESET);
	
	CS_ENC = 1;

	DelayMS(10);
}	


void ENC_WritePHYRegister(unsigned char reg, unsigned short data)
{
	ENC_WriteControlRegister(MIREGADR, reg);
	ENC_WriteControlRegister(MIWRL, (unsigned char)data);
	ENC_WriteControlRegister(MIWRH, (unsigned char)(data >> 8));
	while(ENC_ReadControlRegister(MISTAT) & MISTAT_BUSY)
		Delay10us(1);
}	


unsigned short ENC_ReadPHYRegister(unsigned char reg)
{
	ENC_WriteControlRegister(MIREGADR, reg);
	ENC_WriteControlRegister(MICMD, MICMD_MIIRD);
	
	Delay10us(1);
	while (ENC_ReadControlRegister(MISTAT) & MISTAT_BUSY)
		Delay10us(1);
	
	ENC_WriteControlRegister(MICMD, 0x00);
	
	WORD_VAL wb;
	wb.byte.HB = ENC_ReadControlRegister(MIRDH);
	wb.byte.LB = ENC_ReadControlRegister(MIRDL);
	
	return wb.Val;
}	

void ENC_ReadPHYRegisters(ENC_PHY* phy)
{
	phy->phcon1 = ENC_ReadPHYRegister(PHCON1);
	phy->phstat1 = ENC_ReadPHYRegister(PHSTAT1);
	phy->phid1 = ENC_ReadPHYRegister(PHID1);
	phy->phid2 = ENC_ReadPHYRegister(PHID2);
	phy->phcon2 = ENC_ReadPHYRegister(PHCON2);
	phy->phstat2 = ENC_ReadPHYRegister(PHSTAT2);
	phy->phie = ENC_ReadPHYRegister(PHIE);
	phy->phir = ENC_ReadPHYRegister(PHIR);
	phy->phlcon = ENC_ReadPHYRegister(PHLCON);
}	


void ENC_SetBit(unsigned char banked_reg, unsigned char mask)
{
	BYTE reg = banked_reg & REG_MASK;
	
	if (reg < EIE)
		ENC_SetCurrentBank(banked_reg);
	
	CS_ENC = 0;
	
	dummy = SPI2BUF;
	SPI_SEND_AND_WAIT(BIT_FIELD_SET | reg);

	dummy = SPI2BUF;
	SPI_SEND_AND_WAIT(mask);
	
	CS_ENC = 1;
}	

void ENC_ResetBit(unsigned char banked_reg, unsigned char mask)
{
	BYTE reg = banked_reg & REG_MASK;
	
	if (reg < EIE)
		ENC_SetCurrentBank(banked_reg);
	
	CS_ENC = 0;
	
	dummy = SPI2BUF;
	SPI_SEND_AND_WAIT(BIT_FIELD_CLEAR | reg);
	
	dummy = SPI2BUF;
	SPI_SEND_AND_WAIT(mask);
	
	CS_ENC = 1;
}	



void ENC_SetHardwareAddress(const ETH_ADDR *address)
{
	unsigned char * mac = address->byte;
	ENC_WriteControlRegister(MAADR1, mac[0]);
	ENC_WriteControlRegister(MAADR2, mac[1]);
	ENC_WriteControlRegister(MAADR3, mac[2]);
	ENC_WriteControlRegister(MAADR4, mac[3]);
	ENC_WriteControlRegister(MAADR5, mac[4]);
	ENC_WriteControlRegister(MAADR6, mac[5]);
	
}	



void ENC_ReadMemoryBuffer(unsigned char* buffer, int count)
{
	
	CS_ENC = 0;
	dummy = SPI2BUF;
	SPI_SEND_AND_WAIT(READ_BUFFER_MEMORY);
	dummy = SPI2BUF;
	
	while (count > 0)
	{
		SPI_SEND_AND_WAIT(0x00);
		*buffer = SPI2BUF;
		buffer++;
		count--;
	}	
	CS_ENC = 1;
}	


//void ENC_WriteMemoryBuffer(const unsigned char* buffer, int count)
//{
//	CS = 0;
//	
//	dummy = SPI2BUF;
//	SPI_SEND_AND_WAIT(WRITE_BUFFER_MEMORY);
//	
//	while (count > 0)
//	{
//		SPI_SEND_AND_WAIT(*buffer);
//		buffer++;
//		count--;
//	}	
//	
//	CS = 1;
//
//	dummy = SPI2BUF;
//	SPI2STATbits.SPIROV = 0;
//}	
//	
	
void ENC_SendPacket(const unsigned char* packet, unsigned int length)
{
	ENC_WriteControlRegister(EWRPTL, TX_START & 0xFF);
	ENC_WriteControlRegister(EWRPTH, TX_START >> 8);
	
	ENC_WriteControlRegister(ETXNDL, (TX_START + length) & 0xFF);
	ENC_WriteControlRegister(ETXNDH, (TX_START + length) >> 8);
	
	
	CS_ENC = 0;
	
	dummy = SPI2BUF;
	SPI_SEND_AND_WAIT(WRITE_BUFFER_MEMORY);
	
	dummy = SPI2BUF;
	SPI_SEND_AND_WAIT(0x00); // pierwszy bajt

	while (length > 0)
	{
		dummy = SPI2BUF;
		SPI_SEND_AND_WAIT(*packet);
		packet++;
		length--;
	}	
	
	dummy = SPI2BUF;
	SPI2STATbits.SPIROV = 0;
	
	CS_ENC = 1;
	ENC_SetBit(ECON1, ECON1_TXRTS);
}

unsigned short ENC_ReceivePacket(unsigned char* buffer, unsigned short capacity)
{
	if (ENC_ReadControlRegister(EPKTCNT) == 0)
		return 0;
		
	ENC_WriteControlRegister(ERDPTL, next_packet_ptr);
	ENC_WriteControlRegister(ERDPTH, next_packet_ptr >> 8);
	
	RECEIVED_STATUS status;
	ENC_ReadMemoryBuffer((void*)&status, sizeof(RECEIVED_STATUS));
	
	next_packet_ptr = status.next_packet_address;

	if (!status.recv_ok)
		return 0; // nic sie nie odebralo

	unsigned short len = status.frame_length - 4; // dlugosc ramki bez CRC	
	len = (len > capacity) ? capacity : len;
		
	ENC_ReadMemoryBuffer(buffer, len);
	
	ENC_WriteControlRegister(ERXRDPTL, next_packet_ptr);
	ENC_WriteControlRegister(ERXRDPTH, next_packet_ptr >> 8);
	
	ENC_SetBit(ECON2, ECON2_PKTDEC);

	return len;
}	

