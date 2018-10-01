/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

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
#include "ast_annotation.h"

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

#line 152 "fe/idl.tab.cpp" /* yacc.c:339  */

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
    IDL_WSTRING_LITERAL = 340,
    IDL_ANNOTATION_DECL = 341,
    IDL_ANNOTATION_SYMBOL = 342
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

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
  Annotations *annotations;

#line 319 "fe/idl.tab.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE tao_yylval;

int tao_yyparse (void);

#endif /* !YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 336 "fe/idl.tab.cpp" /* yacc.c:358  */

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
#define YYLAST   1464

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  110
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  398
/* YYNRULES -- Number of rules.  */
#define YYNRULES  593
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  868

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   343

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,   103,    98,     2,
     105,   106,   101,    99,    94,   100,     2,   102,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    93,    88,
      91,    95,    92,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   108,     2,   109,    97,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    89,    96,    90,   104,     2,     2,     2,
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
      85,    86,    87,   107
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   367,   367,   370,   371,   375,   378,   381,   387,   392,
     391,   400,   399,   410,   409,   420,   419,   430,   429,   440,
     439,   450,   449,   460,   459,   470,   469,   480,   479,   490,
     489,   500,   499,   510,   509,   520,   519,   530,   529,   544,
     543,   556,   595,   600,   555,   617,   625,   640,   650,   681,
     685,   624,   709,   713,   714,   718,   719,   724,   730,   723,
     818,   824,   817,   895,   896,   901,   940,   945,   900,   963,
     962,   976,  1014,  1045,  1079,  1078,  1090,  1097,  1098,  1099,
    1100,  1104,  1115,  1120,  1164,  1169,  1119,  1198,  1242,  1247,
    1196,  1266,  1264,  1306,  1305,  1319,  1325,  1332,  1339,  1346,
    1372,  1399,  1464,  1465,  1469,  1470,  1471,  1477,  1476,  1487,
    1486,  1499,  1500,  1505,  1504,  1515,  1514,  1525,  1524,  1535,
    1534,  1545,  1544,  1555,  1554,  1565,  1564,  1575,  1574,  1588,
    1601,  1599,  1627,  1641,  1652,  1651,  1679,  1677,  1704,  1715,
    1728,  1774,  1802,  1833,  1838,  1843,  1848,  1832,  1919,  1920,
    1921,  1922,  1923,  1924,  1925,  1937,  1942,  2011,  2013,  2015,
    2016,  2030,  2031,  2045,  2046,  2059,  2060,  2070,  2083,  2084,
    2094,  2107,  2108,  2118,  2128,  2141,  2142,  2152,  2162,  2175,
    2226,  2227,  2236,  2241,  2247,  2255,  2260,  2265,  2271,  2275,
    2280,  2285,  2293,  2362,  2389,  2390,  2394,  2395,  2396,  2400,
    2404,  2405,  2409,  2413,  2414,  2418,  2439,  2445,  2452,  2453,
    2457,  2458,  2462,  2466,  2467,  2471,  2476,  2475,  2486,  2491,
    2496,  2501,  2528,  2537,  2536,  2608,  2609,  2613,  2621,  2622,
    2650,  2651,  2652,  2653,  2654,  2655,  2656,  2657,  2661,  2662,
    2663,  2664,  2668,  2669,  2670,  2674,  2675,  2679,  2692,  2690,
    2718,  2725,  2726,  2730,  2743,  2741,  2769,  2776,  2793,  2812,
    2813,  2817,  2822,  2827,  2835,  2840,  2845,  2853,  2858,  2863,
    2871,  2879,  2884,  2892,  2900,  2908,  2916,  2925,  2924,  2940,
    2974,  2979,  2939,  2998,  3001,  3002,  3006,  3006,  3017,  3022,
    3015,  3086,  3085,  3100,  3099,  3114,  3119,  3155,  3160,  3217,
    3222,  3113,  3246,  3254,  3268,  3278,  3286,  3287,  3395,  3398,
    3399,  3404,  3409,  3403,  3445,  3444,  3458,  3469,  3489,  3497,
    3496,  3512,  3517,  3511,  3534,  3533,  3586,  3610,  3635,  3640,
    3673,  3678,  3634,  3704,  3709,  3707,  3714,  3718,  3755,  3760,
    3753,  3841,  3902,  3912,  3901,  3925,  3935,  3940,  3933,  3987,
    4013,  4023,  4028,  4021,  4064,  4089,  4098,  4097,  4139,  4150,
    4170,  4178,  4183,  4177,  4245,  4246,  4251,  4256,  4261,  4266,
    4250,  4335,  4340,  4345,  4350,  4334,  4428,  4433,  4463,  4468,
    4427,  4486,  4491,  4556,  4561,  4484,  4598,  4604,  4611,  4618,
    4619,  4631,  4637,  4679,  4630,  4701,  4700,  4711,  4710,  4723,
    4728,  4726,  4733,  4738,  4743,  4737,  4784,  4783,  4794,  4793,
    4806,  4811,  4809,  4816,  4821,  4826,  4820,  4873,  4881,  4882,
    4883,  4993,  4998,  5003,  5012,  5017,  5011,  5029,  5037,  5042,
    5036,  5054,  5062,  5067,  5061,  5079,  5087,  5092,  5086,  5104,
    5111,  5124,  5122,  5148,  5155,  5184,  5222,  5223,  5227,  5257,
    5297,  5302,  5256,  5321,  5326,  5319,  5369,  5368,  5379,  5386,
    5387,  5392,  5391,  5402,  5401,  5412,  5411,  5422,  5421,  5432,
    5431,  5442,  5441,  5452,  5451,  5463,  5554,  5561,  5587,  5694,
    5704,  5710,  5716,  5789,  5862,  5937,  5936,  5986,  5991,  5996,
    6001,  6006,  6011,  5985,  6066,  6065,  6076,  6083,  6090,  6098,
    6103,  6097,  6115,  6116,  6120,  6122,  6121,  6132,  6131,  6146,
    6170,  6144,  6198,  6226,  6196,  6252,  6253,  6254,  6258,  6259,
    6263,  6291,  6322,  6367,  6372,  6320,  6389,  6399,  6418,  6430,
    6429,  6469,  6519,  6524,  6467,  6541,  6546,  6554,  6559,  6564,
    6569,  6574,  6579,  6584,  6589,  6594,  6599,  6608,  6643,  6642,
    6664,  6671,  6697,  6715,  6726,  6746,  6753,  6764,  6769,  6788,
    6793,  6763,  6808,  6815,  6820,  6827,  6826,  6835,  6834,  6843,
    6842,  6854,  6924,  6975,  6991,  7005,  7012,  7072,  7077,  7082,
    7076,  7143,  7148,  7142,  7163,  7164,  7169,  7168,  7179,  7178,
    7189,  7188,  7199,  7198
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
  "IDL_RIGHT_SHIFT", "IDL_WCHAR_LITERAL", "IDL_WSTRING_LITERAL",
  "IDL_ANNOTATION_DECL", "IDL_ANNOTATION_SYMBOL", "';'", "'{'", "'}'",
  "'<'", "'>'", "':'", "','", "'='", "'|'", "'^'", "'&'", "'+'", "'-'",
  "'*'", "'/'", "'%'", "'~'", "'('", "')'", "\"default\"", "'['", "']'",
  "$accept", "start", "definitions", "definition",
  "at_least_one_definition", "fixed_definition", "$@1", "$@2", "$@3",
  "$@4", "$@5", "$@6", "$@7", "$@8", "$@9", "$@10", "$@11", "$@12", "$@13",
  "$@14", "$@15", "module_header", "$@16", "module", "$@17", "$@18",
  "$@19", "template_module_header", "template_module", "$@20", "$@21",
  "$@22", "$@23", "$@24", "at_least_one_tpl_definition", "tpl_definitions",
  "tpl_definition", "template_module_ref", "$@25", "$@26",
  "template_module_inst", "$@27", "$@28", "interface_def", "interface",
  "$@29", "$@30", "$@31", "interface_decl", "$@32", "interface_header",
  "inheritance_spec", "$@33", "value_def", "valuetype",
  "value_concrete_decl", "$@34", "$@35", "$@36", "value_abs_decl", "$@37",
  "$@38", "$@39", "value_header", "$@40", "value_decl", "$@41",
  "opt_truncatable", "supports_spec", "value_forward_decl",
  "value_box_decl", "value_elements", "value_element", "state_member",
  "@42", "@43", "exports", "export", "$@44", "$@45", "$@46", "$@47",
  "$@48", "$@49", "$@50", "$@51", "at_least_one_scoped_name",
  "scoped_names", "$@52", "scoped_name", "$@53", "$@54", "id",
  "defining_id", "interface_forward", "const_dcl", "$@55", "$@56", "$@57",
  "$@58", "const_type", "expression", "const_expr", "or_expr", "xor_expr",
  "and_expr", "shift_expr", "add_expr", "mult_expr", "unary_expr",
  "primary_expr", "literal", "positive_int_expr", "annotation_dcl",
  "annotation_body", "annotation_statement", "annotation_member_dcl",
  "annotation_member_type", "defaulted_annotation_member_dcl",
  "annotation_member", "annotations", "annotation_appl",
  "annotation_appl_params_maybe", "annotation_appl_params",
  "at_least_one_annotation_appl_param", "annotation_appl_named_params",
  "annotation_appl_param", "type_dcl", "$@59", "type_declarator", "$@60",
  "type_spec", "simple_type_spec", "base_type_spec", "template_type_spec",
  "constructed_type_spec", "constructed_forward_type_spec",
  "at_least_one_declarator", "declarators", "$@61", "declarator",
  "at_least_one_simple_declarator", "simple_declarators", "$@62",
  "simple_declarator", "complex_declarator", "integer_type", "signed_int",
  "unsigned_int", "floating_pt_type", "fixed_type", "char_type",
  "octet_type", "boolean_type", "any_type", "object_type", "struct_decl",
  "$@63", "struct_type", "$@64", "$@65", "$@66", "at_least_one_member",
  "members", "member", "@67", "member_i", "$@68", "$@69", "$@70",
  "union_decl", "$@71", "union_type", "$@72", "$@73", "$@74", "$@75",
  "$@76", "$@77", "switch_type_spec", "at_least_one_case_branch",
  "case_branches", "case_branch", "$@78", "$@79", "$@80",
  "at_least_one_case_label", "case_labels", "case_label", "$@81", "$@82",
  "$@83", "element_spec", "$@84", "struct_forward_type",
  "union_forward_type", "enum_type", "$@85", "$@86", "$@87", "$@88",
  "at_least_one_enumerator", "enumerators", "$@89", "enumerator",
  "sequence_type_spec", "$@90", "$@91", "seq_head", "$@92", "$@93",
  "fixed_type_spec", "string_type_spec", "$@94", "$@95", "string_head",
  "wstring_type_spec", "$@96", "$@97", "wstring_head", "array_declarator",
  "$@98", "at_least_one_array_dim", "array_dims", "array_dim", "$@99",
  "$@100", "attribute", "attribute_readonly", "$@101", "$@102", "$@103",
  "$@104", "attribute_readwrite", "$@105", "$@106", "$@107", "$@108",
  "exception", "$@109", "$@110", "$@111", "$@112", "operation", "$@113",
  "$@114", "$@115", "$@116", "opt_op_attribute", "op_type_spec",
  "init_decl", "$@117", "$@118", "$@119", "init_parameter_list", "$@120",
  "$@121", "at_least_one_in_parameter", "in_parameters", "$@122",
  "in_parameter", "$@123", "$@124", "parameter_list", "$@125", "$@126",
  "at_least_one_parameter", "parameters", "$@127", "parameter", "$@128",
  "$@129", "param_type_spec", "direction", "opt_raises", "$@130", "$@131",
  "opt_getraises", "$@132", "$@133", "opt_setraises", "$@134", "$@135",
  "opt_context", "$@136", "$@137", "at_least_one_string_literal",
  "string_literals", "$@138", "typeid_dcl", "typeprefix_dcl", "component",
  "component_forward_decl", "component_decl", "$@139", "$@140", "$@141",
  "component_header", "$@142", "$@143", "component_inheritance_spec",
  "$@144", "component_exports", "component_export", "$@145", "$@146",
  "$@147", "$@148", "$@149", "$@150", "$@151", "provides_decl",
  "interface_type", "uses_decl", "uses_opt_multiple", "opt_multiple",
  "emits_decl", "publishes_decl", "consumes_decl", "home_decl", "$@152",
  "home_header", "$@153", "$@154", "$@155", "$@156", "$@157", "$@158",
  "home_inheritance_spec", "$@159", "primary_key_spec", "home_body",
  "$@160", "$@161", "home_exports", "home_export", "$@162", "$@163",
  "factory_decl", "$@164", "$@165", "finder_decl", "$@166", "$@167",
  "event", "event_forward_decl", "event_concrete_forward_decl",
  "event_abs_forward_decl", "event_abs_decl", "$@168", "$@169", "$@170",
  "event_abs_header", "event_custom_header", "event_plain_header",
  "event_rest_of_header", "$@171", "event_decl", "$@172", "$@173", "$@174",
  "event_header", "formal_parameter_type", "at_least_one_formal_parameter",
  "formal_parameters", "$@175", "formal_parameter",
  "at_least_one_formal_parameter_name", "formal_parameter_names",
  "formal_parameter_name", "porttype_decl", "$@176", "$@177", "$@178",
  "$@179", "at_least_one_port_export", "port_exports", "port_export",
  "$@180", "$@181", "$@182", "extended_port_decl",
  "at_least_one_actual_parameter", "actual_parameters", "actual_parameter",
  "connector_decl", "connector_header", "$@183", "$@184", "connector_body",
  "$@185", "$@186", "connector_exports", "connector_export", "$@187",
  "$@188", "$@189", "$@190", YY_NULLPTR
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
     335,   336,   337,   338,   339,   340,   341,   342,    59,   123,
     125,    60,    62,    58,    44,    61,   124,    94,    38,    43,
      45,    42,    47,    37,   126,    40,    41,   343,    91,    93
};
# endif

