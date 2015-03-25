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
#line 73 "fe/idl.ypp" /* yacc.c:339  */

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
#define TAO_YYDEBUG 1

#line 150 "fe/idl.tab.cpp" /* yacc.c:339  */

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
#line 153 "fe/idl.ypp" /* yacc.c:355  */

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
  UTL_String                    *sval;          /* String value         */
  char                          *wsval;         /* WString value        */
  char                          *strval;        /* char * value         */
  Identifier                    *idval;         /* Identifier           */
  UTL_IdList                    *idlist;        /* Identifier list      */
  AST_Decl::NodeType            ntval;          /* Node type value      */
  FE_Utils::T_Param_Info        *pival;         /* Template interface param */
  FE_Utils::T_PARAMLIST_INFO    *plval;         /* List of template params */
  FE_Utils::T_ARGLIST           *alval;         /* List of template args */

#line 313 "fe/idl.tab.cpp" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE tao_yylval;

int tao_yyparse (void);

#endif /* !YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 328 "fe/idl.tab.cpp" /* yacc.c:358  */

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
#define YYLAST   1338

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  107
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  382
/* YYNRULES -- Number of rules.  */
#define YYNRULES  566
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  816

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
      89,    93,    90,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   105,     2,   106,    95,     2,     2,     2,     2,     2,
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
       0,   359,   359,   362,   363,   367,   370,   373,   379,   384,
     383,   394,   393,   404,   403,   414,   413,   424,   423,   434,
     433,   444,   443,   454,   453,   464,   463,   474,   473,   484,
     483,   494,   493,   504,   503,   514,   513,   528,   527,   540,
     579,   584,   539,   601,   609,   624,   634,   665,   669,   608,
     693,   697,   698,   702,   703,   708,   714,   707,   802,   808,
     801,   879,   880,   885,   924,   929,   884,   947,   946,   960,
     998,  1029,  1063,  1062,  1074,  1081,  1082,  1083,  1084,  1088,
    1099,  1104,  1148,  1153,  1103,  1182,  1226,  1231,  1180,  1250,
    1248,  1290,  1289,  1303,  1309,  1316,  1323,  1330,  1356,  1383,
    1448,  1449,  1453,  1454,  1455,  1461,  1460,  1471,  1470,  1483,
    1484,  1489,  1488,  1499,  1498,  1509,  1508,  1519,  1518,  1529,
    1528,  1539,  1538,  1549,  1548,  1559,  1558,  1572,  1585,  1583,
    1611,  1618,  1629,  1628,  1656,  1654,  1681,  1693,  1739,  1767,
    1798,  1803,  1808,  1813,  1797,  1884,  1885,  1886,  1887,  1888,
    1889,  1890,  1902,  1907,  1976,  1978,  1980,  1981,  1995,  1996,
    2010,  2011,  2024,  2025,  2035,  2048,  2049,  2059,  2072,  2073,
    2083,  2093,  2106,  2107,  2117,  2127,  2140,  2191,  2192,  2201,
    2206,  2212,  2220,  2225,  2230,  2236,  2241,  2246,  2254,  2326,
    2325,  2336,  2341,  2346,  2351,  2378,  2387,  2386,  2458,  2459,
    2463,  2471,  2472,  2500,  2501,  2502,  2503,  2504,  2505,  2506,
    2507,  2511,  2512,  2513,  2514,  2518,  2519,  2520,  2524,  2525,
    2529,  2542,  2540,  2568,  2575,  2576,  2580,  2593,  2591,  2619,
    2626,  2643,  2662,  2663,  2667,  2672,  2677,  2685,  2690,  2695,
    2703,  2708,  2713,  2721,  2729,  2734,  2742,  2750,  2758,  2766,
    2775,  2774,  2790,  2824,  2829,  2789,  2848,  2851,  2852,  2856,
    2856,  2866,  2871,  2865,  2934,  2933,  2948,  2947,  2962,  2967,
    3003,  3008,  3070,  3075,  2961,  3099,  3107,  3121,  3131,  3139,
    3140,  3248,  3251,  3252,  3257,  3262,  3256,  3298,  3297,  3311,
    3322,  3342,  3350,  3349,  3365,  3370,  3364,  3387,  3386,  3439,
    3463,  3488,  3493,  3526,  3531,  3487,  3557,  3562,  3560,  3567,
    3571,  3608,  3613,  3606,  3694,  3755,  3765,  3754,  3778,  3787,
    3792,  3785,  3839,  3865,  3875,  3880,  3873,  3916,  3941,  3950,
    3949,  3991,  4002,  4022,  4030,  4035,  4029,  4097,  4098,  4103,
    4108,  4113,  4118,  4102,  4187,  4192,  4197,  4202,  4186,  4280,
    4285,  4315,  4320,  4279,  4338,  4343,  4408,  4413,  4336,  4450,
    4456,  4463,  4470,  4471,  4483,  4489,  4531,  4482,  4553,  4552,
    4563,  4562,  4575,  4580,  4578,  4585,  4590,  4595,  4589,  4636,
    4635,  4646,  4645,  4658,  4663,  4661,  4668,  4673,  4678,  4672,
    4725,  4733,  4734,  4735,  4845,  4850,  4855,  4864,  4869,  4863,
    4881,  4889,  4894,  4888,  4906,  4914,  4919,  4913,  4931,  4939,
    4944,  4938,  4956,  4963,  4976,  4974,  5000,  5007,  5036,  5074,
    5075,  5079,  5109,  5149,  5154,  5108,  5173,  5178,  5171,  5221,
    5220,  5231,  5238,  5239,  5244,  5243,  5254,  5253,  5264,  5263,
    5274,  5273,  5284,  5283,  5294,  5293,  5304,  5303,  5315,  5406,
    5413,  5439,  5546,  5556,  5562,  5568,  5641,  5714,  5789,  5788,
    5838,  5843,  5848,  5853,  5858,  5863,  5837,  5918,  5917,  5928,
    5935,  5942,  5950,  5955,  5949,  5967,  5968,  5972,  5974,  5973,
    5984,  5983,  5998,  6022,  5996,  6050,  6078,  6048,  6104,  6105,
    6106,  6110,  6111,  6115,  6143,  6174,  6219,  6224,  6172,  6241,
    6251,  6270,  6282,  6281,  6321,  6371,  6376,  6319,  6393,  6398,
    6406,  6411,  6416,  6421,  6426,  6431,  6436,  6441,  6446,  6451,
    6460,  6495,  6494,  6516,  6523,  6549,  6567,  6578,  6598,  6605,
    6616,  6621,  6640,  6645,  6615,  6660,  6667,  6672,  6679,  6678,
    6687,  6686,  6695,  6694,  6706,  6776,  6827,  6843,  6857,  6864,
    6924,  6929,  6934,  6928,  6995,  7000,  6994,  7015,  7016,  7021,
    7020,  7031,  7030,  7041,  7040,  7051,  7050
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
  "'+'", "'-'", "'*'", "'/'", "'%'", "'~'", "'('", "')'", "'['", "']'",
  "$accept", "start", "definitions", "definition",
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
  "interface_forward", "const_dcl", "$@54", "$@55", "$@56", "$@57",
  "const_type", "expression", "const_expr", "or_expr", "xor_expr",
  "and_expr", "shift_expr", "add_expr", "mult_expr", "unary_expr",
  "primary_expr", "literal", "positive_int_expr", "type_dcl", "$@58",
  "type_declarator", "$@59", "type_spec", "simple_type_spec",
  "base_type_spec", "template_type_spec", "constructed_type_spec",
  "constructed_forward_type_spec", "at_least_one_declarator",
  "declarators", "$@60", "declarator", "at_least_one_simple_declarator",
  "simple_declarators", "$@61", "simple_declarator", "complex_declarator",
  "integer_type", "signed_int", "unsigned_int", "floating_pt_type",
  "fixed_type", "char_type", "octet_type", "boolean_type", "any_type",
  "object_type", "struct_decl", "$@62", "struct_type", "$@63", "$@64",
  "$@65", "at_least_one_member", "members", "member", "@66", "member_i",
  "$@67", "$@68", "$@69", "union_decl", "$@70", "union_type", "$@71",
  "$@72", "$@73", "$@74", "$@75", "$@76", "switch_type_spec",
  "at_least_one_case_branch", "case_branches", "case_branch", "$@77",
  "$@78", "$@79", "at_least_one_case_label", "case_labels", "case_label",
  "$@80", "$@81", "$@82", "element_spec", "$@83", "struct_forward_type",
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
      47,    37,   126,    40,    41,    91,    93
};
# endif

#define YYPACT_NINF -568

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-568)))

