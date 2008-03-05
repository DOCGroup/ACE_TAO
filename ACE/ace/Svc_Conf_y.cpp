/* A Bison parser, made by GNU Bison 2.0.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with ace_yy or ACE_YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define ACE_YYBISON 1

/* Skeleton name.  */
#define ACE_YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define ACE_YYPURE 1

/* Using locations.  */
#define ACE_YYLSP_NEEDED 0

/* On HP-UX aC++ on Itanium, disable warning 2111, statement is unreachable. */
#if defined (__HP_aCC) && defined (__ia64)
#  pragma diag_suppress 2111
#endif

/* Tokens.  */
#ifndef ACE_YYTOKENTYPE
# define ACE_YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum ace_yytokentype {
     ACE_DYNAMIC = 258,
     ACE_STATIC = 259,
     ACE_SUSPEND = 260,
     ACE_RESUME = 261,
     ACE_REMOVE = 262,
     ACE_USTREAM = 263,
     ACE_MODULE_T = 264,
     ACE_STREAM_T = 265,
     ACE_SVC_OBJ_T = 266,
     ACE_ACTIVE = 267,
     ACE_INACTIVE = 268,
     ACE_PATHNAME = 269,
     ACE_IDENT = 270,
     ACE_STRING = 271
   };
#endif
#define ACE_DYNAMIC 258
#define ACE_STATIC 259
#define ACE_SUSPEND 260
#define ACE_RESUME 261
#define ACE_REMOVE 262
#define ACE_USTREAM 263
#define ACE_MODULE_T 264
#define ACE_STREAM_T 265
#define ACE_SVC_OBJ_T 266
#define ACE_ACTIVE 267
#define ACE_INACTIVE 268
#define ACE_PATHNAME 269
#define ACE_IDENT 270
#define ACE_STRING 271




/* Copy the first part of user declarations.  */


// $Id$

#include "ace/Svc_Conf.h"

#if (ACE_USES_CLASSIC_SVC_CONF == 1)

#include "ace/ARGV.h"
#include "ace/Module.h"
#include "ace/Stream.h"
#include "ace/Service_Types.h"
#include "ace/OS_NS_string.h"


#include "ace/ace_wchar.h"

