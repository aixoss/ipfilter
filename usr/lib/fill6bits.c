/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/fill6bits.c 1.1                             */
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
static char sccsid[] = "@(#)10  1.1  src/ipfl/usr/lib/fill6bits.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:24:40";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: fill6bits.c,v 1.5 2002/03/27 15:09:57 darrenr Exp $
 */

#include "ipf.h"


void fill6bits(bits, msk)
int bits;
u_int *msk;
{
	if (bits == 0) {
		msk[0] = 0;
		msk[1] = 0;
		msk[2] = 0;
		msk[3] = 0;
		return;
	}

	msk[0] = 0xffffffff;
	msk[1] = 0xffffffff;
	msk[2] = 0xffffffff;
	msk[3] = 0xffffffff;

	if (bits == 128)
		return;
	if (bits > 96) {
		msk[3] = htonl(msk[3] << (128 - bits));
	} else if (bits > 64) {
		msk[3] = 0;
		msk[2] = htonl(msk[2] << (96 - bits));
	} else if (bits > 32) {
		msk[3] = 0;
		msk[2] = 0;
		msk[1] = htonl(msk[1] << (64 - bits));
	} else {
		msk[3] = 0;
		msk[2] = 0;
		msk[1] = 0;
		msk[0] = htonl(msk[0] << (32 - bits));
	}
}