#define YYTABLE_NINF -507

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -568,   101,  1189,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,    34,    39,    55,    98,  -568,    34,    34,
    -568,    37,    37,  -568,  -568,  -568,  -568,   -21,  -568,   631,
      12,    43,  -568,  -568,    14,  -568,  -568,  -568,  -568,  -568,
    -568,   170,  -568,  -568,  -568,  -568,  -568,  -568,    54,  -568,
     126,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,    77,
    -568,  -568,  -568,    77,  -568,  -568,    90,   119,   286,    37,
      34,   580,    34,    34,    34,    34,  -568,  -568,  -568,    69,
      34,    72,  -568,   106,    34,  -568,    77,    34,   128,   131,
      34,  -568,    16,  -568,    75,   209,   222,  -568,   140,   143,
    1158,    36,  -568,  -568,   158,   212,  -568,   189,   202,   203,
      44,  -568,    67,  -568,  -568,  -568,  -568,  -568,  -568,   220,
    -568,  -568,  -568,  -568,  -568,  -568,   232,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,   126,  -568,  -568,  -568,    84,  -568,
    -568,   229,  -568,   249,   253,   255,   262,  -568,   264,   265,
     266,   267,   268,   270,   271,   273,  -568,  -568,  -568,   274,
     275,  -568,  -568,  -568,  -568,   232,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,   232,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,   279,  -568,   277,  -568,  -568,   263,
    -568,    34,  -568,  -568,  -568,  -568,  -568,  -568,  -568,   286,
    -568,  -568,  -568,  -568,   287,  -568,  -568,  -568,  -568,   354,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,    51,    51,    51,    36,   232,  -568,  -568,   281,   291,
     293,   120,   122,   110,  -568,  -568,  -568,   292,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,   385,  -568,    36,
     306,   349,  -568,  -568,  -568,  -568,  -568,  -568,  -568,   294,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,   349,
     312,   313,  -568,  -568,  -568,    34,    34,   314,   315,  -568,
    -568,  -568,   307,  -568,    34,   316,   263,  -568,  -568,   401,
    -568,   317,   318,  -568,  -568,  -568,   301,    36,    36,    36,
      36,    36,    36,    36,    36,    36,    36,  -568,   321,    37,
    -568,  -568,  -568,  -568,   322,  -568,    37,  -568,    36,    36,
      36,  -568,  -568,  -568,  -568,  -568,  -568,  -568,   139,  -568,
     303,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,    37,
     349,  -568,  -568,  -568,  -568,  -568,  1267,  -568,   319,  -568,
    -568,  -568,   291,   293,   120,   122,   122,   110,   110,  -568,
    -568,  -568,    34,    36,  -568,   232,  1023,   777,    36,   223,
    -568,  -568,  -568,  -568,  -568,  -568,   254,    80,   360,   941,
    -568,  -568,  -568,  -568,    38,   357,    37,    37,  -568,  -568,
    -568,  -568,  -568,    38,  -568,   328,  -568,   324,   325,   326,
     416,  -568,  1058,   232,  -568,    37,   349,   185,   334,   336,
     423,   345,  1158,  -568,  -568,   341,  -568,  -568,  -568,   346,
    -568,  -568,  -568,  -568,  -568,  -568,   724,  -568,  -568,  -568,
    -568,  -568,   347,  -568,  -568,  -568,   350,   348,  -568,   351,
     353,   359,   352,   356,  -568,  -568,  -568,   429,   232,  -568,
    -568,  -568,  -568,  -568,  -568,    37,    37,    37,  -568,   362,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,    34,    34,  -568,
     363,  -568,  -568,  -568,  1140,   859,   408,   311,  -568,   232,
      34,  -568,  -568,    41,    62,   366,   369,   373,    34,   374,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,   375,
     377,   232,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,   380,  -568,   381,   383,   384,   386,   387,
    -568,   232,  -568,  -568,  -568,  -568,  -568,   390,   391,   468,
     254,   254,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
     393,    34,   382,    63,    63,    63,   399,  -568,   402,   406,
     407,   410,   412,   413,  -568,  -568,  -568,   414,   415,   418,
     419,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,    36,    36,   325,    34,   420,   411,   421,  -568,   436,
     424,   428,   432,   427,   185,  1242,    37,  -568,  -568,  -568,
    -568,  -568,  -568,   516,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,   430,   430,  -568,  -568,  -568,  -568,   311,    34,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,    37,  -568,  -568,  -568,  -568,   438,   232,
    -568,   430,   444,   447,   500,  -568,  -568,  -568,  -568,  -568,
     431,   416,    37,   232,   448,   441,  -568,   435,  -568,  -568,
    -568,   437,   504,   502,   502,    34,   487,   451,  -568,  -568,
     232,   455,  -568,  -568,   442,  -568,   502,    45,  -568,  -568,
     443,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,   486,
     546,   449,   187,   502,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,   311,  -568,   462,   452,   502,   453,   495,    34,
      37,  -568,  -568,   469,  -568,  -568,  -568,  -568,  -568,   454,
    -568,  -568,  -568,   474,    36,   470,   475,    35,   580,   230,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,   232,  -568,
     473,  -568,   476,   311,   523,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,    34,   504,    37,    37,   463,    34,   546,
    -568,  -568,  -568,  -568,   478,    34,   481,  -568,  -568,   466,
     467,  -568,  -568,  -568,   187,    34,   471,  -568,  -568,  -568,
    -568,  -568,    37,  -568,  -568,  -568,   472,   498,  -568,  -568,
     480,   483,  -568,  -568,   503,  -568
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,     0,     1,    35,   140,    37,    67,   189,   250,
     266,   301,   349,     0,     0,     0,     0,    91,     0,     0,
     460,     0,     0,   530,   551,     3,     5,    39,    21,    58,
       0,     0,    19,    61,    74,    63,    23,    75,    80,    76,
      81,    74,    77,    78,    62,    15,     9,   195,   252,   191,
     300,   192,   218,   219,   193,    17,    11,    13,    25,   420,
     419,   422,    27,   458,    29,   490,   492,   491,   489,    74,
     508,   509,   488,    74,    31,    33,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   136,   230,   194,    74,
       0,    74,    85,    74,     0,    79,    74,     0,   426,   501,
       0,   132,     0,   131,     0,     0,     0,    43,     0,     0,
       0,     0,     6,     7,     0,    94,    69,     0,     0,     0,
     234,   236,     0,   240,   241,   244,   245,   246,   247,   243,
     248,   249,   315,   323,   328,    89,   202,    99,   198,   200,
     201,   199,   203,   232,   233,   204,   208,   205,   207,   206,
     209,   210,   252,   215,     0,   216,   217,   211,     0,   214,
     212,   322,   213,   327,     0,     0,     0,   268,     0,     0,
       0,     0,     0,     0,     0,     0,   502,   495,   504,     0,
       0,   554,   550,    36,   243,   153,   141,   145,   149,   150,
     146,   147,   148,   151,   152,    38,    68,   190,   196,   251,
     267,   302,   350,    70,   499,    71,     0,   500,    92,   431,
     461,     0,   417,   134,   418,   531,   552,    40,    22,     0,
     515,   511,   512,   517,   514,   518,   516,   513,   510,     0,
      45,   523,   179,   180,   181,   183,   185,   186,   187,   184,
     182,     0,     0,     0,     0,   176,   549,   154,   155,   156,
     158,   160,   162,   165,   168,   172,   177,     0,   548,    20,
      93,    72,    64,    24,    82,   235,   242,   237,   239,     0,
       0,    96,   314,   311,   319,   324,    16,    10,   253,     0,
      18,    12,    14,    26,   423,    28,   472,   459,    30,    96,
       0,     0,    32,    34,   558,     0,     0,     0,     0,    86,
     429,   427,   469,   133,     0,     0,   431,     4,   519,     0,
     524,     0,   520,   173,   174,   175,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    59,   546,     0,
     110,   101,   238,   188,     0,   316,     0,    90,     0,     0,
       0,   259,   269,   433,   476,   503,   496,   505,   555,   142,
     230,   197,   223,   224,   225,   231,   303,   351,   110,     0,
      96,   467,   462,   135,   532,   553,     0,    41,     0,    46,
     521,   178,   157,   159,   161,   163,   164,   166,   167,   169,
     170,   171,     0,     0,    73,   130,     0,     0,     0,     0,
      95,   312,   320,   325,   254,   258,     0,     0,   424,     0,
     110,   101,   339,   344,     0,   454,     0,     0,   563,   337,
     338,   559,   561,     0,   565,     0,   557,     0,     0,   220,
       0,   258,     0,   430,   428,     0,    96,     0,     3,     0,
       0,     0,     0,    60,   547,   127,   125,   359,   360,     0,
     109,   117,   111,   121,   119,   123,     0,   113,   115,   364,
     107,   105,     0,   100,   102,   103,     0,     0,   317,     0,
       0,     0,     0,   259,   264,   261,   260,   234,   280,   275,
     276,   277,   278,   270,   279,     0,     0,     0,   444,     0,
     432,   434,   436,   438,   440,   442,   446,     0,     0,   477,
       0,   475,   478,   480,     0,     0,     0,     0,   450,   449,
       0,   453,   452,     0,     0,     0,     0,     0,     0,     0,
     556,   143,   334,   330,   333,   221,   310,   304,   309,   259,
       0,   468,   463,   542,   538,   540,   533,   537,    42,   525,
      47,   522,   128,     0,    66,     0,     0,     0,     0,     0,
     363,   393,   390,   391,   392,   354,   362,     0,     0,     0,
       0,     0,    84,   104,   318,   313,   321,   326,   255,   257,
       0,     0,     0,     0,     0,     0,     0,   425,     0,     0,
       0,     0,     0,     0,   482,   485,   474,     0,     0,     0,
       0,   340,   345,   448,   544,   545,   564,   560,   562,   451,
     566,     0,     0,   331,     0,     0,   306,     0,    88,     0,
       0,     0,     0,     0,   535,     0,     0,   126,   118,   112,
     122,   120,   124,     0,   114,   116,   365,   108,   106,   265,
     262,   271,   457,   455,   456,   445,   435,   437,   439,   441,
     443,   447,     0,     0,   479,   481,   498,   507,     0,     0,
     144,   335,   332,   222,   305,   307,   353,   464,   543,   539,
     541,   534,   536,     0,    53,    39,    48,    52,     0,   129,
     355,     0,     0,     0,   368,   483,   486,   341,   346,   229,
       0,     0,     0,    55,     0,     0,    54,     0,   366,   263,
     272,     0,     0,   400,   400,     0,   404,   226,   336,   308,
     465,     0,    49,    51,   381,   356,   400,     0,   369,   376,
       0,   375,   397,   484,   487,   342,   401,   347,   227,   471,
       0,     0,     0,   400,   367,   287,   294,   292,   273,   283,
     284,   291,     0,   371,   372,     0,   400,     0,   408,     0,
       0,   466,   529,     0,   528,   380,   394,   395,   396,     0,
     386,   387,   357,     0,     0,     0,     0,     0,     0,   289,
     377,   373,   398,   343,   402,   405,   348,   228,   470,    56,
     526,   382,   383,     0,   412,   288,   295,   293,   274,   282,
     297,   285,   290,     0,     0,     0,     0,     0,     0,     0,
     384,   388,   409,   358,     0,     0,     0,   378,   374,     0,
       0,   406,    57,   527,     0,     0,     0,   296,   298,   286,
     399,   403,     0,   385,   389,   410,     0,     0,   407,   416,
       0,   413,   411,   414,     0,   415
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -568,  -568,   272,   211,  -568,  -526,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -523,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,   -95,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,   278,  -568,  -568,
      68,  -568,  -568,  -568,   566,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,   570,  -568,   300,  -568,  -568,  -254,  -568,  -568,
     201,  -568,  -568,  -568,  -568,  -315,  -349,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -322,  -568,  -568,   -19,  -568,
    -568,   -13,  -568,     8,  -568,  -568,  -568,  -568,   367,    19,
    -102,  -568,   290,   296,   289,    11,    25,   -72,    65,  -568,
    -307,    13,  -568,  -568,  -568,   -33,   226,  -417,  -568,  -568,
    -568,    48,  -568,  -568,  -567,   -73,  -568,  -568,   -12,  -568,
     -55,  -568,  -568,   -48,   -44,   -54,   -53,   -52,  -568,  -568,
     -30,  -568,   -28,  -568,  -568,  -568,  -568,   190,   276,  -568,
    -217,  -568,  -568,  -568,   -25,  -568,   -23,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -131,  -568,  -568,  -568,
    -568,  -568,  -128,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
     -34,  -568,  -568,  -568,  -568,  -568,  -568,  -568,   -47,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,   -74,  -568,  -568,  -568,
     -66,  -568,  -568,  -568,  -568,  -568,  -568,  -568,    32,  -568,
    -568,  -320,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,    15,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -547,  -568,
    -568,  -568,  -568,  -568,  -146,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -165,  -568,  -568,  -476,  -568,  -483,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,    17,    18,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,   327,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -306,   217,  -299,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,   558,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
     200,  -568,  -568,  -143,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,    40,  -568,  -568,  -568,   240,  -568,  -568,   256,  -568,
    -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,  -568,
    -568,  -568
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    25,   367,    26,   165,   169,   170,   164,
     168,   114,   109,   118,   171,   173,   175,   179,   180,    77,
      27,    79,    28,   108,   307,   429,    29,    30,   110,   311,
     431,   605,   674,   656,   675,   657,   658,   691,   778,    31,
     111,   382,    32,    33,   117,   330,   439,    34,    80,    35,
     135,   329,    36,    37,    38,   119,   331,   452,    39,   206,
     358,   520,    40,   271,    41,    97,   261,   337,    42,    43,
     387,   453,   454,   551,   550,   386,   440,   536,   547,   548,
     535,   538,   537,   539,   533,   384,   435,   606,   245,   211,
     304,   103,    44,   441,    78,   295,   417,   591,   186,   246,
     333,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     334,   442,    81,   197,   296,   465,   138,   139,   140,   141,
      47,   351,   419,   594,   352,   668,   687,   729,   353,   354,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
      48,    82,    49,   166,   341,   462,   394,   463,   559,   396,
     466,   561,   662,   560,    50,    83,    51,   279,   397,   562,
     663,   697,   746,   473,   718,   747,   719,   748,   786,   743,
     720,   749,   721,   745,   744,   784,   771,   785,    52,    53,
      54,    84,   297,   420,   595,   517,   596,   671,   518,   157,
     338,   459,   158,   270,   389,   159,   160,   339,   460,   161,
     162,   340,   461,   163,   355,   418,   513,   593,   514,   592,
     670,   443,   409,   496,   638,   685,   726,   410,   497,   639,
     686,   728,   444,    85,   298,   421,   597,   445,   613,   677,
     713,   764,   446,   545,   456,   549,   661,   696,   665,   681,
     682,   700,   724,   774,   701,   722,   773,   695,   711,   712,
     739,   762,   794,   740,   763,   795,   546,   741,   703,   725,
     775,   707,   727,   776,   756,   777,   802,   783,   796,   807,
     810,   811,   814,   447,   448,    58,    59,    60,   172,   343,
     479,    61,   209,   360,   301,   359,   398,   480,   568,   569,
     570,   571,   572,   566,   573,   524,   500,   525,   413,   502,
     483,   484,   485,    62,   174,    63,   100,   302,   426,   599,
     672,   709,   362,   425,   731,   287,   344,   490,   399,   491,
     577,   578,   492,   632,   683,   493,   633,   684,    64,    65,
      66,    67,    68,   290,   400,   579,    69,    70,    71,   177,
     289,    72,   291,   401,   580,    73,   229,   230,   312,   432,
     231,   733,   760,   734,    74,   105,   305,   427,   603,   526,
     604,   527,   601,   602,   600,   414,   257,   328,   258,    75,
      76,   106,   306,   182,   294,   415,   348,   416,   506,   507,
     505,   509
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      87,    88,   102,   104,   193,    98,    99,   156,   137,   247,
      45,   152,   194,   153,   390,    46,   154,    55,   155,    56,
      57,   582,   136,   187,   190,   191,   192,   643,   408,   542,
     188,   391,   392,   393,   189,   345,   715,    86,   455,    86,
      86,    86,   411,   422,   584,     7,   715,   156,   198,   412,
     489,   152,   265,   153,    86,   266,   154,   498,   155,   185,
     195,     7,   136,   716,   717,   585,    86,   196,   107,   199,
     200,   201,   202,   716,   717,   267,   268,   204,   478,   654,
     542,   207,   655,    86,   208,   494,   666,   210,   467,   121,
     122,   212,   481,   125,   126,   127,   128,   213,   112,   482,
    -137,     3,   116,    11,    17,   115,   424,   523,    90,   232,
     233,   234,   235,   236,   678,   237,   238,   101,   101,   101,
     239,   240,   213,  -281,   232,   233,   234,   235,   236,   113,
     237,   238,   101,   241,   242,   239,   240,   176,   243,   244,
    -299,   176,   316,   213,   213,   193,   455,    17,   167,   654,
     214,    94,   655,   194,   244,  -138,   213,   203,  -139,   205,
     115,   101,   667,   115,   187,   190,   191,   192,   115,   402,
     403,   188,   522,    86,   272,   189,   273,   181,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
       9,    10,   -97,    11,   132,   133,   134,   115,   303,   404,
     185,   704,   320,   321,   405,   183,   787,   406,   407,   324,
     325,   326,   215,   714,  -421,   402,   403,  -493,   798,   322,
     323,   542,   736,   737,   738,   216,    86,   217,   804,   218,
     742,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   753,   259,   404,   750,   132,   133,   134,
     405,   101,   379,   380,   381,   464,   -98,    86,   716,   717,
     260,   115,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,     9,    10,   262,    11,   132,   133,
     134,   247,   349,   350,   523,   641,   457,   781,   263,    86,
     264,   363,    89,    91,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   184,   101,   542,   313,   314,   315,   269,
     385,   133,   134,   213,    86,    93,    96,   385,   274,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   184,   130,
     131,   375,   376,   617,   618,   101,   133,   134,   275,   276,
     423,   277,   469,   470,   471,   472,   542,   377,   378,   278,
     280,   281,   282,   283,   300,   284,   285,   310,   286,   288,
     292,   293,   156,   474,   299,  -494,   152,   101,   153,   433,
     136,   154,   543,   155,    45,   317,   309,   136,   468,    46,
     544,    55,   327,    56,    57,   499,   318,   503,   504,   319,
     402,   403,   101,   332,   499,   335,   336,   342,   361,   346,
     347,   356,   357,   364,   368,   371,   521,   369,  -329,   430,
     370,   475,   476,   383,   388,   501,   510,   511,   515,   516,
     404,   477,    -8,   543,   528,   405,   529,   541,   406,   407,
     512,   544,   530,   532,   534,   552,   553,   265,   554,   581,
     558,   555,     4,   556,  -256,     5,     6,     7,     8,   557,
     567,   576,   586,   789,   790,   587,   563,   564,   565,   588,
     590,     9,    10,  -352,    11,   598,   607,   608,    12,   609,
     610,   616,   611,   612,   574,   575,   614,   615,   541,   619,
     806,    13,    14,    15,    16,   625,   621,   583,   626,   247,
      17,    18,   627,   628,    19,   589,   629,    20,   630,   631,
     634,   635,   647,   645,    21,    22,   636,   637,   644,   646,
     648,    23,    24,   653,   649,   651,   156,   156,   650,   660,
     152,   152,   153,   153,   676,   154,   154,   155,   155,   -50,
     679,   136,   136,   664,   680,  -370,   692,   688,   694,   699,
     702,   698,   706,   708,   710,   730,  -379,   723,   350,   732,
     622,   623,   624,   735,   751,   752,   754,   755,   761,   759,
     765,   767,   782,   768,   543,   779,   791,   799,   780,   797,
     800,   801,   544,   809,   805,   813,   808,   428,   815,   366,
     693,   350,    95,    86,   812,    92,   308,   659,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
       9,    10,   495,    11,   132,   133,   134,   372,   374,   620,
     640,   519,   705,    45,   373,   458,   769,   395,    46,   541,
      55,   772,    56,    57,   689,   642,    87,   669,   788,   803,
     508,   178,   531,   365,   673,   -44,   793,   -44,   486,   434,
       0,     0,   766,     0,   652,     0,     0,     0,   543,     0,
       0,   -44,   -44,   690,   -44,   -44,   544,     0,   -44,     0,
       0,   101,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,   669,     0,     0,     0,     0,     0,     0,
     -44,     0,     0,    45,   -44,     0,     0,     0,    46,   543,
      55,     0,    56,    57,     0,     0,     0,   544,   -44,     0,
       0,     0,     0,   541,     0,     0,     0,     0,     0,     0,
       0,   758,     0,     0,   156,   770,    87,   757,   152,     0,
     153,     0,     0,   154,     0,   155,     0,    86,     0,   136,
       0,     0,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   184,   130,   131,   541,     0,     0,     0,     0,   133,
     134,     0,     0,     0,     0,     0,   385,   385,   540,     0,
     350,     0,     0,     0,     0,   792,     0,     0,     0,     0,
       0,     0,   350,     0,     0,     0,     0,     0,   436,     0,
    -361,     5,   350,   385,     8,  -361,  -361,  -361,  -361,  -361,
    -361,  -361,  -361,  -361,  -361,  -361,  -361,     9,    10,     0,
      11,     0,  -361,  -361,    12,   101,     0,   402,   403,   437,
     438,  -361,     0,     0,     0,     0,     0,    13,     0,     0,
       0,   449,   450,   451,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,    22,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -361,     0,
     436,     0,  -361,     5,     0,   -83,     8,  -361,  -361,  -361,
    -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,     9,
      10,     0,    11,     0,  -361,  -361,    12,     0,     0,   402,
     403,   437,   438,  -361,     0,     0,     0,     0,     0,    13,
       0,     0,     0,   449,   450,   451,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    21,    22,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -361,     0,   436,     0,  -361,     5,     0,  -506,     8,  -361,
    -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,
    -361,     9,    10,     0,    11,     0,  -361,  -361,    12,     0,
       0,   402,   403,   437,   438,  -361,     0,     0,     0,     0,
       0,    13,     0,     0,     0,   487,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   488,     0,     0,     0,     0,
       0,     0,     0,     0,    21,    22,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -361,     0,   436,     0,  -361,     5,     0,  -473,
       8,  -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,
    -361,  -361,  -361,     9,    10,     0,    11,     0,  -361,  -361,
      12,     0,     0,   402,   403,   437,   438,  -361,     0,   436,
       0,  -361,     5,    13,     0,     8,  -361,  -361,  -361,  -361,
    -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,     9,    10,
       0,    11,     0,  -361,  -361,    12,    21,    22,   402,   403,
     437,   438,  -361,     0,     0,     0,     0,     0,    13,     0,
       0,     0,     0,     0,  -361,     0,     0,     0,     0,     0,
       0,   -65,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    21,    22,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -361,
       0,   436,     0,  -361,     5,     0,   -87,     8,  -361,  -361,
    -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,  -361,
       9,    10,   219,    11,   220,  -361,  -361,    12,     0,     0,
     402,   403,   437,   438,  -361,     0,     0,     0,   221,   222,
      13,   223,   224,     0,     0,   225,     0,     0,     0,    -2,
       4,     0,     0,     5,     6,     7,     8,     0,     0,     0,
       0,     0,     0,    21,    22,     0,     0,   226,     0,     9,
      10,   227,    11,     0,     0,     0,    12,     0,     0,     0,
       0,  -361,     0,     0,     0,   228,     0,     0,  -497,    13,
      14,    15,    16,     0,     0,     0,     0,     0,    17,    18,
       0,     0,    19,     4,     0,    20,     5,     6,     7,     8,
       0,     0,    21,    22,     0,     0,     0,     0,     0,    23,
      24,     0,     9,    10,     0,    11,     0,     0,     4,    12,
       0,     5,     6,     7,     8,     0,     0,     0,     0,     0,
       0,     0,    13,    14,    15,    16,     0,     9,    10,     0,
      11,    17,    18,     0,    12,    19,     0,     0,    20,     0,
       0,     0,     0,     0,     0,    21,    22,    13,    14,    15,
      16,     0,    23,    24,   653,     0,    17,    18,     0,     0,
      19,     0,     0,    20,     0,     0,     0,     0,     0,     0,
      21,    22,     0,     0,     0,     0,     0,    23,    24
};

