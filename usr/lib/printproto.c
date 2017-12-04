/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/printproto.c 1.1                            */
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
static char sccsid[] = "@(#)79  1.1  src/ipfl/usr/lib/printproto.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:26:07";
/*
 * Copyright (C) 1993-2005 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 */

#include "ipf.h"


#if !defined(lint)
static const char rcsid[] = "@(#)$Id: printproto.c,v 1.1.2.1 2005/06/12 07:21:53 darrenr Exp $";
#endif


void printproto(pr, p, np)
struct protoent *pr;
int p;
ipnat_t *np;
{
	if (np != NULL) {
		if ((np->in_flags & IPN_TCPUDP) == IPN_TCPUDP)
			printf("tcp/udp");
		else if (np->in_flags & IPN_TCP)
			printf("tcp");
		else if (np->in_flags & IPN_UDP)
			printf("udp");
		else if (np->in_flags & IPN_ICMPQUERY)
			printf("icmp");
#ifdef _AIX51
		/*
		 * To make up for "ip = 252" and "hopopt = 0" in /etc/protocols
		 */
		else if (np->in_p == 0)
			printf("ip");
#endif
		else if (pr != NULL)
			printf("%s", pr->p_name);
		else
			printf("%d", np->in_p);
	} else {
#ifdef _AIX51
		if (p == 0)
			printf("ip");
		else
#endif
		if (pr != NULL)
			printf("%s", pr->p_name);
		else
			printf("%d", p);
	}
}
