
/*  A Bison parser, made from Svc_Conf.y
    by GNU Bison version 1.28  */

#if defined (ACE_HAS_LEGACY_SERVICE_CONFIG)
#define ACE_YYBISON 1  /* Identify Bison output.  */

#define	ACE_DYNAMIC	257
#define	ACE_STATIC	258
#define	ACE_SUSPEND	259
#define	ACE_RESUME	260
#define	ACE_REMOVE	261
#define	ACE_USTREAM	262
#define	ACE_MODULE_T	263
#define	ACE_STREAM_T	264
#define	ACE_SVC_OBJ_T	265
#define	ACE_ACTIVE	266
#define	ACE_INACTIVE	267
#define	ACE_PATHNAME	268
#define	ACE_IDENT	269
#define	ACE_STRING	270
#endif /* ACE_HAS_LEGACY_SERVICE_CONFIG */


// $Id$

#include "ace/ARGV.h"
#include "ace/Svc_Conf.h"
#include "ace/Module.h"
#include "ace/Stream.h"

ACE_RCSID (ace,
           Svc_Conf_y,
           "$Id$")

#if defined (ACE_HAS_LEGACY_SERVICE_CONFIG)
// Prototypes.
static ACE_Module_Type *ace_get_module (ACE_Static_Node *str_rec,
                                        ACE_Static_Node *svc_type);
static ACE_Module_Type *ace_get_module (ACE_Static_Node *str_rec,
                                        const ACE_TCHAR *svc_name);

#define ACE_YYDEBUG_LEXER_TEXT (ace_yytext[ace_yyleng] = '\0', ace_yytext)

// Force the pretty debugging code to compile.
// #define ACE_YYDEBUG 1

// Keeps track of the number of errors encountered so far.
int ace_yyerrno = 0;

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	ACE_YYFINAL		66
#define	ACE_YYFLAG		-32768
#define	ACE_YYNTBASE	23

#define ACE_YYTRANSLATE(x) ((unsigned)(x) <= 270 ? ace_yytranslate[x] : 43)

static const ACE_TCHAR ace_yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    20,
    21,    22,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    19,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    17,     2,    18,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16
};

#if ACE_YYDEBUG != 0
static const short ace_yyprhs[] = {     0,
     0,     3,     6,     7,     9,    11,    13,    15,    17,    19,
    23,    27,    30,    33,    36,    40,    41,    46,    48,    50,
    51,    56,    57,    60,    61,    63,    65,    67,    69,    71,
    76,    78,    80,    81,    85,    91,    96,    99,   102,   105,
   107,   108,   110,   112
};

static const short ace_yyrhs[] = {    23,
    24,     0,    23,     1,     0,     0,    25,     0,    26,     0,
    27,     0,    28,     0,    29,     0,    30,     0,     3,    37,
    41,     0,     4,    15,    41,     0,     5,    15,     0,     6,
    15,     0,     7,    15,     0,     8,    32,    33,     0,     0,
     8,    15,    31,    33,     0,    25,     0,    26,     0,     0,
    17,    34,    35,    18,     0,     0,    35,    36,     0,     0,
    25,     0,    26,     0,    27,     0,    28,     0,    29,     0,
    15,    40,    39,    38,     0,    12,     0,    13,     0,     0,
    42,    19,    15,     0,    42,    19,    15,    20,    21,     0,
    19,    15,    20,    21,     0,     9,    22,     0,    11,    22,
     0,    10,    22,     0,    16,     0,     0,    14,     0,    15,
     0,    16,     0
};

#endif

#if ACE_YYDEBUG != 0
static const short ace_yyrline[] = { 0,
    49,    57,    61,    65,    66,    67,    68,    69,    70,    74,
    84,    91,    98,   105,   112,   116,   117,   123,   126,   132,
   137,   141,   145,   153,   157,   183,   194,   201,   208,   231,
   265,   269,   273,   280,   284,   288,   295,   299,   303,   310,
   311,   315,   316,   317
};
#endif


#if ACE_YYDEBUG != 0 || defined (ACE_YYERROR_VERBOSE)

static const ACE_TCHAR * const ace_yytname[] = { ACE_LIB_TEXT("$"),
                                                 ACE_LIB_TEXT("error"),
                                                 ACE_LIB_TEXT("$undefined."),
                                                 ACE_LIB_TEXT("ACE_DYNAMIC"),
                                                 ACE_LIB_TEXT("ACE_STATIC"),
                                                 ACE_LIB_TEXT("ACE_SUSPEND"),
                                                 ACE_LIB_TEXT("ACE_RESUME"),
                                                 ACE_LIB_TEXT("ACE_REMOVE"),
                                                 ACE_LIB_TEXT("ACE_USTREAM"),
                                                 ACE_LIB_TEXT("ACE_MODULE_T"),
                                                 ACE_LIB_TEXT("ACE_STREAM_T"),
                                                 ACE_LIB_TEXT("ACE_SVC_OBJ_T"),
                                                 ACE_LIB_TEXT("ACE_ACTIVE"),
                                                 ACE_LIB_TEXT("ACE_INACTIVE"),
                                                 ACE_LIB_TEXT("ACE_PATHNAME"),
                                                 ACE_LIB_TEXT("ACE_IDENT"),
                                                 ACE_LIB_TEXT("ACE_STRING"),
                                                 ACE_LIB_TEXT("'{'"),
                                                 ACE_LIB_TEXT("'}'"),
                                                 ACE_LIB_TEXT("':'"),
                                                 ACE_LIB_TEXT("'('"),
                                                 ACE_LIB_TEXT("')'"),
                                                 ACE_LIB_TEXT("'*'"),
                                                 ACE_LIB_TEXT("svc_config_entries"),
                                                 ACE_LIB_TEXT("svc_config_entry"),
                                                 ACE_LIB_TEXT("dynamic"),
                                                 ACE_LIB_TEXT("static"),
                                                 ACE_LIB_TEXT("suspend"),
                                                 ACE_LIB_TEXT("resume"),
                                                 ACE_LIB_TEXT("remove"),
                                                 ACE_LIB_TEXT("stream"),
                                                 ACE_LIB_TEXT("@1"),
                                                 ACE_LIB_TEXT("stream_ops"),
                                                 ACE_LIB_TEXT("stream_modules"),

                                                 ACE_LIB_TEXT("@2"),
                                                 ACE_LIB_TEXT("module_list"),
                                                 ACE_LIB_TEXT("module"),
                                                 ACE_LIB_TEXT("svc_location"),
                                                 ACE_LIB_TEXT("status"),
                                                 ACE_LIB_TEXT("svc_initializer"),
                                                 ACE_LIB_TEXT("type"),
                                                 ACE_LIB_TEXT("parameters_opt"),
                                                 ACE_LIB_TEXT("pathname"),
                                                 NULL
};
#endif