static const yytype_int16 yycheck[] =
{
      13,    13,    21,    22,    78,    18,    19,    41,    41,   111,
       2,    41,    78,    41,   336,     2,    41,     2,    41,     2,
       2,   497,    41,    78,    78,    78,    78,   594,   348,   446,
      78,   338,   339,   340,    78,   289,     1,     3,   387,     3,
       3,     3,   348,   358,     3,     6,     1,    81,    81,   348,
     399,    81,     8,    81,     3,    11,    81,    19,    81,    78,
      79,     6,    81,    28,    29,     3,     3,    80,    89,    82,
      83,    84,    85,    28,    29,     8,     9,    90,   398,   605,
     497,    94,   605,     3,    97,   400,   633,   100,     8,     9,
      10,    75,   398,    13,    14,    15,    16,    81,    86,   398,
      86,     0,    34,    23,    49,    91,   360,   427,    53,    73,
      74,    75,    76,    77,   661,    79,    80,    81,    81,    81,
      84,    85,    81,    88,    73,    74,    75,    76,    77,    86,
      79,    80,    81,    97,    98,    84,    85,    69,   102,   103,
      86,    73,   244,    81,    81,   219,   495,    49,    22,   675,
      75,    53,   675,   219,   103,    86,    81,    89,    86,    91,
      91,    81,   638,    91,   219,   219,   219,   219,    91,    30,
      31,   219,   426,     3,    90,   219,    92,    87,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    86,    23,    24,    25,    26,    91,   211,    60,
     219,   684,    82,    83,    65,    86,   773,    68,    69,    99,
     100,   101,     3,   696,    86,    30,    31,    86,   785,    97,
      98,   638,    35,    36,    37,     3,     3,    87,   795,    86,
     713,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,   726,    86,    60,   722,    24,    25,    26,
      65,    81,   324,   325,   326,     1,    86,     3,    28,    29,
      48,    91,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    87,    23,    24,    25,
      26,   383,   295,   296,   604,   592,   388,   763,    86,     3,
      87,   304,    14,    15,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    81,   722,   241,   242,   243,    89,
     329,    25,    26,    81,     3,    15,    16,   336,    89,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,   320,   321,   550,   551,    81,    25,    26,    89,    86,
     359,    86,   397,   397,   397,   397,   763,   322,   323,    87,
      86,    86,    86,    86,    91,    87,    86,     3,    87,    86,
      86,    86,   396,   397,    87,    86,   396,    81,   396,   382,
     389,   396,   446,   396,   366,    94,    89,   396,   397,   366,
     446,   366,    90,   366,   366,   404,    95,   406,   407,    96,
      30,    31,    81,     8,   413,    89,    47,   103,    91,    87,
      87,    87,    87,    87,     3,   104,   425,    90,   105,    90,
      92,    51,    52,    92,    92,    58,    88,    93,    92,     3,
      60,    61,    88,   497,    88,    65,     3,   446,    68,    69,
     105,   497,    87,    92,    88,    88,    86,     8,    90,    31,
      88,    90,     1,    90,    88,     4,     5,     6,     7,    90,
      88,    88,    86,   775,   776,    86,   475,   476,   477,    86,
      86,    20,    21,    88,    23,    88,    86,    86,    27,    86,
      86,     3,    86,    86,   487,   488,    86,    86,   497,    86,
     802,    40,    41,    42,    43,    86,   104,   500,    86,   591,
      49,    50,    86,    86,    53,   508,    86,    56,    86,    86,
      86,    86,    66,    92,    63,    64,    88,    88,    88,    88,
      86,    70,    71,    72,    86,    88,   550,   551,    86,     3,
     550,   551,   550,   551,    86,   550,   551,   550,   551,    88,
      86,   550,   551,   103,    87,    35,    88,   106,   103,    35,
      38,   104,    55,    92,    89,    59,   104,   104,   561,     3,
     563,   564,   565,   104,    92,   103,   103,    62,   104,    90,
      86,    91,    39,    88,   638,    92,   103,    86,    92,    91,
     104,   104,   638,    75,   103,    92,   104,   366,    75,   307,
     675,   594,    16,     3,   104,    15,   219,   606,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,   401,    23,    24,    25,    26,   317,   319,   561,
     591,   421,   685,   605,   318,   389,   747,   341,   605,   638,
     605,   749,   605,   605,   671,   593,   639,   639,   774,   794,
     413,    73,   432,   306,   653,     4,   779,     6,   398,   383,
      -1,    -1,   744,    -1,   604,    -1,    -1,    -1,   722,    -1,
      -1,    20,    21,   672,    23,    24,   722,    -1,    27,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   685,   685,    -1,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,   675,    53,    -1,    -1,    -1,   675,   763,
     675,    -1,   675,   675,    -1,    -1,    -1,   763,    67,    -1,
      -1,    -1,    -1,   722,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   730,    -1,    -1,   748,   748,   729,   729,   748,    -1,
     748,    -1,    -1,   748,    -1,   748,    -1,     3,    -1,   748,
      -1,    -1,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,   763,    -1,    -1,    -1,    -1,    25,
      26,    -1,    -1,    -1,    -1,    -1,   775,   776,    34,    -1,
     773,    -1,    -1,    -1,    -1,   778,    -1,    -1,    -1,    -1,
      -1,    -1,   785,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,   795,   802,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      23,    -1,    25,    26,    27,    81,    -1,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,
       1,    -1,     3,     4,    -1,    88,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    -1,    25,    26,    27,    -1,    -1,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    44,    45,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,     1,    -1,     3,     4,    -1,    88,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    -1,    25,    26,    27,    -1,
      -1,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,     1,    -1,     3,     4,    -1,    88,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    23,    -1,    25,    26,
      27,    -1,    -1,    30,    31,    32,    33,    34,    -1,     1,
      -1,     3,     4,    40,    -1,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    23,    -1,    25,    26,    27,    63,    64,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      -1,     1,    -1,     3,     4,    -1,    88,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,     4,    23,     6,    25,    26,    27,    -1,    -1,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    20,    21,
      40,    23,    24,    -1,    -1,    27,    -1,    -1,    -1,     0,
       1,    -1,    -1,     4,     5,     6,     7,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    49,    -1,    20,
      21,    53,    23,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    67,    -1,    -1,    88,    40,
      41,    42,    43,    -1,    -1,    -1,    -1,    -1,    49,    50,
      -1,    -1,    53,     1,    -1,    56,     4,     5,     6,     7,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    20,    21,    -1,    23,    -1,    -1,     1,    27,
      -1,     4,     5,     6,     7,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    41,    42,    43,    -1,    20,    21,    -1,
      23,    49,    50,    -1,    27,    53,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    64,    40,    41,    42,
      43,    -1,    70,    71,    72,    -1,    49,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    70,    71
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   108,   109,     0,     1,     4,     5,     6,     7,    20,
      21,    23,    27,    40,    41,    42,    43,    49,    50,    53,
      56,    63,    64,    70,    71,   110,   112,   127,   129,   133,
     134,   146,   149,   150,   154,   156,   159,   160,   161,   165,
     169,   171,   175,   176,   199,   200,   218,   227,   247,   249,
     261,   263,   285,   286,   287,   329,   380,   381,   382,   383,
     384,   388,   410,   412,   435,   436,   437,   438,   439,   443,
     444,   445,   448,   452,   461,   476,   477,   126,   201,   128,
     155,   219,   248,   262,   288,   330,     3,   198,   235,   154,
      53,   154,   169,   171,    53,   161,   171,   172,   198,   198,
     413,    81,   195,   198,   195,   462,   478,    89,   130,   119,
     135,   147,    86,    86,   118,    91,   157,   151,   120,   162,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    24,    25,    26,   157,   195,   222,   223,   224,
     225,   226,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   249,   261,   263,   287,   296,   299,   302,
     303,   306,   307,   310,   116,   113,   250,    22,   117,   114,
     115,   121,   385,   122,   411,   123,   157,   446,   446,   124,
     125,    87,   480,    86,    17,   195,   205,   237,   240,   241,
     242,   243,   244,   303,   307,   195,   198,   220,   222,   198,
     198,   198,   198,   157,   198,   157,   166,   198,   198,   389,
     198,   196,    75,    81,    75,     3,     3,    87,    86,     4,
       6,    20,    21,    23,    24,    27,    49,    53,    67,   453,
     454,   457,    73,    74,    75,    76,    77,    79,    80,    84,
      85,    97,    98,   102,   103,   195,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   473,   475,    86,
      48,   173,    87,    86,    87,     8,    11,     8,     9,    89,
     300,   170,    90,    92,    89,    89,    86,    86,    87,   264,
      86,    86,    86,    86,    87,    86,    87,   422,    86,   447,
     440,   449,    86,    86,   481,   202,   221,   289,   331,    87,
      91,   391,   414,   198,   197,   463,   479,   131,   205,    89,
       3,   136,   455,   215,   215,   215,   207,    94,    95,    96,
      82,    83,    97,    98,    99,   100,   101,    90,   474,   158,
     152,   163,     8,   207,   217,    89,    47,   174,   297,   304,
     308,   251,   103,   386,   423,   174,    87,    87,   483,   198,
     198,   228,   231,   235,   236,   311,    87,    87,   167,   392,
     390,    91,   419,   198,    87,   391,   109,   111,     3,    90,
      92,   104,   209,   210,   211,   212,   212,   213,   213,   214,
     214,   214,   148,    92,   192,   195,   182,   177,    92,   301,
     192,   217,   217,   217,   253,   255,   256,   265,   393,   425,
     441,   450,    30,    31,    60,    65,    68,    69,   318,   319,
     324,   402,   404,   405,   472,   482,   484,   203,   312,   229,
     290,   332,   182,   195,   174,   420,   415,   464,   110,   132,
      90,   137,   456,   198,   475,   193,     1,    32,    33,   153,
     183,   200,   218,   318,   329,   334,   339,   380,   381,    44,
      45,    46,   164,   178,   179,   183,   341,   207,   223,   298,
     305,   309,   252,   254,     1,   222,   257,     8,   195,   237,
     242,   243,   244,   270,   287,    51,    52,    61,   318,   387,
     394,   402,   404,   407,   408,   409,   472,    44,    54,   183,
     424,   426,   429,   432,   182,   177,   320,   325,    19,   195,
     403,    58,   406,   195,   195,   487,   485,   486,   403,   488,
      88,    93,   105,   313,   315,    92,     3,   292,   295,   254,
     168,   195,   174,   318,   402,   404,   466,   468,    88,     3,
      87,   457,    92,   191,    88,   187,   184,   189,   188,   190,
      34,   195,   224,   303,   307,   340,   363,   185,   186,   342,
     181,   180,    88,    86,    90,    90,    90,    90,    88,   255,
     260,   258,   266,   195,   195,   195,   400,    88,   395,   396,
     397,   398,   399,   401,   198,   198,    88,   427,   428,   442,
     451,    31,   363,   198,     3,     3,    86,    86,    86,   198,
      86,   204,   316,   314,   230,   291,   293,   333,    88,   416,
     471,   469,   470,   465,   467,   138,   194,    86,    86,    86,
      86,    86,    86,   335,    86,    86,     3,   257,   257,    86,
     228,   104,   198,   198,   198,    86,    86,    86,    86,    86,
      86,    86,   430,   433,    86,    86,    88,    88,   321,   326,
     206,   217,   315,   231,    88,    92,    88,    66,    86,    86,
      86,    88,   468,    72,   112,   127,   140,   142,   143,   195,
       3,   343,   259,   267,   103,   345,   345,   363,   232,   235,
     317,   294,   417,   195,   139,   141,    86,   336,   345,    86,
      87,   346,   347,   431,   434,   322,   327,   233,   106,   295,
     195,   144,    88,   142,   103,   354,   344,   268,   104,    35,
     348,   351,    38,   365,   365,   232,    55,   368,    92,   418,
      89,   355,   356,   337,   365,     1,    28,    29,   271,   273,
     277,   279,   352,   104,   349,   366,   323,   369,   328,   234,
      59,   421,     3,   458,   460,   104,    35,    36,    37,   357,
     360,   364,   365,   276,   281,   280,   269,   272,   274,   278,
     363,    92,   103,   365,   103,    62,   371,   235,   195,    90,
     459,   104,   358,   361,   338,    86,   207,    91,    88,   273,
     222,   283,   279,   353,   350,   367,   370,   372,   145,    92,
      92,   363,    39,   374,   282,   284,   275,   231,   351,   192,
     192,   103,   198,   460,   359,   362,   375,    91,   231,    86,
     104,   104,   373,   360,   231,   103,   192,   376,   104,    75,
     377,   378,   104,    92,   379,    75
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   107,   108,   109,   109,   110,   110,   110,   111,   113,
     112,   114,   112,   115,   112,   116,   112,   117,   112,   118,
     112,   119,   112,   120,   112,   121,   112,   122,   112,   123,
     112,   124,   112,   125,   112,   126,   112,   128,   127,   130,
     131,   132,   129,   133,   135,   136,   137,   138,   139,   134,
     140,   141,   141,   142,   142,   144,   145,   143,   147,   148,
     146,   149,   149,   151,   152,   153,   150,   155,   154,   156,
     156,   156,   158,   157,   157,   159,   159,   159,   159,   160,
     160,   162,   163,   164,   161,   166,   167,   168,   165,   170,
     169,   172,   171,   173,   173,   174,   174,   175,   175,   176,
     177,   177,   178,   178,   178,   180,   179,   181,   179,   182,
     182,   184,   183,   185,   183,   186,   183,   187,   183,   188,
     183,   189,   183,   190,   183,   191,   183,   192,   194,   193,
     193,   195,   196,   195,   197,   195,   198,   199,   199,   199,
     201,   202,   203,   204,   200,   205,   205,   205,   205,   205,
     205,   205,   205,   205,   206,   207,   208,   208,   209,   209,
     210,   210,   211,   211,   211,   212,   212,   212,   213,   213,
     213,   213,   214,   214,   214,   214,   215,   215,   215,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   217,   219,
     218,   218,   218,   218,   218,   218,   221,   220,   222,   222,
     223,   223,   223,   224,   224,   224,   224,   224,   224,   224,
     224,   225,   225,   225,   225,   226,   226,   226,   227,   227,
     228,   230,   229,   229,   231,   231,   232,   234,   233,   233,
     235,   236,   237,   237,   238,   238,   238,   239,   239,   239,
     240,   240,   240,   241,   242,   242,   243,   244,   245,   246,
     248,   247,   250,   251,   252,   249,   253,   254,   254,   256,
     255,   258,   259,   257,   260,   257,   262,   261,   264,   265,
     266,   267,   268,   269,   263,   270,   270,   270,   270,   270,
     270,   271,   272,   272,   274,   275,   273,   276,   273,   277,
     278,   278,   280,   279,   281,   282,   279,   284,   283,   285,
     286,   288,   289,   290,   291,   287,   292,   294,   293,   293,
     295,   297,   298,   296,   296,   300,   301,   299,   302,   304,
     305,   303,   303,   306,   308,   309,   307,   307,   310,   312,
     311,   313,   314,   314,   316,   317,   315,   318,   318,   320,
     321,   322,   323,   319,   325,   326,   327,   328,   324,   330,
     331,   332,   333,   329,   335,   336,   337,   338,   334,   339,
     339,   339,   340,   340,   342,   343,   344,   341,   346,   345,
     347,   345,   348,   350,   349,   349,   352,   353,   351,   355,
     354,   356,   354,   357,   359,   358,   358,   361,   362,   360,
     363,   363,   363,   363,   364,   364,   364,   366,   367,   365,
     365,   369,   370,   368,   368,   372,   373,   371,   371,   375,
     376,   374,   374,   377,   379,   378,   378,   380,   381,   382,
     382,   383,   385,   386,   387,   384,   389,   390,   388,   392,
     391,   391,   393,   393,   395,   394,   396,   394,   397,   394,
     398,   394,   399,   394,   400,   394,   401,   394,   402,   403,
     403,   404,   405,   406,   406,   407,   408,   409,   411,   410,
     413,   414,   415,   416,   417,   418,   412,   420,   419,   419,
     421,   421,   423,   424,   422,   425,   425,   426,   427,   426,
     428,   426,   430,   431,   429,   433,   434,   432,   435,   435,
     435,   436,   436,   437,   438,   440,   441,   442,   439,   443,
     444,   445,   447,   446,   449,   450,   451,   448,   452,   452,
     453,   453,   453,   453,   453,   453,   453,   453,   453,   453,
     454,   456,   455,   455,   457,   457,   458,   459,   459,   460,
     462,   463,   464,   465,   461,   466,   467,   467,   469,   468,
     470,   468,   471,   468,   472,   472,   473,   474,   474,   475,
     476,   478,   479,   477,   481,   482,   480,   483,   483,   485,
     484,   486,   484,   487,   484,   488,   484
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     2,     2,     2,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       0,     0,     7,     2,     0,     0,     0,     0,     0,    11,
       2,     2,     0,     1,     2,     0,     0,     8,     0,     0,
       6,     1,     1,     0,     0,     0,     7,     0,     3,     2,
       3,     3,     0,     4,     0,     1,     1,     1,     1,     2,
       1,     0,     0,     0,     7,     0,     0,     0,     8,     0,
       4,     0,     3,     1,     0,     2,     0,     2,     1,     2,
       2,     0,     1,     1,     2,     0,     3,     0,     3,     2,
       0,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     2,     0,     4,
       0,     1,     0,     3,     0,     4,     1,     1,     2,     2,
       0,     0,     0,     0,     9,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     3,
       1,     3,     1,     3,     3,     1,     3,     3,     1,     3,
       3,     3,     1,     2,     2,     2,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       3,     1,     1,     1,     2,     1,     0,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     0,     4,     0,     1,     1,     2,     0,     4,     0,
       1,     1,     1,     1,     1,     2,     1,     2,     3,     2,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       0,     3,     0,     0,     0,     7,     2,     2,     0,     0,
       2,     0,     0,     5,     0,     3,     0,     3,     0,     0,
       0,     0,     0,     0,    14,     1,     1,     1,     1,     1,
       1,     2,     2,     0,     0,     0,     5,     0,     3,     2,
       2,     0,     0,     3,     0,     0,     5,     0,     3,     1,
       1,     0,     0,     0,     0,     9,     2,     0,     4,     0,
       1,     0,     0,     6,     2,     0,     0,     5,     6,     0,
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
       0,     3,     0,     3,     3,     3,     2,     3,     0,     1,
       2,     0,     0,     5,     0,     0,     5,     2,     0,     0,
       3,     0,     3,     0,     3,     0,     3
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
#line 368 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 2263 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 371 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 2270 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 374 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 2277 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 384 "fe/idl.ypp" /* yacc.c:1646  */
    {
// fixed_definition : type_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 2286 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 389 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2295 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 394 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | typeid_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 2304 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 399 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2313 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 404 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | typeprefix_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 2322 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 409 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2331 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 414 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | const_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 2340 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 419 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2349 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 424 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | exception
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 2358 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 429 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
        idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2367 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 434 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | interface_def
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
        }
#line 2376 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 439 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2385 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 444 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | module
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
        }
