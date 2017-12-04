/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/printpoolnode.c 1.1                         */
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
static char sccsid[] = "@(#)77  1.1  src/ipfl/usr/lib/printpoolnode.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:26:05";
/*
 * Copyright (C) 2002 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 */

#include "ipf.h"

#define	PRINTF	(void)printf
#define	FPRINTF	(void)fprintf

ip_pool_node_t *printpoolnode(np, opts)
ip_pool_node_t *np;
int opts;
{

	if ((opts & OPT_DEBUG) == 0) {
		putchar(' ');
		if (np->ipn_info == 1)
			PRINTF("! ");
		printip((u_32_t *)&np->ipn_addr.adf_addr.in4);
		printmask((u_32_t *)&np->ipn_mask.adf_addr);
	} else {
		PRINTF("\t\t%s%s", np->ipn_info ? "! " : "",
			inet_ntoa(np->ipn_addr.adf_addr.in4));
		printmask((u_32_t *)&np->ipn_mask.adf_addr);
		PRINTF("\n\t\tHits %lu\tName %s\n",
			np->ipn_hits, np->ipn_name);
	}
	return np->ipn_next;
}
