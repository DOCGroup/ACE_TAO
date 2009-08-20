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
#include "fe_instantiated_interface_header.h"
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
#define TAO_YYLAST   1236

/* TAO_YYNTOKENS -- Number of terminals.  */
#define TAO_YYNTOKENS  107
/* TAO_YYNNTS -- Number of nonterminals.  */
#define TAO_YYNNTS  380
/* TAO_YYNRULES -- Number of rules.  */
#define TAO_YYNRULES  564
/* TAO_YYNRULES -- Number of states.  */
#define TAO_YYNSTATES  814

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
      70,    74,    75,    79,    80,    84,    85,    86,    87,    88,
      98,   100,   102,   103,   104,   105,   113,   114,   118,   121,
     125,   129,   130,   135,   136,   138,   140,   142,   144,   147,
     149,   150,   151,   152,   160,   161,   162,   163,   172,   173,
     178,   179,   183,   185,   186,   189,   190,   193,   195,   198,
     201,   202,   204,   206,   209,   212,   215,   218,   219,   220,
     224,   225,   229,   230,   234,   235,   239,   240,   244,   245,
     249,   250,   254,   255,   259,   262,   263,   268,   269,   271,
     272,   276,   277,   282,   284,   286,   289,   292,   293,   294,
     295,   296,   306,   308,   310,   312,   314,   316,   318,   320,
     322,   324,   326,   328,   330,   334,   336,   340,   342,   346,
     348,   352,   356,   358,   362,   366,   368,   372,   376,   380,
     382,   385,   388,   391,   393,   395,   399,   401,   403,   405,
     407,   409,   411,   413,   415,   417,   419,   420,   424,   426,
     428,   430,   433,   435,   436,   440,   442,   444,   446,   448,
     450,   452,   454,   456,   458,   460,   462,   464,   466,   468,
     470,   472,   474,   476,   478,   480,   482,   485,   486,   491,
     492,   494,   496,   499,   500,   505,   506,   508,   510,   512,
     514,   516,   519,   521,   524,   528,   531,   533,   535,   538,
     540,   542,   544,   546,   548,   550,   552,   553,   557,   558,
     559,   560,   568,   571,   574,   575,   576,   579,   580,   581,
     587,   588,   592,   593,   597,   598,   599,   600,   601,   602,
     603,   618,   620,   622,   624,   626,   628,   630,   633,   636,
     637,   638,   639,   645,   646,   650,   653,   656,   657,   658,
     662,   663,   664,   670,   671,   675,   677,   679,   680,   681,
     682,   683,   693,   696,   697,   702,   703,   705,   706,   707,
     714,   717,   718,   719,   725,   726,   727,   734,   736,   738,
     739,   740,   747,   749,   751,   752,   756,   759,   762,   763,
     764,   765,   771,   773,   775,   776,   777,   778,   779,   789,
     790,   791,   792,   793,   803,   804,   805,   806,   807,   817,
     818,   819,   820,   821,   832,   834,   836,   837,   839,   841,
     842,   843,   844,   852,   853,   857,   858,   863,   866,   867,
     872,   873,   874,   875,   881,   882,   886,   887,   892,   895,
     896,   901,   902,   903,   904,   910,   912,   914,   916,   918,
     920,   922,   924,   925,   926,   933,   934,   935,   936,   943,
     944,   945,   946,   953,   954,   955,   956,   963,   964,   967,
     968,   973,   974,   978,   982,   984,   986,   989,   990,   991,
     992,  1000,  1001,  1002,  1009,  1010,  1014,  1015,  1018,  1019,
    1020,  1024,  1025,  1029,  1030,  1034,  1035,  1039,  1040,  1044,
    1045,  1049,  1050,  1054,  1058,  1060,  1062,  1066,  1069,  1071,
    1072,  1076,  1080,  1084,  1085,  1089,  1090,  1091,  1092,  1093,
    1094,  1095,  1109,  1110,  1114,  1115,  1118,  1119,  1120,  1121,
    1127,  1130,  1131,  1133,  1134,  1138,  1139,  1143,  1144,  1145,
    1152,  1153,  1154,  1161,  1163,  1165,  1167,  1169,  1171,  1174,
    1178,  1179,  1180,  1181,  1190,  1194,  1198,  1201,  1202,  1206,
    1207,  1208,  1209,  1218,  1220,  1222,  1224,  1226,  1228,  1230,
    1232,  1234,  1236,  1238,  1239,  1240,  1241,  1249,  1253,  1254,
    1255,  1262,  1263,  1268,  1269,  1272,  1275,  1276,  1279,  1283,
    1284,  1289,  1292,  1296,  1297,  1299,  1300,  1301,  1302,  1303,
    1304,  1316,  1318,  1319,  1322,  1325,  1326,  1327,  1331,  1332,
    1336,  1338,  1342,  1344,  1348,  1350,  1352,  1356,  1360,  1364,
    1368,  1373,  1376,  1377,  1378,  1379,  1387,  1388,  1389,  1395,
    1398,  1401,  1402,  1403,  1407,  1408,  1412,  1413,  1417,  1418,
    1422,  1423,  1427,  1430,  1433
};