static const short ace_yyr1[] = {     0,
    23,    23,    23,    24,    24,    24,    24,    24,    24,    25,
    26,    27,    28,    29,    30,    31,    30,    32,    32,    34,
    33,    33,    35,    35,    36,    36,    36,    36,    36,    37,
    38,    38,    38,    39,    39,    39,    40,    40,    40,    41,
    41,    42,    42,    42
};

static const short ace_yyr2[] = {     0,
     2,     2,     0,     1,     1,     1,     1,     1,     1,     3,
     3,     2,     2,     2,     3,     0,     4,     1,     1,     0,
     4,     0,     2,     0,     1,     1,     1,     1,     1,     4,
     1,     1,     0,     3,     5,     4,     2,     2,     2,     1,
     0,     1,     1,     1
};

static const short ace_yydefact[] = {     3,
     0,     2,     0,     0,     0,     0,     0,     0,     1,     4,
     5,     6,     7,     8,     9,     0,    41,    41,    12,    13,
    14,    16,    18,    19,    22,     0,     0,     0,     0,    40,
    10,    11,    22,    20,    15,    37,    39,    38,    42,    43,
    44,     0,    33,     0,    17,    24,     0,    31,    32,    30,
     0,     0,     0,    34,    21,    25,    26,    27,    28,    29,
    23,    36,     0,    35,     0,     0
};

static const short ace_yydefgoto[] = {     1,
     9,    10,    11,    12,    13,    14,    15,    33,    25,    35,
    46,    52,    61,    17,    50,    43,    29,    31,    44
};

static const short ace_yypact[] = {-32768,
    20,-32768,     1,     3,     7,    14,    18,     4,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,    21,    19,    19,-32768,-32768,
-32768,-32768,-32768,-32768,    -2,    12,    15,    16,    -5,-32768,
-32768,-32768,    -2,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,    24,     0,    17,-32768,-32768,    22,-32768,-32768,-32768,
    25,    -1,    26,    23,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,    27,-32768,    41,-32768
};

static const short ace_yypgoto[] = {-32768,
-32768,    -8,    -7,    -6,    -3,     2,-32768,-32768,-32768,    28,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,    32,-32768
};


#define	ACE_YYLAST		61


static const short ace_yytable[] = {    23,
    24,     3,     4,     5,     6,     7,     3,     4,    39,    40,
    41,    48,    49,    42,    34,    16,    55,    18,    22,    65,
     2,    19,     3,     4,     5,     6,     7,     8,    20,    26,
    27,    28,    21,    36,    30,    51,    37,    38,    47,    54,
    66,    53,    63,    56,    57,    58,    62,    64,    59,    32,
     0,     0,     0,    60,     0,     0,     0,     0,     0,     0,
    45
};

static const short ace_yycheck[] = {     8,
     8,     3,     4,     5,     6,     7,     3,     4,    14,    15,
    16,    12,    13,    19,    17,    15,    18,    15,    15,     0,
     1,    15,     3,     4,     5,     6,     7,     8,    15,     9,
    10,    11,    15,    22,    16,    19,    22,    22,    15,    15,
     0,    20,    20,    52,    52,    52,    21,    21,    52,    18,
    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    -1,
    33
};
#define ACE_YYPURE 1

/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */

/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef ACE_YYSTACK_USE_ALLOCA
#ifdef alloca
#define ACE_YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define ACE_YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define ACE_YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define ACE_YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define ACE_YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* ACE_YYSTACK_USE_ALLOCA not defined */

