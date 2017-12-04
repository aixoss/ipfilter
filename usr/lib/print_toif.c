/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/print_toif.c 1.1                            */
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
static char sccsid[] = "@(#)59  1.1  src/ipfl/usr/lib/print_toif.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:25:40";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: print_toif.c,v 1.8 2002/01/28 06:50:47 darrenr Exp $
 */

#include "ipf.h"


void print_toif(tag, fdp)
char *tag;
frdest_t *fdp;
{
	printf("%s %s%s", tag, fdp->fd_ifname,
		     (fdp->fd_ifp || (long)fdp->fd_ifp == -1) ? "" : "(!)");
#ifdef	USE_INET6
	if (use_inet6 && IP6_NOTZERO(&fdp->fd_ip6.in6)) {
		char ipv6addr[80];

		inet_ntop(AF_INET6, &fdp->fd_ip6, ipv6addr,
			  sizeof(fdp->fd_ip6));
		printf(":%s", ipv6addr);
	} else
#endif
		if (fdp->fd_ip.s_addr)
			printf(":%s", inet_ntoa(fdp->fd_ip));
	putchar(' ');
}
