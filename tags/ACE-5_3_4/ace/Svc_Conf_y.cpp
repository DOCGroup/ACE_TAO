/* A Bison parser, made from Svc_Conf.y
   by GNU bison 1.35.  */

#define ACE_YYBISON 1  /* Identify Bison output.  */

# define	ACE_DYNAMIC	257
# define	ACE_STATIC	258
# define	ACE_SUSPEND	259
# define	ACE_RESUME	260
# define	ACE_REMOVE	261
# define	ACE_USTREAM	262
# define	ACE_MODULE_T	263
# define	ACE_STREAM_T	264
# define	ACE_SVC_OBJ_T	265
# define	ACE_ACTIVE	266
# define	ACE_INACTIVE	267
# define	ACE_PATHNAME	268
# define	ACE_IDENT	269
# define	ACE_STRING	270


// $Id$

#include "ace/Svc_Conf.h"

#if (ACE_USES_CLASSIC_SVC_CONF == 1)

#include "ace/ARGV.h"
#include "ace/Module.h"
#include "ace/Stream.h"
#include "ace/Service_Types.h"


ACE_RCSID (ace,
           Svc_Conf_y,
           "$Id$")


// Prototypes.
static ACE_Module_Type *ace_get_module (ACE_Static_Node *str_rec,
                                        ACE_Static_Node *svc_type,
                                        int & ace_yyerrno);
static ACE_Module_Type *ace_get_module (ACE_Static_Node *str_rec,
                                        const ACE_TCHAR *svc_name,
                                        int & ace_yyerrno);

#define ACE_YYDEBUG_LEXER_TEXT (ace_yytext[ace_yyleng] = '\0', ace_yytext)

// Force the pretty debugging code to compile.
// #define ACE_YYDEBUG 1

#ifndef ACE_YYDEBUG
# define ACE_YYDEBUG 0
#endif



#define	ACE_YYFINAL		66
#define	ACE_YYFLAG		-32768
#define	ACE_YYNTBASE	23

/* ACE_YYTRANSLATE(ACE_YYLEX) -- Bison token number corresponding to ACE_YYLEX. */
#define ACE_YYTRANSLATE(x) ((unsigned)(x) <= 270 ? ace_yytranslate[x] : 43)

/* ACE_YYTRANSLATE[ACE_YYLEX] -- Bison token number corresponding to ACE_YYLEX. */
static const ACE_TCHAR ace_yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      20,    21,    22,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    19,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    17,     2,    18,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16
};

#if ACE_YYDEBUG
static const short ace_yyprhs[] =
{
       0,     0,     3,     6,     7,     9,    11,    13,    15,    17,
      19,    23,    27,    30,    33,    36,    40,    41,    46,    48,
      50,    51,    56,    57,    60,    61,    63,    65,    67,    69,
      71,    76,    78,    80,    81,    85,    91,    96,    99,   102,
     105,   107,   108,   110,   112
};
static const short ace_yyrhs[] =
{
      23,    24,     0,    23,     1,     0,     0,    25,     0,    26,
       0,    27,     0,    28,     0,    29,     0,    30,     0,     3,
      37,    41,     0,     4,    15,    41,     0,     5,    15,     0,
       6,    15,     0,     7,    15,     0,     8,    32,    33,     0,
       0,     8,    15,    31,    33,     0,    25,     0,    26,     0,
       0,    17,    34,    35,    18,     0,     0,    35,    36,     0,
       0,    25,     0,    26,     0,    27,     0,    28,     0,    29,
       0,    15,    40,    39,    38,     0,    12,     0,    13,     0,
       0,    42,    19,    15,     0,    42,    19,    15,    20,    21,
       0,    19,    15,    20,    21,     0,     9,    22,     0,    11,
      22,     0,    10,    22,     0,    16,     0,     0,    14,     0,
      15,     0,    16,     0
};

#endif

#if ACE_YYDEBUG
/* ACE_YYRLINE[ACE_YYN] -- source line where rule number ACE_YYN was defined. */
static const short ace_yyrline[] =
{
       0,    51,    59,    63,    67,    68,    69,    70,    71,    72,
      76,    86,    93,   100,   107,   114,   118,   118,   125,   128,
     134,   134,   143,   147,   155,   159,   186,   199,   207,   215,
     239,   276,   280,   284,   291,   295,   299,   306,   310,   314,
     321,   322,   326,   327,   328
};
#endif


#if (ACE_YYDEBUG) || defined ACE_YYERROR_VERBOSE