#ifdef ACE_YYSTACK_USE_ALLOCA
#define ACE_YYSTACK_ALLOC alloca
#else
#define ACE_YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define ace_yyerrok		(ace_yyerrstatus = 0)
#define ace_yyclearin	(ace_yychar = ACE_YYEMPTY)
#define ACE_YYEMPTY		-2
#define ACE_YYEOF		0
#define ACE_YYACCEPT	goto ace_yyacceptlab
#define ACE_YYABORT 	goto ace_yyabortlab
#define ACE_YYERROR		goto ace_yyerrlab1
/* Like ACE_YYERROR except do call ace_yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of ACE_YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define ACE_YYFAIL		goto ace_yyerrlab
#define ACE_YYRECOVERING()  (!!ace_yyerrstatus)
#define ACE_YYBACKUP(token, value) \
do								\
  if (ace_yychar == ACE_YYEMPTY && ace_yylen == 1)				\
    { ace_yychar = (token), ace_yylval = (value);			\
      ace_yychar1 = ACE_YYTRANSLATE (ace_yychar);				\
      ACE_YYPOPSTACK;						\
      goto ace_yybackup;						\
    }								\
  else								\
    { ace_yyerror (ACE_LIB_TEXT("syntax error: cannot back up")); ACE_YYERROR; }	\
while (0)

#define ACE_YYTERROR	1
#define ACE_YYERRCODE	256

#ifndef ACE_YYPURE
#define ACE_YYLEX		ace_yylex()
#endif

#ifdef ACE_YYPURE
#ifdef ACE_YYLSP_NEEDED
#ifdef ACE_YYLEX_PARAM
#define ACE_YYLEX		ace_yylex(&ace_yylval, &ace_yylloc, ACE_YYLEX_PARAM)
#else
#define ACE_YYLEX		ace_yylex(&ace_yylval, &ace_yylloc)
#endif
#else /* not ACE_YYLSP_NEEDED */
#ifdef ACE_YYLEX_PARAM
#define ACE_YYLEX		ace_yylex(&ace_yylval, ACE_YYLEX_PARAM)
#else
#define ACE_YYLEX		ace_yylex(&ace_yylval)
#endif
#endif /* not ACE_YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef ACE_YYPURE

int	ace_yychar;			/*  the lookahead symbol		*/
ACE_YYSTYPE	ace_yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef ACE_YYLSP_NEEDED
ACE_YYLTYPE ace_yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int ace_yynerrs;			/*  number of parse errors so far       */
#endif  /* not ACE_YYPURE */

#if ACE_YYDEBUG != 0
int ace_yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  ACE_YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	ACE_YYINITDEPTH
#define ACE_YYINITDEPTH 200
#endif

/*  ACE_YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if ACE_YYMAXDEPTH == 0
#undef ACE_YYMAXDEPTH
#endif

#ifndef ACE_YYMAXDEPTH
#define ACE_YYMAXDEPTH 10000
#endif

/* Define __ace_yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __ace_yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__ace_yy_memcpy (to, from, count)
     ACE_TCHAR *to;
     ACE_TCHAR *from;
     unsigned int count;
{
  register ACE_TCHAR *f = from;
  register ACE_TCHAR *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__ace_yy_memcpy (ACE_TCHAR *to, ACE_TCHAR *from, unsigned int count)
{
  register ACE_TCHAR *t = to;
  register ACE_TCHAR *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif



/* The user can define ACE_YYPARSE_PARAM as the name of an argument to be passed
   into ace_yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef ACE_YYPARSE_PARAM
#ifdef __cplusplus
#define ACE_YYPARSE_PARAM_ARG void *ACE_YYPARSE_PARAM
#define ACE_YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define ACE_YYPARSE_PARAM_ARG ACE_YYPARSE_PARAM
#define ACE_YYPARSE_PARAM_DECL void *ACE_YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not ACE_YYPARSE_PARAM */
#define ACE_YYPARSE_PARAM_ARG
#define ACE_YYPARSE_PARAM_DECL
#endif /* not ACE_YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef ACE_YYPARSE_PARAM
int ace_yyparse (void *);
#else
int ace_yyparse (void);
#endif
#endif

