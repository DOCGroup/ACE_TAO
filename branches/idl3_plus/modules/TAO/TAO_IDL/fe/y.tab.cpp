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
#include "ast_template_interface.h"
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
#include "fe_template_interface_header.h"
#include "fe_obv_header.h"
#include "fe_event_header.h"
#include "fe_component_header.h"
#include "fe_home_header.h"
#include "fe_utils.h"
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
  FE_Template_InterfaceHeader   *thval;         /* Template interface header */
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
  FE_Utils::T_Param_Info        *pival;         /* Template interface param */
  FE_Utils::T_PARAMLIST_INFO    *plval;         /* List of template params */
  FE_Utils::T_Ref_Info          *trval;         /* Template interface info */
  FE_Utils::T_REFLIST_INFO      *rlval;         /* List of above structs */
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
#define TAO_YYLAST   1293

/* TAO_YYNTOKENS -- Number of terminals.  */
#define TAO_YYNTOKENS  107
/* TAO_YYNNTS -- Number of nonterminals.  */
#define TAO_YYNNTS  356
/* TAO_YYNRULES -- Number of rules.  */
#define TAO_YYNRULES  529
/* TAO_YYNRULES -- Number of states.  */
#define TAO_YYNSTATES  753

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
      70,    74,    75,    79,    80,    81,    82,    83,    93,    95,
      97,    98,    99,   100,   108,   109,   113,   116,   120,   124,
     125,   130,   131,   133,   135,   137,   139,   142,   144,   145,
     146,   147,   155,   156,   157,   158,   167,   168,   173,   174,
     178,   180,   181,   184,   185,   188,   190,   193,   196,   197,
     199,   201,   204,   207,   210,   213,   214,   215,   219,   220,
     224,   225,   229,   230,   234,   235,   239,   240,   244,   245,
     249,   250,   254,   257,   258,   263,   264,   266,   267,   271,
     272,   277,   279,   281,   284,   287,   288,   289,   290,   291,
     301,   303,   305,   307,   309,   311,   313,   315,   317,   319,
     321,   323,   325,   329,   331,   335,   337,   341,   343,   347,
     351,   353,   357,   361,   363,   367,   371,   375,   377,   380,
     383,   386,   388,   390,   394,   396,   398,   400,   402,   404,
     406,   408,   410,   412,   414,   415,   419,   421,   423,   425,
     428,   430,   431,   435,   437,   439,   441,   443,   445,   447,
     449,   451,   453,   455,   457,   459,   461,   463,   465,   467,
     469,   471,   473,   475,   477,   480,   481,   486,   487,   489,
     491,   494,   495,   500,   501,   503,   505,   507,   509,   511,
     514,   516,   519,   523,   526,   528,   530,   533,   535,   537,
     539,   541,   543,   545,   547,   548,   552,   553,   554,   555,
     563,   566,   569,   570,   571,   574,   575,   576,   582,   583,
     587,   588,   592,   593,   594,   595,   596,   597,   598,   613,
     615,   617,   619,   621,   623,   625,   628,   631,   632,   633,
     634,   640,   641,   645,   648,   651,   652,   653,   657,   658,
     659,   665,   666,   670,   672,   674,   675,   676,   677,   678,
     688,   691,   692,   697,   698,   700,   701,   702,   709,   712,
     713,   714,   720,   721,   722,   729,   731,   733,   734,   735,
     742,   744,   746,   747,   751,   754,   757,   758,   759,   760,
     766,   768,   770,   771,   772,   773,   774,   784,   785,   786,
     787,   788,   798,   799,   800,   801,   802,   812,   813,   814,
     815,   816,   827,   829,   831,   832,   834,   836,   837,   838,
     839,   847,   848,   852,   853,   858,   861,   862,   867,   868,
     869,   870,   876,   877,   881,   882,   887,   890,   891,   896,
     897,   898,   899,   905,   907,   909,   911,   913,   915,   917,
     919,   920,   921,   928,   929,   930,   931,   938,   939,   940,
     941,   948,   949,   950,   951,   958,   959,   962,   963,   968,
     969,   973,   977,   979,   981,   984,   985,   986,   987,   995,
     996,   997,  1004,  1005,  1009,  1010,  1013,  1014,  1015,  1019,
    1020,  1024,  1025,  1029,  1030,  1034,  1035,  1039,  1040,  1044,
    1048,  1050,  1052,  1056,  1059,  1061,  1062,  1066,  1070,  1074,
    1075,  1079,  1080,  1081,  1082,  1083,  1084,  1085,  1099,  1100,
    1104,  1105,  1108,  1109,  1110,  1111,  1117,  1120,  1121,  1123,
    1124,  1128,  1129,  1133,  1134,  1135,  1142,  1143,  1144,  1151,
    1153,  1155,  1157,  1159,  1161,  1164,  1168,  1169,  1170,  1171,
    1180,  1184,  1188,  1191,  1192,  1196,  1197,  1198,  1199,  1208,
    1210,  1212,  1214,  1216,  1218,  1220,  1222,  1224,  1226,  1228,
    1229,  1230,  1231,  1239,  1243,  1244,  1245,  1252,  1253,  1258,
    1259,  1262,  1265,  1266,  1269,  1273,  1274,  1279,  1282,  1286,
    1287,  1289,  1290,  1291,  1292,  1293,  1294,  1306,  1308,  1309,
    1312,  1315,  1316,  1317,  1321,  1322,  1326,  1328,  1332,  1334
};

