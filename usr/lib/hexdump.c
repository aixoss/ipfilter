/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/hexdump.c 1.1                               */
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
static char sccsid[] = "@(#)22  1.1  src/ipfl/usr/lib/hexdump.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:24:57";
#include <ctype.h>

#include "ipf.h"

void hexdump(out, addr, len, ascii)
FILE *out;
void *addr;
int len, ascii;
{
	FILE *fpout;
	u_char *s, *t;
	int i;

	fpout = out ? out : stdout;
	for (i = 0, s = addr; i < len; i++, s++) {
		fprintf(fpout, "%02x", *s);
		if (i % 16 == 15) {
			if (ascii != 0) {
				fputc('\t', fpout);
				for (t = s - 15; t<= s; t++)
					fputc(ISPRINT(*t) ? *t : '.', fpout);
			}
			fputc('\n', fpout);
		} else if (i % 4 == 3) {
			fputc(' ', fpout);
		}
	}
}
