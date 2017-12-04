/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/kernext/netinet/ip_auth.h 1.1                       */
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
/* @(#)19  1.2  src/ipfl/kernext/netinet/ip_auth.h, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/10/06 12:55:16 */
/*
 * Copyright (C) 1997-2001 by Darren Reed & Guido Van Rooij.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: ip_auth.h,v 2.16.2.2 2006/03/16 06:45:49 darrenr Exp $
 *
 */
#ifndef	__IP_AUTH_H__
#define	__IP_AUTH_H__

#define FR_NUMAUTH      32

typedef struct  frauth {
	int	fra_age;
	int	fra_len;
	int	fra_index;
	u_32_t	fra_pass;
	fr_info_t	fra_info;
	char	*fra_buf;
#ifdef	MENTAT
	queue_t	*fra_q;
	mb_t	*fra_m;
#endif
} frauth_t;

typedef	struct	frauthent  {
	struct	frentry	fae_fr;
	struct	frauthent	*fae_next;
	u_long	fae_age;
} frauthent_t;

typedef struct  fr_authstat {
	U_QUAD_T	fas_hits;
	U_QUAD_T	fas_miss;
	u_long		fas_nospace;
	u_long		fas_added;
	u_long		fas_sendfail;
	u_long		fas_sendok;
	u_long		fas_queok;
	u_long		fas_quefail;
	u_long		fas_expire;
	frauthent_t	*fas_faelist;
} fr_authstat_t;


extern	frentry_t	*ipauth;
extern	struct fr_authstat	fr_authstats;
extern	int	fr_defaultauthage;
extern	int	fr_authstart;
extern	int	fr_authend;
extern	int	fr_authsize;
extern	int	fr_authused;
extern	int	fr_auth_lock;
extern	frentry_t *fr_checkauth __P((fr_info_t *, u_32_t *));
extern	void	fr_authexpire __P((void));
extern	int	fr_authinit __P((void));
extern	void	fr_authunload __P((void));
extern	int	fr_authflush __P((void));
extern	mb_t	**fr_authpkts;
extern	int	fr_newauth __P((mb_t *, fr_info_t *));
extern	int	fr_preauthcmd __P((ioctlcmd_t, frentry_t *, frentry_t **));
extern	int	fr_auth_ioctl __P((caddr_t, ioctlcmd_t, int));
extern	int	fr_auth_waiting __P((void));

#endif	/* __IP_AUTH_H__ */
