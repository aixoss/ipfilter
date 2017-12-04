/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/tcp_flags.c 1.1                             */
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
static char sccsid[] = "@(#)91  1.1  src/ipfl/usr/lib/tcp_flags.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:26:21";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: tcp_flags.c,v 1.8 2004/02/07 18:15:54 darrenr Exp $
 */

#include "ipf.h"

extern	char	flagset[];
extern	u_char	flags[];


u_char tcp_flags(flgs, mask, linenum)
char *flgs;
u_char *mask;
int    linenum;
{
	u_char tcpf = 0, tcpfm = 0;
	char *s;

	s = strchr(flgs, '/');
	if (s)
		*s++ = '\0';

	if (*flgs == '0') {
		tcpf = strtol(flgs, NULL, 0);
	} else {
		tcpf = tcpflags(flgs);
	}

	if (s != NULL) {
		if (*s == '0')
			tcpfm = strtol(s, NULL, 0);
		else
			tcpfm = tcpflags(s);
	}

	if (!tcpfm) {
		if (tcpf == TH_SYN)
			tcpfm = 0xff & ~(TH_ECN|TH_CWR);
		else
			tcpfm = 0xff & ~(TH_ECN);
	}
	*mask = tcpfm;
	return tcpf;
}
