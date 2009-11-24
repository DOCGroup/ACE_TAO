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
#include "ast_valuebox.h"
#include "ast_valuetype.h"
#include "ast_valuetype_fwd.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_component.h"
#include "ast_component_fwd.h"
#include "ast_home.h"
#include "ast_template_interface.h"
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
  FE_Template_InterfaceHeader   *thval;         /* Template interface hdr */
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
#define TAO_YYLAST   1286

/* TAO_YYNTOKENS -- Number of terminals.  */
#define TAO_YYNTOKENS  106
/* TAO_YYNNTS -- Number of nonterminals.  */
#define TAO_YYNNTS  383
/* TAO_YYNRULES -- Number of rules.  */
#define TAO_YYNRULES  565
/* TAO_YYNRULES -- Number of states.  */
#define TAO_YYNSTATES  807

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
     145,   155,   156,   157,   164,   166,   168,   169,   170,   171,
     179,   180,   184,   187,   191,   195,   196,   201,   202,   204,
     206,   208,   210,   213,   215,   216,   217,   218,   226,   227,
     228,   229,   238,   239,   244,   245,   249,   251,   252,   255,
     256,   259,   261,   264,   267,   268,   270,   272,   275,   276,
     280,   281,   285,   288,   289,   290,   294,   295,   299,   300,
     304,   305,   309,   310,   314,   315,   319,   320,   324,   325,
     329,   332,   333,   338,   339,   341,   342,   346,   347,   352,
     354,   356,   359,   362,   363,   364,   365,   366,   376,   378,
     380,   382,   384,   386,   388,   390,   392,   394,   396,   398,
     400,   404,   406,   410,   412,   416,   418,   422,   426,   428,
     432,   436,   438,   442,   446,   450,   452,   455,   458,   461,
     463,   465,   469,   471,   473,   475,   477,   479,   481,   483,
     485,   487,   489,   490,   494,   496,   498,   500,   503,   505,
     506,   510,   512,   514,   516,   518,   520,   522,   524,   526,
     528,   530,   532,   534,   536,   538,   540,   542,   544,   546,
     548,   550,   552,   555,   556,   561,   562,   564,   566,   569,
     570,   575,   576,   578,   580,   582,   584,   586,   589,   591,
     594,   598,   601,   603,   605,   608,   610,   612,   614,   616,
     618,   620,   622,   623,   627,   628,   629,   630,   638,   641,
     644,   645,   646,   649,   650,   651,   657,   658,   662,   663,
     667,   668,   669,   670,   671,   672,   673,   688,   690,   692,
     694,   696,   698,   700,   703,   706,   707,   708,   709,   715,
     716,   720,   723,   726,   727,   728,   732,   733,   734,   740,
     741,   745,   747,   749,   750,   751,   752,   753,   763,   766,
     767,   772,   773,   775,   776,   777,   784,   787,   788,   789,
     795,   796,   797,   804,   806,   808,   809,   810,   817,   819,
     821,   822,   826,   829,   832,   833,   834,   835,   841,   843,
     845,   846,   847,   848,   849,   859,   860,   861,   862,   863,
     873,   874,   875,   876,   877,   887,   888,   889,   890,   891,
     902,   904,   906,   907,   909,   911,   912,   913,   914,   922,
     923,   927,   928,   933,   936,   937,   942,   943,   944,   945,
     951,   952,   956,   957,   962,   965,   966,   971,   972,   973,
     974,   980,   982,   984,   986,   988,   990,   992,   994,   995,
     996,  1003,  1004,  1005,  1006,  1013,  1014,  1015,  1016,  1023,
    1024,  1025,  1026,  1033,  1034,  1037,  1038,  1043,  1044,  1048,
    1052,  1054,  1056,  1059,  1060,  1061,  1062,  1070,  1071,  1072,
    1079,  1080,  1084,  1085,  1088,  1089,  1090,  1094,  1095,  1099,
    1100,  1104,  1105,  1109,  1110,  1114,  1115,  1119,  1120,  1124,
    1128,  1130,  1132,  1136,  1139,  1141,  1142,  1146,  1150,  1154,
    1155,  1159,  1160,  1161,  1162,  1163,  1164,  1165,  1179,  1180,
    1184,  1185,  1188,  1189,  1190,  1191,  1197,  1200,  1201,  1203,
    1204,  1208,  1209,  1213,  1214,  1215,  1222,  1223,  1224,  1231,
    1233,  1235,  1237,  1239,  1241,  1244,  1248,  1249,  1250,  1251,
    1260,  1264,  1268,  1271,  1272,  1276,  1277,  1278,  1279,  1288,
    1290,  1292,  1294,  1296,  1298,  1300,  1302,  1304,  1306,  1308,
    1311,  1314,  1315,  1320,  1321,  1324,  1329,  1332,  1336,  1337,
    1339,  1340,  1341,  1342,  1343,  1353,  1356,  1359,  1360,  1361,
    1365,  1366,  1370,  1374,  1378,  1381,  1385,  1386,  1388,  1391,
    1392,  1393,  1399,  1400,  1401,  1407,  1410,  1413,  1414,  1415,
    1419,  1420,  1424,  1425,  1429,  1430
};

