/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/tcpoptnames.c 1.1                           */
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
static char sccsid[] = "@(#)93  1.1  src/ipfl/usr/lib/tcpoptnames.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:26:24";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: tcpoptnames.c,v 1.5 2002/01/28 06:50:48 darrenr Exp $
 */

#include "ipf.h"


struct	ipopt_names	tcpoptnames[] ={
	{ TCPOPT_NOP,			0x000001,	1,	"nop" },
	{ TCPOPT_MAXSEG,		0x000002,	4,	"maxseg" },
	{ TCPOPT_WINDOW,		0x000004,	3,	"wscale" },
	{ TCPOPT_SACK_PERMITTED,	0x000008,	2,	"sackok" },
	{ TCPOPT_SACK,			0x000010,	3,	"sack" },
	{ TCPOPT_TIMESTAMP,		0x000020,	10,	"tstamp" },
	{ 0, 		0,	0,	(char *)NULL }     /* must be last */
};
