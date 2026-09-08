# @(#)57  1.1  src/ipfl/usr/lib/methods/Makefile, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:32:16
# IBM_PROLOG_BEGIN_TAG 
# This is an automatically generated prolog. 
#  
# 53ipfl53H src/ipfl/usr/lib/methods/Makefile 1.1 
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

MAKETOP=../../..
CC = gcc
INCFLAGS = -I${MAKETOP}/kernext -I${MAKETOP}/kernext/netinet
CFLAGS = -D_ALL_SOURCE -D_AIX -D_AIX51 -DAIX -O2 -maix64 ${INCFLAGS}
LIBS = -lodm -lcfg -lrts

cfg_ipf: cfg_ipf.c
	${CC} ${CFLAGS} -o $@ $^ ${LIBS}

all: cfg_ipf
clean:
	rm -f *.o cfg_ipf
