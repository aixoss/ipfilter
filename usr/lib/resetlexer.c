/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/resetlexer.c 1.1                            */
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
static char sccsid[] = "@(#)89  1.1  src/ipfl/usr/lib/resetlexer.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:26:19";
#include "ipf.h"

long	string_start = -1;
long	string_end = -1;
char	*string_val = NULL;
long	pos = 0;


void resetlexer()
{
	string_start = -1;
	string_end = -1;
	string_val = NULL;
	pos = 0;
}
