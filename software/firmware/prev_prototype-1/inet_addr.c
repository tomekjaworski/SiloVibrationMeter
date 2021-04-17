#include <stddef.h>
#include "net.h"
//#include <ctype.h>

#define ISASCII(x) (((x) >= 0) && ((x) <= 127))
#define ISDIGIT(x) (((x) >= '0') && ((x) <= '9'))
#define ISXDIGIT(x)	((((x) >= '0') && ((x) <= '9')) || (((x) >= 'A') && ((x) <= 'F')))
#define ISLOWER(x)	(((x) >= 'a') && ((x) <= 'z'))
#define ISSPACE(x)	(((x) == ' ') || ((x) == '\t') || ((x) == '\n') || ((x) == '\v') || ((x) == '\f') || ((x) == '\r'))

static inline unsigned long int htonl(unsigned long int x);
unsigned long int inet_addr(const char* cp);
int inet_aton(const char *cp, unsigned long int *addr);


unsigned long int inet_addr(const char* cp)
{
	unsigned long int ip;
	if (inet_aton(cp, &ip) == 0)
		return 0;
	return ip;
}	

int inet_aton(const char *cp, unsigned long int *addr)
{
	register unsigned long int val;
	register long int base, n;
	register unsigned char c;
	unsigned long int parts[4];
	register unsigned long int *pp = parts;

	c = *cp;
	for (;;) {
		/*
		 * Collect number up to ``.''.
		 * Values are specified as for C:
		 * 0x=hex, 0=octal, isdigit=decimal.
		 */
		if (!ISDIGIT(c))
			return (0);
		val = 0; base = 10;
		if (c == '0') {
			c = *++cp;
			if (c == 'x' || c == 'X')
				base = 16, c = *++cp;
			else
				base = 8;
		}
		for (;;) {
			if (ISASCII(c) && ISDIGIT(c)) {
				val = (val * base) + (c - '0');
				c = *++cp;
			} else if (base == 16 && ISASCII(c) && ISXDIGIT(c)) {
				val = (val << 4) |
					(c + 10 - (ISLOWER(c) ? 'a' : 'A'));
				c = *++cp;
			} else
				break;
		}
		if (c == '.') {
			/*
			 * Internet format:
			 *	a.b.c.d
			 *	a.b.c	(with c treated as 16 bits)
			 *	a.b	(with b treated as 24 bits)
			 */
			if (pp >= parts + 3)
				return (0);
			*pp++ = val;
			c = *++cp;
		} else
			break;
	}
	/*
	 * Check for trailing characters.
	 */
	if (c != '\0' && (!ISASCII(c) || !ISSPACE(c)))
		return (0);
	/*
	 * Concoct the address according to
	 * the number of parts specified.
	 */
	n = pp - parts + 1;
	switch (n) {

	case 0:
		return (0);		/* initial nondigit */

	case 1:				/* a -- 32 bits */
		break;

	case 2:				/* a.b -- 8.24 bits */
		if ((val > 0xffffff) || (parts[0] > 0xff))
			return (0);
		val |= parts[0] << 24;
		break;

	case 3:				/* a.b.c -- 8.8.16 bits */
		if ((val > 0xffff) || (parts[0] > 0xff) || (parts[1] > 0xff))
			return (0);
		val |= (parts[0] << 24) | (parts[1] << 16);
		break;

	case 4:				/* a.b.c.d -- 8.8.8.8 bits */
		if ((val > 0xff) || (parts[0] > 0xff) || (parts[1] > 0xff) || (parts[2] > 0xff))
			return (0);
		val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
		break;
	}
	if (addr)
		*addr = htonl(val); // htonl(val);
	return (1);
}

static inline unsigned long int htonl(unsigned long int x)
{
	unsigned char *s = (unsigned char *)&x;
	return (unsigned long int)s[0] << 24 | (unsigned long int)s[1] << 16 | (unsigned long int)s[2] << 8 | (unsigned long int)s[3];
}