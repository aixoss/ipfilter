/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/kernext/ip_fil_aix.c 1.7                            */
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
static char sccsid[] = "@(#)02  1.7  src/ipfl/kernext/ip_fil_aix.c, ipflt, 53ipfl53H, 1737A_53ipfl53H 11/4/16 03:27:55";
/*
 * Copyright (C) 1993-2003 by Darren Reed.
 *
 * See the IPFILTER.LICENCE file for details on licencing.
 */
#define	__FULL_PROTO
/*
#if !defined(lint)
static const char sccsid[] = "@(#)ip_fil.c	2.41 6/5/96 (C) 1993-2000 Darren Reed";
static const char rcsid[] = "@(#)$Id: ip_fil_aix.c,v 2.1.2.2 2006/03/25 13:03:00 darrenr Exp $";
#endif
*/

#if defined(KERNEL) || defined(_KERNEL)
# undef KERNEL
# undef _KERNEL
# define        KERNEL	1
# define        _KERNEL	1
#endif
#include <sys/param.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/systm.h>
#include <sys/dir.h>
#include <sys/mbuf.h>
#include <sys/protosw.h>
#include <sys/socket.h>
#include <sys/device.h>
#include <sys/timer.h>
#include <net/if.h>
#include <net/route.h>
#include <netinet/in.h>
#include <netinet/in_var.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_var.h>
#include <netinet/tcp.h>
#include <netinet/tcp_var.h>
#include <netinet/udp.h>
#include <netinet/tcpip.h>
#include <netinet/ip_icmp.h>
#include "netinet/ip_compat.h"
#ifdef USE_INET6
# include <netinet/icmp6.h>
#endif
#include "netinet/ip_fil.h"
#include "netinet/ip_nat.h"
#include "netinet/ip_frag.h"
#include "netinet/ip_state.h"
#include "netinet/ip_proxy.h"
#include "netinet/ip_auth.h"
#ifdef	IPFILTER_SYNC
#include "netinet/ip_sync.h"
#endif
#ifdef	IPFILTER_SCAN
#include "netinet/ip_scan.h"
#endif
#include "netinet/ip_pool.h"
#ifdef INET
extern	int	ip_optcopy __P((struct ip *, struct ip *));
#endif

/*
ipstat
inbound_fw
nethsque
rw_read_locked
ip_output
netisr_dispatch
ipforwarding
ip_output_post_fw
ipintr_noqueue_post_fw
max_linkhdr
in_localaddr
ip_optcopy
outbound_fw
*/


static	int	(*fr_savep) __P((ip_t *, int, void *, int, struct mbuf **));
static	int	fr_send_ip __P((fr_info_t *, mb_t *, mb_t **));
#ifdef KMUTEX_T
extern  ipfmutex_t	ipf_rw;
extern	ipfrwlock_t	ipf_mutex;
#endif
#ifdef USE_INET6
static int ipfr_fastroute6 __P((struct mbuf *, struct mbuf **,
				fr_info_t *, frdest_t *));
#endif

#include <sys/conf.h>
#include <sys/device.h>

#if 0
void fr_check_inbound __P((struct ifnet *, struct mbuf *, inbound_fw_args_t *));
int fr_check_outbound __P((struct ifnet *, struct mbuf *, outbound_fw_args_t *));
#endif
extern (*ip_fltr_in_hook)(struct mbuf **, void **) ;
extern (*ip_fltr_out_hook)(struct ifnet *, struct mbuf **, int, struct ifnet *);int ipf_fltr_in __P((struct mbuf **, void**));
int ipf_fltr_out __P((struct ifnet *, struct mbuf **, int, struct ifnet *));

int ipfopen __P((dev_t, u_long, chan_t, int));
int ipfclose __P((dev_t, chan_t));
int ipfread __P((dev_t, struct uio *, chan_t, int));
int ipfwrite __P((dev_t, struct uio *, chan_t, int));
int ipfioctl __P((dev_t, int, caddr_t, int));
int ipfconfig __P((dev_t, int, struct uio *));

ipfmutex_t	ipl_mutex, ipf_authmx, ipf_rw, ipf_stinsert;
ipfmutex_t	ipf_nat_new, ipf_natio, ipf_timeoutlock;
ipfrwlock_t	ipf_mutex, ipf_global, ipf_ipidfrag;
ipfrwlock_t	ipf_frag, ipf_state, ipf_nat, ipf_natfrag, ipf_auth;
ipfrwlock_t	ipf_frcache;
int		ipf_locks_done = 0;

const struct devsw ipfdevsw  = {
        ipfopen,	/* d_open	entry point for open routine */
        ipfclose,	/* d_close	entry point for close routine */
        ipfread,	/* d_read	entry point for read routine */
        ipfwrite,	/* d_write	entry point for write routine */
        ipfioctl,	/* d_ioctl	entry point for ioctl routine */
        nodev,		/* d_strategy	entry point for strategy routine */
        NULL, 		/* d_ttys	pointer to tty device structure */
        nodev,		/* d_select	entry point for select routine */
        ipfconfig,	/* d_config	entry point for config routine */
        nodev,		/* d_print	entry point for print routine */
        nodev,		/* d_dump	entry point for dump routine */
        nodev,		/* d_mpx	entry point for mpx routine */
        nodev,		/* d_revoke	entry point for revoke routine */
        NULL,		/* d_dsdptr     pointer to device specific data */
        0,		/* d_selptr	ptr to outstanding select cntl blks */
#ifdef __64BIT_KERNEL
	DEV_MPSAFE | DEV_64BIT
#else
        0		/* d_opts	internal device switch control field */
#endif

};


lock_t ipf_cfglock = LOCK_AVAIL;
int ipl_is_open[IPL_LOGMAX+1] = {0};

#if defined (AIX)
struct trb *trb;
#endif /*(AIX)*/

int ipfconfig(devno, cmd, uiop)
dev_t devno;
int cmd;
struct uio *uiop;
{
	int error = EINVAL;
	int i;

	printf("ipfconfig(%u,%x,%p)\n", devno,cmd,uiop);

	lockl(&ipf_cfglock, LOCK_SHORT);

	switch (cmd)
	{
	case CFG_INIT :
		if ((error = pincode(ipfconfig)) != 0 ) 
			break;
		
		if ((error = devswadd(devno, &ipfdevsw)) != 0 ){
			unpincode(ipfconfig);
			break;
		}

#if defined (AIX)
		/*
		 * CFG_INIT may be called multiple times. But we need to initialize the trb only once.
		 * Hence, check if trb is NULL here.
		 */
		if (trb == NULL) {
			trb = talloc();
		
			if (trb == NULL ) {
				(void) devswdel(devno);
				unpincode(ipfconfig);
				break;
			}
		}

		if ((error = ipfattach()) != 0 && error != EBUSY) {
			if (trb != NULL)
			{
				tfree(trb);
				trb = NULL;
			}

			(void) devswdel(devno);
			unpincode(ipfconfig);
			break;
		}
#endif /*(AIX)*/

		for (i=0; i <= IPL_LOGMAX; i++)
			ipl_is_open[i] = 0;
		break;
	case CFG_TERM :
		for  (i=0; i <= IPL_LOGMAX; i++) {
			if (ipl_is_open[i] > 0) {
				error = EBUSY;
				break;
			}
		}
		if (error != EBUSY) {
			error = ipldetach();

#if defined (AIX)
			/*
			 * CFG_TERM can be called multiple times. To avoid
			 * multiple tfree(), we need to check trb is not NULL here.
			 */
			if (trb != NULL)
			{
				tfree(trb);
				trb = NULL;
			}
#endif
			(void) devswdel(devno);
			unpincode(ipfconfig);
		}
		break;
	case CFG_QVPD :
		error = 0;
		break;
	default :
		error = EINVAL;
		break;
	}

