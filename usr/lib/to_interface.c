/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/to_interface.c 1.1                          */
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
static char sccsid[] = "@(#)94  1.1  src/ipfl/usr/lib/to_interface.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:26:25";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: to_interface.c,v 1.8 2002/01/28 06:50:48 darrenr Exp $
 */

#include "ipf.h"


int to_interface(fdp, to, linenum)
frdest_t *fdp;
char *to;
int linenum;
{
	char *s;

	s = strchr(to, ':');
	fdp->fd_ifp = NULL;
	if (s) {
		*s++ = '\0';
		if (hostnum((u_32_t *)&fdp->fd_ip, s, linenum, NULL) == -1)
			return -1;
	}
	(void) strncpy(fdp->fd_ifname, to, sizeof(fdp->fd_ifname) - 1);
	fdp->fd_ifname[sizeof(fdp->fd_ifname) - 1] = '\0';
	return 0;
}