#line 2394 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 449 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2403 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 454 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | value_def
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeDeclSeen);
        }
#line 2412 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 459 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2421 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 464 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | component
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentDeclSeen);
        }
#line 2430 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 469 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2439 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 474 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | home_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeDeclSeen);
        }
#line 2448 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 479 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2457 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 484 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | event
          idl_global->set_parse_state (IDL_GlobalData::PS_EventDeclSeen);
        }
#line 2466 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 489 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2475 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 494 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | porttype_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeDeclSeen);
        }
#line 2484 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 499 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2493 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 504 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | connector_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorDeclSeen);
        }
#line 2502 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 509 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2511 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 514 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 2520 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 519 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 2530 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 528 "fe/idl.ypp" /* yacc.c:1646  */
    {
// module_header  : IDL_MODULE
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
        }
#line 2539 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 533 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 2547 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 540 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 2590 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 579 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
        idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
        }
#line 2599 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 584 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_definition
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
        }
#line 2608 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 589 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
          /*
           * Finished with this module - pop it from the scope stack.
           */

          idl_global->scopes ().pop ();
        }
#line 2622 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 602 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleIDSeen);
        }
#line 2630 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 609 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 2649 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 624 "fe/idl.ypp" /* yacc.c:1646  */
    {
          if (FE_Utils::duplicate_param_id ((yyvsp[0].plval)))
            {
              idl_global->err ()->duplicate_param_id (
                (yyvsp[-2].idlist));

              return 1;
            }
        }
