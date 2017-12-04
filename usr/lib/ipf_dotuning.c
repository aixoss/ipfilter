/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/ipf_dotuning.c 1.1                          */
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
static char sccsid[] = "@(#)30  1.1  src/ipfl/usr/lib/ipf_dotuning.c, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:25:11";
#include "ipf.h"
#include "netinet/ipl.h"
#include <sys/ioctl.h>

void ipf_dotuning(fd, tuneargs, iocfn)
int fd;
char *tuneargs;
ioctlfunc_t iocfn;
{
	ipfobj_t obj;
	ipftune_t tu;
	char *s, *t;

	bzero((char *)&tu, sizeof(tu));
	obj.ipfo_rev = IPFILTER_VERSION;
	obj.ipfo_size = sizeof(tu);;
	obj.ipfo_ptr = (void *)&tu;
	obj.ipfo_type = IPFOBJ_TUNEABLE;

	for (s = strtok(tuneargs, ","); s != NULL; s = strtok(NULL, ",")) {
		if (!strcmp(s, "list")) {
			while (1) {
				if ((*iocfn)(fd, SIOCIPFGETNEXT, &obj) == -1) {
					perror("ioctl(SIOCIPFGETNEXT)");
					break;
				}
				if (tu.ipft_cookie == NULL)
					break;

				tu.ipft_name[sizeof(tu.ipft_name) - 1] = '\0';
				printtunable(&tu);
			}
		} else if ((t = strchr(s, '=')) != NULL) {
			tu.ipft_cookie = NULL;
			*t++ = '\0';
			strncpy(tu.ipft_name, s, sizeof(tu.ipft_name));
			if (sscanf(t, "%lu", &tu.ipft_vlong) == 1) {
				if ((*iocfn)(fd, SIOCIPFSET, &obj) == -1) {
					perror("ioctl(SIOCIPFSET)");
					return;
				}
			} else {
				fprintf(stderr, "invalid value '%s'\n", s);
				return;
			}
		} else {
			tu.ipft_cookie = NULL;
			strncpy(tu.ipft_name, s, sizeof(tu.ipft_name));
			if ((*iocfn)(fd, SIOCIPFGET, &obj) == -1) {
				perror("ioctl(SIOCIPFGET)");
				return;
			}
			if (tu.ipft_cookie == NULL) {
				fprintf(stderr, "Null cookie for %s\n", s);
				return;
			}

			tu.ipft_name[sizeof(tu.ipft_name) - 1] = '\0';
			printtunable(&tu);
		}
	}
}
