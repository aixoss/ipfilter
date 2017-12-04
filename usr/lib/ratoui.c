/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/ratoui.c 1.1                                */
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
static char sccsid[] = "@(#)84  1.1  src/ipfl/usr/lib/ratoui.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:26:13";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: ratoui.c,v 1.4 2001/06/09 17:09:25 darrenr Exp $
 */

#include "ipf.h"


int	ratoui(ps, pi, min, max)
char 	*ps;
u_int	*pi, min, max;
{
	u_int i;
	char *pe;

	i = (u_int)strtol(ps, &pe, 0);
	if (*pe != '\0' || i < min || i > max)
		return 0;
	*pi = i;
	return 1;
}
