/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/getifname.c 1.1                             */
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
static char sccsid[] = "@(#)14  1.1  src/ipfl/usr/lib/getifname.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:24:49";
#include "ipf.h"

#include "kmem.h"

/*
 * Given a pointer to an interface in the kernel, return a pointer to a
 * string which is the interface name.
 */
char *getifname(ptr)
struct ifnet *ptr;
{
#if SOLARIS || defined(__hpux)
# if SOLARIS
#  include <sys/mutex.h>
#  include <sys/condvar.h>
# endif
# ifdef __hpux
#  include "compat.h"
# endif
# include "../pfil/qif.h"
	char *ifname;
	qif_t qif;

	if ((void *)ptr == (void *)-1)
		return "!";
	if (ptr == NULL)
		return "-";

	if (kmemcpy((char *)&qif, (u_long)ptr, sizeof(qif)) == -1)
		return "X";
	ifname = strdup(qif.qf_name);
	if ((ifname != NULL) && (*ifname == '\0')) {
		free(ifname);
		return "!";
	}
	return ifname;
#else
# if defined(NetBSD) && (NetBSD >= 199905) && (NetBSD < 1991011) || \
    defined(__OpenBSD__) || \
    (defined(__FreeBSD__) && (__FreeBSD_version >= 501113))
#else
	char buf[32];
	int len;
# endif
	struct ifnet netif;

	if ((void *)ptr == (void *)-1)
		return "!";
	if (ptr == NULL)
		return "-";

	if (kmemcpy((char *)&netif, (u_long)ptr, sizeof(netif)) == -1)
		return "X";
# if defined(NetBSD) && (NetBSD >= 199905) && (NetBSD < 1991011) || \
    defined(__OpenBSD__) || defined(linux) || \
    (defined(__FreeBSD__) && (__FreeBSD_version >= 501113))
	return strdup(netif.if_xname);
# else
	if (kstrncpy(buf, (u_long)netif.if_name, sizeof(buf)) == -1)
		return "X";
	if (netif.if_unit < 10)
		len = 2;
	else if (netif.if_unit < 1000)
		len = 3;
	else if (netif.if_unit < 10000)
		len = 4;
	else
		len = 5;
	buf[sizeof(buf) - len] = '\0';
	sprintf(buf + strlen(buf), "%d", netif.if_unit % 10000);
	return strdup(buf);
# endif
#endif
}
