/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/printhostmask.c 1.1                         */
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
static char sccsid[] = "@(#)70  1.1  src/ipfl/usr/lib/printhostmask.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:25:52";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: printhostmask.c,v 1.8 2002/04/11 15:01:19 darrenr Exp $
 */

#include "ipf.h"


void	printhostmask(v, addr, mask)
int	v;
u_32_t	*addr, *mask;
{
#ifdef  USE_INET6
	char ipbuf[64];
#else
	struct in_addr ipa;
#endif

	if (!*addr && !*mask)
		printf("any");
	else {
#ifdef  USE_INET6
		void *ptr = addr;
		int af;

		if (v == 4) {
			ptr = addr;
			af = AF_INET;
		} else if (v == 6) {
			ptr = addr;
			af = AF_INET6;
		} else
			af = 0;
		printf("%s", inet_ntop(af, ptr, ipbuf, sizeof(ipbuf)));
#else
		ipa.s_addr = *addr;
		printf("%s", inet_ntoa(ipa));
#endif
		printmask(mask);
	}
}