ACE_RCSID (ace,
           Svc_Conf_y,
           "$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Prototypes.

static ACE_Module_Type *ace_get_module (const ACE_Service_Type *sr,
                                         const ACE_Service_Type *sv,
                                         int & ace_ace_yyerrno);

static ACE_Module_Type *ace_get_module (const ACE_Service_Type *sr,
                                        const ACE_TCHAR *svc_name,
                                         int & ace_ace_yyerrno);

#define ACE_YYDEBUG_LEXER_TEXT (ace_yytext[ace_yyleng] = '\0', ace_yytext)

// Force the pretty debugging code to compile.
// #define ACE_YYDEBUG 1

ACE_END_VERSIONED_NAMESPACE_DECL


/* Enabling traces.  */
#ifndef ACE_YYDEBUG
# define ACE_YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef ACE_YYERROR_VERBOSE
# undef ACE_YYERROR_VERBOSE
# define ACE_YYERROR_VERBOSE 1
#else
# define ACE_YYERROR_VERBOSE 0
#endif

#if ! defined (ACE_YYSTYPE) && ! defined (ACE_YYSTYPE_IS_DECLARED)
typedef int ACE_YYSTYPE;
# define ace_yystype ACE_YYSTYPE /* obsolescent; will be withdrawn */
# define ACE_YYSTYPE_IS_DECLARED 1
# define ACE_YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 213 of yacc.c.  */


#if ! defined (ace_yyoverflow) || ACE_YYERROR_VERBOSE

# ifndef ACE_YYFREE
#  define ACE_YYFREE free
# endif
# ifndef ACE_YYMALLOC
#  define ACE_YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef ACE_YYSTACK_USE_ALLOCA
#  if ACE_YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define ACE_YYSTACK_ALLOC __builtin_alloca
#   else
#    define ACE_YYSTACK_ALLOC alloca
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
#  define ACE_YYSTACK_ALLOC ACE_YYMALLOC
#  define ACE_YYSTACK_FREE ACE_YYFREE
# endif
#endif /* ! defined (ace_yyoverflow) || ACE_YYERROR_VERBOSE */


#if (! defined (ace_yyoverflow) \
     && (! defined (__cplusplus) \
   || (defined (ACE_YYSTYPE_IS_TRIVIAL) && ACE_YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union ace_yyalloc
{
  short int ace_yyss;
  ACE_YYSTYPE ace_yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define ACE_YYSTACK_GAP_MAXIMUM (sizeof (union ace_yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define ACE_YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (ACE_YYSTYPE)) \
      + ACE_YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef ACE_YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define ACE_YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define ACE_YYCOPY(To, From, Count)                    \
      do                                                  \
        {                                                 \
          register ACE_YYSIZE_T ace_yyi;                  \
          for (ace_yyi = 0; ace_yyi < (Count); ace_yyi++) \
            (To)[ace_yyi] = (From)[ace_yyi];              \
        }                                                 \
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables ACE_YYSIZE and ACE_YYSTACKSIZE give the old and new number of
   elements in the stack, and ACE_YYPTR gives the new location of the
   stack.  Advance ACE_YYPTR to a properly aligned location for the next
   stack.  */
# define ACE_YYSTACK_RELOCATE(Stack)                                                  \
    do                                                                                \
      {                                                                               \
        ACE_YYSIZE_T ace_yynewbytes;                                                  \
        ACE_YYCOPY (&ace_yyptr->Stack, Stack, ace_yysize);                            \
        Stack = &ace_yyptr->Stack;                                                    \
        ace_yynewbytes = ace_yystacksize * sizeof (*Stack) + ACE_YYSTACK_GAP_MAXIMUM; \
        ace_yyptr += ace_yynewbytes / sizeof (*ace_yyptr);                            \
      }                                                                               \
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char ace_yysigned_char;
#else
   typedef short int ace_yysigned_char;
#endif

/* ACE_YYFINAL -- State number of the termination state. */
#define ACE_YYFINAL  2
/* ACE_YYLAST -- Last index in ACE_YYTABLE.  */
#define ACE_YYLAST   62

/* ACE_YYNTOKENS -- Number of terminals. */
#define ACE_YYNTOKENS  23
/* ACE_YYNNTS -- Number of nonterminals. */
#define ACE_YYNNTS  21
/* ACE_YYNRULES -- Number of rules. */
#define ACE_YYNRULES  45
/* ACE_YYNRULES -- Number of states. */
#define ACE_YYNSTATES  66

/* ACE_YYTRANSLATE(ACE_YYLEX) -- Bison symbol number corresponding to ACE_YYLEX.  */
#define ACE_YYUNDEFTOK  2
#define ACE_YYMAXUTOK   271

#define ACE_YYTRANSLATE(ACE_YYX)  \
  ((unsigned int) (ACE_YYX) <= ACE_YYMAXUTOK ? ace_yytranslate[ACE_YYX] : ACE_YYUNDEFTOK)

/* ACE_YYTRANSLATE[ACE_YYLEX] -- Bison symbol number corresponding to ACE_YYLEX.  */
static const unsigned char ace_yytranslate[] =
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16
};

#if ACE_YYDEBUG
/* ACE_YYPRHS[ACE_YYN] -- Index of the first RHS symbol of rule number ACE_YYN in
   ACE_YYRHS.  */
static const unsigned char ace_yyprhs[] =
{
       0,     0,     3,     6,     9,    10,    12,    14,    16,    18,
      20,    22,    26,    30,    33,    36,    39,    43,    44,    49,
      51,    53,    54,    59,    60,    63,    64,    66,    68,    70,
      72,    74,    79,    81,    83,    84,    88,    94,    99,   102,
     105,   108,   110,   111,   113,   115
};

/* ACE_YYRHS -- A `-1'-separated list of the rules' RHS. */
static const ace_yysigned_char ace_yyrhs[] =
{
      24,     0,    -1,    24,    25,    -1,    24,     1,    -1,    -1,
      26,    -1,    27,    -1,    28,    -1,    29,    -1,    30,    -1,
      31,    -1,     3,    38,    42,    -1,     4,    15,    42,    -1,
       5,    15,    -1,     6,    15,    -1,     7,    15,    -1,     8,
      33,    34,    -1,    -1,     8,    15,    32,    34,    -1,    26,
      -1,    27,    -1,    -1,    17,    35,    36,    18,    -1,    -1,
      36,    37,    -1,    -1,    26,    -1,    27,    -1,    28,    -1,
      29,    -1,    30,    -1,    15,    41,    40,    39,    -1,    12,
      -1,    13,    -1,    -1,    43,    19,    15,    -1,    43,    19,
      15,    20,    21,    -1,    19,    15,    20,    21,    -1,     9,
      22,    -1,    11,    22,    -1,    10,    22,    -1,    16,    -1,
      -1,    14,    -1,    15,    -1,    16,    -1
};

/* ACE_YYRLINE[ACE_YYN] -- source line where rule number ACE_YYN was defined.  */
static const unsigned short int ace_yyrline[] =
{
       0,    62,    62,    71,    75,    79,    80,    81,    82,    83,
      84,    88,    98,   105,   112,   119,   126,   130,   130,   137,
     140,   147,   146,   155,   159,   167,   171,   196,   210,   219,
     228,   250,   257,   261,   266,   272,   276,   280,   287,   291,
     295,   302,   303,   307,   308,   309
};
#endif

#if ACE_YYDEBUG || ACE_YYERROR_VERBOSE
/* ACE_YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at ACE_YYNTOKENS, nonterminals. */
static const char *const ace_yytname[] =
{
  "$end", "error", "$undefined", "ACE_DYNAMIC", "ACE_STATIC",
  "ACE_SUSPEND", "ACE_RESUME", "ACE_REMOVE", "ACE_USTREAM", "ACE_MODULE_T",
  "ACE_STREAM_T", "ACE_SVC_OBJ_T", "ACE_ACTIVE", "ACE_INACTIVE",
  "ACE_PATHNAME", "ACE_IDENT", "ACE_STRING", "'{'", "'}'", "':'", "'('",
  "')'", "'*'", "$accept", "svc_config_entries", "svc_config_entry",
  "dynamic", "static", "suspend", "resume", "remove", "stream", "@1",
  "stream_ops", "stream_modules", "@2", "module_list", "module",
  "svc_location", "status", "svc_initializer", "type", "parameters_opt",
  "pathname", 0
};
#endif

# ifdef ACE_YYPRINT
/* ACE_YYTOKNUM[ACE_YYLEX-NUM] -- Internal token number corresponding to
   token ACE_YYLEX-NUM.  */
static const unsigned short int ace_yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   123,   125,    58,
      40,    41,    42
};
# endif

/* ACE_YYR1[ACE_YYN] -- Symbol number of symbol that rule ACE_YYN derives.  */
static const unsigned char ace_yyr1[] =
{
       0,    23,    24,    24,    24,    25,    25,    25,    25,    25,
      25,    26,    27,    28,    29,    30,    31,    32,    31,    33,
      33,    35,    34,    34,    36,    36,    37,    37,    37,    37,
      37,    38,    39,    39,    39,    40,    40,    40,    41,    41,
      41,    42,    42,    43,    43,    43
};

/* ACE_YYR2[ACE_YYN] -- Number of symbols composing right hand side of rule ACE_YYN.  */
static const unsigned char ace_yyr2[] =
{
       0,     2,     2,     2,     0,     1,     1,     1,     1,     1,
       1,     3,     3,     2,     2,     2,     3,     0,     4,     1,
       1,     0,     4,     0,     2,     0,     1,     1,     1,     1,
       1,     4,     1,     1,     0,     3,     5,     4,     2,     2,
       2,     1,     0,     1,     1,     1
};

/* ACE_YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when ACE_YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char ace_yydefact[] =
{
       4,     0,     1,     3,     0,     0,     0,     0,     0,     0,
       2,     5,     6,     7,     8,     9,    10,     0,    42,    42,
      13,    14,    15,    17,    19,    20,    23,     0,     0,     0,
       0,    41,    11,    12,    23,    21,    16,    38,    40,    39,
      43,    44,    45,     0,    34,     0,    18,    25,     0,    32,
      33,    31,     0,     0,     0,    35,    22,    26,    27,    28,
      29,    30,    24,    37,     0,    36
};

/* ACE_YYDEFGOTO[NTERM-NUM]. */
static const ace_yysigned_char ace_yydefgoto[] =
{
      -1,     1,    10,    11,    12,    13,    14,    15,    16,    34,
      26,    36,    47,    53,    62,    18,    51,    44,    30,    32,
      45
};

/* ACE_YYPACT[STATE-NUM] -- Index in ACE_YYTABLE of the portion describing
   STATE-NUM.  */
#define ACE_YYPACT_NINF -13
static const ace_yysigned_char ace_yypact[] =
{
     -13,    20,   -13,   -13,     1,     3,     7,    14,    18,     4,
     -13,   -13,   -13,   -13,   -13,   -13,   -13,    21,    19,    19,
     -13,   -13,   -13,   -13,   -13,   -13,    -2,    12,    15,    16,
      -5,   -13,   -13,   -13,    -2,   -13,   -13,   -13,   -13,   -13,
     -13,   -13,   -13,    24,     0,    17,   -13,   -13,    22,   -13,
     -13,   -13,    25,    -1,    26,    23,   -13,   -13,   -13,   -13,
     -13,   -13,   -13,   -13,    27,   -13
};

/* ACE_YYPGOTO[NTERM-NUM].  */
static const ace_yysigned_char ace_yypgoto[] =
{
     -13,   -13,   -13,    -9,    -8,   -12,    -7,    -4,   -13,   -13,
     -13,    28,   -13,   -13,   -13,   -13,   -13,   -13,   -13,    31,
     -13
};

/* ACE_YYTABLE[ACE_YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what ACE_YYDEFACT says.
   If ACE_YYTABLE_NINF, syntax error.  */
#define ACE_YYTABLE_NINF -1
static const unsigned char ace_yytable[] =
{
      24,    25,     4,     5,     6,     7,     8,     4,     5,    40,
      41,    42,    49,    50,    43,    35,    17,    56,    19,    23,
       2,     3,    20,     4,     5,     6,     7,     8,     9,    21,
      27,    28,    29,    22,    37,    31,    52,    38,    39,    48,
      55,    59,    54,    64,    57,    58,    60,    63,    65,    61,
      33,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    46
};

static const ace_yysigned_char ace_yycheck[] =
{
       9,     9,     3,     4,     5,     6,     7,     3,     4,    14,
      15,    16,    12,    13,    19,    17,    15,    18,    15,    15,
       0,     1,    15,     3,     4,     5,     6,     7,     8,    15,
       9,    10,    11,    15,    22,    16,    19,    22,    22,    15,
      15,    53,    20,    20,    53,    53,    53,    21,    21,    53,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34
};

/* ACE_YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char ace_yystos[] =
{
       0,    24,     0,     1,     3,     4,     5,     6,     7,     8,
      25,    26,    27,    28,    29,    30,    31,    15,    38,    15,
      15,    15,    15,    15,    26,    27,    33,     9,    10,    11,
      41,    16,    42,    42,    32,    17,    34,    22,    22,    22,
      14,    15,    16,    19,    40,    43,    34,    35,    15,    12,
      13,    39,    19,    36,    20,    15,    18,    26,    27,    28,
      29,    30,    37,    21,    20,    21
};

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

#define ace_yyerrok   (ace_yyerrstatus = 0)
#define ace_yyclearin (ace_yychar = ACE_YYEMPTY)
#define ACE_YYEMPTY   (-2)
#define ACE_YYEOF     0

#define ACE_YYACCEPT  goto ace_yyacceptlab
#define ACE_YYABORT   goto ace_yyabortlab
#define ACE_YYERROR   goto ace_yyerrorlab


/* Like ACE_YYERROR except do call ace_yyerror.  This remains here temporarily
   to ease the transition to the new meaning of ACE_YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define ACE_YYFAIL    goto ace_yyerrlab

#define ACE_YYRECOVERING()  (!!ace_yyerrstatus)

#define ACE_YYBACKUP(Token, Value)                   \
  do                                                 \
    if (ace_yychar == ACE_YYEMPTY && ace_yylen == 1) \
      {                                              \
        ace_yychar = (Token);                        \
        ace_yylval = (Value);                        \
        ace_yytoken = ACE_YYTRANSLATE (ace_yychar);  \
        ACE_YYPOPSTACK;                              \
        goto ace_yybackup;                           \
      }                                              \
    else                                             \
      {                                              \
        ace_yyerror (ACE_SVC_CONF_PARAM->yyerrno, ACE_SVC_CONF_PARAM->yylineno, "syntax error: cannot back up");\
        ACE_YYERROR;                                 \
      }                                              \
  while (0)


#define ACE_YYTERROR  1
#define ACE_YYERRCODE 256


/* ACE_YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define ACE_YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef ACE_YYLLOC_DEFAULT
# define ACE_YYLLOC_DEFAULT(Current, Rhs, N)                            \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = ACE_YYRHSLOC (Rhs, 1).first_line;    \
          (Current).first_column = ACE_YYRHSLOC (Rhs, 1).first_column;  \
          (Current).last_line    = ACE_YYRHSLOC (Rhs, N).last_line;     \
          (Current).last_column  = ACE_YYRHSLOC (Rhs, N).last_column;   \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            ACE_YYRHSLOC (Rhs, 0).last_line;                            \
          (Current).first_column = (Current).last_column =              \
            ACE_YYRHSLOC (Rhs, 0).last_column;                          \
        }                                                               \
    while (0)
#endif


/* ACE_YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef ACE_YY_LOCATION_PRINT
# if ACE_YYLTYPE_IS_TRIVIAL
#  define ACE_YY_LOCATION_PRINT(File, Loc)                  \
     ACE_OS::fprintf (File, "%d.%d-%d.%d",                  \
                      (Loc).first_line, (Loc).first_column, \
                      (Loc).last_line,  (Loc).last_column)
# else
#  define ACE_YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* ACE_YYLEX -- calling `ace_yylex' with the right arguments.  */

#ifdef ACE_YYLEX_PARAM
# define ACE_YYLEX ace_yylex (&ace_yylval, ACE_YYLEX_PARAM)
#else
# define ACE_YYLEX ace_yylex (&ace_yylval)
#endif

/* Enable debugging if requested.  */
#if ACE_YYDEBUG

# ifndef ACE_YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define ACE_YYFPRINTF ACE_OS::fprintf
# endif

# define ACE_YYDPRINTF(Args) \
  do                         \
    {                        \
      if (ace_yydebug)       \
        ACE_YYFPRINTF Args;  \
    }                        \
  while (0)

# define ACE_YY_SYMBOL_PRINT(Title, Type, Value, Location)  \
  do                                                        \
    {                                                       \
      if (ace_yydebug)                                      \
        {                                                   \
          ACE_YYFPRINTF (stderr, "%s ", Title);             \
          ace_yysymprint (stderr,                           \
                          Type, Value);                     \
          ACE_YYFPRINTF (stderr, "\n");                     \
        }                                                   \
    }                                                       \
  while (0)

/*------------------------------------------------------------------.
| ace_yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
ace_yy_stack_print (short int *bottom, short int *top)
#else
static void
ace_yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  ACE_YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    ACE_YYFPRINTF (stderr, " %d", *bottom);
  ACE_YYFPRINTF (stderr, "\n");
}

# define ACE_YY_STACK_PRINT(Bottom, Top)      \
  do                                          \
    {                                         \
      if (ace_yydebug)                        \
        ace_yy_stack_print ((Bottom), (Top)); \
    }                                         \
  while (0)


/*------------------------------------------------.
| Report that the ACE_YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
ace_yy_reduce_print (int ace_yyrule)
#else
static void
ace_yy_reduce_print (ace_yyrule)
    int ace_yyrule;
#endif
{
  int ace_yyi;
  unsigned int ace_yylno = ace_yyrline[ace_yyrule];
  ACE_YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             ace_yyrule - 1, ace_yylno);
  /* Print the symbols being reduced, and their result.  */
  for (ace_yyi = ace_yyprhs[ace_yyrule]; 0 <= ace_yyrhs[ace_yyi]; ace_yyi++)
    ACE_YYFPRINTF (stderr, "%s ", ace_yytname [ace_yyrhs[ace_yyi]]);
  ACE_YYFPRINTF (stderr, "-> %s\n", ace_yytname [ace_yyr1[ace_yyrule]]);
}

# define ACE_YY_REDUCE_PRINT(Rule)  \
  do                                \
    {                               \
      if (ace_yydebug)              \
        ace_yy_reduce_print (Rule); \
    }                               \
  while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int ace_yydebug;
#else /* !ACE_YYDEBUG */
# define ACE_YYDPRINTF(Args)
# define ACE_YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define ACE_YY_STACK_PRINT(Bottom, Top)
# define ACE_YY_REDUCE_PRINT(Rule)
#endif /* !ACE_YYDEBUG */


/* ACE_YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef  ACE_YYINITDEPTH
# define ACE_YYINITDEPTH 200
#endif

/* ACE_YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < ACE_YYSTACK_BYTES (ACE_YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef ACE_YYMAXDEPTH
# define ACE_YYMAXDEPTH 10000
#endif



#if ACE_YYERROR_VERBOSE

# ifndef ace_yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define ace_yystrlen strlen
#  else
/* Return the length of ACE_YYSTR.  */
static ACE_YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
ace_yystrlen (const char *ace_yystr)
#   else
ace_yystrlen (ace_yystr)
     const char *ace_yystr;
#   endif
{
  register const char *ace_yys = ace_yystr;

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
ace_yystpcpy (ACE_TCHAR *ace_yydest, const char *ace_yysrc)
#   else
ace_yystpcpy (ace_yydest, ace_yysrc)
     char *ace_yydest;
     const char *ace_yysrc;
#   endif
{
  register char *ace_yyd = ace_yydest;
  register const char *ace_yys = ace_yysrc;

  while ((*ace_yyd++ = *ace_yys++) != '\0')
    continue;

  return ace_yyd - 1;
}
#  endif
# endif

#endif /* !ACE_YYERROR_VERBOSE */



#if ACE_YYDEBUG
/*--------------------------------.
| Print this symbol on ACE_YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
ace_yysymprint (FILE *ace_yyoutput, int ace_yytype, ACE_YYSTYPE *ace_yyvaluep)
#else
static void
ace_yysymprint (ace_yyoutput, ace_yytype, ace_yyvaluep)
    FILE *ace_yyoutput;
    int ace_yytype;
    ACE_YYSTYPE *ace_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) ace_yyvaluep;

  if (ace_yytype < ACE_YYNTOKENS)
    ACE_YYFPRINTF (ace_yyoutput, "token %s (", ace_yytname[ace_yytype]);
  else
    ACE_YYFPRINTF (ace_yyoutput, "nterm %s (", ace_yytname[ace_yytype]);


# ifdef ACE_YYPRINT
  if (ace_yytype < ACE_YYNTOKENS)
    ACE_YYPRINT (ace_yyoutput, ace_yytoknum[ace_yytype], *ace_yyvaluep);
# endif
  switch (ace_yytype)
    {
      default:
        break;
    }
  ACE_YYFPRINTF (ace_yyoutput, ")");
}

#endif /* ! ACE_YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
ace_yydestruct (const char *ace_yymsg, int ace_yytype, ACE_YYSTYPE *ace_yyvaluep)
#else
static void
ace_yydestruct (ace_yymsg, ace_yytype, ace_yyvaluep)
    const char *ace_yymsg;
    int ace_yytype;
    ACE_YYSTYPE *ace_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) ace_yyvaluep;
  (void) ace_yytype;

  if (!ace_yymsg)
    ace_yymsg = "Deleting";
  ACE_YY_SYMBOL_PRINT (ace_yymsg, ace_yytype, ace_yyvaluep, ace_yylocationp);
}


ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef ACE_YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int ace_yyparse (void *ACE_YYPARSE_PARAM);
# else
int ace_yyparse ();
# endif
#else /* ! ACE_YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int ace_yyparse (void);
#else
int ace_yyparse ();
#endif
#endif /* ! ACE_YYPARSE_PARAM */


/*----------.
| ace_yyparse.  |
`----------*/

#ifdef ACE_YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int ace_yyparse (void *ACE_YYPARSE_PARAM)
# else
int ace_yyparse (ACE_YYPARSE_PARAM)
  void *ACE_YYPARSE_PARAM;
# endif
#else /* ! ACE_YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
ace_yyparse (void)
#else
int
ace_yyparse ()

#endif
#endif
{
  /* The look-ahead symbol.  */
int ace_yychar;

/* The semantic value of the look-ahead symbol.  */
ACE_YYSTYPE ace_yylval;

#if defined (ACE_YYSTYPE_IS_TRIVIAL)
  ace_yylval = 0;
#else
  ace_yylval.type_ = 0;
#endif /* ACE_YYSTYPE_IS_TRIVIAL */

/* Number of syntax errors so far.  */
int ace_yynerrs;

  register int ace_yystate;
  register int ace_yyn;
  int ace_yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int ace_yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int ace_yytoken = 0;

  /* Three stacks and their tools:
     `ace_yyss': related to states,
     `ace_yyvs': related to semantic values,
     `ace_yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow ace_yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int ace_yyssa[ACE_YYINITDEPTH];
  short int *ace_yyss = ace_yyssa;
  register short int *ace_yyssp;

  /* The semantic value stack.  */
  ACE_YYSTYPE ace_yyvsa[ACE_YYINITDEPTH];
  ACE_YYSTYPE *ace_yyvs = ace_yyvsa;
  register ACE_YYSTYPE *ace_yyvsp;



#define ACE_YYPOPSTACK   (ace_yyvsp--, ace_yyssp--)

  ACE_YYSIZE_T ace_yystacksize = ACE_YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  ACE_YYSTYPE ace_yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int ace_yylen;

  ACE_YYDPRINTF ((stderr, "Starting parse\n"));

  ace_yystate = 0;
  ace_yyerrstatus = 0;
  ace_yynerrs = 0;
  ace_yychar = ACE_YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  ace_yyssp = ace_yyss;
  ace_yyvsp = ace_yyvs;


  ace_yyvsp[0] = ace_yylval;

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

  if (ace_yyss + ace_yystacksize - 1 <= ace_yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      ACE_YYSIZE_T ace_yysize = ace_yyssp - ace_yyss + 1;

#ifdef ace_yyoverflow
      {
  /* Give user a chance to reallocate the stack. Use copies of
     these so that the &'s don't force the real ones into
     memory.  */
  ACE_YYSTYPE *ace_yyvs1 = ace_yyvs;
  short int *ace_yyss1 = ace_yyss;


  /* Each stack pointer address is followed by the size of the
     data in use in that stack, in bytes.  This used to be a
     conditional around just the two extra args, but that might
     be undefined if ace_yyoverflow is a macro.  */
  ace_yyoverflow ("parser stack overflow",
        &ace_yyss1, ace_yysize * sizeof (*ace_yyssp),
        &ace_yyvs1, ace_yysize * sizeof (*ace_yyvsp),

        &ace_yystacksize);

  ace_yyss = ace_yyss1;
  ace_yyvs = ace_yyvs1;
      }
#else /* no ace_yyoverflow */
# ifndef ACE_YYSTACK_RELOCATE
      goto ace_yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (ACE_YYMAXDEPTH <= ace_yystacksize)
  goto ace_yyoverflowlab;
      ace_yystacksize *= 2;
      if (ACE_YYMAXDEPTH < ace_yystacksize)
  ace_yystacksize = ACE_YYMAXDEPTH;

      {
  short int *ace_yyss1 = ace_yyss;
  union ace_yyalloc *ace_yyptr =
    (union ace_yyalloc *) ACE_YYSTACK_ALLOC (ACE_YYSTACK_BYTES (ace_yystacksize));
  if (! ace_yyptr)
    goto ace_yyoverflowlab;
  ACE_YYSTACK_RELOCATE (ace_yyss);
  ACE_YYSTACK_RELOCATE (ace_yyvs);

#  undef ACE_YYSTACK_RELOCATE
  if (ace_yyss1 != ace_yyssa)
    ACE_YYSTACK_FREE (ace_yyss1);
      }
# endif
#endif /* no ace_yyoverflow */

      ace_yyssp = ace_yyss + ace_yysize - 1;
      ace_yyvsp = ace_yyvs + ace_yysize - 1;


      ACE_YYDPRINTF ((stderr, "Stack size increased to %lu\n",
      (unsigned long int) ace_yystacksize));

      if (ace_yyss + ace_yystacksize - 1 <= ace_yyssp)
  ACE_YYABORT;
    }

  ACE_YYDPRINTF ((stderr, "Entering state %d\n", ace_yystate));

  goto ace_yybackup;

/*-----------.
| ace_yybackup.  |
`-----------*/
ace_yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* ace_yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  ace_yyn = ace_yypact[ace_yystate];
  if (ace_yyn == ACE_YYPACT_NINF)
    goto ace_yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* ACE_YYCHAR is either ACE_YYEMPTY or ACE_YYEOF or a valid look-ahead symbol.  */
  if (ace_yychar == ACE_YYEMPTY)
    {
      ACE_YYDPRINTF ((stderr, "Reading a token: "));
      ace_yychar = ACE_YYLEX;
    }

  if (ace_yychar <= ACE_YYEOF)
    {
      ace_yychar = ace_yytoken = ACE_YYEOF;
      ACE_YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      ace_yytoken = ACE_YYTRANSLATE (ace_yychar);
      ACE_YY_SYMBOL_PRINT ("Next token is", ace_yytoken, &ace_yylval, &ace_yylloc);
    }

  /* If the proper action on seeing token ACE_YYTOKEN is to reduce or to
     detect an error, take that action.  */
  ace_yyn += ace_yytoken;
  if (ace_yyn < 0 || ACE_YYLAST < ace_yyn || ace_yycheck[ace_yyn] != ace_yytoken)
    goto ace_yydefault;
  ace_yyn = ace_yytable[ace_yyn];
  if (ace_yyn <= 0)
    {
      if (ace_yyn == 0 || ace_yyn == ACE_YYTABLE_NINF)
  goto ace_yyerrlab;
      ace_yyn = -ace_yyn;
      goto ace_yyreduce;
    }

  if (ace_yyn == ACE_YYFINAL)
    ACE_YYACCEPT;

  /* Shift the look-ahead token.  */
  ACE_YY_SYMBOL_PRINT ("Shifting", ace_yytoken, &ace_yylval, &ace_yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (ace_yychar != ACE_YYEOF)
    ace_yychar = ACE_YYEMPTY;

  *++ace_yyvsp = ace_yylval;


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

     Otherwise, the following line sets ACE_YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to ACE_YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that ACE_YYVAL may be used uninitialized.  */
  ace_yyval = ace_yyvsp[1-ace_yylen];


  ACE_YY_REDUCE_PRINT (ace_yyn);
  switch (ace_yyn)
    {
        case 2:

    {
      if ((ace_yyvsp[0].parse_node_) != 0)
      {
        (ace_yyvsp[0].parse_node_)->apply (ACE_SVC_CONF_PARAM->config, ACE_SVC_CONF_PARAM->yyerrno);
        delete (ace_yyvsp[0].parse_node_);
      }
      ACE_SVC_CONF_PARAM->obstack.release ();
    ;}
    break;

  case 3:

    {
      ACE_SVC_CONF_PARAM->obstack.release ();
    ;}
    break;

  case 11:

    {
      if ((ace_yyvsp[-1].svc_record_) != 0)
        (ace_yyval.parse_node_) = new ACE_Dynamic_Node ((ace_yyvsp[-1].svc_record_), (ace_yyvsp[0].ident_));
      else
        (ace_yyval.parse_node_) = 0;
    ;}
    break;

  case 12:

    {
      (ace_yyval.parse_node_) = new ACE_Static_Node ((ace_yyvsp[-1].ident_), (ace_yyvsp[0].ident_));
    ;}
    break;

  case 13:

    {
      (ace_yyval.parse_node_) = new ACE_Suspend_Node ((ace_yyvsp[0].ident_));
    ;}
    break;

  case 14:

    {
      (ace_yyval.parse_node_) = new ACE_Resume_Node ((ace_yyvsp[0].ident_));
    ;}
    break;

  case 15:

    {
      (ace_yyval.parse_node_) = new ACE_Remove_Node ((ace_yyvsp[0].ident_));
    ;}
    break;

  case 16:

    {
      (ace_yyval.parse_node_) = new ACE_Stream_Node ((ace_yyvsp[-1].static_node_), (ace_yyvsp[0].parse_node_));
    ;}
    break;

  case 17:

    { (ace_yyval.static_node_) = new ACE_Static_Node ((ace_yyvsp[0].ident_)); ;}
    break;

  case 18:

    {
      (ace_yyval.parse_node_) = new ACE_Dummy_Node ((ace_yyvsp[-1].static_node_), (ace_yyvsp[0].parse_node_));
    ;}
    break;

  case 19:

    {
    ;}
    break;

  case 20:

    {
    ;}
    break;

  case 21:

    {
      // Initialize left context...
      (ace_yyval.static_node_) = (ace_yyvsp[-1].static_node_);
    ;}
    break;

  case 22:

    {
      (ace_yyval.parse_node_) = (ace_yyvsp[-1].parse_node_);
    ;}
    break;

  case 23:

    { (ace_yyval.parse_node_) = 0; ;}
    break;

  case 24:

    {
      if ((ace_yyvsp[0].parse_node_) != 0)
        {
          (ace_yyvsp[0].parse_node_)->link ((ace_yyvsp[-1].parse_node_));
          (ace_yyval.parse_node_) = (ace_yyvsp[0].parse_node_);
        }
    ;}
    break;

  case 25:

    { (ace_yyval.parse_node_) = 0; ;}
    break;

  case 26:

    {
      ACE_Static_Node *svc_type = (ace_yyvsp[0].static_node_);

      if (svc_type != 0)
        {
          ACE_Static_Node *module = (ace_yyvsp[-2].static_node_);

          ACE_ARGV args (svc_type->parameters ());
          ACE_Module_Type *mt = ace_get_module (module->record (ACE_SVC_CONF_PARAM->config),
                                                svc_type->record (ACE_SVC_CONF_PARAM->config),
                                                ACE_SVC_CONF_PARAM->yyerrno);
          ACE_Stream_Type *st =
            dynamic_cast<ACE_Stream_Type *> (const_cast<ACE_Service_Type_Impl *> (module->record (ACE_SVC_CONF_PARAM->config)->type ()));

          if (!st
              || !mt
              || mt->init (args.argc (), args.argv ()) == -1
              || st->push (mt) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("dynamic initialization failed for Module %s\n"),
                          svc_type->name ()));
              ACE_SVC_CONF_PARAM->yyerrno++;
            }
        }
    ;}
    break;

  case 27:

    {
      ACE_Static_Node *sn = (ace_yyvsp[-2].static_node_);
      ACE_Module_Type *mt = ace_get_module (sn->record (ACE_SVC_CONF_PARAM->config),
                                            (ace_yyvsp[0].static_node_)->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);

      if (((ACE_Stream_Type *) sn->record (ACE_SVC_CONF_PARAM->config)->type ())->push (mt) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Problem with static\n")));
          ACE_SVC_CONF_PARAM->yyerrno++;
        }
    ;}
    break;

  case 28:

    {
      ACE_Static_Node *sn = (ace_yyvsp[-2].static_node_);
      ACE_Module_Type *mt = ace_get_module (sn->record (ACE_SVC_CONF_PARAM->config),
                                            sn->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);
      if (mt != 0)
        mt->suspend ();
    ;}
    break;

  case 29:

    {
      ACE_Static_Node *sn = (ace_yyvsp[-2].static_node_);
      ACE_Module_Type *mt = ace_get_module (sn->record (ACE_SVC_CONF_PARAM->config),
                                            (ace_yyvsp[0].static_node_)->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);
      if (mt != 0)
        mt->resume ();
    ;}
    break;

  case 30:

    {
      ACE_Static_Node *stream = (ace_yyvsp[-2].static_node_);
      ACE_Static_Node *module = (ace_yyvsp[0].static_node_);
      ACE_Module_Type *mt = ace_get_module (stream->record (ACE_SVC_CONF_PARAM->config),
                                            module->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);

      ACE_Stream_Type *st =
        dynamic_cast<ACE_Stream_Type *> (const_cast<ACE_Service_Type_Impl *> (stream->record (ACE_SVC_CONF_PARAM->config)->type ()));
      if (!st || (mt != 0 && st->remove (mt) == -1))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("cannot remove Module_Type %s from STREAM_Type %s\n"),
                      module->name (),
                      stream->name ()));
          ACE_SVC_CONF_PARAM->yyerrno++;
        }
    ;}
    break;

  case 31:

    {
      (ace_yyval.svc_record_) = new ACE_Service_Type_Factory ((ace_yyvsp[-3].ident_), (ace_yyvsp[-2].type_), (ace_yyvsp[-1].location_node_), (ace_yyvsp[0].type_));
    ;}
    break;

  case 32:

    {
      (ace_yyval.type_) = 1;
    ;}
    break;

  case 33:

    {
      (ace_yyval.type_) = 0;
    ;}
    break;

  case 34:

    {
      (ace_yyval.type_) = 1;
    ;}
    break;

  case 35:

    {
      (ace_yyval.location_node_) = new ACE_Object_Node ((ace_yyvsp[-2].ident_), (ace_yyvsp[0].ident_));
    ;}
    break;

  case 36:

    {
      (ace_yyval.location_node_) = new ACE_Function_Node ((ace_yyvsp[-4].ident_), (ace_yyvsp[-2].ident_));
    ;}
    break;

  case 37:

    {
      (ace_yyval.location_node_) = new ACE_Static_Function_Node ((ace_yyvsp[-2].ident_));
    ;}
    break;

  case 38:

    {
      (ace_yyval.type_) = ACE_MODULE_T;
    ;}
    break;

  case 39:

    {
      (ace_yyval.type_) = ACE_SVC_OBJ_T;
    ;}
    break;

  case 40:

    {
      (ace_yyval.type_) = ACE_STREAM_T;
    ;}
    break;

  case 42:

    { (ace_yyval.ident_) = 0; ;}
    break;


    }

