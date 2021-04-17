#if !defined(_DEFS_H_)
#define _DEFS_H_

#define FCY 40000000UL
#include <libpic30.h>

#define FULL_SPEED

#define _AUTOPSV	__attribute__((auto_psv))
#define _NOAUTOPSV	__attribute__((no_auto_psv))

//#define LED0	LATBbits.LATB12
//#define LED1	LATBbits.LATB13
//#define LED2	LATBbits.LATB14

#define LED0	LATDbits.LATD3
#define LED1	LATDbits.LATD2
#define LED2	LATDbits.LATD1

#define CS_ENC	LATGbits.LATG9

#define CS_REF1	LATEbits.LATE7
#define CS_REF2	LATEbits.LATE6
#define CS_REF3	LATEbits.LATE5

#define CS_AMP1	LATEbits.LATE2
#define CS_AMP2	LATEbits.LATE3
#define CS_AMP3	LATEbits.LATE4

#define	SDI		PORTGbits.RG7
#define SDO		LATGbits.LATG8
#define SCK		LATGbits.LATG6

#define NEG_SET(x, y) {x = !(y);}

//void __delay32(DWORD cnt);

#endif // _DEFS_H_