int
ace_yyparse(ACE_YYPARSE_PARAM_ARG)
     ACE_YYPARSE_PARAM_DECL
{
  register int ace_yystate;
  register int ace_yyn;
  register short *ace_yyssp;
  register ACE_YYSTYPE *ace_yyvsp;
  int ace_yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int ace_yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	ace_yyssa[ACE_YYINITDEPTH];	/*  the state stack			*/
  ACE_YYSTYPE ace_yyvsa[ACE_YYINITDEPTH];	/*  the semantic value stack		*/

  short *ace_yyss = ace_yyssa;		/*  refer to the stacks thru separate pointers */
  ACE_YYSTYPE *ace_yyvs = ace_yyvsa;	/*  to allow ace_yyoverflow to reallocate them elsewhere */

#ifdef ACE_YYLSP_NEEDED
  ACE_YYLTYPE ace_yylsa[ACE_YYINITDEPTH];	/*  the location stack			*/
  ACE_YYLTYPE *ace_yyls = ace_yylsa;
  ACE_YYLTYPE *ace_yylsp;

#define ACE_YYPOPSTACK   (ace_yyvsp--, ace_yyssp--, ace_yylsp--)
#else
#define ACE_YYPOPSTACK   (ace_yyvsp--, ace_yyssp--)
#endif

  int ace_yystacksize = ACE_YYINITDEPTH;
  int ace_yyfree_stacks = 0;

#ifdef ACE_YYPURE
  int ace_yychar;
  ACE_YYSTYPE ace_yylval;
  int ace_yynerrs;
#ifdef ACE_YYLSP_NEEDED
  ACE_YYLTYPE ace_yylloc;
#endif
#endif

  ACE_YYSTYPE ace_yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int ace_yylen;

#if ACE_YYDEBUG != 0
  if (ace_yydebug)
    ACE_OS::fprintf(stderr, ACE_LIB_TEXT("Starting parse\n"));
#endif

  ace_yystate = 0;
  ace_yyerrstatus = 0;
  ace_yynerrs = 0;
  ace_yychar = ACE_YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  ace_yyssp = ace_yyss - 1;
  ace_yyvsp = ace_yyvs;
#ifdef ACE_YYLSP_NEEDED
  ace_yylsp = ace_yyls;
#endif

/* Push a new state, which is found in  ace_yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
ace_yynewstate:

  *++ace_yyssp = ace_yystate;

  if (ace_yyssp >= ace_yyss + ace_yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      ACE_YYSTYPE *ace_yyvs1 = ace_yyvs;
      short *ace_yyss1 = ace_yyss;
#ifdef ACE_YYLSP_NEEDED
      ACE_YYLTYPE *ace_yyls1 = ace_yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = ace_yyssp - ace_yyss + 1;

#ifdef ace_yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef ACE_YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if ace_yyoverflow is a macro.  */
      ace_yyoverflow(ACE_LIB_TEXT("parser stack overflow"),
		 &ace_yyss1, size * sizeof (*ace_yyssp),
		 &ace_yyvs1, size * sizeof (*ace_yyvsp),
		 &ace_yyls1, size * sizeof (*ace_yylsp),
		 &ace_yystacksize);
#else
      ace_yyoverflow(ACE_LIB_TEXT("parser stack overflow"),
		 &ace_yyss1, size * sizeof (*ace_yyssp),
		 &ace_yyvs1, size * sizeof (*ace_yyvsp),
		 &ace_yystacksize);
#endif

      ace_yyss = ace_yyss1; ace_yyvs = ace_yyvs1;
#ifdef ACE_YYLSP_NEEDED
      ace_yyls = ace_yyls1;
#endif
#else /* no ace_yyoverflow */
      /* Extend the stack our own way.  */
      if (ace_yystacksize >= ACE_YYMAXDEPTH)
	{
	  ace_yyerror(ACE_LIB_TEXT("parser stack overflow"));
	  if (ace_yyfree_stacks)
	    {
	      free (ace_yyss);
	      free (ace_yyvs);
#ifdef ACE_YYLSP_NEEDED
	      free (ace_yyls);
#endif
	    }
	  return 2;
	}
      ace_yystacksize *= 2;
      if (ace_yystacksize > ACE_YYMAXDEPTH)
	ace_yystacksize = ACE_YYMAXDEPTH;
#ifndef ACE_YYSTACK_USE_ALLOCA
      ace_yyfree_stacks = 1;
#endif
      ace_yyss = (short *) ACE_YYSTACK_ALLOC (ace_yystacksize * sizeof (*ace_yyssp));
      __ace_yy_memcpy ((ACE_TCHAR *)ace_yyss, (ACE_TCHAR *)ace_yyss1,
		   size * (unsigned int) sizeof (*ace_yyssp));
      ace_yyvs = (ACE_YYSTYPE *) ACE_YYSTACK_ALLOC (ace_yystacksize * sizeof (*ace_yyvsp));
      __ace_yy_memcpy ((ACE_TCHAR *)ace_yyvs, (ACE_TCHAR *)ace_yyvs1,
		   size * (unsigned int) sizeof (*ace_yyvsp));
#ifdef ACE_YYLSP_NEEDED
      ace_yyls = (ACE_YYLTYPE *) ACE_YYSTACK_ALLOC (ace_yystacksize * sizeof (*ace_yylsp));
      __ace_yy_memcpy ((ACE_TCHAR *)ace_yyls, (ACE_TCHAR *)ace_yyls1,
		   size * (unsigned int) sizeof (*ace_yylsp));
#endif
#endif /* no ace_yyoverflow */

      ace_yyssp = ace_yyss + size - 1;
      ace_yyvsp = ace_yyvs + size - 1;
#ifdef ACE_YYLSP_NEEDED
      ace_yylsp = ace_yyls + size - 1;
#endif

#if ACE_YYDEBUG != 0
      if (ace_yydebug)
	ACE_OS::fprintf(stderr, ACE_LIB_TEXT("Stack size increased to %d\n"), ace_yystacksize);
#endif

      if (ace_yyssp >= ace_yyss + ace_yystacksize - 1)
	ACE_YYABORT;
    }

#if ACE_YYDEBUG != 0
  if (ace_yydebug)
    ACE_OS::fprintf(stderr, ACE_LIB_TEXT("Entering state %d\n"), ace_yystate);
