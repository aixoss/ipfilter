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

MAKETOP=../..
INCFLAGS = -I${MAKETOP}/kernext -I${MAKETOP}/kernext/netinet -include strings.h
CC = gcc

all:   ipf

clean:
	rm -f *.o ipf

KERNEXT_IMPORTS += sockets.exp kernex.exp netinet.exp statcmd.exp 

INCFLAGS = -I. -I./netinet -include strings.h
CFLAGS = -O2 -nostdlib -DAIX -D_AIX -D_AIX51 -DIPFILTER_LKM -DIPFILTER_LOG \
	-DIPFILTER_LOOKUP -DIPFILTER_SCAN -DKERNEL -D_KERNEL -DINET ${INCFLAGS}
FIXRADIX = -Dradix_node=ipf_radix_node -Dradix_node_head=ipf_radix_node_head \
	-Drn_inithead=ipf_rn_inithead -Drn_walktree=ipf_rn_walktree

fil.o:  fil.c
	${CC} -maix64 ${CFLAGS} -DIPF_DEFAULT_PASS=FR_PASS -c $< -o $@

ip_state.o: ip_state.c
	${CC} -maix64 ${CFLAGS} -DIPSTATE_SIZE=249989 -DIPSTATE_MAX=1000000 -DIPSTATE_MAXBUCKET=256 -c $< -o $@

ip_pool.o: ip_pool.c
	${CC} ${CFLAGS} ${FIXRADIX} -maix64 -c $< -o $@

ip_lookup.o: ip_lookup.c
	${CC} ${CFLAGS} ${FIXRADIX} -maix64 -c $< -o $@

radix.o: radix.c
	${CC} ${CFLAGS} ${FIXRADIX} -maix64 -c $< -o $@

ip_fil_aix.o: ip_fil_aix.c
	${CC} ${CFLAGS} -maix64 -c $< -o $@

%.o:  %.c
	${CC} ${CFLAGS} -maix64 -c $< -o $@

OBJFILES = ip_fil_aix.o fil.o md5.o ip_nat.o ip_frag.o ip_state.o \
	ip_proxy.o ip_auth.o ip_log.o ip_pool.o ip_htable.o \
	ip_lookup.o ip_sync.o ip_scan.o ip_rules.o radix.o log.o

ipf: ${OBJFILES}
	ld -b64 -o $@ $^ -bI:/usr/lib/kernex.exp -bI:/usr/lib/netinet.exp -bI:/usr/lib/sockets.exp -bI:/usr/lib/statcmd.exp -e ipfconfig -l csys -l sys
