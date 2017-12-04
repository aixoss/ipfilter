/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/v6optvalue.c 1.1                            */
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
static char sccsid[] = "@(#)96  1.1  src/ipfl/usr/lib/v6optvalue.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:26:27";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: v6optvalue.c,v 1.1 2003/04/26 04:55:58 darrenr Exp $
 */
#include "ipf.h"



u_32_t getv6optbyname(optname)
char *optname;
{
#ifdef	USE_INET6
	struct ipopt_names *io;

	for (io = v6ionames; io->on_name; io++)
		if (!strcasecmp(optname, io->on_name))
			return io->on_bit;
#endif
	return -1;
}


u_32_t getv6optbyvalue(optval)
int optval;
{
#ifdef	USE_INET6
	struct ipopt_names *io;

	for (io = v6ionames; io->on_name; io++)
		if (io->on_value == optval)
			return io->on_bit;
#endif
	return -1;
}
