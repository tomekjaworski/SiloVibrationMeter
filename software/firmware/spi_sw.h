#if !defined(_SPI_SW_H_)
#define _SPI_SW_H_


void AMP_WriteSPI16(WORD value);
void AMP_ResetSPI(void);

void DEV_WriteSPI16(unsigned char devid, WORD value);
void DEV_ResetSPI(unsigned char devid);



#endif // _SPI_SW_H_
