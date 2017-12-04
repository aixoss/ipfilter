/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/getport.c 1.1                               */
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
static char sccsid[] = "@(#)17  1.1  src/ipfl/usr/lib/getport.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:24:52";
#include "ipf.h"

int getport(fr, name, port)
frentry_t *fr;
char *name;
u_short *port;
{
	struct protoent *p;
	struct servent *s;
	u_short p1;

	if (fr == NULL || fr->fr_type != FR_T_IPF) {
		s = getservbyname(name, NULL);
		if (s != NULL) {
			*port = s->s_port;
			return 0;
		}
		return -1;
	}

	/*
	 * Some people will use port names in rules without specifying
	 * either TCP or UDP because it is implied by the group head.
	 * If we don't know the protocol, then the best we can do here is
	 * to take either only the TCP or UDP mapping (if one or the other
	 * is missing) or make sure both of them agree.
	 */
	if (fr->fr_proto == 0) {
		s = getservbyname(name, "tcp");
		if (s != NULL)
			p1 = s->s_port;
		else
			p1 = 0;
		s = getservbyname(name, "udp");
		if (s != NULL) {
			if (p1 != s->s_port)
				return -1;
		}
		if ((p1 == 0) && (s == NULL))
			return -1;
		if (p1)
			*port = p1;
		else
			*port = s->s_port;
		return 0;
	}

	if ((fr->fr_flx & FI_TCPUDP) != 0) {
		/*
		 * If a rule is "tcp/udp" then check that both TCP and UDP
		 * mappings for this protocol name match ports.
		 */
		s = getservbyname(name, "tcp");
		if (s == NULL)
			return -1;
		p1 = s->s_port;
		s = getservbyname(name, "udp");
		if (s == NULL || s->s_port != p1)
			return -1;
		*port = p1;
		return 0;
	}

	p = getprotobynumber(fr->fr_proto);
	s = getservbyname(name, p ? p->p_name : NULL);
	if (s != NULL) {
		*port = s->s_port;
		return 0;
	}
	return -1;
}
