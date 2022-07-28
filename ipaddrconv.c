/**
 * See LICENSE for license details.
 *
 * ipaddrconv - convert string form IPv4 adresses to unsigned int and vice versa
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IPV4ARRSZ 16 /* we need only 16 bytes (xxx.xxx.xxx.xxx + NULL) */
#define USAGE \
	"Usage: %s <FLAG> <ARG>\n" \
	"WARNING: WRONG ARGUMENTS WILL MAKE THE PROGRAM BEHAVE INCORRECTLY!\n" \
	"\n" \
	"FLAG list:\n" \
	"i\t: convert string form IPv4 addr to unsigned int form\n" \
	"s\t: convert unsigned int form IPv4 addr to string form\n" \
	"\n" \
	"ARG list:\n" \
	"For 'i' flag, ARG is STRING form of IPv4 address\n" \
	"For 's' flag, ARG is UNSIGNED INT form of IPv4 address\n" \
	"..............ARG can be in octal (prefix with 0) or\n" \
	".........................in decimal (no prefix)   or\n" \
	".........................in hexadecimal (prefix with 0x)\n"

int inet_aton(char ipv4[]); /* read as "inet address to number" */
void inet_ntoa(unsigned addr, char ipv4[]); /* read as inet number to address */

int
inet_aton(char ipv4[])
{
	unsigned char octet; /* the octet (those numbers separated with dots) */
	unsigned addr = 0; /* the unsigned integer form of ipv4 */
	unsigned cluster = 1 << 24; /* ipv4 cluster size (multiplied with octets) */
	char *ptr; /* pointer to strtok's tokenized chunks */

	ptr = strtok(ipv4, "."); /* get first token */

	while (ptr != NULL)
	{
		octet = strtoul(ptr, NULL, 0); /* compute octet from token */
		addr += octet * cluster; /* compute the addr so far */
		cluster >>= 8; /* shift the cluster bit 8 bits to the right */
		ptr = strtok(NULL, "."); /* get next token */
	}

	return addr;
}

void
inet_ntoa(unsigned addr, char ipv4[])
{
	unsigned divisor = 1 << 24; /* the divisor for addr */
	unsigned char octet; /* the computed octet */
	unsigned char offset = 0; /* this is for sprintf's 1st arg */

	while (addr > 0) /* while addr is positive..., do */
	{
		octet = addr / divisor; /* compute the octet */
		/* put the octet into array, then add sprintf's retval to offset */
		offset += sprintf(ipv4 + offset, "%u", octet);
		ipv4[offset] = '.'; /* put a dot on the array */
		++offset; /* increment the offset as we already added a dot */
		addr -= octet * divisor; /* subtract (octet * divisor) from addr */
		divisor >>= 8; /* shift the divisor bit 8 bits to the right */
	}

	ipv4[offset - 1] = '\0'; /* replace the last dot with a null terminator */
}

int
main(int argc, char *argv[])
{
	char ipv4[IPV4ARRSZ];
	unsigned addr;

	if (argc != 3)
	{
		fprintf(stderr, "Got %d instead of 2 arguments!\n\n", argc - 1);
		fprintf(stderr, USAGE, argv[0]);

		return 1 << 0;
	}

	if (tolower(argv[1][0]) == 'i')
	{
		strncat(ipv4, argv[2], IPV4ARRSZ - 1);
		printf("%u\n", inet_aton(ipv4));
	}
	else if (tolower(argv[1][0]) == 's')
	{
		addr = strtoul(argv[2], NULL, 0);
		inet_ntoa(addr, ipv4);
		printf("%s\n", ipv4);
	}
	else
	{
		fprintf(stderr, "Unknown FLAG: %s\n\n", argv[1]);
		fprintf(stderr, USAGE, argv[0]);

		return 1 << 1;
	}

	return 0;
}