#define YYPACT_NINF -677

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-677)))

#define YYTABLE_NINF -534

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -677,    56,  1208,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,    65,   145,   125,  -677,  -677,  -677,
    -677,    85,    85,  -677,  -677,    88,  -677,  -677,    20,  -677,
     289,    42,    64,  -677,  -677,    44,  -677,  -677,  -677,  -677,
    -677,  -677,   533,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
      82,  -677,   114,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,    50,  -677,  -677,  -677,    50,  -677,  -677,    95,   105,
    1082,    85,  -677,  1374,  -677,  -677,  -677,  -677,  -677,    41,
    -677,    99,  -677,   107,  -677,   121,  -677,  -677,    50,  -677,
     111,   113,  -677,  -677,  -677,    29,  -677,    48,  -677,  -677,
     116,  -677,   140,   164,  1397,  -677,  -677,  -677,   169,   155,
    -677,   174,   181,   182,    73,  -677,   227,  -677,  -677,  -677,
    -677,  -677,  -677,   196,  -677,  -677,  -677,  -677,  -677,  -677,
     192,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,   114,  -677,
    -677,  -677,    97,  -677,  -677,   198,  -677,   203,   215,   220,
     223,   226,  -677,   242,   244,   256,   258,   260,   259,   261,
     263,  -677,  -677,  -677,   264,   266,  -677,  -677,  -677,  -677,
     192,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
     192,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,    85,
    -677,  -677,   269,  -677,   271,  -677,  -677,   272,  -677,   361,
    -677,  -677,  -677,    46,    55,  -677,  -677,  -677,  1082,  -677,
    -677,  -677,  -677,   275,  -677,  -677,  -677,  -677,   364,  -677,
    -677,    40,   277,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,   362,  -677,   201,   280,   327,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,   270,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,   327,   287,   288,  -677,  -677,  -677,
    -677,  -677,   290,   291,    -3,  -677,  -677,  -677,   292,  -677,
     361,  -677,  -677,   669,  -677,  -677,   380,  -677,   297,   299,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
      80,    80,    80,   201,   192,  -677,  -677,   300,   298,   303,
     156,   143,   110,  -677,  -677,  -677,  -677,  -677,    85,  -677,
    -677,  -677,  -677,   304,  -677,    85,  -677,   201,   201,   201,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,    38,  -677,   294,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,   369,  -677,  -677,
      85,   327,  -677,  -677,  -677,   315,   272,  -677,  -677,  -677,
     317,   301,  -677,  -677,  -677,  -677,  -677,  1286,  -677,   320,
    -677,  -677,  -677,  -677,  -677,   307,   201,   201,   201,   201,
     201,   201,   201,   201,   201,   201,   313,  -677,  -677,   192,
     990,   822,   201,   207,  -677,  -677,  -677,  -677,  -677,  -677,
     716,   245,   268,   575,  -677,  -677,  -677,  -677,    54,   358,
      85,    85,  -677,  -677,  -677,  -677,  -677,    54,  -677,   328,
    -677,   322,   311,   326,  -677,  -677,   329,  -677,   316,  -677,
    -677,  1025,   192,  -677,    85,   327,  -677,  -677,  -677,   417,
    -677,   335,   336,   424,   340,  1397,  -677,   298,   303,   156,
     143,   143,   110,   110,  -677,  -677,  -677,  -677,  -677,   337,
    -677,  -677,  -677,   342,  -677,  -677,  -677,  -677,  -677,  -677,
     862,  -677,  -677,  -677,  -677,  -677,   343,  -677,  -677,  -677,
     346,   338,  -677,   344,   345,   347,   348,   350,  -677,  1350,
    -677,   427,   192,  -677,  -677,  -677,  -677,  -677,  -677,    85,
      85,    85,  -677,   351,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,   365,  -677,  -677,  -677,  1109,   906,
     420,   946,  -677,   192,   361,  -677,  -677,    90,    96,   368,
     370,   372,   361,   382,  -677,  -677,  -677,  -677,  -677,  -677,
      60,  -677,  -677,   367,   201,  -677,   377,   385,   192,  -677,
     137,  1082,  -677,  -677,  -677,  -677,    40,  -677,   384,  -677,
     388,   389,   391,   392,   393,  -677,   192,  -677,  -677,  -677,
    -677,  -677,   394,   395,  -677,   716,   716,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,   396,  -677,   379,   102,   102,
     102,   398,  -677,   399,   403,   404,   405,   406,   409,  -677,
    -677,  -677,   413,   414,   426,   428,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,   201,   201,   311,  -677,
    -677,   429,   410,   430,  -677,   449,  -677,   437,  -677,  -677,
    -677,  -677,  -677,  -677,  1261,  -677,    85,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,    61,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,   412,   412,  -677,  -677,  -677,  -677,   946,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,   329,  -677,  -677,
     433,   434,   435,   436,   137,    85,  -677,  -677,  -677,  -677,
     439,   192,    62,  -677,  -677,   440,   489,  -677,  -677,  -677,
    -677,  -677,   416,  -677,    85,  -677,  -677,  -677,  -677,  -677,
     192,   438,   441,  -677,  -677,   412,   442,  -677,   431,   497,
     495,   495,  -677,   480,   444,  -677,  -677,   192,   448,  -677,
    1154,   450,  -677,  -677,    66,  -677,  -677,   454,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,   502,   559,  -677,   457,
    -677,   495,  -677,  -677,  -677,  -677,  -677,  -677,  -677,   946,
    -677,   471,   461,   495,   463,   507,  -677,    85,  -677,  -677,
     478,  -677,   465,   214,   495,  -677,   484,   201,   481,   483,
      51,  -677,   255,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,   192,  -677,   486,  -677,  -677,  -677,  -677,   469,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  1350,  -677,  -677,
     497,    85,    85,   472,  -677,   559,  -677,   487,   946,   558,
     506,  -677,  -677,  -677,  -677,   498,   504,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,   515,  -677,  -677,    85,
     214,  -677,   511,  -677,  -677,   505,  -677,  -677,  -677,  -677,
     542,  -677,   512,   526,  -677,  -677,   547,  -677
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,     0,     1,    37,   143,    39,    69,   216,   277,
     293,   328,   376,   206,     0,     0,     0,    93,   206,   206,
     487,     0,     0,   557,   578,     0,     3,     5,    41,    23,
      60,     0,     0,    21,    63,    76,    65,    25,    77,    82,
      78,    83,    76,    79,    80,    64,    17,     9,    11,   222,
     279,   218,   327,   219,   245,   246,   220,    19,    13,    15,
      27,   447,   446,   449,    29,   485,    31,   517,   519,   518,
     516,    76,   535,   536,   515,    76,    33,    35,     0,     0,
       0,     0,   206,     0,   206,   206,   206,   206,   257,     0,
     221,    76,   206,    76,    87,    76,   206,    81,    76,   206,
     453,   528,   206,   138,   134,     0,   133,     0,   206,   206,
       0,    45,     0,     0,     0,   206,     6,     7,     0,    96,
      71,     0,     0,     0,   261,   263,     0,   267,   268,   271,
     272,   273,   274,   270,   275,   276,   342,   350,   355,    91,
     229,   101,   225,   227,   228,   226,   230,   259,   260,   231,
     235,   232,   234,   233,   236,   237,   279,   242,     0,   243,
     244,   238,     0,   241,   239,   349,   240,   354,     0,     0,
       0,     0,   295,     0,     0,     0,     0,     0,     0,     0,
       0,   529,   522,   531,     0,     0,   581,   577,    38,   270,
     156,   144,   148,   152,   153,   149,   150,   151,   154,   155,
      40,    70,   217,   223,   278,   294,   329,   377,   139,     0,
     205,    72,   526,    73,     0,   527,    94,   458,   488,     0,
     444,   136,   445,     0,     0,   195,    42,    24,     0,   542,
     538,   539,   544,   541,   545,   543,   540,   537,     0,    47,
     550,     0,     0,    22,    95,    74,    66,    26,    84,   262,
     269,   264,   266,     0,     0,    98,   341,   338,   346,   351,
      18,    10,    12,   280,     0,    20,    14,    16,    28,   450,
      30,   499,   486,    32,    98,     0,     0,    34,    36,   585,
     206,   206,     0,     0,   209,    88,   456,   454,   496,   135,
       0,   558,   579,     0,     4,   546,     0,   551,     0,   547,
     182,   183,   184,   186,   189,   188,   190,   191,   187,   185,
       0,     0,     0,     0,   179,   576,   157,   158,   159,   161,
     163,   165,   168,   171,   175,   180,   575,    61,     0,   112,
     103,   265,   192,     0,   343,     0,    92,     0,     0,     0,
     286,   296,   460,   503,   530,   523,   532,   582,   145,   257,
     224,   250,   251,   252,   258,   330,   378,     0,   207,   112,
       0,    98,   494,   489,   137,     0,   458,   193,   197,   200,
       0,   203,   206,   204,   196,   198,   201,     0,    43,     0,
      48,   548,   176,   177,   178,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   573,   206,    75,   132,
       0,     0,     0,     0,    97,   339,   347,   352,   281,   285,
       0,     0,   451,     0,   112,   103,   366,   371,     0,   481,
       0,     0,   590,   364,   365,   586,   588,     0,   592,     0,
     584,     0,     0,   247,   206,   285,   138,   210,     0,   211,
     214,     0,   457,   455,     0,    98,   559,   580,   194,     0,
     199,     3,     0,     0,     0,     0,   181,   160,   162,   164,
     166,   167,   169,   170,   172,   173,   174,   206,    62,   129,
     127,   386,   387,     0,   111,   119,   113,   123,   121,   125,
       0,   115,   117,   391,   109,   107,     0,   102,   104,   105,
       0,     0,   344,     0,     0,     0,     0,   286,   291,     0,
     287,   261,   307,   302,   303,   304,   305,   297,   306,     0,
       0,     0,   471,     0,   459,   461,   463,   465,   467,   469,
     473,   206,   206,   504,     0,   502,   505,   507,     0,     0,
       0,     0,   477,   476,     0,   480,   479,     0,     0,     0,
       0,     0,     0,     0,   583,   146,   361,   357,   360,   248,
       0,   331,   336,   286,     0,   208,   212,     0,   495,   490,
       0,     0,    44,   552,    49,   549,     0,   130,     0,    68,
       0,     0,     0,     0,     0,   390,   420,   417,   418,   419,
     381,   389,     0,     0,   206,     0,     0,    86,   106,   345,
     340,   348,   353,   282,   284,     0,   288,     0,     0,     0,
       0,     0,   452,     0,     0,     0,     0,     0,     0,   509,
     512,   501,     0,     0,     0,     0,   367,   372,   475,   571,
     572,   591,   587,   589,   478,   593,     0,     0,   358,   206,
     337,     0,   333,     0,   215,     0,    90,     0,   569,   565,
     567,   560,   564,   202,     0,   574,     0,   128,   120,   114,
     124,   122,   126,   206,   116,   118,     0,   110,   108,   292,
     206,   298,   484,   482,   483,   472,   462,   464,   466,   468,
     470,   474,     0,     0,   506,   508,   525,   534,     0,   206,
     147,   362,   359,   249,   332,   334,   380,     0,   213,   491,
       0,     0,     0,     0,   562,     0,    55,    41,    50,    54,
       0,   131,     0,   392,   289,     0,   395,   510,   513,   368,
     373,   256,     0,   206,     0,   570,   566,   568,   561,   563,
      57,     0,   206,    56,   382,     0,     0,   299,     0,     0,
     427,   427,   206,   431,   253,   363,   335,   492,     0,    51,
       0,     0,   393,   290,     0,   396,   403,     0,   402,   424,
     511,   514,   369,   428,   374,   254,   498,     0,    53,   408,
     383,   427,   314,   321,   319,   300,   310,   311,   318,     0,
     398,   399,     0,   427,     0,   435,   206,     0,   493,   556,
       0,   555,     0,     0,   427,   394,     0,     0,     0,     0,
       0,   206,   316,   404,   400,   425,   370,   429,   432,   375,
     255,   497,    58,   553,   407,   421,   422,   423,     0,   413,
     414,   384,   315,   322,   320,   301,   309,     0,   317,   206,
       0,     0,     0,     0,   206,     0,   409,   410,     0,   439,
       0,   324,   312,   405,   401,     0,     0,   433,    59,   554,
     411,   415,   436,   385,   323,   206,     0,   426,   430,     0,
       0,   206,     0,   325,   313,     0,   412,   416,   437,   434,
       0,   443,     0,   440,   438,   441,     0,   442
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -677,  -677,   330,   246,  -677,  -602,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -594,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -112,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,   282,  -677,
    -677,   115,  -677,  -677,  -677,   614,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,   616,  -677,   302,  -677,  -677,  -249,  -677,
    -677,   217,  -677,  -677,  -677,  -677,  -305,  -360,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -315,  -677,  -677,   -21,
    -677,  -677,  -189,   -10,  -677,     9,  -677,  -677,  -677,  -677,
    -193,     7,  -223,  -677,   248,   250,   247,   -68,   -57,  -128,
     -20,  -677,  -300,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
     -86,  -677,  -677,  -677,  -677,  -677,     5,    11,  -677,  -677,
    -677,   -71,   238,  -444,  -677,  -677,  -677,   -18,  -677,  -677,
    -589,   -89,  -677,  -677,   -11,  -677,   -56,  -677,  -677,   -47,
     -46,   -53,   -52,   -48,   352,  -677,   -37,  -677,   -36,  -677,
    -677,  -677,  -677,   209,   306,  -677,  -245,  -677,  -677,  -677,
     -35,  -677,   -32,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -143,  -677,  -677,  -677,  -677,  -677,  -144,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,   -38,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,   -64,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,   -66,  -677,  -677,  -677,   -65,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,    22,  -677,  -677,  -316,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,    14,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -603,  -677,  -677,  -677,  -677,  -677,
    -169,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -198,
    -677,  -677,  -505,  -677,  -676,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,    15,
      17,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
     296,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -286,   233,  -281,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,   579,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,   208,  -677,  -677,  -161,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,   -28,  -677,  -677,
    -677,   276,  -677,  -677,   101,  -677,  -677,  -677,  -677,  -677,
    -677,  -677,  -677,  -677,  -677,  -677,  -677,  -677
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    26,   378,    27,   169,   170,   174,   175,
     168,   173,   118,   113,   122,   176,   178,   180,   184,   185,
      79,    28,    81,    29,   112,   294,   452,    30,    31,   114,
     298,   454,   644,   721,   698,   722,   699,   700,   738,   824,
      32,   115,   397,    33,    34,   121,   329,   473,    35,    82,
      36,   139,   328,    37,    38,    39,   123,   330,   486,    40,
     214,   359,   557,    41,   255,    42,    99,   245,   336,    43,
      44,   401,   487,   488,   586,   585,   400,   474,   571,   582,
     583,   570,   573,   572,   574,   568,   398,   469,   646,   314,
     219,   290,   106,   349,    45,   475,    80,   280,   431,   626,
     191,   315,   332,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   333,    47,   293,   370,   371,   372,   373,   374,
      89,   210,   358,   438,   439,   556,   440,   476,    83,   202,
     281,   141,   142,   143,   144,   145,    49,   350,   433,   629,
     351,   710,   734,   776,   352,   353,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,    50,    84,    51,   171,
     340,   496,   408,   497,   594,   410,   500,   660,   726,   595,
      52,    85,    53,   264,   411,   597,   705,   744,   789,   507,
     765,   790,   766,   791,   846,   786,   767,   792,   768,   788,
     787,   830,   832,   845,    54,    55,    56,    86,   282,   434,
     631,   551,   632,   713,   552,   161,   337,   493,   162,   254,
     403,   163,   164,   338,   494,   165,   166,   339,   495,   167,
     354,   432,   547,   628,   548,   627,   712,   477,   423,   530,
     678,   732,   773,   424,   531,   679,   733,   775,   478,    87,
     283,   435,   633,   479,   653,   741,   784,   829,   480,   580,
     490,   584,   725,   761,   707,   728,   729,   747,   771,   820,
     748,   769,   819,   760,   782,   783,   808,   827,   850,   809,
     828,   851,   581,   810,   750,   772,   821,   754,   774,   822,
     799,   823,   849,   843,   852,   860,   862,   863,   866,   481,
     482,    60,    61,    62,   177,   342,   513,    63,   217,   361,
     287,   360,   412,   514,   603,   604,   605,   606,   607,   601,
     608,   639,   534,   640,   427,   536,   517,   518,   519,    64,
     179,    65,   102,   288,   445,   637,   714,   756,   363,   444,
     778,   272,   343,   524,   413,   525,   612,   613,   526,   672,
     730,   527,   673,   731,    66,    67,    68,    69,    70,   275,
     414,   614,    71,    72,    73,   182,   274,    74,   276,   415,
     615,    75,   238,   239,   299,   455,   240,   780,   803,   781,
      76,   108,   365,   560,   693,   641,   694,   642,   691,   692,
     690,   428,   242,   396,   326,    77,    78,   109,   366,   187,
     279,   429,   347,   430,   540,   541,   539,   543
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     105,   107,    90,    88,   160,   156,   157,   158,   100,   101,
     159,    46,   203,    48,   198,   199,    57,    58,   316,    59,
     404,   140,   223,   224,   192,   344,   617,   195,   196,   241,
     289,   422,   197,   193,   194,   295,   577,   405,   406,   407,
     683,   489,   696,   103,   208,   160,   156,   157,   158,   291,
     697,   159,   762,   523,   441,   751,     3,   103,   292,   190,
     200,   425,   140,   630,   703,   724,   426,   762,   416,   417,
     708,     7,   201,   532,   204,   205,   206,   207,   221,   763,
     764,   249,   212,   103,   250,   785,   215,   577,   103,   216,
     385,   110,   218,   619,   763,   764,   512,   796,   418,   620,
     369,   364,   357,   419,   220,   103,   420,   421,   811,   528,
     221,   111,   443,   300,   301,   302,   303,   304,   305,   306,
     307,   104,   742,   222,   308,   309,   515,   209,   209,   221,
     116,   516,  -140,   209,   437,   104,   172,   119,   696,   310,
     311,  -308,   209,   119,   312,   313,   697,   209,   209,   209,
     120,     7,   117,   300,   301,   302,   303,   304,   305,   306,
     307,   104,   198,   199,   308,   309,   104,   416,   417,   489,
    -326,   221,   192,   709,    17,   195,   196,   221,    96,   491,
     197,   193,   194,   221,   186,   313,   181,  -141,   284,   256,
     181,   257,   119,   188,    17,  -142,   559,   418,    92,  -448,
     119,  -520,   419,   244,   103,   225,   211,   190,   213,   -99,
     103,   393,   394,   395,   119,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   198,   199,   226,
     833,   136,   137,   138,   577,   251,   252,   192,   389,   390,
     195,   196,   391,   392,   638,   197,   193,   194,   103,   805,
     806,   807,   227,   501,   125,   126,   853,   243,   129,   130,
     131,   132,   857,   246,   793,   464,   465,   466,    11,   247,
     348,   248,   190,   221,   300,   301,   302,   303,   304,   305,
     306,   307,   104,   763,   764,   308,   309,   253,   104,   258,
     382,   383,   384,   -46,   259,   -46,    91,    93,   416,   417,
     310,   311,   368,   260,   375,   312,   313,   399,   261,   -46,
     -46,   262,   -46,   -46,   399,   263,   -46,    95,    98,   509,
     510,   460,   461,   841,   499,   577,   104,   681,   418,   511,
     265,   634,   266,   419,   462,   463,   420,   421,   -46,   442,
     657,   658,   -46,   316,   267,   618,   268,   270,   550,   269,
     271,   273,   277,   624,   278,   503,   -46,  -521,   504,   505,
     285,   450,    88,   506,   103,   286,   296,   297,   643,   327,
     331,   334,   436,   508,   335,   341,   345,   346,   638,   355,
     356,   566,   140,   379,   577,   362,    46,   468,    48,   380,
     502,    57,    58,   381,    59,   387,   386,   533,   402,   537,
     538,   388,  -356,   316,   446,   448,   533,   467,   449,   662,
     663,   664,   453,   456,   578,   579,   535,   545,   544,   546,
     549,   561,   555,   558,   554,    -8,   562,   563,   596,   564,
     589,   567,   569,   587,   588,   249,   590,   591,   593,   592,
    -283,   602,   300,   301,   302,   303,   304,   305,   306,   307,
     104,   616,   687,   308,   309,   611,   621,  -379,   622,   576,
     623,   160,   156,   157,   158,   578,   579,   159,   310,   311,
     625,   635,   647,   312,   313,   636,   648,   649,   140,   650,
     651,   652,   654,   655,   659,   661,   665,   666,   598,   599,
     600,   667,   668,   669,   670,   198,   199,   671,   656,   499,
     499,   674,   675,   689,   685,   192,   835,   836,   195,   196,
     576,   609,   610,   197,   193,   194,   676,   706,   677,   684,
     686,   715,   716,   717,  -397,   735,   718,   723,   739,   727,
     743,   -52,   746,   749,   855,   753,   103,   745,   755,   757,
     190,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,     9,    10,   759,    11,   136,   137,   138,
     770,   777,   779,  -406,   813,   794,   795,   702,   797,   798,
     802,   804,   812,   815,   814,   826,   470,   837,  -388,     5,
     825,   840,     8,  -388,  -388,  -388,  -388,  -388,  -388,  -388,
    -388,  -388,  -388,  -388,  -388,     9,    10,   842,    11,   844,
    -388,  -388,    12,   854,   847,   416,   417,   471,   472,  -388,
     848,   859,   578,   579,   104,    13,   858,   861,   864,   521,
     865,  -100,   867,   451,   377,   701,   119,   550,   758,   522,
      97,    94,   529,   680,   457,   459,   740,   458,    21,    22,
     688,   492,   704,   752,   553,   376,   409,   816,   818,   736,
     682,   834,   856,    46,   183,    48,  -388,   576,    57,    58,
     542,    59,   447,   565,   839,  -500,   719,   645,   711,    88,
       0,     0,   103,     5,   720,     0,     8,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   189,   134,   520,     9,
      10,     0,    11,   737,   137,   138,     0,     0,     0,     0,
       0,     0,     0,   578,   579,   817,     0,     0,     0,    13,
       0,     0,     0,     0,     0,     0,     0,   498,     0,  -206,
       0,   711,    88,     0,  -206,  -206,  -206,  -206,  -206,  -206,
    -206,  -206,  -206,  -206,  -206,  -206,  -206,  -206,     0,  -206,
    -206,  -206,  -206,     0,     0,     0,   831,     0,   576,    46,
     104,    48,     0,     0,    57,    58,   801,    59,     0,   367,
       0,     0,   578,   579,     0,   800,    88,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   160,
     156,   157,   158,     0,     0,   159,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   140,  -206,     0,     0,
     399,   399,     0,  -206,     0,     0,     0,   576,     0,     0,
       0,     0,     0,     0,   838,     0,     0,     0,     0,     0,
       0,     0,     0,   470,     0,  -388,     5,     0,   399,     8,
    -388,  -388,  -388,  -388,  -388,  -388,  -388,  -388,  -388,  -388,
    -388,  -388,     9,    10,     0,    11,     0,  -388,  -388,    12,
       0,     0,   416,   417,   471,   472,  -388,     0,     0,     0,
       0,     0,    13,     0,     0,   103,   483,   484,   485,     0,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   189,
     134,   135,     0,     0,     0,    21,    22,   137,   138,     0,
       0,     0,     0,     0,     0,     0,   575,     0,     0,     0,
       0,     0,     0,  -388,     0,     0,     0,   470,     0,  -388,
       5,     0,   -85,     8,  -388,  -388,  -388,  -388,  -388,  -388,
    -388,  -388,  -388,  -388,  -388,  -388,     9,    10,     0,    11,
       0,  -388,  -388,    12,     0,     0,   416,   417,   471,   472,
    -388,     0,     0,   104,     0,     0,    13,     0,     0,   103,
     483,   484,   485,     0,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   189,   134,   135,     0,     0,     0,    21,
      22,   137,   138,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -388,     0,     0,
       0,   470,     0,  -388,     5,     0,  -533,     8,  -388,  -388,
    -388,  -388,  -388,  -388,  -388,  -388,  -388,  -388,  -388,  -388,
       9,    10,     0,    11,     0,  -388,  -388,    12,     0,     0,
     416,   417,   471,   472,  -388,     0,   470,   104,  -388,     5,
      13,     0,     8,  -388,  -388,  -388,  -388,  -388,  -388,  -388,
    -388,  -388,  -388,  -388,  -388,     9,    10,     0,    11,     0,
    -388,  -388,    12,    21,    22,   416,   417,   471,   472,  -388,
       0,     0,     0,     0,     0,    13,     0,     0,     0,     0,
       0,  -388,     0,     0,     0,     0,     0,     0,     0,     0,
     -67,     0,     0,     0,     0,   103,     0,     0,    21,    22,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   189,
       0,     0,     0,     0,     0,     0,  -388,   137,   138,     0,
     470,     0,  -388,     5,     0,   -89,     8,  -388,  -388,  -388,
    -388,  -388,  -388,  -388,  -388,  -388,  -388,  -388,  -388,     9,
      10,     0,    11,     0,  -388,  -388,    12,     0,     0,   416,
     417,   471,   472,  -388,     0,     0,     0,     0,     0,    13,
       0,     0,     0,     0,     0,     4,     0,     0,     5,     6,
       7,     8,     0,   104,     0,     0,     0,     0,     0,     0,
       0,     0,    21,    22,     9,    10,     0,    11,     0,     0,
       0,    12,     0,     0,     0,     0,     0,     0,     0,     0,
    -388,     0,     0,     0,    13,    14,    15,    16,     0,  -524,
       0,     0,     0,    17,    18,     0,     0,    19,    -2,     4,
      20,     0,     5,     6,     7,     8,     0,    21,    22,     0,
       0,     0,     0,     0,    23,    24,   695,     0,     9,    10,
       0,    11,     0,     0,     0,    12,     0,     0,     0,     0,
      25,   209,     0,     0,     0,     0,     0,     0,    13,    14,
      15,    16,     0,     0,     0,     0,     0,    17,    18,     0,
       0,    19,     4,     0,    20,     5,     6,     7,     8,     0,
       0,    21,    22,     0,     0,     0,     0,     0,    23,    24,
       0,     9,    10,     0,    11,     0,     0,     4,    12,     0,
       5,     6,     7,     8,    25,     0,     0,     0,     0,     0,
       0,    13,    14,    15,    16,     0,     9,    10,     0,    11,
      17,    18,     0,    12,    19,     0,     0,    20,     0,     0,
       0,     0,     0,     0,    21,    22,    13,    14,    15,    16,
       0,    23,    24,   695,     0,    17,    18,     0,     0,    19,
       0,     0,    20,     0,     0,     0,     0,    25,     0,    21,
      22,     0,     0,   103,     0,     0,    23,    24,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
       9,    10,    25,    11,   136,   137,   138,   103,     0,     0,
       0,     0,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,     9,    10,     0,    11,   136,   137,
     138,   228,     0,   229,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   230,   231,     0,
     232,   233,     0,     0,   234,     0,     0,     0,     0,     0,
       0,   104,     0,     0,     0,     0,     0,   209,     0,     0,
       0,     0,     0,     0,     0,     0,   235,     0,     0,     0,
     236,     0,     0,     0,     0,   104,     0,     0,     0,     0,
       0,     0,     0,     0,   237
};

