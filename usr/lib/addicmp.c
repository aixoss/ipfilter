/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/addicmp.c 1.1                               */
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
static char sccsid[] = "@(#)98  1.1  src/ipfl/usr/lib/addicmp.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:24:27";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: addicmp.c,v 1.10.2.4 2006/02/25 17:41:57 darrenr Exp $
 */

#include <ctype.h>

#include "ipf.h"


char	*icmptypes[MAX_ICMPTYPE + 1] = {
	"echorep", (char *)NULL, (char *)NULL, "unreach", "squench",
	"redir", (char *)NULL, (char *)NULL, "echo", "routerad",
	"routersol", "timex", "paramprob", "timest", "timestrep",
	"inforeq", "inforep", "maskreq", "maskrep", "END"
};
