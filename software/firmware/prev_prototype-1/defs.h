#if !defined(_DEFS_H_)
#define _DEFS_H_

#define LED0	LATBbits.LATB12
#define LED1	LATBbits.LATB13
#define LED2	LATBbits.LATB14

#define CS_ENC	LATGbits.LATG9
#define CS_REF	LATFbits.LATF3
#define CS_AMP	LATFbits.LATF5

#define	SDI		PORTGbits.RG7
#define SDO		LATGbits.LATG8
#define SCK		LATGbits.LATG6

void __delay32(DWORD cnt);

#endif // _DEFS_H_