/* TAO_YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const tao_yytype_int16 tao_yyrhs[] =
{
     108,     0,    -1,   109,    -1,   109,   110,    -1,    -1,    -1,
     198,   111,    86,    -1,    -1,   359,   112,    86,    -1,    -1,
     360,   113,    86,    -1,    -1,   180,   114,    86,    -1,    -1,
     308,   115,    86,    -1,    -1,   131,   116,    86,    -1,    -1,
     433,   117,    86,    -1,    -1,   126,   118,    86,    -1,    -1,
     141,   119,    86,    -1,    -1,   361,   120,    86,    -1,    -1,
     389,   121,    86,    -1,    -1,   414,   122,    86,    -1,    -1,
     451,   123,    86,    -1,    -1,   469,   124,    86,    -1,    -1,
       1,   125,    86,    -1,    -1,    -1,    -1,    -1,     5,   127,
       3,   128,    87,   129,   109,   130,    88,    -1,   132,    -1,
     179,    -1,    -1,    -1,    -1,   138,   133,    87,   134,   162,
     135,    88,    -1,    -1,     6,   137,   178,    -1,   136,   139,
      -1,    41,   136,   139,    -1,    42,   136,   139,    -1,    -1,
      89,   155,   140,   172,    -1,    -1,   142,    -1,   147,    -1,
     157,    -1,   158,    -1,    43,   143,    -1,   143,    -1,    -1,
      -1,    -1,   151,   144,    87,   145,   159,   146,    88,    -1,
      -1,    -1,    -1,    42,   151,   148,    87,   149,   162,   150,
      88,    -1,    -1,   153,   139,   152,   156,    -1,    -1,    49,
     154,   178,    -1,    48,    -1,    -1,    47,   172,    -1,    -1,
      42,   153,    -1,   153,    -1,   153,   202,    -1,   159,   160,
      -1,    -1,   161,    -1,   163,    -1,   320,    86,    -1,    46,
     237,    -1,    45,   237,    -1,   162,   163,    -1,    -1,    -1,
     198,   164,    86,    -1,    -1,   359,   165,    86,    -1,    -1,
     360,   166,    86,    -1,    -1,   180,   167,    86,    -1,    -1,
     308,   168,    86,    -1,    -1,   297,   169,    86,    -1,    -1,
     313,   170,    86,    -1,    -1,     1,   171,    86,    -1,   175,
     173,    -1,    -1,   173,    90,   174,   175,    -1,    -1,   178,
      -1,    -1,    81,   176,   178,    -1,    -1,   175,    81,   177,
     178,    -1,     3,    -1,   136,    -1,    41,   136,    -1,    42,
     136,    -1,    -1,    -1,    -1,    -1,     4,   181,   185,   182,
     178,   183,    91,   184,   186,    -1,   217,    -1,   222,    -1,
     223,    -1,   224,    -1,   220,    -1,   221,    -1,   282,    -1,
     286,    -1,   175,    -1,   187,    -1,   188,    -1,   189,    -1,
     188,    92,   189,    -1,   190,    -1,   189,    93,   190,    -1,
     191,    -1,   190,    94,   191,    -1,   192,    -1,   191,    82,
     192,    -1,   191,    83,   192,    -1,   193,    -1,   192,    95,
     193,    -1,   192,    96,   193,    -1,   194,    -1,   193,    97,
     194,    -1,   193,    98,   194,    -1,   193,    99,   194,    -1,
     195,    -1,    95,   195,    -1,    96,   195,    -1,   100,   195,
      -1,   175,    -1,   196,    -1,   101,   187,   102,    -1,    74,
      -1,    75,    -1,    76,    -1,    85,    -1,    77,    -1,    84,
      -1,    78,    -1,    79,    -1,    80,    -1,   187,    -1,    -1,
       7,   199,   200,    -1,   229,    -1,   243,    -1,   267,    -1,
      40,   215,    -1,   207,    -1,    -1,   202,   201,   208,    -1,
     203,    -1,   206,    -1,   204,    -1,   205,    -1,   175,    -1,
     217,    -1,   220,    -1,   222,    -1,   224,    -1,   223,    -1,
     221,    -1,   225,    -1,   226,    -1,   276,    -1,   282,    -1,
     286,    -1,   229,    -1,   243,    -1,   267,    -1,   265,    -1,
     266,    -1,   211,   209,    -1,    -1,   209,    90,   210,   211,
      -1,    -1,   215,    -1,   216,    -1,   215,   213,    -1,    -1,
     213,    90,   214,   215,    -1,    -1,   178,    -1,   290,    -1,
     218,    -1,   219,    -1,     8,    -1,     8,     8,    -1,     9,
      -1,    10,     8,    -1,    10,     8,     8,    -1,    10,     9,
      -1,    11,    -1,    12,    -1,     8,    11,    -1,    17,    -1,
      13,    -1,    14,    -1,    15,    -1,    16,    -1,    18,    -1,
      19,    -1,    -1,    20,   228,   178,    -1,    -1,    -1,    -1,
     227,   230,    87,   231,   233,   232,    88,    -1,   235,   234,
      -1,   234,   235,    -1,    -1,    -1,   236,   237,    -1,    -1,
      -1,   202,   238,   208,   239,    86,    -1,    -1,     1,   240,
      86,    -1,    -1,    21,   242,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   241,    22,   244,   101,   245,   250,   246,
     102,   247,    87,   248,   251,   249,    88,    -1,   217,    -1,
     222,    -1,   223,    -1,   224,    -1,   267,    -1,   175,    -1,
     253,   252,    -1,   252,   253,    -1,    -1,    -1,    -1,   257,
     254,   263,   255,    86,    -1,    -1,     1,   256,    86,    -1,
     259,   258,    -1,   258,   259,    -1,    -1,    -1,    29,   260,
      89,    -1,    -1,    -1,    28,   261,   187,   262,    89,    -1,
      -1,   202,   264,   211,    -1,   227,    -1,   241,    -1,    -1,
      -1,    -1,    -1,    23,   268,   178,   269,    87,   270,   272,
     271,    88,    -1,   275,   273,    -1,    -1,   273,    90,   274,
     275,    -1,    -1,     3,    -1,    -1,    -1,   279,    90,   277,
     197,   278,   103,    -1,   279,   103,    -1,    -1,    -1,    24,
     280,   104,   281,   203,    -1,    -1,    -1,   285,   104,   283,
     197,   284,   103,    -1,   285,    -1,    25,    -1,    -1,    -1,
     289,   104,   287,   197,   288,   103,    -1,   289,    -1,    26,
      -1,    -1,   178,   291,   292,    -1,   294,   293,    -1,   293,
     294,    -1,    -1,    -1,    -1,   105,   295,   197,   296,   106,
      -1,   298,    -1,   303,    -1,    -1,    -1,    -1,    -1,    30,
     299,    31,   300,   342,   301,   212,   302,   344,    -1,    -1,
      -1,    -1,    -1,    31,   304,   342,   305,   212,   306,   347,
     307,   350,    -1,    -1,    -1,    -1,    -1,    27,   309,   178,
     310,    87,   311,   234,   312,    88,    -1,    -1,    -1,    -1,
      -1,   318,   319,   314,     3,   315,   333,   316,   344,   317,
     353,    -1,    32,    -1,    33,    -1,    -1,   342,    -1,    34,
      -1,    -1,    -1,    -1,    44,   321,     3,   322,   324,   323,
     344,    -1,    -1,   101,   325,   102,    -1,    -1,   101,   326,
     327,   102,    -1,   330,   328,    -1,    -1,   328,    90,   329,
     330,    -1,    -1,    -1,    -1,    35,   331,   342,   332,   211,
      -1,    -1,   101,   334,   102,    -1,    -1,   101,   335,   336,
     102,    -1,   339,   337,    -1,    -1,   337,    90,   338,   339,
      -1,    -1,    -1,    -1,   343,   340,   342,   341,   211,    -1,
     204,    -1,   282,    -1,   286,    -1,   175,    -1,    35,    -1,
      36,    -1,    37,    -1,    -1,    -1,    38,   345,   101,   346,
     172,   102,    -1,    -1,    -1,    -1,    55,   348,   101,   349,
     172,   102,    -1,    -1,    -1,    -1,    62,   351,   101,   352,
     172,   102,    -1,    -1,    -1,    -1,    39,   354,   101,   355,
     356,   102,    -1,    -1,    76,   357,    -1,    -1,   357,    90,
     358,    76,    -1,    -1,    63,   175,    76,    -1,    64,   175,
      76,    -1,   363,    -1,   362,    -1,    50,   178,    -1,    -1,
      -1,    -1,   367,   364,    87,   365,   372,   366,    88,    -1,
      -1,    -1,    50,   178,   368,   370,   369,   156,    -1,    -1,
      89,   371,   175,    -1,    -1,   372,   373,    -1,    -1,    -1,
     381,   374,    86,    -1,    -1,   383,   375,    86,    -1,    -1,
     386,   376,    86,    -1,    -1,   387,   377,    86,    -1,    -1,
     388,   378,    86,    -1,    -1,   297,   379,    86,    -1,    -1,
     465,   380,    86,    -1,    60,   382,   178,    -1,   175,    -1,
      19,    -1,   384,   382,   178,    -1,    65,   385,    -1,    58,
      -1,    -1,    52,   175,   178,    -1,    61,   175,   178,    -1,
      51,   175,   178,    -1,    -1,   391,   390,   401,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,   392,   178,   393,   398,
     394,   156,   395,    66,   396,   175,   397,   400,    -1,    -1,
      89,   399,   175,    -1,    -1,    59,   175,    -1,    -1,    -1,
      -1,    87,   402,   404,   403,    88,    -1,   404,   405,    -1,
      -1,   163,    -1,    -1,   408,   406,    86,    -1,    -1,   411,
     407,    86,    -1,    -1,    -1,    44,   178,   409,   324,   410,
     344,    -1,    -1,    -1,    54,   178,   412,   324,   413,   344,
      -1,   427,    -1,   418,    -1,   415,    -1,   417,    -1,   416,
      -1,    53,   178,    -1,    42,    53,   178,    -1,    -1,    -1,
      -1,   422,   425,   419,    87,   420,   162,   421,    88,    -1,
      42,    53,   178,    -1,    43,    53,   178,    -1,    53,   178,
      -1,    -1,   139,   426,   156,    -1,    -1,    -1,    -1,   431,
     425,   428,    87,   429,   159,   430,    88,    -1,   423,    -1,
     424,    -1,    67,    -1,    20,    -1,    53,    -1,    68,    -1,
      17,    -1,    24,    -1,     6,    -1,    49,    -1,    -1,    -1,
      -1,   437,   434,    87,   435,   162,   436,    88,    -1,   136,
     438,   444,    -1,    -1,    -1,   104,   439,   443,   441,   440,
     103,    -1,    -1,   441,    90,   442,   443,    -1,    -1,   432,
       3,    -1,    89,   445,    -1,    -1,   447,   446,    -1,   446,
      90,   447,    -1,    -1,   175,   104,   448,   103,    -1,   450,
     449,    -1,   449,    90,   450,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    71,   452,     3,   453,   457,   454,
      87,   455,   458,   456,    88,    -1,   438,    -1,    -1,   460,
     459,    -1,   459,   460,    -1,    -1,    -1,   463,   461,    86,
      -1,    -1,   464,   462,    86,    -1,   381,    -1,    60,   447,
       3,    -1,   383,    -1,   384,   447,     3,    -1,   466,    -1,
     467,    -1,    69,   468,     3,    -1,    70,   468,     3,    -1,
      69,   175,     3,    -1,    70,   175,     3,    -1,   175,   104,
     172,   103,    -1,   470,   474,    -1,    -1,    -1,    -1,    72,
     471,     3,   472,   457,   473,   370,    -1,    -1,    -1,    87,
     475,   477,   476,    88,    -1,   479,   478,    -1,   478,   479,
      -1,    -1,    -1,   381,   480,    86,    -1,    -1,   383,   481,
      86,    -1,    -1,   297,   482,    86,    -1,    -1,   467,   483,
      86,    -1,    -1,   485,   484,    86,    -1,    69,   486,    -1,
      70,   486,    -1,   447,     3,    -1
};

/* TAO_YYRLINE[TAO_YYN] -- source line where rule number TAO_YYN was defined.  */
static const tao_yytype_uint16 tao_yyrline[] =
{
       0,   368,   368,   371,   372,   377,   376,   387,   386,   397,
     396,   407,   406,   417,   416,   427,   426,   437,   436,   447,
     446,   457,   456,   467,   466,   477,   476,   487,   486,   497,
     496,   507,   506,   517,   516,   530,   535,   561,   566,   529,
     582,   583,   588,   627,   632,   587,   650,   649,   663,   701,
     732,   766,   765,   777,   784,   785,   786,   787,   791,   802,
     807,   851,   856,   806,   885,   924,   929,   883,   948,   946,
     986,   985,   999,  1005,  1012,  1019,  1026,  1052,  1079,  1144,
    1145,  1149,  1150,  1151,  1156,  1162,  1171,  1172,  1177,  1176,
    1187,  1186,  1197,  1196,  1207,  1206,  1217,  1216,  1227,  1226,
    1237,  1236,  1247,  1246,  1260,  1273,  1271,  1301,  1308,  1319,
    1318,  1346,  1344,  1371,  1383,  1429,  1457,  1488,  1493,  1498,
    1503,  1487,  1556,  1557,  1558,  1559,  1560,  1561,  1562,  1574,
    1579,  1654,  1656,  1658,  1659,  1673,  1674,  1688,  1689,  1702,
    1703,  1713,  1726,  1727,  1737,  1750,  1751,  1761,  1771,  1784,
    1785,  1795,  1805,  1818,  1853,  1854,  1863,  1868,  1875,  1883,
    1888,  1893,  1899,  1904,  1909,  1917,  1989,  1988,  1998,  2003,
    2008,  2013,  2040,  2049,  2048,  2119,  2120,  2124,  2132,  2133,
    2161,  2162,  2163,  2164,  2165,  2166,  2167,  2168,  2172,  2173,
    2174,  2178,  2179,  2180,  2184,  2185,  2189,  2202,  2200,  2230,
    2237,  2238,  2242,  2255,  2253,  2283,  2290,  2307,  2326,  2327,
    2331,  2336,  2341,  2349,  2354,  2359,  2367,  2372,  2377,  2385,
    2399,  2404,  2412,  2420,  2428,  2436,  2445,  2444,  2460,  2494,
    2499,  2459,  2518,  2521,  2522,  2526,  2526,  2536,  2541,  2535,
    2604,  2603,  2618,  2617,  2632,  2637,  2642,  2647,  2694,  2699,
    2631,  2723,  2731,  2745,  2755,  2763,  2764,  2872,  2875,  2876,
    2881,  2886,  2880,  2922,  2921,  2935,  2946,  2968,  2976,  2975,
    2991,  2996,  2990,  3013,  3012,  3065,  3089,  3114,  3119,  3152,
    3157,  3113,  3183,  3188,  3186,  3193,  3197,  3234,  3239,  3232,
    3306,  3361,  3371,  3360,  3386,  3391,  3384,  3432,  3458,  3468,
    3473,  3466,  3503,  3528,  3537,  3536,  3572,  3583,  3605,  3613,
    3618,  3612,  3655,  3656,  3661,  3666,  3671,  3676,  3660,  3745,
    3750,  3755,  3760,  3744,  3838,  3843,  3873,  3878,  3837,  3896,
    3901,  3954,  3959,  3894,  3996,  4002,  4009,  4016,  4017,  4029,
    4035,  4077,  4028,  4099,  4098,  4109,  4108,  4121,  4126,  4124,
    4131,  4136,  4141,  4135,  4182,  4181,  4192,  4191,  4204,  4209,
    4207,  4214,  4219,  4224,  4218,  4271,  4279,  4280,  4281,  4369,
    4374,  4379,  4388,  4393,  4387,  4405,  4413,  4418,  4412,  4430,
    4438,  4443,  4437,  4455,  4463,  4468,  4462,  4480,  4487,  4500,
    4498,  4526,  4533,  4562,  4600,  4601,  4605,  4635,  4675,  4680,
    4634,  4699,  4704,  4697,  4746,  4745,  4756,  4763,  4764,  4769,
    4768,  4779,  4778,  4789,  4788,  4799,  4798,  4809,  4808,  4819,
    4818,  4829,  4828,  4840,  4899,  4906,  4930,  5004,  5014,  5020,
    5026,  5073,  5120,  5169,  5168,  5218,  5223,  5228,  5233,  5238,
    5243,  5217,  5297,  5296,  5307,  5314,  5321,  5329,  5334,  5328,
    5346,  5347,  5351,  5353,  5352,  5363,  5362,  5377,  5413,  5375,
    5447,  5483,  5445,  5515,  5516,  5517,  5521,  5522,  5526,  5554,
    5585,  5630,  5635,  5583,  5652,  5662,  5681,  5693,  5692,  5732,
    5782,  5787,  5730,  5804,  5809,  5817,  5822,  5827,  5832,  5837,
    5850,  5855,  5860,  5869,  5891,  5896,  5868,  5913,  5931,  5936,
    5930,  5959,  5958,  5980,  5987,  6001,  6007,  6014,  6033,  6050,
    6057,  6067,  6078,  6102,  6109,  6120,  6125,  6130,  6151,  6156,
    6119,  6171,  6177,  6184,  6191,  6196,  6203,  6202,  6211,  6210,
    6221,  6226,  6280,  6285,  6339,  6340,  6344,  6402,  6463,  6523,
    6586,  6597,  6602,  6607,  6612,  6601,  6670,  6675,  6669,  6690,
    6694,  6695,  6700,  6699,  6710,  6709,  6720,  6719,  6730,  6729,
    6740,  6739,  6756,  6777,  6801
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
  "@5", "@6", "@7", "@8", "@9", "@10", "@11", "@12", "@13", "@14", "@15",
  "module", "@16", "@17", "@18", "@19", "interface_def", "interface",
  "@20", "@21", "@22", "interface_decl", "@23", "interface_header",
  "inheritance_spec", "@24", "value_def", "valuetype",
  "value_concrete_decl", "@25", "@26", "@27", "value_abs_decl", "@28",
  "@29", "@30", "value_header", "@31", "value_decl", "@32",
  "opt_truncatable", "supports_spec", "value_forward_decl",
  "value_box_decl", "value_elements", "value_element", "state_member",
  "exports", "export", "@33", "@34", "@35", "@36", "@37", "@38", "@39",
  "@40", "at_least_one_scoped_name", "scoped_names", "@41", "scoped_name",
  "@42", "@43", "id", "interface_forward", "const_dcl", "@44", "@45",
  "@46", "@47", "const_type", "expression", "const_expr", "or_expr",
  "xor_expr", "and_expr", "shift_expr", "add_expr", "mult_expr",
  "unary_expr", "primary_expr", "literal", "positive_int_expr", "type_dcl",
  "@48", "type_declarator", "@49", "type_spec", "simple_type_spec",
  "base_type_spec", "template_type_spec", "constructed_type_spec",
  "constructed_forward_type_spec", "at_least_one_declarator",
  "declarators", "@50", "declarator", "at_least_one_simple_declarator",
  "simple_declarators", "@51", "simple_declarator", "complex_declarator",
  "integer_type", "signed_int", "unsigned_int", "floating_pt_type",
  "fixed_type", "char_type", "octet_type", "boolean_type", "any_type",
  "object_type", "struct_decl", "@52", "struct_type", "@53", "@54", "@55",
  "at_least_one_member", "members", "member", "@56", "member_i", "@57",
  "@58", "@59", "union_decl", "@60", "union_type", "@61", "@62", "@63",
  "@64", "@65", "@66", "switch_type_spec", "at_least_one_case_branch",
  "case_branches", "case_branch", "@67", "@68", "@69",
  "at_least_one_case_label", "case_labels", "case_label", "@70", "@71",
  "@72", "element_spec", "@73", "struct_forward_type",
  "union_forward_type", "enum_type", "@74", "@75", "@76", "@77",
  "at_least_one_enumerator", "enumerators", "@78", "enumerator",
  "sequence_type_spec", "@79", "@80", "seq_head", "@81", "@82",
  "string_type_spec", "@83", "@84", "string_head", "wstring_type_spec",
  "@85", "@86", "wstring_head", "array_declarator", "@87",
  "at_least_one_array_dim", "array_dims", "array_dim", "@88", "@89",
  "attribute", "attribute_readonly", "@90", "@91", "@92", "@93",
  "attribute_readwrite", "@94", "@95", "@96", "@97", "exception", "@98",
  "@99", "@100", "@101", "operation", "@102", "@103", "@104", "@105",
  "opt_op_attribute", "op_type_spec", "init_decl", "@106", "@107", "@108",
  "init_parameter_list", "@109", "@110", "at_least_one_in_parameter",
  "in_parameters", "@111", "in_parameter", "@112", "@113",
  "parameter_list", "@114", "@115", "at_least_one_parameter", "parameters",
  "@116", "parameter", "@117", "@118", "param_type_spec", "direction",
  "opt_raises", "@119", "@120", "opt_getraises", "@121", "@122",
  "opt_setraises", "@123", "@124", "opt_context", "@125", "@126",
  "at_least_one_string_literal", "string_literals", "@127", "typeid_dcl",
  "typeprefix_dcl", "component", "component_forward_decl",
  "component_decl", "@128", "@129", "@130", "component_header", "@131",
  "@132", "component_inheritance_spec", "@133", "component_exports",
  "component_export", "@134", "@135", "@136", "@137", "@138", "@139",
  "@140", "provides_decl", "interface_type", "uses_decl",
  "uses_opt_multiple", "opt_multiple", "emits_decl", "publishes_decl",
  "consumes_decl", "home_decl", "@141", "home_header", "@142", "@143",
  "@144", "@145", "@146", "@147", "home_inheritance_spec", "@148",
  "primary_key_spec", "home_body", "@149", "@150", "home_exports",
  "home_export", "@151", "@152", "factory_decl", "@153", "@154",
  "finder_decl", "@155", "@156", "event", "event_forward_decl",
  "event_concrete_forward_decl", "event_abs_forward_decl",
  "event_abs_decl", "@157", "@158", "@159", "event_abs_header",
  "event_custom_header", "event_plain_header", "event_rest_of_header",
  "@160", "event_decl", "@161", "@162", "@163", "event_header",
  "type_classifier", "template_interface_def", "@164", "@165", "@166",
  "template_interface_header", "at_least_one_template_param", "@167",
  "@168", "template_params", "@169", "template_param",
  "template_inheritance_spec", "at_least_one_template_ref",
  "template_refs", "template_ref", "at_least_one_template_param_ref",
  "template_param_refs", "template_param_ref", "porttype_decl", "@170",
  "@171", "@172", "@173", "@174", "opt_template_params",
  "at_least_one_port_export", "port_exports", "port_export", "@175",
  "@176", "extended_provides_decl", "extended_uses_decl",
  "extended_port_decl", "template_port_decl", "non_template_port_decl",
  "template_inst", "connector_decl", "connector_header", "@177", "@178",
  "@179", "connector_body", "@180", "@181",
  "at_least_one_connector_export", "connector_exports", "connector_export",
  "@182", "@183", "@184", "@185", "@186", "template_extended_port_decl",
  "template_ref_decl", 0
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
     110,   124,   110,   125,   110,   127,   128,   129,   130,   126,
     131,   131,   133,   134,   135,   132,   137,   136,   138,   138,
     138,   140,   139,   139,   141,   141,   141,   141,   142,   142,
     144,   145,   146,   143,   148,   149,   150,   147,   152,   151,
     154,   153,   155,   155,   156,   156,   157,   157,   158,   159,
     159,   160,   160,   160,   161,   161,   162,   162,   164,   163,
     165,   163,   166,   163,   167,   163,   168,   163,   169,   163,
     170,   163,   171,   163,   172,   174,   173,   173,   175,   176,
     175,   177,   175,   178,   179,   179,   179,   181,   182,   183,
     184,   180,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   186,   187,   188,   188,   189,   189,   190,   190,   191,
     191,   191,   192,   192,   192,   193,   193,   193,   193,   194,
     194,   194,   194,   195,   195,   195,   196,   196,   196,   196,
     196,   196,   196,   196,   196,   197,   199,   198,   198,   198,
     198,   198,   198,   201,   200,   202,   202,   203,   203,   203,
     204,   204,   204,   204,   204,   204,   204,   204,   205,   205,
     205,   206,   206,   206,   207,   207,   208,   210,   209,   209,
     211,   211,   212,   214,   213,   213,   215,   216,   217,   217,
     218,   218,   218,   219,   219,   219,   220,   220,   220,   221,
     222,   222,   223,   224,   225,   226,   228,   227,   230,   231,
     232,   229,   233,   234,   234,   236,   235,   238,   239,   237,
     240,   237,   242,   241,   244,   245,   246,   247,   248,   249,
     243,   250,   250,   250,   250,   250,   250,   251,   252,   252,
     254,   255,   253,   256,   253,   257,   258,   258,   260,   259,
     261,   262,   259,   264,   263,   265,   266,   268,   269,   270,
     271,   267,   272,   274,   273,   273,   275,   277,   278,   276,
     276,   280,   281,   279,   283,   284,   282,   282,   285,   287,
     288,   286,   286,   289,   291,   290,   292,   293,   293,   295,
     296,   294,   297,   297,   299,   300,   301,   302,   298,   304,
     305,   306,   307,   303,   309,   310,   311,   312,   308,   314,
     315,   316,   317,   313,   318,   318,   318,   319,   319,   321,
     322,   323,   320,   325,   324,   326,   324,   327,   329,   328,
     328,   331,   332,   330,   334,   333,   335,   333,   336,   338,
     337,   337,   340,   341,   339,   342,   342,   342,   342,   343,
     343,   343,   345,   346,   344,   344,   348,   349,   347,   347,
     351,   352,   350,   350,   354,   355,   353,   353,   356,   358,
     357,   357,   359,   360,   361,   361,   362,   364,   365,   366,
     363,   368,   369,   367,   371,   370,   370,   372,   372,   374,
     373,   375,   373,   376,   373,   377,   373,   378,   373,   379,
     373,   380,   373,   381,   382,   382,   383,   384,   385,   385,
     386,   387,   388,   390,   389,   392,   393,   394,   395,   396,
     397,   391,   399,   398,   398,   400,   400,   402,   403,   401,
     404,   404,   405,   406,   405,   407,   405,   409,   410,   408,
     412,   413,   411,   414,   414,   414,   415,   415,   416,   417,
     419,   420,   421,   418,   422,   423,   424,   426,   425,   428,
     429,   430,   427,   431,   431,   432,   432,   432,   432,   432,
     432,   432,   432,   434,   435,   436,   433,   437,   439,   440,
     438,   442,   441,   441,   443,   444,   444,   445,   446,   446,
     447,   448,   449,   449,   450,   452,   453,   454,   455,   456,
     451,   457,   457,   458,   459,   459,   461,   460,   462,   460,
     463,   463,   464,   464,   465,   465,   466,   466,   467,   467,
     468,   469,   471,   472,   473,   470,   475,   476,   474,   477,
     478,   478,   480,   479,   481,   479,   482,   479,   483,   479,
     484,   479,   485,   485,   486
};

