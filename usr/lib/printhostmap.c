/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 71ipfl71c src/ipfl/usr/lib/printhostmap.c 1.1                          */
/*                                                                        */
/* Licensed Materials - Property of IBM                                   */
/*                                                                        */
/* Restricted Materials of IBM                                            */
/*                                                                        */
/* COPYRIGHT International Business Machines Corp. 2006                   */
/* All Rights Reserved                                                    */
/*                                                                        */
/* US Government Users Restricted Rights - Use, duplication or            */
/* disclosure restricted by GSA ADP Schedule Contract with IBM Corp.      */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
static char sccsid[] = "@(#)67  1.1  src/ipfl/usr/lib/printhostmap.c, ipflt, 71ipfl71c 5/8/06 07:25:48";
#include "ipf.h"

void printhostmap(hmp, hv)
hostmap_t *hmp;
u_int hv;
{
	struct in_addr in;

	printf("%s,", inet_ntoa(hmp->hm_srcip));
	printf("%s -> ", inet_ntoa(hmp->hm_dstip));
	in.s_addr = htonl(hmp->hm_mapip.s_addr);
	printf("%s ", inet_ntoa(in));
	printf("(use = %d hv = %u)\n", hmp->hm_ref, hv);
}