#endif

  goto ace_yybackup;
 ace_yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* ace_yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  ace_yyn = ace_yypact[ace_yystate];
  if (ace_yyn == ACE_YYFLAG)
    goto ace_yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* ace_yychar is either ACE_YYEMPTY or ACE_YYEOF
     or a valid token in external form.  */

  if (ace_yychar == ACE_YYEMPTY)
    {
#if ACE_YYDEBUG != 0
      if (ace_yydebug)
	ACE_OS::fprintf(stderr, ACE_LIB_TEXT("Reading a token: "));
#endif
      ace_yychar = ACE_YYLEX;
    }

  /* Convert token to internal form (in ace_yychar1) for indexing tables with */

  if (ace_yychar <= 0)		/* This means end of input. */
    {
      ace_yychar1 = 0;
      ace_yychar = ACE_YYEOF;		/* Don't call ACE_YYLEX any more */

#if ACE_YYDEBUG != 0
      if (ace_yydebug)
	ACE_OS::fprintf(stderr, ACE_LIB_TEXT("Now at end of input.\n"));
#endif
    }
  else
    {
      ace_yychar1 = ACE_YYTRANSLATE(ace_yychar);

#if ACE_YYDEBUG != 0
      if (ace_yydebug)
	{
	  ACE_OS::fprintf (stderr, ACE_LIB_TEXT("Next token is %d (%s"), ace_yychar, ace_yytname[ace_yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef ACE_YYPRINT
	  ACE_YYPRINT (stderr, ace_yychar, ace_yylval);
#endif
	  ACE_OS::fprintf (stderr, ACE_LIB_TEXT(")\n"));
	}
#endif
    }

  ace_yyn += ace_yychar1;
  if (ace_yyn < 0 || ace_yyn > ACE_YYLAST || ace_yycheck[ace_yyn] != ace_yychar1)
    goto ace_yydefault;

  ace_yyn = ace_yytable[ace_yyn];

  /* ace_yyn is what to do for this token type in this state.
     Negative => reduce, -ace_yyn is rule number.
     Positive => shift, ace_yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (ace_yyn < 0)
    {
      if (ace_yyn == ACE_YYFLAG)
	goto ace_yyerrlab;
      ace_yyn = -ace_yyn;
      goto ace_yyreduce;
    }
  else if (ace_yyn == 0)
    goto ace_yyerrlab;

  if (ace_yyn == ACE_YYFINAL)
    ACE_YYACCEPT;

  /* Shift the lookahead token.  */

#if ACE_YYDEBUG != 0
  if (ace_yydebug)
    ACE_OS::fprintf(stderr, ACE_LIB_TEXT("Shifting token %d (%s), "), ace_yychar, ace_yytname[ace_yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (ace_yychar != ACE_YYEOF)
    ace_yychar = ACE_YYEMPTY;

  *++ace_yyvsp = ace_yylval;
#ifdef ACE_YYLSP_NEEDED
  *++ace_yylsp = ace_yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (ace_yyerrstatus) ace_yyerrstatus--;

  ace_yystate = ace_yyn;
  goto ace_yynewstate;

/* Do the default action for the current state.  */
ace_yydefault:

  ace_yyn = ace_yydefact[ace_yystate];
  if (ace_yyn == 0)
    goto ace_yyerrlab;

/* Do a reduction.  ace_yyn is the number of a rule to reduce with.  */
ace_yyreduce:
  ace_yylen = ace_yyr2[ace_yyn];
  if (ace_yylen > 0)
    ace_yyval = ace_yyvsp[1-ace_yylen]; /* implement default value of the action */

#if ACE_YYDEBUG != 0
  if (ace_yydebug)
    {
      int i;

      ACE_OS::fprintf (stderr, ACE_LIB_TEXT("Reducing via rule %d (line %d), "),
	       ace_yyn, ace_yyrline[ace_yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = ace_yyprhs[ace_yyn]; ace_yyrhs[i] > 0; i++)
	ACE_OS::fprintf (stderr, ACE_LIB_TEXT("%s "), ace_yytname[ace_yyrhs[i]]);
      ACE_OS::fprintf (stderr, ACE_LIB_TEXT(" -> %s\n"), ace_yytname[ace_yyr1[ace_yyn]]);
    }
#endif


  switch (ace_yyn) {

case 1:
{
      if (ace_yyvsp[0].parse_node_ != 0)
      {
        ace_yyvsp[0].parse_node_->apply (); delete ace_yyvsp[0].parse_node_;
      }
      ACE_SVC_CONF_PARAM->obstack.release ();
    ;
    break;}
case 2:
{
      ACE_SVC_CONF_PARAM->obstack.release ();
    ;
    break;}
case 10:
{
      if (ace_yyvsp[-1].svc_record_ != 0)
        ace_yyval.parse_node_ = new ACE_Dynamic_Node (ace_yyvsp[-1].svc_record_, ace_yyvsp[0].ident_);
      else
        ace_yyval.parse_node_ = 0;
    ;
    break;}
case 11:
{
      ace_yyval.parse_node_ = new ACE_Static_Node (ace_yyvsp[-1].ident_, ace_yyvsp[0].ident_);
    ;
    break;}
case 12:
{
      ace_yyval.parse_node_ = new ACE_Suspend_Node (ace_yyvsp[0].ident_);
    ;
    break;}
case 13:
{
      ace_yyval.parse_node_ = new ACE_Resume_Node (ace_yyvsp[0].ident_);
    ;
    break;}
case 14:
{
      ace_yyval.parse_node_ = new ACE_Remove_Node (ace_yyvsp[0].ident_);
    ;
    break;}
case 15:
{
      ace_yyval.parse_node_ = new ACE_Stream_Node (ace_yyvsp[-1].static_node_, ace_yyvsp[0].parse_node_);
    ;
    break;}
case 16:
{ ace_yyval.static_node_ = new ACE_Static_Node (ace_yyvsp[0].ident_); ;
    break;}
case 17:
{
      ace_yyval.parse_node_ = new ACE_Dummy_Node (ace_yyvsp[-1].static_node_, ace_yyvsp[0].parse_node_);
    ;
    break;}
case 18:
{
    ;
    break;}
case 19:
{
    ;
    break;}
case 20:
{
      // Initialize left context...
      ace_yyval.static_node_ = ace_yyvsp[-1].static_node_;
    ;
    break;}
case 21:
{
      ace_yyval.parse_node_ = ace_yyvsp[-1].parse_node_;
    ;
    break;}
case 22:
{ ace_yyval.parse_node_ = 0; ;
    break;}
case 23:
{
      if (ace_yyvsp[0].parse_node_ != 0)
        {
          ace_yyvsp[0].parse_node_->link (ace_yyvsp[-1].parse_node_);
          ace_yyval.parse_node_ = ace_yyvsp[0].parse_node_;
        }
    ;
    break;}
case 24:
{ ace_yyval.parse_node_ = 0; ;
    break;}
case 25:
{
      ACE_Static_Node *svc_type = ace_yyvsp[0].static_node_;

      if (svc_type != 0)
        {
          ACE_Static_Node *module = ace_yyvsp[-2].static_node_;

          ACE_ARGV args (svc_type->parameters ());
          ACE_Module_Type *mt = ace_get_module (module,
                                                svc_type);
          ACE_Stream_Type *st =
            ACE_dynamic_cast (ACE_Stream_Type *,
                              ACE_const_cast (ACE_Service_Type_Impl *,
                                              module->record ()->type ()));

          if (mt->init (args.argc (), args.argv ()) == -1
              || st->push (mt) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_LIB_TEXT ("dynamic initialization failed for Module %s\n"),
                          svc_type->name ()));
              ACE_SVC_CONF_PARAM->yyerrno++;
            }
        }
    ;
    break;}
case 26:
{
      ACE_Module_Type *mt = ace_get_module (ace_yyvsp[-2].static_node_, ace_yyvsp[0].static_node_->name ());

      if (((ACE_Stream_Type *) (ace_yyvsp[-2].static_node_)->record ()->type ())->push (mt) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("Problem with static\n")));
          ACE_SVC_CONF_PARAM->yyerrno++;
        }
    ;
    break;}
case 27:
{
      ACE_Module_Type *mt = ace_get_module (ace_yyvsp[-2].static_node_,
                                            ace_yyvsp[0].static_node_->name ());
      if (mt != 0)
        mt->suspend ();
    ;
    break;}
case 28:
{
      ACE_Module_Type *mt = ace_get_module (ace_yyvsp[-2].static_node_,
                                            ace_yyvsp[0].static_node_->name ());
      if (mt != 0)
        mt->resume ();
    ;
    break;}
case 29:
{
      ACE_Static_Node *stream = ace_yyvsp[-2].static_node_;
      ACE_Static_Node *module = ace_yyvsp[0].static_node_;
      ACE_Module_Type *mt = ace_get_module (stream,
                                            module->name ());

      ACE_Stream_Type *st =
        ACE_dynamic_cast (ACE_Stream_Type *,
                          ACE_const_cast (ACE_Service_Type_Impl *,
                                          stream->record ()->type ()));
      if (mt != 0 && st->remove (mt) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("cannot remove Module_Type %s from STREAM_Type %s\n"),
                      module->name (),
                      stream->name ()));
          ACE_SVC_CONF_PARAM->yyerrno++;
        }
    ;
    break;}
