/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         tao_yyparse
#define yylex           tao_yylex
#define yyerror         tao_yyerror
#define yydebug         tao_yydebug
#define yynerrs         tao_yynerrs

#define yylval          tao_yylval
#define yychar          tao_yychar

/* Copy the first part of user declarations.  */
#line 71 "fe/idl.ypp" /* yacc.c:339  */

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

#include "ast_argument.h"
#include "ast_array.h"
#include "ast_attribute.h"
#include "ast_field.h"
#include "ast_fixed.h"
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
#include "ast_finder.h"
#include "ast_exception.h"
#include "ast_param_holder.h"
#include "ast_visitor_tmpl_module_inst.h"
#include "ast_visitor_tmpl_module_ref.h"
#include "ast_visitor_context.h"

#include "fe_declarator.h"
#include "fe_interface_header.h"
#include "fe_obv_header.h"
#include "fe_component_header.h"
#include "fe_home_header.h"
#include "fe_utils.h"

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
#define YYDEBUG 1

#line 151 "fe/idl.tab.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "idl.tab.hpp".  */
#ifndef YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED
# define YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int tao_yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
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
    IDL_FIXED_PT_LITERAL = 333,
    IDL_TRUETOK = 334,
    IDL_FALSETOK = 335,
    IDL_SCOPE_DELIMITOR = 336,
    IDL_LEFT_SHIFT = 337,
    IDL_RIGHT_SHIFT = 338,
    IDL_WCHAR_LITERAL = 339,
    IDL_WSTRING_LITERAL = 340
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 152 "fe/idl.ypp" /* yacc.c:355  */

  AST_Decl                      *dcval;         /* Decl value           */
  UTL_StrList                   *slval;         /* String list          */
  UTL_NameList                  *nlval;         /* Name list            */
  UTL_ExprList                  *elval;         /* Expression list      */
  UTL_LabelList                 *llval;         /* Label list           */
  UTL_DeclList                  *dlval;         /* Declaration list     */
  FE_InterfaceHeader            *ihval;         /* Interface header     */
  FE_OBVHeader                  *vhval;         /* Valuetype header     */
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
  ACE_CDR::LongLong             ival;           /* Long Long value      */
  ACE_CDR::ULongLong            uival;          /* Unsigned long long   */
  ACE_CDR::Double               dval;           /* Double value         */
  ACE_CDR::Float                fval;           /* Float value          */
  ACE_CDR::Char                 cval;           /* Char value           */
  ACE_CDR::WChar                wcval;          /* WChar value          */
  ACE_CDR::Fixed                fixval;         /* Fixed point value    */
  UTL_String                    *sval;          /* String value         */
  char                          *wsval;         /* WString value        */
  char                          *strval;        /* char * value         */
  Identifier                    *idval;         /* Identifier           */
  UTL_IdList                    *idlist;        /* Identifier list      */
  AST_Decl::NodeType            ntval;          /* Node type value      */
  FE_Utils::T_Param_Info        *pival;         /* Template interface param */
  FE_Utils::T_PARAMLIST_INFO    *plval;         /* List of template params */
  FE_Utils::T_ARGLIST           *alval;         /* List of template args */

#line 315 "fe/idl.tab.cpp" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE tao_yylval;

int tao_yyparse (void);

#endif /* !YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 330 "fe/idl.tab.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1285

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  108
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  389
/* YYNRULES -- Number of rules.  */
#define YYNRULES  578
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  848

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   340

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   101,    96,     2,
     103,   104,    99,    97,    92,    98,     2,   100,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    91,    86,
      89,    93,    90,     2,   105,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   106,     2,   107,    95,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    87,    94,    88,   102,     2,     2,     2,
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

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   360,   360,   363,   364,   368,   371,   374,   380,   392,
     391,   402,   401,   412,   411,   422,   421,   432,   431,   442,
     441,   452,   451,   462,   461,   472,   471,   482,   481,   492,
     491,   502,   501,   512,   511,   522,   521,   536,   535,   548,
     587,   592,   547,   609,   617,   632,   642,   673,   677,   616,
     701,   705,   706,   710,   711,   716,   722,   715,   810,   816,
     809,   887,   888,   893,   932,   937,   892,   955,   954,   968,
    1006,  1037,  1071,  1070,  1082,  1089,  1090,  1091,  1092,  1096,
    1107,  1112,  1156,  1161,  1111,  1190,  1234,  1239,  1188,  1258,
    1256,  1298,  1297,  1311,  1317,  1324,  1331,  1338,  1364,  1391,
    1456,  1457,  1461,  1462,  1463,  1469,  1468,  1479,  1478,  1491,
    1492,  1497,  1496,  1507,  1506,  1517,  1516,  1527,  1526,  1537,
    1536,  1547,  1546,  1557,  1556,  1567,  1566,  1580,  1593,  1591,
    1619,  1626,  1637,  1636,  1664,  1662,  1689,  1700,  1713,  1759,
    1787,  1818,  1823,  1828,  1833,  1817,  1904,  1905,  1906,  1907,
    1908,  1909,  1910,  1922,  1927,  1996,  1998,  2000,  2001,  2015,
    2016,  2030,  2031,  2044,  2045,  2055,  2068,  2069,  2079,  2092,
    2093,  2103,  2113,  2126,  2127,  2137,  2147,  2160,  2211,  2212,
    2221,  2226,  2232,  2240,  2245,  2250,  2256,  2260,  2265,  2270,
    2278,  2352,  2353,  2357,  2358,  2362,  2363,  2367,  2371,  2372,
    2376,  2381,  2380,  2391,  2396,  2401,  2406,  2433,  2442,  2441,
    2513,  2514,  2518,  2526,  2527,  2555,  2556,  2557,  2558,  2559,
    2560,  2561,  2562,  2566,  2567,  2568,  2569,  2573,  2574,  2575,
    2579,  2580,  2584,  2597,  2595,  2623,  2630,  2631,  2635,  2648,
    2646,  2674,  2681,  2698,  2717,  2718,  2722,  2727,  2732,  2740,
    2745,  2750,  2758,  2763,  2768,  2776,  2784,  2789,  2797,  2805,
    2813,  2821,  2830,  2829,  2845,  2879,  2884,  2844,  2903,  2906,
    2907,  2911,  2911,  2922,  2927,  2920,  2990,  2989,  3004,  3003,
    3018,  3023,  3059,  3064,  3121,  3126,  3017,  3150,  3158,  3172,
    3182,  3190,  3191,  3299,  3302,  3303,  3308,  3313,  3307,  3349,
    3348,  3362,  3373,  3393,  3401,  3400,  3416,  3421,  3415,  3438,
    3437,  3490,  3514,  3539,  3544,  3577,  3582,  3538,  3608,  3613,
    3611,  3618,  3622,  3659,  3664,  3657,  3745,  3806,  3816,  3805,
    3829,  3839,  3844,  3837,  3891,  3917,  3927,  3932,  3925,  3968,
    3993,  4002,  4001,  4043,  4054,  4074,  4082,  4087,  4081,  4149,
    4150,  4155,  4160,  4165,  4170,  4154,  4239,  4244,  4249,  4254,
    4238,  4332,  4337,  4367,  4372,  4331,  4390,  4395,  4460,  4465,
    4388,  4502,  4508,  4515,  4522,  4523,  4535,  4541,  4583,  4534,
    4605,  4604,  4615,  4614,  4627,  4632,  4630,  4637,  4642,  4647,
    4641,  4688,  4687,  4698,  4697,  4710,  4715,  4713,  4720,  4725,
    4730,  4724,  4777,  4785,  4786,  4787,  4897,  4902,  4907,  4916,
    4921,  4915,  4933,  4941,  4946,  4940,  4958,  4966,  4971,  4965,
    4983,  4991,  4996,  4990,  5008,  5015,  5028,  5026,  5052,  5059,
    5088,  5126,  5127,  5131,  5161,  5201,  5206,  5160,  5225,  5230,
    5223,  5273,  5272,  5283,  5290,  5291,  5296,  5295,  5306,  5305,
    5316,  5315,  5326,  5325,  5336,  5335,  5346,  5345,  5356,  5355,
    5367,  5458,  5465,  5491,  5598,  5608,  5614,  5620,  5693,  5766,
    5841,  5840,  5890,  5895,  5900,  5905,  5910,  5915,  5889,  5970,
    5969,  5980,  5987,  5994,  6002,  6007,  6001,  6019,  6020,  6024,
    6026,  6025,  6036,  6035,  6050,  6074,  6048,  6102,  6130,  6100,
    6156,  6157,  6158,  6162,  6163,  6167,  6195,  6226,  6271,  6276,
    6224,  6293,  6303,  6322,  6334,  6333,  6373,  6423,  6428,  6371,
    6445,  6450,  6458,  6463,  6468,  6473,  6478,  6483,  6488,  6493,
    6498,  6503,  6512,  6547,  6546,  6568,  6575,  6601,  6619,  6630,
    6650,  6657,  6668,  6673,  6692,  6697,  6667,  6712,  6719,  6724,
    6731,  6730,  6739,  6738,  6747,  6746,  6758,  6828,  6879,  6895,
    6909,  6916,  6976,  6981,  6986,  6980,  7047,  7052,  7046,  7067,
    7068,  7073,  7072,  7083,  7082,  7093,  7092,  7103,  7102
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
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
  "IDL_FLOATING_PT_LITERAL", "IDL_FIXED_PT_LITERAL", "IDL_TRUETOK",
  "IDL_FALSETOK", "IDL_SCOPE_DELIMITOR", "IDL_LEFT_SHIFT",
  "IDL_RIGHT_SHIFT", "IDL_WCHAR_LITERAL", "IDL_WSTRING_LITERAL", "';'",
  "'{'", "'}'", "'<'", "'>'", "':'", "','", "'='", "'|'", "'^'", "'&'",
  "'+'", "'-'", "'*'", "'/'", "'%'", "'~'", "'('", "')'", "'@'", "'['",
  "']'", "$accept", "start", "definitions", "definition",
  "at_least_one_definition", "fixed_definition", "$@1", "$@2", "$@3",
  "$@4", "$@5", "$@6", "$@7", "$@8", "$@9", "$@10", "$@11", "$@12", "$@13",
  "$@14", "module_header", "$@15", "module", "$@16", "$@17", "$@18",
  "template_module_header", "template_module", "$@19", "$@20", "$@21",
  "$@22", "$@23", "at_least_one_tpl_definition", "tpl_definitions",
  "tpl_definition", "template_module_ref", "$@24", "$@25",
  "template_module_inst", "$@26", "$@27", "interface_def", "interface",
  "$@28", "$@29", "$@30", "interface_decl", "$@31", "interface_header",
  "inheritance_spec", "$@32", "value_def", "valuetype",
  "value_concrete_decl", "$@33", "$@34", "$@35", "value_abs_decl", "$@36",
  "$@37", "$@38", "value_header", "$@39", "value_decl", "$@40",
  "opt_truncatable", "supports_spec", "value_forward_decl",
  "value_box_decl", "value_elements", "value_element", "state_member",
  "@41", "@42", "exports", "export", "$@43", "$@44", "$@45", "$@46",
  "$@47", "$@48", "$@49", "$@50", "at_least_one_scoped_name",
  "scoped_names", "$@51", "scoped_name", "$@52", "$@53", "id",
  "defining_id", "interface_forward", "const_dcl", "$@54", "$@55", "$@56",
  "$@57", "const_type", "expression", "const_expr", "or_expr", "xor_expr",
  "and_expr", "shift_expr", "add_expr", "mult_expr", "unary_expr",
  "primary_expr", "literal", "positive_int_expr", "annotations",
  "annotation_appl", "annotation_appl_params",
  "at_least_one_annotation_appl_param", "annotation_appl_named_params",
  "annotation_appl_param", "type_dcl", "$@58", "type_declarator", "$@59",
  "type_spec", "simple_type_spec", "base_type_spec", "template_type_spec",
  "constructed_type_spec", "constructed_forward_type_spec",
  "at_least_one_declarator", "declarators", "$@60", "declarator",
  "at_least_one_simple_declarator", "simple_declarators", "$@61",
  "simple_declarator", "complex_declarator", "integer_type", "signed_int",
  "unsigned_int", "floating_pt_type", "fixed_type", "char_type",
  "octet_type", "boolean_type", "any_type", "object_type", "struct_decl",
  "$@62", "struct_type", "$@63", "$@64", "$@65", "at_least_one_member",
  "members", "member", "@66", "member_i", "$@67", "$@68", "$@69",
  "union_decl", "$@70", "union_type", "$@71", "$@72", "$@73", "$@74",
  "$@75", "$@76", "switch_type_spec", "at_least_one_case_branch",
  "case_branches", "case_branch", "$@77", "$@78", "$@79",
  "at_least_one_case_label", "case_labels", "case_label", "$@80", "$@81",
  "$@82", "element_spec", "$@83", "struct_forward_type",
  "union_forward_type", "enum_type", "$@84", "$@85", "$@86", "$@87",
  "at_least_one_enumerator", "enumerators", "$@88", "enumerator",
  "sequence_type_spec", "$@89", "$@90", "seq_head", "$@91", "$@92",
  "fixed_type_spec", "string_type_spec", "$@93", "$@94", "string_head",
  "wstring_type_spec", "$@95", "$@96", "wstring_head", "array_declarator",
  "$@97", "at_least_one_array_dim", "array_dims", "array_dim", "$@98",
  "$@99", "attribute", "attribute_readonly", "$@100", "$@101", "$@102",
  "$@103", "attribute_readwrite", "$@104", "$@105", "$@106", "$@107",
  "exception", "$@108", "$@109", "$@110", "$@111", "operation", "$@112",
  "$@113", "$@114", "$@115", "opt_op_attribute", "op_type_spec",
  "init_decl", "$@116", "$@117", "$@118", "init_parameter_list", "$@119",
  "$@120", "at_least_one_in_parameter", "in_parameters", "$@121",
  "in_parameter", "$@122", "$@123", "parameter_list", "$@124", "$@125",
  "at_least_one_parameter", "parameters", "$@126", "parameter", "$@127",
  "$@128", "param_type_spec", "direction", "opt_raises", "$@129", "$@130",
  "opt_getraises", "$@131", "$@132", "opt_setraises", "$@133", "$@134",
  "opt_context", "$@135", "$@136", "at_least_one_string_literal",
  "string_literals", "$@137", "typeid_dcl", "typeprefix_dcl", "component",
  "component_forward_decl", "component_decl", "$@138", "$@139", "$@140",
  "component_header", "$@141", "$@142", "component_inheritance_spec",
  "$@143", "component_exports", "component_export", "$@144", "$@145",
  "$@146", "$@147", "$@148", "$@149", "$@150", "provides_decl",
  "interface_type", "uses_decl", "uses_opt_multiple", "opt_multiple",
  "emits_decl", "publishes_decl", "consumes_decl", "home_decl", "$@151",
  "home_header", "$@152", "$@153", "$@154", "$@155", "$@156", "$@157",
  "home_inheritance_spec", "$@158", "primary_key_spec", "home_body",
  "$@159", "$@160", "home_exports", "home_export", "$@161", "$@162",
  "factory_decl", "$@163", "$@164", "finder_decl", "$@165", "$@166",
  "event", "event_forward_decl", "event_concrete_forward_decl",
  "event_abs_forward_decl", "event_abs_decl", "$@167", "$@168", "$@169",
  "event_abs_header", "event_custom_header", "event_plain_header",
  "event_rest_of_header", "$@170", "event_decl", "$@171", "$@172", "$@173",
  "event_header", "formal_parameter_type", "at_least_one_formal_parameter",
  "formal_parameters", "$@174", "formal_parameter",
  "at_least_one_formal_parameter_name", "formal_parameter_names",
  "formal_parameter_name", "porttype_decl", "$@175", "$@176", "$@177",
  "$@178", "at_least_one_port_export", "port_exports", "port_export",
  "$@179", "$@180", "$@181", "extended_port_decl",
  "at_least_one_actual_parameter", "actual_parameters", "actual_parameter",
  "connector_decl", "connector_header", "$@182", "$@183", "connector_body",
  "$@184", "$@185", "connector_exports", "connector_export", "$@186",
  "$@187", "$@188", "$@189", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,    59,   123,   125,    60,
      62,    58,    44,    61,   124,    94,    38,    43,    45,    42,
      47,    37,   126,    40,    41,    64,    91,    93
};
# endif

#define YYPACT_NINF -597

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-597)))

