// $Id$

/*  A Bison parser, made from CC_command.y
    by GNU Bison version 1.28  */

#define ACE_CC_YYBISON 1  /* Identify Bison output.  */

#define T_START_CMD 257
#define T_CREATE_CMD  258
#define T_LOCK_CMD  259
#define T_UNLOCK_CMD  260
#define T_TRYLOCK_CMD 261
#define T_CHANGEMODE_CMD  262
#define T_SLEEP_CMD 263
#define T_WAIT_CMD  264
#define T_REPEAT_CMD  265
#define T_EXCEP_CMD 266
#define T_PRINT_CMD 267
#define T_LOOKUP_CMD  268
#define T_TERM  269
#define T_READ  270
#define T_IREAD 271
#define T_UPGRADE 272
#define T_WRITE 273
#define T_IWRITE  274
#define T_IDENT 275
#define Q_STRING  276
#define T_NUM 277


#include "CC_command.h"
#include "ace/ACE.h"
#include "orbsvcs/CosConcurrencyControlC.h"
#include "ace/OS_NS_stdio.h"

void ace_cc_yyerror (const char* s);
int ace_cc_yylex(void);

extern int line_no;
extern char line_buf[500];
extern char *ace_cc_yytext;
extern CC_CommandList *cmdlist;

typedef union {
  CC_Command *command;
  CosConcurrencyControl::lock_mode lock_mode;
  char *id;
  int num;
} ACE_CC_YYSTYPE;
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define ACE_CC_YYFINAL    66
#define ACE_CC_YYFLAG   -32768
#define ACE_CC_YYNTBASE 24

#define ACE_CC_YYTRANSLATE(x) ((unsigned)(x) <= 277 ? ace_cc_yytranslate[x] : 31)

static const char ace_cc_yytranslate[] = {     0,
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
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23
};

#if ACE_CC_YYDEBUG != 0
static const short ace_cc_yyprhs[] = {     0,
     0,     1,     3,     6,     8,    12,    16,    19,    24,    28,
    33,    37,    42,    46,    52,    57,    61,    64,    68,    72,
    76,    80,    84,    86,    88,    90,    92,    94,    96,    98
};

static const short ace_cc_yyrhs[] = {    -1,
    25,     0,    25,    26,     0,    26,     0,     3,    27,    15,
     0,     4,    28,    15,     0,     4,    15,     0,     5,    28,
    30,    15,     0,     5,    30,    15,     0,     6,    28,    30,
    15,     0,     6,    30,    15,     0,     7,    28,    30,    15,
     0,     7,    30,    15,     0,     8,    28,    30,    30,    15,
     0,     8,    30,    30,    15,     0,     9,    23,    15,     0,
    10,    15,     0,    10,    22,    15,     0,    13,    22,    15,
     0,    14,    28,    15,     0,    11,    23,    15,     0,    12,
    29,    15,     0,    21,     0,    21,     0,    21,     0,    16,
     0,    17,     0,    18,     0,    19,     0,    20,     0
};

#endif

#if ACE_CC_YYDEBUG != 0
static const short ace_cc_yyrline[] = { 0,
    51,    52,    60,    61,    64,    66,    68,    70,    72,    74,
    76,    78,    80,    82,    84,    86,    88,    90,    92,    94,
    96,    99,   103,   105,   107,   109,   110,   111,   112,   113
};
#endif


#if ACE_CC_YYDEBUG != 0 || defined (ACE_CC_YYERROR_VERBOSE)

static const char * const ace_cc_yytname[] = {   "$","error","$undefined.","T_START_CMD",
"T_CREATE_CMD","T_LOCK_CMD","T_UNLOCK_CMD","T_TRYLOCK_CMD","T_CHANGEMODE_CMD",
"T_SLEEP_CMD","T_WAIT_CMD","T_REPEAT_CMD","T_EXCEP_CMD","T_PRINT_CMD","T_LOOKUP_CMD",
"T_TERM","T_READ","T_IREAD","T_UPGRADE","T_WRITE","T_IWRITE","T_IDENT","Q_STRING",
"T_NUM","start","cmd_list","cmd","config_file_name","lock_set_name","exception_name",
"lock_mode", 0
};
#endif

static const short ace_cc_yyr1[] = {     0,
    24,    24,    25,    25,    26,    26,    26,    26,    26,    26,
    26,    26,    26,    26,    26,    26,    26,    26,    26,    26,
    26,    26,    27,    28,    29,    30,    30,    30,    30,    30
};

static const short ace_cc_yyr2[] = {     0,
     0,     1,     2,     1,     3,     3,     2,     4,     3,     4,
     3,     4,     3,     5,     4,     3,     2,     3,     3,     3,
     3,     3,     1,     1,     1,     1,     1,     1,     1,     1
};

