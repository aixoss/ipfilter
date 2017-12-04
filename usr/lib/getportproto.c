/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/getportproto.c 1.1                          */
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
static char sccsid[] = "@(#)18  1.1  src/ipfl/usr/lib/getportproto.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:24:53";
#include <ctype.h>
#include "ipf.h"

int getportproto(name, proto)
char *name;
int proto;
{
	struct servent *s;
	struct protoent *p;

	if (ISDIGIT(*name)) {
		int number;
		char *s;

		for (s = name; *s != '\0'; s++)
			if (!ISDIGIT(*s))
				return -1;

		number = atoi(name);
		if (number < 0 || number > 65535)
			return -1;
		return htons(number);
	}

	p = getprotobynumber(proto);
	s = getservbyname(name, p ? p->p_name : NULL);
	if (s != NULL)
		return s->s_port;
	return -1;
}