#define YYTABLE_NINF -519

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -597,    49,    56,  -597,   187,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,    77,   193,    62,  -597,  -597,
    -597,  -597,    42,    42,  -597,  -597,    42,  -597,  -597,    29,
    -597,   308,    17,    44,  -597,  -597,     2,  -597,  -597,  -597,
    -597,  -597,  -597,   564,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,    78,  -597,   181,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,    98,  -597,  -597,  -597,    98,  -597,  -597,   100,
     132,   923,    42,  -597,   598,  -597,  -597,  -597,  -597,  -597,
      47,  -597,   104,  -597,   114,  -597,   115,  -597,  -597,    98,
    -597,   145,   225,  -597,  -597,  -597,    -2,  -597,    99,  -597,
    -597,     9,  -597,   232,   235,   776,  -597,  -597,  -597,   238,
     278,  -597,   240,   247,   250,   171,  -597,   236,  -597,  -597,
    -597,  -597,  -597,  -597,   245,  -597,  -597,  -597,  -597,  -597,
    -597,   258,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,   181,
    -597,  -597,  -597,   149,  -597,  -597,   251,  -597,   253,   262,
     263,   257,  -597,   270,   273,   274,   276,   271,   277,   283,
     285,  -597,  -597,  -597,   287,   288,  -597,  -597,  -597,  -597,
     258,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
     258,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
     290,  -597,   291,  -597,  -597,   292,  -597,   376,  -597,  -597,
    -597,    52,    58,   342,  -597,  -597,   923,  -597,  -597,  -597,
    -597,   295,  -597,  -597,  -597,  -597,   378,  -597,  -597,   220,
     296,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,   377,
    -597,   402,   299,   344,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,   289,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,   344,   310,   313,  -597,  -597,  -597,  -597,  -597,   315,
     317,  -597,  -597,  -597,   316,  -597,   376,  -597,  -597,   318,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
     451,   451,   451,   402,   258,  -597,   312,   314,   328,   165,
     167,   116,  -597,  -597,  -597,   306,  -597,  -597,  -597,  -597,
     409,  -597,   324,   336,  -597,  -597,  -597,  -597,    42,  -597,
    -597,  -597,  -597,   337,  -597,    42,  -597,   402,   402,   402,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,   222,  -597,   325,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,    42,   344,
    -597,  -597,  -597,   343,   292,   402,  -597,  -597,  -597,   329,
     402,   402,   402,   402,   402,   402,   402,   402,   402,   402,
    -597,   340,  -597,  -597,   345,  -597,  -597,   346,  -597,  -597,
     258,  1047,   801,   402,   650,  -597,  -597,  -597,  -597,  -597,
    -597,   130,   548,   890,   965,  -597,  -597,  -597,  -597,    38,
     379,    42,    42,  -597,  -597,  -597,  -597,  -597,    38,  -597,
     348,  -597,   341,   335,   350,  -597,  -597,  1082,   258,  -597,
      42,   344,  -597,  -597,  -597,  -597,   314,   328,   165,   167,
     167,   116,   116,  -597,  -597,  -597,   452,   187,   368,   454,
     371,   776,  -597,  -597,   367,  -597,  -597,  -597,   380,  -597,
    -597,  -597,  -597,  -597,  -597,   702,  -597,  -597,  -597,  -597,
    -597,   381,  -597,  -597,  -597,   384,   374,  -597,   398,   399,
     400,   383,   403,  -597,   260,  -597,   459,   258,  -597,  -597,
    -597,  -597,  -597,  -597,    42,    42,    42,  -597,   404,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,   405,
    -597,  -597,  -597,  1164,   883,   466,   841,  -597,   258,   376,
    -597,  -597,    51,    92,   412,   415,   416,   376,   417,  -597,
    -597,  -597,  -597,  -597,  -597,    60,  -597,  -597,   418,   419,
     258,  -597,   202,   318,  -597,   420,  -597,  -597,  -597,  -597,
     220,  -597,   423,  -597,   424,   425,   426,   428,   429,  -597,
     258,  -597,  -597,  -597,  -597,  -597,   430,   431,  -597,   130,
     130,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,   432,
    -597,   433,    96,    96,    96,   434,  -597,   435,   436,   437,
     447,   448,   453,  -597,  -597,  -597,   455,   456,   450,   457,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
     402,   402,   335,  -597,  -597,   458,   427,   460,  -597,   474,
    -597,  -597,  -597,  -597,  -597,  1213,  -597,    42,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,    61,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,   440,   440,  -597,  -597,  -597,  -597,   841,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,   461,
     463,   464,   465,   202,    42,  -597,  -597,  -597,  -597,   469,
     258,    63,  -597,  -597,   473,   509,  -597,  -597,  -597,  -597,
    -597,   445,  -597,    42,  -597,  -597,  -597,  -597,  -597,   258,
     477,   478,  -597,  -597,   440,   500,  -597,   487,   559,   557,
     557,  -597,   541,   505,  -597,  -597,   258,   510,  -597,    64,
     495,  -597,  -597,   184,  -597,  -597,   496,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,   543,   600,  -597,   516,  -597,
     557,  -597,  -597,  -597,  -597,  -597,  -597,  -597,   841,  -597,
     512,   522,   557,   523,   565,  -597,    42,  -597,  -597,   538,
    -597,   526,   189,   557,  -597,   545,   402,   542,   544,    93,
    -597,    73,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
     258,  -597,   550,  -597,  -597,  -597,  -597,   531,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,   260,    97,  -597,   559,
      42,    42,   533,  -597,   600,  -597,   554,   841,   601,   556,
    -597,  -597,  -597,  -597,  -597,   547,   552,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,   562,  -597,  -597,    42,
     189,  -597,   551,  -597,  -597,   566,  -597,  -597,  -597,  -597,
     597,  -597,   569,   585,  -597,  -597,   603,  -597
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,   192,     1,     0,    35,   141,    37,    67,   201,
     262,   278,   313,   361,   192,     0,     0,     0,    91,   192,
     192,   472,     0,     0,   542,   563,     0,     3,     5,    39,
      21,    58,     0,     0,    19,    61,    74,    63,    23,    75,
      80,    76,    81,    74,    77,    78,    62,    15,   191,     9,
     207,   264,   203,   312,   204,   230,   231,   205,    17,    11,
      13,    25,   432,   431,   434,    27,   470,    29,   502,   504,
     503,   501,    74,   520,   521,   500,    74,    31,    33,     0,
       0,     0,     0,   192,     0,   192,   192,   192,   192,   242,
       0,   206,    74,   192,    74,    85,    74,   192,    79,    74,
     192,   438,   513,   192,   136,   132,     0,   131,     0,   192,
     192,   194,    43,     0,     0,     0,   192,     6,     7,     0,
      94,    69,     0,     0,     0,   246,   248,     0,   252,   253,
     256,   257,   258,   259,   255,   260,   261,   327,   335,   340,
      89,   214,    99,   210,   212,   213,   211,   215,   244,   245,
     216,   220,   217,   219,   218,   221,   222,   264,   227,     0,
     228,   229,   223,     0,   226,   224,   334,   225,   339,     0,
       0,     0,   280,     0,     0,     0,     0,     0,     0,     0,
       0,   514,   507,   516,     0,     0,   566,   562,    36,   255,
     154,   142,   146,   150,   151,   147,   148,   149,   152,   153,
      38,    68,   202,   208,   263,   279,   314,   362,   137,    70,
     511,    71,     0,   512,    92,   443,   473,     0,   429,   134,
     430,     0,     0,     0,    40,    22,     0,   527,   523,   524,
     529,   526,   530,   528,   525,   522,     0,    45,   535,     0,
       0,    20,    93,    72,    64,    24,    82,   247,   254,   249,
     251,     0,     0,    96,   326,   323,   331,   336,    16,    10,
     265,     0,    18,    12,    14,    26,   435,    28,   484,   471,
      30,    96,     0,     0,    32,    34,   570,   192,   192,     0,
       0,    86,   441,   439,   481,   133,     0,   543,   564,   136,
     180,   181,   182,   184,   187,   186,   188,   189,   185,   183,
       0,     0,     0,     0,   177,   195,   156,   157,   159,   161,
     163,   166,   169,   173,   178,     0,   196,   199,     4,   531,
       0,   536,     0,   532,   561,   155,   560,    59,     0,   110,
     101,   250,   190,     0,   328,     0,    90,     0,     0,     0,
     271,   281,   445,   488,   515,   508,   517,   567,   143,   242,
     209,   235,   236,   237,   243,   315,   363,   110,     0,    96,
     479,   474,   135,     0,   443,     0,   174,   175,   176,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     193,   197,   192,    41,     0,    46,   533,   558,   192,    73,
     130,     0,     0,     0,     0,    95,   324,   332,   337,   266,
     270,     0,     0,   436,     0,   110,   101,   351,   356,     0,
     466,     0,     0,   575,   349,   350,   571,   573,     0,   577,
       0,   569,     0,     0,   232,   192,   270,     0,   442,   440,
       0,    96,   544,   565,   200,   179,   158,   160,   162,   164,
     165,   167,   168,   170,   171,   172,     0,     0,     0,     0,
       0,     0,   192,    60,   127,   125,   371,   372,     0,   109,
     117,   111,   121,   119,   123,     0,   113,   115,   376,   107,
     105,     0,   100,   102,   103,     0,     0,   329,     0,     0,
       0,     0,   271,   276,     0,   272,   246,   292,   287,   288,
     289,   290,   282,   291,     0,     0,     0,   456,     0,   444,
     446,   448,   450,   452,   454,   458,   192,   192,   489,     0,
     487,   490,   492,     0,     0,     0,     0,   462,   461,     0,
     465,   464,     0,     0,     0,     0,     0,     0,     0,   568,
     144,   346,   342,   345,   233,     0,   316,   321,   271,     0,
     480,   475,     0,     0,   198,     3,    42,   537,    47,   534,
       0,   128,     0,    66,     0,     0,     0,     0,     0,   375,
     405,   402,   403,   404,   366,   374,     0,     0,   192,     0,
       0,    84,   104,   330,   325,   333,   338,   267,   269,     0,
     273,     0,     0,     0,     0,     0,   437,     0,     0,     0,
       0,     0,     0,   494,   497,   486,     0,     0,     0,     0,
     352,   357,   460,   556,   557,   576,   572,   574,   463,   578,
       0,     0,   343,   192,   322,     0,   318,     0,    88,     0,
     554,   550,   552,   545,   549,     0,   559,     0,   126,   118,
     112,   122,   120,   124,   192,   114,   116,     0,   108,   106,
     277,   192,   283,   469,   467,   468,   457,   447,   449,   451,
     453,   455,   459,     0,     0,   491,   493,   510,   519,     0,
     192,   145,   347,   344,   234,   317,   319,   365,   476,     0,
       0,     0,     0,   547,     0,    53,    39,    48,    52,     0,
     129,     0,   377,   274,     0,   380,   495,   498,   353,   358,
     241,     0,   192,     0,   555,   551,   553,   546,   548,    55,
       0,   192,    54,   367,     0,     0,   284,     0,     0,   412,
     412,   192,   416,   238,   348,   320,   477,     0,    49,     0,
       0,   378,   275,     0,   381,   388,     0,   387,   409,   496,
     499,   354,   413,   359,   239,   483,     0,    51,   393,   368,
     412,   299,   306,   304,   285,   295,   296,   303,     0,   383,
     384,     0,   412,     0,   420,   192,     0,   478,   541,     0,
     540,     0,     0,   412,   379,     0,     0,     0,     0,     0,
     192,   301,   389,   385,   410,   355,   414,   417,   360,   240,
     482,    56,   538,   392,   406,   407,   408,     0,   398,   399,
     369,   300,   307,   305,   286,   294,     0,     0,   192,     0,
       0,     0,     0,   192,     0,   394,   395,     0,   424,     0,
     309,   297,   302,   390,   386,     0,     0,   418,    57,   539,
     396,   400,   421,   370,   308,   192,     0,   411,   415,     0,
       0,   192,     0,   310,   298,     0,   397,   401,   422,   419,
       0,   428,     0,   425,   423,   426,     0,   427
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -597,  -597,   362,   237,  -597,  -582,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -558,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,   -34,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,   244,  -597,  -597,
     162,  -597,  -597,  -597,   666,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,   670,  -597,   272,  -597,  -597,  -235,  -597,  -597,
     281,  -597,  -597,  -597,  -597,  -276,  -352,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -316,  -597,  -597,   -22,  -597,
    -597,  -189,   -11,  -597,    13,  -597,  -597,  -597,  -597,   462,
      79,  -207,  -597,   320,   322,   319,   -65,   -73,   -24,    66,
    -597,  -304,     0,  -597,  -597,  -597,  -597,   248,    14,  -597,
    -597,  -597,   -71,   301,  -438,  -597,  -597,  -597,    55,  -597,
    -597,  -576,   -14,  -597,  -597,    -3,  -597,   -56,  -597,  -597,
     -43,   -42,   -55,   -51,   -50,  -597,  -597,   -37,  -597,   -36,
    -597,  -597,  -597,  -597,   280,   358,  -597,  -254,  -597,  -597,
    -597,   -33,  -597,   -31,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,   -70,  -597,  -597,  -597,  -597,  -597,   -95,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,   -38,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,    11,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,   -67,  -597,  -597,  -597,   -66,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,    95,  -597,  -597,  -323,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,    16,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -596,  -597,  -597,  -597,  -597,
    -597,   -90,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -126,  -597,  -597,  -487,  -597,  -543,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
      18,    19,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,   359,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -305,   304,  -303,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,   648,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,   279,  -597,  -597,
     -75,  -597,  -597,  -597,  -597,  -597,  -597,  -597,    69,  -597,
    -597,  -597,   347,  -597,  -597,   195,  -597,  -597,  -597,  -597,
    -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597,  -597
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    27,   383,    28,   170,   174,   175,   169,
     173,   119,   114,   123,   176,   178,   180,   184,   185,    80,
      29,    82,    30,   113,   318,   448,    31,    32,   115,   322,
     450,   625,   700,   677,   701,   678,   679,   717,   803,    33,
     116,   388,    34,    35,   122,   329,   458,    36,    83,    37,
     140,   328,    38,    39,    40,   124,   330,   471,    41,   212,
     357,   539,    42,   253,    43,   100,   243,   336,    44,    45,
     392,   472,   473,   570,   569,   391,   459,   555,   566,   567,
     554,   557,   556,   558,   552,   389,   454,   627,   304,   217,
     286,   107,   349,    46,   460,    81,   277,   422,   610,   191,
     324,   332,   306,   307,   308,   309,   310,   311,   312,   313,
     314,   333,    90,    48,   315,   316,   381,   317,   461,    84,
     202,   278,   142,   143,   144,   145,   146,    50,   350,   424,
     613,   351,   689,   713,   755,   352,   353,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,    51,    85,    52,
     171,   340,   481,   399,   482,   578,   401,   485,   641,   705,
     579,    53,    86,    54,   261,   402,   581,   684,   723,   768,
     492,   744,   769,   745,   770,   826,   765,   746,   771,   747,
     767,   766,   809,   811,   825,    55,    56,    57,    87,   279,
     425,   615,   536,   616,   692,   537,   162,   337,   478,   163,
     252,   394,   164,   165,   338,   479,   166,   167,   339,   480,
     168,   354,   423,   532,   612,   533,   611,   691,   462,   414,
     515,   659,   711,   752,   415,   516,   660,   712,   754,   463,
      88,   280,   426,   617,   464,   634,   720,   763,   808,   465,
     564,   475,   568,   704,   740,   686,   707,   708,   726,   750,
     799,   727,   748,   798,   739,   761,   762,   787,   806,   830,
     788,   807,   831,   565,   789,   729,   751,   800,   733,   753,
     801,   778,   802,   829,   823,   832,   840,   842,   843,   846,
     466,   467,    61,    62,    63,   177,   342,   498,    64,   215,
     359,   283,   358,   403,   499,   587,   588,   589,   590,   591,
     585,   592,   621,   519,   622,   418,   521,   502,   503,   504,
      65,   179,    66,   103,   284,   431,   619,   693,   735,   361,
     430,   757,   269,   343,   509,   404,   510,   596,   597,   511,
     653,   709,   512,   654,   710,    67,    68,    69,    70,    71,
     272,   405,   598,    72,    73,    74,   182,   271,    75,   273,
     406,   599,    76,   236,   237,   323,   451,   238,   759,   782,
     760,    77,   109,   363,   542,   672,   623,   673,   624,   670,
     671,   669,   419,   240,   387,   326,    78,    79,   110,   364,
     187,   276,   420,   347,   421,   525,   526,   524,   528
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     106,   108,     4,    89,   111,   161,   157,   158,   101,   102,
     159,    91,   160,   203,   198,   199,   305,    47,    49,   395,
      58,   141,    59,    60,   413,   192,   195,   561,   285,   601,
     196,   197,   325,   396,   397,   398,   344,   664,   193,   194,
     474,   104,   416,   675,   417,   104,   161,   157,   158,     3,
     208,   159,   508,   160,   603,   287,    -2,   517,   687,   190,
     200,   288,   141,   614,   682,     5,   703,   676,     6,     7,
       8,     9,   201,   218,   204,   205,   206,   207,   561,   219,
     497,   427,   210,     8,    10,    11,   213,    12,  -138,   214,
     219,    13,   216,   120,   741,   604,   369,   362,   500,   104,
     501,  -192,  -192,   117,    14,    15,    16,    17,   721,   221,
     222,    18,   223,    18,    19,    97,   239,    20,   112,   105,
      21,   742,   743,   105,   429,   742,   743,    22,    23,   513,
     118,   483,   219,  -192,    24,    25,   674,   675,  -192,  -192,
    -192,  -192,  -192,  -192,  -192,  -192,  -192,  -192,  -192,  -192,
    -192,  -192,    26,  -192,  -192,  -192,  -192,    26,   434,   198,
     199,   676,   474,    26,  -311,    26,    26,   730,    26,    26,
     192,   195,   688,   219,   220,   196,   197,   219,  -192,   247,
     219,  -293,   248,   193,   194,   741,   476,   186,     5,   120,
    -139,     6,     7,     8,     9,   120,   541,   764,   121,     8,
    -140,   -97,    26,   172,   190,   120,   120,    10,    11,   775,
      12,  -192,   742,   743,    13,   377,   378,   379,   188,   620,
     790,   561,   813,   104,   784,   785,   786,    14,    15,    16,
      17,  -433,   407,   408,   181,  -192,    18,    19,   181,   254,
      20,   255,    18,    21,   249,   250,    93,   373,   374,   833,
      22,    23,   407,   408,   209,   837,   211,    24,    25,    92,
      94,   772,   409,   104,   375,   376,   348,   410,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
      10,    11,   409,    12,   137,   138,   139,   410,    96,    99,
     411,   412,    26,   290,   291,   292,   293,   294,   295,   296,
     297,   105,   441,   442,   298,   299,   390,   662,   439,   440,
     561,  -505,   -44,   390,   -44,   638,   639,   300,   301,   224,
     821,   225,   302,   303,   241,    26,   242,   244,   -44,   -44,
     602,   -44,   -44,   245,   251,   -44,   428,   246,   608,   219,
     256,   105,   257,   325,   260,   289,   488,   489,   258,   259,
     620,   490,   491,   443,   444,   445,   262,   -44,   266,   263,
     264,   -44,   265,   267,   493,    26,   366,   367,   368,   561,
     268,   270,   141,   274,   275,   -44,  -506,   453,   281,   104,
     487,   321,   447,   282,   320,   331,   327,   518,   334,   522,
     523,   335,   341,   643,   644,   645,   518,   345,   562,   563,
     346,   484,   355,   325,   356,   104,   370,   360,   540,   371,
     380,   365,   384,   580,   385,   290,   291,   292,   293,   294,
     295,   296,   297,   105,   372,   535,   298,   299,   386,   393,
     432,  -341,   446,   435,   530,   449,   529,   520,   452,   300,
     301,   531,   534,   560,   302,   303,   161,   157,   158,   562,
     563,   159,   550,   160,   104,   543,   546,   547,   548,   551,
      47,    49,   141,    58,   573,    59,    60,   247,   553,   571,
     572,   577,   582,   583,   584,   290,   291,   292,   293,   294,
     295,   296,   297,   105,   815,   816,   298,   299,   574,   575,
     576,  -268,   586,   595,   560,   593,   594,   600,   605,   300,
     301,   606,   607,   609,   302,   303,  -364,   618,    -8,   628,
     629,   630,   631,   835,   632,   633,   635,   636,   640,   666,
     646,   647,   648,   649,   290,   291,   292,   293,   294,   295,
     296,   297,   105,   650,   651,   298,   299,   642,   657,   652,
     668,   655,   656,   685,  -382,   658,   665,   694,   667,   695,
     696,   104,   714,   697,   303,   702,   486,   126,   127,   792,
     706,   130,   131,   132,   133,   718,   -50,   104,   637,   484,
     484,    12,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,    10,    11,   722,    12,   137,   138,
     139,   724,   562,   563,   725,   728,   732,   734,   738,   736,
     749,   104,   756,   758,   773,   680,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,    10,    11,
    -391,    12,   137,   138,   139,   774,   776,   777,   781,   105,
     783,   791,   794,   793,   681,   805,   817,   560,    47,    49,
     822,    58,   804,    59,    60,   105,   820,   824,   834,    89,
     -98,   827,   699,   104,   838,   120,   828,   690,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     839,   716,   841,   844,   137,   138,   139,   845,   847,   105,
     382,   562,   563,    98,   545,   737,    95,   514,   319,   661,
     436,   438,   535,   437,   544,   477,   683,   731,   400,   795,
      89,   719,   812,   715,   836,   104,   538,   663,   690,   814,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   189,
     135,   136,   527,   433,   183,   810,   560,   138,   139,   819,
     549,   105,    47,    49,   780,    58,   559,    59,    60,     0,
     562,   563,   698,     0,    89,   626,     0,     0,     0,     0,
     505,     0,   779,     0,     0,     0,     0,     0,   161,   157,
     158,     0,     0,   159,     0,   160,     0,     0,     0,     0,
     796,   797,     0,     0,   141,     0,     0,     0,   390,   390,
     226,     0,   227,   105,     0,   560,     0,     0,     0,     0,
       0,     0,   818,     0,     0,     0,   228,   229,     0,   230,
     231,     0,   455,   232,  -373,     6,     0,   390,     9,  -373,
    -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,
    -373,    10,    11,     0,    12,   233,  -373,  -373,    13,   234,
       0,   407,   408,   456,   457,  -373,     0,     0,     0,     0,
       0,    14,     0,   235,   104,   468,   469,   470,     0,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   189,   135,
     136,     0,     0,     0,    22,    23,   138,   139,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -373,     0,   455,     0,  -373,     6,     0,   -83,
       9,  -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,
    -373,  -373,  -373,    10,    11,     0,    12,     0,  -373,  -373,
      13,     0,     0,   407,   408,   456,   457,  -373,     0,     0,
     407,   408,   105,    14,     0,     0,   104,   468,   469,   470,
       0,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     189,   494,   495,     0,     0,     0,    22,    23,   138,   139,
     409,   496,     0,     0,     0,   410,     0,     0,   411,   412,
       0,     0,     0,     0,  -373,     0,   455,     0,  -373,     6,
       0,  -518,     9,  -373,  -373,  -373,  -373,  -373,  -373,  -373,
    -373,  -373,  -373,  -373,  -373,    10,    11,     0,    12,     0,
    -373,  -373,    13,     0,     0,   407,   408,   456,   457,  -373,
       0,     0,     0,     0,   105,    14,     0,     0,     0,   506,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   507,
       0,     0,     0,     0,     0,     0,     0,     0,    22,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -373,     0,   455,     0,
    -373,     6,     0,  -485,     9,  -373,  -373,  -373,  -373,  -373,
    -373,  -373,  -373,  -373,  -373,  -373,  -373,    10,    11,     0,
      12,     0,  -373,  -373,    13,     0,     0,   407,   408,   456,
     457,  -373,     0,   455,     0,  -373,     6,    14,     0,     9,
    -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,
    -373,  -373,    10,    11,     0,    12,     0,  -373,  -373,    13,
      22,    23,   407,   408,   456,   457,  -373,     0,     0,     0,
       0,     0,    14,     0,     0,     0,     0,     0,  -373,     0,
       0,     0,     0,     0,     0,   -65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,    23,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -373,     0,   455,     0,  -373,     6,     0,
     -87,     9,  -373,  -373,  -373,  -373,  -373,  -373,  -373,  -373,
    -373,  -373,  -373,  -373,    10,    11,     0,    12,     0,  -373,
    -373,    13,     0,     0,   407,   408,   456,   457,  -373,     0,
       0,     0,     0,     0,    14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,     0,     0,     6,     7,     8,
       9,     0,     0,     0,     0,     0,     0,    22,    23,     0,
       0,     0,     0,    10,    11,     0,    12,     0,     0,     0,
      13,     0,     0,     0,     0,  -373,     0,     0,     0,     0,
       0,     0,  -509,    14,    15,    16,    17,     0,     0,     0,
       0,     0,    18,    19,     0,     0,    20,     0,     0,    21,
       0,     0,     0,     0,     0,     0,    22,    23,     0,     0,
       0,     0,     0,    24,    25,   674
};