/* ACE_YYTNAME[TOKEN_NUM] -- String name of the token TOKEN_NUM. */
static const ACE_TCHAR *const ace_yytname[] =
{
  ACE_LIB_TEXT ("$"),
  ACE_LIB_TEXT ("error"),
  ACE_LIB_TEXT ("$undefined."),
  ACE_LIB_TEXT ("ACE_DYNAMIC"),
  ACE_LIB_TEXT ("ACE_STATIC"),
  ACE_LIB_TEXT ("ACE_SUSPEND"),
  ACE_LIB_TEXT ("ACE_RESUME"),
  ACE_LIB_TEXT ("ACE_REMOVE"),
  ACE_LIB_TEXT ("ACE_USTREAM"),
  ACE_LIB_TEXT ("ACE_MODULE_T",)
  ACE_LIB_TEXT ("ACE_STREAM_T"),
  ACE_LIB_TEXT ("ACE_SVC_OBJ_T"),
  ACE_LIB_TEXT ("ACE_ACTIVE"),
  ACE_LIB_TEXT ("ACE_INACTIVE",)
  ACE_LIB_TEXT ("ACE_PATHNAME"),
  ACE_LIB_TEXT ("ACE_IDENT"),
  ACE_LIB_TEXT ("ACE_STRING"),
  ACE_LIB_TEXT ("'{'"),
  ACE_LIB_TEXT ("'}'"),
  ACE_LIB_TEXT ("':'"),
  ACE_LIB_TEXT ("'('"),
  ACE_LIB_TEXT ("')'"),
  ACE_LIB_TEXT ("'*'"),
  ACE_LIB_TEXT ("svc_config_entries"),
  ACE_LIB_TEXT ("svc_config_entry"),
  ACE_LIB_TEXT ("dynamic",)
  ACE_LIB_TEXT ("static"),
  ACE_LIB_TEXT ("suspend"),
  ACE_LIB_TEXT ("resume"),
  ACE_LIB_TEXT ("remove"),
  ACE_LIB_TEXT ("stream"),
  ACE_LIB_TEXT ("@1"),
  ACE_LIB_TEXT ("stream_ops",)
  ACE_LIB_TEXT ("stream_modules"),
  ACE_LIB_TEXT ("@2"),
  ACE_LIB_TEXT ("module_list"),
  ACE_LIB_TEXT ("module"),
  ACE_LIB_TEXT ("svc_location",)
  ACE_LIB_TEXT ("status"),
  ACE_LIB_TEXT ("svc_initializer"),
  ACE_LIB_TEXT ("type"),
  ACE_LIB_TEXT ("parameters_opt"),
  ACE_LIB_TEXT ("pathname"),
  0
};
#endif

/* ACE_YYR1[ACE_YYN] -- Symbol number of symbol that rule ACE_YYN derives. */
static const short ace_yyr1[] =
{
       0,    23,    23,    23,    24,    24,    24,    24,    24,    24,
      25,    26,    27,    28,    29,    30,    31,    30,    32,    32,
      34,    33,    33,    35,    35,    36,    36,    36,    36,    36,
      37,    38,    38,    38,    39,    39,    39,    40,    40,    40,
      41,    41,    42,    42,    42
};

/* ACE_YYR2[ACE_YYN] -- Number of symbols composing right hand side of rule ACE_YYN. */
static const short ace_yyr2[] =
{
       0,     2,     2,     0,     1,     1,     1,     1,     1,     1,
       3,     3,     2,     2,     2,     3,     0,     4,     1,     1,
       0,     4,     0,     2,     0,     1,     1,     1,     1,     1,
       4,     1,     1,     0,     3,     5,     4,     2,     2,     2,
       1,     0,     1,     1,     1
};

/* ACE_YYDEFACT[S] -- default rule to reduce with in state S when ACE_YYTABLE
   doesn't specify something else to do.  Zero means the default is an
   error. */
static const short ace_yydefact[] =
{
       3,     0,     2,     0,     0,     0,     0,     0,     0,     1,
       4,     5,     6,     7,     8,     9,     0,    41,    41,    12,
      13,    14,    16,    18,    19,    22,     0,     0,     0,     0,
      40,    10,    11,    22,    20,    15,    37,    39,    38,    42,
      43,    44,     0,    33,     0,    17,    24,     0,    31,    32,
      30,     0,     0,     0,    34,    21,    25,    26,    27,    28,
      29,    23,    36,     0,    35,     0,     0
};

static const short ace_yydefgoto[] =
{
       1,     9,    10,    11,    12,    13,    14,    15,    33,    25,
      35,    46,    52,    61,    17,    50,    43,    29,    31,    44
};

static const short ace_yypact[] =
{
  -32768,    20,-32768,     1,     3,     7,    14,    18,     4,-32768,
  -32768,-32768,-32768,-32768,-32768,-32768,    21,    19,    19,-32768,
  -32768,-32768,-32768,-32768,-32768,    -2,    12,    15,    16,    -5,
  -32768,-32768,-32768,    -2,-32768,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,    24,     0,    17,-32768,-32768,    22,-32768,-32768,
  -32768,    25,    -1,    26,    23,-32768,-32768,-32768,-32768,-32768,
  -32768,-32768,-32768,    27,-32768,    41,-32768
};

static const short ace_yypgoto[] =
{
  -32768,-32768,    -8,    -7,    -6,    -3,     2,-32768,-32768,-32768,
      28,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    32,-32768
};


#define	ACE_YYLAST		61


static const short ace_yytable[] =
{
      23,    24,     3,     4,     5,     6,     7,     3,     4,    39,
      40,    41,    48,    49,    42,    34,    16,    55,    18,    22,
      65,     2,    19,     3,     4,     5,     6,     7,     8,    20,
      26,    27,    28,    21,    36,    30,    51,    37,    38,    47,
      54,    66,    53,    63,    56,    57,    58,    62,    64,    59,
      32,     0,     0,     0,    60,     0,     0,     0,     0,     0,
       0,    45
};