static const short ace_cc_yydefact[] = {     1,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     2,     4,    23,     0,     7,    24,     0,    26,
    27,    28,    29,    30,     0,     0,     0,     0,     0,     0,
     0,     0,     0,    17,     0,     0,    25,     0,     0,     0,
     3,     5,     6,     0,     9,     0,    11,     0,    13,     0,
     0,    16,    18,    21,    22,    19,    20,     8,    10,    12,
     0,    15,    14,     0,     0,     0
};

static const short ace_cc_yydefgoto[] = {    64,
    13,    14,    16,    19,    38,    26
};

static const short ace_cc_yypact[] = {     0,
    -1,    14,    20,    20,    20,    20,     7,     4,     8,     1,
    10,    13,     0,-32768,-32768,    18,-32768,-32768,    27,-32768,
-32768,-32768,-32768,-32768,    31,    28,    31,    29,    31,    30,
    31,    31,    37,-32768,    38,    39,-32768,    40,    41,    42,
-32768,-32768,-32768,    43,-32768,    44,-32768,    45,-32768,    31,
    46,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
    47,-32768,-32768,    63,    64,-32768
};

static const short ace_cc_yypgoto[] = {-32768,
-32768,    52,-32768,    12,-32768,    -4
};


#define ACE_CC_YYLAST   65


static const short ace_cc_yytable[] = {    28,
    30,    32,     1,     2,     3,     4,     5,     6,     7,     8,
     9,    10,    11,    12,    25,    27,    29,    31,    34,    15,
    44,    37,    46,    40,    48,    35,    50,    51,    17,    33,
    36,    39,    42,    18,    18,    20,    21,    22,    23,    24,
    18,    43,    45,    47,    49,    61,    20,    21,    22,    23,
    24,    52,    53,    54,    55,    56,    57,    58,    59,    60,
    62,    63,    65,    66,    41
};

static const short ace_cc_yycheck[] = {     4,
     5,     6,     3,     4,     5,     6,     7,     8,     9,    10,
    11,    12,    13,    14,     3,     4,     5,     6,    15,    21,
    25,    21,    27,    12,    29,    22,    31,    32,    15,    23,
    23,    22,    15,    21,    21,    16,    17,    18,    19,    20,
    21,    15,    15,    15,    15,    50,    16,    17,    18,    19,
    20,    15,    15,    15,    15,    15,    15,    15,    15,    15,
    15,    15,     0,     0,    13
};
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

#ifndef ACE_CC_YYSTACK_USE_ALLOCA
#ifdef alloca
#define ACE_CC_YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define ACE_CC_YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || (defined (__sun) && defined (__i386))
#define ACE_CC_YYSTACK_USE_ALLOCA
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
#define ACE_CC_YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
     and on HPUX 10.  Eventually we can turn this on.  */
#define ACE_CC_YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* ACE_CC_YYSTACK_USE_ALLOCA not defined */

#ifdef ACE_CC_YYSTACK_USE_ALLOCA
#define ACE_CC_YYSTACK_ALLOC alloca
#else
#define ACE_CC_YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define ace_cc_yyerrok    (ace_cc_yyerrstatus = 0)
#define ace_cc_yyclearin  (ace_cc_yychar = ACE_CC_YYEMPTY)
#define ACE_CC_YYEMPTY    -2
#define ACE_CC_YYEOF    0
#define ACE_CC_YYACCEPT goto ace_cc_yyacceptlab
#define ACE_CC_YYABORT  goto ace_cc_yyabortlab
#define ACE_CC_YYERROR    goto ace_cc_yyerrlab1
/* Like ACE_CC_YYERROR except do call ace_cc_yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of ACE_CC_YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define ACE_CC_YYFAIL   goto ace_cc_yyerrlab
#define ACE_CC_YYRECOVERING()  (!!ace_cc_yyerrstatus)
#define ACE_CC_YYBACKUP(token, value) \
do                \
  if (ace_cc_yychar == ACE_CC_YYEMPTY && ace_cc_yylen == 1)       \
    { ace_cc_yychar = (token), ace_cc_yylval = (value);     \
      ace_cc_yychar1 = ACE_CC_YYTRANSLATE (ace_cc_yychar);        \
      ACE_CC_YYPOPSTACK;            \
      goto ace_cc_yybackup;           \
    }               \
  else                \
    { ace_cc_yyerror ("syntax error: cannot back up"); ACE_CC_YYERROR; }  \
while (0)

#define ACE_CC_YYTERROR 1
#define ACE_CC_YYERRCODE  256

#ifndef ACE_CC_YYPURE
#define ACE_CC_YYLEX    ace_cc_yylex()
#endif

#ifdef ACE_CC_YYPURE
#ifdef ACE_CC_YYLSP_NEEDED
#ifdef ACE_CC_YYLEX_PARAM
#define ACE_CC_YYLEX    ace_cc_yylex(&ace_cc_yylval, &ace_cc_yylloc, ACE_CC_YYLEX_PARAM)
#else
#define ACE_CC_YYLEX    ace_cc_yylex(&ace_cc_yylval, &ace_cc_yylloc)
#endif
#else /* not ACE_CC_YYLSP_NEEDED */
#ifdef ACE_CC_YYLEX_PARAM
#define ACE_CC_YYLEX    ace_cc_yylex(&ace_cc_yylval, ACE_CC_YYLEX_PARAM)
#else
#define ACE_CC_YYLEX    ace_cc_yylex(&ace_cc_yylval)
#endif
#endif /* not ACE_CC_YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef ACE_CC_YYPURE