/* TAO_YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const tao_yytype_int16 tao_yyrhs[] =
{
     107,     0,    -1,   108,    -1,   108,   109,    -1,    -1,   112,
      -1,   134,    85,    -1,   147,    85,    -1,   111,   112,    -1,
     111,   112,    -1,    -1,    -1,   219,   113,    85,    -1,    -1,
     380,   114,    85,    -1,    -1,   381,   115,    85,    -1,    -1,
     201,   116,    85,    -1,    -1,   329,   117,    85,    -1,    -1,
     150,   118,    85,    -1,    -1,   129,   119,    85,    -1,    -1,
     160,   120,    85,    -1,    -1,   382,   121,    85,    -1,    -1,
     410,   122,    85,    -1,    -1,   435,   123,    85,    -1,    -1,
     461,   124,    85,    -1,    -1,   475,   125,    85,    -1,    -1,
       1,   126,    85,    -1,    -1,     5,   128,   196,    -1,    -1,
      -1,    -1,   127,   130,    86,   131,   110,   132,    87,    -1,
     127,    88,    -1,    -1,    -1,    -1,    -1,    -1,   133,   135,
     454,   136,    89,   137,    86,   138,   140,   139,    87,    -1,
     142,   141,    -1,   141,   142,    -1,    -1,   112,    -1,   143,
      85,    -1,    -1,    -1,    -1,    72,   144,   196,   145,    88,
     458,    89,   146,     3,    -1,    -1,    -1,   133,   148,   472,
      89,   149,   199,    -1,   151,    -1,   200,    -1,    -1,    -1,
      -1,   157,   152,    86,   153,   183,   154,    87,    -1,    -1,
       6,   156,   199,    -1,   155,   158,    -1,    41,   155,   158,
      -1,    42,   155,   158,    -1,    -1,    90,   174,   159,   193,
      -1,    -1,   161,    -1,   166,    -1,   176,    -1,   177,    -1,
      43,   162,    -1,   162,    -1,    -1,    -1,    -1,   170,   163,
      86,   164,   178,   165,    87,    -1,    -1,    -1,    -1,    42,
     170,   167,    86,   168,   183,   169,    87,    -1,    -1,   172,
     158,   171,   175,    -1,    -1,    49,   173,   199,    -1,    48,
      -1,    -1,    47,   193,    -1,    -1,    42,   172,    -1,   172,
      -1,   172,   223,    -1,   178,   179,    -1,    -1,   180,    -1,
     184,    -1,   341,    85,    -1,    -1,    46,   181,   258,    -1,
      -1,    45,   182,   258,    -1,   183,   184,    -1,    -1,    -1,
     219,   185,    85,    -1,    -1,   380,   186,    85,    -1,    -1,
     381,   187,    85,    -1,    -1,   201,   188,    85,    -1,    -1,
     329,   189,    85,    -1,    -1,   318,   190,    85,    -1,    -1,
     334,   191,    85,    -1,    -1,     1,   192,    85,    -1,   196,
     194,    -1,    -1,   194,    91,   195,   196,    -1,    -1,   199,
      -1,    -1,    80,   197,   199,    -1,    -1,   196,    80,   198,
     199,    -1,     3,    -1,   155,    -1,    41,   155,    -1,    42,
     155,    -1,    -1,    -1,    -1,    -1,     4,   202,   206,   203,
     199,   204,    92,   205,   207,    -1,   238,    -1,   243,    -1,
     244,    -1,   245,    -1,   241,    -1,   242,    -1,   303,    -1,
     307,    -1,   196,    -1,   208,    -1,   209,    -1,   210,    -1,
     209,    93,   210,    -1,   211,    -1,   210,    94,   211,    -1,
     212,    -1,   211,    95,   212,    -1,   213,    -1,   212,    81,
     213,    -1,   212,    82,   213,    -1,   214,    -1,   213,    96,
     214,    -1,   213,    97,   214,    -1,   215,    -1,   214,    98,
     215,    -1,   214,    99,   215,    -1,   214,   100,   215,    -1,
     216,    -1,    96,   216,    -1,    97,   216,    -1,   101,   216,
      -1,   196,    -1,   217,    -1,   102,   208,   103,    -1,    73,
      -1,    74,    -1,    75,    -1,    84,    -1,    76,    -1,    83,
      -1,    77,    -1,    78,    -1,    79,    -1,   208,    -1,    -1,
       7,   220,   221,    -1,   250,    -1,   264,    -1,   288,    -1,
      40,   236,    -1,   228,    -1,    -1,   223,   222,   229,    -1,
     224,    -1,   227,    -1,   225,    -1,   226,    -1,   196,    -1,
     238,    -1,   241,    -1,   243,    -1,   245,    -1,   244,    -1,
     242,    -1,   246,    -1,   247,    -1,   297,    -1,   303,    -1,
     307,    -1,   250,    -1,   264,    -1,   288,    -1,   286,    -1,
     287,    -1,   232,   230,    -1,    -1,   230,    91,   231,   232,
      -1,    -1,   236,    -1,   237,    -1,   236,   234,    -1,    -1,
     234,    91,   235,   236,    -1,    -1,   199,    -1,   311,    -1,
     239,    -1,   240,    -1,     8,    -1,     8,     8,    -1,     9,
      -1,    10,     8,    -1,    10,     8,     8,    -1,    10,     9,
      -1,    11,    -1,    12,    -1,     8,    11,    -1,    17,    -1,
      13,    -1,    14,    -1,    15,    -1,    16,    -1,    18,    -1,
      19,    -1,    -1,    20,   249,   199,    -1,    -1,    -1,    -1,
     248,   251,    86,   252,   254,   253,    87,    -1,   256,   255,
      -1,   255,   256,    -1,    -1,    -1,   257,   258,    -1,    -1,
      -1,   223,   259,   229,   260,    85,    -1,    -1,     1,   261,
      85,    -1,    -1,    21,   263,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   262,    22,   265,   102,   266,   271,   267,
     103,   268,    86,   269,   272,   270,    87,    -1,   238,    -1,
     243,    -1,   244,    -1,   245,    -1,   288,    -1,   196,    -1,
     274,   273,    -1,   273,   274,    -1,    -1,    -1,    -1,   278,
     275,   284,   276,    85,    -1,    -1,     1,   277,    85,    -1,
     280,   279,    -1,   279,   280,    -1,    -1,    -1,    29,   281,
      90,    -1,    -1,    -1,    28,   282,   208,   283,    90,    -1,
      -1,   223,   285,   232,    -1,   248,    -1,   262,    -1,    -1,
      -1,    -1,    -1,    23,   289,   199,   290,    86,   291,   293,
     292,    87,    -1,   296,   294,    -1,    -1,   294,    91,   295,
     296,    -1,    -1,     3,    -1,    -1,    -1,   300,    91,   298,
     218,   299,    89,    -1,   300,    89,    -1,    -1,    -1,    24,
     301,    88,   302,   224,    -1,    -1,    -1,   306,    88,   304,
     218,   305,    89,    -1,   306,    -1,    25,    -1,    -1,    -1,
     310,    88,   308,   218,   309,    89,    -1,   310,    -1,    26,
      -1,    -1,   199,   312,   313,    -1,   315,   314,    -1,   314,
     315,    -1,    -1,    -1,    -1,   104,   316,   218,   317,   105,
      -1,   319,    -1,   324,    -1,    -1,    -1,    -1,    -1,    30,
     320,    31,   321,   363,   322,   233,   323,   365,    -1,    -1,
      -1,    -1,    -1,    31,   325,   363,   326,   233,   327,   368,
     328,   371,    -1,    -1,    -1,    -1,    -1,    27,   330,   199,
     331,    86,   332,   255,   333,    87,    -1,    -1,    -1,    -1,
      -1,   339,   340,   335,     3,   336,   354,   337,   365,   338,
     374,    -1,    32,    -1,    33,    -1,    -1,   363,    -1,    34,
      -1,    -1,    -1,    -1,    44,   342,     3,   343,   345,   344,
     365,    -1,    -1,   102,   346,   103,    -1,    -1,   102,   347,
     348,   103,    -1,   351,   349,    -1,    -1,   349,    91,   350,
     351,    -1,    -1,    -1,    -1,    35,   352,   363,   353,   232,
      -1,    -1,   102,   355,   103,    -1,    -1,   102,   356,   357,
     103,    -1,   360,   358,    -1,    -1,   358,    91,   359,   360,
      -1,    -1,    -1,    -1,   364,   361,   363,   362,   232,    -1,
     225,    -1,   303,    -1,   307,    -1,   196,    -1,    35,    -1,
      36,    -1,    37,    -1,    -1,    -1,    38,   366,   102,   367,
     193,   103,    -1,    -1,    -1,    -1,    55,   369,   102,   370,
     193,   103,    -1,    -1,    -1,    -1,    62,   372,   102,   373,
     193,   103,    -1,    -1,    -1,    -1,    39,   375,   102,   376,
     377,   103,    -1,    -1,    75,   378,    -1,    -1,   378,    91,
     379,    75,    -1,    -1,    63,   196,    75,    -1,    64,   196,
      75,    -1,   384,    -1,   383,    -1,    50,   199,    -1,    -1,
      -1,    -1,   388,   385,    86,   386,   393,   387,    87,    -1,
      -1,    -1,    50,   199,   389,   391,   390,   175,    -1,    -1,
      90,   392,   196,    -1,    -1,   393,   394,    -1,    -1,    -1,
     402,   395,    85,    -1,    -1,   404,   396,    85,    -1,    -1,
     407,   397,    85,    -1,    -1,   408,   398,    85,    -1,    -1,
     409,   399,    85,    -1,    -1,   318,   400,    85,    -1,    -1,
     471,   401,    85,    -1,    60,   403,   199,    -1,   196,    -1,
      19,    -1,   405,   403,   199,    -1,    65,   406,    -1,    58,
      -1,    -1,    52,   196,   199,    -1,    61,   196,   199,    -1,
      51,   196,   199,    -1,    -1,   412,   411,   422,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,   413,   199,   414,   419,
     415,   175,   416,    66,   417,   196,   418,   421,    -1,    -1,
      90,   420,   196,    -1,    -1,    59,   196,    -1,    -1,    -1,
      -1,    86,   423,   425,   424,    87,    -1,   425,   426,    -1,
      -1,   184,    -1,    -1,   429,   427,    85,    -1,    -1,   432,
     428,    85,    -1,    -1,    -1,    44,   199,   430,   345,   431,
     365,    -1,    -1,    -1,    54,   199,   433,   345,   434,   365,
      -1,   448,    -1,   439,    -1,   436,    -1,   438,    -1,   437,
      -1,    53,   199,    -1,    42,    53,   199,    -1,    -1,    -1,
      -1,   443,   446,   440,    86,   441,   183,   442,    87,    -1,
      42,    53,   199,    -1,    43,    53,   199,    -1,    53,   199,
      -1,    -1,   158,   447,   175,    -1,    -1,    -1,    -1,   452,
     446,   449,    86,   450,   178,   451,    87,    -1,   444,    -1,
     445,    -1,    67,    -1,    20,    -1,    21,    -1,    53,    -1,
      24,    -1,     6,    -1,    49,    -1,    27,    -1,     4,   206,
      -1,   457,   455,    -1,    -1,   455,    91,   456,   457,    -1,
      -1,   453,     3,    -1,    24,    88,     3,    89,    -1,   460,
     459,    -1,   459,    91,   460,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    70,   462,     3,   463,    86,   464,   466,
     465,    87,    -1,   468,   467,    -1,   467,   468,    -1,    -1,
      -1,   402,   469,    85,    -1,    -1,   404,   470,    85,    -1,
      68,   196,     3,    -1,    69,   196,     3,    -1,   474,   473,
      -1,   473,    91,   474,    -1,    -1,   207,    -1,   476,   479,
      -1,    -1,    -1,    71,   477,     3,   478,   391,    -1,    -1,
      -1,    86,   480,   482,   481,    87,    -1,   484,   483,    -1,
     483,   484,    -1,    -1,    -1,   402,   485,    85,    -1,    -1,
     404,   486,    85,    -1,    -1,   318,   487,    85,    -1,    -1,
     471,   488,    85,    -1
};

/* TAO_YYRLINE[TAO_YYN] -- source line where rule number TAO_YYN was defined.  */
static const tao_yytype_uint16 tao_yyrline[] =
{
       0,   355,   355,   358,   359,   363,   366,   369,   375,   379,
     380,   385,   384,   395,   394,   405,   404,   415,   414,   425,
     424,   435,   434,   445,   444,   455,   454,   465,   464,   475,
     474,   485,   484,   495,   494,   505,   504,   515,   514,   529,
     528,   541,   564,   569,   540,   585,   593,   598,   601,   619,
     623,   592,   638,   642,   643,   647,   648,   653,   656,   659,
     652,   668,   671,   667,   679,   680,   685,   724,   729,   684,
     747,   746,   760,   798,   829,   863,   862,   874,   881,   882,
     883,   884,   888,   899,   904,   948,   953,   903,   982,  1021,
    1026,   980,  1045,  1043,  1083,  1082,  1096,  1102,  1109,  1116,
    1123,  1149,  1176,  1241,  1242,  1246,  1247,  1248,  1254,  1253,
    1261,  1260,  1270,  1271,  1276,  1275,  1286,  1285,  1296,  1295,
    1306,  1305,  1316,  1315,  1326,  1325,  1336,  1335,  1346,  1345,
    1359,  1372,  1370,  1400,  1407,  1418,  1417,  1445,  1443,  1470,
    1482,  1528,  1556,  1587,  1592,  1597,  1602,  1586,  1655,  1656,
    1657,  1658,  1659,  1660,  1661,  1673,  1678,  1753,  1755,  1757,
    1758,  1772,  1773,  1787,  1788,  1801,  1802,  1812,  1825,  1826,
    1836,  1849,  1850,  1860,  1870,  1883,  1884,  1894,  1904,  1917,
    1952,  1953,  1962,  1967,  1974,  1982,  1987,  1992,  1998,  2003,
    2008,  2016,  2088,  2087,  2097,  2102,  2107,  2112,  2139,  2148,
    2147,  2218,  2219,  2223,  2231,  2232,  2260,  2261,  2262,  2263,
    2264,  2265,  2266,  2267,  2271,  2272,  2273,  2277,  2278,  2279,
    2283,  2284,  2288,  2301,  2299,  2329,  2336,  2337,  2341,  2354,
    2352,  2382,  2389,  2406,  2425,  2426,  2430,  2435,  2440,  2448,
    2453,  2458,  2466,  2471,  2476,  2484,  2498,  2503,  2511,  2519,
    2527,  2535,  2544,  2543,  2559,  2593,  2598,  2558,  2617,  2620,
    2621,  2625,  2625,  2635,  2640,  2634,  2703,  2702,  2717,  2716,
    2731,  2736,  2741,  2746,  2793,  2798,  2730,  2822,  2830,  2844,
    2854,  2862,  2863,  2971,  2974,  2975,  2980,  2985,  2979,  3021,
    3020,  3034,  3045,  3067,  3075,  3074,  3090,  3095,  3089,  3112,
    3111,  3164,  3188,  3213,  3218,  3251,  3256,  3212,  3282,  3287,
    3285,  3292,  3296,  3333,  3338,  3331,  3405,  3460,  3470,  3459,
    3485,  3490,  3483,  3531,  3557,  3567,  3572,  3565,  3602,  3627,
    3636,  3635,  3671,  3682,  3704,  3712,  3717,  3711,  3754,  3755,
    3760,  3765,  3770,  3775,  3759,  3844,  3849,  3854,  3859,  3843,
    3937,  3942,  3972,  3977,  3936,  3995,  4000,  4053,  4058,  3993,
    4095,  4101,  4108,  4115,  4116,  4128,  4134,  4176,  4127,  4198,
    4197,  4208,  4207,  4220,  4225,  4223,  4230,  4235,  4240,  4234,
    4281,  4280,  4291,  4290,  4303,  4308,  4306,  4313,  4318,  4323,
    4317,  4370,  4378,  4379,  4380,  4485,  4490,  4495,  4504,  4509,
    4503,  4521,  4529,  4534,  4528,  4546,  4554,  4559,  4553,  4571,
    4579,  4584,  4578,  4596,  4603,  4616,  4614,  4642,  4649,  4678,
    4716,  4717,  4721,  4751,  4791,  4796,  4750,  4815,  4820,  4813,
    4862,  4861,  4872,  4879,  4880,  4885,  4884,  4895,  4894,  4905,
    4904,  4915,  4914,  4925,  4924,  4935,  4934,  4945,  4944,  4956,
    5015,  5022,  5046,  5120,  5130,  5136,  5142,  5189,  5236,  5285,
    5284,  5334,  5339,  5344,  5349,  5354,  5359,  5333,  5413,  5412,
    5423,  5430,  5437,  5445,  5450,  5444,  5462,  5463,  5467,  5469,
    5468,  5479,  5478,  5493,  5529,  5491,  5563,  5599,  5561,  5631,
    5632,  5633,  5637,  5638,  5642,  5670,  5701,  5746,  5751,  5699,
    5768,  5778,  5797,  5809,  5808,  5848,  5898,  5903,  5846,  5920,
    5925,  5933,  5938,  5943,  5948,  5953,  5958,  5963,  5968,  5973,
    5981,  5999,  5998,  6041,  6048,  6059,  6073,  6080,  6104,  6111,
    6122,  6127,  6132,  6137,  6121,  6152,  6159,  6164,  6171,  6170,
    6179,  6178,  6189,  6258,  6308,  6312,  6313,  6317,  6331,  6336,
    6341,  6335,  6401,  6406,  6400,  6421,  6425,  6426,  6431,  6430,
    6441,  6440,  6451,  6450,  6461,  6460
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
  "tpl_definition", "template_module_ref", "@24", "@25", "@26",
  "template_module_inst", "@27", "@28", "interface_def", "interface",
  "@29", "@30", "@31", "interface_decl", "@32", "interface_header",
  "inheritance_spec", "@33", "value_def", "valuetype",
  "value_concrete_decl", "@34", "@35", "@36", "value_abs_decl", "@37",
  "@38", "@39", "value_header", "@40", "value_decl", "@41",
  "opt_truncatable", "supports_spec", "value_forward_decl",
  "value_box_decl", "value_elements", "value_element", "state_member",
  "@42", "@43", "exports", "export", "@44", "@45", "@46", "@47", "@48",
  "@49", "@50", "@51", "at_least_one_scoped_name", "scoped_names", "@52",
  "scoped_name", "@53", "@54", "id", "interface_forward", "const_dcl",
  "@55", "@56", "@57", "@58", "const_type", "expression", "const_expr",
  "or_expr", "xor_expr", "and_expr", "shift_expr", "add_expr", "mult_expr",
  "unary_expr", "primary_expr", "literal", "positive_int_expr", "type_dcl",
  "@59", "type_declarator", "@60", "type_spec", "simple_type_spec",
  "base_type_spec", "template_type_spec", "constructed_type_spec",
  "constructed_forward_type_spec", "at_least_one_declarator",
  "declarators", "@61", "declarator", "at_least_one_simple_declarator",
  "simple_declarators", "@62", "simple_declarator", "complex_declarator",
  "integer_type", "signed_int", "unsigned_int", "floating_pt_type",
  "fixed_type", "char_type", "octet_type", "boolean_type", "any_type",
  "object_type", "struct_decl", "@63", "struct_type", "@64", "@65", "@66",
  "at_least_one_member", "members", "member", "@67", "member_i", "@68",
  "@69", "@70", "union_decl", "@71", "union_type", "@72", "@73", "@74",
  "@75", "@76", "@77", "switch_type_spec", "at_least_one_case_branch",
  "case_branches", "case_branch", "@78", "@79", "@80",
  "at_least_one_case_label", "case_labels", "case_label", "@81", "@82",
  "@83", "element_spec", "@84", "struct_forward_type",
  "union_forward_type", "enum_type", "@85", "@86", "@87", "@88",
  "at_least_one_enumerator", "enumerators", "@89", "enumerator",
  "sequence_type_spec", "@90", "@91", "seq_head", "@92", "@93",
  "string_type_spec", "@94", "@95", "string_head", "wstring_type_spec",
  "@96", "@97", "wstring_head", "array_declarator", "@98",
  "at_least_one_array_dim", "array_dims", "array_dim", "@99", "@100",
  "attribute", "attribute_readonly", "@101", "@102", "@103", "@104",
  "attribute_readwrite", "@105", "@106", "@107", "@108", "exception",
  "@109", "@110", "@111", "@112", "operation", "@113", "@114", "@115",
  "@116", "opt_op_attribute", "op_type_spec", "init_decl", "@117", "@118",
  "@119", "init_parameter_list", "@120", "@121",
  "at_least_one_in_parameter", "in_parameters", "@122", "in_parameter",
  "@123", "@124", "parameter_list", "@125", "@126",
  "at_least_one_parameter", "parameters", "@127", "parameter", "@128",
  "@129", "param_type_spec", "direction", "opt_raises", "@130", "@131",
  "opt_getraises", "@132", "@133", "opt_setraises", "@134", "@135",
  "opt_context", "@136", "@137", "at_least_one_string_literal",
  "string_literals", "@138", "typeid_dcl", "typeprefix_dcl", "component",
  "component_forward_decl", "component_decl", "@139", "@140", "@141",
  "component_header", "@142", "@143", "component_inheritance_spec", "@144",
  "component_exports", "component_export", "@145", "@146", "@147", "@148",
  "@149", "@150", "@151", "provides_decl", "interface_type", "uses_decl",
  "uses_opt_multiple", "opt_multiple", "emits_decl", "publishes_decl",
  "consumes_decl", "home_decl", "@152", "home_header", "@153", "@154",
  "@155", "@156", "@157", "@158", "home_inheritance_spec", "@159",
  "primary_key_spec", "home_body", "@160", "@161", "home_exports",
  "home_export", "@162", "@163", "factory_decl", "@164", "@165",
  "finder_decl", "@166", "@167", "event", "event_forward_decl",
  "event_concrete_forward_decl", "event_abs_forward_decl",
  "event_abs_decl", "@168", "@169", "@170", "event_abs_header",
  "event_custom_header", "event_plain_header", "event_rest_of_header",
  "@171", "event_decl", "@172", "@173", "@174", "event_header",
  "formal_parameter_type", "at_least_one_formal_parameter",
  "formal_parameters", "@175", "formal_parameter",
  "at_least_one_formal_parameter_name", "formal_parameter_names",
  "formal_parameter_name", "porttype_decl", "@176", "@177", "@178", "@179",
  "at_least_one_port_export", "port_exports", "port_export", "@180",
  "@181", "extended_port_decl", "at_least_one_actual_parameter",
  "actual_parameters", "actual_parameter", "connector_decl",
  "connector_header", "@182", "@183", "connector_body", "@184", "@185",
  "at_least_one_connector_export", "connector_exports", "connector_export",
  "@186", "@187", "@188", "@189", 0
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
     139,   134,   140,   141,   141,   142,   142,   144,   145,   146,
     143,   148,   149,   147,   150,   150,   152,   153,   154,   151,
     156,   155,   157,   157,   157,   159,   158,   158,   160,   160,
     160,   160,   161,   161,   163,   164,   165,   162,   167,   168,
     169,   166,   171,   170,   173,   172,   174,   174,   175,   175,
     176,   176,   177,   178,   178,   179,   179,   179,   181,   180,
     182,   180,   183,   183,   185,   184,   186,   184,   187,   184,
     188,   184,   189,   184,   190,   184,   191,   184,   192,   184,
     193,   195,   194,   194,   196,   197,   196,   198,   196,   199,
     200,   200,   200,   202,   203,   204,   205,   201,   206,   206,
     206,   206,   206,   206,   206,   206,   206,   207,   208,   209,
     209,   210,   210,   211,   211,   212,   212,   212,   213,   213,
     213,   214,   214,   214,   214,   215,   215,   215,   215,   216,
     216,   216,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   218,   220,   219,   219,   219,   219,   219,   219,   222,
     221,   223,   223,   224,   224,   224,   225,   225,   225,   225,
     225,   225,   225,   225,   226,   226,   226,   227,   227,   227,
     228,   228,   229,   231,   230,   230,   232,   232,   233,   235,
     234,   234,   236,   237,   238,   238,   239,   239,   239,   240,
     240,   240,   241,   241,   241,   242,   243,   243,   244,   245,
     246,   247,   249,   248,   251,   252,   253,   250,   254,   255,
     255,   257,   256,   259,   260,   258,   261,   258,   263,   262,
     265,   266,   267,   268,   269,   270,   264,   271,   271,   271,
     271,   271,   271,   272,   273,   273,   275,   276,   274,   277,
     274,   278,   279,   279,   281,   280,   282,   283,   280,   285,
     284,   286,   287,   289,   290,   291,   292,   288,   293,   295,
     294,   294,   296,   298,   299,   297,   297,   301,   302,   300,
     304,   305,   303,   303,   306,   308,   309,   307,   307,   310,
     312,   311,   313,   314,   314,   316,   317,   315,   318,   318,
     320,   321,   322,   323,   319,   325,   326,   327,   328,   324,
     330,   331,   332,   333,   329,   335,   336,   337,   338,   334,
     339,   339,   339,   340,   340,   342,   343,   344,   341,   346,
     345,   347,   345,   348,   350,   349,   349,   352,   353,   351,
     355,   354,   356,   354,   357,   359,   358,   358,   361,   362,
     360,   363,   363,   363,   363,   364,   364,   364,   366,   367,
     365,   365,   369,   370,   368,   368,   372,   373,   371,   371,
     375,   376,   374,   374,   377,   379,   378,   378,   380,   381,
     382,   382,   383,   385,   386,   387,   384,   389,   390,   388,
     392,   391,   391,   393,   393,   395,   394,   396,   394,   397,
     394,   398,   394,   399,   394,   400,   394,   401,   394,   402,
     403,   403,   404,   405,   406,   406,   407,   408,   409,   411,
     410,   413,   414,   415,   416,   417,   418,   412,   420,   419,
     419,   421,   421,   423,   424,   422,   425,   425,   426,   427,
     426,   428,   426,   430,   431,   429,   433,   434,   432,   435,
     435,   435,   436,   436,   437,   438,   440,   441,   442,   439,
     443,   444,   445,   447,   446,   449,   450,   451,   448,   452,
     452,   453,   453,   453,   453,   453,   453,   453,   453,   453,
     454,   456,   455,   455,   457,   457,   458,   459,   459,   460,
     462,   463,   464,   465,   461,   466,   467,   467,   469,   468,
     470,   468,   471,   471,   472,   473,   473,   474,   475,   477,
     478,   476,   480,   481,   479,   482,   483,   483,   485,   484,
     486,   484,   487,   484,   488,   484
};

