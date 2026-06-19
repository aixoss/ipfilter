# @(#)52  1.1  src/ipfl/usr/Makefile, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:32:09
# IBM_PROLOG_BEGIN_TAG 
# This is an automatically generated prolog. 
#  
# 53ipfl53H src/ipfl/usr/Makefile 1.1 
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
SUBDIRS = lib sbin
EXPLIB_SUBDIRS = lib

#.include <${RULES_MK}>
all:
	for i in $(SUBDIRS); do \
		(cd $$i && $(MAKE) $(MAKEFLAGS)); \
	done
