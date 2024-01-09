/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/methods/cfg_ipf.c 1.3                       */
/*                                                                        */
/* Licensed Materials - Property of IBM                                   */
/*                                                                        */
/* Restricted Materials of IBM                                            */
/*                                                                        */
/* COPYRIGHT International Business Machines Corp. 2006,2016              */
/* All Rights Reserved                                                    */
/*                                                                        */
/* US Government Users Restricted Rights - Use, duplication or            */
/* disclosure restricted by GSA ADP Schedule Contract with IBM Corp.      */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
static char sccsid[] = "@(#)99  1.3  src/ipfl/usr/lib/methods/cfg_ipf.c, ipflt, 53ipfl53H, 1737A_53ipfl53H 11/2/16 02:33:08";
/*
 * Copyright (C) 2005 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: aix_cfg_ipf.c,v 1.1.2.1 2005/06/12 07:18:34 darrenr Exp $
 */
#include <sys/param.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/ldr.h>
/*
 * This is a workaround for <sys/uio.h> troubles on FreeBSD, HPUX, OpenBSD.
 * Needed here because on some systems <sys/uio.h> gets included by things
 * like <sys/socket.h>
 */
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <net/if.h>
#define _TCP_DEBUG_H_
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

#include <arpa/inet.h>

#include <errno.h>
#include <limits.h>
#include <netdb.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "netinet/ip_compat.h"
#include "netinet/ip_fil.h"

#ifndef __P
# ifdef __STDC__
#  define	__P(x)	x
# else
#  define	__P(x)	()
# endif
#endif
#ifndef __STDC__
# undef		const
# define	const
#endif

/*
 * AIX requires a specific configure/unconfigure program
 */
#undef ASSERT
#include <sys/device.h>
#include <sys/sysconfig.h>
#include <sys/systemcfg.h>

void loadipf __P((int major, int minor, dev_t devno, char *));
void unloadipf __P((int major, int minor, dev_t devno));
void queryipf __P((int major, int minor, dev_t devno));
int checkarg __P((int, char *arg));
void usage __P((char *));

static	char	*ipf_devfiles[] = { IPL_NAME, IPNAT_NAME, IPSTATE_NAME,
				    IPAUTH_NAME, IPSYNC_NAME, IPSCAN_NAME,
				    IPLOOKUP_NAME, NULL };

int
main(int argc, char *argv[])
{
	int major, minor, action;
	dev_t devno;

#ifndef __64BIT__
        if (__KERNEL_64())
        {
                struct stat stbuf;
                if (!stat("/usr/lib/methods/cfg_ipf64", &stbuf))
                {
                        exit(execv("/usr/lib/methods/cfg_ipf64", argv));
                }
                else
                {
                        fprintf(stderr,"%s : failed.  The kernel is running in 64-bit mode, but %s cannot be found\n",argv[0], "/usr/lib/methods/cfg_ipf64");
                        exit(1);
                }
        }
#else /* __64BIT__ */
        if (!__KERNEL_64())
        {
                fprintf(stderr, "%s : failed.  The kernel is running in 32-bit mode.  The 64-bit version of the command cannot be run on this kernel\n",argv[0]);
                exit(1);
        }
#endif /* __64BIT__ */


	action = checkarg(argc, argv[1]);
	if (action == -1)
		usage(argv[0]);

	if (odm_initialize() == -1) {
		fprintf(stderr, "odm_initialize() failed\n");
		exit(1);
	}

	major = genmajor("ipf");
	if (major == -1) {
		fprintf(stderr, "genmajor(ipf) failed\n");
		exit(1);
	}
	minor = 0;

#ifdef  __64BIT__
        devno = makedev64(major, minor);
#else
	devno = makedev(major, minor);
#endif
	if (devno == -1) {
		fprintf(stderr, "makedev(%d,%d) failed\n", major, minor);
		exit(1);
	}
	printf("Major %d\ndevno %x\n", major, devno);

	switch (action)
	{
	case 1 :
		loadipf(major, minor, devno, argv[2]);
		break;
	case 2 :
		unloadipf(major, minor, devno);
		break;
	case 3 :
		queryipf(major, minor, devno);
		break;
	}

	odm_terminate();

	return 0;
}


void usage(char *prog)
{
	fprintf(stderr, "Usage:\t%s -l\n\t%s -u\n\t%s -q\n",
		prog, prog, prog);
	exit(1);
}


