# @(#)57  1.1  src/ipfl/usr/lib/methods/Makefile, ipflt, 71ipfl71c 5/8/06 07:32:16
# IBM_PROLOG_BEGIN_TAG 
# This is an automatically generated prolog. 
#  
# 71ipfl71c src/ipfl/usr/lib/methods/Makefile 1.2 
#  
# Licensed Materials - Property of IBM 
#  
# Restricted Materials of IBM 
#  
# COPYRIGHT International Business Machines Corp. 2006,2025 
# All Rights Reserved 
#  
# US Government Users Restricted Rights - Use, duplication or 
# disclosure restricted by GSA ADP Schedule Contract with IBM Corp. 
#  
# IBM_PROLOG_END_TAG 
PROGRAMS = cfg_ipf cfg_ipf64

cfg_ipf64_BUILD64 = true
VPATH = .

cfg_ipf_OFILES = cfg_ipf.o
cfg_ipf64_OFILES = cfg_ipf.64o

ILIST = ${PROGRAMS}
IDIR = /usr/lib/methods/

INCFLAGS = -I${MAKETOP}/ipfl/kernext -I${MAKETOP}/ipfl/kernext/netinet
CFLAGS = -D_ALL_SOURCE -D_AIX -D_AIX51 -DAIX
LIBS = -lodm -lcfg -lrts

.include<${RULES_MK}>