/* TAO_YYR2[TAO_YYN] -- Number of symbols composing right hand side of rule TAO_YYN.  */
static const tao_yytype_uint8 tao_yyr2[] =
{
       0,     2,     1,     2,     0,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     0,     0,     0,     9,
       1,     1,     0,     0,     0,     7,     0,     3,     2,     3,
       3,     0,     4,     0,     1,     1,     1,     1,     2,     1,
       0,     0,     0,     7,     0,     0,     0,     8,     0,     4,
       0,     3,     1,     0,     2,     0,     2,     1,     2,     2,
       0,     1,     1,     2,     2,     2,     2,     0,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     2,     0,     4,     0,     1,     0,
       3,     0,     4,     1,     1,     2,     2,     0,     0,     0,
       0,     9,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       2,     2,     2,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     3,     1,     1,
       1,     2,     1,     0,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     0,     4,     0,
       1,     1,     2,     0,     4,     0,     1,     1,     1,     1,
       1,     2,     1,     2,     3,     2,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     0,     3,     0,     0,
       0,     7,     2,     2,     0,     0,     2,     0,     0,     5,
       0,     3,     0,     3,     0,     0,     0,     0,     0,     0,
      14,     1,     1,     1,     1,     1,     1,     2,     2,     0,
       0,     0,     5,     0,     3,     2,     2,     0,     0,     3,
       0,     0,     5,     0,     3,     1,     1,     0,     0,     0,
       0,     9,     2,     0,     4,     0,     1,     0,     0,     6,
       2,     0,     0,     5,     0,     0,     6,     1,     1,     0,
       0,     6,     1,     1,     0,     3,     2,     2,     0,     0,
       0,     5,     1,     1,     0,     0,     0,     0,     9,     0,
       0,     0,     0,     9,     0,     0,     0,     0,     9,     0,
       0,     0,     0,    10,     1,     1,     0,     1,     1,     0,
       0,     0,     7,     0,     3,     0,     4,     2,     0,     4,
       0,     0,     0,     5,     0,     3,     0,     4,     2,     0,
       4,     0,     0,     0,     5,     1,     1,     1,     1,     1,
       1,     1,     0,     0,     6,     0,     0,     0,     6,     0,
       0,     0,     6,     0,     0,     0,     6,     0,     2,     0,
       4,     0,     3,     3,     1,     1,     2,     0,     0,     0,
       7,     0,     0,     6,     0,     3,     0,     2,     0,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     3,     1,     1,     3,     2,     1,     0,
       3,     3,     3,     0,     3,     0,     0,     0,     0,     0,
       0,    13,     0,     3,     0,     2,     0,     0,     0,     5,
       2,     0,     1,     0,     3,     0,     3,     0,     0,     6,
       0,     0,     6,     1,     1,     1,     1,     1,     2,     3,
       0,     0,     0,     8,     3,     3,     2,     0,     3,     0,
       0,     0,     8,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     0,     0,     7,     3,     0,     0,
       6,     0,     4,     0,     2,     2,     0,     2,     3,     0,
       4,     2,     3,     0,     1,     0,     0,     0,     0,     0,
      11,     1,     0,     2,     2,     0,     0,     3,     0,     3,
       1,     3,     1,     3,     1,     1,     3,     3,     3,     3,
       4,     2,     0,     0,     0,     7,     0,     0,     5,     2,
       2,     0,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     2,     2,     2
};

/* TAO_YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when TAO_YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const tao_yytype_uint16 tao_yydefact[] =
{
       4,     0,     0,     1,    33,   117,    35,    46,   166,   226,
     242,   277,   324,     0,     0,     0,     0,    70,     0,     0,
     435,     0,     0,   515,   542,     3,    19,    15,    40,    53,
      42,    21,    54,    59,    55,    60,    53,    56,    57,    41,
      11,     5,   172,   228,   168,   276,   169,   194,   195,   170,
      13,     7,     9,    23,   395,   394,   397,    25,   433,    27,
     465,   467,   466,   464,    53,   483,   484,   463,    53,    17,
     493,    29,    31,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   113,   206,   171,    53,     0,    53,    64,
      53,     0,    58,    53,     0,   401,   476,     0,   109,     0,
     108,     0,     0,     0,     0,     0,    73,   498,    48,   506,
       0,     0,     0,   210,   212,     0,   216,   217,   220,   221,
     222,   223,   219,   224,   225,   291,   298,   303,    68,   179,
      78,   175,   177,   178,   176,   180,   208,   209,   181,   185,
     182,   184,   183,   186,   187,   228,   191,     0,   192,   193,
     188,     0,   189,   297,   190,   302,     0,     0,     0,   244,
       0,     0,     0,     0,     0,     0,     0,     0,   477,   470,
     479,     0,     0,     0,     0,   546,   541,    34,   130,   118,
     122,   126,   127,   123,   124,   125,   128,   129,    36,    47,
     167,   173,   227,   243,   278,   325,    49,   474,    50,     0,
     475,    71,   406,   436,     0,   392,   111,   393,   516,   543,
      20,    16,    72,    51,     0,     0,   497,    43,    22,    61,
     211,   218,   213,   215,     0,    75,   287,   290,   294,   299,
      12,     6,   229,     0,    14,     8,    10,    24,   398,    26,
     447,   434,    28,    75,     0,     0,    18,   494,    30,    32,
       0,     0,     0,     0,     0,     0,    65,   404,   402,   444,
     110,     0,   522,   522,     0,   491,   489,   486,   490,   492,
     487,   485,   488,     0,   503,     0,   505,   509,    87,    80,
     214,   292,     0,    69,     0,     0,     0,   235,   245,   408,
     451,   478,   471,   480,    87,   314,   319,     0,   429,     0,
       0,   556,   312,   313,   552,   554,     0,   558,   547,   551,
     560,   119,    37,   206,   174,   199,   200,   201,   207,   279,
     326,    87,     0,    75,   442,   437,   112,   521,   517,   544,
      52,   107,   504,   499,     0,   507,     0,     0,     0,    74,
     156,   157,   158,   160,   162,   163,   164,   161,   159,     0,
       0,     0,     0,   153,   165,   132,   133,   135,   137,   139,
     142,   145,   149,   154,   288,   295,   300,   230,   234,     0,
       0,   399,     0,    87,    80,     0,     0,     0,   425,   424,
       0,   428,   427,     0,     0,   562,     0,   563,     0,     0,
       0,     0,     0,     0,   549,     0,     0,     4,     0,   196,
       0,   234,     0,   405,   403,     0,    75,     0,   406,   104,
     501,     0,   514,     0,   513,     0,   102,   334,   335,     0,
      86,    94,    88,    98,    96,   100,     0,    90,    92,   339,
       0,     0,     0,    79,    81,    82,     0,   293,   150,   151,
     152,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   235,   240,   237,   236,
     210,   256,   251,   252,   253,   254,   246,   255,     0,     0,
       0,     0,     0,   419,     0,   407,   409,   411,   413,   415,
     417,   421,   534,   535,     0,     0,   452,     0,   450,   453,
     455,     0,     0,     0,   315,   368,   365,   366,   367,   320,
     423,   538,   564,   539,   557,   553,   555,   426,   559,   548,
     550,   561,   120,     0,   309,   305,   308,   197,   286,   280,
     285,   235,     0,   443,   438,   518,   545,   105,     0,   500,
     510,   511,   508,     0,    45,     0,     0,     0,     0,     0,
     338,   329,   337,     0,     0,     0,    85,    84,    63,    83,
     155,   134,   136,   138,   140,   141,   143,   144,   146,   147,
     148,   289,   296,   301,   231,   233,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
       0,     0,     0,     0,   457,   460,   449,     0,     0,     0,
       0,   496,     0,     0,     0,     0,     0,   306,     0,     0,
     282,     0,    67,     0,     0,     0,   502,     0,   103,    95,
      89,    99,    97,   101,     0,    91,    93,   340,   241,   238,
     247,   432,   430,   431,     0,   536,   537,   420,   410,   412,
     414,   416,   418,   422,     0,     0,   454,   456,   473,   482,
     316,   321,   205,   121,   131,    39,   310,   307,   198,   281,
     283,   328,   439,     0,   530,   532,     0,   519,   525,   526,
     528,   106,   512,   330,     0,     0,     0,     0,   343,   458,
     461,     0,   379,   202,     0,     0,     0,   424,     0,     0,
       0,   523,     0,     0,     0,   341,   239,   248,   540,     0,
       0,   375,   375,   317,   376,   322,   203,   311,   284,   440,
     531,   533,   520,   524,   527,   529,   356,   331,   375,     0,
     344,   351,     0,   350,   372,   459,   462,   375,     0,   383,
       0,   446,     0,     0,   375,   342,   263,   270,   268,   249,
     259,   260,   267,     0,   346,   347,     0,   318,   377,   380,
     323,   204,     0,   441,   355,   369,   370,   371,     0,   361,
     362,   332,     0,     0,     0,     0,     0,     0,   265,   352,
     348,   373,     0,     0,   445,   357,   358,     0,   387,   264,
     271,   269,   250,   258,   273,   261,   266,     0,     0,     0,
       0,   381,   359,   363,   384,   333,     0,     0,     0,   353,
     349,     0,   378,     0,     0,     0,     0,   272,   274,   262,
     374,     0,   360,   364,   385,   382,     0,   391,     0,   388,
     386,   389,     0,   390
};

/* TAO_YYDEFGOTO[NTERM-NUM].  */
static const tao_yytype_int16 tao_yydefgoto[] =
{
      -1,     1,     2,    25,   157,   161,   162,   156,   160,   105,
     171,   104,   111,   163,   165,   167,   173,   174,    74,    26,
      76,   252,   397,   595,    27,    28,   110,   278,   419,    29,
      77,    30,   128,   264,    31,    32,    33,   112,   279,   432,
      34,   199,   321,   522,    35,   225,    36,    94,   213,   283,
      37,    38,   337,   433,   434,   336,   420,   536,   543,   544,
     535,   538,   537,   539,   533,   330,   409,   605,   353,   204,
     261,   100,    39,   421,    75,   251,   396,   594,   179,   643,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   422,    78,   190,   253,   458,   131,   132,   133,   134,
      42,   314,   399,   598,   315,   641,   673,   720,   316,   317,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
      43,    79,    44,   158,   287,   455,   367,   456,   565,   369,
     459,   567,   665,   566,    45,    80,    46,   233,   370,   568,
     666,   709,   755,   466,   729,   756,   730,   757,   788,   752,
     731,   758,   732,   754,   753,   786,   775,   787,    47,    48,
      49,    81,   254,   400,   599,   519,   600,   675,   520,   150,
     284,   452,   151,   224,   338,   152,   285,   453,   153,   154,
     286,   454,   155,   318,   398,   515,   597,   516,   596,   674,
     423,   302,   376,   592,   671,   717,   303,   377,   593,   672,
     719,   424,    82,   255,   401,   601,   425,   614,   684,   724,
     768,   426,   541,   436,   545,   664,   708,   669,   689,   690,
     712,   735,   778,   713,   733,   777,   707,   722,   723,   748,
     766,   794,   749,   767,   795,   499,   750,   715,   736,   779,
     695,   718,   762,   740,   763,   793,   785,   796,   806,   808,
     809,   812,   427,   428,    53,    54,    55,   164,   289,   474,
      56,   202,   323,   258,   322,   371,   475,   578,   579,   580,
     581,   582,   576,   583,   304,   380,   305,   306,   382,   478,
     479,   480,    57,   166,    58,    97,   259,   406,   603,   676,
     721,   325,   405,   743,   241,   290,   487,   372,   488,   587,
     588,   489,   634,   691,   490,   635,   692,    59,    60,    61,
      62,    63,   244,   373,   589,    64,    65,    66,   169,   243,
      67,   245,   374,   590,    68,   273,    69,   172,   294,   493,
      70,   327,   214,   411,   333,   528,   274,   216,   276,   335,
     384,   413,   531,   414,    71,   102,   262,   407,   604,   680,
     328,   657,   681,   658,   682,   683,   659,   660,   481,   482,
     307,   573,    72,    73,   103,   263,   408,   176,   250,   393,
     308,   394,   309,   389,   390,   388,   392,   395,   310,   385
};