#line 2663 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 634 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 2698 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 665 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleSqSeen);
        }
#line 2706 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 669 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleBodySeen);
        }
#line 2714 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 673 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 2736 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 708 "fe/idl.ypp" /* yacc.c:1646  */
    {
// template_module_ref : IDL_ALIAS scoped_name
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefSeen);
        }
#line 2746 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 714 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        '<' at_least_one_formal_parameter_name '>'
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefParamsSeen);
        }
#line 2756 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 720 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        id
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
#line 2839 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 802 "fe/idl.ypp" /* yacc.c:1646  */
    {
// template_module_inst : template_module_header
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleSeen);
        }
#line 2849 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 808 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        at_least_one_actual_parameter '>'
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleArgsSeen);
        }
#line 2859 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 814 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        id
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
#line 2926 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 885 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 2969 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 924 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
#line 2978 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 929 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
#line 2987 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 934 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
#line 3001 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 947 "fe/idl.ypp" /* yacc.c:1646  */
    {
// interface_decl : IDL_INTERFACE
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
#line 3010 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 952 "fe/idl.ypp" /* yacc.c:1646  */
    {
//       id
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (yyval.idval) = (yyvsp[0].idval);
         }
#line 3020 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 961 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3061 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 999 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3095 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 1030 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3129 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 1063 "fe/idl.ypp" /* yacc.c:1646  */
    {
// inheritance_spec : ':' opt_truncatable
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 3138 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 1068 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_scoped_name
          (yyvsp[0].nlval)->truncatable ((yyvsp[-2].bval));
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3148 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 1074 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.nlval) = 0;
        }
#line 3157 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 1089 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3172 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 1104 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3220 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 1148 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3229 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 1153 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3238 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 1158 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3262 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 1182 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3310 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 1226 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3319 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 1231 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3328 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 1236 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 3342 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 1250 "fe/idl.ypp" /* yacc.c:1646  */
    {
// value_header : value_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 3351 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 1255 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3387 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 1290 "fe/idl.ypp" /* yacc.c:1646  */
    {
// value_decl : IDL_VALUETYPE
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
#line 3396 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 1295 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 3406 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 1304 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_truncatable : IDL_TRUNCATABLE
          (yyval.bval) = true;
        }
#line 3415 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 1309 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.bval) = false;
        }
#line 3424 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 1318 "fe/idl.ypp" /* yacc.c:1646  */
    {
// supports_spec : IDL_SUPPORTS at_least_one_scoped_name
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3433 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 1323 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |    EMPTY */
          (yyval.nlval) = 0;
        }
#line 3442 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 1332 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3470 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 1357 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3498 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 1384 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3564 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 1461 "fe/idl.ypp" /* yacc.c:1646  */
    {
// state_member : IDL_PUBLIC
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_PUBLIC;
        }
#line 3574 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 1467 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        member_i
        }
#line 3582 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 1471 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_PRIVATE
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_PRIVATE;
        }
#line 3592 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 1477 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        member_i
        }
#line 3600 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 1489 "fe/idl.ypp" /* yacc.c:1646  */
    {
// export : type_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 3609 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 1494 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3618 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 1499 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | typeid_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 3627 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 1504 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3636 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 1509 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | typeprefix_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 3645 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 1514 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3654 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 1519 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | const_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 3663 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 1524 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
#line 3672 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 1529 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | exception
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 3681 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 1534 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3690 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 1539 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 3699 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 1544 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3708 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 1549 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | operation
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
#line 3717 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 1554 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3726 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 1559 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 3735 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 1564 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 3745 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 1573 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_scoped_name : scoped_name scoped_names
          ACE_NEW_RETURN ((yyval.nlval),
                          UTL_NameList ((yyvsp[-1].idlist),
                                        (yyvsp[0].nlval)),
                          1);
        }
#line 3757 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 1585 "fe/idl.ypp" /* yacc.c:1646  */
    {
// scoped_names : scoped_names ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
#line 3766 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 1590 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3791 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 1611 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.nlval) = 0;
        }
#line 3800 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 1619 "fe/idl.ypp" /* yacc.c:1646  */
    {
// scoped_name : id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN ((yyval.idlist),
                          UTL_IdList ((yyvsp[0].idval),
                                      0),
                          1);
        }
