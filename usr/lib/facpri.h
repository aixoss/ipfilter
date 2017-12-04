/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/lib/facpri.h 1.1                                */
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
/* @(#)09  1.2  src/ipfl/usr/lib/facpri.h, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/10/06 12:55:06 */
/*
 * Copyright (C) 1999-2001 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 *
 * $Id: facpri.h,v 1.3 2001/06/09 17:19:50 darrenr Exp $
 */

#ifndef	__FACPRI_H__
#define	__FACPRI_H__

#ifndef	__P
# define P_DEF
# ifdef	__STDC__
#  define	__P(x) x
# else
#  define	__P(x) ()
# endif
#endif

extern	char	*fac_toname __P((int));
extern	int	fac_findname __P((char *));

extern	char	*pri_toname __P((int));
extern	int	pri_findname __P((char *));

#ifdef P_DEF
# undef	__P
# undef	P_DEF
#endif

#if LOG_CRON == (9<<3)
# define	LOG_CRON1	LOG_CRON
# define	LOG_CRON2	(15<<3)
#endif
#if LOG_CRON == (15<<3)
# define	LOG_CRON1	(9<<3)
# define	LOG_CRON2	LOG_CRON
#endif

#endif /* __FACPRI_H__ */