/* Line 1037 of yacc.c.  */


  ace_yyvsp -= ace_yylen;
  ace_yyssp -= ace_yylen;


  ACE_YY_STACK_PRINT (ace_yyss, ace_yyssp);

  *++ace_yyvsp = ace_yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  ace_yyn = ace_yyr1[ace_yyn];

  ace_yystate = ace_yypgoto[ace_yyn - ACE_YYNTOKENS] + *ace_yyssp;
  if (0 <= ace_yystate && ace_yystate <= ACE_YYLAST && ace_yycheck[ace_yystate] == *ace_yyssp)
    ace_yystate = ace_yytable[ace_yystate];
  else
    ace_yystate = ace_yydefgoto[ace_yyn - ACE_YYNTOKENS];

  goto ace_yynewstate;


/*------------------------------------.
| ace_yyerrlab -- here on detecting error |
`------------------------------------*/
ace_yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!ace_yyerrstatus)
    {
      ++ace_yynerrs;
#if ACE_YYERROR_VERBOSE
      ace_yyn = ace_yypact[ace_yystate];

      if (ACE_YYPACT_NINF < ace_yyn && ace_yyn < ACE_YYLAST)
  {
    ACE_YYSIZE_T ace_yysize = 0;
    int ace_yytype = ACE_YYTRANSLATE (ace_yychar);
    const char* ace_yyprefix;
    char *ace_yymsg;
    int ace_yyx;

    /* Start ACE_YYX at -ACE_YYN if negative to avoid negative indexes in
       ACE_YYCHECK.  */
    int ace_yyxbegin = ace_yyn < 0 ? -ace_yyn : 0;

    /* Stay within bounds of both ace_yycheck and ace_yytname.  */
    int ace_yychecklim = ACE_YYLAST - ace_yyn;
    int ace_yyxend = ace_yychecklim < ACE_YYNTOKENS ? ace_yychecklim : ACE_YYNTOKENS;
    int ace_yycount = 0;

    ace_yyprefix = ", expecting ";
    for (ace_yyx = ace_yyxbegin; ace_yyx < ace_yyxend; ++ace_yyx)
      if (ace_yycheck[ace_yyx + ace_yyn] == ace_yyx && ace_yyx != ACE_YYTERROR)
        {
    ace_yysize += ace_yystrlen (ace_yyprefix) + ace_yystrlen (ace_yytname [ace_yyx]);
    ace_yycount += 1;
    if (ace_yycount == 5)
      {
        ace_yysize = 0;
        break;
      }
        }
    ace_yysize += (sizeof ("syntax error, unexpected ")
         + ace_yystrlen (ace_yytname[ace_yytype]));
    ace_yymsg = (ACE_TCHAR *) ACE_YYSTACK_ALLOC (ace_yysize);
    if (ace_yymsg != 0)
      {
        char *ace_yyp = ace_yystpcpy (ace_yymsg, "syntax error, unexpected ");
        ace_yyp = ace_yystpcpy (ace_yyp, ace_yytname[ace_yytype]);

        if (ace_yycount < 5)
    {
      ace_yyprefix = ", expecting ";
      for (ace_yyx = ace_yyxbegin; ace_yyx < ace_yyxend; ++ace_yyx)
        if (ace_yycheck[ace_yyx + ace_yyn] == ace_yyx && ace_yyx != ACE_YYTERROR)
          {
      ace_yyp = ace_yystpcpy (ace_yyp, ace_yyprefix);
      ace_yyp = ace_yystpcpy (ace_yyp, ace_yytname[ace_yyx]);
      ace_yyprefix = " or ";
          }
    }
        ace_yyerror (ACE_SVC_CONF_PARAM->yyerrno, ACE_SVC_CONF_PARAM->yylineno, ace_yymsg);
        ACE_YYSTACK_FREE (ace_yymsg);
      }
    else
      ace_yyerror (ACE_SVC_CONF_PARAM->yyerrno, ACE_SVC_CONF_PARAM->yylineno, "syntax error; also virtual memory exhausted");
  }
      else
#endif /* ACE_YYERROR_VERBOSE */
  ace_yyerror (ACE_SVC_CONF_PARAM->yyerrno, ACE_SVC_CONF_PARAM->yylineno, "syntax error");
    }



  if (ace_yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
   error, discard it.  */

      if (ace_yychar <= ACE_YYEOF)
        {
          /* If at end of input, pop the error token,
       then the rest of the stack, then return failure.  */
    if (ace_yychar == ACE_YYEOF)
       for (;;)
         {

     ACE_YYPOPSTACK;
     if (ace_yyssp == ace_yyss)
       ACE_YYABORT;
     ace_yydestruct ("Error: popping",
                             ace_yystos[*ace_yyssp], ace_yyvsp);
         }
        }
      else
  {
    ace_yydestruct ("Error: discarding", ace_yytoken, &ace_yylval);
    ace_yychar = ACE_YYEMPTY;
  }
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto ace_yyerrlab1;


/*---------------------------------------------------.
| ace_yyerrorlab -- error raised explicitly by ACE_YYERROR.  |
`---------------------------------------------------*/
ace_yyerrorlab:

#if defined (__GNUC__) || defined (ACE_WIN32) || defined (__HP_aCC) || defined (__DECCXX)
  /* Pacify GCC when the user code never invokes ACE_YYERROR and the label
     ace_yyerrorlab therefore never appears in user code.  */
  if (0)
     goto ace_yyerrorlab;
#endif

ace_yyvsp -= ace_yylen;
  ace_yyssp -= ace_yylen;
  ace_yystate = *ace_yyssp;
  goto ace_yyerrlab1;


/*-------------------------------------------------------------.
| ace_yyerrlab1 -- common code for both syntax error and ACE_YYERROR.  |
`-------------------------------------------------------------*/
ace_yyerrlab1:
  ace_yyerrstatus = 3; /* Each real token shifted decrements this.  */

  for (;;)
    {
      ace_yyn = ace_yypact[ace_yystate];
      if (ace_yyn != ACE_YYPACT_NINF)
  {
    ace_yyn += ACE_YYTERROR;
    if (0 <= ace_yyn && ace_yyn <= ACE_YYLAST && ace_yycheck[ace_yyn] == ACE_YYTERROR)
      {
        ace_yyn = ace_yytable[ace_yyn];
        if (0 < ace_yyn)
    break;
      }
  }

      /* Pop the current state because it cannot handle the error token.  */
      if (ace_yyssp == ace_yyss)
  ACE_YYABORT;


      ace_yydestruct ("Error: popping", ace_yystos[ace_yystate], ace_yyvsp);
      ACE_YYPOPSTACK;
      ace_yystate = *ace_yyssp;
      ACE_YY_STACK_PRINT (ace_yyss, ace_yyssp);
    }

  if (ace_yyn == ACE_YYFINAL)
    ACE_YYACCEPT;

  *++ace_yyvsp = ace_yylval;


  /* Shift the error token. */
  ACE_YY_SYMBOL_PRINT ("Shifting", ace_yystos[ace_yyn], ace_yyvsp, ace_yylsp);

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
  ace_yydestruct ("Error: discarding lookahead",
              ace_yytoken, &ace_yylval);
  ace_yychar = ACE_YYEMPTY;
  ace_yyresult = 1;
  goto ace_yyreturn;

#ifndef ace_yyoverflow
/*----------------------------------------------.
| ace_yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
ace_yyoverflowlab:
  ace_yyerror (ACE_SVC_CONF_PARAM->yyerrno, ACE_SVC_CONF_PARAM->yylineno, "parser stack overflow");
  ace_yyresult = 2;
  /* Fall through.  */
#endif

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
ace_yyerror (int ace_yyerrno, int ace_yylineno, char const *s)
{
#if defined (ACE_NLOGGING)
  ACE_UNUSED_ARG (ace_yyerrno);
  ACE_UNUSED_ARG (ace_yylineno);
  ACE_UNUSED_ARG (s);
#endif /* ACE_NLOGGING */

  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("ACE (%P|%t) [error %d] on line %d: %s\n"),
              ace_yyerrno,
              ace_yylineno,
              ACE_TEXT_CHAR_TO_TCHAR (s)));
}