int ace_cc_yychar;      /*  the lookahead symbol    */
ACE_CC_YYSTYPE  ace_cc_yylval;      /*  the semantic value of the   */
        /*  lookahead symbol      */

#ifdef ACE_CC_YYLSP_NEEDED
ACE_CC_YYLTYPE ace_cc_yylloc;     /*  location data for the lookahead */
        /*  symbol        */
#endif

int ace_cc_yynerrs;     /*  number of parse errors so far       */
#endif  /* not ACE_CC_YYPURE */

#if ACE_CC_YYDEBUG != 0
int ace_cc_yydebug;     /*  nonzero means print parse trace */
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  ACE_CC_YYINITDEPTH indicates the initial size of the parser's stacks  */

#ifndef ACE_CC_YYINITDEPTH
#define ACE_CC_YYINITDEPTH 200
#endif

/*  ACE_CC_YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if ACE_CC_YYMAXDEPTH == 0
#undef ACE_CC_YYMAXDEPTH
#endif

#ifndef ACE_CC_YYMAXDEPTH
#define ACE_CC_YYMAXDEPTH 10000
#endif

/* Define __ace_cc_yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1    /* GNU C and GNU C++ define this.  */
#define __ace_cc_yy_memcpy(TO,FROM,COUNT) __builtin_memcpy(TO,FROM,COUNT)
#else       /* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__ace_cc_yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__ace_cc_yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif



/* The user can define ACE_CC_YYPARSE_PARAM as the name of an argument to be passed
   into ace_cc_yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef ACE_CC_YYPARSE_PARAM
#ifdef __cplusplus
#define ACE_CC_YYPARSE_PARAM_ARG void *ACE_CC_YYPARSE_PARAM
#define ACE_CC_YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define ACE_CC_YYPARSE_PARAM_ARG ACE_CC_YYPARSE_PARAM
#define ACE_CC_YYPARSE_PARAM_DECL void *ACE_CC_YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not ACE_CC_YYPARSE_PARAM */
#define ACE_CC_YYPARSE_PARAM_ARG
#define ACE_CC_YYPARSE_PARAM_DECL
#endif /* not ACE_CC_YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef ACE_CC_YYPARSE_PARAM
int ace_cc_yyparse (void *);
#else
int ace_cc_yyparse (void);
#endif
#endif

