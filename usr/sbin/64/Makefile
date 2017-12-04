# @(#)52  1.1  src/ipfl/usr/sbin/64/Makefile, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 13:52:47
# IBM_PROLOG_BEGIN_TAG 
# This is an automatically generated prolog. 
#  
# 53ipfl53H src/ipfl/usr/sbin/64/Makefile 1.1 
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
BUILD64 = true
KOBJ = 64

PROGRAMS = ipf64 ipfs64 ipfstat64 ipmon64 ipnat64 ippool64 \
		ipscan64 ipsyncm64 ipsyncs64

ILIST = ${PROGRAMS}
IDIR = /usr/sbin/

INCFLAGS = -I${MAKETOP}/ipfl/kernext -I${MAKETOP}/ipfl/kernext/netinet
LIBFLAGS = -L${EXPORTBASE}/usr/lib/ipfilter
VPATH = ..:${MAKETOP}/ipfl/usr/lib

LIBS = -lipf
CFLAGS = -D_AIX -D_AIX51 -DAIX

ipf64_OFILES = ipf.o ipfcomp.o ipf_y.o ipf_l.o
ipfs64_OFILES = ipfs.o
ipfstat64_OFILES = ipfstat.o
ipmon64_OFILES = ipmon.o ipmon_y.o ipmon_l.o
ipnat64_OFILES = ipnat.o ipnat_y.o ipnat_l.o
ippool64_OFILES = ippool_y.o ippool_l.o kmem.o ippool.o
ipscan64_OFILES = ipscan_y.o ipscan_l.o
ipsyncm64_OFILES = ipsyncm.o
ipsyncs64_OFILES = ipsyncs.o

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