#line 3814 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 1629 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_SCOPE_DELIMITOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
#line 3823 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 1634 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3848 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 1656 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | scoped_name IDL_SCOPE_DELIMITOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);

          // This cleans up all the non-global "::"s in scoped names.
          // If there is a global one, it gets put into the UTL_IdList,
          // so we clean it up in the case above.
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3863 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 1667 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3880 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 1682 "fe/idl.ypp" /* yacc.c:1646  */
    {
// id: IDENTIFIER
          ACE_NEW_RETURN ((yyval.idval),
                          Identifier ((yyvsp[0].strval)),
                          1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3893 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 1694 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3942 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 1740 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3973 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 1768 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4004 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 1798 "fe/idl.ypp" /* yacc.c:1646  */
    {
// const_dcl : IDL_CONST
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
#line 4013 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 1803 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      const_type
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
#line 4022 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 1808 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
#line 4031 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 1813 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '='
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
#line 4040 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 1818 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4108 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 1891 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4124 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 1903 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | wstring_type_spec
          (yyval.etval) = AST_Expression::EV_wstring;
        }
#line 4133 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 1908 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4204 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 1982 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4219 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 1997 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4234 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 2012 "fe/idl.ypp" /* yacc.c:1646  */
    {
// and_expr : shift_expr | and_expr '&' shift_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_and,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4248 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 2026 "fe/idl.ypp" /* yacc.c:1646  */
    {
// shift_expr : add_expr | shift_expr IDL_LEFT_SHIFT add_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_left,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4262 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 2036 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | shift_expr IDL_RIGHT_SHIFT add_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_right,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4276 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 2050 "fe/idl.ypp" /* yacc.c:1646  */
    {
// add_expr : mult_expr | add_expr '+' mult_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_add,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4290 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 2060 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | add_expr '-' mult_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_minus,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4304 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 2074 "fe/idl.ypp" /* yacc.c:1646  */
    {
// mult_expr : unary_expr | mult_expr '*' unary_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mul,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4318 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 2084 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | mult_expr '/' unary_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_div,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4332 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 2094 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | mult_expr '%' unary_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mod,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4346 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 2108 "fe/idl.ypp" /* yacc.c:1646  */
    {
// unary_expr : primary_expr | '+' primary_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_u_plus,
                                    (yyvsp[0].exval),
                                    0
                                  );
        }
#line 4360 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 2118 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | '-' primary_expr
          (yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_u_minus,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4374 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 2128 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | '~' primary_expr
          (yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_bit_neg,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4388 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 2141 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4443 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 2193 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | literal
//      | '(' const_expr ')'
          (yyval.exval) = (yyvsp[-1].exval);
        }
#line 4453 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 2202 "fe/idl.ypp" /* yacc.c:1646  */
    {
// literal : IDL_INTEGER_LITERAL
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].ival));
        }
#line 4462 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 2207 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_UINTEGER_LITERAL
          (yyval.exval) =
            idl_global->gen ()->create_expr ((yyvsp[0].uival));
        }
#line 4472 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 2213 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_STRING_LITERAL
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].sval));
          (yyvsp[0].sval)->destroy ();
          delete (yyvsp[0].sval);
          (yyvsp[0].sval) = 0;
        }
#line 4484 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 2221 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_WSTRING_LITERAL
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].wsval));
        }
#line 4493 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 2226 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_CHARACTER_LITERAL
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].cval));
        }
#line 4502 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 2231 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_WCHAR_LITERAL
          ACE_OutputCDR::from_wchar wc ((yyvsp[0].wcval));
          (yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
#line 4512 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 2237 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_FLOATING_PT_LITERAL
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].dval));
        }
#line 4521 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 2242 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_TRUETOK
          (yyval.exval) = idl_global->gen ()->create_expr (true);
        }
#line 4530 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 2247 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_FALSETOK
          (yyval.exval) = idl_global->gen ()->create_expr (false);
        }
#line 4539 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 2255 "fe/idl.ypp" /* yacc.c:1646  */
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
              (yyval.exval) = (yyvsp[0].exval);
            }
          else
            {
              idl_global->err ()->syntax_error (idl_global->parse_state ());
            }
        }
#line 4611 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 2326 "fe/idl.ypp" /* yacc.c:1646  */
    {
// type_dcl : IDL_TYPEDEF
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 4621 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 2332 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      type_declarator
          (yyval.dcval) = 0;
        }
#line 4630 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 2337 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | struct_type
          (yyval.dcval) = 0;
        }
#line 4639 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 2342 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | union_type
          (yyval.dcval) = 0;
        }
#line 4648 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 2347 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | enum_type
          (yyval.dcval) = 0;
        }
#line 4657 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 2352 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4688 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 195:
#line 2379 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | constructed_forward_type_spec
          (yyval.dcval) = 0;
        }
#line 4697 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 196:
#line 2387 "fe/idl.ypp" /* yacc.c:1646  */
    {
// type_declarator : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
#line 4706 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 197:
#line 2392 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4774 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 200:
#line 2464 "fe/idl.ypp" /* yacc.c:1646  */
    {
// simple_type_spec : base_type_spec
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 4786 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 2473 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4815 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 220:
#line 2530 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_declarator : declarator declarators
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 4827 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 221:
#line 2542 "fe/idl.ypp" /* yacc.c:1646  */
    {
// declarators : declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 4836 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 222:
#line 2547 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4861 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 223:
#line 2568 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.dlval) = 0;
        }
#line 4870 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 226:
#line 2581 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_simple_declarator : simple_declarator simple_declarators
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 4882 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 227:
#line 2593 "fe/idl.ypp" /* yacc.c:1646  */
    {
// simple_declarators : simple_declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 4891 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 228:
#line 2598 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4916 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 229:
#line 2619 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.dlval) = 0;
        }
#line 4925 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 230:
#line 2627 "fe/idl.ypp" /* yacc.c:1646  */
    {
// simple_declarator : id
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
#line 4943 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 231:
#line 2644 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4963 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 234:
#line 2668 "fe/idl.ypp" /* yacc.c:1646  */
    {
// signed_int : IDL_LONG
          (yyval.etval) = AST_Expression::EV_long;
        }
#line 4972 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 235:
#line 2673 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_LONG IDL_LONG
          (yyval.etval) = AST_Expression::EV_longlong;
        }
#line 4981 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 236:
#line 2678 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_SHORT
          (yyval.etval) = AST_Expression::EV_short;
        }
#line 4990 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 237:
#line 2686 "fe/idl.ypp" /* yacc.c:1646  */
    {
// unsigned_int : IDL_UNSIGNED IDL_LONG
          (yyval.etval) = AST_Expression::EV_ulong;
        }
#line 4999 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 238:
#line 2691 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_UNSIGNED IDL_LONG IDL_LONG
          (yyval.etval) = AST_Expression::EV_ulonglong;
        }
#line 5008 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 239:
#line 2696 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_UNSIGNED IDL_SHORT
          (yyval.etval) = AST_Expression::EV_ushort;
        }
#line 5017 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 240:
#line 2704 "fe/idl.ypp" /* yacc.c:1646  */
    {
// floating_pt_type : IDL_DOUBLE
          (yyval.etval) = AST_Expression::EV_double;
        }
#line 5026 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 241:
#line 2709 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_FLOAT
          (yyval.etval) = AST_Expression::EV_float;
        }
#line 5035 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 242:
#line 2714 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_LONG IDL_DOUBLE
          (yyval.etval) = AST_Expression::EV_longdouble;
        }
#line 5044 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 243:
#line 2722 "fe/idl.ypp" /* yacc.c:1646  */
    {
// fixed_type : IDL_FIXED
          (yyval.etval) = AST_Expression::EV_fixed;
        }
#line 5053 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 244:
#line 2730 "fe/idl.ypp" /* yacc.c:1646  */
    {
// char_type : IDL_CHAR
          (yyval.etval) = AST_Expression::EV_char;
        }
#line 5062 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 245:
#line 2735 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_WCHAR
          (yyval.etval) = AST_Expression::EV_wchar;
        }
#line 5071 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 246:
#line 2743 "fe/idl.ypp" /* yacc.c:1646  */
    {
// octet_type : IDL_OCTET
          (yyval.etval) = AST_Expression::EV_octet;
        }
#line 5080 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 247:
#line 2751 "fe/idl.ypp" /* yacc.c:1646  */
    {
// boolean_type : IDL_BOOLEAN
          (yyval.etval) = AST_Expression::EV_bool;
        }
#line 5089 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 248:
#line 2759 "fe/idl.ypp" /* yacc.c:1646  */
    {
// any_type : IDL_ANY
          (yyval.etval) = AST_Expression::EV_any;
        }
#line 5098 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 249:
#line 2767 "fe/idl.ypp" /* yacc.c:1646  */
    {
// object_type : IDL_OBJECT
          (yyval.etval) = AST_Expression::EV_object;
        }
#line 5107 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 250:
#line 2775 "fe/idl.ypp" /* yacc.c:1646  */
    {
// struct_decl : IDL_STRUCT
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
#line 5116 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 251:
#line 2780 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5126 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 252:
#line 2790 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5164 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 253:
#line 2824 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
#line 5173 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 254:
#line 2829 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_member
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
#line 5182 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 255:
#line 2834 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5199 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 259:
#line 2856 "fe/idl.ypp" /* yacc.c:1646  */
    {
// member  :
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_NA;
        }
#line 5209 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 261:
#line 2866 "fe/idl.ypp" /* yacc.c:1646  */
    {
// member_i : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
#line 5218 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 262:
#line 2871 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
#line 5227 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 263:
#line 2876 "fe/idl.ypp" /* yacc.c:1646  */
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
                                            (yyvsp[-5].vival)
                                          );
                  (void) s->fe_add_field (f);
                }
            }

          (yyvsp[-2].dlval)->destroy ();
          delete (yyvsp[-2].dlval);
          (yyvsp[-2].dlval) = 0;
        }
#line 5289 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 264:
#line 2934 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state ());
        }