int
ace_cc_yyparse(ACE_CC_YYPARSE_PARAM_ARG)
     ACE_CC_YYPARSE_PARAM_DECL
{
  register int ace_cc_yystate;
  register int ace_cc_yyn;
  register short *ace_cc_yyssp;
  register ACE_CC_YYSTYPE *ace_cc_yyvsp;
  int ace_cc_yyerrstatus; /*  number of tokens to shift before error messages enabled */
  int ace_cc_yychar1 = 0;   /*  lookahead token as an internal (translated) token number */

  short ace_cc_yyssa[ACE_CC_YYINITDEPTH]; /*  the state stack     */
  ACE_CC_YYSTYPE ace_cc_yyvsa[ACE_CC_YYINITDEPTH];  /*  the semantic value stack    */

  short *ace_cc_yyss = ace_cc_yyssa;    /*  refer to the stacks thru separate pointers */
  ACE_CC_YYSTYPE *ace_cc_yyvs = ace_cc_yyvsa; /*  to allow ace_cc_yyoverflow to reallocate them elsewhere */

#ifdef ACE_CC_YYLSP_NEEDED
  ACE_CC_YYLTYPE ace_cc_yylsa[ACE_CC_YYINITDEPTH];  /*  the location stack      */
  ACE_CC_YYLTYPE *ace_cc_yyls = ace_cc_yylsa;
  ACE_CC_YYLTYPE *ace_cc_yylsp;

#define ACE_CC_YYPOPSTACK   (ace_cc_yyvsp--, ace_cc_yyssp--, ace_cc_yylsp--)
#else
#define ACE_CC_YYPOPSTACK   (ace_cc_yyvsp--, ace_cc_yyssp--)
#endif

  int ace_cc_yystacksize = ACE_CC_YYINITDEPTH;
  int ace_cc_yyfree_stacks = 0;

#ifdef ACE_CC_YYPURE
  int ace_cc_yychar;
  ACE_CC_YYSTYPE ace_cc_yylval;
  int ace_cc_yynerrs;
#ifdef ACE_CC_YYLSP_NEEDED
  ACE_CC_YYLTYPE ace_cc_yylloc;
#endif
#endif

  ACE_CC_YYSTYPE ace_cc_yyval;    /*  the variable used to return   */
        /*  semantic values from the action */
        /*  routines        */

  int ace_cc_yylen;

#if ACE_CC_YYDEBUG != 0
  if (ace_cc_yydebug)
    ACE_OS::fprintf(stderr, "Starting parse\n");
#endif

  ace_cc_yystate = 0;
  ace_cc_yyerrstatus = 0;
  ace_cc_yynerrs = 0;
  ace_cc_yychar = ACE_CC_YYEMPTY;   /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  ace_cc_yyssp = ace_cc_yyss - 1;
  ace_cc_yyvsp = ace_cc_yyvs;
#ifdef ACE_CC_YYLSP_NEEDED
  ace_cc_yylsp = ace_cc_yyls;
#endif

/* Push a new state, which is found in  ace_cc_yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
ace_cc_yynewstate:

  *++ace_cc_yyssp = ace_cc_yystate;

  if (ace_cc_yyssp >= ace_cc_yyss + ace_cc_yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      ACE_CC_YYSTYPE *ace_cc_yyvs1 = ace_cc_yyvs;
      short *ace_cc_yyss1 = ace_cc_yyss;
#ifdef ACE_CC_YYLSP_NEEDED
      ACE_CC_YYLTYPE *ace_cc_yyls1 = ace_cc_yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = ace_cc_yyssp - ace_cc_yyss + 1;

#ifdef ace_cc_yyoverflow
      /* Each stack pointer address is followed by the size of
   the data in use in that stack, in bytes.  */
#ifdef ACE_CC_YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
   but that might be undefined if ace_cc_yyoverflow is a macro.  */
      ace_cc_yyoverflow("parser stack overflow",
     &ace_cc_yyss1, size * sizeof (*ace_cc_yyssp),
     &ace_cc_yyvs1, size * sizeof (*ace_cc_yyvsp),
     &ace_cc_yyls1, size * sizeof (*ace_cc_yylsp),
     &ace_cc_yystacksize);
#else
      ace_cc_yyoverflow("parser stack overflow",
     &ace_cc_yyss1, size * sizeof (*ace_cc_yyssp),
     &ace_cc_yyvs1, size * sizeof (*ace_cc_yyvsp),
     &ace_cc_yystacksize);
#endif

      ace_cc_yyss = ace_cc_yyss1; ace_cc_yyvs = ace_cc_yyvs1;
#ifdef ACE_CC_YYLSP_NEEDED
      ace_cc_yyls = ace_cc_yyls1;
#endif
#else /* no ace_cc_yyoverflow */
      /* Extend the stack our own way.  */
      if (ace_cc_yystacksize >= ACE_CC_YYMAXDEPTH)
  {
    ace_cc_yyerror("parser stack overflow");
    if (ace_cc_yyfree_stacks)
      {
        ACE_OS::free (ace_cc_yyss);
        ACE_OS::free (ace_cc_yyvs);
#ifdef ACE_CC_YYLSP_NEEDED
        ACE_OS::free (ace_cc_yyls);
#endif
      }
    return 2;
  }
      ace_cc_yystacksize *= 2;
      if (ace_cc_yystacksize > ACE_CC_YYMAXDEPTH)
  ace_cc_yystacksize = ACE_CC_YYMAXDEPTH;
#ifndef ACE_CC_YYSTACK_USE_ALLOCA
      ace_cc_yyfree_stacks = 1;
#endif
      ace_cc_yyss = (short *) ACE_CC_YYSTACK_ALLOC (ace_cc_yystacksize * sizeof (*ace_cc_yyssp));
      __ace_cc_yy_memcpy ((char *)ace_cc_yyss, (char *)ace_cc_yyss1,
       size * (unsigned int) sizeof (*ace_cc_yyssp));
      ace_cc_yyvs = (ACE_CC_YYSTYPE *) ACE_CC_YYSTACK_ALLOC (ace_cc_yystacksize * sizeof (*ace_cc_yyvsp));
      __ace_cc_yy_memcpy ((char *)ace_cc_yyvs, (char *)ace_cc_yyvs1,
       size * (unsigned int) sizeof (*ace_cc_yyvsp));
#ifdef ACE_CC_YYLSP_NEEDED
      ace_cc_yyls = (ACE_CC_YYLTYPE *) ACE_CC_YYSTACK_ALLOC (ace_cc_yystacksize * sizeof (*ace_cc_yylsp));
      __ace_cc_yy_memcpy ((char *)ace_cc_yyls, (char *)ace_cc_yyls1,
       size * (unsigned int) sizeof (*ace_cc_yylsp));
#endif
#endif /* no ace_cc_yyoverflow */

      ace_cc_yyssp = ace_cc_yyss + size - 1;
      ace_cc_yyvsp = ace_cc_yyvs + size - 1;
#ifdef ACE_CC_YYLSP_NEEDED
      ace_cc_yylsp = ace_cc_yyls + size - 1;
#endif

#if ACE_CC_YYDEBUG != 0
      if (ace_cc_yydebug)
        ACE_OS::fprintf(stderr, "Stack size increased to %d\n", ace_cc_yystacksize);
#endif

      if (ace_cc_yyssp >= ace_cc_yyss + ace_cc_yystacksize - 1)
  ACE_CC_YYABORT;
    }