static const yytype_int16 yycheck[] =
{
      22,    23,     2,    14,    26,    43,    43,    43,    19,    20,
      43,    14,    43,    84,    81,    81,   223,     4,     4,   335,
       4,    43,     4,     4,   347,    81,    81,   465,   217,   516,
      81,    81,   239,   337,   338,   339,   271,   613,    81,    81,
     392,     3,   347,   625,   347,     3,    84,    84,    84,     0,
       3,    84,   404,    84,     3,     3,     0,    19,   654,    81,
      82,     3,    84,     3,     3,     1,     3,   625,     4,     5,
       6,     7,    83,    75,    85,    86,    87,    88,   516,    81,
     403,   357,    93,     6,    20,    21,    97,    23,    86,   100,
      81,    27,   103,    91,     1,     3,   303,   286,   403,     3,
     403,    28,    29,    86,    40,    41,    42,    43,   704,   109,
     110,    49,   103,    49,    50,    53,   116,    53,    89,    81,
      56,    28,    29,    81,   359,    28,    29,    63,    64,   405,
      86,     1,    81,     3,    70,    71,    72,   719,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   105,    23,    24,    25,    26,   105,   365,   226,
     226,   719,   514,   105,    86,   105,   105,   710,   105,   105,
     226,   226,   659,    81,    75,   226,   226,    81,   105,     8,
      81,    88,    11,   226,   226,     1,   393,    87,     1,    91,
      86,     4,     5,     6,     7,    91,   431,   740,    36,     6,
      86,    86,   105,    22,   226,    91,    91,    20,    21,   752,
      23,    81,    28,    29,    27,    99,   100,   101,    86,   542,
     763,   659,   798,     3,    35,    36,    37,    40,    41,    42,
      43,    86,    30,    31,    72,   105,    49,    50,    76,    90,
      53,    92,    49,    56,     8,     9,    53,    82,    83,   825,
      63,    64,    30,    31,    92,   831,    94,    70,    71,    15,
      16,   748,    60,     3,    97,    98,   277,    65,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    60,    23,    24,    25,    26,    65,    16,    17,
      68,    69,   105,    73,    74,    75,    76,    77,    78,    79,
      80,    81,   375,   376,    84,    85,   328,   611,   373,   374,
     748,    86,     4,   335,     6,   569,   570,    97,    98,    87,
     807,    86,   102,   103,    86,   105,    48,    87,    20,    21,
     519,    23,    24,    86,    89,    27,   358,    87,   527,    81,
      89,    81,    89,   550,    87,     3,   402,   402,    86,    86,
     673,   402,   402,   377,   378,   379,    86,    49,    87,    86,
      86,    53,    86,    86,   402,   105,   300,   301,   302,   807,
      87,    86,   394,    86,    86,    67,    86,   388,    87,     3,
     402,     3,   382,    91,    89,     8,    90,   409,    89,   411,
     412,    47,   103,   582,   583,   584,   418,    87,   465,   465,
      87,   401,    87,   610,    87,     3,    94,    91,   430,    95,
     104,    93,     3,   484,    90,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    96,   425,    84,    85,    92,    92,
      87,   106,    92,   104,    93,    90,    88,    58,    92,    97,
      98,   106,    92,   465,   102,   103,   484,   484,   484,   516,
     516,   484,   452,   484,     3,     3,    88,     3,    87,    92,
     447,   447,   484,   447,    90,   447,   447,     8,    88,    88,
      86,    88,   494,   495,   496,    73,    74,    75,    76,    77,
      78,    79,    80,    81,   800,   801,    84,    85,    90,    90,
      90,    88,    88,    88,   516,   506,   507,    31,    86,    97,
      98,    86,    86,    86,   102,   103,    88,    88,    88,    86,
      86,    86,    86,   829,    86,    86,    86,    86,    86,    92,
      86,    86,    86,    86,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    86,    86,    84,    85,   104,    88,    86,
      66,    86,    86,   103,    35,    88,    88,    86,    88,    86,
      86,     3,   107,    88,   103,    86,     8,     9,    10,   766,
      87,    13,    14,    15,    16,    88,    88,     3,   568,   569,
     570,    23,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    86,    23,    24,    25,
      26,   104,   659,   659,    35,    38,    55,    92,   103,    89,
     104,     3,    59,     3,    92,   627,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     104,    23,    24,    25,    26,   103,   103,    62,    90,    81,
     104,    86,    88,    91,   634,   104,   103,   659,   625,   625,
      39,   625,    92,   625,   625,    81,    92,    91,    86,   660,
      86,   104,   674,     3,   103,    91,   104,   660,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
     104,   693,    75,   104,    24,    25,    26,    92,    75,    81,
     318,   748,   748,    17,   447,   719,    16,   406,   226,   610,
     370,   372,   692,   371,   446,   394,   641,   711,   340,   769,
     711,   701,   797,   692,   830,     3,   426,   612,   711,   799,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,   418,   364,    76,   796,   748,    25,    26,   804,
     451,    81,   719,   719,   756,   719,    34,   719,   719,    -1,
     807,   807,   673,    -1,   755,   550,    -1,    -1,    -1,    -1,
     403,    -1,   755,    -1,    -1,    -1,    -1,    -1,   796,   796,
     796,    -1,    -1,   796,    -1,   796,    -1,    -1,    -1,    -1,
     770,   771,    -1,    -1,   796,    -1,    -1,    -1,   800,   801,
       4,    -1,     6,    81,    -1,   807,    -1,    -1,    -1,    -1,
      -1,    -1,   803,    -1,    -1,    -1,    20,    21,    -1,    23,
      24,    -1,     1,    27,     3,     4,    -1,   829,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    49,    25,    26,    27,    53,
      -1,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    67,     3,    44,    45,    46,    -1,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    63,    64,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,     1,    -1,     3,     4,    -1,    88,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    23,    -1,    25,    26,
      27,    -1,    -1,    30,    31,    32,    33,    34,    -1,    -1,
      30,    31,    81,    40,    -1,    -1,     3,    44,    45,    46,
      -1,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    51,    52,    -1,    -1,    -1,    63,    64,    25,    26,
      60,    61,    -1,    -1,    -1,    65,    -1,    -1,    68,    69,
      -1,    -1,    -1,    -1,    81,    -1,     1,    -1,     3,     4,
      -1,    88,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    23,    -1,
      25,    26,    27,    -1,    -1,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    -1,    81,    40,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
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
      -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    -1,     1,    -1,     3,     4,    -1,
      88,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    -1,    25,
      26,    27,    -1,    -1,    30,    31,    32,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,    -1,     4,     5,     6,
       7,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    20,    21,    -1,    23,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    40,    41,    42,    43,    -1,    -1,    -1,
      -1,    -1,    49,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    70,    71,    72
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   109,   110,     0,   220,     1,     4,     5,     6,     7,
      20,    21,    23,    27,    40,    41,    42,    43,    49,    50,
      53,    56,    63,    64,    70,    71,   105,   111,   113,   128,
     130,   134,   135,   147,   150,   151,   155,   157,   160,   161,
     162,   166,   170,   172,   176,   177,   201,   202,   221,   226,
     235,   255,   257,   269,   271,   293,   294,   295,   337,   388,
     389,   390,   391,   392,   396,   418,   420,   443,   444,   445,
     446,   447,   451,   452,   453,   456,   460,   469,   484,   485,
     127,   203,   129,   156,   227,   256,   270,   296,   338,   200,
     220,   243,   155,    53,   155,   170,   172,    53,   162,   172,
     173,   200,   200,   421,     3,    81,   196,   199,   196,   470,
     486,   196,    89,   131,   120,   136,   148,    86,    86,   119,
      91,   158,   152,   121,   163,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    24,    25,    26,
     158,   196,   230,   231,   232,   233,   234,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   257,   269,
     271,   295,   304,   307,   310,   311,   314,   315,   318,   117,
     114,   258,    22,   118,   115,   116,   122,   393,   123,   419,
     124,   158,   454,   454,   125,   126,    87,   488,    86,    17,
     196,   207,   245,   248,   249,   250,   251,   252,   311,   315,
     196,   200,   228,   230,   200,   200,   200,   200,     3,   158,
     200,   158,   167,   200,   200,   397,   200,   197,    75,    81,
      75,   220,   220,   103,    87,    86,     4,     6,    20,    21,
      23,    24,    27,    49,    53,    67,   461,   462,   465,   220,
     481,    86,    48,   174,    87,    86,    87,     8,    11,     8,
       9,    89,   308,   171,    90,    92,    89,    89,    86,    86,
      87,   272,    86,    86,    86,    86,    87,    86,    87,   430,
      86,   455,   448,   457,    86,    86,   489,   204,   229,   297,
     339,    87,    91,   399,   422,   199,   198,     3,     3,     3,
      73,    74,    75,    76,    77,    78,    79,    80,    84,    85,
      97,    98,   102,   103,   196,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   222,   223,   225,   132,   207,
      89,     3,   137,   463,   208,   209,   483,    90,   159,   153,
     164,     8,   209,   219,    89,    47,   175,   305,   312,   316,
     259,   103,   394,   431,   175,    87,    87,   491,   200,   200,
     236,   239,   243,   244,   319,    87,    87,   168,   400,   398,
      91,   427,   199,   471,   487,    93,   217,   217,   217,   209,
      94,    95,    96,    82,    83,    97,    98,    99,   100,   101,
     104,   224,   110,   112,     3,    90,    92,   482,   149,   193,
     196,   183,   178,    92,   309,   193,   219,   219,   219,   261,
     263,   264,   273,   401,   433,   449,   458,    30,    31,    60,
      65,    68,    69,   326,   327,   332,   410,   412,   413,   480,
     490,   492,   205,   320,   237,   298,   340,   183,   196,   175,
     428,   423,    87,   399,   209,   104,   211,   212,   213,   214,
     214,   215,   215,   216,   216,   216,    92,   220,   133,    90,
     138,   464,    92,   200,   194,     1,    32,    33,   154,   184,
     202,   226,   326,   337,   342,   347,   388,   389,    44,    45,
      46,   165,   179,   180,   184,   349,   209,   231,   306,   313,
     317,   260,   262,     1,   220,   265,     8,   196,   245,   250,
     251,   252,   278,   295,    51,    52,    61,   326,   395,   402,
     410,   412,   415,   416,   417,   480,    44,    54,   184,   432,
     434,   437,   440,   183,   178,   328,   333,    19,   196,   411,
      58,   414,   196,   196,   495,   493,   494,   411,   496,    88,
      93,   106,   321,   323,    92,   220,   300,   303,   262,   169,
     196,   175,   472,     3,   225,   111,    88,     3,    87,   465,
     220,    92,   192,    88,   188,   185,   190,   189,   191,    34,
     196,   232,   311,   315,   348,   371,   186,   187,   350,   182,
     181,    88,    86,    90,    90,    90,    90,    88,   263,   268,
     230,   274,   196,   196,   196,   408,    88,   403,   404,   405,
     406,   407,   409,   200,   200,    88,   435,   436,   450,   459,
      31,   371,   199,     3,     3,    86,    86,    86,   199,    86,
     206,   324,   322,   238,     3,   299,   301,   341,    88,   424,
     326,   410,   412,   474,   476,   139,   483,   195,    86,    86,
      86,    86,    86,    86,   343,    86,    86,   220,   265,   265,
      86,   266,   104,   199,   199,   199,    86,    86,    86,    86,
      86,    86,    86,   438,   441,    86,    86,    88,    88,   329,
     334,   208,   219,   323,   239,    88,    92,    88,    66,   479,
     477,   478,   473,   475,    72,   113,   128,   141,   143,   144,
     196,   220,     3,   236,   275,   103,   353,   353,   371,   240,
     243,   325,   302,   425,    86,    86,    86,    88,   476,   196,
     140,   142,    86,     3,   351,   267,    87,   354,   355,   439,
     442,   330,   335,   241,   107,   303,   196,   145,    88,   220,
     344,   353,    86,   276,   104,    35,   356,   359,    38,   373,
     373,   240,    55,   376,    92,   426,    89,   143,   103,   362,
     352,     1,    28,    29,   279,   281,   285,   287,   360,   104,
     357,   374,   331,   377,   336,   242,    59,   429,     3,   466,
     468,   363,   364,   345,   373,   284,   289,   288,   277,   280,
     282,   286,   371,    92,   103,   373,   103,    62,   379,   243,
     196,    90,   467,   104,    35,    36,    37,   365,   368,   372,
     373,    86,   209,    91,    88,   281,   220,   220,   361,   358,
     375,   378,   380,   146,    92,   104,   366,   369,   346,   290,
     230,   291,   287,   239,   359,   193,   193,   103,   200,   468,
      92,   371,    39,   382,    91,   292,   283,   104,   104,   381,
     367,   370,   383,   239,    86,   193,   368,   239,   103,   104,
     384,    75,   385,   386,   104,    92,   387,    75
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   108,   109,   110,   110,   111,   111,   111,   112,   114,
     113,   115,   113,   116,   113,   117,   113,   118,   113,   119,
     113,   120,   113,   121,   113,   122,   113,   123,   113,   124,
     113,   125,   113,   126,   113,   127,   113,   129,   128,   131,
     132,   133,   130,   134,   136,   137,   138,   139,   140,   135,
     141,   142,   142,   143,   143,   145,   146,   144,   148,   149,
     147,   150,   150,   152,   153,   154,   151,   156,   155,   157,
     157,   157,   159,   158,   158,   160,   160,   160,   160,   161,
     161,   163,   164,   165,   162,   167,   168,   169,   166,   171,
     170,   173,   172,   174,   174,   175,   175,   176,   176,   177,
     178,   178,   179,   179,   179,   181,   180,   182,   180,   183,
     183,   185,   184,   186,   184,   187,   184,   188,   184,   189,
     184,   190,   184,   191,   184,   192,   184,   193,   195,   194,
     194,   196,   197,   196,   198,   196,   199,   200,   201,   201,
     201,   203,   204,   205,   206,   202,   207,   207,   207,   207,
     207,   207,   207,   207,   207,   208,   209,   210,   210,   211,
     211,   212,   212,   213,   213,   213,   214,   214,   214,   215,
     215,   215,   215,   216,   216,   216,   216,   217,   217,   217,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     219,   220,   220,   221,   221,   222,   222,   223,   224,   224,
     225,   227,   226,   226,   226,   226,   226,   226,   229,   228,
     230,   230,   231,   231,   231,   232,   232,   232,   232,   232,
     232,   232,   232,   233,   233,   233,   233,   234,   234,   234,
     235,   235,   236,   238,   237,   237,   239,   239,   240,   242,
     241,   241,   243,   244,   245,   245,   246,   246,   246,   247,
     247,   247,   248,   248,   248,   249,   250,   250,   251,   252,
     253,   254,   256,   255,   258,   259,   260,   257,   261,   262,
     262,   264,   263,   266,   267,   265,   268,   265,   270,   269,
     272,   273,   274,   275,   276,   277,   271,   278,   278,   278,
     278,   278,   278,   279,   280,   280,   282,   283,   281,   284,
     281,   285,   286,   286,   288,   287,   289,   290,   287,   292,
     291,   293,   294,   296,   297,   298,   299,   295,   300,   302,
     301,   301,   303,   305,   306,   304,   304,   308,   309,   307,
     310,   312,   313,   311,   311,   314,   316,   317,   315,   315,
     318,   320,   319,   321,   322,   322,   324,   325,   323,   326,
     326,   328,   329,   330,   331,   327,   333,   334,   335,   336,
     332,   338,   339,   340,   341,   337,   343,   344,   345,   346,
     342,   347,   347,   347,   348,   348,   350,   351,   352,   349,
     354,   353,   355,   353,   356,   358,   357,   357,   360,   361,
     359,   363,   362,   364,   362,   365,   367,   366,   366,   369,
     370,   368,   371,   371,   371,   371,   372,   372,   372,   374,
     375,   373,   373,   377,   378,   376,   376,   380,   381,   379,
     379,   383,   384,   382,   382,   385,   387,   386,   386,   388,
     389,   390,   390,   391,   393,   394,   395,   392,   397,   398,
     396,   400,   399,   399,   401,   401,   403,   402,   404,   402,
     405,   402,   406,   402,   407,   402,   408,   402,   409,   402,
     410,   411,   411,   412,   413,   414,   414,   415,   416,   417,
     419,   418,   421,   422,   423,   424,   425,   426,   420,   428,
     427,   427,   429,   429,   431,   432,   430,   433,   433,   434,
     435,   434,   436,   434,   438,   439,   437,   441,   442,   440,
     443,   443,   443,   444,   444,   445,   446,   448,   449,   450,
     447,   451,   452,   453,   455,   454,   457,   458,   459,   456,
     460,   460,   461,   461,   461,   461,   461,   461,   461,   461,
     461,   461,   462,   464,   463,   463,   465,   465,   466,   467,
     467,   468,   470,   471,   472,   473,   469,   474,   475,   475,
     477,   476,   478,   476,   479,   476,   480,   480,   481,   482,
     482,   483,   484,   486,   487,   485,   489,   490,   488,   491,
     491,   493,   492,   494,   492,   495,   492,   496,   492
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     0,     1,     2,     2,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       0,     0,     7,     2,     0,     0,     0,     0,     0,    11,
       2,     3,     0,     1,     2,     0,     0,     8,     0,     0,
       6,     1,     1,     0,     0,     0,     7,     0,     3,     2,
       3,     3,     0,     4,     0,     1,     1,     1,     1,     2,
       1,     0,     0,     0,     7,     0,     0,     0,     8,     0,
       4,     0,     3,     1,     0,     2,     0,     2,     1,     2,
       2,     0,     1,     1,     2,     0,     3,     0,     3,     2,
       0,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     2,     0,     4,
       0,     1,     0,     3,     0,     4,     1,     2,     1,     2,
       2,     0,     0,     0,     0,     9,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       3,     1,     3,     1,     3,     3,     1,     3,     3,     1,
       3,     3,     3,     1,     2,     2,     2,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     0,     5,     2,     1,     1,     2,     3,     0,
       3,     0,     3,     1,     1,     1,     2,     1,     0,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     0,     4,     0,     1,     1,     2,     0,
       4,     0,     1,     1,     1,     1,     1,     2,     1,     2,
       3,     2,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     0,     3,     0,     0,     0,     7,     2,     2,
       0,     0,     2,     0,     0,     6,     0,     3,     0,     3,
       0,     0,     0,     0,     0,     0,    14,     1,     1,     1,
       1,     1,     1,     2,     2,     0,     0,     0,     6,     0,
       3,     2,     3,     0,     0,     3,     0,     0,     5,     0,
       3,     1,     1,     0,     0,     0,     0,     9,     2,     0,
       4,     0,     2,     0,     0,     6,     2,     0,     0,     5,
       6,     0,     0,     6,     1,     1,     0,     0,     6,     1,
       1,     0,     3,     2,     2,     0,     0,     0,     5,     1,
       1,     0,     0,     0,     0,     9,     0,     0,     0,     0,
       9,     0,     0,     0,     0,     9,     0,     0,     0,     0,
      11,     1,     1,     0,     1,     1,     0,     0,     0,     8,
       0,     3,     0,     4,     2,     0,     4,     0,     0,     0,
       5,     0,     3,     0,     4,     2,     0,     4,     0,     0,
       0,     5,     1,     1,     1,     1,     1,     1,     1,     0,
       0,     6,     0,     0,     0,     6,     0,     0,     0,     6,
       0,     0,     0,     6,     0,     2,     0,     4,     0,     3,
       3,     1,     1,     2,     0,     0,     0,     7,     0,     0,
       6,     0,     3,     0,     2,     0,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       3,     1,     1,     3,     2,     1,     0,     3,     3,     3,
       0,     3,     0,     0,     0,     0,     0,     0,    13,     0,
       3,     0,     2,     0,     0,     0,     5,     2,     0,     1,
       0,     3,     0,     3,     0,     0,     6,     0,     0,     6,
       1,     1,     1,     1,     1,     2,     3,     0,     0,     0,
       8,     3,     3,     2,     0,     3,     0,     0,     0,     8,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     0,     4,     0,     2,     5,     2,     3,
       0,     1,     0,     0,     0,     0,    10,     2,     2,     0,
       0,     3,     0,     3,     0,     3,     3,     3,     3,     4,
       0,     1,     2,     0,     0,     6,     0,     0,     5,     2,
       0,     0,     3,     0,     3,     0,     3,     0,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 5:
#line 369 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 2270 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 372 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 2277 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 375 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 2284 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 392 "fe/idl.ypp" /* yacc.c:1646  */
    {
// fixed_definition : type_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 2293 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 397 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2302 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 402 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | typeid_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 2311 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 407 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2320 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 412 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | typeprefix_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 2329 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 417 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2338 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 422 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | const_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 2347 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 427 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2356 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 432 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | exception
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 2365 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 437 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
        idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2374 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 442 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | interface_def
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
        }
#line 2383 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 447 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2392 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 452 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | module
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
        }
