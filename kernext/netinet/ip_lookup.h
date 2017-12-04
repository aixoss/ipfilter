/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/kernext/netinet/ip_lookup.h 1.1                     */
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
/* @(#)24  1.2  src/ipfl/kernext/netinet/ip_lookup.h, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/10/06 12:55:23 */

#ifndef __IP_LOOKUP_H__
#define __IP_LOOKUP_H__

#if defined(__STDC__) || defined(__GNUC__) || defined(_AIX51)
# define	SIOCLOOKUPADDTABLE	_IOWR('r', 60, struct iplookupop)
# define	SIOCLOOKUPDELTABLE	_IOWR('r', 61, struct iplookupop)
# define	SIOCLOOKUPSTAT		_IOWR('r', 64, struct iplookupop)
# define	SIOCLOOKUPSTATW		_IOW('r', 64, struct iplookupop)
# define	SIOCLOOKUPFLUSH		_IOWR('r', 65, struct iplookupflush)
# define	SIOCLOOKUPADDNODE	_IOWR('r', 67, struct iplookupop)
# define	SIOCLOOKUPADDNODEW	_IOW('r', 67, struct iplookupop)
# define	SIOCLOOKUPDELNODE	_IOWR('r', 68, struct iplookupop)
# define	SIOCLOOKUPDELNODEW	_IOW('r', 68, struct iplookupop)
#else
# define	SIOCLOOKUPADDTABLE	_IOWR(r, 60, struct iplookupop)
# define	SIOCLOOKUPDELTABLE	_IOWR(r, 61, struct iplookupop)
# define	SIOCLOOKUPSTAT		_IOWR(r, 64, struct iplookupop)
# define	SIOCLOOKUPSTATW		_IOW(r, 64, struct iplookupop)
# define	SIOCLOOKUPFLUSH		_IOWR(r, 65, struct iplookupflush)
# define	SIOCLOOKUPADDNODE	_IOWR(r, 67, struct iplookupop)
# define	SIOCLOOKUPADDNODEW	_IOW(r, 67, struct iplookupop)
# define	SIOCLOOKUPDELNODE	_IOWR(r, 68, struct iplookupop)
# define	SIOCLOOKUPDELNODEW	_IOW(r, 68, struct iplookupop)
#endif

typedef	struct	iplookupop	{
	int	iplo_type;	/* IPLT_* */
	int	iplo_unit;	/* IPL_LOG* */
	u_int	iplo_arg;
	char	iplo_name[FR_GROUPLEN];
	size_t	iplo_size;	/* sizeof struct at iplo_struct */
	void	*iplo_struct;
} iplookupop_t;

typedef	struct	iplookupflush	{
	int	iplf_type;	/* IPLT_* */
	int	iplf_unit;	/* IPL_LOG* */
	u_int	iplf_arg;
	size_t	iplf_count;
	char	iplf_name[FR_GROUPLEN];
} iplookupflush_t;

typedef	struct	iplookuplink	{
	int	ipll_type;	/* IPLT_* */
	int	ipll_unit;	/* IPL_LOG* */
	u_int	ipll_num;
	char	ipll_group[FR_GROUPLEN];
} iplookuplink_t;

#define	IPLT_ALL	-1
#define	IPLT_NONE	0
#define	IPLT_POOL	1
#define	IPLT_HASH	2

#define	IPLT_ANON	0x80000000

extern int ip_lookup_init __P((void));
extern int ip_lookup_ioctl __P((caddr_t, ioctlcmd_t, int));
extern void ip_lookup_unload __P((void));
extern void ip_lookup_deref __P((int, void *));

#endif /* __IP_LOOKUP_H__ */
