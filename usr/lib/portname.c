/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/portname.c 1.1                              */
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
static char sccsid[] = "@(#)56  1.1  src/ipfl/usr/lib/portname.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:25:37";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: portname.c,v 1.7 2003/08/14 14:27:43 darrenr Exp $
 */
#include "ipf.h"


char	*portname(pr, port)
int	pr, port;
{
	static	char	buf[32];
	struct	protoent	*p = NULL;
	struct	servent	*sv = NULL, *sv1 = NULL;

	if ((opts & OPT_NORESOLVE) == 0) {
		if (pr == -1) {
			if ((sv = getservbyport(htons(port), "tcp"))) {
				strncpy(buf, sv->s_name, sizeof(buf)-1);
				buf[sizeof(buf)-1] = '\0';
				sv1 = getservbyport(htons(port), "udp");
				sv = strncasecmp(buf, sv->s_name, strlen(buf)) ?
				     NULL : sv1;
			}
			if (sv)
				return buf;
		} else if ((pr != -2) && (p = getprotobynumber(pr))) {
			if ((sv = getservbyport(htons(port), p->p_name))) {
				strncpy(buf, sv->s_name, sizeof(buf)-1);
				buf[sizeof(buf)-1] = '\0';
				return buf;
			}
		}
	}

	(void) sprintf(buf, "%d", port);
	return buf;
}