case 30:
{
      u_int flags
        = ACE_Service_Type::DELETE_THIS
        | (ace_yyvsp[-1].location_node_->dispose () == 0 ? 0 : ACE_Service_Type::DELETE_OBJ);
      ACE_Service_Object_Exterminator gobbler = 0;
      void *sym = ace_yyvsp[-1].location_node_->symbol (&gobbler);

      if (sym != 0)
        {
          ACE_Service_Type_Impl *stp
            = ace_create_service_type (ace_yyvsp[-3].ident_,
                                       ace_yyvsp[-2].type_,
                                       sym,
                                       flags,
                                       gobbler);
          if (stp == 0)
            ace_yyerrno++;

          ace_yyval.svc_record_ = new ACE_Service_Type (ace_yyvsp[-3].ident_,
                                     stp,
                                     ace_yyvsp[-1].location_node_->handle (),
                                     ace_yyvsp[0].type_);
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_LIB_TEXT ("Unable to find service: %s\n"),
                      ace_yyvsp[-3].ident_));
          ++ACE_SVC_CONF_PARAM->yyerrno;
          ace_yyval.svc_record_ = 0;
        }
      delete ace_yyvsp[-1].location_node_;
    ;
    break;}
case 31:
{
      ace_yyval.type_ = 1;
    ;
    break;}
case 32:
{
      ace_yyval.type_ = 0;
    ;
    break;}
case 33:
{
      ace_yyval.type_ = 1;
    ;
    break;}
case 34:
{
      ace_yyval.location_node_ = new ACE_Object_Node (ace_yyvsp[-2].ident_, ace_yyvsp[0].ident_);
    ;
    break;}
case 35:
{
      ace_yyval.location_node_ = new ACE_Function_Node (ace_yyvsp[-4].ident_, ace_yyvsp[-2].ident_);
    ;
    break;}
case 36:
{
      ace_yyval.location_node_ = new ACE_Static_Function_Node (ace_yyvsp[-2].ident_);
    ;
    break;}
case 37:
{
      ace_yyval.type_ = ACE_MODULE_T;
    ;
    break;}
case 38:
{
      ace_yyval.type_ = ACE_SVC_OBJ_T;
    ;
    break;}
case 39:
{
      ace_yyval.type_ = ACE_STREAM_T;
    ;
    break;}