static const yytype_int16 yycheck[] =
{
      21,    22,    13,    13,    42,    42,    42,    42,    18,    19,
      42,     2,    83,     2,    80,    80,     2,     2,   241,     2,
     335,    42,   108,   109,    80,   274,   531,    80,    80,   115,
     219,   347,    80,    80,    80,   228,   480,   337,   338,   339,
     629,   401,   644,     3,     3,    83,    83,    83,    83,     3,
     644,    83,     1,   413,   359,   731,     0,     3,     3,    80,
      81,   347,    83,     3,     3,     3,   347,     1,    30,    31,
     673,     6,    82,    19,    84,    85,    86,    87,    81,    28,
      29,     8,    92,     3,    11,   761,    96,   531,     3,    99,
     313,     3,   102,     3,    28,    29,   412,   773,    60,     3,
     293,   290,   105,    65,    75,     3,    68,    69,   784,   414,
      81,    91,   361,    73,    74,    75,    76,    77,    78,    79,
      80,    81,   725,    75,    84,    85,   412,    87,    87,    81,
      88,   412,    88,    87,   357,    81,    22,    93,   740,    99,
     100,    90,    87,    93,   104,   105,   740,    87,    87,    87,
      35,     6,    88,    73,    74,    75,    76,    77,    78,    79,
      80,    81,   228,   228,    84,    85,    81,    30,    31,   529,
      88,    81,   228,   678,    49,   228,   228,    81,    53,   402,
     228,   228,   228,    81,    89,   105,    71,    88,   209,    92,
      75,    94,    93,    88,    49,    88,   445,    60,    53,    88,
      93,    88,    65,    48,     3,    89,    91,   228,    93,    88,
       3,   101,   102,   103,    93,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,   293,   293,    89,
     819,    24,    25,    26,   678,     8,     9,   293,    82,    83,
     293,   293,    99,   100,   560,   293,   293,   293,     3,    35,
      36,    37,    88,     8,     9,    10,   845,    88,    13,    14,
      15,    16,   851,    89,   769,   393,   394,   395,    23,    88,
     280,    89,   293,    81,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    28,    29,    84,    85,    91,    81,    91,
     310,   311,   312,     4,    91,     6,    14,    15,    30,    31,
      99,   100,   293,    88,   293,   104,   105,   328,    88,    20,
      21,    88,    23,    24,   335,    89,    27,    15,    16,    51,
      52,   389,   390,   828,   410,   769,    81,   627,    60,    61,
      88,   554,    88,    65,   391,   392,    68,    69,    49,   360,
     585,   586,    53,   566,    88,   534,    88,    88,   434,    89,
      89,    88,    88,   542,    88,   411,    67,    88,   411,   411,
      89,   372,   372,   411,     3,    93,    91,     3,   561,    92,
       8,    91,     3,   411,    47,   105,    89,    89,   694,    89,
      89,   467,   403,     3,   828,    93,   377,   397,   377,    92,
     411,   377,   377,    94,   377,    97,    96,   418,    94,   420,
     421,    98,   108,   626,    89,    88,   427,    94,   107,   598,
     599,   600,    92,   106,   480,   480,    58,    95,    90,   108,
      94,     4,   106,   444,    95,    90,    90,     3,   499,    89,
      92,    94,    90,    90,    88,     8,    92,    92,    90,    92,
      90,    90,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    31,     3,    84,    85,    90,    88,    90,    88,   480,
      88,   499,   499,   499,   499,   531,   531,   499,    99,   100,
      88,    94,    88,   104,   105,    90,    88,    88,   499,    88,
      88,    88,    88,    88,    88,   106,    88,    88,   509,   510,
     511,    88,    88,    88,    88,   561,   561,    88,   584,   585,
     586,    88,    88,    66,    94,   561,   821,   822,   561,   561,
     531,   521,   522,   561,   561,   561,    90,   105,    90,    90,
      90,    88,    88,    88,    35,   109,    90,    88,    90,    89,
      88,    90,    35,    38,   849,    55,     3,   106,    94,    91,
     561,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   105,    23,    24,    25,    26,
     106,    59,     3,   106,   787,    94,   105,   653,   105,    62,
      92,   106,    88,    90,    93,   106,     1,   105,     3,     4,
      94,    94,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    39,    23,    93,
      25,    26,    27,    88,   106,    30,    31,    32,    33,    34,
     106,   106,   678,   678,    81,    40,   105,    75,   106,    44,
      94,    88,    75,   377,   294,   646,    93,   713,   740,    54,
      16,    15,   415,   626,   386,   388,   722,   387,    63,    64,
     635,   403,   660,   732,   435,   293,   340,   790,   792,   713,
     628,   820,   850,   644,    75,   644,    81,   678,   644,   644,
     427,   644,   366,   455,   825,    90,   694,   566,   679,   679,
      -1,    -1,     3,     4,   695,    -1,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,   412,    20,
      21,    -1,    23,   714,    25,    26,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   769,   769,   791,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
      -1,   732,   732,    -1,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    -1,    -1,    -1,   817,    -1,   769,   740,
      81,   740,    -1,    -1,   740,   740,   777,   740,    -1,    90,
      -1,    -1,   828,   828,    -1,   776,   776,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   817,
     817,   817,   817,    -1,    -1,   817,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   817,    81,    -1,    -1,
     821,   822,    -1,    87,    -1,    -1,    -1,   828,    -1,    -1,
      -1,    -1,    -1,    -1,   824,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,     4,    -1,   849,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    23,    -1,    25,    26,    27,
      -1,    -1,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,     3,    44,    45,    46,    -1,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    63,    64,    25,    26,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    -1,    -1,    -1,     1,    -1,     3,
       4,    -1,    90,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      -1,    25,    26,    27,    -1,    -1,    30,    31,    32,    33,
      34,    -1,    -1,    81,    -1,    -1,    40,    -1,    -1,     3,
      44,    45,    46,    -1,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    63,
      64,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,
      -1,     1,    -1,     3,     4,    -1,    90,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    23,    -1,    25,    26,    27,    -1,    -1,
      30,    31,    32,    33,    34,    -1,     1,    81,     3,     4,
      40,    -1,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    23,    -1,
      25,    26,    27,    63,    64,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,     3,    -1,    -1,    63,    64,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    25,    26,    -1,
       1,    -1,     3,     4,    -1,    90,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    -1,    25,    26,    27,    -1,    -1,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,     1,    -1,    -1,     4,     5,
       6,     7,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    20,    21,    -1,    23,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    40,    41,    42,    43,    -1,    90,
      -1,    -1,    -1,    49,    50,    -1,    -1,    53,     0,     1,
      56,    -1,     4,     5,     6,     7,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    71,    72,    -1,    20,    21,
      -1,    23,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    40,    41,
      42,    43,    -1,    -1,    -1,    -1,    -1,    49,    50,    -1,
      -1,    53,     1,    -1,    56,     4,     5,     6,     7,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    20,    21,    -1,    23,    -1,    -1,     1,    27,    -1,
       4,     5,     6,     7,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    40,    41,    42,    43,    -1,    20,    21,    -1,    23,
      49,    50,    -1,    27,    53,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    40,    41,    42,    43,
      -1,    70,    71,    72,    -1,    49,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    86,    -1,    63,
      64,    -1,    -1,     3,    -1,    -1,    70,    71,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    86,    23,    24,    25,    26,     3,    -1,    -1,
      -1,    -1,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    24,    25,
      26,     4,    -1,     6,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      23,    24,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    67
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   111,   112,     0,     1,     4,     5,     6,     7,    20,
      21,    23,    27,    40,    41,    42,    43,    49,    50,    53,
      56,    63,    64,    70,    71,    86,   113,   115,   131,   133,
     137,   138,   150,   153,   154,   158,   160,   163,   164,   165,
     169,   173,   175,   179,   180,   204,   205,   223,   237,   246,
     266,   268,   280,   282,   304,   305,   306,   348,   399,   400,
     401,   402,   403,   407,   429,   431,   454,   455,   456,   457,
     458,   462,   463,   464,   467,   471,   480,   495,   496,   130,
     206,   132,   159,   238,   267,   281,   307,   349,   203,   230,
     254,   158,    53,   158,   173,   175,    53,   165,   175,   176,
     203,   203,   432,     3,    81,   199,   202,   199,   481,   497,
       3,    91,   134,   123,   139,   151,    88,    88,   122,    93,
     161,   155,   124,   166,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    24,    25,    26,   161,
     199,   241,   242,   243,   244,   245,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   268,   280,   282,
     306,   315,   318,   321,   322,   325,   326,   329,   120,   116,
     117,   269,    22,   121,   118,   119,   125,   404,   126,   430,
     127,   161,   465,   465,   128,   129,    89,   499,    88,    17,
     199,   210,   256,   259,   260,   261,   262,   263,   322,   326,
     199,   203,   239,   241,   203,   203,   203,   203,     3,    87,
     231,   161,   203,   161,   170,   203,   203,   408,   203,   200,
      75,    81,    75,   230,   230,    89,    89,    88,     4,     6,
      20,    21,    23,    24,    27,    49,    53,    67,   472,   473,
     476,   230,   492,    88,    48,   177,    89,    88,    89,     8,
      11,     8,     9,    91,   319,   174,    92,    94,    91,    91,
      88,    88,    88,    89,   283,    88,    88,    88,    88,    89,
      88,    89,   441,    88,   466,   459,   468,    88,    88,   500,
     207,   240,   308,   350,   199,    89,    93,   410,   433,   202,
     201,     3,     3,   224,   135,   210,    91,     3,   140,   474,
      73,    74,    75,    76,    77,    78,    79,    80,    84,    85,
      99,   100,   104,   105,   199,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   494,    92,   162,   156,
     167,     8,   212,   222,    91,    47,   178,   316,   323,   327,
     270,   105,   405,   442,   178,    89,    89,   502,   203,   203,
     247,   250,   254,   255,   330,    89,    89,   105,   232,   171,
     411,   409,    93,   438,   202,   482,   498,    90,   205,   210,
     225,   226,   227,   228,   229,   237,   264,   112,   114,     3,
      92,    94,   220,   220,   220,   212,    96,    97,    98,    82,
      83,    99,   100,   101,   102,   103,   493,   152,   196,   199,
     186,   181,    94,   320,   196,   222,   222,   222,   272,   274,
     275,   284,   412,   444,   460,   469,    30,    31,    60,    65,
      68,    69,   337,   338,   343,   421,   423,   424,   491,   501,
     503,   208,   331,   248,   309,   351,     3,   212,   233,   234,
     236,   186,   199,   178,   439,   434,    89,   410,    88,   107,
     254,   113,   136,    92,   141,   475,   106,   214,   215,   216,
     217,   217,   218,   218,   219,   219,   219,    94,   203,   197,
       1,    32,    33,   157,   187,   205,   237,   337,   348,   353,
     358,   399,   400,    44,    45,    46,   168,   182,   183,   187,
     360,   212,   242,   317,   324,   328,   271,   273,     1,   230,
     276,     8,   199,   256,   261,   262,   263,   289,   306,    51,
      52,    61,   337,   406,   413,   421,   423,   426,   427,   428,
     491,    44,    54,   187,   443,   445,   448,   451,   186,   181,
     339,   344,    19,   199,   422,    58,   425,   199,   199,   506,
     504,   505,   422,   507,    90,    95,   108,   332,   334,    94,
     230,   311,   314,   273,    95,   106,   235,   172,   199,   178,
     483,     4,    90,     3,    89,   476,   230,    94,   195,    90,
     191,   188,   193,   192,   194,    34,   199,   243,   322,   326,
     359,   382,   189,   190,   361,   185,   184,    90,    88,    92,
      92,    92,    92,    90,   274,   279,   241,   285,   199,   199,
     199,   419,    90,   414,   415,   416,   417,   418,   420,   203,
     203,    90,   446,   447,   461,   470,    31,   382,   202,     3,
       3,    88,    88,    88,   202,    88,   209,   335,   333,   249,
       3,   310,   312,   352,   212,    94,    90,   435,   337,   421,
     423,   485,   487,   210,   142,   494,   198,    88,    88,    88,
      88,    88,    88,   354,    88,    88,   230,   276,   276,    88,
     277,   106,   202,   202,   202,    88,    88,    88,    88,    88,
      88,    88,   449,   452,    88,    88,    90,    90,   340,   345,
     211,   222,   334,   250,    90,    94,    90,     3,   236,    66,
     490,   488,   489,   484,   486,    72,   115,   131,   144,   146,
     147,   199,   230,     3,   247,   286,   105,   364,   364,   382,
     251,   254,   336,   313,   436,    88,    88,    88,    90,   487,
     199,   143,   145,    88,     3,   362,   278,    89,   365,   366,
     450,   453,   341,   346,   252,   109,   314,   199,   148,    90,
     230,   355,   364,    88,   287,   106,    35,   367,   370,    38,
     384,   384,   251,    55,   387,    94,   437,    91,   146,   105,
     373,   363,     1,    28,    29,   290,   292,   296,   298,   371,
     106,   368,   385,   342,   388,   347,   253,    59,   440,     3,
     477,   479,   374,   375,   356,   384,   295,   300,   299,   288,
     291,   293,   297,   382,    94,   105,   384,   105,    62,   390,
     254,   199,    92,   478,   106,    35,    36,    37,   376,   379,
     383,   384,    88,   212,    93,    90,   292,   230,   298,   372,
     369,   386,   389,   391,   149,    94,   106,   377,   380,   357,
     301,   241,   302,   250,   370,   196,   196,   105,   203,   479,
      94,   382,    39,   393,    93,   303,   294,   106,   106,   392,
     378,   381,   394,   250,    88,   196,   379,   250,   105,   106,
     395,    75,   396,   397,   106,    94,   398,    75
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   110,   111,   112,   112,   113,   113,   113,   114,   116,
     115,   117,   115,   118,   115,   119,   115,   120,   115,   121,
     115,   122,   115,   123,   115,   124,   115,   125,   115,   126,
     115,   127,   115,   128,   115,   129,   115,   130,   115,   132,
     131,   134,   135,   136,   133,   137,   139,   140,   141,   142,
     143,   138,   144,   145,   145,   146,   146,   148,   149,   147,
     151,   152,   150,   153,   153,   155,   156,   157,   154,   159,
     158,   160,   160,   160,   162,   161,   161,   163,   163,   163,
     163,   164,   164,   166,   167,   168,   165,   170,   171,   172,
     169,   174,   173,   176,   175,   177,   177,   178,   178,   179,
     179,   180,   181,   181,   182,   182,   182,   184,   183,   185,
     183,   186,   186,   188,   187,   189,   187,   190,   187,   191,
     187,   192,   187,   193,   187,   194,   187,   195,   187,   196,
     198,   197,   197,   199,   200,   199,   201,   199,   202,   203,
     204,   204,   204,   206,   207,   208,   209,   205,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   211,   212,   213,
     213,   214,   214,   215,   215,   216,   216,   216,   217,   217,
     217,   218,   218,   218,   218,   219,   219,   219,   219,   220,
     220,   220,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   222,   223,   224,   224,   225,   225,   225,   226,
     227,   227,   228,   229,   229,   230,   230,   231,   232,   232,
     233,   233,   234,   235,   235,   236,   238,   237,   237,   237,
     237,   237,   237,   240,   239,   241,   241,   242,   242,   242,
     243,   243,   243,   243,   243,   243,   243,   243,   244,   244,
     244,   244,   245,   245,   245,   246,   246,   247,   249,   248,
     248,   250,   250,   251,   253,   252,   252,   254,   255,   256,
     256,   257,   257,   257,   258,   258,   258,   259,   259,   259,
     260,   261,   261,   262,   263,   264,   265,   267,   266,   269,
     270,   271,   268,   272,   273,   273,   275,   274,   277,   278,
     276,   279,   276,   281,   280,   283,   284,   285,   286,   287,
     288,   282,   289,   289,   289,   289,   289,   289,   290,   291,
     291,   293,   294,   292,   295,   292,   296,   297,   297,   299,
     298,   300,   301,   298,   303,   302,   304,   305,   307,   308,
     309,   310,   306,   311,   313,   312,   312,   314,   316,   317,
     315,   315,   319,   320,   318,   321,   323,   324,   322,   322,
     325,   327,   328,   326,   326,   329,   331,   330,   332,   333,
     333,   335,   336,   334,   337,   337,   339,   340,   341,   342,
     338,   344,   345,   346,   347,   343,   349,   350,   351,   352,
     348,   354,   355,   356,   357,   353,   358,   358,   358,   359,
     359,   361,   362,   363,   360,   365,   364,   366,   364,   367,
     369,   368,   368,   371,   372,   370,   374,   373,   375,   373,
     376,   378,   377,   377,   380,   381,   379,   382,   382,   382,
     382,   383,   383,   383,   385,   386,   384,   384,   388,   389,
     387,   387,   391,   392,   390,   390,   394,   395,   393,   393,
     396,   398,   397,   397,   399,   400,   401,   401,   402,   404,
     405,   406,   403,   408,   409,   407,   411,   410,   410,   412,
     412,   414,   413,   415,   413,   416,   413,   417,   413,   418,
     413,   419,   413,   420,   413,   421,   422,   422,   423,   424,
     425,   425,   426,   427,   428,   430,   429,   432,   433,   434,
     435,   436,   437,   431,   439,   438,   438,   440,   440,   442,
     443,   441,   444,   444,   445,   446,   445,   447,   445,   449,
     450,   448,   452,   453,   451,   454,   454,   454,   455,   455,
     456,   457,   459,   460,   461,   458,   462,   463,   464,   466,
     465,   468,   469,   470,   467,   471,   471,   472,   472,   472,
     472,   472,   472,   472,   472,   472,   472,   473,   475,   474,
     474,   476,   476,   477,   478,   478,   479,   481,   482,   483,
     484,   480,   485,   486,   486,   488,   487,   489,   487,   490,
     487,   491,   491,   492,   493,   493,   494,   495,   497,   498,
     496,   500,   501,   499,   502,   502,   504,   503,   505,   503,
     506,   503,   507,   503
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     2,     2,     2,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     0,     0,     7,     2,     0,     0,     0,     0,
       0,    11,     2,     3,     0,     1,     2,     0,     0,     8,
       0,     0,     6,     1,     1,     0,     0,     0,     7,     0,
       3,     2,     3,     3,     0,     4,     0,     1,     1,     1,
       1,     2,     1,     0,     0,     0,     7,     0,     0,     0,
       8,     0,     4,     0,     3,     1,     0,     2,     0,     2,
       1,     2,     2,     0,     1,     1,     2,     0,     3,     0,
       3,     2,     0,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     2,
       0,     4,     0,     1,     0,     3,     0,     4,     1,     2,
       1,     2,     2,     0,     0,     0,     0,     9,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     3,     1,     3,     1,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     1,     2,     2,     2,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     5,     3,     0,     1,     1,     1,     2,
       1,     1,     4,     1,     1,     2,     0,     3,     3,     0,
       1,     1,     2,     3,     0,     3,     0,     3,     1,     1,
       1,     2,     1,     0,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     0,     4,
       0,     1,     1,     2,     0,     4,     0,     1,     1,     1,
       1,     1,     2,     1,     2,     3,     2,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     0,     3,     0,
       0,     0,     7,     2,     2,     0,     0,     2,     0,     0,
       6,     0,     3,     0,     3,     0,     0,     0,     0,     0,
       0,    14,     1,     1,     1,     1,     1,     1,     2,     2,
       0,     0,     0,     6,     0,     3,     2,     2,     0,     0,
       3,     0,     0,     5,     0,     3,     1,     1,     0,     0,
       0,     0,     9,     2,     0,     4,     0,     2,     0,     0,
       6,     2,     0,     0,     5,     6,     0,     0,     6,     1,
       1,     0,     0,     6,     1,     1,     0,     3,     2,     2,
       0,     0,     0,     5,     1,     1,     0,     0,     0,     0,
       9,     0,     0,     0,     0,     9,     0,     0,     0,     0,
       9,     0,     0,     0,     0,    11,     1,     1,     0,     1,
       1,     0,     0,     0,     8,     0,     3,     0,     4,     2,
       0,     4,     0,     0,     0,     5,     0,     3,     0,     4,
       2,     0,     4,     0,     0,     0,     5,     1,     1,     1,
       1,     1,     1,     1,     0,     0,     6,     0,     0,     0,
       6,     0,     0,     0,     6,     0,     0,     0,     6,     0,
       2,     0,     4,     0,     3,     3,     1,     1,     2,     0,
       0,     0,     7,     0,     0,     6,     0,     3,     0,     2,
       0,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     3,     1,     1,     3,     2,
       1,     0,     3,     3,     3,     0,     3,     0,     0,     0,
       0,     0,     0,    13,     0,     3,     0,     2,     0,     0,
       0,     5,     2,     0,     1,     0,     3,     0,     3,     0,
       0,     6,     0,     0,     6,     1,     1,     1,     1,     1,
       2,     3,     0,     0,     0,     8,     3,     3,     2,     0,
       3,     0,     0,     0,     8,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     0,     4,
       0,     2,     5,     2,     3,     0,     1,     0,     0,     0,
       0,    10,     2,     2,     0,     0,     3,     0,     3,     0,
       3,     3,     3,     3,     4,     0,     1,     2,     0,     0,
       6,     0,     0,     5,     2,     0,     0,     3,     0,     3,
       0,     3,     0,     3
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
#line 376 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 2330 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 379 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 2337 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 382 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 2344 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 392 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AnnotationDeclSeen);
        }