#line 2401 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 457 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2410 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 462 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | value_def
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeDeclSeen);
        }
#line 2419 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 467 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2428 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 472 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | component
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentDeclSeen);
        }
#line 2437 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 477 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2446 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 482 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | home_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeDeclSeen);
        }
#line 2455 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 487 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2464 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 492 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | event
          idl_global->set_parse_state (IDL_GlobalData::PS_EventDeclSeen);
        }
#line 2473 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 497 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2482 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 502 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | porttype_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeDeclSeen);
        }
#line 2491 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 507 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2500 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 512 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | connector_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorDeclSeen);
        }
#line 2509 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 517 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2518 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 522 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 2527 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 527 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 2537 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 536 "fe/idl.ypp" /* yacc.c:1646  */
    {
// module_header  : IDL_MODULE
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
        }
#line 2546 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 541 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 2554 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 548 "fe/idl.ypp" /* yacc.c:1646  */
    {
// module : module_header
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleIDSeen);

          // The module_header rule is common to template module, fixed
          // module and instantiated template module. In the last
          // case, a fully scoped name is allowed, but here we
          // allow only an identifier (a scoped name of length
          // 1). If not satisfied, we output a parse error with
          // the appropriate message.
          if ((yyvsp[0].idlist)->length () != 1)
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
                                                     (yyvsp[0].idlist));
              (void) s->fe_add_module (m);
            }

          (yyvsp[0].idlist)->destroy ();
          delete (yyvsp[0].idlist);
          (yyvsp[0].idlist) = 0;

          /*
           * Push it on the stack
           */
          idl_global->scopes ().push (m);
        }
#line 2597 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 587 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
        idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
        }
#line 2606 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 592 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_definition
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
        }
#line 2615 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 597 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
          /*
           * Finished with this module - pop it from the scope stack.
           */

          idl_global->scopes ().pop ();
        }
#line 2629 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 610 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleIDSeen);
        }
#line 2637 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 617 "fe/idl.ypp" /* yacc.c:1646  */
    {
// template_module : template_module_header
          // The module_header rule is common to template module, fixed
          // module and instantiated template module. In the last
          // case, a fully scoped name is allowed, but here we
          // allow only an identifier (a scoped name of length
          // 1). If not satisfied, we output a syntax error with
          // the appropriate message.
          if ((yyvsp[0].idlist)->length () != 1)
            {
              idl_global->err ()->syntax_error (
                IDL_GlobalData::PS_ModuleIDSeen);
            }
        }
#line 2656 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 632 "fe/idl.ypp" /* yacc.c:1646  */
    {
          if (FE_Utils::duplicate_param_id ((yyvsp[0].plval)))
            {
              idl_global->err ()->duplicate_param_id (
                (yyvsp[-2].idlist));

              return 1;
            }
        }
#line 2670 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 642 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        '>'
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleParamsSeen);

          AST_Template_Module *tm =
            idl_global->gen ()->create_template_module ((yyvsp[-4].idlist),
                                                        (yyvsp[-2].plval));

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Module *m = s->fe_add_module (tm);

          // We've probably tried to reopen a template module,
          // going further will cause a crash.
          if (m == 0)
            {
              return 1;
            }

          /*
           * Push it on the stack
           */
          idl_global->scopes ().push (tm);

          // Contained items not part of an alias will get flag set.
          idl_global->in_tmpl_mod_no_alias (true);

          // Store these for reference as we parse the scope
          // of the template module.
          idl_global->current_params ((yyvsp[-2].plval));
        }
#line 2705 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 673 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleSqSeen);
        }
#line 2713 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 677 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleBodySeen);
        }
#line 2721 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 681 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleQsSeen);

          /*
           * Finished with this module - pop it from the scope stack.
           */
          idl_global->scopes ().pop ();

          // Unset the flag, the no_alias version because any scope
          // traversal triggered by an alias would have ended by now.
          idl_global->in_tmpl_mod_no_alias (false);

          // Clear the pointer so scoped name lookup will know
          // that we are no longer in a template module scope.
          idl_global->current_params (0);
        }
#line 2743 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 716 "fe/idl.ypp" /* yacc.c:1646  */
    {
// template_module_ref : IDL_ALIAS scoped_name
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefSeen);
        }
#line 2753 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 722 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        '<' at_least_one_formal_parameter_name '>'
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefParamsSeen);
        }
#line 2763 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 728 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        defining_id
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefIDSeen);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-6].idlist));

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[-6].idlist));
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
            ref->match_param_refs ((yyvsp[-3].slval),
                                   s);

          if (! refs_match)
            {
              // Error message is already output.
              return 1;
            }

          UTL_ScopedName sn ((yyvsp[0].idval), 0);

          AST_Template_Module_Ref *tmr =
            idl_global->gen ()->create_template_module_ref (
              &sn,
              ref,
              (yyvsp[-3].slval));

          (void) s->fe_add_template_module_ref (tmr);

          sn.destroy ();
          (yyvsp[-6].idlist)->destroy ();
          delete (yyvsp[-6].idlist);
          (yyvsp[-6].idlist) = 0;

          // Save the current flag value to be restored below.
          bool itmna_flag = idl_global->in_tmpl_mod_no_alias ();
          idl_global->in_tmpl_mod_no_alias (false);
          idl_global->in_tmpl_mod_alias (true);

          ast_visitor_context ctx;
          ctx.template_params (ref->template_params ());
          ast_visitor_tmpl_module_ref v (&ctx);

          // The implied IDL resulting from this reference is
          // created here, in the template module scope. Upon
          // instantiation of the enclosing template module, the
          // visitor copies this implied IDL to the instantiated
          // module scope. The extra copy is less than ideal, but
          // otherwise we have ugly lookup issues when the
          // referenced template module's contents are referenced
          // using the aliased scoped name.
          if (v.visit_template_module_ref (tmr) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("visit_template_module_ref")
                          ACE_TEXT (" failed\n")));

              idl_global->set_err_count (idl_global->err_count () + 1);
            }

          idl_global->in_tmpl_mod_no_alias (itmna_flag);
          idl_global->in_tmpl_mod_alias (false);
        }
#line 2846 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 810 "fe/idl.ypp" /* yacc.c:1646  */
    {
// template_module_inst : template_module_header
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleSeen);
        }
#line 2856 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 816 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        at_least_one_actual_parameter '>'
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleArgsSeen);
        }
#line 2866 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 822 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        defining_id
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleIDSeen);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName *sn = (yyvsp[-5].idlist);
          AST_Template_Module *ref = 0;
          AST_Decl *d = s->lookup_by_name (sn);

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
          (yyvsp[-5].idlist) = 0;

          if (! ref->match_arg_names ((yyvsp[-3].alval)))
            {
              return 1;
            }

          ACE_NEW_RETURN (sn,
                          UTL_ScopedName ((yyvsp[0].idval),
                                           0),
                          1);

          AST_Template_Module_Inst *tmi =
            idl_global->gen ()->create_template_module_inst (
              sn,
              ref,
              (yyvsp[-3].alval));

          (void) s->fe_add_template_module_inst (tmi);

          ast_visitor_context ctx;
          ctx.template_args ((yyvsp[-3].alval));
          ast_visitor_tmpl_module_inst v (&ctx);

          if (v.visit_template_module_inst (tmi) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("visit_template_module_inst")
                          ACE_TEXT (" failed\n")));

              idl_global->set_err_count (idl_global->err_count () + 1);
            }
        }
#line 2933 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 893 "fe/idl.ypp" /* yacc.c:1646  */
    {
// interface : interface_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Interface *i = 0;

          /*
           * Make a new interface node and add it to its enclosing scope.
           */
          if (s != 0 && (yyvsp[0].ihval) != 0)
            {
              i =
                idl_global->gen ()->create_interface (
                                        (yyvsp[0].ihval)->name (),
                                        (yyvsp[0].ihval)->inherits (),
                                        (yyvsp[0].ihval)->n_inherits (),
                                        (yyvsp[0].ihval)->inherits_flat (),
                                        (yyvsp[0].ihval)->n_inherits_flat (),
                                        (yyvsp[0].ihval)->is_local (),
                                        (yyvsp[0].ihval)->is_abstract ()
                                      );
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the interface to its definition scope.
               */
              (void) s->fe_add_interface (i);

              // This FE_InterfaceHeader class isn't destroyed with the AST.
              (yyvsp[0].ihval)->destroy ();
              delete (yyvsp[0].ihval);
              (yyvsp[0].ihval) = 0;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (i);
        }
#line 2976 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 932 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
#line 2985 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 937 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
#line 2994 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 942 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
#line 3008 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 955 "fe/idl.ypp" /* yacc.c:1646  */
    {
// interface_decl : IDL_INTERFACE
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
#line 3017 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 960 "fe/idl.ypp" /* yacc.c:1646  */
    {
//       defining_id
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (yyval.idval) = (yyvsp[0].idval);
         }
#line 3027 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 969 "fe/idl.ypp" /* yacc.c:1646  */
    {
// interface_header : interface_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);

          if ((yyvsp[0].nlval) != 0 && (yyvsp[0].nlval)->truncatable ())
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
                          UTL_ScopedName ((yyvsp[-1].idval), 0),
                          1);
          ACE_NEW_RETURN ((yyval.ihval),
                          FE_InterfaceHeader (n,
                                              (yyvsp[0].nlval),
                                              false,
                                              false,
                                              true),
                          1);

          if (0 != (yyvsp[0].nlval))
            {
              (yyvsp[0].nlval)->destroy ();
              delete (yyvsp[0].nlval);
              (yyvsp[0].nlval) = 0;
            }
        }
#line 3068 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 1007 "fe/idl.ypp" /* yacc.c:1646  */
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
                          UTL_ScopedName ((yyvsp[-1].idval), 0),
                          1);
          ACE_NEW_RETURN ((yyval.ihval),
                          FE_InterfaceHeader (n,
                                              (yyvsp[0].nlval),
                                              true,
                                              false,
                                              true),
                          1);

          if (0 != (yyvsp[0].nlval))
            {
              (yyvsp[0].nlval)->destroy ();
              delete (yyvsp[0].nlval);
              (yyvsp[0].nlval) = 0;
            }
        }
#line 3102 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 1038 "fe/idl.ypp" /* yacc.c:1646  */
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
                          UTL_ScopedName ((yyvsp[-1].idval), 0),
                          1);
          ACE_NEW_RETURN ((yyval.ihval),
                          FE_InterfaceHeader (n,
                                              (yyvsp[0].nlval),
                                              false,
                                              true,
                                              true),
                          1);

          if (0 != (yyvsp[0].nlval))
            {
              (yyvsp[0].nlval)->destroy ();
              delete (yyvsp[0].nlval);
              (yyvsp[0].nlval) = 0;
            }
        }
#line 3136 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 1071 "fe/idl.ypp" /* yacc.c:1646  */
    {
// inheritance_spec : ':' opt_truncatable
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 3145 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 1076 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_scoped_name
          (yyvsp[0].nlval)->truncatable ((yyvsp[-2].bval));
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3155 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 1082 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.nlval) = 0;
        }
#line 3164 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 1097 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3179 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 1112 "fe/idl.ypp" /* yacc.c:1646  */
    {
// value_concrete_decl : value_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_ValueType *v = 0;
          AST_Interface *i = 0;

          if (s != 0 && (yyvsp[0].vhval) != 0)
            {
              v =
                idl_global->gen ()->create_valuetype (
                  (yyvsp[0].vhval)->name (),
                  (yyvsp[0].vhval)->inherits (),
                  (yyvsp[0].vhval)->n_inherits (),
                  (yyvsp[0].vhval)->inherits_concrete (),
                  (yyvsp[0].vhval)->inherits_flat (),
                  (yyvsp[0].vhval)->n_inherits_flat (),
                  (yyvsp[0].vhval)->supports (),
                  (yyvsp[0].vhval)->n_supports (),
                  (yyvsp[0].vhval)->supports_concrete (),
                  false,
                  (yyvsp[0].vhval)->truncatable (),
                  false);

              i = AST_Interface::narrow_from_decl (v);
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the valuetype to its definition scope
               */
              v = AST_ValueType::narrow_from_decl (i);
              (void) s->fe_add_valuetype (v);

              // FE_OBVHeader is not automatically destroyed in the AST
              (yyvsp[0].vhval)->destroy ();
              delete (yyvsp[0].vhval);
              (yyvsp[0].vhval) = 0;
            }

          /*
           * Push it on the scope stack
           */
          idl_global->scopes ().push (v);
        }
#line 3227 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 1156 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3236 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 1161 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3245 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 1166 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3269 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 1190 "fe/idl.ypp" /* yacc.c:1646  */
    {
// value_abs_decl : IDL_ABSTRACT value_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_ValueType *v = 0;
          AST_Interface *i = 0;

          if (s != 0 && (yyvsp[0].vhval) != 0)
            {
              v =
                idl_global->gen ()->create_valuetype (
                    (yyvsp[0].vhval)->name (),
                    (yyvsp[0].vhval)->inherits (),
                    (yyvsp[0].vhval)->n_inherits (),
                    (yyvsp[0].vhval)->inherits_concrete (),
                    (yyvsp[0].vhval)->inherits_flat (),
                    (yyvsp[0].vhval)->n_inherits_flat (),
                    (yyvsp[0].vhval)->supports (),
                    (yyvsp[0].vhval)->n_supports (),
                    (yyvsp[0].vhval)->supports_concrete (),
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

              // FE_OBVHeader is not automatically destroyed in the AST
              (yyvsp[0].vhval)->destroy ();
              delete (yyvsp[0].vhval);
              (yyvsp[0].vhval) = 0;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (v);
        }
#line 3317 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 1234 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3326 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 1239 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3335 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 1244 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 3349 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 1258 "fe/idl.ypp" /* yacc.c:1646  */
    {
// value_header : value_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 3358 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 1263 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);

          UTL_ScopedName *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_ScopedName ((yyvsp[-3].idval), 0),
                          1);
          ACE_NEW_RETURN ((yyval.vhval),
                          FE_OBVHeader (sn,
                                        (yyvsp[-2].nlval),
                                        (yyvsp[0].nlval),
                                        ((yyvsp[-2].nlval) != 0
                                           ? (yyvsp[-2].nlval)->truncatable ()
                                           : false)),
                          1);

          if (0 != (yyvsp[0].nlval))
            {
              (yyvsp[0].nlval)->destroy ();
              delete (yyvsp[0].nlval);
              (yyvsp[0].nlval) = 0;
            }

          if (0 != (yyvsp[-2].nlval))
            {
              (yyvsp[-2].nlval)->destroy ();
              delete (yyvsp[-2].nlval);
              (yyvsp[-2].nlval) = 0;
            }
        }
#line 3394 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 1298 "fe/idl.ypp" /* yacc.c:1646  */
    {
// value_decl : IDL_VALUETYPE
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
#line 3403 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 1303 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      defining_id
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 3413 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 1312 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_truncatable : IDL_TRUNCATABLE
          (yyval.bval) = true;
        }
#line 3422 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 1317 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.bval) = false;
        }
#line 3431 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 1326 "fe/idl.ypp" /* yacc.c:1646  */
    {
// supports_spec : IDL_SUPPORTS at_least_one_scoped_name
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3440 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 1331 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |    EMPTY */
          (yyval.nlval) = 0;
        }
#line 3449 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 1340 "fe/idl.ypp" /* yacc.c:1646  */
    {
// value_forward_decl : IDL_ABSTRACT value_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
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

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 3477 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 1365 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | value_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
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

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 3505 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 1392 "fe/idl.ypp" /* yacc.c:1646  */
    {
// value_box_decl : value_decl type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueBoxDeclSeen);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[-1].idval),
                            0);

          if (s != 0 && (yyvsp[0].dcval) != 0)
            {
              /*
              * Get the type_spec associated with the valuebox
              */
              AST_Type *tp = 0;
              AST_Typedef *td
                = AST_Typedef::narrow_from_decl ((yyvsp[0].dcval));

              if (td == 0)
                {
                  tp = AST_Type::narrow_from_decl ((yyvsp[0].dcval));
                }
              else
                {
                  tp = td->primitive_base_type ();
                }

              if (tp == 0)
                {
                  // The <type_spec> given is a valid type
                  idl_global->err ()->not_a_type ((yyvsp[0].dcval));
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

          (yyvsp[-1].idval)->destroy ();
          delete (yyvsp[-1].idval);
          (yyvsp[-1].idval) = 0;
        }
#line 3571 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 1469 "fe/idl.ypp" /* yacc.c:1646  */
    {
// state_member : IDL_PUBLIC
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_PUBLIC;
        }
#line 3581 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 1475 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        member_i
        }
#line 3589 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 1479 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_PRIVATE
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_PRIVATE;
        }
#line 3599 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 1485 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        member_i
        }
#line 3607 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 1497 "fe/idl.ypp" /* yacc.c:1646  */
    {
// export : type_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 3616 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 1502 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3625 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 1507 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | typeid_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 3634 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 1512 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3643 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 1517 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | typeprefix_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 3652 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 1522 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3661 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 1527 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | const_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 3670 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 1532 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
#line 3679 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 1537 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | exception
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 3688 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 1542 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3697 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 1547 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 3706 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 1552 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3715 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 1557 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | operation
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
#line 3724 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 1562 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3733 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 1567 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 3742 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 1572 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 3752 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 1581 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_scoped_name : scoped_name scoped_names
          ACE_NEW_RETURN ((yyval.nlval),
                          UTL_NameList ((yyvsp[-1].idlist),
                                        (yyvsp[0].nlval)),
                          1);
        }
#line 3764 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 1593 "fe/idl.ypp" /* yacc.c:1646  */
    {
// scoped_names : scoped_names ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
#line 3773 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 1598 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      scoped_name
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopedNameSeen);

          UTL_NameList *nl = 0;
          ACE_NEW_RETURN (nl,
                          UTL_NameList ((yyvsp[0].idlist),
                                        0),
                          1);

          if ((yyvsp[-3].nlval) == 0)
            {
              (yyval.nlval) = nl;
            }
          else
            {
              (yyvsp[-3].nlval)->nconc (nl);
              (yyval.nlval) = (yyvsp[-3].nlval);
            }
        }
#line 3798 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 1619 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.nlval) = 0;
        }
#line 3807 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 1627 "fe/idl.ypp" /* yacc.c:1646  */
    {
// scoped_name : id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN ((yyval.idlist),
                          UTL_IdList ((yyvsp[0].idval),
                                      0),
                          1);
        }
#line 3821 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 1637 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_SCOPE_DELIMITOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
#line 3830 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 1642 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          Identifier *id = 0;
          ACE_NEW_RETURN (id,
                          Identifier ((yyvsp[-2].strval)),
                          1);
          ACE::strdelete ((yyvsp[-2].strval));
          (yyvsp[-2].strval) = 0;
          UTL_IdList *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_IdList ((yyvsp[0].idval),
                                      0),
                          1);
          ACE_NEW_RETURN ((yyval.idlist),
                          UTL_IdList (id,
                                      sn),
                          1);
        }
#line 3855 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 1664 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | scoped_name IDL_SCOPE_DELIMITOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);

          // This cleans up all the non-global "::"s in scoped names.
          // If there is a global one, it gets put into the UTL_IdList,
          // so we clean it up in the case above.
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3870 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 1675 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          UTL_IdList *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_IdList ((yyvsp[0].idval),
                                      0),
                          1);
          (yyvsp[-3].idlist)->nconc (sn);
          (yyval.idlist) = (yyvsp[-3].idlist);
        }
