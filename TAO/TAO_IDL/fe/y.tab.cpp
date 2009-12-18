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
     IDL_PORT = 323,
     IDL_MIRRORPORT = 324,
     IDL_PORTTYPE = 325,
     IDL_CONNECTOR = 326,
     IDL_ALIAS = 327,
     IDL_INTEGER_LITERAL = 328,
     IDL_UINTEGER_LITERAL = 329,
     IDL_STRING_LITERAL = 330,
     IDL_CHARACTER_LITERAL = 331,
     IDL_FLOATING_PT_LITERAL = 332,
     IDL_TRUETOK = 333,
     IDL_FALSETOK = 334,
     IDL_SCOPE_DELIMITOR = 335,
     IDL_LEFT_SHIFT = 336,
     IDL_RIGHT_SHIFT = 337,
     IDL_WCHAR_LITERAL = 338,
     IDL_WSTRING_LITERAL = 339
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
#define IDL_PORT 323
#define IDL_MIRRORPORT 324
#define IDL_PORTTYPE 325
#define IDL_CONNECTOR 326
#define IDL_ALIAS 327
#define IDL_INTEGER_LITERAL 328
#define IDL_UINTEGER_LITERAL 329
#define IDL_STRING_LITERAL 330
#define IDL_CHARACTER_LITERAL 331
#define IDL_FLOATING_PT_LITERAL 332
#define IDL_TRUETOK 333
#define IDL_FALSETOK 334
#define IDL_SCOPE_DELIMITOR 335
#define IDL_LEFT_SHIFT 336
#define IDL_RIGHT_SHIFT 337
#define IDL_WCHAR_LITERAL 338
#define IDL_WSTRING_LITERAL 339




/* Copy the first part of user declarations.  */


#include "ast_argument.h"
#include "ast_array.h"
#include "ast_attribute.h"
#include "ast_field.h"
#include "ast_expression.h"
#include "ast_operation.h"
#include "ast_generator.h"
#include "ast_template_module.h"
#include "ast_template_module_inst.h"
#include "ast_template_module_ref.h"
#include "ast_typedef.h"
#include "ast_valuebox.h"
#include "ast_valuetype.h"
#include "ast_valuetype_fwd.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_component.h"
#include "ast_component_fwd.h"
#include "ast_home.h"
#include "ast_porttype.h"
#include "ast_connector.h"
#include "ast_uses.h"
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
#include "ast_param_holder.h"

#include "fe_declarator.h"
#include "fe_interface_header.h"
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
AST_Enum *tao_enum_constant_decl = 0;
AST_Expression::ExprType t_param_const_type = AST_Expression::EV_none;
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
  FE_Utils::T_Param_Info        *pival;         /* Template interface param */
  FE_Utils::T_PARAMLIST_INFO    *plval;         /* List of template params */
  FE_Utils::T_Ref_Info          *trval;         /* Template interface info */
  FE_Utils::T_REFLIST_INFO      *rlval;         /* List of above structs */
  FE_Utils::T_Inst_Info         *tival;         /* Template instantiation */
  FE_Utils::T_Port_Info         *ptval;         /* Porttype reference */
  FE_Utils::T_ARGLIST           *alval;         /* List of template args */
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
#define TAO_YYLAST   1264

/* TAO_YYNTOKENS -- Number of terminals.  */
#define TAO_YYNTOKENS  106
/* TAO_YYNNTS -- Number of nonterminals.  */
#define TAO_YYNNTS  382
/* TAO_YYNRULES -- Number of rules.  */
#define TAO_YYNRULES  565
/* TAO_YYNRULES -- Number of states.  */
#define TAO_YYNSTATES  808

/* TAO_YYTRANSLATE(TAO_YYLEX) -- Bison symbol number corresponding to TAO_YYLEX.  */
#define TAO_YYUNDEFTOK  2
#define TAO_YYMAXUTOK   339

#define TAO_YYTRANSLATE(TAO_YYX)                                                \
  ((unsigned int) (TAO_YYX) <= TAO_YYMAXUTOK ? tao_yytranslate[TAO_YYX] : TAO_YYUNDEFTOK)

/* TAO_YYTRANSLATE[TAO_YYLEX] -- Bison symbol number corresponding to TAO_YYLEX.  */
static const tao_yytype_uint8 tao_yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   100,    95,     2,
     102,   103,    98,    96,    91,    97,     2,    99,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    90,    85,
      88,    92,    89,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   104,     2,   105,    94,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    86,    93,    87,   101,     2,     2,     2,
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
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84
};

#if TAO_YYDEBUG
/* TAO_YYPRHS[TAO_YYN] -- Index of the first RHS symbol of rule number TAO_YYN in
   TAO_YYRHS.  */
static const tao_yytype_uint16 tao_yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    11,    14,    17,    20,
      23,    24,    25,    29,    30,    34,    35,    39,    40,    44,
      45,    49,    50,    54,    55,    59,    60,    64,    65,    69,
      70,    74,    75,    79,    80,    84,    85,    89,    90,    94,
      95,    99,   100,   101,   102,   110,   113,   114,   115,   116,
     117,   118,   130,   133,   136,   137,   139,   142,   143,   144,
     153,   154,   155,   162,   164,   166,   167,   168,   169,   177,
     178,   182,   185,   189,   193,   194,   199,   200,   202,   204,
     206,   208,   211,   213,   214,   215,   216,   224,   225,   226,
     227,   236,   237,   242,   243,   247,   249,   250,   253,   254,
     257,   259,   262,   265,   266,   268,   270,   273,   274,   278,
     279,   283,   286,   287,   288,   292,   293,   297,   298,   302,
     303,   307,   308,   312,   313,   317,   318,   322,   323,   327,
     330,   331,   336,   337,   339,   340,   344,   345,   350,   352,
     354,   357,   360,   361,   362,   363,   364,   374,   376,   378,
     380,   382,   384,   386,   388,   390,   392,   394,   396,   398,
     402,   404,   408,   410,   414,   416,   420,   424,   426,   430,
     434,   436,   440,   444,   448,   450,   453,   456,   459,   461,
     463,   467,   469,   471,   473,   475,   477,   479,   481,   483,
     485,   487,   488,   492,   494,   496,   498,   501,   503,   504,
     508,   510,   512,   514,   516,   518,   520,   522,   524,   526,
     528,   530,   532,   534,   536,   538,   540,   542,   544,   546,
     548,   550,   553,   554,   559,   560,   562,   564,   567,   568,
     573,   574,   576,   578,   580,   582,   584,   587,   589,   592,
     596,   599,   601,   603,   606,   608,   610,   612,   614,   616,
     618,   620,   621,   625,   626,   627,   628,   636,   639,   642,
     643,   644,   647,   648,   649,   655,   656,   660,   661,   665,
     666,   667,   668,   669,   670,   671,   686,   688,   690,   692,
     694,   696,   698,   701,   704,   705,   706,   707,   713,   714,
     718,   721,   724,   725,   726,   730,   731,   732,   738,   739,
     743,   745,   747,   748,   749,   750,   751,   761,   764,   765,
     770,   771,   773,   774,   775,   782,   785,   786,   787,   793,
     794,   795,   802,   804,   806,   807,   808,   815,   817,   819,
     820,   824,   827,   830,   831,   832,   833,   839,   841,   843,
     844,   845,   846,   847,   857,   858,   859,   860,   861,   871,
     872,   873,   874,   875,   885,   886,   887,   888,   889,   900,
     902,   904,   905,   907,   909,   910,   911,   912,   920,   921,
     925,   926,   931,   934,   935,   940,   941,   942,   943,   949,
     950,   954,   955,   960,   963,   964,   969,   970,   971,   972,
     978,   980,   982,   984,   986,   988,   990,   992,   993,   994,
    1001,  1002,  1003,  1004,  1011,  1012,  1013,  1014,  1021,  1022,
    1023,  1024,  1031,  1032,  1035,  1036,  1041,  1042,  1046,  1050,
    1052,  1054,  1057,  1058,  1059,  1060,  1068,  1069,  1070,  1077,
    1078,  1082,  1083,  1086,  1087,  1088,  1092,  1093,  1097,  1098,
    1102,  1103,  1107,  1108,  1112,  1113,  1117,  1118,  1122,  1126,
    1128,  1130,  1134,  1137,  1139,  1140,  1144,  1148,  1152,  1153,
    1157,  1158,  1159,  1160,  1161,  1162,  1163,  1177,  1178,  1182,
    1183,  1186,  1187,  1188,  1189,  1195,  1198,  1199,  1201,  1202,
    1206,  1207,  1211,  1212,  1213,  1220,  1221,  1222,  1229,  1231,
    1233,  1235,  1237,  1239,  1242,  1246,  1247,  1248,  1249,  1258,
    1262,  1266,  1269,  1270,  1274,  1275,  1276,  1277,  1286,  1288,
    1290,  1292,  1294,  1296,  1298,  1300,  1302,  1304,  1306,  1308,
    1311,  1314,  1315,  1320,  1321,  1324,  1330,  1333,  1337,  1338,
    1340,  1341,  1342,  1343,  1344,  1354,  1357,  1360,  1361,  1362,
    1366,  1367,  1371,  1375,  1379,  1382,  1386,  1387,  1389,  1392,
    1393,  1394,  1400,  1401,  1402,  1408,  1411,  1414,  1415,  1416,
    1420,  1421,  1425,  1426,  1430,  1431
};