#line 2352 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 396 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2360 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 400 "fe/idl.ypp" /* yacc.c:1646  */
    {
// fixed_definition : type_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 2369 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 405 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2378 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 410 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | typeid_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 2387 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 415 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2396 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 420 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | typeprefix_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 2405 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 425 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2414 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 430 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | const_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 2423 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 435 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2432 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 440 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | exception
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 2441 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 445 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
        idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2450 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 450 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | interface_def
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
        }
#line 2459 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 455 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2468 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 460 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | module
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
        }
#line 2477 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 465 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2486 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 470 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | value_def
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeDeclSeen);
        }
#line 2495 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 475 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2504 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 480 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | component
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentDeclSeen);
        }
#line 2513 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 485 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2522 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 490 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | home_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeDeclSeen);
        }
#line 2531 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 495 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2540 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 500 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | event
          idl_global->set_parse_state (IDL_GlobalData::PS_EventDeclSeen);
        }
#line 2549 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 505 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2558 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 510 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | porttype_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeDeclSeen);
        }
#line 2567 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 515 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2576 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 520 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | connector_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorDeclSeen);
        }
#line 2585 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 525 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 2594 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 530 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 2603 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 535 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 2613 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 544 "fe/idl.ypp" /* yacc.c:1646  */
    {
// module_header  : IDL_MODULE
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
        }