	unlockl(&ipf_cfglock);
	return error;
}


int ipfattach()
{
	int s, rc;
#if defined (AIX)
	int ticks;
#endif /* AIX */
	SPL_NET(s);
#if 0
	if ((fr_running > 0) || (inbound_fw == fr_check_inbound)) {
#endif
	if ((fr_running > 0) || (ip_fltr_in_hook == ipf_fltr_in)) {
		printf("IP Filter: already initialized\n");
		SPL_X(s);
		return EBUSY;
	}

	bzero((char *)frcache, sizeof(frcache));
	MUTEX_INIT(&ipf_rw, "ipf rw mutex");
	MUTEX_INIT(&ipf_timeoutlock, "ipf timeout lock mutex");
	RWLOCK_INIT(&ipf_ipidfrag, "ipf IP NAT-Frag rwlock");
	ipf_locks_done = 1;

	if (fr_initialise() < 0) {
		SPL_X(s);
		return EIO;
	}

	bzero((char *)frcache, sizeof(frcache));
#if 0
	inbound_fw = fr_check_inbound;
	outbound_fw = fr_check_outbound;
#endif
	ip_fltr_in_hook = ipf_fltr_in;
	ip_fltr_out_hook = ipf_fltr_out;
	fr_running = 1;

	if (fr_control_forwarding & 1)
		ipforwarding = 1;

	SPL_X(s);
#if defined(AIX)
	READ_ENTER(&ipf_global);
	ticks = (hz / IPF_HZ_DIVIDE) * IPF_HZ_MULT;
	rc = ipfl_start_timer(ticks);
	RWLOCK_EXIT(&ipf_global);
	return rc;
#else
	rc = timeoutcf(2);
	timeout(fr_slowtimer, NULL, (hz / IPF_HZ_DIVIDE) * IPF_HZ_MULT);

	return 0;
#endif /*AIX*/
}

#if defined (AIX)
/*
 * ipfl_start_timer(): initializes trb and starts timer.
 * This function is used to solve the defect 997640
 */
int ipfl_start_timer(int tick)
{
	struct itimerstruc_t tv;

	if (trb==NULL)
		return ENOBUFS;

	tv.it_value.tv_sec  =  (tick) / HZ;
	tv.it_value.tv_nsec = (tick % HZ) * (NS_PER_SEC / HZ);

	trb->timeout = tv;
	trb->flags = 0;
	trb->func = fr_slowtimer;
	trb->ipri = INTTIMER;
	trb->func_data =  NULL;
	trb->id = -1;

	tstart(trb);

	return 0;

}
#endif /*AIX*/
/*
 * Disable the filter by removing the hooks from the IP input/output
 * stream.
 */
int ipldetach()
{
	int s,rc;

	if ((fr_running < 1) || (ip_fltr_in_hook == NULL ))
                return 0;

#if !defined (AIX)
	untimeout(fr_slowtimer, NULL);
	rc = timeoutcf(-2);
#endif /*AIX*/

#if defined (AIX)
	while (tstop(trb));
	trb->func = NULL;
#endif /* AIX*/
	SPL_NET(s);

#if 0
	inbound_fw = NULL;
	outbound_fw = NULL;
#endif
	ip_fltr_in_hook = NULL;
	ip_fltr_out_hook = NULL;
	(void) frflush(IPL_LOGIPF, 0, FR_INQUE|FR_OUTQUE|FR_INACTIVE);
	(void) frflush(IPL_LOGIPF, 0, FR_INQUE|FR_OUTQUE);

	if (fr_control_forwarding & 2)
		ipforwarding = 0;

	fr_deinitialise();
	fr_running = -1;

	SPL_X(s);
	return 0;
}


/*
 * explicit inbound hook to call fr_check from
 */
int ipf_fltr_in(m_pp, args)
struct mbuf **m_pp;
void **args;
{
        struct mbuf *m = *m_pp;
        struct ip *ip;
        struct ifnet *ifp;
        int rc;

        ip = mtod(m, struct ip *);
        ifp = m->m_pkthdr.rcvif;

        return(fr_check (ip, ip->ip_hl << 2, ifp, 0, m_pp));
}


/*
 * explicit outbound hook to call fr_check from
 */
int ipf_fltr_out(ifp, mp, flags, args)
struct ifnet *ifp;
struct mbuf **mp;
int flags;
struct ifnet *args;
{
        /* struct mbuf *m = *mp; */
        struct ip *ip;

        ip = mtod(*mp, struct ip *);

        return(fr_check (ip, ip->ip_hl << 2, ifp, 1, mp));
}

#if 0
/*
 * explicit inbound hook to call fr_check from
 */
void fr_check_inbound(ifp, m, args)
struct ifnet *ifp;
struct mbuf *m;
inbound_fw_args_t *args;
{
	ip_t *ip;

	if (fr_check_mbuf(&m) == -1) {
		if (m != NULL) {
			FREE_MB_T(m);
		}
		return;
	}

	ip = mtod(m, ip_t *);

	switch (fr_check (ip, ip->ip_hl << 2, ifp, 0, &m))
	{
	case 0 :
		ipintr_noqueue_post_fw(ifp, m, args);
		break;
	default :
		if (m != NULL) {
			FREE_MB_T(m);
		}
		break;
	}

	return;
}


/*
 * explicit outbound hook to call fr_check from
 */
int fr_check_outbound(ifp, m, args)
struct ifnet *ifp;
struct mbuf *m;
outbound_fw_args_t *args;
{
	ip_t *ip;

	if (fr_check_mbuf(&m) == -1) {
		if (m != NULL) {
			FREE_MB_T(m);
		}
		return 1;	/* FIREWALL_NOTOK */
	}

	ip = mtod(m, ip_t *);

	switch (fr_check (ip, ip->ip_hl << 2, ifp, 1, &m))
	{
	case 0 :
		ip_output_post_fw(ifp, m, args);
		return 0;	/* FIREWALL_OK */
	default :
		break;
	}

	if (m != NULL) {
		FREE_MB_T(m);
	}
	return 1;		/* FIREWALL_NOTOK */
}
#endif

int fr_check_mbuf(mp)
struct mbuf **mp;
{
	struct mbuf *m, *m0;
	int i, hlen;
	ip_t *ip;

	m = *mp;

	if ((m->m_len < sizeof (struct ip)) &&
	    (m = m_pullup(m, sizeof (struct ip))) == 0) {
		*mp = m;
		ipstat.ips_toosmall++;
		return -1;
	}
	ip = mtod(m, struct ip *);

	switch (ip->ip_v)
	{
	case 4 :
		hlen = ip->ip_hl << 2;
		if (hlen < sizeof(struct ip)) {	/* minimum header length */
			ipstat.ips_badhlen++;
			return -1;
		}
		if ((hlen > sizeof(struct ip)) && (hlen > m->m_len)) {
			if ((m = m_pullup(m, hlen)) == 0) {
				*mp = m;
				ipstat.ips_badhlen++;
				return -2;
			}
			ip = mtod(m, struct ip *);
		}
		if (ip->ip_sum = in_cksum(m, hlen)) {
			ipstat.ips_badsum++;
			return -1;
		}

		/*
		 * Convert fields to host representation.
		 * XXX - no need for NTOHS on big endian (sparc)
		 */
		NTOHS(ip->ip_len);
		if (ip->ip_len < hlen) {
			ipstat.ips_badlen++;
			return -1;
		}
		NTOHS(ip->ip_id);
		NTOHS(ip->ip_off);

		/*
		 * Check that the amount of data in the buffers
		 * is as at least much as the IP header would have us expect.
		 * Trim mbufs if longer than we expect.
		 * Drop packet if shorter than we expect.
		 */
		i = -(u_short)ip->ip_len;
		m0 = m;
		for (;;) {
			i += m->m_len;
			if (m->m_next == 0)
				break;
			m = m->m_next;
		}
		if (i != 0) {
			if (i < 0) {
				ipstat.ips_tooshort++;
				m = m0;
				return -1;
			}
			if (i <= m->m_len)
				m->m_len -= i;
			else
				m_adj(m0, -i);
		}
		break;
#ifdef USE_INET6
	case 6 :
		break;
#endif
	default :
		return -1;
	}
	return 0;
}


/*
 * Filter ioctl interface.
 */
int ipfioctl(dev, cmd, data, mode)
dev_t dev;
int cmd;
caddr_t data;
int mode;
{
	int s;
	int error = 0, unit = 0, tmp;
	friostat_t fio;

	unit = GET_MINOR(dev);
	if ((IPL_LOGMAX < unit) || (unit < 0))
		return ENXIO;

	if (fr_running <= 0) {
		if (unit != IPL_LOGIPF)
			return EIO;
		if (cmd != SIOCIPFGETNEXT && cmd != SIOCIPFGET &&
		    cmd != SIOCIPFSET && cmd != SIOCFRENB &&
		    cmd != SIOCGETFS && cmd != SIOCGETFF)
			return EIO;
	}

	SPL_NET(s);

	error = fr_ioctlswitch(unit, data, cmd, mode);
	if (error != -1) {
		SPL_X(s);
		return error;
	}
	error = 0;

	switch (cmd)
	{
	case FIONREAD :
#ifdef IPFILTER_LOG
		BCOPYOUT(&iplused[IPL_LOGIPF], (caddr_t)data,
			 sizeof(iplused[IPL_LOGIPF]));
#endif
		break;
	case SIOCFRENB :
		if (!(mode & FWRITE))
			error = EPERM;
		else {
			BCOPYIN(data, &tmp, sizeof(tmp));
			if (tmp) {
				if (fr_running > 0)
					error = 0;
				else
					error = ipfattach();
				if (error == 0)
					fr_running = 1;
				else
					(void) ipldetach();
			} else {
				error = ipldetach();
				if (error == 0)
					fr_running = -1;
			}
		}
		break;
	case SIOCIPFSET :
		if (!(mode & FWRITE)) {
			error = EPERM;
			break;
		}
	case SIOCIPFGETNEXT :
	case SIOCIPFGET :
		error = fr_ipftune(cmd, data);
		break;
	case SIOCSETFF :
		if (!(mode & FWRITE))
			error = EPERM;
		else
			BCOPYIN(data, &fr_flags, sizeof(fr_flags));
		break;
	case SIOCGETFF :
		BCOPYOUT(&fr_flags, data, sizeof(fr_flags));
		break;
	case SIOCFUNCL :
		error = fr_resolvefunc(data);
		break;
	case SIOCINAFR :
	case SIOCRMAFR :
	case SIOCADAFR :
	case SIOCZRLST :
		if (!(mode & FWRITE))
			error = EPERM;
		else
			error = frrequest(unit, cmd, data, fr_active, 1);
		break;
	case SIOCINIFR :
	case SIOCRMIFR :
	case SIOCADIFR :
		if (!(mode & FWRITE))
			error = EPERM;
		else
			error = frrequest(unit, cmd, data, 1 - fr_active, 1);
		break;
	case SIOCSWAPA :
		if (!(mode & FWRITE))
			error = EPERM;
		else {
			bzero((char *)frcache, sizeof(frcache[0]) * 2);
			*(u_int *)data = fr_active;
			fr_active = 1 - fr_active;
		}
		break;
	case SIOCGETFS :
		fr_getstat(&fio);
		error = fr_outobj(data, &fio, IPFOBJ_IPFSTAT);
		break;
	case SIOCFRZST :
		if (!(mode & FWRITE))
			error = EPERM;
		else
			error = fr_zerostats(data);
		break;
	case SIOCIPFFL :
		if (!(mode & FWRITE))
			error = EPERM;
		else {
			BCOPYIN(data, &tmp, sizeof(tmp));
			tmp = frflush(unit, 4, tmp);
			BCOPYOUT(&tmp, data, sizeof(tmp));
		}
		break;
#ifdef USE_INET6
	case SIOCIPFL6 :
		if (!(mode & FWRITE))
			error = EPERM;
		else {
			BCOPYIN(data, &tmp, sizeof(tmp));
			tmp = frflush(unit, 6, tmp);
			BCOPYOUT(&tmp, data, sizeof(tmp));
		}
		break;
#endif
	case SIOCSTLCK :
		BCOPYIN(data, &tmp, sizeof(tmp));
		fr_state_lock = tmp;
		fr_nat_lock = tmp;
		fr_frag_lock = tmp;
		fr_auth_lock = tmp;
		break;
#ifdef IPFILTER_LOG
	case SIOCIPFFB :
		if (!(mode & FWRITE))
			error = EPERM;
		else
			*(int *)data = ipflog_clear(unit);
		break;
#endif /* IPFILTER_LOG */
	case SIOCGFRST :
		error = fr_outobj(data, fr_fragstats(), IPFOBJ_FRAGSTAT);
		break;
	case SIOCFRSYN :
		if (!(mode & FWRITE))
			error = EPERM;
		else {
			frsync(NULL);
		}
		break;
	default :
		error = EINVAL;
		break;
	}
	SPL_X(s);
	return error;
}


#if 0
void fr_forgetifp(ifp)
void *ifp;
{
	register frentry_t *f;

	WRITE_ENTER(&ipf_mutex);
	for (f = ipacct[0][fr_active]; (f != NULL); f = f->fr_next)
		if (f->fr_ifa == ifp)
			f->fr_ifa = (void *)-1;
	for (f = ipacct[1][fr_active]; (f != NULL); f = f->fr_next)
		if (f->fr_ifa == ifp)
			f->fr_ifa = (void *)-1;
	for (f = ipfilter[0][fr_active]; (f != NULL); f = f->fr_next)
		if (f->fr_ifa == ifp)
			f->fr_ifa = (void *)-1;
	for (f = ipfilter[1][fr_active]; (f != NULL); f = f->fr_next)
		if (f->fr_ifa == ifp)
			f->fr_ifa = (void *)-1;
#ifdef USE_INET6
	for (f = ipacct6[0][fr_active]; (f != NULL); f = f->fr_next)
		if (f->fr_ifa == ifp)
			f->fr_ifa = (void *)-1;
	for (f = ipacct6[1][fr_active]; (f != NULL); f = f->fr_next)
		if (f->fr_ifa == ifp)
			f->fr_ifa = (void *)-1;
	for (f = ipfilter6[0][fr_active]; (f != NULL); f = f->fr_next)
		if (f->fr_ifa == ifp)
			f->fr_ifa = (void *)-1;
	for (f = ipfilter6[1][fr_active]; (f != NULL); f = f->fr_next)
		if (f->fr_ifa == ifp)
			f->fr_ifa = (void *)-1;
#endif
	RWLOCK_EXIT(&ipf_mutex);
	fr_natsync(ifp);
}
#endif


/*
 * routines below for saving IP headers to buffer
 */
int ipfopen(dev_t dev, u_long flags, chan_t chan, int ext)
{
	u_int min = GET_MINOR(dev);

	lockl(&ipf_cfglock, LOCK_SHORT);

	if (IPL_LOGMAX < min){
		min = ENXIO;
	} else {
		ipl_is_open[min] = 1;
		min = 0;
	}

	unlockl(&ipf_cfglock);

	return min;
}


int ipfclose(dev_t dev, chan_t chan)
{
	u_int	min = GET_MINOR(dev);

	lockl(&ipf_cfglock, LOCK_SHORT);

	if (IPL_LOGMAX < min){
		min = ENXIO;
	} else {
		ipl_is_open[min] = 0;
		min = 0;
	}

	unlockl(&ipf_cfglock);
	return min;
}

/*
 * ipfread/ipllog
 * both of these must operate with at least splnet() lest they be
 * called during packet processing and cause an inconsistancy to appear in
 * the filter lists.
 */
int ipfread(dev_t dev, struct uio *uio, chan_t chan, int ext)
{

# ifdef	IPFILTER_SYNC
	if (GET_MINOR(dev) == IPL_LOGSYNC)
		return ipfsync_read(uio);
# endif

#ifdef IPFILTER_LOG
	return ipflog_read(GET_MINOR(dev), uio);
#else
	return ENXIO;
#endif
}


/*
 * ipfwrite
 * both of these must operate with at least splnet() lest they be
 * called during packet processing and cause an inconsistancy to appear in
 * the filter lists.
 */
int ipfwrite(dev_t dev, struct uio *uio, chan_t chan, int ext)
{

#ifdef	IPFILTER_SYNC
	if (GET_MINOR(dev) == IPL_LOGSYNC)
		return ipfsync_write(uio);
#endif
	return ENXIO;
}


/*
 * fr_send_reset - this could conceivably be a call to tcp_respond(), but that
 * requires a large amount of setting up and isn't any more efficient.
 */
int fr_send_reset(fin)
fr_info_t *fin;
{
	struct tcphdr *tcp, *tcp2;
	int tlen = 0, hlen;
	struct mbuf *m;
#ifdef USE_INET6
	ip6_t *ip6;
#endif
	ip_t *ip;

	tcp = fin->fin_dp;
	if (tcp->th_flags & TH_RST)
		return -1;		/* feedback loop */

#ifndef	IPFILTER_CKSUM
	if (fr_checkl4sum(fin) == -1)
		return -1;
#endif

	tlen = fin->fin_dlen - (TCP_OFF(tcp) << 2) +
			((tcp->th_flags & TH_SYN) ? 1 : 0) +
			((tcp->th_flags & TH_FIN) ? 1 : 0);

#ifdef USE_INET6
	hlen = (fin->fin_v == 6) ? sizeof(ip6_t) : sizeof(ip_t);
#else
	hlen = sizeof(ip_t);
#endif
	m = m_gethdr(M_DONTWAIT, MT_HEADER);
	if (m == NULL)
		return -1;
	if (sizeof(*tcp2) + hlen > MHLEN) {
		MCLGET(m, M_DONTWAIT);
		if (m == NULL)
			return -1;
		if ((m->m_flags & M_EXT) == 0) {
			FREE_MB_T(m);
			return -1;
		}
	}

	m->m_len = sizeof(*tcp2) + hlen;
	m->m_data += max_linkhdr;
	m->m_pkthdr.len = m->m_len;
	m->m_pkthdr.rcvif = (struct ifnet *)0;
	ip = mtod(m, struct ip *);
	bzero((char *)ip, hlen);
#ifdef USE_INET6
	ip6 = (ip6_t *)ip;
#endif
	bzero((char *)ip, sizeof(*tcp2) + hlen);
	tcp2 = (struct tcphdr *)((char *)ip + hlen);
	tcp2->th_sport = tcp->th_dport;
	tcp2->th_dport = tcp->th_sport;

	if (tcp->th_flags & TH_ACK) {
		tcp2->th_seq = tcp->th_ack;
		tcp2->th_flags = TH_RST;
		tcp2->th_ack = 0;
	} else {
		tcp2->th_seq = 0;
		tcp2->th_ack = ntohl(tcp->th_seq);
		tcp2->th_ack += tlen;
		tcp2->th_ack = htonl(tcp2->th_ack);
		tcp2->th_flags = TH_RST|TH_ACK;
	}
	tcp2->th_x2 = 0;
	TCP_OFF_A(tcp2, sizeof(*tcp2) >> 2);
	tcp2->th_win = tcp->th_win;
	tcp2->th_sum = 0;
	tcp2->th_urp = 0;

#ifdef USE_INET6
	if (fin->fin_v == 6) {
		ip6->ip6_flow = ((ip6_t *)fin->fin_ip)->ip6_flow;
		ip6->ip6_plen = htons(sizeof(struct tcphdr));
		ip6->ip6_nxt = IPPROTO_TCP;
		ip6->ip6_hlim = 0;
		ip6->ip6_src = fin->fin_dst6;
		ip6->ip6_dst = fin->fin_src6;
		tcp2->th_sum = in6_cksum(m, IPPROTO_TCP,
					 sizeof(*ip6), sizeof(*tcp2));
		return fr_send_ip(fin, m, &m);
	}
#endif
#ifdef INET
	ip->ip_p = IPPROTO_TCP;
	ip->ip_len = htons(sizeof(struct tcphdr));
	ip->ip_src.s_addr = fin->fin_daddr;
	ip->ip_dst.s_addr = fin->fin_saddr;
	tcp2->th_sum = in_cksum(m, hlen + sizeof(*tcp2));
	ip->ip_len = hlen + sizeof(*tcp2);
	return fr_send_ip(fin, m, &m);
#else
	return 0;
#endif
}


static int fr_send_ip(fin, m, mpp)
fr_info_t *fin;
mb_t *m, **mpp;
{
	fr_info_t fnew;
#ifdef INET
	ip_t *oip;
#endif
	ip_t *ip;
	int hlen;

	ip = mtod(m, ip_t *);
	bzero((char *)&fnew, sizeof(fnew));

	IP_V_A(ip, fin->fin_v);
	switch (fin->fin_v)
	{
#ifdef INET
	case 4 :
		fnew.fin_v = 4;
		oip = fin->fin_ip;
		IP_HL_A(ip, sizeof(*oip) >> 2);
		ip->ip_tos = oip->ip_tos;
		ip->ip_id = fr_nextipid(fin);
		ip->ip_off = 0;
		ip->ip_ttl = IPDEFTTL;
		ip->ip_sum = 0;
		hlen = sizeof(*oip);
		break;
#endif
#ifdef USE_INET6
	case 6 :
	{
		ip6_t *ip6 = (ip6_t *)ip;

		ip6->ip6_vfc = 0x60;
		ip6->ip6_hlim = IPDEFTTL;

		fnew.fin_v = 6;
		hlen = sizeof(*ip6);
		break;
	}
#endif
	default :
		return EINVAL;
	}
#ifdef IPSEC
	m->m_pkthdr.rcvif = NULL;
#endif

	fnew.fin_ifp = fin->fin_ifp;
	fnew.fin_flx = FI_NOCKSUM;
	fnew.fin_m = m;
	fnew.fin_ip = ip;
	fnew.fin_mp = mpp;
	fnew.fin_hlen = hlen;
	fnew.fin_dp = (char *)ip + hlen;
	(void) fr_makefrip(hlen, ip, &fnew);

	return fr_fastroute(m, mpp, &fnew, NULL);
}


int fr_send_icmp_err(type, fin, dst)
int type;
fr_info_t *fin;
int dst;
{
	int err, hlen, xtra, iclen, ohlen, avail, code;
	struct in_addr dst4;
	struct icmp *icmp;
	struct mbuf *m;
	void *ifp;
#ifdef USE_INET6
	ip6_t *ip6;
	struct in6_addr dst6;
#endif
	ip_t *ip, *ip2;

	if ((type < 0) || (type > ICMP_MAXTYPE))
		return -1;

	code = fin->fin_icode;
#ifdef USE_INET6
	if ((code < 0) || (code > sizeof(icmptoicmp6unreach)/sizeof(int)))
		return -1;
#endif

#ifndef	IPFILTER_CKSUM
	if (fr_checkl4sum(fin) == -1)
		return -1;
#endif
	m = m_gethdr(M_DONTWAIT, MT_HEADER);
	if (m == NULL)
		return -1;
	avail = MHLEN;

	xtra = 0;
	hlen = 0;
	ohlen = 0;
	ifp = fin->fin_ifp;
	if (fin->fin_v == 4) {
		if ((fin->fin_p == IPPROTO_ICMP) &&
		    !(fin->fin_flx & FI_SHORT))
			switch (ntohs(fin->fin_data[0]) >> 8)
			{
			case ICMP_ECHO :
			case ICMP_TSTAMP :
			case ICMP_IREQ :
			case ICMP_MASKREQ :
				break;
			default :
				FREE_MB_T(m);
				return 0;
			}

		if (dst == 0) {
			if (fr_ifpaddr(4, FRI_NORMAL, ifp,
				       &dst4, NULL) == -1) {
				FREE_MB_T(m);
				return -1;
			}
		} else
			dst4.s_addr = fin->fin_daddr;

		hlen = sizeof(ip_t);
		ohlen = fin->fin_hlen;
		if (fin->fin_hlen < fin->fin_plen)
			xtra = MIN(fin->fin_dlen, 8);
		else
			xtra = 0;
	}

#ifdef USE_INET6
	else if (fin->fin_v == 6) {
		hlen = sizeof(ip6_t);
		ohlen = sizeof(ip6_t);
		type = icmptoicmp6types[type];
		if (type == ICMP6_DST_UNREACH)
			code = icmptoicmp6unreach[code];

		if (hlen + sizeof(*icmp) + max_linkhdr +
		    fin->fin_plen > avail) {
			MCLGET(m, M_DONTWAIT);
			if (m == NULL)
				return -1;
			if ((m->m_flags & M_EXT) == 0) {
				FREE_MB_T(m);
				return -1;
			}
			avail = MCLBYTES;
		}
		xtra = MIN(fin->fin_plen,
			   avail - hlen - sizeof(*icmp) - max_linkhdr);
		if (dst == 0) {
			if (fr_ifpaddr(6, FRI_NORMAL, ifp,
				       (struct in_addr *)&dst6, NULL) == -1) {
				FREE_MB_T(m);
				return -1;
			}
		} else
			dst6 = fin->fin_dst6;
	}
#endif
	else {
		FREE_MB_T(m);
		return -1;
	}

	iclen = hlen + sizeof(*icmp);
	avail -= (max_linkhdr + iclen);
	if (avail < 0) {
		FREE_MB_T(m);
		return -1;
	}
	if (xtra > avail)
		xtra = avail;
	iclen += xtra;
	m->m_data += max_linkhdr;
	m->m_pkthdr.rcvif = (struct ifnet *)0;
	m->m_pkthdr.len = iclen;
	m->m_len = iclen;
	ip = mtod(m, ip_t *);
	icmp = (struct icmp *)((char *)ip + hlen);
	ip2 = (ip_t *)&icmp->icmp_ip;

	icmp->icmp_type = type;
	icmp->icmp_code = fin->fin_icode;
	icmp->icmp_cksum = 0;
#ifdef icmp_nextmtu
	if (type == ICMP_UNREACH &&
	    fin->fin_icode == ICMP_UNREACH_NEEDFRAG && ifp)
		icmp->icmp_nextmtu = htons(((struct ifnet *)ifp)->if_mtu);
#endif

	bcopy((char *)fin->fin_ip, (char *)ip2, ohlen);

#if defined(M_CSUM_IPv4)
	/*
	 * Clear any in-bound checksum flags for this packet.
	 */
	m->m_pkthdr.csuminfo = 0;
#endif /* __NetBSD__ && M_CSUM_IPv4 */

#ifdef USE_INET6
	ip6 = (ip6_t *)ip;
	if (fin->fin_v == 6) {
		ip6->ip6_flow = ((ip6_t *)fin->fin_ip)->ip6_flow;
		ip6->ip6_plen = htons(iclen - hlen);
		ip6->ip6_nxt = IPPROTO_ICMPV6;
		ip6->ip6_hlim = 0;
		ip6->ip6_src = dst6;
		ip6->ip6_dst = fin->fin_src6;
		if (xtra > 0)
			bcopy((char *)fin->fin_ip + ohlen,
			      (char *)&icmp->icmp_ip + ohlen, xtra);
		icmp->icmp_cksum = in6_cksum(m, IPPROTO_ICMPV6,
					     sizeof(*ip6), iclen - hlen);
	} else
#endif
	{
		ip2->ip_len = htons(ip2->ip_len);
		ip2->ip_off = htons(ip2->ip_off);
		ip->ip_p = IPPROTO_ICMP;
		ip->ip_src.s_addr = dst4.s_addr;
		ip->ip_dst.s_addr = fin->fin_saddr;

		if (xtra > 0)
			bcopy((char *)fin->fin_ip + ohlen,
			      (char *)&icmp->icmp_ip + ohlen, xtra);
		icmp->icmp_cksum = ipf_cksum((u_short *)icmp,
					     sizeof(*icmp) + 8);
		ip->ip_len = iclen;
		ip->ip_p = IPPROTO_ICMP;
	}
	err = fr_send_ip(fin, m, &m);
	return err;
}


int fr_fastroute(m0, mpp, fin, fdp)
mb_t *m0, **mpp;
fr_info_t *fin;
frdest_t *fdp;
{
	register struct ip *ip, *mhip;
	register struct mbuf *m = m0;
	register struct route *ro;
	int len, off, error = 0, hlen, code;
	struct ifnet *ifp, *sifp;
	struct sockaddr_in *dst;
	struct route iproute;
	u_short ip_off;
	frentry_t *fr;

	if (fin->fin_v == 6) {
#ifdef USE_INET6
		error = ipfr_fastroute6(m0, mpp, fin, fdp);
#else
		error = EPROTONOSUPPORT;
#endif
		if ((error != 0) && (*mpp != NULL)) {
			FREE_MB_T(*mpp);
			*mpp = NULL;
		}
		return error;
	}
#ifndef INET
	return EPROTONOSUPPORT;
#else

	hlen = fin->fin_hlen;
	ip = mtod(m0, struct ip *);

# if defined(M_CSUM_IPv4)
	/*
	 * Clear any in-bound checksum flags for this packet.
	 */
	m0->m_pkthdr.csuminfo = 0;
# endif /* __NetBSD__ && M_CSUM_IPv4 */

	/*
	 * Route packet.
	 */
	ro = &iproute;
	bzero((caddr_t)ro, sizeof (*ro));
	dst = (struct sockaddr_in *)&ro->ro_dst;
	dst->sin_family = AF_INET;
	dst->sin_addr = ip->ip_dst;

	fr = fin->fin_fr;
	if (fdp != NULL)
		ifp = fdp->fd_ifp;
	else
		ifp = fin->fin_ifp;

	if ((ifp == NULL) && (!fr || !(fr->fr_flags & FR_FASTROUTE))) {
		error = -2;
		goto bad;
	}

	if ((fdp != NULL) && (fdp->fd_ip.s_addr != 0))
		dst->sin_addr = fdp->fd_ip;

	dst->sin_len = sizeof(*dst);
	rtalloc(ro);

	if ((ifp == NULL) && (ro->ro_rt != NULL))
		ifp = ro->ro_rt->rt_ifp;

	if ((ro->ro_rt == NULL) || (ifp == NULL)) {
		if (in_localaddr(ip->ip_dst))
			error = EHOSTUNREACH;
		else
			error = ENETUNREACH;
		goto bad;
	}
	if (ro->ro_rt->rt_flags & RTF_GATEWAY)
		dst = (struct sockaddr_in *)ro->ro_rt->rt_gateway;
	if (ro->ro_rt)
		ro->ro_rt->rt_use++;

	/*
	 * For input packets which are being "fastrouted", they won't
	 * go back through output filtering and miss their chance to get
	 * NAT'd and counted.
	 */
	if (fin->fin_out == 0) {
		sifp = fin->fin_ifp;
		fin->fin_ifp = ifp;
		fin->fin_out = 1;
		(void) fr_acctpkt(fin, NULL);
		fin->fin_fr = NULL;
		if (!fr || !(fr->fr_flags & FR_RETMASK)) {
			u_32_t pass;

			(void) fr_checkstate(fin, &pass);
		}

		switch (fr_checknatout(fin, NULL))
		{
		case 0 :
			break;
		case 1 :
			ip->ip_sum = 0;
			break;
		case -1 :
			error = -1;
			goto done;
			break;
		}

		fin->fin_ifp = sifp;
		fin->fin_out = 0;
	} else
		ip->ip_sum = 0;
	/*
	 * If small enough for interface, can just send directly.
	 */
	if (ip->ip_len <= ifp->if_mtu) {
		int i = 0;

		if (m->m_flags & M_EXT)
			i = 1;

		ip->ip_len = htons(ip->ip_len);
		ip->ip_off = htons(ip->ip_off);
# if defined(M_CSUM_IPv4)
#  if (__NetBSD_Version__ >= 105009999)
		if (ifp->if_csum_flags_tx & M_CSUM_IPv4)
			m->m_pkthdr.csuminfo |= M_CSUM_IPv4;
#  else
		if (ifp->if_capabilities & IFCAP_CSUM_IPv4)
			m->m_pkthdr.csuminfo |= M_CSUM_IPv4;
#  endif /* (__NetBSD_Version__ >= 105009999) */
		else if (ip->ip_sum == 0)
			ip->ip_sum = in_cksum(m, hlen);
# else
		if (!ip->ip_sum)
			ip->ip_sum = in_cksum(m, hlen);
# endif /* M_CSUM_IPv4 */
		error = (*ifp->if_output)(ifp, m, (struct sockaddr *)dst,
					  ro->ro_rt);
		if (i) {
			ip->ip_len = ntohs(ip->ip_len);
			ip->ip_off = ntohs(ip->ip_off);
		}
		goto done;
	}

	/*
	 * Too large for interface; fragment if possible.
	 * Must be able to put at least 8 bytes per fragment.
	 */
	ip_off = ip->ip_off;
	if (ip_off & IP_DF) {
		error = EMSGSIZE;
		goto bad;
	}
	len = (ifp->if_mtu - hlen) &~ 7;
	if (len < 8) {
		error = EMSGSIZE;
		goto bad;
	}

    {
	int mhlen, firstlen = len;
	struct mbuf **mnext = &m->m_act;

	/*
	 * Loop through length of segment after first fragment,
	 * make new header and copy data of each part and link onto chain.
	 */
	m0 = m;
	mhlen = sizeof (struct ip);
	for (off = hlen + len; off < ip->ip_len; off += len) {
		m = m_gethdr(M_DONTWAIT, MT_HEADER);
		if (m == 0) {
			m = m0;
			error = ENOBUFS;
			goto bad;
		}
		m->m_data += max_linkhdr;
		mhip = mtod(m, struct ip *);
		bcopy((char *)ip, (char *)mhip, sizeof(*ip));
		if (hlen > sizeof (struct ip)) {
			mhlen = ip_optcopy(ip, mhip) + sizeof (struct ip);
			IP_HL_A(mhip, mhlen >> 2);
		}
		m->m_len = mhlen;
		mhip->ip_off = ((off - hlen) >> 3) + ip_off;
		if (off + len >= ip->ip_len)
			len = ip->ip_len - off;
		else
			mhip->ip_off |= IP_MF;
		mhip->ip_len = htons((u_short)(len + mhlen));
		m->m_next = m_copy(m0, off, len);
		if (m->m_next == 0) {
			error = ENOBUFS;	/* ??? */
			goto sendorfree;
		}
		m->m_pkthdr.len = mhlen + len;
		m->m_pkthdr.rcvif = NULL;
		mhip->ip_off = htons((u_short)mhip->ip_off);
		mhip->ip_sum = 0;
		mhip->ip_sum = in_cksum(m, mhlen);
		*mnext = m;
		mnext = &m->m_act;
	}
	/*
	 * Update first fragment by trimming what's been copied out
	 * and updating header, then send each fragment (in order).
	 */
	m_adj(m0, hlen + firstlen - ip->ip_len);
	ip->ip_len = htons((u_short)(hlen + firstlen));
	ip->ip_off = htons((u_short)IP_MF);
	ip->ip_sum = 0;
	ip->ip_sum = in_cksum(m0, hlen);
sendorfree:
	for (m = m0; m; m = m0) {
		m0 = m->m_act;
		m->m_act = 0;
		if (error == 0)
			error = (*ifp->if_output)(ifp, m,
			    (struct sockaddr *)dst, ro->ro_rt);
		else
			FREE_MB_T(m);
	}
    }
done:
	if (!error)
		fr_frouteok[0]++;
	else
		fr_frouteok[1]++;

	if (ro->ro_rt) {
		RTFREE(ro->ro_rt);
	}
	*mpp = NULL;
	return error;
bad:
	if (error == EMSGSIZE) {
		sifp = fin->fin_ifp;
		code = fin->fin_icode;
		fin->fin_icode = ICMP_UNREACH_NEEDFRAG;
		fin->fin_ifp = ifp;
		(void) fr_send_icmp_err(ICMP_UNREACH, fin, 1);
		fin->fin_ifp = sifp;
		fin->fin_icode = code;
	}
	FREE_MB_T(m);
	goto done;
#endif /* INET */
}


#if defined(USE_INET6)
/*
 * This is the IPv6 specific fastroute code.  It doesn't clean up the mbuf's
 * or ensure that it is an IPv6 packet that is being forwarded, those are
 * expected to be done by the called (ipfr_fastroute).
 */
static int ipfr_fastroute6(m0, mpp, fin, fdp)
struct mbuf *m0, **mpp;
fr_info_t *fin;
frdest_t *fdp;
{
	struct route_in6 ip6route;
	struct sockaddr_in6 *dst6;
	struct route_in6 *ro;
	struct rtentry *rt;
	struct ifnet *ifp;
	frentry_t *fr;
	u_long mtu;
	int error;

	ro = &ip6route;
	fr = fin->fin_fr;
	bzero((caddr_t)ro, sizeof(*ro));
	dst6 = (struct sockaddr_in6 *)&ro->ro_dst;
	dst6->sin6_family = AF_INET6;
	dst6->sin6_len = sizeof(struct sockaddr_in6);
	dst6->sin6_addr = fin->fin_fi.fi_dst.in6;

	if (fdp != NULL)
		ifp = fdp->fd_ifp;
	else
		ifp = fin->fin_ifp;

	if ((fr != NULL) && (fin->fin_rev != 0)) {
		if ((ifp != NULL) && (fdp == &fr->fr_tif))
			return 0;
	}
	if (fdp != NULL) {
		if (IP6_NOTZERO(&fdp->fd_ip6))
			dst6->sin6_addr = fdp->fd_ip6.in6;
	}

	rtalloc((struct route *)ro);

	if ((ifp == NULL) && (ro->ro_rt != NULL))
		ifp = ro->ro_rt->rt_ifp;

	if ((ro->ro_rt == NULL) || (ifp == NULL)) {
		error = EHOSTUNREACH;
		goto bad;
	}

	rt = fdp ? NULL : ro->ro_rt;

	/* KAME */
	if (IN6_IS_ADDR_LINKLOCAL(&dst6->sin6_addr))
		dst6->sin6_addr.s6_addr16[1] = htons(ifp->if_index);

	{
		if (ro->ro_rt->rt_flags & RTF_GATEWAY)
			dst6 = (struct sockaddr_in6 *)ro->ro_rt->rt_gateway;
		ro->ro_rt->rt_use++;

		error = ip6_getpmtu(ro, ro, ifp, &finaldst, &mtu, &frag);
		if ((error == 0) && (m0->m_pkthdr.len <= mtu)) {
			*mpp = NULL;
			error = nd6_output(ifp, ifp, m0, dst6, rt);
		} else {
			error = EMSGSIZE;
		}
	}
bad:
	if (ro->ro_rt != NULL) {
		RTFREE(ro->ro_rt);
	}
	return error;
}
#endif


int fr_verifysrc(fin)
fr_info_t *fin;
{
	struct sockaddr_in *dst;
	struct route iproute;

	bzero((char *)&iproute, sizeof(iproute));
	dst = (struct sockaddr_in *)&iproute.ro_dst;
	dst->sin_len = sizeof(*dst);
	dst->sin_family = AF_INET;
	dst->sin_addr = fin->fin_src;
	rtalloc(&iproute);
	if (iproute.ro_rt == NULL)
		return 0;
	return (fin->fin_ifp == iproute.ro_rt->rt_ifp);
}


/*
 * return the first IP Address associated with an interface
 */
int fr_ifpaddr(v, atype, ifptr, inp, inpmask)
int v, atype;
void *ifptr;
struct in_addr *inp, *inpmask;
{
#ifdef USE_INET6
	struct in6_addr *inp6 = NULL;
#endif
	struct sockaddr *sock, *mask;
	struct sockaddr_in *sin;
	struct ifaddr *ifa;
	struct ifnet *ifp;

	if ((ifptr == NULL) || (ifptr == (void *)-1))
		return -1;

	ifp = ifptr;
	mask = NULL;

	if (v == 4)
		inp->s_addr = 0;
#ifdef USE_INET6
	else if (v == 6)
		bzero((char *)inp, sizeof(struct in6_addr));
#endif

	ifa = ifp->if_addrlist;
	sock = ifa->ifa_addr;
	while (sock != NULL && ifa != NULL) {
		sin = (struct sockaddr_in *)sock;
		if ((v == 4) && (sin->sin_family == AF_INET))
			break;
#ifdef USE_INET6
		if ((v == 6) && (sin->sin_family == AF_INET6)) {
			inp6 = &((struct sockaddr_in6 *)sin)->sin6_addr;
			if (!IN6_IS_ADDR_LINKLOCAL(inp6) &&
			    !IN6_IS_ADDR_LOOPBACK(inp6))
				break;
		}
#endif
		ifa = ifa->ifa_next;
		if (ifa != NULL)
			sock = ifa->ifa_addr;
	}
	if (ifa == NULL || sock == NULL)
		return -1;

	mask = ifa->ifa_netmask;
	if (atype == FRI_BROADCAST)
		sock = ifa->ifa_broadaddr;
	else if (atype == FRI_PEERADDR)
		sock = ifa->ifa_dstaddr;

#ifdef USE_INET6
	if (v == 6)
		return fr_ifpfillv6addr(atype, (struct sockaddr_in6 *)sock,
					(struct sockaddr_in6 *)mask,
					inp, inpmask);
#endif
	return fr_ifpfillv4addr(atype, (struct sockaddr_in *)sock,
				(struct sockaddr_in *)mask, inp, inpmask);
}


u_32_t fr_newisn(fin)
fr_info_t *fin;
{
	u_32_t newiss;
#if 0
	static int iss_seq_off = 0;
	u_char hash[16];
	MD5_CTX ctx;

	/*
	 * Compute the base value of the ISS.  It is a hash
	 * of (saddr, sport, daddr, dport, secret).
	 */
	MD5Init(&ctx);

	MD5Update(&ctx, (u_char *) &fin->fin_fi.fi_src,
		  sizeof(fin->fin_fi.fi_src));
	MD5Update(&ctx, (u_char *) &fin->fin_fi.fi_dst,
		  sizeof(fin->fin_fi.fi_dst));
	MD5Update(&ctx, (u_char *) &fin->fin_dat, sizeof(fin->fin_dat));

	MD5Update(&ctx, ipf_iss_secret, sizeof(ipf_iss_secret));

	MD5Final(hash, &ctx);

	memcpy(&newiss, hash, sizeof(newiss));

	/*
	 * Now increment our "timer", and add it in to
	 * the computed value.
	 *
	 * XXX Use `addin'?
	 * XXX TCP_ISSINCR too large to use?
	 */
	iss_seq_off += 0x00010000;
	newiss += iss_seq_off;
#endif
	return newiss;
}


/* ------------------------------------------------------------------------ */
/* Function:    fr_nextipid                                                 */
/* Returns:     int - 0 == success, -1 == error (packet should be droppped) */
/* Parameters:  fin(I) - pointer to packet information                      */
/*                                                                          */
/* Returns the next IPv4 ID to use for this packet.                         */
/* ------------------------------------------------------------------------ */
u_short fr_nextipid(fin)
fr_info_t *fin;
{
	static u_short ipid = 0;
	u_short id;

	MUTEX_ENTER(&ipf_rw);
	id = ipid++;
	MUTEX_EXIT(&ipf_rw);

	return id;
}


INLINE void fr_checkv4sum(fin)
fr_info_t *fin;
{
#ifdef M_CSUM_TCP_UDP_BAD
	int manual, pflag, cflags, active;
	mb_t *m;

	if ((fin->fin_flx & FI_NOCKSUM) != 0)
		return;

	manual = 0;
	m = fin->fin_m;
	if (m == NULL) {
		manual = 1;
		goto skipauto;
	}

	switch (fin->fin_p)
	{
	case IPPROTO_UDP :
		pflag = M_CSUM_UDPv4;
		break;
	case IPPROTO_TCP :
		pflag = M_CSUM_TCPv4;
		break;
	default :
		pflag = 0;
		manual = 1;
		break;
	}

	active = ((struct ifnet *)fin->fin_ifp)->if_csum_flags_rx & pflag;
	active |= M_CSUM_TCP_UDP_BAD | M_CSUM_DATA;
	cflags = m->m_pkthdr.csum_flags & active;

	if (pflag != 0) {
		if (cflags == (pflag | M_CSUM_TCP_UDP_BAD)) {
			fin->fin_flx |= FI_BAD;
		} else if (cflags == (pflag | M_CSUM_DATA)) {
			if ((m->m_pkthdr.csum_data ^ 0xffff) != 0)
				fin->fin_flx |= FI_BAD;
		} else if (cflags == pflag) {
			;
		} else {
			manual = 1;
		}
	}
skipauto:
# ifdef IPFILTER_CKSUM
	if (manual != 0)
		if (fr_checkl4sum(fin) == -1)
			fin->fin_flx |= FI_BAD;
# else
	;
# endif
#else
# ifdef IPFILTER_CKSUM
	if (fr_checkl4sum(fin) == -1)
		fin->fin_flx |= FI_BAD;
# endif
#endif
}


#ifdef USE_INET6
INLINE void fr_checkv6sum(fin)
fr_info_t *fin;
{
# ifdef M_CSUM_TCP_UDP_BAD
	int manual, pflag, cflags, active;
	mb_t *m;

	if ((fin->fin_flx & FI_NOCKSUM) != 0)
		return;

	manual = 0;
	m = fin->fin_m;

	switch (fin->fin_p)
	{
	case IPPROTO_UDP :
		pflag = M_CSUM_UDPv6;
		break;
	case IPPROTO_TCP :
		pflag = M_CSUM_TCPv6;
		break;
	default :
		pflag = 0;
		manual = 1;
		break;
	}

	active = ((struct ifnet *)fin->fin_ifp)->if_csum_flags_rx & pflag;
	active |= M_CSUM_TCP_UDP_BAD | M_CSUM_DATA;
	cflags = m->m_pkthdr.csum_flags & active;

	if (pflag != 0) {
		if (cflags == (pflag | M_CSUM_TCP_UDP_BAD)) {
			fin->fin_flx |= FI_BAD;
		} else if (cflags == (pflag | M_CSUM_DATA)) {
			if ((m->m_pkthdr.csum_data ^ 0xffff) != 0)
				fin->fin_flx |= FI_BAD;
		} else if (cflags == pflag) {
			;
		} else {
			manual = 1;
		}
	}
#  ifdef IPFILTER_CKSUM
	if (manual != 0)
		if (fr_checkl4sum(fin) == -1)
			fin->fin_flx |= FI_BAD;
#  endif
# else
#  ifdef IPFILTER_CKSUM
	if (fr_checkl4sum(fin) == -1)
		fin->fin_flx |= FI_BAD;
#  endif
# endif
}
#endif /* USE_INET6 */


size_t mbufchainlen(m0)
struct mbuf *m0;
{
	size_t len;

	if ((m0->m_flags & M_PKTHDR) != 0) {
		len = m0->m_pkthdr.len;
	} else {
		struct mbuf *m;

		for (m = m0, len = 0; m != NULL; m = m->m_next)
			len += m->m_len;
	}
	return len;
}


/* ------------------------------------------------------------------------ */
/* Function:    fr_pullup                                                   */
/* Returns:     NULL == pullup failed, else pointer to protocol header      */
/* Parameters:  m(I)   - pointer to buffer where data packet starts         */
/*              fin(I) - pointer to packet information                      */
/*              len(I) - number of bytes to pullup                          */
/*                                                                          */
/* Attempt to move at least len bytes (from the start of the buffer) into a */
/* single buffer for ease of access.  Operating system native functions are */
/* used to manage buffers - if necessary.  If the entire packet ends up in  */
/* a single buffer, set the FI_COALESCE flag even though fr_coalesce() has  */
/* not been called.  Both fin_ip and fin_dp are updated before exiting _IF_ */
/* and ONLY if the pullup succeeds.                                         */
/*                                                                          */
/* We assume that 'min' is a pointer to a buffer that is part of the chain  */
/* of buffers that starts at *fin->fin_mp.                                  */
/* ------------------------------------------------------------------------ */
void *fr_pullup(min, fin, len)
mb_t *min;
fr_info_t *fin;
int len;
{
	int out = fin->fin_out, dpoff, ipoff;
	mb_t *m = min;
	char *ip;

	if (m == NULL)
		return NULL;

	ip = (char *)fin->fin_ip;
	if ((fin->fin_flx & FI_COALESCE) != 0)
		return ip;

	ipoff = fin->fin_ipoff;
	if (fin->fin_dp != NULL)
		dpoff = (char *)fin->fin_dp - (char *)ip;
	else
		dpoff = 0;

	if (M_LEN(m) < len) {
#ifdef MHLEN
		/*
		 * Assume that M_PKTHDR is set and just work with what is left
		 * rather than check..
		 * Should not make any real difference, anyway.
		 */
		if (len > MHLEN)
#else
		if (len > MLEN)
#endif
		{
			FREE_MB_T(*fin->fin_mp);
			m = NULL;
		} else
		{
			m = m_pullup(m, len);
		}
		*fin->fin_mp = m;
		fin->fin_m = m;
		if (m == NULL) {
			ATOMIC_INCL(frstats[out].fr_pull[1]);
			return NULL;
		}
		ip = MTOD(m, char *) + ipoff;
	}

	ATOMIC_INCL(frstats[out].fr_pull[0]);
	fin->fin_ip = (ip_t *)ip;
	if (fin->fin_dp != NULL)
		fin->fin_dp = (char *)fin->fin_ip + dpoff;

	if (len == fin->fin_plen)
		fin->fin_flx |= FI_COALESCE;
	return ip;
}


void *getifp(name, v)
char *name;
int v;
{
	return NULL;
}