#if ACE_CC_YYDEBUG != 0
  if (ace_cc_yydebug)
    ACE_OS::fprintf(stderr, "Entering state %d\n", ace_cc_yystate);
#endif

  goto ace_cc_yybackup;
 ace_cc_yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* ace_cc_yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  ace_cc_yyn = ace_cc_yypact[ace_cc_yystate];
  if (ace_cc_yyn == ACE_CC_YYFLAG)
    goto ace_cc_yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* ace_cc_yychar is either ACE_CC_YYEMPTY or ACE_CC_YYEOF
     or a valid token in external form.  */

  if (ace_cc_yychar == ACE_CC_YYEMPTY)
    {
#if ACE_CC_YYDEBUG != 0
      if (ace_cc_yydebug)
        ACE_OS::fprintf(stderr, "Reading a token: ");
#endif
      ace_cc_yychar = ACE_CC_YYLEX;
    }

  /* Convert token to internal form (in ace_cc_yychar1) for indexing tables with */

  if (ace_cc_yychar <= 0)   /* This means end of input. */
    {
      ace_cc_yychar1 = 0;
      ace_cc_yychar = ACE_CC_YYEOF;   /* Don't call ACE_CC_YYLEX any more */

#if ACE_CC_YYDEBUG != 0
      if (ace_cc_yydebug)
        ACE_OS::fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      ace_cc_yychar1 = ACE_CC_YYTRANSLATE(ace_cc_yychar);

#if ACE_CC_YYDEBUG != 0
      if (ace_cc_yydebug)
  {
    ACE_OS::fprintf (stderr, "Next token is %d (%s", ace_cc_yychar, ace_cc_yytname[ace_cc_yychar1]);
    /* Give the individual parser a way to print the precise meaning
       of a token, for further debugging info.  */
#ifdef ACE_CC_YYPRINT
    ACE_CC_YYPRINT (stderr, ace_cc_yychar, ace_cc_yylval);
#endif
    ACE_OS::fprintf (stderr, ")\n");
  }
#endif
    }

  ace_cc_yyn += ace_cc_yychar1;
  if (ace_cc_yyn < 0 || ace_cc_yyn > ACE_CC_YYLAST || ace_cc_yycheck[ace_cc_yyn] != ace_cc_yychar1)
    goto ace_cc_yydefault;

  ace_cc_yyn = ace_cc_yytable[ace_cc_yyn];

  /* ace_cc_yyn is what to do for this token type in this state.
     Negative => reduce, -ace_cc_yyn is rule number.
     Positive => shift, ace_cc_yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (ace_cc_yyn < 0)
    {
      if (ace_cc_yyn == ACE_CC_YYFLAG)
  goto ace_cc_yyerrlab;
      ace_cc_yyn = -ace_cc_yyn;
      goto ace_cc_yyreduce;
    }
  else if (ace_cc_yyn == 0)
    goto ace_cc_yyerrlab;

  if (ace_cc_yyn == ACE_CC_YYFINAL)
    ACE_CC_YYACCEPT;

  /* Shift the lookahead token.  */

