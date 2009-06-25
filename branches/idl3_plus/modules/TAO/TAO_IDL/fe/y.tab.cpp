// $Id$
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

/* All symbols defined below should begin with tao_yy or TAO_YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define TAO_YYBISON 1

/* Bison version.  */
#define TAO_YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define TAO_YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define TAO_YYPURE 0

/* Using locations.  */
#define TAO_YYLSP_NEEDED 0



/* Tokens.  */
#ifndef TAO_YYTOKENTYPE
# define TAO_YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum tao_yytokentype {
     IDENTIFIER = 258,
     IDL_CONST = 259,
     IDL_MODULE = 260,
     IDL_INTERFACE = 261,
     IDL_TYPEDEF = 262,
     IDL_LONG = 263,
     IDL_SHORT = 264,
     IDL_UNSIGNED = 265,
     IDL_DOUBLE = 266,
     IDL_FLOAT = 267,
     IDL_CHAR = 268,
     IDL_WCHAR = 269,
     IDL_OCTET = 270,
     IDL_BOOLEAN = 271,
     IDL_FIXED = 272,
     IDL_ANY = 273,
     IDL_OBJECT = 274,
     IDL_STRUCT = 275,
     IDL_UNION = 276,
     IDL_SWITCH = 277,
     IDL_ENUM = 278,
     IDL_SEQUENCE = 279,
     IDL_STRING = 280,
     IDL_WSTRING = 281,
     IDL_EXCEPTION = 282,
     IDL_CASE = 283,
     IDL_DEFAULT = 284,
     IDL_READONLY = 285,
     IDL_ATTRIBUTE = 286,
     IDL_ONEWAY = 287,
     IDL_IDEMPOTENT = 288,
     IDL_VOID = 289,
     IDL_IN = 290,
     IDL_OUT = 291,
     IDL_INOUT = 292,
     IDL_RAISES = 293,
     IDL_CONTEXT = 294,
     IDL_NATIVE = 295,
     IDL_LOCAL = 296,
     IDL_ABSTRACT = 297,
     IDL_CUSTOM = 298,
     IDL_FACTORY = 299,
     IDL_PRIVATE = 300,
     IDL_PUBLIC = 301,
     IDL_SUPPORTS = 302,
     IDL_TRUNCATABLE = 303,
     IDL_VALUETYPE = 304,
     IDL_COMPONENT = 305,
     IDL_CONSUMES = 306,
     IDL_EMITS = 307,
     IDL_EVENTTYPE = 308,
     IDL_FINDER = 309,
     IDL_GETRAISES = 310,
     IDL_HOME = 311,
     IDL_IMPORT = 312,
     IDL_MULTIPLE = 313,
     IDL_PRIMARYKEY = 314,
     IDL_PROVIDES = 315,
     IDL_PUBLISHES = 316,
     IDL_SETRAISES = 317,
     IDL_TYPEID = 318,
     IDL_TYPEPREFIX = 319,
     IDL_USES = 320,
     IDL_MANAGES = 321,
     IDL_TYPENAME = 322,
     IDL_PRIMITIVE = 323,
     IDL_PORT = 324,
     IDL_MIRRORPORT = 325,
     IDL_PORTTYPE = 326,
     IDL_CONNECTOR = 327,
     IDL_CONCAT = 328,
     IDL_INTEGER_LITERAL = 329,
     IDL_UINTEGER_LITERAL = 330,
     IDL_STRING_LITERAL = 331,
     IDL_CHARACTER_LITERAL = 332,
     IDL_FLOATING_PT_LITERAL = 333,
     IDL_TRUETOK = 334,
     IDL_FALSETOK = 335,
     IDL_SCOPE_DELIMITOR = 336,
     IDL_LEFT_SHIFT = 337,
     IDL_RIGHT_SHIFT = 338,
     IDL_WCHAR_LITERAL = 339,
     IDL_WSTRING_LITERAL = 340
   };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define IDL_CONST 259
#define IDL_MODULE 260
#define IDL_INTERFACE 261
#define IDL_TYPEDEF 262
#define IDL_LONG 263
#define IDL_SHORT 264
#define IDL_UNSIGNED 265
#define IDL_DOUBLE 266
#define IDL_FLOAT 267
#define IDL_CHAR 268
#define IDL_WCHAR 269
#define IDL_OCTET 270
#define IDL_BOOLEAN 271
#define IDL_FIXED 272
#define IDL_ANY 273
#define IDL_OBJECT 274
#define IDL_STRUCT 275
#define IDL_UNION 276
#define IDL_SWITCH 277
#define IDL_ENUM 278
#define IDL_SEQUENCE 279
#define IDL_STRING 280
#define IDL_WSTRING 281
#define IDL_EXCEPTION 282
#define IDL_CASE 283
#define IDL_DEFAULT 284
#define IDL_READONLY 285
#define IDL_ATTRIBUTE 286
#define IDL_ONEWAY 287
#define IDL_IDEMPOTENT 288
#define IDL_VOID 289
#define IDL_IN 290
#define IDL_OUT 291
#define IDL_INOUT 292
#define IDL_RAISES 293
#define IDL_CONTEXT 294
#define IDL_NATIVE 295
#define IDL_LOCAL 296
#define IDL_ABSTRACT 297
#define IDL_CUSTOM 298
#define IDL_FACTORY 299
#define IDL_PRIVATE 300
#define IDL_PUBLIC 301
#define IDL_SUPPORTS 302
#define IDL_TRUNCATABLE 303
#define IDL_VALUETYPE 304
#define IDL_COMPONENT 305
#define IDL_CONSUMES 306
#define IDL_EMITS 307
#define IDL_EVENTTYPE 308
#define IDL_FINDER 309
#define IDL_GETRAISES 310
#define IDL_HOME 311
#define IDL_IMPORT 312
#define IDL_MULTIPLE 313
#define IDL_PRIMARYKEY 314
#define IDL_PROVIDES 315
#define IDL_PUBLISHES 316
#define IDL_SETRAISES 317
#define IDL_TYPEID 318
#define IDL_TYPEPREFIX 319
#define IDL_USES 320
#define IDL_MANAGES 321
#define IDL_TYPENAME 322
#define IDL_PRIMITIVE 323
#define IDL_PORT 324
#define IDL_MIRRORPORT 325
#define IDL_PORTTYPE 326
#define IDL_CONNECTOR 327
#define IDL_CONCAT 328
#define IDL_INTEGER_LITERAL 329
#define IDL_UINTEGER_LITERAL 330
#define IDL_STRING_LITERAL 331
#define IDL_CHARACTER_LITERAL 332
#define IDL_FLOATING_PT_LITERAL 333
#define IDL_TRUETOK 334
#define IDL_FALSETOK 335
#define IDL_SCOPE_DELIMITOR 336
#define IDL_LEFT_SHIFT 337
#define IDL_RIGHT_SHIFT 338
#define IDL_WCHAR_LITERAL 339
#define IDL_WSTRING_LITERAL 340




/* Copy the first part of user declarations.  */


#include "ast_argument.h"
#include "ast_array.h"
#include "ast_attribute.h"
#include "ast_field.h"
#include "ast_expression.h"
#include "ast_operation.h"
#include "ast_generator.h"
#include "ast_module.h"
#include "ast_valuebox.h"
#include "ast_valuetype.h"
#include "ast_valuetype_fwd.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_component.h"
#include "ast_component_fwd.h"
#include "ast_home.h"
#include "ast_constant.h"
#include "ast_union.h"
#include "ast_union_fwd.h"
#include "ast_structure_fwd.h"
#include "ast_extern.h"
#include "ast_enum.h"
#include "ast_root.h"
#include "ast_sequence.h"
#include "ast_string.h"
#include "ast_factory.h"
#include "ast_exception.h"
#include "fe_declarator.h"
#include "fe_interface_header.h"
#include "utl_identifier.h"
#include "utl_err.h"
#include "utl_string.h"
#include "utl_strlist.h"
#include "utl_namelist.h"
#include "utl_exprlist.h"
#include "utl_labellist.h"
#include "utl_decllist.h"
#include "global_extern.h"
#include "nr_extern.h"

#if (defined(apollo) || defined(hpux)) && defined(__cplusplus)
extern  "C" int tao_yywrap();
#endif  // (defined(apollo) || defined(hpux)) && defined(__cplusplus)


void tao_yyerror (const char *);
int tao_yylex (void);
extern "C" int tao_yywrap (void);
extern char tao_yytext[];
extern int tao_yyleng;
AST_Decl *tao_enum_constant_decl = 0;
#define TAO_YYDEBUG_LEXER_TEXT (tao_yytext[tao_yyleng] = '\0', tao_yytext)
// Force the pretty debugging code to compile.
#define TAO_YYDEBUG 1


/* Enabling traces.  */
#ifndef TAO_YYDEBUG
# define TAO_YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef TAO_YYERROR_VERBOSE
# undef TAO_YYERROR_VERBOSE
# define TAO_YYERROR_VERBOSE 1
#else
# define TAO_YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef TAO_YYTOKEN_TABLE
# define TAO_YYTOKEN_TABLE 0
#endif

#if ! defined TAO_YYSTYPE && ! defined TAO_YYSTYPE_IS_DECLARED
typedef union TAO_YYSTYPE

{
  AST_Decl                      *dcval;         /* Decl value           */
  UTL_StrList                   *slval;         /* String list          */
  UTL_NameList                  *nlval;         /* Name list            */
  UTL_ExprList                  *elval;         /* Expression list      */
  UTL_LabelList                 *llval;         /* Label list           */
  UTL_DeclList                  *dlval;         /* Declaration list     */
  FE_InterfaceHeader            *ihval;         /* Interface header     */
  FE_OBVHeader                  *vhval;         /* Valuetype header     */
  FE_EventHeader                *ehval;         /* Event header         */
  FE_ComponentHeader            *chval;         /* Component header     */
  FE_HomeHeader                 *hhval;         /* Home header          */
  AST_Expression                *exval;         /* Expression value     */
  AST_UnionLabel                *ulval;         /* Union label          */
  AST_Field                     *ffval;         /* Field value          */
  AST_Field::Visibility         vival;          /* N/A, pub or priv     */
  AST_Expression::ExprType      etval;          /* Expression type      */
  AST_Argument::Direction       dival;          /* Argument direction   */
  AST_Operation::Flags          ofval;          /* Operation flags      */
  FE_Declarator                 *deval;         /* Declarator value     */
  ACE_CDR::Boolean              bval;           /* Boolean value        */
  ACE_CDR::Long                 ival;           /* Long value           */
  ACE_CDR::ULong                uival;          /* Unsigned long value  */
  ACE_CDR::Double               dval;           /* Double value         */
  ACE_CDR::Float                fval;           /* Float value          */
  ACE_CDR::Char                 cval;           /* Char value           */
  ACE_CDR::WChar                wcval;          /* WChar value          */
  UTL_String                    *sval;          /* String value         */
  char                          *wsval;         /* WString value        */
  char                          *strval;        /* char * value         */
  Identifier                    *idval;         /* Identifier           */
  UTL_IdList                    *idlist;        /* Identifier list      */
  AST_Decl::NodeType            ntval;          /* Node type value      */
}
/* Line 187 of yacc.c.  */

        TAO_YYSTYPE;
# define tao_yystype TAO_YYSTYPE /* obsolescent; will be withdrawn */
# define TAO_YYSTYPE_IS_DECLARED 1
# define TAO_YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */


#ifdef short
# undef short
#endif

#ifdef TAO_YYTYPE_UINT8
typedef TAO_YYTYPE_UINT8 tao_yytype_uint8;
#else
typedef unsigned char tao_yytype_uint8;
#endif

#ifdef TAO_YYTYPE_INT8
typedef TAO_YYTYPE_INT8 tao_yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char tao_yytype_int8;
#else
typedef short int tao_yytype_int8;
#endif

#ifdef TAO_YYTYPE_UINT16
typedef TAO_YYTYPE_UINT16 tao_yytype_uint16;
#else
typedef unsigned short int tao_yytype_uint16;
#endif

#ifdef TAO_YYTYPE_INT16
typedef TAO_YYTYPE_INT16 tao_yytype_int16;
#else
typedef short int tao_yytype_int16;
#endif

#ifndef TAO_YYSIZE_T
# ifdef __SIZE_TYPE__
#  define TAO_YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define TAO_YYSIZE_T size_t
# elif ! defined TAO_YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define TAO_YYSIZE_T size_t
# else
#  define TAO_YYSIZE_T unsigned int
# endif
#endif

#define TAO_YYSIZE_MAXIMUM ((TAO_YYSIZE_T) -1)

#ifndef TAO_YY_
# if TAO_YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define TAO_YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef TAO_YY_
#  define TAO_YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define TAO_YYUSE(e) ((void) (e))
#else
# define TAO_YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define TAO_YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
TAO_YYID (int i)
#else
static int
TAO_YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined tao_yyoverflow || TAO_YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef TAO_YYSTACK_USE_ALLOCA
#  if TAO_YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define TAO_YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define TAO_YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define TAO_YYSTACK_ALLOC alloca
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