/* TAO_YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const tao_yytype_int16 tao_yyrhs[] =
{
     107,     0,    -1,   108,    -1,   108,   109,    -1,    -1,   112,
      -1,   134,    85,    -1,   146,    85,    -1,   111,   112,    -1,
     111,   112,    -1,    -1,    -1,   218,   113,    85,    -1,    -1,
     379,   114,    85,    -1,    -1,   380,   115,    85,    -1,    -1,
     200,   116,    85,    -1,    -1,   328,   117,    85,    -1,    -1,
     149,   118,    85,    -1,    -1,   129,   119,    85,    -1,    -1,
     159,   120,    85,    -1,    -1,   381,   121,    85,    -1,    -1,
     409,   122,    85,    -1,    -1,   434,   123,    85,    -1,    -1,
     460,   124,    85,    -1,    -1,   474,   125,    85,    -1,    -1,
       1,   126,    85,    -1,    -1,     5,   128,   195,    -1,    -1,
      -1,    -1,   127,   130,    86,   131,   110,   132,    87,    -1,
     127,    88,    -1,    -1,    -1,    -1,    -1,    -1,   133,   135,
     453,   136,    89,   137,    86,   138,   140,   139,    87,    -1,
     142,   141,    -1,   141,   142,    -1,    -1,   112,    -1,   143,
      85,    -1,    -1,    -1,    72,   195,   144,    88,   457,    89,
     145,   198,    -1,    -1,    -1,   133,   147,   471,    89,   148,
     198,    -1,   150,    -1,   199,    -1,    -1,    -1,    -1,   156,
     151,    86,   152,   182,   153,    87,    -1,    -1,     6,   155,
     198,    -1,   154,   157,    -1,    41,   154,   157,    -1,    42,
     154,   157,    -1,    -1,    90,   173,   158,   192,    -1,    -1,
     160,    -1,   165,    -1,   175,    -1,   176,    -1,    43,   161,
      -1,   161,    -1,    -1,    -1,    -1,   169,   162,    86,   163,
     177,   164,    87,    -1,    -1,    -1,    -1,    42,   169,   166,
      86,   167,   182,   168,    87,    -1,    -1,   171,   157,   170,
     174,    -1,    -1,    49,   172,   198,    -1,    48,    -1,    -1,
      47,   192,    -1,    -1,    42,   171,    -1,   171,    -1,   171,
     222,    -1,   177,   178,    -1,    -1,   179,    -1,   183,    -1,
     340,    85,    -1,    -1,    46,   180,   257,    -1,    -1,    45,
     181,   257,    -1,   182,   183,    -1,    -1,    -1,   218,   184,
      85,    -1,    -1,   379,   185,    85,    -1,    -1,   380,   186,
      85,    -1,    -1,   200,   187,    85,    -1,    -1,   328,   188,
      85,    -1,    -1,   317,   189,    85,    -1,    -1,   333,   190,
      85,    -1,    -1,     1,   191,    85,    -1,   195,   193,    -1,
      -1,   193,    91,   194,   195,    -1,    -1,   198,    -1,    -1,
      80,   196,   198,    -1,    -1,   195,    80,   197,   198,    -1,
       3,    -1,   154,    -1,    41,   154,    -1,    42,   154,    -1,
      -1,    -1,    -1,    -1,     4,   201,   205,   202,   198,   203,
      92,   204,   206,    -1,   237,    -1,   242,    -1,   243,    -1,
     244,    -1,   240,    -1,   241,    -1,   302,    -1,   306,    -1,
     195,    -1,   207,    -1,   208,    -1,   209,    -1,   208,    93,
     209,    -1,   210,    -1,   209,    94,   210,    -1,   211,    -1,
     210,    95,   211,    -1,   212,    -1,   211,    81,   212,    -1,
     211,    82,   212,    -1,   213,    -1,   212,    96,   213,    -1,
     212,    97,   213,    -1,   214,    -1,   213,    98,   214,    -1,
     213,    99,   214,    -1,   213,   100,   214,    -1,   215,    -1,
      96,   215,    -1,    97,   215,    -1,   101,   215,    -1,   195,
      -1,   216,    -1,   102,   207,   103,    -1,    73,    -1,    74,
      -1,    75,    -1,    84,    -1,    76,    -1,    83,    -1,    77,
      -1,    78,    -1,    79,    -1,   207,    -1,    -1,     7,   219,
     220,    -1,   249,    -1,   263,    -1,   287,    -1,    40,   235,
      -1,   227,    -1,    -1,   222,   221,   228,    -1,   223,    -1,
     226,    -1,   224,    -1,   225,    -1,   195,    -1,   237,    -1,
     240,    -1,   242,    -1,   244,    -1,   243,    -1,   241,    -1,
     245,    -1,   246,    -1,   296,    -1,   302,    -1,   306,    -1,
     249,    -1,   263,    -1,   287,    -1,   285,    -1,   286,    -1,
     231,   229,    -1,    -1,   229,    91,   230,   231,    -1,    -1,
     235,    -1,   236,    -1,   235,   233,    -1,    -1,   233,    91,
     234,   235,    -1,    -1,   198,    -1,   310,    -1,   238,    -1,
     239,    -1,     8,    -1,     8,     8,    -1,     9,    -1,    10,
       8,    -1,    10,     8,     8,    -1,    10,     9,    -1,    11,
      -1,    12,    -1,     8,    11,    -1,    17,    -1,    13,    -1,
      14,    -1,    15,    -1,    16,    -1,    18,    -1,    19,    -1,
      -1,    20,   248,   198,    -1,    -1,    -1,    -1,   247,   250,
      86,   251,   253,   252,    87,    -1,   255,   254,    -1,   254,
     255,    -1,    -1,    -1,   256,   257,    -1,    -1,    -1,   222,
     258,   228,   259,    85,    -1,    -1,     1,   260,    85,    -1,
      -1,    21,   262,   198,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   261,    22,   264,   102,   265,   270,   266,   103,   267,
      86,   268,   271,   269,    87,    -1,   237,    -1,   242,    -1,
     243,    -1,   244,    -1,   287,    -1,   195,    -1,   273,   272,
      -1,   272,   273,    -1,    -1,    -1,    -1,   277,   274,   283,
     275,    85,    -1,    -1,     1,   276,    85,    -1,   279,   278,
      -1,   278,   279,    -1,    -1,    -1,    29,   280,    90,    -1,
      -1,    -1,    28,   281,   207,   282,    90,    -1,    -1,   222,
     284,   231,    -1,   247,    -1,   261,    -1,    -1,    -1,    -1,
      -1,    23,   288,   198,   289,    86,   290,   292,   291,    87,
      -1,   295,   293,    -1,    -1,   293,    91,   294,   295,    -1,
      -1,     3,    -1,    -1,    -1,   299,    91,   297,   217,   298,
      89,    -1,   299,    89,    -1,    -1,    -1,    24,   300,    88,
     301,   223,    -1,    -1,    -1,   305,    88,   303,   217,   304,
      89,    -1,   305,    -1,    25,    -1,    -1,    -1,   309,    88,
     307,   217,   308,    89,    -1,   309,    -1,    26,    -1,    -1,
     198,   311,   312,    -1,   314,   313,    -1,   313,   314,    -1,
      -1,    -1,    -1,   104,   315,   217,   316,   105,    -1,   318,
      -1,   323,    -1,    -1,    -1,    -1,    -1,    30,   319,    31,
     320,   362,   321,   232,   322,   364,    -1,    -1,    -1,    -1,
      -1,    31,   324,   362,   325,   232,   326,   367,   327,   370,
      -1,    -1,    -1,    -1,    -1,    27,   329,   198,   330,    86,
     331,   254,   332,    87,    -1,    -1,    -1,    -1,    -1,   338,
     339,   334,     3,   335,   353,   336,   364,   337,   373,    -1,
      32,    -1,    33,    -1,    -1,   362,    -1,    34,    -1,    -1,
      -1,    -1,    44,   341,     3,   342,   344,   343,   364,    -1,
      -1,   102,   345,   103,    -1,    -1,   102,   346,   347,   103,
      -1,   350,   348,    -1,    -1,   348,    91,   349,   350,    -1,
      -1,    -1,    -1,    35,   351,   362,   352,   231,    -1,    -1,
     102,   354,   103,    -1,    -1,   102,   355,   356,   103,    -1,
     359,   357,    -1,    -1,   357,    91,   358,   359,    -1,    -1,
      -1,    -1,   363,   360,   362,   361,   231,    -1,   224,    -1,
     302,    -1,   306,    -1,   195,    -1,    35,    -1,    36,    -1,
      37,    -1,    -1,    -1,    38,   365,   102,   366,   192,   103,
      -1,    -1,    -1,    -1,    55,   368,   102,   369,   192,   103,
      -1,    -1,    -1,    -1,    62,   371,   102,   372,   192,   103,
      -1,    -1,    -1,    -1,    39,   374,   102,   375,   376,   103,
      -1,    -1,    75,   377,    -1,    -1,   377,    91,   378,    75,
      -1,    -1,    63,   195,    75,    -1,    64,   195,    75,    -1,
     383,    -1,   382,    -1,    50,   198,    -1,    -1,    -1,    -1,
     387,   384,    86,   385,   392,   386,    87,    -1,    -1,    -1,
      50,   198,   388,   390,   389,   174,    -1,    -1,    90,   391,
     195,    -1,    -1,   392,   393,    -1,    -1,    -1,   401,   394,
      85,    -1,    -1,   403,   395,    85,    -1,    -1,   406,   396,
      85,    -1,    -1,   407,   397,    85,    -1,    -1,   408,   398,
      85,    -1,    -1,   317,   399,    85,    -1,    -1,   470,   400,
      85,    -1,    60,   402,   198,    -1,   195,    -1,    19,    -1,
     404,   402,   198,    -1,    65,   405,    -1,    58,    -1,    -1,
      52,   195,   198,    -1,    61,   195,   198,    -1,    51,   195,
     198,    -1,    -1,   411,   410,   421,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,   412,   198,   413,   418,   414,   174,
     415,    66,   416,   195,   417,   420,    -1,    -1,    90,   419,
     195,    -1,    -1,    59,   195,    -1,    -1,    -1,    -1,    86,
     422,   424,   423,    87,    -1,   424,   425,    -1,    -1,   183,
      -1,    -1,   428,   426,    85,    -1,    -1,   431,   427,    85,
      -1,    -1,    -1,    44,   198,   429,   344,   430,   364,    -1,
      -1,    -1,    54,   198,   432,   344,   433,   364,    -1,   447,
      -1,   438,    -1,   435,    -1,   437,    -1,   436,    -1,    53,
     198,    -1,    42,    53,   198,    -1,    -1,    -1,    -1,   442,
     445,   439,    86,   440,   182,   441,    87,    -1,    42,    53,
     198,    -1,    43,    53,   198,    -1,    53,   198,    -1,    -1,
     157,   446,   174,    -1,    -1,    -1,    -1,   451,   445,   448,
      86,   449,   177,   450,    87,    -1,   443,    -1,   444,    -1,
      67,    -1,    20,    -1,    21,    -1,    53,    -1,    24,    -1,
       6,    -1,    49,    -1,    23,    -1,    27,    -1,     4,   205,
      -1,   456,   454,    -1,    -1,   454,    91,   455,   456,    -1,
      -1,   452,     3,    -1,    24,    88,     3,    89,     3,    -1,
     459,   458,    -1,   458,    91,   459,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    70,   461,     3,   462,    86,   463,
     465,   464,    87,    -1,   467,   466,    -1,   466,   467,    -1,
      -1,    -1,   401,   468,    85,    -1,    -1,   403,   469,    85,
      -1,    68,   195,     3,    -1,    69,   195,     3,    -1,   473,
     472,    -1,   472,    91,   473,    -1,    -1,   206,    -1,   475,
     478,    -1,    -1,    -1,    71,   476,     3,   477,   390,    -1,
      -1,    -1,    86,   479,   481,   480,    87,    -1,   483,   482,
      -1,   482,   483,    -1,    -1,    -1,   401,   484,    85,    -1,
      -1,   403,   485,    85,    -1,    -1,   317,   486,    85,    -1,
      -1,   470,   487,    85,    -1
};

/* TAO_YYRLINE[TAO_YYN] -- source line where rule number TAO_YYN was defined.  */
static const tao_yytype_uint16 tao_yyrline[] =
{
       0,   360,   360,   363,   364,   368,   371,   374,   380,   384,
     385,   390,   389,   400,   399,   410,   409,   420,   419,   430,
     429,   440,   439,   450,   449,   460,   459,   470,   469,   480,
     479,   490,   489,   500,   499,   510,   509,   520,   519,   534,
     533,   546,   581,   586,   545,   602,   610,   625,   635,   657,
     661,   609,   680,   684,   685,   689,   690,   695,   701,   694,
     759,   765,   758,   823,   824,   829,   868,   873,   828,   891,
     890,   904,   942,   973,  1007,  1006,  1018,  1025,  1026,  1027,
    1028,  1032,  1043,  1048,  1092,  1097,  1047,  1126,  1165,  1170,
    1124,  1189,  1187,  1227,  1226,  1240,  1246,  1253,  1260,  1267,
    1293,  1320,  1385,  1386,  1390,  1391,  1392,  1398,  1397,  1405,
    1404,  1414,  1415,  1420,  1419,  1430,  1429,  1440,  1439,  1450,
    1449,  1460,  1459,  1470,  1469,  1480,  1479,  1490,  1489,  1503,
    1516,  1514,  1542,  1549,  1560,  1559,  1587,  1585,  1612,  1624,
    1670,  1698,  1729,  1734,  1739,  1744,  1728,  1797,  1798,  1799,
    1800,  1801,  1802,  1803,  1815,  1820,  1889,  1891,  1893,  1894,
    1908,  1909,  1923,  1924,  1937,  1938,  1948,  1961,  1962,  1972,
    1985,  1986,  1996,  2006,  2019,  2020,  2030,  2040,  2053,  2104,
    2105,  2114,  2119,  2126,  2134,  2139,  2144,  2150,  2155,  2160,
    2168,  2240,  2239,  2249,  2254,  2259,  2264,  2291,  2300,  2299,
    2370,  2371,  2375,  2383,  2384,  2412,  2413,  2414,  2415,  2416,
    2417,  2418,  2419,  2423,  2424,  2425,  2429,  2430,  2431,  2435,
    2436,  2440,  2453,  2451,  2479,  2486,  2487,  2491,  2504,  2502,
    2530,  2537,  2554,  2573,  2574,  2578,  2583,  2588,  2596,  2601,
    2606,  2614,  2619,  2624,  2632,  2646,  2651,  2659,  2667,  2675,
    2683,  2692,  2691,  2707,  2741,  2746,  2706,  2765,  2768,  2769,
    2773,  2773,  2783,  2788,  2782,  2851,  2850,  2865,  2864,  2879,
    2884,  2889,  2894,  2941,  2946,  2878,  2970,  2978,  2992,  3002,
    3010,  3011,  3119,  3122,  3123,  3128,  3133,  3127,  3169,  3168,
    3182,  3193,  3213,  3221,  3220,  3236,  3241,  3235,  3258,  3257,
    3310,  3334,  3359,  3364,  3397,  3402,  3358,  3428,  3433,  3431,
    3438,  3442,  3479,  3484,  3477,  3551,  3606,  3616,  3605,  3631,
    3636,  3629,  3677,  3703,  3713,  3718,  3711,  3748,  3773,  3782,
    3781,  3817,  3828,  3848,  3856,  3861,  3855,  3898,  3899,  3904,
    3909,  3914,  3919,  3903,  3988,  3993,  3998,  4003,  3987,  4081,
    4086,  4116,  4121,  4080,  4139,  4144,  4197,  4202,  4137,  4239,
    4245,  4252,  4259,  4260,  4272,  4278,  4320,  4271,  4342,  4341,
    4352,  4351,  4364,  4369,  4367,  4374,  4379,  4384,  4378,  4425,
    4424,  4435,  4434,  4447,  4452,  4450,  4457,  4462,  4467,  4461,
    4514,  4522,  4523,  4524,  4629,  4634,  4639,  4648,  4653,  4647,
    4665,  4673,  4678,  4672,  4690,  4698,  4703,  4697,  4715,  4723,
    4728,  4722,  4740,  4747,  4760,  4758,  4784,  4791,  4820,  4858,
    4859,  4863,  4893,  4933,  4938,  4892,  4957,  4962,  4955,  5005,
    5004,  5015,  5022,  5023,  5028,  5027,  5038,  5037,  5048,  5047,
    5058,  5057,  5068,  5067,  5078,  5077,  5088,  5087,  5099,  5158,
    5165,  5191,  5265,  5275,  5281,  5287,  5334,  5381,  5430,  5429,
    5479,  5484,  5489,  5494,  5499,  5504,  5478,  5559,  5558,  5569,
    5576,  5583,  5591,  5596,  5590,  5608,  5609,  5613,  5615,  5614,
    5625,  5624,  5639,  5675,  5637,  5709,  5745,  5707,  5777,  5778,
    5779,  5783,  5784,  5788,  5816,  5847,  5892,  5897,  5845,  5914,
    5924,  5943,  5955,  5954,  5994,  6044,  6049,  5992,  6066,  6071,
    6079,  6084,  6089,  6094,  6099,  6104,  6109,  6114,  6119,  6124,
    6133,  6168,  6167,  6189,  6196,  6220,  6238,  6249,  6269,  6276,
    6287,  6292,  6297,  6302,  6286,  6317,  6324,  6329,  6336,  6335,
    6344,  6343,  6354,  6423,  6473,  6489,  6503,  6510,  6570,  6575,
    6580,  6574,  6640,  6645,  6639,  6660,  6664,  6665,  6670,  6669,
    6680,  6679,  6690,  6689,  6700,  6699
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
  "IDL_USES", "IDL_MANAGES", "IDL_TYPENAME", "IDL_PORT", "IDL_MIRRORPORT",
  "IDL_PORTTYPE", "IDL_CONNECTOR", "IDL_ALIAS", "IDL_INTEGER_LITERAL",
  "IDL_UINTEGER_LITERAL", "IDL_STRING_LITERAL", "IDL_CHARACTER_LITERAL",
  "IDL_FLOATING_PT_LITERAL", "IDL_TRUETOK", "IDL_FALSETOK",
  "IDL_SCOPE_DELIMITOR", "IDL_LEFT_SHIFT", "IDL_RIGHT_SHIFT",
  "IDL_WCHAR_LITERAL", "IDL_WSTRING_LITERAL", "';'", "'{'", "'}'", "'<'",
  "'>'", "':'", "','", "'='", "'|'", "'^'", "'&'", "'+'", "'-'", "'*'",
  "'/'", "'%'", "'~'", "'('", "')'", "'['", "']'", "$accept", "start",
  "definitions", "definition", "at_least_one_fixed_definition",
  "fixed_definitions", "fixed_definition", "@1", "@2", "@3", "@4", "@5",
  "@6", "@7", "@8", "@9", "@10", "@11", "@12", "@13", "@14",
  "module_header", "@15", "fixed_module", "@16", "@17", "@18",
  "template_module_header", "template_module", "@19", "@20", "@21", "@22",
  "@23", "at_least_one_tpl_definition", "tpl_definitions",
  "tpl_definition", "template_module_ref", "@24", "@25",
  "template_module_inst", "@26", "@27", "interface_def", "interface",
  "@28", "@29", "@30", "interface_decl", "@31", "interface_header",
  "inheritance_spec", "@32", "value_def", "valuetype",
  "value_concrete_decl", "@33", "@34", "@35", "value_abs_decl", "@36",
  "@37", "@38", "value_header", "@39", "value_decl", "@40",
  "opt_truncatable", "supports_spec", "value_forward_decl",
  "value_box_decl", "value_elements", "value_element", "state_member",
  "@41", "@42", "exports", "export", "@43", "@44", "@45", "@46", "@47",
  "@48", "@49", "@50", "at_least_one_scoped_name", "scoped_names", "@51",
  "scoped_name", "@52", "@53", "id", "interface_forward", "const_dcl",
  "@54", "@55", "@56", "@57", "const_type", "expression", "const_expr",
  "or_expr", "xor_expr", "and_expr", "shift_expr", "add_expr", "mult_expr",
  "unary_expr", "primary_expr", "literal", "positive_int_expr", "type_dcl",
  "@58", "type_declarator", "@59", "type_spec", "simple_type_spec",
  "base_type_spec", "template_type_spec", "constructed_type_spec",
  "constructed_forward_type_spec", "at_least_one_declarator",
  "declarators", "@60", "declarator", "at_least_one_simple_declarator",
  "simple_declarators", "@61", "simple_declarator", "complex_declarator",
  "integer_type", "signed_int", "unsigned_int", "floating_pt_type",
  "fixed_type", "char_type", "octet_type", "boolean_type", "any_type",
  "object_type", "struct_decl", "@62", "struct_type", "@63", "@64", "@65",
  "at_least_one_member", "members", "member", "@66", "member_i", "@67",
  "@68", "@69", "union_decl", "@70", "union_type", "@71", "@72", "@73",
  "@74", "@75", "@76", "switch_type_spec", "at_least_one_case_branch",
  "case_branches", "case_branch", "@77", "@78", "@79",
  "at_least_one_case_label", "case_labels", "case_label", "@80", "@81",
  "@82", "element_spec", "@83", "struct_forward_type",
  "union_forward_type", "enum_type", "@84", "@85", "@86", "@87",
  "at_least_one_enumerator", "enumerators", "@88", "enumerator",
  "sequence_type_spec", "@89", "@90", "seq_head", "@91", "@92",
  "string_type_spec", "@93", "@94", "string_head", "wstring_type_spec",
  "@95", "@96", "wstring_head", "array_declarator", "@97",
  "at_least_one_array_dim", "array_dims", "array_dim", "@98", "@99",
  "attribute", "attribute_readonly", "@100", "@101", "@102", "@103",
  "attribute_readwrite", "@104", "@105", "@106", "@107", "exception",
  "@108", "@109", "@110", "@111", "operation", "@112", "@113", "@114",
  "@115", "opt_op_attribute", "op_type_spec", "init_decl", "@116", "@117",
  "@118", "init_parameter_list", "@119", "@120",
  "at_least_one_in_parameter", "in_parameters", "@121", "in_parameter",
  "@122", "@123", "parameter_list", "@124", "@125",
  "at_least_one_parameter", "parameters", "@126", "parameter", "@127",
  "@128", "param_type_spec", "direction", "opt_raises", "@129", "@130",
  "opt_getraises", "@131", "@132", "opt_setraises", "@133", "@134",
  "opt_context", "@135", "@136", "at_least_one_string_literal",
  "string_literals", "@137", "typeid_dcl", "typeprefix_dcl", "component",
  "component_forward_decl", "component_decl", "@138", "@139", "@140",
  "component_header", "@141", "@142", "component_inheritance_spec", "@143",
  "component_exports", "component_export", "@144", "@145", "@146", "@147",
  "@148", "@149", "@150", "provides_decl", "interface_type", "uses_decl",
  "uses_opt_multiple", "opt_multiple", "emits_decl", "publishes_decl",
  "consumes_decl", "home_decl", "@151", "home_header", "@152", "@153",
  "@154", "@155", "@156", "@157", "home_inheritance_spec", "@158",
  "primary_key_spec", "home_body", "@159", "@160", "home_exports",
  "home_export", "@161", "@162", "factory_decl", "@163", "@164",
  "finder_decl", "@165", "@166", "event", "event_forward_decl",
  "event_concrete_forward_decl", "event_abs_forward_decl",
  "event_abs_decl", "@167", "@168", "@169", "event_abs_header",
  "event_custom_header", "event_plain_header", "event_rest_of_header",
  "@170", "event_decl", "@171", "@172", "@173", "event_header",
  "formal_parameter_type", "at_least_one_formal_parameter",
  "formal_parameters", "@174", "formal_parameter",
  "at_least_one_formal_parameter_name", "formal_parameter_names",
  "formal_parameter_name", "porttype_decl", "@175", "@176", "@177", "@178",
  "at_least_one_port_export", "port_exports", "port_export", "@179",
  "@180", "extended_port_decl", "at_least_one_actual_parameter",
  "actual_parameters", "actual_parameter", "connector_decl",
  "connector_header", "@181", "@182", "connector_body", "@183", "@184",
  "at_least_one_connector_export", "connector_exports", "connector_export",
  "@185", "@186", "@187", "@188", 0
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
     335,   336,   337,   338,   339,    59,   123,   125,    60,    62,
      58,    44,    61,   124,    94,    38,    43,    45,    42,    47,
      37,   126,    40,    41,    91,    93
};
# endif

/* TAO_YYR1[TAO_YYN] -- Symbol number of symbol that rule TAO_YYN derives.  */
static const tao_yytype_uint16 tao_yyr1[] =
{
       0,   106,   107,   108,   108,   109,   109,   109,   110,   111,
     111,   113,   112,   114,   112,   115,   112,   116,   112,   117,
     112,   118,   112,   119,   112,   120,   112,   121,   112,   122,
     112,   123,   112,   124,   112,   125,   112,   126,   112,   128,
     127,   130,   131,   132,   129,   133,   135,   136,   137,   138,
     139,   134,   140,   141,   141,   142,   142,   144,   145,   143,
     147,   148,   146,   149,   149,   151,   152,   153,   150,   155,
     154,   156,   156,   156,   158,   157,   157,   159,   159,   159,
     159,   160,   160,   162,   163,   164,   161,   166,   167,   168,
     165,   170,   169,   172,   171,   173,   173,   174,   174,   175,
     175,   176,   177,   177,   178,   178,   178,   180,   179,   181,
     179,   182,   182,   184,   183,   185,   183,   186,   183,   187,
     183,   188,   183,   189,   183,   190,   183,   191,   183,   192,
     194,   193,   193,   195,   196,   195,   197,   195,   198,   199,
     199,   199,   201,   202,   203,   204,   200,   205,   205,   205,
     205,   205,   205,   205,   205,   205,   206,   207,   208,   208,
     209,   209,   210,   210,   211,   211,   211,   212,   212,   212,
     213,   213,   213,   213,   214,   214,   214,   214,   215,   215,
     215,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     217,   219,   218,   218,   218,   218,   218,   218,   221,   220,
     222,   222,   223,   223,   223,   224,   224,   224,   224,   224,
     224,   224,   224,   225,   225,   225,   226,   226,   226,   227,
     227,   228,   230,   229,   229,   231,   231,   232,   234,   233,
     233,   235,   236,   237,   237,   238,   238,   238,   239,   239,
     239,   240,   240,   240,   241,   242,   242,   243,   244,   245,
     246,   248,   247,   250,   251,   252,   249,   253,   254,   254,
     256,   255,   258,   259,   257,   260,   257,   262,   261,   264,
     265,   266,   267,   268,   269,   263,   270,   270,   270,   270,
     270,   270,   271,   272,   272,   274,   275,   273,   276,   273,
     277,   278,   278,   280,   279,   281,   282,   279,   284,   283,
     285,   286,   288,   289,   290,   291,   287,   292,   294,   293,
     293,   295,   297,   298,   296,   296,   300,   301,   299,   303,
     304,   302,   302,   305,   307,   308,   306,   306,   309,   311,
     310,   312,   313,   313,   315,   316,   314,   317,   317,   319,
     320,   321,   322,   318,   324,   325,   326,   327,   323,   329,
     330,   331,   332,   328,   334,   335,   336,   337,   333,   338,
     338,   338,   339,   339,   341,   342,   343,   340,   345,   344,
     346,   344,   347,   349,   348,   348,   351,   352,   350,   354,
     353,   355,   353,   356,   358,   357,   357,   360,   361,   359,
     362,   362,   362,   362,   363,   363,   363,   365,   366,   364,
     364,   368,   369,   367,   367,   371,   372,   370,   370,   374,
     375,   373,   373,   376,   378,   377,   377,   379,   380,   381,
     381,   382,   384,   385,   386,   383,   388,   389,   387,   391,
     390,   390,   392,   392,   394,   393,   395,   393,   396,   393,
     397,   393,   398,   393,   399,   393,   400,   393,   401,   402,
     402,   403,   404,   405,   405,   406,   407,   408,   410,   409,
     412,   413,   414,   415,   416,   417,   411,   419,   418,   418,
     420,   420,   422,   423,   421,   424,   424,   425,   426,   425,
     427,   425,   429,   430,   428,   432,   433,   431,   434,   434,
     434,   435,   435,   436,   437,   439,   440,   441,   438,   442,
     443,   444,   446,   445,   448,   449,   450,   447,   451,   451,
     452,   452,   452,   452,   452,   452,   452,   452,   452,   452,
     453,   455,   454,   454,   456,   456,   457,   458,   458,   459,
     461,   462,   463,   464,   460,   465,   466,   466,   468,   467,
     469,   467,   470,   470,   471,   472,   472,   473,   474,   476,
     477,   475,   479,   480,   478,   481,   482,   482,   484,   483,
     485,   483,   486,   483,   487,   483
};