/* TAO_YYR2[TAO_YYN] -- Number of symbols composing right hand side of rule TAO_YYN.  */
static const tao_yytype_uint8 tao_yyr2[] =
{
       0,     2,     1,     2,     0,     1,     2,     2,     2,     2,
       0,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     0,     0,     7,     2,     0,     0,     0,     0,
       0,    11,     2,     2,     0,     1,     2,     0,     0,     0,
       9,     0,     0,     6,     1,     1,     0,     0,     0,     7,
       0,     3,     2,     3,     3,     0,     4,     0,     1,     1,
       1,     1,     2,     1,     0,     0,     0,     7,     0,     0,
       0,     8,     0,     4,     0,     3,     1,     0,     2,     0,
       2,     1,     2,     2,     0,     1,     1,     2,     0,     3,
       0,     3,     2,     0,     0,     3,     0,     3,     0,     3,
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
       3,     0,     3,     0,     3,     0,     3,     0,     3,     3,
       1,     1,     3,     2,     1,     0,     3,     3,     3,     0,
       3,     0,     0,     0,     0,     0,     0,    13,     0,     3,
       0,     2,     0,     0,     0,     5,     2,     0,     1,     0,
       3,     0,     3,     0,     0,     6,     0,     0,     6,     1,
       1,     1,     1,     1,     2,     3,     0,     0,     0,     8,
       3,     3,     2,     0,     3,     0,     0,     0,     8,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     0,     4,     0,     2,     4,     2,     3,     0,     1,
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
       4,     0,     0,     1,    37,   143,    39,    70,   192,   252,
     268,   303,   350,     0,     0,     0,     0,    94,     0,     0,
     461,     0,     0,   530,   549,     3,     5,    41,    23,    61,
       0,     0,    21,    64,    77,    66,    25,    78,    83,    79,
      84,    77,    80,    81,    65,    17,    11,   198,   254,   194,
     302,   195,   220,   221,   196,    19,    13,    15,    27,   421,
     420,   423,    29,   459,    31,   491,   493,   492,   490,    77,
     509,   510,   489,    77,    33,    35,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   139,   232,   197,    77,
       0,    77,    88,    77,     0,    82,    77,     0,   427,   502,
       0,   135,     0,   134,     0,     0,     0,    45,     0,     0,
       0,     0,     6,     7,     0,    97,    72,     0,     0,     0,
     236,   238,     0,   242,   243,   246,   247,   248,   249,   245,
     250,   251,   317,   324,   329,    92,   205,   102,   201,   203,
     204,   202,   206,   234,   235,   207,   211,   208,   210,   209,
     212,   213,   254,   217,     0,   218,   219,   214,     0,   215,
     323,   216,   328,     0,     0,     0,   270,     0,     0,     0,
       0,     0,     0,     0,     0,   503,   496,   505,     0,     0,
     552,   548,    38,   156,   144,   148,   152,   153,   149,   150,
     151,   154,   155,    40,    71,   193,   199,   253,   269,   304,
     351,    73,   500,    74,     0,   501,    95,   432,   462,     0,
     418,   137,   419,   531,   550,    42,    24,     0,   516,   512,
     513,   515,   518,   517,   514,   511,     0,    47,   523,   182,
     183,   184,   186,   188,   189,   190,   187,   185,     0,     0,
       0,     0,   179,   547,   157,   158,   159,   161,   163,   165,
     168,   171,   175,   180,     0,   546,    22,    96,    75,    67,
      26,    85,   237,   244,   239,   241,     0,    99,   316,   313,
     320,   325,    18,    12,   255,     0,    20,    14,    16,    28,
     424,    30,   473,   460,    32,    99,     0,     0,    34,    36,
       0,     0,     0,     0,     0,    89,   430,   428,   470,   136,
       0,     0,   432,    10,   519,     0,   524,     0,   520,   176,
     177,   178,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    62,   544,     0,   113,   104,   240,   318,
       0,    93,     0,     0,     0,   261,   271,   434,   477,   504,
     497,   506,   340,   345,     0,   455,     0,     0,   562,   338,
     339,   558,   560,     0,   564,   553,   557,   145,   232,   200,
     225,   226,   227,   233,   305,   352,   113,     0,    99,   468,
     463,   138,   532,   551,    43,     0,     0,    48,   521,   181,
     160,   162,   164,   166,   167,   169,   170,   172,   173,   174,
       0,     0,    76,   133,     0,     0,     0,    98,   191,   314,
     321,   326,   256,   260,     0,     0,   425,     0,   113,   104,
       0,     0,   451,   450,     0,   454,   453,     0,     0,     0,
       0,     0,     0,     0,     0,   555,     0,     0,   222,     0,
     260,     0,   431,   429,     0,    99,     0,     0,     9,    41,
     525,     0,     0,    63,   545,   130,   128,   360,   361,     0,
     112,   120,   114,   124,   122,   126,     0,   116,   118,   365,
     110,   108,     0,   103,   105,   106,     0,   319,     0,     0,
       0,     0,   261,   266,   263,   262,   236,   282,   277,   278,
     279,   280,   272,   281,     0,     0,     0,   445,     0,   433,
     435,   437,   439,   441,   443,   447,     0,     0,   478,     0,
     476,   479,   481,     0,     0,   341,   394,   391,   392,   393,
     346,   449,   542,   543,   563,   559,   561,   452,   565,   554,
     556,   146,   335,   331,   334,   223,   312,   306,   311,   261,
       0,   469,   464,   538,   540,   533,   537,    44,    49,   522,
     131,     0,    69,     0,     0,     0,     0,     0,   364,   355,
     363,     0,     0,     0,     0,     0,    87,   107,   315,   322,
     327,   257,   259,     0,     0,     0,     0,     0,     0,     0,
     426,     0,     0,     0,     0,     0,     0,   483,   486,   475,
       0,     0,     0,     0,     0,     0,     0,     0,   332,     0,
       0,   308,     0,    91,     0,     0,     0,     0,   535,     0,
       0,   129,   121,   115,   125,   123,   127,     0,   117,   119,
     366,   111,   109,   267,   264,   273,   458,   456,   457,   446,
     436,   438,   440,   442,   444,   448,     0,     0,   480,   482,
     499,   508,   342,   347,   231,   147,   336,   333,   224,   307,
     309,   354,   465,   539,   541,   534,   536,    57,    55,    50,
      54,     0,   132,   356,     0,     0,     0,   369,   484,   487,
       0,   405,   228,     0,     0,     0,     0,     0,     0,    56,
       0,   367,   265,   274,     0,     0,   401,   401,   343,   402,
     348,   229,   337,   310,   466,    58,    51,    53,   382,   357,
     401,     0,   370,   377,     0,   376,   398,   485,   488,   401,
       0,   409,     0,   472,     0,     0,     0,   401,   368,   289,
     296,   294,   275,   285,   286,   293,     0,   372,   373,     0,
     344,   403,   406,   349,   230,     0,   467,     0,   381,   395,
     396,   397,     0,   387,   388,   358,     0,     0,     0,     0,
       0,     0,   291,   378,   374,   399,     0,     0,   471,   529,
       0,   528,   383,   384,     0,   413,   290,   297,   295,   276,
     284,   299,   287,   292,     0,     0,     0,     0,   407,    59,
     526,   385,   389,   410,   359,     0,     0,     0,   379,   375,
       0,   404,     0,     0,     0,     0,     0,     0,   298,   300,
     288,   400,     0,    60,   527,   386,   390,   411,   408,     0,
     417,     0,   414,   412,   415,     0,   416
};