#line 2622 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 549 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 2630 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 556 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 2673 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 595 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
        idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
        }
#line 2682 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 600 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_definition
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
        }
#line 2691 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 605 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
          /*
           * Finished with this module - pop it from the scope stack.
           */

          idl_global->scopes ().pop ();
        }
#line 2705 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 618 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleIDSeen);
        }
#line 2713 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 625 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 2732 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 640 "fe/idl.ypp" /* yacc.c:1646  */
    {
          if (FE_Utils::duplicate_param_id ((yyvsp[0].plval)))
            {
              idl_global->err ()->duplicate_param_id (
                (yyvsp[-2].idlist));

              return 1;
            }
        }
#line 2746 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 650 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 2781 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 681 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleSqSeen);
        }
#line 2789 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 685 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleBodySeen);
        }
#line 2797 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 689 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 2819 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 724 "fe/idl.ypp" /* yacc.c:1646  */
    {
// template_module_ref : IDL_ALIAS scoped_name
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefSeen);
        }
#line 2829 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 730 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        '<' at_least_one_formal_parameter_name '>'
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefParamsSeen);
        }
#line 2839 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 736 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 2922 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 818 "fe/idl.ypp" /* yacc.c:1646  */
    {
// template_module_inst : template_module_header
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleSeen);
        }
