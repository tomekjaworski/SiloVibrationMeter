#if !defined(_COMM_H_)
#define _COMM_H_

#include <uart.h>

#undef putc
#undef getc
#undef puts

void putc(char c);
void puts(const char* str);

char* gets(char* str);
char getc(void);

#define KEYPRESSED (DataRdyUART2())

#endif //_COMM_H_