# ifdef TAO_YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define TAO_YYSTACK_FREE(Ptr) do { /* empty */; } while (TAO_YYID (0))
#  ifndef TAO_YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define TAO_YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define TAO_YYSTACK_ALLOC TAO_YYMALLOC
#  define TAO_YYSTACK_FREE TAO_YYFREE
#  ifndef TAO_YYSTACK_ALLOC_MAXIMUM
#   define TAO_YYSTACK_ALLOC_MAXIMUM TAO_YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined TAO_YYMALLOC || defined malloc) \
             && (defined TAO_YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef TAO_YYMALLOC
#   define TAO_YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (TAO_YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef TAO_YYFREE
#   define TAO_YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined tao_yyoverflow || TAO_YYERROR_VERBOSE */


#if (! defined tao_yyoverflow \
     && (! defined __cplusplus \
         || (defined TAO_YYSTYPE_IS_TRIVIAL && TAO_YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union tao_yyalloc
{
  tao_yytype_int16 tao_yyss;
  TAO_YYSTYPE tao_yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define TAO_YYSTACK_GAP_MAXIMUM (sizeof (union tao_yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define TAO_YYSTACK_BYTES(N) \
     ((N) * (sizeof (tao_yytype_int16) + sizeof (TAO_YYSTYPE)) \
      + TAO_YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef TAO_YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define TAO_YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define TAO_YYCOPY(To, From, Count)                \
      do                                        \
        {                                        \
          TAO_YYSIZE_T tao_yyi;                                \
          for (tao_yyi = 0; tao_yyi < (Count); tao_yyi++)        \
            (To)[tao_yyi] = (From)[tao_yyi];                \
        }                                        \
      while (TAO_YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables TAO_YYSIZE and TAO_YYSTACKSIZE give the old and new number of
   elements in the stack, and TAO_YYPTR gives the new location of the
   stack.  Advance TAO_YYPTR to a properly aligned location for the next
   stack.  */
# define TAO_YYSTACK_RELOCATE(Stack)                                        \
    do                                                                        \
      {                                                                        \
        TAO_YYSIZE_T tao_yynewbytes;                                                \
        TAO_YYCOPY (&tao_yyptr->Stack, Stack, tao_yysize);                                \
        Stack = &tao_yyptr->Stack;                                                \
        tao_yynewbytes = tao_yystacksize * sizeof (*Stack) + TAO_YYSTACK_GAP_MAXIMUM; \
        tao_yyptr += tao_yynewbytes / sizeof (*tao_yyptr);                                \
      }                                                                        \
    while (TAO_YYID (0))

#endif

/* TAO_YYFINAL -- State number of the termination state.  */
#define TAO_YYFINAL  3
/* TAO_YYLAST -- Last index in TAO_YYTABLE.  */
#define TAO_YYLAST   1197

/* TAO_YYNTOKENS -- Number of terminals.  */
#define TAO_YYNTOKENS  107
/* TAO_YYNNTS -- Number of nonterminals.  */
#define TAO_YYNNTS  340
/* TAO_YYNRULES -- Number of rules.  */
#define TAO_YYNRULES  507
/* TAO_YYNRULES -- Number of states.  */
#define TAO_YYNSTATES  719

/* TAO_YYTRANSLATE(TAO_YYLEX) -- Bison symbol number corresponding to TAO_YYLEX.  */
#define TAO_YYUNDEFTOK  2
#define TAO_YYMAXUTOK   340

#define TAO_YYTRANSLATE(TAO_YYX)                                                \
  ((unsigned int) (TAO_YYX) <= TAO_YYMAXUTOK ? tao_yytranslate[TAO_YYX] : TAO_YYUNDEFTOK)

/* TAO_YYTRANSLATE[TAO_YYLEX] -- Bison symbol number corresponding to TAO_YYLEX.  */
static const tao_yytype_uint8 tao_yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    99,    94,     2,
     101,   102,    97,    95,    90,    96,     2,    98,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    89,    86,
     104,    91,   103,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   105,     2,   106,    93,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    87,    92,    88,   100,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85
};

#if TAO_YYDEBUG
/* TAO_YYPRHS[TAO_YYN] -- Index of the first RHS symbol of rule number TAO_YYN in
   TAO_YYRHS.  */
static const tao_yytype_uint16 tao_yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    10,    14,    15,    19,
      20,    24,    25,    29,    30,    34,    35,    39,    40,    44,
      45,    49,    50,    54,    55,    59,    60,    64,    65,    69,
      70,    74,    75,    76,    77,    78,    88,    90,    92,    93,
      94,    95,   103,   104,   108,   111,   115,   119,   120,   125,
     126,   128,   130,   132,   134,   137,   139,   140,   141,   142,
     150,   151,   152,   153,   162,   163,   168,   169,   173,   175,
     176,   179,   180,   183,   185,   188,   191,   192,   194,   196,
     199,   202,   205,   208,   209,   210,   214,   215,   219,   220,
     224,   225,   229,   230,   234,   235,   239,   240,   244,   245,
     249,   252,   253,   258,   259,   261,   262,   266,   267,   272,
     274,   276,   279,   282,   283,   284,   285,   286,   296,   298,
     300,   302,   304,   306,   308,   310,   312,   314,   316,   318,
     320,   324,   326,   330,   332,   336,   338,   342,   346,   348,
     352,   356,   358,   362,   366,   370,   372,   375,   378,   381,
     383,   385,   389,   391,   393,   395,   397,   399,   401,   403,
     405,   407,   409,   410,   414,   416,   418,   420,   423,   425,
     426,   430,   432,   434,   436,   438,   440,   442,   444,   446,
     448,   450,   452,   454,   456,   458,   460,   462,   464,   466,
     468,   470,   472,   475,   476,   481,   482,   484,   486,   489,
     490,   495,   496,   498,   500,   502,   504,   506,   509,   511,
     514,   518,   521,   523,   525,   528,   530,   532,   534,   536,
     538,   540,   542,   543,   547,   548,   549,   550,   558,   561,
     564,   565,   566,   569,   570,   571,   577,   578,   582,   583,
     587,   588,   589,   590,   591,   592,   593,   608,   610,   612,
     614,   616,   618,   620,   623,   626,   627,   628,   629,   635,
     636,   640,   643,   646,   647,   648,   652,   653,   654,   660,
     661,   665,   667,   669,   670,   671,   672,   673,   683,   686,
     687,   692,   693,   695,   696,   697,   704,   707,   708,   709,
     715,   716,   717,   724,   726,   728,   729,   730,   737,   739,
     741,   742,   746,   749,   752,   753,   754,   755,   761,   763,
     765,   766,   767,   768,   769,   779,   780,   781,   782,   783,
     793,   794,   795,   796,   797,   807,   808,   809,   810,   811,
     822,   824,   826,   827,   829,   831,   832,   833,   834,   842,
     843,   847,   848,   853,   856,   857,   862,   863,   864,   865,
     871,   872,   876,   877,   882,   885,   886,   891,   892,   893,
     894,   900,   902,   904,   906,   908,   910,   912,   914,   915,
     916,   923,   924,   925,   926,   933,   934,   935,   936,   943,
     944,   945,   946,   953,   954,   957,   958,   963,   964,   968,
     972,   974,   976,   979,   980,   981,   982,   990,   991,   992,
     999,  1000,  1004,  1005,  1008,  1009,  1010,  1014,  1015,  1019,
    1020,  1024,  1025,  1029,  1030,  1034,  1035,  1039,  1043,  1045,
    1047,  1052,  1054,  1055,  1059,  1063,  1067,  1068,  1072,  1073,
    1074,  1075,  1076,  1077,  1078,  1092,  1093,  1097,  1098,  1101,
    1102,  1103,  1104,  1110,  1113,  1114,  1116,  1117,  1121,  1122,
    1126,  1127,  1128,  1135,  1136,  1137,  1144,  1146,  1148,  1150,
    1152,  1154,  1157,  1161,  1162,  1163,  1164,  1173,  1177,  1181,
    1184,  1185,  1189,  1190,  1191,  1192,  1201,  1203,  1205,  1207,
    1209,  1211,  1213,  1215,  1217,  1219,  1221,  1222,  1223,  1224,
    1232,  1233,  1237,  1238,  1242,  1247,  1251,  1252,  1255,  1258,
    1259,  1262,  1266,  1267,  1272,  1275,  1279,  1280
};

/* TAO_YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const tao_yytype_int16 tao_yyrhs[] =
{
     108,     0,    -1,   109,    -1,   109,   110,    -1,    -1,    -1,
     196,   111,    86,    -1,    -1,   357,   112,    86,    -1,    -1,
     358,   113,    86,    -1,    -1,   178,   114,    86,    -1,    -1,
     306,   115,    86,    -1,    -1,   129,   116,    86,    -1,    -1,
     429,   117,    86,    -1,    -1,   124,   118,    86,    -1,    -1,
     139,   119,    86,    -1,    -1,   359,   120,    86,    -1,    -1,
     385,   121,    86,    -1,    -1,   410,   122,    86,    -1,    -1,
       1,   123,    86,    -1,    -1,    -1,    -1,    -1,     5,   125,
       3,   126,    87,   127,   109,   128,    88,    -1,   130,    -1,
     177,    -1,    -1,    -1,    -1,   136,   131,    87,   132,   160,
     133,    88,    -1,    -1,     6,   135,   176,    -1,   134,   137,
      -1,    41,   134,   137,    -1,    42,   134,   137,    -1,    -1,
      89,   153,   138,   170,    -1,    -1,   140,    -1,   145,    -1,
     155,    -1,   156,    -1,    43,   141,    -1,   141,    -1,    -1,
      -1,    -1,   149,   142,    87,   143,   157,   144,    88,    -1,
      -1,    -1,    -1,    42,   149,   146,    87,   147,   160,   148,
      88,    -1,    -1,   151,   137,   150,   154,    -1,    -1,    49,
     152,   176,    -1,    48,    -1,    -1,    47,   170,    -1,    -1,
      42,   151,    -1,   151,    -1,   151,   200,    -1,   157,   158,
      -1,    -1,   159,    -1,   161,    -1,   318,    86,    -1,    46,
     235,    -1,    45,   235,    -1,   160,   161,    -1,    -1,    -1,
     196,   162,    86,    -1,    -1,   357,   163,    86,    -1,    -1,
     358,   164,    86,    -1,    -1,   178,   165,    86,    -1,    -1,
     306,   166,    86,    -1,    -1,   295,   167,    86,    -1,    -1,
     311,   168,    86,    -1,    -1,     1,   169,    86,    -1,   173,
     171,    -1,    -1,   171,    90,   172,   173,    -1,    -1,   176,
      -1,    -1,    81,   174,   176,    -1,    -1,   173,    81,   175,
     176,    -1,     3,    -1,   134,    -1,    41,   134,    -1,    42,
     134,    -1,    -1,    -1,    -1,    -1,     4,   179,   183,   180,
     176,   181,    91,   182,   184,    -1,   215,    -1,   220,    -1,
     221,    -1,   222,    -1,   218,    -1,   219,    -1,   280,    -1,
     284,    -1,   173,    -1,   185,    -1,   186,    -1,   187,    -1,
     186,    92,   187,    -1,   188,    -1,   187,    93,   188,    -1,
     189,    -1,   188,    94,   189,    -1,   190,    -1,   189,    82,
     190,    -1,   189,    83,   190,    -1,   191,    -1,   190,    95,
     191,    -1,   190,    96,   191,    -1,   192,    -1,   191,    97,
     192,    -1,   191,    98,   192,    -1,   191,    99,   192,    -1,
     193,    -1,    95,   193,    -1,    96,   193,    -1,   100,   193,
      -1,   173,    -1,   194,    -1,   101,   185,   102,    -1,    74,
      -1,    75,    -1,    76,    -1,    85,    -1,    77,    -1,    84,
      -1,    78,    -1,    79,    -1,    80,    -1,   185,    -1,    -1,
       7,   197,   198,    -1,   227,    -1,   241,    -1,   265,    -1,
      40,   213,    -1,   205,    -1,    -1,   200,   199,   206,    -1,
     201,    -1,   204,    -1,   202,    -1,   203,    -1,   173,    -1,
     215,    -1,   218,    -1,   220,    -1,   222,    -1,   221,    -1,
     219,    -1,   223,    -1,   224,    -1,   274,    -1,   280,    -1,
     284,    -1,   227,    -1,   241,    -1,   265,    -1,   263,    -1,
     264,    -1,   209,   207,    -1,    -1,   207,    90,   208,   209,
      -1,    -1,   213,    -1,   214,    -1,   213,   211,    -1,    -1,
     211,    90,   212,   213,    -1,    -1,   176,    -1,   288,    -1,
     216,    -1,   217,    -1,     8,    -1,     8,     8,    -1,     9,
      -1,    10,     8,    -1,    10,     8,     8,    -1,    10,     9,
      -1,    11,    -1,    12,    -1,     8,    11,    -1,    17,    -1,
      13,    -1,    14,    -1,    15,    -1,    16,    -1,    18,    -1,
      19,    -1,    -1,    20,   226,   176,    -1,    -1,    -1,    -1,
     225,   228,    87,   229,   231,   230,    88,    -1,   233,   232,
      -1,   232,   233,    -1,    -1,    -1,   234,   235,    -1,    -1,
      -1,   200,   236,   206,   237,    86,    -1,    -1,     1,   238,
      86,    -1,    -1,    21,   240,   176,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   239,    22,   242,   101,   243,   248,   244,
     102,   245,    87,   246,   249,   247,    88,    -1,   215,    -1,
     220,    -1,   221,    -1,   222,    -1,   265,    -1,   173,    -1,
     251,   250,    -1,   250,   251,    -1,    -1,    -1,    -1,   255,
     252,   261,   253,    86,    -1,    -1,     1,   254,    86,    -1,
     257,   256,    -1,   256,   257,    -1,    -1,    -1,    29,   258,
      89,    -1,    -1,    -1,    28,   259,   185,   260,    89,    -1,
      -1,   200,   262,   209,    -1,   225,    -1,   239,    -1,    -1,
      -1,    -1,    -1,    23,   266,   176,   267,    87,   268,   270,
     269,    88,    -1,   273,   271,    -1,    -1,   271,    90,   272,
     273,    -1,    -1,     3,    -1,    -1,    -1,   277,    90,   275,
     195,   276,   103,    -1,   277,   103,    -1,    -1,    -1,    24,
     278,   104,   279,   201,    -1,    -1,    -1,   283,   104,   281,
     195,   282,   103,    -1,   283,    -1,    25,    -1,    -1,    -1,
     287,   104,   285,   195,   286,   103,    -1,   287,    -1,    26,
      -1,    -1,   176,   289,   290,    -1,   292,   291,    -1,   291,
     292,    -1,    -1,    -1,    -1,   105,   293,   195,   294,   106,
      -1,   296,    -1,   301,    -1,    -1,    -1,    -1,    -1,    30,
     297,    31,   298,   340,   299,   210,   300,   342,    -1,    -1,
      -1,    -1,    -1,    31,   302,   340,   303,   210,   304,   345,
     305,   348,    -1,    -1,    -1,    -1,    -1,    27,   307,   176,
     308,    87,   309,   232,   310,    88,    -1,    -1,    -1,    -1,
      -1,   316,   317,   312,     3,   313,   331,   314,   342,   315,
     351,    -1,    32,    -1,    33,    -1,    -1,   340,    -1,    34,
      -1,    -1,    -1,    -1,    44,   319,     3,   320,   322,   321,
     342,    -1,    -1,   101,   323,   102,    -1,    -1,   101,   324,
     325,   102,    -1,   328,   326,    -1,    -1,   326,    90,   327,
     328,    -1,    -1,    -1,    -1,    35,   329,   340,   330,   209,
      -1,    -1,   101,   332,   102,    -1,    -1,   101,   333,   334,
     102,    -1,   337,   335,    -1,    -1,   335,    90,   336,   337,
      -1,    -1,    -1,    -1,   341,   338,   340,   339,   209,    -1,
     202,    -1,   280,    -1,   284,    -1,   173,    -1,    35,    -1,
      36,    -1,    37,    -1,    -1,    -1,    38,   343,   101,   344,
     170,   102,    -1,    -1,    -1,    -1,    55,   346,   101,   347,
     170,   102,    -1,    -1,    -1,    -1,    62,   349,   101,   350,
     170,   102,    -1,    -1,    -1,    -1,    39,   352,   101,   353,
     354,   102,    -1,    -1,    76,   355,    -1,    -1,   355,    90,
     356,    76,    -1,    -1,    63,   173,    76,    -1,    64,   173,
      76,    -1,   361,    -1,   360,    -1,    50,   176,    -1,    -1,
      -1,    -1,   365,   362,    87,   363,   370,   364,    88,    -1,
      -1,    -1,    50,   176,   366,   368,   367,   154,    -1,    -1,
      89,   369,   173,    -1,    -1,   370,   371,    -1,    -1,    -1,
     378,   372,    86,    -1,    -1,   380,   373,    86,    -1,    -1,
     382,   374,    86,    -1,    -1,   383,   375,    86,    -1,    -1,
     384,   376,    86,    -1,    -1,   295,   377,    86,    -1,    60,
     379,   176,    -1,   173,    -1,    19,    -1,    65,   381,   379,
     176,    -1,    58,    -1,    -1,    52,   173,   176,    -1,    61,
     173,   176,    -1,    51,   173,   176,    -1,    -1,   387,   386,
     397,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,   388,
     176,   389,   394,   390,   154,   391,    66,   392,   173,   393,
     396,    -1,    -1,    89,   395,   173,    -1,    -1,    59,   173,
      -1,    -1,    -1,    -1,    87,   398,   400,   399,    88,    -1,
     400,   401,    -1,    -1,   161,    -1,    -1,   404,   402,    86,
      -1,    -1,   407,   403,    86,    -1,    -1,    -1,    44,   176,
     405,   322,   406,   342,    -1,    -1,    -1,    54,   176,   408,
     322,   409,   342,    -1,   423,    -1,   414,    -1,   411,    -1,
     413,    -1,   412,    -1,    53,   176,    -1,    42,    53,   176,
      -1,    -1,    -1,    -1,   418,   421,   415,    87,   416,   160,
     417,    88,    -1,    42,    53,   176,    -1,    43,    53,   176,
      -1,    53,   176,    -1,    -1,   137,   422,   154,    -1,    -1,
      -1,    -1,   427,   421,   424,    87,   425,   157,   426,    88,
      -1,   419,    -1,   420,    -1,    67,    -1,    20,    -1,    53,
      -1,    68,    -1,    17,    -1,    24,    -1,     6,    -1,    49,
      -1,    -1,    -1,    -1,   433,   430,    87,   431,   160,   432,
      88,    -1,    -1,   435,   434,   440,    -1,    -1,   134,   436,
     437,    -1,   104,   439,   438,   103,    -1,   438,    90,   439,
      -1,    -1,   428,     3,    -1,    89,   441,    -1,    -1,   443,
     442,    -1,   442,    90,   443,    -1,    -1,   173,   104,   444,
     103,    -1,   446,   445,    -1,   445,    90,   446,    -1,    -1,
       3,    -1
};

/* TAO_YYRLINE[TAO_YYN] -- source line where rule number TAO_YYN was defined.  */
static const tao_yytype_uint16 tao_yyrline[] =
{
       0,   331,   331,   334,   335,   340,   339,   350,   349,   360,
     359,   370,   369,   380,   379,   390,   389,   401,   400,   412,
     411,   422,   421,   432,   431,   442,   441,   452,   451,   462,
     461,   475,   480,   506,   511,   474,   527,   528,   533,   572,
     577,   532,   595,   594,   608,   646,   677,   711,   710,   724,
     732,   733,   734,   735,   739,   750,   755,   799,   804,   754,
     833,   872,   877,   831,   896,   894,   934,   933,   947,   953,
     960,   967,   974,  1000,  1027,  1092,  1093,  1097,  1098,  1099,
    1104,  1110,  1119,  1120,  1125,  1124,  1135,  1134,  1145,  1144,
    1155,  1154,  1165,  1164,  1175,  1174,  1185,  1184,  1195,  1194,
    1208,  1221,  1219,  1249,  1256,  1267,  1266,  1294,  1292,  1319,
    1331,  1377,  1405,  1436,  1441,  1446,  1451,  1435,  1504,  1505,
    1506,  1507,  1508,  1509,  1510,  1522,  1527,  1602,  1604,  1606,
    1607,  1621,  1622,  1636,  1637,  1650,  1651,  1661,  1674,  1675,
    1685,  1698,  1699,  1709,  1719,  1732,  1733,  1743,  1753,  1766,
    1801,  1802,  1811,  1816,  1823,  1831,  1836,  1841,  1847,  1852,
    1857,  1865,  1937,  1936,  1946,  1951,  1956,  1961,  1988,  1997,
    1996,  2067,  2068,  2072,  2080,  2081,  2109,  2110,  2111,  2112,
    2113,  2114,  2115,  2116,  2120,  2121,  2122,  2126,  2127,  2128,
    2132,  2133,  2137,  2150,  2148,  2178,  2185,  2186,  2190,  2203,
    2201,  2231,  2238,  2255,  2274,  2275,  2279,  2284,  2289,  2297,
    2302,  2307,  2315,  2320,  2325,  2333,  2347,  2352,  2360,  2368,
    2376,  2384,  2393,  2392,  2408,  2442,  2447,  2407,  2466,  2469,
    2470,  2474,  2474,  2484,  2489,  2483,  2552,  2551,  2566,  2565,
    2580,  2585,  2590,  2595,  2642,  2647,  2579,  2671,  2679,  2693,
    2703,  2711,  2712,  2820,  2823,  2824,  2829,  2834,  2828,  2870,
    2869,  2883,  2894,  2916,  2924,  2923,  2939,  2944,  2938,  2961,
    2960,  3013,  3037,  3062,  3067,  3100,  3105,  3061,  3131,  3136,
    3134,  3141,  3145,  3182,  3187,  3180,  3254,  3309,  3319,  3308,
    3334,  3339,  3332,  3380,  3406,  3416,  3421,  3414,  3451,  3476,
    3485,  3484,  3520,  3531,  3553,  3561,  3566,  3560,  3603,  3604,
    3609,  3614,  3619,  3624,  3608,  3693,  3698,  3703,  3708,  3692,
    3786,  3791,  3821,  3826,  3785,  3844,  3849,  3902,  3907,  3842,
    3944,  3950,  3957,  3964,  3965,  3977,  3983,  4025,  3976,  4047,
    4046,  4057,  4056,  4069,  4074,  4072,  4079,  4084,  4089,  4083,
    4130,  4129,  4140,  4139,  4152,  4157,  4155,  4162,  4167,  4172,
    4166,  4219,  4227,  4228,  4229,  4317,  4322,  4327,  4336,  4341,
    4335,  4353,  4361,  4366,  4360,  4378,  4386,  4391,  4385,  4403,
    4411,  4416,  4410,  4428,  4435,  4448,  4446,  4474,  4481,  4510,
    4548,  4549,  4553,  4583,  4623,  4628,  4582,  4647,  4652,  4645,
    4694,  4693,  4704,  4711,  4712,  4717,  4716,  4727,  4726,  4737,
    4736,  4747,  4746,  4757,  4756,  4767,  4766,  4779,  4848,  4855,
    4880,  4958,  4964,  4971,  5034,  5097,  5161,  5160,  5210,  5215,
    5220,  5225,  5230,  5235,  5209,  5289,  5288,  5299,  5306,  5313,
    5321,  5326,  5320,  5338,  5339,  5343,  5345,  5344,  5355,  5354,
    5369,  5405,  5367,  5439,  5475,  5437,  5507,  5508,  5509,  5513,
    5514,  5518,  5546,  5577,  5622,  5627,  5575,  5644,  5654,  5673,
    5685,  5684,  5724,  5774,  5779,  5722,  5796,  5801,  5809,  5815,
    5820,  5825,  5830,  5843,  5848,  5853,  5862,  5867,  5872,  5861,
    5890,  5889,  5903,  5902,  5915,  5923,  5929,  5935,  5943,  5948,
    5954,  5961,  5966,  5972,  5979,  5986,  5991,  5997
};
#endif

#if TAO_YYDEBUG || TAO_YYERROR_VERBOSE || TAO_YYTOKEN_TABLE
/* TAO_YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at TAO_YYNTOKENS, nonterminals.  */
static const char *const tao_yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "IDL_CONST", "IDL_MODULE",
  "IDL_INTERFACE", "IDL_TYPEDEF", "IDL_LONG", "IDL_SHORT", "IDL_UNSIGNED",
  "IDL_DOUBLE", "IDL_FLOAT", "IDL_CHAR", "IDL_WCHAR", "IDL_OCTET",
  "IDL_BOOLEAN", "IDL_FIXED", "IDL_ANY", "IDL_OBJECT", "IDL_STRUCT",
  "IDL_UNION", "IDL_SWITCH", "IDL_ENUM", "IDL_SEQUENCE", "IDL_STRING",
  "IDL_WSTRING", "IDL_EXCEPTION", "IDL_CASE", "IDL_DEFAULT",
  "IDL_READONLY", "IDL_ATTRIBUTE", "IDL_ONEWAY", "IDL_IDEMPOTENT",
  "IDL_VOID", "IDL_IN", "IDL_OUT", "IDL_INOUT", "IDL_RAISES",
  "IDL_CONTEXT", "IDL_NATIVE", "IDL_LOCAL", "IDL_ABSTRACT", "IDL_CUSTOM",
  "IDL_FACTORY", "IDL_PRIVATE", "IDL_PUBLIC", "IDL_SUPPORTS",
  "IDL_TRUNCATABLE", "IDL_VALUETYPE", "IDL_COMPONENT", "IDL_CONSUMES",
  "IDL_EMITS", "IDL_EVENTTYPE", "IDL_FINDER", "IDL_GETRAISES", "IDL_HOME",
  "IDL_IMPORT", "IDL_MULTIPLE", "IDL_PRIMARYKEY", "IDL_PROVIDES",
  "IDL_PUBLISHES", "IDL_SETRAISES", "IDL_TYPEID", "IDL_TYPEPREFIX",
  "IDL_USES", "IDL_MANAGES", "IDL_TYPENAME", "IDL_PRIMITIVE", "IDL_PORT",
  "IDL_MIRRORPORT", "IDL_PORTTYPE", "IDL_CONNECTOR", "IDL_CONCAT",
  "IDL_INTEGER_LITERAL", "IDL_UINTEGER_LITERAL", "IDL_STRING_LITERAL",
  "IDL_CHARACTER_LITERAL", "IDL_FLOATING_PT_LITERAL", "IDL_TRUETOK",
  "IDL_FALSETOK", "IDL_SCOPE_DELIMITOR", "IDL_LEFT_SHIFT",
  "IDL_RIGHT_SHIFT", "IDL_WCHAR_LITERAL", "IDL_WSTRING_LITERAL", "';'",
  "'{'", "'}'", "':'", "','", "'='", "'|'", "'^'", "'&'", "'+'", "'-'",
  "'*'", "'/'", "'%'", "'~'", "'('", "')'", "'>'", "'<'", "'['", "']'",
  "$accept", "start", "definitions", "definition", "@1", "@2", "@3", "@4",
  "@5", "@6", "@7", "@8", "@9", "@10", "@11", "@12", "@13", "module",
  "@14", "@15", "@16", "@17", "interface_def", "interface", "@18", "@19",
  "@20", "interface_decl", "@21", "interface_header", "inheritance_spec",
  "@22", "value_def", "valuetype", "value_concrete_decl", "@23", "@24",
  "@25", "value_abs_decl", "@26", "@27", "@28", "value_header", "@29",
  "value_decl", "@30", "opt_truncatable", "supports_spec",
  "value_forward_decl", "value_box_decl", "value_elements",
  "value_element", "state_member", "exports", "export", "@31", "@32",
  "@33", "@34", "@35", "@36", "@37", "@38", "at_least_one_scoped_name",
  "scoped_names", "@39", "scoped_name", "@40", "@41", "id",
  "interface_forward", "const_dcl", "@42", "@43", "@44", "@45",
  "const_type", "expression", "const_expr", "or_expr", "xor_expr",
  "and_expr", "shift_expr", "add_expr", "mult_expr", "unary_expr",
  "primary_expr", "literal", "positive_int_expr", "type_dcl", "@46",
  "type_declarator", "@47", "type_spec", "simple_type_spec",
  "base_type_spec", "template_type_spec", "constructed_type_spec",
  "constructed_forward_type_spec", "at_least_one_declarator",
  "declarators", "@48", "declarator", "at_least_one_simple_declarator",
  "simple_declarators", "@49", "simple_declarator", "complex_declarator",
  "integer_type", "signed_int", "unsigned_int", "floating_pt_type",
  "fixed_type", "char_type", "octet_type", "boolean_type", "any_type",
  "object_type", "struct_decl", "@50", "struct_type", "@51", "@52", "@53",
  "at_least_one_member", "members", "member", "@54", "member_i", "@55",
  "@56", "@57", "union_decl", "@58", "union_type", "@59", "@60", "@61",
  "@62", "@63", "@64", "switch_type_spec", "at_least_one_case_branch",
  "case_branches", "case_branch", "@65", "@66", "@67",
  "at_least_one_case_label", "case_labels", "case_label", "@68", "@69",
  "@70", "element_spec", "@71", "struct_forward_type",
  "union_forward_type", "enum_type", "@72", "@73", "@74", "@75",
  "at_least_one_enumerator", "enumerators", "@76", "enumerator",
  "sequence_type_spec", "@77", "@78", "seq_head", "@79", "@80",
  "string_type_spec", "@81", "@82", "string_head", "wstring_type_spec",
  "@83", "@84", "wstring_head", "array_declarator", "@85",
  "at_least_one_array_dim", "array_dims", "array_dim", "@86", "@87",
  "attribute", "attribute_readonly", "@88", "@89", "@90", "@91",
  "attribute_readwrite", "@92", "@93", "@94", "@95", "exception", "@96",
  "@97", "@98", "@99", "operation", "@100", "@101", "@102", "@103",
  "opt_op_attribute", "op_type_spec", "init_decl", "@104", "@105", "@106",
  "init_parameter_list", "@107", "@108", "at_least_one_in_parameter",
  "in_parameters", "@109", "in_parameter", "@110", "@111",
  "parameter_list", "@112", "@113", "at_least_one_parameter", "parameters",
  "@114", "parameter", "@115", "@116", "param_type_spec", "direction",
  "opt_raises", "@117", "@118", "opt_getraises", "@119", "@120",
  "opt_setraises", "@121", "@122", "opt_context", "@123", "@124",
  "at_least_one_string_literal", "string_literals", "@125", "typeid_dcl",
  "typeprefix_dcl", "component", "component_forward_decl",
  "component_decl", "@126", "@127", "@128", "component_header", "@129",
  "@130", "component_inheritance_spec", "@131", "component_exports",
  "component_export", "@132", "@133", "@134", "@135", "@136", "@137",
  "provides_decl", "interface_type", "uses_decl", "opt_multiple",
  "emits_decl", "publishes_decl", "consumes_decl", "home_decl", "@138",
  "home_header", "@139", "@140", "@141", "@142", "@143", "@144",
  "home_inheritance_spec", "@145", "primary_key_spec", "home_body", "@146",
  "@147", "home_exports", "home_export", "@148", "@149", "factory_decl",
  "@150", "@151", "finder_decl", "@152", "@153", "event",
  "event_forward_decl", "event_concrete_forward_decl",
  "event_abs_forward_decl", "event_abs_decl", "@154", "@155", "@156",
  "event_abs_header", "event_custom_header", "event_plain_header",
  "event_rest_of_header", "@157", "event_decl", "@158", "@159", "@160",
  "event_header", "type_classifier", "template_interface_def", "@161",
  "@162", "@163", "template_interface_header", "@164",
  "template_interface_decl", "@165", "at_least_one_template_param",
  "template_params", "template_param", "template_inheritance_spec",
  "at_least_one_template_ref", "template_refs", "template_ref",
  "at_least_one_template_param_ref", "template_param_refs",
  "template_param_ref", 0
};
#endif

# ifdef TAO_YYPRINT
/* TAO_YYTOKNUM[TAO_YYLEX-NUM] -- Internal token number corresponding to
   token TAO_YYLEX-NUM.  */
static const tao_yytype_uint16 tao_yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,    59,   123,   125,    58,
      44,    61,   124,    94,    38,    43,    45,    42,    47,    37,
     126,    40,    41,    62,    60,    91,    93
};
# endif

/* TAO_YYR1[TAO_YYN] -- Symbol number of symbol that rule TAO_YYN derives.  */
static const tao_yytype_uint16 tao_yyr1[] =
{
       0,   107,   108,   109,   109,   111,   110,   112,   110,   113,
     110,   114,   110,   115,   110,   116,   110,   117,   110,   118,
     110,   119,   110,   120,   110,   121,   110,   122,   110,   123,
     110,   125,   126,   127,   128,   124,   129,   129,   131,   132,
     133,   130,   135,   134,   136,   136,   136,   138,   137,   137,
     139,   139,   139,   139,   140,   140,   142,   143,   144,   141,
     146,   147,   148,   145,   150,   149,   152,   151,   153,   153,
     154,   154,   155,   155,   156,   157,   157,   158,   158,   158,
     159,   159,   160,   160,   162,   161,   163,   161,   164,   161,
     165,   161,   166,   161,   167,   161,   168,   161,   169,   161,
     170,   172,   171,   171,   173,   174,   173,   175,   173,   176,
     177,   177,   177,   179,   180,   181,   182,   178,   183,   183,
     183,   183,   183,   183,   183,   183,   183,   184,   185,   186,
     186,   187,   187,   188,   188,   189,   189,   189,   190,   190,
     190,   191,   191,   191,   191,   192,   192,   192,   192,   193,
     193,   193,   194,   194,   194,   194,   194,   194,   194,   194,
     194,   195,   197,   196,   196,   196,   196,   196,   196,   199,
     198,   200,   200,   201,   201,   201,   202,   202,   202,   202,
     202,   202,   202,   202,   203,   203,   203,   204,   204,   204,
     205,   205,   206,   208,   207,   207,   209,   209,   210,   212,
     211,   211,   213,   214,   215,   215,   216,   216,   216,   217,
     217,   217,   218,   218,   218,   219,   220,   220,   221,   222,
     223,   224,   226,   225,   228,   229,   230,   227,   231,   232,
     232,   234,   233,   236,   237,   235,   238,   235,   240,   239,
     242,   243,   244,   245,   246,   247,   241,   248,   248,   248,
     248,   248,   248,   249,   250,   250,   252,   253,   251,   254,
     251,   255,   256,   256,   258,   257,   259,   260,   257,   262,
     261,   263,   264,   266,   267,   268,   269,   265,   270,   272,
     271,   271,   273,   275,   276,   274,   274,   278,   279,   277,
     281,   282,   280,   280,   283,   285,   286,   284,   284,   287,
     289,   288,   290,   291,   291,   293,   294,   292,   295,   295,
     297,   298,   299,   300,   296,   302,   303,   304,   305,   301,
     307,   308,   309,   310,   306,   312,   313,   314,   315,   311,
     316,   316,   316,   317,   317,   319,   320,   321,   318,   323,
     322,   324,   322,   325,   327,   326,   326,   329,   330,   328,
     332,   331,   333,   331,   334,   336,   335,   335,   338,   339,
     337,   340,   340,   340,   340,   341,   341,   341,   343,   344,
     342,   342,   346,   347,   345,   345,   349,   350,   348,   348,
     352,   353,   351,   351,   354,   356,   355,   355,   357,   358,
     359,   359,   360,   362,   363,   364,   361,   366,   367,   365,
     369,   368,   368,   370,   370,   372,   371,   373,   371,   374,
     371,   375,   371,   376,   371,   377,   371,   378,   379,   379,
     380,   381,   381,   382,   383,   384,   386,   385,   388,   389,
     390,   391,   392,   393,   387,   395,   394,   394,   396,   396,
     398,   399,   397,   400,   400,   401,   402,   401,   403,   401,
     405,   406,   404,   408,   409,   407,   410,   410,   410,   411,
     411,   412,   413,   415,   416,   417,   414,   418,   419,   420,
     422,   421,   424,   425,   426,   423,   427,   427,   428,   428,
     428,   428,   428,   428,   428,   428,   430,   431,   432,   429,
     434,   433,   436,   435,   437,   438,   438,   439,   440,   440,
     441,   442,   442,   443,   444,   445,   445,   446
};

/* TAO_YYR2[TAO_YYN] -- Number of symbols composing right hand side of rule TAO_YYN.  */
static const tao_yytype_uint8 tao_yyr2[] =
{
       0,     2,     1,     2,     0,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     0,     0,     0,     9,     1,     1,     0,     0,
       0,     7,     0,     3,     2,     3,     3,     0,     4,     0,
       1,     1,     1,     1,     2,     1,     0,     0,     0,     7,
       0,     0,     0,     8,     0,     4,     0,     3,     1,     0,
       2,     0,     2,     1,     2,     2,     0,     1,     1,     2,
       2,     2,     2,     0,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       2,     0,     4,     0,     1,     0,     3,     0,     4,     1,
       1,     2,     2,     0,     0,     0,     0,     9,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     3,     1,     3,     1,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     1,     2,     2,     2,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     3,     1,     1,     1,     2,     1,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     0,     4,     0,     1,     1,     2,     0,
       4,     0,     1,     1,     1,     1,     1,     2,     1,     2,
       3,     2,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     0,     3,     0,     0,     0,     7,     2,     2,
       0,     0,     2,     0,     0,     5,     0,     3,     0,     3,
       0,     0,     0,     0,     0,     0,    14,     1,     1,     1,
       1,     1,     1,     2,     2,     0,     0,     0,     5,     0,
       3,     2,     2,     0,     0,     3,     0,     0,     5,     0,
       3,     1,     1,     0,     0,     0,     0,     9,     2,     0,
       4,     0,     1,     0,     0,     6,     2,     0,     0,     5,
       0,     0,     6,     1,     1,     0,     0,     6,     1,     1,
       0,     3,     2,     2,     0,     0,     0,     5,     1,     1,
       0,     0,     0,     0,     9,     0,     0,     0,     0,     9,
       0,     0,     0,     0,     9,     0,     0,     0,     0,    10,
       1,     1,     0,     1,     1,     0,     0,     0,     7,     0,
       3,     0,     4,     2,     0,     4,     0,     0,     0,     5,
       0,     3,     0,     4,     2,     0,     4,     0,     0,     0,
       5,     1,     1,     1,     1,     1,     1,     1,     0,     0,
       6,     0,     0,     0,     6,     0,     0,     0,     6,     0,
       0,     0,     6,     0,     2,     0,     4,     0,     3,     3,
       1,     1,     2,     0,     0,     0,     7,     0,     0,     6,
       0,     3,     0,     2,     0,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     3,     1,     1,
       4,     1,     0,     3,     3,     3,     0,     3,     0,     0,
       0,     0,     0,     0,    13,     0,     3,     0,     2,     0,
       0,     0,     5,     2,     0,     1,     0,     3,     0,     3,
       0,     0,     6,     0,     0,     6,     1,     1,     1,     1,
       1,     2,     3,     0,     0,     0,     8,     3,     3,     2,
       0,     3,     0,     0,     0,     8,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     0,     0,     7,
       0,     3,     0,     3,     4,     3,     0,     2,     2,     0,
       2,     3,     0,     4,     2,     3,     0,     1
};

/* TAO_YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when TAO_YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const tao_yytype_uint16 tao_yydefact[] =
{
       4,     0,     0,     1,    29,   113,    31,    42,   162,   222,
     238,   273,   320,     0,     0,     0,     0,    66,     0,     0,
     428,     0,     0,     3,    19,    15,    36,    49,    38,    21,
      50,    55,    51,    56,    49,    52,    53,    37,    11,     5,
     168,   224,   164,   272,   165,   190,   191,   166,    13,     7,
       9,    23,   391,   390,   393,    25,   426,    27,   458,   460,
     459,   457,    49,   476,   477,   456,    49,    17,   486,   490,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   109,
     202,   167,    49,     0,    49,    60,    49,     0,    54,    49,
       0,   397,   469,     0,   105,     0,   104,     0,     0,     0,
      69,    44,     0,     0,     0,     0,   206,   208,     0,   212,
     213,   216,   217,   218,   219,   215,   220,   221,   287,   294,
     299,    64,   175,    74,   171,   173,   174,   172,   176,   204,
     205,   177,   181,   178,   180,   179,   182,   183,   224,   187,
       0,   188,   189,   184,     0,   185,   293,   186,   298,     0,
       0,     0,   240,     0,     0,     0,     0,     0,     0,     0,
       0,   470,   463,   472,     0,     0,   499,    30,   126,   114,
     118,   122,   123,   119,   120,   121,   124,   125,    32,    43,
     163,   169,   223,   239,   274,   321,    45,   467,    46,     0,
     468,    67,   402,   429,     0,   388,   107,   389,    20,    16,
      68,    47,     0,   493,    39,    22,    57,   207,   214,   209,
     211,     0,    71,   283,   286,   290,   295,    12,     6,   225,
       0,    14,     8,    10,    24,   394,    26,   440,   427,    28,
      71,     0,     0,    18,   487,     0,   491,     0,     0,     0,
       0,     0,    61,   400,   398,   437,   106,     0,     0,   484,
     482,   479,   483,   485,   480,   478,   481,     0,   496,    83,
      76,   210,   288,     0,    65,     0,     0,     0,   231,   241,
     404,   444,   471,   464,   473,    83,     0,   498,   502,   115,
      33,   202,   170,   195,   196,   197,   203,   275,   322,    83,
       0,    71,   435,   430,   108,    48,   103,   497,     0,     0,
       0,     0,    70,   152,   153,   154,   156,   158,   159,   160,
     157,   155,     0,     0,     0,     0,   149,   161,   128,   129,
     131,   133,   135,   138,   141,   145,   150,   284,   291,   296,
     226,   230,     0,     0,   395,     0,    83,    76,     0,     0,
     500,     0,     4,     0,   192,     0,   230,     0,   401,   399,
       0,    71,   100,     0,   494,    98,   310,   315,   330,   331,
       0,    82,    90,    84,    94,   308,   309,    92,    96,     0,
      86,    88,   335,     0,     0,     0,    75,    77,    78,     0,
     289,   146,   147,   148,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   231,
     236,   233,   232,   206,   252,   247,   248,   249,   250,   242,
     251,     0,     0,     0,     0,   422,   415,     0,   403,   405,
     407,   409,   411,   413,     0,     0,   445,     0,   443,   446,
     448,     0,     0,     0,   507,     0,   506,     0,   116,     0,
     305,   301,   304,   193,   282,   276,   281,   231,     0,   436,
     431,   101,   495,     0,     0,     0,    41,     0,     0,     0,
       0,     0,   334,   364,   361,   362,   363,   325,   333,     0,
       0,     0,    81,    80,    59,    79,   151,   130,   132,   134,
     136,   137,   139,   140,   142,   143,   144,   285,   292,   297,
     227,   229,     0,     0,     0,     0,     0,   419,   418,     0,
       0,   421,     0,     0,   396,     0,     0,     0,     0,     0,
     450,   453,   442,     0,     0,     0,     0,   489,   503,   504,
     501,     0,     0,     0,   302,     0,     0,   278,     0,    63,
       0,     0,    99,   311,   316,    91,    85,    95,    93,    97,
       0,    87,    89,   336,   237,   234,   243,   425,   423,   417,
     424,     0,   416,   406,   408,   410,   412,   414,     0,     0,
     447,   449,   466,   475,     0,   117,   127,    35,   306,   303,
     194,   277,   279,   324,   432,   102,     0,     0,   326,     0,
       0,     0,   420,   339,   451,   454,   505,     0,     0,     0,
     312,   317,   201,     0,   337,   235,   244,     0,     0,   371,
     371,   307,   280,   433,     0,   375,   198,   352,   327,   371,
       0,   340,   347,     0,   346,   368,   452,   455,   439,   313,
     372,   318,   199,     0,     0,   371,   338,   259,   266,   264,
     245,   255,   256,   263,     0,   342,   343,     0,     0,   434,
     371,     0,   379,     0,   351,   365,   366,   367,     0,   357,
     358,   328,     0,     0,     0,     0,     0,     0,   261,   348,
     344,   369,   438,   314,   373,   376,   319,   200,   353,   354,
       0,   383,   260,   267,   265,   246,   254,   269,   257,   262,
       0,     0,     0,     0,     0,   355,   359,   380,   329,     0,
       0,     0,   349,   345,     0,     0,   377,     0,     0,     0,
     268,   270,   258,   370,   374,     0,   356,   360,   381,     0,
       0,   378,   387,     0,   384,   382,   385,     0,   386
};

/* TAO_YYDEFGOTO[NTERM-NUM].  */
static const tao_yytype_int16 tao_yydefgoto[] =
{
      -1,     1,     2,    23,   150,   154,   155,   149,   153,    99,
     164,    98,   104,   156,   158,   160,    70,    24,    72,   238,
     342,   522,    25,    26,   103,   259,   360,    27,    73,    28,
     121,   248,    29,    30,    31,   105,   260,   375,    32,   189,
     289,   448,    33,   212,    34,    90,   201,   264,    35,    36,
     300,   376,   377,   299,   361,   458,   469,   470,   457,   460,
     459,   461,   453,   295,   352,   531,   316,   194,   247,    96,
      37,   362,    71,   237,   341,   521,   169,   565,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   363,
      74,   180,   239,   401,   124,   125,   126,   127,    40,   282,
     344,   525,   283,   591,   606,   643,   284,   285,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,    41,    75,
      42,   151,   268,   398,   330,   399,   491,   332,   402,   493,
     580,   492,    43,    76,    44,   220,   333,   494,   581,   610,
     655,   409,   630,   656,   631,   657,   691,   652,   632,   658,
     633,   654,   653,   689,   678,   690,    45,    46,    47,    77,
     240,   345,   526,   445,   527,   588,   446,   143,   265,   395,
     144,   211,   301,   145,   266,   396,   146,   147,   267,   397,
     148,   286,   343,   441,   524,   442,   523,   587,   364,   365,
     454,   576,   604,   640,   366,   455,   577,   605,   642,   367,
      78,   241,   346,   528,   368,   540,   593,   625,   671,   369,
     467,   379,   471,   579,   609,   584,   597,   598,   613,   636,
     681,   614,   634,   680,   608,   623,   624,   648,   669,   697,
     649,   670,   698,   468,   650,   616,   637,   682,   621,   641,
     683,   666,   684,   705,   688,   699,   710,   713,   714,   717,
     370,   371,    51,    52,    53,   157,   270,   417,    54,   192,
     291,   244,   290,   334,   418,   505,   506,   507,   508,   509,
     503,   419,   499,   420,   502,   421,   422,   423,    55,   159,
      56,    93,   245,   351,   530,   589,   618,   293,   350,   639,
     228,   271,   427,   335,   428,   513,   514,   429,   558,   599,
     430,   559,   600,    57,    58,    59,    60,    61,   231,   336,
     515,    62,    63,    64,   162,   230,    65,   232,   337,   516,
      66,   257,    67,   165,   275,   433,    68,   166,    69,   102,
     203,   298,   258,   236,   277,   340,   278,   435,   519,   436
};

/* TAO_YYPACT[STATE-NUM] -- Index in TAO_YYTABLE of the portion describing
   STATE-NUM.  */
#define TAO_YYPACT_NINF -519
static const tao_yytype_int16 tao_yypact[] =
{
    -519,    52,  1133,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,    27,    41,    30,    50,  -519,    27,    27,
    -519,    35,    35,  -519,  -519,  -519,  -519,    37,  -519,  -519,
    -519,  -519,  -519,  -519,   361,  -519,  -519,  -519,  -519,  -519,
    -519,   -19,  -519,    56,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,     2,  -519,  -519,  -519,     2,  -519,  -519,  -519,
      10,   645,    66,    27,   482,    27,    27,    27,    27,  -519,
    -519,  -519,    12,    27,    60,  -519,    62,    27,  -519,     2,
      27,    14,    75,    27,  -519,   -41,  -519,   -30,    88,    94,
      74,  -519,    84,    95,   116,   126,   136,  -519,    96,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,   133,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
      56,  -519,  -519,  -519,   -17,  -519,   111,  -519,   121,   137,
     141,   142,  -519,   146,   147,   151,   154,   144,   157,   159,
     158,  -519,  -519,  -519,   161,   163,   160,  -519,   133,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,   165,  -519,   166,
    -519,  -519,   170,  -519,    27,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,   119,  -519,  -519,  -519,  -519,  -519,  -519,   233,
    -519,   148,   201,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
     153,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
     201,   173,   174,  -519,  -519,    35,  -519,    27,   175,    27,
     176,   179,  -519,  -519,  -519,   178,  -519,    27,    35,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,   266,  -519,  -519,
    -519,  -519,  -519,    35,  -519,    34,    34,    34,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,   -47,  -519,  -519,  -519,
    -519,   168,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
      35,   201,  -519,  -519,  -519,  -519,   133,  -519,    -6,   931,
     685,   536,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,    78,    78,    78,    34,   133,  -519,   182,   177,
     181,    45,    42,    72,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,   450,    79,   145,   849,  -519,  -519,   966,   273,
     187,   189,  -519,   185,   191,   279,  -519,  1048,   133,  -519,
      35,   201,   202,   119,  -519,  -519,  -519,  -519,  -519,  -519,
     203,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,   585,
    -519,  -519,  -519,   450,   450,   205,  -519,  -519,  -519,   208,
    -519,  -519,  -519,  -519,   193,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,   194,   195,   196,   214,   216,
    -519,  -519,  -519,   288,   133,  -519,  -519,  -519,  -519,  -519,
    -519,    35,    35,    36,    35,   247,  -519,   218,  -519,  -519,
    -519,  -519,  -519,  -519,    27,    27,  -519,   220,  -519,  -519,
    -519,  1083,   767,   221,  -519,   207,  -519,    35,  -519,   215,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,   229,   230,   133,
    -519,  -519,  -519,   234,   290,   725,  -519,   237,   239,   240,
     241,   242,  -519,   133,  -519,  -519,  -519,  -519,  -519,   243,
     244,   316,  -519,  -519,  -519,  -519,  -519,   177,   181,    45,
      42,    42,    72,    72,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,   245,    27,   231,    40,    40,  -519,   133,    27,
      40,  -519,    36,   248,  -519,   249,   250,   251,   252,   253,
    -519,  -519,  -519,   254,   257,   256,   258,  -519,  -519,   255,
    -519,    34,   259,    34,   185,    27,   264,   269,   272,  -519,
     295,    35,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
     359,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,    27,  -519,  -519,  -519,  -519,  -519,  -519,   262,   262,
    -519,  -519,  -519,  -519,   273,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,   133,   725,    27,  -519,   262,
     281,   296,  -519,   333,  -519,  -519,  -519,   282,   279,    35,
    -519,  -519,  -519,   289,  -519,  -519,  -519,   287,   356,   354,
     354,  -519,  -519,   133,    27,   343,   310,   299,  -519,   354,
      47,  -519,  -519,   300,  -519,  -519,  -519,  -519,   344,  -519,
    -519,  -519,  -519,   302,   155,   354,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,   725,  -519,   315,   305,    35,  -519,
     354,   306,   346,    27,  -519,  -519,  -519,  -519,   307,  -519,
    -519,  -519,   324,    34,   325,   327,    43,   482,   104,  -519,
    -519,  -519,   133,  -519,  -519,  -519,  -519,  -519,  -519,   323,
     725,   377,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
      27,   356,    35,    35,   317,  -519,  -519,  -519,  -519,   328,
      27,   334,  -519,  -519,   319,   320,  -519,   155,    27,   322,
    -519,  -519,  -519,  -519,  -519,    35,  -519,  -519,  -519,   326,
     348,  -519,  -519,   329,   335,  -519,  -519,   353,  -519
};

/* TAO_YYPGOTO[NTERM-NUM].  */
static const tao_yytype_int16 tao_yypgoto[] =
{
    -519,  -519,    90,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,   152,  -519,  -519,
      58,  -519,  -519,  -519,   414,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,   418,  -519,   162,  -519,  -519,  -201,  -519,  -519,
      97,  -519,  -519,  -230,  -267,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -256,  -519,  -519,   -18,  -519,  -519,   -13,
    -519,     6,  -519,  -519,  -519,  -519,  -519,  -519,  -303,  -519,
      51,    49,    53,  -204,  -196,  -194,  -105,  -519,  -235,     9,
    -519,  -519,  -519,   -24,   138,  -348,  -519,  -519,  -519,   -55,
    -519,  -519,  -497,  -163,  -519,  -519,   -12,  -519,   -49,  -519,
    -519,   372,   373,   -48,   -46,   -44,  -519,  -519,   -25,  -519,
     -20,  -519,  -519,  -519,  -519,   100,   184,  -519,  -162,  -519,
    -519,  -519,   -16,  -519,    -8,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -184,  -519,  -519,  -519,  -519,  -519,
    -203,  -519,  -519,  -519,  -519,  -519,  -519,  -519,   -32,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -111,  -519,  -519,  -519,
    -519,  -519,  -519,   -58,  -519,  -519,  -519,   -56,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,   -45,  -519,  -519,   180,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,    15,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -518,  -519,  -519,  -519,  -519,
    -519,  -200,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -219,  -519,  -519,  -431,  -519,  -436,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
      17,    18,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,   -14,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,   423,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,  -519,
    -519,  -519,   156,  -519,  -519,  -519,    67,  -519,  -519,   -54
};

/* TAO_YYTABLE[TAO_YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what TAO_YYDEFACT says.
   If TAO_YYTABLE_NINF, syntax error.  */
#define TAO_YYTABLE_NINF -493
static const tao_yytype_int16 tao_yytable[] =
{
      80,    81,   142,    95,    97,    91,    92,   302,    38,   138,
     123,    39,   384,   176,   139,   177,   122,    48,   140,    49,
      50,   464,   170,   173,   534,   174,   141,   175,   570,   272,
      79,   328,   329,   378,   196,   195,     7,    79,    79,    79,
     196,   585,   142,    79,   627,   338,   197,     7,   627,   138,
     181,   196,     3,   168,   139,   497,   122,   339,   140,   347,
     179,   594,   182,   183,   184,   185,   141,  -271,   426,   178,
     187,   628,   629,   213,   190,   628,   629,   191,   152,    17,
     193,    79,    79,    83,   353,   101,   214,   403,   107,   108,
     349,   100,   111,   112,   113,   114,   167,   354,  -111,    17,
    -392,   100,    11,    87,   209,   210,   431,   464,   303,   304,
     305,   306,   307,   308,   309,    94,    94,    94,   310,   311,
     161,   196,   200,  -110,   161,   249,   100,   388,   389,   312,
     313,  -253,   628,   629,   314,   315,   250,   390,   391,   251,
     186,  -492,   188,   252,   207,   590,  -112,   208,   -72,   100,
     450,   100,   303,   304,   305,   306,   307,   308,   309,    94,
      94,  -461,   310,   311,   617,   378,    82,    84,   253,   392,
     393,   394,   254,   626,   198,   356,   357,    86,    89,   315,
     199,   246,   204,   692,   480,   481,   255,   256,   202,   651,
     645,   646,   647,   701,   482,   483,   411,   412,   484,   485,
     486,   707,   205,   659,   663,   413,   414,   381,   382,   383,
     415,   472,   473,   206,   196,   215,     4,   276,   566,     5,
       6,     7,     8,   217,   279,   216,   281,   218,   464,   219,
     296,   225,   221,   222,   294,     9,    10,   223,    11,   686,
     224,   261,    12,   226,   229,   296,   227,   233,   263,   235,
     234,  -462,   262,   242,   269,    13,    14,    15,    16,   243,
     273,   274,   280,   287,    17,    18,   288,   292,    19,   297,
     386,    20,   348,  -300,   385,   387,   434,   437,    21,    22,
     438,   443,   444,   122,   405,   406,   464,   407,   568,   408,
     440,   456,   451,   474,   475,   476,   207,   487,   488,   489,
     142,   410,   490,   -34,  -228,   501,   504,   138,   512,   517,
     518,   465,   139,   466,   122,   404,   140,  -323,   529,   543,
     532,   533,   464,   535,   141,   536,   537,   538,   539,   541,
     542,   544,   449,   546,   552,   553,   554,   555,   556,   557,
     560,   142,   142,   561,   562,   564,   563,   567,   138,   138,
     673,   463,   571,   139,   139,   122,   122,   140,   140,   572,
     573,   574,   578,   583,    79,   141,   141,   595,  -341,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,     9,    10,   596,    11,   118,   119,   120,   601,   611,
     607,   612,   615,   495,   496,   498,   500,   465,   620,   466,
     622,  -350,   635,   638,   644,   660,   661,   664,   665,   668,
     672,   510,   511,   685,   674,   675,   687,   700,   696,   276,
     702,   703,   704,   708,   712,   716,   694,   695,   711,   718,
      88,   715,   439,    85,   432,   478,   477,   463,   545,   380,
     479,   619,    94,   171,   172,    38,   447,   -73,    39,   709,
     100,   400,   331,    79,    48,   679,    49,    50,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
       9,    10,   676,    11,   118,   119,   120,   602,   706,   569,
     281,   693,   547,   548,   498,    79,   549,   550,   551,   163,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,     9,    10,   520,    11,   118,   119,   120,   452,
     586,     0,   281,   575,   416,     0,     0,     0,   465,     0,
     466,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,   582,    79,
       0,     0,     0,     0,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,     0,     0,   463,     0,
     118,   119,   120,    94,    80,   592,     0,     0,     0,     0,
       0,   603,     0,     0,     0,     0,   465,     0,   466,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    79,     0,
       0,    80,   592,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,     0,     0,     0,     0,     0,
     119,   120,   465,     0,   466,     0,   463,    94,     0,   462,
     662,     0,     0,     0,     0,   142,     0,     0,     0,     0,
      80,   667,   138,   677,     0,     0,     0,   139,     0,   122,
       0,   140,     0,     0,     0,     0,     0,     0,    79,   141,
       0,     0,   463,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,     0,   296,   296,    94,   281,     0,     0,
     119,   120,     0,     0,     0,     0,     0,   281,     0,     0,
       0,     0,     0,     0,     0,   281,   355,   296,  -332,     5,
       0,     0,     8,  -332,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,  -332,  -332,  -332,  -332,     9,    10,     0,    11,     0,
    -332,  -332,    12,     0,     0,   356,   357,   358,   359,  -332,
       0,     0,     0,     0,     0,    13,    94,     0,    79,   372,
     373,   374,     0,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,     0,     0,     0,    21,    22,
     119,   120,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -332,     0,   355,     0,
    -332,     5,     0,   -58,     8,  -332,  -332,  -332,  -332,  -332,
    -332,  -332,  -332,  -332,  -332,  -332,  -332,     9,    10,     0,
      11,     0,  -332,  -332,    12,     0,     0,   356,   357,   358,
     359,  -332,     0,     0,     0,     0,    94,    13,     0,     0,
       0,   372,   373,   374,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,    22,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -332,     0,
     355,     0,  -332,     5,     0,  -474,     8,  -332,  -332,  -332,
    -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,     9,
      10,     0,    11,     0,  -332,  -332,    12,     0,     0,   356,
     357,   358,   359,  -332,     0,     0,     0,     0,     0,    13,
       0,     0,     0,   424,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   425,     0,     0,     0,     0,     0,     0,
       0,     0,    21,    22,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -332,     0,   355,     0,  -332,     5,     0,  -441,     8,  -332,
    -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,     9,    10,     0,    11,     0,  -332,  -332,    12,     0,
       0,   356,   357,   358,   359,  -332,     0,   355,     0,  -332,
       5,    13,     0,     8,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,  -332,  -332,  -332,  -332,  -332,     9,    10,     0,    11,
       0,  -332,  -332,    12,    21,    22,   356,   357,   358,   359,
    -332,     0,     0,     0,     0,     0,    13,     0,     0,     0,
       0,     0,  -332,     0,     0,     0,     0,     0,     0,   -40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -332,     0,   355,
       0,  -332,     5,     0,  -488,     8,  -332,  -332,  -332,  -332,
    -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,     9,    10,
       0,    11,     0,  -332,  -332,    12,     0,     0,   356,   357,
     358,   359,  -332,     0,   355,     0,  -332,     5,    13,     0,
       8,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,  -332,  -332,     9,    10,     0,    11,     0,  -332,  -332,
      12,    21,    22,   356,   357,   358,   359,  -332,     0,     0,
       0,     0,     0,    13,     0,     0,     0,     0,     0,  -332,
       0,     0,     0,    -2,     4,     0,   -62,     5,     6,     7,
       8,     0,     0,     0,     0,     0,    21,    22,     0,     0,
       0,     0,     0,     9,    10,     0,    11,     0,     0,     0,
      12,     0,     0,     0,  -332,     0,     0,     0,     0,     0,
       0,  -465,     0,    13,    14,    15,    16,     0,     0,     0,
       0,     0,    17,    18,     0,     0,    19,     0,     0,    20,
       0,     0,     0,     0,     0,     0,    21,    22
};

static const tao_yytype_int16 tao_yycheck[] =
{
      13,    13,    34,    21,    22,    18,    19,   263,     2,    34,
      34,     2,   315,    71,    34,    71,    34,     2,    34,     2,
       2,   369,    71,    71,   455,    71,    34,    71,   525,   230,
       3,   266,   267,   300,    81,    76,     6,     3,     3,     3,
      81,   559,    74,     3,     1,   275,    76,     6,     1,    74,
      74,    81,     0,    71,    74,    19,    74,   104,    74,   289,
      73,   579,    75,    76,    77,    78,    74,    86,   335,     3,
      83,    28,    29,    90,    87,    28,    29,    90,    22,    49,
      93,     3,     3,    53,    90,    27,   103,     8,     9,    10,
     291,    89,    13,    14,    15,    16,    86,   103,    86,    49,
      86,    89,    23,    53,     8,     9,   336,   455,    74,    75,
      76,    77,    78,    79,    80,    81,    81,    81,    84,    85,
      62,    81,    48,    86,    66,     6,    89,    82,    83,    95,
      96,    88,    28,    29,   100,   101,    17,    95,    96,    20,
      82,   104,    84,    24,     8,   576,    86,    11,    86,    89,
     351,    89,    74,    75,    76,    77,    78,    79,    80,    81,
      81,    86,    84,    85,   600,   432,    14,    15,    49,    97,
      98,    99,    53,   609,    86,    30,    31,    15,    16,   101,
      86,   194,    87,   680,   388,   389,    67,    68,   104,   625,
      35,    36,    37,   690,   390,   391,    51,    52,   392,   393,
     394,   698,    86,   634,   640,    60,    61,   312,   313,   314,
      65,   373,   374,    87,    81,   104,     1,   235,   521,     4,
       5,     6,     7,    86,   237,   104,   239,    86,   576,    87,
     248,    87,    86,    86,   247,    20,    21,    86,    23,   670,
      86,     8,    27,    86,    86,   263,    87,    86,    47,    89,
      87,    86,   104,    87,   101,    40,    41,    42,    43,    89,
      87,    87,    87,    87,    49,    50,    87,    89,    53,     3,
      93,    56,   290,   105,    92,    94,     3,    90,    63,    64,
      91,    90,     3,   301,   333,   333,   634,   333,   523,   333,
     105,    88,    90,    88,    86,   102,     8,   103,   103,   103,
     332,   333,    88,    88,    88,    58,    88,   332,    88,    88,
     103,   369,   332,   369,   332,   333,   332,    88,    88,     3,
      86,    31,   670,    86,   332,    86,    86,    86,    86,    86,
      86,    86,   350,   102,    86,    86,    86,    86,    86,    86,
      86,   373,   374,    86,    88,    90,    88,    88,   373,   374,
     653,   369,    88,   373,   374,   373,   374,   373,   374,    90,
      88,    66,     3,   101,     3,   373,   374,    86,    35,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    87,    23,    24,    25,    26,   106,   102,
     101,    35,    38,   411,   412,   413,   414,   455,    55,   455,
      90,   102,   102,    59,   102,    90,   101,   101,    62,   102,
      86,   424,   425,    90,    89,    88,    39,    89,   101,   437,
      86,   102,   102,   101,    76,    90,   682,   683,   102,    76,
      16,   102,   342,    15,   337,   386,   385,   455,   493,   301,
     387,   604,    81,    71,    71,   439,   346,    86,   439,   705,
      89,     1,   268,     3,   439,   658,   439,   439,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   656,    23,    24,    25,    26,   588,   697,   524,
     493,   681,   495,   496,   502,     3,   499,   500,   502,    66,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,   437,    23,    24,    25,    26,   353,
     564,    -1,   525,   531,   334,    -1,    -1,    -1,   576,    -1,
     576,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,   551,     3,
      -1,    -1,    -1,    -1,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,   576,    -1,
      24,    25,    26,    81,   577,   577,    -1,    -1,    -1,    -1,
      -1,   589,    -1,    -1,    -1,    -1,   634,    -1,   634,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      -1,   604,   604,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,   670,    -1,   670,    -1,   634,    81,    -1,    34,
     638,    -1,    -1,    -1,    -1,   657,    -1,    -1,    -1,    -1,
     643,   643,   657,   657,    -1,    -1,    -1,   657,    -1,   657,
      -1,   657,    -1,    -1,    -1,    -1,    -1,    -1,     3,   657,
      -1,    -1,   670,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    -1,   682,   683,    81,   680,    -1,    -1,
      25,    26,    -1,    -1,    -1,    -1,    -1,   690,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   698,     1,   705,     3,     4,
      -1,    -1,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    23,    -1,
      25,    26,    27,    -1,    -1,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    81,    -1,     3,    44,
      45,    46,    -1,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    63,    64,
      25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,     1,    -1,
       3,     4,    -1,    88,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      23,    -1,    25,    26,    27,    -1,    -1,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    -1,    81,    40,    -1,    -1,
      -1,    44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,
       1,    -1,     3,     4,    -1,    88,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    -1,    25,    26,    27,    -1,    -1,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,     1,    -1,     3,     4,    -1,    88,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    -1,    25,    26,    27,    -1,
      -1,    30,    31,    32,    33,    34,    -1,     1,    -1,     3,
       4,    40,    -1,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      -1,    25,    26,    27,    63,    64,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,     1,
      -1,     3,     4,    -1,    88,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    23,    -1,    25,    26,    27,    -1,    -1,    30,    31,
      32,    33,    34,    -1,     1,    -1,     3,     4,    40,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    23,    -1,    25,    26,
      27,    63,    64,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    81,
      -1,    -1,    -1,     0,     1,    -1,    88,     4,     5,     6,
       7,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    20,    21,    -1,    23,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    40,    41,    42,    43,    -1,    -1,    -1,
      -1,    -1,    49,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64
};

/* TAO_YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const tao_yytype_uint16 tao_yystos[] =
{
       0,   108,   109,     0,     1,     4,     5,     6,     7,    20,
      21,    23,    27,    40,    41,    42,    43,    49,    50,    53,
      56,    63,    64,   110,   124,   129,   130,   134,   136,   139,
     140,   141,   145,   149,   151,   155,   156,   177,   178,   196,
     205,   225,   227,   239,   241,   263,   264,   265,   306,   357,
     358,   359,   360,   361,   365,   385,   387,   410,   411,   412,
     413,   414,   418,   419,   420,   423,   427,   429,   433,   435,
     123,   179,   125,   135,   197,   226,   240,   266,   307,     3,
     176,   213,   134,    53,   134,   149,   151,    53,   141,   151,
     152,   176,   176,   388,    81,   173,   176,   173,   118,   116,
      89,   137,   436,   131,   119,   142,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    24,    25,
      26,   137,   173,   200,   201,   202,   203,   204,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   227,
     239,   241,   265,   274,   277,   280,   283,   284,   287,   114,
     111,   228,    22,   115,   112,   113,   120,   362,   121,   386,
     122,   137,   421,   421,   117,   430,   434,    86,   173,   183,
     215,   218,   219,   220,   221,   222,   280,   284,     3,   176,
     198,   200,   176,   176,   176,   176,   137,   176,   137,   146,
     176,   176,   366,   176,   174,    76,    81,    76,    86,    86,
      48,   153,   104,   437,    87,    86,    87,     8,    11,     8,
       9,   278,   150,    90,   103,   104,   104,    86,    86,    87,
     242,    86,    86,    86,    86,    87,    86,    87,   397,    86,
     422,   415,   424,    86,    87,    89,   440,   180,   126,   199,
     267,   308,    87,    89,   368,   389,   176,   175,   138,     6,
      17,    20,    24,    49,    53,    67,    68,   428,   439,   132,
     143,     8,   104,    47,   154,   275,   281,   285,   229,   101,
     363,   398,   154,    87,    87,   431,   173,   441,   443,   176,
      87,   176,   206,   209,   213,   214,   288,    87,    87,   147,
     369,   367,    89,   394,   176,   170,   173,     3,   438,   160,
     157,   279,   170,    74,    75,    76,    77,    78,    79,    80,
      84,    85,    95,    96,   100,   101,   173,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   195,   195,
     231,   233,   234,   243,   370,   400,   416,   425,   160,   104,
     442,   181,   127,   289,   207,   268,   309,   160,   173,   154,
     395,   390,   171,    90,   103,     1,    30,    31,    32,    33,
     133,   161,   178,   196,   295,   296,   301,   306,   311,   316,
     357,   358,    44,    45,    46,   144,   158,   159,   161,   318,
     201,   193,   193,   193,   185,    92,    93,    94,    82,    83,
      95,    96,    97,    98,    99,   276,   282,   286,   230,   232,
       1,   200,   235,     8,   173,   215,   220,   221,   222,   248,
     265,    51,    52,    60,    61,    65,   295,   364,   371,   378,
     380,   382,   383,   384,    44,    54,   161,   399,   401,   404,
     407,   160,   157,   432,     3,   444,   446,    90,    91,   109,
     105,   290,   292,    90,     3,   270,   273,   232,   148,   173,
     154,    90,   439,   169,   297,   302,    88,   165,   162,   167,
     166,   168,    34,   173,   202,   280,   284,   317,   340,   163,
     164,   319,   235,   235,    88,    86,   102,   187,   188,   189,
     190,   190,   191,   191,   192,   192,   192,   103,   103,   103,
      88,   233,   238,   236,   244,   173,   173,    19,   173,   379,
     173,    58,   381,   377,    88,   372,   373,   374,   375,   376,
     176,   176,    88,   402,   403,   417,   426,    88,   103,   445,
     443,   182,   128,   293,   291,   208,   269,   271,   310,    88,
     391,   172,    86,    31,   340,    86,    86,    86,    86,    86,
     312,    86,    86,     3,    86,   206,   102,   176,   176,   176,
     176,   379,    86,    86,    86,    86,    86,    86,   405,   408,
      86,    86,    88,    88,    90,   184,   185,    88,   195,   292,
     209,    88,    90,    88,    66,   173,   298,   303,     3,   320,
     237,   245,   176,   101,   322,   322,   446,   294,   272,   392,
     340,   210,   213,   313,   322,    86,    87,   323,   324,   406,
     409,   106,   273,   173,   299,   304,   211,   101,   331,   321,
     246,   102,    35,   325,   328,    38,   342,   342,   393,   210,
      55,   345,    90,   332,   333,   314,   342,     1,    28,    29,
     249,   251,   255,   257,   329,   102,   326,   343,    59,   396,
     300,   346,   305,   212,   102,    35,    36,    37,   334,   337,
     341,   342,   254,   259,   258,   247,   250,   252,   256,   340,
      90,   101,   173,   342,   101,    62,   348,   213,   102,   335,
     338,   315,    86,   185,    89,    88,   251,   200,   261,   257,
     330,   327,   344,   347,   349,    90,   340,    39,   351,   260,
     262,   253,   209,   328,   170,   170,   101,   336,   339,   352,
      89,   209,    86,   102,   102,   350,   337,   209,   101,   170,
     353,   102,    76,   354,   355,   102,    90,   356,    76
};

#define tao_yyerrok                (tao_yyerrstatus = 0)
#define tao_yyclearin        (tao_yychar = TAO_YYEMPTY)
#define TAO_YYEMPTY                (-2)
#define TAO_YYEOF                0

#define TAO_YYACCEPT        goto tao_yyacceptlab
#define TAO_YYABORT                goto tao_yyabortlab
#define TAO_YYERROR                goto tao_yyerrorlab


/* Like TAO_YYERROR except do call tao_yyerror.  This remains here temporarily
   to ease the transition to the new meaning of TAO_YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define TAO_YYFAIL                goto tao_yyerrlab

#define TAO_YYRECOVERING()  (!!tao_yyerrstatus)

#define TAO_YYBACKUP(Token, Value)                                        \
do                                                                \
  if (tao_yychar == TAO_YYEMPTY && tao_yylen == 1)                                \
    {                                                                \
      tao_yychar = (Token);                                                \
      tao_yylval = (Value);                                                \
      tao_yytoken = TAO_YYTRANSLATE (tao_yychar);                                \
      TAO_YYPOPSTACK (1);                                                \
      goto tao_yybackup;                                                \
    }                                                                \
  else                                                                \
    {                                                                \
      tao_yyerror (TAO_YY_("syntax error: cannot back up")); \
      TAO_YYERROR;                                                        \
    }                                                                \
while (TAO_YYID (0))


#define TAO_YYTERROR        1
#define TAO_YYERRCODE        256


/* TAO_YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define TAO_YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef TAO_YYLLOC_DEFAULT
# define TAO_YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                        \
      if (TAO_YYID (N))                                                    \
        {                                                                \
          (Current).first_line   = TAO_YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = TAO_YYRHSLOC (Rhs, 1).first_column;        \
          (Current).last_line    = TAO_YYRHSLOC (Rhs, N).last_line;                \
          (Current).last_column  = TAO_YYRHSLOC (Rhs, N).last_column;        \
        }                                                                \
      else                                                                \
        {                                                                \
          (Current).first_line   = (Current).last_line   =                \
            TAO_YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =                \
            TAO_YYRHSLOC (Rhs, 0).last_column;                                \
        }                                                                \
    while (TAO_YYID (0))
#endif


/* TAO_YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef TAO_YY_LOCATION_PRINT
# if TAO_YYLTYPE_IS_TRIVIAL
#  define TAO_YY_LOCATION_PRINT(File, Loc)                        \
     fprintf (File, "%d.%d-%d.%d",                        \
              (Loc).first_line, (Loc).first_column,        \
              (Loc).last_line,  (Loc).last_column)
# else
#  define TAO_YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* TAO_YYLEX -- calling `tao_yylex' with the right arguments.  */

#ifdef TAO_YYLEX_PARAM
# define TAO_YYLEX tao_yylex (TAO_YYLEX_PARAM)
#else
# define TAO_YYLEX tao_yylex ()
#endif

/* Enable debugging if requested.  */
#if TAO_YYDEBUG

# ifndef TAO_YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define TAO_YYFPRINTF fprintf
# endif

# define TAO_YYDPRINTF(Args)                        \
do {                                                \
  if (tao_yydebug)                                        \
    TAO_YYFPRINTF Args;                                \
} while (TAO_YYID (0))

# define TAO_YY_SYMBOL_PRINT(Title, Type, Value, Location)                          \
do {                                                                          \
  if (tao_yydebug)                                                                  \
    {                                                                          \
      TAO_YYFPRINTF (stderr, "%s ", Title);                                          \
      tao_yy_symbol_print (stderr,                                                  \
                  Type, Value); \
      TAO_YYFPRINTF (stderr, "\n");                                                  \
    }                                                                          \
} while (TAO_YYID (0))


/*--------------------------------.
| Print this symbol on TAO_YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
tao_yy_symbol_value_print (FILE *tao_yyoutput, int tao_yytype, TAO_YYSTYPE const * const tao_yyvaluep)
#else
static void
tao_yy_symbol_value_print (tao_yyoutput, tao_yytype, tao_yyvaluep)
    FILE *tao_yyoutput;
    int tao_yytype;
    TAO_YYSTYPE const * const tao_yyvaluep;
#endif
{
  if (!tao_yyvaluep)
    return;
# ifdef TAO_YYPRINT
  if (tao_yytype < TAO_YYNTOKENS)
    TAO_YYPRINT (tao_yyoutput, tao_yytoknum[tao_yytype], *tao_yyvaluep);
# else
  TAO_YYUSE (tao_yytype);
  TAO_YYUSE (tao_yyoutput);
# endif
}


/*--------------------------------.
| Print this symbol on TAO_YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
tao_yy_symbol_print (FILE *tao_yyoutput, int tao_yytype, TAO_YYSTYPE const * const tao_yyvaluep)
#else
static void
tao_yy_symbol_print (tao_yyoutput, tao_yytype, tao_yyvaluep)
    FILE *tao_yyoutput;
    int tao_yytype;
    TAO_YYSTYPE const * const tao_yyvaluep;
#endif
{
  if (tao_yytype < TAO_YYNTOKENS)
    TAO_YYFPRINTF (tao_yyoutput, "token %s (", tao_yytname[tao_yytype]);
  else
    TAO_YYFPRINTF (tao_yyoutput, "nterm %s (", tao_yytname[tao_yytype]);

  tao_yy_symbol_value_print (tao_yyoutput, tao_yytype, tao_yyvaluep);
  TAO_YYFPRINTF (tao_yyoutput, ")");
}

/*------------------------------------------------------------------.
| tao_yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
tao_yy_stack_print (tao_yytype_int16 *bottom, tao_yytype_int16 *top)
#else
static void
tao_yy_stack_print (bottom, top)
    tao_yytype_int16 *bottom;
    tao_yytype_int16 *top;
#endif
{
  TAO_YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    TAO_YYFPRINTF (stderr, " %d", *bottom);
  TAO_YYFPRINTF (stderr, "\n");
}

# define TAO_YY_STACK_PRINT(Bottom, Top)                                \
do {                                                                \
  if (tao_yydebug)                                                        \
    tao_yy_stack_print ((Bottom), (Top));                                \
} while (TAO_YYID (0))


/*------------------------------------------------.
| Report that the TAO_YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
tao_yy_reduce_print (TAO_YYSTYPE *tao_yyvsp, int tao_yyrule)
#else
static void
tao_yy_reduce_print (tao_yyvsp, tao_yyrule)
    TAO_YYSTYPE *tao_yyvsp;
    int tao_yyrule;
#endif
{
  int tao_yynrhs = tao_yyr2[tao_yyrule];
  int tao_yyi;
  unsigned long int tao_yylno = tao_yyrline[tao_yyrule];
  TAO_YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             tao_yyrule - 1, tao_yylno);
  /* The symbols being reduced.  */
  for (tao_yyi = 0; tao_yyi < tao_yynrhs; tao_yyi++)
    {
      fprintf (stderr, "   $%d = ", tao_yyi + 1);
      tao_yy_symbol_print (stderr, tao_yyrhs[tao_yyprhs[tao_yyrule] + tao_yyi],
                       &(tao_yyvsp[(tao_yyi + 1) - (tao_yynrhs)])
                                              );
      fprintf (stderr, "\n");
    }
}

# define TAO_YY_REDUCE_PRINT(Rule)                \
do {                                        \
  if (tao_yydebug)                                \
    tao_yy_reduce_print (tao_yyvsp, Rule); \
} while (TAO_YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int tao_yydebug;
#else /* !TAO_YYDEBUG */
# define TAO_YYDPRINTF(Args)
# define TAO_YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define TAO_YY_STACK_PRINT(Bottom, Top)
# define TAO_YY_REDUCE_PRINT(Rule)
#endif /* !TAO_YYDEBUG */


/* TAO_YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef        TAO_YYINITDEPTH
# define TAO_YYINITDEPTH 200
#endif

/* TAO_YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   TAO_YYSTACK_ALLOC_MAXIMUM < TAO_YYSTACK_BYTES (TAO_YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef TAO_YYMAXDEPTH
# define TAO_YYMAXDEPTH 10000
#endif



#if TAO_YYERROR_VERBOSE

# ifndef tao_yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define tao_yystrlen strlen
#  else
/* Return the length of TAO_YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static TAO_YYSIZE_T
tao_yystrlen (const char *tao_yystr)
#else
static TAO_YYSIZE_T
tao_yystrlen (tao_yystr)
    const char *tao_yystr;
#endif
{
  TAO_YYSIZE_T tao_yylen;
  for (tao_yylen = 0; tao_yystr[tao_yylen]; tao_yylen++)
    continue;
  return tao_yylen;
}
#  endif
# endif

# ifndef tao_yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define tao_yystpcpy stpcpy
#  else
/* Copy TAO_YYSRC to TAO_YYDEST, returning the address of the terminating '\0' in
   TAO_YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
tao_yystpcpy (char *tao_yydest, const char *tao_yysrc)
#else
static char *
tao_yystpcpy (tao_yydest, tao_yysrc)
    char *tao_yydest;
    const char *tao_yysrc;
#endif
{
  char *tao_yyd = tao_yydest;
  const char *tao_yys = tao_yysrc;

  while ((*tao_yyd++ = *tao_yys++) != '\0')
    continue;

  return tao_yyd - 1;
}
#  endif
# endif

# ifndef tao_yytnamerr
/* Copy to TAO_YYRES the contents of TAO_YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for tao_yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  TAO_YYSTR is taken from tao_yytname.  If TAO_YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static TAO_YYSIZE_T
tao_yytnamerr (char *tao_yyres, const char *tao_yystr)
{
  if (*tao_yystr == '"')
    {
      TAO_YYSIZE_T tao_yyn = 0;
      char const *tao_yyp = tao_yystr;

      for (;;)
        switch (*++tao_yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++tao_yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (tao_yyres)
              tao_yyres[tao_yyn] = *tao_yyp;
            tao_yyn++;
            break;

          case '"':
            if (tao_yyres)
              tao_yyres[tao_yyn] = '\0';
            return tao_yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! tao_yyres)
    return tao_yystrlen (tao_yystr);

  return tao_yystpcpy (tao_yyres, tao_yystr) - tao_yyres;
}
# endif

/* Copy into TAO_YYRESULT an error message about the unexpected token
   TAO_YYCHAR while in state TAO_YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If TAO_YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return TAO_YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static TAO_YYSIZE_T
tao_yysyntax_error (char *tao_yyresult, int tao_yystate, int tao_yychar)
{
  int tao_yyn = tao_yypact[tao_yystate];

  if (! (TAO_YYPACT_NINF < tao_yyn && tao_yyn <= TAO_YYLAST))
    return 0;
  else
    {
      int tao_yytype = TAO_YYTRANSLATE (tao_yychar);
      TAO_YYSIZE_T tao_yysize0 = tao_yytnamerr (0, tao_yytname[tao_yytype]);
      TAO_YYSIZE_T tao_yysize = tao_yysize0;
      TAO_YYSIZE_T tao_yysize1;
      int tao_yysize_overflow = 0;
      enum { TAO_YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *tao_yyarg[TAO_YYERROR_VERBOSE_ARGS_MAXIMUM];
      int tao_yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
         constructed on the fly.  */
      TAO_YY_("syntax error, unexpected %s");
      TAO_YY_("syntax error, unexpected %s, expecting %s");
      TAO_YY_("syntax error, unexpected %s, expecting %s or %s");
      TAO_YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      TAO_YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *tao_yyfmt;
      char const *tao_yyf;
      static char const tao_yyunexpected[] = "syntax error, unexpected %s";
      static char const tao_yyexpecting[] = ", expecting %s";
      static char const tao_yyor[] = " or %s";
      char tao_yyformat[sizeof tao_yyunexpected
                    + sizeof tao_yyexpecting - 1
                    + ((TAO_YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
                       * (sizeof tao_yyor - 1))];
      char const *tao_yyprefix = tao_yyexpecting;

      /* Start TAO_YYX at -TAO_YYN if negative to avoid negative indexes in
         TAO_YYCHECK.  */
      int tao_yyxbegin = tao_yyn < 0 ? -tao_yyn : 0;

      /* Stay within bounds of both tao_yycheck and tao_yytname.  */
      int tao_yychecklim = TAO_YYLAST - tao_yyn + 1;
      int tao_yyxend = tao_yychecklim < TAO_YYNTOKENS ? tao_yychecklim : TAO_YYNTOKENS;
      int tao_yycount = 1;

      tao_yyarg[0] = tao_yytname[tao_yytype];
      tao_yyfmt = tao_yystpcpy (tao_yyformat, tao_yyunexpected);

      for (tao_yyx = tao_yyxbegin; tao_yyx < tao_yyxend; ++tao_yyx)
        if (tao_yycheck[tao_yyx + tao_yyn] == tao_yyx && tao_yyx != TAO_YYTERROR)
          {
            if (tao_yycount == TAO_YYERROR_VERBOSE_ARGS_MAXIMUM)
              {
                tao_yycount = 1;
                tao_yysize = tao_yysize0;
                tao_yyformat[sizeof tao_yyunexpected - 1] = '\0';
                break;
              }
            tao_yyarg[tao_yycount++] = tao_yytname[tao_yyx];
            tao_yysize1 = tao_yysize + tao_yytnamerr (0, tao_yytname[tao_yyx]);
            tao_yysize_overflow |= (tao_yysize1 < tao_yysize);
            tao_yysize = tao_yysize1;
            tao_yyfmt = tao_yystpcpy (tao_yyfmt, tao_yyprefix);
            tao_yyprefix = tao_yyor;
          }

      tao_yyf = TAO_YY_(tao_yyformat);
      tao_yysize1 = tao_yysize + tao_yystrlen (tao_yyf);
      tao_yysize_overflow |= (tao_yysize1 < tao_yysize);
      tao_yysize = tao_yysize1;

      if (tao_yysize_overflow)
        return TAO_YYSIZE_MAXIMUM;

      if (tao_yyresult)
        {
          /* Avoid sprintf, as that infringes on the user's name space.
             Don't have undefined behavior even if the translation
             produced a string with the wrong number of "%s"s.  */
          char *tao_yyp = tao_yyresult;
          int tao_yyi = 0;
          while ((*tao_yyp = *tao_yyf) != '\0')
            {
              if (*tao_yyp == '%' && tao_yyf[1] == 's' && tao_yyi < tao_yycount)
                {
                  tao_yyp += tao_yytnamerr (tao_yyp, tao_yyarg[tao_yyi++]);
                  tao_yyf += 2;
                }
              else
                {
                  tao_yyp++;
                  tao_yyf++;
                }
            }
        }
      return tao_yysize;
    }
}
#endif /* TAO_YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
tao_yydestruct (const char *tao_yymsg, int tao_yytype, TAO_YYSTYPE *tao_yyvaluep)
#else
static void
tao_yydestruct (tao_yymsg, tao_yytype, tao_yyvaluep)
    const char *tao_yymsg;
    int tao_yytype;
    TAO_YYSTYPE *tao_yyvaluep;
#endif
{
  TAO_YYUSE (tao_yyvaluep);

  if (!tao_yymsg)
    tao_yymsg = "Deleting";
  TAO_YY_SYMBOL_PRINT (tao_yymsg, tao_yytype, tao_yyvaluep, tao_yylocationp);
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef TAO_YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int tao_yyparse (void *TAO_YYPARSE_PARAM);
#else
int tao_yyparse ();
#endif
#else /* ! TAO_YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int tao_yyparse (void);
#else
int tao_yyparse ();
#endif
#endif /* ! TAO_YYPARSE_PARAM */



/* The look-ahead symbol.  */
int tao_yychar;

/* The semantic value of the look-ahead symbol.  */
TAO_YYSTYPE tao_yylval;

/* Number of syntax errors so far.  */
int tao_yynerrs;



/*----------.
| tao_yyparse.  |
`----------*/

#ifdef TAO_YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
tao_yyparse (void *TAO_YYPARSE_PARAM)
#else
int
tao_yyparse (TAO_YYPARSE_PARAM)
    void *TAO_YYPARSE_PARAM;
#endif
#else /* ! TAO_YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
tao_yyparse (void)
#else
int
tao_yyparse ()

#endif
#endif
{

  int tao_yystate;
  int tao_yyn;
  int tao_yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int tao_yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int tao_yytoken = 0;
#if TAO_YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char tao_yymsgbuf[128];
  char *tao_yymsg = tao_yymsgbuf;
  TAO_YYSIZE_T tao_yymsg_alloc = sizeof tao_yymsgbuf;
#endif

  /* Three stacks and their tools:
     `tao_yyss': related to states,
     `tao_yyvs': related to semantic values,
     `tao_yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow tao_yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  tao_yytype_int16 tao_yyssa[TAO_YYINITDEPTH];
  tao_yytype_int16 *tao_yyss = tao_yyssa;
  tao_yytype_int16 *tao_yyssp;

  /* The semantic value stack.  */
  TAO_YYSTYPE tao_yyvsa[TAO_YYINITDEPTH];
  TAO_YYSTYPE *tao_yyvs = tao_yyvsa;
  TAO_YYSTYPE *tao_yyvsp;



#define TAO_YYPOPSTACK(N)   (tao_yyvsp -= (N), tao_yyssp -= (N))

  TAO_YYSIZE_T tao_yystacksize = TAO_YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  TAO_YYSTYPE tao_yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int tao_yylen = 0;

  TAO_YYDPRINTF ((stderr, "Starting parse\n"));

  tao_yystate = 0;
  tao_yyerrstatus = 0;
  tao_yynerrs = 0;
  tao_yychar = TAO_YYEMPTY;                /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  tao_yyssp = tao_yyss;
  tao_yyvsp = tao_yyvs;

  goto tao_yysetstate;

/*------------------------------------------------------------.
| tao_yynewstate -- Push a new state, which is found in tao_yystate.  |
`------------------------------------------------------------*/
 tao_yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  tao_yyssp++;

 tao_yysetstate:
  *tao_yyssp = tao_yystate;

  if (tao_yyss + tao_yystacksize - 1 <= tao_yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      TAO_YYSIZE_T tao_yysize = tao_yyssp - tao_yyss + 1;

#ifdef tao_yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        TAO_YYSTYPE *tao_yyvs1 = tao_yyvs;
        tao_yytype_int16 *tao_yyss1 = tao_yyss;


        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if tao_yyoverflow is a macro.  */
        tao_yyoverflow (TAO_YY_("memory exhausted"),
                    &tao_yyss1, tao_yysize * sizeof (*tao_yyssp),
                    &tao_yyvs1, tao_yysize * sizeof (*tao_yyvsp),

                    &tao_yystacksize);

        tao_yyss = tao_yyss1;
        tao_yyvs = tao_yyvs1;
      }
#else /* no tao_yyoverflow */
# ifndef TAO_YYSTACK_RELOCATE
      goto tao_yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (TAO_YYMAXDEPTH <= tao_yystacksize)
        goto tao_yyexhaustedlab;
      tao_yystacksize *= 2;
      if (TAO_YYMAXDEPTH < tao_yystacksize)
        tao_yystacksize = TAO_YYMAXDEPTH;

      {
        tao_yytype_int16 *tao_yyss1 = tao_yyss;
        union tao_yyalloc *tao_yyptr =
          (union tao_yyalloc *) TAO_YYSTACK_ALLOC (TAO_YYSTACK_BYTES (tao_yystacksize));
        if (! tao_yyptr)
          goto tao_yyexhaustedlab;
        TAO_YYSTACK_RELOCATE (tao_yyss);
        TAO_YYSTACK_RELOCATE (tao_yyvs);

#  undef TAO_YYSTACK_RELOCATE
        if (tao_yyss1 != tao_yyssa)
          TAO_YYSTACK_FREE (tao_yyss1);
      }
# endif
#endif /* no tao_yyoverflow */

      tao_yyssp = tao_yyss + tao_yysize - 1;
      tao_yyvsp = tao_yyvs + tao_yysize - 1;


      TAO_YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) tao_yystacksize));

      if (tao_yyss + tao_yystacksize - 1 <= tao_yyssp)
        TAO_YYABORT;
    }

  TAO_YYDPRINTF ((stderr, "Entering state %d\n", tao_yystate));

  goto tao_yybackup;

/*-----------.
| tao_yybackup.  |
`-----------*/
tao_yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  tao_yyn = tao_yypact[tao_yystate];
  if (tao_yyn == TAO_YYPACT_NINF)
    goto tao_yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* TAO_YYCHAR is either TAO_YYEMPTY or TAO_YYEOF or a valid look-ahead symbol.  */
  if (tao_yychar == TAO_YYEMPTY)
    {
      TAO_YYDPRINTF ((stderr, "Reading a token: "));
      tao_yychar = TAO_YYLEX;
    }

  if (tao_yychar <= TAO_YYEOF)
    {
      tao_yychar = tao_yytoken = TAO_YYEOF;
      TAO_YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      tao_yytoken = TAO_YYTRANSLATE (tao_yychar);
      TAO_YY_SYMBOL_PRINT ("Next token is", tao_yytoken, &tao_yylval, &tao_yylloc);
    }

  /* If the proper action on seeing token TAO_YYTOKEN is to reduce or to
     detect an error, take that action.  */
  tao_yyn += tao_yytoken;
  if (tao_yyn < 0 || TAO_YYLAST < tao_yyn || tao_yycheck[tao_yyn] != tao_yytoken)
    goto tao_yydefault;
  tao_yyn = tao_yytable[tao_yyn];
  if (tao_yyn <= 0)
    {
      if (tao_yyn == 0 || tao_yyn == TAO_YYTABLE_NINF)
        goto tao_yyerrlab;
      tao_yyn = -tao_yyn;
      goto tao_yyreduce;
    }

  if (tao_yyn == TAO_YYFINAL)
    TAO_YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (tao_yyerrstatus)
    tao_yyerrstatus--;

  /* Shift the look-ahead token.  */
  TAO_YY_SYMBOL_PRINT ("Shifting", tao_yytoken, &tao_yylval, &tao_yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (tao_yychar != TAO_YYEOF)
    tao_yychar = TAO_YYEMPTY;

  tao_yystate = tao_yyn;
  *++tao_yyvsp = tao_yylval;

  goto tao_yynewstate;


/*-----------------------------------------------------------.
| tao_yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
tao_yydefault:
  tao_yyn = tao_yydefact[tao_yystate];
  if (tao_yyn == 0)
    goto tao_yyerrlab;
  goto tao_yyreduce;


/*-----------------------------.
| tao_yyreduce -- Do a reduction.  |
`-----------------------------*/
tao_yyreduce:
  /* tao_yyn is the number of a rule to reduce with.  */
  tao_yylen = tao_yyr2[tao_yyn];

  /* If TAO_YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets TAO_YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to TAO_YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that TAO_YYVAL may be used uninitialized.  */
  tao_yyval = tao_yyvsp[1-tao_yylen];


  TAO_YY_REDUCE_PRINT (tao_yyn);
  switch (tao_yyn)
    {
        case 5:

    {
// definition : type_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
    break;

  case 6:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 7:

    {
//      | typeid_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
    break;

  case 8:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 9:

    {
//      | typeprefix_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
    break;

  case 10:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 11:

    {
//      | const_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
    break;

  case 12:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 13:

    {
//      | exception
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
    break;

  case 14:

    {
//      ';'
        idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 15:

    {
//      | interface_def
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
ACE_DEBUG ((LM_DEBUG, "interface_def\n"));
        }
    break;

  case 16:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 17:

    {
//      | template_interface_def
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
ACE_DEBUG ((LM_DEBUG, "template_interface_def\n"));
        }
    break;

  case 18:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 19:

    {
//      | module
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
        }
    break;

  case 20:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 21:

    {
//      | value_def
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeDeclSeen);
        }
    break;

  case 22:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 23:

    {
//      | component
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentDeclSeen);
        }
    break;

  case 24:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 25:

    {
//      | home_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeDeclSeen);
        }
    break;

  case 26:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 27:

    {
//      | event
          idl_global->set_parse_state (IDL_GlobalData::PS_EventDeclSeen);
        }
    break;

  case 28:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 29:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
    break;

  case 30:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 31:

    {
// module  : IDL_MODULE
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
          }
    break;

  case 32:

    {
//        IDENTIFIER
            Identifier id ((tao_yyvsp[(3) - (3)].strval));
            ACE::strdelete ((tao_yyvsp[(3) - (3)].strval));
            (tao_yyvsp[(3) - (3)].strval) = 0;
            UTL_ScopedName n (&id,
                              0);
            AST_Module *m = 0;
            UTL_Scope *s = idl_global->scopes ().top_non_null ();

            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleIDSeen);
            /*
             * Make a new module and add it to the enclosing scope
             */
            if (s != 0)
              {
                m = idl_global->gen ()->create_module (s,
                                                       &n);
                (void) s->fe_add_module (m);
              }
            /*
             * Push it on the stack
             */
            idl_global->scopes ().push (m);
          }
    break;

  case 33:

    {
//        '{'
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
          }
    break;

  case 34:

    {
//        definitions
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
          }
    break;

  case 35:

    {
//        '}'
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
            /*
             * Finished with this module - pop it from the scope stack.
             */
            idl_global->scopes ().pop ();
          }
    break;

  case 38:

    {
// interface : interface_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Interface *i = 0;

          /*
           * Make a new interface node and add it to its enclosing scope.
           */
          if (s != 0 && (tao_yyvsp[(1) - (1)].ihval) != 0)
            {
              i =
                idl_global->gen ()->create_interface (
                                        (tao_yyvsp[(1) - (1)].ihval)->name (),
                                        (tao_yyvsp[(1) - (1)].ihval)->inherits (),
                                        (tao_yyvsp[(1) - (1)].ihval)->n_inherits (),
                                        (tao_yyvsp[(1) - (1)].ihval)->inherits_flat (),
                                        (tao_yyvsp[(1) - (1)].ihval)->n_inherits_flat (),
                                        (tao_yyvsp[(1) - (1)].ihval)->is_local (),
                                        (tao_yyvsp[(1) - (1)].ihval)->is_abstract ()
                                      );
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the interface to its definition scope.
               */
              (void) s->fe_add_interface (i);

              // This FE_InterfaceHeader class isn't destroyed with the AST.
              (tao_yyvsp[(1) - (1)].ihval)->destroy ();
              delete (tao_yyvsp[(1) - (1)].ihval);
              (tao_yyvsp[(1) - (1)].ihval) = 0;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (i);
        }
    break;

  case 39:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
    break;

  case 40:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
    break;

  case 41:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 42:

    {
// interface_decl : IDL_INTERFACE
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
    break;

  case 43:

    {
//       id
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
         }
    break;

  case 44:

    {
// interface_header : interface_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);

          if ((tao_yyvsp[(2) - (2)].nlval) != 0 && (tao_yyvsp[(2) - (2)].nlval)->truncatable ())
            {
              idl_global->err ()->syntax_error (
                                      IDL_GlobalData::PS_InheritColonSeen
                                    );
            }

          /*
           * Create an AST representation of the information in the header
           * part of an interface - this representation contains a computed
           * list of all interfaces which this interface inherits from,
           * recursively
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ((tao_yyvsp[(1) - (2)].idval), 0),
                          1);
          ACE_NEW_RETURN ((tao_yyval.ihval),
                          FE_InterfaceHeader (n,
                                              (tao_yyvsp[(2) - (2)].nlval),
                                              false,
                                              false,
                                              true),
                          1);

          if (0 != (tao_yyvsp[(2) - (2)].nlval))
            {
              (tao_yyvsp[(2) - (2)].nlval)->destroy ();
              delete (tao_yyvsp[(2) - (2)].nlval);
              (tao_yyvsp[(2) - (2)].nlval) = 0;
            }
        }
    break;

  case 45:

    {
//      | IDL_LOCAL interface_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);

          /*
           * Create an AST representation of the information in the header
           * part of an interface - this representation contains a computed
           * list of all interfaces which this interface inherits from,
           * recursively
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ((tao_yyvsp[(2) - (3)].idval), 0),
                          1);
          ACE_NEW_RETURN ((tao_yyval.ihval),
                          FE_InterfaceHeader (n,
                                              (tao_yyvsp[(3) - (3)].nlval),
                                              true,
                                              false,
                                              true),
                          1);

          if (0 != (tao_yyvsp[(3) - (3)].nlval))
            {
              (tao_yyvsp[(3) - (3)].nlval)->destroy ();
              delete (tao_yyvsp[(3) - (3)].nlval);
              (tao_yyvsp[(3) - (3)].nlval) = 0;
            }
        }
    break;

  case 46:

    {
//      | IDL_ABSTRACT interface_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);

          /*
           * Create an AST representation of the information in the header
           * part of an interface - this representation contains a computed
           * list of all interfaces which this interface inherits from,
           * recursively
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ((tao_yyvsp[(2) - (3)].idval), 0),
                          1);
          ACE_NEW_RETURN ((tao_yyval.ihval),
                          FE_InterfaceHeader (n,
                                              (tao_yyvsp[(3) - (3)].nlval),
                                              false,
                                              true,
                                              true),
                          1);

          if (0 != (tao_yyvsp[(3) - (3)].nlval))
            {
              (tao_yyvsp[(3) - (3)].nlval)->destroy ();
              delete (tao_yyvsp[(3) - (3)].nlval);
              (tao_yyvsp[(3) - (3)].nlval) = 0;
            }
        }
    break;

  case 47:

    {
// inheritance_spec : ':' opt_truncatable
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
ACE_DEBUG ((LM_DEBUG, ": opt_truncatable\n"));
        }
    break;

  case 48:

    {
//      at_least_one_scoped_name
          (tao_yyvsp[(4) - (4)].nlval)->truncatable ((tao_yyvsp[(2) - (4)].bval));
          (tao_yyval.nlval) = (tao_yyvsp[(4) - (4)].nlval);
ACE_DEBUG ((LM_DEBUG, "at_least_one_scoped_name\n"));
        }
    break;

  case 49:

    {
/*      |  EMPTY */
          (tao_yyval.nlval) = 0;
ACE_DEBUG ((LM_DEBUG, "EMPTY\n"));
        }
    break;

  case 54:

    {
// valuetype : IDL_CUSTOM value_concrete_decl
           ACE_DEBUG ((LM_DEBUG,
                       ACE_TEXT ("error in %s line %d\n"),
                       idl_global->filename ()->get_string (),
                       idl_global->lineno ()));
           ACE_DEBUG ((LM_DEBUG,
                       ACE_TEXT ("Sorry, I (TAO_IDL) can't handle")
                       ACE_TEXT (" custom yet\n")));
        }
    break;

  case 56:

    {
// value_concrete_decl : value_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_ValueType *v = 0;
          AST_Interface *i = 0;

          if (s != 0 && (tao_yyvsp[(1) - (1)].vhval) != 0)
            {
              v =
                idl_global->gen ()->create_valuetype (
                    (tao_yyvsp[(1) - (1)].vhval)->name (),
                    (tao_yyvsp[(1) - (1)].vhval)->inherits (),
                    (tao_yyvsp[(1) - (1)].vhval)->n_inherits (),
                    (tao_yyvsp[(1) - (1)].vhval)->inherits_concrete (),
                    (tao_yyvsp[(1) - (1)].vhval)->inherits_flat (),
                    (tao_yyvsp[(1) - (1)].vhval)->n_inherits_flat (),
                    (tao_yyvsp[(1) - (1)].vhval)->supports (),
                    (tao_yyvsp[(1) - (1)].vhval)->n_supports (),
                    (tao_yyvsp[(1) - (1)].vhval)->supports_concrete (),
                    false,
                    (tao_yyvsp[(1) - (1)].vhval)->truncatable (),
                    false
                  );
              i = AST_Interface::narrow_from_decl (v);
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the valuetype to its definition scope
               */
              v = AST_ValueType::narrow_from_decl (i);
              (void) s->fe_add_valuetype (v);

              // FE_OBVHeader is not automatically destroyed in the AST
              (tao_yyvsp[(1) - (1)].vhval)->destroy ();
              delete (tao_yyvsp[(1) - (1)].vhval);
              (tao_yyvsp[(1) - (1)].vhval) = 0;
            }

          /*
           * Push it on the scope stack
           */
          idl_global->scopes ().push (v);
        }
    break;

  case 57:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
    break;

  case 58:

    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
    break;

  case 59:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          AST_ValueType *vt =
            AST_ValueType::narrow_from_scope (
                idl_global->scopes ().top_non_null ()
              );

          if (vt != 0 && vt->will_have_factory ())
            {
              idl_global->valuefactory_seen_ = true;
            }

          /*
           * Done with this value type - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 60:

    {
// value_abs_decl : IDL_ABSTRACT value_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_ValueType *v = 0;
          AST_Interface *i = 0;

          if (s != 0 && (tao_yyvsp[(2) - (2)].vhval) != 0)
            {
              v =
                idl_global->gen ()->create_valuetype (
                    (tao_yyvsp[(2) - (2)].vhval)->name (),
                    (tao_yyvsp[(2) - (2)].vhval)->inherits (),
                    (tao_yyvsp[(2) - (2)].vhval)->n_inherits (),
                    (tao_yyvsp[(2) - (2)].vhval)->inherits_concrete (),
                    (tao_yyvsp[(2) - (2)].vhval)->inherits_flat (),
                    (tao_yyvsp[(2) - (2)].vhval)->n_inherits_flat (),
                    (tao_yyvsp[(2) - (2)].vhval)->supports (),
                    (tao_yyvsp[(2) - (2)].vhval)->n_supports (),
                    (tao_yyvsp[(2) - (2)].vhval)->supports_concrete (),
                    true,
                    false,
                    false
                  );
              i = AST_Interface::narrow_from_decl (v);
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the valuetype to its definition scope
               */
              v = AST_ValueType::narrow_from_decl (i);
              (void) s->fe_add_valuetype (v);
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (v);
        }
    break;

  case 61:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
    break;

  case 62:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
    break;

  case 63:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 64:

    {
// value_header : value_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 65:

    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);

          UTL_ScopedName *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_ScopedName ((tao_yyvsp[(1) - (4)].idval), 0),
                          1);
          ACE_NEW_RETURN ((tao_yyval.vhval),
                          FE_OBVHeader (sn,
                                        (tao_yyvsp[(2) - (4)].nlval),
                                        (tao_yyvsp[(4) - (4)].nlval),
                                        (tao_yyvsp[(2) - (4)].nlval) ? (tao_yyvsp[(2) - (4)].nlval)->truncatable () : false),
                          1);

          if (0 != (tao_yyvsp[(4) - (4)].nlval))
            {
              (tao_yyvsp[(4) - (4)].nlval)->destroy ();
              delete (tao_yyvsp[(4) - (4)].nlval);
              (tao_yyvsp[(4) - (4)].nlval) = 0;
            }

          if (0 != (tao_yyvsp[(2) - (4)].nlval))
            {
              (tao_yyvsp[(2) - (4)].nlval)->destroy ();
              delete (tao_yyvsp[(2) - (4)].nlval);
              (tao_yyvsp[(2) - (4)].nlval) = 0;
            }
        }
    break;

  case 66:

    {
// value_decl : IDL_VALUETYPE
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
    break;

  case 67:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 68:

    {
// opt_truncatable : IDL_TRUNCATABLE
          (tao_yyval.bval) = true;
        }
    break;

  case 69:

    {
/*      |  EMPTY */
          (tao_yyval.bval) = false;
        }
    break;

  case 70:

    {
// supports_spec : IDL_SUPPORTS at_least_one_scoped_name
          (tao_yyval.nlval) = (tao_yyvsp[(2) - (2)].nlval);
        }
    break;

  case 71:

    {
/*      |    EMPTY */
          (tao_yyval.nlval) = 0;
        }
    break;

  case 72:

    {
// value_forward_decl : IDL_ABSTRACT value_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((tao_yyvsp[(2) - (2)].idval),
                            0);
          AST_ValueTypeFwd *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeForwardSeen);

          /*
           * Create a node representing a forward declaration of an
           * valuetype. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_valuetype_fwd (&n,
                                                            true);
              (void) s->fe_add_valuetype_fwd (f);
            }

          (tao_yyvsp[(2) - (2)].idval)->destroy ();
          delete (tao_yyvsp[(2) - (2)].idval);
          (tao_yyvsp[(2) - (2)].idval) = 0;
        }
    break;

  case 73:

    {
//      | value_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((tao_yyvsp[(1) - (1)].idval),
                            0);
          AST_ValueTypeFwd *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeForwardSeen);

          /*
           * Create a node representing a forward declaration of an
           * valuetype. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_valuetype_fwd (&n,
                                                            false);
              (void) s->fe_add_valuetype_fwd (f);
            }

          (tao_yyvsp[(1) - (1)].idval)->destroy ();
          delete (tao_yyvsp[(1) - (1)].idval);
          (tao_yyvsp[(1) - (1)].idval) = 0;
        }
    break;

  case 74:

    {
// value_box_decl : value_decl type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueBoxDeclSeen);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((tao_yyvsp[(1) - (2)].idval),
                            0);

          if (s != 0 && (tao_yyvsp[(2) - (2)].dcval) != 0)
            {
              /*
              * Get the type_spec associated with the valuebox
              */
              AST_Type *tp = 0;
              AST_Typedef *td
                = AST_Typedef::narrow_from_decl ((tao_yyvsp[(2) - (2)].dcval));

              if (td == 0)
                {
                  tp = AST_Type::narrow_from_decl ((tao_yyvsp[(2) - (2)].dcval));
                }
              else
                {
                  tp = td->primitive_base_type ();
                }

              if (tp == 0)
                {
                  // The <type_spec> given is a valid type
                  idl_global->err ()->not_a_type ((tao_yyvsp[(2) - (2)].dcval));
                }
              else
                {
                  AST_Decl::NodeType nt = tp->node_type ();

                  if (nt == AST_Decl::NT_valuetype
                      || nt == AST_Decl::NT_eventtype)
                    {
                      // valuetype is not allowed as <type_spec>
                      // for boxed value
                      idl_global->err ()->error0 (
                          UTL_Error::EIDL_ILLEGAL_BOXED_TYPE
                        );
                    }
                  else
                    {
                      /*
                      * Add the valuebox to its definition scope
                      */
                      AST_ValueBox *vb =
                        idl_global->gen ()->create_valuebox (&n,
                                                             tp);
                      (void) s->fe_add_valuebox (vb);
                    }
                }
            }

          (tao_yyvsp[(1) - (2)].idval)->destroy ();
          delete (tao_yyvsp[(1) - (2)].idval);
          (tao_yyvsp[(1) - (2)].idval) = 0;
        }
    break;

  case 80:

    {
// state_member : IDL_PUBLIC
          /* is $0 to member_i */
          (tao_yyval.vival) = AST_Field::vis_PUBLIC;
        }
    break;

  case 81:

    {
//      IDL_PRIVATE
          /* is $0 to member_i */
          (tao_yyval.vival) = AST_Field::vis_PRIVATE;
        }
    break;

  case 84:

    {
// export : type_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
    break;

  case 85:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 86:

    {
//      | typeid_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
    break;

  case 87:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 88:

    {
//      | typeprefix_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
    break;

  case 89:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 90:

    {
//      | const_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
    break;

  case 91:

    {
//      ';'
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
    break;

  case 92:

    {
//      | exception
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
    break;

  case 93:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 94:

    {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
    break;

  case 95:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 96:

    {
//      | operation
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
    break;

  case 97:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 98:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
    break;

  case 99:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 100:

    {
// at_least_one_scoped_name : scoped_name scoped_names
          ACE_NEW_RETURN ((tao_yyval.nlval),
                          UTL_NameList ((tao_yyvsp[(1) - (2)].idlist),
                                        (tao_yyvsp[(2) - (2)].nlval)),
                          1);
        }
    break;

  case 101:

    {
// scoped_names : scoped_names ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
    break;

  case 102:

    {
//      scoped_name
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopedNameSeen);

          if ((tao_yyvsp[(1) - (4)].nlval) == 0)
            {
              ACE_NEW_RETURN ((tao_yyval.nlval),
                              UTL_NameList ((tao_yyvsp[(4) - (4)].idlist),
                                            0),
                              1);
            }
          else
            {
              UTL_NameList *nl = 0;
              ACE_NEW_RETURN (nl,
                              UTL_NameList ((tao_yyvsp[(4) - (4)].idlist),
                                            0),
                              1);
              (tao_yyvsp[(1) - (4)].nlval)->nconc (nl);
              (tao_yyval.nlval) = (tao_yyvsp[(1) - (4)].nlval);
            }
        }
    break;

  case 103:

    {
/*      |  EMPTY */
          (tao_yyval.nlval) = 0;
        }
    break;

  case 104:

    {
// scoped_name : id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN ((tao_yyval.idlist),
                          UTL_IdList ((tao_yyvsp[(1) - (1)].idval),
                                      0),
                          1);
        }
    break;

  case 105:

    {
//      | IDL_SCOPE_DELIMITOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
    break;

  case 106:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          Identifier *id = 0;
          ACE_NEW_RETURN (id,
                          Identifier ((tao_yyvsp[(1) - (3)].strval)),
                          1);
          ACE::strdelete ((tao_yyvsp[(1) - (3)].strval));
          (tao_yyvsp[(1) - (3)].strval) = 0;
          UTL_IdList *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_IdList ((tao_yyvsp[(3) - (3)].idval),
                                      0),
                          1);
          ACE_NEW_RETURN ((tao_yyval.idlist),
                          UTL_IdList (id,
                                      sn),
                          1);
        }
    break;

  case 107:

    {
//      | scoped_name IDL_SCOPE_DELIMITOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);

          // This cleans up all the non-global "::"s in scoped names.
          // If there is a global one, it gets put into the UTL_IdList,
          // so we clean it up in the case above.
          ACE::strdelete ((tao_yyvsp[(2) - (2)].strval));
          (tao_yyvsp[(2) - (2)].strval) = 0;
        }
    break;

  case 108:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          UTL_IdList *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_IdList ((tao_yyvsp[(4) - (4)].idval),
                                      0),
                          1);
          (tao_yyvsp[(1) - (4)].idlist)->nconc (sn);
          (tao_yyval.idlist) = (tao_yyvsp[(1) - (4)].idlist);
        }
    break;

  case 109:

    {
// id: IDENTIFIER
          ACE_NEW_RETURN ((tao_yyval.idval),
                          Identifier ((tao_yyvsp[(1) - (1)].strval)),
                          1);
          ACE::strdelete ((tao_yyvsp[(1) - (1)].strval));
          (tao_yyvsp[(1) - (1)].strval) = 0;
        }
    break;

  case 110:

    {
// interface_forward : interface_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((tao_yyvsp[(1) - (1)].idval),
                            0);

          if (ACE_OS::strcmp ((tao_yyvsp[(1) - (1)].idval)->get_string (),
                              "TypeCode") == 0
              && !idl_global->in_main_file ())
            {
              AST_PredefinedType *pdt =
                idl_global->gen ()->create_predefined_type (
                                        AST_PredefinedType::PT_pseudo,
                                        &n
                                      );
              (void) s->add_predefined_type (pdt);
              s->add_to_scope (pdt);

              (tao_yyvsp[(1) - (1)].idval)->destroy ();
              delete (tao_yyvsp[(1) - (1)].idval);
              (tao_yyvsp[(1) - (1)].idval) = 0;

              break;
            }

          AST_InterfaceFwd *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceForwardSeen);

          /*
           * Create a node representing a forward declaration of an
           * interface. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_interface_fwd (&n,
                                                            0,
                                                            0);
              (void) s->fe_add_interface_fwd (f);
            }

          (tao_yyvsp[(1) - (1)].idval)->destroy ();
          delete (tao_yyvsp[(1) - (1)].idval);
          (tao_yyvsp[(1) - (1)].idval) = 0;
        }
    break;

  case 111:

    {
//      | IDL_LOCAL interface_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((tao_yyvsp[(2) - (2)].idval),
                            0);
          AST_InterfaceFwd *f = 0;
          idl_global->set_parse_state (
                          IDL_GlobalData::PS_InterfaceForwardSeen
                        );

          /*
           * Create a node representing a forward declaration of an
           * interface. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_interface_fwd (&n,
                                                            1,
                                                            0);
              (void) s->fe_add_interface_fwd (f);
            }

          (tao_yyvsp[(2) - (2)].idval)->destroy ();
          delete (tao_yyvsp[(2) - (2)].idval);
          (tao_yyvsp[(2) - (2)].idval) = 0;
        }
    break;

  case 112:

    {
//      | IDL_ABSTRACT interface_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((tao_yyvsp[(2) - (2)].idval),
                            0);
          AST_InterfaceFwd *f = 0;
          idl_global->set_parse_state (
                          IDL_GlobalData::PS_InterfaceForwardSeen
                        );

          /*
           * Create a node representing a forward declaration of an
           * interface. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_interface_fwd (&n,
                                                            0,
                                                            1);
              (void) s->fe_add_interface_fwd (f);
            }

          (tao_yyvsp[(2) - (2)].idval)->destroy ();
          delete (tao_yyvsp[(2) - (2)].idval);
          (tao_yyvsp[(2) - (2)].idval) = 0;
        }
    break;

  case 113:

    {
// const_dcl : IDL_CONST
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
    break;

  case 114:

    {
//      const_type
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
    break;

  case 115:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
    break;

  case 116:

    {
//      '='
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
    break;

  case 117:

    {
//      expression
          UTL_ScopedName n ((tao_yyvsp[(5) - (9)].idval),
                            0);
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Constant *c = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstExprSeen);

          /*
           * Create a node representing a constant declaration. Store
           * it in the enclosing scope.
           */
          if ((tao_yyvsp[(9) - (9)].exval) != 0 && s != 0)
            {
              AST_Expression::AST_ExprValue *result =
                (tao_yyvsp[(9) - (9)].exval)->check_and_coerce ((tao_yyvsp[(3) - (9)].etval),
                                      tao_enum_constant_decl);
              tao_enum_constant_decl = 0;

              if (result == 0)
                {
                  idl_global->err ()->coercion_error ((tao_yyvsp[(9) - (9)].exval),
                                                      (tao_yyvsp[(3) - (9)].etval));
                  (tao_yyvsp[(9) - (9)].exval)->destroy ();
                  delete (tao_yyvsp[(9) - (9)].exval);
                  (tao_yyvsp[(9) - (9)].exval) = 0;
                }
              else
                {
                  c =
                    idl_global->gen ()->create_constant (
                                            (tao_yyvsp[(3) - (9)].etval),
                                            (tao_yyvsp[(9) - (9)].exval),
                                            &n
                                          );
                  (void) s->fe_add_constant (c);
                  delete result;
                  result = 0;
                }

              (tao_yyvsp[(5) - (9)].idval)->destroy ();
              delete (tao_yyvsp[(5) - (9)].idval);
              (tao_yyvsp[(5) - (9)].idval) = 0;
            }
        }
    break;

  case 124:

    {
// const_type
//      : integer_type
//      | char_type
//      | octet_type
//      | boolean_type
//      | floating_pt_type
//      | fixed_type
//      | string_type_spec
          (tao_yyval.etval) = AST_Expression::EV_string;
        }
    break;

  case 125:

    {
//      | wstring_type_spec
          (tao_yyval.etval) = AST_Expression::EV_wstring;
        }
    break;

  case 126:

    {
//      | scoped_name
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_PredefinedType *c = 0;
          AST_Typedef *t = 0;

          /*
           * If the constant's type is a scoped name, it must resolve
           * to a scalar constant type
           */
          AST_Decl *d =
            s->lookup_by_name ((tao_yyvsp[(1) - (1)].idlist),
                               true);

          (tao_yyvsp[(1) - (1)].idlist)->destroy ();
          delete (tao_yyvsp[(1) - (1)].idlist);
          (tao_yyvsp[(1) - (1)].idlist) = 0;

          if (s != 0  && d != 0)
            {
              tao_enum_constant_decl = d;

              /*
               * Look through typedefs.
               */
              while (d->node_type () == AST_Decl::NT_typedef)
                {
                  t = AST_Typedef::narrow_from_decl (d);

                  if (t == 0)
                    {
                      break;
                    }

                  d = t->base_type ();
                }

              if (d == 0)
                {
                  (tao_yyval.etval) = AST_Expression::EV_enum;
                }
              else if (d->node_type () == AST_Decl::NT_pre_defined)
                {
                  c = AST_PredefinedType::narrow_from_decl (d);

                  if (c != 0)
                    {
                      (tao_yyval.etval) = idl_global->PredefinedTypeToExprType (c->pt ());
                    }
                  else
                    {
                      (tao_yyval.etval) = AST_Expression::EV_enum;
                    }
                }
              else if (d->node_type () == AST_Decl::NT_string)
                {
                  (tao_yyval.etval) = AST_Expression::EV_string;
                }
              else if (d->node_type () == AST_Decl::NT_wstring)
                {
                  (tao_yyval.etval) = AST_Expression::EV_wstring;
                }
              else
                {
                  (tao_yyval.etval) = AST_Expression::EV_enum;
                }
            }
          else
            {
              (tao_yyval.etval) = AST_Expression::EV_enum;
            }
        }
    break;

  case 130:

    {
// or_expr : xor_expr
//      | or_expr '|' xor_expr
          (tao_yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_or,
                                    (tao_yyvsp[(1) - (3)].exval),
                                    (tao_yyvsp[(3) - (3)].exval)
                                  );
        }
    break;

  case 132:

    {
// xor_expr : and_expr
//      | xor_expr '^' and_expr
          (tao_yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_xor,
                                    (tao_yyvsp[(1) - (3)].exval),
                                    (tao_yyvsp[(3) - (3)].exval)
                                  );
        }
    break;

  case 134:

    {
// and_expr : shift_expr | and_expr '&' shift_expr
          (tao_yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_and,
                                    (tao_yyvsp[(1) - (3)].exval),
                                    (tao_yyvsp[(3) - (3)].exval)
                                  );
        }
    break;

  case 136:

    {
// shift_expr : add_expr | shift_expr IDL_LEFT_SHIFT add_expr
          (tao_yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_left,
                                    (tao_yyvsp[(1) - (3)].exval),
                                    (tao_yyvsp[(3) - (3)].exval)
                                  );
        }
    break;

  case 137:

    {
//      | shift_expr IDL_RIGHT_SHIFT add_expr
          (tao_yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_right,
                                    (tao_yyvsp[(1) - (3)].exval),
                                    (tao_yyvsp[(3) - (3)].exval)
                                  );
        }
    break;

  case 139:

    {
// add_expr : mult_expr | add_expr '+' mult_expr
          (tao_yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_add,
                                    (tao_yyvsp[(1) - (3)].exval),
                                    (tao_yyvsp[(3) - (3)].exval)
                                  );
        }
    break;

  case 140:

    {
//      | add_expr '-' mult_expr
          (tao_yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_minus,
                                    (tao_yyvsp[(1) - (3)].exval),
                                    (tao_yyvsp[(3) - (3)].exval)
                                  );
        }
    break;

  case 142:

    {
// mult_expr : unary_expr | mult_expr '*' unary_expr
          (tao_yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mul,
                                    (tao_yyvsp[(1) - (3)].exval),
                                    (tao_yyvsp[(3) - (3)].exval)
                                  );
        }
    break;

  case 143:

    {
//      | mult_expr '/' unary_expr
          (tao_yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_div,
                                    (tao_yyvsp[(1) - (3)].exval),
                                    (tao_yyvsp[(3) - (3)].exval)
                                  );
        }
    break;

  case 144:

    {
//      | mult_expr '%' unary_expr
          (tao_yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mod,
                                    (tao_yyvsp[(1) - (3)].exval),
                                    (tao_yyvsp[(3) - (3)].exval)
                                  );
        }
    break;

  case 146:

    {
// unary_expr : primary_expr | '+' primary_expr
          (tao_yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_u_plus,
                                    (tao_yyvsp[(2) - (2)].exval),
                                    0
                                  );
        }
    break;

  case 147:

    {
//      | '-' primary_expr
          (tao_yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_u_minus,
                                   (tao_yyvsp[(2) - (2)].exval),
                                   0
                                 );
        }
    break;

  case 148:

    {
//      | '~' primary_expr
          (tao_yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_bit_neg,
                                   (tao_yyvsp[(2) - (2)].exval),
                                   0
                                 );
        }
    break;

  case 149:

    {
// primary_expr : scoped_name
          /*
           * An expression which is a scoped name is not resolved now,
           * but only when it is evaluated (such as when it is assigned
           * as a constant value).
           */
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(1) - (1)].idlist),
                                           true);

          /*
           * If the scoped name is an IDL constant, it may be used in an
           * array dim, a string bound, or a sequence bound. If so, it
           * must be unsigned and > 0. We assign the constant's value
           * and type to the expression created here so we can check
           * them later.
           */
          if (d != 0 && d->node_type () == AST_Decl::NT_const)
            {
              AST_Constant *c = AST_Constant::narrow_from_decl (d);
              (tao_yyval.exval) =
                idl_global->gen ()->create_expr (c->constant_value (),
                                                 c->et ());

              (tao_yyvsp[(1) - (1)].idlist)->destroy ();
              delete (tao_yyvsp[(1) - (1)].idlist);
              (tao_yyvsp[(1) - (1)].idlist) = 0;
            }
          else
            {
              (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].idlist));
            }
        }
    break;

  case 151:

    {
//      | literal
//      | '(' const_expr ')'
          (tao_yyval.exval) = (tao_yyvsp[(2) - (3)].exval);
        }
    break;

  case 152:

    {
// literal : IDL_INTEGER_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].ival));
        }
    break;

  case 153:

    {
//      | IDL_UINTEGER_LITERAL
          (tao_yyval.exval) =
            idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].uival),
                                             AST_Expression::EV_ulonglong);
        }
    break;

  case 154:

    {
//      | IDL_STRING_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].sval));
          (tao_yyvsp[(1) - (1)].sval)->destroy ();
          delete (tao_yyvsp[(1) - (1)].sval);
          (tao_yyvsp[(1) - (1)].sval) = 0;
        }
    break;

  case 155:

    {
//      | IDL_WSTRING_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].wsval));
        }
    break;

  case 156:

    {
//      | IDL_CHARACTER_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].cval));
        }
    break;

  case 157:

    {
//      | IDL_WCHAR_LITERAL
          ACE_OutputCDR::from_wchar wc ((tao_yyvsp[(1) - (1)].wcval));
          (tao_yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
    break;

  case 158:

    {
//      | IDL_FLOATING_PT_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].dval));
        }
    break;

  case 159:

    {
//      | IDL_TRUETOK
          (tao_yyval.exval) = idl_global->gen ()->create_expr (true);
        }
    break;

  case 160:

    {
//      | IDL_FALSETOK
          (tao_yyval.exval) = idl_global->gen ()->create_expr (false);
        }
    break;

  case 161:

    {
// positive_int_expr : const_expr
          int good_expression = 1;
          (tao_yyvsp[(1) - (1)].exval)->evaluate (AST_Expression::EK_positive_int);
          AST_Expression::AST_ExprValue *ev = (tao_yyvsp[(1) - (1)].exval)->ev ();

          /*
           * If const_expr is an enum value (AST_EnumVal inherits from
           * AST_Constant), the AST_ExprValue will probably not be set,
           * but there's no need to check anyway
           */
          if (ev != 0)
            {
              switch (ev->et)
              {
                case AST_Expression::EV_ushort:
                  if (ev->u.usval == 0)
                    {
                      good_expression = 0;
                    }

                  break;
                case AST_Expression::EV_ulong:
                  if (ev->u.ulval == 0)
                    {
                      good_expression = 0;
                    }

                  break;
#if !defined (ACE_LACKS_LONGLONG_T)
                case AST_Expression::EV_ulonglong:
                  if (ev->u.ullval == 0)
                    {
                      good_expression = 0;
                    }

                  break;
#endif
                case AST_Expression::EV_octet:
                  if (ev->u.oval == 0)
                    {
                      good_expression = 0;
                    }

                  break;
                case AST_Expression::EV_bool:
                  if (ev->u.bval == 0)
                    {
                      good_expression = 0;
                    }

                  break;
                default:
                  good_expression = 0;
                  break;
              }
            }

          if (good_expression)
            {
              (tao_yyval.exval) = (tao_yyvsp[(1) - (1)].exval);
            }
          else
            {
              idl_global->err ()->syntax_error (idl_global->parse_state ());
            }
        }
    break;

  case 162:

    {
// type_dcl : IDL_TYPEDEF
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
        }
    break;

  case 163:

    {
//      type_declarator
          (tao_yyval.ival) = 0;
        }
    break;

  case 164:

    {
//      | struct_type
          (tao_yyval.ival) = 0;
        }
    break;

  case 165:

    {
//      | union_type
          (tao_yyval.ival) = 0;
        }
    break;

  case 166:

    {
//      | enum_type
          (tao_yyval.ival) = 0;
        }
    break;

  case 167:

    {
//      | IDL_NATIVE simple_declarator
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Native *node = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_NativeSeen);

          /*
           * Create a node representing a Native and add it to its
           * enclosing scope
           */
          if (s != 0)
            {
              node =
                idl_global->gen ()->create_native (
                                        (tao_yyvsp[(2) - (2)].deval)->name ()
                                      );
              /*
               * Add it to its defining scope
               */
              (void) s->fe_add_native (node);
            }

          (tao_yyvsp[(2) - (2)].deval)->destroy ();
          delete (tao_yyvsp[(2) - (2)].deval);
          (tao_yyvsp[(2) - (2)].deval) = 0;
        }
    break;

  case 168:

    {
//      | constructed_forward_type_spec
          (tao_yyval.ival) = 0;
        }
    break;

  case 169:

    {
// type_declarator : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
    break;

  case 170:

    {
//      at_least_one_declarator
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          FE_Declarator *d = 0;
          AST_Typedef *t = 0;
          unsigned long index = 0UL;
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclaratorsSeen);

          /*
           * Create a list of type renamings. Add them to the
           * enclosing scope
           */
          if (s != 0 && (tao_yyvsp[(1) - (3)].dcval) != 0 && (tao_yyvsp[(3) - (3)].dlval) != 0)
            {
              for (UTL_DecllistActiveIterator l ((tao_yyvsp[(3) - (3)].dlval));
                   !l.is_done ();
                   l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type * tp = d->compose ((tao_yyvsp[(1) - (3)].dcval));

                  if (tp == 0)
                    {
                      continue;
                    }

                  if (AST_Decl::NT_except == tp->node_type ())
                    {
                      idl_global->err ()->not_a_type (tp);
                      continue;
                    }

                  t = idl_global->gen ()->create_typedef (tp,
                                                          d->name (),
                                                          s->is_local (),
                                                          s->is_abstract ());

                  // If the base type is a sequence or array, the typedef
                  // constructor sets owns_base_type_ to true. But if
                  // there is a comma-separated list of such typedefs,
                  // the base type can be destroyed only once. In all
                  // other cases, the line below has no effect.
                  if (index++ > 0)
                    {
                      t->owns_base_type (false);
                    }

                  (void) s->fe_add_typedef (t);
                }

              // This FE_Declarator class isn't destroyed with the AST.
              (tao_yyvsp[(3) - (3)].dlval)->destroy ();
              delete (tao_yyvsp[(3) - (3)].dlval);
              (tao_yyvsp[(3) - (3)].dlval) = 0;
            }
        }
    break;

  case 173:

    {
// simple_type_spec : base_type_spec
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 175:

    {
//      | template_type_spec
//      | scoped_name
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = 0;

          if (s != 0)
            {
              d = s->lookup_by_name ((tao_yyvsp[(1) - (1)].idlist),
                                     true);
            }

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(1) - (1)].idlist));
            }
          else
            {
              d->last_referenced_as ((tao_yyvsp[(1) - (1)].idlist));
            }


          (tao_yyval.dcval) = d;
        }
    break;

  case 192:

    {
// at_least_one_declarator : declarator declarators
          ACE_NEW_RETURN ((tao_yyval.dlval),
                          UTL_DeclList ((tao_yyvsp[(1) - (2)].deval),
                                        (tao_yyvsp[(2) - (2)].dlval)),
                          1);
        }
    break;

  case 193:

    {
// declarators : declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
    break;

  case 194:

    {
//      declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsDeclSeen);

          if ((tao_yyvsp[(1) - (4)].dlval) == 0)
            {
              ACE_NEW_RETURN ((tao_yyval.dlval),
                              UTL_DeclList ((tao_yyvsp[(4) - (4)].deval),
                                            0),
                              1);
            }
          else
            {
              UTL_DeclList *dl = 0;
              ACE_NEW_RETURN (dl,
                              UTL_DeclList ((tao_yyvsp[(4) - (4)].deval),
                                            0),
                              1);
              (tao_yyvsp[(1) - (4)].dlval)->nconc (dl);
              (tao_yyval.dlval) = (tao_yyvsp[(1) - (4)].dlval);
            }
        }
    break;

  case 195:

    {
/*      |  EMPTY */
          (tao_yyval.dlval) = 0;
        }
    break;

  case 198:

    {
// at_least_one_simple_declarator : simple_declarator simple_declarators
          ACE_NEW_RETURN ((tao_yyval.dlval),
                          UTL_DeclList ((tao_yyvsp[(1) - (2)].deval),
                                        (tao_yyvsp[(2) - (2)].dlval)),
                          1);
        }
    break;

  case 199:

    {
// simple_declarators : simple_declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
    break;

  case 200:

    {
//      simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsDeclSeen);

          if ((tao_yyvsp[(1) - (4)].dlval) == 0)
            {
              ACE_NEW_RETURN ((tao_yyval.dlval),
                              UTL_DeclList ((tao_yyvsp[(4) - (4)].deval),
                                            0),
                              1);
            }
          else
            {
              UTL_DeclList *dl = 0;
              ACE_NEW_RETURN (dl,
                              UTL_DeclList ((tao_yyvsp[(4) - (4)].deval),
                                            0),
                              1);
              (tao_yyvsp[(1) - (4)].dlval)->nconc (dl);
              (tao_yyval.dlval) = (tao_yyvsp[(1) - (4)].dlval);
            }
        }
    break;

  case 201:

    {
/*      |  EMPTY */
          (tao_yyval.dlval) = 0;
        }
    break;

  case 202:

    {
// simple_declarator : id
          UTL_ScopedName *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_ScopedName ((tao_yyvsp[(1) - (1)].idval),
                                          0),
                          1);
          ACE_NEW_RETURN ((tao_yyval.deval),
                          FE_Declarator (sn,
                                         FE_Declarator::FD_simple,
                                         0),
                          1);
        }
    break;

  case 203:

    {
// complex_declarator : array_declarator
          UTL_ScopedName *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_ScopedName (
                              (tao_yyvsp[(1) - (1)].dcval)->local_name ()->copy (),
                              0
                            ),
                          1);
          ACE_NEW_RETURN ((tao_yyval.deval),
                          FE_Declarator (sn,
                                         FE_Declarator::FD_complex,
                                         (tao_yyvsp[(1) - (1)].dcval)),
                          1);
        }
    break;

  case 206:

    {
// signed_int : IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_long;
        }
    break;

  case 207:

    {
//      | IDL_LONG IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_longlong;
        }
    break;

  case 208:

    {
//      | IDL_SHORT
          (tao_yyval.etval) = AST_Expression::EV_short;
        }
    break;

  case 209:

    {
// unsigned_int : IDL_UNSIGNED IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_ulong;
        }
    break;

  case 210:

    {
//      | IDL_UNSIGNED IDL_LONG IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_ulonglong;
        }
    break;

  case 211:

    {
//      | IDL_UNSIGNED IDL_SHORT
          (tao_yyval.etval) = AST_Expression::EV_ushort;
        }
    break;

  case 212:

    {
// floating_pt_type : IDL_DOUBLE
          (tao_yyval.etval) = AST_Expression::EV_double;
        }
    break;

  case 213:

    {
//      | IDL_FLOAT
          (tao_yyval.etval) = AST_Expression::EV_float;
        }
    break;

  case 214:

    {
//      | IDL_LONG IDL_DOUBLE
          (tao_yyval.etval) = AST_Expression::EV_longdouble;
        }
    break;

  case 215:

    {
// fixed_type : IDL_FIXED
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("error in %s line %d\n"),
                      idl_global->filename ()->get_string (),
                      idl_global->lineno ()));
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Sorry, I (TAO_IDL) can't handle")
                      ACE_TEXT (" fixed types yet\n")));
        }
    break;

  case 216:

    {
// char_type : IDL_CHAR
          (tao_yyval.etval) = AST_Expression::EV_char;
        }
    break;

  case 217:

    {
//      | IDL_WCHAR
          (tao_yyval.etval) = AST_Expression::EV_wchar;
        }
    break;

  case 218:

    {
// octet_type : IDL_OCTET
          (tao_yyval.etval) = AST_Expression::EV_octet;
        }
    break;

  case 219:

    {
// boolean_type : IDL_BOOLEAN
          (tao_yyval.etval) = AST_Expression::EV_bool;
        }
    break;

  case 220:

    {
// any_type : IDL_ANY
          (tao_yyval.etval) = AST_Expression::EV_any;
        }
    break;

  case 221:

    {
// object_type : IDL_OBJECT
          (tao_yyval.etval) = AST_Expression::EV_object;
        }
    break;

  case 222:

    {
// struct_decl : IDL_STRUCT
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
    break;

  case 223:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 224:

    {
// struct_type : struct_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((tao_yyvsp[(1) - (1)].idval),
                            0);
          AST_Structure *d = 0;

          /*
           * Create a node representing a struct declaration. Add it
           * to the enclosing scope
           */
          if (s != 0)
            {
              d =
                idl_global->gen ()->create_structure (
                                        &n,
                                        s->is_local (),
                                        s->is_abstract ()
                                      );
              AST_Structure::fwd_redefinition_helper (d,
                                                      s);
              (void) s->fe_add_structure (d);
            }

          /*
           * Push the scope of the struct on the scopes stack.
           */
          idl_global->scopes ().push (d);

          (tao_yyvsp[(1) - (1)].idval)->destroy ();
          delete (tao_yyvsp[(1) - (1)].idval);
          (tao_yyvsp[(1) - (1)].idval) = 0;
        }
    break;

  case 225:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
    break;

  case 226:

    {
//      at_least_one_member
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
    break;

  case 227:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_StructQsSeen);

          /*
           * Done with this struct. Pop its scope off the scopes stack.
           */
          (tao_yyval.dcval) = AST_Structure::narrow_from_scope (
                   idl_global->scopes ().top_non_null ()
                 );
          idl_global->scopes ().pop ();
        }
    break;

  case 231:

    {
// member  :
          /* is $0 to member_i */
          (tao_yyval.vival) = AST_Field::vis_NA;
        }
    break;

  case 233:

    {
// member_i : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
    break;

  case 234:

    {
//      at_least_one_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
    break;

  case 235:

    {
//      ';'
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          FE_Declarator *d = 0;
          AST_Field *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsCompleted);

          /*
           * Check for illegal recursive use of type.
           */
          if ((tao_yyvsp[(1) - (5)].dcval) != 0
              && AST_illegal_recursive_type ((tao_yyvsp[(1) - (5)].dcval)))
            {
              idl_global->err ()->error1 (UTL_Error::EIDL_RECURSIVE_TYPE,
                                          (tao_yyvsp[(1) - (5)].dcval));
            }
          /*
           * Create a node representing a struct or exception member
           * Add it to the enclosing scope.
           */
          else if (s != 0
                   && (tao_yyvsp[(1) - (5)].dcval) != 0
                   && (tao_yyvsp[(3) - (5)].dlval) != 0)
            {
              for (UTL_DecllistActiveIterator l ((tao_yyvsp[(3) - (5)].dlval));
                   !l.is_done ();
                   l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type *tp = d->compose ((tao_yyvsp[(1) - (5)].dcval));

                  if (tp == 0)
                    {
                      continue;
                    }

                  /* $0 denotes Visibility, must be on yacc reduction stack. */
                  f =
                    idl_global->gen ()->create_field (
                                            tp,
                                            d->name (),
                                            (tao_yyvsp[(0) - (5)].vival)
                                          );
                  (void) s->fe_add_field (f);
                }
            }

          (tao_yyvsp[(3) - (5)].dlval)->destroy ();
          delete (tao_yyvsp[(3) - (5)].dlval);
          (tao_yyvsp[(3) - (5)].dlval) = 0;
        }
    break;

  case 236:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state ());
        }
    break;

  case 237:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 238:

    {
// union_decl : IDL_UNION
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
    break;

  case 239:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 240:

    {
// union_type : union_decl IDL_SWITCH
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
    break;

  case 241:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchOpenParSeen);
        }
    break;

  case 242:

    {
//      switch_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
    break;

  case 243:

    {
//      ')'
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((tao_yyvsp[(1) - (8)].idval),
                            0);
          AST_Union *u = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchCloseParSeen);

          /*
           * Create a node representing a union. Add it to its enclosing
           * scope.
           */
          if ((tao_yyvsp[(6) - (8)].dcval) != 0
              && s != 0)
            {
              AST_ConcreteType *tp =
                AST_ConcreteType::narrow_from_decl ((tao_yyvsp[(6) - (8)].dcval));

              if (tp == 0)
                {
                  idl_global->err ()->not_a_type ((tao_yyvsp[(6) - (8)].dcval));
                }
              else
                {
                  u = idl_global->gen ()->create_union (tp,
                                                        &n,
                                                        s->is_local (),
                                                        s->is_abstract ());
                }

              AST_Structure *st = AST_Structure::narrow_from_decl (u);
              AST_Structure::fwd_redefinition_helper (st,
                                                      s);
              u = AST_Union::narrow_from_decl (st);
              (void) s->fe_add_union (u);
            }

          /*
           * Push the scope of the union on the scopes stack
           */
          idl_global->scopes ().push (u);

          (tao_yyvsp[(1) - (8)].idval)->destroy ();
          delete (tao_yyvsp[(1) - (8)].idval);
          (tao_yyvsp[(1) - (8)].idval) = 0;
        }
    break;

  case 244:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
    break;

  case 245:

    {
//      at_least_one_case_branch
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
    break;

  case 246:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionQsSeen);

          /*
           * Done with this union. Pop its scope from the scopes stack.
           */
          (tao_yyval.dcval) = AST_Union::narrow_from_scope (
                   idl_global->scopes ().top_non_null ()
                 );

          if ((tao_yyval.dcval) != 0)
            {
              idl_global->scopes ().pop ();
            }
        }
    break;

  case 247:

    {
// switch_type_spec : integer_type
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 248:

    {
//      | char_type
          /* wchars are not allowed. */
          if ((tao_yyvsp[(1) - (1)].etval) == AST_Expression::EV_wchar)
            {
              idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
            }

          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 249:

    {
//      | octet_type
          /* octets are not allowed. */
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 250:

    {
//      | boolean_type
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 252:

    {
//      | enum_type
//      | scoped_name
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = 0;
          AST_PredefinedType *p = 0;
          AST_Typedef *t = 0;
          bool found = false;

          /*
           * The discriminator is a scoped name. Try to resolve to
           * one of the scalar types or to an enum. Thread through
           * typedef's to arrive at the base type at the end of the
           * chain.
           */
          d = s->lookup_by_name ((tao_yyvsp[(1) - (1)].idlist),
                                 true);

          if (s != 0 && d != 0)
            {
              while (!found)
                {
                  switch (d->node_type ())
                  {
                    case AST_Decl::NT_enum:
                      (tao_yyval.dcval) = d;
                      found = true;
                      break;
                    case AST_Decl::NT_pre_defined:
                      p = AST_PredefinedType::narrow_from_decl (d);

                      if (p != 0)
                        {
                          switch (p->pt ())
                          {
                            case AST_PredefinedType::PT_long:
                            case AST_PredefinedType::PT_ulong:
                            case AST_PredefinedType::PT_longlong:
                            case AST_PredefinedType::PT_ulonglong:
                            case AST_PredefinedType::PT_short:
                            case AST_PredefinedType::PT_ushort:
                            case AST_PredefinedType::PT_char:
                            case AST_PredefinedType::PT_boolean:
                              (tao_yyval.dcval) = p;
                              found = true;
                              break;
                            case AST_PredefinedType::PT_wchar:
                            case AST_PredefinedType::PT_octet:
                              /* octets and wchars are not allowed */
                              idl_global->err ()->error0 (
                                  UTL_Error::EIDL_DISC_TYPE
                                );
                              (tao_yyval.dcval) = 0;
                              found = true;
                              break;
                            default:
                              (tao_yyval.dcval) = 0;
                              found = true;
                              break;
                          }
                        }
                      else
                        {
                          (tao_yyval.dcval) = 0;
                          found = true;
                        }

                      break;
                    case AST_Decl::NT_typedef:
                      t = AST_Typedef::narrow_from_decl (d);

                      if (t != 0)
                        {
                          d = t->base_type ();
                        }

                      break;
                    default:
                      (tao_yyval.dcval) = 0;
                      found = true;
                      break;
                  }
                }
            }
          else
            {
              (tao_yyval.dcval) = 0;
            }

          if ((tao_yyval.dcval) == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(1) - (1)].idlist));

              (tao_yyvsp[(1) - (1)].idlist)->destroy ();
              delete (tao_yyvsp[(1) - (1)].idlist);
              (tao_yyvsp[(1) - (1)].idlist) = 0;

              /* If we don't return here, we'll crash later.*/
              return 1;
            }

          (tao_yyvsp[(1) - (1)].idlist)->destroy ();
          delete (tao_yyvsp[(1) - (1)].idlist);
          (tao_yyvsp[(1) - (1)].idlist) = 0;
        }
    break;

  case 256:

    {
// case_branch : at_least_one_case_label
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
    break;

  case 257:

    {
//      element_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
    break;

  case 258:

    {
//      ';'
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_UnionBranch *b = 0;
          AST_Field *f = (tao_yyvsp[(3) - (5)].ffval);
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemCompleted);

          /*
           * Create several nodes representing branches of a union.
           * Add them to the enclosing scope (the union scope)
           */
          if (s != 0
              &&  (tao_yyvsp[(1) - (5)].llval) != 0
              && (tao_yyvsp[(3) - (5)].ffval) != 0)
            {
              b =
                idl_global->gen ()->create_union_branch (
                                        (tao_yyvsp[(1) - (5)].llval),
                                        f->field_type (),
                                        f->name ()
                                      );
              (void) s->fe_add_union_branch (b);

              // f has passed its field type to the union branch,
              // but the rest still needs to be cleaned up.
              f->AST_Decl::destroy ();
              delete f;
              f = 0;
            }
        }
    break;

  case 259:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
    break;

  case 260:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 261:

    {
// at_least_one_case_label : case_label case_labels
          ACE_NEW_RETURN ((tao_yyval.llval),
                          UTL_LabelList ((tao_yyvsp[(1) - (2)].ulval),
                                         (tao_yyvsp[(2) - (2)].llval)),
                          1);
        }
    break;

  case 262:

    {
// case_labels : case_labels case_label
          if ((tao_yyvsp[(1) - (2)].llval) == 0)
            {
              ACE_NEW_RETURN ((tao_yyval.llval),
                              UTL_LabelList ((tao_yyvsp[(2) - (2)].ulval),
                                             0),
                              1);
            }
          else
            {
              UTL_LabelList *ll = 0;
              ACE_NEW_RETURN (ll,
                              UTL_LabelList ((tao_yyvsp[(2) - (2)].ulval),
                                             0),
                              1);
              (tao_yyvsp[(1) - (2)].llval)->nconc (ll);
              (tao_yyval.llval) = (tao_yyvsp[(1) - (2)].llval);
            }
        }
    break;

  case 263:

    {
/*      |  EMPTY */
          (tao_yyval.llval) = 0;
        }
    break;

  case 264:

    {
// case_label : IDL_DEFAULT
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
    break;

  case 265:

    {
//      ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (tao_yyval.ulval) = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
    break;

  case 266:

    {
//      | IDL_CASE
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
    break;

  case 267:

    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
    break;

  case 268:

    {
//      const_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (tao_yyval.ulval) = idl_global->gen()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      (tao_yyvsp[(3) - (5)].exval)
                                    );
        }
    break;

  case 269:

    {
// element_spec : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
    break;

  case 270:

    {
//      declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemDeclSeen);

          /*
           * Check for illegal recursive use of type
           */
          if ((tao_yyvsp[(1) - (3)].dcval) != 0
              && AST_illegal_recursive_type ((tao_yyvsp[(1) - (3)].dcval)))
            {
              idl_global->err()->error1 (UTL_Error::EIDL_RECURSIVE_TYPE,
                                         (tao_yyvsp[(1) - (3)].dcval));

              (tao_yyval.ffval) = 0;
            }
          /*
           * Create a field in a union branch
           */
          else if ((tao_yyvsp[(1) - (3)].dcval) == 0
                   || (tao_yyvsp[(3) - (3)].deval) == 0)
            {
              (tao_yyval.ffval) = 0;
            }
          else
            {
              AST_Type *tp = (tao_yyvsp[(3) - (3)].deval)->compose ((tao_yyvsp[(1) - (3)].dcval));

              if (tp == 0)
                {
                  (tao_yyval.ffval) = 0;
                }
              else
                {
                  (tao_yyval.ffval) = idl_global->gen ()->create_field (
                                               tp,
                                               (tao_yyvsp[(3) - (3)].deval)->name ()
                                             );
                }

              (tao_yyvsp[(3) - (3)].deval)->destroy ();
              delete (tao_yyvsp[(3) - (3)].deval);
              (tao_yyvsp[(3) - (3)].deval) = 0;
            }
        }
    break;

  case 271:

    {
// struct_forward_type : struct_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((tao_yyvsp[(1) - (1)].idval),
                            0);
          AST_StructureFwd *d = 0;

          /*
           * Create a node representing a forward declaration of a struct.
           */
          if (s != 0)
            {
              d = idl_global->gen ()->create_structure_fwd (&n);
              (void) s->fe_add_structure_fwd (d);
            }

          (tao_yyvsp[(1) - (1)].idval)->destroy ();
          delete (tao_yyvsp[(1) - (1)].idval);
          (tao_yyvsp[(1) - (1)].idval) = 0;
        }
    break;

  case 272:

    {
// union_forward_type : union_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((tao_yyvsp[(1) - (1)].idval),
                            0);
          AST_UnionFwd *u = 0;

          /*
           * Create a node representing a forward declaration of a union.
           */
          if (s != 0)
            {
              u = idl_global->gen ()->create_union_fwd (&n);
              (void) s->fe_add_union_fwd (u);
            }

          (tao_yyvsp[(1) - (1)].idval)->destroy ();
          delete (tao_yyvsp[(1) - (1)].idval);
          (tao_yyvsp[(1) - (1)].idval) = 0;
        }
    break;

  case 273:

    {
// enum_type : IDL_ENUM
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
    break;

  case 274:

    {
//      id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((tao_yyvsp[(3) - (3)].idval),
                            0);
          AST_Enum *e = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumIDSeen);

          /*
           * Create a node representing an enum and add it to its
           * enclosing scope.
           */
          if (s != 0)
            {
              e = idl_global->gen ()->create_enum (&n,
                                                   s->is_local (),
                                                   s->is_abstract ());
              /*
               * Add it to its defining scope
               */
              (void) s->fe_add_enum (e);
            }

          /*
           * Push the enum scope on the scopes stack.
           */
          idl_global->scopes ().push (e);

          (tao_yyvsp[(3) - (3)].idval)->destroy ();
          delete (tao_yyvsp[(3) - (3)].idval);
          (tao_yyvsp[(3) - (3)].idval) = 0;
        }
    break;

  case 275:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
    break;

  case 276:

    {
//      at_least_one_enumerator
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
    break;

  case 277:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumQsSeen);

          /*
           * Done with this enum. Pop its scope from the scopes stack.
           */
          if (idl_global->scopes ().top () == 0)
            {
              (tao_yyval.dcval) = 0;
            }
          else
            {
              (tao_yyval.dcval) = AST_Enum::narrow_from_scope (
                       idl_global->scopes ().top_non_null ()
                     );
              idl_global->scopes ().pop ();
            }
        }
    break;

  case 279:

    {
// enumerators : enumerators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
    break;

  case 282:

    {
// enumerator : IDENTIFIER
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          Identifier id ((tao_yyvsp[(1) - (1)].strval));
          ACE::strdelete ((tao_yyvsp[(1) - (1)].strval));
          (tao_yyvsp[(1) - (1)].strval) = 0;
          UTL_ScopedName n (&id,
                            0);
          AST_EnumVal *e = 0;
          AST_Enum *c = 0;

          /*
           * Create a node representing one enumerator in an enum
           * Add it to the enclosing scope (the enum scope)
           */
          if (s != 0
              && s->scope_node_type () == AST_Decl::NT_enum)
            {
              c = AST_Enum::narrow_from_scope (s);

              if (c != 0)
                {
                  e = idl_global->gen ()->create_enum_val (
                                              c->next_enum_val (),
                                              &n
                                            );
                }

              (void) s->fe_add_enum_val (e);
            }
        }
    break;

  case 283:

    {
// sequence_type_spec : seq_head ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
    break;

  case 284:

    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
    break;

  case 285:

    {
//      '>'
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceQsSeen);

          /*
           * Remove sequence marker from scopes stack.
           */
          if (idl_global->scopes ().top() == 0)
            {
              idl_global->scopes ().pop ();
            }

          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          /*
           * Create a node representing a sequence
           */
          AST_Expression::AST_ExprValue *ev = 0;

          if ((tao_yyvsp[(4) - (6)].exval) != 0)
            {
              ev = (tao_yyvsp[(4) - (6)].exval)->coerce (AST_Expression::EV_ulong);
            }

          if (0 == (tao_yyvsp[(4) - (6)].exval) || 0 == ev)
            {
              idl_global->err ()->coercion_error ((tao_yyvsp[(4) - (6)].exval),
                                                  AST_Expression::EV_ulong);
              (tao_yyval.dcval) = 0;
            }
          else if (0 == (tao_yyvsp[(1) - (6)].dcval))
            {
              (tao_yyval.dcval) = 0;
            }
          else
            {
              AST_Type *tp = AST_Type::narrow_from_decl ((tao_yyvsp[(1) - (6)].dcval));

              if (0 == tp)
                {
                  ; // Error will be caught in FE_Declarator.
                }
              else
                {
                  Identifier id ("sequence");
                  UTL_ScopedName sn (&id,
                                     0);

                  (tao_yyval.dcval) =
                    idl_global->gen ()->create_sequence (
                                            (tao_yyvsp[(4) - (6)].exval),
                                            tp,
                                            &sn,
                                            s->is_local (),
                                            s->is_abstract ()
                                          );
                }
            }

          delete ev;
          ev = 0;
        }
    break;

  case 286:

    {
//      | seq_head '>'
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceQsSeen);

          /*
           * Remove sequence marker from scopes stack.
           */
          if (idl_global->scopes ().top () == 0)
            {
             idl_global->scopes ().pop ();
            }

          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          /*
           * Create a node representing a sequence.
           */
          if ((tao_yyvsp[(1) - (2)].dcval) == 0)
            {
              (tao_yyval.dcval) = 0;
            }
          else
            {
              AST_Type *tp = AST_Type::narrow_from_decl ((tao_yyvsp[(1) - (2)].dcval));

              if (tp == 0)
                {
                  ; // Error will be caught in FE_Declarator.
                }
              else
                {
                  Identifier id ("sequence");
                  UTL_ScopedName sn (&id, 0);
                  ACE_CDR::ULong bound = 0UL;

                  (tao_yyval.dcval) =
                    idl_global->gen ()->create_sequence (
                        idl_global->gen ()->create_expr (
                                                bound,
                                                AST_Expression::EV_ulong
                                              ),
                        tp,
                        &sn,
                        s->is_local (),
                        s->is_abstract ()
                      );
                }
            }
        }
    break;

  case 287:

    {
// seq_head : IDL_SEQUENCE
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
    break;

  case 288:

    {
//      '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
    break;

  case 289:

    {
//      simple_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          (tao_yyval.dcval) = (tao_yyvsp[(5) - (5)].dcval);
        }
    break;

  case 290:

    {
// string_type_spec : string_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
    break;

  case 291:

    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
    break;

  case 292:

    {
//      '>'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringQsSeen);

          /*
           * Create a node representing a string.
           */
          AST_Expression::AST_ExprValue *ev = 0;

          if ((tao_yyvsp[(4) - (6)].exval) != 0)
            {
              ev = (tao_yyvsp[(4) - (6)].exval)->coerce (AST_Expression::EV_ulong);
            }

          if (0 == (tao_yyvsp[(4) - (6)].exval) || 0 == ev)
            {
              idl_global->err ()->coercion_error ((tao_yyvsp[(4) - (6)].exval),
                                                  AST_Expression::EV_ulong);
              (tao_yyval.dcval) = 0;
            }
          else
            {
              (tao_yyval.dcval) = idl_global->gen ()->create_string ((tao_yyvsp[(4) - (6)].exval));
              /*
               * Add this AST_String to the types defined in the global scope.
               */
              (void) idl_global->root ()->fe_add_string (
                                              AST_String::narrow_from_decl (
                                                  (tao_yyval.dcval)
                                                )
                                            );
            }

          delete ev;
          ev = 0;
        }
    break;

  case 293:

    {
//      | string_head
          idl_global->set_parse_state (IDL_GlobalData::PS_StringCompleted);
          /*
           * Create a node representing a string.
           */
          ACE_CDR::ULong bound = 0UL;

          (tao_yyval.dcval) =
            idl_global->gen ()->create_string (
                idl_global->gen ()->create_expr (bound,
                                                 AST_Expression::EV_ulong)
              );
          /*
           * Add this AST_String to the types defined in the global scope.
           */
          (void) idl_global->root ()->fe_add_string (
                                          AST_String::narrow_from_decl (
                                              (tao_yyval.dcval)
                                            )
                                        );
        }
    break;

  case 294:

    {
// string_head : IDL_STRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
    break;

  case 295:

    {
// wstring_type_spec : wstring_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
    break;

  case 296:

    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
    break;

  case 297:

    {
//      '>'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringQsSeen);

          /*
           * Create a node representing a string.
           */
          if ((tao_yyvsp[(4) - (6)].exval) == 0
              || (tao_yyvsp[(4) - (6)].exval)->coerce (AST_Expression::EV_ulong) == 0)
            {
              idl_global->err ()->coercion_error ((tao_yyvsp[(4) - (6)].exval),
                                                  AST_Expression::EV_ulong);
              (tao_yyval.dcval) = 0;
            }
          else
            {
              (tao_yyval.dcval) = idl_global->gen ()->create_wstring ((tao_yyvsp[(4) - (6)].exval));
              /*
               * Add this AST_String to the types defined in the global scope.
               */
              (void) idl_global->root ()->fe_add_string (
                                              AST_String::narrow_from_decl ((tao_yyval.dcval))
                                            );
            }
        }
    break;

  case 298:

    {
//      | wstring_head
          idl_global->set_parse_state (IDL_GlobalData::PS_StringCompleted);

          /*
           * Create a node representing a wstring.
           */
          ACE_CDR::ULong bound = 0UL;

          (tao_yyval.dcval) =
            idl_global->gen ()->create_wstring (
                idl_global->gen ()->create_expr (bound,
                                                 AST_Expression::EV_ulong)
              );
          /*
           * Add this AST_String to the types defined in the global scope.
           */
          (void) idl_global->root ()->fe_add_string (
                                          AST_String::narrow_from_decl ((tao_yyval.dcval))
                                        );
        }
    break;

  case 299:

    {
// wstring_head : IDL_WSTRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
    break;

  case 300:

    {
// array_declarator : id
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
    break;

  case 301:

    {
//      at_least_one_array_dim
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayCompleted);

          /*
           * Create a node representing an array.
           */
          if ((tao_yyvsp[(3) - (3)].elval) != 0)
            {
              UTL_ScopedName sn ((tao_yyvsp[(1) - (3)].idval),
                                 0);
              (tao_yyval.dcval) =
                idl_global->gen ()->create_array (
                                        &sn,
                                        (tao_yyvsp[(3) - (3)].elval)->length (),
                                        (tao_yyvsp[(3) - (3)].elval),
                                        0,
                                        0
                                      );

              (tao_yyvsp[(3) - (3)].elval)->destroy ();
              delete (tao_yyvsp[(3) - (3)].elval);
              (tao_yyvsp[(3) - (3)].elval) = 0;

              sn.destroy ();
            }
        }
    break;

  case 302:

    {
// at_least_one_array_dim : array_dim array_dims
          ACE_NEW_RETURN ((tao_yyval.elval),
                          UTL_ExprList ((tao_yyvsp[(1) - (2)].exval),
                                        (tao_yyvsp[(2) - (2)].elval)),
                          1);
        }
    break;

  case 303:

    {
// array_dims : array_dims array_dim
          if ((tao_yyvsp[(1) - (2)].elval) == 0)
            {
              ACE_NEW_RETURN ((tao_yyval.elval),
                              UTL_ExprList ((tao_yyvsp[(2) - (2)].exval),
                                            0),
                              1);
            }
          else
            {
              UTL_ExprList *el = 0;
              ACE_NEW_RETURN (el,
                              UTL_ExprList ((tao_yyvsp[(2) - (2)].exval),
                                            0),
                              1);
              (tao_yyvsp[(1) - (2)].elval)->nconc (el);
              (tao_yyval.elval) = (tao_yyvsp[(1) - (2)].elval);
            }
        }
    break;

  case 304:

    {
/*      |  EMPTY */
          (tao_yyval.elval) = 0;
        }
    break;

  case 305:

    {
// array_dim : '['
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
    break;

  case 306:

    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
    break;

  case 307:

    {
//      ']'
          idl_global->set_parse_state (IDL_GlobalData::PS_DimQsSeen);

          /*
           * Array dimensions are expressions which must be coerced to
           * positive integers.
           */
          AST_Expression::AST_ExprValue *ev = 0;

          if ((tao_yyvsp[(3) - (5)].exval) != 0)
            {
              ev = (tao_yyvsp[(3) - (5)].exval)->coerce (AST_Expression::EV_ulong);
            }

          if (0 == (tao_yyvsp[(3) - (5)].exval) || 0 == ev)
            {
              idl_global->err ()->coercion_error ((tao_yyvsp[(3) - (5)].exval),
                                                  AST_Expression::EV_ulong);
              (tao_yyval.exval) = 0;
            }
          else
            {
              (tao_yyval.exval) = (tao_yyvsp[(3) - (5)].exval);
            }

          delete ev;
          ev = 0;
        }
    break;

  case 310:

    {
// attribute_readonly : IDL_READONLY
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
    break;

  case 311:

    {
//      IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
    break;

  case 312:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
    break;

  case 313:

    {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
    break;

  case 314:

    {
//      opt_raises
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Attribute *a = 0;
          FE_Declarator *d = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          /*
           * Create nodes representing attributes and add them to the
           * enclosing scope.
           */
          if (s != 0
              && (tao_yyvsp[(5) - (9)].dcval) != 0
              && (tao_yyvsp[(7) - (9)].dlval) != 0)
            {
              for (UTL_DecllistActiveIterator l ((tao_yyvsp[(7) - (9)].dlval));
                   !l.is_done ();
                   l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type *tp = d->compose ((tao_yyvsp[(5) - (9)].dcval));

                  if (tp == 0)
                    {
                      continue;
                    }

                  a =
                    idl_global->gen ()->create_attribute (
                                            true,
                                            tp,
                                            d->name (),
                                            s->is_local (),
                                            s->is_abstract ()
                                          );

                  if ((tao_yyvsp[(9) - (9)].nlval) != 0)
                    {
                      (void) a->fe_add_get_exceptions ((tao_yyvsp[(9) - (9)].nlval));

                      (tao_yyvsp[(9) - (9)].nlval)->destroy ();
                      delete (tao_yyvsp[(9) - (9)].nlval);
                      (tao_yyvsp[(9) - (9)].nlval) = 0;
                    }

                  (void) s->fe_add_attribute (a);
                }
            }

          (tao_yyvsp[(7) - (9)].dlval)->destroy ();
          delete (tao_yyvsp[(7) - (9)].dlval);
          (tao_yyvsp[(7) - (9)].dlval) = 0;
        }
    break;

  case 315:

    {
// attribute_readonly : IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
    break;

  case 316:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
    break;

  case 317:

    {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
    break;

  case 318:

    {
//      opt_getraises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
    break;

  case 319:

    {
//      opt_setraises
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Attribute *a = 0;
          FE_Declarator *d = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseCompleted);

          /*
           * Create nodes representing attributes and add them to the
           * enclosing scope.
           */
          if (s != 0
              && (tao_yyvsp[(3) - (9)].dcval) != 0
              && (tao_yyvsp[(5) - (9)].dlval) != 0)
            {
              for (UTL_DecllistActiveIterator l ((tao_yyvsp[(5) - (9)].dlval));
                   !l.is_done ();
                   l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type *tp = d->compose ((tao_yyvsp[(3) - (9)].dcval));

                  if (tp == 0)
                    {
                      continue;
                    }

                  a =
                    idl_global->gen ()->create_attribute (
                                            false,
                                            tp,
                                            d->name (),
                                            s->is_local (),
                                            s->is_abstract ()
                                          );

                  if ((tao_yyvsp[(7) - (9)].nlval) != 0)
                    {
                      (void) a->fe_add_get_exceptions ((tao_yyvsp[(7) - (9)].nlval));

                      (tao_yyvsp[(7) - (9)].nlval)->destroy ();
                      delete (tao_yyvsp[(7) - (9)].nlval);
                      (tao_yyvsp[(7) - (9)].nlval) = 0;
                    }

                  if ((tao_yyvsp[(9) - (9)].nlval) != 0)
                    {
                      (void) a->fe_add_set_exceptions ((tao_yyvsp[(9) - (9)].nlval));

                      (tao_yyvsp[(9) - (9)].nlval)->destroy ();
                      delete (tao_yyvsp[(9) - (9)].nlval);
                      (tao_yyvsp[(9) - (9)].nlval) = 0;
                    }

                  (void) s->fe_add_attribute (a);
                }
            }

          (tao_yyvsp[(5) - (9)].dlval)->destroy ();
          delete (tao_yyvsp[(5) - (9)].dlval);
          (tao_yyvsp[(5) - (9)].dlval) = 0;
        }
    break;

  case 320:

    {
// exception : IDL_EXCEPTION
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
    break;

  case 321:

    {
//      id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((tao_yyvsp[(3) - (3)].idval),
                            0);
          AST_Exception *e = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptIDSeen);

          /*
           * Create a node representing an exception and add it to
           * the enclosing scope.
           */
          if (s != 0)
            {
              e = idl_global->gen ()->create_exception (&n,
                                                        s->is_local (),
                                                        s->is_abstract ());
              (void) s->fe_add_exception (e);
            }

          /*
           * Push the exception scope on the scope stack.
           */
          idl_global->scopes ().push (e);

          (tao_yyvsp[(3) - (3)].idval)->destroy ();
          delete (tao_yyvsp[(3) - (3)].idval);
          (tao_yyvsp[(3) - (3)].idval) = 0;
        }
    break;

  case 322:

    {
//       '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
    break;

  case 323:

    {
//      members
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
    break;

  case 324:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 325:

    {
// operation : opt_op_attribute op_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
    break;

  case 326:

    {
//      IDENTIFIER
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          Identifier id ((tao_yyvsp[(4) - (4)].strval));
          ACE::strdelete ((tao_yyvsp[(4) - (4)].strval));
          (tao_yyvsp[(4) - (4)].strval) = 0;

          UTL_ScopedName n (&id,
                            0);
          AST_Operation *o = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing an operation on an interface
           * and add it to its enclosing scope.
           */
          if (s != 0 && (tao_yyvsp[(2) - (4)].dcval) != 0)
            {
              AST_Type *tp =
                AST_Type::narrow_from_decl ((tao_yyvsp[(2) - (4)].dcval));

              if (tp == 0)
                {
                  idl_global->err ()->not_a_type ((tao_yyvsp[(2) - (4)].dcval));
                }
              else if (tp->node_type () == AST_Decl::NT_except)
                {
                  idl_global->err ()->not_a_type ((tao_yyvsp[(2) - (4)].dcval));
                }
              else
                {
                  AST_Decl *d = ScopeAsDecl (s);
                  AST_Decl::NodeType nt = d->node_type ();
                  bool local =
                    s->is_local ()
                    || nt == AST_Decl::NT_valuetype
                    || nt == AST_Decl::NT_eventtype;
                  o =
                    idl_global->gen ()->create_operation (tp,
                                                          (tao_yyvsp[(1) - (4)].ofval),
                                                          &n,
                                                          local,
                                                          s->is_abstract ());
                  (void) s->fe_add_operation (o);
                }
            }

          /*
           * Push the operation scope onto the scopes stack.
           */
          idl_global->scopes ().push (o);
        }
    break;

  case 327:

    {
//      parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 328:

    {
//      opt_raises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
    break;

  case 329:

    {
//      opt_context
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Operation *o = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpCompleted);

          /*
           * Add exceptions and context to the operation.
           */
          if (s != 0 && s->scope_node_type () == AST_Decl::NT_op)
            {
              o = AST_Operation::narrow_from_scope (s);

              if ((tao_yyvsp[(8) - (10)].nlval) != 0 && o != 0)
                {
                  (void) o->fe_add_exceptions ((tao_yyvsp[(8) - (10)].nlval));
                }

              if ((tao_yyvsp[(10) - (10)].slval) != 0)
                {
                  (void) o->fe_add_context ((tao_yyvsp[(10) - (10)].slval));
                }
            }

          /*
           * Done with this operation. Pop its scope from the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 330:

    {
// opt_op_attribute : IDL_ONEWAY
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (tao_yyval.ofval) = AST_Operation::OP_oneway;
        }
    break;

  case 331:

    {
//      | IDL_IDEMPOTENT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (tao_yyval.ofval) = AST_Operation::OP_idempotent;
        }
    break;

  case 332:

    {
/*      |  EMPTY */
          (tao_yyval.ofval) = AST_Operation::OP_noflags;
        }
    break;

  case 334:

    {
// op_type_spec : param_type_spec | IDL_VOID
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
    break;

  case 335:

    {
// init_decl : IDL_FACTORY
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
    break;

  case 336:

    {
//      IDENTIFIER
          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          if (s->is_abstract ())
            {
              //@@ Fire error
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("error in %s line %d:\n")
                          ACE_TEXT ("Abstract valuetype can't have a ")
                          ACE_TEXT ("factory construct.\n"),
                          idl_global->filename ()->get_string (),
                          idl_global->lineno ()));

              idl_global->set_err_count (idl_global->err_count () + 1);
            }

          Identifier id ((tao_yyvsp[(3) - (3)].strval));
          ACE::strdelete ((tao_yyvsp[(3) - (3)].strval));
          (tao_yyvsp[(3) - (3)].strval) = 0;

          UTL_ScopedName n (&id,
                            0);
          AST_Factory *factory = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing an factory construct
           * and add it to its enclosing scope
           */
          if (s != 0)
            {
              factory = idl_global->gen ()->create_factory (&n);
              (void) s->fe_add_factory (factory);
            }

          /*
           * Push the operation scope onto the scopes stack.
           */
          idl_global->scopes ().push (factory);
        }
    break;

  case 337:

    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 338:

    {
//      opt_raises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          if ((tao_yyvsp[(7) - (7)].nlval) != 0)
            {
              UTL_Scope *s = idl_global->scopes ().top_non_null ();
              AST_Factory *f = AST_Factory::narrow_from_scope (s);
              (void) f->fe_add_exceptions ((tao_yyvsp[(7) - (7)].nlval));
            }

          idl_global->scopes ().pop ();
        }
    break;

  case 339:

    {
// init_parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 340:

    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 341:

    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 342:

    {
//      at_least_one_in_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 344:

    {
// in_parameters : in_parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
    break;

  case 347:

    {
// in_parameter : IDL_IN
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
    break;

  case 348:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
    break;

  case 349:

    {
//      declarator
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Argument *a = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDeclSeen);

          /*
           * Create a node representing an argument to an operation
           * Add it to the enclosing scope (the operation scope).
           */
          if ((tao_yyvsp[(3) - (5)].dcval) != 0
              && (tao_yyvsp[(5) - (5)].deval) != 0 &&
              s != 0)
            {
              AST_Type *tp = (tao_yyvsp[(5) - (5)].deval)->compose ((tao_yyvsp[(3) - (5)].dcval));

              if (tp != 0)
                {
                  a = idl_global->gen ()->create_argument (
                          AST_Argument::dir_IN,
                          tp,
                          (tao_yyvsp[(5) - (5)].deval)->name ()
                        );

                  (void) s->fe_add_argument (a);
                }
            }

          (tao_yyvsp[(5) - (5)].deval)->destroy ();
          delete (tao_yyvsp[(5) - (5)].deval);
          (tao_yyvsp[(5) - (5)].deval) = 0;
        }
    break;

  case 350:

    {
// parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 351:

    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 352:

    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 353:

    {
//      at_least_one_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 355:

    {
// parameters : parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
    break;

  case 358:

    {
// parameter : direction
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
    break;

  case 359:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
    break;

  case 360:

    {
//      declarator
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Argument *a = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDeclSeen);

          /*
           * Create a node representing an argument to an operation
           * Add it to the enclosing scope (the operation scope).
           */
          if ((tao_yyvsp[(3) - (5)].dcval) != 0
              && (tao_yyvsp[(5) - (5)].deval) != 0
              && s != 0)
            {
              AST_Type *tp = (tao_yyvsp[(5) - (5)].deval)->compose ((tao_yyvsp[(3) - (5)].dcval));

              if (tp != 0)
                {
                  if (!s->is_local () && tp->is_local ())
                    {
                      idl_global->err ()->local_remote_mismatch (tp, s);
                    }
                  else
                    {
                      a =
                        idl_global->gen ()->create_argument (
                            (tao_yyvsp[(1) - (5)].dival),
                            tp,
                            (tao_yyvsp[(5) - (5)].deval)->name ()
                          );
                      (void) s->fe_add_argument (a);
                    }
                }
            }

          (tao_yyvsp[(5) - (5)].deval)->destroy ();
          delete (tao_yyvsp[(5) - (5)].deval);
          (tao_yyvsp[(5) - (5)].deval) = 0;
        }
    break;

  case 361:

    {
// param_type_spec : base_type_spec
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 364:

    {
//      | string_type_spec
//      | wstring_type_spec
//      | scoped_name
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = 0;

          if (s != 0)
            {
              d = s->lookup_by_name ((tao_yyvsp[(1) - (1)].idlist),
                                     true);
            }

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(1) - (1)].idlist));
            }
          else
            {
              d->last_referenced_as ((tao_yyvsp[(1) - (1)].idlist));
              AST_Decl::NodeType nt = d->node_type ();
              AST_Type *t = AST_Type::narrow_from_decl (d);
              AST_Typedef *td = 0;
              bool can_be_undefined = false;

              if (nt == AST_Decl::NT_struct_fwd
                  || nt == AST_Decl::NT_union_fwd
                  || nt == AST_Decl::NT_struct
                  || nt == AST_Decl::NT_union
                  || nt == AST_Decl::NT_typedef)
                {
                  // This code block ensures that a sequence of
                  // as-yet-undefined struct or union isn't used
                  // as a return type or argument.
                  if (nt == AST_Decl::NT_typedef)
                    {
                      td = AST_Typedef::narrow_from_decl (d);
                      AST_Type *pbt = td->primitive_base_type ();

                      if (pbt->node_type () == AST_Decl::NT_sequence)
                        {
                          t = pbt;
                          AST_Sequence *seq_type =
                            AST_Sequence::narrow_from_decl (pbt);
                          AST_Type *elem_type =
                            seq_type->base_type ();
                          AST_Decl::NodeType elem_nt =
                            elem_type->node_type ();

                          if (elem_nt == AST_Decl::NT_typedef)
                            {
                              AST_Typedef *elem_td =
                                AST_Typedef::narrow_from_decl (elem_type);
                              elem_type = elem_td->primitive_base_type ();
                              elem_nt = elem_type->node_type ();
                            }

                          if (elem_nt == AST_Decl::NT_interface
                              || elem_nt == AST_Decl::NT_interface_fwd
                              || elem_nt == AST_Decl::NT_valuetype
                              || elem_nt == AST_Decl::NT_valuetype_fwd
                              || elem_nt == AST_Decl::NT_component
                              || elem_nt == AST_Decl::NT_component_fwd)
                            {
                              can_be_undefined = true;
                            }
                        }
                    }

                  if (! t->is_defined () && ! can_be_undefined)
                    {
                      idl_global->err ()->error1 (
                        UTL_Error::EIDL_ILLEGAL_ADD,
                        (nt == AST_Decl::NT_typedef ? td : t));

                      /* If we don't return here, we'll crash later.*/
                      return 1;
                    }
                }
            }


          (tao_yyval.dcval) = d;
        }
    break;

  case 365:

    {
// direction : IDL_IN
          (tao_yyval.dival) = AST_Argument::dir_IN;
        }
    break;

  case 366:

    {
//      | IDL_OUT
          (tao_yyval.dival) = AST_Argument::dir_OUT;
        }
    break;

  case 367:

    {
//      | IDL_INOUT
          (tao_yyval.dival) = AST_Argument::dir_INOUT;
        }
    break;

  case 368:

    {
// opt_raises : IDL_RAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
    break;

  case 369:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
    break;

  case 370:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          (tao_yyval.nlval) = (tao_yyvsp[(5) - (6)].nlval);
        }
    break;

  case 371:

    {
          (tao_yyval.nlval) = 0;
/*      |  EMPTY */
        }
    break;

  case 372:

    {
// opt_getraises : IDL_GETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
    break;

  case 373:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
    break;

  case 374:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          (tao_yyval.nlval) = (tao_yyvsp[(5) - (6)].nlval);
        }
    break;

  case 375:

    {
          (tao_yyval.nlval) = 0;
/*      |  EMPTY */
        }
    break;

  case 376:

    {
// opt_setraises : IDL_SETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
    break;

  case 377:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
    break;

  case 378:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (tao_yyval.nlval) = (tao_yyvsp[(5) - (6)].nlval);
        }
    break;

  case 379:

    {
          (tao_yyval.nlval) = 0;
/*      |  EMPTY */
        }
    break;

  case 380:

    {
// opt_context : IDL_CONTEXT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
    break;

  case 381:

    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
//      '('
        }
    break;

  case 382:

    {
//      at_least_one_string_literal ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (tao_yyval.slval) = (tao_yyvsp[(5) - (6)].slval);
        }
    break;

  case 383:

    {
/*      |  EMPTY */
          (tao_yyval.slval) = 0;
        }
    break;

  case 384:

    {
// at_least_one_string_literal : IDL_STRING_LITERAL string_literals
          ACE_NEW_RETURN ((tao_yyval.slval),
                          UTL_StrList ((tao_yyvsp[(1) - (2)].sval),
                                       (tao_yyvsp[(2) - (2)].slval)),
                          1);
        }
    break;

  case 385:

    {
// string_literals : string_literals ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
    break;

  case 386:

    {
//      IDL_STRING_LITERAL
          if ((tao_yyvsp[(1) - (4)].slval) == 0)
            {
              ACE_NEW_RETURN ((tao_yyval.slval),
                              UTL_StrList ((tao_yyvsp[(4) - (4)].sval),
                                           0),
                              1);
            }
          else
            {
              UTL_StrList *sl = 0;
              ACE_NEW_RETURN (sl,
                              UTL_StrList ((tao_yyvsp[(4) - (4)].sval),
                                           0),
                              1);
              (tao_yyvsp[(1) - (4)].slval)->nconc (sl);
              (tao_yyval.slval) = (tao_yyvsp[(1) - (4)].slval);
            }
        }
    break;

  case 387:

    {
/*      |  EMPTY */
          (tao_yyval.slval) = 0;
        }
    break;

  case 388:

    {
// typeid_dcl : IDL_TYPEID scoped_name IDL_STRING_LITERAL
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist),
                                           true);

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));
            }
          else
            {
              d->set_id_with_typeid (
                     (tao_yyvsp[(3) - (3)].sval)->get_string ()
                   );
            }

          (tao_yyvsp[(2) - (3)].idlist)->destroy ();
          delete (tao_yyvsp[(2) - (3)].idlist);
          (tao_yyvsp[(2) - (3)].idlist) = 0;

          (tao_yyvsp[(3) - (3)].sval)->destroy ();
          delete (tao_yyvsp[(3) - (3)].sval);
          (tao_yyvsp[(3) - (3)].sval) = 0;
        }
    break;

  case 389:

    {
// typeprefix_dcl : IDL_TYPEPREFIX scoped_name IDL_STRING_LITERAL
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = ScopeAsDecl (s);

          // If we are in a module, we want to avoid a lookup unless the
          // typeprefix is to be applied to some other scope, since we
          // might get a previous opening of the module, and the prefix
          // of this opening would never get checked or set.
          if (d->name ()->compare ((tao_yyvsp[(2) - (3)].idlist)) != 0)
            {
              d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist),
                                     true);
            }

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));
            }
          else
            {
              d->set_prefix_with_typeprefix (
                     (tao_yyvsp[(3) - (3)].sval)->get_string ()
                   );
            }

          (tao_yyvsp[(2) - (3)].idlist)->destroy ();
          delete (tao_yyvsp[(2) - (3)].idlist);
          (tao_yyvsp[(2) - (3)].idlist) = 0;

          (tao_yyvsp[(3) - (3)].sval)->destroy ();
          delete (tao_yyvsp[(3) - (3)].sval);
          (tao_yyvsp[(3) - (3)].sval) = 0;
        }
    break;

  case 392:

    {
// component_forward_decl : IDL_COMPONENT id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((tao_yyvsp[(2) - (2)].idval),
                            0);
          AST_ComponentFwd *f = 0;
          idl_global->set_parse_state (
                          IDL_GlobalData::PS_ComponentForwardSeen
                        );

          /*
           * Create a node representing a forward declaration of a
           * component. Store it in the enclosing scope.
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_component_fwd (&n);
              (void) s->fe_add_component_fwd (f);
            }

          (tao_yyvsp[(2) - (2)].idval)->destroy ();
          delete (tao_yyvsp[(2) - (2)].idval);
          (tao_yyvsp[(2) - (2)].idval) = 0;
        }
    break;

  case 393:

    {
// component_decl : component_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Component *c = 0;

          /*
           * Make a new component node and add it to the enclosing scope.
           */
          if (s != 0 && (tao_yyvsp[(1) - (1)].chval) != 0)
            {
              c =
                idl_global->gen ()->create_component (
                                        (tao_yyvsp[(1) - (1)].chval)->name (),
                                        (tao_yyvsp[(1) - (1)].chval)->base_component (),
                                        (tao_yyvsp[(1) - (1)].chval)->supports (),
                                        (tao_yyvsp[(1) - (1)].chval)->n_supports (),
                                        (tao_yyvsp[(1) - (1)].chval)->supports_flat (),
                                        (tao_yyvsp[(1) - (1)].chval)->n_supports_flat ()
                                      );
              AST_Interface *i = AST_Interface::narrow_from_decl (c);
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the component to its definition scope.
               */
              c = AST_Component::narrow_from_decl (i);
              (void) s->fe_add_component (c);

              // This FE_ComponentHeader class isn't destroyed with the AST.
              (tao_yyvsp[(1) - (1)].chval)->destroy ();
              delete (tao_yyvsp[(1) - (1)].chval);
              (tao_yyvsp[(1) - (1)].chval) = 0;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (c);
        }
    break;

  case 394:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
    break;

  case 395:

    {
//      component_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
    break;

  case 396:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 397:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
    break;

  case 398:

    {
//      component_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 399:

    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);

          /*
           * Create an AST representation of the information in the header
           * part of a component.
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ((tao_yyvsp[(2) - (6)].idval), 0),
                          1);
          ACE_NEW_RETURN ((tao_yyval.chval),
                          FE_ComponentHeader (n,
                                              (tao_yyvsp[(4) - (6)].idlist),
                                              (tao_yyvsp[(6) - (6)].nlval),
                                              false),
                          1);

          if (0 != (tao_yyvsp[(6) - (6)].nlval))
            {
              (tao_yyvsp[(6) - (6)].nlval)->destroy ();
              delete (tao_yyvsp[(6) - (6)].nlval);
              (tao_yyvsp[(6) - (6)].nlval) = 0;
            }

          if (0 != (tao_yyvsp[(4) - (6)].idlist))
            {
              (tao_yyvsp[(4) - (6)].idlist)->destroy ();
              delete (tao_yyvsp[(4) - (6)].idlist);
              (tao_yyvsp[(4) - (6)].idlist) = 0;
            }
        }
    break;

  case 400:

    {
// component_inheritance_spec : ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
    break;

  case 401:

    {
//      scoped_name
          (tao_yyval.idlist) = (tao_yyvsp[(3) - (3)].idlist);
        }
    break;

  case 402:

    {
/*      |  EMPTY */
          (tao_yyval.idlist) = 0;
        }
    break;

  case 405:

    {
// component_export : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
    break;

  case 406:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 407:

    {
//      | uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
    break;

  case 408:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 409:

    {
//      | emits_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
        }
    break;

  case 410:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 411:

    {
//      | publishes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
        }
    break;

  case 412:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 413:

    {
//      | consumes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
        }
    break;

  case 414:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 415:

    {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
    break;

  case 416:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 417:

    {
// provides_decl : IDL_PROVIDES interface_type id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Component *c = AST_Component::narrow_from_scope (s);

          if (c != 0)
            {
              AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist),
                                               true);
              if (0 == d)
                {
                  idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));

                  (tao_yyvsp[(2) - (3)].idlist)->destroy ();
                  delete (tao_yyvsp[(2) - (3)].idlist);
                  (tao_yyvsp[(2) - (3)].idlist) = 0;

                  (tao_yyvsp[(3) - (3)].idval)->destroy ();
                  delete (tao_yyvsp[(3) - (3)].idval);
                  (tao_yyvsp[(3) - (3)].idval) = 0;

                  break;
                }
              else if (d->node_type () != AST_Decl::NT_interface)
                {
                  // Nothing else but CORBA::Object can have
                  // this identifier.
                  if (ACE_OS::strcmp (d->local_name ()->get_string (),
                                      "Object")
                        != 0)
                    {
                      idl_global->err ()->interface_expected (d);

                      (tao_yyvsp[(2) - (3)].idlist)->destroy ();
                      delete (tao_yyvsp[(2) - (3)].idlist);
                      (tao_yyvsp[(2) - (3)].idlist) = 0;

                      (tao_yyvsp[(3) - (3)].idval)->destroy ();
                      delete (tao_yyvsp[(3) - (3)].idval);
                      (tao_yyvsp[(3) - (3)].idval) = 0;

                      break;
                    }
                }

              AST_Type *interface_type =
                AST_Type::narrow_from_decl (d);

              AST_Component::port_description pd;

              // Strip off _cxx_, if any, for port name.
              idl_global->original_local_name ((tao_yyvsp[(3) - (3)].idval));

              pd.id = (tao_yyvsp[(3) - (3)].idval);
              pd.impl = interface_type;
              pd.line_number = idl_global->lineno ();
              c->provides ().enqueue_tail (pd);
            }

          (tao_yyvsp[(2) - (3)].idlist)->destroy ();
          delete (tao_yyvsp[(2) - (3)].idlist);
          (tao_yyvsp[(2) - (3)].idlist) = 0;
        }
    break;

  case 418:

    {
// interface_type : scoped_name
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (tao_yyval.idlist) = (tao_yyvsp[(1) - (1)].idlist);
        }
    break;

  case 419:

    {
//      | IDL_OBJECT
          Identifier *corba_id = 0;
          ACE_NEW_RETURN (corba_id,
                          Identifier ("Object"),
                          1);
          UTL_IdList *conc_name = 0;
          ACE_NEW_RETURN (conc_name,
                          UTL_IdList (corba_id,
                                      0),
                          1);
          ACE_NEW_RETURN (corba_id,
                          Identifier ("CORBA"),
                          1);
          UTL_IdList *corba_name = 0;
          ACE_NEW_RETURN (corba_name,
                          UTL_IdList (corba_id,
                                      conc_name),
                          1);
          (tao_yyval.idlist) = corba_name;
        }
    break;

  case 420:

    {
// uses_decl : IDL_USES opt_multiple interface_type id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(3) - (4)].idlist),
                                           true);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(3) - (4)].idlist));

              (tao_yyvsp[(3) - (4)].idlist)->destroy ();
              delete (tao_yyvsp[(3) - (4)].idlist);
              (tao_yyvsp[(3) - (4)].idlist) = 0;

              (tao_yyvsp[(4) - (4)].idval)->destroy ();
              delete (tao_yyvsp[(4) - (4)].idval);
              (tao_yyvsp[(4) - (4)].idval) = 0;

              break;
            }
          else if (d->node_type () != AST_Decl::NT_interface)
            {
              if (ACE_OS::strcmp (d->local_name ()->get_string (),
                                  "Object")
                    != 0)
                {
                  idl_global->err ()->interface_expected (d);

                  (tao_yyvsp[(3) - (4)].idlist)->destroy ();
                  delete (tao_yyvsp[(3) - (4)].idlist);
                  (tao_yyvsp[(3) - (4)].idlist) = 0;

                  (tao_yyvsp[(4) - (4)].idval)->destroy ();
                  delete (tao_yyvsp[(4) - (4)].idval);
                  (tao_yyvsp[(4) - (4)].idval) = 0;

                  break;
                }
            }

          AST_Type *interface_type = AST_Type::narrow_from_decl (d);
          AST_Component *c = AST_Component::narrow_from_scope (s);

          if (c != 0)
            {
              AST_Component::port_description ud;

              // Strip off _cxx_, if any, for port name.
              idl_global->original_local_name ((tao_yyvsp[(4) - (4)].idval));

              ud.id = (tao_yyvsp[(4) - (4)].idval);
              ud.impl = interface_type;
              ud.is_multiple = (tao_yyvsp[(2) - (4)].bval);
              ud.line_number = idl_global->lineno ();
              c->uses ().enqueue_tail (ud);

              if (ud.is_multiple == true
                  && !idl_global->using_ifr_backend ()
                  && !idl_global->ignore_idl3 ())
                {
                  // These datatypes must be created in the
                  // front end so they can be looked up
                  // when compiling the generated executor IDL.
                  idl_global->create_uses_multiple_stuff (c, ud);
                }
            }

          (tao_yyvsp[(3) - (4)].idlist)->destroy ();
          delete (tao_yyvsp[(3) - (4)].idlist);
          (tao_yyvsp[(3) - (4)].idlist) = 0;
        }
    break;

  case 421:

    {
// opt_multiple : IDL_MULTIPLE
          (tao_yyval.bval) = true;
        }
    break;

  case 422:

    {
/*      |  EMPTY */
          (tao_yyval.bval) = false;
        }
    break;

  case 423:

    {
// emits_decl : IDL_EMITS scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist),
                                           true);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));

              (tao_yyvsp[(2) - (3)].idlist)->destroy ();
              delete (tao_yyvsp[(2) - (3)].idlist);
              (tao_yyvsp[(2) - (3)].idlist) = 0;

              (tao_yyvsp[(3) - (3)].idval)->destroy ();
              delete (tao_yyvsp[(3) - (3)].idval);
              (tao_yyvsp[(3) - (3)].idval) = 0;

              break;
            }
          else if (d->node_type () != AST_Decl::NT_eventtype)
            {
              idl_global->err ()->eventtype_expected (d);

              (tao_yyvsp[(2) - (3)].idlist)->destroy ();
              delete (tao_yyvsp[(2) - (3)].idlist);
              (tao_yyvsp[(2) - (3)].idlist) = 0;

              (tao_yyvsp[(3) - (3)].idval)->destroy ();
              delete (tao_yyvsp[(3) - (3)].idval);
              (tao_yyvsp[(3) - (3)].idval) = 0;

              break;
            }
          else
            {
              AST_Type *event_type = AST_Type::narrow_from_decl (d);
              AST_Component *c = AST_Component::narrow_from_scope (s);

              if (c != 0)
                {
                  AST_Component::port_description pd;

                  // Strip off _cxx_, if any, for port name.
                  idl_global->original_local_name ((tao_yyvsp[(3) - (3)].idval));

                  pd.id = (tao_yyvsp[(3) - (3)].idval);
                  pd.impl = event_type;
                  pd.line_number = idl_global->lineno ();
                  c->emits ().enqueue_tail (pd);
                }
            }

          (tao_yyvsp[(2) - (3)].idlist)->destroy ();
          delete (tao_yyvsp[(2) - (3)].idlist);
          (tao_yyvsp[(2) - (3)].idlist) = 0;
        }
    break;

  case 424:

    {
// publishes_decl : IDL_PUBLISHES scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist),
                                           true);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));

              (tao_yyvsp[(2) - (3)].idlist)->destroy ();
              delete (tao_yyvsp[(2) - (3)].idlist);
              (tao_yyvsp[(2) - (3)].idlist) = 0;

              (tao_yyvsp[(3) - (3)].idval)->destroy ();
              delete (tao_yyvsp[(3) - (3)].idval);
              (tao_yyvsp[(3) - (3)].idval) = 0;

              break;
            }
          else if (d->node_type () != AST_Decl::NT_eventtype)
            {
              idl_global->err ()->eventtype_expected (d);

              (tao_yyvsp[(2) - (3)].idlist)->destroy ();
              delete (tao_yyvsp[(2) - (3)].idlist);
              (tao_yyvsp[(2) - (3)].idlist) = 0;

              (tao_yyvsp[(3) - (3)].idval)->destroy ();
              delete (tao_yyvsp[(3) - (3)].idval);
              (tao_yyvsp[(3) - (3)].idval) = 0;

              break;
            }
          else
            {
              AST_Type *event_type = AST_Type::narrow_from_decl (d);
              AST_Component *c = AST_Component::narrow_from_scope (s);

              if (c != 0)
                {
                  AST_Component::port_description pd;

                  // Strip off _cxx_, if any, for port name.
                  idl_global->original_local_name ((tao_yyvsp[(3) - (3)].idval));

                  pd.id = (tao_yyvsp[(3) - (3)].idval);
                  pd.impl = event_type;
                  pd.line_number = idl_global->lineno ();
                  c->publishes ().enqueue_tail (pd);
                }
            }

          (tao_yyvsp[(2) - (3)].idlist)->destroy ();
          delete (tao_yyvsp[(2) - (3)].idlist);
          (tao_yyvsp[(2) - (3)].idlist) = 0;
        }
    break;

  case 425:

    {
// consumes_decl : IDL_CONSUMES scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist),
                                           true);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));

              (tao_yyvsp[(2) - (3)].idlist)->destroy ();
              delete (tao_yyvsp[(2) - (3)].idlist);
              (tao_yyvsp[(2) - (3)].idlist) = 0;

              (tao_yyvsp[(3) - (3)].idval)->destroy ();
              delete (tao_yyvsp[(3) - (3)].idval);
              (tao_yyvsp[(3) - (3)].idval) = 0;

              break;
            }
          else if (d->node_type () != AST_Decl::NT_eventtype)
            {
              idl_global->err ()->eventtype_expected (d);

              (tao_yyvsp[(2) - (3)].idlist)->destroy ();
              delete (tao_yyvsp[(2) - (3)].idlist);
              (tao_yyvsp[(2) - (3)].idlist) = 0;

              (tao_yyvsp[(3) - (3)].idval)->destroy ();
              delete (tao_yyvsp[(3) - (3)].idval);
              (tao_yyvsp[(3) - (3)].idval) = 0;

              break;
            }
          else
            {
              AST_Type *event_type = AST_Type::narrow_from_decl (d);
              AST_Component *c = AST_Component::narrow_from_scope (s);

              if (c != 0)
                {
                  AST_Component::port_description pd;

                  // Strip off _cxx_, if any, for port name.
                  idl_global->original_local_name ((tao_yyvsp[(3) - (3)].idval));

                  pd.id = (tao_yyvsp[(3) - (3)].idval);
                  pd.impl = event_type;
                  pd.line_number = idl_global->lineno ();
                  c->consumes ().enqueue_tail (pd);
                }
            }

          (tao_yyvsp[(2) - (3)].idlist)->destroy ();
          delete (tao_yyvsp[(2) - (3)].idlist);
          (tao_yyvsp[(2) - (3)].idlist) = 0;
        }
    break;

  case 426:

    {
// home_decl : home_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Home *h = 0;

          /*
           * Make a new home node and add it to the enclosing scope.
           */
          if (s != 0 && (tao_yyvsp[(1) - (1)].hhval) != 0)
            {
              h =
                idl_global->gen ()->create_home (
                                        (tao_yyvsp[(1) - (1)].hhval)->name (),
                                        (tao_yyvsp[(1) - (1)].hhval)->base_home (),
                                        (tao_yyvsp[(1) - (1)].hhval)->managed_component (),
                                        (tao_yyvsp[(1) - (1)].hhval)->primary_key (),
                                        (tao_yyvsp[(1) - (1)].hhval)->supports (),
                                        (tao_yyvsp[(1) - (1)].hhval)->n_supports (),
                                        (tao_yyvsp[(1) - (1)].hhval)->supports_flat (),
                                        (tao_yyvsp[(1) - (1)].hhval)->n_supports_flat ()
                                      );
              /*
               * Add the home to its definition scope.
               */
              (void) s->fe_add_home (h);

              // This FE_HomeHeader class isn't destroyed with the AST.
              (tao_yyvsp[(1) - (1)].hhval)->destroy ();
              delete (tao_yyvsp[(1) - (1)].hhval);
              (tao_yyvsp[(1) - (1)].hhval) = 0;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (h);
        }
    break;

  case 427:

    {
//      home_body
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 428:

    {
// home_header : IDL_HOME
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
    break;

  case 429:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
    break;

  case 430:

    {
//      home_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 431:

    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
    break;

  case 432:

    {
//      IDL_MANAGES
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
    break;

  case 433:

    {
//      scoped_name
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
    break;

  case 434:

    {
//      primary_key_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_PrimaryKeySpecSeen);

          /*
           * Create an AST representation of the information in the header
           * part of a component home.
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ((tao_yyvsp[(3) - (13)].idval), 0),
                          1);
          ACE_NEW_RETURN ((tao_yyval.hhval),
                          FE_HomeHeader (n,
                                         (tao_yyvsp[(5) - (13)].idlist),
                                         (tao_yyvsp[(7) - (13)].nlval),
                                         (tao_yyvsp[(11) - (13)].idlist),
                                         (tao_yyvsp[(13) - (13)].idlist)),
                          1);

          (tao_yyvsp[(11) - (13)].idlist)->destroy ();
          delete (tao_yyvsp[(11) - (13)].idlist);
          (tao_yyvsp[(11) - (13)].idlist) = 0;

          if (0 != (tao_yyvsp[(5) - (13)].idlist))
            {
              (tao_yyvsp[(5) - (13)].idlist)->destroy ();
              delete (tao_yyvsp[(5) - (13)].idlist);
              (tao_yyvsp[(5) - (13)].idlist) = 0;
            }

          if (0 != (tao_yyvsp[(13) - (13)].idlist))
            {
              (tao_yyvsp[(13) - (13)].idlist)->destroy ();
              delete (tao_yyvsp[(13) - (13)].idlist);
              (tao_yyvsp[(13) - (13)].idlist) = 0;
            }

          if (0 != (tao_yyvsp[(7) - (13)].nlval))
            {
              (tao_yyvsp[(7) - (13)].nlval)->destroy ();
              delete (tao_yyvsp[(7) - (13)].nlval);
              (tao_yyvsp[(7) - (13)].nlval) = 0;
            }
        }
    break;

  case 435:

    {
// home_inheritance_spec ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
    break;

  case 436:

    {
//      scoped_name
          (tao_yyval.idlist) = (tao_yyvsp[(3) - (3)].idlist);
        }
    break;

  case 437:

    {
/*      |  EMPTY */
          (tao_yyval.idlist) = 0;
        }
    break;

  case 438:

    {
// primary_key_spec : IDL_PRIMARYKEY scoped_name
          (tao_yyval.idlist) = (tao_yyvsp[(2) - (2)].idlist);
        }
    break;

  case 439:

    {
/*      |  EMPTY */
          (tao_yyval.idlist) = 0;
        }
    break;

  case 440:

    {
// home_body : '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
    break;

  case 441:

    {
//      home_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
    break;

  case 442:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
    break;

  case 446:

    {
// home_export : factory_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
    break;

  case 447:

    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 448:

    {
//      | finder_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
    break;

  case 449:

    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 450:

    {
// factory_decl : IDL_FACTORY id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((tao_yyvsp[(2) - (2)].idval),
                            0);
          AST_Operation *o = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing a factory operation
           * and add it to the enclosing scope.
           */
          if (s != 0)
            {
              AST_Home *home = AST_Home::narrow_from_scope (s);
              o =
                idl_global->gen ()->create_operation (
                                        home->managed_component (),
                                        AST_Operation::OP_noflags,
                                        &n,
                                        false,
                                        false
                                      );
              home->factories ().enqueue_tail (o);
            }

          (tao_yyvsp[(2) - (2)].idval)->destroy ();
          delete (tao_yyvsp[(2) - (2)].idval);
          (tao_yyvsp[(2) - (2)].idval) = 0;

          /*
           * Push the operation scope onto the scopes stack.
           */
          idl_global->scopes ().push (o);
        }
    break;

  case 451:

    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 452:

    {
//      opt_raises
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Operation *o = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          /*
           * Add exceptions and context to the operation.
           */
          if (s != 0 && s->scope_node_type () == AST_Decl::NT_op)
            {
              o = AST_Operation::narrow_from_scope (s);

              if ((tao_yyvsp[(6) - (6)].nlval) != 0 && o != 0)
                {
                  (void) o->fe_add_exceptions ((tao_yyvsp[(6) - (6)].nlval));
                }
            }

          /*
           * Done with this operation. Pop its scope from the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 453:

    {
// finder_decl : IDL_FINDER id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((tao_yyvsp[(2) - (2)].idval),
                            0);
          AST_Operation *o = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing a finder operation
           * and add it to the enclosing scope.
           */
          if (s != 0)
            {
              AST_Home *home = AST_Home::narrow_from_scope (s);
              o =
                idl_global->gen ()->create_operation (
                                        home->managed_component (),
                                        AST_Operation::OP_noflags,
                                        &n,
                                        false,
                                        false
                                      );
              home->finders ().enqueue_tail (o);
            }

          (tao_yyvsp[(2) - (2)].idval)->destroy ();
          delete (tao_yyvsp[(2) - (2)].idval);
          (tao_yyvsp[(2) - (2)].idval) = 0;

          /*
           * Push the operation scope onto the scopes stack.
           */
          idl_global->scopes ().push (o);
        }
    break;

  case 454:

    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 455:

    {
//      opt_raises
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Operation *o = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          /*
           * Add exceptions and context to the operation.
           */
          if (s != 0 && s->scope_node_type () == AST_Decl::NT_op)
            {
              o = AST_Operation::narrow_from_scope (s);

              if ((tao_yyvsp[(6) - (6)].nlval) != 0 && o != 0)
                {
                  (void) o->fe_add_exceptions ((tao_yyvsp[(6) - (6)].nlval));
                }
            }

          /*
           * Done with this operation. Pop its scope from the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 461:

    {
// event_concrete_forward_decl : IDL_EVENTTYPE id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((tao_yyvsp[(2) - (2)].idval),
                            0);
          AST_EventTypeFwd *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeForwardSeen);

          /*
           * Create a node representing a forward declaration of an
           * eventtype. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_eventtype_fwd (&n,
                                                            false);
              (void) s->fe_add_valuetype_fwd (f);
            }

          (tao_yyvsp[(2) - (2)].idval)->destroy ();
          delete (tao_yyvsp[(2) - (2)].idval);
          (tao_yyvsp[(2) - (2)].idval) = 0;
        }
    break;

  case 462:

    {
// event_abs_forward_decl : IDL_ABSTRACT IDL_EVENTTYPE id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((tao_yyvsp[(3) - (3)].idval),
                            0);
          AST_EventTypeFwd *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeForwardSeen);

          /*
           * Create a node representing a forward declaration of an
           * eventtype. Store it in the enclosing scope
           */
          if (s != 0)
            {
              f = idl_global->gen ()->create_eventtype_fwd (&n,
                                                            true);
              (void) s->fe_add_valuetype_fwd (f);
            }

          (tao_yyvsp[(3) - (3)].idval)->destroy ();
          delete (tao_yyvsp[(3) - (3)].idval);
          (tao_yyvsp[(3) - (3)].idval) = 0;
        }
    break;

  case 463:

    {
// event_abs_decl : event_abs_header event_rest_of_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_EventType *e = 0;
          AST_Interface *i = 0;

          if (s != 0 && (tao_yyvsp[(1) - (2)].idval) != 0)
            {
              UTL_ScopedName sn ((tao_yyvsp[(1) - (2)].idval),
                                 0);
              e =
                idl_global->gen ()->create_eventtype (
                    &sn,
                    (tao_yyvsp[(2) - (2)].ehval)->inherits (),
                    (tao_yyvsp[(2) - (2)].ehval)->n_inherits (),
                    (tao_yyvsp[(2) - (2)].ehval)->inherits_concrete (),
                    (tao_yyvsp[(2) - (2)].ehval)->inherits_flat (),
                    (tao_yyvsp[(2) - (2)].ehval)->n_inherits_flat (),
                    (tao_yyvsp[(2) - (2)].ehval)->supports (),
                    (tao_yyvsp[(2) - (2)].ehval)->n_supports (),
                    (tao_yyvsp[(2) - (2)].ehval)->supports_concrete (),
                    true,
                    false,
                    false
                  );
              i = AST_Interface::narrow_from_decl (e);
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the eventetype to its definition scope
               */
              e = AST_EventType::narrow_from_decl (i);
              (void) s->fe_add_eventtype (e);
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (e);

          (tao_yyvsp[(1) - (2)].idval)->destroy ();
          delete (tao_yyvsp[(1) - (2)].idval);
          (tao_yyvsp[(1) - (2)].idval) = 0;
        }
    break;

  case 464:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
    break;

  case 465:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
    break;

  case 466:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 467:

    {
// event_abs_header : IDL_ABSTRACT IDL_EVENTTYPE id
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 468:

    {
// event_custom_header : IDL_CUSTOM IDL_EVENTTYPE id
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("error in %s line %d\n"),
                      idl_global->filename ()->get_string (),
                      idl_global->lineno ()));
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Sorry, I (TAO_IDL) can't handle")
                      ACE_TEXT (" custom yet\n")));
          (tao_yyval.idval) = 0;
        }
    break;

  case 469:

    {
// event_plain_header : IDL_EVENTTYPE id
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (tao_yyval.idval) = (tao_yyvsp[(2) - (2)].idval);
        }
    break;

  case 470:

    {
// event_rest_of_header : inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 471:

    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);

          ACE_NEW_RETURN ((tao_yyval.ehval),
                          FE_EventHeader (
                              0,
                              (tao_yyvsp[(1) - (3)].nlval),
                              (tao_yyvsp[(3) - (3)].nlval),
                              (tao_yyvsp[(1) - (3)].nlval)
                                ? (tao_yyvsp[(1) - (3)].nlval)->truncatable ()
                                : false
                            ),
                          1);

          if (0 != (tao_yyvsp[(3) - (3)].nlval))
            {
              (tao_yyvsp[(3) - (3)].nlval)->destroy ();
              delete (tao_yyvsp[(3) - (3)].nlval);
              (tao_yyvsp[(3) - (3)].nlval) = 0;
            }

          if (0 != (tao_yyvsp[(1) - (3)].nlval))
            {
              (tao_yyvsp[(1) - (3)].nlval)->destroy ();
              delete (tao_yyvsp[(1) - (3)].nlval);
              (tao_yyvsp[(1) - (3)].nlval) = 0;
            }
        }
    break;

  case 472:

    {
// event_decl : event_header event_rest_of_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_EventType *e = 0;
          AST_Interface *i = 0;

          if (s != 0 && (tao_yyvsp[(1) - (2)].idval) != 0)
            {
              // We create the scoped name here instead of with the
              // FE_EventHeader because there is a token for it only here.
              UTL_ScopedName sn ((tao_yyvsp[(1) - (2)].idval),
                                 0);
              e =
                idl_global->gen ()->create_eventtype (
                    &sn,
                    (tao_yyvsp[(2) - (2)].ehval)->inherits (),
                    (tao_yyvsp[(2) - (2)].ehval)->n_inherits (),
                    (tao_yyvsp[(2) - (2)].ehval)->inherits_concrete (),
                    (tao_yyvsp[(2) - (2)].ehval)->inherits_flat (),
                    (tao_yyvsp[(2) - (2)].ehval)->n_inherits_flat (),
                    (tao_yyvsp[(2) - (2)].ehval)->supports (),
                    (tao_yyvsp[(2) - (2)].ehval)->n_supports (),
                    (tao_yyvsp[(2) - (2)].ehval)->supports_concrete (),
                    false,
                    (tao_yyvsp[(2) - (2)].ehval)->truncatable (),
                    false
                  );
              i = AST_Interface::narrow_from_decl (e);
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the eventetype to its definition scope
               */
              e = AST_EventType::narrow_from_decl (i);
              (void) s->fe_add_eventtype (e);

              // FE_EventHeader is not automatically destroyed in the AST
              (tao_yyvsp[(2) - (2)].ehval)->destroy ();
              delete (tao_yyvsp[(2) - (2)].ehval);
              (tao_yyvsp[(2) - (2)].ehval) = 0;

              sn.destroy ();
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (e);
        }
    break;

  case 473:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
    break;

  case 474:

    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
    break;

  case 475:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 476:

    {
// event_header : event_custom_header
          (tao_yyval.idval) = (tao_yyvsp[(1) - (1)].idval);
        }
    break;

  case 477:

    {
// event_header : event_plain_header
          (tao_yyval.idval) = (tao_yyvsp[(1) - (1)].idval);
        }
    break;

  case 478:

    {
// type_classifier : IDL_TYPENAME
          (tao_yyval.ntval) = AST_Decl::NT_type;
ACE_DEBUG ((LM_DEBUG, "typename token\n"));
        }
    break;

  case 479:

    {
//        IDL_STRUCT
          (tao_yyval.ntval) = AST_Decl::NT_struct;
        }
    break;

  case 480:

    {
//        IDL_EVENTTYPE
          (tao_yyval.ntval) = AST_Decl::NT_eventtype;
        }
    break;

  case 481:

    {
//        IDL_PRIMITIVE
          (tao_yyval.ntval) = AST_Decl::NT_pre_defined;
        }
    break;

  case 482:

    {
//        IDL_FIXED
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("error in %C line %d:\n")
                      ACE_TEXT ("Fixed types not supported ")
                      ACE_TEXT ("in TAO.\n"),
                      idl_global->filename ()->get_string (),
                      idl_global->lineno ()));

          // Caught and handled later.
          (tao_yyval.ntval) = AST_Decl::NT_fixed;
        }
    break;

  case 483:

    {
//        IDL_SEQUENCE
          (tao_yyval.ntval) = AST_Decl::NT_sequence;
        }
    break;

  case 484:

    {
//        IDL_INTERFACE
          (tao_yyval.ntval) = AST_Decl::NT_interface;
        }
    break;

  case 485:

    {
//        IDL_VALUETYPE
          (tao_yyval.ntval) = AST_Decl::NT_valuetype;
        }
    break;

  case 486:

    {
// template_interface_def : template_interface_header
ACE_DEBUG ((LM_DEBUG, "template_interface_header\n"));
        }
    break;

  case 487:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
    break;

  case 488:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
    break;

  case 489:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          /*
           * Done with this interface - pop it off the scopes stack
           */
 //         idl_global->scopes ().pop ();
        }
    break;

  case 490:

    {
// template_interface_header : template_interface_decl
ACE_DEBUG ((LM_DEBUG, "template_interface_decl\n"));
        }
    break;

  case 491:

    {
//        template_inheritance_spec
ACE_DEBUG ((LM_DEBUG, "template_inheritance_spec\n"));
        }
    break;

  case 492:

    {
// template_interface_decl : interface_decl
ACE_DEBUG ((LM_DEBUG, "interface_decl\n"));
        }
    break;

  case 493:

    {
//        at_least_one_template_param
ACE_DEBUG ((LM_DEBUG, "at_least_one_template_param\n"));
        }
    break;

  case 494:

    {
// at_least_one_template_param : '<' template_param template_params '>'
ACE_DEBUG ((LM_DEBUG, "'<' template_param template_params '>'\n"));
        }
    break;

  case 495:

    {
// template_params : template_params ',' template_param
ACE_DEBUG ((LM_DEBUG, "template_params\n"));
        }
    break;

  case 496:

    {
//        /* EMPTY */
        }
    break;

  case 497:

    {
// template_param : type_classifier IDENTIFIER
ACE_DEBUG ((LM_DEBUG, "template_param\n"));
        }
    break;

  case 498:

    {
// template_inneritance_spec : ':' at_least_one_template_ref
        }
    break;

  case 499:

    {
//        /* EMPTY */
        }
    break;

  case 500:

    {
// at_least_one_template_ref : template_ref template_refs
        }
    break;

  case 501:

    {
// template_refs : template_refs ',' template_ref
        }
    break;

  case 502:

    {
//        /* EMPTY */
        }
    break;

  case 503:

    {
// template_ref : scoped_name '<' at_least_one_template_param_ref '>'
        }
    break;

  case 504:

    {
// at_least_one_template_param_ref : template_param_ref template_param_refs
        }
    break;

  case 505:

    {
// template_param_refs : template_param_refs ',' template_param_ref
        }
    break;

  case 506:

    {
//        /* EMPTY */
        }
    break;

  case 507:

    {
// template_param_ref : IDENTIFIER
        }
    break;


/* Line 1267 of yacc.c.  */

      default: break;
    }
  TAO_YY_SYMBOL_PRINT ("-> $$ =", tao_yyr1[tao_yyn], &tao_yyval, &tao_yyloc);

  TAO_YYPOPSTACK (tao_yylen);
  tao_yylen = 0;
  TAO_YY_STACK_PRINT (tao_yyss, tao_yyssp);

  *++tao_yyvsp = tao_yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  tao_yyn = tao_yyr1[tao_yyn];

  tao_yystate = tao_yypgoto[tao_yyn - TAO_YYNTOKENS] + *tao_yyssp;
  if (0 <= tao_yystate && tao_yystate <= TAO_YYLAST && tao_yycheck[tao_yystate] == *tao_yyssp)
    tao_yystate = tao_yytable[tao_yystate];
  else
    tao_yystate = tao_yydefgoto[tao_yyn - TAO_YYNTOKENS];

  goto tao_yynewstate;


/*------------------------------------.
| tao_yyerrlab -- here on detecting error |
`------------------------------------*/
tao_yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!tao_yyerrstatus)
    {
      ++tao_yynerrs;
#if ! TAO_YYERROR_VERBOSE
      tao_yyerror (TAO_YY_("syntax error"));
#else
      {
        TAO_YYSIZE_T tao_yysize = tao_yysyntax_error (0, tao_yystate, tao_yychar);
        if (tao_yymsg_alloc < tao_yysize && tao_yymsg_alloc < TAO_YYSTACK_ALLOC_MAXIMUM)
          {
            TAO_YYSIZE_T tao_yyalloc = 2 * tao_yysize;
            if (! (tao_yysize <= tao_yyalloc && tao_yyalloc <= TAO_YYSTACK_ALLOC_MAXIMUM))
              tao_yyalloc = TAO_YYSTACK_ALLOC_MAXIMUM;
            if (tao_yymsg != tao_yymsgbuf)
              TAO_YYSTACK_FREE (tao_yymsg);
            tao_yymsg = (char *) TAO_YYSTACK_ALLOC (tao_yyalloc);
            if (tao_yymsg)
              tao_yymsg_alloc = tao_yyalloc;
            else
              {
                tao_yymsg = tao_yymsgbuf;
                tao_yymsg_alloc = sizeof tao_yymsgbuf;
              }
          }

        if (0 < tao_yysize && tao_yysize <= tao_yymsg_alloc)
          {
            (void) tao_yysyntax_error (tao_yymsg, tao_yystate, tao_yychar);
            tao_yyerror (tao_yymsg);
          }
        else
          {
            tao_yyerror (TAO_YY_("syntax error"));
            if (tao_yysize != 0)
              goto tao_yyexhaustedlab;
          }
      }
#endif
    }



  if (tao_yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
         error, discard it.  */

      if (tao_yychar <= TAO_YYEOF)
        {
          /* Return failure if at end of input.  */
          if (tao_yychar == TAO_YYEOF)
            TAO_YYABORT;
        }
      else
        {
          tao_yydestruct ("Error: discarding",
                      tao_yytoken, &tao_yylval);
          tao_yychar = TAO_YYEMPTY;
        }
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto tao_yyerrlab1;


/*---------------------------------------------------.
| tao_yyerrorlab -- error raised explicitly by TAO_YYERROR.  |
`---------------------------------------------------*/
tao_yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     TAO_YYERROR and the label tao_yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto tao_yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this TAO_YYERROR.  */
  TAO_YYPOPSTACK (tao_yylen);
  tao_yylen = 0;
  TAO_YY_STACK_PRINT (tao_yyss, tao_yyssp);
  tao_yystate = *tao_yyssp;
  goto tao_yyerrlab1;


/*-------------------------------------------------------------.
| tao_yyerrlab1 -- common code for both syntax error and TAO_YYERROR.  |
`-------------------------------------------------------------*/
tao_yyerrlab1:
  tao_yyerrstatus = 3;        /* Each real token shifted decrements this.  */

  for (;;)
    {
      tao_yyn = tao_yypact[tao_yystate];
      if (tao_yyn != TAO_YYPACT_NINF)
        {
          tao_yyn += TAO_YYTERROR;
          if (0 <= tao_yyn && tao_yyn <= TAO_YYLAST && tao_yycheck[tao_yyn] == TAO_YYTERROR)
            {
              tao_yyn = tao_yytable[tao_yyn];
              if (0 < tao_yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (tao_yyssp == tao_yyss)
        TAO_YYABORT;


      tao_yydestruct ("Error: popping",
                  tao_yystos[tao_yystate], tao_yyvsp);
      TAO_YYPOPSTACK (1);
      tao_yystate = *tao_yyssp;
      TAO_YY_STACK_PRINT (tao_yyss, tao_yyssp);
    }

  if (tao_yyn == TAO_YYFINAL)
    TAO_YYACCEPT;

  *++tao_yyvsp = tao_yylval;


  /* Shift the error token.  */
  TAO_YY_SYMBOL_PRINT ("Shifting", tao_yystos[tao_yyn], tao_yyvsp, tao_yylsp);

  tao_yystate = tao_yyn;
  goto tao_yynewstate;


/*-------------------------------------.
| tao_yyacceptlab -- TAO_YYACCEPT comes here.  |
`-------------------------------------*/
tao_yyacceptlab:
  tao_yyresult = 0;
  goto tao_yyreturn;

/*-----------------------------------.
| tao_yyabortlab -- TAO_YYABORT comes here.  |
`-----------------------------------*/
tao_yyabortlab:
  tao_yyresult = 1;
  goto tao_yyreturn;

#ifndef tao_yyoverflow
/*-------------------------------------------------.
| tao_yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
tao_yyexhaustedlab:
  tao_yyerror (TAO_YY_("memory exhausted"));
  tao_yyresult = 2;
  /* Fall through.  */
#endif

tao_yyreturn:
  if (tao_yychar != TAO_YYEOF && tao_yychar != TAO_YYEMPTY)
     tao_yydestruct ("Cleanup: discarding lookahead",
                 tao_yytoken, &tao_yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this TAO_YYABORT or TAO_YYACCEPT.  */
  TAO_YYPOPSTACK (tao_yylen);
  TAO_YY_STACK_PRINT (tao_yyss, tao_yyssp);
  while (tao_yyssp != tao_yyss)
    {
      tao_yydestruct ("Cleanup: popping",
                  tao_yystos[*tao_yyssp], tao_yyvsp);
      TAO_YYPOPSTACK (1);
    }
#ifndef tao_yyoverflow
  if (tao_yyss != tao_yyssa)
    TAO_YYSTACK_FREE (tao_yyss);
#endif
#if TAO_YYERROR_VERBOSE
  if (tao_yymsg != tao_yymsgbuf)
    TAO_YYSTACK_FREE (tao_yymsg);
#endif
  /* Make sure TAO_YYID is used.  */
  return TAO_YYID (tao_yyresult);
}




/* programs */

/*
 * ???
 */
int
tao_yywrap (void)
{
  return 1;
}

/*
 * Report an error situation discovered in a production
 */
void
tao_yyerror (const char *msg)
{
  ACE_ERROR ((LM_ERROR,
              "%s\n",
              msg));
}