/* TAO_YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const tao_yytype_int16 tao_yyrhs[] =
{
     108,     0,    -1,   109,    -1,   109,   110,    -1,    -1,    -1,
     197,   111,    86,    -1,    -1,   358,   112,    86,    -1,    -1,
     359,   113,    86,    -1,    -1,   179,   114,    86,    -1,    -1,
     307,   115,    86,    -1,    -1,   130,   116,    86,    -1,    -1,
     431,   117,    86,    -1,    -1,   125,   118,    86,    -1,    -1,
     140,   119,    86,    -1,    -1,   360,   120,    86,    -1,    -1,
     387,   121,    86,    -1,    -1,   412,   122,    86,    -1,    -1,
     449,   123,    86,    -1,    -1,     1,   124,    86,    -1,    -1,
      -1,    -1,    -1,     5,   126,     3,   127,    87,   128,   109,
     129,    88,    -1,   131,    -1,   178,    -1,    -1,    -1,    -1,
     137,   132,    87,   133,   161,   134,    88,    -1,    -1,     6,
     136,   177,    -1,   135,   138,    -1,    41,   135,   138,    -1,
      42,   135,   138,    -1,    -1,    89,   154,   139,   171,    -1,
      -1,   141,    -1,   146,    -1,   156,    -1,   157,    -1,    43,
     142,    -1,   142,    -1,    -1,    -1,    -1,   150,   143,    87,
     144,   158,   145,    88,    -1,    -1,    -1,    -1,    42,   150,
     147,    87,   148,   161,   149,    88,    -1,    -1,   152,   138,
     151,   155,    -1,    -1,    49,   153,   177,    -1,    48,    -1,
      -1,    47,   171,    -1,    -1,    42,   152,    -1,   152,    -1,
     152,   201,    -1,   158,   159,    -1,    -1,   160,    -1,   162,
      -1,   319,    86,    -1,    46,   236,    -1,    45,   236,    -1,
     161,   162,    -1,    -1,    -1,   197,   163,    86,    -1,    -1,
     358,   164,    86,    -1,    -1,   359,   165,    86,    -1,    -1,
     179,   166,    86,    -1,    -1,   307,   167,    86,    -1,    -1,
     296,   168,    86,    -1,    -1,   312,   169,    86,    -1,    -1,
       1,   170,    86,    -1,   174,   172,    -1,    -1,   172,    90,
     173,   174,    -1,    -1,   177,    -1,    -1,    81,   175,   177,
      -1,    -1,   174,    81,   176,   177,    -1,     3,    -1,   135,
      -1,    41,   135,    -1,    42,   135,    -1,    -1,    -1,    -1,
      -1,     4,   180,   184,   181,   177,   182,    91,   183,   185,
      -1,   216,    -1,   221,    -1,   222,    -1,   223,    -1,   219,
      -1,   220,    -1,   281,    -1,   285,    -1,   174,    -1,   186,
      -1,   187,    -1,   188,    -1,   187,    92,   188,    -1,   189,
      -1,   188,    93,   189,    -1,   190,    -1,   189,    94,   190,
      -1,   191,    -1,   190,    82,   191,    -1,   190,    83,   191,
      -1,   192,    -1,   191,    95,   192,    -1,   191,    96,   192,
      -1,   193,    -1,   192,    97,   193,    -1,   192,    98,   193,
      -1,   192,    99,   193,    -1,   194,    -1,    95,   194,    -1,
      96,   194,    -1,   100,   194,    -1,   174,    -1,   195,    -1,
     101,   186,   102,    -1,    74,    -1,    75,    -1,    76,    -1,
      85,    -1,    77,    -1,    84,    -1,    78,    -1,    79,    -1,
      80,    -1,   186,    -1,    -1,     7,   198,   199,    -1,   228,
      -1,   242,    -1,   266,    -1,    40,   214,    -1,   206,    -1,
      -1,   201,   200,   207,    -1,   202,    -1,   205,    -1,   203,
      -1,   204,    -1,   174,    -1,   216,    -1,   219,    -1,   221,
      -1,   223,    -1,   222,    -1,   220,    -1,   224,    -1,   225,
      -1,   275,    -1,   281,    -1,   285,    -1,   228,    -1,   242,
      -1,   266,    -1,   264,    -1,   265,    -1,   210,   208,    -1,
      -1,   208,    90,   209,   210,    -1,    -1,   214,    -1,   215,
      -1,   214,   212,    -1,    -1,   212,    90,   213,   214,    -1,
      -1,   177,    -1,   289,    -1,   217,    -1,   218,    -1,     8,
      -1,     8,     8,    -1,     9,    -1,    10,     8,    -1,    10,
       8,     8,    -1,    10,     9,    -1,    11,    -1,    12,    -1,
       8,    11,    -1,    17,    -1,    13,    -1,    14,    -1,    15,
      -1,    16,    -1,    18,    -1,    19,    -1,    -1,    20,   227,
     177,    -1,    -1,    -1,    -1,   226,   229,    87,   230,   232,
     231,    88,    -1,   234,   233,    -1,   233,   234,    -1,    -1,
      -1,   235,   236,    -1,    -1,    -1,   201,   237,   207,   238,
      86,    -1,    -1,     1,   239,    86,    -1,    -1,    21,   241,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   240,    22,
     243,   101,   244,   249,   245,   102,   246,    87,   247,   250,
     248,    88,    -1,   216,    -1,   221,    -1,   222,    -1,   223,
      -1,   266,    -1,   174,    -1,   252,   251,    -1,   251,   252,
      -1,    -1,    -1,    -1,   256,   253,   262,   254,    86,    -1,
      -1,     1,   255,    86,    -1,   258,   257,    -1,   257,   258,
      -1,    -1,    -1,    29,   259,    89,    -1,    -1,    -1,    28,
     260,   186,   261,    89,    -1,    -1,   201,   263,   210,    -1,
     226,    -1,   240,    -1,    -1,    -1,    -1,    -1,    23,   267,
     177,   268,    87,   269,   271,   270,    88,    -1,   274,   272,
      -1,    -1,   272,    90,   273,   274,    -1,    -1,     3,    -1,
      -1,    -1,   278,    90,   276,   196,   277,   103,    -1,   278,
     103,    -1,    -1,    -1,    24,   279,   104,   280,   202,    -1,
      -1,    -1,   284,   104,   282,   196,   283,   103,    -1,   284,
      -1,    25,    -1,    -1,    -1,   288,   104,   286,   196,   287,
     103,    -1,   288,    -1,    26,    -1,    -1,   177,   290,   291,
      -1,   293,   292,    -1,   292,   293,    -1,    -1,    -1,    -1,
     105,   294,   196,   295,   106,    -1,   297,    -1,   302,    -1,
      -1,    -1,    -1,    -1,    30,   298,    31,   299,   341,   300,
     211,   301,   343,    -1,    -1,    -1,    -1,    -1,    31,   303,
     341,   304,   211,   305,   346,   306,   349,    -1,    -1,    -1,
      -1,    -1,    27,   308,   177,   309,    87,   310,   233,   311,
      88,    -1,    -1,    -1,    -1,    -1,   317,   318,   313,     3,
     314,   332,   315,   343,   316,   352,    -1,    32,    -1,    33,
      -1,    -1,   341,    -1,    34,    -1,    -1,    -1,    -1,    44,
     320,     3,   321,   323,   322,   343,    -1,    -1,   101,   324,
     102,    -1,    -1,   101,   325,   326,   102,    -1,   329,   327,
      -1,    -1,   327,    90,   328,   329,    -1,    -1,    -1,    -1,
      35,   330,   341,   331,   210,    -1,    -1,   101,   333,   102,
      -1,    -1,   101,   334,   335,   102,    -1,   338,   336,    -1,
      -1,   336,    90,   337,   338,    -1,    -1,    -1,    -1,   342,
     339,   341,   340,   210,    -1,   203,    -1,   281,    -1,   285,
      -1,   174,    -1,    35,    -1,    36,    -1,    37,    -1,    -1,
      -1,    38,   344,   101,   345,   171,   102,    -1,    -1,    -1,
      -1,    55,   347,   101,   348,   171,   102,    -1,    -1,    -1,
      -1,    62,   350,   101,   351,   171,   102,    -1,    -1,    -1,
      -1,    39,   353,   101,   354,   355,   102,    -1,    -1,    76,
     356,    -1,    -1,   356,    90,   357,    76,    -1,    -1,    63,
     174,    76,    -1,    64,   174,    76,    -1,   362,    -1,   361,
      -1,    50,   177,    -1,    -1,    -1,    -1,   366,   363,    87,
     364,   371,   365,    88,    -1,    -1,    -1,    50,   177,   367,
     369,   368,   155,    -1,    -1,    89,   370,   174,    -1,    -1,
     371,   372,    -1,    -1,    -1,   379,   373,    86,    -1,    -1,
     381,   374,    86,    -1,    -1,   384,   375,    86,    -1,    -1,
     385,   376,    86,    -1,    -1,   386,   377,    86,    -1,    -1,
     296,   378,    86,    -1,    60,   380,   177,    -1,   174,    -1,
      19,    -1,   382,   380,   177,    -1,    65,   383,    -1,    58,
      -1,    -1,    52,   174,   177,    -1,    61,   174,   177,    -1,
      51,   174,   177,    -1,    -1,   389,   388,   399,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,   390,   177,   391,   396,
     392,   155,   393,    66,   394,   174,   395,   398,    -1,    -1,
      89,   397,   174,    -1,    -1,    59,   174,    -1,    -1,    -1,
      -1,    87,   400,   402,   401,    88,    -1,   402,   403,    -1,
      -1,   162,    -1,    -1,   406,   404,    86,    -1,    -1,   409,
     405,    86,    -1,    -1,    -1,    44,   177,   407,   323,   408,
     343,    -1,    -1,    -1,    54,   177,   410,   323,   411,   343,
      -1,   425,    -1,   416,    -1,   413,    -1,   415,    -1,   414,
      -1,    53,   177,    -1,    42,    53,   177,    -1,    -1,    -1,
      -1,   420,   423,   417,    87,   418,   161,   419,    88,    -1,
      42,    53,   177,    -1,    43,    53,   177,    -1,    53,   177,
      -1,    -1,   138,   424,   155,    -1,    -1,    -1,    -1,   429,
     423,   426,    87,   427,   158,   428,    88,    -1,   421,    -1,
     422,    -1,    67,    -1,    20,    -1,    53,    -1,    68,    -1,
      17,    -1,    24,    -1,     6,    -1,    49,    -1,    -1,    -1,
      -1,   435,   432,    87,   433,   161,   434,    88,    -1,   135,
     436,   442,    -1,    -1,    -1,   104,   437,   441,   439,   438,
     103,    -1,    -1,   439,    90,   440,   441,    -1,    -1,   430,
       3,    -1,    89,   443,    -1,    -1,   445,   444,    -1,   444,
      90,   445,    -1,    -1,   174,   104,   446,   103,    -1,   448,
     447,    -1,   447,    90,   448,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    71,   450,     3,   451,   455,   452,
      87,   453,   456,   454,    88,    -1,   436,    -1,    -1,   458,
     457,    -1,   457,   458,    -1,    -1,    -1,   461,   459,    86,
      -1,    -1,   462,   460,    86,    -1,   379,    -1,    60,   445,
       3,    -1,   381,    -1,   382,   445,     3,    -1
};

/* TAO_YYRLINE[TAO_YYN] -- source line where rule number TAO_YYN was defined.  */
static const tao_yytype_uint16 tao_yyrline[] =
{
       0,   357,   357,   360,   361,   366,   365,   376,   375,   386,
     385,   396,   395,   406,   405,   416,   415,   426,   425,   436,
     435,   446,   445,   456,   455,   466,   465,   476,   475,   486,
     485,   496,   495,   509,   514,   540,   545,   508,   561,   562,
     567,   606,   611,   566,   629,   628,   642,   680,   711,   745,
     744,   756,   763,   764,   765,   766,   770,   781,   786,   830,
     835,   785,   864,   903,   908,   862,   927,   925,   965,   964,
     978,   984,   991,   998,  1005,  1031,  1058,  1123,  1124,  1128,
    1129,  1130,  1135,  1141,  1150,  1151,  1156,  1155,  1166,  1165,
    1176,  1175,  1186,  1185,  1196,  1195,  1206,  1205,  1216,  1215,
    1226,  1225,  1239,  1252,  1250,  1280,  1287,  1298,  1297,  1325,
    1323,  1350,  1362,  1408,  1436,  1467,  1472,  1477,  1482,  1466,
    1535,  1536,  1537,  1538,  1539,  1540,  1541,  1553,  1558,  1633,
    1635,  1637,  1638,  1652,  1653,  1667,  1668,  1681,  1682,  1692,
    1705,  1706,  1716,  1729,  1730,  1740,  1750,  1763,  1764,  1774,
    1784,  1797,  1832,  1833,  1842,  1847,  1854,  1862,  1867,  1872,
    1878,  1883,  1888,  1896,  1968,  1967,  1977,  1982,  1987,  1992,
    2019,  2028,  2027,  2098,  2099,  2103,  2111,  2112,  2140,  2141,
    2142,  2143,  2144,  2145,  2146,  2147,  2151,  2152,  2153,  2157,
    2158,  2159,  2163,  2164,  2168,  2181,  2179,  2209,  2216,  2217,
    2221,  2234,  2232,  2262,  2269,  2286,  2305,  2306,  2310,  2315,
    2320,  2328,  2333,  2338,  2346,  2351,  2356,  2364,  2378,  2383,
    2391,  2399,  2407,  2415,  2424,  2423,  2439,  2473,  2478,  2438,
    2497,  2500,  2501,  2505,  2505,  2515,  2520,  2514,  2583,  2582,
    2597,  2596,  2611,  2616,  2621,  2626,  2673,  2678,  2610,  2702,
    2710,  2724,  2734,  2742,  2743,  2851,  2854,  2855,  2860,  2865,
    2859,  2901,  2900,  2914,  2925,  2947,  2955,  2954,  2970,  2975,
    2969,  2992,  2991,  3044,  3068,  3093,  3098,  3131,  3136,  3092,
    3162,  3167,  3165,  3172,  3176,  3213,  3218,  3211,  3285,  3340,
    3350,  3339,  3365,  3370,  3363,  3411,  3437,  3447,  3452,  3445,
    3482,  3507,  3516,  3515,  3551,  3562,  3584,  3592,  3597,  3591,
    3634,  3635,  3640,  3645,  3650,  3655,  3639,  3724,  3729,  3734,
    3739,  3723,  3817,  3822,  3852,  3857,  3816,  3875,  3880,  3933,
    3938,  3873,  3975,  3981,  3988,  3995,  3996,  4008,  4014,  4056,
    4007,  4078,  4077,  4088,  4087,  4100,  4105,  4103,  4110,  4115,
    4120,  4114,  4161,  4160,  4171,  4170,  4183,  4188,  4186,  4193,
    4198,  4203,  4197,  4250,  4258,  4259,  4260,  4348,  4353,  4358,
    4367,  4372,  4366,  4384,  4392,  4397,  4391,  4409,  4417,  4422,
    4416,  4434,  4442,  4447,  4441,  4459,  4466,  4479,  4477,  4505,
    4512,  4541,  4579,  4580,  4584,  4614,  4654,  4659,  4613,  4678,
    4683,  4676,  4725,  4724,  4735,  4742,  4743,  4748,  4747,  4758,
    4757,  4768,  4767,  4778,  4777,  4788,  4787,  4798,  4797,  4809,
    4876,  4883,  4907,  4982,  4992,  4998,  5005,  5068,  5131,  5195,
    5194,  5244,  5249,  5254,  5259,  5264,  5269,  5243,  5323,  5322,
    5333,  5340,  5347,  5355,  5360,  5354,  5372,  5373,  5377,  5379,
    5378,  5389,  5388,  5403,  5439,  5401,  5473,  5509,  5471,  5541,
    5542,  5543,  5547,  5548,  5552,  5580,  5611,  5656,  5661,  5609,
    5678,  5688,  5707,  5719,  5718,  5758,  5808,  5813,  5756,  5830,
    5835,  5843,  5848,  5853,  5858,  5863,  5876,  5881,  5886,  5895,
    5917,  5922,  5894,  5939,  5957,  5962,  5956,  5985,  5984,  6006,
    6013,  6027,  6033,  6040,  6059,  6076,  6083,  6093,  6104,  6128,
    6135,  6146,  6151,  6156,  6160,  6165,  6145,  6177,  6182,  6188,
    6195,  6200,  6207,  6206,  6215,  6214,  6225,  6230,  6243,  6248
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
  "@5", "@6", "@7", "@8", "@9", "@10", "@11", "@12", "@13", "@14",
  "module", "@15", "@16", "@17", "@18", "interface_def", "interface",
  "@19", "@20", "@21", "interface_decl", "@22", "interface_header",
  "inheritance_spec", "@23", "value_def", "valuetype",
  "value_concrete_decl", "@24", "@25", "@26", "value_abs_decl", "@27",
  "@28", "@29", "value_header", "@30", "value_decl", "@31",
  "opt_truncatable", "supports_spec", "value_forward_decl",
  "value_box_decl", "value_elements", "value_element", "state_member",
  "exports", "export", "@32", "@33", "@34", "@35", "@36", "@37", "@38",
  "@39", "at_least_one_scoped_name", "scoped_names", "@40", "scoped_name",
  "@41", "@42", "id", "interface_forward", "const_dcl", "@43", "@44",
  "@45", "@46", "const_type", "expression", "const_expr", "or_expr",
  "xor_expr", "and_expr", "shift_expr", "add_expr", "mult_expr",
  "unary_expr", "primary_expr", "literal", "positive_int_expr", "type_dcl",
  "@47", "type_declarator", "@48", "type_spec", "simple_type_spec",
  "base_type_spec", "template_type_spec", "constructed_type_spec",
  "constructed_forward_type_spec", "at_least_one_declarator",
  "declarators", "@49", "declarator", "at_least_one_simple_declarator",
  "simple_declarators", "@50", "simple_declarator", "complex_declarator",
  "integer_type", "signed_int", "unsigned_int", "floating_pt_type",
  "fixed_type", "char_type", "octet_type", "boolean_type", "any_type",
  "object_type", "struct_decl", "@51", "struct_type", "@52", "@53", "@54",
  "at_least_one_member", "members", "member", "@55", "member_i", "@56",
  "@57", "@58", "union_decl", "@59", "union_type", "@60", "@61", "@62",
  "@63", "@64", "@65", "switch_type_spec", "at_least_one_case_branch",
  "case_branches", "case_branch", "@66", "@67", "@68",
  "at_least_one_case_label", "case_labels", "case_label", "@69", "@70",
  "@71", "element_spec", "@72", "struct_forward_type",
  "union_forward_type", "enum_type", "@73", "@74", "@75", "@76",
  "at_least_one_enumerator", "enumerators", "@77", "enumerator",
  "sequence_type_spec", "@78", "@79", "seq_head", "@80", "@81",
  "string_type_spec", "@82", "@83", "string_head", "wstring_type_spec",
  "@84", "@85", "wstring_head", "array_declarator", "@86",
  "at_least_one_array_dim", "array_dims", "array_dim", "@87", "@88",
  "attribute", "attribute_readonly", "@89", "@90", "@91", "@92",
  "attribute_readwrite", "@93", "@94", "@95", "@96", "exception", "@97",
  "@98", "@99", "@100", "operation", "@101", "@102", "@103", "@104",
  "opt_op_attribute", "op_type_spec", "init_decl", "@105", "@106", "@107",
  "init_parameter_list", "@108", "@109", "at_least_one_in_parameter",
  "in_parameters", "@110", "in_parameter", "@111", "@112",
  "parameter_list", "@113", "@114", "at_least_one_parameter", "parameters",
  "@115", "parameter", "@116", "@117", "param_type_spec", "direction",
  "opt_raises", "@118", "@119", "opt_getraises", "@120", "@121",
  "opt_setraises", "@122", "@123", "opt_context", "@124", "@125",
  "at_least_one_string_literal", "string_literals", "@126", "typeid_dcl",
  "typeprefix_dcl", "component", "component_forward_decl",
  "component_decl", "@127", "@128", "@129", "component_header", "@130",
  "@131", "component_inheritance_spec", "@132", "component_exports",
  "component_export", "@133", "@134", "@135", "@136", "@137", "@138",
  "provides_decl", "interface_type", "uses_decl", "uses_opt_multiple",
  "opt_multiple", "emits_decl", "publishes_decl", "consumes_decl",
  "home_decl", "@139", "home_header", "@140", "@141", "@142", "@143",
  "@144", "@145", "home_inheritance_spec", "@146", "primary_key_spec",
  "home_body", "@147", "@148", "home_exports", "home_export", "@149",
  "@150", "factory_decl", "@151", "@152", "finder_decl", "@153", "@154",
  "event", "event_forward_decl", "event_concrete_forward_decl",
  "event_abs_forward_decl", "event_abs_decl", "@155", "@156", "@157",
  "event_abs_header", "event_custom_header", "event_plain_header",
  "event_rest_of_header", "@158", "event_decl", "@159", "@160", "@161",
  "event_header", "type_classifier", "template_interface_def", "@162",
  "@163", "@164", "template_interface_header",
  "at_least_one_template_param", "@165", "@166", "template_params", "@167",
  "template_param", "template_inheritance_spec",
  "at_least_one_template_ref", "template_refs", "template_ref",
  "at_least_one_template_param_ref", "template_param_refs",
  "template_param_ref", "porttype_decl", "@168", "@169", "@170", "@171",
  "@172", "opt_template_params", "at_least_one_port_export",
  "port_exports", "port_export", "@173", "@174", "extended_provides_decl",
  "extended_uses_decl", 0
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
     110,   124,   110,   126,   127,   128,   129,   125,   130,   130,
     132,   133,   134,   131,   136,   135,   137,   137,   137,   139,
     138,   138,   140,   140,   140,   140,   141,   141,   143,   144,
     145,   142,   147,   148,   149,   146,   151,   150,   153,   152,
     154,   154,   155,   155,   156,   156,   157,   158,   158,   159,
     159,   159,   160,   160,   161,   161,   163,   162,   164,   162,
     165,   162,   166,   162,   167,   162,   168,   162,   169,   162,
     170,   162,   171,   173,   172,   172,   174,   175,   174,   176,
     174,   177,   178,   178,   178,   180,   181,   182,   183,   179,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   185,
     186,   187,   187,   188,   188,   189,   189,   190,   190,   190,
     191,   191,   191,   192,   192,   192,   192,   193,   193,   193,
     193,   194,   194,   194,   195,   195,   195,   195,   195,   195,
     195,   195,   195,   196,   198,   197,   197,   197,   197,   197,
     197,   200,   199,   201,   201,   202,   202,   202,   203,   203,
     203,   203,   203,   203,   203,   203,   204,   204,   204,   205,
     205,   205,   206,   206,   207,   209,   208,   208,   210,   210,
     211,   213,   212,   212,   214,   215,   216,   216,   217,   217,
     217,   218,   218,   218,   219,   219,   219,   220,   221,   221,
     222,   223,   224,   225,   227,   226,   229,   230,   231,   228,
     232,   233,   233,   235,   234,   237,   238,   236,   239,   236,
     241,   240,   243,   244,   245,   246,   247,   248,   242,   249,
     249,   249,   249,   249,   249,   250,   251,   251,   253,   254,
     252,   255,   252,   256,   257,   257,   259,   258,   260,   261,
     258,   263,   262,   264,   265,   267,   268,   269,   270,   266,
     271,   273,   272,   272,   274,   276,   277,   275,   275,   279,
     280,   278,   282,   283,   281,   281,   284,   286,   287,   285,
     285,   288,   290,   289,   291,   292,   292,   294,   295,   293,
     296,   296,   298,   299,   300,   301,   297,   303,   304,   305,
     306,   302,   308,   309,   310,   311,   307,   313,   314,   315,
     316,   312,   317,   317,   317,   318,   318,   320,   321,   322,
     319,   324,   323,   325,   323,   326,   328,   327,   327,   330,
     331,   329,   333,   332,   334,   332,   335,   337,   336,   336,
     339,   340,   338,   341,   341,   341,   341,   342,   342,   342,
     344,   345,   343,   343,   347,   348,   346,   346,   350,   351,
     349,   349,   353,   354,   352,   352,   355,   357,   356,   356,
     358,   359,   360,   360,   361,   363,   364,   365,   362,   367,
     368,   366,   370,   369,   369,   371,   371,   373,   372,   374,
     372,   375,   372,   376,   372,   377,   372,   378,   372,   379,
     380,   380,   381,   382,   383,   383,   384,   385,   386,   388,
     387,   390,   391,   392,   393,   394,   395,   389,   397,   396,
     396,   398,   398,   400,   401,   399,   402,   402,   403,   404,
     403,   405,   403,   407,   408,   406,   410,   411,   409,   412,
     412,   412,   413,   413,   414,   415,   417,   418,   419,   416,
     420,   421,   422,   424,   423,   426,   427,   428,   425,   429,
     429,   430,   430,   430,   430,   430,   430,   430,   430,   432,
     433,   434,   431,   435,   437,   438,   436,   440,   439,   439,
     441,   442,   442,   443,   444,   444,   445,   446,   447,   447,
     448,   450,   451,   452,   453,   454,   449,   455,   455,   456,
     457,   457,   459,   458,   460,   458,   461,   461,   462,   462
};