int checkarg(int argc, char *arg)
{
	if (argc < 2)
		return -1;

	if (!strcmp(arg, "-l") && (argc <= 3))
		return 1;
	
	if (!strcmp(arg, "-u") && (argc == 2))
		return 2;

	if (!strcmp(arg, "-q") && (argc == 2))
		return 3;

	return -1;
}


void
loadipf(int major, int minor, dev_t devno, char *path)
{
	struct cfg_dd ipfcfg;
	struct cfg_load cfg;
	char *buffer[1024];
	char *ipfpath;
	int i;
	dev_t lastdev = devno;

	bzero(buffer, sizeof(buffer));
	if (path != NULL)
		ipfpath = path;
	else
		ipfpath = "/usr/lib/drivers/ipf";

#if 0
	bzero((char *)&cfg, sizeof(cfg));
	cfg.path = ipfpath;
	cfg.libpath = "/usr/lib/drivers/";
	sysconfig(SYS_SINGLELOAD, &cfg, sizeof(cfg));
	ipfcfg.kmid = cfg.kmid;
#else
	ipfcfg.kmid = (mid_t)loadext(ipfpath, TRUE, TRUE);
#endif
	if (ipfcfg.kmid == (mid_t)NULL)
	{
		perror("loadext");
		buffer[0] = "execerror";
		buffer[1] = "ipf";
		loadquery(1, &buffer[2], sizeof(buffer) - sizeof(*buffer)*2);
		execvp("/usr/sbin/execerror", buffer);
		exit(errno);
	}

	ipfcfg.devno = devno;
	ipfcfg.cmd = CFG_INIT;
	ipfcfg.ddsptr = (caddr_t)NULL;
	ipfcfg.ddslen = 0;

	if (sysconfig(SYS_CFGDD, &ipfcfg, sizeof(ipfcfg)) == -1) {
		perror("sysconifg(SYS_CFGDD)");
		exit(errno);
	}

	for (i = 0; ipf_devfiles[i] != NULL; i++) {
		unlink(ipf_devfiles[i]);
		if (mknod(ipf_devfiles[i], 0600 | _S_IFCHR, lastdev++) == -1) {
			perror("mknod(devfile)");
			exit(errno);
		}
	}
}


void
unloadipf(int major, int minor, dev_t devno)
{
	struct cfg_dd ipfcfg;
	struct cfg_load cfg;
	int i;

	bzero(&cfg, sizeof(cfg));
	cfg.path = "/usr/lib/drivers/ipf";
	cfg.kmid = 0;
	if (sysconfig(SYS_QUERYLOAD, &cfg, sizeof(cfg)) == -1) {
		perror("sysconfig(SYS_QUERYLOAD)");
		exit(errno);
	}

	ipfcfg.kmid = cfg.kmid;
	ipfcfg.devno = devno;
	ipfcfg.cmd = CFG_TERM;
	if (sysconfig(SYS_CFGDD, &ipfcfg, sizeof(ipfcfg)) == -1) {
		perror("sysconfig(SYS_CFGDD)");
		exit(errno);
	}

	for (i = 0; ipf_devfiles[i] != NULL; i++) {
		unlink(ipf_devfiles[i]);
	}

	if (loadext("ipf", FALSE, FALSE) == NULL) {
		perror("loadext");
		exit(errno);
	}
}


void
queryipf(int major, int minor, dev_t devno)
{
	struct cfg_dd ipfcfg;
	struct cfg_load cfg;
	int i;

	bzero(&cfg, sizeof(cfg));
	cfg.path = "/usr/lib/drivers/ipf";
	cfg.kmid = 0;
	if (sysconfig(SYS_QUERYLOAD, &cfg, sizeof(cfg)) == -1) {
		perror("sysconfig(SYS_QUERYLOAD)");
		exit(errno);
	}

	printf("Kernel module ID: %d\n", cfg.kmid);

	ipfcfg.kmid = cfg.kmid;
	ipfcfg.devno = devno;
	ipfcfg.cmd = CFG_QVPD;
	if (sysconfig(SYS_CFGDD, &ipfcfg, sizeof(ipfcfg)) == -1) {
		perror("sysconfig(SYS_CFGDD)");
		exit(errno);
	}
}