/* TAO_YYDEFGOTO[NTERM-NUM].  */
static const tao_yytype_int16 tao_yydefgoto[] =
{
      -1,     1,     2,    25,   374,   375,   648,   164,   168,   169,
     163,   167,   114,   109,   118,   170,   172,   174,   178,   179,
      77,   439,    79,    28,   108,   303,   437,    29,    30,   110,
     307,   441,   599,   667,   649,   668,   650,   651,   666,   704,
     783,    31,   111,   390,    32,    33,   117,   326,   449,    34,
      80,    35,   135,   325,    36,    37,    38,   119,   327,   462,
      39,   204,   366,   530,    40,   267,    41,    97,   258,   331,
      42,    43,   395,   463,   464,   555,   554,   394,   450,   544,
     551,   552,   543,   546,   545,   547,   541,   392,   445,   600,
     242,   209,   300,   103,    44,   451,    78,   291,   426,   586,
     184,   243,   398,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   399,   452,    81,   195,   292,   474,   138,   139,
     140,   141,    47,   359,   428,   589,   360,   633,   662,   702,
     361,   362,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,    48,    82,    49,   165,   335,   471,   402,   472,
     562,   404,   475,   564,   655,   563,    50,    83,    51,   275,
     405,   565,   656,   691,   739,   482,   712,   740,   713,   741,
     777,   736,   714,   742,   715,   738,   737,   775,   762,   776,
      52,    53,    54,    84,   293,   429,   590,   527,   591,   664,
     528,   157,   332,   468,   158,   266,   396,   159,   333,   469,
     160,   161,   334,   470,   162,   363,   427,   523,   588,   524,
     587,   663,   453,   349,   410,   584,   660,   699,   350,   411,
     585,   661,   701,   454,    85,   294,   430,   592,   455,   607,
     670,   707,   755,   456,   549,   466,   553,   654,   690,   658,
     674,   675,   694,   718,   765,   695,   716,   764,   689,   705,
     706,   732,   753,   785,   733,   754,   786,   510,   734,   697,
     719,   766,   680,   700,   746,   723,   747,   782,   774,   787,
     799,   801,   802,   805,   457,   458,    58,    59,    60,   171,
     337,   488,    61,   207,   368,   297,   367,   406,   489,   571,
     572,   573,   574,   575,   569,   576,   351,   414,   352,   353,
     416,   492,   493,   494,    62,   173,    63,   100,   298,   435,
     594,   665,   703,   370,   434,   726,   283,   338,   499,   407,
     500,   580,   581,   501,   626,   676,   502,   627,   677,    64,
      65,    66,    67,    68,   286,   408,   582,    69,    70,    71,
     176,   285,    72,   287,   409,   583,    73,   226,   227,   308,
     442,   228,   750,   770,   751,    74,   105,   301,   436,   597,
     535,   598,   536,   595,   596,   354,   254,   324,   255,    75,
      76,   106,   302,   181,   290,   424,   355,   425,   356,   420,
     421,   419,   423
};

/* TAO_YYPACT[STATE-NUM] -- Index in TAO_YYTABLE of the portion describing
   STATE-NUM.  */
#define TAO_YYPACT_NINF -561
static const tao_yytype_int16 tao_yypact[] =
{
    -561,    48,  1137,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,    98,    60,    33,     1,  -561,    98,    98,
    -561,    42,    42,  -561,  -561,  -561,  -561,   -12,  -561,   269,
      53,    55,  -561,  -561,    -5,  -561,  -561,  -561,  -561,  -561,
    -561,   219,  -561,  -561,  -561,  -561,  -561,  -561,    69,  -561,
     113,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,    58,
    -561,  -561,  -561,    58,  -561,  -561,    72,    82,   728,    42,
      98,   575,    98,    98,    98,    98,  -561,  -561,  -561,    54,
      98,    56,  -561,    81,    98,  -561,    58,    98,    91,    94,
      98,  -561,    22,  -561,    24,   180,   182,  -561,   104,   114,
     426,    35,  -561,  -561,   132,   175,  -561,   155,   162,   181,
      49,  -561,   112,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,   196,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,   113,  -561,  -561,  -561,    16,  -561,
     186,  -561,   189,   195,   201,   202,  -561,   207,   210,   213,
     218,   220,   222,   225,   223,  -561,  -561,  -561,   227,   228,
    -561,  -561,  -561,   196,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,   196,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,   229,  -561,   230,  -561,  -561,   231,  -561,    98,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,   728,  -561,  -561,
    -561,   232,  -561,  -561,  -561,  -561,   312,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,    50,    50,
      50,    35,   196,  -561,  -561,   224,   234,   235,   -40,    73,
      89,  -561,  -561,  -561,   242,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,   311,  -561,   244,   276,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,   236,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,   276,   248,   251,  -561,  -561,
     126,    98,    98,   253,   254,  -561,  -561,  -561,   252,  -561,
      98,   255,   231,  -561,  -561,   340,  -561,   257,   260,  -561,
    -561,  -561,   241,    35,    35,    35,    35,    35,    35,    35,
      35,    35,    35,  -561,   263,    42,  -561,  -561,  -561,  -561,
      42,  -561,    35,    35,    35,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,    97,   290,    42,    42,  -561,  -561,
    -561,  -561,  -561,    97,  -561,  -561,  -561,  -561,   256,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,    42,   276,  -561,
    -561,  -561,  -561,  -561,  -561,  1215,   266,  -561,  -561,  -561,
     234,   235,   -40,    73,    73,    89,    89,  -561,  -561,  -561,
      98,    35,  -561,   196,   941,   767,   611,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,   245,    80,   150,   883,  -561,  -561,
     325,  1114,  -561,   196,    98,  -561,  -561,    62,    65,   273,
     274,   277,    98,   278,   281,   126,   272,   261,   270,   366,
    -561,   976,   196,  -561,    42,   276,   108,   284,   288,  -561,
    -561,   293,   426,  -561,  -561,   289,  -561,  -561,  -561,   295,
    -561,  -561,  -561,  -561,  -561,  -561,  1067,  -561,  -561,  -561,
    -561,  -561,   296,  -561,  -561,  -561,   303,  -561,   305,   306,
     307,   310,   313,  -561,  -561,  -561,   390,   196,  -561,  -561,
    -561,  -561,  -561,  -561,    42,    42,    42,  -561,   316,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,    98,    98,  -561,   317,
    -561,  -561,  -561,  1034,   825,  -561,   196,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,   318,
     319,   196,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,   314,  -561,   322,   323,   326,   327,   328,  -561,  -561,
    -561,   330,   331,   407,   245,   245,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,   332,    98,   315,    75,    75,    75,   335,
    -561,   336,   341,   342,   343,   344,   346,  -561,  -561,  -561,
     348,   350,   353,   355,  1114,    98,    35,    35,   261,    98,
     356,   357,   362,  -561,   371,   359,   367,   364,   108,  1162,
      42,  -561,  -561,  -561,  -561,  -561,  -561,   452,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,   354,   354,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,   372,   196,  -561,   354,   377,   383,   428,  -561,  -561,
      98,   417,   382,   373,   366,    42,    42,   389,   418,  -561,
     375,  -561,  -561,  -561,   384,   445,   447,   447,  -561,  -561,
    -561,  -561,  -561,  -561,   196,   196,  -561,  -561,   388,  -561,
     447,    45,  -561,  -561,   391,  -561,  -561,  -561,  -561,   447,
     393,   424,    98,   437,   409,   395,   170,   447,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  1114,  -561,   408,   398,
    -561,  -561,  -561,  -561,  -561,    42,  -561,   498,  -561,  -561,
    -561,  -561,   399,  -561,  -561,  -561,   419,    35,   413,   420,
      63,   575,   164,  -561,  -561,  -561,    42,   404,   196,  -561,
     421,  -561,  -561,   422,  1114,   469,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,    98,   445,    42,   406,  -561,  -561,
     423,  -561,  -561,  -561,  -561,   425,    98,   427,  -561,  -561,
     416,  -561,    42,   508,   498,   170,    98,   429,  -561,  -561,
    -561,  -561,   430,  -561,  -561,  -561,  -561,  -561,  -561,   451,
    -561,   433,   436,  -561,  -561,   457,  -561
};

/* TAO_YYPGOTO[NTERM-NUM].  */
static const tao_yytype_int16 tao_yypgoto[] =
{
    -561,  -561,  -561,  -561,  -561,  -561,    18,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,   535,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -130,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,   194,
    -561,  -561,   109,  -561,  -561,  -561,   523,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,   525,  -561,   205,  -561,  -561,  -251,
    -561,  -561,   133,  -561,  -561,  -561,  -561,  -310,  -355,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -312,  -561,  -561,
     -20,  -561,  -561,   -13,  -561,     6,  -561,  -561,  -561,  -561,
     324,   -41,   -94,  -561,   233,   238,   243,   -67,   -18,  -108,
      44,  -561,  -306,    11,  -561,  -561,  -561,   -37,   147,  -381,
    -561,  -561,  -561,   -17,  -561,  -561,  -560,  -116,  -561,  -561,
      -3,  -561,   -56,  -561,  -561,   -43,   -42,   -55,   -53,   -52,
    -561,  -561,   -34,  -561,   -32,  -561,  -561,  -561,  -561,   118,
     214,  -561,  -303,  -561,  -561,  -561,   -30,  -561,   -26,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -190,  -561,
    -561,  -561,  -561,  -561,  -186,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,   -38,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -107,  -561,  -561,  -561,  -561,  -561,  -561,   -66,  -561,  -561,
    -561,   -54,  -561,  -561,  -561,  -561,  -561,  -561,  -561,   -29,
    -561,  -561,  -253,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,    12,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -548,
    -561,  -561,  -561,  -561,  -561,  -205,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -224,  -561,  -561,  -425,  -561,  -405,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,    14,    17,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,   264,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -374,   209,  -373,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
     490,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,
    -561,   123,  -561,  -561,  -217,  -561,  -561,  -561,  -561,  -561,
    -561,  -561,   -28,  -561,  -561,   163,  -561,  -561,   177,  -561,
    -561,  -561,  -561,  -561,  -561,  -561,  -561,  -561,   146,  -561,
    -561,  -561,  -561
};

/* TAO_YYTABLE[TAO_YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what TAO_YYDEFACT says.
   If TAO_YYTABLE_NINF, syntax error.  */
