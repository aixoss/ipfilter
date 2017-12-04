/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/printtunable.c 1.1                          */
/*                                                                        */
/* Licensed Materials - Property of IBM                                   */
/*                                                                        */
/* Restricted Materials of IBM                                            */
/*                                                                        */
/* (C) COPYRIGHT International Business Machines Corp. 2006               */
/* All Rights Reserved                                                    */
/*                                                                        */
/* US Government Users Restricted Rights - Use, duplication or            */
/* disclosure restricted by GSA ADP Schedule Contract with IBM Corp.      */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
static char sccsid[] = "@(#)82  1.1  src/ipfl/usr/lib/printtunable.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:26:11";
#include "ipf.h"

void printtunable(tup)
ipftune_t *tup;
{
	printf("%s\tmin %#lx\tmax %#lx\tcurrent ",
		tup->ipft_name, tup->ipft_min, tup->ipft_max);
	if (tup->ipft_sz == sizeof(u_long))
		printf("%lu\n", tup->ipft_vlong);
	else if (tup->ipft_sz == sizeof(u_int))
		printf("%u\n", tup->ipft_vint);
	else if (tup->ipft_sz == sizeof(u_short))
		printf("%hu\n", tup->ipft_vshort);
	else if (tup->ipft_sz == sizeof(u_char))
		printf("%u\n", (u_int)tup->ipft_vchar);
	else {
		printf("sz = %d\n", tup->ipft_sz);
	}
}
