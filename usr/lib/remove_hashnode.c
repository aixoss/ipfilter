/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/remove_hashnode.c 1.1                       */
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
static char sccsid[] = "@(#)86  1.1  src/ipfl/usr/lib/remove_hashnode.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:26:15";
/*
 * Copyright (C) 2002 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: remove_hashnode.c,v 1.1 2003/04/13 06:40:14 darrenr Exp $
 */

#include <fcntl.h>
#include <sys/ioctl.h>
#include "ipf.h"
#include "netinet/ip_lookup.h"
#include "netinet/ip_htable.h"

static int hashfd = -1;


int remove_hashnode(unit, name, node, iocfunc)
int unit;
char *name;
iphtent_t *node;
ioctlfunc_t iocfunc;
{
	iplookupop_t op;
	iphtent_t ipe;

	if ((hashfd == -1) && ((opts & OPT_DONOTHING) == 0))
		hashfd = open(IPLOOKUP_NAME, O_RDWR);
	if ((hashfd == -1) && ((opts & OPT_DONOTHING) == 0))
		return -1;

	op.iplo_type = IPLT_HASH;
	op.iplo_unit = unit;
	op.iplo_size = sizeof(ipe);
	op.iplo_struct = &ipe;
	op.iplo_arg = 0;
	strncpy(op.iplo_name, name, sizeof(op.iplo_name));

	bzero((char *)&ipe, sizeof(ipe));
	bcopy((char *)&node->ipe_addr, (char *)&ipe.ipe_addr,
	      sizeof(ipe.ipe_addr));
	bcopy((char *)&node->ipe_mask, (char *)&ipe.ipe_mask,
	      sizeof(ipe.ipe_mask));

	if (opts & OPT_DEBUG) {
		printf("\t%s - ", inet_ntoa(ipe.ipe_addr.in4));
		printf("%s\n", inet_ntoa(ipe.ipe_mask.in4));
	}

	if ((*iocfunc)(hashfd, SIOCLOOKUPDELNODE, &op))
		if (!(opts & OPT_DONOTHING)) {
			perror("remove_hash:SIOCLOOKUPDELNODE");
			return -1;
		}
	return 0;
}
