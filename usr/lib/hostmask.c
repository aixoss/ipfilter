/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/hostmask.c 1.1                              */
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
static char sccsid[] = "@(#)23  1.1  src/ipfl/usr/lib/hostmask.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:24:58";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: hostmask.c,v 1.10 2002/01/28 06:50:46 darrenr Exp $
 */

#include "ipf.h"


/*
 * returns -1 if neither "hostmask/num" or "hostmask mask addr" are
 * found in the line segments, there is an error processing this information,
 * or there is an error processing ports information.
 */
int	hostmask(seg, proto, ifname, sa, msk, linenum)
char	***seg, *proto, *ifname;
u_32_t	*sa, *msk;
int	linenum;
{
	struct in_addr maskaddr;
	char *s;

	if ((s = strchr(**seg, '='))) {
		*s++ = '\0';
		if (!strcmp(**seg, "pool")) {
			*sa = atoi(s);
			return 1;
		}
	}

	/*
	 * is it possibly hostname/num ?
	 */
	if ((s = strchr(**seg, '/')) ||
	    ((s = strchr(**seg, ':')) && !strchr(s + 1, ':'))) {
		*s++ ='\0';
		if (genmask(s, msk) == -1) {
			fprintf(stderr, "%d: bad mask (%s)\n", linenum, s);
			return -1;
		}
		if (hostnum(sa, **seg, linenum, ifname) == -1) {
			fprintf(stderr, "%d: bad host (%s)\n", linenum, **seg);
			return -1;
		}
		*sa &= *msk;
		(*seg)++;
		return 0;
	}

	/*
	 * look for extra segments if "mask" found in right spot
	 */
	if (*(*seg+1) && *(*seg+2) && !strcasecmp(*(*seg+1), "mask")) {
		if (hostnum(sa, **seg, linenum, ifname) == -1) {
			fprintf(stderr, "%d: bad host (%s)\n", linenum, **seg);
			return -1;
		}
		(*seg)++;
		(*seg)++;
		if (inet_aton(**seg, &maskaddr) == 0) {
			fprintf(stderr, "%d: bad mask (%s)\n", linenum, **seg);
			return -1;
		}
		*msk = maskaddr.s_addr;
		(*seg)++;
		*sa &= *msk;
		return 0;
	}

	if (**seg) {
		u_32_t k;

		if (hostnum(sa, **seg, linenum, ifname) == -1) {
			fprintf(stderr, "%d: bad host (%s)\n", linenum, **seg);
			return -1;
		}
		(*seg)++;
		k = *sa ? 0xffffffff : 0;
#ifdef	USE_INET6
		if (use_inet6) {
			msk[1] = k;
			msk[2] = k;
			msk[3] = k;
		}
#endif
		*msk = k;
		return 0;
	}
	fprintf(stderr, "%d: bad host (%s)\n", linenum, **seg);
	return -1;
}
