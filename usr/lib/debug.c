/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/debug.c 1.1                                 */
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
static char sccsid[] = "@(#)06  1.1  src/ipfl/usr/lib/debug.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:24:36";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: debug.c,v 1.6 2001/06/09 17:09:24 darrenr Exp $
 */

#if defined(__STDC__)
# include <stdarg.h>
#else
# include <varargs.h>
#endif
#include <stdio.h>

#include "ipt.h"
#include "opts.h"


#ifdef	__STDC__
void	debug(char *fmt, ...)
#else
void	debug(fmt, va_alist)
char *fmt;
va_dcl
#endif
{
	va_list pvar;

	va_start(pvar, fmt);

	if (opts & OPT_DEBUG)
		vprintf(fmt, pvar);
	va_end(pvar);
}
