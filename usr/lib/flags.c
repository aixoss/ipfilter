/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/flags.c 1.1                                 */
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
static char sccsid[] = "@(#)11  1.1  src/ipfl/usr/lib/flags.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:24:41";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: flags.c,v 1.4 2002/11/02 07:16:36 darrenr Exp $
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

char	flagset[] = "FSRPAUEC";
u_char	flags[] = { TH_FIN, TH_SYN, TH_RST, TH_PUSH, TH_ACK, TH_URG,
		    TH_ECN, TH_CWR };