#line 2932 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 824 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        at_least_one_actual_parameter '>'
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleArgsSeen);
        }
#line 2942 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 830 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3009 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 901 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3052 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 940 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
#line 3061 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 945 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
#line 3070 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 950 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
#line 3084 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 963 "fe/idl.ypp" /* yacc.c:1646  */
    {
// interface_decl : IDL_INTERFACE
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
#line 3093 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 968 "fe/idl.ypp" /* yacc.c:1646  */
    {
//       defining_id
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (yyval.idval) = (yyvsp[0].idval);
         }
#line 3103 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 977 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3144 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 1015 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3178 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 1046 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3212 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 1079 "fe/idl.ypp" /* yacc.c:1646  */
    {
// inheritance_spec : ':' opt_truncatable
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 3221 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 1084 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_scoped_name
          (yyvsp[0].nlval)->truncatable ((yyvsp[-2].bval));
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3231 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 1090 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.nlval) = 0;
        }
#line 3240 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 1105 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3255 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 1120 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3303 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 1164 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3312 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 1169 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3321 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 1174 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3345 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 1198 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3393 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 1242 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3402 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 1247 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3411 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 1252 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 3425 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 1266 "fe/idl.ypp" /* yacc.c:1646  */
    {
// value_header : value_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 3434 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 1271 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3470 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 1306 "fe/idl.ypp" /* yacc.c:1646  */
    {
// value_decl : IDL_VALUETYPE
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
#line 3479 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 1311 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      defining_id
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 3489 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 1320 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_truncatable : IDL_TRUNCATABLE
          (yyval.bval) = true;
        }
#line 3498 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 1325 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.bval) = false;
        }
#line 3507 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 1334 "fe/idl.ypp" /* yacc.c:1646  */
    {
// supports_spec : IDL_SUPPORTS at_least_one_scoped_name
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3516 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 1339 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |    EMPTY */
          (yyval.nlval) = 0;
        }
#line 3525 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 1348 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3553 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 1373 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3581 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 1400 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3647 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 1477 "fe/idl.ypp" /* yacc.c:1646  */
    {
// state_member : IDL_PUBLIC
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_PUBLIC;
        }
#line 3657 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 1483 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        member_i
        }
#line 3665 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 1487 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_PRIVATE
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_PRIVATE;
        }
#line 3675 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 1493 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        member_i
        }
#line 3683 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 1505 "fe/idl.ypp" /* yacc.c:1646  */
    {
// export : type_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 3692 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 1510 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3701 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 1515 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | typeid_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 3710 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 1520 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3719 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 1525 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | typeprefix_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 3728 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 1530 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3737 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 1535 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | const_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 3746 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 1540 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
#line 3755 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 1545 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | exception
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 3764 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 1550 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3773 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 1555 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 3782 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 1560 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3791 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 1565 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | operation
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
#line 3800 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 1570 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3809 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 1575 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 3818 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 1580 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 3828 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 1589 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_scoped_name : scoped_name scoped_names
          ACE_NEW_RETURN ((yyval.nlval),
                          UTL_NameList ((yyvsp[-1].idlist),
                                        (yyvsp[0].nlval)),
                          1);
        }
#line 3840 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 1601 "fe/idl.ypp" /* yacc.c:1646  */
    {
// scoped_names : scoped_names ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
#line 3849 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 1606 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3874 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 1627 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.nlval) = 0;
        }
#line 3883 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 1642 "fe/idl.ypp" /* yacc.c:1646  */
    {
// scoped_name : id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN ((yyval.idlist),
                          UTL_IdList ((yyvsp[0].idval),
                                      0),
                          1);
        }
#line 3897 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 1652 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_SCOPE_DELIMITOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
#line 3906 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 1657 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3931 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 1679 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | scoped_name IDL_SCOPE_DELIMITOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);

          // This cleans up all the non-global "::"s in scoped names.
          // If there is a global one, it gets put into the UTL_IdList,
          // so we clean it up in the case above.
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3946 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 1690 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 3963 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 1705 "fe/idl.ypp" /* yacc.c:1646  */
    {
// id: IDENTIFIER
          ACE_NEW_RETURN ((yyval.idval),
                          Identifier ((yyvsp[0].strval)),
                          1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3976 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 1716 "fe/idl.ypp" /* yacc.c:1646  */
    {
          /* defining_id is a defining identifier
             whereas id is usually a reference to a defining identifier */
          ACE_NEW_RETURN ((yyval.idval),
                          Identifier ((yyvsp[0].strval)),
                          1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3990 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 1729 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4039 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 1775 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4070 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 1803 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4101 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 1833 "fe/idl.ypp" /* yacc.c:1646  */
    {
// const_dcl : IDL_CONST
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
#line 4110 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 1838 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      const_type
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
#line 4119 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 1843 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      defining_id
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
#line 4128 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 1848 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '='
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
#line 4137 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 1853 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4205 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 1926 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4221 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 1938 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | wstring_type_spec
          (yyval.etval) = AST_Expression::EV_wstring;
        }
#line 4230 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 1943 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4301 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 2017 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4316 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 2032 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4331 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 2047 "fe/idl.ypp" /* yacc.c:1646  */
    {
// and_expr : shift_expr | and_expr '&' shift_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_and,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4345 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 2061 "fe/idl.ypp" /* yacc.c:1646  */
    {
// shift_expr : add_expr | shift_expr IDL_LEFT_SHIFT add_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_left,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4359 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 2071 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | shift_expr IDL_RIGHT_SHIFT add_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_right,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4373 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 2085 "fe/idl.ypp" /* yacc.c:1646  */
    {
// add_expr : mult_expr | add_expr '+' mult_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_add,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4387 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 2095 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | add_expr '-' mult_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_minus,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4401 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 2109 "fe/idl.ypp" /* yacc.c:1646  */
    {
// mult_expr : unary_expr | mult_expr '*' unary_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mul,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4415 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 2119 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | mult_expr '/' unary_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_div,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4429 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 2129 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | mult_expr '%' unary_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mod,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4443 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 2143 "fe/idl.ypp" /* yacc.c:1646  */
    {
// unary_expr : primary_expr | '+' primary_expr
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_u_plus,
                                    (yyvsp[0].exval),
                                    0
                                  );
        }
#line 4457 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 177:
#line 2153 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | '-' primary_expr
          (yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_u_minus,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4471 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 2163 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | '~' primary_expr
          (yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_bit_neg,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4485 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 2176 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4540 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 2228 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | literal
//      | '(' const_expr ')'
          (yyval.exval) = (yyvsp[-1].exval);
        }
#line 4550 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 2237 "fe/idl.ypp" /* yacc.c:1646  */
    {
// literal : IDL_INTEGER_LITERAL
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].ival));
        }
#line 4559 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 2242 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_UINTEGER_LITERAL
          (yyval.exval) =
            idl_global->gen ()->create_expr ((yyvsp[0].uival));
        }
#line 4569 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 2248 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_STRING_LITERAL
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].sval));
          (yyvsp[0].sval)->destroy ();
          delete (yyvsp[0].sval);
          (yyvsp[0].sval) = 0;
        }
#line 4581 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 2256 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_WSTRING_LITERAL
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].wsval));
        }
#line 4590 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 2261 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_CHARACTER_LITERAL
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].cval));
        }
#line 4599 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 2266 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_WCHAR_LITERAL
          ACE_OutputCDR::from_wchar wc ((yyvsp[0].wcval));
          (yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
#line 4609 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 2272 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].fixval));
        }
#line 4617 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 2276 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_FLOATING_PT_LITERAL
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].dval));
        }
#line 4626 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 2281 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_TRUETOK
          (yyval.exval) = idl_global->gen ()->create_expr (true);
        }
#line 4635 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 2286 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_FALSETOK
          (yyval.exval) = idl_global->gen ()->create_expr (false);
        }
#line 4644 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 2294 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4714 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 2362 "fe/idl.ypp" /* yacc.c:1646  */
    {
          if (idl_global->idl_version_ >= IDL_VERSION_4)
            {
              ACE_DEBUG ((LM_WARNING,
                ACE_TEXT ("WARNING: in %C on line %d:\n")
                ACE_TEXT ("Declaring annotations is not supported at the ")
                ACE_TEXT ("momment, this annotation is being ignored!\n"),
                idl_global->filename ()->get_string (),
                idl_global->lineno ()
                ));
            }
          else
            {
              ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("ERROR: in %C on line %d:\n")
                ACE_TEXT ("Annotations are an IDL4 feature, ")
                ACE_TEXT ("they not supported in IDL %C!\n"),
                idl_global->filename ()->get_string (),
                idl_global->lineno (),
                idl_global->idl_version_.to_string ()
                ));
              idl_global->err()->syntax_error (idl_global->parse_state());
            }
        }
#line 4743 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 2418 "fe/idl.ypp" /* yacc.c:1646  */
    {
          if (idl_global->idl_version_ < IDL_VERSION_4)
            {
              ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("ERROR: in %C on line %d:\n")
                ACE_TEXT ("Annotations are an IDL4 feature, ")
                ACE_TEXT ("they not supported in IDL %C!\n"),
                idl_global->filename ()->get_string (),
                idl_global->lineno (),
                idl_global->idl_version_.to_string ()
                ));
              idl_global->err ()->syntax_error (idl_global->parse_state ());
            }

          Annotations *annotations = (yyvsp[-1].annotations);
          AST_Annotation *annotation =
            idl_global->gen ()->create_annotation ((yyvsp[0].idlist));
          annotations->insert_head (annotation);
          (yyval.annotations) = annotations;
        }
#line 4768 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 2439 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.annotations) = new Annotations ();
        }
#line 4776 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 2446 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.idlist) = (yyvsp[-1].idlist);
        }
#line 4784 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 216:
#line 2476 "fe/idl.ypp" /* yacc.c:1646  */
    {
// type_dcl : IDL_TYPEDEF
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 4794 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 217:
#line 2482 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      type_declarator
          (yyval.dcval) = 0;
        }
#line 4803 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 218:
#line 2487 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | struct_type
          (yyval.dcval) = 0;
        }
#line 4812 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 219:
#line 2492 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | union_type
          (yyval.dcval) = 0;
        }
#line 4821 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 220:
#line 2497 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | enum_type
          (yyval.dcval) = 0;
        }
#line 4830 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 221:
#line 2502 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4861 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 222:
#line 2529 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | constructed_forward_type_spec
          (yyval.dcval) = 0;
        }
#line 4870 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 223:
#line 2537 "fe/idl.ypp" /* yacc.c:1646  */
    {
// type_declarator : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
#line 4879 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 224:
#line 2542 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4947 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 227:
#line 2614 "fe/idl.ypp" /* yacc.c:1646  */
    {
// simple_type_spec : base_type_spec
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 4959 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 229:
#line 2623 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4988 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 247:
#line 2680 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_declarator : declarator declarators
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5000 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 248:
#line 2692 "fe/idl.ypp" /* yacc.c:1646  */
    {
// declarators : declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5009 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 249:
#line 2697 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5034 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 250:
#line 2718 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.dlval) = 0;
        }
#line 5043 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 253:
#line 2731 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_simple_declarator : simple_declarator simple_declarators
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5055 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 254:
#line 2743 "fe/idl.ypp" /* yacc.c:1646  */
    {
// simple_declarators : simple_declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5064 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 255:
#line 2748 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5089 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 256:
#line 2769 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.dlval) = 0;
        }
#line 5098 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 257:
#line 2777 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5116 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 258:
#line 2794 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5136 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 261:
#line 2818 "fe/idl.ypp" /* yacc.c:1646  */
    {
// signed_int : IDL_LONG
          (yyval.etval) = AST_Expression::EV_long;
        }
#line 5145 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 262:
#line 2823 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_LONG IDL_LONG
          (yyval.etval) = AST_Expression::EV_longlong;
        }
#line 5154 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 263:
#line 2828 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_SHORT
          (yyval.etval) = AST_Expression::EV_short;
        }
