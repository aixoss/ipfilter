/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/getsumd.c 1.1                               */
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
static char sccsid[] = "@(#)21  1.1  src/ipfl/usr/lib/getsumd.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:24:56";
#include "ipf.h"

char *getsumd(sum)
u_32_t sum;
{
	static char sumdbuf[17];

	if (sum & NAT_HW_CKSUM)
		sprintf(sumdbuf, "hw(%#0x)", sum & 0xffff);
	else
		sprintf(sumdbuf, "%#0x", sum);
	return sumdbuf;
}