// Note that SRC_REC represents left context, which is the STREAM *
// record.

static ACE_Module_Type *
ace_get_module (ACE_Service_Type const * sr,
                ACE_TCHAR const * svc_name,
                int & ace_yyerrno)
{
  ACE_Service_Type_Impl const * const type = sr->type ();
  ACE_Stream_Type const * const st =
    (sr == 0
     ? 0
     : dynamic_cast<ACE_Stream_Type const *> (type));
  ACE_Module_Type const * const mt = (st == 0 ? 0 : st->find (svc_name));

  if (sr == 0 || st == 0 || mt == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("cannot locate Module_Type %s ")
                  ACE_TEXT ("in STREAM_Type %s\n"),
                  svc_name,
                  (sr ? sr->name () : ACE_TEXT ("(nil)"))));
      ++ace_yyerrno;
    }

  return const_cast<ACE_Module_Type *> (mt);
}

static ACE_Module_Type *
ace_get_module (ACE_Service_Type const * sr,
                ACE_Service_Type const * sv,
                int & ace_yyerrno)
{
  ACE_Stream_Type const * const st =
    (sr == 0
     ? 0
     : static_cast<ACE_Stream_Type const *> (sr->type ()));

  ACE_Module_Type const * const mt =
    static_cast <ACE_Module_Type const *> (sv->type ());

  ACE_TCHAR const * const module_type_name =
    (mt ? mt->name () : ACE_TEXT ("(nil)"));

  if (sr == 0 || st == 0 || mt == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("cannot locate Module_Type %s or STREAM_Type %s\n"),
                  module_type_name,
                  (sr ? sr->name () : ACE_TEXT ("(nil)"))));
      ++ace_yyerrno;
    }

  // Make sure that the Module has the same name as the
  // Module_Type object from the svc.conf file.
  ACE_Module<ACE_SYNCH> * const mp =
    static_cast<ACE_Module<ACE_SYNCH> *> (mt ? mt->object () : 0);

  if (mp && ACE_OS::strcmp (mp->name (), module_type_name) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("warning: assigning Module_Type name %s to Module %s since names differ\n"),
                  module_type_name,
                  mp->name ()));
      mp->name (module_type_name);
    }

  return const_cast<ACE_Module_Type *> (mt);
}

#if defined (SVC_CONF_Y_DEBUGGING)
// Main driver program.

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_Svc_Conf_Param param (0, stdin);

  // Try to reopen any filename argument to use ACE_YYIN.
  if (argc > 1 && (ace_yyin = freopen (argv[1], "r", stdin)) == 0)
    (void) ACE_OS::fprintf (stderr, ACE_TEXT ("usage: %s [file]\n"), argv[0]), ACE_OS::exit (1);

  return ::ace_yyparse (&param);
}
#endif /* SVC_CONF_Y_DEBUGGING */

ACE_END_VERSIONED_NAMESPACE_DECL

#endif  /* ACE_USES_CLASSIC_SVC_CONF == 1 */