#line 5298 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 265:
#line 2939 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 5308 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 266:
#line 2948 "fe/idl.ypp" /* yacc.c:1646  */
    {
// union_decl : IDL_UNION
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
#line 5317 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 267:
#line 2953 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5327 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 268:
#line 2962 "fe/idl.ypp" /* yacc.c:1646  */
    {
// union_type : union_decl IDL_SWITCH
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
#line 5336 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 269:
#line 2967 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5376 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 270:
#line 3003 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      switch_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
#line 5385 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 271:
#line 3008 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ')'
          /*
           * The top of the scopes must an empty union we added after we
           * encountered 'union <id> switch ('. Now we are ready to add a
           * correct one. Temporarily remove the top so that we setup the
           * correct union in a right scope. But first save pragma prefix
           * since UTL_ScopeStack::pop() removes it.
           */
          char *prefix = 0;
          idl_global->pragma_prefixes ().top (prefix);
          prefix = ACE::strnew (prefix);
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
          idl_global->pragma_prefixes ().push (prefix);

          (yyvsp[-7].idval)->destroy ();
          delete (yyvsp[-7].idval);
          (yyvsp[-7].idval) = 0;
        }
#line 5451 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 272:
#line 3070 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
#line 5460 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 273:
#line 3075 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_case_branch
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
#line 5469 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 274:
#line 3080 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5490 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 275:
#line 3100 "fe/idl.ypp" /* yacc.c:1646  */
    {
// switch_type_spec : integer_type
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5502 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 276:
#line 3108 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5520 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 277:
#line 3122 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | octet_type
          /* octets are not allowed. */
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5534 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 278:
#line 3132 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | boolean_type
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5546 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 280:
#line 3141 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5656 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 284:
#line 3257 "fe/idl.ypp" /* yacc.c:1646  */
    {
// case_branch : at_least_one_case_label
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
#line 5665 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 285:
#line 3262 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      element_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
#line 5674 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 286:
#line 3267 "fe/idl.ypp" /* yacc.c:1646  */
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
              &&  (yyvsp[-4].llval) != 0
              && (yyvsp[-2].ffval) != 0)
            {
              b =
                idl_global->gen ()->create_union_branch (
                                        (yyvsp[-4].llval),
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
#line 5709 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 287:
#line 3298 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 5718 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 288:
#line 3303 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 5728 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 289:
#line 3312 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_case_label : case_label case_labels
          ACE_NEW_RETURN ((yyval.llval),
                          UTL_LabelList ((yyvsp[-1].ulval),
                                         (yyvsp[0].llval)),
                          1);
        }
#line 5740 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 290:
#line 3323 "fe/idl.ypp" /* yacc.c:1646  */
    {
// case_labels : case_labels case_label
          UTL_LabelList *ll = 0;
          ACE_NEW_RETURN (ll,
                          UTL_LabelList ((yyvsp[0].ulval),
                                         0),
                          1);

          if ((yyvsp[-1].llval) == 0)
            {
              (yyval.llval) = ll;
            }
          else
            {
              (yyvsp[-1].llval)->nconc (ll);
              (yyval.llval) = (yyvsp[-1].llval);
            }
        }
#line 5763 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 291:
#line 3342 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.llval) = 0;
        }
#line 5772 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 292:
#line 3350 "fe/idl.ypp" /* yacc.c:1646  */
    {
// case_label : IDL_DEFAULT
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
#line 5781 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 293:
#line 3355 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
#line 5795 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 294:
#line 3365 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_CASE
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
#line 5804 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 295:
#line 3370 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
#line 5812 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 296:
#line 3374 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      const_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      (yyvsp[-2].exval)
                                    );
        }
#line 5826 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 297:
#line 3387 "fe/idl.ypp" /* yacc.c:1646  */
    {
// element_spec : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
#line 5835 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 298:
#line 3392 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5884 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 299:
#line 3440 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5909 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 300:
#line 3464 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5934 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 301:
#line 3488 "fe/idl.ypp" /* yacc.c:1646  */
    {
// enum_type : IDL_ENUM
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
#line 5943 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 302:
#line 3493 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      id
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
#line 5980 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 303:
#line 3526 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
#line 5989 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 304:
#line 3531 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_enumerator
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
#line 5998 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 305:
#line 3536 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6022 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 307:
#line 3562 "fe/idl.ypp" /* yacc.c:1646  */
    {
// enumerators : enumerators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
#line 6031 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 310:
#line 3572 "fe/idl.ypp" /* yacc.c:1646  */
    {
// enumerator : IDENTIFIER
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
#line 6067 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 311:
#line 3608 "fe/idl.ypp" /* yacc.c:1646  */
    {
// sequence_type_spec : seq_head ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
#line 6076 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 312:
#line 3613 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
#line 6085 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 313:
#line 3618 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6166 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 314:
#line 3696 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6226 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 315:
#line 3755 "fe/idl.ypp" /* yacc.c:1646  */
    {
// seq_head : IDL_SEQUENCE
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
#line 6240 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 316:
#line 3765 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
#line 6249 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 317:
#line 3770 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      simple_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 6259 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 318:
#line 3779 "fe/idl.ypp" /* yacc.c:1646  */
    {
          //TODO
        }
#line 6267 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 319:
#line 3787 "fe/idl.ypp" /* yacc.c:1646  */
    {
// string_type_spec : string_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 6276 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 320:
#line 3792 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 6285 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 321:
#line 3797 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6332 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 322:
#line 3840 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6359 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 323:
#line 3866 "fe/idl.ypp" /* yacc.c:1646  */
    {
// string_head : IDL_STRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 6368 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 324:
#line 3875 "fe/idl.ypp" /* yacc.c:1646  */
    {
// wstring_type_spec : wstring_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 6377 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 325:
#line 3880 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 6386 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 326:
#line 3885 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6422 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 327:
#line 3917 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6448 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 328:
#line 3942 "fe/idl.ypp" /* yacc.c:1646  */
    {
// wstring_head : IDL_WSTRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 6457 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 329:
#line 3950 "fe/idl.ypp" /* yacc.c:1646  */
    {
// array_declarator : id
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
#line 6466 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 330:
#line 3955 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6504 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 331:
#line 3992 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_array_dim : array_dim array_dims
          ACE_NEW_RETURN ((yyval.elval),
                          UTL_ExprList ((yyvsp[-1].exval),
                                        (yyvsp[0].elval)),
                          1);
        }
#line 6516 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 332:
#line 4003 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6539 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 333:
#line 4022 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.elval) = 0;
        }
#line 6548 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 334:
#line 4030 "fe/idl.ypp" /* yacc.c:1646  */
    {
// array_dim : '['
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
#line 6557 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 335:
#line 4035 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
#line 6566 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 336:
#line 4040 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6625 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 339:
#line 4103 "fe/idl.ypp" /* yacc.c:1646  */
    {
// attribute_readonly : IDL_READONLY
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
#line 6634 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 340:
#line 4108 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 6643 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 341:
#line 4113 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 6652 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 342:
#line 4118 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 6661 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 343:
#line 4123 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6726 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 344:
#line 4187 "fe/idl.ypp" /* yacc.c:1646  */
    {
// attribute_readwrite : IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 6735 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 345:
#line 4192 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 6744 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 346:
#line 4197 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 6753 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 347:
#line 4202 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      opt_getraises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
#line 6762 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 348:
#line 4207 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6836 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 349:
#line 4280 "fe/idl.ypp" /* yacc.c:1646  */
    {
// exception : IDL_EXCEPTION
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
#line 6845 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 350:
#line 4285 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      id
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
#line 6879 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 351:
#line 4315 "fe/idl.ypp" /* yacc.c:1646  */
    {
//       '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
#line 6888 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 352:
#line 4320 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      members
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
#line 6897 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 353:
#line 4325 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();
        }
#line 6910 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 354:
#line 4338 "fe/idl.ypp" /* yacc.c:1646  */
    {
// operation : opt_op_attribute op_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 6919 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 355:
#line 4343 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      IDENTIFIER
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
          if (s != 0 && (yyvsp[-2].dcval) != 0)
            {
              AST_Type *tp =
                AST_Type::narrow_from_decl ((yyvsp[-2].dcval));

              if (tp == 0)
                {
                  idl_global->err ()->not_a_type ((yyvsp[-2].dcval));
                }
              else if (tp->node_type () == AST_Decl::NT_except)
                {
                  idl_global->err ()->not_a_type ((yyvsp[-2].dcval));
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
                      (yyvsp[-3].ofval),
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
#line 6988 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 356:
#line 4408 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 6997 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 357:
#line 4413 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      opt_raises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
#line 7006 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 358:
#line 4418 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7040 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 359:
#line 4451 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_op_attribute : IDL_ONEWAY
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_oneway;
        }
#line 7050 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 360:
#line 4457 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_IDEMPOTENT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_idempotent;
        }
#line 7060 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 361:
#line 4463 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.ofval) = AST_Operation::OP_noflags;
        }
#line 7069 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 363:
#line 4472 "fe/idl.ypp" /* yacc.c:1646  */
    {
// op_type_spec : param_type_spec | IDL_VOID
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
#line 7081 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 364:
#line 4483 "fe/idl.ypp" /* yacc.c:1646  */
    {
// init_decl : IDL_FACTORY
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7091 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 365:
#line 4489 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7137 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 366:
#line 4531 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7146 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 367:
#line 4536 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7164 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 368:
#line 4553 "fe/idl.ypp" /* yacc.c:1646  */
    {
// init_parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7173 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 369:
#line 4558 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7182 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 370:
#line 4563 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7191 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 371:
#line 4569 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_in_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7200 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 373:
#line 4580 "fe/idl.ypp" /* yacc.c:1646  */
    {
// in_parameters : in_parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7209 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 376:
#line 4590 "fe/idl.ypp" /* yacc.c:1646  */
    {
// in_parameter : IDL_IN
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7218 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 377:
#line 4595 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7227 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 378:
#line 4600 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7264 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 379:
#line 4636 "fe/idl.ypp" /* yacc.c:1646  */
    {
// parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7273 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 380:
#line 4641 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7282 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 381:
#line 4646 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7291 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 382:
#line 4652 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7300 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 384:
#line 4663 "fe/idl.ypp" /* yacc.c:1646  */
    {
// parameters : parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7309 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 387:
#line 4673 "fe/idl.ypp" /* yacc.c:1646  */
    {
// parameter : direction
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7318 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 388:
#line 4678 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7327 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 389:
#line 4683 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7371 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 390:
#line 4726 "fe/idl.ypp" /* yacc.c:1646  */
    {
// param_type_spec : base_type_spec
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 7383 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 393:
#line 4736 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7494 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 394:
#line 4846 "fe/idl.ypp" /* yacc.c:1646  */
    {
// direction : IDL_IN
          (yyval.dival) = AST_Argument::dir_IN;
        }
#line 7503 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 395:
#line 4851 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_OUT
          (yyval.dival) = AST_Argument::dir_OUT;
        }
#line 7512 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 396:
#line 4856 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_INOUT
          (yyval.dival) = AST_Argument::dir_INOUT;
        }
#line 7521 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 397:
#line 4864 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_raises : IDL_RAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
#line 7530 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 398:
#line 4869 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
#line 7539 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 399:
#line 4875 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7549 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 400:
#line 4881 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.nlval) = 0;
/*      |  EMPTY */
        }
#line 7558 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 401:
#line 4889 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_getraises : IDL_GETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
#line 7567 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 402:
#line 4894 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
#line 7576 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 403:
#line 4900 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7586 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 404:
#line 4906 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.nlval) = 0;
/*      |  EMPTY */
        }
#line 7595 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 405:
#line 4914 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_setraises : IDL_SETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
#line 7604 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 406:
#line 4919 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
#line 7613 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 407:
#line 4925 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7623 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 408:
#line 4931 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.nlval) = 0;
/*      |  EMPTY */
        }
#line 7632 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 409:
#line 4939 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_context : IDL_CONTEXT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
#line 7641 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 410:
#line 4944 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
//      '('
        }
#line 7650 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 411:
#line 4950 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_string_literal ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (yyval.slval) = (yyvsp[-1].slval);
        }
#line 7660 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 412:
#line 4956 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.slval) = 0;
        }
#line 7669 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 413:
#line 4964 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_string_literal : IDL_STRING_LITERAL string_literals
          ACE_NEW_RETURN ((yyval.slval),
                          UTL_StrList ((yyvsp[-1].sval),
                                       (yyvsp[0].slval)),
                          1);
        }
#line 7681 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 414:
#line 4976 "fe/idl.ypp" /* yacc.c:1646  */
    {
// string_literals : string_literals ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
#line 7690 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 415:
#line 4981 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7713 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 416:
#line 5000 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.slval) = 0;
        }
#line 7722 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 417:
#line 5008 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7752 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 418:
#line 5037 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7791 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 421:
#line 5081 "fe/idl.ypp" /* yacc.c:1646  */
    {
// component_forward_decl : IDL_COMPONENT id
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
#line 7820 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 422:
#line 5109 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7864 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 423:
#line 5149 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
#line 7873 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 424:
#line 5154 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      component_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
#line 7882 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 425:
#line 5159 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 7896 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 426:
#line 5173 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      component_header: IDL_COMPONENT id
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
#line 7905 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 427:
#line 5178 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      component_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 7914 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 428:
#line 5183 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7953 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 429:
#line 5221 "fe/idl.ypp" /* yacc.c:1646  */
    {
// component_inheritance_spec : ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 7962 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 430:
#line 5226 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      scoped_name
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 7971 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 431:
#line 5231 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.idlist) = 0;
        }
#line 7980 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 434:
#line 5244 "fe/idl.ypp" /* yacc.c:1646  */
    {
// component_export : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 7989 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 435:
#line 5249 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 7998 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 436:
#line 5254 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 8007 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 437:
#line 5259 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8016 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 438:
#line 5264 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | emits_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
        }
#line 8025 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 439:
#line 5269 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8034 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 440:
#line 5274 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | publishes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
        }
#line 8043 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 441:
#line 5279 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8052 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 442:
#line 5284 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | consumes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
        }
#line 8061 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 443:
#line 5289 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8070 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 444:
#line 5294 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 8079 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 445:
#line 5299 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8088 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 446:
#line 5304 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | extended_port_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 8097 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 447:
#line 5309 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8106 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 448:
#line 5316 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8198 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 449:
#line 5407 "fe/idl.ypp" /* yacc.c:1646  */
    {
// interface_type : scoped_name
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8209 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 450:
#line 5414 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8237 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 451:
#line 5440 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8345 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 452:
#line 5547 "fe/idl.ypp" /* yacc.c:1646  */
    {
// uses_opt_multiple : IDL_USES opt_multiple
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          (yyval.bval) = (yyvsp[0].bval);
        }
#line 8356 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 453:
#line 5557 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_multiple : IDL_MULTIPLE
          (yyval.bval) = true;
        }
#line 8365 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 454:
#line 5562 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.bval) = false;
        }
#line 8374 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 455:
#line 5569 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8449 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 456:
#line 5642 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8524 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 457:
#line 5715 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8599 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 458:
#line 5789 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8641 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 459:
#line 5827 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      home_body
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 8653 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 460:
#line 5838 "fe/idl.ypp" /* yacc.c:1646  */
    {
// home_header : IDL_HOME
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
#line 8662 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 461:
#line 5843 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
#line 8671 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 462:
#line 5848 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      home_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 8680 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 463:
#line 5853 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
#line 8689 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 464:
#line 5858 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      IDL_MANAGES
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
#line 8698 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 465:
#line 5863 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      scoped_name
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
#line 8707 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 466:
#line 5868 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8758 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 467:
#line 5918 "fe/idl.ypp" /* yacc.c:1646  */
    {
// home_inheritance_spec ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 8767 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 468:
#line 5923 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      scoped_name
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8776 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 469:
#line 5928 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.idlist) = 0;
        }
#line 8785 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 470:
#line 5937 "fe/idl.ypp" /* yacc.c:1646  */
    {
// primary_key_spec : IDL_PRIMARYKEY scoped_name
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8794 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 471:
#line 5942 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.idlist) = 0;
        }
#line 8803 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 472:
#line 5950 "fe/idl.ypp" /* yacc.c:1646  */
    {
// home_body : '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
#line 8812 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 473:
#line 5955 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      home_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
#line 8821 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 474:
#line 5960 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
#line 8830 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 478:
#line 5974 "fe/idl.ypp" /* yacc.c:1646  */
    {
// home_export : factory_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
#line 8839 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 479:
#line 5979 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8848 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 480:
#line 5984 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | finder_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
#line 8857 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 481:
#line 5989 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8866 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 482:
#line 5998 "fe/idl.ypp" /* yacc.c:1646  */
    {
// factory_decl : IDL_FACTORY id
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
#line 8894 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 483:
#line 6022 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 8903 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 484:
#line 6027 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8926 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 485:
#line 6050 "fe/idl.ypp" /* yacc.c:1646  */
    {
// finder_decl : IDL_FINDER id
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
#line 8958 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 486:
#line 6078 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 8967 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 487:
#line 6083 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8990 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 493:
#line 6117 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_concrete_forward_decl : IDL_EVENTTYPE id
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
#line 9018 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 494:
#line 6146 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_abs_forward_decl : IDL_ABSTRACT IDL_EVENTTYPE id
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
#line 9046 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 495:
#line 6174 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9095 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 496:
#line 6219 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9104 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 497:
#line 6224 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9113 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 498:
#line 6229 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9127 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 499:
#line 6244 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_abs_header : IDL_ABSTRACT IDL_EVENTTYPE id
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9136 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 500:
#line 6254 "fe/idl.ypp" /* yacc.c:1646  */
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
          (yyval.idval) = 0;
        }
#line 9154 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 501:
#line 6272 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_plain_header : IDL_EVENTTYPE id
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9165 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 502:
#line 6282 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_rest_of_header : inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 9174 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 503:
#line 6287 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9208 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 504:
#line 6321 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9262 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 505:
#line 6371 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9271 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 506:
#line 6376 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9280 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 507:
#line 6381 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9294 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 508:
#line 6394 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_header : event_custom_header
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9303 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 509:
#line 6399 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_header : event_plain_header
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9312 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 510:
#line 6407 "fe/idl.ypp" /* yacc.c:1646  */
    {
// formal_parameter_type : IDL_TYPENAME
          (yyval.ntval) = AST_Decl::NT_type;
        }
#line 9321 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 511:
#line 6412 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_STRUCT
          (yyval.ntval) = AST_Decl::NT_struct;
        }
#line 9330 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 512:
#line 6417 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_UNION
          (yyval.ntval) = AST_Decl::NT_union;
        }
#line 9339 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 513:
#line 6422 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_EVENTTYPE
          (yyval.ntval) = AST_Decl::NT_eventtype;
        }
#line 9348 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 514:
#line 6427 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_SEQUENCE
          (yyval.ntval) = AST_Decl::NT_sequence;
        }
#line 9357 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 515:
#line 6432 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_INTERFACE
          (yyval.ntval) = AST_Decl::NT_interface;
        }
#line 9366 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 516:
#line 6437 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_VALUETYPE
          (yyval.ntval) = AST_Decl::NT_valuetype;
        }
#line 9375 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 517:
#line 6442 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_ENUM
          (yyval.ntval) = AST_Decl::NT_enum;
        }
#line 9384 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 518:
#line 6447 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_EXCEPTION
          (yyval.ntval) = AST_Decl::NT_except;
        }
#line 9393 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 519:
#line 6452 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_CONST const_type
          (yyval.ntval) = AST_Decl::NT_const;
          t_param_const_type = (yyvsp[0].etval);
        }
#line 9403 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 520:
#line 6461 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9438 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 521:
#line 6495 "fe/idl.ypp" /* yacc.c:1646  */
    {
// formal_parameters : formal_parameters ','
          // Maybe add a new parse state to set here.
        }
#line 9447 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 522:
#line 6500 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9467 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 523:
#line 6516 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        /* EMPTY */
          (yyval.plval) = 0;
        }
#line 9476 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 524:
#line 6524 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9506 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 525:
#line 6550 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9525 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 526:
#line 6568 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_formal_parameter_name : formal_parameter_name formal_parameter_names
          ACE_NEW_RETURN ((yyval.slval),
                          UTL_StrList ((yyvsp[-1].sval),
                                       (yyvsp[0].slval)),
                          1);
        }
#line 9537 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 527:
#line 6579 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9560 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 528:
#line 6598 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        /* EMPTY */
          (yyval.slval) = 0;
        }
#line 9569 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 529:
#line 6606 "fe/idl.ypp" /* yacc.c:1646  */
    {
// formal_parameter_name : IDENTIFIER
          ACE_NEW_RETURN ((yyval.sval),
                          UTL_String ((yyvsp[0].strval), true),
                          1);
        }
#line 9580 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 530:
#line 6616 "fe/idl.ypp" /* yacc.c:1646  */
    {
// porttype_decl : IDL_PORTTYPE
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSeen);
        }
#line 9589 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 531:
#line 6621 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDENTIFIER
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
#line 9612 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 532:
#line 6640 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSqSeen);
        }
#line 9621 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 533:
#line 6645 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        at_least_one_port_export
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeBodySeen);
        }