/* TAO_YYR2[TAO_YYN] -- Number of symbols composing right hand side of rule TAO_YYN.  */
static const tao_yytype_uint8 tao_yyr2[] =
{
       0,     2,     1,     2,     0,     1,     2,     2,     2,     2,
       0,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     0,     0,     7,     2,     0,     0,     0,     0,
       0,    11,     2,     2,     0,     1,     2,     0,     0,     8,
       0,     0,     6,     1,     1,     0,     0,     0,     7,     0,
       3,     2,     3,     3,     0,     4,     0,     1,     1,     1,
       1,     2,     1,     0,     0,     0,     7,     0,     0,     0,
       8,     0,     4,     0,     3,     1,     0,     2,     0,     2,
       1,     2,     2,     0,     1,     1,     2,     0,     3,     0,
       3,     2,     0,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     2,
       0,     4,     0,     1,     0,     3,     0,     4,     1,     1,
       2,     2,     0,     0,     0,     0,     9,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     1,     2,     2,     2,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     1,     1,     1,     2,     1,     0,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     0,     4,     0,     1,     1,     2,     0,     4,
       0,     1,     1,     1,     1,     1,     2,     1,     2,     3,
       2,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     0,     0,     0,     7,     2,     2,     0,
       0,     2,     0,     0,     5,     0,     3,     0,     3,     0,
       0,     0,     0,     0,     0,    14,     1,     1,     1,     1,
       1,     1,     2,     2,     0,     0,     0,     5,     0,     3,
       2,     2,     0,     0,     3,     0,     0,     5,     0,     3,
       1,     1,     0,     0,     0,     0,     9,     2,     0,     4,
       0,     1,     0,     0,     6,     2,     0,     0,     5,     0,
       0,     6,     1,     1,     0,     0,     6,     1,     1,     0,
       3,     2,     2,     0,     0,     0,     5,     1,     1,     0,
       0,     0,     0,     9,     0,     0,     0,     0,     9,     0,
       0,     0,     0,     9,     0,     0,     0,     0,    10,     1,
       1,     0,     1,     1,     0,     0,     0,     7,     0,     3,
       0,     4,     2,     0,     4,     0,     0,     0,     5,     0,
       3,     0,     4,     2,     0,     4,     0,     0,     0,     5,
       1,     1,     1,     1,     1,     1,     1,     0,     0,     6,
       0,     0,     0,     6,     0,     0,     0,     6,     0,     0,
       0,     6,     0,     2,     0,     4,     0,     3,     3,     1,
       1,     2,     0,     0,     0,     7,     0,     0,     6,     0,
       3,     0,     2,     0,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     3,     1,
       1,     3,     2,     1,     0,     3,     3,     3,     0,     3,
       0,     0,     0,     0,     0,     0,    13,     0,     3,     0,
       2,     0,     0,     0,     5,     2,     0,     1,     0,     3,
       0,     3,     0,     0,     6,     0,     0,     6,     1,     1,
       1,     1,     1,     2,     3,     0,     0,     0,     8,     3,
       3,     2,     0,     3,     0,     0,     0,     8,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     0,     4,     0,     2,     5,     2,     3,     0,     1,
       0,     0,     0,     0,     9,     2,     2,     0,     0,     3,
       0,     3,     3,     3,     2,     3,     0,     1,     2,     0,
       0,     5,     0,     0,     5,     2,     2,     0,     0,     3,
       0,     3,     0,     3,     0,     3
};

/* TAO_YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when TAO_YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const tao_yytype_uint16 tao_yydefact[] =
{
       4,     0,     0,     1,    37,   142,    39,    69,   191,   251,
     267,   302,   349,     0,     0,     0,     0,    93,     0,     0,
     460,     0,     0,   530,   549,     3,     5,    41,    23,    60,
       0,     0,    21,    63,    76,    65,    25,    77,    82,    78,
      83,    76,    79,    80,    64,    17,    11,   197,   253,   193,
     301,   194,   219,   220,   195,    19,    13,    15,    27,   420,
     419,   422,    29,   458,    31,   490,   492,   491,   489,    76,
     508,   509,   488,    76,    33,    35,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   138,   231,   196,    76,
       0,    76,    87,    76,     0,    81,    76,     0,   426,   501,
       0,   134,     0,   133,     0,     0,     0,    45,     0,     0,
       0,     0,     6,     7,     0,    96,    71,     0,     0,     0,
     235,   237,     0,   241,   242,   245,   246,   247,   248,   244,
     249,   250,   316,   323,   328,    91,   204,   101,   200,   202,
     203,   201,   205,   233,   234,   206,   210,   207,   209,   208,
     211,   212,   253,   216,     0,   217,   218,   213,     0,   214,
     322,   215,   327,     0,     0,     0,   269,     0,     0,     0,
       0,     0,     0,     0,     0,   502,   495,   504,     0,     0,
     552,   548,    38,   155,   143,   147,   151,   152,   148,   149,
     150,   153,   154,    40,    70,   192,   198,   252,   268,   303,
     350,    72,   499,    73,     0,   500,    94,   431,   461,     0,
     417,   136,   418,   531,   550,    42,    24,     0,   515,   511,
     512,   517,   514,   518,   516,   513,   510,     0,    47,   523,
     181,   182,   183,   185,   187,   188,   189,   186,   184,     0,
       0,     0,     0,   178,   547,   156,   157,   158,   160,   162,
     164,   167,   170,   174,   179,     0,   546,    22,    95,    74,
      66,    26,    84,   236,   243,   238,   240,     0,    98,   315,
     312,   319,   324,    18,    12,   254,     0,    20,    14,    16,
      28,   423,    30,   472,   459,    32,    98,     0,     0,    34,
      36,     0,     0,     0,     0,     0,    88,   429,   427,   469,
     135,     0,     0,   431,    10,   519,     0,   524,     0,   520,
     175,   176,   177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    61,   544,     0,   112,   103,   239,
     317,     0,    92,     0,     0,     0,   260,   270,   433,   476,
     503,   496,   505,   339,   344,     0,   454,     0,     0,   562,
     337,   338,   558,   560,     0,   564,   553,   557,   144,   231,
     199,   224,   225,   226,   232,   304,   351,   112,     0,    98,
     467,   462,   137,   532,   551,    43,     0,     0,    48,   521,
     180,   159,   161,   163,   165,   166,   168,   169,   171,   172,
     173,     0,     0,    75,   132,     0,     0,     0,    97,   190,
     313,   320,   325,   255,   259,     0,     0,   424,     0,   112,
     103,     0,     0,   450,   449,     0,   453,   452,     0,     0,
       0,     0,     0,     0,     0,     0,   555,     0,     0,   221,
       0,   259,     0,   430,   428,     0,    98,     0,     0,     9,
      41,     0,     0,     0,    62,   545,   129,   127,   359,   360,
       0,   111,   119,   113,   123,   121,   125,     0,   115,   117,
     364,   109,   107,     0,   102,   104,   105,     0,   318,     0,
       0,     0,     0,   260,   265,   262,   261,   235,   281,   276,
     277,   278,   279,   271,   280,     0,     0,     0,   444,     0,
     432,   434,   436,   438,   440,   442,   446,     0,     0,   477,
       0,   475,   478,   480,     0,     0,   340,   393,   390,   391,
     392,   345,   448,   542,   543,   563,   559,   561,   451,   565,
     554,   556,   145,   334,   330,   333,   222,   311,   305,   310,
     260,     0,   468,   463,   538,   540,   533,   537,    44,   525,
      49,   522,   130,     0,    68,     0,     0,     0,     0,     0,
     363,   354,   362,     0,     0,     0,     0,     0,    86,   106,
     314,   321,   326,   256,   258,     0,     0,     0,     0,     0,
       0,     0,   425,     0,     0,     0,     0,     0,     0,   482,
     485,   474,     0,     0,     0,     0,     0,     0,     0,     0,
     331,     0,     0,   307,     0,    90,     0,     0,     0,     0,
     535,     0,     0,   128,   120,   114,   124,   122,   126,     0,
     116,   118,   365,   110,   108,   266,   263,   272,   457,   455,
     456,   445,   435,   437,   439,   441,   443,   447,     0,     0,
     479,   481,   498,   507,   341,   346,   230,   146,   335,   332,
     223,   306,   308,   353,   464,   539,   541,   534,   536,     0,
      55,    50,    54,     0,   131,   355,     0,     0,     0,   368,
     483,   486,     0,   404,   227,     0,     0,     0,    57,     0,
       0,    56,     0,   366,   264,   273,     0,     0,   400,   400,
     342,   401,   347,   228,   336,   309,   465,     0,    51,    53,
     381,   356,   400,     0,   369,   376,     0,   375,   397,   484,
     487,   400,     0,   408,     0,   471,     0,     0,     0,   400,
     367,   288,   295,   293,   274,   284,   285,   292,     0,   371,
     372,     0,   343,   402,   405,   348,   229,     0,   466,   529,
       0,   528,   380,   394,   395,   396,     0,   386,   387,   357,
       0,     0,     0,     0,     0,     0,   290,   377,   373,   398,
       0,     0,   470,    58,   526,   382,   383,     0,   412,   289,
     296,   294,   275,   283,   298,   286,   291,     0,     0,     0,
       0,   406,     0,     0,   384,   388,   409,   358,     0,     0,
       0,   378,   374,     0,   403,     0,    59,   527,     0,     0,
       0,   297,   299,   287,   399,     0,   385,   389,   410,   407,
       0,   416,     0,   413,   411,   414,     0,   415
};

/* TAO_YYDEFGOTO[NTERM-NUM].  */
static const tao_yytype_int16 tao_yydefgoto[] =
{
      -1,     1,     2,    25,   375,   376,   650,   164,   168,   169,
     163,   167,   114,   109,   118,   170,   172,   174,   178,   179,
      77,   440,    79,    28,   108,   304,   438,    29,    30,   110,
     308,   442,   601,   669,   651,   670,   652,   653,   687,   772,
      31,   111,   391,    32,    33,   117,   327,   450,    34,    80,
      35,   135,   326,    36,    37,    38,   119,   328,   463,    39,
     204,   367,   531,    40,   268,    41,    97,   259,   332,    42,
      43,   396,   464,   465,   557,   556,   395,   451,   546,   553,
     554,   545,   548,   547,   549,   543,   393,   446,   602,   243,
     209,   301,   103,    44,   452,    78,   292,   427,   588,   184,
     244,   399,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   400,   453,    81,   195,   293,   475,   138,   139,   140,
     141,    47,   360,   429,   591,   361,   635,   664,   704,   362,
     363,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,    48,    82,    49,   165,   336,   472,   403,   473,   564,
     405,   476,   566,   657,   565,    50,    83,    51,   276,   406,
     567,   658,   693,   743,   483,   714,   744,   715,   745,   780,
     740,   716,   746,   717,   742,   741,   778,   765,   779,    52,
      53,    54,    84,   294,   430,   592,   528,   593,   666,   529,
     157,   333,   469,   158,   267,   397,   159,   334,   470,   160,
     161,   335,   471,   162,   364,   428,   524,   590,   525,   589,
     665,   454,   350,   411,   586,   662,   701,   351,   412,   587,
     663,   703,   455,    85,   295,   431,   594,   456,   609,   672,
     709,   758,   457,   551,   467,   555,   656,   692,   660,   676,
     677,   696,   720,   768,   697,   718,   767,   691,   707,   708,
     736,   756,   788,   737,   757,   789,   511,   738,   699,   721,
     769,   682,   702,   750,   725,   751,   785,   777,   790,   800,
     802,   803,   806,   458,   459,    58,    59,    60,   171,   338,
     489,    61,   207,   369,   298,   368,   407,   490,   573,   574,
     575,   576,   577,   571,   578,   352,   415,   353,   354,   417,
     493,   494,   495,    62,   173,    63,   100,   299,   436,   596,
     667,   705,   371,   435,   728,   284,   339,   500,   408,   501,
     582,   583,   502,   628,   678,   503,   629,   679,    64,    65,
      66,    67,    68,   287,   409,   584,    69,    70,    71,   176,
     286,    72,   288,   410,   585,    73,   227,   228,   309,   443,
     229,   730,   754,   731,    74,   105,   302,   437,   599,   536,
     600,   537,   597,   598,   355,   255,   325,   256,    75,    76,
     106,   303,   181,   291,   425,   356,   426,   357,   421,   422,
     420,   424
};

/* TAO_YYPACT[STATE-NUM] -- Index in TAO_YYTABLE of the portion describing
   STATE-NUM.  */
#define TAO_YYPACT_NINF -591
static const tao_yytype_int16 tao_yypact[] =
{
    -591,    54,  1115,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,    42,    51,    59,    26,  -591,    42,    42,
    -591,    38,    38,  -591,  -591,  -591,  -591,   -12,  -591,   292,
     -17,    -7,  -591,  -591,    -5,  -591,  -591,  -591,  -591,  -591,
    -591,    80,  -591,  -591,  -591,  -591,  -591,  -591,     1,  -591,
      85,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,    25,
    -591,  -591,  -591,    25,  -591,  -591,    35,    63,  1092,    38,
      42,   483,    42,    42,    42,    42,  -591,  -591,  -591,    46,
      42,    52,  -591,    72,    42,  -591,    25,    42,    69,    71,
      42,  -591,    60,  -591,    96,   131,   141,  -591,    91,    73,
     697,    49,  -591,  -591,    94,   145,  -591,   116,   121,   126,
     130,  -591,   215,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,   137,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,    85,  -591,  -591,  -591,    22,  -591,
     138,  -591,   161,   166,   179,   180,  -591,   185,   189,   193,
     198,   203,   207,   211,   214,  -591,  -591,  -591,   216,   217,
    -591,  -591,  -591,   137,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,   137,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,   218,  -591,   219,  -591,  -591,   220,  -591,    42,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  1092,  -591,  -591,
    -591,  -591,   229,  -591,  -591,  -591,  -591,   315,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,   111,
     111,   111,    49,   137,  -591,  -591,   228,   237,   238,   151,
     159,    82,  -591,  -591,  -591,   243,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,   327,  -591,   235,   289,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,   240,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,   289,   251,   263,  -591,
    -591,   335,    42,    42,   264,   265,  -591,  -591,  -591,   250,
    -591,    42,   271,   220,  -591,  -591,   355,  -591,   272,   269,
    -591,  -591,  -591,   259,    49,    49,    49,    49,    49,    49,
      49,    49,    49,    49,  -591,   273,    38,  -591,  -591,  -591,
    -591,    38,  -591,    49,    49,    49,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,    37,   309,    38,    38,  -591,
    -591,  -591,  -591,  -591,    37,  -591,  -591,  -591,  -591,   266,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,    38,   289,
    -591,  -591,  -591,  -591,  -591,  -591,  1193,   283,  -591,  -591,
    -591,   237,   238,   151,   159,   159,    82,    82,  -591,  -591,
    -591,    42,    49,  -591,   137,   886,   227,   437,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,   408,   195,   278,   828,  -591,
    -591,   343,   654,  -591,   137,    42,  -591,  -591,    39,    40,
     291,   295,   296,    42,   297,   307,   335,   304,   279,   293,
     395,  -591,   921,   137,  -591,    38,   289,   113,   314,   318,
    -591,   403,   321,   697,  -591,  -591,   317,  -591,  -591,  -591,
     325,  -591,  -591,  -591,  -591,  -591,  -591,   576,  -591,  -591,
    -591,  -591,  -591,   326,  -591,  -591,  -591,   329,  -591,   341,
     346,   347,   351,   352,  -591,  -591,  -591,   434,   137,  -591,
    -591,  -591,  -591,  -591,  -591,    38,    38,    38,  -591,   356,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,    42,    42,  -591,
     357,  -591,  -591,  -591,   979,   770,  -591,   137,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
     370,   371,   137,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,   374,  -591,   379,   383,   384,   385,   386,
    -591,  -591,  -591,   387,   388,   471,   408,   408,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,   390,    42,   375,    50,    50,
      50,   392,  -591,   394,   396,   397,   398,   402,   405,  -591,
    -591,  -591,   420,   425,   393,   424,   654,    42,    49,    49,
     279,    42,   426,   421,   427,  -591,   449,   431,   436,   442,
     113,  1140,    38,  -591,  -591,  -591,  -591,  -591,  -591,   529,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,   432,   432,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,    38,
    -591,  -591,  -591,   448,   137,  -591,   432,   450,   452,   504,
    -591,  -591,    42,   485,   451,   438,   395,    38,   137,   454,
    1027,  -591,   443,  -591,  -591,  -591,   441,   511,   509,   509,
    -591,  -591,  -591,  -591,  -591,  -591,   137,   460,  -591,  -591,
     446,  -591,   509,    45,  -591,  -591,   447,  -591,  -591,  -591,
    -591,   509,   456,   489,    42,   493,   551,   457,   164,   509,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,   654,  -591,
     468,   459,  -591,  -591,  -591,  -591,  -591,    38,  -591,  -591,
     473,  -591,  -591,  -591,  -591,  -591,   461,  -591,  -591,  -591,
     480,    49,   477,   481,    81,   483,   234,  -591,  -591,  -591,
      38,   467,   137,  -591,   479,  -591,   482,   654,   532,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,    42,   511,    38,
     469,  -591,    42,   551,  -591,  -591,  -591,  -591,   486,    42,
     490,  -591,  -591,   478,  -591,    38,  -591,  -591,   164,    42,
     494,  -591,  -591,  -591,  -591,   495,  -591,  -591,  -591,  -591,
     505,  -591,   496,   492,  -591,  -591,   522,  -591
};