#if ACE_CC_YYDEBUG != 0
  if (ace_cc_yydebug)
    ACE_OS::fprintf(stderr, "Shifting token %d (%s), ", ace_cc_yychar, ace_cc_yytname[ace_cc_yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (ace_cc_yychar != ACE_CC_YYEOF)
    ace_cc_yychar = ACE_CC_YYEMPTY;

  *++ace_cc_yyvsp = ace_cc_yylval;
#ifdef ACE_CC_YYLSP_NEEDED
  *++ace_cc_yylsp = ace_cc_yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (ace_cc_yyerrstatus) ace_cc_yyerrstatus--;

  ace_cc_yystate = ace_cc_yyn;
  goto ace_cc_yynewstate;

/* Do the default action for the current state.  */
ace_cc_yydefault:

  ace_cc_yyn = ace_cc_yydefact[ace_cc_yystate];
  if (ace_cc_yyn == 0)
    goto ace_cc_yyerrlab;

/* Do a reduction.  ace_cc_yyn is the number of a rule to reduce with.  */
ace_cc_yyreduce:
  ace_cc_yylen = ace_cc_yyr2[ace_cc_yyn];
  if (ace_cc_yylen > 0)
    ace_cc_yyval = ace_cc_yyvsp[1-ace_cc_yylen]; /* implement default value of the action */

#if ACE_CC_YYDEBUG != 0
  if (ace_cc_yydebug)
    {
      int i;

      ACE_OS::fprintf (stderr, "Reducing via rule %d (line %d), ",
                       ace_cc_yyn, ace_cc_yyrline[ace_cc_yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = ace_cc_yyprhs[ace_cc_yyn]; ace_cc_yyrhs[i] > 0; i++)
        ACE_OS::fprintf (stderr, "%s ", ace_cc_yytname[ace_cc_yyrhs[i]]);
      ACE_OS::fprintf (stderr, " -> %s\n", ace_cc_yytname[ace_cc_yyr1[ace_cc_yyn]]);
    }
#endif


  switch (ace_cc_yyn) {

case 2:
{ cmdlist->add(new CC_Sleep_Cmd(0));
         /* dummy to check exeption in the last command */
      if(cmdlist->execute()!=0) //CC_FAIL
                    { ACE_OS::printf(" ** Test succeded!!\n"); }
                  else
                    { ACE_OS::printf(" ** Test FAILED!!\n"); } ;
    break;}
case 3:
{ cmdlist->add(ace_cc_yyvsp[0].command); ;
    break;}
case 4:
{ cmdlist->add(ace_cc_yyvsp[0].command); ;
    break;}
case 5:
{ ace_cc_yyval.command = new CC_Start_Cmd(ace_cc_yyvsp[-1].id); ;
    break;}
case 6:
{ ace_cc_yyval.command = new CC_CreateLockSet_Cmd(ace_cc_yyvsp[-1].id); ;
    break;}
case 7:
{ ace_cc_yyval.command = new CC_CreateLockSet_Cmd(""); ;
    break;}
case 8:
{ ace_cc_yyval.command = new CC_Lock_Cmd(ace_cc_yyvsp[-2].id, ace_cc_yyvsp[-1].lock_mode); ;
    break;}
case 9:
{ ace_cc_yyval.command = new CC_Lock_Cmd("", ace_cc_yyvsp[-1].lock_mode); ;
    break;}
case 10:
{ ace_cc_yyval.command = new CC_UnLock_Cmd(ace_cc_yyvsp[-2].id, ace_cc_yyvsp[-1].lock_mode); ;
    break;}
case 11:
{ ace_cc_yyval.command = new CC_UnLock_Cmd("", ace_cc_yyvsp[-1].lock_mode); ;
    break;}
case 12:
{ ace_cc_yyval.command = new CC_TryLock_Cmd(ace_cc_yyvsp[-2].id, ace_cc_yyvsp[-1].lock_mode); ;
    break;}
case 13:
{ ace_cc_yyval.command = new CC_TryLock_Cmd("", ace_cc_yyvsp[-1].lock_mode); ;
    break;}
case 14:
{ ace_cc_yyval.command = new CC_ChangeMode_Cmd(ace_cc_yyvsp[-3].id, ace_cc_yyvsp[-2].lock_mode, ace_cc_yyvsp[-1].lock_mode); ;
    break;}
case 15:
{ ace_cc_yyval.command = new CC_ChangeMode_Cmd("", ace_cc_yyvsp[-2].lock_mode, ace_cc_yyvsp[-1].lock_mode); ;
    break;}
case 16:
{ ace_cc_yyval.command = new CC_Sleep_Cmd(ace_cc_yyvsp[-1].num); ;
    break;}
case 17:
{ ace_cc_yyval.command = new CC_Wait_Cmd(""); ;
    break;}
case 18:
{ ace_cc_yyval.command = new CC_Wait_Cmd(ace_cc_yyvsp[-1].id); ;
    break;}
case 19:
{ ace_cc_yyval.command = new CC_Print_Cmd(ace_cc_yyvsp[-1].id); ;
    break;}
case 20:
{ ace_cc_yyval.command = new CC_Lookup_Cmd(ace_cc_yyvsp[-1].id); ;
    break;}
case 21:
{ cmdlist->setrepeat(ace_cc_yyvsp[-1].num);
                          ace_cc_yyval.command = new CC_Dummy_Cmd();;
    break;}
case 22:
{ ace_cc_yyval.command = new CC_Excep_Cmd(ace_cc_yyvsp[-1].id); ;
    break;}
case 23:
{ace_cc_yyval.id = ace_cc_yyvsp[0].id; ;
    break;}
case 24:
{ace_cc_yyval.id = ace_cc_yyvsp[0].id; ;
    break;}
case 25:
{ace_cc_yyval.id = ace_cc_yyvsp[0].id; ;
    break;}
case 26:
{ ace_cc_yyval.lock_mode = CosConcurrencyControl::read; ;
    break;}
case 27:
{ ace_cc_yyval.lock_mode = CosConcurrencyControl::intention_read; ;
    break;}
case 28:
{ ace_cc_yyval.lock_mode = CosConcurrencyControl::upgrade; ;
    break;}
case 29:
{ ace_cc_yyval.lock_mode = CosConcurrencyControl::write; ;
    break;}
case 30:
{ ace_cc_yyval.lock_mode = CosConcurrencyControl::intention_write; ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */


  ace_cc_yyvsp -= ace_cc_yylen;
  ace_cc_yyssp -= ace_cc_yylen;
#ifdef ACE_CC_YYLSP_NEEDED
  ace_cc_yylsp -= ace_cc_yylen;
#endif

#if ACE_CC_YYDEBUG != 0
  if (ace_cc_yydebug)
    {
      short *ssp1 = ace_cc_yyss - 1;
      ACE_OS::fprintf (stderr, "state stack now");
      while (ssp1 != ace_cc_yyssp)
        ACE_OS::fprintf (stderr, " %d", *++ssp1);
      ACE_OS::fprintf (stderr, "\n");
    }
#endif

  *++ace_cc_yyvsp = ace_cc_yyval;

#ifdef ACE_CC_YYLSP_NEEDED
  ace_cc_yylsp++;
  if (ace_cc_yylen == 0)
    {
      ace_cc_yylsp->first_line = ace_cc_yylloc.first_line;
      ace_cc_yylsp->first_column = ace_cc_yylloc.first_column;
      ace_cc_yylsp->last_line = (ace_cc_yylsp-1)->last_line;
      ace_cc_yylsp->last_column = (ace_cc_yylsp-1)->last_column;
      ace_cc_yylsp->text = 0;
    }
  else
    {
      ace_cc_yylsp->last_line = (ace_cc_yylsp+ace_cc_yylen-1)->last_line;
      ace_cc_yylsp->last_column = (ace_cc_yylsp+ace_cc_yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  ace_cc_yyn = ace_cc_yyr1[ace_cc_yyn];

  ace_cc_yystate = ace_cc_yypgoto[ace_cc_yyn - ACE_CC_YYNTBASE] + *ace_cc_yyssp;
  if (ace_cc_yystate >= 0 && ace_cc_yystate <= ACE_CC_YYLAST && ace_cc_yycheck[ace_cc_yystate] == *ace_cc_yyssp)
    ace_cc_yystate = ace_cc_yytable[ace_cc_yystate];
  else
    ace_cc_yystate = ace_cc_yydefgoto[ace_cc_yyn - ACE_CC_YYNTBASE];

  goto ace_cc_yynewstate;

ace_cc_yyerrlab:   /* here on detecting error */

  if (! ace_cc_yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++ace_cc_yynerrs;

#ifdef ACE_CC_YYERROR_VERBOSE
      ace_cc_yyn = ace_cc_yypact[ace_cc_yystate];

      if (ace_cc_yyn > ACE_CC_YYFLAG && ace_cc_yyn < ACE_CC_YYLAST)
  {
    int size = 0;
    char *msg;
    int x, count;

    count = 0;
    /* Start X at -ace_cc_yyn if nec to avoid negative indexes in ace_cc_yycheck.  */
    for (x = (ace_cc_yyn < 0 ? -ace_cc_yyn : 0);
         x < (sizeof(ace_cc_yytname) / sizeof(char *)); x++)
      if (ace_cc_yycheck[x + ace_cc_yyn] == x)
        size += ACE_OS::strlen(ace_cc_yytname[x]) + 15, count++;
    msg = (char *) ACE_OS::malloc(size + 15);
    if (msg != 0)
      {
        ACE_OS::strcpy(msg, "parse error");

        if (count < 5)
    {
      count = 0;
      for (x = (ace_cc_yyn < 0 ? -ace_cc_yyn : 0);
           x < (sizeof(ace_cc_yytname) / sizeof(char *)); x++)
        if (ace_cc_yycheck[x + ace_cc_yyn] == x)
          {
            ACE_OS::strcat(msg, count == 0 ? ", expecting `" : " or `");
            ACE_OS::strcat(msg, ace_cc_yytname[x]);
            ACE_OS::strcat(msg, "'");
            count++;
          }
    }
        ace_cc_yyerror(msg);
        ACE_OS::free(msg);
      }
    else
      ace_cc_yyerror ("parse error; also virtual memory exceeded");
  }
      else
#endif /* ACE_CC_YYERROR_VERBOSE */
  ace_cc_yyerror("parse error");
    }

  goto ace_cc_yyerrlab1;
ace_cc_yyerrlab1:   /* here on error raised explicitly by an action */

  if (ace_cc_yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (ace_cc_yychar == ACE_CC_YYEOF)
  ACE_CC_YYABORT;

#if ACE_CC_YYDEBUG != 0
      if (ace_cc_yydebug)
        ACE_OS::fprintf(stderr, "Discarding token %d (%s).\n", ace_cc_yychar, ace_cc_yytname[ace_cc_yychar1]);
#endif

      ace_cc_yychar = ACE_CC_YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  ace_cc_yyerrstatus = 3;   /* Each real token shifted decrements this */

  goto ace_cc_yyerrhandle;

ace_cc_yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  ace_cc_yyn = ace_cc_yydefact[ace_cc_yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (ace_cc_yyn) goto ace_cc_yydefault;
#endif

ace_cc_yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (ace_cc_yyssp == ace_cc_yyss) ACE_CC_YYABORT;
  ace_cc_yyvsp--;
  ace_cc_yystate = *--ace_cc_yyssp;
#ifdef ACE_CC_YYLSP_NEEDED
  ace_cc_yylsp--;
#endif

#if ACE_CC_YYDEBUG != 0
  if (ace_cc_yydebug)
    {
      short *ssp1 = ace_cc_yyss - 1;
      ACE_OS::fprintf (stderr, "Error: state stack now");
      while (ssp1 != ace_cc_yyssp)
        ACE_OS::fprintf (stderr, " %d", *++ssp1);
      ACE_OS::fprintf (stderr, "\n");
    }
#endif

ace_cc_yyerrhandle:

  ace_cc_yyn = ace_cc_yypact[ace_cc_yystate];
  if (ace_cc_yyn == ACE_CC_YYFLAG)
    goto ace_cc_yyerrdefault;

  ace_cc_yyn += ACE_CC_YYTERROR;
  if (ace_cc_yyn < 0 || ace_cc_yyn > ACE_CC_YYLAST || ace_cc_yycheck[ace_cc_yyn] != ACE_CC_YYTERROR)
    goto ace_cc_yyerrdefault;

  ace_cc_yyn = ace_cc_yytable[ace_cc_yyn];
  if (ace_cc_yyn < 0)
    {
      if (ace_cc_yyn == ACE_CC_YYFLAG)
  goto ace_cc_yyerrpop;
      ace_cc_yyn = -ace_cc_yyn;
      goto ace_cc_yyreduce;
    }
  else if (ace_cc_yyn == 0)
    goto ace_cc_yyerrpop;

  if (ace_cc_yyn == ACE_CC_YYFINAL)
    ACE_CC_YYACCEPT;

#if ACE_CC_YYDEBUG != 0
  if (ace_cc_yydebug)
    ACE_OS::fprintf(stderr, "Shifting error token, ");
#endif

  *++ace_cc_yyvsp = ace_cc_yylval;
#ifdef ACE_CC_YYLSP_NEEDED
  *++ace_cc_yylsp = ace_cc_yylloc;
#endif

  ace_cc_yystate = ace_cc_yyn;
  goto ace_cc_yynewstate;

 ace_cc_yyacceptlab:
  /* ACE_CC_YYACCEPT comes here.  */
  if (ace_cc_yyfree_stacks)
    {
      ACE_OS::free (ace_cc_yyss);
      ACE_OS::free (ace_cc_yyvs);
#ifdef ACE_CC_YYLSP_NEEDED
      ACE_OS::free (ace_cc_yyls);
#endif
    }
  return 0;

 ace_cc_yyabortlab:
  /* ACE_CC_YYABORT comes here.  */
  if (ace_cc_yyfree_stacks)
    {
      ACE_OS::free (ace_cc_yyss);
      ACE_OS::free (ace_cc_yyvs);
#ifdef ACE_CC_YYLSP_NEEDED
      ACE_OS::free (ace_cc_yyls);
#endif
    }
  return 1;
}


void
ace_cc_yyerror (const char *s)
{
  ACE_OS::printf ("%d: %s at %s in:\n%s\n",
                  line_no,
                  s,
                  ace_cc_yytext,
                  line_buf);
  ACE_OS::exit (-1);
}
