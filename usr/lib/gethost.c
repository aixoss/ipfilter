/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/gethost.c 1.1                               */
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
static char sccsid[] = "@(#)13  1.1  src/ipfl/usr/lib/gethost.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:24:48";
#include "ipf.h"

int gethost(name, hostp)
char *name;
u_32_t *hostp;
{
	struct hostent *h;
	struct netent *n;
	u_32_t addr;

	if (!strcmp(name, "test.host.dots")) {
		*hostp = htonl(0xfedcba98);
		return 0;
	}

	if (!strcmp(name, "<thishost>"))
		name = thishost;

	h = gethostbyname(name);
	if (h != NULL) {
		if ((h->h_addr != NULL) && (h->h_length == sizeof(addr))) {
			bcopy(h->h_addr, (char *)&addr, sizeof(addr));
			*hostp = addr;
			return 0;
		}
	}

	n = getnetbyname(name);
	if (n != NULL) {
		*hostp = (u_32_t)htonl(n->n_net & 0xffffffff);
		return 0;
	}
	return -1;
}