static const short ace_yycheck[] =
{
       8,     8,     3,     4,     5,     6,     7,     3,     4,    14,
      15,    16,    12,    13,    19,    17,    15,    18,    15,    15,
       0,     1,    15,     3,     4,     5,     6,     7,     8,    15,
       9,    10,    11,    15,    22,    16,    19,    22,    22,    15,
      15,     0,    20,    20,    52,    52,    52,    21,    21,    52,
      18,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    33
};
#define ACE_YYPURE 1

/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */

/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software
   Foundation, Inc.

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

/* This is the parser code that is written into each bison parser when
   the %semantic_parser declaration is not specified in the grammar.
   It was written by Richard Stallman by simplifying the hairy parser
   used when %semantic_parser is specified.  */

/* All symbols defined below should begin with ace_yy or ACE_YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

#if ! defined (ace_yyoverflow) || defined (ACE_YYERROR_VERBOSE)

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if ACE_YYSTACK_USE_ALLOCA
#  define ACE_YYSTACK_ALLOC alloca
# else
#  ifndef ACE_YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define ACE_YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define ACE_YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef ACE_YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define ACE_YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define ACE_YYSIZE_T size_t
#  endif
#  define ACE_YYSTACK_ALLOC malloc
#  define ACE_YYSTACK_FREE free
# endif
#endif /* ! defined (ace_yyoverflow) || defined (ACE_YYERROR_VERBOSE) */