/* TAO_YYPGOTO[NTERM-NUM].  */
static const tao_yytype_int16 tao_yypgoto[] =
{
    -591,  -591,  -591,  -591,  -591,  -591,    21,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,   598,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,   -67,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,   254,  -591,
    -591,   231,  -591,  -591,  -591,   588,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,   590,  -591,   261,  -591,  -591,  -253,  -591,
    -591,   196,  -591,  -591,  -591,  -591,  -307,  -358,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -309,  -591,  -591,   -20,
    -591,  -591,   -13,  -591,    11,  -591,  -591,  -591,  -591,   391,
      19,   -99,  -591,   300,   294,   301,   -36,   -25,  -107,   -19,
    -591,  -305,    12,  -591,  -591,  -591,   -33,   222,  -394,  -591,
    -591,  -591,    55,  -591,  -591,  -560,   -40,  -591,  -591,   -10,
    -591,   -54,  -591,  -591,   -43,   -42,   -53,   -51,   -50,  -591,
    -591,   -34,  -591,   -32,  -591,  -591,  -591,  -591,   192,   275,
    -591,  -271,  -591,  -591,  -591,   -30,  -591,   -26,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -124,  -591,  -591,
    -591,  -591,  -591,  -122,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,   -37,  -591,  -591,  -591,  -591,  -591,  -591,  -591,   -41,
    -591,  -591,  -591,  -591,  -591,  -591,   -68,  -591,  -591,  -591,
     -58,  -591,  -591,  -591,  -591,  -591,  -591,  -591,    36,  -591,
    -591,  -254,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,    14,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -590,  -591,
    -591,  -591,  -591,  -591,  -141,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -160,  -591,  -591,  -431,  -591,  -540,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,    15,    17,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,   328,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -375,   276,  -373,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,   559,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,  -591,
     190,  -591,  -591,  -139,  -591,  -591,  -591,  -591,  -591,  -591,
    -591,    41,  -591,  -591,   230,  -591,  -591,   244,  -591,  -591,
    -591,  -591,  -591,  -591,  -591,  -591,  -591,   209,  -591,  -591,
    -591,  -591
};

/* TAO_YYTABLE[TAO_YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what TAO_YYDEFACT says.
   If TAO_YYTABLE_NINF, syntax error.  */
#define TAO_YYTABLE_NINF -507
static const tao_yytype_int16 tao_yytable[] =
{
      87,   102,   104,    88,   156,    98,    99,   152,   137,   153,
     191,   154,   245,    45,    46,   155,    55,    56,   508,    57,
     192,   136,   398,    26,   185,   188,   552,   189,   190,   401,
     402,   640,   491,   340,   492,   186,   187,   349,   466,   661,
      86,    86,   513,   514,   156,    86,   711,   152,   196,   153,
     499,   154,    86,    86,     3,   155,   413,     7,   183,   193,
     432,   136,   534,   508,   535,     7,   673,   194,   112,   197,
     198,   199,   200,   712,   713,    17,   107,   202,   113,    94,
    -139,   205,   711,    86,   206,   115,  -300,   208,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
       9,    10,   504,    11,   132,   133,   134,   166,    17,   712,
     713,   269,    90,   270,    86,   115,   434,   101,   101,   211,
     211,   180,   230,   231,   232,   233,   234,   235,   236,   101,
     211,  -140,   237,   238,   213,   210,   115,  -141,   263,   700,
     211,   264,   115,   313,   214,   239,   240,   466,   182,   191,
     241,   242,   710,   488,  -421,   634,  -493,   -99,   216,   192,
     101,   722,   115,   185,   188,  -100,   189,   190,  -282,   739,
     115,   212,   349,   345,   186,   187,   211,   215,   346,   257,
     321,   322,   323,   533,   230,   231,   232,   233,   234,   235,
     236,   101,   508,   258,   237,   238,   300,   183,    86,   733,
     734,   735,   260,   477,   121,   122,   261,   781,   125,   126,
     127,   128,   262,   242,   388,   389,   390,   211,    11,   792,
     310,   311,   312,   265,   266,   534,   271,   535,   447,   797,
    -361,     5,   317,   318,     8,  -361,  -361,  -361,  -361,  -361,
    -361,  -361,  -361,  -361,  -361,  -361,  -361,     9,    10,   272,
      11,   273,  -361,  -361,    12,   319,   320,   343,   344,   448,
     449,  -361,   712,   713,   274,   116,   275,    13,    89,    91,
     277,   460,   461,   462,   278,   101,    93,    96,   279,   358,
     359,   384,   385,   280,   638,   613,   614,   747,   372,   281,
      21,    22,   282,   245,   386,   387,   -46,   283,   -46,   285,
     175,   289,   290,  -494,   175,   296,   394,  -361,   343,   344,
     297,   394,   -46,   -46,   -85,   -46,   -46,   306,   307,   -46,
     201,   314,   203,   330,   508,   414,   775,   418,   419,   485,
     486,   315,   324,   316,   414,   329,   331,   341,   345,   487,
     370,   -46,   337,   346,   509,   -46,   347,   348,   433,   342,
     365,   366,   479,   480,   510,   481,   482,   373,   377,   -46,
     379,   378,   380,   508,   392,   343,   344,   416,   156,   484,
    -329,   152,   441,   153,   506,   154,   515,   136,   444,   155,
     516,   517,   519,   523,   526,   136,   478,    45,    46,   509,
      55,    56,   507,    57,   520,   345,   522,   439,   527,   510,
     346,   538,   512,   347,   348,    -8,   539,   540,   542,   474,
     518,    86,   544,   558,   559,   532,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,     9,    10,
     560,    11,   132,   133,   134,   561,   562,   507,   563,  -257,
      86,   770,   263,   572,   581,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,  -352,   595,   603,
     783,   132,   133,   134,   604,   568,   569,   570,   605,   606,
     607,   608,   610,   611,   612,   615,   795,   621,   617,   622,
     632,   623,   624,   625,   579,   580,    86,   626,   101,   245,
     627,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,     9,    10,   630,    11,   132,   133,   134,
     631,   633,   642,   641,   643,   644,   645,   101,   509,   156,
     156,   646,   152,   152,   153,   153,   154,   154,   510,   647,
     155,   155,   655,   671,   659,   674,   136,   136,   675,  -370,
     681,   688,   683,   684,   694,   690,   695,   698,   706,  -379,
     719,   724,   727,   359,   729,   618,   619,   620,   723,   748,
     732,   749,   753,   101,   755,   759,   507,   761,   762,   771,
     773,   776,   784,   774,    87,   793,   791,   636,   359,    86,
     801,   794,   654,   805,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   798,   807,   799,   804,
      27,   133,   134,   689,    95,    92,   505,   637,   305,   382,
     550,   404,    45,    46,   381,    55,    56,   383,    57,   468,
     763,   616,   680,   530,   766,   685,   639,   782,   796,   668,
     423,   374,   177,   541,   787,   521,   445,   496,     0,     0,
       0,   648,   760,     0,     0,     0,     0,   686,     0,    87,
     509,     0,   636,     0,     0,     0,   101,    86,     0,     0,
     510,     0,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,     0,     0,     0,     0,     0,   133,
     134,    45,    46,     0,    55,    56,     0,    57,     0,   509,
       0,    87,     0,     0,   726,     0,     0,     0,   507,   510,
       0,   217,     0,   218,     0,     0,     0,   752,   156,     0,
       0,   152,   764,   153,     0,   154,     0,   219,   220,   155,
     221,   222,     0,     0,   223,   136,     0,     0,     0,     0,
     394,     0,     0,     0,   101,     0,     0,   507,     0,     0,
       0,     0,     0,     0,     0,     0,   224,     0,     0,   394,
     225,     0,     0,     0,   359,     0,     0,     0,     0,   786,
       0,     0,     0,     0,   226,   394,   359,     0,     0,     0,
       0,   447,     0,  -361,     5,     0,   359,     8,  -361,  -361,
    -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,
       9,    10,     0,    11,     0,  -361,  -361,    12,     0,     0,
     343,   344,   448,   449,  -361,     0,     0,     0,     0,     0,
      13,     0,     0,     0,   460,   461,   462,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   447,
       0,  -361,     5,    21,    22,     8,  -361,  -361,  -361,  -361,
    -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,     9,    10,
    -361,    11,     0,  -361,  -361,    12,     0,  -506,   343,   344,
     448,   449,  -361,     0,     0,     0,     0,     0,    13,     0,
       0,     0,   497,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   498,     0,     0,     0,     0,   447,     0,  -361,
       5,    21,    22,     8,  -361,  -361,  -361,  -361,  -361,  -361,
    -361,  -361,  -361,  -361,  -361,  -361,     9,    10,  -361,    11,
       0,  -361,  -361,    12,     0,  -473,   343,   344,   448,   449,
    -361,     0,   447,     0,  -361,     5,    13,     0,     8,  -361,
    -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,
    -361,     9,    10,     0,    11,     0,  -361,  -361,    12,    21,
      22,   343,   344,   448,   449,  -361,     0,     0,     0,     0,
       0,    13,     0,     0,     0,     0,  -361,     0,     0,     0,
       0,     0,     0,   -67,     0,     0,     0,     0,     0,     0,
     447,     0,  -361,     5,    21,    22,     8,  -361,  -361,  -361,
    -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,     9,
      10,  -361,    11,     0,  -361,  -361,    12,     0,   -89,   343,
     344,   448,   449,  -361,     0,     0,     0,     0,     0,    13,
       0,     0,     0,     0,     0,     0,     0,     0,     4,     0,
       0,     5,     6,     7,     8,     0,     0,     0,     0,     0,
       0,     0,    21,    22,     0,     0,     0,     9,    10,     0,
      11,     0,     0,     0,    12,     0,     0,     0,     0,  -361,
       0,     0,     0,     0,     0,     0,  -497,    13,    14,    15,
      16,     0,     0,     0,     0,     0,    17,    18,     0,     0,
      19,     0,     0,    20,     0,     0,     0,     0,     0,     0,
      21,    22,     0,     0,     0,    86,     0,    23,    24,   649,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
       0,     0,     0,     0,   -52,    -2,     4,   133,   134,     5,
       6,     7,     8,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     9,    10,     0,    11,     0,
       0,     4,    12,     0,     5,     6,     7,     8,     0,     0,
       0,     0,     0,     0,     0,    13,    14,    15,    16,     0,
       9,    10,     0,    11,    17,    18,     0,    12,    19,     0,
       0,    20,   101,     0,     0,     0,     0,     0,    21,    22,
      13,    14,    15,    16,     0,    23,    24,     0,     0,    17,
      18,     0,     0,    19,     4,     0,    20,     5,     6,     7,
       8,     0,     0,    21,    22,     0,     0,     0,     0,     0,
      23,    24,   649,     9,    10,     0,    11,     0,     0,     0,
      12,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    13,    14,    15,    16,     0,     0,     0,
       0,     0,    17,    18,     0,     0,    19,     0,     0,    20,
       0,     0,     0,     0,     0,     0,    21,    22,     0,     0,
       0,     0,     0,    23,    24
};

static const tao_yytype_int16 tao_yycheck[] =
{
      13,    21,    22,    13,    41,    18,    19,    41,    41,    41,
      78,    41,   111,     2,     2,    41,     2,     2,   412,     2,
      78,    41,   331,     2,    78,    78,   457,    78,    78,   334,
     335,   591,   407,   286,   407,    78,    78,   291,   396,   629,
       3,     3,     3,     3,    81,     3,     1,    81,    81,    81,
     408,    81,     3,     3,     0,    81,    19,     6,    78,    79,
     367,    81,   437,   457,   437,     6,   656,    80,    85,    82,
      83,    84,    85,    28,    29,    49,    88,    90,    85,    53,
      85,    94,     1,     3,    97,    90,    85,   100,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   409,    23,    24,    25,    26,    22,    49,    28,
      29,    89,    53,    91,     3,    90,   369,    80,    80,    80,
      80,    86,    73,    74,    75,    76,    77,    78,    79,    80,
      80,    85,    83,    84,     3,    75,    90,    85,     8,   679,
      80,    11,    90,   242,     3,    96,    97,   505,    85,   217,
     101,   102,   692,   407,    85,   586,    85,    85,    85,   217,
      80,   701,    90,   217,   217,    85,   217,   217,    87,   709,
      90,    75,   426,    60,   217,   217,    80,    86,    65,    85,
      98,    99,   100,   436,    73,    74,    75,    76,    77,    78,
      79,    80,   586,    48,    83,    84,   209,   217,     3,    35,
      36,    37,    86,     8,     9,    10,    85,   767,    13,    14,
      15,    16,    86,   102,   321,   322,   323,    80,    23,   779,
     239,   240,   241,     8,     9,   600,    88,   600,     1,   789,
       3,     4,    81,    82,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    88,
      23,    85,    25,    26,    27,    96,    97,    30,    31,    32,
      33,    34,    28,    29,    85,    34,    86,    40,    14,    15,
      85,    44,    45,    46,    85,    80,    15,    16,    85,   292,
     293,   317,   318,    85,   589,   556,   557,   718,   301,    86,
      63,    64,    85,   392,   319,   320,     4,    86,     6,    85,
      69,    85,    85,    85,    73,    86,   326,    80,    30,    31,
      90,   331,    20,    21,    87,    23,    24,    88,     3,    27,
      89,    93,    91,    88,   718,   345,   757,   347,   348,    51,
      52,    94,    89,    95,   354,     8,    47,    86,    60,    61,
      90,    49,   102,    65,   412,    53,    68,    69,   368,    86,
      86,    86,   406,   406,   412,   406,   406,    86,     3,    67,
      91,    89,   103,   757,    91,    30,    31,    58,   405,   406,
     104,   405,    89,   405,    31,   405,    85,   397,   391,   405,
      85,    85,    85,   104,    91,   405,   406,   376,   376,   457,
     376,   376,   412,   376,    87,    60,    92,   376,     3,   457,
      65,    87,   415,    68,    69,    87,     3,    86,    91,     1,
     423,     3,    87,    87,    85,   435,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      89,    23,    24,    25,    26,    89,    89,   457,    87,    87,
       3,   750,     8,    87,    87,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    87,    87,    85,
     769,    24,    25,    26,    85,   485,   486,   487,    85,    85,
      85,    85,    85,    85,     3,    85,   785,    85,   103,    85,
      87,    85,    85,    85,   497,   498,     3,    85,    80,   588,
      85,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    85,    23,    24,    25,    26,
      85,    87,    91,    87,    87,    66,    85,    80,   586,   556,
     557,    85,   556,   557,   556,   557,   556,   557,   586,    87,
     556,   557,     3,    85,   102,    85,   556,   557,    86,    35,
      55,    87,    91,   105,   103,   102,    35,    38,    88,   103,
     103,    62,    59,   566,     3,   568,   569,   570,   102,    91,
     103,   102,    89,    80,   103,    85,   586,    90,    87,   102,
      91,    39,   103,    91,   587,    85,    90,   587,   591,     3,
      75,   103,   602,    91,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,   102,    75,   103,   103,
       2,    25,    26,   670,    16,    15,   410,   588,   217,   315,
      34,   336,   601,   601,   314,   601,   601,   316,   601,   397,
     744,   566,   662,   431,   746,   666,   590,   768,   788,   649,
     354,   303,    73,   443,   773,   426,   392,   407,    -1,    -1,
      -1,   600,   741,    -1,    -1,    -1,    -1,   667,    -1,   662,
     718,    -1,   662,    -1,    -1,    -1,    80,     3,    -1,    -1,
     718,    -1,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    -1,    -1,    25,
      26,   670,   670,    -1,   670,   670,    -1,   670,    -1,   757,
      -1,   704,    -1,    -1,   704,    -1,    -1,    -1,   718,   757,
      -1,     4,    -1,     6,    -1,    -1,    -1,   727,   745,    -1,
      -1,   745,   745,   745,    -1,   745,    -1,    20,    21,   745,
      23,    24,    -1,    -1,    27,   745,    -1,    -1,    -1,    -1,
     750,    -1,    -1,    -1,    80,    -1,    -1,   757,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,   769,
      53,    -1,    -1,    -1,   767,    -1,    -1,    -1,    -1,   772,
      -1,    -1,    -1,    -1,    67,   785,   779,    -1,    -1,    -1,
      -1,     1,    -1,     3,     4,    -1,   789,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    23,    -1,    25,    26,    27,    -1,    -1,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    44,    45,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,     4,    63,    64,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      80,    23,    -1,    25,    26,    27,    -1,    87,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,     1,    -1,     3,
       4,    63,    64,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    80,    23,
      -1,    25,    26,    27,    -1,    87,    30,    31,    32,    33,
      34,    -1,     1,    -1,     3,     4,    40,    -1,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    -1,    25,    26,    27,    63,
      64,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,     3,     4,    63,    64,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    80,    23,    -1,    25,    26,    27,    -1,    87,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
      -1,     4,     5,     6,     7,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    20,    21,    -1,
      23,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    40,    41,    42,
      43,    -1,    -1,    -1,    -1,    -1,    49,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,     3,    -1,    70,    71,    72,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      -1,    -1,    -1,    -1,    87,     0,     1,    25,    26,     4,
       5,     6,     7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    20,    21,    -1,    23,    -1,
      -1,     1,    27,    -1,     4,     5,     6,     7,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    40,    41,    42,    43,    -1,
      20,    21,    -1,    23,    49,    50,    -1,    27,    53,    -1,
      -1,    56,    80,    -1,    -1,    -1,    -1,    -1,    63,    64,
      40,    41,    42,    43,    -1,    70,    71,    -1,    -1,    49,
      50,    -1,    -1,    53,     1,    -1,    56,     4,     5,     6,
       7,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,
      70,    71,    72,    20,    21,    -1,    23,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    41,    42,    43,    -1,    -1,    -1,
      -1,    -1,    49,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    70,    71
};

