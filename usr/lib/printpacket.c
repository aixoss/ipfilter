/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/printpacket.c 1.1                           */
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
static char sccsid[] = "@(#)74  1.1  src/ipfl/usr/lib/printpacket.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:26:01";
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: printpacket.c,v 1.12.4.2 2005/12/04 09:33:06 darrenr Exp $
 */

#include "ipf.h"

#ifndef	IP_OFFMASK
# define	IP_OFFMASK	0x3fff
#endif


void printpacket(ip)
struct ip *ip;
{
	struct	tcphdr	*tcp;
	u_short len;
	u_short off;

	if (IP_V(ip) == 6) {
		off = 0;
		len = ntohs(((u_short *)ip)[2]) + 40;
	} else {
		off = ntohs(ip->ip_off);
		len = ntohs(ip->ip_len);
	}

	if ((opts & OPT_HEX) == OPT_HEX) {
		u_char *s;
		int i;

		for (s = (u_char *)ip, i = 0; i < len; i++) {
			printf("%02x", *s++ & 0xff);
			if (len - i > 1) {
				i++;
				printf("%02x", *s++ & 0xff);
			}
			putchar(' ');
		}
		putchar('\n');
		return;
	}

	if (IP_V(ip) == 6) {
		printpacket6(ip);
		return;
	}

	tcp = (struct tcphdr *)((char *)ip + (IP_HL(ip) << 2));
	printf("ip #%d %d(%d) %d", ntohs(ip->ip_id), ntohs(ip->ip_len),
	       IP_HL(ip) << 2, ip->ip_p);
	if (off & IP_OFFMASK)
		printf(" @%d", off << 3);
	printf(" %s", inet_ntoa(ip->ip_src));
	if (!(off & IP_OFFMASK))
		if (ip->ip_p == IPPROTO_TCP || ip->ip_p == IPPROTO_UDP)
			printf(",%d", ntohs(tcp->th_sport));
	printf(" > ");
	printf("%s", inet_ntoa(ip->ip_dst));
	if (!(off & IP_OFFMASK)) {
		if (ip->ip_p == IPPROTO_TCP || ip->ip_p == IPPROTO_UDP)
			printf(",%d", ntohs(tcp->th_dport));
		if ((ip->ip_p == IPPROTO_TCP) && (tcp->th_flags != 0)) {
			putchar(' ');
			if (tcp->th_flags & TH_FIN)
				putchar('F');
			if (tcp->th_flags & TH_SYN)
				putchar('S');
			if (tcp->th_flags & TH_RST)
				putchar('R');
			if (tcp->th_flags & TH_PUSH)
				putchar('P');
			if (tcp->th_flags & TH_ACK)
				putchar('A');
			if (tcp->th_flags & TH_URG)
				putchar('U');
			if (tcp->th_flags & TH_ECN)
				putchar('E');
			if (tcp->th_flags & TH_CWR)
				putchar('C');
		}
	}

	putchar('\n');
}