/* TAO_YYPACT[STATE-NUM] -- Index in TAO_YYTABLE of the portion describing
   STATE-NUM.  */
#define TAO_YYPACT_NINF -636
static const tao_yytype_int16 tao_yypact[] =
{
    -636,    77,  1164,  -636,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,    44,    65,   125,   147,  -636,    44,    44,
    -636,    51,    51,  -636,  -636,  -636,  -636,  -636,  -636,   -24,
    -636,  -636,  -636,  -636,  -636,  -636,    84,  -636,  -636,  -636,
    -636,  -636,  -636,    50,  -636,   149,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,    58,  -636,  -636,  -636,    58,  -636,
    -636,  -636,  -636,    89,    97,   343,   166,    44,  1111,    44,
      44,    44,    44,  -636,  -636,  -636,    66,    44,   112,  -636,
     113,    44,  -636,    58,    44,   131,   146,    44,  -636,    30,
    -636,    67,   230,   244,   179,   180,   204,  -636,  -636,   183,
     181,   187,   182,   195,  -636,    82,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,   201,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,   149,  -636,  -636,
    -636,    87,  -636,   190,  -636,   191,   197,   198,   200,  -636,
     205,   210,   213,   215,   216,   218,   219,   225,  -636,  -636,
    -636,   227,   221,   229,   232,  -636,  -636,  -636,   201,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,   235,  -636,   236,
    -636,  -636,   211,  -636,    44,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,   222,    51,  -636,  -636,  -636,  -636,
    -636,  -636,   268,  -636,   220,   258,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,   226,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,   258,   239,   241,  -636,  -636,  -636,  -636,
     228,    44,   242,    44,   246,   254,  -636,  -636,  -636,   233,
    -636,    44,   243,   243,    51,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,   328,  -636,   -21,  -636,  -636,  -636,  -636,
    -636,  -636,    51,  -636,    49,    49,    49,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,    39,   285,    51,
      51,  -636,  -636,  -636,  -636,  -636,    39,  -636,  -636,  -636,
    -636,  -636,  -636,   240,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,    51,   258,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,   201,  -636,   259,   359,   275,   483,   610,   450,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,    83,
      83,    83,    49,   201,  -636,   274,   278,   279,    71,   -40,
     111,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,   580,
     172,   361,   879,  -636,  -636,   961,   341,  1135,  -636,   201,
      44,  -636,  -636,    35,   371,  -636,    36,  -636,   289,   290,
     295,    44,   296,   297,   228,   298,   292,  -636,   281,   299,
     385,  -636,   996,   201,  -636,    51,   258,   303,   211,   304,
    -636,   305,  -636,   306,  -636,    51,  -636,  -636,  -636,   311,
    -636,  -636,  -636,  -636,  -636,  -636,   837,  -636,  -636,  -636,
     580,   580,   312,  -636,  -636,  -636,   307,  -636,  -636,  -636,
    -636,   308,    49,    49,    49,    49,    49,    49,    49,    49,
      49,    49,   313,   314,   315,   319,   326,  -636,  -636,  -636,
     412,   201,  -636,  -636,  -636,  -636,  -636,  -636,    51,    51,
      51,    51,    51,  -636,   335,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,    44,    44,  -636,   339,  -636,  -636,
    -636,  1078,   797,   340,  -636,   201,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,   214,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,   344,   345,   201,  -636,  -636,  -636,  -636,   222,  -636,
    -636,   347,  -636,   348,  -636,   349,   352,   353,   356,   357,
    -636,  -636,  -636,   358,   360,   426,  -636,  -636,  -636,  -636,
    -636,   278,   279,    71,   -40,   -40,   111,   111,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,   362,    44,   350,    56,
      56,    56,    37,   442,    38,   444,   363,  -636,   364,   365,
     369,   370,   384,   387,  -636,  -636,  -636,   391,   397,   401,
     419,  -636,  1135,    44,    49,   423,    49,   281,    44,   424,
     395,   430,  -636,   454,    86,    51,  -636,   359,  -636,  -636,
    -636,  -636,  -636,  -636,   518,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,    51,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,   421,   421,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,    39,  -636,  -636,    39,  -636,  -636,  -636,
    -636,   201,  -636,  -636,   421,   446,   447,   432,   498,  -636,
    -636,    44,   481,   448,   431,   385,    51,   -21,   536,   537,
     453,    86,   456,   457,   443,  -636,  -636,  -636,  -636,   449,
     510,   511,   511,  -636,  -636,  -636,  -636,  -636,  -636,   201,
    -636,  -636,  -636,  -636,  -636,  -636,   451,  -636,   511,   165,
    -636,  -636,   458,  -636,  -636,  -636,  -636,   511,   460,   486,
      44,   491,   461,   208,   511,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  1135,  -636,   462,   464,  -636,  -636,  -636,
    -636,  -636,    51,  -636,  -636,  -636,  -636,  -636,   465,  -636,
    -636,  -636,   469,    49,   470,   474,    47,  1111,    93,  -636,
    -636,  -636,    51,   467,   201,  -636,   476,  1135,   530,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,    44,   510,    51,
     468,  -636,  -636,  -636,  -636,  -636,   484,    44,   488,  -636,
    -636,   473,  -636,    51,   208,    44,   471,  -636,  -636,  -636,
    -636,   475,  -636,  -636,  -636,  -636,   500,  -636,   477,   492,
    -636,  -636,   502,  -636
};

/* TAO_YYPGOTO[NTERM-NUM].  */
static const tao_yytype_int16 tao_yypgoto[] =
{
    -636,  -636,   212,  -636,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,    98,
    -636,  -636,   143,  -636,  -636,  -636,   570,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,   572,  -636,   189,  -636,  -636,  -209,
    -636,  -636,   234,  -636,  -636,  -258,  -300,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -274,  -636,  -636,    10,  -636,
    -636,   -13,  -636,    11,  -636,  -636,  -636,  -636,  -636,  -636,
    -334,  -636,   168,   164,   188,  -232,  -224,  -200,   -70,  -636,
    -259,    12,  -636,  -636,  -636,   -34,   300,  -356,  -636,  -636,
    -636,    45,  -636,  -636,  -565,   -55,  -636,  -636,    -9,  -636,
     -53,  -636,  -636,   564,   571,   -51,   -50,   -45,  -636,  -636,
     -33,  -636,   -29,  -636,  -636,  -636,  -636,   247,   366,  -636,
    -204,  -636,  -636,  -636,   -27,  -636,   -25,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -111,  -636,  -636,  -636,
    -636,  -636,  -109,  -636,  -636,  -636,  -636,  -636,  -636,  -636,
     -35,  -636,  -636,  -636,  -636,  -636,  -636,  -636,   -28,  -636,
    -636,  -636,  -636,  -636,  -636,   -65,  -636,  -636,  -636,   -63,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,    54,  -636,  -636,
    -215,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,    13,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,  -585,  -636,  -636,
    -636,  -636,  -636,  -126,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -137,  -636,  -636,  -403,  -636,  -635,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,    15,    17,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,   251,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,  -343,  -286,  -342,  -543,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,   592,  -636,
    -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,   635,  -636,  -636,  -636,  -636,   137,  -636,  -636,  -636,
    -199,  -636,  -636,    60,  -636,  -636,  -636,  -636,  -636,  -636,
     406,  -636,  -636,   -10,  -636,  -636,  -636,  -636,  -636,  -636,
     301,   203,  -636,  -636,  -636,  -636,  -636,  -636,  -636,  -636,
    -636,  -636,   282,  -636,  -636,  -636,  -636,  -636,  -636,   377
};

/* TAO_YYTABLE[TAO_YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what TAO_YYDEFACT says.
   If TAO_YYTABLE_NINF, syntax error.  */
