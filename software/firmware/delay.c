#include <p33fj128mc706.h>
#include <libpic30.h>
#include <generic.h>
#include "delay.h"


void Delay10us(unsigned short times)
{
	unsigned short i;
	while(times-- > 0)
	{
		i = 80;
		while(TRUE)
		{
			i--;
			if (i == 0)
			break;
		}	
	}
}

void DelayMS(unsigned short ms)
{
	while(ms--)
		__delay32(40000);
}	

void Delay100US(unsigned short _100us)
{
	while(_100us--)
		__delay32(4000);
}