#line 9630 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 534:
#line 6650 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeQsSeen);

          // Done with this port type - pop it off the scopes stack.
          idl_global->scopes ().pop ();
        }
#line 9642 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 535:
#line 6661 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_port_export : port_export port_exports
        }
#line 9650 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 536:
#line 6668 "fe/idl.ypp" /* yacc.c:1646  */
    {
// port_exports : port_exports port_export
        }
#line 9658 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 537:
#line 6672 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        | /* EMPTY */
        }
#line 9666 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 538:
#line 6679 "fe/idl.ypp" /* yacc.c:1646  */
    {
// port_export : provides_decl
        }
#line 9674 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 539:
#line 6683 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
        }
#line 9682 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 540:
#line 6687 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        | uses_decl
        }
#line 9690 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 541:
#line 6691 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
        }
#line 9698 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 542:
#line 6695 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 9707 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 543:
#line 6700 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
        }
#line 9715 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 544:
#line 6707 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9789 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 545:
#line 6777 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9841 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 546:
#line 6828 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_actual_parameter : actual_parameter actual_parameters
          if ((yyvsp[0].alval) == 0)
            {
              ACE_NEW_RETURN ((yyvsp[0].alval),
                              FE_Utils::T_ARGLIST,
                              1);
            }

          (yyvsp[0].alval)->enqueue_head ((yyvsp[-1].dcval));
          (yyval.alval) = (yyvsp[0].alval);
        }
#line 9858 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 547:
#line 6844 "fe/idl.ypp" /* yacc.c:1646  */
    {
// actual_parameters : actual_parameters ',' actual_parameter
          if ((yyvsp[-2].alval) == 0)
            {
              ACE_NEW_RETURN ((yyvsp[-2].alval),
                              FE_Utils::T_ARGLIST,
                              1);
            }

          (yyvsp[-2].alval)->enqueue_tail ((yyvsp[0].dcval));
          (yyval.alval) = (yyvsp[-2].alval);
        }
#line 9875 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 548:
#line 6857 "fe/idl.ypp" /* yacc.c:1646  */
    {
//         | /* EMPTY */
          (yyval.alval) = 0;
        }
#line 9884 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 549:
#line 6865 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9945 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 551:
#line 6929 "fe/idl.ypp" /* yacc.c:1646  */
    {
// connector_header : IDL_CONNECTOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSeen);
        }
#line 9954 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 552:
#line 6934 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorIDSeen);
        }
#line 9963 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 553:
#line 6939 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 10020 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 554:
#line 6995 "fe/idl.ypp" /* yacc.c:1646  */
    {
// connector_body " '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSqSeen);
        }
#line 10029 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 555:
#line 7000 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        connector_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorBodySeen);
        }
#line 10038 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 556:
#line 7005 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        '}
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorQsSeen);

          // Done with this connector - pop it off the scope stack.
          idl_global->scopes ().pop ();
        }
#line 10050 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 559:
#line 7021 "fe/idl.ypp" /* yacc.c:1646  */
    {
// connector_export : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 10059 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 560:
#line 7026 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10068 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 561:
#line 7031 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        | uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 10077 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 562:
#line 7036 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10086 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 563:
#line 7041 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 10095 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 564:
#line 7046 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10104 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 565:
#line 7051 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | extended_port_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 10113 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 566:
#line 7056 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10122 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;


#line 10126 "fe/idl.tab.cpp" /* yacc.c:1646  */
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
#line 7062 "fe/idl.ypp" /* yacc.c:1906  */

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
