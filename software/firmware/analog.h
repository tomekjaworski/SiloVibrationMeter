#if !defined(_ANALOG_H_)
#define _ANALOG_H_

union __DAC_COMMAND
{
	WORD command;
	struct
	{
		unsigned value	: 12;
		unsigned SHDN	: 1;
		unsigned GA		: 1;
		unsigned BUF	: 1;
		unsigned AB		: 1;
	};	
};	

union __AMP_COMMAND
{
	WORD command;
	
	struct
	{
		// bajt danych
		union
		{
			struct // A0 = 0 - gain register
			{
				unsigned char GAIN		: 3;
				unsigned char __reserved : 5;
			};
			
			struct // A0 = 1 - channel register
			{
				unsigned char CHANNEL	: 3;
				unsigned char __reserved	: 5;
			};	
		};	
		
		// bajt instrukcji
		unsigned char A0			: 1;
		unsigned char __reserved	: 4;
		unsigned char MODE		: 3;		
	};	
};	

struct __ANALOG_CONFIG
{
	union __DAC_COMMAND dac1;
	union __DAC_COMMAND dac2;
	union __DAC_COMMAND dac3;
	
	union __AMP_COMMAND amp1;
	union __AMP_COMMAND amp2;
	union __AMP_COMMAND amp3;
	
	union __AMP_COMMAND amp_zero;
	
};

extern struct __ANALOG_CONFIG analog_config;

extern void Analog_Update(void);
extern void Analog_Init(void);


#endif // _ANALOG_H_