#if (! defined (ace_yyoverflow) \
     && (! defined (__cplusplus) \
	 || (ACE_YYLTYPE_IS_TRIVIAL && ACE_YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union ace_yyalloc
{
  short ace_yyss;
  ACE_YYSTYPE ace_yyvs;
# if ACE_YYLSP_NEEDED
  ACE_YYLTYPE ace_yyls;
# endif
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define ACE_YYSTACK_GAP_MAX (sizeof (union ace_yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# if ACE_YYLSP_NEEDED
#  define ACE_YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (ACE_YYSTYPE) + sizeof (ACE_YYLTYPE))	\
      + 2 * ACE_YYSTACK_GAP_MAX)
# else
#  define ACE_YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (ACE_YYSTYPE))				\
      + ACE_YYSTACK_GAP_MAX)
# endif

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef ACE_YYCOPY
#  if 1 < __GNUC__
#   define ACE_YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define ACE_YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register ACE_YYSIZE_T ace_yyi;		\
	  for (ace_yyi = 0; ace_yyi < (Count); ace_yyi++)	\
	    (To)[ace_yyi] = (From)[ace_yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables ACE_YYSIZE and ACE_YYSTACKSIZE give the old and new number of
   elements in the stack, and ACE_YYPTR gives the new location of the
   stack.  Advance ACE_YYPTR to a properly aligned location for the next
   stack.  */
# define ACE_YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	ACE_YYSIZE_T ace_yynewbytes;						\
	ACE_YYCOPY (&ace_yyptr->Stack, Stack, ace_yysize);				\
	Stack = &ace_yyptr->Stack;						\
	ace_yynewbytes = ace_yystacksize * sizeof (*Stack) + ACE_YYSTACK_GAP_MAX;	\
	ace_yyptr += ace_yynewbytes / sizeof (*ace_yyptr);				\
      }									\
    while (0)

#endif


#if ! defined (ACE_YYSIZE_T) && defined (__SIZE_TYPE__)
# define ACE_YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (ACE_YYSIZE_T) && defined (size_t)
# define ACE_YYSIZE_T size_t
#endif
#if ! defined (ACE_YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define ACE_YYSIZE_T size_t
# endif
#endif
#if ! defined (ACE_YYSIZE_T)
# define ACE_YYSIZE_T unsigned int
#endif

#define ace_yyerrok		(ace_yyerrstatus = 0)
#define ace_yyclearin	(ace_yychar = ACE_YYEMPTY)
#define ACE_YYEMPTY		-2
#define ACE_YYEOF		0
#define ACE_YYACCEPT	goto ace_yyacceptlab
#define ACE_YYABORT 	goto ace_yyabortlab
#define ACE_YYERROR		goto ace_yyerrlab1
/* Like ACE_YYERROR except do call ace_yyerror.  This remains here temporarily
   to ease the transition to the new meaning of ACE_YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define ACE_YYFAIL		goto ace_yyerrlab
#define ACE_YYRECOVERING()  (!!ace_yyerrstatus)
#define ACE_YYBACKUP(Token, Value)					\
do								\
  if (ace_yychar == ACE_YYEMPTY && ace_yylen == 1)				\
    {								\
      ace_yychar = (Token);						\
      ace_yylval = (Value);						\
      ace_yychar1 = ACE_YYTRANSLATE (ace_yychar);				\
      ACE_YYPOPSTACK;						\
      goto ace_yybackup;						\
    }								\
  else								\
    { 								\
      ace_yyerror (ACE_SVC_CONF_PARAM->yyerrno, ACE_SVC_CONF_PARAM->yylineno, ACE_LIB_TEXT ("syntax error: cannot back up"));			\
      ACE_YYERROR;							\
    }								\
while (0)

#define ACE_YYTERROR	1
#define ACE_YYERRCODE	256


/* ACE_YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).

   When ACE_YYLLOC_DEFAULT is run, CURRENT is set the location of the
   first token.  By default, to implement support for ranges, extend
   its range to the last symbol.  */

#ifndef ACE_YYLLOC_DEFAULT
# define ACE_YYLLOC_DEFAULT(Current, Rhs, N)       	\
   Current.last_line   = Rhs[N].last_line;	\
   Current.last_column = Rhs[N].last_column;
#endif


/* ACE_YYLEX -- calling `ace_yylex' with the right arguments.  */

#if ACE_YYPURE
# if ACE_YYLSP_NEEDED
#  ifdef ACE_YYLEX_PARAM
#   define ACE_YYLEX		ace_yylex (&ace_yylval, &ace_yylloc, ACE_YYLEX_PARAM)
#  else
#   define ACE_YYLEX		ace_yylex (&ace_yylval, &ace_yylloc)
#  endif
# else /* !ACE_YYLSP_NEEDED */
#  ifdef ACE_YYLEX_PARAM
#   define ACE_YYLEX		ace_yylex (&ace_yylval, ACE_YYLEX_PARAM)
#  else
#   define ACE_YYLEX		ace_yylex (&ace_yylval)
#  endif
# endif /* !ACE_YYLSP_NEEDED */
#else /* !ACE_YYPURE */
# define ACE_YYLEX			ace_yylex ()
#endif /* !ACE_YYPURE */


/* Enable debugging if requested.  */
#if ACE_YYDEBUG

# ifndef ACE_YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define ACE_YYFPRINTF ACE_OS::fprintf
# endif

# define ACE_YYDPRINTF(Args)			\
do {						\
  if (ace_yydebug)					\
    ACE_YYFPRINTF Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int ace_yydebug;
#else /* !ACE_YYDEBUG */
# define ACE_YYDPRINTF(Args)
#endif /* !ACE_YYDEBUG */

/* ACE_YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	ACE_YYINITDEPTH
# define ACE_YYINITDEPTH 200
#endif

/* ACE_YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < ACE_YYSTACK_BYTES (ACE_YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if ACE_YYMAXDEPTH == 0
# undef ACE_YYMAXDEPTH
#endif

#ifndef ACE_YYMAXDEPTH
# define ACE_YYMAXDEPTH 10000
#endif

#ifdef ACE_YYERROR_VERBOSE

# ifndef ace_yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define ace_yystrlen strlen
#  else
/* Return the length of ACE_YYSTR.  */
static ACE_YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
ace_yystrlen (const ACE_TCHAR *ace_yystr)
#   else
ace_yystrlen (ace_yystr)
     const ACE_TCHAR *ace_yystr;
#   endif
{
  register const ACE_TCHAR *ace_yys = ace_yystr;

  while (*ace_yys++ != '\0')
    continue;

  return ace_yys - ace_yystr - 1;
}
#  endif
# endif

# ifndef ace_yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define ace_yystpcpy stpcpy
#  else
/* Copy ACE_YYSRC to ACE_YYDEST, returning the address of the terminating '\0' in
   ACE_YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
ace_yystpcpy (ACE_TCHAR *ace_yydest, const ACE_TCHAR *ace_yysrc)
#   else
ace_yystpcpy (ace_yydest, ace_yysrc)
     ACE_TCHAR *ace_yydest;
     const ACE_TCHAR *ace_yysrc;
#   endif
{
  register ACE_TCHAR *ace_yyd = ace_yydest;
  register const ACE_TCHAR *ace_yys = ace_yysrc;

  while ((*ace_yyd++ = *ace_yys++) != '\0')
    continue;

  return ace_yyd - 1;
}
#  endif
# endif
#endif



/* The user can define ACE_YYPARSE_PARAM as the name of an argument to be passed
   into ace_yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef ACE_YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define ACE_YYPARSE_PARAM_ARG void *ACE_YYPARSE_PARAM
#  define ACE_YYPARSE_PARAM_DECL
# else
#  define ACE_YYPARSE_PARAM_ARG ACE_YYPARSE_PARAM
#  define ACE_YYPARSE_PARAM_DECL void *ACE_YYPARSE_PARAM;
# endif
#else /* !ACE_YYPARSE_PARAM */
# define ACE_YYPARSE_PARAM_ARG
# define ACE_YYPARSE_PARAM_DECL
#endif /* !ACE_YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef ACE_YYPARSE_PARAM
int ace_yyparse (void *);
# else
int ace_yyparse (void);
# endif
#endif

/* ACE_YY_DECL_VARIABLES -- depending whether we use a pure parser,
   variables are global, or local to ACE_YYPARSE.  */

#define ACE_YY_DECL_NON_LSP_VARIABLES			\
/* The lookahead symbol.  */				\
int ace_yychar;						\
							\
/* The semantic value of the lookahead symbol. */	\
ACE_YYSTYPE ace_yylval;						\
							\
/* Number of parse errors so far.  */			\
int ace_yynerrs;

#if ACE_YYLSP_NEEDED
# define ACE_YY_DECL_VARIABLES			\
ACE_YY_DECL_NON_LSP_VARIABLES			\
						\
/* Location data for the lookahead symbol.  */	\
ACE_YYLTYPE ace_yylloc;
#else
# define ACE_YY_DECL_VARIABLES			\
ACE_YY_DECL_NON_LSP_VARIABLES
#endif


/* If nonreentrant, generate the variables here. */

#if !ACE_YYPURE
ACE_YY_DECL_VARIABLES
#endif  /* !ACE_YYPURE */

int
ace_yyparse (ACE_YYPARSE_PARAM_ARG)
     ACE_YYPARSE_PARAM_DECL
{
  /* If reentrant, generate the variables here. */
#if ACE_YYPURE
  ACE_YY_DECL_VARIABLES
#endif  /* !ACE_YYPURE */

  register int ace_yystate;
  register int ace_yyn;
  int ace_yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int ace_yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int ace_yychar1 = 0;

  /* Three stacks and their tools:
     `ace_yyss': related to states,
     `ace_yyvs': related to semantic values,
     `ace_yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow ace_yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack. */
  short	ace_yyssa[ACE_YYINITDEPTH];
  short *ace_yyss = ace_yyssa;
  register short *ace_yyssp;

  /* The semantic value stack.  */
  ACE_YYSTYPE ace_yyvsa[ACE_YYINITDEPTH];
  ACE_YYSTYPE *ace_yyvs = ace_yyvsa;
  register ACE_YYSTYPE *ace_yyvsp;

#if ACE_YYLSP_NEEDED
  /* The location stack.  */
  ACE_YYLTYPE ace_yylsa[ACE_YYINITDEPTH];
  ACE_YYLTYPE *ace_yyls = ace_yylsa;
  ACE_YYLTYPE *ace_yylsp;
#endif

#if ACE_YYLSP_NEEDED
# define ACE_YYPOPSTACK   (ace_yyvsp--, ace_yyssp--, ace_yylsp--)
#else
# define ACE_YYPOPSTACK   (ace_yyvsp--, ace_yyssp--)
#endif

  ACE_YYSIZE_T ace_yystacksize = ACE_YYINITDEPTH;


  /* The variables used to return semantic value and location from the
     action routines.  */
  ACE_YYSTYPE ace_yyval;
#if ACE_YYLSP_NEEDED
  ACE_YYLTYPE ace_yyloc;
#endif

  /* When reducing, the number of symbols on the RHS of the reduced
     rule. */
  int ace_yylen;

  ACE_YYDPRINTF ((stderr, ACE_LIB_TEXT ("Starting parse\n")));

  ace_yystate = 0;
  ace_yyerrstatus = 0;
  ace_yynerrs = 0;
  ace_yychar = ACE_YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  ace_yyssp = ace_yyss;
  ace_yyvsp = ace_yyvs;
#if ACE_YYLSP_NEEDED
  ace_yylsp = ace_yyls;
#endif
  goto ace_yysetstate;

/*------------------------------------------------------------.
| ace_yynewstate -- Push a new state, which is found in ace_yystate.  |
`------------------------------------------------------------*/
 ace_yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  ace_yyssp++;

 ace_yysetstate:
  *ace_yyssp = ace_yystate;

  if (ace_yyssp >= ace_yyss + ace_yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      ACE_YYSIZE_T ace_yysize = ace_yyssp - ace_yyss + 1;

#ifdef ace_yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	ACE_YYSTYPE *ace_yyvs1 = ace_yyvs;
	short *ace_yyss1 = ace_yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  */
# if ACE_YYLSP_NEEDED
	ACE_YYLTYPE *ace_yyls1 = ace_yyls;
	/* This used to be a conditional around just the two extra args,
	   but that might be undefined if ace_yyoverflow is a macro.  */
	ace_yyoverflow (ACE_LIB_TEXT ("parser stack overflow"),
		    &ace_yyss1, ace_yysize * sizeof (*ace_yyssp),
		    &ace_yyvs1, ace_yysize * sizeof (*ace_yyvsp),
		    &ace_yyls1, ace_yysize * sizeof (*ace_yylsp),
		    &ace_yystacksize);
	ace_yyls = ace_yyls1;
# else
	ace_yyoverflow (ACE_LIB_TEXT ("parser stack overflow"),
		    &ace_yyss1, ace_yysize * sizeof (*ace_yyssp),
		    &ace_yyvs1, ace_yysize * sizeof (*ace_yyvsp),
		    &ace_yystacksize);
# endif
	ace_yyss = ace_yyss1;
	ace_yyvs = ace_yyvs1;
      }
#else /* no ace_yyoverflow */
# ifndef ACE_YYSTACK_RELOCATE
      goto ace_yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (ace_yystacksize >= ACE_YYMAXDEPTH)
	goto ace_yyoverflowlab;
      ace_yystacksize *= 2;
      if (ace_yystacksize > ACE_YYMAXDEPTH)
	ace_yystacksize = ACE_YYMAXDEPTH;

      {
	short *ace_yyss1 = ace_yyss;
	union ace_yyalloc *ace_yyptr =
	  (union ace_yyalloc *) ACE_YYSTACK_ALLOC (ACE_YYSTACK_BYTES (ace_yystacksize));
	if (! ace_yyptr)
	  goto ace_yyoverflowlab;
	ACE_YYSTACK_RELOCATE (ace_yyss);
	ACE_YYSTACK_RELOCATE (ace_yyvs);
# if ACE_YYLSP_NEEDED
	ACE_YYSTACK_RELOCATE (ace_yyls);
# endif
# undef ACE_YYSTACK_RELOCATE
	if (ace_yyss1 != ace_yyssa)
	  ACE_YYSTACK_FREE (ace_yyss1);
      }
# endif
#endif /* no ace_yyoverflow */

      ace_yyssp = ace_yyss + ace_yysize - 1;
      ace_yyvsp = ace_yyvs + ace_yysize - 1;
#if ACE_YYLSP_NEEDED
      ace_yylsp = ace_yyls + ace_yysize - 1;
#endif

      ACE_YYDPRINTF ((stderr, ACE_LIB_TEXT ("Stack size increased to %lu\n"),
		  (unsigned long int) ace_yystacksize));

      if (ace_yyssp >= ace_yyss + ace_yystacksize - 1)
	ACE_YYABORT;
    }

  ACE_YYDPRINTF ((stderr, ACE_LIB_TEXT ("Entering state %d\n"), ace_yystate));

  goto ace_yybackup;


/*-----------.
| ace_yybackup.  |
`-----------*/
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
      ACE_YYDPRINTF ((stderr, ACE_LIB_TEXT ("Reading a token: ")));
      ace_yychar = ACE_YYLEX;
    }

  /* Convert token to internal form (in ace_yychar1) for indexing tables with */

  if (ace_yychar <= 0)		/* This means end of input. */
    {
      ace_yychar1 = 0;
      ace_yychar = ACE_YYEOF;		/* Don't call ACE_YYLEX any more */

      ACE_YYDPRINTF ((stderr, ACE_LIB_TEXT ("Now at end of input.\n")));
    }
  else
    {
      ace_yychar1 = ACE_YYTRANSLATE (ace_yychar);

#if ACE_YYDEBUG
     /* We have to keep this `#if ACE_YYDEBUG', since we use variables
	which are defined only if `ACE_YYDEBUG' is set.  */
      if (ace_yydebug)
	{
	  ACE_YYFPRINTF (stderr, ACE_LIB_TEXT ("Next token is %d (%s"),
		     ace_yychar, ace_yytname[ace_yychar1]);
	  /* Give the individual parser a way to print the precise
	     meaning of a token, for further debugging info.  */
# ifdef ACE_YYPRINT
	  ACE_YYPRINT (stderr, ace_yychar, ace_yylval);
# endif
	  ACE_YYFPRINTF (stderr, ACE_LIB_TEXT (")\n"));
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
  ACE_YYDPRINTF ((stderr, ACE_LIB_TEXT ("Shifting token %d (%s), "),
	      ace_yychar, ace_yytname[ace_yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (ace_yychar != ACE_YYEOF)
    ace_yychar = ACE_YYEMPTY;

  *++ace_yyvsp = ace_yylval;
#if ACE_YYLSP_NEEDED
  *++ace_yylsp = ace_yylloc;
#endif

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (ace_yyerrstatus)
    ace_yyerrstatus--;

  ace_yystate = ace_yyn;
  goto ace_yynewstate;


/*-----------------------------------------------------------.
| ace_yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
ace_yydefault:
  ace_yyn = ace_yydefact[ace_yystate];
  if (ace_yyn == 0)
    goto ace_yyerrlab;
  goto ace_yyreduce;


/*-----------------------------.
| ace_yyreduce -- Do a reduction.  |
`-----------------------------*/
ace_yyreduce:
  /* ace_yyn is the number of a rule to reduce with.  */
  ace_yylen = ace_yyr2[ace_yyn];

  /* If ACE_YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets ACE_YYVAL to the semantic value of
     the lookahead token.  This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to ACE_YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that ACE_YYVAL may be used uninitialized.  */
  ace_yyval = ace_yyvsp[1-ace_yylen];

#if ACE_YYLSP_NEEDED
  /* Similarly for the default location.  Let the user run additional
     commands if for instance locations are ranges.  */
  ace_yyloc = ace_yylsp[1-ace_yylen];
  ACE_YYLLOC_DEFAULT (ace_yyloc, (ace_yylsp - ace_yylen), ace_yylen);
#endif

#if ACE_YYDEBUG
  /* We have to keep this `#if ACE_YYDEBUG', since we use variables which
     are defined only if `ACE_YYDEBUG' is set.  */
  if (ace_yydebug)
    {
      int ace_yyi;

      ACE_YYFPRINTF (stderr, ACE_LIB_TEXT ("Reducing via rule %d (line %d), "),
		 ace_yyn, ace_yyrline[ace_yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (ace_yyi = ace_yyprhs[ace_yyn]; ace_yyrhs[ace_yyi] > 0; ace_yyi++)
	ACE_YYFPRINTF (stderr, ACE_LIB_TEXT ("%s "), ace_yytname[ace_yyrhs[ace_yyi]]);
      ACE_YYFPRINTF (stderr, ACE_LIB_TEXT (" -> %s\n"), ace_yytname[ace_yyr1[ace_yyn]]);
    }
#endif

  switch (ace_yyn) {

case 1:
{
      if (ace_yyvsp[0].parse_node_ != 0)
      {
        ace_yyvsp[0].parse_node_->apply (ACE_SVC_CONF_PARAM->yyerrno); delete ace_yyvsp[0].parse_node_;
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
                                                svc_type,
                                                ACE_SVC_CONF_PARAM->yyerrno);
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
      ACE_Module_Type *mt = ace_get_module (ace_yyvsp[-2].static_node_,
                                            ace_yyvsp[0].static_node_->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);

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
                                            ace_yyvsp[0].static_node_->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);
      if (mt != 0)
        mt->suspend ();
    ;
    break;}
case 28:
{
      ACE_Module_Type *mt = ace_get_module (ace_yyvsp[-2].static_node_,
                                            ace_yyvsp[0].static_node_->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);
      if (mt != 0)
        mt->resume ();
    ;
    break;}
case 29:
{
      ACE_Static_Node *stream = ace_yyvsp[-2].static_node_;
      ACE_Static_Node *module = ace_yyvsp[0].static_node_;
      ACE_Module_Type *mt = ace_get_module (stream,
                                            module->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);

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
      void *sym = ace_yyvsp[-1].location_node_->symbol (ACE_SVC_CONF_PARAM->yyerrno, &gobbler);

      if (sym != 0)
        {
          ACE_Service_Type_Impl *stp
            = ACE_Service_Config::create_service_type_impl (ace_yyvsp[-3].ident_,
                                                            ace_yyvsp[-2].type_,
                                                            sym,
                                                            flags,
                                                            gobbler);
          if (stp == 0)
            ++ACE_SVC_CONF_PARAM->yyerrno;

          ace_yyval.svc_record_ = new ACE_Service_Type (ace_yyvsp[-3].ident_,
                                     stp,
                                     ace_yyvsp[-1].location_node_->dll (),
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



  ace_yyvsp -= ace_yylen;
  ace_yyssp -= ace_yylen;
#if ACE_YYLSP_NEEDED
  ace_yylsp -= ace_yylen;
#endif

#if ACE_YYDEBUG
  if (ace_yydebug)
    {
      short *ace_yyssp1 = ace_yyss - 1;
      ACE_YYFPRINTF (stderr, ACE_LIB_TEXT ("state stack now"));
      while (ace_yyssp1 != ace_yyssp)
	ACE_YYFPRINTF (stderr, ACE_LIB_TEXT (" %d"), *++ace_yyssp1);
      ACE_YYFPRINTF (stderr, ACE_LIB_TEXT ("\n"));
    }
#endif

  *++ace_yyvsp = ace_yyval;
#if ACE_YYLSP_NEEDED
  *++ace_yylsp = ace_yyloc;
#endif

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  ace_yyn = ace_yyr1[ace_yyn];

  ace_yystate = ace_yypgoto[ace_yyn - ACE_YYNTBASE] + *ace_yyssp;
  if (ace_yystate >= 0 && ace_yystate <= ACE_YYLAST && ace_yycheck[ace_yystate] == *ace_yyssp)
    ace_yystate = ace_yytable[ace_yystate];
  else
    ace_yystate = ace_yydefgoto[ace_yyn - ACE_YYNTBASE];

  goto ace_yynewstate;


/*------------------------------------.
| ace_yyerrlab -- here on detecting error |
`------------------------------------*/
ace_yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!ace_yyerrstatus)
    {
      ++ace_yynerrs;

#ifdef ACE_YYERROR_VERBOSE
      ace_yyn = ace_yypact[ace_yystate];

      if (ace_yyn > ACE_YYFLAG && ace_yyn < ACE_YYLAST)
	{
	  ACE_YYSIZE_T ace_yysize = 0;
	  ACE_TCHAR *ace_yymsg;
	  int ace_yyx, ace_yycount;

	  ace_yycount = 0;
	  /* Start ACE_YYX at -ACE_YYN if negative to avoid negative indexes in
	     ACE_YYCHECK.  */
	  for (ace_yyx = ace_yyn < 0 ? -ace_yyn : 0;
	       ace_yyx < (int) (sizeof (ace_yytname) / sizeof (ACE_TCHAR *)); ace_yyx++)
	    if (ace_yycheck[ace_yyx + ace_yyn] == ace_yyx)
	      ace_yysize += ace_yystrlen (ace_yytname[ace_yyx]) + 15, ace_yycount++;
	  ace_yysize += ace_yystrlen (ACE_LIB_TEXT ("parse error, unexpected ")) + 1;
	  ace_yysize += ace_yystrlen (ace_yytname[ACE_YYTRANSLATE (ace_yychar)]);
	  ace_yymsg = (ACE_TCHAR *) ACE_YYSTACK_ALLOC (ace_yysize);
	  if (ace_yymsg != 0)
	    {
	      ACE_TCHAR *ace_yyp = ace_yystpcpy (ace_yymsg, ACE_LIB_TEXT ("parse error, unexpected "));
	      ace_yyp = ace_yystpcpy (ace_yyp, ace_yytname[ACE_YYTRANSLATE (ace_yychar)]);

	      if (ace_yycount < 5)
		{
		  ace_yycount = 0;
		  for (ace_yyx = ace_yyn < 0 ? -ace_yyn : 0;
		       ace_yyx < (int) (sizeof (ace_yytname) / sizeof (ACE_TCHAR *));
		       ace_yyx++)
		    if (ace_yycheck[ace_yyx + ace_yyn] == ace_yyx)
		      {
			const ACE_TCHAR *ace_yyq = ! ace_yycount ? ACE_LIB_TEXT (", expecting ") : ACE_LIB_TEXT (" or ");
			ace_yyp = ace_yystpcpy (ace_yyp, ace_yyq);
			ace_yyp = ace_yystpcpy (ace_yyp, ace_yytname[ace_yyx]);
			ace_yycount++;
		      }
		}
	      ace_yyerror (ACE_SVC_CONF_PARAM->yyerrno, ACE_SVC_CONF_PARAM->yylineno, ace_yymsg);
	      ACE_YYSTACK_FREE (ace_yymsg);
	    }
	  else
	    ace_yyerror (ACE_SVC_CONF_PARAM->yyerrno, ACE_SVC_CONF_PARAM->yylineno, ACE_LIB_TEXT ("parse error; also virtual memory exhausted"));
	}
      else
#endif /* defined (ACE_YYERROR_VERBOSE) */
	ace_yyerror (ACE_SVC_CONF_PARAM->yyerrno, ACE_SVC_CONF_PARAM->yylineno, ACE_LIB_TEXT ("parse error"));
    }
  goto ace_yyerrlab1;


/*--------------------------------------------------.
| ace_yyerrlab1 -- error raised explicitly by an action |
`--------------------------------------------------*/
ace_yyerrlab1:
  if (ace_yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* return failure if at end of input */
      if (ace_yychar == ACE_YYEOF)
	ACE_YYABORT;
      ACE_YYDPRINTF ((stderr, ACE_LIB_TEXT ("Discarding token %d (%s).\n"),
		  ace_yychar, ace_yytname[ace_yychar1]));
      ace_yychar = ACE_YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  ace_yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto ace_yyerrhandle;


/*-------------------------------------------------------------------.
| ace_yyerrdefault -- current state does not do anything special for the |
| error token.                                                       |
`-------------------------------------------------------------------*/
ace_yyerrdefault:
#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */

  /* If its default is to accept any token, ok.  Otherwise pop it.  */
  ace_yyn = ace_yydefact[ace_yystate];
  if (ace_yyn)
    goto ace_yydefault;
#endif


/*---------------------------------------------------------------.
| ace_yyerrpop -- pop the current state because it cannot handle the |
| error token                                                    |
`---------------------------------------------------------------*/
ace_yyerrpop:
  if (ace_yyssp == ace_yyss)
    ACE_YYABORT;
  ace_yyvsp--;
  ace_yystate = *--ace_yyssp;
#if ACE_YYLSP_NEEDED
  ace_yylsp--;
#endif

#if ACE_YYDEBUG
  if (ace_yydebug)
    {
      short *ace_yyssp1 = ace_yyss - 1;
      ACE_YYFPRINTF (stderr, ACE_LIB_TEXT ("Error: state stack now"));
      while (ace_yyssp1 != ace_yyssp)
	ACE_YYFPRINTF (stderr, ACE_LIB_TEXT (" %d"), *++ace_yyssp1);
      ACE_YYFPRINTF (stderr, ACE_LIB_TEXT ("\n"));
    }
#endif

/*--------------.
| ace_yyerrhandle.  |
`--------------*/
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

  ACE_YYDPRINTF ((stderr, ACE_LIB_TEXT ("Shifting error token, ")));

  *++ace_yyvsp = ace_yylval;
#if ACE_YYLSP_NEEDED
  *++ace_yylsp = ace_yylloc;
#endif

  ace_yystate = ace_yyn;
  goto ace_yynewstate;


/*-------------------------------------.
| ace_yyacceptlab -- ACE_YYACCEPT comes here.  |
`-------------------------------------*/
ace_yyacceptlab:
  ace_yyresult = 0;
  goto ace_yyreturn;

/*-----------------------------------.
| ace_yyabortlab -- ACE_YYABORT comes here.  |
`-----------------------------------*/
ace_yyabortlab:
  ace_yyresult = 1;
  goto ace_yyreturn;

/*---------------------------------------------.
| ace_yyoverflowab -- parser overflow comes here.  |
`---------------------------------------------*/
ace_yyoverflowlab:
  ace_yyerror (ACE_SVC_CONF_PARAM->yyerrno, ACE_SVC_CONF_PARAM->yylineno, ACE_LIB_TEXT ("parser stack overflow"));
  ace_yyresult = 2;
  /* Fall through.  */

ace_yyreturn:
#ifndef ace_yyoverflow
  if (ace_yyss != ace_yyssa)
    ACE_YYSTACK_FREE (ace_yyss);
#endif
  return ace_yyresult;
}

// Prints the error string to standard output.  Cleans up the error
// messages.

void
ace_yyerror (int ace_yyerrno, int ace_yylineno, const ACE_TCHAR *s)
{
#if defined (ACE_NLOGGING)
  ACE_UNUSED_ARG (ace_yyerrno);
  ACE_UNUSED_ARG (ace_yylineno);
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
                const ACE_TCHAR *svc_name,
                int & ace_yyerrno)
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
                ACE_Static_Node *svc_type,
                int & ace_yyerrno)
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
// Main driver program.

int
main (int argc, char *argv[])
{
  ACE_Svc_Conf_Param param (stdin);

  // Try to reopen any filename argument to use ACE_YYIN.
  if (argc > 1 && (ace_yyin = freopen (argv[1], "r", stdin)) == 0)
    (void) ACE_OS::ACE_OS::fprintf (stderr, ACE_LIB_TEXT ("usage: %s [file]\n"), argv[0]), ACE_OS::exit (1);

  return ::ace_yyparse (&param);
}
#endif /* DEBUGGING */
#endif  /* ACE_USES_CLASSIC_SVC_CONF == 1 */
