/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* 53ipfl53H src/ipfl/usr/sbin/lexer.h 1.1                                */
/*                                                                        */
/* Licensed Materials - Property of IBM                                   */
/*                                                                        */
/* Restricted Materials of IBM                                            */
/*                                                                        */
/* (C) COPYRIGHT International Business Machines Corp. 2006               */
/* All Rights Reserved                                                    */
/*                                                                        */
/* US Government Users Restricted Rights - Use, duplication or            */
/* disclosure restricted by GSA ADP Schedule Contract with IBM Corp.      */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
/* @(#)78  1.2  src/ipfl/usr/sbin/lexer.h, ipflt, 53ipfl53H, 0619A_53ipfl53H 5/10/06 12:55:05 */

typedef	struct	wordtab	{
	char	*w_word;
	int	w_value;
} wordtab_t;

#ifdef	NO_YACC
#define	YY_COMMENT	1000
#define	YY_CMP_NE	1001
#define	YY_CMP_LE	1002
#define	YY_RANGE_OUT	1003
#define	YY_CMP_GE	1004
#define	YY_RANGE_IN	1005
#define	YY_HEX		1006
#define	YY_NUMBER	1007
#define	YY_IPV6		1008
#define	YY_STR		1009
#define	YY_IPADDR	1010
#endif

#define	YYBUFSIZ	8192

extern	wordtab_t	*yysettab __P((wordtab_t *));
extern	void		yysetdict __P((wordtab_t *));
extern	int		yylex __P((void));
extern	void		yyerror __P((char *));
extern	char		*yykeytostr __P((int));
extern	void		yyresetdict __P((void));

extern	FILE	*yyin;
extern	int	yylineNum;
extern	int	yyexpectaddr;
extern	int	yybreakondot;
extern	int	yyvarnext;