#line 3887 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 1690 "fe/idl.ypp" /* yacc.c:1646  */
    {
// id: IDENTIFIER
          ACE_NEW_RETURN ((yyval.idval),
                          Identifier ((yyvsp[0].strval)),
                          1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3900 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 1701 "fe/idl.ypp" /* yacc.c:1646  */
    {
          /* defining_id is a defining identifier
             whereas id is usually a reference to a defining identifier */
          ACE_NEW_RETURN ((yyval.idval),
                          Identifier ((yyvsp[0].strval)),
                          1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3914 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 1714 "fe/idl.ypp" /* yacc.c:1646  */
    {
// interface_forward : interface_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
                            0);

          if (ACE_OS::strcmp ((yyvsp[0].idval)->get_string (),
                              "TypeCode") == 0
              && !idl_global->in_main_file ())
            {
              AST_PredefinedType *pdt =
                idl_global->gen ()->create_predefined_type (
                                        AST_PredefinedType::PT_pseudo,
                                        &n
                                      );

              s->add_to_scope (pdt);

              (yyvsp[0].idval)->destroy ();
              delete (yyvsp[0].idval);
              (yyvsp[0].idval) = 0;

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

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 3963 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 1760 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_LOCAL interface_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
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

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 3994 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 1788 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_ABSTRACT interface_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
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

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 4025 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 1818 "fe/idl.ypp" /* yacc.c:1646  */
    {
// const_dcl : IDL_CONST
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
#line 4034 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 1823 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      const_type
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
#line 4043 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 1828 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      defining_id
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
#line 4052 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 1833 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '='
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
#line 4061 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 1838 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      expression
          UTL_ScopedName n ((yyvsp[-4].idval),
                            0);
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Constant *c = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstExprSeen);

          /*
           * Create a node representing a constant declaration. Store
           * it in the enclosing scope.
           */
          if ((yyvsp[0].exval) != 0 && s != 0)
            {
              AST_Param_Holder *param_holder =
                (yyvsp[0].exval)->param_holder ();

              AST_Expression::AST_ExprValue *result =
                (yyvsp[0].exval)->check_and_coerce ((yyvsp[-6].etval),
                                      tao_enum_constant_decl);
              tao_enum_constant_decl = 0;

              // If the expression is a template parameter place
              // holder, 'result' will be 0, but it's ok.
              if (result == 0 && param_holder == 0)
                {
                  idl_global->err ()->coercion_error ((yyvsp[0].exval),
                                                      (yyvsp[-6].etval));
                  (yyvsp[0].exval)->destroy ();
                  delete (yyvsp[0].exval);
                  (yyvsp[0].exval) = 0;
                }
              else
                {
                  AST_Expression::ExprType et =
                    (yyvsp[-6].etval);

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
                          (yyvsp[-6].etval),
                          (yyvsp[0].exval),
                          &n);

                      (void) s->fe_add_constant (c);
                    }
                }

              (yyvsp[-4].idval)->destroy ();
              delete (yyvsp[-4].idval);
              (yyvsp[-4].idval) = 0;

              delete result;
              result = 0;
            }
        }
#line 4129 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 1911 "fe/idl.ypp" /* yacc.c:1646  */
    {
// const_type
//      : integer_type
//      | char_type
//      | octet_type
//      | boolean_type
//      | floating_pt_type
//      | fixed_type
//      | string_type_spec
          (yyval.etval) = AST_Expression::EV_string;
        }
#line 4145 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 1923 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | wstring_type_spec
          (yyval.etval) = AST_Expression::EV_wstring;
        }
#line 4154 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 1928 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | scoped_name
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_PredefinedType *c = 0;
          AST_Typedef *t = 0;
          UTL_ScopedName *sn = (yyvsp[0].idlist);

          /*
           * If the constant's type is a scoped name, it must resolve
           * to a scalar constant type
           */
          AST_Decl *d =
            s->lookup_by_name (sn);

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

                  (yyval.etval) = FE_Utils::PredefinedTypeToExprType (c->pt ());
                }
              else if (d->node_type () == AST_Decl::NT_string)
                {
                  (yyval.etval) = AST_Expression::EV_string;
                }
              else if (d->node_type () == AST_Decl::NT_wstring)
                {
                  (yyval.etval) = AST_Expression::EV_wstring;
                }
              else if (d->node_type () == AST_Decl::NT_enum)
                {
                  (yyval.etval) = AST_Expression::EV_enum;
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
          (yyvsp[0].idlist) = 0;
        }
#line 4225 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 2002 "fe/idl.ypp" /* yacc.c:1646  */
    {
// or_expr : xor_expr
//      | or_expr '|' xor_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_or,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4240 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 2017 "fe/idl.ypp" /* yacc.c:1646  */
    {
// xor_expr : and_expr
//      | xor_expr '^' and_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_xor,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4255 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 2032 "fe/idl.ypp" /* yacc.c:1646  */
    {
// and_expr : shift_expr | and_expr '&' shift_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_and,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4269 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 2046 "fe/idl.ypp" /* yacc.c:1646  */
    {
// shift_expr : add_expr | shift_expr IDL_LEFT_SHIFT add_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_left,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4283 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 2056 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | shift_expr IDL_RIGHT_SHIFT add_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_right,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4297 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 2070 "fe/idl.ypp" /* yacc.c:1646  */
    {
// add_expr : mult_expr | add_expr '+' mult_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_add,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4311 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 2080 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | add_expr '-' mult_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_minus,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4325 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 2094 "fe/idl.ypp" /* yacc.c:1646  */
    {
// mult_expr : unary_expr | mult_expr '*' unary_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mul,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4339 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 2104 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | mult_expr '/' unary_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_div,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4353 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 2114 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | mult_expr '%' unary_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mod,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4367 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 2128 "fe/idl.ypp" /* yacc.c:1646  */
    {
// unary_expr : primary_expr | '+' primary_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_u_plus,
                                    (yyvsp[0].exval),
                                    0
                                  );
        }
#line 4381 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 2138 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | '-' primary_expr
          (yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_u_minus,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4395 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 2148 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | '~' primary_expr
          (yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_bit_neg,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4409 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 177:
#line 2161 "fe/idl.ypp" /* yacc.c:1646  */
    {
// primary_expr : scoped_name
          /*
           * An expression which is a scoped name is not resolved now,
           * but only when it is evaluated (such as when it is assigned
           * as a constant value).
           */
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d =
            s->lookup_by_name ((yyvsp[0].idlist));

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[0].idlist));
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

              (yyval.exval) =
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
              (yyval.exval) =
                idl_global->gen ()->create_expr (
                  (yyvsp[0].idlist)->copy ());
            }

          (yyvsp[0].idlist)->destroy ();
          delete (yyvsp[0].idlist);
          (yyvsp[0].idlist) = 0;
        }
#line 4464 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 2213 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | literal
//      | '(' const_expr ')'
          (yyval.exval) = (yyvsp[-1].exval);
        }
#line 4474 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 2222 "fe/idl.ypp" /* yacc.c:1646  */
    {
// literal : IDL_INTEGER_LITERAL
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].ival));
        }
#line 4483 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 2227 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_UINTEGER_LITERAL
          (yyval.exval) =
            idl_global->gen ()->create_expr ((yyvsp[0].uival));
        }
#line 4493 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 2233 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_STRING_LITERAL
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].sval));
          (yyvsp[0].sval)->destroy ();
          delete (yyvsp[0].sval);
          (yyvsp[0].sval) = 0;
        }
#line 4505 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 2241 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_WSTRING_LITERAL
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].wsval));
        }
#line 4514 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 2246 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_CHARACTER_LITERAL
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].cval));
        }
#line 4523 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 2251 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_WCHAR_LITERAL
          ACE_OutputCDR::from_wchar wc ((yyvsp[0].wcval));
          (yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
#line 4533 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 2257 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].fixval));
        }
#line 4541 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 2261 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_FLOATING_PT_LITERAL
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].dval));
        }
#line 4550 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 2266 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_TRUETOK
          (yyval.exval) = idl_global->gen ()->create_expr (true);
        }
#line 4559 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 2271 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_FALSETOK
          (yyval.exval) = idl_global->gen ()->create_expr (false);
        }
#line 4568 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 2279 "fe/idl.ypp" /* yacc.c:1646  */
    {
// positive_int_expr : const_expr
          int good_expression = 1;
          (yyvsp[0].exval)->evaluate (AST_Expression::EK_positive_int);
          AST_Expression::AST_ExprValue *ev = (yyvsp[0].exval)->ev ();

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
                case AST_Expression::EV_ulonglong:
                  if (ev->u.ullval == 0)
                    {
                      good_expression = 0;
                    }

                  break;
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
              (yyval.exval) = (yyvsp[0].exval);
            }
          else
            {
              idl_global->err ()->syntax_error (idl_global->parse_state ());
            }
        }
#line 4638 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 2381 "fe/idl.ypp" /* yacc.c:1646  */
    {
// type_dcl : IDL_TYPEDEF
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 4648 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 2387 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      type_declarator
          (yyval.dcval) = 0;
        }
#line 4657 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 2392 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | struct_type
          (yyval.dcval) = 0;
        }
#line 4666 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 2397 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | union_type
          (yyval.dcval) = 0;
        }
#line 4675 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 2402 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | enum_type
          (yyval.dcval) = 0;
        }
#line 4684 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 2407 "fe/idl.ypp" /* yacc.c:1646  */
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
                                        (yyvsp[0].deval)->name ()
                                      );
              /*
               * Add it to its defining scope
               */
              (void) s->fe_add_native (node);
            }

          (yyvsp[0].deval)->destroy ();
          delete (yyvsp[0].deval);
          (yyvsp[0].deval) = 0;
        }
#line 4715 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 2434 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | constructed_forward_type_spec
          (yyval.dcval) = 0;
        }
#line 4724 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 2442 "fe/idl.ypp" /* yacc.c:1646  */
    {
// type_declarator : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
#line 4733 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 2447 "fe/idl.ypp" /* yacc.c:1646  */
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
          if (s != 0 && (yyvsp[-2].dcval) != 0 && (yyvsp[0].dlval) != 0)
            {
              for (UTL_DecllistActiveIterator l ((yyvsp[0].dlval));
                   !l.is_done ();
                   l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type * tp = d->compose ((yyvsp[-2].dcval));

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
                  idl_global->in_typedef (false);
                }

              // This FE_Declarator class isn't destroyed with the AST.
              (yyvsp[0].dlval)->destroy ();
              delete (yyvsp[0].dlval);
              (yyvsp[0].dlval) = 0;
            }
        }
#line 4801 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 2519 "fe/idl.ypp" /* yacc.c:1646  */
    {
// simple_type_spec : base_type_spec
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 4813 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 214:
#line 2528 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | template_type_spec
//      | scoped_name
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = 0;

          if (s != 0)
            {
              d =
                s->lookup_by_name ((yyvsp[0].idlist));
            }

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[0].idlist));
            }
          else
            {
              d->last_referenced_as ((yyvsp[0].idlist));
            }


          (yyval.dcval) = d;
        }
#line 4842 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 232:
#line 2585 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_declarator : declarator declarators
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 4854 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 233:
#line 2597 "fe/idl.ypp" /* yacc.c:1646  */
    {
// declarators : declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 4863 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 234:
#line 2602 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsDeclSeen);

          UTL_DeclList *dl = 0;
          ACE_NEW_RETURN (dl,
                          UTL_DeclList ((yyvsp[0].deval),
                                        0),
                          1);

          if ((yyvsp[-3].dlval) == 0)
            {
              (yyval.dlval) = dl;
            }
          else
            {
              (yyvsp[-3].dlval)->nconc (dl);
              (yyval.dlval) = (yyvsp[-3].dlval);
            }
        }
#line 4888 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 235:
#line 2623 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.dlval) = 0;
        }
#line 4897 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 238:
#line 2636 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_simple_declarator : simple_declarator simple_declarators
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 4909 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 239:
#line 2648 "fe/idl.ypp" /* yacc.c:1646  */
    {
// simple_declarators : simple_declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 4918 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 240:
#line 2653 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsDeclSeen);

          UTL_DeclList *dl = 0;
          ACE_NEW_RETURN (dl,
                          UTL_DeclList ((yyvsp[0].deval),
                                        0),
                          1);

          if ((yyvsp[-3].dlval) == 0)
            {
              (yyval.dlval) = dl;
            }
          else
            {
              (yyvsp[-3].dlval)->nconc (dl);
              (yyval.dlval) = (yyvsp[-3].dlval);
            }
        }
#line 4943 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 241:
#line 2674 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.dlval) = 0;
        }
#line 4952 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 242:
#line 2682 "fe/idl.ypp" /* yacc.c:1646  */
    {
// simple_declarator : defining_id
          UTL_ScopedName *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_ScopedName ((yyvsp[0].idval),
                                          0),
                          1);
          ACE_NEW_RETURN ((yyval.deval),
                          FE_Declarator (sn,
                                         FE_Declarator::FD_simple,
                                         0),
                          1);
        }
#line 4970 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 243:
#line 2699 "fe/idl.ypp" /* yacc.c:1646  */
    {
// complex_declarator : array_declarator
          UTL_ScopedName *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_ScopedName (
                              (yyvsp[0].dcval)->local_name ()->copy (),
                              0
                            ),
                          1);
          ACE_NEW_RETURN ((yyval.deval),
                          FE_Declarator (sn,
                                         FE_Declarator::FD_complex,
                                         (yyvsp[0].dcval)),
                          1);
        }
#line 4990 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 246:
#line 2723 "fe/idl.ypp" /* yacc.c:1646  */
    {
// signed_int : IDL_LONG
          (yyval.etval) = AST_Expression::EV_long;
        }
#line 4999 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 247:
#line 2728 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_LONG IDL_LONG
          (yyval.etval) = AST_Expression::EV_longlong;
        }
#line 5008 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 248:
#line 2733 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_SHORT
          (yyval.etval) = AST_Expression::EV_short;
        }
#line 5017 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 249:
#line 2741 "fe/idl.ypp" /* yacc.c:1646  */
    {
// unsigned_int : IDL_UNSIGNED IDL_LONG
          (yyval.etval) = AST_Expression::EV_ulong;
        }
#line 5026 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 250:
#line 2746 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_UNSIGNED IDL_LONG IDL_LONG
          (yyval.etval) = AST_Expression::EV_ulonglong;
        }
#line 5035 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 251:
#line 2751 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_UNSIGNED IDL_SHORT
          (yyval.etval) = AST_Expression::EV_ushort;
        }
#line 5044 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 252:
#line 2759 "fe/idl.ypp" /* yacc.c:1646  */
    {
// floating_pt_type : IDL_DOUBLE
          (yyval.etval) = AST_Expression::EV_double;
        }
#line 5053 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 253:
#line 2764 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_FLOAT
          (yyval.etval) = AST_Expression::EV_float;
        }
#line 5062 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 254:
#line 2769 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_LONG IDL_DOUBLE
          (yyval.etval) = AST_Expression::EV_longdouble;
        }
#line 5071 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 255:
#line 2777 "fe/idl.ypp" /* yacc.c:1646  */
    {
// fixed_type : IDL_FIXED
          (yyval.etval) = AST_Expression::EV_fixed;
        }
#line 5080 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 256:
#line 2785 "fe/idl.ypp" /* yacc.c:1646  */
    {
// char_type : IDL_CHAR
          (yyval.etval) = AST_Expression::EV_char;
        }
#line 5089 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 257:
#line 2790 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_WCHAR
          (yyval.etval) = AST_Expression::EV_wchar;
        }
#line 5098 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 258:
#line 2798 "fe/idl.ypp" /* yacc.c:1646  */
    {
// octet_type : IDL_OCTET
          (yyval.etval) = AST_Expression::EV_octet;
        }
#line 5107 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 259:
#line 2806 "fe/idl.ypp" /* yacc.c:1646  */
    {
// boolean_type : IDL_BOOLEAN
          (yyval.etval) = AST_Expression::EV_bool;
        }
#line 5116 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 260:
#line 2814 "fe/idl.ypp" /* yacc.c:1646  */
    {
// any_type : IDL_ANY
          (yyval.etval) = AST_Expression::EV_any;
        }
#line 5125 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 261:
#line 2822 "fe/idl.ypp" /* yacc.c:1646  */
    {
// object_type : IDL_OBJECT
          (yyval.etval) = AST_Expression::EV_object;
        }
#line 5134 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 262:
#line 2830 "fe/idl.ypp" /* yacc.c:1646  */
    {
// struct_decl : IDL_STRUCT
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
#line 5143 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 263:
#line 2835 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      defining_id
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5153 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 264:
#line 2845 "fe/idl.ypp" /* yacc.c:1646  */
    {
// struct_type : struct_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
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

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 5191 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 265:
#line 2879 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
#line 5200 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 266:
#line 2884 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_member
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
#line 5209 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 267:
#line 2889 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_StructQsSeen);

          /*
           * Done with this struct. Pop its scope off the scopes stack.
           */
          (yyval.dcval) = AST_Structure::narrow_from_scope (
                   idl_global->scopes ().top_non_null ()
                 );
          idl_global->scopes ().pop ();
        }
#line 5226 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 271:
#line 2911 "fe/idl.ypp" /* yacc.c:1646  */
    {
// member  :
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_NA;
        }
#line 5236 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 273:
#line 2922 "fe/idl.ypp" /* yacc.c:1646  */
    {
// member_i : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
#line 5245 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 274:
#line 2927 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
#line 5254 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 275:
#line 2932 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          FE_Declarator *d = 0;
          AST_Field *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsCompleted);

          /*
           * Check for illegal recursive use of type.
           */
          if ((yyvsp[-4].dcval) != 0
              && AST_illegal_recursive_type ((yyvsp[-4].dcval)))
            {
              idl_global->err ()->error1 (UTL_Error::EIDL_RECURSIVE_TYPE,
                                          (yyvsp[-4].dcval));
            }
          /*
           * Create a node representing a struct or exception member
           * Add it to the enclosing scope.
           */
          else if (s != 0
                   && (yyvsp[-4].dcval) != 0
                   && (yyvsp[-2].dlval) != 0)
            {
              for (UTL_DecllistActiveIterator l ((yyvsp[-2].dlval));
                   !l.is_done ();
                   l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type *tp = d->compose ((yyvsp[-4].dcval));

                  if (tp == 0)
                    {
                      continue;
                    }

                  /* $0 denotes Visibility, must be on yacc reduction stack. */
                  f =
                    idl_global->gen ()->create_field (
                                            tp,
                                            d->name (),
                                            (yyvsp[-6].vival)
                                          );
                  (void) s->fe_add_field (f);
                }
            }

          (yyvsp[-2].dlval)->destroy ();
          delete (yyvsp[-2].dlval);
          (yyvsp[-2].dlval) = 0;
        }
#line 5316 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 276:
#line 2990 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state ());
        }
#line 5325 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 277:
#line 2995 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 5335 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 278:
#line 3004 "fe/idl.ypp" /* yacc.c:1646  */
    {
// union_decl : IDL_UNION
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
#line 5344 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 279:
#line 3009 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      defining_id
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5354 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 280:
#line 3018 "fe/idl.ypp" /* yacc.c:1646  */
    {
// union_type : union_decl IDL_SWITCH
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
#line 5363 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 281:
#line 3023 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '('
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[-3].idval),
                            0);
          AST_Union *u = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchOpenParSeen);

          /*
           * Create a node representing an empty union. Add it to its enclosing
           * scope.
           */
          if (s != 0)
            {
              u = idl_global->gen ()->create_union (0,
                                                    &n,
                                                    s->is_local (),
                                                    s->is_abstract ());

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

          /*
           * Don't delete $1 yet; we'll need it a bit later.
           */
        }
#line 5403 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 282:
#line 3059 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      switch_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
#line 5412 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 283:
#line 3064 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ')'
          /*
           * The top of the scopes must be an empty union we added after we
           * encountered 'union <id> switch ('. Now we are ready to add a
           * correct one. Temporarily remove the top so that we setup the
           * correct union in a right scope.
           */
          UTL_Scope *top = idl_global->scopes ().top_non_null ();
          idl_global->scopes ().pop ();

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[-7].idval),
                            0);
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchCloseParSeen);

          /*
           * Create a node representing a union.
           */
          if ((yyvsp[-2].dcval) != 0
              && s != 0)
            {
              AST_ConcreteType *tp =
                AST_ConcreteType::narrow_from_decl ((yyvsp[-2].dcval));

              if (tp == 0)
                {
                  idl_global->err ()->not_a_type ((yyvsp[-2].dcval));
                }
              else
                {
                  /* Create a union with a correct discriminator. */
                  AST_Union *u = 0;
                  u = idl_global->gen ()->create_union (tp,
                                                        &n,
                                                        s->is_local (),
                                                        s->is_abstract ());
                  /* Narrow the enclosing scope. */
                  AST_Union *e = AST_Union::narrow_from_scope (top);

                  e->redefine (u);

                  u->destroy ();
                  delete u;
                }
            }

          /*
           * Restore the top.
           */
          idl_global->scopes ().push (top);

          (yyvsp[-7].idval)->destroy ();
          delete (yyvsp[-7].idval);
          (yyvsp[-7].idval) = 0;
        }
