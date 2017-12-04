/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/loglevel.c 1.1                              */
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
static char sccsid[] = "@(#)45  1.1  src/ipfl/usr/lib/loglevel.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:25:26";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: loglevel.c,v 1.5 2001/06/09 17:09:24 darrenr Exp $
 */

#include "ipf.h"


int loglevel(cpp, facpri, linenum)
char **cpp;
u_int *facpri;
int linenum;
{
	int fac, pri;
	char *s;

	fac = 0;
	pri = 0;
	if (!*++cpp) {
		fprintf(stderr, "%d: %s\n", linenum,
			"missing identifier after level");
		return -1;
	}

	s = strchr(*cpp, '.');
	if (s) {
		*s++ = '\0';
		fac = fac_findname(*cpp);
		if (fac == -1) {
			fprintf(stderr, "%d: %s %s\n", linenum,
				"Unknown facility", *cpp);
			return -1;
		}
		pri = pri_findname(s);
		if (pri == -1) {
			fprintf(stderr, "%d: %s %s\n", linenum,
				"Unknown priority", s);
			return -1;
		}
	} else {
		pri = pri_findname(*cpp);
		if (pri == -1) {
			fprintf(stderr, "%d: %s %s\n", linenum,
				"Unknown priority", *cpp);
			return -1;
		}
	}
	*facpri = fac|pri;
	return 0;
}
