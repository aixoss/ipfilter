# @(#)58  1.1  src/ipfl/kernext/Makefile, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:32:17
# IBM_PROLOG_BEGIN_TAG 
# This is an automatically generated prolog. 
#  
# 53ipfl53H src/ipfl/kernext/Makefile 1.1 
#  
# Licensed Materials - Property of IBM 
#  
# Restricted Materials of IBM 
#  
# (C) COPYRIGHT International Business Machines Corp. 2006 
# All Rights Reserved 
#  
# US Government Users Restricted Rights - Use, duplication or 
# disclosure restricted by GSA ADP Schedule Contract with IBM Corp. 
#  
# IBM_PROLOG_END_TAG 
.include <${MAKETOP}bos/kernext/Kernext.mk>

VPATH = .:${MAKETOP}/bos/kernel/exp:${MAKETOP}/bos/kernext/inet

DUALMODE_KEXT   = true

KERNEL_EXT = ipf
KERNEL_EXT32 = ipf32
KERNEL_EXT64 = ipf64

IDIR            = /usr/lib/drivers/
ILIST           = ${KERNEL_EXT}

LIBS            = ${KERNEXT_LIBS}

ipf32_ENTRYPOINT = ipfconfig
ipf64_ENTRYPOINT = ipfconfig

KERNEXT_IMPORTS += sockets.exp kernex.exp netinet.exp statcmd.exp 

INCFLAGS = -I./netinet
CFLAGS = -DAIX -D_AIX -D_AIX51 -DIPFILTER_LKM -DIPFILTER_LOG \
	-DIPFILTER_LOOKUP -DIPFILTER_SCAN -DKERNEL -D_KERNEL -DINET
FIXRADIX = -Dradix_node=ipf_radix_node -Dradix_node_head=ipf_radix_node_head \
	-Drn_inithead=ipf_rn_inithead -Drn_walktree=ipf_rn_walktree

fil.o_CFLAGS = ${CFLAGS} -DIPF_DEFAULT_PASS=FR_PASS
ip_state.o_CFLAGS = ${CFLAGS} -DIPSTATE_SIZE=127
ip_pool.o_CFLAGS = ${CFLAGS} ${FIXRADIX}
ip_lookup.o_CFLAGS = ${CFLAGS} ${FIXRADIX}

fil.64o_CFLAGS = ${CFLAGS} -DIPF_DEFAULT_PASS=FR_PASS
ip_state.64o_CFLAGS = ${CFLAGS} -DIPSTATE_SIZE=127
ip_pool.64o_CFLAGS = ${CFLAGS} ${FIXRADIX}
ip_lookup.64o_CFLAGS = ${CFLAGS} ${FIXRADIX}

OBJFILES = ip_fil_aix.o fil.o md5.o ip_nat.o ip_frag.o ip_state.o \
	ip_proxy.o ip_auth.o ip_log.o ip_pool.o ip_htable.o \
	ip_lookup.o ip_sync.o ip_scan.o ip_rules.o radix.o

ipf32_OFILES = ${OBJFILES}
ipf64_OFILES = ${OBJFILES:.o=.64o}

.include <${RULES_MK}>