#line 5473 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 284:
#line 3121 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
#line 5482 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 285:
#line 3126 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_case_branch
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
#line 5491 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 286:
#line 3131 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionQsSeen);

          /*
           * Done with this union. Pop its scope from the scopes stack.
           */
          (yyval.dcval) = AST_Union::narrow_from_scope (
                   idl_global->scopes ().top_non_null ()
                 );

          if ((yyval.dcval) != 0)
            {
              idl_global->scopes ().pop ();
            }
        }
#line 5512 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 287:
#line 3151 "fe/idl.ypp" /* yacc.c:1646  */
    {
// switch_type_spec : integer_type
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5524 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 288:
#line 3159 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | char_type
          /* wchars are not allowed. */
          if ((yyvsp[0].etval) == AST_Expression::EV_wchar)
            {
              idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
            }

          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5542 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 289:
#line 3173 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | octet_type
          /* octets are not allowed. */
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5556 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 290:
#line 3183 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | boolean_type
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5568 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 292:
#line 3192 "fe/idl.ypp" /* yacc.c:1646  */
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
          d =
            s->lookup_by_name ((yyvsp[0].idlist));

          if (s != 0 && d != 0)
            {
              while (!found)
                {
                  switch (d->node_type ())
                  {
                    case AST_Decl::NT_enum:
                      (yyval.dcval) = d;
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
                              (yyval.dcval) = p;
                              found = true;
                              break;
                            case AST_PredefinedType::PT_wchar:
                            case AST_PredefinedType::PT_octet:
                              /* octets and wchars are not allowed */
                              idl_global->err ()->error0 (
                                  UTL_Error::EIDL_DISC_TYPE
                                );
                              (yyval.dcval) = 0;
                              found = true;
                              break;
                            default:
                              (yyval.dcval) = 0;
                              found = true;
                              break;
                          }
                        }
                      else
                        {
                          (yyval.dcval) = 0;
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
                      (yyval.dcval) = 0;
                      found = true;
                      break;
                  }
                }
            }
          else
            {
              (yyval.dcval) = 0;
            }

          if ((yyval.dcval) == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[0].idlist));

              (yyvsp[0].idlist)->destroy ();
              delete (yyvsp[0].idlist);
              (yyvsp[0].idlist) = 0;

              /* If we don't return here, we'll crash later.*/
              return 1;
            }

          (yyvsp[0].idlist)->destroy ();
          delete (yyvsp[0].idlist);
          (yyvsp[0].idlist) = 0;
        }
#line 5678 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 296:
#line 3308 "fe/idl.ypp" /* yacc.c:1646  */
    {
// case_branch : at_least_one_case_label
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
#line 5687 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 297:
#line 3313 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      annotations element_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
#line 5696 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 298:
#line 3318 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_UnionBranch *b = 0;
          AST_Field *f = (yyvsp[-2].ffval);
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemCompleted);

          /*
           * Create several nodes representing branches of a union.
           * Add them to the enclosing scope (the union scope)
           */
          if (s != 0
              &&  (yyvsp[-5].llval) != 0
              && (yyvsp[-2].ffval) != 0)
            {
              b =
                idl_global->gen ()->create_union_branch (
                                        (yyvsp[-5].llval),
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
#line 5731 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 299:
#line 3349 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 5740 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 300:
#line 3354 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 5750 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 301:
#line 3363 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_case_label : case_label case_labels
          ACE_NEW_RETURN ((yyval.llval),
                          UTL_LabelList ((yyvsp[-1].ulval),
                                         (yyvsp[0].llval)),
                          1);
        }
#line 5762 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 302:
#line 3374 "fe/idl.ypp" /* yacc.c:1646  */
    {
// case_labels : case_labels annotations case_label
          UTL_LabelList *ll = 0;
          ACE_NEW_RETURN (ll,
                          UTL_LabelList ((yyvsp[0].ulval),
                                         0),
                          1);

          if ((yyvsp[-2].llval) == 0)
            {
              (yyval.llval) = ll;
            }
          else
            {
              (yyvsp[-2].llval)->nconc (ll);
              (yyval.llval) = (yyvsp[-2].llval);
            }
        }
#line 5785 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 303:
#line 3393 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.llval) = 0;
        }
#line 5794 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 304:
#line 3401 "fe/idl.ypp" /* yacc.c:1646  */
    {
// case_label : IDL_DEFAULT
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
#line 5803 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 305:
#line 3406 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
#line 5817 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 306:
#line 3416 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_CASE
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
#line 5826 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 307:
#line 3421 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
#line 5834 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 308:
#line 3425 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      const_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      (yyvsp[-2].exval)
                                    );
        }
#line 5848 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 309:
#line 3438 "fe/idl.ypp" /* yacc.c:1646  */
    {
// element_spec : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
#line 5857 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 310:
#line 3443 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemDeclSeen);

          /*
           * Check for illegal recursive use of type
           */
          if ((yyvsp[-2].dcval) != 0
              && AST_illegal_recursive_type ((yyvsp[-2].dcval)))
            {
              idl_global->err()->error1 (UTL_Error::EIDL_RECURSIVE_TYPE,
                                         (yyvsp[-2].dcval));

              (yyval.ffval) = 0;
            }
          /*
           * Create a field in a union branch
           */
          else if ((yyvsp[-2].dcval) == 0
                   || (yyvsp[0].deval) == 0)
            {
              (yyval.ffval) = 0;
            }
          else
            {
              AST_Type *tp = (yyvsp[0].deval)->compose ((yyvsp[-2].dcval));

              if (tp == 0)
                {
                  (yyval.ffval) = 0;
                }
              else
                {
                  (yyval.ffval) = idl_global->gen ()->create_field (
                                               tp,
                                               (yyvsp[0].deval)->name ()
                                             );
                }

              (yyvsp[0].deval)->destroy ();
              delete (yyvsp[0].deval);
              (yyvsp[0].deval) = 0;
            }
        }
#line 5906 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 311:
#line 3491 "fe/idl.ypp" /* yacc.c:1646  */
    {
// struct_forward_type : struct_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
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

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 5931 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 312:
#line 3515 "fe/idl.ypp" /* yacc.c:1646  */
    {
// union_forward_type : union_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
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

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 5956 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 313:
#line 3539 "fe/idl.ypp" /* yacc.c:1646  */
    {
// enum_type : IDL_ENUM
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
#line 5965 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 314:
#line 3544 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      defining_id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
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

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 6002 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 315:
#line 3577 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
#line 6011 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 316:
#line 3582 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_enumerator
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
#line 6020 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 317:
#line 3587 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumQsSeen);

          /*
           * Done with this enum. Pop its scope from the scopes stack.
           */
          if (idl_global->scopes ().top () == 0)
            {
              (yyval.dcval) = 0;
            }
          else
            {
              (yyval.dcval) = AST_Enum::narrow_from_scope (
                       idl_global->scopes ().top_non_null ()
                     );
              idl_global->scopes ().pop ();
            }
        }
#line 6044 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 319:
#line 3613 "fe/idl.ypp" /* yacc.c:1646  */
    {
// enumerators : enumerators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
#line 6053 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 322:
#line 3623 "fe/idl.ypp" /* yacc.c:1646  */
    {
// enumerator : annotations IDENTIFIER
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          Identifier id ((yyvsp[0].strval));
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
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
#line 6089 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 323:
#line 3659 "fe/idl.ypp" /* yacc.c:1646  */
    {
// sequence_type_spec : seq_head ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
#line 6098 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 324:
#line 3664 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
#line 6107 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 325:
#line 3669 "fe/idl.ypp" /* yacc.c:1646  */
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

          if ((yyvsp[-2].exval) != 0)
            {
              param_holder =
                (yyvsp[-2].exval)->param_holder ();

              ev = (yyvsp[-2].exval)->coerce (AST_Expression::EV_ulong);
            }

          // If the expression corresponds to a template parameter,
          // it's ok for the coercion to fail at this point. We check
          // for a type mismatch below.
          if (0 == (yyvsp[-2].exval)
              || (0 == ev && 0 == param_holder))
            {
              idl_global->err ()->coercion_error ((yyvsp[-2].exval),
                                                  AST_Expression::EV_ulong);
              (yyval.dcval) = 0;
            }
          else if (0 == (yyvsp[-5].dcval))
            {
              (yyval.dcval) = 0;
            }
          else
            {
              AST_Type *tp = AST_Type::narrow_from_decl ((yyvsp[-5].dcval));

              if (0 == tp)
                {
                  ; // Error will be caught in FE_Declarator.
                }
              else
                {
                  Identifier id ("sequence");
                  UTL_ScopedName sn (&id,
                                     0);

                  (yyval.dcval) =
                    idl_global->gen ()->create_sequence (
                                            (yyvsp[-2].exval),
                                            tp,
                                            &sn,
                                            s->is_local (),
                                            s->is_abstract ()
                                          );

                  if (!idl_global->in_typedef ()
                      && !idl_global->anon_silent ())
                    {
                      idl_global->err ()->anonymous_type_diagnostic ();
                    }
                }
            }

          delete ev;
          ev = 0;
        }
#line 6188 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 326:
#line 3747 "fe/idl.ypp" /* yacc.c:1646  */
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
          if ((yyvsp[-1].dcval) == 0)
            {
              (yyval.dcval) = 0;
            }
          else
            {
              AST_Type *tp = AST_Type::narrow_from_decl ((yyvsp[-1].dcval));

              if (tp == 0)
                {
                  ; // Error will be caught in FE_Declarator.
                }
              else
                {
                  Identifier id ("sequence");
                  UTL_ScopedName sn (&id, 0);
                  ACE_CDR::ULong bound = 0UL;

                  (yyval.dcval) =
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

                  if (!idl_global->in_typedef ()
                      && !idl_global->anon_silent ())
                    {
                      idl_global->err ()->anonymous_type_diagnostic ();
                    }
                }
            }
        }
#line 6248 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 327:
#line 3806 "fe/idl.ypp" /* yacc.c:1646  */
    {
// seq_head : IDL_SEQUENCE
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
#line 6262 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 328:
#line 3816 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
#line 6271 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 329:
#line 3821 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      simple_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 6281 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 330:
#line 3830 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyvsp[-1].exval)->evaluate (AST_Expression::EK_positive_int);
          (yyval.dcval) = idl_global->gen ()->create_fixed ((yyvsp[-3].exval), (yyvsp[-1].exval));
        }
#line 6290 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 331:
#line 3839 "fe/idl.ypp" /* yacc.c:1646  */
    {
// string_type_spec : string_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 6299 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 332:
#line 3844 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 6308 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 333:
#line 3849 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '>'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringQsSeen);

          /*
           * Create a node representing a string.
           */
          AST_Expression::AST_ExprValue *ev = 0;

          if ((yyvsp[-2].exval) != 0)
            {
              ev = (yyvsp[-2].exval)->coerce (AST_Expression::EV_ulong);
            }

          if (0 == (yyvsp[-2].exval) || 0 == ev)
            {
              idl_global->err ()->coercion_error ((yyvsp[-2].exval),
                                                  AST_Expression::EV_ulong);
              (yyval.dcval) = 0;
            }
          else
            {
              (yyval.dcval) = idl_global->gen ()->create_string ((yyvsp[-2].exval));
              /*
               * Add this AST_String to the types defined in the global scope.
               */
              (void) idl_global->root ()->fe_add_string (
                                              AST_String::narrow_from_decl (
                                                  (yyval.dcval)
                                                )
                                            );

              if (!idl_global->in_typedef ()
                  && !idl_global->anon_silent ())
                {
                  idl_global->err ()->anonymous_type_diagnostic ();
                }
            }

          delete ev;
          ev = 0;
        }
#line 6355 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 334:
#line 3892 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | string_head
          idl_global->set_parse_state (IDL_GlobalData::PS_StringCompleted);
          /*
           * Create a node representing a string.
           */
          ACE_CDR::ULong bound = 0UL;

          (yyval.dcval) =
            idl_global->gen ()->create_string (
                idl_global->gen ()->create_expr (bound,
                                                 AST_Expression::EV_ulong)
              );
          /*
           * Add this AST_String to the types defined in the global scope.
           */
          (void) idl_global->root ()->fe_add_string (
                                          AST_String::narrow_from_decl (
                                              (yyval.dcval)
                                            )
                                        );
        }
#line 6382 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 335:
#line 3918 "fe/idl.ypp" /* yacc.c:1646  */
    {
// string_head : IDL_STRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 6391 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 336:
#line 3927 "fe/idl.ypp" /* yacc.c:1646  */
    {
// wstring_type_spec : wstring_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 6400 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 337:
#line 3932 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 6409 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 338:
#line 3937 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '>'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringQsSeen);

          /*
           * Create a node representing a string.
           */
          if ((yyvsp[-2].exval) == 0
              || (yyvsp[-2].exval)->coerce (AST_Expression::EV_ulong) == 0)
            {
              idl_global->err ()->coercion_error ((yyvsp[-2].exval),
                                                  AST_Expression::EV_ulong);
              (yyval.dcval) = 0;
            }
          else
            {
              (yyval.dcval) = idl_global->gen ()->create_wstring ((yyvsp[-2].exval));
              /*
               * Add this AST_String to the types defined in the global scope.
               */
              (void) idl_global->root ()->fe_add_string (
                                              AST_String::narrow_from_decl ((yyval.dcval))
                                            );

              if (!idl_global->in_typedef ()
                  && !idl_global->anon_silent ())
                {
                  idl_global->err ()->anonymous_type_diagnostic ();
                }
            }
        }
#line 6445 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 339:
#line 3969 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | wstring_head
          idl_global->set_parse_state (IDL_GlobalData::PS_StringCompleted);

          /*
           * Create a node representing a wstring.
           */
          ACE_CDR::ULong bound = 0UL;

          (yyval.dcval) =
            idl_global->gen ()->create_wstring (
                idl_global->gen ()->create_expr (bound,
                                                 AST_Expression::EV_ulong)
              );
          /*
           * Add this AST_String to the types defined in the global scope.
           */
          (void) idl_global->root ()->fe_add_string (
                                          AST_String::narrow_from_decl ((yyval.dcval))
                                        );
        }
#line 6471 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 340:
#line 3994 "fe/idl.ypp" /* yacc.c:1646  */
    {
// wstring_head : IDL_WSTRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 6480 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 341:
#line 4002 "fe/idl.ypp" /* yacc.c:1646  */
    {
// array_declarator : defining_id
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
#line 6489 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 342:
#line 4007 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_array_dim
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayCompleted);

          /*
           * Create a node representing an array.
           */
          if ((yyvsp[0].elval) != 0)
            {
              UTL_ScopedName sn ((yyvsp[-2].idval),
                                 0);
              (yyval.dcval) =
                idl_global->gen ()->create_array (
                                        &sn,
                                        (yyvsp[0].elval)->length (),
                                        (yyvsp[0].elval),
                                        0,
                                        0
                                      );

              (yyvsp[0].elval)->destroy ();
              delete (yyvsp[0].elval);
              (yyvsp[0].elval) = 0;

              sn.destroy ();

              if (!idl_global->in_typedef ()
                  && !idl_global->anon_silent ())
                {
                  idl_global->err ()->anonymous_type_diagnostic ();
                }
            }
        }
#line 6527 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 343:
#line 4044 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_array_dim : array_dim array_dims
          ACE_NEW_RETURN ((yyval.elval),
                          UTL_ExprList ((yyvsp[-1].exval),
                                        (yyvsp[0].elval)),
                          1);
        }
#line 6539 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 344:
#line 4055 "fe/idl.ypp" /* yacc.c:1646  */
    {
// array_dims : array_dims array_dim
          UTL_ExprList *el = 0;
          ACE_NEW_RETURN (el,
                          UTL_ExprList ((yyvsp[0].exval),
                                        0),
                          1);

          if ((yyvsp[-1].elval) == 0)
            {
              (yyval.elval) = el;
            }
          else
            {
              (yyvsp[-1].elval)->nconc (el);
              (yyval.elval) = (yyvsp[-1].elval);
            }
        }
#line 6562 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 345:
#line 4074 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.elval) = 0;
        }
#line 6571 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 346:
#line 4082 "fe/idl.ypp" /* yacc.c:1646  */
    {
// array_dim : '['
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
#line 6580 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 347:
#line 4087 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
#line 6589 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 348:
#line 4092 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ']'
          idl_global->set_parse_state (IDL_GlobalData::PS_DimQsSeen);

          /*
           * Array dimensions are expressions which must be coerced to
           * positive integers.
           */
          AST_Expression::AST_ExprValue *ev = 0;
          AST_Param_Holder *param_holder = 0;

          if ((yyvsp[-2].exval) != 0)
            {
              param_holder =
                (yyvsp[-2].exval)->param_holder ();

              ev =
                (yyvsp[-2].exval)->coerce (AST_Expression::EV_ulong);
            }

          if (0 == (yyvsp[-2].exval)
              || (ev == 0 && param_holder == 0))
            {
              idl_global->err ()->coercion_error ((yyvsp[-2].exval),
                                                  AST_Expression::EV_ulong);
              (yyval.exval) = 0;
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

              (yyval.exval) = (yyvsp[-2].exval);
            }

          delete ev;
          ev = 0;
        }
#line 6648 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 351:
#line 4155 "fe/idl.ypp" /* yacc.c:1646  */
    {
// attribute_readonly : IDL_READONLY
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
#line 6657 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 352:
#line 4160 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 6666 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 353:
#line 4165 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 6675 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 354:
#line 4170 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 6684 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 355:
#line 4175 "fe/idl.ypp" /* yacc.c:1646  */
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
              && (yyvsp[-4].dcval) != 0
              && (yyvsp[-2].dlval) != 0)
            {
              for (UTL_DecllistActiveIterator l ((yyvsp[-2].dlval));
                   !l.is_done ();
                   l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type *tp = d->compose ((yyvsp[-4].dcval));

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

                  if ((yyvsp[0].nlval) != 0)
                    {
                      (void) a->fe_add_get_exceptions ((yyvsp[0].nlval));

                      (yyvsp[0].nlval)->destroy ();
                      delete (yyvsp[0].nlval);
                      (yyvsp[0].nlval) = 0;
                    }

                  (void) s->fe_add_attribute (a);
                }
            }

          (yyvsp[-2].dlval)->destroy ();
          delete (yyvsp[-2].dlval);
          (yyvsp[-2].dlval) = 0;
        }
#line 6749 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 356:
#line 4239 "fe/idl.ypp" /* yacc.c:1646  */
    {
// attribute_readwrite : IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 6758 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 357:
#line 4244 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 6767 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 358:
#line 4249 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 6776 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 359:
#line 4254 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      opt_getraises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
#line 6785 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 360:
#line 4259 "fe/idl.ypp" /* yacc.c:1646  */
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
              && (yyvsp[-6].dcval) != 0
              && (yyvsp[-4].dlval) != 0)
            {
              for (UTL_DecllistActiveIterator l ((yyvsp[-4].dlval));
                   !l.is_done ();
                   l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type *tp = d->compose ((yyvsp[-6].dcval));

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

                  if ((yyvsp[-2].nlval) != 0)
                    {
                      (void) a->fe_add_get_exceptions ((yyvsp[-2].nlval));

                      (yyvsp[-2].nlval)->destroy ();
                      delete (yyvsp[-2].nlval);
                      (yyvsp[-2].nlval) = 0;
                    }

                  if ((yyvsp[0].nlval) != 0)
                    {
                      (void) a->fe_add_set_exceptions ((yyvsp[0].nlval));

                      (yyvsp[0].nlval)->destroy ();
                      delete (yyvsp[0].nlval);
                      (yyvsp[0].nlval) = 0;
                    }

                  (void) s->fe_add_attribute (a);
                }
            }

          (yyvsp[-4].dlval)->destroy ();
          delete (yyvsp[-4].dlval);
          (yyvsp[-4].dlval) = 0;
        }
