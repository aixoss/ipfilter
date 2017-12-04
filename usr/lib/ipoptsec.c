/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/ipoptsec.c 1.1                              */
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
static char sccsid[] = "@(#)37  1.1  src/ipfl/usr/lib/ipoptsec.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:25:18";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: ipoptsec.c,v 1.2 2002/01/28 06:50:46 darrenr Exp $
 */

#include "ipf.h"


struct	ipopt_names	secclass[] = {
	{ IPSO_CLASS_RES4,	0x01,	0, "reserv-4" },
	{ IPSO_CLASS_TOPS,	0x02,	0, "topsecret" },
	{ IPSO_CLASS_SECR,	0x04,	0, "secret" },
	{ IPSO_CLASS_RES3,	0x08,	0, "reserv-3" },
	{ IPSO_CLASS_CONF,	0x10,	0, "confid" },
	{ IPSO_CLASS_UNCL,	0x20,	0, "unclass" },
	{ IPSO_CLASS_RES2,	0x40,	0, "reserv-2" },
	{ IPSO_CLASS_RES1,	0x80,	0, "reserv-1" },
	{ 0, 0, 0, NULL }	/* must be last */
};


u_char seclevel(slevel)
char *slevel;
{
	struct ipopt_names *so;

	for (so = secclass; so->on_name; so++)
		if (!strcasecmp(slevel, so->on_name))
			break;

	if (!so->on_name) {
		fprintf(stderr, "no such security level: %s\n", slevel);
		return 0;
	}
	return (u_char)so->on_value;
}


u_char secbit(class)
int class;
{
	struct ipopt_names *so;

	for (so = secclass; so->on_name; so++)
		if (so->on_value == class)
			break;

	if (!so->on_name) {
		fprintf(stderr, "no such security class: %d\n", class);
		return 0;
	}
	return (u_char)so->on_bit;
}
