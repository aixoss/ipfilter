/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/remove_poolnode.c 1.1                       */
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
static char sccsid[] = "@(#)88  1.1  src/ipfl/usr/lib/remove_poolnode.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:26:18";
/*
 * Copyright (C) 2002 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: remove_poolnode.c,v 1.3 2003/11/22 10:14:36 darrenr Exp $
 */

#include <fcntl.h>
#include <sys/ioctl.h>
#include "ipf.h"
#include "netinet/ip_lookup.h"
#include "netinet/ip_pool.h"

static int poolfd = -1;


int remove_poolnode(unit, name, node, iocfunc)
int unit;
char *name;
ip_pool_node_t *node;
ioctlfunc_t iocfunc;
{
	ip_pool_node_t pn;
	iplookupop_t op;

	if ((poolfd == -1) && ((opts & OPT_DONOTHING) == 0))
		poolfd = open(IPLOOKUP_NAME, O_RDWR);
	if ((poolfd == -1) && ((opts & OPT_DONOTHING) == 0))
		return -1;

	op.iplo_unit = unit;
	op.iplo_type = IPLT_POOL;
	op.iplo_arg = 0;
	strncpy(op.iplo_name, name, sizeof(op.iplo_name));
	op.iplo_struct = &pn;
	op.iplo_size = sizeof(pn);

	bzero((char *)&pn, sizeof(pn));
	bcopy((char *)&node->ipn_addr, (char *)&pn.ipn_addr,
	      sizeof(pn.ipn_addr));
	bcopy((char *)&node->ipn_mask, (char *)&pn.ipn_mask,
	      sizeof(pn.ipn_mask));
	pn.ipn_info = node->ipn_info;
	strncpy(pn.ipn_name, node->ipn_name, sizeof(pn.ipn_name));

	if ((*iocfunc)(poolfd, SIOCLOOKUPDELNODE, &op)) {
		if ((opts & OPT_DONOTHING) == 0) {
			perror("remove_pool:SIOCLOOKUPDELNODE");
			return -1;
		}
	}

	return 0;
}