#line 6859 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 361:
#line 4332 "fe/idl.ypp" /* yacc.c:1646  */
    {
// exception : IDL_EXCEPTION
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
#line 6868 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 362:
#line 4337 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      defining_id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
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

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 6902 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 363:
#line 4367 "fe/idl.ypp" /* yacc.c:1646  */
    {
//       '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
#line 6911 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 364:
#line 4372 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      members
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
#line 6920 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 365:
#line 4377 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();
        }
#line 6933 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 366:
#line 4390 "fe/idl.ypp" /* yacc.c:1646  */
    {
// operation : opt_op_attribute op_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 6942 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 367:
#line 4395 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      annotations IDENTIFIER
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          Identifier id ((yyvsp[0].strval));
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;

          UTL_ScopedName n (&id, 0);
          AST_Operation *o = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing an operation on an interface
           * and add it to its enclosing scope.
           */
          if (s != 0 && (yyvsp[-3].dcval) != 0)
            {
              AST_Type *tp =
                AST_Type::narrow_from_decl ((yyvsp[-3].dcval));

              if (tp == 0)
                {
                  idl_global->err ()->not_a_type ((yyvsp[-3].dcval));
                }
              else if (tp->node_type () == AST_Decl::NT_except)
                {
                  idl_global->err ()->not_a_type ((yyvsp[-3].dcval));
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
                    idl_global->gen ()->create_operation (
                      tp,
                      (yyvsp[-4].ofval),
                      &n,
                      local,
                      s->is_abstract ());

                  if (!local && tp->is_local ())
                    {
                      idl_global->err ()->local_remote_mismatch (tp, o);
                      o->destroy ();
                      delete o;
                      o = 0;
                    }
                  else
                    {
                      (void) s->fe_add_operation (o);
                    }
                }
            }

          /*
           * Push the operation scope onto the scopes stack.
           */
          idl_global->scopes ().push (o);
        }
#line 7011 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 368:
#line 4460 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7020 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 369:
#line 4465 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      opt_raises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
#line 7029 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 370:
#line 4470 "fe/idl.ypp" /* yacc.c:1646  */
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

              if ((yyvsp[-2].nlval) != 0 && o != 0)
                {
                  (void) o->fe_add_exceptions ((yyvsp[-2].nlval));
                }

              if ((yyvsp[0].slval) != 0)
                {
                  (void) o->fe_add_context ((yyvsp[0].slval));
                }
            }

          /*
           * Done with this operation. Pop its scope from the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 7063 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 371:
#line 4503 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_op_attribute : IDL_ONEWAY
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_oneway;
        }
#line 7073 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 372:
#line 4509 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_IDEMPOTENT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_idempotent;
        }
#line 7083 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 373:
#line 4515 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.ofval) = AST_Operation::OP_noflags;
        }
#line 7092 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 375:
#line 4524 "fe/idl.ypp" /* yacc.c:1646  */
    {
// op_type_spec : param_type_spec | IDL_VOID
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
#line 7104 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 376:
#line 4535 "fe/idl.ypp" /* yacc.c:1646  */
    {
// init_decl : IDL_FACTORY
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7114 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 377:
#line 4541 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      annotations IDENTIFIER
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

          Identifier id ((yyvsp[0].strval));
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;

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
#line 7160 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 378:
#line 4583 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7169 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 379:
#line 4588 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      opt_raises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          if ((yyvsp[0].nlval) != 0)
            {
              UTL_Scope *s = idl_global->scopes ().top_non_null ();
              AST_Factory *f = AST_Factory::narrow_from_scope (s);
              (void) f->fe_add_exceptions ((yyvsp[0].nlval));
            }

          idl_global->scopes ().pop ();
        }
#line 7187 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 380:
#line 4605 "fe/idl.ypp" /* yacc.c:1646  */
    {
// init_parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7196 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 381:
#line 4610 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7205 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 382:
#line 4615 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7214 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 383:
#line 4621 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_in_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7223 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 385:
#line 4632 "fe/idl.ypp" /* yacc.c:1646  */
    {
// in_parameters : in_parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7232 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 388:
#line 4642 "fe/idl.ypp" /* yacc.c:1646  */
    {
// in_parameter : IDL_IN
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7241 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 389:
#line 4647 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7250 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 390:
#line 4652 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      declarator
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Argument *a = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDeclSeen);

          /*
           * Create a node representing an argument to an operation
           * Add it to the enclosing scope (the operation scope).
           */
          if ((yyvsp[-2].dcval) != 0
              && (yyvsp[0].deval) != 0 &&
              s != 0)
            {
              AST_Type *tp = (yyvsp[0].deval)->compose ((yyvsp[-2].dcval));

              if (tp != 0)
                {
                  a = idl_global->gen ()->create_argument (
                          AST_Argument::dir_IN,
                          tp,
                          (yyvsp[0].deval)->name ()
                        );

                  (void) s->fe_add_argument (a);
                }
            }

          (yyvsp[0].deval)->destroy ();
          delete (yyvsp[0].deval);
          (yyvsp[0].deval) = 0;
        }
#line 7287 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 391:
#line 4688 "fe/idl.ypp" /* yacc.c:1646  */
    {
// parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7296 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 392:
#line 4693 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7305 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 393:
#line 4698 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7314 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 394:
#line 4704 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7323 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 396:
#line 4715 "fe/idl.ypp" /* yacc.c:1646  */
    {
// parameters : parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7332 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 399:
#line 4725 "fe/idl.ypp" /* yacc.c:1646  */
    {
// parameter : direction
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7341 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 400:
#line 4730 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7350 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 401:
#line 4735 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      declarator
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Argument *a = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDeclSeen);

          /*
           * Create a node representing an argument to an operation
           * Add it to the enclosing scope (the operation scope).
           */
          if ((yyvsp[-2].dcval) != 0
              && (yyvsp[0].deval) != 0
              && s != 0)
            {
              AST_Type *tp = (yyvsp[0].deval)->compose ((yyvsp[-2].dcval));

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
                            (yyvsp[-4].dival),
                            tp,
                            (yyvsp[0].deval)->name ()
                          );
                      (void) s->fe_add_argument (a);
                    }
                }
            }

          (yyvsp[0].deval)->destroy ();
          delete (yyvsp[0].deval);
          (yyvsp[0].deval) = 0;
        }
#line 7394 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 402:
#line 4778 "fe/idl.ypp" /* yacc.c:1646  */
    {
// param_type_spec : base_type_spec
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 7406 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 405:
#line 4788 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | string_type_spec
//      | wstring_type_spec
//      | scoped_name
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = 0;
          UTL_ScopedName *n = (yyvsp[0].idlist);

          if (s != 0)
            {
              d = s->lookup_by_name (n, false, false);
            }

          if (d == 0)
            {
              idl_global->err ()->lookup_error (n);
              (yyvsp[0].idlist)->destroy ();
              (yyvsp[0].idlist) = 0;

              /* If we don't return here, we'll crash later.*/
              return 1;
            }
          else
            {
              d->last_referenced_as ((yyvsp[0].idlist));
              AST_Decl::NodeType nt = d->node_type ();
              AST_Type *t = AST_Type::narrow_from_decl (d);
              AST_Typedef *td = 0;
              bool can_be_undefined = false;

              if (nt == AST_Decl::NT_typedef)
                {
                  // This code block ensures that a sequence of
                  // as-yet-undefined struct or union isn't used
                  // as a return type or argument.
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

                  if (! t->is_defined () && ! can_be_undefined)
                    {
                      idl_global->err ()->error1 (
                        UTL_Error::EIDL_ILLEGAL_ADD,
                        (nt == AST_Decl::NT_typedef ? td : t));

                      /* If we don't return here, we'll crash later.*/
                      return 1;
                    }
                }
              else
                {
                  // For forward declared structs and unions, we
                  // want the full definition, but we need to
                  // check that it's been fully defined.
                  AST_StructureFwd *fwd =
                    AST_StructureFwd::narrow_from_decl (d);

                  if (fwd != 0)
                    {
                      t = fwd->full_definition ();

                      if (! t->is_defined ())
                        {
                          idl_global->err ()->error1 (
                            UTL_Error::EIDL_ILLEGAL_ADD,
                            t);

                          /* If we don't return here, we'll crash later.*/
                          return 1;
                        }

                      d = t;
                    }
                }
            }

          (yyval.dcval) = d;
        }
#line 7517 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 406:
#line 4898 "fe/idl.ypp" /* yacc.c:1646  */
    {
// direction : IDL_IN
          (yyval.dival) = AST_Argument::dir_IN;
        }
#line 7526 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 407:
#line 4903 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_OUT
          (yyval.dival) = AST_Argument::dir_OUT;
        }
#line 7535 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 408:
#line 4908 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_INOUT
          (yyval.dival) = AST_Argument::dir_INOUT;
        }
#line 7544 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 409:
#line 4916 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_raises : IDL_RAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
#line 7553 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 410:
#line 4921 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
#line 7562 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 411:
#line 4927 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7572 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 412:
#line 4933 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.nlval) = 0;
/*      |  EMPTY */
        }
#line 7581 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 413:
#line 4941 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_getraises : IDL_GETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
#line 7590 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 414:
#line 4946 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
#line 7599 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 415:
#line 4952 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7609 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 416:
#line 4958 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.nlval) = 0;
/*      |  EMPTY */
        }
#line 7618 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 417:
#line 4966 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_setraises : IDL_SETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
#line 7627 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 418:
#line 4971 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
#line 7636 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 419:
#line 4977 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7646 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 420:
#line 4983 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.nlval) = 0;
/*      |  EMPTY */
        }
#line 7655 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 421:
#line 4991 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_context : IDL_CONTEXT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
#line 7664 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 422:
#line 4996 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
//      '('
        }
#line 7673 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 423:
#line 5002 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_string_literal ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (yyval.slval) = (yyvsp[-1].slval);
        }
#line 7683 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 424:
#line 5008 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.slval) = 0;
        }
#line 7692 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 425:
#line 5016 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_string_literal : IDL_STRING_LITERAL string_literals
          ACE_NEW_RETURN ((yyval.slval),
                          UTL_StrList ((yyvsp[-1].sval),
                                       (yyvsp[0].slval)),
                          1);
        }
#line 7704 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 426:
#line 5028 "fe/idl.ypp" /* yacc.c:1646  */
    {
// string_literals : string_literals ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
#line 7713 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 427:
#line 5033 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      IDL_STRING_LITERAL
          UTL_StrList *sl = 0;
          ACE_NEW_RETURN (sl,
                          UTL_StrList ((yyvsp[0].sval),
                                       0),
                          1);

          if ((yyvsp[-3].slval) == 0)
            {
              (yyval.slval) = sl;
            }
          else
            {
              (yyvsp[-3].slval)->nconc (sl);
              (yyval.slval) = (yyvsp[-3].slval);
            }
        }
#line 7736 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 428:
#line 5052 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.slval) = 0;
        }
#line 7745 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 429:
#line 5060 "fe/idl.ypp" /* yacc.c:1646  */
    {
// typeid_dcl : IDL_TYPEID scoped_name IDL_STRING_LITERAL
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-1].idlist));

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
            }
          else
            {
              d->set_id_with_typeid (
                     (yyvsp[0].sval)->get_string ()
                   );
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyvsp[0].sval)->destroy ();
          delete (yyvsp[0].sval);
          (yyvsp[0].sval) = 0;
        }
#line 7775 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 430:
#line 5089 "fe/idl.ypp" /* yacc.c:1646  */
    {
// typeprefix_dcl : IDL_TYPEPREFIX scoped_name IDL_STRING_LITERAL
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = ScopeAsDecl (s);

          // If we are in a module, we want to avoid a lookup unless the
          // typeprefix is to be applied to some other scope, since we
          // might get a previous opening of the module, and the prefix
          // of this opening would never get checked or set.
          if (d->name ()->compare ((yyvsp[-1].idlist)) != 0)
            {
              d =
                s->lookup_by_name ((yyvsp[-1].idlist));
            }

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
            }
          else
            {
              d->set_prefix_with_typeprefix (
                     (yyvsp[0].sval)->get_string ()
                   );
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyvsp[0].sval)->destroy ();
          delete (yyvsp[0].sval);
          (yyvsp[0].sval) = 0;
        }
#line 7814 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 433:
#line 5133 "fe/idl.ypp" /* yacc.c:1646  */
    {
// component_forward_decl : IDL_COMPONENT defining_id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
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

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 7843 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 434:
#line 5161 "fe/idl.ypp" /* yacc.c:1646  */
    {
// component_decl : component_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Component *c = 0;

          /*
           * Make a new component node and add it to the enclosing scope.
           */
          if (s != 0 && (yyvsp[0].chval) != 0)
            {
              c =
                idl_global->gen ()->create_component (
                                        (yyvsp[0].chval)->name (),
                                        (yyvsp[0].chval)->base_component (),
                                        (yyvsp[0].chval)->supports (),
                                        (yyvsp[0].chval)->n_supports (),
                                        (yyvsp[0].chval)->supports_flat (),
                                        (yyvsp[0].chval)->n_supports_flat ()
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
              (yyvsp[0].chval)->destroy ();
              delete (yyvsp[0].chval);
              (yyvsp[0].chval) = 0;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (c);
        }
#line 7887 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 435:
#line 5201 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
#line 7896 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 436:
#line 5206 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      component_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
#line 7905 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 437:
#line 5211 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 7919 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 438:
#line 5225 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      component_header: IDL_COMPONENT defining_id
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
#line 7928 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 439:
#line 5230 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      component_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 7937 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 440:
#line 5235 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);

          /*
           * Create an AST representation of the information in the header
           * part of a component.
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ((yyvsp[-4].idval),
                                          0),
                          1);
          ACE_NEW_RETURN ((yyval.chval),
                          FE_ComponentHeader (n,
                                              (yyvsp[-2].idlist),
                                              (yyvsp[0].nlval),
                                              false),
                          1);

          if (0 != (yyvsp[0].nlval))
            {
              (yyvsp[0].nlval)->destroy ();
              delete (yyvsp[0].nlval);
              (yyvsp[0].nlval) = 0;
            }

          if (0 != (yyvsp[-2].idlist))
            {
              (yyvsp[-2].idlist)->destroy ();
              delete (yyvsp[-2].idlist);
              (yyvsp[-2].idlist) = 0;
            }
        }
#line 7976 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 441:
#line 5273 "fe/idl.ypp" /* yacc.c:1646  */
    {
// component_inheritance_spec : ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 7985 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 442:
#line 5278 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      scoped_name
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 7994 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 443:
#line 5283 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.idlist) = 0;
        }
#line 8003 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 446:
#line 5296 "fe/idl.ypp" /* yacc.c:1646  */
    {
// component_export : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 8012 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 447:
#line 5301 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8021 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 448:
#line 5306 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 8030 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 449:
#line 5311 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8039 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 450:
#line 5316 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | emits_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
        }
#line 8048 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 451:
#line 5321 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8057 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 452:
#line 5326 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | publishes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
        }
#line 8066 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 453:
#line 5331 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8075 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 454:
#line 5336 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | consumes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
        }
#line 8084 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 455:
#line 5341 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8093 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 456:
#line 5346 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 8102 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 457:
#line 5351 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8111 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 458:
#line 5356 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | extended_port_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 8120 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 459:
#line 5361 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8129 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 460:
#line 5368 "fe/idl.ypp" /* yacc.c:1646  */
    {
// provides_decl : IDL_PROVIDES interface_type id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;

          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-1].idlist), true, false);

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
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
              FE_Utils::original_local_name ((yyvsp[0].idval));

              UTL_ScopedName sn ((yyvsp[0].idval),
                                 0);

              AST_Provides *p =
                idl_global->gen ()->create_provides (&sn,
                                                     port_interface_type);

              (void) s->fe_add_provides (p);
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 8221 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 461:
#line 5459 "fe/idl.ypp" /* yacc.c:1646  */
    {
// interface_type : scoped_name
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8232 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 462:
#line 5466 "fe/idl.ypp" /* yacc.c:1646  */
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

          ACE_NEW_RETURN ((yyval.idlist),
                          UTL_IdList (corba_id,
                                      conc_name),
                          1);
        }
#line 8260 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 463:
#line 5492 "fe/idl.ypp" /* yacc.c:1646  */
    {
// uses_decl : uses_opt_multiple interface_type id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;

          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-1].idlist), true, false);

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
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
              FE_Utils::original_local_name ((yyvsp[0].idval));

              UTL_ScopedName sn ((yyvsp[0].idval),
                                 0);

              AST_Uses *u =
                idl_global->gen ()->create_uses (&sn,
                                                 port_interface_type,
                                                 (yyvsp[-2].bval));

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
                  FE_Utils::create_uses_multiple_stuff (c, u);
                }
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 8368 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 464:
#line 5599 "fe/idl.ypp" /* yacc.c:1646  */
    {
// uses_opt_multiple : IDL_USES opt_multiple
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          (yyval.bval) = (yyvsp[0].bval);
        }
#line 8379 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 465:
#line 5609 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_multiple : IDL_MULTIPLE
          (yyval.bval) = true;
        }
#line 8388 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 466:
#line 5614 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.bval) = false;
        }
#line 8397 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 467:
#line 5621 "fe/idl.ypp" /* yacc.c:1646  */
    {
// emits_decl : IDL_EMITS scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;

          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-1].idlist), true, false);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
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
              FE_Utils::original_local_name ((yyvsp[0].idval));

              UTL_ScopedName sn ((yyvsp[0].idval),
                                 0);

              AST_Emits *e =
                idl_global->gen ()->create_emits (&sn,
                                                  event_type);

              (void) s->fe_add_emits (e);
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 8472 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 468:
#line 5694 "fe/idl.ypp" /* yacc.c:1646  */
    {
// publishes_decl : IDL_PUBLISHES scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;

          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-1].idlist), true, false);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
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
              FE_Utils::original_local_name ((yyvsp[0].idval));

              UTL_ScopedName sn ((yyvsp[0].idval),
                                 0);

              AST_Publishes *p =
                idl_global->gen ()->create_publishes (&sn,
                                                      event_type);

              (void) s->fe_add_publishes (p);
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 8547 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 469:
#line 5767 "fe/idl.ypp" /* yacc.c:1646  */
    {
// consumes_decl : IDL_CONSUMES scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          bool so_far_so_good = true;
          AST_Decl::NodeType nt = AST_Decl::NT_type;
          AST_Param_Holder *ph = 0;

          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-1].idlist), true, false);

          if (0 == d)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
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
              FE_Utils::original_local_name ((yyvsp[0].idval));

              UTL_ScopedName sn ((yyvsp[0].idval),
                                 0);

              AST_Consumes *c =
                idl_global->gen ()->create_consumes (&sn,
                                                     event_type);

              (void) s->fe_add_consumes (c);
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
       }
