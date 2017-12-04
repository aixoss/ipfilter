# @(#)51  1.2  src/ipfl/usr/sbin/Makefile, ipflt, 53ipfl53H, 0624A_53ipfl53H 6/13/06 18:40:58
# IBM_PROLOG_BEGIN_TAG 
# This is an automatically generated prolog. 
#  
# 53ipfl53H src/ipfl/usr/sbin/Makefile 1.2 
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
SUBDIRS = 64 man swlag
PROGRAMS = ipf ipfs ipfstat ipmon ipnat ippool ipscan \
		ipsyncm ipsyncs

ILIST = ${PROGRAMS} IPFL.README
IPFL.README_IDIR = /usr/lpp/ipfl/
IDIR = /usr/sbin/

INCFLAGS = -I${MAKETOP}/ipfl/kernext -I${MAKETOP}/ipfl/kernext/netinet
LIBFLAGS = -L${EXPORTBASE}/usr/lib/ipfilter
VPATH = .:${MAKETOP}/ipfl/usr/lib

LIBS = -lipf
CFLAGS = -D_AIX -D_AIX51 -DAIX

ipf_OFILES = ipf.o ipfcomp.o ipf_y.o ipf_l.o
ipfs_OFILES = ipfs.o
ipfstat_OFILES = ipfstat.o
ipmon_OFILES = ipmon.o ipmon_y.o ipmon_l.o
ipnat_OFILES = ipnat.o ipnat_y.o ipnat_l.o
ippool_OFILES = ippool_y.o ippool_l.o kmem.o ippool.o
ipscan_OFILES = ipscan_y.o ipscan_l.o
ipsyncm_OFILES = ipsyncm.o
ipsyncs_OFILES = ipsyncs.o

.include <${RULES_MK}>

ipf_y.h: ipf_y.c
ipf_y.c: ipf_l.h
	${YACC} -d $<
	${SED} -e 's/yy/ipf_yy/g' -e 's/y.tab.c/ipf_y.c/' \
		y.tab.c > $@
	${SED} -e 's/yy/ipf_yy/g' -e 's/y.tab.h/ipf_y.h/' \
		y.tab.h > ipf_y.h
	${RM} -f y.tab.c y.tab.h
ipf_l.c: lexer.c ipf_y.h
	${SED} -e 's/yy/ipf_yy/g' -e 's/y.tab.h/ipf_y.h/' \
		-e 's/lexer.h/ipf_l.h/' ${lexer.c:P} > $@
ipf_l.h: lexer.h
	${SED} -e 's/yy/ipf_yy/g' ${lexer.h:P} > $@

ipmon_y.h: ipmon_y.c
ipmon_y.c: ipmon_l.h
	${YACC} -d $<
	${SED} -e 's/yy/ipmon_yy/g' y.tab.c > $@
	${SED} -e 's/yy/ipmon_yy/g' y.tab.h > ipmon_y.h
	${RM} -f y.tab.c y.tab.h
ipmon_l.c: lexer.c ipmon_y.h
	${SED} -e 's/yy/ipmon_yy/g' -e 's/y.tab.h/ipmon_y.h/' \
		-e 's/lexer.h/ipmon_l.h/' ${lexer.c:P} > $@
ipmon_l.h: lexer.h
	${SED} -e 's/yy/ipmon_yy/g' ${lexer.h:P} > $@

ipnat_y.h: ipnat_y.c
ipnat_y.c: ipnat_l.h
	${YACC} -d $<
	${SED} -e 's/yy/ipnat_yy/g' -e 's/y.tab.c/ipnat_y.c/' \
		y.tab.c > $@
	${SED} -e 's/yy/ipnat_yy/g' -e 's/y.tab.h/ipnat_y.h/' \
		y.tab.h > ipnat_y.h
	${RM} -f y.tab.c y.tab.h
ipnat_l.c: lexer.c ipnat_y.h
	${SED} -e 's/yy/ipnat_yy/g' -e 's/y.tab.h/ipnat_y.h/' \
		-e 's/lexer.h/ipnat_l.h/' ${lexer.c:P} > $@
ipnat_l.h: lexer.h
	${SED} -e 's/yy/ipnat_yy/g' ${lexer.h:P} > $@

ippool_y.h: ippool_y.c
ippool_y.c: ippool_l.h
	${YACC} -d $<
	${SED} -e 's/yy/ippool_yy/g' y.tab.c > $@
	${SED} -e 's/yy/ippool_yy/g' y.tab.h > ippool_y.h
	${RM} -f y.tab.c y.tab.h
ippool_l.c: lexer.c ippool_y.h
	${SED} -e 's/yy/ippool_yy/g' -e 's/y.tab.h/ippool_y.h/' \
		-e 's/lexer.h/ippool_l.h/' ${lexer.c:P} > $@
ippool_l.h: lexer.h
	${SED} -e 's/yy/ippool_yy/g' ${lexer.h:P} > $@

ipscan_y.h: ipscan_y.c
ipscan_y.c: ipscan_l.h
	${YACC} -d $<
	${SED} -e 's/yy/ipscan_yy/g' y.tab.c > $@
	${SED} -e 's/yy/ipscan_yy/g' y.tab.h > ipscan_y.h
	${RM} -f y.tab.c y.tab.h
ipscan_l.c: lexer.c ipscan_y.h
	${SED} -e 's/yy/ipscan_yy/g' -e 's/y.tab.h/ipscan_y.h/' \
		-e 's/lexer.h/ipscan_l.h/' ${lexer.c:P} > $@
ipscan_l.h: lexer.h
	${SED} -e 's/yy/ipscan_yy/g' ${lexer.h:P} > $@