/* TAO_YYR2[TAO_YYN] -- Number of symbols composing right hand side of rule TAO_YYN.  */
static const tao_yytype_uint8 tao_yyr2[] =
{
       0,     2,     1,     2,     0,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     0,     0,     0,     9,     1,     1,
       0,     0,     0,     7,     0,     3,     2,     3,     3,     0,
       4,     0,     1,     1,     1,     1,     2,     1,     0,     0,
       0,     7,     0,     0,     0,     8,     0,     4,     0,     3,
       1,     0,     2,     0,     2,     1,     2,     2,     0,     1,
       1,     2,     2,     2,     2,     0,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     2,     0,     4,     0,     1,     0,     3,     0,
       4,     1,     1,     2,     2,     0,     0,     0,     0,     9,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     3,     1,     3,     1,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     1,     2,     2,
       2,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     3,     1,     1,     1,     2,
       1,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     0,     4,     0,     1,     1,
       2,     0,     4,     0,     1,     1,     1,     1,     1,     2,
       1,     2,     3,     2,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     0,     3,     0,     0,     0,     7,
       2,     2,     0,     0,     2,     0,     0,     5,     0,     3,
       0,     3,     0,     0,     0,     0,     0,     0,    14,     1,
       1,     1,     1,     1,     1,     2,     2,     0,     0,     0,
       5,     0,     3,     2,     2,     0,     0,     3,     0,     0,
       5,     0,     3,     1,     1,     0,     0,     0,     0,     9,
       2,     0,     4,     0,     1,     0,     0,     6,     2,     0,
       0,     5,     0,     0,     6,     1,     1,     0,     0,     6,
       1,     1,     0,     3,     2,     2,     0,     0,     0,     5,
       1,     1,     0,     0,     0,     0,     9,     0,     0,     0,
       0,     9,     0,     0,     0,     0,     9,     0,     0,     0,
       0,    10,     1,     1,     0,     1,     1,     0,     0,     0,
       7,     0,     3,     0,     4,     2,     0,     4,     0,     0,
       0,     5,     0,     3,     0,     4,     2,     0,     4,     0,
       0,     0,     5,     1,     1,     1,     1,     1,     1,     1,
       0,     0,     6,     0,     0,     0,     6,     0,     0,     0,
       6,     0,     0,     0,     6,     0,     2,     0,     4,     0,
       3,     3,     1,     1,     2,     0,     0,     0,     7,     0,
       0,     6,     0,     3,     0,     2,     0,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     3,
       1,     1,     3,     2,     1,     0,     3,     3,     3,     0,
       3,     0,     0,     0,     0,     0,     0,    13,     0,     3,
       0,     2,     0,     0,     0,     5,     2,     0,     1,     0,
       3,     0,     3,     0,     0,     6,     0,     0,     6,     1,
       1,     1,     1,     1,     2,     3,     0,     0,     0,     8,
       3,     3,     2,     0,     3,     0,     0,     0,     8,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       0,     0,     7,     3,     0,     0,     6,     0,     4,     0,
       2,     2,     0,     2,     3,     0,     4,     2,     3,     0,
       1,     0,     0,     0,     0,     0,    11,     1,     0,     2,
       2,     0,     0,     3,     0,     3,     1,     3,     1,     3
};

/* TAO_YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when TAO_YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const tao_yytype_uint16 tao_yydefact[] =
{
       4,     0,     0,     1,    31,   115,    33,    44,   164,   224,
     240,   275,   322,     0,     0,     0,     0,    68,     0,     0,
     431,     0,     0,   511,     3,    19,    15,    38,    51,    40,
      21,    52,    57,    53,    58,    51,    54,    55,    39,    11,
       5,   170,   226,   166,   274,   167,   192,   193,   168,    13,
       7,     9,    23,   393,   392,   395,    25,   429,    27,   461,
     463,   462,   460,    51,   479,   480,   459,    51,    17,   489,
      29,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     111,   204,   169,    51,     0,    51,    62,    51,     0,    56,
      51,     0,   399,   472,     0,   107,     0,   106,     0,     0,
       0,     0,    71,   494,    46,   502,     0,     0,     0,   208,
     210,     0,   214,   215,   218,   219,   220,   221,   217,   222,
     223,   289,   296,   301,    66,   177,    76,   173,   175,   176,
     174,   178,   206,   207,   179,   183,   180,   182,   181,   184,
     185,   226,   189,     0,   190,   191,   186,     0,   187,   295,
     188,   300,     0,     0,     0,   242,     0,     0,     0,     0,
       0,     0,     0,     0,   473,   466,   475,     0,     0,     0,
      32,   128,   116,   120,   124,   125,   121,   122,   123,   126,
     127,    34,    45,   165,   171,   225,   241,   276,   323,    47,
     470,    48,     0,   471,    69,   404,   432,     0,   390,   109,
     391,   512,    20,    16,    70,    49,     0,     0,   493,    41,
      22,    59,   209,   216,   211,   213,     0,    73,   285,   288,
     292,   297,    12,     6,   227,     0,    14,     8,    10,    24,
     396,    26,   443,   430,    28,    73,     0,     0,    18,   490,
      30,     0,     0,     0,     0,     0,    63,   402,   400,   440,
     108,     0,   518,     0,   487,   485,   482,   486,   488,   483,
     481,   484,     0,   499,     0,   501,   505,    85,    78,   212,
     290,     0,    67,     0,     0,     0,   233,   243,   406,   447,
     474,   467,   476,    85,   117,    35,   204,   172,   197,   198,
     199,   205,   277,   324,    85,     0,    73,   438,   433,   110,
     517,   513,    50,   105,   500,   495,     0,   503,     0,     0,
       0,    72,   154,   155,   156,   158,   160,   161,   162,   159,
     157,     0,     0,     0,     0,   151,   163,   130,   131,   133,
     135,   137,   140,   143,   147,   152,   286,   293,   298,   228,
     232,     0,     0,   397,     0,    85,    78,     0,     0,     4,
       0,   194,     0,   232,     0,   403,   401,     0,    73,     0,
     102,   497,     0,   510,     0,   509,     0,   100,   312,   317,
     332,   333,     0,    84,    92,    86,    96,   310,   311,    94,
      98,     0,    88,    90,   337,     0,     0,     0,    77,    79,
      80,     0,   291,   148,   149,   150,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   233,   238,   235,   234,   208,   254,   249,   250,   251,
     252,   244,   253,     0,     0,     0,     0,   425,   417,     0,
     405,   407,   409,     0,   411,   413,   415,     0,     0,   448,
       0,   446,   449,   451,     0,     0,     0,   118,     0,   307,
     303,   306,   195,   284,   278,   283,   233,     0,   439,   434,
     514,   103,     0,   496,   506,   507,   504,     0,     0,     0,
      43,     0,     0,     0,     0,     0,   336,   366,   363,   364,
     365,   327,   335,     0,     0,     0,    83,    82,    61,    81,
     153,   132,   134,   136,   138,   139,   141,   142,   144,   145,
     146,   287,   294,   299,   229,   231,     0,     0,     0,     0,
       0,   421,   420,     0,     0,   424,   423,     0,   398,     0,
       0,     0,     0,     0,     0,   453,   456,   445,     0,     0,
       0,     0,   492,     0,     0,     0,   304,     0,     0,   280,
       0,    65,     0,     0,     0,   498,     0,   101,   313,   318,
      93,    87,    97,    95,    99,     0,    89,    91,   338,   239,
     236,   245,   428,   426,   419,   427,   418,   408,   410,   422,
     412,   414,   416,     0,     0,   450,   452,   469,   478,   119,
     129,    37,   308,   305,   196,   279,   281,   326,   435,     0,
     526,   528,     0,   515,   521,   522,   524,   104,   508,     0,
       0,   328,     0,     0,     0,   341,   454,   457,     0,     0,
       0,   420,     0,     0,     0,   519,     0,     0,   314,   319,
     203,     0,   339,   237,   246,     0,     0,   373,   373,   309,
     282,   436,   527,   529,   516,   520,   523,   525,     0,   377,
     200,   354,   329,   373,     0,   342,   349,     0,   348,   370,
     455,   458,   442,   315,   374,   320,   201,     0,     0,   373,
     340,   261,   268,   266,   247,   257,   258,   265,     0,   344,
     345,     0,     0,   437,   373,     0,   381,     0,   353,   367,
     368,   369,     0,   359,   360,   330,     0,     0,     0,     0,
       0,     0,   263,   350,   346,   371,   441,   316,   375,   378,
     321,   202,   355,   356,     0,   385,   262,   269,   267,   248,
     256,   271,   259,   264,     0,     0,     0,     0,     0,   357,
     361,   382,   331,     0,     0,     0,   351,   347,     0,     0,
     379,     0,     0,     0,   270,   272,   260,   372,   376,     0,
     358,   362,   383,     0,     0,   380,   389,     0,   386,   384,
     387,     0,   388
};

/* TAO_YYDEFGOTO[NTERM-NUM].  */
static const tao_yytype_int16 tao_yydefgoto[] =
{
      -1,     1,     2,    24,   153,   157,   158,   152,   156,   101,
     167,   100,   107,   159,   161,   163,   169,    71,    25,    73,
     242,   349,   534,    26,    27,   106,   267,   372,    28,    74,
      29,   124,   253,    30,    31,    32,   108,   268,   387,    33,
     192,   294,   457,    34,   217,    35,    91,   205,   272,    36,
      37,   309,   388,   389,   308,   373,   472,   483,   484,   471,
     474,   473,   475,   467,   302,   360,   544,   325,   197,   251,
      97,    38,   374,    72,   241,   348,   533,   172,   579,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     375,    75,   183,   243,   413,   127,   128,   129,   130,    41,
     287,   351,   537,   288,   619,   640,   677,   289,   290,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,    42,
      76,    43,   154,   276,   410,   339,   411,   505,   341,   414,
     507,   603,   506,    44,    77,    45,   225,   342,   508,   604,
     644,   689,   421,   664,   690,   665,   691,   725,   686,   666,
     692,   667,   688,   687,   723,   712,   724,    46,    47,    48,
      78,   244,   352,   538,   454,   539,   609,   455,   146,   273,
     407,   147,   216,   310,   148,   274,   408,   149,   150,   275,
     409,   151,   291,   350,   450,   536,   451,   535,   608,   376,
     377,   468,   599,   638,   674,   378,   469,   600,   639,   676,
     379,    79,   245,   353,   540,   380,   555,   621,   659,   705,
     381,   481,   391,   485,   602,   643,   606,   625,   626,   647,
     670,   715,   648,   668,   714,   642,   657,   658,   682,   703,
     731,   683,   704,   732,   482,   684,   650,   671,   716,   655,
     675,   717,   700,   718,   739,   722,   733,   744,   747,   748,
     751,   382,   383,    52,    53,    54,   160,   278,   429,    55,
     195,   296,   248,   295,   343,   430,   519,   520,   522,   523,
     524,   517,   590,   513,   591,   592,   516,   434,   435,   436,
      56,   162,    57,    94,   249,   358,   542,   610,   652,   298,
     357,   673,   233,   279,   440,   344,   441,   528,   529,   442,
     573,   627,   443,   574,   628,    58,    59,    60,    61,    62,
     236,   345,   530,    63,    64,    65,   165,   235,    66,   237,
     346,   531,    67,   262,    68,   168,   283,   446,    69,   105,
     206,   362,   305,   462,   263,   208,   265,   307,   266,   364,
     465,   365,    70,    99,   252,   359,   543,   614,   301,   593,
     615,   594,   616,   617,   595,   596
};