#define TAO_YYTABLE_NINF -508
static const tao_yytype_int16 tao_yytable[] =
{
      87,   102,   104,   156,   137,    98,    99,   152,    45,   153,
      88,   154,   191,    46,    55,   155,    56,   244,   397,    57,
      26,   136,   185,   188,   192,   189,   190,   400,   401,   638,
     507,   550,   490,   491,   339,   186,   187,   348,    86,     7,
     465,   316,   317,   156,   196,    86,   709,   152,     3,   153,
      17,   154,   498,    86,    94,   155,   431,   262,   183,   193,
     263,   136,   533,   534,   709,   512,     7,   194,   513,   197,
     198,   199,   200,   710,   711,   507,   107,   202,    86,   659,
    -140,   205,    17,    86,   206,   115,    90,   208,   476,   121,
     122,   710,   711,   125,   126,   127,   128,   210,   503,   212,
      86,    86,   211,    11,   211,   268,   671,   269,   229,   230,
     231,   232,   233,   234,   235,   101,   412,   433,   236,   237,
     264,   265,   101,   229,   230,   231,   232,   233,   234,   235,
     101,   238,   239,   236,   237,   166,   240,   241,   112,  -141,
     113,  -142,   211,   116,   115,   211,   115,   312,   115,   465,
    -283,   191,   241,   487,  -301,   211,   342,   343,   180,   632,
     101,   185,   188,   192,   189,   190,  -100,   182,   344,   318,
     319,   115,   348,   345,   186,   187,  -422,   101,   175,  -494,
     342,   343,   175,   213,   532,   214,   344,   320,   321,   322,
     215,   345,   710,   711,   346,   347,   299,   183,   201,   216,
     203,   484,   485,   507,   778,   729,   730,   731,    89,    91,
     344,   486,   387,   388,   389,   345,   789,   256,   346,   347,
      93,    96,    86,   257,   533,   534,   796,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,     9,
      10,   259,    11,   132,   133,   134,   473,   260,    86,   383,
     384,   611,   612,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,     9,    10,   261,    11,   132,
     133,   134,   698,   -46,   270,   -46,   211,   271,   357,   358,
     272,   636,   309,   310,   311,   708,   273,   371,   274,   -46,
     -46,   743,   276,   -46,   720,   277,   -46,   244,   278,   101,
     385,   386,   735,   279,  -101,   393,   280,   281,   284,   115,
     393,   282,   288,   289,  -495,   306,   295,   313,   -46,   328,
     305,   296,   -46,   330,   413,   101,   417,   418,   314,   772,
     315,   323,   329,   413,   340,   507,   -46,   341,   336,   364,
     365,   372,   369,   376,   379,   508,   377,   432,   415,   478,
     479,   378,   480,   481,   391,   440,   505,   509,   514,   515,
    -330,   525,   516,   518,   521,   522,   156,   483,   519,   526,
     152,   537,   153,   507,   154,    -8,   136,   443,   155,   538,
     540,    45,   542,   556,   136,   477,    46,    55,   557,    56,
     508,   506,    57,   438,   558,   559,   560,   561,   262,   601,
    -258,   511,   509,   570,   579,  -353,   593,   602,   603,   517,
     610,   604,   605,   606,   531,   608,   609,   613,   615,     4,
     619,   620,     5,     6,     7,     8,   621,   622,   623,   624,
     217,   625,   218,   628,   767,   629,   506,   642,     9,    10,
     630,    11,   631,   639,   643,    12,   219,   220,   640,   641,
     221,   645,   644,   222,   780,   653,   657,   669,    13,    14,
      15,    16,   672,  -371,   566,   567,   568,    17,    18,   673,
     792,    19,   679,   681,    20,   223,   686,   688,   682,   224,
     693,    21,    22,   577,   578,   696,   722,   692,    23,    24,
     647,  -380,   244,   225,   717,   721,   725,   727,   728,   744,
     745,   749,   752,   758,   756,   -52,   768,   759,   773,   781,
     769,   793,   790,   771,   784,   788,   156,   156,   508,   791,
     152,   152,   153,   153,   154,   154,   800,   804,   155,   155,
     509,   797,   806,   798,   136,   136,   803,    27,   687,    95,
      92,   304,   504,   467,   678,   635,   380,   614,   529,   403,
     760,   358,   381,   616,   617,   618,   763,   683,   382,   637,
     779,   795,   422,   177,   506,   539,   373,   794,   444,   495,
     646,   520,    87,     0,     0,     0,   358,     0,    86,     0,
     652,     0,   634,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,     9,    10,     0,    11,   132,
     133,   134,     0,     0,     0,    45,     0,     0,     0,     0,
      46,    55,     0,    56,    86,     0,    57,     0,     0,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,     0,     0,     0,     0,   132,   133,   134,     0,     0,
       0,     0,     0,   757,     0,   684,   685,    87,     0,     0,
     508,     0,     0,     0,     0,   101,     0,   634,     0,     0,
       0,     0,   509,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    45,     0,     0,     0,     0,    46,
      55,     0,    56,     0,     0,    57,     0,     0,   508,    87,
       0,   101,     0,     0,     0,     0,   506,     0,     0,   724,
     509,     0,     0,   156,   761,   748,     0,   152,     0,   153,
       0,   154,     0,     0,     0,   155,     0,     0,     0,     0,
       0,   136,     0,     0,     0,     0,   393,     0,     0,     0,
       0,    86,     0,     0,   506,     0,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   393,     0,     0,     0,
       0,   358,     0,   133,   134,     0,     0,     0,     0,     0,
       0,     0,   393,   358,     0,     0,     0,     0,   446,     0,
    -362,     5,     0,   358,     8,  -362,  -362,  -362,  -362,  -362,
    -362,  -362,  -362,  -362,  -362,  -362,  -362,     9,    10,     0,
      11,     0,  -362,  -362,    12,     0,     0,   342,   343,   447,
     448,  -362,     0,     0,     0,     0,     0,    13,   101,     0,
       0,   459,   460,   461,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   446,     0,  -362,     5,
      21,    22,     8,  -362,  -362,  -362,  -362,  -362,  -362,  -362,
    -362,  -362,  -362,  -362,  -362,     9,    10,  -362,    11,     0,
    -362,  -362,    12,     0,   -86,   342,   343,   447,   448,  -362,
       0,     0,     0,     0,     0,    13,     0,     0,     0,   459,
     460,   461,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   446,     0,  -362,     5,    21,    22,
       8,  -362,  -362,  -362,  -362,  -362,  -362,  -362,  -362,  -362,
    -362,  -362,  -362,     9,    10,  -362,    11,     0,  -362,  -362,
      12,     0,  -507,   342,   343,   447,   448,  -362,     0,     0,
       0,     0,     0,    13,     0,     0,     0,   496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   497,     0,     0,
       0,     0,   446,     0,  -362,     5,    21,    22,     8,  -362,
    -362,  -362,  -362,  -362,  -362,  -362,  -362,  -362,  -362,  -362,
    -362,     9,    10,  -362,    11,     0,  -362,  -362,    12,     0,
    -474,   342,   343,   447,   448,  -362,     0,   446,     0,  -362,
       5,    13,     0,     8,  -362,  -362,  -362,  -362,  -362,  -362,
    -362,  -362,  -362,  -362,  -362,  -362,     9,    10,     0,    11,
       0,  -362,  -362,    12,    21,    22,   342,   343,   447,   448,
    -362,     0,     0,     0,     0,     0,    13,     0,     0,     0,
       0,  -362,     0,     0,     0,     0,     0,     0,   -68,     0,
       0,     0,     0,     0,     0,   446,     0,  -362,     5,    21,
      22,     8,  -362,  -362,  -362,  -362,  -362,  -362,  -362,  -362,
    -362,  -362,  -362,  -362,     9,    10,  -362,    11,     0,  -362,
    -362,    12,     0,   -90,   342,   343,   447,   448,  -362,     0,
      86,     0,     0,     0,    13,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,     0,     0,     0,
       0,     0,   133,   134,     0,     0,     0,    21,    22,     0,
       0,   548,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -362,     0,     0,    86,     0,     0,
       0,  -498,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,     0,     0,     0,    -2,     4,   133,
     134,     5,     6,     7,     8,     0,     0,   101,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     9,    10,     0,
      11,     0,     0,     4,    12,     0,     5,     6,     7,     8,
       0,     0,     0,     0,     0,     0,     0,    13,    14,    15,
      16,     0,     9,    10,     0,    11,    17,    18,     0,    12,
      19,     0,     0,    20,   101,     0,     0,     0,     0,     0,
      21,    22,    13,    14,    15,    16,     0,    23,    24,     0,
       0,    17,    18,     0,     0,    19,     4,     0,    20,     5,
       6,     7,     8,     0,     0,    21,    22,     0,     0,     0,
       0,     0,    23,    24,   647,     9,    10,     0,    11,     0,
       0,     0,    12,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    13,    14,    15,    16,     0,
       0,     0,     0,     0,    17,    18,     0,     0,    19,     0,
       0,    20,     0,     0,     0,     0,     0,     0,    21,    22,
       0,     0,     0,     0,     0,    23,    24
};

static const tao_yytype_int16 tao_yycheck[] =
{
      13,    21,    22,    41,    41,    18,    19,    41,     2,    41,
      13,    41,    78,     2,     2,    41,     2,   111,   330,     2,
       2,    41,    78,    78,    78,    78,    78,   333,   334,   589,
     411,   456,   406,   406,   285,    78,    78,   290,     3,     6,
     395,    81,    82,    81,    81,     3,     1,    81,     0,    81,
      49,    81,   407,     3,    53,    81,   366,     8,    78,    79,
      11,    81,   436,   436,     1,     3,     6,    80,     3,    82,
      83,    84,    85,    28,    29,   456,    88,    90,     3,   627,
      85,    94,    49,     3,    97,    90,    53,   100,     8,     9,
      10,    28,    29,    13,    14,    15,    16,    75,   408,    75,
       3,     3,    80,    23,    80,    89,   654,    91,    73,    74,
      75,    76,    77,    78,    79,    80,    19,   368,    83,    84,
       8,     9,    80,    73,    74,    75,    76,    77,    78,    79,
      80,    96,    97,    83,    84,    22,   101,   102,    85,    85,
      85,    85,    80,    34,    90,    80,    90,   241,    90,   504,
      87,   217,   102,   406,    85,    80,    30,    31,    86,   584,
      80,   217,   217,   217,   217,   217,    85,    85,    60,    96,
      97,    90,   425,    65,   217,   217,    85,    80,    69,    85,
      30,    31,    73,     3,   435,     3,    60,    98,    99,   100,
      86,    65,    28,    29,    68,    69,   209,   217,    89,    85,
      91,    51,    52,   584,   764,    35,    36,    37,    14,    15,
      60,    61,   320,   321,   322,    65,   776,    85,    68,    69,
      15,    16,     3,    48,   598,   598,   786,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    86,    23,    24,    25,    26,     1,    85,     3,   316,
     317,   554,   555,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    86,    23,    24,
      25,    26,   677,     4,    88,     6,    80,    88,   291,   292,
      85,   587,   238,   239,   240,   690,    85,   300,    86,    20,
      21,   716,    85,    24,   699,    85,    27,   391,    85,    80,
     318,   319,   707,    85,    85,   325,    86,    85,    85,    90,
     330,    86,    85,    85,    85,     3,    86,    93,    49,     8,
      88,    90,    53,    47,   344,    80,   346,   347,    94,   754,
      95,    89,    88,   353,    86,   716,    67,    86,   102,    86,
      86,    86,    90,     3,   103,   411,    89,   367,    58,   405,
     405,    91,   405,   405,    91,    89,    31,   411,    85,    85,
     104,    91,    85,    85,    92,   104,   404,   405,    87,     3,
     404,    87,   404,   754,   404,    87,   396,   390,   404,    86,
      91,   375,    87,    87,   404,   405,   375,   375,    85,   375,
     456,   411,   375,   375,    89,    89,    89,    87,     8,    85,
      87,   414,   456,    87,    87,    87,    87,    85,    85,   422,
       3,    85,    85,    85,   434,    85,    85,    85,   103,     1,
      85,    85,     4,     5,     6,     7,    85,    85,    85,    85,
       4,    85,     6,    85,   746,    85,   456,    66,    20,    21,
      87,    23,    87,    87,    85,    27,    20,    21,    91,    87,
      24,    87,    85,    27,   766,     3,   102,    85,    40,    41,
      42,    43,    85,    35,   484,   485,   486,    49,    50,    86,
     782,    53,    55,    91,    56,    49,    87,   102,   105,    53,
      35,    63,    64,   496,   497,    38,    62,   103,    70,    71,
      72,   103,   586,    67,   103,   102,    59,    88,   103,    91,
     102,     3,   103,    90,    85,    87,   102,    87,    39,   103,
      89,     3,    85,    91,    91,    90,   554,   555,   584,   103,
     554,   555,   554,   555,   554,   555,    75,    91,   554,   555,
     584,   102,    75,   103,   554,   555,   103,     2,   668,    16,
      15,   217,   409,   396,   660,   586,   313,   564,   430,   335,
     740,   564,   314,   566,   567,   568,   742,   664,   315,   588,
     765,   785,   353,    73,   584,   442,   302,   784,   391,   406,
     598,   425,   585,    -1,    -1,    -1,   589,    -1,     3,    -1,
     600,    -1,   585,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    23,    24,
      25,    26,    -1,    -1,    -1,   599,    -1,    -1,    -1,    -1,
     599,   599,    -1,   599,     3,    -1,   599,    -1,    -1,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    24,    25,    26,    -1,    -1,
      -1,    -1,    -1,   737,    -1,   665,   666,   660,    -1,    -1,
     716,    -1,    -1,    -1,    -1,    80,    -1,   660,    -1,    -1,
      -1,    -1,   716,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   668,    -1,    -1,    -1,    -1,   668,
     668,    -1,   668,    -1,    -1,   668,    -1,    -1,   754,   702,
      -1,    80,    -1,    -1,    -1,    -1,   716,    -1,    -1,   702,
     754,    -1,    -1,   741,   741,   725,    -1,   741,    -1,   741,
      -1,   741,    -1,    -1,    -1,   741,    -1,    -1,    -1,    -1,
      -1,   741,    -1,    -1,    -1,    -1,   746,    -1,    -1,    -1,
      -1,     3,    -1,    -1,   754,    -1,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,   766,    -1,    -1,    -1,
      -1,   764,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   782,   776,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,    -1,   786,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      23,    -1,    25,    26,    27,    -1,    -1,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    80,    -1,
      -1,    44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,
      63,    64,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    80,    23,    -1,
      25,    26,    27,    -1,    87,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    44,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,     3,     4,    63,    64,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    80,    23,    -1,    25,    26,
      27,    -1,    87,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,
      -1,    -1,     1,    -1,     3,     4,    63,    64,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    80,    23,    -1,    25,    26,    27,    -1,
      87,    30,    31,    32,    33,    34,    -1,     1,    -1,     3,
       4,    40,    -1,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      -1,    25,    26,    27,    63,    64,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,    63,
      64,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    80,    23,    -1,    25,
      26,    27,    -1,    87,    30,    31,    32,    33,    34,    -1,
       3,    -1,    -1,    -1,    40,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    -1,    25,    26,    -1,    -1,    -1,    63,    64,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    80,    -1,    -1,     3,    -1,    -1,
      -1,    87,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,     0,     1,    25,
      26,     4,     5,     6,     7,    -1,    -1,    80,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      23,    -1,    -1,     1,    27,    -1,     4,     5,     6,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,
      43,    -1,    20,    21,    -1,    23,    49,    50,    -1,    27,
      53,    -1,    -1,    56,    80,    -1,    -1,    -1,    -1,    -1,
      63,    64,    40,    41,    42,    43,    -1,    70,    71,    -1,
      -1,    49,    50,    -1,    -1,    53,     1,    -1,    56,     4,
       5,     6,     7,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    70,    71,    72,    20,    21,    -1,    23,    -1,
      -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    40,    41,    42,    43,    -1,
      -1,    -1,    -1,    -1,    49,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    70,    71
};