/* TAO_YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const tao_yytype_uint16 tao_yystos[] =
{
       0,   107,   108,     0,     1,     4,     5,     6,     7,    20,
      21,    23,    27,    40,    41,    42,    43,    49,    50,    53,
      56,    63,    64,    70,    71,   109,   112,   127,   129,   133,
     134,   146,   149,   150,   154,   156,   159,   160,   161,   165,
     169,   171,   175,   176,   199,   200,   218,   227,   247,   249,
     261,   263,   285,   286,   287,   328,   379,   380,   381,   382,
     383,   387,   409,   411,   434,   435,   436,   437,   438,   442,
     443,   444,   447,   451,   460,   474,   475,   126,   201,   128,
     155,   219,   248,   262,   288,   329,     3,   198,   235,   154,
      53,   154,   169,   171,    53,   161,   171,   172,   198,   198,
     412,    80,   195,   198,   195,   461,   476,    88,   130,   119,
     135,   147,    85,    85,   118,    90,   157,   151,   120,   162,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    24,    25,    26,   157,   195,   222,   223,   224,
     225,   226,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   249,   261,   263,   287,   296,   299,   302,
     305,   306,   309,   116,   113,   250,    22,   117,   114,   115,
     121,   384,   122,   410,   123,   157,   445,   445,   124,   125,
      86,   478,    85,   195,   205,   237,   240,   241,   242,   243,
     244,   302,   306,   195,   198,   220,   222,   198,   198,   198,
     198,   157,   198,   157,   166,   198,   198,   388,   198,   196,
      75,    80,    75,     3,     3,    86,    85,     4,     6,    20,
      21,    23,    24,    27,    49,    53,    67,   452,   453,   456,
      73,    74,    75,    76,    77,    78,    79,    83,    84,    96,
      97,   101,   102,   195,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   471,   473,    85,    48,   173,
      86,    85,    86,     8,    11,     8,     9,   300,   170,    89,
      91,    88,    88,    85,    85,    86,   264,    85,    85,    85,
      85,    86,    85,    86,   421,    85,   446,   439,   448,    85,
      85,   479,   202,   221,   289,   330,    86,    90,   390,   413,
     198,   197,   462,   477,   131,   205,    88,     3,   136,   454,
     215,   215,   215,   207,    93,    94,    95,    81,    82,    96,
      97,    98,    99,   100,    89,   472,   158,   152,   163,     8,
      88,    47,   174,   297,   303,   307,   251,   102,   385,   422,
     174,    86,    86,    30,    31,    60,    65,    68,    69,   317,
     318,   323,   401,   403,   404,   470,   481,   483,   198,   198,
     228,   231,   235,   236,   310,    86,    86,   167,   391,   389,
      90,   418,   198,    86,   390,   110,   111,     3,    89,    91,
     103,   209,   210,   211,   212,   212,   213,   213,   214,   214,
     214,   148,    91,   192,   195,   182,   177,   301,   192,   207,
     217,   217,   217,   253,   255,   256,   265,   392,   424,   440,
     449,   319,   324,    19,   195,   402,    58,   405,   195,   195,
     486,   484,   485,   402,   487,   480,   482,   203,   311,   229,
     290,   331,   182,   195,   174,   419,   414,   463,   132,   112,
     127,    89,   137,   455,   198,   473,   193,     1,    32,    33,
     153,   183,   200,   218,   317,   328,   333,   338,   379,   380,
      44,    45,    46,   164,   178,   179,   183,   340,   223,   298,
     304,   308,   252,   254,     1,   222,   257,     8,   195,   237,
     242,   243,   244,   270,   287,    51,    52,    61,   317,   386,
     393,   401,   403,   406,   407,   408,   470,    44,    54,   183,
     423,   425,   428,   431,   182,   177,    31,   195,   224,   302,
     306,   362,   198,     3,     3,    85,    85,    85,   198,    85,
      87,   483,    92,   104,   312,   314,    91,     3,   292,   295,
     254,   168,   195,   174,   401,   403,   465,   467,    87,     3,
      86,   456,    91,   191,    87,   187,   184,   189,   188,   190,
      34,   339,   362,   185,   186,   341,   181,   180,    87,    85,
      89,    89,    89,    87,   255,   260,   258,   266,   195,   195,
     195,   399,    87,   394,   395,   396,   397,   398,   400,   198,
     198,    87,   426,   427,   441,   450,   320,   325,   204,   315,
     313,   230,   291,   293,   332,    87,   415,   468,   469,   464,
     466,   138,   194,    85,    85,    85,    85,    85,    85,   334,
      85,    85,     3,   257,   257,    85,   228,   103,   198,   198,
     198,    85,    85,    85,    85,    85,    85,    85,   429,   432,
      85,    85,    87,    87,   362,   232,   235,   206,   217,   314,
     231,    87,    91,    87,    66,    85,    85,    87,   467,    72,
     112,   140,   142,   143,   195,     3,   342,   259,   267,   102,
     344,   344,   321,   326,   233,   316,   294,   416,   195,   139,
     141,    85,   335,   344,    85,    86,   345,   346,   430,   433,
     232,    55,   367,    91,   105,   295,   195,   144,    87,   142,
     102,   353,   343,   268,   103,    35,   347,   350,    38,   364,
     364,   322,   368,   327,   234,   417,    88,   354,   355,   336,
     364,     1,    28,    29,   271,   273,   277,   279,   351,   103,
     348,   365,   364,   102,    62,   370,   235,    59,   420,     3,
     457,   459,   103,    35,    36,    37,   356,   359,   363,   364,
     276,   281,   280,   269,   272,   274,   278,   362,    91,   102,
     369,   371,   195,    89,   458,   103,   357,   360,   337,    85,
     207,    90,    87,   273,   222,   283,   279,   352,   349,   366,
     192,   102,   145,    91,    91,   362,    39,   373,   282,   284,
     275,   231,   350,   192,   103,   372,   198,   459,   358,   361,
     374,    90,   231,    85,   103,   192,   359,   231,   102,   103,
     375,    75,   376,   377,   103,    91,   378,    75
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
        }
    break;

  case 6:

    {
        }
    break;

  case 7:

    {
        }
    break;

  case 11:

    {
// fixed_definition : type_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
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
//      | typeid_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
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
//      | typeprefix_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
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
//      | const_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
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
//      | exception
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
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
//      | interface_def
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
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
//      | fixed_module
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
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
//      | value_def
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeDeclSeen);
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
//      | component
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentDeclSeen);
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
//      | home_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeDeclSeen);
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
//      | event
          idl_global->set_parse_state (IDL_GlobalData::PS_EventDeclSeen);
        }
    break;

  case 32:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 33:

    {
//      | porttype_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeDeclSeen);
        }
    break;

  case 34:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 35:

    {
//      | connector_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorDeclSeen);
        }
    break;

  case 36:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 37:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
    break;

  case 38:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 39:

    {
// module_header  : IDL_MODULE
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
        }
    break;

  case 40:

    {
          (tao_yyval.idlist) = (tao_yyvsp[(3) - (3)].idlist);
        }
    break;

  case 41:

    {
// fixed_module : module_header
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleIDSeen);

          // The module_header rule is common to template module, fixed
          // module and instantiated template module. In the last
          // case, a fully scoped name is allowed, but here we
          // allow only an identifier (a scoped name of length
          // 1). If not satisfied, we output a parse error with
          // the appropriate message.
          if ((tao_yyvsp[(1) - (1)].idlist)->length () != 1)
            {
              idl_global->err ()->syntax_error (
                IDL_GlobalData::PS_ModuleIDSeen);
            }

          AST_Module *m = 0;
          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          /*
           * Make a new module and add it to the enclosing scope
           */
          if (s != 0)
            {
              m = idl_global->gen ()->create_module (s,
                                                     (tao_yyvsp[(1) - (1)].idlist));
              (void) s->fe_add_module (m);
            }

          /*
           * Push it on the stack
           */
          idl_global->scopes ().push (m);
        }
    break;

  case 42:

    {
//      '{'
        idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
        }
    break;

  case 43:

    {
//      at_least_one_fixed_definition
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
        }
    break;

  case 44:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
          /*
           * Finished with this module - pop it from the scope stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 45:

    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleIDSeen);
        }
    break;

  case 46:

    {
// template_module : template_module_header
          // The module_header rule is common to template module, fixed
          // module and instantiated template module. In the last
          // case, a fully scoped name is allowed, but here we
          // allow only an identifier (a scoped name of length
          // 1). If not satisfied, we output a syntax error with
          // the appropriate message.
          if ((tao_yyvsp[(1) - (1)].idlist)->length () != 1)
            {
              idl_global->err ()->syntax_error (
                IDL_GlobalData::PS_ModuleIDSeen);
            }
        }
    break;

  case 47:

    {
          if (FE_Utils::duplicate_param_id ((tao_yyvsp[(3) - (3)].plval)))
            {
              idl_global->err ()->duplicate_param_id (
                (tao_yyvsp[(1) - (3)].idlist));

              return 1;
            }
        }
    break;

  case 48:

    {
//        '>'
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleParamsSeen);

          AST_Template_Module *tm =
            idl_global->gen ()->create_template_module ((tao_yyvsp[(1) - (5)].idlist),
                                                        (tao_yyvsp[(3) - (5)].plval));

          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          (void) s->fe_add_module (tm);

          /*
           * Push it on the stack
           */
          idl_global->scopes ().push (tm);

          // Store these for reference as we parse the scope
          // of the template module.
          idl_global->current_params ((tao_yyvsp[(3) - (5)].plval));
        }
    break;

  case 49:

    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleSqSeen);
        }
    break;

  case 50:

    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleBodySeen);
        }
    break;

  case 51:

    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleQsSeen);

          /*
           * Finished with this module - pop it from the scope stack.
           */
          idl_global->scopes ().pop ();

          // Clear the pointer so scoped name lookup will know
          // that we are no longer in a template module scope.
          idl_global->current_params (0);
        }
    break;

  case 57:

    {
// template_module_ref : IDL_ALIAS scoped_name
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefSeen);
        }
    break;

  case 58:

    {
//        '<' at_least_one_formal_parameter_name '>'
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefParamsSeen);
        }
    break;

  case 59:

    {
//        id
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefIDSeen);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (8)].idlist), true);

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (8)].idlist));
              return 1;
            }

          AST_Template_Module *ref =
            AST_Template_Module::narrow_from_decl (d);

          if (ref == 0)
            {
              idl_global->err ()->template_module_expected (d);
              return 1;
            }

          bool refs_match =
            ref->match_param_refs ((tao_yyvsp[(5) - (8)].slval),
                                   s);

          if (! refs_match)
            {
              // Error message is already output.
              return 1;
            }

          UTL_ScopedName sn ((tao_yyvsp[(8) - (8)].idval), 0);

          AST_Template_Module_Ref *tmr =
            idl_global->gen ()->create_template_module_ref (
              &sn,
              ref,
              (tao_yyvsp[(5) - (8)].slval));

           (void) s->fe_add_template_module_ref (tmr);

           sn.destroy ();
           (tao_yyvsp[(2) - (8)].idlist)->destroy ();
           delete (tao_yyvsp[(2) - (8)].idlist);
           (tao_yyvsp[(2) - (8)].idlist) = 0;
        }
    break;

  case 60:

    {
// template_module_inst : template_module_header
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleSeen);
        }
    break;

  case 61:

    {
//        at_least_one_actual_parameter '>'
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleArgsSeen);
        }
    break;

  case 62:

    {
//        id
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleIDSeen);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName *sn = (tao_yyvsp[(1) - (6)].idlist);
          AST_Template_Module *ref = 0;
          AST_Decl *d = s->lookup_by_name (sn, true);

          if (d == 0)
            {
              idl_global->err ()->lookup_error (sn);
              return 1;
            }
          else
            {
              ref = AST_Template_Module::narrow_from_decl (d);

              if (ref == 0)
                {
                  idl_global->err ()->template_module_expected (d);
                  return 1;
                }
            }

          sn->destroy ();
          delete sn;
          sn = 0;
          (tao_yyvsp[(1) - (6)].idlist) = 0;

          if (! ref->match_arg_names ((tao_yyvsp[(3) - (6)].alval)))
            {
              return 1;
            }

          ACE_NEW_RETURN (sn,
                          UTL_ScopedName ((tao_yyvsp[(6) - (6)].idval),
                                           0),
                          1);

          AST_Template_Module_Inst *tmi =
            idl_global->gen ()->create_template_module_inst (
              sn,
              ref,
              (tao_yyvsp[(3) - (6)].alval));

          (void) s->fe_add_template_module_inst (tmi);
        }
    break;

  case 65:

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

  case 66:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
    break;

  case 67:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
    break;

  case 68:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 69:

    {
// interface_decl : IDL_INTERFACE
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
    break;

  case 70:

    {
//       id
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
         }
    break;

  case 71:

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

  case 72:

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

  case 73:

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

  case 74:

    {
// inheritance_spec : ':' opt_truncatable
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
    break;

  case 75:

    {
//      at_least_one_scoped_name
          (tao_yyvsp[(4) - (4)].nlval)->truncatable ((tao_yyvsp[(2) - (4)].bval));
          (tao_yyval.nlval) = (tao_yyvsp[(4) - (4)].nlval);
        }
    break;

  case 76:

    {
/*      |  EMPTY */
          (tao_yyval.nlval) = 0;
        }
    break;

  case 81:

    {
// valuetype : IDL_CUSTOM value_concrete_decl
           ACE_DEBUG ((LM_DEBUG,
                       ACE_TEXT ("error in %C line %d\n"),
                       idl_global->filename ()->get_string (),
                       idl_global->lineno ()));
           ACE_DEBUG ((LM_DEBUG,
                       ACE_TEXT ("Sorry, I (TAO_IDL) can't handle")
                       ACE_TEXT (" custom yet\n")));
        }
    break;

  case 83:

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

  case 84:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
    break;

  case 85:

    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
    break;

  case 86:

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

  case 87:

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

  case 88:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
    break;

  case 89:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
    break;

  case 90:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 91:

    {
// value_header : value_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 92:

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

  case 93:

    {
// value_decl : IDL_VALUETYPE
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
    break;

  case 94:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 95:

    {
// opt_truncatable : IDL_TRUNCATABLE
          (tao_yyval.bval) = true;
        }
    break;

  case 96:

    {
/*      |  EMPTY */
          (tao_yyval.bval) = false;
        }
    break;

  case 97:

    {
// supports_spec : IDL_SUPPORTS at_least_one_scoped_name
          (tao_yyval.nlval) = (tao_yyvsp[(2) - (2)].nlval);
        }
    break;

  case 98:

    {
/*      |    EMPTY */
          (tao_yyval.nlval) = 0;
        }
    break;

  case 99:

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

  case 100:

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

  case 101:

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

  case 107:

    {
// state_member : IDL_PUBLIC
          /* is $0 to member_i */
          (tao_yyval.vival) = AST_Field::vis_PUBLIC;
        }
    break;

  case 109:

    {
//      IDL_PRIVATE
          /* is $0 to member_i */
          (tao_yyval.vival) = AST_Field::vis_PRIVATE;
        }
    break;

  case 113:

    {
// export : type_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
    break;

  case 114:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 115:

    {
//      | typeid_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
    break;

  case 116:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 117:

    {
//      | typeprefix_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
    break;

  case 118:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 119:

    {
//      | const_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
    break;

  case 120:

    {
//      ';'
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
    break;

  case 121:

    {
//      | exception
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
    break;

  case 122:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 123:

    {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
    break;

  case 124:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 125:

    {
//      | operation
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
    break;

  case 126:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 127:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
    break;

  case 128:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 129:

    {
// at_least_one_scoped_name : scoped_name scoped_names
          ACE_NEW_RETURN ((tao_yyval.nlval),
                          UTL_NameList ((tao_yyvsp[(1) - (2)].idlist),
                                        (tao_yyvsp[(2) - (2)].nlval)),
                          1);
        }
    break;

  case 130:

    {
// scoped_names : scoped_names ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
    break;

  case 131:

    {
//      scoped_name
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopedNameSeen);

          UTL_NameList *nl = 0;
          ACE_NEW_RETURN (nl,
                          UTL_NameList ((tao_yyvsp[(4) - (4)].idlist),
                                        0),
                          1);

          if ((tao_yyvsp[(1) - (4)].nlval) == 0)
            {
              (tao_yyval.nlval) = nl;
            }
          else
            {
              (tao_yyvsp[(1) - (4)].nlval)->nconc (nl);
              (tao_yyval.nlval) = (tao_yyvsp[(1) - (4)].nlval);
            }
        }
    break;

  case 132:

    {
/*      |  EMPTY */
          (tao_yyval.nlval) = 0;
        }
    break;

  case 133:

    {
// scoped_name : id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN ((tao_yyval.idlist),
                          UTL_IdList ((tao_yyvsp[(1) - (1)].idval),
                                      0),
                          1);
        }
    break;

  case 134:

    {
//      | IDL_SCOPE_DELIMITOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
    break;

  case 135:

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

  case 136:

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

  case 137:

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

  case 138:

    {
// id: IDENTIFIER
          ACE_NEW_RETURN ((tao_yyval.idval),
                          Identifier ((tao_yyvsp[(1) - (1)].strval)),
                          1);
          ACE::strdelete ((tao_yyvsp[(1) - (1)].strval));
          (tao_yyvsp[(1) - (1)].strval) = 0;
        }
    break;

  case 139:

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

  case 140:

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

  case 141:

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

  case 142:

    {
// const_dcl : IDL_CONST
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
    break;

  case 143:

    {
//      const_type
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
    break;

  case 144:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
    break;

  case 145:

    {
//      '='
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
    break;

  case 146:

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
              AST_Param_Holder *param_holder =
                (tao_yyvsp[(9) - (9)].exval)->param_holder ();

              AST_Expression::AST_ExprValue *result =
                (tao_yyvsp[(9) - (9)].exval)->check_and_coerce ((tao_yyvsp[(3) - (9)].etval),
                                      tao_enum_constant_decl);
              tao_enum_constant_decl = 0;
              
              // If the expression is a template parameter place
              // holder, 'result' will be 0, but it's ok.
              if (result == 0 && param_holder == 0)
                {
                  idl_global->err ()->coercion_error ((tao_yyvsp[(9) - (9)].exval),
                                                      (tao_yyvsp[(3) - (9)].etval));
                  (tao_yyvsp[(9) - (9)].exval)->destroy ();
                  delete (tao_yyvsp[(9) - (9)].exval);
                  (tao_yyvsp[(9) - (9)].exval) = 0;
                }
              else
                {
                  AST_Expression::ExprType et =
                    (tao_yyvsp[(3) - (9)].etval);
                    
                  if (param_holder != 0
                      && et != param_holder->info ()->const_type_)
                    {
                      idl_global->err ()->mismatched_template_param (
                        param_holder->info ()->name_.c_str ());
                    }
                  else
                    {                    
                      c =
                        idl_global->gen ()->create_constant (
                          (tao_yyvsp[(3) - (9)].etval),
                          (tao_yyvsp[(9) - (9)].exval),
                          &n);
                          
                      (void) s->fe_add_constant (c);
                    }
                }

              (tao_yyvsp[(5) - (9)].idval)->destroy ();
              delete (tao_yyvsp[(5) - (9)].idval);
              (tao_yyvsp[(5) - (9)].idval) = 0;
              
              delete result;
              result = 0;
            }
        }
    break;

  case 153:

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

  case 154:

    {
//      | wstring_type_spec
          (tao_yyval.etval) = AST_Expression::EV_wstring;
        }
    break;

  case 155:

    {
//      | scoped_name
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_PredefinedType *c = 0;
          AST_Typedef *t = 0;
          UTL_ScopedName *sn = (tao_yyvsp[(1) - (1)].idlist);

          /*
           * If the constant's type is a scoped name, it must resolve
           * to a scalar constant type
           */
          AST_Decl *d =
            s->lookup_by_name (sn, true);

          if (s != 0 && d != 0)
            {
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

              if (d->node_type () == AST_Decl::NT_pre_defined)
                {
                  c = AST_PredefinedType::narrow_from_decl (d);

                  (tao_yyval.etval) = idl_global->PredefinedTypeToExprType (c->pt ());
                }
              else if (d->node_type () == AST_Decl::NT_string)
                {
                  (tao_yyval.etval) = AST_Expression::EV_string;
                }
              else if (d->node_type () == AST_Decl::NT_wstring)
                {
                  (tao_yyval.etval) = AST_Expression::EV_wstring;
                }
              else if (d->node_type () == AST_Decl::NT_enum)
                {
                  (tao_yyval.etval) = AST_Expression::EV_enum;
                  tao_enum_constant_decl =
                    AST_Enum::narrow_from_decl (d);
                }
              else
                {
                  idl_global->err ()->constant_expected (sn, d);
                }
            }
          else
            {
              idl_global->err ()->lookup_error (sn);
            }

          sn->destroy ();
          delete sn;
          sn = 0;
          (tao_yyvsp[(1) - (1)].idlist) = 0;
        }
    break;

  case 159:

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

  case 161:

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

  case 163:

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

  case 165:

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

  case 166:

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

  case 168:

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

  case 169:

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

  case 171:

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

  case 172:

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

  case 173:

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

  case 175:

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

  case 176:

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

  case 177:

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

  case 178:

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

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(1) - (1)].idlist));
              return 1;
            }
          else if (d->node_type () == AST_Decl::NT_const)
            {
              /*
               * If the scoped name is an IDL constant, it
               * may be used in an array dim, a string
               * bound, or a sequence bound. If so, it
               * must be unsigned and > 0. We assign the
               * constant's value and type to the
               * expression created here so we can check
               * them later.
               */
              AST_Constant *c =
                AST_Constant::narrow_from_decl (d);

              (tao_yyval.exval) =
                idl_global->gen ()->create_expr (
                  c->constant_value (),
                  c->et ());
            }
          else
            {
              // An AST_Expression owns the scoped name
              // passed in this constructor, so we copy it
              // and destroy it below no matter which case
              // is followed.
              (tao_yyval.exval) =
                idl_global->gen ()->create_expr (
                  (tao_yyvsp[(1) - (1)].idlist)->copy ());
            }

          (tao_yyvsp[(1) - (1)].idlist)->destroy ();
          delete (tao_yyvsp[(1) - (1)].idlist);
          (tao_yyvsp[(1) - (1)].idlist) = 0;
        }
    break;

  case 180:

    {
//      | literal
//      | '(' const_expr ')'
          (tao_yyval.exval) = (tao_yyvsp[(2) - (3)].exval);
        }
    break;

  case 181:

    {
// literal : IDL_INTEGER_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].ival));
        }
    break;

  case 182:

    {
//      | IDL_UINTEGER_LITERAL
          (tao_yyval.exval) =
            idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].uival),
                                             AST_Expression::EV_ulonglong);
        }
    break;

  case 183:

    {
//      | IDL_STRING_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].sval));
          (tao_yyvsp[(1) - (1)].sval)->destroy ();
          delete (tao_yyvsp[(1) - (1)].sval);
          (tao_yyvsp[(1) - (1)].sval) = 0;
        }
    break;

  case 184:

    {
//      | IDL_WSTRING_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].wsval));
        }
    break;

  case 185:

    {
//      | IDL_CHARACTER_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].cval));
        }
    break;

  case 186:

    {
//      | IDL_WCHAR_LITERAL
          ACE_OutputCDR::from_wchar wc ((tao_yyvsp[(1) - (1)].wcval));
          (tao_yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
    break;

  case 187:

    {
//      | IDL_FLOATING_PT_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].dval));
        }
    break;

  case 188:

    {
//      | IDL_TRUETOK
          (tao_yyval.exval) = idl_global->gen ()->create_expr (true);
        }
    break;

  case 189:

    {
//      | IDL_FALSETOK
          (tao_yyval.exval) = idl_global->gen ()->create_expr (false);
        }
    break;

  case 190:

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

  case 191:

    {
// type_dcl : IDL_TYPEDEF
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
        }
    break;

  case 192:

    {
//      type_declarator
          (tao_yyval.dcval) = 0;
        }
    break;

  case 193:

    {
//      | struct_type
          (tao_yyval.dcval) = 0;
        }
    break;

  case 194:

    {
//      | union_type
          (tao_yyval.dcval) = 0;
        }
    break;

  case 195:

    {
//      | enum_type
          (tao_yyval.dcval) = 0;
        }
    break;

  case 196:

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

  case 197:

    {
//      | constructed_forward_type_spec
          (tao_yyval.dcval) = 0;
        }
    break;

  case 198:

    {
// type_declarator : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
    break;

  case 199:

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

  case 202:

    {
// simple_type_spec : base_type_spec
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 204:

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

  case 221:

    {
// at_least_one_declarator : declarator declarators
          ACE_NEW_RETURN ((tao_yyval.dlval),
                          UTL_DeclList ((tao_yyvsp[(1) - (2)].deval),
                                        (tao_yyvsp[(2) - (2)].dlval)),
                          1);
        }
    break;

  case 222:

    {
// declarators : declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
    break;

  case 223:

    {
//      declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsDeclSeen);

          UTL_DeclList *dl = 0;
          ACE_NEW_RETURN (dl,
                          UTL_DeclList ((tao_yyvsp[(4) - (4)].deval),
                                        0),
                          1);

          if ((tao_yyvsp[(1) - (4)].dlval) == 0)
            {
              (tao_yyval.dlval) = dl;
            }
          else
            {
              (tao_yyvsp[(1) - (4)].dlval)->nconc (dl);
              (tao_yyval.dlval) = (tao_yyvsp[(1) - (4)].dlval);
            }
        }
    break;

  case 224:

    {
/*      |  EMPTY */
          (tao_yyval.dlval) = 0;
        }
    break;

  case 227:

    {
// at_least_one_simple_declarator : simple_declarator simple_declarators
          ACE_NEW_RETURN ((tao_yyval.dlval),
                          UTL_DeclList ((tao_yyvsp[(1) - (2)].deval),
                                        (tao_yyvsp[(2) - (2)].dlval)),
                          1);
        }
    break;

  case 228:

    {
// simple_declarators : simple_declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
    break;

  case 229:

    {
//      simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsDeclSeen);

          UTL_DeclList *dl = 0;
          ACE_NEW_RETURN (dl,
                          UTL_DeclList ((tao_yyvsp[(4) - (4)].deval),
                                        0),
                          1);

          if ((tao_yyvsp[(1) - (4)].dlval) == 0)
            {
              (tao_yyval.dlval) = dl;
            }
          else
            {
              (tao_yyvsp[(1) - (4)].dlval)->nconc (dl);
              (tao_yyval.dlval) = (tao_yyvsp[(1) - (4)].dlval);
            }
        }
    break;

  case 230:

    {
/*      |  EMPTY */
          (tao_yyval.dlval) = 0;
        }
    break;

  case 231:

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

  case 232:

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

  case 235:

    {
// signed_int : IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_long;
        }
    break;

  case 236:

    {
//      | IDL_LONG IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_longlong;
        }
    break;

  case 237:

    {
//      | IDL_SHORT
          (tao_yyval.etval) = AST_Expression::EV_short;
        }
    break;

  case 238:

    {
// unsigned_int : IDL_UNSIGNED IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_ulong;
        }
    break;

  case 239:

    {
//      | IDL_UNSIGNED IDL_LONG IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_ulonglong;
        }
    break;

  case 240:

    {
//      | IDL_UNSIGNED IDL_SHORT
          (tao_yyval.etval) = AST_Expression::EV_ushort;
        }
    break;

  case 241:

    {
// floating_pt_type : IDL_DOUBLE
          (tao_yyval.etval) = AST_Expression::EV_double;
        }
    break;

  case 242:

    {
//      | IDL_FLOAT
          (tao_yyval.etval) = AST_Expression::EV_float;
        }
    break;

  case 243:

    {
//      | IDL_LONG IDL_DOUBLE
          (tao_yyval.etval) = AST_Expression::EV_longdouble;
        }
    break;

  case 244:

    {
// fixed_type : IDL_FIXED
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("error in %C line %d\n"),
                      idl_global->filename ()->get_string (),
                      idl_global->lineno ()));
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Sorry, I (TAO_IDL) can't handle")
                      ACE_TEXT (" fixed types yet\n")));
        }
    break;

  case 245:

    {
// char_type : IDL_CHAR
          (tao_yyval.etval) = AST_Expression::EV_char;
        }
    break;

  case 246:

    {
//      | IDL_WCHAR
          (tao_yyval.etval) = AST_Expression::EV_wchar;
        }
    break;

  case 247:

    {
// octet_type : IDL_OCTET
          (tao_yyval.etval) = AST_Expression::EV_octet;
        }
    break;

  case 248:

    {
// boolean_type : IDL_BOOLEAN
          (tao_yyval.etval) = AST_Expression::EV_bool;
        }
    break;

  case 249:

    {
// any_type : IDL_ANY
          (tao_yyval.etval) = AST_Expression::EV_any;
        }
    break;

  case 250:

    {
// object_type : IDL_OBJECT
          (tao_yyval.etval) = AST_Expression::EV_object;
        }
    break;

  case 251:

    {
// struct_decl : IDL_STRUCT
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
    break;

  case 252:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 253:

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

  case 254:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
    break;

  case 255:

    {
//      at_least_one_member
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
    break;

  case 256:

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

  case 260:

    {
// member  :
          /* is $0 to member_i */
          (tao_yyval.vival) = AST_Field::vis_NA;
        }
    break;

  case 262:

    {
// member_i : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
    break;

  case 263:

    {
//      at_least_one_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
    break;

  case 264:

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

  case 265:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state ());
        }
    break;

  case 266:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 267:

    {
// union_decl : IDL_UNION
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
    break;

  case 268:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 269:

    {
// union_type : union_decl IDL_SWITCH
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
    break;

  case 270:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchOpenParSeen);
        }
    break;

  case 271:

    {
//      switch_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
    break;

  case 272:

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

  case 273:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
    break;

  case 274:

    {
//      at_least_one_case_branch
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
    break;

  case 275:

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

  case 276:

    {
// switch_type_spec : integer_type
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 277:

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

  case 278:

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

  case 279:

    {
//      | boolean_type
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 281:

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

  case 285:

    {
// case_branch : at_least_one_case_label
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
    break;

  case 286:

    {
//      element_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
    break;

  case 287:

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

  case 288:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
    break;

  case 289:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 290:

    {
// at_least_one_case_label : case_label case_labels
          ACE_NEW_RETURN ((tao_yyval.llval),
                          UTL_LabelList ((tao_yyvsp[(1) - (2)].ulval),
                                         (tao_yyvsp[(2) - (2)].llval)),
                          1);
        }
    break;

  case 291:

    {
// case_labels : case_labels case_label
          UTL_LabelList *ll = 0;
          ACE_NEW_RETURN (ll,
                          UTL_LabelList ((tao_yyvsp[(2) - (2)].ulval),
                                         0),
                          1);

          if ((tao_yyvsp[(1) - (2)].llval) == 0)
            {
              (tao_yyval.llval) = ll;
            }
          else
            {
              (tao_yyvsp[(1) - (2)].llval)->nconc (ll);
              (tao_yyval.llval) = (tao_yyvsp[(1) - (2)].llval);
            }
        }
    break;

  case 292:

    {
/*      |  EMPTY */
          (tao_yyval.llval) = 0;
        }
    break;

  case 293:

    {
// case_label : IDL_DEFAULT
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
    break;

  case 294:

    {
//      ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (tao_yyval.ulval) = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
    break;

  case 295:

    {
//      | IDL_CASE
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
    break;

  case 296:

    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
    break;

  case 297:

    {
//      const_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (tao_yyval.ulval) = idl_global->gen()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      (tao_yyvsp[(3) - (5)].exval)
                                    );
        }
    break;

  case 298:

    {
// element_spec : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
    break;

  case 299:

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

  case 300:

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

  case 301:

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

  case 302:

    {
// enum_type : IDL_ENUM
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
    break;

  case 303:

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

  case 304:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
    break;

  case 305:

    {
//      at_least_one_enumerator
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
    break;

  case 306:

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

  case 308:

    {
// enumerators : enumerators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
    break;

  case 311:

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

  case 312:

    {
// sequence_type_spec : seq_head ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
    break;

  case 313:

    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
    break;

  case 314:

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
          AST_Param_Holder *param_holder = 0;

          if ((tao_yyvsp[(4) - (6)].exval) != 0)
            {
              param_holder =
                (tao_yyvsp[(4) - (6)].exval)->param_holder ();
                
              ev = (tao_yyvsp[(4) - (6)].exval)->coerce (AST_Expression::EV_ulong);
            }

          // If the expression corresponds to a template parameter,
          // it's ok for the coercion to fail at this point. We check
          // for a type mismatch below.
          if (0 == (tao_yyvsp[(4) - (6)].exval)
              || (0 == ev && 0 == param_holder))
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
                  if (param_holder != 0)
                    {
                      AST_Expression::ExprType et =
                        param_holder->info ()->const_type_;
                        
                      // If the bound expression represents a
                      // template parameter, it must be a const
                      // and of type unsigned long.  
                      if (et != AST_Expression::EV_ulong)
                        {
                          idl_global->err ()->mismatched_template_param (
                            param_holder->info ()->name_.c_str ());
                            
                          delete ev;
                          ev = 0;
                          return 1;
                        }
                    }
                    
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

  case 315:

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

  case 316:

    {
// seq_head : IDL_SEQUENCE
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
    break;

  case 317:

    {
//      '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
    break;

  case 318:

    {
//      simple_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          (tao_yyval.dcval) = (tao_yyvsp[(5) - (5)].dcval);
        }
    break;

  case 319:

    {
// string_type_spec : string_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
    break;

  case 320:

    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
    break;

  case 321:

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

  case 322:

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

  case 323:

    {
// string_head : IDL_STRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
    break;

  case 324:

    {
// wstring_type_spec : wstring_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
    break;

  case 325:

    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
    break;

  case 326:

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

  case 327:

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

  case 328:

    {
// wstring_head : IDL_WSTRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
    break;

  case 329:

    {
// array_declarator : id
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
    break;

  case 330:

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

  case 331:

    {
// at_least_one_array_dim : array_dim array_dims
          ACE_NEW_RETURN ((tao_yyval.elval),
                          UTL_ExprList ((tao_yyvsp[(1) - (2)].exval),
                                        (tao_yyvsp[(2) - (2)].elval)),
                          1);
        }
    break;

  case 332:

    {
// array_dims : array_dims array_dim
          UTL_ExprList *el = 0;
          ACE_NEW_RETURN (el,
                          UTL_ExprList ((tao_yyvsp[(2) - (2)].exval),
                                        0),
                          1);

          if ((tao_yyvsp[(1) - (2)].elval) == 0)
            {
              (tao_yyval.elval) = el;
            }
          else
            {
              (tao_yyvsp[(1) - (2)].elval)->nconc (el);
              (tao_yyval.elval) = (tao_yyvsp[(1) - (2)].elval);
            }
        }
    break;

  case 333:

    {
/*      |  EMPTY */
          (tao_yyval.elval) = 0;
        }
    break;

  case 334:

    {
// array_dim : '['
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
    break;

  case 335:

    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
    break;

  case 336:

    {
//      ']'
          idl_global->set_parse_state (IDL_GlobalData::PS_DimQsSeen);

          /*
           * Array dimensions are expressions which must be coerced to
           * positive integers.
           */
          AST_Expression::AST_ExprValue *ev = 0;
          AST_Param_Holder *param_holder = 0;

          if ((tao_yyvsp[(3) - (5)].exval) != 0)
            {
              param_holder =
                (tao_yyvsp[(3) - (5)].exval)->param_holder ();

              ev = (tao_yyvsp[(3) - (5)].exval)->coerce (AST_Expression::EV_ulong);
            }

          if (0 == (tao_yyvsp[(3) - (5)].exval)
              || (0 == ev && param_holder == 0))
            {
              idl_global->err ()->coercion_error ((tao_yyvsp[(3) - (5)].exval),
                                                  AST_Expression::EV_ulong);
              (tao_yyval.exval) = 0;
            }
          else
            {
              if (param_holder != 0)
                {
                  AST_Expression::ExprType et =
                    param_holder->info ()->const_type_;
                    
                  // If the bound expression represents a
                  // template parameter, it must be a const
                  // and of type unsigned long.  
                  if (et != AST_Expression::EV_ulong)
                    {
                      idl_global->err ()->mismatched_template_param (
                        param_holder->info ()->name_.c_str ());
                        
                      delete ev;
                      ev = 0;
                      return 1;
                    }
                }

              (tao_yyval.exval) = (tao_yyvsp[(3) - (5)].exval);
            }

          delete ev;
          ev = 0;
        }
    break;

  case 339:

    {
// attribute_readonly : IDL_READONLY
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
    break;

  case 340:

    {
//      IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
    break;

  case 341:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
    break;

  case 342:

    {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
    break;

  case 343:

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

  case 344:

    {
// attribute_readonly : IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
    break;

  case 345:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
    break;

  case 346:

    {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
    break;

  case 347:

    {
//      opt_getraises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
    break;

  case 348:

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

  case 349:

    {
// exception : IDL_EXCEPTION
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
    break;

  case 350:

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

  case 351:

    {
//       '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
    break;

  case 352:

    {
//      members
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
    break;

  case 353:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 354:

    {
// operation : opt_op_attribute op_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
    break;

  case 355:

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

  case 356:

    {
//      parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 357:

    {
//      opt_raises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
    break;

  case 358:

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

  case 359:

    {
// opt_op_attribute : IDL_ONEWAY
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (tao_yyval.ofval) = AST_Operation::OP_oneway;
        }
    break;

  case 360:

    {
//      | IDL_IDEMPOTENT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (tao_yyval.ofval) = AST_Operation::OP_idempotent;
        }
    break;

  case 361:

    {
/*      |  EMPTY */
          (tao_yyval.ofval) = AST_Operation::OP_noflags;
        }
    break;

  case 363:

    {
// op_type_spec : param_type_spec | IDL_VOID
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
    break;

  case 364:

    {
// init_decl : IDL_FACTORY
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
    break;

  case 365:

    {
//      IDENTIFIER
          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          if (s->is_abstract ())
            {
              //@@ Fire error
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("error in %C line %d:\n")
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

  case 366:

    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 367:

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

  case 368:

    {
// init_parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 369:

    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 370:

    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 371:

    {
//      at_least_one_in_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 373:

    {
// in_parameters : in_parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
    break;

  case 376:

    {
// in_parameter : IDL_IN
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
    break;

  case 377:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
    break;

  case 378:

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

  case 379:

    {
// parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 380:

    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 381:

    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 382:

    {
//      at_least_one_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 384:

    {
// parameters : parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
    break;

  case 387:

    {
// parameter : direction
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
    break;

  case 388:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
    break;

  case 389:

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

  case 390:

    {
// param_type_spec : base_type_spec
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 393:

    {
//      | string_type_spec
//      | wstring_type_spec
//      | scoped_name
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = 0;
          UTL_ScopedName *n = (tao_yyvsp[(1) - (1)].idlist);

          if (s != 0)
            {
              d = s->lookup_by_name (n, true);
            }

          if (d == 0)
            {
              bool so_far_so_good = false;

              // We're looking for a template parameter ref, so
              // the scoped name would just be a simple identifier.
              if (n->length () == 1)
                {
                  // TODO
                }

              if (!so_far_so_good)
                {
                  idl_global->err ()->lookup_error (n);
                  (tao_yyvsp[(1) - (1)].idlist)->destroy ();
                  (tao_yyvsp[(1) - (1)].idlist) = 0;

                  /* If we don't return here, we'll crash later.*/
                  return 1;
                }
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

  case 394:

    {
// direction : IDL_IN
          (tao_yyval.dival) = AST_Argument::dir_IN;
        }
    break;

  case 395:

    {
//      | IDL_OUT
          (tao_yyval.dival) = AST_Argument::dir_OUT;
        }
    break;

  case 396:

    {
//      | IDL_INOUT
          (tao_yyval.dival) = AST_Argument::dir_INOUT;
        }
    break;

  case 397:

    {
// opt_raises : IDL_RAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
    break;

  case 398:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
    break;

  case 399:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          (tao_yyval.nlval) = (tao_yyvsp[(5) - (6)].nlval);
        }
    break;

  case 400:

    {
          (tao_yyval.nlval) = 0;
/*      |  EMPTY */
        }
    break;

  case 401:

    {
// opt_getraises : IDL_GETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
    break;

  case 402:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
    break;

  case 403:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          (tao_yyval.nlval) = (tao_yyvsp[(5) - (6)].nlval);
        }
    break;

  case 404:

    {
          (tao_yyval.nlval) = 0;
/*      |  EMPTY */
        }
    break;

  case 405:

    {
// opt_setraises : IDL_SETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
    break;

  case 406:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
    break;

  case 407:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (tao_yyval.nlval) = (tao_yyvsp[(5) - (6)].nlval);
        }
    break;

  case 408:

    {
          (tao_yyval.nlval) = 0;
/*      |  EMPTY */
        }
    break;

  case 409:

    {
// opt_context : IDL_CONTEXT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
    break;

  case 410:

    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
//      '('
        }
    break;

  case 411:

    {
//      at_least_one_string_literal ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (tao_yyval.slval) = (tao_yyvsp[(5) - (6)].slval);
        }
    break;

  case 412:

    {
/*      |  EMPTY */
          (tao_yyval.slval) = 0;
        }
    break;

  case 413:

    {
// at_least_one_string_literal : IDL_STRING_LITERAL string_literals
          ACE_NEW_RETURN ((tao_yyval.slval),
                          UTL_StrList ((tao_yyvsp[(1) - (2)].sval),
                                       (tao_yyvsp[(2) - (2)].slval)),
                          1);
        }
    break;

  case 414:

    {
// string_literals : string_literals ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
    break;

  case 415:

    {
//      IDL_STRING_LITERAL
          UTL_StrList *sl = 0;
          ACE_NEW_RETURN (sl,
                          UTL_StrList ((tao_yyvsp[(4) - (4)].sval),
                                       0),
                          1);

          if ((tao_yyvsp[(1) - (4)].slval) == 0)
            {
              (tao_yyval.slval) = sl;
            }
          else
            {
              (tao_yyvsp[(1) - (4)].slval)->nconc (sl);
              (tao_yyval.slval) = (tao_yyvsp[(1) - (4)].slval);
            }
        }
    break;

  case 416:

    {
/*      |  EMPTY */
          (tao_yyval.slval) = 0;
        }
    break;

  case 417:

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

  case 418:

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

  case 421:

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

  case 422:

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

  case 423:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
    break;

  case 424:

    {
//      component_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
    break;

  case 425:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 426:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
    break;

  case 427:

    {
//      component_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 428:

    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);

          /*
           * Create an AST representation of the information in the header
           * part of a component.
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ((tao_yyvsp[(2) - (6)].idval),
                                          0),
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

  case 429:

    {
// component_inheritance_spec : ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
    break;

  case 430:

    {
//      scoped_name
          (tao_yyval.idlist) = (tao_yyvsp[(3) - (3)].idlist);
        }
    break;

  case 431:

    {
/*      |  EMPTY */
          (tao_yyval.idlist) = 0;
        }
    break;

  case 434:

    {
// component_export : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
    break;

  case 435:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 436:

    {
//      | uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
    break;

  case 437:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 438:

    {
//      | emits_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
        }
    break;

  case 439:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 440:

    {
//      | publishes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
        }
    break;

  case 441:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 442:

    {
//      | consumes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
        }
    break;

  case 443:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 444:

    {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
    break;

  case 445:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 446:

    {
//      | extended_port_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
    break;

  case 447:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 448:

    {
// provides_decl : IDL_PROVIDES interface_type id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;

          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist),
                                           true);
          if (d == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));
              so_far_so_good = false;
            }
          else
            {
              int compare = 0;
              nt = d->node_type ();
            
              switch (nt)
                {
                  case AST_Decl::NT_interface:
                    break;
                  case AST_Decl::NT_param_holder:
                    ph = AST_Param_Holder::narrow_from_decl (d);
                    nt = ph->info ()->type_;
                    
                    if (nt != AST_Decl::NT_type
                       && nt != AST_Decl::NT_interface)
                      {
                        idl_global->err ()->mismatched_template_param (
                          ph->info ()->name_.c_str ());
                          
                        so_far_so_good = false;
                      }
                      
                    break;
                  case AST_Decl::NT_pre_defined:
                    // Nothing else but CORBA::Object can have
                    // this identifier.
                    compare =
                      ACE_OS::strcmp (d->local_name ()->get_string (),
                                      "Object");

                    // Simple provides port must use IDL interface
                    // or CORBA::Object.
                    if (compare != 0)
                      {
                        idl_global->err ()->interface_expected (d);
                        so_far_so_good = false;
                      }
                      
                    break;
                  default:
                    idl_global->err ()->interface_expected (d);
                    so_far_so_good = false;
                    break;
                }
            }

          if (so_far_so_good)
            {
              AST_Type *port_interface_type =
                AST_Type::narrow_from_decl (d);

              // Strip off _cxx_, if any, for port name.
              idl_global->original_local_name ((tao_yyvsp[(3) - (3)].idval));

              UTL_ScopedName sn ((tao_yyvsp[(3) - (3)].idval),
                                 0);

              AST_Provides *p =
                idl_global->gen ()->create_provides (&sn,
                                                     port_interface_type);

              (void) s->fe_add_provides (p);
            }

          (tao_yyvsp[(2) - (3)].idlist)->destroy ();
          delete (tao_yyvsp[(2) - (3)].idlist);
          (tao_yyvsp[(2) - (3)].idlist) = 0;

          (tao_yyvsp[(3) - (3)].idval)->destroy ();
          delete (tao_yyvsp[(3) - (3)].idval);
          (tao_yyvsp[(3) - (3)].idval) = 0;
        }
    break;

  case 449:

    {
// interface_type : scoped_name
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (tao_yyval.idlist) = (tao_yyvsp[(1) - (1)].idlist);
        }
    break;

  case 450:

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

          ACE_NEW_RETURN ((tao_yyval.idlist),
                          UTL_IdList (corba_id,
                                      conc_name),
                          1);
        }
    break;

  case 451:

    {
// uses_decl : uses_opt_multiple interface_type id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;

          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist),
                                           true);
          if (d == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));
              so_far_so_good = false;
            }
          else
            {
              int compare = 0;
              nt = d->node_type ();
            
              switch (nt)
                {
                  case AST_Decl::NT_interface:
                    break;
                  case AST_Decl::NT_param_holder:
                    ph = AST_Param_Holder::narrow_from_decl (d);
                    nt = ph->info ()->type_;
                    
                    if (nt != AST_Decl::NT_type
                       && nt != AST_Decl::NT_interface)
                      {
                        idl_global->err ()->mismatched_template_param (
                          ph->info ()->name_.c_str ());
                          
                        so_far_so_good = false;
                      }
                      
                    break;
                  case AST_Decl::NT_pre_defined:
                    // Nothing else but CORBA::Object can have
                    // this identifier.
                    compare =
                      ACE_OS::strcmp (d->local_name ()->get_string (),
                                      "Object");

                    // Simple provides port must use IDL interface
                    // or CORBA::Object.
                    if (compare != 0)
                      {
                        idl_global->err ()->interface_expected (d);
                        so_far_so_good = false;
                      }
                      
                    break;
                  default:
                    idl_global->err ()->interface_expected (d);
                    so_far_so_good = false;
                    break;
                }
            }

          if (so_far_so_good)
            {
              AST_Type *port_interface_type =
                AST_Type::narrow_from_decl (d);

              // Strip off _cxx_, if any, for port name.
              idl_global->original_local_name ((tao_yyvsp[(3) - (3)].idval));

              UTL_ScopedName sn ((tao_yyvsp[(3) - (3)].idval),
                                 0);

              AST_Uses *u =
                idl_global->gen ()->create_uses (&sn,
                                                 port_interface_type,
                                                 (tao_yyvsp[(1) - (3)].bval));

              (void) s->fe_add_uses (u);

              AST_Component *c =
                AST_Component::narrow_from_scope (s);

              if (c != 0
                  && u->is_multiple ()
                  && !idl_global->using_ifr_backend ()
                  && !idl_global->ignore_idl3 ()
                  && nt != AST_Decl::NT_param_holder)
                {
                  // These datatypes must be created in the
                  // front end so they can be looked up
                  // when compiling the generated executor IDL.
                  idl_global->create_uses_multiple_stuff (c, u);
                }
            }

          (tao_yyvsp[(2) - (3)].idlist)->destroy ();
          delete (tao_yyvsp[(2) - (3)].idlist);
          (tao_yyvsp[(2) - (3)].idlist) = 0;

          (tao_yyvsp[(3) - (3)].idval)->destroy ();
          delete (tao_yyvsp[(3) - (3)].idval);
          (tao_yyvsp[(3) - (3)].idval) = 0;
        }
    break;

  case 452:

    {
// uses_opt_multiple : IDL_USES opt_multiple
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          (tao_yyval.bval) = (tao_yyvsp[(2) - (2)].bval);
        }
    break;

  case 453:

    {
// opt_multiple : IDL_MULTIPLE
          (tao_yyval.bval) = true;
        }
    break;

  case 454:

    {
/*      |  EMPTY */
          (tao_yyval.bval) = false;
        }
    break;

  case 455:

    {
// emits_decl : IDL_EMITS scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;
          
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist),
                                           true);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));
              so_far_so_good = false;
            }
          else
            {
              nt = d->node_type ();
            
              switch (nt)
                {
                  case AST_Decl::NT_eventtype:
                    break;
                  case AST_Decl::NT_param_holder:
                    ph = AST_Param_Holder::narrow_from_decl (d);
                    nt = ph->info ()->type_;
                    
                    if (nt != AST_Decl::NT_type
                       && nt != AST_Decl::NT_eventtype)
                      {
                        idl_global->err ()->mismatched_template_param (
                          ph->info ()->name_.c_str ());
                          
                        so_far_so_good = false;
                      }
                      
                    break;
                  default:
                    idl_global->err ()->eventtype_expected (d);
                    so_far_so_good = false;
                    break;
                }
            }

          if (so_far_so_good)
            {
              AST_Type *event_type =
                AST_Type::narrow_from_decl (d);

              // Strip off _cxx_, if any, for port name.
              idl_global->original_local_name ((tao_yyvsp[(3) - (3)].idval));

              UTL_ScopedName sn ((tao_yyvsp[(3) - (3)].idval),
                                 0);

              AST_Emits *e =
                idl_global->gen ()->create_emits (&sn,
                                                  event_type);

              (void) s->fe_add_emits (e);
            }

          (tao_yyvsp[(2) - (3)].idlist)->destroy ();
          delete (tao_yyvsp[(2) - (3)].idlist);
          (tao_yyvsp[(2) - (3)].idlist) = 0;

          (tao_yyvsp[(3) - (3)].idval)->destroy ();
          delete (tao_yyvsp[(3) - (3)].idval);
          (tao_yyvsp[(3) - (3)].idval) = 0;
        }
    break;

  case 456:

    {
// publishes_decl : IDL_PUBLISHES scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;
          
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist),
                                           true);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));
              so_far_so_good = false;
            }
          else
            {
              nt = d->node_type ();
            
              switch (nt)
                {
                  case AST_Decl::NT_eventtype:
                    break;
                  case AST_Decl::NT_param_holder:
                    ph = AST_Param_Holder::narrow_from_decl (d);
                    nt = ph->info ()->type_;
                    
                    if (nt != AST_Decl::NT_type
                       && nt != AST_Decl::NT_eventtype)
                      {
                        idl_global->err ()->mismatched_template_param (
                          ph->info ()->name_.c_str ());
                          
                        so_far_so_good = false;
                      }
                      
                    break;
                  default:
                    idl_global->err ()->eventtype_expected (d);
                    so_far_so_good = false;
                    break;
                }
            }

          if (so_far_so_good)
            {
              AST_Type *event_type =
                AST_Type::narrow_from_decl (d);

              // Strip off _cxx_, if any, for port name.
              idl_global->original_local_name ((tao_yyvsp[(3) - (3)].idval));

              UTL_ScopedName sn ((tao_yyvsp[(3) - (3)].idval),
                                 0);

              AST_Publishes *p =
                idl_global->gen ()->create_publishes (&sn,
                                                      event_type);

              (void) s->fe_add_publishes (p);
            }

          (tao_yyvsp[(2) - (3)].idlist)->destroy ();
          delete (tao_yyvsp[(2) - (3)].idlist);
          (tao_yyvsp[(2) - (3)].idlist) = 0;

          (tao_yyvsp[(3) - (3)].idval)->destroy ();
          delete (tao_yyvsp[(3) - (3)].idval);
          (tao_yyvsp[(3) - (3)].idval) = 0;
        }
    break;

  case 457:

    {
// consumes_decl : IDL_CONSUMES scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;
          
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist),
                                           true);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));
              so_far_so_good = false;
            }
          else
            {
              nt = d->node_type ();
            
              switch (nt)
                {
                  case AST_Decl::NT_eventtype:
                    break;
                  case AST_Decl::NT_param_holder:
                    ph = AST_Param_Holder::narrow_from_decl (d);
                    nt = ph->info ()->type_;
                    
                    if (nt != AST_Decl::NT_type
                       && nt != AST_Decl::NT_eventtype)
                      {
                        idl_global->err ()->mismatched_template_param (
                          ph->info ()->name_.c_str ());
                          
                        so_far_so_good = false;
                      }
                      
                    break;
                  default:
                    idl_global->err ()->eventtype_expected (d);
                    so_far_so_good = false;
                    break;
                }
            }

          if (so_far_so_good)
            {
              AST_Type *event_type =
                AST_Type::narrow_from_decl (d);

              // Strip off _cxx_, if any, for port name.
              idl_global->original_local_name ((tao_yyvsp[(3) - (3)].idval));

              UTL_ScopedName sn ((tao_yyvsp[(3) - (3)].idval),
                                 0);

              AST_Consumes *c =
                idl_global->gen ()->create_consumes (&sn,
                                                     event_type);

              (void) s->fe_add_consumes (c);
            }

          (tao_yyvsp[(2) - (3)].idlist)->destroy ();
          delete (tao_yyvsp[(2) - (3)].idlist);
          (tao_yyvsp[(2) - (3)].idlist) = 0;

          (tao_yyvsp[(3) - (3)].idval)->destroy ();
          delete (tao_yyvsp[(3) - (3)].idval);
          (tao_yyvsp[(3) - (3)].idval) = 0;
       }
    break;

  case 458:

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

  case 459:

    {
//      home_body
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 460:

    {
// home_header : IDL_HOME
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
    break;

  case 461:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
    break;

  case 462:

    {
//      home_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 463:

    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
    break;

  case 464:

    {
//      IDL_MANAGES
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
    break;

  case 465:

    {
//      scoped_name
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
    break;

  case 466:

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

  case 467:

    {
// home_inheritance_spec ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
    break;

  case 468:

    {
//      scoped_name
          (tao_yyval.idlist) = (tao_yyvsp[(3) - (3)].idlist);
        }
    break;

  case 469:

    {
/*      |  EMPTY */
          (tao_yyval.idlist) = 0;
        }
    break;

  case 470:

    {
// primary_key_spec : IDL_PRIMARYKEY scoped_name
          (tao_yyval.idlist) = (tao_yyvsp[(2) - (2)].idlist);
        }
    break;

  case 471:

    {
/*      |  EMPTY */
          (tao_yyval.idlist) = 0;
        }
    break;

  case 472:

    {
// home_body : '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
    break;

  case 473:

    {
//      home_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
    break;

  case 474:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
    break;

  case 478:

    {
// home_export : factory_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
    break;

  case 479:

    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 480:

    {
//      | finder_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
    break;

  case 481:

    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 482:

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

  case 483:

    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 484:

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

  case 485:

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

  case 486:

    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 487:

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

  case 493:

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

  case 494:

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

  case 495:

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

  case 496:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
    break;

  case 497:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
    break;

  case 498:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 499:

    {
// event_abs_header : IDL_ABSTRACT IDL_EVENTTYPE id
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 500:

    {
// event_custom_header : IDL_CUSTOM IDL_EVENTTYPE id
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("error in %C line %d\n"),
                      idl_global->filename ()->get_string (),
                      idl_global->lineno ()));
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Sorry, I (TAO_IDL) can't handle")
                      ACE_TEXT (" custom yet\n")));
          (tao_yyval.idval) = 0;
        }
    break;

  case 501:

    {
// event_plain_header : IDL_EVENTTYPE id
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (tao_yyval.idval) = (tao_yyvsp[(2) - (2)].idval);
        }
    break;

  case 502:

    {
// event_rest_of_header : inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 503:

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

  case 504:

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

  case 505:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
    break;

  case 506:

    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
    break;

  case 507:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 508:

    {
// event_header : event_custom_header
          (tao_yyval.idval) = (tao_yyvsp[(1) - (1)].idval);
        }
    break;

  case 509:

    {
// event_header : event_plain_header
          (tao_yyval.idval) = (tao_yyvsp[(1) - (1)].idval);
        }
    break;

  case 510:

    {
// formal_parameter_type : IDL_TYPENAME
          (tao_yyval.ntval) = AST_Decl::NT_type;
        }
    break;

  case 511:

    {
//        IDL_STRUCT
          (tao_yyval.ntval) = AST_Decl::NT_struct;
        }
    break;

  case 512:

    {
//        IDL_UNION
          (tao_yyval.ntval) = AST_Decl::NT_union;
        }
    break;

  case 513:

    {
//        IDL_EVENTTYPE
          (tao_yyval.ntval) = AST_Decl::NT_eventtype;
        }
    break;

  case 514:

    {
//        IDL_SEQUENCE
          (tao_yyval.ntval) = AST_Decl::NT_sequence;
        }
    break;

  case 515:

    {
//        IDL_INTERFACE
          (tao_yyval.ntval) = AST_Decl::NT_interface;
        }
    break;

  case 516:

    {
//        IDL_VALUETYPE
          (tao_yyval.ntval) = AST_Decl::NT_valuetype;
        }
    break;

  case 517:

    {
//        IDL_ENUM
          (tao_yyval.ntval) = AST_Decl::NT_enum;
        }
    break;

  case 518:

    {
//        IDL_EXCEPTION
          (tao_yyval.ntval) = AST_Decl::NT_except;
        }
    break;

  case 519:

    {
//        IDL_CONST const_type
          (tao_yyval.ntval) = AST_Decl::NT_const;
          t_param_const_type = (tao_yyvsp[(2) - (2)].etval);
        }
    break;

  case 520:

    {
// at_least_one_formal_parameter : formal_parameter formal_parameters
          if ((tao_yyvsp[(2) - (2)].plval) == 0)
            {
              ACE_NEW_RETURN ((tao_yyvsp[(2) - (2)].plval),
                              FE_Utils::T_PARAMLIST_INFO,
                              1);
            }

          (tao_yyvsp[(2) - (2)].plval)->enqueue_head (*(tao_yyvsp[(1) - (2)].pival));
          delete (tao_yyvsp[(1) - (2)].pival);
          (tao_yyvsp[(1) - (2)].pival) = 0;

          // The param added above is always the last one parsed,
          // so we check for matches between sequence<T> & T here.
          ACE_CString bad_id =
            idl_global->check_for_seq_of_param (
              (tao_yyvsp[(2) - (2)].plval));

          if (!bad_id.empty ())
            {
              delete (tao_yyvsp[(2) - (2)].plval);
              (tao_yyvsp[(2) - (2)].plval) = 0;

              idl_global->err ()->mismatch_seq_of_param (bad_id.c_str ());
              return 1;
            }

          (tao_yyval.plval) = (tao_yyvsp[(2) - (2)].plval);
        }
    break;

  case 521:

    {
// formal_parameters : formal_parameters ','
          // Maybe add a new parse state to set here.
        }
    break;

  case 522:

    {
//        formal_parameter
          if ((tao_yyvsp[(1) - (4)].plval) == 0)
            {
              ACE_NEW_RETURN ((tao_yyvsp[(1) - (4)].plval),
                              FE_Utils::T_PARAMLIST_INFO,
                              1);
            }

          (tao_yyvsp[(1) - (4)].plval)->enqueue_tail (*(tao_yyvsp[(4) - (4)].pival));
          (tao_yyval.plval) = (tao_yyvsp[(1) - (4)].plval);

          delete (tao_yyvsp[(4) - (4)].pival);
          (tao_yyvsp[(4) - (4)].pival) = 0;
        }
    break;

  case 523:

    {
//        /* EMPTY */
          (tao_yyval.plval) = 0;
        }
    break;

  case 524:

    {
// formal_parameter : formal_parameter_type IDENTIFIER

          ACE_NEW_RETURN ((tao_yyval.pival),
                          FE_Utils::T_Param_Info,
                          1);

          AST_Decl::NodeType nt = (tao_yyvsp[(1) - (2)].ntval);

          (tao_yyval.pival)->type_ = nt;
          (tao_yyval.pival)->name_ = (tao_yyvsp[(2) - (2)].strval);

          if (nt == AST_Decl::NT_const)
            {
              (tao_yyval.pival)->const_type_ = t_param_const_type;
              (tao_yyval.pival)->enum_const_type_decl_ =
                tao_enum_constant_decl;

              // Reset these values.
              t_param_const_type = AST_Expression::EV_none;
              tao_enum_constant_decl = 0;
            }
        }
    break;

  case 525:

    {
          ACE_NEW_RETURN ((tao_yyval.pival),
                          FE_Utils::T_Param_Info,
                          1);

          (tao_yyval.pival)->type_ = AST_Decl::NT_sequence;
          (tao_yyval.pival)->seq_param_ref_  = (tao_yyvsp[(3) - (5)].strval);
          (tao_yyval.pival)->name_ += (tao_yyvsp[(5) - (5)].strval);

          ACE::strdelete ((tao_yyvsp[(3) - (5)].strval));
          (tao_yyvsp[(3) - (5)].strval) = 0;
          ACE::strdelete ((tao_yyvsp[(5) - (5)].strval));
          (tao_yyvsp[(5) - (5)].strval) = 0;
        }
    break;

  case 526:

    {
// at_least_one_formal_parameter_name : formal_parameter_name formal_parameter_names
          ACE_NEW_RETURN ((tao_yyval.slval),
                          UTL_StrList ((tao_yyvsp[(1) - (2)].sval),
                                       (tao_yyvsp[(2) - (2)].slval)),
                          1);
        }
    break;

  case 527:

    {
// formal_parameter_names : formal_parameter_names ',' formal_parameter_name
          UTL_StrList *sl = 0;
          ACE_NEW_RETURN (sl,
                          UTL_StrList ((tao_yyvsp[(3) - (3)].sval),
                                       0),
                          1);

          if ((tao_yyvsp[(1) - (3)].slval) == 0)
            {
              (tao_yyval.slval) = sl;
            }
          else
            {
              (tao_yyvsp[(1) - (3)].slval)->nconc (sl);
              (tao_yyval.slval) = (tao_yyvsp[(1) - (3)].slval);
            }
        }
    break;

  case 528:

    {
//        /* EMPTY */
          (tao_yyval.slval) = 0;
        }
    break;

  case 529:

    {
// formal_parameter_name : IDENTIFIER
          ACE_NEW_RETURN ((tao_yyval.sval),
                          UTL_String ((tao_yyvsp[(1) - (1)].strval)),
                          1);
        }
    break;

  case 530:

    {
// porttype_decl : IDL_PORTTYPE
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSeen);
        }
    break;

  case 531:

    {
//        IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeIDSeen);
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          
          Identifier id ((tao_yyvsp[(3) - (3)].strval));
          ACE::strdelete ((tao_yyvsp[(3) - (3)].strval));
          (tao_yyvsp[(3) - (3)].strval) = 0;
          
          UTL_ScopedName sn (&id, 0);
          AST_PortType *p =
            idl_global->gen ()->create_porttype (&sn);
            
          (void) s->fe_add_porttype (p);
          
          // Push it on the scopes stack.
          idl_global->scopes ().push (p);
        }
    break;

  case 532:

    {
//        '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSqSeen);
        }
    break;

  case 533:

    {
//        at_least_one_port_export
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeBodySeen);
        }
    break;

  case 534:

    {
//        '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeQsSeen);

          // Done with this port type - pop it off the scopes stack.
          idl_global->scopes ().pop ();
        }
    break;

  case 535:

    {
// at_least_one_port_export : port_export port_exports
        }
    break;

  case 536:

    {
// port_exports : port_exports port_export
        }
    break;

  case 537:

    {
//        | /* EMPTY */
        }
    break;

  case 538:

    {
// port_export : provides_decl
        }
    break;

  case 539:

    {
//        ';'
        }
    break;

  case 540:

    {
//        | uses_decl
        }
    break;

  case 541:

    {
//        ';'
        }
    break;

  case 542:

    {
// extended_port_decl : IDL_PORT scoped_name IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist), true);
          AST_PortType *pt = 0;
          bool so_far_so_good = true;

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));
              so_far_so_good = false;
            }
          else
            {
              pt = AST_PortType::narrow_from_decl (d);

              if (pt == 0)
                {
                  idl_global->err ()->error1 (UTL_Error::EIDL_PORTTYPE_EXPECTED,
                                              d);
                  so_far_so_good = false;
                }
            }

          if (so_far_so_good)
            {
              Identifier id ((tao_yyvsp[(3) - (3)].strval));
              ACE::strdelete ((tao_yyvsp[(3) - (3)].strval));
              (tao_yyvsp[(3) - (3)].strval) = 0;

              UTL_ScopedName sn (&id,
                                 0);

              AST_Extended_Port *ep =
                idl_global->gen ()->create_extended_port (
                  &sn,
                  pt);

              (void) s->fe_add_extended_port (ep);

              // Create (in the AST) the struct(s) and sequence(s)
              // needed for multiplex uses ports, if any.
              for (UTL_ScopeActiveIterator i (pt, UTL_Scope::IK_decls);
                   !i.is_done ();
                   i.next ())
                {
                  d = i.item ();

                  AST_Uses *u = AST_Uses::narrow_from_decl (d);

                  if (u != 0 && u->is_multiple ())
                    {
                      AST_Component *c =
                        AST_Component::narrow_from_scope (s);

                      idl_global->create_uses_multiple_stuff (
                        c,
                        u,
                        id.get_string ());
                    }
                }
            }

          (tao_yyvsp[(2) - (3)].idlist)->destroy ();
          delete (tao_yyvsp[(2) - (3)].idlist);
          (tao_yyvsp[(2) - (3)].idlist) = 0;
        }
    break;

  case 543:

    {
//        | IDL_MIRRORPORT scoped_name IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_MirrorPortDeclSeen);
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist), true);
          AST_PortType *pt = 0;
          bool so_far_so_good = true;

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));
              so_far_so_good = false;
            }
           else
             {
               pt = AST_PortType::narrow_from_decl (d);

               if (pt == 0)
                 {
                   idl_global->err ()->error1 (UTL_Error::EIDL_PORTTYPE_EXPECTED,
                                               d);
                   so_far_so_good = false;
                 }
             }

          if (so_far_so_good)
            {
              Identifier id ((tao_yyvsp[(3) - (3)].strval));
              ACE::strdelete ((tao_yyvsp[(3) - (3)].strval));
              (tao_yyvsp[(3) - (3)].strval) = 0;

              UTL_ScopedName sn (&id,
                                 0);

              AST_Mirror_Port *mp =
                idl_global->gen ()->create_mirror_port (
                  &sn,
                  pt);

              (void) s->fe_add_mirror_port (mp);
            }

          (tao_yyvsp[(2) - (3)].idlist)->destroy ();
          delete (tao_yyvsp[(2) - (3)].idlist);
          (tao_yyvsp[(2) - (3)].idlist) = 0;
        }
    break;

  case 544:

    {
// at_least_one_actual_parameter : actual_parameter actual_parameters
          if ((tao_yyvsp[(2) - (2)].alval) == 0)
            {
              ACE_NEW_RETURN ((tao_yyvsp[(2) - (2)].alval),
                              FE_Utils::T_ARGLIST,
                              1);
            }

          (tao_yyvsp[(2) - (2)].alval)->enqueue_head ((tao_yyvsp[(1) - (2)].dcval));
          (tao_yyval.alval) = (tao_yyvsp[(2) - (2)].alval);
        }
    break;

  case 545:

    {
// actual_parameters : actual_parameters ',' actual_parameter
          if ((tao_yyvsp[(1) - (3)].alval) == 0)
            {
              ACE_NEW_RETURN ((tao_yyvsp[(1) - (3)].alval),
                              FE_Utils::T_ARGLIST,
                              1);
            }

          (tao_yyvsp[(1) - (3)].alval)->enqueue_tail ((tao_yyvsp[(3) - (3)].dcval));
          (tao_yyval.alval) = (tao_yyvsp[(1) - (3)].alval);
        }
    break;

  case 546:

    {
//         | /* EMPTY */
          (tao_yyval.alval) = 0;
        }
    break;

  case 547:

    {
// actual_parameter : expression
          // To avoid grammar conflicts with this LALR(1) parser,
          // we take advantage of the fact that an expression can
          // be a scoped name. At that lower level, we create an
          // expression containing the scoped name, and at a
          // higher lever, deduce that it's not supposed to be
          // a constant and look up the type to add to the template
          // arg list.
          AST_Expression *ex = (tao_yyvsp[(1) - (1)].exval);
          UTL_ScopedName *sn = ex->n ();
          AST_Decl *d = 0;
          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          if (sn != 0)
            {
              d = s->lookup_by_name (sn, true);

              if (d == 0)
                {
                  idl_global->err ()->lookup_error (sn);
                  return 1;
                }
              else
                {
                  AST_Decl::NodeType nt = d->node_type ();

                  if (nt == AST_Decl::NT_enum_val)
                    {
                      (tao_yyvsp[(1) - (1)].exval)->evaluate (
                        AST_Expression::EK_const);

                      (tao_yyval.dcval) =
                        idl_global->gen ()->create_constant (
                          (tao_yyvsp[(1) - (1)].exval)->ev ()->et,
                          (tao_yyvsp[(1) - (1)].exval),
                          sn);
                    }
                  else
                    {
                      (tao_yyval.dcval) = d;
                    }
                }
            }
          else
            {
              (tao_yyvsp[(1) - (1)].exval)->evaluate (
                AST_Expression::EK_const);

              (tao_yyval.dcval) =
                idl_global->gen ()->create_constant (
                  (tao_yyvsp[(1) - (1)].exval)->ev ()->et,
                  (tao_yyvsp[(1) - (1)].exval),
                  0);
            }
        }
    break;

  case 549:

    {
// connector_header : IDL_CONNECTOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSeen);
        }
    break;

  case 550:

    {
//        IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorIDSeen);
        }
    break;

  case 551:

    {
//        component_inheritance_spec
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Connector *parent = 0;
          bool so_far_so_good = true;

          Identifier id ((tao_yyvsp[(3) - (5)].strval));
          ACE::strdelete ((tao_yyvsp[(3) - (5)].strval));
          (tao_yyvsp[(3) - (5)].strval) = 0;

          UTL_ScopedName sn (&id, 0);

          if ((tao_yyvsp[(5) - (5)].idlist) != 0)
            {
              AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(5) - (5)].idlist), true);

              if (d == 0)
                {
                  idl_global->err ()->lookup_error ((tao_yyvsp[(5) - (5)].idlist));
                  so_far_so_good = false;
                }

              parent =
                AST_Connector::narrow_from_decl (d);

              if (parent == 0)
                {
                  idl_global->err ()->error1 (
                    UTL_Error::EIDL_CONNECTOR_EXPECTED,
                    d);

                  so_far_so_good = false;
                }

              (tao_yyvsp[(5) - (5)].idlist)->destroy ();
              delete (tao_yyvsp[(5) - (5)].idlist);
              (tao_yyvsp[(5) - (5)].idlist) = 0;
            }

          if (so_far_so_good)
            {
              AST_Connector *c =
                idl_global->gen ()->create_connector (&sn,
                                                      parent);

              (void) s->fe_add_connector (c);

              // Push it on the scopes stack.
              idl_global->scopes ().push (c);
           }
        }
    break;

  case 552:

    {
// connector_body " '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSqSeen);
        }
    break;

  case 553:

    {
//        at_least_one_connector_export
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorBodySeen);
        }
    break;

  case 554:

    {
//        '}
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorQsSeen);

          // Done with this connector - pop it off the scope stack.
          idl_global->scopes ().pop ();
        }
    break;

  case 558:

    {
// connector_export : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
    break;

  case 559:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 560:

    {
//        | uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
    break;

  case 561:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 562:

    {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
    break;

  case 563:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 564:

    {
//      | extended_port_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
    break;

  case 565:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
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
              "%C\n",
              msg));
}