#line 5163 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 264:
#line 2836 "fe/idl.ypp" /* yacc.c:1646  */
    {
// unsigned_int : IDL_UNSIGNED IDL_LONG
          (yyval.etval) = AST_Expression::EV_ulong;
        }
#line 5172 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 265:
#line 2841 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_UNSIGNED IDL_LONG IDL_LONG
          (yyval.etval) = AST_Expression::EV_ulonglong;
        }
#line 5181 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 266:
#line 2846 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_UNSIGNED IDL_SHORT
          (yyval.etval) = AST_Expression::EV_ushort;
        }
#line 5190 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 267:
#line 2854 "fe/idl.ypp" /* yacc.c:1646  */
    {
// floating_pt_type : IDL_DOUBLE
          (yyval.etval) = AST_Expression::EV_double;
        }
#line 5199 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 268:
#line 2859 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_FLOAT
          (yyval.etval) = AST_Expression::EV_float;
        }
#line 5208 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 269:
#line 2864 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_LONG IDL_DOUBLE
          (yyval.etval) = AST_Expression::EV_longdouble;
        }
#line 5217 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 270:
#line 2872 "fe/idl.ypp" /* yacc.c:1646  */
    {
// fixed_type : IDL_FIXED
          (yyval.etval) = AST_Expression::EV_fixed;
        }
#line 5226 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 271:
#line 2880 "fe/idl.ypp" /* yacc.c:1646  */
    {
// char_type : IDL_CHAR
          (yyval.etval) = AST_Expression::EV_char;
        }
#line 5235 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 272:
#line 2885 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_WCHAR
          (yyval.etval) = AST_Expression::EV_wchar;
        }
#line 5244 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 273:
#line 2893 "fe/idl.ypp" /* yacc.c:1646  */
    {
// octet_type : IDL_OCTET
          (yyval.etval) = AST_Expression::EV_octet;
        }
#line 5253 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 274:
#line 2901 "fe/idl.ypp" /* yacc.c:1646  */
    {
// boolean_type : IDL_BOOLEAN
          (yyval.etval) = AST_Expression::EV_bool;
        }
#line 5262 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 275:
#line 2909 "fe/idl.ypp" /* yacc.c:1646  */
    {
// any_type : IDL_ANY
          (yyval.etval) = AST_Expression::EV_any;
        }
#line 5271 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 276:
#line 2917 "fe/idl.ypp" /* yacc.c:1646  */
    {
// object_type : IDL_OBJECT
          (yyval.etval) = AST_Expression::EV_object;
        }
#line 5280 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 277:
#line 2925 "fe/idl.ypp" /* yacc.c:1646  */
    {
// struct_decl : IDL_STRUCT
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
#line 5289 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 278:
#line 2930 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      defining_id
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5299 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 279:
#line 2940 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5337 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 280:
#line 2974 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
#line 5346 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 281:
#line 2979 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_member
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
#line 5355 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 282:
#line 2984 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5372 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 286:
#line 3006 "fe/idl.ypp" /* yacc.c:1646  */
    {
// member  :
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_NA;
        }
#line 5382 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 288:
#line 3017 "fe/idl.ypp" /* yacc.c:1646  */
    {
// member_i : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
#line 5391 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 289:
#line 3022 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
#line 5400 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 290:
#line 3027 "fe/idl.ypp" /* yacc.c:1646  */
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
                  f->annotations ((yyvsp[-5].annotations));
                  (void) s->fe_add_field (f);
                }
            }

          (yyvsp[-2].dlval)->destroy ();
          delete (yyvsp[-2].dlval);
          (yyvsp[-2].dlval) = 0;
        }
#line 5463 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 291:
#line 3086 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state ());
        }
