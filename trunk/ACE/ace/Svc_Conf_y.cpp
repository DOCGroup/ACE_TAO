/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

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
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with ace_yy or ACE_YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define ACE_YYBISON 1

/* Bison version.  */
#define ACE_YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define ACE_YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define ACE_YYPURE 1

/* Using locations.  */
#define ACE_YYLSP_NEEDED 0



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
/* Tokens.  */
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

static ACE_Module_Type *ace_get_module (ACE_Service_Type const * sr,
                                        ACE_Service_Type const * sv,
                                        int & ace_ace_yyerrno);

static ACE_Module_Type *ace_get_module (ACE_Service_Type const * sr,
                                        ACE_TCHAR const * svc_name,
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

/* Enabling the token table.  */
#ifndef ACE_YYTOKEN_TABLE
# define ACE_YYTOKEN_TABLE 0
#endif

#if ! defined ACE_YYSTYPE && ! defined ACE_YYSTYPE_IS_DECLARED
typedef int ACE_YYSTYPE;
# define ace_yystype ACE_YYSTYPE /* obsolescent; will be withdrawn */
# define ACE_YYSTYPE_IS_DECLARED 1
# define ACE_YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */


#ifdef short
# undef short
#endif

#ifdef ACE_YYTYPE_UINT8
typedef ACE_YYTYPE_UINT8 ace_yytype_uint8;
#else
typedef unsigned char ace_yytype_uint8;
#endif

#ifdef ACE_YYTYPE_INT8
typedef ACE_YYTYPE_INT8 ace_yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char ace_yytype_int8;
#else
typedef short int ace_yytype_int8;
#endif

#ifdef ACE_YYTYPE_UINT16
typedef ACE_YYTYPE_UINT16 ace_yytype_uint16;
#else
typedef unsigned short int ace_yytype_uint16;
#endif

#ifdef ACE_YYTYPE_INT16
typedef ACE_YYTYPE_INT16 ace_yytype_int16;
#else
typedef short int ace_yytype_int16;
#endif

#ifndef ACE_YYSIZE_T
# ifdef __SIZE_TYPE__
#  define ACE_YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define ACE_YYSIZE_T size_t
# elif ! defined ACE_YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define ACE_YYSIZE_T size_t
# else
#  define ACE_YYSIZE_T unsigned int
# endif
#endif

#define ACE_YYSIZE_MAXIMUM ((ACE_YYSIZE_T) -1)

#ifndef ACE_YY_
# if ACE_YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define ACE_YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef ACE_YY_
#  define ACE_YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define ACE_YYUSE(e) ((void) (e))
#else
# define ACE_YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define ACE_YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
ACE_YYID (int i)
#else
static int
ACE_YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined ace_yyoverflow || ACE_YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef ACE_YYSTACK_USE_ALLOCA
#  if ACE_YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define ACE_YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define ACE_YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define ACE_YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef ACE_YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define ACE_YYSTACK_FREE(Ptr) do { /* empty */; } while (ACE_YYID (0))
#  ifndef ACE_YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define ACE_YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define ACE_YYSTACK_ALLOC ACE_YYMALLOC
#  define ACE_YYSTACK_FREE ACE_YYFREE
#  ifndef ACE_YYSTACK_ALLOC_MAXIMUM
#   define ACE_YYSTACK_ALLOC_MAXIMUM ACE_YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined ACE_YYMALLOC || defined malloc) \
       && (defined ACE_YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef ACE_YYMALLOC
#   define ACE_YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (ACE_YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef ACE_YYFREE
#   define ACE_YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined ace_yyoverflow || ACE_YYERROR_VERBOSE */


#if (! defined ace_yyoverflow \
     && (! defined __cplusplus \
   || (defined ACE_YYSTYPE_IS_TRIVIAL && ACE_YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union ace_yyalloc
{
  ace_yytype_int16 ace_yyss;
  ACE_YYSTYPE ace_yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define ACE_YYSTACK_GAP_MAXIMUM (sizeof (union ace_yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define ACE_YYSTACK_BYTES(N) \
     ((N) * (sizeof (ace_yytype_int16) + sizeof (ACE_YYSTYPE)) \
      + ACE_YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef ACE_YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define ACE_YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define ACE_YYCOPY(To, From, Count)    \
      do          \
  {          \
    ACE_YYSIZE_T ace_yyi;        \
    for (ace_yyi = 0; ace_yyi < (Count); ace_yyi++)  \
      (To)[ace_yyi] = (From)[ace_yyi];    \
  }          \
      while (ACE_YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables ACE_YYSIZE and ACE_YYSTACKSIZE give the old and new number of
   elements in the stack, and ACE_YYPTR gives the new location of the
   stack.  Advance ACE_YYPTR to a properly aligned location for the next
   stack.  */
# define ACE_YYSTACK_RELOCATE(Stack)          \
    do                  \
      {                  \
  ACE_YYSIZE_T ace_yynewbytes;            \
  ACE_YYCOPY (&ace_yyptr->Stack, Stack, ace_yysize);        \
  Stack = &ace_yyptr->Stack;            \
  ace_yynewbytes = ace_yystacksize * sizeof (*Stack) + ACE_YYSTACK_GAP_MAXIMUM; \
  ace_yyptr += ace_yynewbytes / sizeof (*ace_yyptr);        \
      }                  \
    while (ACE_YYID (0))

#endif

/* ACE_YYFINAL -- State number of the termination state.  */
#define ACE_YYFINAL  2
/* ACE_YYLAST -- Last index in ACE_YYTABLE.  */
#define ACE_YYLAST   62

/* ACE_YYNTOKENS -- Number of terminals.  */
#define ACE_YYNTOKENS  23
/* ACE_YYNNTS -- Number of nonterminals.  */
#define ACE_YYNNTS  21
/* ACE_YYNRULES -- Number of rules.  */
#define ACE_YYNRULES  45
/* ACE_YYNRULES -- Number of states.  */
#define ACE_YYNSTATES  66

/* ACE_YYTRANSLATE(ACE_YYLEX) -- Bison symbol number corresponding to ACE_YYLEX.  */
#define ACE_YYUNDEFTOK  2
#define ACE_YYMAXUTOK   271

#define ACE_YYTRANSLATE(ACE_YYX)            \
  ((unsigned int) (ACE_YYX) <= ACE_YYMAXUTOK ? ace_yytranslate[ACE_YYX] : ACE_YYUNDEFTOK)

/* ACE_YYTRANSLATE[ACE_YYLEX] -- Bison symbol number corresponding to ACE_YYLEX.  */
static const ace_yytype_uint8 ace_yytranslate[] =
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
static const ace_yytype_uint8 ace_yyprhs[] =
{
       0,     0,     3,     6,     9,    10,    12,    14,    16,    18,
      20,    22,    26,    30,    33,    36,    39,    43,    44,    49,
      51,    53,    54,    59,    60,    63,    64,    66,    68,    70,
      72,    74,    79,    81,    83,    84,    88,    94,    99,   102,
     105,   108,   110,   111,   113,   115
};

/* ACE_YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const ace_yytype_int8 ace_yyrhs[] =
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
static const ace_yytype_uint16 ace_yyrline[] =
{
       0,    62,    62,    71,    75,    79,    80,    81,    82,    83,
      84,    88,    98,   105,   112,   119,   126,   130,   130,   137,
     140,   147,   146,   155,   159,   167,   171,   174,   188,   197,
     206,   228,   235,   239,   244,   250,   254,   258,   265,   269,
     273,   280,   281,   285,   286,   287
};
#endif

#if ACE_YYDEBUG || ACE_YYERROR_VERBOSE || ACE_YYTOKEN_TABLE
/* ACE_YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at ACE_YYNTOKENS, nonterminals.  */
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
static const ace_yytype_uint16 ace_yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   123,   125,    58,
      40,    41,    42
};
# endif

/* ACE_YYR1[ACE_YYN] -- Symbol number of symbol that rule ACE_YYN derives.  */
static const ace_yytype_uint8 ace_yyr1[] =
{
       0,    23,    24,    24,    24,    25,    25,    25,    25,    25,
      25,    26,    27,    28,    29,    30,    31,    32,    31,    33,
      33,    35,    34,    34,    36,    36,    37,    37,    37,    37,
      37,    38,    39,    39,    39,    40,    40,    40,    41,    41,
      41,    42,    42,    43,    43,    43
};

/* ACE_YYR2[ACE_YYN] -- Number of symbols composing right hand side of rule ACE_YYN.  */
static const ace_yytype_uint8 ace_yyr2[] =
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
static const ace_yytype_uint8 ace_yydefact[] =
{
       4,     0,     1,     3,     0,     0,     0,     0,     0,     0,
       2,     5,     6,     7,     8,     9,    10,     0,    42,    42,
      13,    14,    15,    17,    19,    20,    23,     0,     0,     0,
       0,    41,    11,    12,    23,    21,    16,    38,    40,    39,
      43,    44,    45,     0,    34,     0,    18,    25,     0,    32,
      33,    31,     0,     0,     0,    35,    22,    26,    27,    28,
      29,    30,    24,    37,     0,    36
};

/* ACE_YYDEFGOTO[NTERM-NUM].  */
static const ace_yytype_int8 ace_yydefgoto[] =
{
      -1,     1,    10,    11,    12,    13,    14,    15,    16,    34,
      26,    36,    47,    53,    62,    18,    51,    44,    30,    32,
      45
};

/* ACE_YYPACT[STATE-NUM] -- Index in ACE_YYTABLE of the portion describing
   STATE-NUM.  */
#define ACE_YYPACT_NINF -13
static const ace_yytype_int8 ace_yypact[] =
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
static const ace_yytype_int8 ace_yypgoto[] =
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
static const ace_yytype_uint8 ace_yytable[] =
{
      24,    25,     4,     5,     6,     7,     8,     4,     5,    40,
      41,    42,    49,    50,    43,    35,    17,    56,    19,    23,
       2,     3,    20,     4,     5,     6,     7,     8,     9,    21,
      27,    28,    29,    22,    37,    31,    52,    38,    39,    48,
      55,    59,    54,    64,    57,    58,    60,    63,    65,    61,
      33,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    46
};

static const ace_yytype_int8 ace_yycheck[] =
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
static const ace_yytype_uint8 ace_yystos[] =
{
       0,    24,     0,     1,     3,     4,     5,     6,     7,     8,
      25,    26,    27,    28,    29,    30,    31,    15,    38,    15,
      15,    15,    15,    15,    26,    27,    33,     9,    10,    11,
      41,    16,    42,    42,    32,    17,    34,    22,    22,    22,
      14,    15,    16,    19,    40,    43,    34,    35,    15,    12,
      13,    39,    19,    36,    20,    15,    18,    26,    27,    28,
      29,    30,    37,    21,    20,    21
};

#define ace_yyerrok    (ace_yyerrstatus = 0)
#define ace_yyclearin  (ace_yychar = ACE_YYEMPTY)
#define ACE_YYEMPTY    (-2)
#define ACE_YYEOF    0

#define ACE_YYACCEPT  goto ace_yyacceptlab
#define ACE_YYABORT    goto ace_yyabortlab
#define ACE_YYERROR    goto ace_yyerrorlab


/* Like ACE_YYERROR except do call ace_yyerror.  This remains here temporarily
   to ease the transition to the new meaning of ACE_YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define ACE_YYFAIL    goto ace_yyerrlab

#define ACE_YYRECOVERING()  (!!ace_yyerrstatus)

#define ACE_YYBACKUP(Token, Value)          \
do                \
  if (ace_yychar == ACE_YYEMPTY && ace_yylen == 1)        \
    {                \
      ace_yychar = (Token);            \
      ace_yylval = (Value);            \
      ace_yytoken = ACE_YYTRANSLATE (ace_yychar);        \
      ACE_YYPOPSTACK (1);            \
      goto ace_yybackup;            \
    }                \
  else                \
    {                \
      ace_yyerror (ACE_SVC_CONF_PARAM->yyerrno, ACE_SVC_CONF_PARAM->yylineno, ACE_YY_("syntax error: cannot back up")); \
      ACE_YYERROR;              \
    }                \
while (ACE_YYID (0))


#define ACE_YYTERROR  1
#define ACE_YYERRCODE  256


/* ACE_YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define ACE_YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef ACE_YYLLOC_DEFAULT
# define ACE_YYLLOC_DEFAULT(Current, Rhs, N)        \
    do                  \
      if (ACE_YYID (N))                                                    \
  {                \
    (Current).first_line   = ACE_YYRHSLOC (Rhs, 1).first_line;  \
    (Current).first_column = ACE_YYRHSLOC (Rhs, 1).first_column;  \
    (Current).last_line    = ACE_YYRHSLOC (Rhs, N).last_line;    \
    (Current).last_column  = ACE_YYRHSLOC (Rhs, N).last_column;  \
  }                \
      else                \
  {                \
    (Current).first_line   = (Current).last_line   =    \
      ACE_YYRHSLOC (Rhs, 0).last_line;        \
    (Current).first_column = (Current).last_column =    \
      ACE_YYRHSLOC (Rhs, 0).last_column;        \
  }                \
    while (ACE_YYID (0))
#endif


/* ACE_YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef ACE_YY_LOCATION_PRINT
# if ACE_YYLTYPE_IS_TRIVIAL
#  define ACE_YY_LOCATION_PRINT(File, Loc)      \
     ACE_OS::fprintf (File, "%d.%d-%d.%d",      \
        (Loc).first_line, (Loc).first_column,  \
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

# define ACE_YYDPRINTF(Args)      \
do {            \
  if (ace_yydebug)          \
    ACE_YYFPRINTF Args;        \
} while (ACE_YYID (0))

# define ACE_YY_SYMBOL_PRINT(Title, Type, Value, Location)        \
do {                    \
  if (ace_yydebug)                  \
    {                    \
      ACE_YYFPRINTF (stderr, "%s ", Title);            \
      ace_yy_symbol_print (stderr,              \
      Type, Value); \
      ACE_YYFPRINTF (stderr, "\n");              \
    }                    \
} while (ACE_YYID (0))


/*--------------------------------.
| Print this symbol on ACE_YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
ace_yy_symbol_value_print (FILE *ace_yyoutput, int ace_yytype, ACE_YYSTYPE const * const ace_yyvaluep)
#else
static void
ace_yy_symbol_value_print (ace_yyoutput, ace_yytype, ace_yyvaluep)
    FILE *ace_yyoutput;
    int ace_yytype;
    ACE_YYSTYPE const * const ace_yyvaluep;
#endif
{
  if (!ace_yyvaluep)
    return;
# ifdef ACE_YYPRINT
  if (ace_yytype < ACE_YYNTOKENS)
    ACE_YYPRINT (ace_yyoutput, ace_yytoknum[ace_yytype], *ace_yyvaluep);
# else
  ACE_YYUSE (ace_yyoutput);
# endif
  switch (ace_yytype)
    {
      default:
  break;
    }
}


/*--------------------------------.
| Print this symbol on ACE_YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
ace_yy_symbol_print (FILE *ace_yyoutput, int ace_yytype, ACE_YYSTYPE const * const ace_yyvaluep)
#else
static void
ace_yy_symbol_print (ace_yyoutput, ace_yytype, ace_yyvaluep)
    FILE *ace_yyoutput;
    int ace_yytype;
    ACE_YYSTYPE const * const ace_yyvaluep;
#endif
{
  if (ace_yytype < ACE_YYNTOKENS)
    ACE_YYFPRINTF (ace_yyoutput, "token %s (", ace_yytname[ace_yytype]);
  else
    ACE_YYFPRINTF (ace_yyoutput, "nterm %s (", ace_yytname[ace_yytype]);

  ace_yy_symbol_value_print (ace_yyoutput, ace_yytype, ace_yyvaluep);
  ACE_YYFPRINTF (ace_yyoutput, ")");
}

/*------------------------------------------------------------------.
| ace_yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
ace_yy_stack_print (ace_yytype_int16 *bottom, ace_yytype_int16 *top)
#else
static void
ace_yy_stack_print (bottom, top)
    ace_yytype_int16 *bottom;
    ace_yytype_int16 *top;
#endif
{
  ACE_YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    ACE_YYFPRINTF (stderr, " %d", *bottom);
  ACE_YYFPRINTF (stderr, "\n");
}

# define ACE_YY_STACK_PRINT(Bottom, Top)        \
do {                \
  if (ace_yydebug)              \
    ace_yy_stack_print ((Bottom), (Top));        \
} while (ACE_YYID (0))


/*------------------------------------------------.
| Report that the ACE_YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
ace_yy_reduce_print (ACE_YYSTYPE *ace_yyvsp, int ace_yyrule)
#else
static void
ace_yy_reduce_print (ace_yyvsp, ace_yyrule)
    ACE_YYSTYPE *ace_yyvsp;
    int ace_yyrule;
#endif
{
  int ace_yynrhs = ace_yyr2[ace_yyrule];
  int ace_yyi;
  unsigned long int ace_yylno = ace_yyrline[ace_yyrule];
  ACE_YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
       ace_yyrule - 1, ace_yylno);
  /* The symbols being reduced.  */
  for (ace_yyi = 0; ace_yyi < ace_yynrhs; ace_yyi++)
    {
      ACE_OS::fprintf (stderr, "   $%d = ", ace_yyi + 1);
      ace_yy_symbol_print (stderr, ace_yyrhs[ace_yyprhs[ace_yyrule] + ace_yyi],
           &(ace_yyvsp[(ace_yyi + 1) - (ace_yynrhs)])
                      );
      ACE_OS::fprintf (stderr, "\n");
    }
}

# define ACE_YY_REDUCE_PRINT(Rule)    \
do {          \
  if (ace_yydebug)        \
    ace_yy_reduce_print (ace_yyvsp, Rule); \
} while (ACE_YYID (0))

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
   ACE_YYSTACK_ALLOC_MAXIMUM < ACE_YYSTACK_BYTES (ACE_YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef ACE_YYMAXDEPTH
# define ACE_YYMAXDEPTH 10000
#endif



#if ACE_YYERROR_VERBOSE

# ifndef ace_yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define ace_yystrlen strlen
#  else
/* Return the length of ACE_YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static ACE_YYSIZE_T
ace_yystrlen (const char *ace_yystr)
#else
static ACE_YYSIZE_T
ace_yystrlen (ace_yystr)
    const char *ace_yystr;
#endif
{
  ACE_YYSIZE_T ace_yylen;
  for (ace_yylen = 0; ace_yystr[ace_yylen]; ace_yylen++)
    continue;
  return ace_yylen;
}
#  endif
# endif

# ifndef ace_yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define ace_yystpcpy stpcpy
#  else
/* Copy ACE_YYSRC to ACE_YYDEST, returning the address of the terminating '\0' in
   ACE_YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
ace_yystpcpy (ACE_TCHAR *ace_yydest, const char *ace_yysrc)
#else
static char *
ace_yystpcpy (ace_yydest, ace_yysrc)
    char *ace_yydest;
    const char *ace_yysrc;
#endif
{
  char *ace_yyd = ace_yydest;
  const char *ace_yys = ace_yysrc;

  while ((*ace_yyd++ = *ace_yys++) != '\0')
    continue;

  return ace_yyd - 1;
}
#  endif
# endif

# ifndef ace_yytnamerr
/* Copy to ACE_YYRES the contents of ACE_YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for ace_yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  ACE_YYSTR is taken from ace_yytname.  If ACE_YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static ACE_YYSIZE_T
ace_yytnamerr (ACE_TCHAR *ace_yyres, const char *ace_yystr)
{
  if (*ace_yystr == '"')
    {
      ACE_YYSIZE_T ace_yyn = 0;
      char const *ace_yyp = ace_yystr;

      for (;;)
  switch (*++ace_yyp)
    {
    case '\'':
    case ',':
      goto do_not_strip_quotes;

    case '\\':
      if (*++ace_yyp != '\\')
        goto do_not_strip_quotes;
      /* Fall through.  */
    default:
      if (ace_yyres)
        ace_yyres[ace_yyn] = *ace_yyp;
      ace_yyn++;
      break;

    case '"':
      if (ace_yyres)
        ace_yyres[ace_yyn] = '\0';
      return ace_yyn;
    }
    do_not_strip_quotes: ;
    }

  if (! ace_yyres)
    return ace_yystrlen (ace_yystr);

  return ace_yystpcpy (ace_yyres, ace_yystr) - ace_yyres;
}
# endif

/* Copy into ACE_YYRESULT an error message about the unexpected token
   ACE_YYCHAR while in state ACE_YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If ACE_YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return ACE_YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static ACE_YYSIZE_T
ace_yysyntax_error (ACE_TCHAR *ace_yyresult, int ace_yystate, int ace_yychar)
{
  int ace_yyn = ace_yypact[ace_yystate];

  if (! (ACE_YYPACT_NINF < ace_yyn && ace_yyn <= ACE_YYLAST))
    return 0;
  else
    {
      int ace_yytype = ACE_YYTRANSLATE (ace_yychar);
      ACE_YYSIZE_T ace_yysize0 = ace_yytnamerr (0, ace_yytname[ace_yytype]);
      ACE_YYSIZE_T ace_yysize = ace_yysize0;
      ACE_YYSIZE_T ace_yysize1;
      int ace_yysize_overflow = 0;
      enum { ACE_YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *ace_yyarg[ACE_YYERROR_VERBOSE_ARGS_MAXIMUM];
      int ace_yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
   constructed on the fly.  */
      ACE_YY_("syntax error, unexpected %s");
      ACE_YY_("syntax error, unexpected %s, expecting %s");
      ACE_YY_("syntax error, unexpected %s, expecting %s or %s");
      ACE_YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      ACE_YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *ace_yyfmt;
      char const *ace_yyf;
      static char const ace_yyunexpected[] = "syntax error, unexpected %s";
      static char const ace_yyexpecting[] = ", expecting %s";
      static char const ace_yyor[] = " or %s";
      char ace_yyformat[sizeof ace_yyunexpected
        + sizeof ace_yyexpecting - 1
        + ((ACE_YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
           * (sizeof ace_yyor - 1))];
      char const *ace_yyprefix = ace_yyexpecting;

      /* Start ACE_YYX at -ACE_YYN if negative to avoid negative indexes in
   ACE_YYCHECK.  */
      int ace_yyxbegin = ace_yyn < 0 ? -ace_yyn : 0;

      /* Stay within bounds of both ace_yycheck and ace_yytname.  */
      int ace_yychecklim = ACE_YYLAST - ace_yyn + 1;
      int ace_yyxend = ace_yychecklim < ACE_YYNTOKENS ? ace_yychecklim : ACE_YYNTOKENS;
      int ace_yycount = 1;

      ace_yyarg[0] = ace_yytname[ace_yytype];
      ace_yyfmt = ace_yystpcpy (ace_yyformat, ace_yyunexpected);

      for (ace_yyx = ace_yyxbegin; ace_yyx < ace_yyxend; ++ace_yyx)
  if (ace_yycheck[ace_yyx + ace_yyn] == ace_yyx && ace_yyx != ACE_YYTERROR)
    {
      if (ace_yycount == ACE_YYERROR_VERBOSE_ARGS_MAXIMUM)
        {
    ace_yycount = 1;
    ace_yysize = ace_yysize0;
    ace_yyformat[sizeof ace_yyunexpected - 1] = '\0';
    break;
        }
      ace_yyarg[ace_yycount++] = ace_yytname[ace_yyx];
      ace_yysize1 = ace_yysize + ace_yytnamerr (0, ace_yytname[ace_yyx]);
      ace_yysize_overflow |= (ace_yysize1 < ace_yysize);
      ace_yysize = ace_yysize1;
      ace_yyfmt = ace_yystpcpy (ace_yyfmt, ace_yyprefix);
      ace_yyprefix = ace_yyor;
    }

      ace_yyf = ACE_YY_(ace_yyformat);
      ace_yysize1 = ace_yysize + ace_yystrlen (ace_yyf);
      ace_yysize_overflow |= (ace_yysize1 < ace_yysize);
      ace_yysize = ace_yysize1;

      if (ace_yysize_overflow)
  return ACE_YYSIZE_MAXIMUM;

      if (ace_yyresult)
  {
    /* Avoid sprintf, as that infringes on the user's name space.
       Don't have undefined behavior even if the translation
       produced a string with the wrong number of "%s"s.  */
    char *ace_yyp = ace_yyresult;
    int ace_yyi = 0;
    while ((*ace_yyp = *ace_yyf) != '\0')
      {
        if (*ace_yyp == '%' && ace_yyf[1] == 's' && ace_yyi < ace_yycount)
    {
      ace_yyp += ace_yytnamerr (ace_yyp, ace_yyarg[ace_yyi++]);
      ace_yyf += 2;
    }
        else
    {
      ace_yyp++;
      ace_yyf++;
    }
      }
  }
      return ace_yysize;
    }
}
#endif /* ACE_YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
  ACE_YYUSE (ace_yyvaluep);

  if (!ace_yymsg)
    ace_yymsg = "Deleting";
  ACE_YY_SYMBOL_PRINT (ace_yymsg, ace_yytype, ace_yyvaluep, ace_yylocationp);

  switch (ace_yytype)
    {

      default:
  break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef ACE_YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int ace_yyparse (void *ACE_YYPARSE_PARAM);
#else
int ace_yyparse ();
#endif
#else /* ! ACE_YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int ace_yyparse (void);
#else
int ace_yyparse ();
#endif
#endif /* ! ACE_YYPARSE_PARAM */






/*----------.
| ace_yyparse.  |
`----------*/

#ifdef ACE_YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
ace_yyparse (void *ACE_YYPARSE_PARAM)
#else
int
ace_yyparse (ACE_YYPARSE_PARAM)
    void *ACE_YYPARSE_PARAM;
#endif
#else /* ! ACE_YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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

/* Number of syntax errors so far.  */
int ace_yynerrs;

  int ace_yystate;
  int ace_yyn;
  int ace_yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int ace_yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int ace_yytoken = 0;
#if ACE_YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char ace_yymsgbuf[128];
  char *ace_yymsg = ace_yymsgbuf;
  ACE_YYSIZE_T ace_yymsg_alloc = sizeof ace_yymsgbuf;
#endif

  /* Three stacks and their tools:
     `ace_yyss': related to states,
     `ace_yyvs': related to semantic values,
     `ace_yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow ace_yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  ace_yytype_int16 ace_yyssa[ACE_YYINITDEPTH];
  ace_yytype_int16 *ace_yyss = ace_yyssa;
  ace_yytype_int16 *ace_yyssp;

  /* The semantic value stack.  */
  ACE_YYSTYPE ace_yyvsa[ACE_YYINITDEPTH];
  ACE_YYSTYPE *ace_yyvs = ace_yyvsa;
  ACE_YYSTYPE *ace_yyvsp;



#define ACE_YYPOPSTACK(N)   (ace_yyvsp -= (N), ace_yyssp -= (N))

  ACE_YYSIZE_T ace_yystacksize = ACE_YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  ACE_YYSTYPE ace_yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int ace_yylen = 0;

  ACE_YYDPRINTF ((stderr, "Starting parse\n"));

  ace_yystate = 0;
  ace_yyerrstatus = 0;
  ace_yynerrs = 0;
  ace_yychar = ACE_YYEMPTY;    /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  ace_yyssp = ace_yyss;
  ace_yyvsp = ace_yyvs;

  goto ace_yysetstate;

/*------------------------------------------------------------.
| ace_yynewstate -- Push a new state, which is found in ace_yystate.  |
`------------------------------------------------------------*/
 ace_yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  ace_yyssp++;

 ace_yysetstate:
  *ace_yyssp = ace_yystate;

  if (ace_yyss + ace_yystacksize - 1 <= ace_yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      ACE_YYSIZE_T ace_yysize = ace_yyssp - ace_yyss + 1;

#ifdef ace_yyoverflow
      {
  /* Give user a chance to reallocate the stack.  Use copies of
     these so that the &'s don't force the real ones into
     memory.  */
  ACE_YYSTYPE *ace_yyvs1 = ace_yyvs;
  ace_yytype_int16 *ace_yyss1 = ace_yyss;


  /* Each stack pointer address is followed by the size of the
     data in use in that stack, in bytes.  This used to be a
     conditional around just the two extra args, but that might
     be undefined if ace_yyoverflow is a macro.  */
  ace_yyoverflow (ACE_YY_("memory exhausted"),
        &ace_yyss1, ace_yysize * sizeof (*ace_yyssp),
        &ace_yyvs1, ace_yysize * sizeof (*ace_yyvsp),

        &ace_yystacksize);

  ace_yyss = ace_yyss1;
  ace_yyvs = ace_yyvs1;
      }
#else /* no ace_yyoverflow */
# ifndef ACE_YYSTACK_RELOCATE
      goto ace_yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (ACE_YYMAXDEPTH <= ace_yystacksize)
  goto ace_yyexhaustedlab;
      ace_yystacksize *= 2;
      if (ACE_YYMAXDEPTH < ace_yystacksize)
  ace_yystacksize = ACE_YYMAXDEPTH;

      {
  ace_yytype_int16 *ace_yyss1 = ace_yyss;
  union ace_yyalloc *ace_yyptr =
    (union ace_yyalloc *) ACE_YYSTACK_ALLOC (ACE_YYSTACK_BYTES (ace_yystacksize));
  if (! ace_yyptr)
    goto ace_yyexhaustedlab;
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

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

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

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (ace_yyerrstatus)
    ace_yyerrstatus--;

  /* Shift the look-ahead token.  */
  ACE_YY_SYMBOL_PRINT ("Shifting", ace_yytoken, &ace_yylval, &ace_yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (ace_yychar != ACE_YYEOF)
    ace_yychar = ACE_YYEMPTY;

  ace_yystate = ace_yyn;
  *++ace_yyvsp = ace_yylval;

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
      if ((ace_yyvsp[(2) - (2)].parse_node_) != 0)
      {
        (ace_yyvsp[(2) - (2)].parse_node_)->apply (ACE_SVC_CONF_PARAM->config, ACE_SVC_CONF_PARAM->yyerrno);
        delete (ace_yyvsp[(2) - (2)].parse_node_);
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
      if ((ace_yyvsp[(2) - (3)].svc_record_) != 0)
        (ace_yyval.parse_node_) = new ACE_Dynamic_Node ((ace_yyvsp[(2) - (3)].svc_record_), (ace_yyvsp[(3) - (3)].ident_));
      else
        (ace_yyval.parse_node_) = 0;
    ;}
    break;

  case 12:

    {
      (ace_yyval.parse_node_) = new ACE_Static_Node ((ace_yyvsp[(2) - (3)].ident_), (ace_yyvsp[(3) - (3)].ident_));
    ;}
    break;

  case 13:

    {
      (ace_yyval.parse_node_) = new ACE_Suspend_Node ((ace_yyvsp[(2) - (2)].ident_));
    ;}
    break;

  case 14:

    {
      (ace_yyval.parse_node_) = new ACE_Resume_Node ((ace_yyvsp[(2) - (2)].ident_));
    ;}
    break;

  case 15:

    {
      (ace_yyval.parse_node_) = new ACE_Remove_Node ((ace_yyvsp[(2) - (2)].ident_));
    ;}
    break;

  case 16:

    {
      (ace_yyval.parse_node_) = new ACE_Stream_Node ((ace_yyvsp[(2) - (3)].static_node_), (ace_yyvsp[(3) - (3)].parse_node_));
    ;}
    break;

  case 17:

    { (ace_yyval.static_node_) = new ACE_Static_Node ((ace_yyvsp[(2) - (2)].ident_)); ;}
    break;

  case 18:

    {
      (ace_yyval.parse_node_) = new ACE_Dummy_Node ((ace_yyvsp[(3) - (4)].static_node_), (ace_yyvsp[(4) - (4)].parse_node_));
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
      (ace_yyval.static_node_) = (ace_yyvsp[(0) - (1)].static_node_);
    ;}
    break;

  case 22:

    {
      (ace_yyval.parse_node_) = (ace_yyvsp[(3) - (4)].parse_node_);
    ;}
    break;

  case 23:

    { (ace_yyval.parse_node_) = 0; ;}
    break;

  case 24:

    {
      if ((ace_yyvsp[(2) - (2)].parse_node_) != 0)
        {
          (ace_yyvsp[(2) - (2)].parse_node_)->link ((ace_yyvsp[(1) - (2)].parse_node_));
          (ace_yyval.parse_node_) = (ace_yyvsp[(2) - (2)].parse_node_);
        }
    ;}
    break;

  case 25:

    { (ace_yyval.parse_node_) = 0; ;}
    break;

  case 26:

    {
            ;}
    break;

  case 27:

    {
      ACE_Static_Node *sn = (ace_yyvsp[(-1) - (1)].static_node_);
      ACE_Module_Type *mt = ace_get_module (sn->record (ACE_SVC_CONF_PARAM->config),
                                            (ace_yyvsp[(1) - (1)].static_node_)->name (),
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
      ACE_Static_Node *sn = (ace_yyvsp[(-1) - (1)].static_node_);
      ACE_Module_Type *mt = ace_get_module (sn->record (ACE_SVC_CONF_PARAM->config),
                                            sn->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);
      if (mt != 0)
        mt->suspend ();
    ;}
    break;

  case 29:

    {
      ACE_Static_Node *sn = (ace_yyvsp[(-1) - (1)].static_node_);
      ACE_Module_Type *mt = ace_get_module (sn->record (ACE_SVC_CONF_PARAM->config),
                                            (ace_yyvsp[(1) - (1)].static_node_)->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);
      if (mt != 0)
        mt->resume ();
    ;}
    break;

  case 30:

    {
      ACE_Static_Node *stream = (ace_yyvsp[(-1) - (1)].static_node_);
      ACE_Static_Node *module = (ace_yyvsp[(1) - (1)].static_node_);
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
      (ace_yyval.svc_record_) = new ACE_Service_Type_Factory ((ace_yyvsp[(1) - (4)].ident_), (ace_yyvsp[(2) - (4)].type_), (ace_yyvsp[(3) - (4)].location_node_), (ace_yyvsp[(4) - (4)].type_));
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
      (ace_yyval.location_node_) = new ACE_Object_Node ((ace_yyvsp[(1) - (3)].ident_), (ace_yyvsp[(3) - (3)].ident_));
    ;}
    break;

  case 36:

    {
      (ace_yyval.location_node_) = new ACE_Function_Node ((ace_yyvsp[(1) - (5)].ident_), (ace_yyvsp[(3) - (5)].ident_));
    ;}
    break;

  case 37:

    {
      (ace_yyval.location_node_) = new ACE_Static_Function_Node ((ace_yyvsp[(2) - (4)].ident_));
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


/* Line 1267 of yacc.c.  */

      default: break;
    }
  ACE_YY_SYMBOL_PRINT ("-> $$ =", ace_yyr1[ace_yyn], &ace_yyval, &ace_yyloc);

  ACE_YYPOPSTACK (ace_yylen);
  ace_yylen = 0;
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
#if ! ACE_YYERROR_VERBOSE
      ace_yyerror (ACE_SVC_CONF_PARAM->yyerrno, ACE_SVC_CONF_PARAM->yylineno, ACE_YY_("syntax error"));
#else
      {
  ACE_YYSIZE_T ace_yysize = ace_yysyntax_error (0, ace_yystate, ace_yychar);
  if (ace_yymsg_alloc < ace_yysize && ace_yymsg_alloc < ACE_YYSTACK_ALLOC_MAXIMUM)
    {
      ACE_YYSIZE_T ace_yyalloc = 2 * ace_yysize;
      if (! (ace_yysize <= ace_yyalloc && ace_yyalloc <= ACE_YYSTACK_ALLOC_MAXIMUM))
        ace_yyalloc = ACE_YYSTACK_ALLOC_MAXIMUM;
      if (ace_yymsg != ace_yymsgbuf)
        ACE_YYSTACK_FREE (ace_yymsg);
      ace_yymsg = (ACE_TCHAR *) ACE_YYSTACK_ALLOC (ace_yyalloc);
      if (ace_yymsg)
        ace_yymsg_alloc = ace_yyalloc;
      else
        {
    ace_yymsg = ace_yymsgbuf;
    ace_yymsg_alloc = sizeof ace_yymsgbuf;
        }
    }

  if (0 < ace_yysize && ace_yysize <= ace_yymsg_alloc)
    {
      (void) ace_yysyntax_error (ace_yymsg, ace_yystate, ace_yychar);
      ace_yyerror (ACE_SVC_CONF_PARAM->yyerrno, ACE_SVC_CONF_PARAM->yylineno, ace_yymsg);
    }
  else
    {
      ace_yyerror (ACE_SVC_CONF_PARAM->yyerrno, ACE_SVC_CONF_PARAM->yylineno, ACE_YY_("syntax error"));
      if (ace_yysize != 0)
        goto ace_yyexhaustedlab;
    }
      }
#endif
    }



  if (ace_yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
   error, discard it.  */

      if (ace_yychar <= ACE_YYEOF)
  {
    /* Return failure if at end of input.  */
    if (ace_yychar == ACE_YYEOF)
      ACE_YYABORT;
  }
      else
  {
    ace_yydestruct ("Error: discarding",
          ace_yytoken, &ace_yylval);
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

  /* Pacify compilers like GCC when the user code never invokes
     ACE_YYERROR and the label ace_yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto ace_yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this ACE_YYERROR.  */
  ACE_YYPOPSTACK (ace_yylen);
  ace_yylen = 0;
  ACE_YY_STACK_PRINT (ace_yyss, ace_yyssp);
  ace_yystate = *ace_yyssp;
  goto ace_yyerrlab1;


/*-------------------------------------------------------------.
| ace_yyerrlab1 -- common code for both syntax error and ACE_YYERROR.  |
`-------------------------------------------------------------*/
ace_yyerrlab1:
  ace_yyerrstatus = 3;  /* Each real token shifted decrements this.  */

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


      ace_yydestruct ("Error: popping",
      ace_yystos[ace_yystate], ace_yyvsp);
      ACE_YYPOPSTACK (1);
      ace_yystate = *ace_yyssp;
      ACE_YY_STACK_PRINT (ace_yyss, ace_yyssp);
    }

  if (ace_yyn == ACE_YYFINAL)
    ACE_YYACCEPT;

  *++ace_yyvsp = ace_yylval;


  /* Shift the error token.  */
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
  ace_yyresult = 1;
  goto ace_yyreturn;

#ifndef ace_yyoverflow
/*-------------------------------------------------.
| ace_yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
ace_yyexhaustedlab:
  ace_yyerror (ACE_SVC_CONF_PARAM->yyerrno, ACE_SVC_CONF_PARAM->yylineno, ACE_YY_("memory exhausted"));
  ace_yyresult = 2;
  /* Fall through.  */
#endif

ace_yyreturn:
  if (ace_yychar != ACE_YYEOF && ace_yychar != ACE_YYEMPTY)
     ace_yydestruct ("Cleanup: discarding lookahead",
     ace_yytoken, &ace_yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this ACE_YYABORT or ACE_YYACCEPT.  */
  ACE_YYPOPSTACK (ace_yylen);
  ACE_YY_STACK_PRINT (ace_yyss, ace_yyssp);
  while (ace_yyssp != ace_yyss)
    {
      ace_yydestruct ("Cleanup: popping",
      ace_yystos[*ace_yyssp], ace_yyvsp);
      ACE_YYPOPSTACK (1);
    }
#ifndef ace_yyoverflow
  if (ace_yyss != ace_yyssa)
    ACE_YYSTACK_FREE (ace_yyss);
#endif
#if ACE_YYERROR_VERBOSE
  if (ace_yymsg != ace_yymsgbuf)
    ACE_YYSTACK_FREE (ace_yymsg);
#endif
  /* Make sure ACE_YYID is used.  */
  return ACE_YYID (ace_yyresult);
}





ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Prints the error string to standard output.  Cleans up the error
// messages.

void
ace_yyerror (ACE_TCHAR const * s)
{
  ace_yyerror (-1, -1, s);
}

void
ace_yyerror (int ace_yyerrno, int ace_yylineno, char const * s)
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
main (int argc, char *argv[])
{
  ACE_Svc_Conf_Param param (0, stdin);

  // Try to reopen any filename argument to use ACE_YYIN.
  if (argc > 1 && (ace_yyin = freopen (argv[1], "r", stdin)) == 0)
    (void) ACE_OS::ACE_OS::fprintf (stderr, ACE_TEXT ("usage: %s [file]\n"), argv[0]), ACE_OS::exit (1);

  return ::ace_yyparse (&param);
}
#endif /* SVC_CONF_Y_DEBUGGING */

ACE_END_VERSIONED_NAMESPACE_DECL

#endif  /* ACE_USES_CLASSIC_SVC_CONF == 1 */

