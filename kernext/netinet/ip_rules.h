/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 71ipfl71c src/ipfl/kernext/netinet/ip_rules.h 1.2                      */
/*                                                                        */
/* Licensed Materials - Property of IBM                                   */
/*                                                                        */
/* Restricted Materials of IBM                                            */
/*                                                                        */
/* COPYRIGHT International Business Machines Corp. 2006                   */
/* All Rights Reserved                                                    */
/*                                                                        */
/* US Government Users Restricted Rights - Use, duplication or            */
/* disclosure restricted by GSA ADP Schedule Contract with IBM Corp.      */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
/* @(#)28  1.2  src/ipfl/kernext/netinet/ip_rules.h, ipflt, 71ipfl71c 5/10/06 12:55:27 */
extern int ipfrule_add __P((void));
extern int ipfrule_remove __P((void));

extern frentry_t *ipfrule_match_out_ __P((fr_info_t *, u_32_t *));
extern frentry_t *ipf_rules_out_[1];

extern int ipfrule_add_out_ __P((void));
extern int ipfrule_remove_out_ __P((void));

extern frentry_t *ipfrule_match_in_ __P((fr_info_t *, u_32_t *));
extern frentry_t *ipf_rules_in_[1];

extern int ipfrule_add_in_ __P((void));
extern int ipfrule_remove_in_ __P((void));