#line 5472 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 292:
#line 3091 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 5482 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 293:
#line 3100 "fe/idl.ypp" /* yacc.c:1646  */
    {
// union_decl : IDL_UNION
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
#line 5491 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 294:
#line 3105 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      defining_id
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5501 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 295:
#line 3114 "fe/idl.ypp" /* yacc.c:1646  */
    {
// union_type : union_decl IDL_SWITCH
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
#line 5510 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 296:
#line 3119 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5550 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 297:
#line 3155 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      switch_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
#line 5559 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 298:
#line 3160 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5620 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 299:
#line 3217 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
#line 5629 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 300:
#line 3222 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_case_branch
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
#line 5638 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 301:
#line 3227 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5659 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 302:
#line 3247 "fe/idl.ypp" /* yacc.c:1646  */
    {
// switch_type_spec : integer_type
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5671 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 303:
#line 3255 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5689 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 304:
#line 3269 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | octet_type
          /* octets are not allowed. */
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5703 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 305:
#line 3279 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | boolean_type
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5715 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 307:
#line 3288 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5825 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 311:
#line 3404 "fe/idl.ypp" /* yacc.c:1646  */
    {
// case_branch : at_least_one_case_label
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
#line 5834 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 312:
#line 3409 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      annotations element_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
#line 5843 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 313:
#line 3414 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 5878 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 314:
#line 3445 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 5887 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 315:
#line 3450 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 5897 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 316:
#line 3459 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_case_label : case_label case_labels
          ACE_NEW_RETURN ((yyval.llval),
                          UTL_LabelList ((yyvsp[-1].ulval),
                                         (yyvsp[0].llval)),
                          1);
        }
#line 5909 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 317:
#line 3470 "fe/idl.ypp" /* yacc.c:1646  */
    {
// case_labels : case_labels annotations case_label
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
#line 5932 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 318:
#line 3489 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.llval) = 0;
        }
#line 5941 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 319:
#line 3497 "fe/idl.ypp" /* yacc.c:1646  */
    {
// case_label : IDL_DEFAULT
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
#line 5950 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 320:
#line 3502 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
#line 5964 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 321:
#line 3512 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_CASE
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
#line 5973 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 322:
#line 3517 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
#line 5981 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 323:
#line 3521 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      const_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      (yyvsp[-2].exval)
                                    );
        }
#line 5995 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 324:
#line 3534 "fe/idl.ypp" /* yacc.c:1646  */
    {
// element_spec : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
#line 6004 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 325:
#line 3539 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6053 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 326:
#line 3587 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6078 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 327:
#line 3611 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6103 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 328:
#line 3635 "fe/idl.ypp" /* yacc.c:1646  */
    {
// enum_type : IDL_ENUM
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
#line 6112 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 329:
#line 3640 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6149 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 330:
#line 3673 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
#line 6158 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 331:
#line 3678 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_enumerator
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
#line 6167 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 332:
#line 3683 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6191 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 334:
#line 3709 "fe/idl.ypp" /* yacc.c:1646  */
    {
// enumerators : enumerators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
#line 6200 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 337:
#line 3719 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6236 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 338:
#line 3755 "fe/idl.ypp" /* yacc.c:1646  */
    {
// sequence_type_spec : seq_head ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
#line 6245 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 339:
#line 3760 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
#line 6254 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 340:
#line 3765 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6335 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 341:
#line 3843 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6395 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 342:
#line 3902 "fe/idl.ypp" /* yacc.c:1646  */
    {
// seq_head : IDL_SEQUENCE
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
#line 6409 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 343:
#line 3912 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
#line 6418 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 344:
#line 3917 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      simple_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 6428 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 345:
#line 3926 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyvsp[-1].exval)->evaluate (AST_Expression::EK_positive_int);
          (yyval.dcval) = idl_global->gen ()->create_fixed ((yyvsp[-3].exval), (yyvsp[-1].exval));
        }
#line 6437 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 346:
#line 3935 "fe/idl.ypp" /* yacc.c:1646  */
    {
// string_type_spec : string_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 6446 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 347:
#line 3940 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 6455 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 348:
#line 3945 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6502 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 349:
#line 3988 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6529 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 350:
#line 4014 "fe/idl.ypp" /* yacc.c:1646  */
    {
// string_head : IDL_STRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 6538 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 351:
#line 4023 "fe/idl.ypp" /* yacc.c:1646  */
    {
// wstring_type_spec : wstring_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 6547 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 352:
#line 4028 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 6556 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 353:
#line 4033 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6592 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 354:
#line 4065 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6618 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 355:
#line 4090 "fe/idl.ypp" /* yacc.c:1646  */
    {
// wstring_head : IDL_WSTRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 6627 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 356:
#line 4098 "fe/idl.ypp" /* yacc.c:1646  */
    {
// array_declarator : defining_id
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
#line 6636 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 357:
#line 4103 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6674 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 358:
#line 4140 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_array_dim : array_dim array_dims
          ACE_NEW_RETURN ((yyval.elval),
                          UTL_ExprList ((yyvsp[-1].exval),
                                        (yyvsp[0].elval)),
                          1);
        }
#line 6686 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 359:
#line 4151 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6709 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 360:
#line 4170 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.elval) = 0;
        }
#line 6718 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 361:
#line 4178 "fe/idl.ypp" /* yacc.c:1646  */
    {
// array_dim : '['
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
#line 6727 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 362:
#line 4183 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
#line 6736 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 363:
#line 4188 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6795 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 366:
#line 4251 "fe/idl.ypp" /* yacc.c:1646  */
    {
// attribute_readonly : IDL_READONLY
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
#line 6804 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 367:
#line 4256 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 6813 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 368:
#line 4261 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 6822 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 369:
#line 4266 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 6831 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 370:
#line 4271 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 6896 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 371:
#line 4335 "fe/idl.ypp" /* yacc.c:1646  */
    {
// attribute_readwrite : IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 6905 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 372:
#line 4340 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 6914 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 373:
#line 4345 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 6923 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 374:
#line 4350 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      opt_getraises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
#line 6932 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 375:
#line 4355 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7006 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 376:
#line 4428 "fe/idl.ypp" /* yacc.c:1646  */
    {
// exception : IDL_EXCEPTION
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
#line 7015 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 377:
#line 4433 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7049 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 378:
#line 4463 "fe/idl.ypp" /* yacc.c:1646  */
    {
//       '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
#line 7058 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 379:
#line 4468 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      members
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
#line 7067 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 380:
#line 4473 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();
        }
#line 7080 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 381:
#line 4486 "fe/idl.ypp" /* yacc.c:1646  */
    {
// operation : opt_op_attribute op_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7089 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 382:
#line 4491 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7158 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 383:
#line 4556 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7167 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 384:
#line 4561 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      opt_raises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
#line 7176 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 385:
#line 4566 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7210 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 386:
#line 4599 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_op_attribute : IDL_ONEWAY
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_oneway;
        }
#line 7220 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 387:
#line 4605 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_IDEMPOTENT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_idempotent;
        }
#line 7230 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 388:
#line 4611 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.ofval) = AST_Operation::OP_noflags;
        }
#line 7239 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 390:
#line 4620 "fe/idl.ypp" /* yacc.c:1646  */
    {
// op_type_spec : param_type_spec | IDL_VOID
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
#line 7251 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 391:
#line 4631 "fe/idl.ypp" /* yacc.c:1646  */
    {
// init_decl : IDL_FACTORY
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7261 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 392:
#line 4637 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7307 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 393:
#line 4679 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7316 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 394:
#line 4684 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7334 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 395:
#line 4701 "fe/idl.ypp" /* yacc.c:1646  */
    {
// init_parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7343 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 396:
#line 4706 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7352 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 397:
#line 4711 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7361 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 398:
#line 4717 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_in_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7370 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 400:
#line 4728 "fe/idl.ypp" /* yacc.c:1646  */
    {
// in_parameters : in_parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7379 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 403:
#line 4738 "fe/idl.ypp" /* yacc.c:1646  */
    {
// in_parameter : IDL_IN
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7388 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 404:
#line 4743 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7397 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 405:
#line 4748 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7434 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 406:
#line 4784 "fe/idl.ypp" /* yacc.c:1646  */
    {
// parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7443 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 407:
#line 4789 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7452 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 408:
#line 4794 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7461 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 409:
#line 4800 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7470 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 411:
#line 4811 "fe/idl.ypp" /* yacc.c:1646  */
    {
// parameters : parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7479 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 414:
#line 4821 "fe/idl.ypp" /* yacc.c:1646  */
    {
// parameter : direction
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7488 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 415:
#line 4826 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7497 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 416:
#line 4831 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7541 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 417:
#line 4874 "fe/idl.ypp" /* yacc.c:1646  */
    {
// param_type_spec : base_type_spec
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 7553 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 420:
#line 4884 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7664 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 421:
#line 4994 "fe/idl.ypp" /* yacc.c:1646  */
    {
// direction : IDL_IN
          (yyval.dival) = AST_Argument::dir_IN;
        }
#line 7673 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 422:
#line 4999 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_OUT
          (yyval.dival) = AST_Argument::dir_OUT;
        }
#line 7682 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 423:
#line 5004 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | IDL_INOUT
          (yyval.dival) = AST_Argument::dir_INOUT;
        }
#line 7691 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 424:
#line 5012 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_raises : IDL_RAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
#line 7700 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 425:
#line 5017 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
#line 7709 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 426:
#line 5023 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7719 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 427:
#line 5029 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.nlval) = 0;
/*      |  EMPTY */
        }
#line 7728 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 428:
#line 5037 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_getraises : IDL_GETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
#line 7737 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 429:
#line 5042 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
#line 7746 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 430:
#line 5048 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7756 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 431:
#line 5054 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.nlval) = 0;
/*      |  EMPTY */
        }
#line 7765 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 432:
#line 5062 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_setraises : IDL_SETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
#line 7774 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 433:
#line 5067 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
#line 7783 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 434:
#line 5073 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7793 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 435:
#line 5079 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.nlval) = 0;
/*      |  EMPTY */
        }
#line 7802 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 436:
#line 5087 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_context : IDL_CONTEXT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
#line 7811 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 437:
#line 5092 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
//      '('
        }
#line 7820 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 438:
#line 5098 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      at_least_one_string_literal ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (yyval.slval) = (yyvsp[-1].slval);
        }
#line 7830 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 439:
#line 5104 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.slval) = 0;
        }
#line 7839 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 440:
#line 5112 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_string_literal : IDL_STRING_LITERAL string_literals
          ACE_NEW_RETURN ((yyval.slval),
                          UTL_StrList ((yyvsp[-1].sval),
                                       (yyvsp[0].slval)),
                          1);
        }
#line 7851 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 441:
#line 5124 "fe/idl.ypp" /* yacc.c:1646  */
    {
// string_literals : string_literals ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
#line 7860 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 442:
#line 5129 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7883 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 443:
#line 5148 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.slval) = 0;
        }
#line 7892 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 444:
#line 5156 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7922 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 445:
#line 5185 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7961 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 448:
#line 5229 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 7990 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 449:
#line 5257 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8034 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 450:
#line 5297 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
#line 8043 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 451:
#line 5302 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      component_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
#line 8052 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 452:
#line 5307 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 8066 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 453:
#line 5321 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      component_header: IDL_COMPONENT defining_id
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
#line 8075 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 454:
#line 5326 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      component_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 8084 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 455:
#line 5331 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8123 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 456:
#line 5369 "fe/idl.ypp" /* yacc.c:1646  */
    {
// component_inheritance_spec : ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 8132 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 457:
#line 5374 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      scoped_name
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8141 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 458:
#line 5379 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.idlist) = 0;
        }
#line 8150 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 461:
#line 5392 "fe/idl.ypp" /* yacc.c:1646  */
    {
// component_export : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 8159 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 462:
#line 5397 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8168 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 463:
#line 5402 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 8177 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 464:
#line 5407 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8186 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 465:
#line 5412 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | emits_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
        }
#line 8195 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 466:
#line 5417 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8204 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 467:
#line 5422 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | publishes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
        }
#line 8213 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 468:
#line 5427 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8222 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 469:
#line 5432 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | consumes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
        }
#line 8231 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 470:
#line 5437 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8240 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 471:
#line 5442 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 8249 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 472:
#line 5447 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8258 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 473:
#line 5452 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | extended_port_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 8267 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 474:
#line 5457 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8276 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 475:
#line 5464 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8368 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 476:
#line 5555 "fe/idl.ypp" /* yacc.c:1646  */
    {
// interface_type : scoped_name
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8379 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 477:
#line 5562 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8407 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 478:
#line 5588 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8515 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 479:
#line 5695 "fe/idl.ypp" /* yacc.c:1646  */
    {
// uses_opt_multiple : IDL_USES opt_multiple
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          (yyval.bval) = (yyvsp[0].bval);
        }
#line 8526 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 480:
#line 5705 "fe/idl.ypp" /* yacc.c:1646  */
    {
// opt_multiple : IDL_MULTIPLE
          (yyval.bval) = true;
        }
#line 8535 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 481:
#line 5710 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.bval) = false;
        }
#line 8544 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 482:
#line 5717 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8619 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 483:
#line 5790 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8694 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 484:
#line 5863 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8769 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 485:
#line 5937 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8811 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 486:
#line 5975 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      home_body
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 8823 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 487:
#line 5986 "fe/idl.ypp" /* yacc.c:1646  */
    {
// home_header : IDL_HOME
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
#line 8832 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 488:
#line 5991 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      defining_id
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
#line 8841 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 489:
#line 5996 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      home_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 8850 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 490:
#line 6001 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
#line 8859 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 491:
#line 6006 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      IDL_MANAGES
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
#line 8868 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 492:
#line 6011 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      scoped_name
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
#line 8877 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 493:
#line 6016 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 8928 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 494:
#line 6066 "fe/idl.ypp" /* yacc.c:1646  */
    {
// home_inheritance_spec ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 8937 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 495:
#line 6071 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      scoped_name
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8946 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 496:
#line 6076 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.idlist) = 0;
        }
#line 8955 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 497:
#line 6085 "fe/idl.ypp" /* yacc.c:1646  */
    {
// primary_key_spec : IDL_PRIMARYKEY scoped_name
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8964 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 498:
#line 6090 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |  EMPTY */
          (yyval.idlist) = 0;
        }
#line 8973 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 499:
#line 6098 "fe/idl.ypp" /* yacc.c:1646  */
    {
// home_body : '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
#line 8982 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 500:
#line 6103 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      home_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
#line 8991 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 501:
#line 6108 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
#line 9000 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 505:
#line 6122 "fe/idl.ypp" /* yacc.c:1646  */
    {
// home_export : factory_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
#line 9009 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 506:
#line 6127 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9018 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 507:
#line 6132 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | finder_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
#line 9027 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 508:
#line 6137 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9036 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 509:
#line 6146 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9064 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 510:
#line 6170 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 9073 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 511:
#line 6175 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9096 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 512:
#line 6198 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9128 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 513:
#line 6226 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 9137 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 514:
#line 6231 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9160 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 520:
#line 6265 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9188 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 521:
#line 6294 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9216 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 522:
#line 6322 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9265 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 523:
#line 6367 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9274 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 524:
#line 6372 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9283 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 525:
#line 6377 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9297 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 526:
#line 6392 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_abs_header : IDL_ABSTRACT IDL_EVENTTYPE defining_id
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9306 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 527:
#line 6402 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9324 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 528:
#line 6420 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_plain_header : IDL_EVENTTYPE defining_id
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9335 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 529:
#line 6430 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_rest_of_header : inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 9344 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 530:
#line 6435 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9378 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 531:
#line 6469 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9432 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 532:
#line 6519 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9441 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 533:
#line 6524 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9450 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 534:
#line 6529 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9464 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 535:
#line 6542 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_header : event_custom_header
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9473 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 536:
#line 6547 "fe/idl.ypp" /* yacc.c:1646  */
    {
// event_header : event_plain_header
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9482 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 537:
#line 6555 "fe/idl.ypp" /* yacc.c:1646  */
    {
// formal_parameter_type : IDL_TYPENAME
          (yyval.ntval) = AST_Decl::NT_type;
        }
#line 9491 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 538:
#line 6560 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_STRUCT
          (yyval.ntval) = AST_Decl::NT_struct;
        }
#line 9500 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 539:
#line 6565 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_UNION
          (yyval.ntval) = AST_Decl::NT_union;
        }
#line 9509 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 540:
#line 6570 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_EVENTTYPE
          (yyval.ntval) = AST_Decl::NT_eventtype;
        }
#line 9518 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 541:
#line 6575 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_SEQUENCE
          (yyval.ntval) = AST_Decl::NT_sequence;
        }
#line 9527 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 542:
#line 6580 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_INTERFACE
          (yyval.ntval) = AST_Decl::NT_interface;
        }
#line 9536 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 543:
#line 6585 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_VALUETYPE
          (yyval.ntval) = AST_Decl::NT_valuetype;
        }
#line 9545 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 544:
#line 6590 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_ENUM
          (yyval.ntval) = AST_Decl::NT_enum;
        }
#line 9554 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 545:
#line 6595 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_EXCEPTION
          (yyval.ntval) = AST_Decl::NT_except;
        }
#line 9563 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 546:
#line 6600 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        IDL_CONST const_type
          (yyval.ntval) = AST_Decl::NT_const;
          t_param_const_type = (yyvsp[0].etval);
        }
#line 9573 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 547:
#line 6609 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9608 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 548:
#line 6643 "fe/idl.ypp" /* yacc.c:1646  */
    {
// formal_parameters : formal_parameters ','
          // Maybe add a new parse state to set here.
        }
#line 9617 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 549:
#line 6648 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9637 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 550:
#line 6664 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        /* EMPTY */
          (yyval.plval) = 0;
        }
#line 9646 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 551:
#line 6672 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9676 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 552:
#line 6698 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9695 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 553:
#line 6716 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_formal_parameter_name : formal_parameter_name formal_parameter_names
          ACE_NEW_RETURN ((yyval.slval),
                          UTL_StrList ((yyvsp[-1].sval),
                                       (yyvsp[0].slval)),
                          1);
        }
#line 9707 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 554:
#line 6727 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9730 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 555:
#line 6746 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        /* EMPTY */
          (yyval.slval) = 0;
        }
#line 9739 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 556:
#line 6754 "fe/idl.ypp" /* yacc.c:1646  */
    {
// formal_parameter_name : IDENTIFIER
          ACE_NEW_RETURN ((yyval.sval),
                          UTL_String ((yyvsp[0].strval), true),
                          1);
        }
#line 9750 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 557:
#line 6764 "fe/idl.ypp" /* yacc.c:1646  */
    {
// porttype_decl : IDL_PORTTYPE
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSeen);
        }
#line 9759 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 558:
#line 6769 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9782 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 559:
#line 6788 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSqSeen);
        }
#line 9791 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 560:
#line 6793 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        at_least_one_port_export
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeBodySeen);
        }
#line 9800 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 561:
#line 6798 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeQsSeen);

          // Done with this port type - pop it off the scopes stack.
          idl_global->scopes ().pop ();
        }
#line 9812 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 562:
#line 6809 "fe/idl.ypp" /* yacc.c:1646  */
    {
// at_least_one_port_export : port_export port_exports
        }
#line 9820 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 563:
#line 6816 "fe/idl.ypp" /* yacc.c:1646  */
    {
// port_exports : port_exports port_export
        }
#line 9828 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 564:
#line 6820 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        | /* EMPTY */
        }
#line 9836 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 565:
#line 6827 "fe/idl.ypp" /* yacc.c:1646  */
    {
// port_export : provides_decl
        }
#line 9844 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 566:
#line 6831 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
        }
#line 9852 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 567:
#line 6835 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        | uses_decl
        }
#line 9860 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 568:
#line 6839 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
        }
#line 9868 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 569:
#line 6843 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 9877 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 570:
#line 6848 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
        }
#line 9885 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 571:
#line 6855 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9959 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 572:
#line 6925 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 10011 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 573:
#line 6976 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 10028 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 574:
#line 6992 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 10045 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 575:
#line 7005 "fe/idl.ypp" /* yacc.c:1646  */
    {
//         | /* EMPTY */
          (yyval.alval) = 0;
        }
#line 10054 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 576:
#line 7013 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 10115 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 578:
#line 7077 "fe/idl.ypp" /* yacc.c:1646  */
    {
// connector_header : IDL_CONNECTOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSeen);
        }
#line 10124 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 579:
#line 7082 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        annotations IDENTIFIER
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorIDSeen);
        }
#line 10133 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 580:
#line 7087 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 10190 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 581:
#line 7143 "fe/idl.ypp" /* yacc.c:1646  */
    {
// connector_body " '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSqSeen);
        }
#line 10199 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 582:
#line 7148 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        connector_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorBodySeen);
        }
#line 10208 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 583:
#line 7153 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        '}
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorQsSeen);

          // Done with this connector - pop it off the scope stack.
          idl_global->scopes ().pop ();
        }
#line 10220 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 586:
#line 7169 "fe/idl.ypp" /* yacc.c:1646  */
    {
// connector_export : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 10229 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 587:
#line 7174 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10238 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 588:
#line 7179 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        | uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 10247 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 589:
#line 7184 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10256 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 590:
#line 7189 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 10265 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 591:
#line 7194 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10274 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 592:
#line 7199 "fe/idl.ypp" /* yacc.c:1646  */
    {
//      | extended_port_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 10283 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 593:
#line 7204 "fe/idl.ypp" /* yacc.c:1646  */
    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10292 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;


#line 10296 "fe/idl.tab.cpp" /* yacc.c:1646  */
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
#line 7210 "fe/idl.ypp" /* yacc.c:1906  */

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
