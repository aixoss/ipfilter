/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/ntomask.c 1.1                               */
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
static char sccsid[] = "@(#)50  1.1  src/ipfl/usr/lib/ntomask.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:25:31";
#include "ipf.h"

int ntomask(v, nbits, ap)
int v, nbits;
u_32_t *ap;
{
	u_32_t mask;

	if (nbits < 0)
		return -1;

	switch (v)
	{
	case 4 :
		if (nbits > 32 || use_inet6 != 0)
			return -1;
		if (nbits == 0) {
			mask = 0;
		} else {
			mask = 0xffffffff;
			mask <<= (32 - nbits);
		}
		*ap = htonl(mask);
		break;

	case 6 :
		if ((nbits > 128) || (use_inet6 == 0))
			return -1;
		fill6bits(nbits, ap);
		break;

	default :
		return -1;
	}
	return 0;
}
