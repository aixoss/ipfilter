/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/optname.c 1.1                               */
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
static char sccsid[] = "@(#)51  1.1  src/ipfl/usr/lib/optname.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:25:32";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: optname.c,v 1.3 2001/06/09 17:09:24 darrenr Exp $
 */

#include "ipf.h"


u_32_t optname(cp, sp, linenum)
char ***cp;
u_short *sp;
int linenum;
{
	struct ipopt_names *io, *so;
	u_long msk = 0;
	u_short smsk = 0;
	char *s;
	int sec = 0;

	for (s = strtok(**cp, ","); s; s = strtok(NULL, ",")) {
		for (io = ionames; io->on_name; io++)
			if (!strcasecmp(s, io->on_name)) {
				msk |= io->on_bit;
				break;
			}
		if (!io->on_name) {
			fprintf(stderr, "%d: unknown IP option name %s\n",
				linenum, s);
			return 0;
		}
		if (!strcasecmp(s, "sec-class"))
			sec = 1;
	}

	if (sec && !*(*cp + 1)) {
		fprintf(stderr, "%d: missing security level after sec-class\n",
			linenum);
		return 0;
	}

	if (sec) {
		(*cp)++;
		for (s = strtok(**cp, ","); s; s = strtok(NULL, ",")) {
			for (so = secclass; so->on_name; so++)
				if (!strcasecmp(s, so->on_name)) {
					smsk |= so->on_bit;
					break;
				}
			if (!so->on_name) {
				fprintf(stderr,
					"%d: no such security level: %s\n",
					linenum, s);
				return 0;
			}
		}
		if (smsk)
			*sp = smsk;
	}
	return msk;
}