#line 8622 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 470:
#line 5841 "fe/idl.ypp" /* yacc.c:1646  */
    {
// home_decl : home_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Home *h = 0;

          /*
           * Make a new home node and add it to the enclosing scope.
           */
          if (s != 0 && (yyvsp[0].hhval) != 0)
            {
              h =
                idl_global->gen ()->create_home (
                                        (yyvsp[0].hhval)->name (),
                                        (yyvsp[0].hhval)->base_home (),
                                        (yyvsp[0].hhval)->managed_component (),
                                        (yyvsp[0].hhval)->primary_key (),
                                        (yyvsp[0].hhval)->supports (),
                                        (yyvsp[0].hhval)->n_supports (),
                                        (yyvsp[0].hhval)->supports_flat (),
                                        (yyvsp[0].hhval)->n_supports_flat ()
                                      );
              /*
               * Add the home to its definition scope.
               */
              (void) s->fe_add_home (h);

              // This FE_HomeHeader class isn't destroyed with the AST.
              (yyvsp[0].hhval)->destroy ();
              delete (yyvsp[0].hhval);
              (yyvsp[0].hhval) = 0;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (h);
        }
#line 8664 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 471:
#line 5879 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      home_body
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 8676 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 472:
#line 5890 "fe/idl.ypp" /* yacc.c:1646  */
    {
// home_header : IDL_HOME
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
#line 8685 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 473:
#line 5895 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      defining_id
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
#line 8694 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 474:
#line 5900 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      home_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 8703 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 475:
#line 5905 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
#line 8712 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 476:
#line 5910 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      IDL_MANAGES
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
#line 8721 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 477:
#line 5915 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      scoped_name
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
#line 8730 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 478:
#line 5920 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      primary_key_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_PrimaryKeySpecSeen);

          /*
           * Create an AST representation of the information in the header
           * part of a component home.
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName ((yyvsp[-10].idval), 0),
                          1);

          ACE_NEW_RETURN ((yyval.hhval),
                          FE_HomeHeader (n,
                                         (yyvsp[-8].idlist),
                                         (yyvsp[-6].nlval),
                                         (yyvsp[-2].idlist),
                                         (yyvsp[0].idlist)),
                          1);

          (yyvsp[-2].idlist)->destroy ();
          delete (yyvsp[-2].idlist);
          (yyvsp[-2].idlist) = 0;

          if (0 != (yyvsp[-8].idlist))
            {
              (yyvsp[-8].idlist)->destroy ();
              delete (yyvsp[-8].idlist);
              (yyvsp[-8].idlist) = 0;
            }

          if (0 != (yyvsp[0].idlist))
            {
              (yyvsp[0].idlist)->destroy ();
              delete (yyvsp[0].idlist);
              (yyvsp[0].idlist) = 0;
            }

          if (0 != (yyvsp[-6].nlval))
            {
              (yyvsp[-6].nlval)->destroy ();
              delete (yyvsp[-6].nlval);
              (yyvsp[-6].nlval) = 0;
            }
        }
#line 8781 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 479:
#line 5970 "fe/idl.ypp" /* yacc.c:1646  */
    {
// home_inheritance_spec ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 8790 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 480:
#line 5975 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      scoped_name
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8799 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 481:
#line 5980 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.idlist) = 0;
        }
#line 8808 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 482:
#line 5989 "fe/idl.ypp" /* yacc.c:1646  */
    {
// primary_key_spec : IDL_PRIMARYKEY scoped_name
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8817 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 483:
#line 5994 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.idlist) = 0;
        }
#line 8826 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 484:
#line 6002 "fe/idl.ypp" /* yacc.c:1646  */
    {
// home_body : '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
#line 8835 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 485:
#line 6007 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      home_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
#line 8844 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 486:
#line 6012 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
#line 8853 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 490:
#line 6026 "fe/idl.ypp" /* yacc.c:1646  */
    {
// home_export : factory_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
#line 8862 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 491:
#line 6031 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8871 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 492:
#line 6036 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | finder_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
#line 8880 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 493:
#line 6041 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8889 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 494:
#line 6050 "fe/idl.ypp" /* yacc.c:1646  */
    {
// factory_decl : IDL_FACTORY defining_id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
                            0);
          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing a factory operation
           * and add it to the enclosing scope.
           */
          AST_Factory *f = idl_global->gen ()->create_factory (&n);
          (void) s->fe_add_factory (f);

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;

          /*
           * Push the factory scope onto the scopes stack.
           */
          idl_global->scopes ().push (f);
        }
#line 8917 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 495:
#line 6074 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 8926 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 496:
#line 6079 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      opt_raises
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          /*
           * Add exceptions and context to the factory.
           */
          if ((yyvsp[0].nlval) != 0)
            {
              (void) s->fe_add_exceptions ((yyvsp[0].nlval));
            }

          /*
           * Done with this factory. Pop its scope from the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 8949 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 497:
#line 6102 "fe/idl.ypp" /* yacc.c:1646  */
    {
// finder_decl : IDL_FINDER defining_id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
                            0);

          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing a home finder
           * and add it to the enclosing scope.
           */
          AST_Finder *f =
            idl_global->gen ()->create_finder (&n);

          (void) s->fe_add_finder (f);


          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;

          /*
           * Push the operation scope onto the scopes stack.
           */
          idl_global->scopes ().push (f);
        }
#line 8981 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 498:
#line 6130 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 8990 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 499:
#line 6135 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      opt_raises
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          /*
           * Add exceptions and context to the finder.
           */
          if ((yyvsp[0].nlval) != 0)
            {
              (void) s->fe_add_exceptions ((yyvsp[0].nlval));
            }

          /*
           * Done with this operation. Pop its scope from the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9013 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 505:
#line 6169 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_concrete_forward_decl : IDL_EVENTTYPE defining_id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
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

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 9041 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 506:
#line 6198 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_abs_forward_decl : IDL_ABSTRACT IDL_EVENTTYPE defining_id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval),
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

          (yyvsp[0].idval)->destroy ();
          delete (yyvsp[0].idval);
          (yyvsp[0].idval) = 0;
        }
#line 9069 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 507:
#line 6226 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_abs_decl : event_abs_header event_rest_of_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_EventType *e = 0;
          AST_Interface *i = 0;

          if (s != 0 && (yyvsp[-1].idval) != 0)
            {
              UTL_ScopedName sn ((yyvsp[-1].idval),
                                 0);
              e =
                idl_global->gen ()->create_eventtype (
                    &sn,
                    (yyvsp[0].vhval)->inherits (),
                    (yyvsp[0].vhval)->n_inherits (),
                    (yyvsp[0].vhval)->inherits_concrete (),
                    (yyvsp[0].vhval)->inherits_flat (),
                    (yyvsp[0].vhval)->n_inherits_flat (),
                    (yyvsp[0].vhval)->supports (),
                    (yyvsp[0].vhval)->n_supports (),
                    (yyvsp[0].vhval)->supports_concrete (),
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

          (yyvsp[-1].idval)->destroy ();
          delete (yyvsp[-1].idval);
          (yyvsp[-1].idval) = 0;
        }
#line 9118 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 508:
#line 6271 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9127 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 509:
#line 6276 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9136 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 510:
#line 6281 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9150 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 511:
#line 6296 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_abs_header : IDL_ABSTRACT IDL_EVENTTYPE defining_id
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9159 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 512:
#line 6306 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_custom_header : IDL_CUSTOM IDL_EVENTTYPE defining_id
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("error in %C line %d\n"),
                      idl_global->filename ()->get_string (),
                      idl_global->lineno ()));
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Sorry, I (TAO_IDL) can't handle")
                      ACE_TEXT (" custom yet\n")));
          (yyval.idval) = 0;
        }
#line 9177 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 513:
#line 6324 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_plain_header : IDL_EVENTTYPE defining_id
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9188 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 514:
#line 6334 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_rest_of_header : inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 9197 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 515:
#line 6339 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);

          ACE_NEW_RETURN ((yyval.vhval),
                          FE_OBVHeader (
                            0,
                            (yyvsp[-2].nlval),
                            (yyvsp[0].nlval),
                            (yyvsp[-2].nlval)
                              ? (yyvsp[-2].nlval)->truncatable ()
                              : false,
                            true),
                          1);

          if (0 != (yyvsp[0].nlval))
            {
              (yyvsp[0].nlval)->destroy ();
              delete (yyvsp[0].nlval);
              (yyvsp[0].nlval) = 0;
            }

          if (0 != (yyvsp[-2].nlval))
            {
              (yyvsp[-2].nlval)->destroy ();
              delete (yyvsp[-2].nlval);
              (yyvsp[-2].nlval) = 0;
            }
        }
#line 9231 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 516:
#line 6373 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_decl : event_header event_rest_of_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_EventType *e = 0;
          AST_Interface *i = 0;

          if (s != 0 && (yyvsp[-1].idval) != 0)
            {
              // We create the scoped name here instead of with the
              // FE_EventHeader because there is a token for it only here.
              UTL_ScopedName sn ((yyvsp[-1].idval),
                                 0);
              e =
                idl_global->gen ()->create_eventtype (
                    &sn,
                    (yyvsp[0].vhval)->inherits (),
                    (yyvsp[0].vhval)->n_inherits (),
                    (yyvsp[0].vhval)->inherits_concrete (),
                    (yyvsp[0].vhval)->inherits_flat (),
                    (yyvsp[0].vhval)->n_inherits_flat (),
                    (yyvsp[0].vhval)->supports (),
                    (yyvsp[0].vhval)->n_supports (),
                    (yyvsp[0].vhval)->supports_concrete (),
                    false,
                    (yyvsp[0].vhval)->truncatable (),
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
              (yyvsp[0].vhval)->destroy ();
              delete (yyvsp[0].vhval);
              (yyvsp[0].vhval) = 0;

              sn.destroy ();
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (e);
        }
#line 9285 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 517:
#line 6423 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9294 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 518:
#line 6428 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9303 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 519:
#line 6433 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9317 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 520:
#line 6446 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_header : event_custom_header
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9326 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 521:
#line 6451 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_header : event_plain_header
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9335 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 522:
#line 6459 "fe/idl.ypp" /* yacc.c:1646  */
    {
// formal_parameter_type : IDL_TYPENAME
          (yyval.ntval) = AST_Decl::NT_type;
        }
#line 9344 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 523:
#line 6464 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_STRUCT
          (yyval.ntval) = AST_Decl::NT_struct;
        }
#line 9353 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 524:
#line 6469 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_UNION
          (yyval.ntval) = AST_Decl::NT_union;
        }
#line 9362 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 525:
#line 6474 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_EVENTTYPE
          (yyval.ntval) = AST_Decl::NT_eventtype;
        }
#line 9371 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 526:
#line 6479 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_SEQUENCE
          (yyval.ntval) = AST_Decl::NT_sequence;
        }
#line 9380 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 527:
#line 6484 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_INTERFACE
          (yyval.ntval) = AST_Decl::NT_interface;
        }
#line 9389 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 528:
#line 6489 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_VALUETYPE
          (yyval.ntval) = AST_Decl::NT_valuetype;
        }
#line 9398 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 529:
#line 6494 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_ENUM
          (yyval.ntval) = AST_Decl::NT_enum;
        }
#line 9407 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 530:
#line 6499 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_EXCEPTION
          (yyval.ntval) = AST_Decl::NT_except;
        }
#line 9416 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 531:
#line 6504 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_CONST const_type
          (yyval.ntval) = AST_Decl::NT_const;
          t_param_const_type = (yyvsp[0].etval);
        }
#line 9426 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 532:
#line 6513 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_formal_parameter : formal_parameter formal_parameters
          if ((yyvsp[0].plval) == 0)
            {
              ACE_NEW_RETURN ((yyvsp[0].plval),
                              FE_Utils::T_PARAMLIST_INFO,
                              1);
            }

          (yyvsp[0].plval)->enqueue_head (*(yyvsp[-1].pival));
          delete (yyvsp[-1].pival);
          (yyvsp[-1].pival) = 0;

          // The param added above is always the last one parsed,
          // so we check for matches between sequence<T> & T here.
          ACE_CString bad_id =
            FE_Utils::check_for_seq_of_param (
              (yyvsp[0].plval));

          if (!bad_id.empty ())
            {
              delete (yyvsp[0].plval);
              (yyvsp[0].plval) = 0;

              idl_global->err ()->mismatch_seq_of_param (bad_id.c_str ());
              return 1;
            }

          (yyval.plval) = (yyvsp[0].plval);
        }
#line 9461 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 533:
#line 6547 "fe/idl.ypp" /* yacc.c:1646  */
    {
// formal_parameters : formal_parameters ','
          // Maybe add a new parse state to set here.
        }
#line 9470 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 534:
#line 6552 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        formal_parameter
          if ((yyvsp[-3].plval) == 0)
            {
              ACE_NEW_RETURN ((yyvsp[-3].plval),
                              FE_Utils::T_PARAMLIST_INFO,
                              1);
            }

          (yyvsp[-3].plval)->enqueue_tail (*(yyvsp[0].pival));
          (yyval.plval) = (yyvsp[-3].plval);

          delete (yyvsp[0].pival);
          (yyvsp[0].pival) = 0;
        }
#line 9490 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 535:
#line 6568 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        /* EMPTY */
          (yyval.plval) = 0;
        }
#line 9499 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 536:
#line 6576 "fe/idl.ypp" /* yacc.c:1646  */
    {
// formal_parameter : formal_parameter_type IDENTIFIER

          ACE_NEW_RETURN ((yyval.pival),
                          FE_Utils::T_Param_Info,
                          1);

          AST_Decl::NodeType nt = (yyvsp[-1].ntval);

          (yyval.pival)->type_ = nt;
          (yyval.pival)->name_ = (yyvsp[0].strval);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;

          if (nt == AST_Decl::NT_const)
            {
              (yyval.pival)->const_type_ = t_param_const_type;
              (yyval.pival)->enum_const_type_decl_ =
                tao_enum_constant_decl;

              // Reset these values.
              t_param_const_type = AST_Expression::EV_none;
              tao_enum_constant_decl = 0;
            }
        }
#line 9529 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 537:
#line 6602 "fe/idl.ypp" /* yacc.c:1646  */
    {
          ACE_NEW_RETURN ((yyval.pival),
                          FE_Utils::T_Param_Info,
                          1);

          (yyval.pival)->type_ = AST_Decl::NT_sequence;
          (yyval.pival)->seq_param_ref_  = (yyvsp[-2].strval);
          (yyval.pival)->name_ += (yyvsp[0].strval);

          ACE::strdelete ((yyvsp[-2].strval));
          (yyvsp[-2].strval) = 0;
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 9548 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 538:
#line 6620 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_formal_parameter_name : formal_parameter_name formal_parameter_names
          ACE_NEW_RETURN ((yyval.slval),
                          UTL_StrList ((yyvsp[-1].sval),
                                       (yyvsp[0].slval)),
                          1);
        }
#line 9560 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 539:
#line 6631 "fe/idl.ypp" /* yacc.c:1646  */
    {
// formal_parameter_names : formal_parameter_names ',' formal_parameter_name
          UTL_StrList *sl = 0;
          ACE_NEW_RETURN (sl,
                          UTL_StrList ((yyvsp[0].sval),
                                       0),
                          1);

          if ((yyvsp[-2].slval) == 0)
            {
              (yyval.slval) = sl;
            }
          else
            {
              (yyvsp[-2].slval)->nconc (sl);
              (yyval.slval) = (yyvsp[-2].slval);
            }
        }
#line 9583 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 540:
#line 6650 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        /* EMPTY */
          (yyval.slval) = 0;
        }
#line 9592 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 541:
#line 6658 "fe/idl.ypp" /* yacc.c:1646  */
    {
// formal_parameter_name : IDENTIFIER
          ACE_NEW_RETURN ((yyval.sval),
                          UTL_String ((yyvsp[0].strval), true),
                          1);
        }
#line 9603 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 542:
#line 6668 "fe/idl.ypp" /* yacc.c:1646  */
    {
// porttype_decl : IDL_PORTTYPE
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSeen);
        }
#line 9612 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 543:
#line 6673 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        annotations IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeIDSeen);
          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          Identifier id ((yyvsp[0].strval));
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;

          UTL_ScopedName sn (&id, 0);
          AST_PortType *p =
            idl_global->gen ()->create_porttype (&sn);

          (void) s->fe_add_porttype (p);

          // Push it on the scopes stack.
          idl_global->scopes ().push (p);
        }
#line 9635 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 544:
#line 6692 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSqSeen);
        }
#line 9644 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 545:
#line 6697 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        at_least_one_port_export
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeBodySeen);
        }
#line 9653 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 546:
#line 6702 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeQsSeen);

          // Done with this port type - pop it off the scopes stack.
          idl_global->scopes ().pop ();
        }
#line 9665 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 547:
#line 6713 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_port_export : port_export port_exports
        }
#line 9673 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 548:
#line 6720 "fe/idl.ypp" /* yacc.c:1646  */
    {
// port_exports : port_exports port_export
        }
#line 9681 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 549:
#line 6724 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        | /* EMPTY */
        }
#line 9689 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 550:
#line 6731 "fe/idl.ypp" /* yacc.c:1646  */
    {
// port_export : provides_decl
        }
#line 9697 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 551:
#line 6735 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
        }
#line 9705 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 552:
#line 6739 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        | uses_decl
        }
#line 9713 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 553:
#line 6743 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
        }
#line 9721 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 554:
#line 6747 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 9730 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 555:
#line 6752 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
        }
#line 9738 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 556:
#line 6759 "fe/idl.ypp" /* yacc.c:1646  */
    {
// extended_port_decl : IDL_PORT scoped_name IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-1].idlist));
          AST_PortType *pt = 0;
          bool so_far_so_good = true;

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
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
              Identifier id ((yyvsp[0].strval));
              ACE::strdelete ((yyvsp[0].strval));
              (yyvsp[0].strval) = 0;

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

                      FE_Utils::create_uses_multiple_stuff (
                        c,
                        u,
                        id.get_string ());
                    }
                }
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;
        }
#line 9812 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 557:
#line 6829 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        | IDL_MIRRORPORT scoped_name IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_MirrorPortDeclSeen);
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d =
            s->lookup_by_name ((yyvsp[-1].idlist));
          AST_PortType *pt = 0;
          bool so_far_so_good = true;

          if (d == 0)
            {
              idl_global->err ()->lookup_error ((yyvsp[-1].idlist));
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
              Identifier id ((yyvsp[0].strval));
              ACE::strdelete ((yyvsp[0].strval));
              (yyvsp[0].strval) = 0;

              UTL_ScopedName sn (&id,
                                 0);

              AST_Mirror_Port *mp =
                idl_global->gen ()->create_mirror_port (
                  &sn,
                  pt);

              (void) s->fe_add_mirror_port (mp);
            }

          (yyvsp[-1].idlist)->destroy ();
          delete (yyvsp[-1].idlist);
          (yyvsp[-1].idlist) = 0;
        }
#line 9864 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 558:
#line 6880 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_actual_parameter : annotations actual_parameter actual_parameters
          if ((yyvsp[0].alval) == 0)
            {
              ACE_NEW_RETURN ((yyvsp[0].alval),
                              FE_Utils::T_ARGLIST,
                              1);
            }

          (yyvsp[0].alval)->enqueue_head ((yyvsp[-1].dcval));
          (yyval.alval) = (yyvsp[0].alval);
        }
#line 9881 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 559:
#line 6896 "fe/idl.ypp" /* yacc.c:1646  */
    {
// actual_parameters : actual_parameters ',' annotations actual_parameter
          if ((yyvsp[-3].alval) == 0)
            {
              ACE_NEW_RETURN ((yyvsp[-3].alval),
                              FE_Utils::T_ARGLIST,
                              1);
            }

          (yyvsp[-3].alval)->enqueue_tail ((yyvsp[0].dcval));
          (yyval.alval) = (yyvsp[-3].alval);
        }
#line 9898 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 560:
#line 6909 "fe/idl.ypp" /* yacc.c:1646  */
    {
//         | /* EMPTY */
          (yyval.alval) = 0;
        }
#line 9907 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 561:
#line 6917 "fe/idl.ypp" /* yacc.c:1646  */
    {
// actual_parameter : expression
          // To avoid grammar conflicts with this LALR(1) parser,
          // we take advantage of the fact that an expression can
          // be a scoped name. At that lower level, we create an
          // expression containing the scoped name, and at a
          // higher lever, deduce that it's not supposed to be
          // a constant and look up the type to add to the template
          // arg list.
          AST_Expression *ex = (yyvsp[0].exval);
          UTL_ScopedName *sn = ex->n ();
          AST_Decl *d = 0;
          UTL_Scope *s = idl_global->scopes ().top_non_null ();

          if (sn != 0)
            {
              d = s->lookup_by_name (sn);

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
                      (yyvsp[0].exval)->evaluate (
                        AST_Expression::EK_const);

                      (yyval.dcval) =
                        idl_global->gen ()->create_constant (
                          (yyvsp[0].exval)->ev ()->et,
                          (yyvsp[0].exval),
                          sn);
                    }
                  else
                    {
                      (yyval.dcval) = d;
                    }
                }
            }
          else
            {
              (yyvsp[0].exval)->evaluate (
                AST_Expression::EK_const);

              (yyval.dcval) =
                idl_global->gen ()->create_constant (
                  (yyvsp[0].exval)->ev ()->et,
                  (yyvsp[0].exval),
                  0);
            }
        }
#line 9968 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 563:
#line 6981 "fe/idl.ypp" /* yacc.c:1646  */
    {
// connector_header : IDL_CONNECTOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSeen);
        }
#line 9977 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 564:
#line 6986 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        annotations IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorIDSeen);
        }
#line 9986 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 565:
#line 6991 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        component_inheritance_spec
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Connector *parent = 0;
          bool so_far_so_good = true;

          Identifier id ((yyvsp[-2].strval));
          ACE::strdelete ((yyvsp[-2].strval));
          (yyvsp[-2].strval) = 0;

          UTL_ScopedName sn (&id, 0);

          if ((yyvsp[0].idlist) != 0)
            {
              AST_Decl *d =
                s->lookup_by_name ((yyvsp[0].idlist));

              if (d == 0)
                {
                  idl_global->err ()->lookup_error ((yyvsp[0].idlist));
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

              (yyvsp[0].idlist)->destroy ();
              delete (yyvsp[0].idlist);
              (yyvsp[0].idlist) = 0;
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
#line 10043 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 566:
#line 7047 "fe/idl.ypp" /* yacc.c:1646  */
    {
// connector_body " '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSqSeen);
        }
#line 10052 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 567:
#line 7052 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        connector_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorBodySeen);
        }
#line 10061 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 568:
#line 7057 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        '}
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorQsSeen);

          // Done with this connector - pop it off the scope stack.
          idl_global->scopes ().pop ();
        }
#line 10073 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 571:
#line 7073 "fe/idl.ypp" /* yacc.c:1646  */
    {
// connector_export : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 10082 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 572:
#line 7078 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10091 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 573:
#line 7083 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        | uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 10100 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 574:
#line 7088 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10109 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 575:
#line 7093 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 10118 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 576:
#line 7098 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10127 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 577:
#line 7103 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | extended_port_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 10136 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 578:
#line 7108 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10145 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;


#line 10149 "fe/idl.tab.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 7114 "fe/idl.ypp" /* yacc.c:1906  */

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
