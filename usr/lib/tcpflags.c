/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/tcpflags.c 1.1                              */
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
static char sccsid[] = "@(#)92  1.1  src/ipfl/usr/lib/tcpflags.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:26:23";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: tcpflags.c,v 1.3 2002/11/02 07:18:01 darrenr Exp $
 */

#include "ipf.h"


/*
 * ECN is a new addition to TCP - RFC 2481
 */
#ifndef TH_ECN
# define	TH_ECN  0x40
#endif
#ifndef TH_CWR
# define	TH_CWR  0x80
#endif

extern	char	flagset[];
extern	u_char	flags[];


u_char tcpflags(flgs)
char *flgs;
{
	u_char tcpf = 0;
	char *s, *t;

	for (s = flgs; *s; s++) {
		if (*s == 'W')
			tcpf |= TH_CWR;
		else {
			if (!(t = strchr(flagset, *s))) {
				return 0;
			}
			tcpf |= flags[t - flagset];
		}
	}
	return tcpf;
}
