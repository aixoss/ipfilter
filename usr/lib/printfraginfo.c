/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/printfraginfo.c 1.1                         */
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
static char sccsid[] = "@(#)64  1.1  src/ipfl/usr/lib/printfraginfo.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:25:45";
/*
 * Copyright (C) 2004 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: printfraginfo.c,v 1.1.2.2 2004/03/23 15:15:45 darrenr Exp $
 */
#include "ipf.h"
#include "kmem.h"

void printfraginfo(prefix, ifr)
char *prefix;
struct ipfr *ifr;
{
	frentry_t fr;

	fr.fr_flags = 0xffffffff;

	printf("%s%s -> ", prefix, hostname(4, &ifr->ipfr_src));
	if (kmemcpy((char *)&fr, (u_long)ifr->ipfr_rule,
		    sizeof(fr)) == -1)
		return;
	printf("%s id %d ttl %d pr %d seen0 %d ifp %p tos %#02x = %#x\n",
		hostname(4, &ifr->ipfr_dst), ifr->ipfr_id, ifr->ipfr_seen0,
		ifr->ipfr_ttl, ifr->ipfr_p, ifr->ipfr_ifp, ifr->ipfr_tos,
		fr.fr_flags);
}