#define TAO_YYTABLE_NINF -496
static const tao_yytype_int16 tao_yytable[] =
{
      84,   149,   130,   145,    85,    95,    96,   146,   339,   147,
     186,   148,   187,    40,    41,    50,   277,    51,   441,    52,
     391,   496,   180,   542,   183,   184,   365,   366,   476,   477,
     185,    99,   101,   648,   291,   301,   375,   435,   501,   503,
     501,   503,    83,   149,   191,   145,   129,    83,   726,   146,
     670,   147,    83,   148,    83,   447,   448,   716,   378,    83,
     206,   656,  -114,   402,   189,   106,   192,   193,   194,   195,
     496,     7,   486,   725,   197,   727,   728,     3,   200,   685,
     107,   201,   737,   334,   203,   178,    83,    83,   129,   751,
     222,   223,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,     9,    10,   205,    11,   125,   126,
     127,   206,    86,    88,   404,   491,   206,   206,   206,   206,
      98,   727,   728,   340,   341,   342,   343,   344,   345,   346,
      98,     7,    98,   347,   348,  -257,  -275,   206,   656,   334,
     334,   624,   624,   207,   349,   350,   653,   106,   206,   351,
     352,   298,  -115,   445,   446,   106,   473,   340,   341,   342,
     343,   344,   345,   346,    98,    98,   726,   347,   348,   188,
     -77,   159,   108,   106,    17,    83,   175,   226,    87,   301,
     460,   114,   115,   177,   352,   118,   119,   120,   121,   640,
     227,   260,   435,   727,   728,    11,    17,   524,  -116,   -76,
      91,   106,   106,   220,    90,    93,   221,   168,   449,   450,
     451,   168,   789,   554,   555,     4,   532,  -396,     5,     6,
       7,     8,   798,   556,   557,   275,   546,   547,   265,   196,
     803,   198,  -468,   208,     9,    10,   496,    11,   311,   266,
     313,    12,   267,   745,   746,   747,   268,   209,   326,   558,
     559,   560,   212,    98,    13,    14,    15,    16,   295,   296,
     644,   654,   655,    17,    18,   210,   211,    19,   217,   219,
      20,   269,   215,   218,   331,   270,   280,    21,    22,   438,
     439,   440,   206,   230,   231,    23,    24,   232,   297,   271,
     272,   234,   331,   298,   228,   229,   235,   299,   300,   236,
     257,   237,   -38,   238,   239,   282,   240,   379,   247,   383,
     386,   242,   497,   246,   498,   248,   379,   462,   249,   463,
     464,  -469,   324,   256,   281,   465,   292,   288,   293,   312,
     759,   332,   403,   319,   149,   467,   145,   646,   654,   655,
     146,   320,   147,   381,   148,  -304,    83,   107,   129,   410,
     667,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   497,   412,   498,   783,   415,   442,   500,   126,   127,
     391,   443,   494,   444,   502,   504,   505,   496,   507,   129,
     461,   506,   508,   512,   511,   509,   514,   495,   518,   517,
     525,   295,   296,   549,   527,   149,   149,   145,   145,   534,
     548,   146,   146,   147,   147,   148,   148,   564,   529,   530,
     550,   496,   468,   469,  -232,   523,   561,   562,   563,   770,
     220,   297,   470,   577,    98,   275,   298,   586,   591,   617,
     471,   472,  -327,   602,   608,   609,   495,   607,   610,   611,
     129,   129,   612,   613,   615,   625,   616,   626,   618,   627,
     628,   629,   620,    83,   678,   630,   631,   679,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     632,   584,   585,   633,   125,   126,   127,   636,   569,   570,
     571,   572,   574,   637,   416,   650,  -336,     5,   780,   638,
       8,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,
    -336,  -336,  -336,     9,    10,   791,    11,   639,  -336,  -336,
      12,   645,   649,   295,   296,   417,   418,  -336,   651,   801,
     652,   663,   668,    13,    40,    41,    50,   497,    51,   498,
      52,    98,   686,  -345,   687,   688,   694,   697,   696,   700,
     701,   702,   704,   705,   706,   711,    21,    22,   739,   714,
     742,   710,   760,  -354,   313,   769,   621,   622,   623,   771,
     734,   738,   772,   744,  -336,   761,   782,   765,   781,   784,
     792,   -44,   804,   797,   799,   800,   807,   805,   813,   810,
      84,   457,   811,    83,   642,   313,    92,    89,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
       9,    10,   495,    11,   125,   126,   127,   552,   492,   513,
     551,   416,   619,  -336,     5,   661,   693,     8,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,
       9,    10,   553,    11,   331,  -336,  -336,    12,   437,   181,
     295,   296,   417,   418,  -336,   773,   182,   698,   521,   776,
      13,   647,   790,   368,   429,   430,   431,   802,    84,   526,
     170,    98,   642,   677,   109,   606,   677,   662,   497,   329,
     498,   703,   483,    21,    22,   575,   510,   387,     0,     0,
       0,     0,     0,     0,     0,     0,   699,     0,     0,     0,
       0,  -336,     0,     0,     0,     0,     0,     0,   -62,     0,
       0,     0,   497,     0,   498,     0,     0,    84,     0,     0,
       0,   741,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   149,   774,   145,     0,     0,     0,   146,     0,
     147,     0,   148,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   495,     0,     0,     0,     0,     0,     0,
       0,     0,   764,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   313,     0,     0,   129,     0,     0,
       0,     0,   331,     0,   313,     0,     0,   495,     0,     0,
       0,     0,   313,     0,     0,     0,     0,     0,     0,   331,
       0,     0,     0,     0,     0,     0,     0,     0,   416,     0,
    -336,     5,     0,   331,     8,  -336,  -336,  -336,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,     9,    10,     0,
      11,     0,  -336,  -336,    12,     0,     0,   295,   296,   417,
     418,  -336,     0,     0,     0,     0,     0,    13,     0,     0,
      83,   429,   430,   431,     0,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,     0,     0,     0,
      21,    22,   126,   127,     0,     0,     0,     0,     0,     0,
       0,   540,     0,     0,     0,     0,     0,     0,  -336,     0,
     416,     0,  -336,     5,     0,  -481,     8,  -336,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,     9,
      10,     0,    11,     0,  -336,  -336,    12,     0,     0,   295,
     296,   417,   418,  -336,     0,     0,     0,     0,    98,    13,
       0,     0,     0,   484,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   485,     0,     0,     0,     0,     0,     0,
       0,     0,    21,    22,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -336,     0,   416,     0,  -336,     5,     0,  -448,     8,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,
    -336,     9,    10,     0,    11,     0,  -336,  -336,    12,     0,
       0,   295,   296,   417,   418,  -336,     0,   416,     0,  -336,
       5,    13,     0,     8,  -336,  -336,  -336,  -336,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,     9,    10,     0,    11,
       0,  -336,  -336,    12,    21,    22,   295,   296,   417,   418,
    -336,     0,     0,     0,     0,     0,    13,     0,     0,     0,
       0,     0,  -336,     0,     0,     0,     0,     0,     0,  -495,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -336,     0,   416,
       0,  -336,     5,     0,   -66,     8,  -336,  -336,  -336,  -336,
    -336,  -336,  -336,  -336,  -336,  -336,  -336,  -336,     9,    10,
       0,    11,     0,  -336,  -336,    12,     0,     0,   295,   296,
     417,   418,  -336,     0,    83,     0,     0,     0,    13,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,     9,    10,     0,    11,   125,   126,   127,    83,     0,
       0,    21,    22,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,     0,     0,     0,     0,  -336,
     126,   127,     0,     0,    -2,     4,  -472,     0,     5,     6,
       7,     8,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     9,    10,     0,    11,     0,     0,
       0,    12,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    13,    14,    15,    16,     0,     0,
       0,     0,     0,    17,    18,     0,    98,    19,     0,     0,
      20,     0,     0,     0,     0,     0,     0,    21,    22,     0,
       0,     0,     0,     0,     0,    23,    24
};

static const tao_yytype_int16 tao_yycheck[] =
{
      13,    36,    36,    36,    13,    18,    19,    36,   282,    36,
      75,    36,    75,     2,     2,     2,   215,     2,   352,     2,
     306,   377,    75,   426,    75,    75,   285,   286,   371,   371,
      75,    21,    22,   598,   243,   250,   294,   337,     3,     3,
       3,     3,     3,    78,    78,    78,    36,     3,     1,    78,
     635,    78,     3,    78,     3,    95,    96,   692,    19,     3,
      81,   604,    86,   321,    77,    89,    79,    80,    81,    82,
     426,     6,   372,   708,    87,    28,    29,     0,    91,   664,
     104,    94,   717,   104,    97,    75,     3,     3,    78,   724,
       8,     9,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    76,    23,    24,    25,
      26,    81,    14,    15,   323,   373,    81,    81,    81,    81,
      81,    28,    29,    74,    75,    76,    77,    78,    79,    80,
      81,     6,    81,    84,    85,    88,    86,    81,   681,   104,
     104,   104,   104,    76,    95,    96,    60,    89,    81,   100,
     101,    65,    86,    82,    83,    89,   371,    74,    75,    76,
      77,    78,    79,    80,    81,    81,     1,    84,    85,     3,
      86,    22,    29,    89,    49,     3,    87,    90,    53,   394,
       8,     9,    10,    86,   101,    13,    14,    15,    16,   592,
     103,   204,   492,    28,    29,    23,    49,   406,    86,    86,
      53,    89,    89,     8,    15,    16,    11,    64,    97,    98,
      99,    68,   777,   445,   446,     1,   415,    86,     4,     5,
       6,     7,   787,   447,   448,   215,   430,   431,     6,    86,
     795,    88,    86,     3,    20,    21,   592,    23,   251,    17,
     253,    27,    20,    35,    36,    37,    24,     3,   261,   449,
     450,   451,    48,    81,    40,    41,    42,    43,    30,    31,
     594,   604,   604,    49,    50,    86,    86,    53,    87,    87,
      56,    49,    89,    86,   264,    53,     8,    63,    64,   349,
     350,   351,    81,    86,    86,    71,    72,    87,    60,    67,
      68,    86,   282,    65,   104,   104,    86,    69,    70,    86,
      89,    86,    88,    87,    86,    47,    87,   297,    87,   299,
     300,    86,   377,    86,   377,    86,   306,   370,    86,   370,
     370,    86,    89,    87,   104,   370,    87,   101,    87,    87,
     733,     3,   322,    87,   369,   370,   369,   596,   681,   681,
     369,    87,   369,    58,   369,   105,     3,   104,   338,    90,
     624,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,   426,     3,   426,   767,    90,    92,   380,    25,    26,
     656,    93,    31,    94,     3,    86,    86,   733,   391,   369,
     370,    86,    86,    91,    86,    88,   105,   377,     3,    90,
      87,    30,    31,    86,    90,   430,   431,   430,   431,    88,
      88,   430,   431,   430,   431,   430,   431,    88,   103,   103,
     102,   767,    51,    52,    88,   405,   103,   103,   103,   753,
       8,    60,    61,    88,    81,   415,    65,    88,    88,     3,
      69,    70,    88,    88,    86,    86,   426,    90,    86,    86,
     430,   431,    86,    86,    86,     3,    86,     3,    86,    86,
      86,    86,   102,     3,   653,    86,    86,   656,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      86,   484,   485,    86,    24,    25,    26,    86,   468,   469,
     470,   471,   472,    86,     1,    90,     3,     4,   762,    88,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   779,    23,    88,    25,    26,
      27,    88,    88,    30,    31,    32,    33,    34,    88,   793,
      66,     3,   101,    40,   513,   513,   513,   592,   513,   592,
     513,    81,    86,    35,    87,   103,    55,   106,    90,     3,
       3,    88,    86,    86,   101,    35,    63,    64,    62,    38,
      59,   102,    90,   102,   567,    86,   569,   570,   571,    89,
     102,   101,    88,   102,    81,   101,    90,   102,   101,    39,
     102,    88,   101,    89,    86,   102,    76,   102,    76,   102,
     593,     1,    90,     3,   593,   598,    16,    15,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   592,    23,    24,    25,    26,   443,   374,   397,
     442,     1,   567,     3,     4,   605,   671,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   444,    23,   624,    25,    26,    27,   338,    75,
      30,    31,    32,    33,    34,   756,    75,   675,   401,   758,
      40,   597,   778,   287,    44,    45,    46,   794,   671,   408,
      68,    81,   671,   653,    29,   528,   656,   607,   733,   263,
     733,   681,   371,    63,    64,   472,   394,   300,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   676,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,   767,    -1,   767,    -1,    -1,   720,    -1,    -1,
      -1,   720,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   757,   757,   757,    -1,    -1,    -1,   757,    -1,
     757,    -1,   757,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   733,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   742,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   777,    -1,    -1,   757,    -1,    -1,
      -1,    -1,   762,    -1,   787,    -1,    -1,   767,    -1,    -1,
      -1,    -1,   795,    -1,    -1,    -1,    -1,    -1,    -1,   779,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,    -1,   793,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      23,    -1,    25,    26,    27,    -1,    -1,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
       3,    44,    45,    46,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      63,    64,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,
       1,    -1,     3,     4,    -1,    88,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    -1,    25,    26,    27,    -1,    -1,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    81,    40,
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
      32,    33,    34,    -1,     3,    -1,    -1,    -1,    40,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    24,    25,    26,     3,    -1,
      -1,    63,    64,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    81,
      25,    26,    -1,    -1,     0,     1,    88,    -1,     4,     5,
       6,     7,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    23,    -1,    -1,
      -1,    27,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    40,    41,    42,    43,    -1,    -1,
      -1,    -1,    -1,    49,    50,    -1,    81,    53,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72
};