/* TAO_YYPACT[STATE-NUM] -- Index in TAO_YYTABLE of the portion describing
   STATE-NUM.  */
#define TAO_YYPACT_NINF -530
static const tao_yytype_int16 tao_yypact[] =
{
    -530,    48,  1222,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,    34,    93,    90,    51,  -530,    34,    34,
    -530,    37,    37,  -530,  -530,  -530,  -530,  -530,    73,  -530,
    -530,  -530,  -530,  -530,  -530,   541,  -530,  -530,  -530,  -530,
    -530,  -530,    -2,  -530,   100,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,    60,  -530,  -530,  -530,    60,  -530,  -530,
    -530,    70,    77,   162,    34,  1195,    34,    34,    34,    34,
    -530,  -530,  -530,    12,    34,    19,  -530,    78,    34,  -530,
      60,    34,    82,    99,    34,  -530,   -38,  -530,    -9,   190,
     101,   127,   175,  -530,  -530,   153,   140,   157,   159,   155,
    -530,    68,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,   166,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,   100,  -530,  -530,  -530,   -41,  -530,   149,
    -530,   152,   164,   172,   173,  -530,   176,   177,   178,   180,
     174,   182,   183,   185,  -530,  -530,  -530,   187,   188,   191,
    -530,   166,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
     192,  -530,   194,  -530,  -530,   170,  -530,    34,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,   231,    37,  -530,  -530,
    -530,  -530,  -530,  -530,   261,  -530,   168,   227,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,   181,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,   227,   196,   198,  -530,  -530,
    -530,    34,   199,    34,   200,   203,  -530,  -530,  -530,   208,
    -530,    34,   197,    37,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,   276,  -530,   -45,  -530,  -530,  -530,  -530,  -530,
    -530,    37,  -530,    36,    36,    36,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,   195,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,    37,   227,  -530,  -530,  -530,
    -530,  -530,  -530,   166,  -530,   212,   286,   213,   960,   714,
     186,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,    49,    49,    49,    36,   166,  -530,   214,   211,   215,
      24,    80,    81,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  1171,   138,   184,   878,  -530,  -530,   995,   201,  -530,
     207,   217,   302,  -530,  1077,   166,  -530,    37,   227,   228,
     224,  -530,   216,  -530,   222,  -530,    37,  -530,  -530,  -530,
    -530,  -530,   229,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,   501,  -530,  -530,  -530,  1171,  1171,   230,  -530,  -530,
    -530,   240,  -530,  -530,  -530,  -530,   225,    36,    36,    36,
      36,    36,    36,    36,    36,    36,    36,   226,   232,   233,
     242,   243,  -530,  -530,  -530,   324,   166,  -530,  -530,  -530,
    -530,  -530,  -530,    37,    37,    38,    37,   275,  -530,   246,
    -530,  -530,  -530,    38,  -530,  -530,  -530,    34,    34,  -530,
     249,  -530,  -530,  -530,  1112,   796,   251,  -530,   370,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,   252,   253,   166,  -530,
    -530,  -530,   231,  -530,  -530,   255,  -530,   256,   312,   587,
    -530,   260,   262,   263,   264,   265,  -530,   166,  -530,  -530,
    -530,  -530,  -530,   266,   267,   353,  -530,  -530,  -530,  -530,
    -530,   211,   215,    24,    80,    80,    81,    81,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,   273,    34,   268,    57,
      57,  -530,   166,    34,    57,  -530,  -530,   277,  -530,   287,
     292,    34,   293,   294,   295,  -530,  -530,  -530,   296,   297,
     284,   298,  -530,    36,   299,    36,   207,    34,   300,   304,
     301,  -530,   318,    14,    37,  -530,   286,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,   389,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,   307,   307,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,    38,
    -530,  -530,    38,  -530,  -530,  -530,  -530,   166,  -530,   587,
      34,  -530,   307,   309,   311,   361,  -530,  -530,   315,   302,
      37,   -45,   396,   397,   313,    14,   316,   317,  -530,  -530,
    -530,   314,  -530,  -530,  -530,   320,   382,   380,   380,  -530,
    -530,   166,  -530,  -530,  -530,  -530,  -530,  -530,    34,   372,
     338,   327,  -530,   380,    41,  -530,  -530,   328,  -530,  -530,
    -530,  -530,   373,  -530,  -530,  -530,  -530,   329,   146,   380,
    -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,   587,  -530,
     345,   335,    37,  -530,   380,   336,   376,    34,  -530,  -530,
    -530,  -530,   337,  -530,  -530,  -530,   354,    36,   355,   357,
      54,  1195,   116,  -530,  -530,  -530,   166,  -530,  -530,  -530,
    -530,  -530,  -530,   352,   587,   404,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,    34,   382,    37,    37,   346,  -530,
    -530,  -530,  -530,   359,    34,   360,  -530,  -530,   347,   349,
    -530,   146,    34,   351,  -530,  -530,  -530,  -530,  -530,    37,
    -530,  -530,  -530,   358,   381,  -530,  -530,   362,   366,  -530,
    -530,   386,  -530
};

/* TAO_YYPGOTO[NTERM-NUM].  */
static const tao_yytype_int16 tao_yypgoto[] =
{
    -530,  -530,   118,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,   158,  -530,
    -530,   107,  -530,  -530,  -530,   449,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,   453,  -530,   202,  -530,  -530,  -201,  -530,
    -530,   123,  -530,  -530,  -248,  -276,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -263,  -530,  -530,   -19,  -530,  -530,
     -13,  -530,     7,  -530,  -530,  -530,  -530,  -530,  -530,  -302,
    -530,    74,    72,    75,  -175,  -170,  -197,  -101,  -530,  -247,
      11,  -530,  -530,  -530,   -24,   163,  -360,  -530,  -530,  -530,
     -35,  -530,  -530,  -508,  -163,  -530,  -530,   -12,  -530,   -49,
    -530,  -530,   405,   406,   -48,   -47,   -46,  -530,  -530,   -28,
    -530,   -25,  -530,  -530,  -530,  -530,   126,   204,  -530,  -145,
    -530,  -530,  -530,   -21,  -530,   -17,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -209,  -530,  -530,  -530,  -530,
    -530,  -210,  -530,  -530,  -530,  -530,  -530,  -530,  -530,   -31,
    -530,  -530,  -530,  -530,  -530,  -530,  -530,  -126,  -530,  -530,
    -530,  -530,  -530,  -530,   -60,  -530,  -530,  -530,   -53,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,   -52,  -530,  -530,   142,
    -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
      13,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,  -529,  -530,  -530,  -530,
    -530,  -530,  -229,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -244,  -530,  -530,  -439,  -530,  -488,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,    15,    18,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,   145,  -401,   147,   148,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,   422,  -530,  -530,  -530,
    -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,   241,
    -530,  -530,  -530,  -530,    30,  -530,  -530,  -530,  -335,  -530,
    -530,   -51,  -530,  -530,  -530,  -530,  -530,  -530,  -530,  -530,
    -530,  -117,  -530,  -530,  -530,  -530
};

/* TAO_YYTABLE[TAO_YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what TAO_YYDEFACT says.
   If TAO_YYTABLE_NINF, syntax error.  */
