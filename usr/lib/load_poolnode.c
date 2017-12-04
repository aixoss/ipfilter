/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/load_poolnode.c 1.1                         */
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
static char sccsid[] = "@(#)44  1.1  src/ipfl/usr/lib/load_poolnode.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:25:25";
/*
 * Copyright (C) 2002 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: load_poolnode.c,v 1.3.2.1 2004/03/06 14:33:29 darrenr Exp $
 */

#include <fcntl.h>
#include <sys/ioctl.h>
#include "ipf.h"
#include "netinet/ip_lookup.h"
#include "netinet/ip_pool.h"

static int poolfd = -1;


int load_poolnode(role, name, node, iocfunc)
int role;
char *name;
ip_pool_node_t *node;
ioctlfunc_t iocfunc;
{
	ip_pool_node_t pn;
	iplookupop_t op;
	int err;

	if ((poolfd == -1) && ((opts & OPT_DONOTHING) == 0))
		poolfd = open(IPLOOKUP_NAME, O_RDWR);
	if ((poolfd == -1) && ((opts & OPT_DONOTHING) == 0))
		return -1;

	op.iplo_unit = role;
	op.iplo_type = IPLT_POOL;
	op.iplo_arg = 0;
	op.iplo_struct = &pn;
	op.iplo_size = sizeof(pn);
	strncpy(op.iplo_name, name, sizeof(op.iplo_name));

	bzero((char *)&pn, sizeof(pn));
	bcopy((char *)&node->ipn_addr, (char *)&pn.ipn_addr,
	      sizeof(pn.ipn_addr));
	bcopy((char *)&node->ipn_mask, (char *)&pn.ipn_mask,
	      sizeof(pn.ipn_mask));
	pn.ipn_info = node->ipn_info;
	strncpy(pn.ipn_name, node->ipn_name, sizeof(pn.ipn_name));

	if ((opts & OPT_REMOVE) == 0)
		err = (*iocfunc)(poolfd, SIOCLOOKUPADDNODE, &op);
	else
		err = (*iocfunc)(poolfd, SIOCLOOKUPDELNODE, &op);

	if (err != 0) {
		if ((opts & OPT_DONOTHING) == 0) {
			perror("load_pool:SIOCLOOKUP*NODE");
			return -1;
		}
	}

	return 0;
}
