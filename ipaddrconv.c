/**
 * Made by Richie Seputro
 * on July 28, 2022
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


int
ipv4_to_uint(char ipv4[])
{
	unsigned char idx; /* the index for ipv4 array */
	unsigned char octet = 0; /* the octet (those numbers separated with dots) */
	unsigned addr = 0; /* the unsigned integer form of ipv4 */
	unsigned cluster = 1 << 24; /* ipv4 cluster size (multiplied with octets) */


	for (idx = 0; ipv4[idx] != '\0'; ++idx)
	{
		if (ipv4[idx] == '.') /* if we found a dot */
		{
			addr = addr + octet * cluster; /* compute the address so far */
			cluster /= 1 << 8; /* divide the cluster by 2^8 */
			octet = 0; /* reset the octet value */
		}
		else
		{
			octet = octet * 10 + (ipv4[idx] - '0'); /* compute the octet */
		}
	}

	/* add the last octet to addr */
	addr = addr + octet * cluster;

	return addr;
}

void
uint_to_ipv4(unsigned addr, char ipv4[])
{
	unsigned divisor = 1 << 24; /* the divisor for our addr */
	unsigned char octet; /* the computed octet */
	unsigned char offset = 0; /* this is for sprintf's 1st arg */

	while (addr > 0) /* while we still can divide addr, do... */
	{
		octet = addr / divisor; /* compute the octet */
		/* put the octet into array, then add sprintf's retval to offset */
		offset += sprintf(ipv4 + offset, "%u", octet);
		ipv4[offset] = '.'; /* put a dot on the array */
		++offset;
		addr -= octet * divisor; /* subtract (octet * divisor) from addr */
		divisor /= 1 << 8; /* divide the divisor by 2^8 */
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
		fprintf(stderr, "Only got %d out of 2 arguments!\n\n", argc - 1);
		fprintf(stderr, USAGE, argv[0]);

		return 1 << 0;
	}

	if (tolower(argv[1][0]) == 'i')
	{
		strncat(ipv4, argv[2], IPV4ARRSZ - 1);
		printf("%u\n", ipv4_to_uint(ipv4));
	}
	else if (tolower(argv[1][0]) == 's')
	{
		addr = strtoul(argv[2], NULL, 0);
		uint_to_ipv4(addr, ipv4);
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