case 41:
{ ace_yyval.ident_ = 0; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */


  ace_yyvsp -= ace_yylen;
  ace_yyssp -= ace_yylen;
#ifdef ACE_YYLSP_NEEDED
  ace_yylsp -= ace_yylen;
#endif

#if ACE_YYDEBUG != 0
  if (ace_yydebug)
    {
      short *ssp1 = ace_yyss - 1;
      ACE_OS::fprintf (stderr, ACE_LIB_TEXT("state stack now"));
      while (ssp1 != ace_yyssp)
	ACE_OS::fprintf (stderr, ACE_LIB_TEXT(" %d"), *++ssp1);
      ACE_OS::fprintf (stderr, ACE_LIB_TEXT("\n"));
    }
#endif

  *++ace_yyvsp = ace_yyval;

#ifdef ACE_YYLSP_NEEDED
  ace_yylsp++;
  if (ace_yylen == 0)
    {
      ace_yylsp->first_line = ace_yylloc.first_line;
      ace_yylsp->first_column = ace_yylloc.first_column;
      ace_yylsp->last_line = (ace_yylsp-1)->last_line;
      ace_yylsp->last_column = (ace_yylsp-1)->last_column;
      ace_yylsp->text = 0;
    }
  else
    {
      ace_yylsp->last_line = (ace_yylsp+ace_yylen-1)->last_line;
      ace_yylsp->last_column = (ace_yylsp+ace_yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  ace_yyn = ace_yyr1[ace_yyn];

  ace_yystate = ace_yypgoto[ace_yyn - ACE_YYNTBASE] + *ace_yyssp;
  if (ace_yystate >= 0 && ace_yystate <= ACE_YYLAST && ace_yycheck[ace_yystate] == *ace_yyssp)
    ace_yystate = ace_yytable[ace_yystate];
  else
    ace_yystate = ace_yydefgoto[ace_yyn - ACE_YYNTBASE];

  goto ace_yynewstate;

ace_yyerrlab:   /* here on detecting error */

  if (! ace_yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++ace_yynerrs;

#ifdef ACE_YYERROR_VERBOSE
      ace_yyn = ace_yypact[ace_yystate];

      if (ace_yyn > ACE_YYFLAG && ace_yyn < ACE_YYLAST)
	{
	  int size = 0;
	  ACE_TCHAR *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -ace_yyn if nec to avoid negative indexes in ace_yycheck.  */
	  for (x = (ace_yyn < 0 ? -ace_yyn : 0);
	       x < (sizeof(ace_yytname) / sizeof(ACE_TCHAR *)); x++)
	    if (ace_yycheck[x + ace_yyn] == x)
	      size += ACE_OS::strlen(ace_yytname[x]) + 15, count++;
	  msg = new ACE_TCHAR[size + 15];
	  if (msg != 0)
	    {
	      ACE_OS::strcpy(msg, ACE_LIB_TEXT("parse error"));

	      if (count < 5)
		{
		  count = 0;
		  for (x = (ace_yyn < 0 ? -ace_yyn : 0);
		       x < (sizeof(ace_yytname) / sizeof(ACE_TCHAR *)); x++)
		    if (ace_yycheck[x + ace_yyn] == x)
		      {
			ACE_OS::strcat(msg, count == 0 ? ACE_LIB_TEXT(", expecting `") : ACE_LIB_TEXT(" or `"));
			ACE_OS::strcat(msg, ACE_TEXT_CHAR_TO_TCHAR (ace_yytname[x]));
			ACE_OS::strcat(msg, ACE_LIB_TEXT("'"));
			count++;
		      }
		}
	      ace_yyerror(msg);
	      delete [] msg;
	    }
	  else
	    ace_yyerror (ACE_LIB_TEXT("parse error; also virtual memory exceeded"));
	}
      else
#endif /* ACE_YYERROR_VERBOSE */
	ace_yyerror(ACE_LIB_TEXT("parse error"));
    }

  goto ace_yyerrlab1;
ace_yyerrlab1:   /* here on error raised explicitly by an action */

  if (ace_yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (ace_yychar == ACE_YYEOF)
	ACE_YYABORT;

#if ACE_YYDEBUG != 0
      if (ace_yydebug)
	ACE_OS::fprintf(stderr, ACE_LIB_TEXT("Discarding token %d (%s).\n"), ace_yychar, ace_yytname[ace_yychar1]);
#endif

      ace_yychar = ACE_YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  ace_yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto ace_yyerrhandle;

ace_yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  ace_yyn = ace_yydefact[ace_yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (ace_yyn) goto ace_yydefault;
#endif

ace_yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (ace_yyssp == ace_yyss) ACE_YYABORT;
  ace_yyvsp--;
  ace_yystate = *--ace_yyssp;
#ifdef ACE_YYLSP_NEEDED
  ace_yylsp--;
#endif

#if ACE_YYDEBUG != 0
  if (ace_yydebug)
    {
      short *ssp1 = ace_yyss - 1;
      ACE_OS::fprintf (stderr, ACE_LIB_TEXT("Error: state stack now"));
      while (ssp1 != ace_yyssp)
	ACE_OS::fprintf (stderr, ACE_LIB_TEXT(" %d"), *++ssp1);
      ACE_OS::fprintf (stderr, ACE_LIB_TEXT("\n"));
    }
#endif

ace_yyerrhandle:

  ace_yyn = ace_yypact[ace_yystate];
  if (ace_yyn == ACE_YYFLAG)
    goto ace_yyerrdefault;

  ace_yyn += ACE_YYTERROR;
  if (ace_yyn < 0 || ace_yyn > ACE_YYLAST || ace_yycheck[ace_yyn] != ACE_YYTERROR)
    goto ace_yyerrdefault;

  ace_yyn = ace_yytable[ace_yyn];
  if (ace_yyn < 0)
    {
      if (ace_yyn == ACE_YYFLAG)
	goto ace_yyerrpop;
      ace_yyn = -ace_yyn;
      goto ace_yyreduce;
    }
  else if (ace_yyn == 0)
    goto ace_yyerrpop;

  if (ace_yyn == ACE_YYFINAL)
    ACE_YYACCEPT;

#if ACE_YYDEBUG != 0
  if (ace_yydebug)
    ACE_OS::fprintf(stderr, ACE_LIB_TEXT("Shifting error token, "));
#endif

  *++ace_yyvsp = ace_yylval;
#ifdef ACE_YYLSP_NEEDED
  *++ace_yylsp = ace_yylloc;
#endif

  ace_yystate = ace_yyn;
  goto ace_yynewstate;

 ace_yyacceptlab:
  /* ACE_YYACCEPT comes here.  */
  if (ace_yyfree_stacks)
    {
      free (ace_yyss);
      free (ace_yyvs);
#ifdef ACE_YYLSP_NEEDED
      free (ace_yyls);
#endif
    }
  return 0;

 ace_yyabortlab:
  /* ACE_YYABORT comes here.  */
  if (ace_yyfree_stacks)
    {
      free (ace_yyss);
      free (ace_yyvs);
#ifdef ACE_YYLSP_NEEDED
      free (ace_yyls);
#endif
    }
  return 1;
}

// Prints the error string to standard output.  Cleans up the error
// messages.

void
ace_yyerror (const ACE_TCHAR *s)
{
#if defined (ACE_NLOGGING)
  ACE_UNUSED_ARG (s);
#endif /* ACE_NLOGGING */

  ACE_ERROR ((LM_ERROR,
              ACE_LIB_TEXT ("[error %d] on line %d: %s\n"),
              ace_yyerrno,
              ace_yylineno,
              s));
}

// Note that SRC_REC represents left context, which is the STREAM *
// record.

static ACE_Module_Type *
ace_get_module (ACE_Static_Node *str_rec,
                const ACE_TCHAR *svc_name)
{
  const ACE_Service_Type *sr = str_rec->record ();
  const ACE_Service_Type_Impl *type = sr->type ();
  ACE_Stream_Type *st = sr == 0
    ? 0
    : ACE_dynamic_cast (ACE_Stream_Type *,
                        ACE_const_cast (ACE_Service_Type_Impl *,
                                        type));
  ACE_Module_Type *mt = st == 0 ? 0 : st->find (svc_name);

  if (sr == 0 || st == 0 || mt == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("cannot locate Module_Type %s in STREAM_Type %s\n"),
                  svc_name,
                  str_rec->name ()));
      ace_yyerrno++;
    }

  return mt;
}

static ACE_Module_Type *
ace_get_module (ACE_Static_Node *str_rec,
                ACE_Static_Node *svc_type)
{
  const ACE_Service_Type *sr = str_rec->record ();
  const ACE_Service_Type_Impl *type = sr->type ();
  ACE_Stream_Type *st = sr == 0 ? 0 : (ACE_Stream_Type *) type;
  const ACE_Service_Type *sv = svc_type->record ();
  type = sv->type ();
  ACE_Module_Type *mt = (ACE_Module_Type *) type;
  const ACE_TCHAR *module_type_name = svc_type->name ();

  if (sr == 0 || st == 0 || mt == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("cannot locate Module_Type %s or STREAM_Type %s\n"),
                  module_type_name,
                  str_rec->name ()));
      ace_yyerrno++;
    }

  // Make sure that the Module has the same name as the
  // Module_Type object from the svc.conf file.
  ACE_Module<ACE_SYNCH> *mp = (ACE_Module<ACE_SYNCH> *) mt->object ();

  if (ACE_OS::strcmp (mp->name (), module_type_name) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("warning: assigning Module_Type name %s to Module %s since names differ\n"),
                  module_type_name,
                  mp->name ()));
      mp->name (module_type_name);
    }

  return mt;
}