#define TAO_YYTABLE_NINF -492
static const tao_yytype_int16 tao_yytable[] =
{
      81,    82,    96,    98,   145,    92,    93,   141,   311,    39,
     142,   126,   179,    40,   143,    49,   125,    50,   144,   180,
      51,   478,   396,   173,   176,   177,   178,   337,   338,   584,
     549,   466,   521,   390,   280,   347,   199,    80,   198,    80,
      80,    80,   661,   199,   145,   607,   354,   141,     3,   218,
     142,   184,    80,   171,   143,   661,   125,   511,   144,   306,
      80,   182,   219,   185,   186,   187,   188,   200,   439,   662,
     663,   190,   199,   622,   589,   193,   214,   215,   194,   427,
      80,   196,   662,   663,  -273,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   356,     7,   444,  -113,     7,
      17,   102,   122,   123,    88,  -114,   400,   401,   102,   478,
     312,   313,   314,   315,   316,   317,   318,    95,    95,    95,
     319,   320,   155,   312,   313,   314,   315,   316,   317,   318,
      95,   321,   322,   319,   320,   104,   323,   324,   199,    17,
     651,    80,  -255,    84,   662,   663,   415,   110,   111,   102,
     324,   114,   115,   116,   117,   660,   170,   459,    95,  -112,
     618,    11,   102,   212,   -74,   181,   213,   102,  -394,   390,
     164,   685,    83,    85,   164,   402,   403,   103,   404,   405,
     406,   679,   680,   681,   250,  -464,   697,   202,   264,    80,
     189,   521,   191,   201,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   726,   498,   499,   500,
     121,   122,   123,   203,   368,   369,   735,    87,    90,    95,
     393,   394,   395,   204,   741,   494,   495,   209,   284,   693,
     286,   580,   496,   497,   303,   423,   424,   254,   299,   478,
     486,   487,   207,   210,   425,   426,   211,   199,   255,   427,
     222,   256,   303,   220,   612,   257,   221,   613,   223,   247,
     224,   230,   226,   227,   228,   720,   229,    95,   231,   269,
     232,   234,   270,   238,   271,   239,   355,   240,  -465,   304,
     258,   246,   277,   281,   259,   282,   285,   292,   582,   363,
     293,   125,   447,   417,   418,   419,   420,   297,   260,   261,
    -302,   103,   361,   366,   398,   453,   397,   452,   478,   399,
     145,   422,   449,   141,   461,   460,   142,   470,   488,   463,
     143,   479,   125,   416,   144,   464,   489,   490,   480,   501,
     504,  -230,   212,   515,   518,   502,   503,   527,   458,   532,
    -325,   541,   547,   548,   478,   546,   550,   264,   551,   552,
     553,   554,   556,   557,   145,   145,   558,   141,   141,   559,
     142,   142,   477,   566,   143,   143,   125,   125,   144,   144,
     561,     4,   577,   567,     5,     6,     7,     8,   568,   570,
     571,   572,   575,   576,   588,   707,   578,   581,   585,   587,
       9,    10,   601,    11,   586,   623,  -343,    12,   624,   632,
     633,   634,   636,   637,   509,   510,   512,   514,   605,   479,
      13,    14,    15,    16,   512,   641,   480,   646,   649,    17,
      18,   629,   645,    19,   525,   526,    20,   654,   656,  -352,
     669,   678,   672,    21,    22,   694,   695,   698,   699,   702,
     706,    23,   719,   721,   708,   709,   736,   730,   734,   737,
     477,   738,   742,   728,   729,    39,   750,   746,   -36,    40,
     745,    49,   752,    50,   749,    89,    51,   448,    86,   445,
     492,   491,   560,   392,   493,   653,   743,   174,   175,   456,
     340,   710,   713,   630,   583,   428,   727,   740,   431,   166,
     432,   433,   545,   300,   286,   598,   562,   563,   635,     0,
     564,   565,     0,     0,    80,     0,     0,     0,   569,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,     0,     0,     0,   286,   597,   122,   123,     0,     0,
       0,     0,     0,     0,     0,   476,     0,     0,     0,   479,
       0,     0,     0,     0,    80,     0,   480,     0,     0,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,     9,    10,     0,    11,   121,   122,   123,     0,     0,
     611,     0,     0,   611,     0,     0,     0,     0,     0,     0,
     477,     0,    95,     0,     0,     0,     0,    81,   620,     0,
      80,   631,     0,     0,     0,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,     0,   479,     0,
       0,     0,   122,   123,     0,   480,     0,     0,     0,     0,
       0,     0,    95,     0,     0,    81,   620,   -75,     0,     0,
     102,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   479,     0,     0,     0,     0,   477,
       0,   480,     0,   696,     0,     0,     0,     0,     0,     0,
     145,     0,     0,   141,    81,   701,   142,   711,    95,     0,
     143,     0,   125,     0,   144,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   477,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   303,   303,     0,
       0,   286,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   286,     0,     0,     0,   367,     0,  -334,     5,   286,
     303,     8,  -334,  -334,  -334,  -334,  -334,  -334,  -334,  -334,
    -334,  -334,  -334,  -334,     9,    10,     0,    11,     0,  -334,
    -334,    12,     0,     0,   368,   369,   370,   371,  -334,     0,
       0,     0,     0,     0,    13,     0,     0,     0,   384,   385,
     386,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,    22,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -334,     0,   367,     0,  -334,
       5,     0,   -60,     8,  -334,  -334,  -334,  -334,  -334,  -334,
    -334,  -334,  -334,  -334,  -334,  -334,     9,    10,     0,    11,
       0,  -334,  -334,    12,     0,     0,   368,   369,   370,   371,
    -334,     0,     0,     0,     0,     0,    13,     0,     0,     0,
     384,   385,   386,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -334,     0,   367,
       0,  -334,     5,     0,  -477,     8,  -334,  -334,  -334,  -334,
    -334,  -334,  -334,  -334,  -334,  -334,  -334,  -334,     9,    10,
       0,    11,     0,  -334,  -334,    12,     0,     0,   368,   369,
     370,   371,  -334,     0,     0,     0,     0,     0,    13,     0,
       0,     0,   437,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   438,     0,     0,     0,     0,     0,     0,     0,
       0,    21,    22,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -334,
       0,   367,     0,  -334,     5,     0,  -444,     8,  -334,  -334,
    -334,  -334,  -334,  -334,  -334,  -334,  -334,  -334,  -334,  -334,
       9,    10,     0,    11,     0,  -334,  -334,    12,     0,     0,
     368,   369,   370,   371,  -334,     0,   367,     0,  -334,     5,
      13,     0,     8,  -334,  -334,  -334,  -334,  -334,  -334,  -334,
    -334,  -334,  -334,  -334,  -334,     9,    10,     0,    11,     0,
    -334,  -334,    12,    21,    22,   368,   369,   370,   371,  -334,
       0,     0,     0,     0,     0,    13,     0,     0,     0,     0,
       0,  -334,     0,     0,     0,     0,     0,     0,   -42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    21,    22,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -334,     0,   367,     0,
    -334,     5,     0,  -491,     8,  -334,  -334,  -334,  -334,  -334,
    -334,  -334,  -334,  -334,  -334,  -334,  -334,     9,    10,     0,
      11,     0,  -334,  -334,    12,     0,     0,   368,   369,   370,
     371,  -334,     0,   367,     0,  -334,     5,    13,     0,     8,
    -334,  -334,  -334,  -334,  -334,  -334,  -334,  -334,  -334,  -334,
    -334,  -334,     9,    10,     0,    11,     0,  -334,  -334,    12,
      21,    22,   368,   369,   370,   371,  -334,     0,     0,     0,
       0,     0,    13,     0,     0,     0,     0,     0,  -334,     0,
       0,     0,     0,     0,     0,   -64,     0,     0,     0,     0,
       0,     0,   412,     0,    80,    21,    22,     0,     0,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,     9,    10,  -334,    11,   121,   122,   123,    80,     0,
    -468,     0,     0,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,     9,    10,     0,    11,   121,
     122,   123,    -2,     4,     0,     0,     5,     6,     7,     8,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     9,    10,     0,    11,     0,     0,     0,    12,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    13,    14,    15,    16,     0,     0,     0,     0,
       0,    17,    18,     0,     0,    19,    95,     0,    20,     0,
       0,     0,     0,     0,     0,    21,    22,     0,     0,     0,
       0,     0,     0,    23
};

static const tao_yytype_int16 tao_yycheck[] =
{
      13,    13,    21,    22,    35,    18,    19,    35,   271,     2,
      35,    35,    72,     2,    35,     2,    35,     2,    35,    72,
       2,   381,   324,    72,    72,    72,    72,   274,   275,   537,
     469,   366,   433,   309,   235,   283,    81,     3,    76,     3,
       3,     3,     1,    81,    75,   574,   294,    75,     0,    90,
      75,    75,     3,    72,    75,     1,    75,    19,    75,   104,
       3,    74,   103,    76,    77,    78,    79,    76,   344,    28,
      29,    84,    81,   602,    60,    88,     8,     9,    91,    65,
       3,    94,    28,    29,    86,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,   296,     6,   345,    86,     6,
      49,    89,    25,    26,    53,    86,    82,    83,    89,   469,
      74,    75,    76,    77,    78,    79,    80,    81,    81,    81,
      84,    85,    22,    74,    75,    76,    77,    78,    79,    80,
      81,    95,    96,    84,    85,    28,   100,   101,    81,    49,
     628,     3,    88,    53,    28,    29,     8,     9,    10,    89,
     101,    13,    14,    15,    16,   643,    86,   358,    81,    86,
     599,    23,    89,     8,    86,     3,    11,    89,    86,   445,
      63,   659,    14,    15,    67,    95,    96,   104,    97,    98,
      99,    35,    36,    37,   197,    86,   674,    86,   207,     3,
      83,   592,    85,     3,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,   714,   404,   405,   406,
      24,    25,    26,    86,    30,    31,   724,    15,    16,    81,
     321,   322,   323,    48,   732,   400,   401,    87,   241,   668,
     243,   533,   402,   403,   253,    51,    52,     6,   251,   599,
     385,   386,    89,    86,    60,    61,    87,    81,    17,    65,
      86,    20,   271,   104,   589,    24,   104,   592,    86,    89,
      87,    87,    86,    86,    86,   704,    86,    81,    86,     8,
      87,    86,   104,    86,    47,    87,   295,    86,    86,     3,
      49,    87,   101,    87,    53,    87,    87,    87,   535,     3,
      87,   310,    91,   342,   342,   342,   342,    89,    67,    68,
     105,   104,    90,    90,    93,     3,    92,    90,   668,    94,
     341,   342,   105,   341,    90,    87,   341,    88,    88,   103,
     341,   381,   341,   342,   341,   103,    86,   102,   381,   103,
      88,    88,     8,    58,    88,   103,   103,    88,   357,    88,
      88,    88,    86,    31,   704,    90,    86,   366,    86,    86,
      86,    86,    86,    86,   385,   386,     3,   385,   386,    86,
     385,   386,   381,    86,   385,   386,   385,   386,   385,   386,
     102,     1,    88,    86,     4,     5,     6,     7,    86,    86,
      86,    86,    86,    86,    66,   687,    88,    88,    88,    88,
      20,    21,     3,    23,    90,    86,    35,    27,    87,     3,
       3,    88,    86,    86,   423,   424,   425,   426,   101,   469,
      40,    41,    42,    43,   433,   101,   469,    35,    38,    49,
      50,   106,   102,    53,   437,   438,    56,    55,    90,   102,
     102,   102,    59,    63,    64,    90,   101,   101,    62,   102,
      86,    71,    90,    39,    89,    88,    86,   101,    89,   102,
     469,   102,   101,   716,   717,   448,    90,    76,    88,   448,
     102,   448,    76,   448,   102,    16,   448,   349,    15,   346,
     398,   397,   507,   310,   399,   638,   739,    72,    72,   353,
     276,   690,   692,   609,   536,   343,   715,   731,   343,    67,
     343,   343,   462,   252,   507,   546,   509,   510,   615,    -1,
     513,   514,    -1,    -1,     3,    -1,    -1,    -1,   521,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,   537,   544,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,   599,
      -1,    -1,    -1,    -1,     3,    -1,   599,    -1,    -1,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    24,    25,    26,    -1,    -1,
     589,    -1,    -1,   592,    -1,    -1,    -1,    -1,    -1,    -1,
     599,    -1,    81,    -1,    -1,    -1,    -1,   600,   600,    -1,
       3,   610,    -1,    -1,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    -1,   668,    -1,
      -1,    -1,    25,    26,    -1,   668,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,   638,   638,    86,    -1,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   704,    -1,    -1,    -1,    -1,   668,
      -1,   704,    -1,   672,    -1,    -1,    -1,    -1,    -1,    -1,
     691,    -1,    -1,   691,   677,   677,   691,   691,    81,    -1,
     691,    -1,   691,    -1,   691,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   704,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   716,   717,    -1,
      -1,   714,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   724,    -1,    -1,    -1,     1,    -1,     3,     4,   732,
     739,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    -1,    25,
      26,    27,    -1,    -1,    30,    31,    32,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    44,    45,
      46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    -1,     1,    -1,     3,
       4,    -1,    88,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      -1,    25,    26,    27,    -1,    -1,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,     1,
      -1,     3,     4,    -1,    88,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    23,    -1,    25,    26,    27,    -1,    -1,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      -1,     1,    -1,     3,     4,    -1,    88,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    23,    -1,    25,    26,    27,    -1,    -1,
      30,    31,    32,    33,    34,    -1,     1,    -1,     3,     4,
      40,    -1,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    23,    -1,
      25,    26,    27,    63,    64,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,     1,    -1,
       3,     4,    -1,    88,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      23,    -1,    25,    26,    27,    -1,    -1,    30,    31,    32,
      33,    34,    -1,     1,    -1,     3,     4,    40,    -1,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    23,    -1,    25,    26,    27,
      63,    64,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,    63,    64,    -1,    -1,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    81,    23,    24,    25,    26,     3,    -1,
      88,    -1,    -1,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    23,    24,
      25,    26,     0,     1,    -1,    -1,     4,     5,     6,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    23,    -1,    -1,    -1,    27,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    41,    42,    43,    -1,    -1,    -1,    -1,
      -1,    49,    50,    -1,    -1,    53,    81,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71
};

