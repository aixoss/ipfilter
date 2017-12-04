# @(#)56  1.1  src/ipfl/usr/lib/Makefile, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/8/06 07:32:14
# IBM_PROLOG_BEGIN_TAG 
# This is an automatically generated prolog. 
#  
# 53ipfl53H src/ipfl/usr/lib/Makefile 1.1 
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
SUBDIRS = methods

LIBRARIES = libipf.a
EXPLIB_TARGETS = export_libipf.a
EXPDIR = /usr/lib/ipfilter/

INCFLAGS = -I${MAKETOP}/ipfl/kernext -I${MAKETOP}/ipfl/kernext/netinet
CFLAGS = -D_AIX -D_AIX51 -DAIX -qlanglvl=ansi

OFILES = addicmp.o addipopt.o bcopywrap.o \
     binprint.o buildopts.o checkrev.o count6bits.o count4bits.o \
     debug.o extras.o facpri.o flags.o fill6bits.o genmask.o \
     gethost.o getifname.o getline.o getnattype.o getport.o \
     getportproto.o getproto.o getsumd.o hexdump.o hostmask.o \
     hostname.o hostnum.o icmpcode.o inet_addr.o initparse.o \
     ionames.o ipoptsec.o ipf_dotuning.o ipft_ef.o ipft_hx.o \
     ipft_pc.o ipft_sn.o ipft_td.o ipft_tx.o kmem.o kmemcpywrap.o \
     kvatoname.o load_hash.o load_hashnode.o load_pool.o load_poolnode.o \
     loglevel.o mutex_emul.o nametokva.o nat_setgroupmap.o \
     ntomask.o optname.o optprint.o optprintv6.o optvalue.o portname.o \
     portnum.o ports.o print_toif.o printactivenat.o printaps.o printbuf.o \
     printhash.o printhashnode.o printip.o printpool.o printpoolnode.o \
     printproto.o printfr.o printfraginfo.o printhostmap.o printifname.o \
     printhostmask.o printlog.o printmask.o printnat.o printportcmp.o \
     printpacket.o printpacket6.o printsbuf.o printstate.o printtunable.o \
     ratoi.o ratoui.o remove_hash.o remove_hashnode.o remove_pool.o \
     remove_poolnode.o resetlexer.o rwlock_emul.o tcpflags.o tcp_flags.o \
     to_interface.o var.o verbose.o v6ionames.o v6optvalue.o
OFILES64 = ${OFILES:.o=.64o}

.include <${RULES_MK}>