#if defined (DEBUGGING)
// Current line number.
int ace_yylineno = 1;

// Name given on the command-line to envoke the program.
ACE_TCHAR *program_name;

// Main driver program.

int
main (int argc, ACE_TCHAR *argv[])
{
  ACE_Svc_Conf_Param param (stdin);

  // Try to reopen any filename argument to use ACE_YYIN.
  if (argc > 1 && (ace_yyin = freopen (argv[1], ACE_LIB_TEXT("r"), stdin)) == 0)
    ACE_OS::fprintf (stderr, ACE_LIB_TEXT("usage: %s [file]\n"), argv[0]), ACE_OS::exit (1);

  return ace_yyparse (&param);
}
#endif /* DEBUGGING */
#endif /* ACE_HAS_LEGACY_SERVICE_CONFIG */

ACE_Service_Type_Impl *
ace_create_service_type (const ACE_TCHAR *name,
                         int type,
                         void *symbol,
                         u_int flags,
                         ACE_Service_Object_Exterminator gobbler)
{
  ACE_Service_Type_Impl *stp = 0;

  // Note, the only place we need to put a case statement.  This is
  // also the place where we'd put the RTTI tests, if the compiler
  // actually supported them!

  switch (type)
    {
    case ACE_SVC_OBJ_T:
      ACE_NEW_RETURN (stp,
                      ACE_Service_Object_Type ((ACE_Service_Object *) symbol,
                                               name, flags,
                                               gobbler),
                      0);
      break;
    case ACE_MODULE_T:
      ACE_NEW_RETURN (stp,
                      ACE_Module_Type (symbol, name, flags),
                      0);
      break;
    case ACE_STREAM_T:
      ACE_NEW_RETURN (stp,
                      ACE_Stream_Type (symbol, name, flags),
                      0);
      break;
    default:
      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("unknown case\n")));
      break;
    }
  return stp;
}