/* TAO_YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const tao_yytype_uint16 tao_yystos[] =
{
       0,   108,   109,     0,     1,     4,     5,     6,     7,    20,
      21,    23,    27,    40,    41,    42,    43,    49,    50,    53,
      56,    63,    64,    71,    72,   110,   126,   131,   132,   136,
     138,   141,   142,   143,   147,   151,   153,   157,   158,   179,
     180,   198,   207,   227,   229,   241,   243,   265,   266,   267,
     308,   359,   360,   361,   362,   363,   367,   389,   391,   414,
     415,   416,   417,   418,   422,   423,   424,   427,   431,   433,
     437,   451,   469,   470,   125,   181,   127,   137,   199,   228,
     242,   268,   309,     3,   178,   215,   136,    53,   136,   151,
     153,    53,   143,   153,   154,   178,   178,   392,    81,   175,
     178,   175,   452,   471,   118,   116,    89,   104,   139,   438,
     133,   119,   144,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    24,    25,    26,   139,   175,
     202,   203,   204,   205,   206,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   229,   241,   243,   267,
     276,   279,   282,   285,   286,   289,   114,   111,   230,    22,
     115,   112,   113,   120,   364,   121,   390,   122,   139,   425,
     425,   117,   434,   123,   124,    87,   474,    86,   175,   185,
     217,   220,   221,   222,   223,   224,   282,   286,     3,   178,
     200,   202,   178,   178,   178,   178,   139,   178,   139,   148,
     178,   178,   368,   178,   176,    76,    81,    76,     3,     3,
      86,    86,    48,   155,   439,    89,   444,    87,    86,    87,
       8,    11,     8,     9,   280,   152,    90,   103,   104,   104,
      86,    86,    87,   244,    86,    86,    86,    86,    87,    86,
      87,   401,    86,   426,   419,   428,    86,    87,    86,    86,
     475,   182,   128,   201,   269,   310,    87,    89,   370,   393,
     178,   177,   453,   472,   140,     6,    17,    20,    24,    49,
      53,    67,    68,   432,   443,   175,   445,   447,   134,   145,
       8,   104,    47,   156,   277,   283,   287,   231,   101,   365,
     402,   156,    87,    87,   435,    30,    31,    60,    65,    69,
      70,   297,   298,   303,   381,   383,   384,   467,   477,   479,
     485,   178,    87,   178,   208,   211,   215,   216,   290,    87,
      87,   149,   371,   369,    89,   398,   178,   438,   457,   457,
     172,   175,     3,   441,   104,   446,   162,   159,   281,   172,
      74,    75,    76,    77,    78,    79,    80,    84,    85,    95,
      96,   100,   101,   175,   187,   188,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   197,   197,   233,   235,   236,
     245,   372,   404,   420,   429,   162,   299,   304,    19,   175,
     382,    58,   385,   175,   447,   486,   175,   486,   482,   480,
     481,   382,   483,   476,   478,   484,   183,   129,   291,   209,
     270,   311,   162,   175,   156,   399,   394,   454,   473,   173,
      90,   440,     3,   448,   450,    90,     1,    32,    33,   135,
     163,   180,   198,   297,   308,   313,   318,   359,   360,    44,
      45,    46,   146,   160,   161,   163,   320,   203,   195,   195,
     195,   187,    92,    93,    94,    82,    83,    95,    96,    97,
      98,    99,   278,   284,   288,   232,   234,     1,   202,   237,
       8,   175,   217,   222,   223,   224,   250,   267,    51,    52,
      61,    69,    70,   297,   366,   373,   381,   383,   386,   387,
     388,   465,   466,   467,    44,    54,   163,   403,   405,   408,
     411,   162,   159,   436,    31,   175,   204,   282,   286,   342,
     178,     3,     3,     3,    86,    86,    86,   178,    86,    88,
     479,    86,    91,   109,   105,   292,   294,    90,     3,   272,
     275,   234,   150,   175,   156,    87,   370,    90,   442,   103,
     103,   449,   447,   171,    88,   167,   164,   169,   168,   170,
      34,   319,   342,   165,   166,   321,   237,   237,    88,    86,
     102,   189,   190,   191,   192,   192,   193,   193,   194,   194,
     194,   103,   103,   103,    88,   235,   240,   238,   246,   175,
     175,   175,   175,   468,   175,   468,   379,    88,   374,   375,
     376,   377,   378,   380,   178,   178,    88,   406,   407,   421,
     430,    88,   300,   305,   184,   130,   295,   293,   210,   271,
     273,   312,    88,   395,   455,   174,   443,    90,    86,    86,
      86,    86,    86,    86,   314,    86,    86,     3,    86,   208,
     102,   178,   178,   178,   104,     3,     3,    86,    86,    86,
      86,    86,    86,    86,   409,   412,    86,    86,    88,    88,
     342,   212,   215,   186,   187,    88,   197,   294,   211,    88,
      90,    88,    66,    60,   381,   383,   384,   458,   460,   463,
     464,   175,   450,     3,   322,   239,   247,   172,   101,   324,
     324,   301,   306,   213,   296,   274,   396,   175,   447,   447,
     456,   459,   461,   462,   315,   324,    86,    87,   103,   325,
     326,   410,   413,   212,    55,   347,    90,   106,   275,   175,
       3,     3,    88,   460,    86,    86,   101,   333,   323,   248,
     102,    35,   327,   330,    38,   344,   344,   302,   348,   307,
     214,   397,   334,   335,   316,   344,     1,    28,    29,   251,
     253,   257,   259,   331,   102,   328,   345,   344,   101,    62,
     350,   215,    59,   400,   102,    35,    36,    37,   336,   339,
     343,   344,   256,   261,   260,   249,   252,   254,   258,   342,
      90,   101,   349,   351,   175,   102,   337,   340,   317,    86,
     187,    89,    88,   253,   202,   263,   259,   332,   329,   346,
     172,   101,    90,   342,    39,   353,   262,   264,   255,   211,
     330,   172,   102,   352,   338,   341,   354,    89,   211,    86,
     102,   172,   339,   211,   101,   102,   355,    76,   356,   357,
     102,    90,   358,    76
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
//      | connector_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorDeclSeen);
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
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
    break;

  case 34:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 35:

    {
// module  : IDL_MODULE
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
          }
    break;

  case 36:

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

  case 37:

    {
//        '{'
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
          }
    break;

  case 38:

    {
//        definitions
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
          }
    break;

  case 39:

    {
//        '}'
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
            /*
             * Finished with this module - pop it from the scope stack.
             */
            idl_global->scopes ().pop ();
          }
    break;

  case 42:

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

  case 43:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
    break;

  case 44:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
    break;

  case 45:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 46:

    {
// interface_decl : IDL_INTERFACE
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
    break;

  case 47:

    {
//       id
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
         }
    break;

  case 48:

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

  case 49:

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

  case 50:

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

  case 51:

    {
// inheritance_spec : ':' opt_truncatable
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
    break;

  case 52:

    {
//      at_least_one_scoped_name
          (tao_yyvsp[(4) - (4)].nlval)->truncatable ((tao_yyvsp[(2) - (4)].bval));
          (tao_yyval.nlval) = (tao_yyvsp[(4) - (4)].nlval);
        }
    break;

  case 53:

    {
/*      |  EMPTY */
          (tao_yyval.nlval) = 0;
        }
    break;

  case 58:

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

  case 60:

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

  case 61:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
    break;

  case 62:

    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
    break;

  case 63:

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

  case 64:

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

  case 65:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
    break;

  case 66:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
    break;

  case 67:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 68:

    {
// value_header : value_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 69:

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

  case 70:

    {
// value_decl : IDL_VALUETYPE
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
    break;

  case 71:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 72:

    {
// opt_truncatable : IDL_TRUNCATABLE
          (tao_yyval.bval) = true;
        }
    break;

  case 73:

    {
/*      |  EMPTY */
          (tao_yyval.bval) = false;
        }
    break;

  case 74:

    {
// supports_spec : IDL_SUPPORTS at_least_one_scoped_name
          (tao_yyval.nlval) = (tao_yyvsp[(2) - (2)].nlval);
        }
    break;

  case 75:

    {
/*      |    EMPTY */
          (tao_yyval.nlval) = 0;
        }
    break;

  case 76:

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

  case 77:

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

  case 78:

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

  case 84:

    {
// state_member : IDL_PUBLIC
          /* is $0 to member_i */
          (tao_yyval.vival) = AST_Field::vis_PUBLIC;
        }
    break;

  case 85:

    {
//      IDL_PRIVATE
          /* is $0 to member_i */
          (tao_yyval.vival) = AST_Field::vis_PRIVATE;
        }
    break;

  case 88:

    {
// export : type_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
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
//      | typeid_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
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
//      | typeprefix_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
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
//      | const_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
    break;

  case 95:

    {
//      ';'
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
    break;

  case 96:

    {
//      | exception
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
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
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
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
//      | operation
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
    break;

  case 101:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 102:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
    break;

  case 103:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 104:

    {
// at_least_one_scoped_name : scoped_name scoped_names
          ACE_NEW_RETURN ((tao_yyval.nlval),
                          UTL_NameList ((tao_yyvsp[(1) - (2)].idlist),
                                        (tao_yyvsp[(2) - (2)].nlval)),
                          1);
        }
    break;

  case 105:

    {
// scoped_names : scoped_names ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
    break;

  case 106:

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

  case 107:

    {
/*      |  EMPTY */
          (tao_yyval.nlval) = 0;
        }
    break;

  case 108:

    {
// scoped_name : id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN ((tao_yyval.idlist),
                          UTL_IdList ((tao_yyvsp[(1) - (1)].idval),
                                      0),
                          1);
        }
    break;

  case 109:

    {
//      | IDL_SCOPE_DELIMITOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
    break;

  case 110:

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

  case 111:

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

  case 112:

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

  case 113:

    {
// id: IDENTIFIER
          ACE_NEW_RETURN ((tao_yyval.idval),
                          Identifier ((tao_yyvsp[(1) - (1)].strval)),
                          1);
          ACE::strdelete ((tao_yyvsp[(1) - (1)].strval));
          (tao_yyvsp[(1) - (1)].strval) = 0;
        }
    break;

  case 114:

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

  case 115:

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

  case 116:

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

  case 117:

    {
// const_dcl : IDL_CONST
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
    break;

  case 118:

    {
//      const_type
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
    break;

  case 119:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
    break;

  case 120:

    {
//      '='
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
    break;

  case 121:

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

  case 128:

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

  case 129:

    {
//      | wstring_type_spec
          (tao_yyval.etval) = AST_Expression::EV_wstring;
        }
    break;

  case 130:

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

  case 134:

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

  case 136:

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

  case 138:

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

  case 140:

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

  case 141:

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

  case 143:

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

  case 144:

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

  case 146:

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

  case 147:

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

  case 148:

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

  case 150:

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

  case 151:

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

  case 152:

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

  case 153:

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

  case 155:

    {
//      | literal
//      | '(' const_expr ')'
          (tao_yyval.exval) = (tao_yyvsp[(2) - (3)].exval);
        }
    break;

  case 156:

    {
// literal : IDL_INTEGER_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].ival));
        }
    break;

  case 157:

    {
//      | IDL_UINTEGER_LITERAL
          (tao_yyval.exval) =
            idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].uival),
                                             AST_Expression::EV_ulonglong);
        }
    break;

  case 158:

    {
//      | IDL_STRING_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].sval));
          (tao_yyvsp[(1) - (1)].sval)->destroy ();
          delete (tao_yyvsp[(1) - (1)].sval);
          (tao_yyvsp[(1) - (1)].sval) = 0;
        }
    break;

  case 159:

    {
//      | IDL_WSTRING_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].wsval));
        }
    break;

  case 160:

    {
//      | IDL_CHARACTER_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].cval));
        }
    break;

  case 161:

    {
//      | IDL_WCHAR_LITERAL
          ACE_OutputCDR::from_wchar wc ((tao_yyvsp[(1) - (1)].wcval));
          (tao_yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
    break;

  case 162:

    {
//      | IDL_FLOATING_PT_LITERAL
          (tao_yyval.exval) = idl_global->gen ()->create_expr ((tao_yyvsp[(1) - (1)].dval));
        }
    break;

  case 163:

    {
//      | IDL_TRUETOK
          (tao_yyval.exval) = idl_global->gen ()->create_expr (true);
        }
    break;

  case 164:

    {
//      | IDL_FALSETOK
          (tao_yyval.exval) = idl_global->gen ()->create_expr (false);
        }
    break;

  case 165:

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

  case 166:

    {
// type_dcl : IDL_TYPEDEF
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
        }
    break;

  case 167:

    {
//      type_declarator
          (tao_yyval.ival) = 0;
        }
    break;

  case 168:

    {
//      | struct_type
          (tao_yyval.ival) = 0;
        }
    break;

  case 169:

    {
//      | union_type
          (tao_yyval.ival) = 0;
        }
    break;

  case 170:

    {
//      | enum_type
          (tao_yyval.ival) = 0;
        }
    break;

  case 171:

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

  case 172:

    {
//      | constructed_forward_type_spec
          (tao_yyval.ival) = 0;
        }
    break;

  case 173:

    {
// type_declarator : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
    break;

  case 174:

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

  case 177:

    {
// simple_type_spec : base_type_spec
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 179:

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

  case 196:

    {
// at_least_one_declarator : declarator declarators
          ACE_NEW_RETURN ((tao_yyval.dlval),
                          UTL_DeclList ((tao_yyvsp[(1) - (2)].deval),
                                        (tao_yyvsp[(2) - (2)].dlval)),
                          1);
        }
    break;

  case 197:

    {
// declarators : declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
    break;

  case 198:

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

  case 199:

    {
/*      |  EMPTY */
          (tao_yyval.dlval) = 0;
        }
    break;

  case 202:

    {
// at_least_one_simple_declarator : simple_declarator simple_declarators
          ACE_NEW_RETURN ((tao_yyval.dlval),
                          UTL_DeclList ((tao_yyvsp[(1) - (2)].deval),
                                        (tao_yyvsp[(2) - (2)].dlval)),
                          1);
        }
    break;

  case 203:

    {
// simple_declarators : simple_declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
    break;

  case 204:

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

  case 205:

    {
/*      |  EMPTY */
          (tao_yyval.dlval) = 0;
        }
    break;

  case 206:

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

  case 207:

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

  case 210:

    {
// signed_int : IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_long;
        }
    break;

  case 211:

    {
//      | IDL_LONG IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_longlong;
        }
    break;

  case 212:

    {
//      | IDL_SHORT
          (tao_yyval.etval) = AST_Expression::EV_short;
        }
    break;

  case 213:

    {
// unsigned_int : IDL_UNSIGNED IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_ulong;
        }
    break;

  case 214:

    {
//      | IDL_UNSIGNED IDL_LONG IDL_LONG
          (tao_yyval.etval) = AST_Expression::EV_ulonglong;
        }
    break;

  case 215:

    {
//      | IDL_UNSIGNED IDL_SHORT
          (tao_yyval.etval) = AST_Expression::EV_ushort;
        }
    break;

  case 216:

    {
// floating_pt_type : IDL_DOUBLE
          (tao_yyval.etval) = AST_Expression::EV_double;
        }
    break;

  case 217:

    {
//      | IDL_FLOAT
          (tao_yyval.etval) = AST_Expression::EV_float;
        }
    break;

  case 218:

    {
//      | IDL_LONG IDL_DOUBLE
          (tao_yyval.etval) = AST_Expression::EV_longdouble;
        }
    break;

  case 219:

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

  case 220:

    {
// char_type : IDL_CHAR
          (tao_yyval.etval) = AST_Expression::EV_char;
        }
    break;

  case 221:

    {
//      | IDL_WCHAR
          (tao_yyval.etval) = AST_Expression::EV_wchar;
        }
    break;

  case 222:

    {
// octet_type : IDL_OCTET
          (tao_yyval.etval) = AST_Expression::EV_octet;
        }
    break;

  case 223:

    {
// boolean_type : IDL_BOOLEAN
          (tao_yyval.etval) = AST_Expression::EV_bool;
        }
    break;

  case 224:

    {
// any_type : IDL_ANY
          (tao_yyval.etval) = AST_Expression::EV_any;
        }
    break;

  case 225:

    {
// object_type : IDL_OBJECT
          (tao_yyval.etval) = AST_Expression::EV_object;
        }
    break;

  case 226:

    {
// struct_decl : IDL_STRUCT
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
    break;

  case 227:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 228:

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

  case 229:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
    break;

  case 230:

    {
//      at_least_one_member
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
    break;

  case 231:

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

  case 235:

    {
// member  :
          /* is $0 to member_i */
          (tao_yyval.vival) = AST_Field::vis_NA;
        }
    break;

  case 237:

    {
// member_i : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
    break;

  case 238:

    {
//      at_least_one_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
    break;

  case 239:

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

  case 240:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state ());
        }
    break;

  case 241:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 242:

    {
// union_decl : IDL_UNION
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
    break;

  case 243:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 244:

    {
// union_type : union_decl IDL_SWITCH
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
    break;

  case 245:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchOpenParSeen);
        }
    break;

  case 246:

    {
//      switch_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
    break;

  case 247:

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

  case 248:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
    break;

  case 249:

    {
//      at_least_one_case_branch
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
    break;

  case 250:

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

  case 251:

    {
// switch_type_spec : integer_type
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 252:

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

  case 253:

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

  case 254:

    {
//      | boolean_type
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 256:

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

  case 260:

    {
// case_branch : at_least_one_case_label
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
    break;

  case 261:

    {
//      element_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
    break;

  case 262:

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

  case 263:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
    break;

  case 264:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 265:

    {
// at_least_one_case_label : case_label case_labels
          ACE_NEW_RETURN ((tao_yyval.llval),
                          UTL_LabelList ((tao_yyvsp[(1) - (2)].ulval),
                                         (tao_yyvsp[(2) - (2)].llval)),
                          1);
        }
    break;

  case 266:

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

  case 267:

    {
/*      |  EMPTY */
          (tao_yyval.llval) = 0;
        }
    break;

  case 268:

    {
// case_label : IDL_DEFAULT
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
    break;

  case 269:

    {
//      ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (tao_yyval.ulval) = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
    break;

  case 270:

    {
//      | IDL_CASE
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
    break;

  case 271:

    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
    break;

  case 272:

    {
//      const_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (tao_yyval.ulval) = idl_global->gen()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      (tao_yyvsp[(3) - (5)].exval)
                                    );
        }
    break;

  case 273:

    {
// element_spec : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
    break;

  case 274:

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

  case 275:

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

  case 276:

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

  case 277:

    {
// enum_type : IDL_ENUM
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
    break;

  case 278:

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

  case 279:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
    break;

  case 280:

    {
//      at_least_one_enumerator
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
    break;

  case 281:

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

  case 283:

    {
// enumerators : enumerators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
    break;

  case 286:

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

  case 287:

    {
// sequence_type_spec : seq_head ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
    break;

  case 288:

    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
    break;

  case 289:

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

  case 290:

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

  case 291:

    {
// seq_head : IDL_SEQUENCE
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
    break;

  case 292:

    {
//      '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
    break;

  case 293:

    {
//      simple_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          (tao_yyval.dcval) = (tao_yyvsp[(5) - (5)].dcval);
        }
    break;

  case 294:

    {
// string_type_spec : string_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
    break;

  case 295:

    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
    break;

  case 296:

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

  case 297:

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

  case 298:

    {
// string_head : IDL_STRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
    break;

  case 299:

    {
// wstring_type_spec : wstring_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
    break;

  case 300:

    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
    break;

  case 301:

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

  case 302:

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

  case 303:

    {
// wstring_head : IDL_WSTRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
    break;

  case 304:

    {
// array_declarator : id
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
    break;

  case 305:

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

  case 306:

    {
// at_least_one_array_dim : array_dim array_dims
          ACE_NEW_RETURN ((tao_yyval.elval),
                          UTL_ExprList ((tao_yyvsp[(1) - (2)].exval),
                                        (tao_yyvsp[(2) - (2)].elval)),
                          1);
        }
    break;

  case 307:

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

  case 308:

    {
/*      |  EMPTY */
          (tao_yyval.elval) = 0;
        }
    break;

  case 309:

    {
// array_dim : '['
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
    break;

  case 310:

    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
    break;

  case 311:

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

  case 314:

    {
// attribute_readonly : IDL_READONLY
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
    break;

  case 315:

    {
//      IDL_ATTRIBUTE
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

  case 319:

    {
// attribute_readonly : IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
    break;

  case 320:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
    break;

  case 321:

    {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
    break;

  case 322:

    {
//      opt_getraises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
    break;

  case 323:

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

  case 324:

    {
// exception : IDL_EXCEPTION
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
    break;

  case 325:

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

  case 326:

    {
//       '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
    break;

  case 327:

    {
//      members
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
    break;

  case 328:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 329:

    {
// operation : opt_op_attribute op_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
    break;

  case 330:

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

  case 331:

    {
//      parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 332:

    {
//      opt_raises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
    break;

  case 333:

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

  case 334:

    {
// opt_op_attribute : IDL_ONEWAY
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (tao_yyval.ofval) = AST_Operation::OP_oneway;
        }
    break;

  case 335:

    {
//      | IDL_IDEMPOTENT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (tao_yyval.ofval) = AST_Operation::OP_idempotent;
        }
    break;

  case 336:

    {
/*      |  EMPTY */
          (tao_yyval.ofval) = AST_Operation::OP_noflags;
        }
    break;

  case 338:

    {
// op_type_spec : param_type_spec | IDL_VOID
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
    break;

  case 339:

    {
// init_decl : IDL_FACTORY
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
    break;

  case 340:

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

  case 341:

    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 342:

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

  case 343:

    {
// init_parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 344:

    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 345:

    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 346:

    {
//      at_least_one_in_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 348:

    {
// in_parameters : in_parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
    break;

  case 351:

    {
// in_parameter : IDL_IN
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
    break;

  case 352:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
    break;

  case 353:

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

  case 354:

    {
// parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 355:

    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 356:

    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 357:

    {
//      at_least_one_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 359:

    {
// parameters : parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
    break;

  case 362:

    {
// parameter : direction
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
    break;

  case 363:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
    break;

  case 364:

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

  case 365:

    {
// param_type_spec : base_type_spec
          (tao_yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (tao_yyvsp[(1) - (1)].etval)
                                               );
        }
    break;

  case 368:

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

  case 369:

    {
// direction : IDL_IN
          (tao_yyval.dival) = AST_Argument::dir_IN;
        }
    break;

  case 370:

    {
//      | IDL_OUT
          (tao_yyval.dival) = AST_Argument::dir_OUT;
        }
    break;

  case 371:

    {
//      | IDL_INOUT
          (tao_yyval.dival) = AST_Argument::dir_INOUT;
        }
    break;

  case 372:

    {
// opt_raises : IDL_RAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
    break;

  case 373:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
    break;

  case 374:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
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
// opt_getraises : IDL_GETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
    break;

  case 377:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
    break;

  case 378:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
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
// opt_setraises : IDL_SETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
    break;

  case 381:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
    break;

  case 382:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (tao_yyval.nlval) = (tao_yyvsp[(5) - (6)].nlval);
        }
    break;

  case 383:

    {
          (tao_yyval.nlval) = 0;
/*      |  EMPTY */
        }
    break;

  case 384:

    {
// opt_context : IDL_CONTEXT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
    break;

  case 385:

    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
//      '('
        }
    break;

  case 386:

    {
//      at_least_one_string_literal ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (tao_yyval.slval) = (tao_yyvsp[(5) - (6)].slval);
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
// at_least_one_string_literal : IDL_STRING_LITERAL string_literals
          ACE_NEW_RETURN ((tao_yyval.slval),
                          UTL_StrList ((tao_yyvsp[(1) - (2)].sval),
                                       (tao_yyvsp[(2) - (2)].slval)),
                          1);
        }
    break;

  case 389:

    {
// string_literals : string_literals ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
    break;

  case 390:

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

  case 391:

    {
/*      |  EMPTY */
          (tao_yyval.slval) = 0;
        }
    break;

  case 392:

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

  case 393:

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

  case 396:

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

  case 397:

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

  case 398:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
    break;

  case 399:

    {
//      component_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
    break;

  case 400:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 401:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
    break;

  case 402:

    {
//      component_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 403:

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

  case 404:

    {
// component_inheritance_spec : ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
    break;

  case 405:

    {
//      scoped_name
          (tao_yyval.idlist) = (tao_yyvsp[(3) - (3)].idlist);
        }
    break;

  case 406:

    {
/*      |  EMPTY */
          (tao_yyval.idlist) = 0;
        }
    break;

  case 409:

    {
// component_export : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
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
//      | uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
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
//      | emits_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
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
//      | publishes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
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
//      | consumes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
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
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
    break;

  case 420:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 421:

    {
//      | extended_port_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
    break;

  case 422:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 423:

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

  case 424:

    {
// interface_type : scoped_name
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (tao_yyval.idlist) = (tao_yyvsp[(1) - (1)].idlist);
        }
    break;

  case 425:

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

  case 426:

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

  case 427:

    {
// uses_opt_multiple : IDL_USES opt_multiple
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          (tao_yyval.bval) = (tao_yyvsp[(2) - (2)].bval);
        }
    break;

  case 428:

    {
// opt_multiple : IDL_MULTIPLE
          (tao_yyval.bval) = true;
        }
    break;

  case 429:

    {
/*      |  EMPTY */
          (tao_yyval.bval) = false;
        }
    break;

  case 430:

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

  case 431:

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

  case 432:

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

  case 433:

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

  case 434:

    {
//      home_body
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 435:

    {
// home_header : IDL_HOME
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
    break;

  case 436:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
    break;

  case 437:

    {
//      home_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 438:

    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
    break;

  case 439:

    {
//      IDL_MANAGES
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
    break;

  case 440:

    {
//      scoped_name
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
    break;

  case 441:

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

  case 442:

    {
// home_inheritance_spec ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
    break;

  case 443:

    {
//      scoped_name
          (tao_yyval.idlist) = (tao_yyvsp[(3) - (3)].idlist);
        }
    break;

  case 444:

    {
/*      |  EMPTY */
          (tao_yyval.idlist) = 0;
        }
    break;

  case 445:

    {
// primary_key_spec : IDL_PRIMARYKEY scoped_name
          (tao_yyval.idlist) = (tao_yyvsp[(2) - (2)].idlist);
        }
    break;

  case 446:

    {
/*      |  EMPTY */
          (tao_yyval.idlist) = 0;
        }
    break;

  case 447:

    {
// home_body : '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
    break;

  case 448:

    {
//      home_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
    break;

  case 449:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
    break;

  case 453:

    {
// home_export : factory_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
    break;

  case 454:

    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 455:

    {
//      | finder_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
    break;

  case 456:

    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 457:

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

  case 458:

    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 459:

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

  case 460:

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

  case 461:

    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 462:

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

  case 468:

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

  case 469:

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

  case 470:

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

  case 471:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
    break;

  case 472:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
    break;

  case 473:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 474:

    {
// event_abs_header : IDL_ABSTRACT IDL_EVENTTYPE id
          (tao_yyval.idval) = (tao_yyvsp[(3) - (3)].idval);
        }
    break;

  case 475:

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

  case 476:

    {
// event_plain_header : IDL_EVENTTYPE id
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (tao_yyval.idval) = (tao_yyvsp[(2) - (2)].idval);
        }
    break;

  case 477:

    {
// event_rest_of_header : inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 478:

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

  case 479:

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

  case 480:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
    break;

  case 481:

    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
    break;

  case 482:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 483:

    {
// event_header : event_custom_header
          (tao_yyval.idval) = (tao_yyvsp[(1) - (1)].idval);
        }
    break;

  case 484:

    {
// event_header : event_plain_header
          (tao_yyval.idval) = (tao_yyvsp[(1) - (1)].idval);
        }
    break;

  case 485:

    {
// type_classifier : IDL_TYPENAME
          (tao_yyval.ntval) = AST_Decl::NT_type;
        }
    break;

  case 486:

    {
//        IDL_STRUCT
          (tao_yyval.ntval) = AST_Decl::NT_struct;
        }
    break;

  case 487:

    {
//        IDL_EVENTTYPE
          (tao_yyval.ntval) = AST_Decl::NT_eventtype;
        }
    break;

  case 488:

    {
//        IDL_PRIMITIVE
          (tao_yyval.ntval) = AST_Decl::NT_pre_defined;
        }
    break;

  case 489:

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

  case 490:

    {
//        IDL_SEQUENCE
          (tao_yyval.ntval) = AST_Decl::NT_sequence;
        }
    break;

  case 491:

    {
//        IDL_INTERFACE
          (tao_yyval.ntval) = AST_Decl::NT_interface;
        }
    break;

  case 492:

    {
//        IDL_VALUETYPE
          (tao_yyval.ntval) = AST_Decl::NT_valuetype;
        }
    break;

  case 493:

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

  case 494:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
    break;

  case 495:

    {
//      exports - TODO: change to include concatenated identifiers
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
    break;

  case 496:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 497:

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

  case 498:

    {
// at_least_one_template_param : '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplInterfaceSqSeen);
        }
    break;

  case 499:

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

  case 500:

    {
//        '>'
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplInterfaceQsSeen);
          (tao_yyval.plval) = (tao_yyvsp[(4) - (6)].plval);
        }
    break;

  case 501:

    {
// template_params : template_params ','
          // Maybe add a new parse state to set here.
        }
    break;

  case 502:

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

  case 503:

    {
//        /* EMPTY */
          (tao_yyval.plval) = 0;
        }
    break;

  case 504:

    {
// template_param : type_classifier IDENTIFIER

          ACE_NEW_RETURN ((tao_yyval.pival),
                          FE_Utils::T_Param_Info,
                          1);

          (tao_yyval.pival)->type_ = (tao_yyvsp[(1) - (2)].ntval);
          (tao_yyval.pival)->name_ = (tao_yyvsp[(2) - (2)].strval);
        }
    break;

  case 505:

    {
// template_inheritance_spec : ':' at_least_one_template_ref
          (tao_yyval.rlval) = (tao_yyvsp[(2) - (2)].rlval);
        }
    break;

  case 506:

    {
//        /* EMPTY */
          (tao_yyval.rlval) = 0;
        }
    break;

  case 507:

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

  case 508:

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

  case 509:

    {
//        /* EMPTY */
          (tao_yyval.rlval) = 0;
        }
    break;

  case 510:

    {
// template_ref : scoped_name '<' at_least_one_template_param_ref '>'
          ACE_NEW_RETURN ((tao_yyval.trval),
                          FE_Utils::T_Ref_Info ((tao_yyvsp[(1) - (4)].idlist), (tao_yyvsp[(3) - (4)].slval)),
                          1);
        }
    break;

  case 511:

    {
// at_least_one_template_param_ref : template_param_ref template_param_refs
          ACE_NEW_RETURN ((tao_yyval.slval),
                          UTL_StrList ((tao_yyvsp[(1) - (2)].sval),
                                       (tao_yyvsp[(2) - (2)].slval)),
                          1);
        }
    break;

  case 512:

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

  case 513:

    {
//        /* EMPTY */
          (tao_yyval.slval) = 0;
        }
    break;

  case 514:

    {
// template_param_ref : IDENTIFIER
          ACE_NEW_RETURN ((tao_yyval.sval),
                          UTL_String ((tao_yyvsp[(1) - (1)].strval)),
                          1);
        }
    break;

  case 515:

    {
// porttype_decl : IDL_PORTTYPE
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSeen);
        }
    break;

  case 516:

    {
//        IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeIDSeen);
        }
    break;

  case 517:

    {
//        opt_template_params
          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          Identifier id ((tao_yyvsp[(3) - (5)].strval));
          ACE::strdelete ((tao_yyvsp[(3) - (5)].strval));
          (tao_yyvsp[(3) - (5)].strval) = 0;

          UTL_ScopedName sn (&id,
                             0);

          AST_PortType *p =
            idl_global->gen ()->create_porttype (&sn,
                                                 (tao_yyvsp[(5) - (5)].plval));

          (void) s->fe_add_porttype (p);

          // Push it on the scopes stack.
          idl_global->scopes ().push (p);
        }
    break;

  case 518:

    {
//        '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSqSeen);
        }
    break;

  case 519:

    {
//        at_least_one_port_export
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeBodySeen);
        }
    break;

  case 520:

    {
//        '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeQsSeen);

          // Done with this port type - pop it off the scopes stack.
          idl_global->scopes ().pop ();
        }
    break;

  case 521:

    {
// opt_template_params : at_least_one_template_param
          (tao_yyval.plval) = (tao_yyvsp[(1) - (1)].plval);
        }
    break;

  case 522:

    {
//        | /* EMPTY */
          (tao_yyval.plval) = 0;
        }
    break;

  case 523:

    {
// at_least_one_port_export : port_export port_exports
        }
    break;

  case 524:

    {
// port_exports : port_exports port_export
        }
    break;

  case 525:

    {
//        | /* EMPTY */
        }
    break;

  case 526:

    {
// port_export : extended_provides_decl
        }
    break;

  case 527:

    {
//        ';'
        }
    break;

  case 528:

    {
//        | extended_uses_decl
        }
    break;

  case 529:

    {
//        ';'
        }
    break;

  case 530:

    {
// extended_provides_decl : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
    break;

  case 531:

    {
//        | IDL_PROVIDES template_ref IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtProvidesDeclSeen);
          bool so_far_so_good =  true;
          AST_Template_Interface *i = 0;
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].trval)->name_,
                                           true);

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].trval)->name_);
              so_far_so_good = false;
            }
          else
            {
              i = AST_Template_Interface::narrow_from_decl (d);

              if (i == 0)
                {
                  idl_global->err ()->error1 (
                    UTL_Error::EIDL_TMPL_IFACE_EXPECTED,
                    d);
                  so_far_so_good = false;
                }
              else if (! i->match_param_names ((tao_yyvsp[(2) - (3)].trval)->params_))
                {
                  idl_global->err ()->mismatched_template_param ((tao_yyvsp[(2) - (3)].trval)->name_);
                  so_far_so_good = false;
                }
            }

          if (so_far_so_good)
            {
              Identifier id ((tao_yyvsp[(3) - (3)].strval));
              UTL_ScopedName sn (&id, 0);

              AST_Provides *p =
                idl_global->gen ()->create_provides (&sn, i);

              (void) s->fe_add_provides (p);
            }

          (tao_yyvsp[(2) - (3)].trval)->destroy ();
          delete (tao_yyvsp[(2) - (3)].trval);
          (tao_yyvsp[(2) - (3)].trval) = 0;

          ACE::strdelete ((tao_yyvsp[(3) - (3)].strval));
          (tao_yyvsp[(3) - (3)].strval) = 0;
        }
    break;

  case 532:

    {
// extended_uses_decl : uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
    break;

  case 533:

    {
//        | uses_opt_multiple template_ref IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtUsesDeclSeen);
          bool so_far_so_good =  true;
          AST_Template_Interface *i = 0;
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].trval)->name_,
                                           true);

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].trval)->name_);
              so_far_so_good = false;
            }
          else
            {
              i = AST_Template_Interface::narrow_from_decl (d);

              if (i == 0)
                {
                  idl_global->err ()->error1 (
                    UTL_Error::EIDL_TMPL_IFACE_EXPECTED,
                    d);
                  so_far_so_good = false;
                }
              else if (! i->match_param_names ((tao_yyvsp[(2) - (3)].trval)->params_))
                {
                  idl_global->err ()->mismatched_template_param ((tao_yyvsp[(2) - (3)].trval)->name_);
                  so_far_so_good = false;
                }
            }

          if (so_far_so_good)
            {
              Identifier id ((tao_yyvsp[(3) - (3)].strval));
              UTL_ScopedName sn (&id, 0);

              AST_Uses *u =
                idl_global->gen ()->create_uses (&sn, i, (tao_yyvsp[(1) - (3)].bval));

              (void) s->fe_add_uses (u);
            }

          (tao_yyvsp[(2) - (3)].trval)->destroy ();
          delete (tao_yyvsp[(2) - (3)].trval);
          (tao_yyvsp[(2) - (3)].trval) = 0;

          ACE::strdelete ((tao_yyvsp[(3) - (3)].strval));
          (tao_yyvsp[(3) - (3)].strval) = 0;
        }
    break;

  case 536:

    {
// extended_port_decl : IDL_PORT template_inst IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].tival)->name_, true);
          AST_PortType *pt = 0;
          AST_PortType::T_ARGLIST *args = 0;
          bool so_far_so_good = true;

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].tival)->name_);
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
               else
                 {
                   args =
                     pt->match_arg_names ((tao_yyvsp[(2) - (3)].tival)->args_);

                   if (args == 0)
                     {
                       so_far_so_good = false;
                     }
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
                idl_global->gen ()->create_extended_port (&sn,
                                                          pt,
                                                          args);

              (void) s->fe_add_extended_port (ep);
            }

          (tao_yyvsp[(2) - (3)].tival)->destroy ();
          delete (tao_yyvsp[(2) - (3)].tival);
          (tao_yyvsp[(2) - (3)].tival) = 0;
        }
    break;

  case 537:

    {
//        | IDL_MIRRORPORT template_inst IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_MirrorPortDeclSeen);
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(2) - (3)].tival)->name_, true);
          AST_PortType *pt = 0;
          AST_PortType::T_ARGLIST *args = 0;
          bool so_far_so_good = true;

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(2) - (3)].tival)->name_);
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
               else
                 {
                   args =
                     pt->match_arg_names ((tao_yyvsp[(2) - (3)].tival)->args_);

                   if (args == 0)
                     {
                       so_far_so_good = false;
                     }
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
                idl_global->gen ()->create_mirror_port (&sn,
                                                        pt,
                                                        args);

              (void) s->fe_add_mirror_port (mp);
            }

          (tao_yyvsp[(2) - (3)].tival)->destroy ();
          delete (tao_yyvsp[(2) - (3)].tival);
          (tao_yyvsp[(2) - (3)].tival) = 0;
        }
    break;

  case 538:

    {
// non_template_port_decl : IDL_PORT scoped_name IDENTIFIER
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
              else
                {
                  FE_Utils::T_PARAMLIST_INFO *p_list =
                    pt->template_params ();

                  if (p_list != 0 && p_list->size () != 0)
                    {
                      idl_global->err ()->error0 (
                        UTL_Error::EIDL_T_ARG_LENGTH);
                      so_far_so_good = false;
                    }
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
                  pt,
                  0);

              (void) s->fe_add_extended_port (ep);
            }

          (tao_yyvsp[(2) - (3)].idlist)->destroy ();
          delete (tao_yyvsp[(2) - (3)].idlist);
          (tao_yyvsp[(2) - (3)].idlist) = 0;
        }
    break;

  case 539:

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
               else
                 {
                   FE_Utils::T_PARAMLIST_INFO *p_list =
                     pt->template_params ();

                   if (p_list != 0 && p_list->size () != 0)
                     {
                       idl_global->err ()->error0 (
                         UTL_Error::EIDL_T_ARG_LENGTH);
                       so_far_so_good = false;
                     }
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
                  pt,
                  0);

              (void) s->fe_add_mirror_port (mp);
            }

          (tao_yyvsp[(2) - (3)].idlist)->destroy ();
          delete (tao_yyvsp[(2) - (3)].idlist);
          (tao_yyvsp[(2) - (3)].idlist) = 0;
        }
    break;

  case 540:

    {
// template_inst : scoped_name '<' at_least_one_scoped_name '>'
          ACE_NEW_RETURN ((tao_yyval.tival),
                          FE_Utils::T_Inst_Info ((tao_yyvsp[(1) - (4)].idlist),
                                                 (tao_yyvsp[(3) - (4)].nlval)),
                          1);
        }
    break;

  case 542:

    {
// connector_header : IDL_CONNECTOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSeen);
        }
    break;

  case 543:

    {
//        IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorIDSeen);
        }
    break;

  case 544:

    {
//        opt_template_params
        }
    break;

  case 545:

    {
//        component_inheritance_spec
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Connector *parent = 0;
          bool so_far_so_good = true;

          Identifier id ((tao_yyvsp[(3) - (7)].strval));
          ACE::strdelete ((tao_yyvsp[(3) - (7)].strval));
          (tao_yyvsp[(3) - (7)].strval) = 0;

          UTL_ScopedName sn (&id, 0);

          if ((tao_yyvsp[(7) - (7)].idlist) != 0)
            {
              AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(7) - (7)].idlist), true);

              if (d == 0)
                {
                  idl_global->err ()->lookup_error ((tao_yyvsp[(7) - (7)].idlist));
                  so_far_so_good = false;
                }

              parent =
                AST_Connector::narrow_from_decl (d);

              if (parent == 0)
                {
                  idl_global->err ()->error1 (UTL_Error::EIDL_CONNECTOR_EXPECTED,
                                              d);
                  so_far_so_good = false;
                }

              (tao_yyvsp[(7) - (7)].idlist)->destroy ();
              delete (tao_yyvsp[(7) - (7)].idlist);
              (tao_yyvsp[(7) - (7)].idlist) = 0;
            }

          if (so_far_so_good)
            {
              AST_Connector *c =
                idl_global->gen ()->create_connector (&sn,
                                                      parent,
                                                      (tao_yyvsp[(5) - (7)].plval));

              (void) s->fe_add_connector (c);

              // Push it on the scopes stack.
              idl_global->scopes ().push (c);
           }
        }
    break;

  case 546:

    {
// connector_body " '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSqSeen);
        }
    break;

  case 547:

    {
//        at_least_one_connector_export
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorBodySeen);
        }
    break;

  case 548:

    {
//        '}
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorQsSeen);

          // Done with this connector - pop it off the scope stack.
          idl_global->scopes ().pop ();
        }
    break;

  case 552:

    {
// connector_export : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
    break;

  case 553:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 554:

    {
//        | uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
    break;

  case 555:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 556:

    {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
    break;

  case 557:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 558:

    {
//      | non_template_port_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
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
//      | template_extended_port_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
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
// template_extended_port_decl : IDL_PORT template_ref_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          if ((tao_yyvsp[(2) - (2)].ptval) != 0)
            {
              Identifier id ((tao_yyvsp[(2) - (2)].ptval)->name_.c_str ());
              UTL_ScopedName sn (&id, 0);

              AST_Tmpl_Port *pt =
                idl_global->gen ()->create_tmpl_port (
                  &sn,
                  (tao_yyvsp[(2) - (2)].ptval)->type_);

              (void) s->fe_add_tmpl_port (pt);

              delete (tao_yyvsp[(2) - (2)].ptval);
              (tao_yyvsp[(2) - (2)].ptval) = 0;
            }
        }
    break;

  case 563:

    {
//      | IDL_MIRRORPORT template_ref_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          if ((tao_yyvsp[(2) - (2)].ptval) != 0)
            {
              Identifier id ((tao_yyvsp[(2) - (2)].ptval)->name_.c_str ());
              UTL_ScopedName sn (&id, 0);

              AST_Tmpl_Mirror_Port *pt =
                idl_global->gen ()->create_tmpl_mirror_port (
                  &sn,
                  (tao_yyvsp[(2) - (2)].ptval)->type_);

              (void) s->fe_add_tmpl_mirror_port (pt);

              delete (tao_yyvsp[(2) - (2)].ptval);
              (tao_yyvsp[(2) - (2)].ptval) = 0;
            }
        }
    break;

  case 564:

    {
// template_ref_decl : template_ref IDENTIFIER
          (tao_yyval.ptval) = 0;
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name ((tao_yyvsp[(1) - (2)].trval)->name_,
                                           true);

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((tao_yyvsp[(1) - (2)].trval)->name_);
            }
          else
            {
              AST_PortType *pt = AST_PortType::narrow_from_decl (d);

              if (pt == 0)
                {
                  idl_global->err ()->error1 (UTL_Error::EIDL_PORTTYPE_EXPECTED,
                                              d);
                }
              else
                {
                  ACE_NEW_RETURN ((tao_yyval.ptval),
                                  FE_Utils::T_Port_Info ((tao_yyvsp[(2) - (2)].strval),
                                                         pt),
                                  1);
                }
            }

          (tao_yyvsp[(1) - (2)].trval)->destroy ();
          delete (tao_yyvsp[(1) - (2)].trval);
          (tao_yyvsp[(1) - (2)].trval) = 0;

          ACE::strdelete ((tao_yyvsp[(2) - (2)].strval));
          (tao_yyvsp[(2) - (2)].strval) = 0;
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