/* TAO_YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const tao_yytype_uint16 tao_yystos[] =
{
       0,   108,   109,     0,     1,     4,     5,     6,     7,    20,
      21,    23,    27,    40,    41,    42,    43,    49,    50,    53,
      56,    63,    64,    71,   110,   125,   130,   131,   135,   137,
     140,   141,   142,   146,   150,   152,   156,   157,   178,   179,
     197,   206,   226,   228,   240,   242,   264,   265,   266,   307,
     358,   359,   360,   361,   362,   366,   387,   389,   412,   413,
     414,   415,   416,   420,   421,   422,   425,   429,   431,   435,
     449,   124,   180,   126,   136,   198,   227,   241,   267,   308,
       3,   177,   214,   135,    53,   135,   150,   152,    53,   142,
     152,   153,   177,   177,   390,    81,   174,   177,   174,   450,
     118,   116,    89,   104,   138,   436,   132,   119,   143,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    24,    25,    26,   138,   174,   201,   202,   203,   204,
     205,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   228,   240,   242,   266,   275,   278,   281,   284,
     285,   288,   114,   111,   229,    22,   115,   112,   113,   120,
     363,   121,   388,   122,   138,   423,   423,   117,   432,   123,
      86,   174,   184,   216,   219,   220,   221,   222,   223,   281,
     285,     3,   177,   199,   201,   177,   177,   177,   177,   138,
     177,   138,   147,   177,   177,   367,   177,   175,    76,    81,
      76,     3,    86,    86,    48,   154,   437,    89,   442,    87,
      86,    87,     8,    11,     8,     9,   279,   151,    90,   103,
     104,   104,    86,    86,    87,   243,    86,    86,    86,    86,
      87,    86,    87,   399,    86,   424,   417,   426,    86,    87,
      86,   181,   127,   200,   268,   309,    87,    89,   369,   391,
     177,   176,   451,   139,     6,    17,    20,    24,    49,    53,
      67,    68,   430,   441,   174,   443,   445,   133,   144,     8,
     104,    47,   155,   276,   282,   286,   230,   101,   364,   400,
     155,    87,    87,   433,   177,    87,   177,   207,   210,   214,
     215,   289,    87,    87,   148,   370,   368,    89,   396,   177,
     436,   455,   171,   174,     3,   439,   104,   444,   161,   158,
     280,   171,    74,    75,    76,    77,    78,    79,    80,    84,
      85,    95,    96,   100,   101,   174,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   196,   196,   232,
     234,   235,   244,   371,   402,   418,   427,   161,   182,   128,
     290,   208,   269,   310,   161,   174,   155,   397,   392,   452,
     172,    90,   438,     3,   446,   448,    90,     1,    30,    31,
      32,    33,   134,   162,   179,   197,   296,   297,   302,   307,
     312,   317,   358,   359,    44,    45,    46,   145,   159,   160,
     162,   319,   202,   194,   194,   194,   186,    92,    93,    94,
      82,    83,    95,    96,    97,    98,    99,   277,   283,   287,
     231,   233,     1,   201,   236,     8,   174,   216,   221,   222,
     223,   249,   266,    51,    52,    60,    61,    65,   296,   365,
     372,   379,   381,   382,   384,   385,   386,    44,    54,   162,
     401,   403,   406,   409,   161,   158,   434,    91,   109,   105,
     291,   293,    90,     3,   271,   274,   233,   149,   174,   155,
      87,    90,   440,   103,   103,   447,   445,   170,   298,   303,
      88,   166,   163,   168,   167,   169,    34,   174,   203,   281,
     285,   318,   341,   164,   165,   320,   236,   236,    88,    86,
     102,   188,   189,   190,   191,   191,   192,   192,   193,   193,
     193,   103,   103,   103,    88,   234,   239,   237,   245,   174,
     174,    19,   174,   380,   174,    58,   383,   378,    88,   373,
     374,   380,   375,   376,   377,   177,   177,    88,   404,   405,
     419,   428,    88,   183,   129,   294,   292,   209,   270,   272,
     311,    88,   393,   453,   173,   441,    90,    86,    31,   341,
      86,    86,    86,    86,    86,   313,    86,    86,     3,    86,
     207,   102,   177,   177,   177,   177,    86,    86,    86,   177,
      86,    86,    86,   407,   410,    86,    86,    88,    88,   185,
     186,    88,   196,   293,   210,    88,    90,    88,    66,    60,
     379,   381,   382,   456,   458,   461,   462,   174,   448,   299,
     304,     3,   321,   238,   246,   101,   323,   323,   295,   273,
     394,   174,   445,   445,   454,   457,   459,   460,   341,   211,
     214,   314,   323,    86,    87,   324,   325,   408,   411,   106,
     274,   174,     3,     3,    88,   458,    86,    86,   300,   305,
     212,   101,   332,   322,   247,   102,    35,   326,   329,    38,
     343,   343,   395,   211,    55,   346,    90,   333,   334,   315,
     343,     1,    28,    29,   250,   252,   256,   258,   330,   102,
     327,   344,    59,   398,   301,   347,   306,   213,   102,    35,
      36,    37,   335,   338,   342,   343,   255,   260,   259,   248,
     251,   253,   257,   341,    90,   101,   174,   343,   101,    62,
     349,   214,   102,   336,   339,   316,    86,   186,    89,    88,
     252,   201,   262,   258,   331,   328,   345,   348,   350,    90,
     341,    39,   352,   261,   263,   254,   210,   329,   171,   171,
     101,   337,   340,   353,    89,   210,    86,   102,   102,   351,
     338,   210,   101,   171,   354,   102,    76,   355,   356,   102,
      90,   357,    76
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
//      | porttype_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeDeclSeen);
        }
    break;

  case 30:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 31:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
    break;

  case 32:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 33:

    {
// module  : IDL_MODULE
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
          }
    break;

  case 34:

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

  case 35:

    {
//        '{'
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
          }
    break;

  case 36:

    {
//        definitions
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
          }
    break;

  case 37:

    {
//        '}'
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
            /*
             * Finished with this module - pop it from the scope stack.
             */
            idl_global->scopes ().pop ();
          }
    break;

  case 40:

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

  case 41:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
    break;

  case 42:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
    break;

  case 43:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 44:

    {
// interface_decl : IDL_INTERFACE
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
    break;

  case 45:

    {
//       id
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
         }
    break;

  case 46:

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

  case 47:

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

  case 48:

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

  case 49:

    {
// inheritance_spec : ':' opt_truncatable
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
    break;

  case 50:

    {
//      at_least_one_scoped_name
          (tao_yyvsp[(4) - (4)].nlval)->truncatable ((tao_yyvsp[(2) - (4)].bval));
          (tao_yyval.nlval) = (tao_yyvsp[(4) - (4)].nlval);
        }
    break;

  case 51:

    {
/*      |  EMPTY */
          (tao_yyval.nlval) = 0;
        }
    break;

  case 56:

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

  case 58:

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

  case 59:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
    break;

  case 60:

    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
    break;

  case 61:

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

  case 62:

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

  case 63:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
    break;

  case 64:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
    break;

  case 65:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 66:

    {
// value_header : value_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 67:

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

  case 68:

    {
// value_decl : IDL_VALUETYPE
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
    break;

  case 69:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 70:

    {
// opt_truncatable : IDL_TRUNCATABLE
          (tao_yyval.bval) = true;
        }
    break;

  case 71:

    {
/*      |  EMPTY */
          (tao_yyval.bval) = false;
        }
    break;

  case 72:

    {
// supports_spec : IDL_SUPPORTS at_least_one_scoped_name
          (tao_yyval.nlval) = (tao_yyvsp[(2) - (2)].nlval);
        }
    break;

  case 73:

    {
/*      |    EMPTY */
          (tao_yyval.nlval) = 0;
        }
    break;

  case 74:

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

  case 75:

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

  case 76:

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

  case 82:

    {
// state_member : IDL_PUBLIC
          /* is $0 to member_i */
          (tao_yyval.vival) = AST_Field::vis_PUBLIC;
        }
    break;

  case 83:

    {
//      IDL_PRIVATE
          /* is $0 to member_i */
          (tao_yyval.vival) = AST_Field::vis_PRIVATE;
        }
    break;

  case 86:

    {
// export : type_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
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
//      | typeid_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
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
//      | typeprefix_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
    break;

  case 91:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 92:

    {
//      | const_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
    break;

  case 93:

    {
//      ';'
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
    break;

  case 94:

    {
//      | exception
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
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
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
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
//      | operation
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
    break;

  case 99:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 100:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
    break;

  case 101:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 102:

    {
// at_least_one_scoped_name : scoped_name scoped_names
          ACE_NEW_RETURN ((tao_yyval.nlval),
                          UTL_NameList ((tao_yyvsp[(1) - (2)].idlist),
                                        (tao_yyvsp[(2) - (2)].nlval)),
                          1);
        }
    break;

  case 103:

    {
// scoped_names : scoped_names ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
    break;

  case 104:

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

  case 105:

    {
/*      |  EMPTY */
          (tao_yyval.nlval) = 0;
        }
    break;

  case 106:

    {
// scoped_name : id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN ((tao_yyval.idlist),
                          UTL_IdList ((tao_yyvsp[(1) - (1)].idval),
                                      0),
                          1);
        }
    break;

  case 107:

    {
//      | IDL_SCOPE_DELIMITOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
    break;

  case 108:

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

  case 109:

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

  case 110:

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

  case 111:

    {
// id: IDENTIFIER
          ACE_NEW_RETURN ((tao_yyval.idval),
                          Identifier ((tao_yyvsp[(1) - (1)].strval)),
                          1);
          ACE::strdelete ((tao_yyvsp[(1) - (1)].strval));
          (tao_yyvsp[(1) - (1)].strval) = 0;
        }
    break;

  case 112:

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

  case 113:

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

  case 114:

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

  case 115:

    {
// const_dcl : IDL_CONST
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
    break;

  case 116:

    {
//      const_type
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
    break;

  case 117:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
    break;

  case 118:

    {
//      '='
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
    break;

  case 119:

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

  case 126:

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

  case 127:

    {
//      | wstring_type_spec
          (tao_yyval.etval) = AST_Expression::EV_wstring;
        }
    break;

  case 128:

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

  case 132:

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

  case 134:

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

  case 136:

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

  case 138:

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

  case 139:

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

  case 141:

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

  case 142:

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

  case 144:

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

  case 145:

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

  case 146:

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

  case 148:

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

  case 149:

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

  case 150:

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

  case 151:

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

  case 153:

    {
//      | literal
//      | '(' const_expr ')'
          (tao_yyval.exval) = (tao_yyvsp[(2) - (3)].exval);
        }
    break;

  case 154:

    {
// literal : IDL_INTEGER_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].ival));
        }
    break;

  case 155:

    {
//      | IDL_UINTEGER_LITERAL
          (tao_yyval.exval) =
            idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].uival),
                                             AST_Expression::EV_ulonglong);
        }
    break;

  case 156:

    {
//      | IDL_STRING_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].sval));
          (tao_yyvsp[(1) - (1)].sval)->destroy ();
          delete (tao_yyvsp[(1) - (1)].sval);
          (tao_yyvsp[(1) - (1)].sval) = 0;
        }
    break;

  case 157:

    {
//      | IDL_WSTRING_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].wsval));
        }
    break;

  case 158:

    {
//      | IDL_CHARACTER_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].cval));
        }
    break;

  case 159:

    {
//      | IDL_WCHAR_LITERAL
          ACE_OutputCDR::from_wchar wc ((tao_yyvsp[(1) - (1)].wcval));
          (tao_yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
    break;

  case 160:

    {
//      | IDL_FLOATING_PT_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].dval));
        }
    break;

  case 161:

    {
//      | IDL_TRUETOK
          (tao_yyval.exval) = idl_global->gen ()->create_expr (true);
        }
    break;

  case 162:

    {
//      | IDL_FALSETOK
          (tao_yyval.exval) = idl_global->gen ()->create_expr (false);
        }
    break;

  case 163:

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

  case 164:

    {
// type_dcl : IDL_TYPEDEF
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
        }
    break;

  case 165:

    {
//      type_declarator
          (tao_yyval.ival) = 0;
        }
    break;

  case 166:

    {
//      | struct_type
          (tao_yyval.ival) = 0;
        }
    break;

  case 167:

    {
//      | union_type
          (tao_yyval.ival) = 0;
        }
    break;

  case 168:

    {
//      | enum_type
          (tao_yyval.ival) = 0;
        }
    break;

  case 169:

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

  case 170:

    {
//      | constructed_forward_type_spec
          (tao_yyval.ival) = 0;
        }
    break;

  case 171:

    {
// type_declarator : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
    break;

  case 172:

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

  case 175:

    {
// simple_type_spec : base_type_spec
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 177:

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

  case 194:

    {
// at_least_one_declarator : declarator declarators
          ACE_NEW_RETURN ((tao_yyval.dlval),
                          UTL_DeclList ((tao_yyvsp[(1) - (2)].deval),
                                        (tao_yyvsp[(2) - (2)].dlval)),
                          1);
        }
    break;

  case 195:

    {
// declarators : declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
    break;

  case 196:

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

  case 197:

    {
/*      |  EMPTY */
          (tao_yyval.dlval) = 0;
        }
    break;

  case 200:

    {
// at_least_one_simple_declarator : simple_declarator simple_declarators
          ACE_NEW_RETURN ((tao_yyval.dlval),
                          UTL_DeclList ((tao_yyvsp[(1) - (2)].deval),
                                        (tao_yyvsp[(2) - (2)].dlval)),
                          1);
        }
    break;

  case 201:

    {
// simple_declarators : simple_declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
    break;

  case 202:

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

  case 203:

    {
/*      |  EMPTY */
          (tao_yyval.dlval) = 0;
        }
    break;

  case 204:

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

  case 205:

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

  case 208:

    {
// signed_int : IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_long;
        }
    break;

  case 209:

    {
//      | IDL_LONG IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_longlong;
        }
    break;

  case 210:

    {
//      | IDL_SHORT
          (tao_yyval.etval) = AST_Expression::EV_short;
        }
    break;

  case 211:

    {
// unsigned_int : IDL_UNSIGNED IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_ulong;
        }
    break;

  case 212:

    {
//      | IDL_UNSIGNED IDL_LONG IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_ulonglong;
        }
    break;

  case 213:

    {
//      | IDL_UNSIGNED IDL_SHORT
          (tao_yyval.etval) = AST_Expression::EV_ushort;
        }
    break;

  case 214:

    {
// floating_pt_type : IDL_DOUBLE
          (tao_yyval.etval) = AST_Expression::EV_double;
        }
    break;

  case 215:

    {
//      | IDL_FLOAT
          (tao_yyval.etval) = AST_Expression::EV_float;
        }
    break;

  case 216:

    {
//      | IDL_LONG IDL_DOUBLE
          (tao_yyval.etval) = AST_Expression::EV_longdouble;
        }
    break;

  case 217:

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

  case 218:

    {
// char_type : IDL_CHAR
          (tao_yyval.etval) = AST_Expression::EV_char;
        }
    break;

  case 219:

    {
//      | IDL_WCHAR
          (tao_yyval.etval) = AST_Expression::EV_wchar;
        }
    break;

  case 220:

    {
// octet_type : IDL_OCTET
          (tao_yyval.etval) = AST_Expression::EV_octet;
        }
    break;

  case 221:

    {
// boolean_type : IDL_BOOLEAN
          (tao_yyval.etval) = AST_Expression::EV_bool;
        }
    break;

  case 222:

    {
// any_type : IDL_ANY
          (tao_yyval.etval) = AST_Expression::EV_any;
        }
    break;

  case 223:

    {
// object_type : IDL_OBJECT
          (tao_yyval.etval) = AST_Expression::EV_object;
        }
    break;

  case 224:

    {
// struct_decl : IDL_STRUCT
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
    break;

  case 225:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 226:

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

  case 227:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
    break;

  case 228:

    {
//      at_least_one_member
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
    break;

  case 229:

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

  case 233:

    {
// member  :
          /* is $0 to member_i */
          (tao_yyval.vival) = AST_Field::vis_NA;
        }
    break;

  case 235:

    {
// member_i : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
    break;

  case 236:

    {
//      at_least_one_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
    break;

  case 237:

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

  case 238:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state ());
        }
    break;

  case 239:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 240:

    {
// union_decl : IDL_UNION
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
    break;

  case 241:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 242:

    {
// union_type : union_decl IDL_SWITCH
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
    break;

  case 243:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchOpenParSeen);
        }
    break;

  case 244:

    {
//      switch_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
    break;

  case 245:

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

  case 246:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
    break;

  case 247:

    {
//      at_least_one_case_branch
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
    break;

  case 248:

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

  case 249:

    {
// switch_type_spec : integer_type
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 250:

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

  case 251:

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

  case 252:

    {
//      | boolean_type
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 254:

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

  case 258:

    {
// case_branch : at_least_one_case_label
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
    break;

  case 259:

    {
//      element_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
    break;

  case 260:

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

  case 261:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
    break;

  case 262:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 263:

    {
// at_least_one_case_label : case_label case_labels
          ACE_NEW_RETURN ((tao_yyval.llval),
                          UTL_LabelList ((tao_yyvsp[(1) - (2)].ulval),
                                         (tao_yyvsp[(2) - (2)].llval)),
                          1);
        }
    break;

  case 264:

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

  case 265:

    {
/*      |  EMPTY */
          (tao_yyval.llval) = 0;
        }
    break;

  case 266:

    {
// case_label : IDL_DEFAULT
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
    break;

  case 267:

    {
//      ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (tao_yyval.ulval) = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
    break;

  case 268:

    {
//      | IDL_CASE
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
    break;

  case 269:

    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
    break;

  case 270:

    {
//      const_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (tao_yyval.ulval) = idl_global->gen()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      (tao_yyvsp[(3) - (5)].exval)
                                    );
        }
    break;

  case 271:

    {
// element_spec : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
    break;

  case 272:

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

  case 273:

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

  case 274:

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

  case 275:

    {
// enum_type : IDL_ENUM
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
    break;

  case 276:

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

  case 277:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
    break;

  case 278:

    {
//      at_least_one_enumerator
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
    break;

  case 279:

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

  case 281:

    {
// enumerators : enumerators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
    break;

  case 284:

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

  case 285:

    {
// sequence_type_spec : seq_head ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
    break;

  case 286:

    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
    break;

  case 287:

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

  case 288:

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

  case 289:

    {
// seq_head : IDL_SEQUENCE
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
    break;

  case 290:

    {
//      '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
    break;

  case 291:

    {
//      simple_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          (tao_yyval.dcval) = (tao_yyvsp[(5) - (5)].dcval);
        }
    break;

  case 292:

    {
// string_type_spec : string_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
    break;

  case 293:

    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
    break;

  case 294:

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

  case 295:

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

  case 296:

    {
// string_head : IDL_STRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
    break;

  case 297:

    {
// wstring_type_spec : wstring_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
    break;

  case 298:

    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
    break;

  case 299:

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

  case 300:

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

  case 301:

    {
// wstring_head : IDL_WSTRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
    break;

  case 302:

    {
// array_declarator : id
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
    break;

  case 303:

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

  case 304:

    {
// at_least_one_array_dim : array_dim array_dims
          ACE_NEW_RETURN ((tao_yyval.elval),
                          UTL_ExprList ((tao_yyvsp[(1) - (2)].exval),
                                        (tao_yyvsp[(2) - (2)].elval)),
                          1);
        }
    break;

  case 305:

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

  case 306:

    {
/*      |  EMPTY */
          (tao_yyval.elval) = 0;
        }
    break;

  case 307:

    {
// array_dim : '['
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
    break;

  case 308:

    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
    break;

  case 309:

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

  case 312:

    {
// attribute_readonly : IDL_READONLY
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
    break;

  case 313:

    {
//      IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
    break;

  case 314:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
    break;

  case 315:

    {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
    break;

  case 316:

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

  case 317:

    {
// attribute_readonly : IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
    break;

  case 318:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
    break;

  case 319:

    {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
    break;

  case 320:

    {
//      opt_getraises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
    break;

  case 321:

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

  case 322:

    {
// exception : IDL_EXCEPTION
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
    break;

  case 323:

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

  case 324:

    {
//       '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
    break;

  case 325:

    {
//      members
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
    break;

  case 326:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 327:

    {
// operation : opt_op_attribute op_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
    break;

  case 328:

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

  case 329:

    {
//      parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 330:

    {
//      opt_raises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
    break;

  case 331:

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

  case 332:

    {
// opt_op_attribute : IDL_ONEWAY
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (tao_yyval.ofval) = AST_Operation::OP_oneway;
        }
    break;

  case 333:

    {
//      | IDL_IDEMPOTENT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (tao_yyval.ofval) = AST_Operation::OP_idempotent;
        }
    break;

  case 334:

    {
/*      |  EMPTY */
          (tao_yyval.ofval) = AST_Operation::OP_noflags;
        }
    break;

  case 336:

    {
// op_type_spec : param_type_spec | IDL_VOID
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
    break;

  case 337:

    {
// init_decl : IDL_FACTORY
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
    break;

  case 338:

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

  case 339:

    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 340:

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

  case 341:

    {
// init_parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 342:

    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 343:

    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 344:

    {
//      at_least_one_in_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 346:

    {
// in_parameters : in_parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
    break;

  case 349:

    {
// in_parameter : IDL_IN
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
    break;

  case 350:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
    break;

  case 351:

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

  case 352:

    {
// parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 353:

    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 354:

    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 355:

    {
//      at_least_one_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 357:

    {
// parameters : parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
    break;

  case 360:

    {
// parameter : direction
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
    break;

  case 361:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
    break;

  case 362:

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

  case 363:

    {
// param_type_spec : base_type_spec
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 366:

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

  case 367:

    {
// direction : IDL_IN
          (tao_yyval.dival) = AST_Argument::dir_IN;
        }
    break;

  case 368:

    {
//      | IDL_OUT
          (tao_yyval.dival) = AST_Argument::dir_OUT;
        }
    break;

  case 369:

    {
//      | IDL_INOUT
          (tao_yyval.dival) = AST_Argument::dir_INOUT;
        }
    break;

  case 370:

    {
// opt_raises : IDL_RAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
    break;

  case 371:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
    break;

  case 372:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          (tao_yyval.nlval) = (tao_yyvsp[(5) - (6)].nlval);
        }
    break;

  case 373:

    {
          (tao_yyval.nlval) = 0;
/*      |  EMPTY */
        }
    break;

  case 374:

    {
// opt_getraises : IDL_GETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
    break;

  case 375:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
    break;

  case 376:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          (tao_yyval.nlval) = (tao_yyvsp[(5) - (6)].nlval);
        }
    break;

  case 377:

    {
          (tao_yyval.nlval) = 0;
/*      |  EMPTY */
        }
    break;

  case 378:

    {
// opt_setraises : IDL_SETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
    break;

  case 379:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
    break;

  case 380:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (tao_yyval.nlval) = (tao_yyvsp[(5) - (6)].nlval);
        }
    break;

  case 381:

    {
          (tao_yyval.nlval) = 0;
/*      |  EMPTY */
        }
    break;

  case 382:

    {
// opt_context : IDL_CONTEXT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
    break;

  case 383:

    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
//      '('
        }
    break;

  case 384:

    {
//      at_least_one_string_literal ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (tao_yyval.slval) = (tao_yyvsp[(5) - (6)].slval);
        }
    break;

  case 385:

    {
/*      |  EMPTY */
          (tao_yyval.slval) = 0;
        }
    break;

  case 386:

    {
// at_least_one_string_literal : IDL_STRING_LITERAL string_literals
          ACE_NEW_RETURN ((tao_yyval.slval),
                          UTL_StrList ((tao_yyvsp[(1) - (2)].sval),
                                       (tao_yyvsp[(2) - (2)].slval)),
                          1);
        }
    break;

  case 387:

    {
// string_literals : string_literals ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
    break;

  case 388:

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

  case 389:

    {
/*      |  EMPTY */
          (tao_yyval.slval) = 0;
        }
    break;

  case 390:

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

  case 391:

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

  case 394:

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

  case 395:

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

  case 396:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
    break;

  case 397:

    {
//      component_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
    break;

  case 398:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 399:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
    break;

  case 400:

    {
//      component_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 401:

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

  case 402:

    {
// component_inheritance_spec : ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
    break;

  case 403:

    {
//      scoped_name
          (tao_yyval.idlist) = (tao_yyvsp[(3) - (3)].idlist);
        }
    break;

  case 404:

    {
/*      |  EMPTY */
          (tao_yyval.idlist) = 0;
        }
    break;

  case 407:

    {
// component_export : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
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
//      | uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
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
//      | emits_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
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
//      | publishes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
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
//      | consumes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
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
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
    break;

  case 418:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 419:

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

  case 420:

    {
// interface_type : scoped_name
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (tao_yyval.idlist) = (tao_yyvsp[(1) - (1)].idlist);
        }
    break;

  case 421:

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

  case 422:

    {
// uses_decl : uses_opt_multiple interface_type id
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
          else if (d->node_type () != AST_Decl::NT_interface)
            {
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

          AST_Type *interface_type = AST_Type::narrow_from_decl (d);
          AST_Component *c = AST_Component::narrow_from_scope (s);

          if (c != 0)
            {
              AST_Component::port_description ud;

              // Strip off _cxx_, if any, for port name.
              idl_global->original_local_name ((tao_yyvsp[(3) - (3)].idval));

              ud.id = (tao_yyvsp[(3) - (3)].idval);
              ud.impl = interface_type;
              ud.is_multiple = (tao_yyvsp[(1) - (3)].bval);
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

          (tao_yyvsp[(2) - (3)].idlist)->destroy ();
          delete (tao_yyvsp[(2) - (3)].idlist);
          (tao_yyvsp[(2) - (3)].idlist) = 0;
        }
    break;

  case 423:

    {
// uses_opt_multiple : IDL_USES opt_multiple
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          (tao_yyval.bval) = (tao_yyvsp[(2) - (2)].bval);
        }
    break;

  case 424:

    {
// opt_multiple : IDL_MULTIPLE
          (tao_yyval.bval) = true;
        }
    break;

  case 425:

    {
/*      |  EMPTY */
          (tao_yyval.bval) = false;
        }
    break;

  case 426:

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

  case 427:

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

  case 428:

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

  case 429:

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

  case 430:

    {
//      home_body
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 431:

    {
// home_header : IDL_HOME
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
    break;

  case 432:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
    break;

  case 433:

    {
//      home_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 434:

    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
    break;

  case 435:

    {
//      IDL_MANAGES
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
    break;

  case 436:

    {
//      scoped_name
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
    break;

  case 437:

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

  case 438:

    {
// home_inheritance_spec ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
    break;

  case 439:

    {
//      scoped_name
          (tao_yyval.idlist) = (tao_yyvsp[(3) - (3)].idlist);
        }
    break;

  case 440:

    {
/*      |  EMPTY */
          (tao_yyval.idlist) = 0;
        }
    break;

  case 441:

    {
// primary_key_spec : IDL_PRIMARYKEY scoped_name
          (tao_yyval.idlist) = (tao_yyvsp[(2) - (2)].idlist);
        }
    break;

  case 442:

    {
/*      |  EMPTY */
          (tao_yyval.idlist) = 0;
        }
    break;

  case 443:

    {
// home_body : '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
    break;

  case 444:

    {
//      home_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
    break;

  case 445:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
    break;

  case 449:

    {
// home_export : factory_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
    break;

  case 450:

    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 451:

    {
//      | finder_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
    break;

  case 452:

    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 453:

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

  case 456:

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

  case 457:

    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 458:

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

  case 464:

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

  case 465:

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

  case 466:

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

  case 467:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
    break;

  case 468:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
    break;

  case 469:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 470:

    {
// event_abs_header : IDL_ABSTRACT IDL_EVENTTYPE id
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 471:

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

  case 472:

    {
// event_plain_header : IDL_EVENTTYPE id
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (tao_yyval.idval) = (tao_yyvsp[(2) - (2)].idval);
        }
    break;

  case 473:

    {
// event_rest_of_header : inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 474:

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

  case 475:

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

  case 476:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
    break;

  case 477:

    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
    break;

  case 478:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 479:

    {
// event_header : event_custom_header
          (tao_yyval.idval) = (tao_yyvsp[(1) - (1)].idval);
        }
    break;

  case 480:

    {
// event_header : event_plain_header
          (tao_yyval.idval) = (tao_yyvsp[(1) - (1)].idval);
        }
    break;

  case 481:

    {
// type_classifier : IDL_TYPENAME
          (tao_yyval.ntval) = AST_Decl::NT_type;
        }
    break;

  case 482:

    {
//        IDL_STRUCT
          (tao_yyval.ntval) = AST_Decl::NT_struct;
        }
    break;

  case 483:

    {
//        IDL_EVENTTYPE
          (tao_yyval.ntval) = AST_Decl::NT_eventtype;
        }
    break;

  case 484:

    {
//        IDL_PRIMITIVE
          (tao_yyval.ntval) = AST_Decl::NT_pre_defined;
        }
    break;

  case 485:

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

  case 486:

    {
//        IDL_SEQUENCE
          (tao_yyval.ntval) = AST_Decl::NT_sequence;
        }
    break;

  case 487:

    {
//        IDL_INTERFACE
          (tao_yyval.ntval) = AST_Decl::NT_interface;
        }
    break;

  case 488:

    {
//        IDL_VALUETYPE
          (tao_yyval.ntval) = AST_Decl::NT_valuetype;
        }
    break;

  case 489:

    {
// template_interface_def : template_interface_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          AST_Template_Interface *i =
            idl_global->gen ()->create_template_interface (
              (tao_yyvsp[(1) - (1)].thval)->name (),
              (tao_yyvsp[(1) - (1)].thval)->inherits (),
              (tao_yyvsp[(1) - (1)].thval)->n_inherits (),
              (tao_yyvsp[(1) - (1)].thval)->inherits_flat (),
              (tao_yyvsp[(1) - (1)].thval)->n_inherits_flat (),
              (tao_yyvsp[(1) - (1)].thval)->param_info ());

          (void) s->fe_add_interface (i);

          (tao_yyvsp[(1) - (1)].thval)->destroy ();
          delete (tao_yyvsp[(1) - (1)].thval);
          (tao_yyvsp[(1) - (1)].thval) = 0;

          idl_global->scopes ().push (i);
        }
    break;

  case 490:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
    break;

  case 491:

    {
//      exports - TODO: change to include concatenated identifiers
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
    break;

  case 492:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 493:

    {
// template_interface_header : interface_decl at_least_one_template_param template_inheritance_spec
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ((tao_yyvsp[(1) - (3)].idval), 0),
                          1);

          ACE_NEW_RETURN ((tao_yyval.thval),
                          FE_Template_InterfaceHeader (n,
                                                       (tao_yyvsp[(2) - (3)].plval),
                                                       (tao_yyvsp[(3) - (3)].rlval)),
                          1);
        }
    break;

  case 494:

    {
// at_least_one_template_param : '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplInterfaceSqSeen);
        }
    break;

  case 495:

    {
//        template_param template_params
          if ((tao_yyvsp[(4) - (4)].plval) == 0)
            {
              ACE_NEW_RETURN ((tao_yyvsp[(4) - (4)].plval),
                              FE_Utils::T_PARAMLIST_INFO,
                              1);
            }

          (tao_yyvsp[(4) - (4)].plval)->enqueue_head (*(tao_yyvsp[(3) - (4)].pival));
          delete (tao_yyvsp[(3) - (4)].pival);
          (tao_yyvsp[(3) - (4)].pival) = 0;
        }
    break;

  case 496:

    {
//        '>'
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplInterfaceQsSeen);
          (tao_yyval.plval) = (tao_yyvsp[(4) - (6)].plval);
        }
    break;

  case 497:

    {
// template_params : template_params ','
          // Maybe add a new parse state to set here.
        }
    break;

  case 498:

    {
//        template_param
          if ((tao_yyvsp[(1) - (4)].plval) == 0)
            {
              ACE_NEW_RETURN ((tao_yyvsp[(1) - (4)].plval),
                              FE_Utils::T_PARAMLIST_INFO,
                              1);
            }

          (tao_yyvsp[(1) - (4)].plval)->enqueue_tail (*(tao_yyvsp[(4) - (4)].pival));
          delete (tao_yyvsp[(4) - (4)].pival);
          (tao_yyvsp[(4) - (4)].pival) = 0;

          (tao_yyval.plval) = (tao_yyvsp[(1) - (4)].plval);
        }
    break;

  case 499:

    {
//        /* EMPTY */
          (tao_yyval.plval) = 0;
        }
    break;

  case 500:

    {
// template_param : type_classifier IDENTIFIER

          ACE_NEW_RETURN ((tao_yyval.pival),
                          FE_Utils::T_Param_Info,
                          1);

          (tao_yyval.pival)->type_ = (tao_yyvsp[(1) - (2)].ntval);
          (tao_yyval.pival)->name_ = (tao_yyvsp[(2) - (2)].strval);
        }
    break;

  case 501:

    {
// template_inheritance_spec : ':' at_least_one_template_ref
          (tao_yyval.rlval) = (tao_yyvsp[(2) - (2)].rlval);
        }
    break;

  case 502:

    {
//        /* EMPTY */
          (tao_yyval.rlval) = 0;
        }
    break;

  case 503:

    {
// at_least_one_template_ref : template_ref template_refs
          if ((tao_yyvsp[(2) - (2)].rlval) == 0)
            {
              ACE_NEW_RETURN ((tao_yyvsp[(2) - (2)].rlval),
                              FE_Utils::T_REFLIST_INFO,
                              1);
            }

          (tao_yyvsp[(2) - (2)].rlval)->enqueue_head (*(tao_yyvsp[(1) - (2)].trval));
          delete (tao_yyvsp[(1) - (2)].trval);
          (tao_yyvsp[(1) - (2)].trval) = 0;

          (tao_yyval.rlval) = (tao_yyvsp[(2) - (2)].rlval);
        }
    break;

  case 504:

    {
// template_refs : template_refs ',' template_ref
          if ((tao_yyvsp[(1) - (3)].rlval) == 0)
            {
              ACE_NEW_RETURN ((tao_yyvsp[(1) - (3)].rlval),
                              FE_Utils::T_REFLIST_INFO,
                              1);

              (tao_yyvsp[(1) - (3)].rlval)->enqueue_tail (*(tao_yyvsp[(3) - (3)].trval));
              delete (tao_yyvsp[(3) - (3)].trval);
              (tao_yyvsp[(3) - (3)].trval) = 0;

              (tao_yyval.rlval) = (tao_yyvsp[(1) - (3)].rlval);
            }
        }
    break;

  case 505:

    {
//        /* EMPTY */
          (tao_yyval.rlval) = 0;
        }
    break;

  case 506:

    {
// template_ref : scoped_name '<' at_least_one_template_param_ref '>'
          ACE_NEW_RETURN ((tao_yyval.trval),
                          FE_Utils::T_Ref_Info ((tao_yyvsp[(1) - (4)].idlist), (tao_yyvsp[(3) - (4)].slval)),
                          1);
        }
    break;

  case 507:

    {
// at_least_one_template_param_ref : template_param_ref template_param_refs
          ACE_NEW_RETURN ((tao_yyval.slval),
                          UTL_StrList ((tao_yyvsp[(1) - (2)].sval),
                                       (tao_yyvsp[(2) - (2)].slval)),
                          1);
        }
    break;

  case 508:

    {
// template_param_refs : template_param_refs ',' template_param_ref
          if ((tao_yyvsp[(1) - (3)].slval) == 0)
            {
              ACE_NEW_RETURN ((tao_yyvsp[(1) - (3)].slval),
                              UTL_StrList ((tao_yyvsp[(3) - (3)].sval),
                                           0),
                              1);
            }
          else
            {
              UTL_StrList *l = 0;
              ACE_NEW_RETURN (l,
                              UTL_StrList ((tao_yyvsp[(3) - (3)].sval),
                                           0),
                              1);

              (tao_yyvsp[(1) - (3)].slval)->nconc (l);
            }

          (tao_yyval.slval) = (tao_yyvsp[(1) - (3)].slval);
        }
    break;

  case 509:

    {
//        /* EMPTY */
          (tao_yyval.slval) = 0;
        }
    break;

  case 510:

    {
// template_param_ref : IDENTIFIER
          ACE_NEW_RETURN ((tao_yyval.sval),
                          UTL_String ((tao_yyvsp[(1) - (1)].strval)),
                          1);
        }
    break;

  case 511:

    {
// porttype_decl : IDL_PORTTYPE
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSeen);
        }
    break;

  case 512:

    {
//        IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeIDSeen);
        }
    break;

  case 513:

    {
//        opt_template_params
        }
    break;

  case 514:

    {
//        '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSqSeen);
        }
    break;

  case 515:

    {
//        at_least_one_port_export
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeBodySeen);
        }
    break;

  case 516:

    {
//        '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeQsSeen);
        }
    break;

  case 517:

    {
// opt_template_params : at_least_one_template_param
        }
    break;

  case 518:

    {
//        | /* EMPTY */
        }
    break;

  case 519:

    {
// at_least_one_port_export : port_export port_exports
        }
    break;

  case 520:

    {
// port_exports : port_exports port_export
        }
    break;

  case 521:

    {
//        | /* EMPTY */
        }
    break;

  case 522:

    {
// port_export : extended_provides_decl
        }
    break;

  case 523:

    {
//        ';'
        }
    break;

  case 524:

    {
//        | extended_uses_decl
        }
    break;

  case 525:

    {
//        ';'
        }
    break;

  case 526:

    {
// extended_provides_decl : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
    break;

  case 527:

    {
//        | provides_type at_least_one_template_param_ref IDENTIFIER

          idl_global->set_parse_state (IDL_GlobalData::PS_ExtProvidesDeclSeen);

          // We allow 'Object' in the grammar, to use more grammar in common with
          // provides_decl and avoid conflicts in the LALR(1) parser. We just
          // check for CORBA::Object below and report an error if found.
        }
    break;

  case 528:

    {
// extended_uses_decl : uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
    break;

  case 529:

    {
//        | uses_opt_multiple template_ref IDENTIFIER

          idl_global->set_parse_state (IDL_GlobalData::PS_ExtUsesDeclSeen);

          // We allow 'Object' in the grammar, to use more grammar in common with
          // uses_decl and avoid conflicts in the LALR(1) parser. We just
          // check for CORBA::Object below and report an error if found.
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