/* TAO_YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const tao_yytype_uint16 tao_yystos[] =
{
       0,   107,   108,     0,     1,     4,     5,     6,     7,    20,
      21,    23,    27,    40,    41,    42,    43,    49,    50,    53,
      56,    63,    64,    70,    71,   109,   112,   127,   129,   133,
     134,   147,   150,   151,   155,   157,   160,   161,   162,   166,
     170,   172,   176,   177,   200,   201,   219,   228,   248,   250,
     262,   264,   286,   287,   288,   329,   380,   381,   382,   383,
     384,   388,   410,   412,   435,   436,   437,   438,   439,   443,
     444,   445,   448,   452,   461,   475,   476,   126,   202,   128,
     156,   220,   249,   263,   289,   330,     3,   199,   236,   155,
      53,   155,   170,   172,    53,   162,   172,   173,   199,   199,
     413,    80,   196,   199,   196,   462,   477,    88,   130,   119,
     135,   148,    85,    85,   118,    90,   158,   152,   120,   163,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    24,    25,    26,   158,   196,   223,   224,   225,
     226,   227,   238,   239,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   250,   262,   264,   288,   297,   300,   303,
     306,   307,   310,   116,   113,   251,    22,   117,   114,   115,
     121,   385,   122,   411,   123,   158,   446,   446,   124,   125,
      86,   479,    85,   196,   206,   238,   241,   242,   243,   244,
     245,   303,   307,   196,   199,   221,   223,   199,   199,   199,
     199,   158,   199,   158,   167,   199,   199,   389,   199,   197,
      75,    80,    75,     3,     3,    86,    85,     4,     6,    20,
      21,    24,    27,    49,    53,    67,   453,   454,   457,    73,
      74,    75,    76,    77,    78,    79,    83,    84,    96,    97,
     101,   102,   196,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   472,   474,    85,    48,   174,    86,
      85,    86,     8,    11,     8,     9,   301,   171,    89,    91,
      88,    88,    85,    85,    86,   265,    85,    85,    85,    85,
      86,    85,    86,   422,    85,   447,   440,   449,    85,    85,
     480,   203,   222,   290,   331,    86,    90,   391,   414,   199,
     198,   463,   478,   131,   206,    88,     3,   136,   455,   216,
     216,   216,   208,    93,    94,    95,    81,    82,    96,    97,
      98,    99,   100,    89,   473,   159,   153,   164,     8,    88,
      47,   175,   298,   304,   308,   252,   102,   386,   423,   175,
      86,    86,    30,    31,    60,    65,    68,    69,   318,   319,
     324,   402,   404,   405,   471,   482,   484,   199,   199,   229,
     232,   236,   237,   311,    86,    86,   168,   392,   390,    90,
     419,   199,    86,   391,   110,   111,     3,    89,    91,   103,
     210,   211,   212,   213,   213,   214,   214,   215,   215,   215,
     149,    91,   193,   196,   183,   178,   302,   193,   208,   218,
     218,   218,   254,   256,   257,   266,   393,   425,   441,   450,
     320,   325,    19,   196,   403,    58,   406,   196,   196,   487,
     485,   486,   403,   488,   481,   483,   204,   312,   230,   291,
     332,   183,   196,   175,   420,   415,   464,   132,   112,   127,
      89,   137,   456,   199,   474,   194,     1,    32,    33,   154,
     184,   201,   219,   318,   329,   334,   339,   380,   381,    44,
      45,    46,   165,   179,   180,   184,   341,   224,   299,   305,
     309,   253,   255,     1,   223,   258,     8,   196,   238,   243,
     244,   245,   271,   288,    51,    52,    61,   318,   387,   394,
     402,   404,   407,   408,   409,   471,    44,    54,   184,   424,
     426,   429,   432,   183,   178,    31,   196,   225,   303,   307,
     363,   199,     3,     3,    85,    85,    85,   199,    85,    87,
     484,    92,   104,   313,   315,    91,     3,   293,   296,   255,
     169,   196,   175,   402,   404,   466,   468,    87,    86,   457,
      91,   192,    87,   188,   185,   190,   189,   191,    34,   340,
     363,   186,   187,   342,   182,   181,    87,    85,    89,    89,
      89,    87,   256,   261,   259,   267,   196,   196,   196,   400,
      87,   395,   396,   397,   398,   399,   401,   199,   199,    87,
     427,   428,   442,   451,   321,   326,   205,   316,   314,   231,
     292,   294,   333,    87,   416,   469,   470,   465,   467,   138,
     195,    85,    85,    85,    85,    85,    85,   335,    85,    85,
       3,   258,   258,    85,   229,   103,   199,   199,   199,    85,
      85,    85,    85,    85,    85,    85,   430,   433,    85,    85,
      87,    87,   363,   233,   236,   207,   218,   315,   232,    87,
      91,    87,    66,    85,    85,    87,   468,    72,   112,   140,
     142,   143,   196,     3,   343,   260,   268,   102,   345,   345,
     322,   327,   234,   317,   295,   417,   144,   139,   141,    85,
     336,   345,    85,    86,   346,   347,   431,   434,   233,    55,
     368,    91,   105,   296,   196,   196,    87,   142,   102,   354,
     344,   269,   103,    35,   348,   351,    38,   365,   365,   323,
     369,   328,   235,   418,   145,   355,   356,   337,   365,     1,
      28,    29,   272,   274,   278,   280,   352,   103,   349,   366,
     365,   102,    62,   371,   236,    59,   421,    88,   103,    35,
      36,    37,   357,   360,   364,   365,   277,   282,   281,   270,
     273,   275,   279,   363,    91,   102,   370,   372,   196,     3,
     458,   460,   103,   358,   361,   338,    85,   208,    90,    87,
     274,   223,   284,   280,   353,   350,   367,   193,   102,    89,
     459,    91,   363,    39,   374,   283,   285,   276,   232,   351,
     193,   103,   373,   146,    91,   359,   362,   375,    90,   232,
      85,   103,   193,     3,   460,   360,   232,   102,   103,   376,
      75,   377,   378,   103,    91,   379,    75
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
          // Check that scoped name contains no delimitor.
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
          // Check that scoped name contains no delimitor.
        }
    break;

  case 47:

    {
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
        }
    break;

  case 57:

    {
        }
    break;

  case 58:

    {
        }
    break;

  case 59:

    {
        }
    break;

  case 60:

    {
        }
    break;

  case 61:

    {
        }
    break;

  case 62:

    {
        }
    break;

  case 63:

    {
        }
    break;

  case 66:

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

  case 67:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
    break;

  case 68:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
    break;

  case 69:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 70:

    {
// interface_decl : IDL_INTERFACE
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
    break;

  case 71:

    {
//       id
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
         }
    break;

  case 72:

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

  case 73:

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

  case 74:

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

  case 75:

    {
// inheritance_spec : ':' opt_truncatable
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
    break;

  case 76:

    {
//      at_least_one_scoped_name
          (tao_yyvsp[(4) - (4)].nlval)->truncatable ((tao_yyvsp[(2) - (4)].bval));
          (tao_yyval.nlval) = (tao_yyvsp[(4) - (4)].nlval);
        }
    break;

  case 77:

    {
/*      |  EMPTY */
          (tao_yyval.nlval) = 0;
        }
    break;

  case 82:

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

  case 84:

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

  case 85:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
    break;

  case 86:

    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
    break;

  case 87:

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

  case 88:

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

  case 89:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
    break;

  case 90:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
    break;

  case 91:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 92:

    {
// value_header : value_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 93:

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

  case 94:

    {
// value_decl : IDL_VALUETYPE
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
    break;

  case 95:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 96:

    {
// opt_truncatable : IDL_TRUNCATABLE
          (tao_yyval.bval) = true;
        }
    break;

  case 97:

    {
/*      |  EMPTY */
          (tao_yyval.bval) = false;
        }
    break;

  case 98:

    {
// supports_spec : IDL_SUPPORTS at_least_one_scoped_name
          (tao_yyval.nlval) = (tao_yyvsp[(2) - (2)].nlval);
        }
    break;

  case 99:

    {
/*      |    EMPTY */
          (tao_yyval.nlval) = 0;
        }
    break;

  case 100:

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

  case 101:

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

  case 102:

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

  case 108:

    {
// state_member : IDL_PUBLIC
          /* is $0 to member_i */
          (tao_yyval.vival) = AST_Field::vis_PUBLIC;
        }
    break;

  case 110:

    {
//      IDL_PRIVATE
          /* is $0 to member_i */
          (tao_yyval.vival) = AST_Field::vis_PRIVATE;
        }
    break;

  case 114:

    {
// export : type_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
    break;

  case 115:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 116:

    {
//      | typeid_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
    break;

  case 117:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 118:

    {
//      | typeprefix_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
    break;

  case 119:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 120:

    {
//      | const_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
    break;

  case 121:

    {
//      ';'
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
    break;

  case 122:

    {
//      | exception
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
    break;

  case 123:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 124:

    {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
    break;

  case 125:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 126:

    {
//      | operation
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
    break;

  case 127:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 128:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
    break;

  case 129:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 130:

    {
// at_least_one_scoped_name : scoped_name scoped_names
          ACE_NEW_RETURN ((tao_yyval.nlval),
                          UTL_NameList ((tao_yyvsp[(1) - (2)].idlist),
                                        (tao_yyvsp[(2) - (2)].nlval)),
                          1);
        }
    break;

  case 131:

    {
// scoped_names : scoped_names ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
    break;

  case 132:

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

  case 133:

    {
/*      |  EMPTY */
          (tao_yyval.nlval) = 0;
        }
    break;

  case 134:

    {
// scoped_name : id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN ((tao_yyval.idlist),
                          UTL_IdList ((tao_yyvsp[(1) - (1)].idval),
                                      0),
                          1);
        }
    break;

  case 135:

    {
//      | IDL_SCOPE_DELIMITOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
    break;

  case 136:

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

  case 137:

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

  case 138:

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

  case 139:

    {
// id: IDENTIFIER
          ACE_NEW_RETURN ((tao_yyval.idval),
                          Identifier ((tao_yyvsp[(1) - (1)].strval)),
                          1);
          ACE::strdelete ((tao_yyvsp[(1) - (1)].strval));
          (tao_yyvsp[(1) - (1)].strval) = 0;
        }
    break;

  case 140:

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

  case 141:

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

  case 142:

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

  case 143:

    {
// const_dcl : IDL_CONST
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
    break;

  case 144:

    {
//      const_type
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
    break;

  case 145:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
    break;

  case 146:

    {
//      '='
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
    break;

  case 147:

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

  case 154:

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

  case 155:

    {
//      | wstring_type_spec
          (tao_yyval.etval) = AST_Expression::EV_wstring;
        }
    break;

  case 156:

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

  case 160:

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

  case 162:

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

  case 164:

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

  case 166:

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

  case 167:

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

  case 169:

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

  case 170:

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

  case 172:

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

  case 173:

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

  case 174:

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

  case 176:

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

  case 177:

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

  case 178:

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

  case 179:

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

  case 181:

    {
//      | literal
//      | '(' const_expr ')'
          (tao_yyval.exval) = (tao_yyvsp[(2) - (3)].exval);
        }
    break;

  case 182:

    {
// literal : IDL_INTEGER_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].ival));
        }
    break;

  case 183:

    {
//      | IDL_UINTEGER_LITERAL
          (tao_yyval.exval) =
            idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].uival),
                                             AST_Expression::EV_ulonglong);
        }
    break;

  case 184:

    {
//      | IDL_STRING_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].sval));
          (tao_yyvsp[(1) - (1)].sval)->destroy ();
          delete (tao_yyvsp[(1) - (1)].sval);
          (tao_yyvsp[(1) - (1)].sval) = 0;
        }
    break;

  case 185:

    {
//      | IDL_WSTRING_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].wsval));
        }
    break;

  case 186:

    {
//      | IDL_CHARACTER_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].cval));
        }
    break;

  case 187:

    {
//      | IDL_WCHAR_LITERAL
          ACE_OutputCDR::from_wchar wc ((tao_yyvsp[(1) - (1)].wcval));
          (tao_yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
    break;

  case 188:

    {
//      | IDL_FLOATING_PT_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].dval));
        }
    break;

  case 189:

    {
//      | IDL_TRUETOK
          (tao_yyval.exval) = idl_global->gen ()->create_expr (true);
        }
    break;

  case 190:

    {
//      | IDL_FALSETOK
          (tao_yyval.exval) = idl_global->gen ()->create_expr (false);
        }
    break;

  case 191:

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

  case 192:

    {
// type_dcl : IDL_TYPEDEF
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
        }
    break;

  case 193:

    {
//      type_declarator
          (tao_yyval.dcval) = 0;
        }
    break;

  case 194:

    {
//      | struct_type
          (tao_yyval.dcval) = 0;
        }
    break;

  case 195:

    {
//      | union_type
          (tao_yyval.dcval) = 0;
        }
    break;

  case 196:

    {
//      | enum_type
          (tao_yyval.dcval) = 0;
        }
    break;

  case 197:

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

  case 198:

    {
//      | constructed_forward_type_spec
          (tao_yyval.dcval) = 0;
        }
    break;

  case 199:

    {
// type_declarator : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
    break;

  case 200:

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

  case 203:

    {
// simple_type_spec : base_type_spec
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 205:

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

  case 222:

    {
// at_least_one_declarator : declarator declarators
          ACE_NEW_RETURN ((tao_yyval.dlval),
                          UTL_DeclList ((tao_yyvsp[(1) - (2)].deval),
                                        (tao_yyvsp[(2) - (2)].dlval)),
                          1);
        }
    break;

  case 223:

    {
// declarators : declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
    break;

  case 224:

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

  case 225:

    {
/*      |  EMPTY */
          (tao_yyval.dlval) = 0;
        }
    break;

  case 228:

    {
// at_least_one_simple_declarator : simple_declarator simple_declarators
          ACE_NEW_RETURN ((tao_yyval.dlval),
                          UTL_DeclList ((tao_yyvsp[(1) - (2)].deval),
                                        (tao_yyvsp[(2) - (2)].dlval)),
                          1);
        }
    break;

  case 229:

    {
// simple_declarators : simple_declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
    break;

  case 230:

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

  case 231:

    {
/*      |  EMPTY */
          (tao_yyval.dlval) = 0;
        }
    break;

  case 232:

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

  case 233:

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

  case 236:

    {
// signed_int : IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_long;
        }
    break;

  case 237:

    {
//      | IDL_LONG IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_longlong;
        }
    break;

  case 238:

    {
//      | IDL_SHORT
          (tao_yyval.etval) = AST_Expression::EV_short;
        }
    break;

  case 239:

    {
// unsigned_int : IDL_UNSIGNED IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_ulong;
        }
    break;

  case 240:

    {
//      | IDL_UNSIGNED IDL_LONG IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_ulonglong;
        }
    break;

  case 241:

    {
//      | IDL_UNSIGNED IDL_SHORT
          (tao_yyval.etval) = AST_Expression::EV_ushort;
        }
    break;

  case 242:

    {
// floating_pt_type : IDL_DOUBLE
          (tao_yyval.etval) = AST_Expression::EV_double;
        }
    break;

  case 243:

    {
//      | IDL_FLOAT
          (tao_yyval.etval) = AST_Expression::EV_float;
        }
    break;

  case 244:

    {
//      | IDL_LONG IDL_DOUBLE
          (tao_yyval.etval) = AST_Expression::EV_longdouble;
        }
    break;

  case 245:

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

  case 246:

    {
// char_type : IDL_CHAR
          (tao_yyval.etval) = AST_Expression::EV_char;
        }
    break;

  case 247:

    {
//      | IDL_WCHAR
          (tao_yyval.etval) = AST_Expression::EV_wchar;
        }
    break;

  case 248:

    {
// octet_type : IDL_OCTET
          (tao_yyval.etval) = AST_Expression::EV_octet;
        }
    break;

  case 249:

    {
// boolean_type : IDL_BOOLEAN
          (tao_yyval.etval) = AST_Expression::EV_bool;
        }
    break;

  case 250:

    {
// any_type : IDL_ANY
          (tao_yyval.etval) = AST_Expression::EV_any;
        }
    break;

  case 251:

    {
// object_type : IDL_OBJECT
          (tao_yyval.etval) = AST_Expression::EV_object;
        }
    break;

  case 252:

    {
// struct_decl : IDL_STRUCT
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
    break;

  case 253:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 254:

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

  case 255:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
    break;

  case 256:

    {
//      at_least_one_member
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
    break;

  case 257:

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

  case 261:

    {
// member  :
          /* is $0 to member_i */
          (tao_yyval.vival) = AST_Field::vis_NA;
        }
    break;

  case 263:

    {
// member_i : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
    break;

  case 264:

    {
//      at_least_one_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
    break;

  case 265:

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

  case 266:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state ());
        }
    break;

  case 267:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 268:

    {
// union_decl : IDL_UNION
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
    break;

  case 269:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 270:

    {
// union_type : union_decl IDL_SWITCH
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
    break;

  case 271:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchOpenParSeen);
        }
    break;

  case 272:

    {
//      switch_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
    break;

  case 273:

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

  case 274:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
    break;

  case 275:

    {
//      at_least_one_case_branch
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
    break;

  case 276:

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

  case 277:

    {
// switch_type_spec : integer_type
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 278:

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

  case 279:

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

  case 280:

    {
//      | boolean_type
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 282:

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

  case 286:

    {
// case_branch : at_least_one_case_label
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
    break;

  case 287:

    {
//      element_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
    break;

  case 288:

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

  case 289:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
    break;

  case 290:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 291:

    {
// at_least_one_case_label : case_label case_labels
          ACE_NEW_RETURN ((tao_yyval.llval),
                          UTL_LabelList ((tao_yyvsp[(1) - (2)].ulval),
                                         (tao_yyvsp[(2) - (2)].llval)),
                          1);
        }
    break;

  case 292:

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

  case 293:

    {
/*      |  EMPTY */
          (tao_yyval.llval) = 0;
        }
    break;

  case 294:

    {
// case_label : IDL_DEFAULT
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
    break;

  case 295:

    {
//      ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (tao_yyval.ulval) = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
    break;

  case 296:

    {
//      | IDL_CASE
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
    break;

  case 297:

    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
    break;

  case 298:

    {
//      const_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (tao_yyval.ulval) = idl_global->gen()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      (tao_yyvsp[(3) - (5)].exval)
                                    );
        }
    break;

  case 299:

    {
// element_spec : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
    break;

  case 300:

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

  case 301:

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

  case 302:

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

  case 303:

    {
// enum_type : IDL_ENUM
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
    break;

  case 304:

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

  case 305:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
    break;

  case 306:

    {
//      at_least_one_enumerator
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
    break;

  case 307:

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

  case 309:

    {
// enumerators : enumerators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
    break;

  case 312:

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

  case 313:

    {
// sequence_type_spec : seq_head ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
    break;

  case 314:

    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
    break;

  case 315:

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

  case 316:

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

  case 317:

    {
// seq_head : IDL_SEQUENCE
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
    break;

  case 318:

    {
//      '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
    break;

  case 319:

    {
//      simple_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          (tao_yyval.dcval) = (tao_yyvsp[(5) - (5)].dcval);
        }
    break;

  case 320:

    {
// string_type_spec : string_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
    break;

  case 321:

    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
    break;

  case 322:

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

  case 323:

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

  case 324:

    {
// string_head : IDL_STRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
    break;

  case 325:

    {
// wstring_type_spec : wstring_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
    break;

  case 326:

    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
    break;

  case 327:

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

  case 328:

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

  case 329:

    {
// wstring_head : IDL_WSTRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
    break;

  case 330:

    {
// array_declarator : id
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
    break;

  case 331:

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

  case 332:

    {
// at_least_one_array_dim : array_dim array_dims
          ACE_NEW_RETURN ((tao_yyval.elval),
                          UTL_ExprList ((tao_yyvsp[(1) - (2)].exval),
                                        (tao_yyvsp[(2) - (2)].elval)),
                          1);
        }
    break;

  case 333:

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

  case 334:

    {
/*      |  EMPTY */
          (tao_yyval.elval) = 0;
        }
    break;

  case 335:

    {
// array_dim : '['
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
    break;

  case 336:

    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
    break;

  case 337:

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

  case 340:

    {
// attribute_readonly : IDL_READONLY
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
    break;

  case 341:

    {
//      IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
    break;

  case 342:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
    break;

  case 343:

    {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
    break;

  case 344:

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

  case 345:

    {
// attribute_readonly : IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
    break;

  case 346:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
    break;

  case 347:

    {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
    break;

  case 348:

    {
//      opt_getraises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
    break;

  case 349:

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

  case 350:

    {
// exception : IDL_EXCEPTION
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
    break;

  case 351:

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

  case 352:

    {
//       '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
    break;

  case 353:

    {
//      members
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
    break;

  case 354:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 355:

    {
// operation : opt_op_attribute op_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
    break;

  case 356:

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

  case 357:

    {
//      parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 358:

    {
//      opt_raises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
    break;

  case 359:

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

  case 360:

    {
// opt_op_attribute : IDL_ONEWAY
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (tao_yyval.ofval) = AST_Operation::OP_oneway;
        }
    break;

  case 361:

    {
//      | IDL_IDEMPOTENT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (tao_yyval.ofval) = AST_Operation::OP_idempotent;
        }
    break;

  case 362:

    {
/*      |  EMPTY */
          (tao_yyval.ofval) = AST_Operation::OP_noflags;
        }
    break;

  case 364:

    {
// op_type_spec : param_type_spec | IDL_VOID
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
    break;

  case 365:

    {
// init_decl : IDL_FACTORY
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
    break;

  case 366:

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

  case 367:

    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 368:

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

  case 369:

    {
// init_parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 370:

    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 371:

    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 372:

    {
//      at_least_one_in_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 374:

    {
// in_parameters : in_parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
    break;

  case 377:

    {
// in_parameter : IDL_IN
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
    break;

  case 378:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
    break;

  case 379:

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

  case 380:

    {
// parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 381:

    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 382:

    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 383:

    {
//      at_least_one_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 385:

    {
// parameters : parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
    break;

  case 388:

    {
// parameter : direction
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
    break;

  case 389:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
    break;

  case 390:

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

  case 391:

    {
// param_type_spec : base_type_spec
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 394:

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

  case 395:

    {
// direction : IDL_IN
          (tao_yyval.dival) = AST_Argument::dir_IN;
        }
    break;

  case 396:

    {
//      | IDL_OUT
          (tao_yyval.dival) = AST_Argument::dir_OUT;
        }
    break;

  case 397:

    {
//      | IDL_INOUT
          (tao_yyval.dival) = AST_Argument::dir_INOUT;
        }
    break;

  case 398:

    {
// opt_raises : IDL_RAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
    break;

  case 399:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
    break;

  case 400:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          (tao_yyval.nlval) = (tao_yyvsp[(5) - (6)].nlval);
        }
    break;

  case 401:

    {
          (tao_yyval.nlval) = 0;
/*      |  EMPTY */
        }
    break;

  case 402:

    {
// opt_getraises : IDL_GETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
    break;

  case 403:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
    break;

  case 404:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          (tao_yyval.nlval) = (tao_yyvsp[(5) - (6)].nlval);
        }
    break;

  case 405:

    {
          (tao_yyval.nlval) = 0;
/*      |  EMPTY */
        }
    break;

  case 406:

    {
// opt_setraises : IDL_SETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
    break;

  case 407:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
    break;

  case 408:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (tao_yyval.nlval) = (tao_yyvsp[(5) - (6)].nlval);
        }
    break;

  case 409:

    {
          (tao_yyval.nlval) = 0;
/*      |  EMPTY */
        }
    break;

  case 410:

    {
// opt_context : IDL_CONTEXT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
    break;

  case 411:

    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
//      '('
        }
    break;

  case 412:

    {
//      at_least_one_string_literal ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (tao_yyval.slval) = (tao_yyvsp[(5) - (6)].slval);
        }
    break;

  case 413:

    {
/*      |  EMPTY */
          (tao_yyval.slval) = 0;
        }
    break;

  case 414:

    {
// at_least_one_string_literal : IDL_STRING_LITERAL string_literals
          ACE_NEW_RETURN ((tao_yyval.slval),
                          UTL_StrList ((tao_yyvsp[(1) - (2)].sval),
                                       (tao_yyvsp[(2) - (2)].slval)),
                          1);
        }
    break;

  case 415:

    {
// string_literals : string_literals ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
    break;

  case 416:

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

  case 417:

    {
/*      |  EMPTY */
          (tao_yyval.slval) = 0;
        }
    break;

  case 418:

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

  case 419:

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

  case 422:

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

  case 423:

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

  case 424:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
    break;

  case 425:

    {
//      component_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
    break;

  case 426:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 427:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
    break;

  case 428:

    {
//      component_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 429:

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

  case 430:

    {
// component_inheritance_spec : ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
    break;

  case 431:

    {
//      scoped_name
          (tao_yyval.idlist) = (tao_yyvsp[(3) - (3)].idlist);
        }
    break;

  case 432:

    {
/*      |  EMPTY */
          (tao_yyval.idlist) = 0;
        }
    break;

  case 435:

    {
// component_export : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
    break;

  case 436:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 437:

    {
//      | uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
    break;

  case 438:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 439:

    {
//      | emits_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
        }
    break;

  case 440:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 441:

    {
//      | publishes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
        }
    break;

  case 442:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 443:

    {
//      | consumes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
        }
    break;

  case 444:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 445:

    {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
    break;

  case 446:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 447:

    {
//      | extended_port_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
    break;

  case 448:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 449:

    {
// provides_decl : IDL_PROVIDES interface_type id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;

          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist),
                                           true);
          if (d == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));
              so_far_so_good = false;
            }
          else if (d->node_type () != AST_Decl::NT_interface)
            {
              // Nothing else but CORBA::Object can have
              // this identifier.
              int comp_result =
                ACE_OS::strcmp (d->local_name ()->get_string (),
                                "Object");

              // Simple provides port must use IDL interface
              // or CORBA::Object.
              if (comp_result != 0)
                {
                  idl_global->err ()->interface_expected (d);
                  so_far_so_good = false;
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

  case 450:

    {
// interface_type : scoped_name
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (tao_yyval.idlist) = (tao_yyvsp[(1) - (1)].idlist);
        }
    break;

  case 451:

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

  case 452:

    {
// uses_decl : uses_opt_multiple interface_type id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;

          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist),
                                           true);
          if (d == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));
              so_far_so_good = false;
            }
          else if (d->node_type () != AST_Decl::NT_interface)
            {
              // Nothing else but CORBA::Object can have
              // this identifier.
              int comp_result =
                ACE_OS::strcmp (d->local_name ()->get_string (),
                                "Object");

              // Simple provides port must use IDL interface
              // or CORBA::Object.
              if (comp_result != 0)
                {
                  idl_global->err ()->interface_expected (d);
                  so_far_so_good = false;
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
                  && !idl_global->ignore_idl3 ())
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

  case 453:

    {
// uses_opt_multiple : IDL_USES opt_multiple
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          (tao_yyval.bval) = (tao_yyvsp[(2) - (2)].bval);
        }
    break;

  case 454:

    {
// opt_multiple : IDL_MULTIPLE
          (tao_yyval.bval) = true;
        }
    break;

  case 455:

    {
/*      |  EMPTY */
          (tao_yyval.bval) = false;
        }
    break;

  case 456:

    {
// emits_decl : IDL_EMITS scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist),
                                           true);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));
              so_far_so_good = false;
            }
          else if (d->node_type () != AST_Decl::NT_eventtype)
            {
              idl_global->err ()->eventtype_expected (d);
              so_far_so_good = false;
            }

          if (so_far_so_good)
            {
              AST_EventType *event_type =
                AST_EventType::narrow_from_decl (d);

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

  case 457:

    {
// publishes_decl : IDL_PUBLISHES scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist),
                                           true);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));
              so_far_so_good = false;
            }
          else if (d->node_type () != AST_Decl::NT_eventtype)
            {
              idl_global->err ()->eventtype_expected (d);
              so_far_so_good = false;
            }

          if (so_far_so_good)
            {
              AST_EventType *event_type =
                AST_EventType::narrow_from_decl (d);

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

  case 458:

    {
// consumes_decl : IDL_CONSUMES scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].idlist),
                                           true);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].idlist));
              so_far_so_good = false;
            }
          else if (d->node_type () != AST_Decl::NT_eventtype)
            {
              idl_global->err ()->eventtype_expected (d);
              so_far_so_good = false;
            }

          if (so_far_so_good)
            {
              AST_EventType *event_type =
                AST_EventType::narrow_from_decl (d);

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

  case 459:

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

  case 460:

    {
//      home_body
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 461:

    {
// home_header : IDL_HOME
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
    break;

  case 462:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
    break;

  case 463:

    {
//      home_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 464:

    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
    break;

  case 465:

    {
//      IDL_MANAGES
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
    break;

  case 466:

    {
//      scoped_name
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
    break;

  case 467:

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

  case 468:

    {
// home_inheritance_spec ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
    break;

  case 469:

    {
//      scoped_name
          (tao_yyval.idlist) = (tao_yyvsp[(3) - (3)].idlist);
        }
    break;

  case 470:

    {
/*      |  EMPTY */
          (tao_yyval.idlist) = 0;
        }
    break;

  case 471:

    {
// primary_key_spec : IDL_PRIMARYKEY scoped_name
          (tao_yyval.idlist) = (tao_yyvsp[(2) - (2)].idlist);
        }
    break;

  case 472:

    {
/*      |  EMPTY */
          (tao_yyval.idlist) = 0;
        }
    break;

  case 473:

    {
// home_body : '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
    break;

  case 474:

    {
//      home_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
    break;

  case 475:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
    break;

  case 479:

    {
// home_export : factory_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
    break;

  case 480:

    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 481:

    {
//      | finder_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
    break;

  case 482:

    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 483:

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

  case 484:

    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 485:

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

  case 486:

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

  case 487:

    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 488:

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

  case 494:

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

  case 495:

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

  case 496:

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

  case 497:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
    break;

  case 498:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
    break;

  case 499:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 500:

    {
// event_abs_header : IDL_ABSTRACT IDL_EVENTTYPE id
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 501:

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

  case 502:

    {
// event_plain_header : IDL_EVENTTYPE id
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (tao_yyval.idval) = (tao_yyvsp[(2) - (2)].idval);
        }
    break;

  case 503:

    {
// event_rest_of_header : inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 504:

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

  case 505:

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

  case 506:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
    break;

  case 507:

    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
    break;

  case 508:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 509:

    {
// event_header : event_custom_header
          (tao_yyval.idval) = (tao_yyvsp[(1) - (1)].idval);
        }
    break;

  case 510:

    {
// event_header : event_plain_header
          (tao_yyval.idval) = (tao_yyvsp[(1) - (1)].idval);
        }
    break;

  case 511:

    {
// formal_parameter_type : IDL_TYPENAME
          (tao_yyval.ntval) = AST_Decl::NT_type;
        }
    break;

  case 512:

    {
//        IDL_STRUCT
          (tao_yyval.ntval) = AST_Decl::NT_struct;
        }
    break;

  case 513:

    {
//        IDL_UNION
          (tao_yyval.ntval) = AST_Decl::NT_union;
        }
    break;

  case 514:

    {
//        IDL_EVENTTYPE
          (tao_yyval.ntval) = AST_Decl::NT_eventtype;
        }
    break;

  case 515:

    {
//        IDL_SEQUENCE
          (tao_yyval.ntval) = AST_Decl::NT_sequence;
        }
    break;

  case 516:

    {
//        IDL_INTERFACE
          (tao_yyval.ntval) = AST_Decl::NT_interface;
        }
    break;

  case 517:

    {
//        IDL_VALUETYPE
          (tao_yyval.ntval) = AST_Decl::NT_valuetype;
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
        }
    break;

  case 520:

    {
//        formal_parameter formal_parameters
          if ((tao_yyvsp[(2) - (2)].plval) == 0)
            {
              ACE_NEW_RETURN ((tao_yyvsp[(2) - (2)].plval),
                              FE_Utils::T_PARAMLIST_INFO,
                              1);
            }

          (tao_yyvsp[(2) - (2)].plval)->enqueue_head (*(tao_yyvsp[(1) - (2)].pival));
          delete (tao_yyvsp[(1) - (2)].pival);
          (tao_yyvsp[(1) - (2)].pival) = 0;
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

          bool so_far_so_good =
            idl_global->check_for_seq_of_param ((tao_yyvsp[(1) - (4)].plval),
                                                (tao_yyvsp[(4) - (4)].pival));

          if (so_far_so_good)
            {
              (tao_yyvsp[(1) - (4)].plval)->enqueue_tail (*(tao_yyvsp[(4) - (4)].pival));
              (tao_yyval.plval) = (tao_yyvsp[(1) - (4)].plval);
            }
          else
            {
              delete (tao_yyvsp[(1) - (4)].plval);
              (tao_yyvsp[(1) - (4)].plval) = 0;

              idl_global->err ()->mismatch_seq_of_param (
                (tao_yyvsp[(4) - (4)].pival)->name_.c_str ());
            }

          delete (tao_yyvsp[(4) - (4)].pival);
          (tao_yyvsp[(4) - (4)].pival) = 0;

          if (!so_far_so_good)
            {
              return 1;
            }

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

          (tao_yyval.pival)->type_ = (tao_yyvsp[(1) - (2)].ntval);
          (tao_yyval.pival)->name_ = (tao_yyvsp[(2) - (2)].strval);
        }
    break;

  case 525:

    {
          ACE_NEW_RETURN ((tao_yyval.pival),
                          FE_Utils::T_Param_Info,
                          1);

          (tao_yyval.pival)->type_ = AST_Decl::NT_sequence;
          (tao_yyval.pival)->name_  = "sequence<";
          (tao_yyval.pival)->name_ += (tao_yyvsp[(3) - (4)].strval);
          (tao_yyval.pival)->name_ += '>';
        }
    break;

  case 526:

    {
          (tao_yyval.slval) = 0;
        }
    break;

  case 527:

    {
// formal_parameter_names : formal_parameter_names ',' formal_parameter_name
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

  case 547:

    {
// actual_parameter : expression
          // To avoid grammar conflicts with this LALR(1) parser,
          // we take advantage of the fact that an expression can
          // be a scoped name. At that lower level, we create an
          // expression containing the scoped name, and at a
          // higher lever, deduce that it's not supposede to be
          // a constant and look up the type to add to the template
          // arg list.
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

