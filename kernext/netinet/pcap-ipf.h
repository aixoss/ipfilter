/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/kernext/netinet/pcap-ipf.h 1.1                      */
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
/* @(#)39  1.2  src/ipfl/kernext/netinet/pcap-ipf.h, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/10/06 12:55:41 */
/*
 * Copyright (C) 1993-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 */
/*
 * This header file is constructed to match the version described by
 * PCAP_VERSION_MAJ.
 *
 * The structure largely derives from libpcap which wouldn't include
 * nicely without bpf.
 */
typedef	struct	pcap_filehdr	{
	u_int	pc_id;
	u_short	pc_v_maj;
	u_short	pc_v_min;
	u_int	pc_zone;
	u_int	pc_sigfigs;
	u_int	pc_slen;
	u_int	pc_type;
} pcaphdr_t;

#define	TCPDUMP_MAGIC		0xa1b2c3d4

#define	PCAP_VERSION_MAJ	2

typedef	struct	pcap_pkthdr	{
	struct	timeval	ph_ts;
	u_int	ph_clen;
	u_int	ph_len;
} pcappkt_t;

