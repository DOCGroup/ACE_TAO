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
#line 72 "fe/idl.ypp" /* yacc.c:339  */

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
#include "ast_union_branch.h"
#include "ast_enum_val.h"

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
// Compile Optional Tracing Output for Parser, can be enabled with --bison-trace
#define YYDEBUG 1

#line 154 "fe/idl.tab.cpp" /* yacc.c:339  */

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
#line 156 "fe/idl.ypp" /* yacc.c:355  */

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
  AST_Annotation *annotation;
  AST_Annotation::Params *annotation_params;
  AST_Annotation::Param *annotation_param;
  Annotations *annotations;
  AST_Decls *ast_decls;

#line 325 "fe/idl.tab.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE tao_yylval;

int tao_yyparse (void);

#endif /* !YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 342 "fe/idl.tab.cpp" /* yacc.c:358  */

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
#define YYLAST   1702

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  110
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  399
/* YYNRULES -- Number of rules.  */
#define YYNRULES  595
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  872

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
       0,   381,   381,   388,   393,   396,   400,   404,   408,   415,
     420,   419,   429,   428,   438,   437,   447,   446,   456,   455,
     465,   464,   474,   473,   483,   482,   492,   491,   501,   500,
     510,   509,   519,   518,   528,   527,   537,   536,   546,   545,
     559,   558,   570,   609,   613,   569,   629,   637,   651,   661,
     691,   695,   636,   718,   722,   723,   727,   728,   733,   738,
     732,   824,   829,   823,   898,   899,   904,   942,   946,   903,
     962,   961,   973,  1010,  1040,  1073,  1072,  1081,  1088,  1089,
    1090,  1091,  1095,  1105,  1110,  1153,  1157,  1109,  1184,  1227,
    1231,  1182,  1248,  1246,  1286,  1285,  1297,  1301,  1308,  1313,
    1321,  1346,  1372,  1436,  1437,  1441,  1442,  1443,  1449,  1448,
    1457,  1456,  1467,  1468,  1473,  1472,  1481,  1480,  1489,  1488,
    1497,  1496,  1505,  1504,  1513,  1512,  1521,  1520,  1529,  1528,
    1540,  1552,  1550,  1575,  1589,  1599,  1598,  1624,  1622,  1647,
    1657,  1668,  1712,  1739,  1769,  1773,  1777,  1781,  1768,  1843,
    1844,  1845,  1846,  1847,  1848,  1849,  1853,  1857,  1925,  1927,
    1929,  1930,  1942,  1943,  1955,  1956,  1968,  1969,  1978,  1990,
    1991,  2000,  2012,  2013,  2022,  2031,  2043,  2044,  2053,  2062,
    2074,  2124,  2125,  2132,  2136,  2141,  2148,  2152,  2156,  2161,
    2165,  2169,  2173,  2180,  2248,  2275,  2276,  2280,  2281,  2282,
    2286,  2290,  2291,  2295,  2299,  2300,  2304,  2313,  2319,  2326,
    2346,  2350,  2357,  2366,  2373,  2381,  2387,  2394,  2405,  2404,
    2413,  2417,  2421,  2425,  2451,  2459,  2458,  2528,  2529,  2533,
    2540,  2541,  2567,  2568,  2569,  2570,  2571,  2572,  2573,  2574,
    2578,  2579,  2580,  2581,  2585,  2586,  2587,  2591,  2592,  2596,
    2608,  2606,  2631,  2638,  2639,  2643,  2655,  2653,  2678,  2685,
    2701,  2719,  2720,  2724,  2728,  2732,  2739,  2743,  2747,  2754,
    2758,  2762,  2769,  2776,  2780,  2787,  2794,  2801,  2808,  2816,
    2815,  2829,  2860,  2864,  2828,  2881,  2884,  2885,  2889,  2889,
    2908,  2912,  2907,  2976,  2975,  2988,  2987,  3000,  3004,  3037,
    3041,  3097,  3101,  2999,  3123,  3130,  3143,  3152,  3159,  3160,
    3266,  3269,  3270,  3275,  3279,  3274,  3312,  3311,  3323,  3333,
    3351,  3359,  3358,  3372,  3376,  3371,  3392,  3391,  3442,  3467,
    3491,  3495,  3526,  3530,  3490,  3554,  3559,  3557,  3563,  3567,
    3603,  3607,  3601,  3686,  3746,  3755,  3745,  3766,  3776,  3780,
    3774,  3825,  3850,  3859,  3863,  3857,  3897,  3921,  3929,  3928,
    3968,  3978,  3996,  4004,  4008,  4003,  4068,  4069,  4074,  4078,
    4082,  4086,  4073,  4153,  4157,  4161,  4165,  4152,  4241,  4245,
    4274,  4278,  4240,  4294,  4298,  4362,  4366,  4292,  4401,  4406,
    4411,  4418,  4419,  4430,  4435,  4476,  4429,  4496,  4495,  4504,
    4503,  4514,  4519,  4517,  4523,  4528,  4532,  4527,  4571,  4570,
    4579,  4578,  4589,  4594,  4592,  4598,  4603,  4607,  4602,  4652,
    4659,  4660,  4661,  4768,  4772,  4776,  4784,  4788,  4783,  4797,
    4805,  4809,  4804,  4818,  4826,  4830,  4825,  4839,  4847,  4851,
    4846,  4860,  4867,  4879,  4877,  4900,  4907,  4935,  4972,  4973,
    4977,  5006,  5045,  5049,  5005,  5066,  5070,  5064,  5111,  5110,
    5118,  5125,  5126,  5131,  5130,  5139,  5138,  5147,  5146,  5155,
    5154,  5163,  5162,  5171,  5170,  5179,  5178,  5188,  5278,  5284,
    5309,  5415,  5424,  5428,  5434,  5506,  5578,  5652,  5651,  5699,
    5703,  5707,  5711,  5715,  5719,  5698,  5772,  5771,  5779,  5786,
    5791,  5799,  5803,  5798,  5813,  5814,  5818,  5820,  5819,  5828,
    5827,  5840,  5863,  5838,  5889,  5916,  5887,  5940,  5941,  5942,
    5946,  5947,  5951,  5978,  6008,  6052,  6056,  6006,  6071,  6080,
    6098,  6109,  6108,  6146,  6195,  6199,  6144,  6214,  6218,  6225,
    6229,  6233,  6237,  6241,  6245,  6249,  6253,  6257,  6261,  6269,
    6303,  6302,  6321,  6328,  6353,  6371,  6381,  6399,  6406,  6416,
    6420,  6438,  6442,  6415,  6455,  6461,  6464,  6471,  6470,  6477,
    6476,  6483,  6482,  6492,  6561,  6611,  6626,  6638,  6645,  6704,
    6709,  6713,  6708,  6772,  6776,  6771,  6789,  6790,  6795,  6794,
    6803,  6802,  6811,  6810,  6819,  6818
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
  "$@14", "$@15", "module_header", "$@16", "module", "@17", "$@18", "$@19",
  "template_module_header", "template_module", "$@20", "$@21", "$@22",
  "$@23", "$@24", "at_least_one_tpl_definition", "tpl_definitions",
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
  "annotation_member", "at_least_one_annotation", "annotations_maybe",
  "annotation_appl", "annotation_appl_params_maybe",
  "annotation_appl_params", "named_annotation_appl_params",
  "more_named_annotation_appl_params", "named_annotation_appl_param",
  "type_dcl", "$@59", "type_declarator", "$@60", "type_spec",
  "simple_type_spec", "base_type_spec", "template_type_spec",
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

#define YYPACT_NINF -639

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-639)))

#define YYTABLE_NINF -536

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -639,    55,  1293,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,    73,    98,    97,   172,  -639,    73,    73,
    -639,    66,    66,  -639,  -639,   104,  -639,  -639,    21,  -639,
     276,    22,    53,  -639,  -639,    75,  -639,  -639,  -639,  -639,
    -639,  -639,   624,  -639,  -639,  -639,  -639,  -639,  1477,    51,
    -639,  -639,    92,  -639,   136,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,    68,  -639,  -639,  -639,    68,  -639,  -639,
      81,   117,   512,    66,    73,  1586,    73,    73,    73,    73,
    -639,  -639,  -639,    91,    73,    93,  -639,   113,    73,  -639,
      68,    73,   120,   141,    73,  -639,  -639,    19,  -639,    24,
    -639,  -639,   163,  -639,   181,   190,   649,  -639,  -639,  -639,
     197,   242,  -639,   206,   214,   209,    82,  -639,   174,  -639,
    -639,  -639,  -639,  -639,  -639,   218,  -639,  -639,  -639,  -639,
    -639,  -639,    86,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
     136,  -639,  -639,  -639,   157,  -639,  -639,   221,  -639,   225,
     231,   235,  -639,    66,   237,   238,   239,  -639,   246,   247,
     248,   249,   250,   252,   256,   259,  -639,  -639,  -639,   260,
     261,  -639,  -639,  -639,  -639,    86,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,    86,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,  -639,   262,  -639,   263,  -639,  -639,   234,
    -639,   335,  -639,  -639,  -639,    44,    47,  -639,  -639,  -639,
     512,  -639,  -639,  -639,  -639,   264,  -639,  -639,  -639,  -639,
     348,  -639,  -639,    40,   265,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,   345,  -639,   119,   269,   314,  -639,  -639,
    -639,  -639,  -639,  -639,   -14,  -639,  -639,   257,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,  -639,  -639,   314,   274,   275,
    -639,  -639,  -639,    73,    73,   279,   281,  -639,  -639,  -639,
     280,  -639,   335,  -639,  -639,  -639,   571,  -639,  -639,   364,
    -639,   282,   277,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,   184,   184,   184,   119,    86,  -639,  -639,
     284,   285,   283,   131,   135,    88,  -639,  -639,  -639,  -639,
    -639,    66,  -639,  -639,  -639,  -639,   289,  -639,    66,  -639,
     119,   119,   119,   119,  -639,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,   142,  -639,   267,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,    66,   314,  -639,  -639,  -639,   287,   234,
    -639,  -639,  -639,   298,   270,    73,  -639,  -639,  -639,  -639,
    1346,  -639,   295,  -639,  -639,  -639,  -639,  -639,   266,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   294,
      73,  -639,    86,  1075,   823,   119,   881,  -639,  -639,  -639,
    -639,   299,  -639,   290,  -639,   296,  -639,  -639,  -639,  -639,
     253,   991,  -639,  -639,  -639,  -639,    67,   331,    66,    66,
    -639,  -639,  -639,  -639,  -639,    67,  -639,   303,  -639,   304,
     293,   308,  -639,  -639,  1110,    86,  -639,    66,   314,  -639,
    -639,  -639,   399,  -639,   316,   320,   401,   322,   649,  -639,
     285,   283,   131,   135,   135,    88,    88,  -639,  -639,  -639,
    -639,  -639,   319,  -639,  -639,  -639,   325,  -639,  -639,  -639,
    -639,  -639,  -639,  1621,  -639,  -639,  -639,  -639,  -639,   326,
    -639,  -639,  -639,   333,   330,  -639,   332,   336,   337,   119,
    -639,   335,  -639,   342,   343,  1426,   278,    66,    66,    66,
    -639,   344,  -639,  -639,  -639,  -639,  -639,  -639,  -639,    73,
      73,  -639,   358,  -639,  -639,  -639,  1194,   907,   392,   428,
    -639,    86,   335,  -639,  -639,    70,    78,   339,   347,   361,
     335,   368,  -639,  -639,  -639,  -639,  -639,  -639,    48,  -639,
    -639,   367,   369,    86,  -639,   185,   512,  -639,  -639,  -639,
    -639,    40,  -639,   370,  -639,   372,   373,   376,   379,   381,
    -639,    86,  -639,  -639,  -639,  -639,  -639,   382,   383,  -639,
    1567,  1567,  -639,  -639,  -639,  -639,  -639,  -639,  -639,   299,
     296,  -639,  -639,  -639,  -639,  -639,   417,    86,  -639,  -639,
    -639,  -639,  -639,  -639,    79,    79,    79,   384,  -639,   385,
     387,   389,   391,   393,   395,  -639,  -639,  -639,   403,   404,
     405,   406,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,   119,   119,   293,    73,  -639,   407,   400,   415,
    -639,   423,  -639,  -639,  -639,  -639,  -639,  -639,  1452,  -639,
      66,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
      49,  -639,  -639,  -639,   418,    73,   408,  -639,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,  -639,  -639,   402,   402,  -639,
    -639,  -639,  -639,   428,    73,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,   424,   425,   429,   426,   185,    66,  -639,
    -639,  -639,  -639,   430,    86,    50,  -639,  -639,  -639,  -639,
     484,  -639,  -639,  -639,  -639,  -639,   422,  -639,    66,  -639,
    -639,  -639,  -639,  -639,    86,   442,  1239,  -639,  -639,   402,
     445,   447,   433,   499,   504,   504,    73,   490,   452,  -639,
    -639,    86,   456,  -639,  -639,   443,  -639,  -639,  -639,  -639,
    -639,   449,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
     497,   554,   455,  -639,   504,   101,   428,  -639,   464,   457,
     504,   458,   502,    73,    66,  -639,  -639,   473,  -639,   460,
     191,   504,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,    86,  -639,
     474,  -639,  -639,  -639,  -639,   461,  -639,  -639,  -639,   481,
     119,   477,   482,    43,  -639,   210,    73,   499,    66,    66,
     468,    73,   554,  -639,   483,   428,   537,  -639,  -639,  -639,
    -639,  -639,  1541,  -639,  -639,  -639,   489,   492,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,   506,  -639,  -639,  -639,  -639,
      66,   191,    73,   495,  -639,    73,   513,   496,  -639,  -639,
    -639,  -639,  -639,  -639,   528,  -639,   498,   511,  -639,  -639,
     531,  -639
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       5,     0,     0,     1,    38,   144,    40,    70,   218,   279,
     295,   330,   378,     0,     0,     0,     0,    94,     0,     0,
     489,     0,     0,   559,   580,     0,     4,     6,    42,    24,
      61,     0,     0,    22,    64,    77,    66,    26,    78,    83,
      79,    84,    77,    80,    81,    65,    18,    10,     0,     0,
      12,   224,   281,   220,   329,   221,   247,   248,   222,    20,
      14,    16,    28,   449,   448,   451,    30,   487,    32,   519,
     521,   520,   518,    77,   537,   538,   517,    77,    34,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     140,   259,   223,    77,     0,    77,    88,    77,     0,    82,
      77,     0,   455,   530,     0,   139,   135,     0,   134,     0,
     208,   208,     0,    46,     0,     0,     0,   208,     7,     8,
       0,    97,    72,     0,     0,     0,   263,   265,     0,   269,
     270,   273,   274,   275,   276,   272,   277,   278,   344,   352,
     357,    92,   231,   102,   227,   229,   230,   228,   232,   261,
     262,   233,   237,   234,   236,   235,   238,   239,   281,   244,
       0,   245,   246,   240,     0,   243,   241,   351,   242,   356,
       0,     0,     3,     0,   206,     0,     0,   297,     0,     0,
       0,     0,     0,     0,     0,     0,   531,   524,   533,     0,
       0,   583,   579,    39,   272,   157,   145,   149,   153,   154,
     150,   151,   152,   155,   156,    41,    71,   219,   225,   280,
     296,   331,   379,    73,   528,    74,     0,   529,    95,   460,
     490,     0,   446,   137,   447,     0,     0,   196,    43,    25,
       0,   544,   540,   541,   546,   543,   547,   545,   542,   539,
       0,    48,   552,     0,     0,    23,    96,    75,    67,    27,
      85,   264,   271,   266,   268,     0,     0,    99,   343,   340,
     348,   353,    19,    11,   211,    13,   282,     0,    21,    15,
      17,    29,   452,    31,   501,   488,    33,    99,     0,     0,
      35,    37,   587,     0,     0,     0,     0,    89,   458,   456,
     498,   136,     0,   560,   207,   581,     0,     5,   548,     0,
     553,     0,   549,   183,   184,   185,   187,   190,   189,   191,
     192,   188,   186,     0,     0,     0,     0,   180,   578,   158,
     159,   160,   162,   164,   166,   169,   172,   176,   181,   577,
      62,     0,   113,   104,   267,   193,     0,   345,     0,    93,
       0,     0,     0,     0,   209,   288,   298,   462,   505,   532,
     525,   534,   584,   146,   259,   226,   252,   253,   254,   260,
     332,   380,   113,     0,    99,   496,   491,   138,     0,   460,
     194,   198,   201,     0,   204,     0,   205,   197,   199,   202,
       0,    44,     0,    49,   550,   177,   178,   179,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   575,
       0,    76,   133,     0,     0,     0,     0,    98,   341,   349,
     354,   134,   212,     0,   213,   216,   283,   287,   208,   208,
     453,     0,   113,   104,   368,   373,     0,   483,     0,     0,
     592,   366,   367,   588,   590,     0,   594,     0,   586,     0,
       0,   249,   208,   287,     0,   459,   457,     0,    99,   561,
     582,   195,     0,   200,     4,     0,     0,     0,     0,   182,
     161,   163,   165,   167,   168,   170,   171,   173,   174,   175,
     208,    63,   130,   128,   388,   389,     0,   112,   120,   114,
     124,   122,   126,     0,   116,   118,   393,   110,   108,     0,
     103,   105,   106,     0,     0,   346,     0,     0,     0,     0,
     210,     0,   214,     0,   288,     0,     0,     0,     0,     0,
     473,     0,   461,   463,   465,   467,   469,   471,   475,     0,
       0,   506,     0,   504,   507,   509,     0,     0,     0,     0,
     479,   478,     0,   482,   481,     0,     0,     0,     0,     0,
       0,     0,   585,   147,   363,   359,   362,   250,     0,   333,
     338,   288,     0,   497,   492,     0,     0,    45,   554,    50,
     551,     0,   131,     0,    69,     0,     0,     0,     0,     0,
     392,   422,   419,   420,   421,   383,   391,     0,     0,   208,
       0,     0,    87,   107,   347,   342,   350,   355,   217,     0,
     216,   284,   286,   293,   290,   289,   263,   309,   304,   305,
     306,   307,   299,   308,     0,     0,     0,     0,   454,     0,
       0,     0,     0,     0,     0,   511,   514,   503,     0,     0,
       0,     0,   369,   374,   477,   573,   574,   593,   589,   591,
     480,   595,     0,     0,   360,     0,   339,     0,   335,     0,
      91,     0,   571,   567,   569,   562,   566,   203,     0,   576,
       0,   129,   121,   115,   125,   123,   127,   208,   117,   119,
       0,   111,   109,   215,     0,     0,     0,   486,   484,   485,
     474,   464,   466,   468,   470,   472,   476,     0,     0,   508,
     510,   527,   536,     0,     0,   148,   364,   361,   251,   334,
     336,   382,   493,     0,     0,     0,     0,   564,     0,    56,
      42,    51,    55,     0,   132,     0,   394,   294,   291,   300,
     397,   512,   515,   370,   375,   258,     0,   208,     0,   572,
     568,   570,   563,   565,    58,     0,     0,    57,   384,     0,
       0,     0,     0,     0,   429,   429,     0,   433,   255,   365,
     337,   494,     0,    52,    54,     0,   395,   292,   301,   398,
     405,     0,   404,   426,   513,   516,   371,   430,   376,   256,
     500,     0,   410,   385,   429,     0,     0,   400,   401,     0,
     429,     0,   437,     0,     0,   495,   558,     0,   557,     0,
       0,   429,   396,   316,   323,   321,   302,   312,   313,   320,
     406,   402,   427,   372,   431,   434,   377,   257,   499,    59,
     555,   409,   423,   424,   425,     0,   415,   416,   386,     0,
       0,     0,     0,     0,   208,   318,     0,     0,     0,     0,
       0,     0,     0,   411,   412,     0,   441,   317,   324,   322,
     303,   311,     0,   319,   407,   403,     0,     0,   435,    60,
     556,   413,   417,   438,   387,     0,   326,   314,   428,   432,
       0,     0,     0,     0,   325,     0,     0,     0,   414,   418,
     439,   327,   315,   436,     0,   445,     0,   442,   440,   443,
       0,   444
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -639,  -639,   310,   -36,  -639,  -564,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
    -639,  -560,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,  -639,  -116,  -639,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,   240,  -639,
    -639,    76,  -639,  -639,  -639,   592,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,   597,  -639,   251,  -639,  -639,  -236,  -639,
    -639,   192,  -639,  -639,  -639,  -639,  -313,  -362,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,  -639,  -320,  -639,  -639,   -21,
    -639,  -639,  -186,    -9,  -639,    12,  -639,  -639,  -639,  -639,
    -198,   -19,  -220,  -639,   227,   224,   228,  -120,  -118,  -165,
     -67,  -639,  -302,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
    -639,   -86,   569,  -639,  -639,  -639,    30,   121,    15,  -639,
    -639,  -639,   -39,   215,  -446,  -639,  -639,  -639,   -42,  -639,
    -639,  -599,  -112,  -639,  -639,    -6,  -639,   -76,  -639,  -639,
     -53,   -52,   -56,   -55,   -54,   329,  -639,   -37,  -639,   -31,
    -639,  -639,  -639,  -639,   183,   301,  -639,  -274,  -639,  -639,
    -639,   -29,  -639,   -27,  -639,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,  -185,  -639,  -639,  -639,  -639,  -639,  -164,
    -639,  -639,  -639,  -639,  -639,  -639,  -639,   -40,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,   -87,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,   -74,  -639,  -639,  -639,   -66,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,    23,  -639,  -639,  -319,  -639,
    -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,    17,
    -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,  -621,  -639,  -639,  -639,  -639,
    -639,  -163,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
    -195,  -639,  -639,  -495,  -639,  -638,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
      18,    20,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
    -639,   302,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,  -278,   223,  -265,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,  -639,   582,  -639,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,  -639,  -639,   216,  -639,  -639,
    -158,  -639,  -639,  -639,  -639,  -639,  -639,  -639,   -30,  -639,
    -639,  -639,   268,  -639,  -639,   118,  -639,  -639,  -639,  -639,
    -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639,  -639
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    26,   381,    27,   171,   175,   179,   180,
     170,   178,   120,   115,   124,   181,   183,   185,   189,   190,
      81,    28,    83,    29,   114,   297,   455,    30,    31,   116,
     301,   457,   648,   725,   701,   726,   702,   703,   742,   821,
      32,   117,   400,    33,    34,   123,   332,   476,    35,    84,
      36,   141,   331,    37,    38,    39,   125,   333,   489,    40,
     216,   362,   552,    41,   257,    42,   101,   247,   339,    43,
      44,   404,   490,   491,   581,   580,   403,   477,   566,   577,
     578,   565,   568,   567,   569,   563,   401,   472,   650,   317,
     221,   292,   108,   354,    45,   478,    82,   283,   439,   632,
     196,   318,   335,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   336,    47,   296,   373,   374,   375,   376,   377,
      48,    49,   294,   344,   413,   414,   502,   415,   479,    85,
     207,   284,   594,   144,   145,   146,   147,    51,   355,   441,
     635,   356,   714,   738,   773,   357,   358,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    52,    86,    53,
     176,   345,   503,   416,   504,   592,   418,   595,   665,   730,
     664,    54,    87,    55,   267,   419,   666,   731,   765,   812,
     602,   786,   813,   787,   814,   856,   809,   788,   815,   789,
     811,   810,   845,   847,   855,    56,    57,    58,    88,   285,
     442,   637,   549,   638,   717,   550,   163,   340,   496,   164,
     256,   406,   165,   166,   341,   497,   167,   168,   342,   498,
     169,   359,   440,   545,   634,   546,   633,   716,   480,   431,
     528,   683,   736,   770,   432,   529,   684,   737,   772,   481,
      89,   286,   443,   639,   482,   657,   745,   781,   826,   483,
     575,   493,   579,   729,   764,   711,   732,   733,   751,   768,
     817,   752,   766,   816,   763,   779,   780,   805,   824,   851,
     806,   825,   852,   576,   807,   754,   769,   818,   758,   771,
     819,   796,   820,   850,   844,   853,   864,   866,   867,   870,
     484,   485,    62,    63,    64,   182,   347,   511,    65,   219,
     364,   289,   363,   420,   512,   609,   610,   611,   612,   613,
     607,   614,   643,   532,   644,   435,   534,   515,   516,   517,
      66,   184,    67,   104,   290,   448,   641,   718,   760,   366,
     447,   775,   275,   348,   522,   421,   523,   618,   619,   524,
     677,   734,   525,   678,   735,    68,    69,    70,    71,    72,
     278,   422,   620,    73,    74,    75,   187,   277,    76,   279,
     423,   621,    77,   240,   241,   302,   458,   242,   777,   800,
     778,    78,   110,   368,   555,   696,   645,   697,   646,   694,
     695,   693,   436,   244,   399,   329,    79,    80,   111,   369,
     192,   282,   437,   352,   438,   538,   539,   537,   541
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     107,   109,   162,   143,    91,   158,   197,    92,   203,   102,
     103,   159,   172,   160,    46,   161,   204,    50,   407,    59,
      60,   142,    61,   319,   225,   226,   200,   201,   202,   198,
     199,   243,   298,   430,   623,   291,   688,   572,   408,   409,
     410,   349,   492,   105,   783,   162,   208,   293,   158,   444,
     295,   636,   706,   728,   159,     3,   160,   712,   161,   521,
      46,   195,   205,    50,   142,    59,    60,   223,    61,   105,
     105,   784,   785,   625,   433,   206,    90,   209,   210,   211,
     212,   626,   105,   572,   699,   214,   530,   434,   700,   217,
     251,   343,   218,   252,   222,   220,   388,   755,   372,   224,
     223,   510,   783,     7,     7,   223,   367,   112,   746,   526,
     118,   122,   113,   303,   304,   305,   306,   307,   308,   309,
     310,   106,   105,   412,   311,   312,   782,   173,   446,   784,
     785,   173,   793,  -310,   173,   173,   173,   173,   173,   313,
     314,   119,   513,   808,   315,   316,    17,   106,   106,   186,
      94,   223,   264,   186,   197,   514,   203,   411,   177,   223,
     223,   121,   699,  -141,   204,   492,   700,   223,   121,   213,
     191,   215,   424,   425,   200,   201,   202,   198,   199,  -142,
    -328,  -143,   253,   254,   121,   494,   121,   105,   713,   396,
     397,   398,   303,   304,   305,   306,   307,   308,   309,   310,
     106,  -100,   426,   311,   312,   193,   121,   427,  -450,   195,
     428,   429,   554,   392,   393,   424,   425,   834,   313,   314,
     197,    17,   203,   315,   316,    98,   802,   803,   804,  -522,
     204,   467,   468,   469,   394,   395,   642,   572,   784,   785,
     200,   201,   202,   198,   199,   426,   385,   386,   387,   258,
     427,   259,   227,   859,    93,    95,   861,   303,   304,   305,
     306,   307,   308,   309,   310,   106,    97,   100,   311,   312,
     228,   790,   463,   464,   353,   195,   465,   466,   229,   588,
     -47,   105,   -47,   424,   425,   245,   596,   127,   128,   316,
     246,   131,   132,   133,   134,   248,   -47,   -47,   250,   -47,
     -47,    11,   249,   -47,   507,   508,   661,   662,   371,   255,
     402,   378,   260,   426,   509,   589,   261,   402,   427,   262,
     572,   428,   429,   263,  -207,   -47,   265,   288,   266,   -47,
     842,   686,   505,   506,   268,   269,   270,   271,   105,   272,
     273,   319,   445,   -47,   454,   274,   624,   276,   280,   281,
    -523,   300,   287,   334,   630,   299,   548,   330,   647,   106,
     337,   338,   346,   350,   351,   173,    91,   382,   360,   453,
     361,   384,   459,   365,   383,  -358,   449,   452,   642,   572,
     389,   391,   390,   405,   561,   142,   451,   456,   470,   533,
     501,   471,    46,   542,   499,    50,   500,    59,    60,   543,
      61,   544,   547,   556,   558,   531,    -9,   535,   536,   573,
     557,   559,   319,   562,   531,   564,   582,   574,   667,   668,
     669,   583,   584,   622,   585,   251,   553,   627,   586,   587,
     598,   105,   591,  -285,   608,   628,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   194,   136,   137,   617,   629,
     599,   600,   601,   139,   140,   573,   631,  -381,   651,   640,
     652,   653,   571,   574,   654,   162,   603,   655,   158,   656,
     658,   659,   670,   671,   159,   672,   160,   673,   161,   674,
     197,   675,   203,   676,   142,   597,   604,   605,   606,   692,
     204,   679,   680,   660,   690,   681,   682,   689,   836,   837,
     200,   201,   202,   198,   199,   691,   707,   710,   571,   106,
     615,   616,   719,   720,   709,   105,   722,   721,   727,  -399,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   194,
     857,   739,   743,   747,   750,   195,   748,   139,   140,   749,
     162,   162,   753,   158,   158,   757,   759,   761,   762,   159,
     159,   160,   160,   161,   161,   767,   774,   776,   791,   142,
     142,  -408,   792,   794,   795,   799,   801,   823,   822,   827,
     829,   705,   830,   838,   105,     5,   843,   841,     8,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   194,   136,
     828,     9,    10,   106,    11,   848,   139,   140,   849,   854,
     860,   862,   863,   865,   868,   869,   871,   380,    99,   573,
     744,    13,    96,   685,   461,   527,   460,   574,   174,   462,
     663,   495,   590,   708,   756,   379,   551,   105,   831,   704,
     740,   548,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,     9,    10,   417,    11,   138,   139,
     140,   833,   106,   230,   835,   231,   858,   687,   540,   188,
      46,   370,   571,    50,   840,    59,    60,   723,    61,   232,
     233,   450,   234,   235,   560,    91,   236,   724,   715,   649,
       0,     0,     0,     0,     0,     0,     0,     0,   518,     0,
       0,     0,   573,     0,     0,     0,     0,   741,   237,     0,
     574,     0,   238,     0,     0,   106,     0,     0,     0,     0,
       0,     0,  -101,     0,     0,     0,   239,   121,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    91,   832,     0,
     715,     0,     0,     0,     0,     0,     0,     0,    46,     0,
       0,    50,     0,    59,    60,   571,    61,     0,     0,     0,
       0,   573,     0,   798,     0,     0,     0,     0,     0,   574,
       0,     0,     0,     0,    91,     0,     0,   797,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   162,   846,     0,   158,     0,   402,   402,     0,
       0,   159,     0,   160,   571,   161,     0,     0,     0,     0,
       0,   142,   839,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   473,     0,  -390,     5,     0,   402,
       8,  -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,
    -390,  -390,  -390,     9,    10,     0,    11,     0,  -390,  -390,
      12,     0,     0,   424,   425,   474,   475,  -390,     0,     0,
       0,     0,     0,    13,     0,     0,     0,   486,   487,   488,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   105,     0,    21,    22,     0,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,     0,     0,     0,  -390,   138,   139,   140,   473,     0,
    -390,     5,     0,   -86,     8,  -390,  -390,  -390,  -390,  -390,
    -390,  -390,  -390,  -390,  -390,  -390,  -390,     9,    10,     0,
      11,     0,  -390,  -390,    12,     0,     0,   424,   425,   474,
     475,  -390,     0,     0,     0,     0,     0,    13,     0,     0,
       0,   486,   487,   488,     0,     0,     0,     0,     0,     0,
       0,     0,   106,     0,     0,     0,     0,     0,     0,     0,
      21,    22,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -390,     0,
       0,     0,   473,     0,  -390,     5,     0,  -535,     8,  -390,
    -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,
    -390,     9,    10,     0,    11,     0,  -390,  -390,    12,     0,
       0,   424,   425,   474,   475,  -390,     0,     0,     0,     0,
       0,    13,     0,     0,     0,   519,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   520,     0,     0,     0,     0,
       0,     0,     0,     0,    21,    22,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -390,     0,     0,     0,   473,     0,  -390,     5,
       0,  -502,     8,  -390,  -390,  -390,  -390,  -390,  -390,  -390,
    -390,  -390,  -390,  -390,  -390,     9,    10,     0,    11,     0,
    -390,  -390,    12,     0,     0,   424,   425,   474,   475,  -390,
       0,   473,     0,  -390,     5,    13,     0,     8,  -390,  -390,
    -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,
       9,    10,     0,    11,     0,  -390,  -390,    12,    21,    22,
     424,   425,   474,   475,  -390,     0,     0,     0,     0,     0,
      13,     0,     0,     0,     0,     0,  -390,     0,     0,     0,
       0,     0,     0,     0,     0,   -68,     0,     0,     0,     0,
       0,     0,     0,    21,    22,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -390,     0,     0,     0,   473,     0,  -390,     5,     0,
     -90,     8,  -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,
    -390,  -390,  -390,  -390,     9,    10,     0,    11,     0,  -390,
    -390,    12,     0,     0,   424,   425,   474,   475,  -390,     0,
       0,     0,     0,     0,    13,     0,     0,     0,     0,     0,
       4,     0,     0,     5,     6,     7,     8,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,    22,     9,
      10,     0,    11,     0,     0,     0,    12,     0,     0,     0,
       0,     0,     0,     0,     0,  -390,     0,     0,     0,    13,
      14,    15,    16,     0,  -526,     0,     0,     0,    17,    18,
       0,     0,    19,    -2,     4,    20,     0,     5,     6,     7,
       8,     0,    21,    22,     0,     0,     0,     0,     0,    23,
      24,   698,     0,     9,    10,     0,    11,     0,     0,     0,
      12,     0,     0,     0,     0,    25,     0,     0,     0,   -53,
       0,     0,     0,    13,    14,    15,    16,     0,     0,     0,
       0,     0,    17,    18,     0,     0,    19,     4,     0,    20,
       5,     6,     7,     8,     0,     0,    21,    22,     0,     0,
       0,     0,     0,    23,    24,     0,     9,    10,     0,    11,
       0,     0,     0,    12,     0,     0,     0,     0,     0,    25,
    -208,     0,     0,     0,     0,     0,    13,    14,    15,    16,
       0,     0,     0,     0,     0,    17,    18,     0,     0,    19,
       0,     0,    20,     0,     0,     0,     0,     0,     0,    21,
      22,     0,     0,     0,     0,     0,    23,    24,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   593,     0,   105,
       0,     0,    25,  -208,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,     9,    10,     0,    11,
     138,   139,   140,     4,     0,     0,     5,     6,     7,     8,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     9,    10,     0,    11,     0,     0,     4,    12,
       0,     5,     6,     7,     8,     0,     0,     0,     0,     0,
       0,     0,    13,    14,    15,    16,     0,     9,    10,     0,
      11,    17,    18,     0,    12,    19,     0,   106,    20,     0,
       0,     0,     0,   173,     0,    21,    22,    13,    14,    15,
      16,     0,    23,    24,   698,     0,    17,    18,     0,     0,
      19,     0,     0,    20,     0,     0,     0,     0,    25,     0,
      21,    22,     0,     0,   105,     0,     0,    23,    24,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,     9,    10,    25,    11,   138,   139,   140,   593,     0,
     105,     0,     0,     0,     0,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,     9,    10,   105,
      11,   138,   139,   140,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,     9,    10,     0,    11,
     138,   139,   140,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   106,     0,   105,     0,     0,     0,   173,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   194,   136,
     137,     0,     0,     0,     0,     0,   139,   140,   106,     0,
       0,     0,     0,     0,     0,   570,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   106,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   106
};

static const yytype_int16 yycheck[] =
{
      21,    22,    42,    42,    13,    42,    82,    13,    82,    18,
      19,    42,    48,    42,     2,    42,    82,     2,   338,     2,
       2,    42,     2,   243,   110,   111,    82,    82,    82,    82,
      82,   117,   230,   352,   529,   221,   635,   483,   340,   341,
     342,   277,   404,     3,     1,    85,    85,     3,    85,   362,
       3,     3,     3,     3,    85,     0,    85,   678,    85,   421,
      48,    82,    83,    48,    85,    48,    48,    81,    48,     3,
       3,    28,    29,     3,   352,    84,     3,    86,    87,    88,
      89,     3,     3,   529,   648,    94,    19,   352,   648,    98,
       8,   105,   101,    11,    75,   104,   316,   735,   296,    75,
      81,   420,     1,     6,     6,    81,   292,     3,   729,   422,
      88,    35,    91,    73,    74,    75,    76,    77,    78,    79,
      80,    81,     3,   343,    84,    85,   764,    87,   364,    28,
      29,    87,   770,    90,    87,    87,    87,    87,    87,    99,
     100,    88,   420,   781,   104,   105,    49,    81,    81,    73,
      53,    81,   173,    77,   230,   420,   230,   343,    22,    81,
      81,    93,   726,    88,   230,   527,   726,    81,    93,    93,
      89,    95,    30,    31,   230,   230,   230,   230,   230,    88,
      88,    88,     8,     9,    93,   405,    93,     3,   683,   101,
     102,   103,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    88,    60,    84,    85,    88,    93,    65,    88,   230,
      68,    69,   448,    82,    83,    30,    31,   816,    99,   100,
     296,    49,   296,   104,   105,    53,    35,    36,    37,    88,
     296,   396,   397,   398,    99,   100,   555,   683,    28,    29,
     296,   296,   296,   296,   296,    60,   313,   314,   315,    92,
      65,    94,    89,   852,    14,    15,   855,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    15,    16,    84,    85,
      89,   766,   392,   393,   283,   296,   394,   395,    88,   499,
       4,     3,     6,    30,    31,    88,     8,     9,    10,   105,
      48,    13,    14,    15,    16,    89,    20,    21,    89,    23,
      24,    23,    88,    27,    51,    52,   580,   581,   296,    91,
     331,   296,    91,    60,    61,   501,    91,   338,    65,    88,
     766,    68,    69,    88,    87,    49,    88,    93,    89,    53,
     825,   633,   418,   419,    88,    88,    88,    88,     3,    89,
      88,   561,   363,    67,   380,    89,   532,    88,    88,    88,
      88,     3,    89,     8,   540,    91,   442,    92,   556,    81,
      91,    47,   105,    89,    89,    87,   375,     3,    89,   375,
      89,    94,   106,    93,    92,   108,    89,   107,   697,   825,
      96,    98,    97,    94,   470,   406,    88,    92,    94,    58,
      94,   400,   380,    90,    95,   380,   106,   380,   380,    95,
     380,   108,    94,     4,     3,   426,    90,   428,   429,   483,
      90,    89,   632,    94,   435,    90,    90,   483,   604,   605,
     606,    88,    92,    31,    92,     8,   447,    88,    92,    92,
     506,     3,    90,    90,    90,    88,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    90,    88,
     506,   506,   506,    25,    26,   529,    88,    90,    88,    90,
      88,    88,   483,   529,    88,   505,   506,    88,   505,    88,
      88,    88,    88,    88,   505,    88,   505,    88,   505,    88,
     556,    88,   556,    88,   505,   506,   507,   508,   509,    66,
     556,    88,    88,   579,    94,    90,    90,    90,   818,   819,
     556,   556,   556,   556,   556,    90,    88,   105,   529,    81,
     519,   520,    88,    88,   106,     3,    90,    88,    88,    35,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
     850,   109,    90,    88,    35,   556,    89,    25,    26,   106,
     580,   581,    38,   580,   581,    55,    94,    91,   105,   580,
     581,   580,   581,   580,   581,   106,    59,     3,    94,   580,
     581,   106,   105,   105,    62,    92,   106,   106,    94,    88,
      93,   657,    90,   105,     3,     4,    39,    94,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
     810,    20,    21,    81,    23,   106,    25,    26,   106,    93,
     105,    88,   106,    75,   106,    94,    75,   297,    16,   683,
     726,    40,    15,   632,   390,   423,   389,   683,    49,   391,
     590,   406,   501,   665,   736,   296,   443,     3,   813,   650,
     717,   717,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   345,    23,    24,    25,
      26,   815,    81,     4,   817,     6,   851,   634,   435,    77,
     648,    90,   683,   648,   822,   648,   648,   697,   648,    20,
      21,   369,    23,    24,   458,   684,    27,   698,   684,   561,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   420,    -1,
      -1,    -1,   766,    -1,    -1,    -1,    -1,   718,    49,    -1,
     766,    -1,    53,    -1,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    67,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   736,   814,    -1,
     736,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   726,    -1,
      -1,   726,    -1,   726,   726,   766,   726,    -1,    -1,    -1,
      -1,   825,    -1,   774,    -1,    -1,    -1,    -1,    -1,   825,
      -1,    -1,    -1,    -1,   773,    -1,    -1,   773,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   832,   832,    -1,   832,    -1,   818,   819,    -1,
      -1,   832,    -1,   832,   825,   832,    -1,    -1,    -1,    -1,
      -1,   832,   821,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     1,    -1,     3,     4,    -1,   850,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    23,    -1,    25,    26,
      27,    -1,    -1,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    44,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,    -1,    63,    64,    -1,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    81,    24,    25,    26,     1,    -1,
       3,     4,    -1,    90,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      23,    -1,    25,    26,    27,    -1,    -1,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,
      -1,    -1,     1,    -1,     3,     4,    -1,    90,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    -1,    25,    26,    27,    -1,
      -1,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,     1,    -1,     3,     4,
      -1,    90,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    23,    -1,
      25,    26,    27,    -1,    -1,    30,    31,    32,    33,    34,
      -1,     1,    -1,     3,     4,    40,    -1,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    23,    -1,    25,    26,    27,    63,    64,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,     1,    -1,     3,     4,    -1,
      90,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    -1,    25,
      26,    27,    -1,    -1,    30,    31,    32,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
       1,    -1,    -1,     4,     5,     6,     7,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    20,
      21,    -1,    23,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    40,
      41,    42,    43,    -1,    90,    -1,    -1,    -1,    49,    50,
      -1,    -1,    53,     0,     1,    56,    -1,     4,     5,     6,
       7,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,
      71,    72,    -1,    20,    21,    -1,    23,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    40,    41,    42,    43,    -1,    -1,    -1,
      -1,    -1,    49,    50,    -1,    -1,    53,     1,    -1,    56,
       4,     5,     6,     7,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    20,    21,    -1,    23,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,    43,
      -1,    -1,    -1,    -1,    -1,    49,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
      -1,    -1,    86,    87,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,     1,    -1,    -1,     4,     5,     6,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    20,    21,    -1,    23,    -1,    -1,     1,    27,
      -1,     4,     5,     6,     7,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    41,    42,    43,    -1,    20,    21,    -1,
      23,    49,    50,    -1,    27,    53,    -1,    81,    56,    -1,
      -1,    -1,    -1,    87,    -1,    63,    64,    40,    41,    42,
      43,    -1,    70,    71,    72,    -1,    49,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    86,    -1,
      63,    64,    -1,    -1,     3,    -1,    -1,    70,    71,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    86,    23,    24,    25,    26,     1,    -1,
       3,    -1,    -1,    -1,    -1,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     3,
      23,    24,    25,    26,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      24,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,     3,    -1,    -1,    -1,    87,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    -1,    25,    26,    81,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   111,   112,     0,     1,     4,     5,     6,     7,    20,
      21,    23,    27,    40,    41,    42,    43,    49,    50,    53,
      56,    63,    64,    70,    71,    86,   113,   115,   131,   133,
     137,   138,   150,   153,   154,   158,   160,   163,   164,   165,
     169,   173,   175,   179,   180,   204,   205,   223,   230,   231,
     238,   247,   267,   269,   281,   283,   305,   306,   307,   349,
     400,   401,   402,   403,   404,   408,   430,   432,   455,   456,
     457,   458,   459,   463,   464,   465,   468,   472,   481,   496,
     497,   130,   206,   132,   159,   239,   268,   282,   308,   350,
       3,   203,   255,   158,    53,   158,   173,   175,    53,   165,
     175,   176,   203,   203,   433,     3,    81,   199,   202,   199,
     482,   498,     3,    91,   134,   123,   139,   151,    88,    88,
     122,    93,   161,   155,   124,   166,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    24,    25,
      26,   161,   199,   242,   243,   244,   245,   246,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   269,
     281,   283,   307,   316,   319,   322,   323,   326,   327,   330,
     120,   116,   113,    87,   232,   117,   270,    22,   121,   118,
     119,   125,   405,   126,   431,   127,   161,   466,   466,   128,
     129,    89,   500,    88,    17,   199,   210,   257,   260,   261,
     262,   263,   264,   323,   327,   199,   203,   240,   242,   203,
     203,   203,   203,   161,   203,   161,   170,   203,   203,   409,
     203,   200,    75,    81,    75,   231,   231,    89,    89,    88,
       4,     6,    20,    21,    23,    24,    27,    49,    53,    67,
     473,   474,   477,   231,   493,    88,    48,   177,    89,    88,
      89,     8,    11,     8,     9,    91,   320,   174,    92,    94,
      91,    91,    88,    88,   199,    88,    89,   284,    88,    88,
      88,    88,    89,    88,    89,   442,    88,   467,   460,   469,
      88,    88,   501,   207,   241,   309,   351,    89,    93,   411,
     434,   202,   201,     3,   232,     3,   224,   135,   210,    91,
       3,   140,   475,    73,    74,    75,    76,    77,    78,    79,
      80,    84,    85,    99,   100,   104,   105,   199,   211,   212,
     213,   214,   215,   216,   217,   218,   219,   220,   221,   495,
      92,   162,   156,   167,     8,   212,   222,    91,    47,   178,
     317,   324,   328,   105,   233,   271,   105,   406,   443,   178,
      89,    89,   503,   203,   203,   248,   251,   255,   256,   331,
      89,    89,   171,   412,   410,    93,   439,   202,   483,   499,
      90,   205,   210,   225,   226,   227,   228,   229,   238,   265,
     112,   114,     3,    92,    94,   220,   220,   220,   212,    96,
      97,    98,    82,    83,    99,   100,   101,   102,   103,   494,
     152,   196,   199,   186,   181,    94,   321,   196,   222,   222,
     222,   202,   212,   234,   235,   237,   273,   275,   276,   285,
     413,   445,   461,   470,    30,    31,    60,    65,    68,    69,
     338,   339,   344,   422,   424,   425,   492,   502,   504,   208,
     332,   249,   310,   352,   186,   199,   178,   440,   435,    89,
     411,    88,   107,   255,   113,   136,    92,   141,   476,   106,
     214,   215,   216,   217,   217,   218,   218,   219,   219,   219,
      94,   203,   197,     1,    32,    33,   157,   187,   205,   238,
     338,   349,   354,   359,   400,   401,    44,    45,    46,   168,
     182,   183,   187,   361,   212,   243,   318,   325,   329,    95,
     106,    94,   236,   272,   274,   231,   231,    51,    52,    61,
     338,   407,   414,   422,   424,   427,   428,   429,   492,    44,
      54,   187,   444,   446,   449,   452,   186,   181,   340,   345,
      19,   199,   423,    58,   426,   199,   199,   507,   505,   506,
     423,   508,    90,    95,   108,   333,   335,    94,   231,   312,
     315,   274,   172,   199,   178,   484,     4,    90,     3,    89,
     477,   231,    94,   195,    90,   191,   188,   193,   192,   194,
      34,   199,   244,   323,   327,   360,   383,   189,   190,   362,
     185,   184,    90,    88,    92,    92,    92,    92,   212,   202,
     237,    90,   275,     1,   242,   277,     8,   199,   257,   262,
     263,   264,   290,   307,   199,   199,   199,   420,    90,   415,
     416,   417,   418,   419,   421,   203,   203,    90,   447,   448,
     462,   471,    31,   383,   202,     3,     3,    88,    88,    88,
     202,    88,   209,   336,   334,   250,     3,   311,   313,   353,
      90,   436,   338,   422,   424,   486,   488,   210,   142,   495,
     198,    88,    88,    88,    88,    88,    88,   355,    88,    88,
     231,   277,   277,   236,   280,   278,   286,   202,   202,   202,
      88,    88,    88,    88,    88,    88,    88,   450,   453,    88,
      88,    90,    90,   341,   346,   211,   222,   335,   251,    90,
      94,    90,    66,   491,   489,   490,   485,   487,    72,   115,
     131,   144,   146,   147,   199,   231,     3,    88,   248,   106,
     105,   365,   365,   383,   252,   255,   337,   314,   437,    88,
      88,    88,    90,   488,   199,   143,   145,    88,     3,   363,
     279,   287,   366,   367,   451,   454,   342,   347,   253,   109,
     315,   199,   148,    90,   146,   356,   365,    88,    89,   106,
      35,   368,   371,    38,   385,   385,   252,    55,   388,    94,
     438,    91,   105,   374,   364,   288,   372,   106,   369,   386,
     343,   389,   348,   254,    59,   441,     3,   478,   480,   375,
     376,   357,   385,     1,    28,    29,   291,   293,   297,   299,
     383,    94,   105,   385,   105,    62,   391,   255,   199,    92,
     479,   106,    35,    36,    37,   377,   380,   384,   385,   296,
     301,   300,   289,   292,   294,   298,   373,   370,   387,   390,
     392,   149,    94,   106,   378,   381,   358,    88,   212,    93,
      90,   293,   231,   299,   251,   371,   196,   196,   105,   203,
     480,    94,   383,    39,   394,   302,   242,   303,   106,   106,
     393,   379,   382,   395,    93,   304,   295,   196,   380,   251,
     105,   251,    88,   106,   396,    75,   397,   398,   106,    94,
     399,    75
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   110,   111,   112,   112,   112,   113,   113,   113,   114,
     116,   115,   117,   115,   118,   115,   119,   115,   120,   115,
     121,   115,   122,   115,   123,   115,   124,   115,   125,   115,
     126,   115,   127,   115,   128,   115,   129,   115,   130,   115,
     132,   131,   134,   135,   136,   133,   137,   139,   140,   141,
     142,   143,   138,   144,   145,   145,   146,   146,   148,   149,
     147,   151,   152,   150,   153,   153,   155,   156,   157,   154,
     159,   158,   160,   160,   160,   162,   161,   161,   163,   163,
     163,   163,   164,   164,   166,   167,   168,   165,   170,   171,
     172,   169,   174,   173,   176,   175,   177,   177,   178,   178,
     179,   179,   180,   181,   181,   182,   182,   182,   184,   183,
     185,   183,   186,   186,   188,   187,   189,   187,   190,   187,
     191,   187,   192,   187,   193,   187,   194,   187,   195,   187,
     196,   198,   197,   197,   199,   200,   199,   201,   199,   202,
     203,   204,   204,   204,   206,   207,   208,   209,   205,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   211,   212,
     213,   213,   214,   214,   215,   215,   216,   216,   216,   217,
     217,   217,   218,   218,   218,   218,   219,   219,   219,   219,
     220,   220,   220,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   222,   223,   224,   224,   225,   225,   225,
     226,   227,   227,   228,   229,   229,   230,   231,   231,   232,
     233,   233,   234,   234,   235,   236,   236,   237,   239,   238,
     238,   238,   238,   238,   238,   241,   240,   242,   242,   243,
     243,   243,   244,   244,   244,   244,   244,   244,   244,   244,
     245,   245,   245,   245,   246,   246,   246,   247,   247,   248,
     250,   249,   249,   251,   251,   252,   254,   253,   253,   255,
     256,   257,   257,   258,   258,   258,   259,   259,   259,   260,
     260,   260,   261,   262,   262,   263,   264,   265,   266,   268,
     267,   270,   271,   272,   269,   273,   274,   274,   276,   275,
     278,   279,   277,   280,   277,   282,   281,   284,   285,   286,
     287,   288,   289,   283,   290,   290,   290,   290,   290,   290,
     291,   292,   292,   294,   295,   293,   296,   293,   297,   298,
     298,   300,   299,   301,   302,   299,   304,   303,   305,   306,
     308,   309,   310,   311,   307,   312,   314,   313,   313,   315,
     317,   318,   316,   316,   320,   321,   319,   322,   324,   325,
     323,   323,   326,   328,   329,   327,   327,   330,   332,   331,
     333,   334,   334,   336,   337,   335,   338,   338,   340,   341,
     342,   343,   339,   345,   346,   347,   348,   344,   350,   351,
     352,   353,   349,   355,   356,   357,   358,   354,   359,   359,
     359,   360,   360,   362,   363,   364,   361,   366,   365,   367,
     365,   368,   370,   369,   369,   372,   373,   371,   375,   374,
     376,   374,   377,   379,   378,   378,   381,   382,   380,   383,
     383,   383,   383,   384,   384,   384,   386,   387,   385,   385,
     389,   390,   388,   388,   392,   393,   391,   391,   395,   396,
     394,   394,   397,   399,   398,   398,   400,   401,   402,   402,
     403,   405,   406,   407,   404,   409,   410,   408,   412,   411,
     411,   413,   413,   415,   414,   416,   414,   417,   414,   418,
     414,   419,   414,   420,   414,   421,   414,   422,   423,   423,
     424,   425,   426,   426,   427,   428,   429,   431,   430,   433,
     434,   435,   436,   437,   438,   432,   440,   439,   439,   441,
     441,   443,   444,   442,   445,   445,   446,   447,   446,   448,
     446,   450,   451,   449,   453,   454,   452,   455,   455,   455,
     456,   456,   457,   458,   460,   461,   462,   459,   463,   464,
     465,   467,   466,   469,   470,   471,   468,   472,   472,   473,
     473,   473,   473,   473,   473,   473,   473,   473,   473,   474,
     476,   475,   475,   477,   477,   478,   479,   479,   480,   482,
     483,   484,   485,   481,   486,   487,   487,   489,   488,   490,
     488,   491,   488,   492,   492,   493,   494,   494,   495,   496,
     498,   499,   497,   501,   502,   500,   503,   503,   505,   504,
     506,   504,   507,   504,   508,   504
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     2,     0,     1,     2,     2,     2,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     0,     0,     7,     2,     0,     0,     0,
       0,     0,    11,     2,     2,     0,     1,     2,     0,     0,
       8,     0,     0,     6,     1,     1,     0,     0,     0,     7,
       0,     3,     2,     3,     3,     0,     4,     0,     1,     1,
       1,     1,     2,     1,     0,     0,     0,     7,     0,     0,
       0,     8,     0,     4,     0,     3,     1,     0,     2,     0,
       2,     1,     2,     2,     0,     1,     1,     2,     0,     3,
       0,     3,     2,     0,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       2,     0,     4,     0,     1,     0,     3,     0,     4,     1,
       1,     1,     2,     2,     0,     0,     0,     0,     9,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     3,     1,     3,     1,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     1,     2,     2,     2,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     5,     3,     0,     1,     1,     1,
       2,     1,     1,     4,     1,     1,     2,     2,     0,     3,
       3,     0,     1,     1,     2,     3,     0,     3,     0,     3,
       1,     1,     1,     2,     1,     0,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       0,     4,     0,     1,     1,     2,     0,     4,     0,     1,
       1,     1,     1,     1,     2,     1,     2,     3,     2,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     0,
       3,     0,     0,     0,     7,     2,     2,     0,     0,     3,
       0,     0,     5,     0,     3,     0,     3,     0,     0,     0,
       0,     0,     0,    15,     1,     1,     1,     1,     1,     1,
       2,     2,     0,     0,     0,     6,     0,     3,     2,     2,
       0,     0,     3,     0,     0,     5,     0,     3,     1,     1,
       0,     0,     0,     0,     9,     2,     0,     4,     0,     2,
       0,     0,     6,     2,     0,     0,     5,     6,     0,     0,
       6,     1,     1,     0,     0,     6,     1,     1,     0,     3,
       2,     2,     0,     0,     0,     5,     1,     1,     0,     0,
       0,     0,     9,     0,     0,     0,     0,     9,     0,     0,
       0,     0,     9,     0,     0,     0,     0,    11,     1,     1,
       0,     1,     1,     0,     0,     0,     8,     0,     3,     0,
       4,     2,     0,     4,     0,     0,     0,     5,     0,     3,
       0,     4,     2,     0,     4,     0,     0,     0,     5,     1,
       1,     1,     1,     1,     1,     1,     0,     0,     6,     0,
       0,     0,     6,     0,     0,     0,     6,     0,     0,     0,
       6,     0,     2,     0,     4,     0,     3,     3,     1,     1,
       2,     0,     0,     0,     7,     0,     0,     6,     0,     3,
       0,     2,     0,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     3,     1,     1,
       3,     2,     1,     0,     3,     3,     3,     0,     3,     0,
       0,     0,     0,     0,     0,    13,     0,     3,     0,     2,
       0,     0,     0,     5,     2,     0,     1,     0,     3,     0,
       3,     0,     0,     6,     0,     0,     6,     1,     1,     1,
       1,     1,     2,     3,     0,     0,     0,     8,     3,     3,
       2,     0,     3,     0,     0,     0,     8,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       0,     4,     0,     2,     5,     2,     3,     0,     1,     0,
       0,     0,     0,    10,     2,     2,     0,     0,     3,     0,
       3,     0,     3,     3,     3,     3,     4,     0,     1,     2,
       0,     0,     6,     0,     0,     5,     2,     0,     0,     3,
       0,     3,     0,     3,     0,     3
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
        case 3:
#line 389 "fe/idl.ypp" /* yacc.c:1646  */
    {
          AST_Decl *d = (yyvsp[0].dcval);
          if (d) d->annotations ((yyvsp[-1].annotations));
        }
#line 2390 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 394 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 2397 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 401 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 2405 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 405 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dcval) = 0;
        }
#line 2413 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 409 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dcval) = 0;
        }
#line 2421 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 420 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AnnotationDeclSeen);
        }
#line 2429 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 424 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2438 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 429 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 2446 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 433 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 2455 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 438 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 2463 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 442 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2472 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 447 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 2480 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 451 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2489 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 456 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 2497 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 460 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 2506 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 465 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 2514 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 469 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2523 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 474 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
        }
#line 2531 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 478 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2540 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 483 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
        }
#line 2548 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 487 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 2557 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 492 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeDeclSeen);
        }
#line 2565 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 496 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2574 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 501 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentDeclSeen);
        }
#line 2582 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 505 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2591 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 510 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeDeclSeen);
        }
#line 2599 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 514 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2608 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 519 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventDeclSeen);
        }
#line 2616 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 523 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2625 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 528 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeDeclSeen);
        }
#line 2633 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 532 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2642 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 537 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorDeclSeen);
        }
#line 2650 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 541 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2659 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 546 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 2667 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 550 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
          (yyval.dcval) = 0;
        }
#line 2677 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 559 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
        }
#line 2685 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 563 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 2693 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 570 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
              m = idl_global->gen ()->create_module (s, (yyvsp[0].idlist));
              (void) s->fe_add_module (m);
            }

          (yyvsp[0].idlist)->destroy ();
          delete (yyvsp[0].idlist);
          (yyvsp[0].idlist) = 0;

          /*
           * Push it on the stack
           */
          idl_global->scopes ().push (m);

          (yyval.dcval) = m;
        }
#line 2736 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 609 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
        }
#line 2744 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 613 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
        }
#line 2752 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 617 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
          /*
           * Finished with this module - pop it from the scope stack.
           */

          idl_global->scopes ().pop ();
          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 2766 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 630 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleIDSeen);
        }
#line 2774 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 637 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 2792 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 651 "fe/idl.ypp" /* yacc.c:1646  */
    {
          if (FE_Utils::duplicate_param_id ((yyvsp[0].plval)))
            {
              idl_global->err ()->duplicate_param_id (
                (yyvsp[-2].idlist));

              return 1;
            }
        }
#line 2806 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 661 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 2840 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 691 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleSqSeen);
        }
#line 2848 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 695 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleBodySeen);
        }
#line 2856 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 699 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 2877 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 733 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefSeen);
        }
#line 2886 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 738 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefParamsSeen);
        }
#line 2895 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 743 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 2977 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 824 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleSeen);
        }
#line 2986 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 829 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleArgsSeen);
        }
#line 2995 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 834 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 3061 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 904 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 3103 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 942 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
#line 3111 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 946 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
#line 3119 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 950 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
#line 3132 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 962 "fe/idl.ypp" /* yacc.c:1646  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
#line 3140 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 966 "fe/idl.ypp" /* yacc.c:1646  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (yyval.idval) = (yyvsp[0].idval);
         }
#line 3149 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 974 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 3189 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 1011 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 3222 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 1041 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 3255 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 1073 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 3263 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 1077 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyvsp[0].nlval)->truncatable ((yyvsp[-2].bval));
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3272 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 1082 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.nlval) = 0;
        }
#line 3280 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 1096 "fe/idl.ypp" /* yacc.c:1646  */
    {
           ACE_DEBUG ((LM_DEBUG,
                       ACE_TEXT ("error in %C line %d\n"),
                       idl_global->filename ()->get_string (),
                       idl_global->lineno ()));
           ACE_DEBUG ((LM_DEBUG,
                       ACE_TEXT ("Sorry, I (TAO_IDL) can't handle")
                       ACE_TEXT (" custom yet\n")));
        }
#line 3294 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 1110 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 3341 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 1153 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3349 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 1157 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3357 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 1161 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 3380 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 1184 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 3427 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 1227 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3435 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 1231 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3443 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 1235 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 3456 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 1248 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 3464 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 1252 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 3499 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 1286 "fe/idl.ypp" /* yacc.c:1646  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
#line 3507 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 1290 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 3516 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 1298 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.bval) = true;
        }
#line 3524 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 1302 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.bval) = false;
        }
#line 3532 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 1310 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3540 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 1314 "fe/idl.ypp" /* yacc.c:1646  */
    {
/*      |    EMPTY */
          (yyval.nlval) = 0;
        }
#line 3549 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 1323 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 3576 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 1347 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 3603 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 1373 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 3668 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 1449 "fe/idl.ypp" /* yacc.c:1646  */
    {
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_PUBLIC;
        }
#line 3677 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 1454 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 3684 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 1457 "fe/idl.ypp" /* yacc.c:1646  */
    {
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_PRIVATE;
        }
#line 3693 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 1462 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 3700 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 1473 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 3708 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 1477 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3716 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 1481 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 3724 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 1485 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3732 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 1489 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 3740 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 1493 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3748 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 1497 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 3756 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 1501 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
#line 3764 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 1505 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 3772 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 1509 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3780 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 1513 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 3788 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 1517 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3796 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 1521 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
#line 3804 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 1525 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3812 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 1529 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 3820 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 1533 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 3829 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 1541 "fe/idl.ypp" /* yacc.c:1646  */
    {
          ACE_NEW_RETURN ((yyval.nlval),
                          UTL_NameList ((yyvsp[-1].idlist),
                                        (yyvsp[0].nlval)),
                          1);
        }
#line 3840 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 1552 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
#line 3848 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 1556 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 3872 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 1576 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.nlval) = 0;
        }
#line 3880 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 1590 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN ((yyval.idlist),
                          UTL_IdList ((yyvsp[0].idval),
                                      0),
                          1);
        }
#line 3893 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 1599 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
#line 3901 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 1603 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 3925 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 1624 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);

          // This cleans up all the non-global "::"s in scoped names.
          // If there is a global one, it gets put into the UTL_IdList,
          // so we clean it up in the case above.
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3939 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 1634 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          UTL_IdList *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_IdList ((yyvsp[0].idval),
                                      0),
                          1);
          (yyvsp[-3].idlist)->nconc (sn);
          (yyval.idlist) = (yyvsp[-3].idlist);
        }
#line 3955 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 1648 "fe/idl.ypp" /* yacc.c:1646  */
    {
          ACE_NEW_RETURN ((yyval.idval),
                          Identifier ((yyvsp[0].strval)),
                          1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3967 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 1658 "fe/idl.ypp" /* yacc.c:1646  */
    {
          /* defining_id is a defining identifier
             whereas id is usually a reference to a defining identifier */
          ACE_NEW_RETURN ((yyval.idval), Identifier ((yyvsp[0].strval)), 1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3979 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 1669 "fe/idl.ypp" /* yacc.c:1646  */
    {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval), 0);

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
#line 4026 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 1713 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 4056 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 1740 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 4086 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 1769 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
#line 4094 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 1773 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
#line 4102 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 1777 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
#line 4110 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 1781 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
#line 4118 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 1785 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dcval) = 0;
          UTL_ScopedName n ((yyvsp[-4].idval), 0);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Constant *c = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstExprSeen);

          /*
           * Create a node representing a constant declaration. Store
           * it in the enclosing scope.
           */
          if ((yyvsp[0].exval) != 0 && s != 0)
            {
              AST_Param_Holder *param_holder = (yyvsp[0].exval)->param_holder ();

              AST_Expression::AST_ExprValue *result =
                (yyvsp[0].exval)->check_and_coerce ((yyvsp[-6].etval), tao_enum_constant_decl);
              tao_enum_constant_decl = 0;

              // If the expression is a template parameter place
              // holder, 'result' will be 0, but it's ok.
              if (result == 0 && param_holder == 0)
                {
                  idl_global->err ()->coercion_error ((yyvsp[0].exval), (yyvsp[-6].etval));
                  (yyvsp[0].exval)->destroy ();
                  delete (yyvsp[0].exval);
                  (yyvsp[0].exval) = 0;
                }
              else
                {
                  AST_Expression::ExprType et = (yyvsp[-6].etval);

                  if (param_holder != 0
                      && et != param_holder->info ()->const_type_)
                    {
                      idl_global->err ()->mismatched_template_param (
                        param_holder->info ()->name_.c_str ());
                    }
                  else
                    {
                      c = idl_global->gen ()->create_constant ((yyvsp[-6].etval), (yyvsp[0].exval), &n);
                      (void) s->fe_add_constant (c);
                      (yyval.dcval) = c;
                    }
                }

              delete result;
              result = 0;
            }

          (yyvsp[-4].idval)->destroy ();
          delete (yyvsp[-4].idval);
          (yyvsp[-4].idval) = 0;
        }
#line 4178 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 1850 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_string;
        }
#line 4186 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 1854 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_wstring;
        }
#line 4194 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 1858 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 4264 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 1931 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_or,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4277 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 1944 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_xor,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4290 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 1957 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_and,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4303 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 1970 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_left,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4316 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 1979 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_right,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4329 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 1992 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_add,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4342 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 2001 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_minus,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4355 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 2014 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mul,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4368 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 2023 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_div,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4381 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 2032 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mod,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4394 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 177:
#line 2045 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_u_plus,
                                    (yyvsp[0].exval),
                                    0
                                  );
        }
#line 4407 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 2054 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_u_minus,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4420 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 2063 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_bit_neg,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4433 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 2075 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 4487 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 2126 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) = (yyvsp[-1].exval);
        }
#line 4495 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 2133 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].ival));
        }
#line 4503 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 2137 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr ((yyvsp[0].uival));
        }
#line 4512 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 2142 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].sval));
          (yyvsp[0].sval)->destroy ();
          delete (yyvsp[0].sval);
          (yyvsp[0].sval) = 0;
        }
#line 4523 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 2149 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].wsval));
        }
#line 4531 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 2153 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].cval));
        }
#line 4539 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 2157 "fe/idl.ypp" /* yacc.c:1646  */
    {
          ACE_OutputCDR::from_wchar wc ((yyvsp[0].wcval));
          (yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
#line 4548 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 2162 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].fixval));
        }
#line 4556 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 2166 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].dval));
        }
#line 4564 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 2170 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr (true);
        }
#line 4572 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 2174 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr (false);
        }
#line 4580 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 2181 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 4649 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 2248 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 4678 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 2305 "fe/idl.ypp" /* yacc.c:1646  */
    {
          Annotations *annotations = (yyvsp[-1].annotations);
          annotations->insert_tail ((yyvsp[0].annotation));
          (yyval.annotations) = annotations;
        }
#line 4688 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 2314 "fe/idl.ypp" /* yacc.c:1646  */
    {
          Annotations *annotations = (yyvsp[-1].annotations);
          annotations->insert_tail ((yyvsp[0].annotation));
          (yyval.annotations) = annotations;
        }
#line 4698 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 2320 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.annotations) = new Annotations ();
        }
#line 4706 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 2327 "fe/idl.ypp" /* yacc.c:1646  */
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

            (yyval.annotation) = idl_global->gen ()->create_annotation ((yyvsp[-1].idlist), (yyvsp[0].annotation_params));
        }
#line 4727 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 2347 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.annotation_params) = (yyvsp[-1].annotation_params);
        }
#line 4735 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 2351 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.annotation_params) = 0;
        }
#line 4743 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 2358 "fe/idl.ypp" /* yacc.c:1646  */
    {
          AST_Annotation::Params *params = new AST_Annotation::Params;
          AST_Annotation::Param *param = new AST_Annotation::Param;
          param->id = 0;
          param->expr = (yyvsp[0].exval);
          params->insert_head (param);
          (yyval.annotation_params) = params;
        }
#line 4756 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 213:
#line 2367 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.annotation_params) = (yyvsp[0].annotation_params);
        }
#line 4764 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 214:
#line 2374 "fe/idl.ypp" /* yacc.c:1646  */
    {
          AST_Annotation::Params *params = (yyvsp[0].annotation_params);
          params->insert_head ((yyvsp[-1].annotation_param));
          (yyval.annotation_params) = params;
        }
#line 4774 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 215:
#line 2382 "fe/idl.ypp" /* yacc.c:1646  */
    {
          AST_Annotation::Params *params = (yyvsp[0].annotation_params);
          params->insert_head ((yyvsp[-1].annotation_param));
          (yyval.annotation_params) = params;
        }
#line 4784 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 216:
#line 2388 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.annotation_params) = new AST_Annotation::Params;
        }
#line 4792 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 217:
#line 2395 "fe/idl.ypp" /* yacc.c:1646  */
    {
          AST_Annotation::Param *param = new AST_Annotation::Param;
          param->id = (yyvsp[-2].idval);
          param->expr = (yyvsp[0].exval);
          (yyval.annotation_param) = param;
        }
#line 4803 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 218:
#line 2405 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 4812 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 219:
#line 2410 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dcval) = 0;
        }
#line 4820 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 220:
#line 2414 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 4828 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 221:
#line 2418 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 4836 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 222:
#line 2422 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 4844 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 223:
#line 2426 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 4874 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 224:
#line 2452 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dcval) = 0;
        }
#line 4882 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 225:
#line 2459 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
#line 4890 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 226:
#line 2463 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 4957 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 229:
#line 2534 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 4968 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 231:
#line 2542 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 4995 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 249:
#line 2597 "fe/idl.ypp" /* yacc.c:1646  */
    {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5006 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 250:
#line 2608 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5014 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 251:
#line 2612 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 5038 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 252:
#line 2632 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dlval) = 0;
        }
#line 5046 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 255:
#line 2644 "fe/idl.ypp" /* yacc.c:1646  */
    {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5057 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 256:
#line 2655 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5065 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 257:
#line 2659 "fe/idl.ypp" /* yacc.c:1646  */
    {
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

  case 258:
#line 2679 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dlval) = 0;
        }
#line 5097 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 259:
#line 2686 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 5114 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 260:
#line 2702 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 5133 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 263:
#line 2725 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_long;
        }
#line 5141 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 264:
#line 2729 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_longlong;
        }
#line 5149 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 265:
#line 2733 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_short;
        }
#line 5157 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 266:
#line 2740 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_ulong;
        }
#line 5165 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 267:
#line 2744 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_ulonglong;
        }
#line 5173 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 268:
#line 2748 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_ushort;
        }
#line 5181 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 269:
#line 2755 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_double;
        }
#line 5189 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 270:
#line 2759 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_float;
        }
#line 5197 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 271:
#line 2763 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_longdouble;
        }
#line 5205 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 272:
#line 2770 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_fixed;
        }
#line 5213 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 273:
#line 2777 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_char;
        }
#line 5221 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 274:
#line 2781 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_wchar;
        }
#line 5229 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 275:
#line 2788 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_octet;
        }
#line 5237 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 276:
#line 2795 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_bool;
        }
#line 5245 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 277:
#line 2802 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_any;
        }
#line 5253 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 278:
#line 2809 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.etval) = AST_Expression::EV_object;
        }
#line 5261 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 279:
#line 2816 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
#line 5269 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 280:
#line 2820 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5278 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 281:
#line 2829 "fe/idl.ypp" /* yacc.c:1646  */
    {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval), 0);
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
              AST_Structure::fwd_redefinition_helper (d, s);
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
#line 5313 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 282:
#line 2860 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
#line 5321 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 283:
#line 2864 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
#line 5329 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 284:
#line 2868 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructQsSeen);

          /*
           * Done with this struct. Pop its scope off the scopes stack.
           */
          (yyval.dcval) = AST_Structure::narrow_from_scope (
                   idl_global->scopes ().top_non_null ()
                 );
          idl_global->scopes ().pop ();
        }
#line 5345 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 288:
#line 2889 "fe/idl.ypp" /* yacc.c:1646  */
    {
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_NA;
        }
#line 5354 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 289:
#line 2894 "fe/idl.ypp" /* yacc.c:1646  */
    {
          if ((yyvsp[0].ast_decls))
            {
              AST_DeclsIter i (*(yyvsp[0].ast_decls));
              for (AST_Decl *d = 0; i.next (d); i.advance ())
                {
                  d->annotations ((yyvsp[-1].annotations));
                }
            }
        }
#line 5369 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 290:
#line 2908 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
#line 5377 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 291:
#line 2912 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
#line 5385 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 292:
#line 2916 "fe/idl.ypp" /* yacc.c:1646  */
    {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          FE_Declarator *d = 0;
          AST_Field *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsCompleted);
          AST_Decls *members = 0;

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
          else if (s && (yyvsp[-4].dcval) && (yyvsp[-2].dlval))
            {
              members = new AST_Decls;
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
                  members->insert_tail (f);
                  (void) s->fe_add_field (f);
                }
            }

          (yyvsp[-2].dlval)->destroy ();
          delete (yyvsp[-2].dlval);
          (yyvsp[-2].dlval) = 0;

          (yyval.ast_decls) = members;
        }
#line 5449 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 293:
#line 2976 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->err()->syntax_error (idl_global->parse_state ());
        }
#line 5457 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 294:
#line 2980 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 5466 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 295:
#line 2988 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
#line 5474 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 296:
#line 2992 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5483 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 297:
#line 3000 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
#line 5491 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 298:
#line 3004 "fe/idl.ypp" /* yacc.c:1646  */
    {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[-3].idval), 0);
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
              AST_Structure::fwd_redefinition_helper (st, s);
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
#line 5528 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 299:
#line 3037 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
#line 5536 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 300:
#line 3041 "fe/idl.ypp" /* yacc.c:1646  */
    {
          /*
           * The top of the scopes must be an empty union we added after we
           * encountered 'union <id> switch ('. Now we are ready to add a
           * correct one. Temporarily remove the top so that we setup the
           * correct union in a right scope.
           */
          UTL_Scope *top = idl_global->scopes ().top_non_null ();
          idl_global->scopes ().pop ();

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[-8].idval), 0);
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchCloseParSeen);

          /*
           * Create a node representing a union.
           */
          if ((yyvsp[-2].dcval) != 0 && s != 0)
            {
              AST_ConcreteType *tp =
                AST_ConcreteType::narrow_from_decl ((yyvsp[-2].dcval));
              tp->annotations((yyvsp[-3].annotations));

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

          (yyvsp[-8].idval)->destroy ();
          delete (yyvsp[-8].idval);
          (yyvsp[-8].idval) = 0;
        }
#line 5596 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 301:
#line 3097 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
#line 5604 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 302:
#line 3101 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
#line 5612 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 303:
#line 3105 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 5632 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 304:
#line 3124 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5643 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 305:
#line 3131 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 5660 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 306:
#line 3144 "fe/idl.ypp" /* yacc.c:1646  */
    {
          /* octets are not allowed. */
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5673 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 307:
#line 3153 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5684 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 309:
#line 3161 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 5792 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 313:
#line 3275 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
#line 5800 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 314:
#line 3279 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
#line 5808 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 315:
#line 3283 "fe/idl.ypp" /* yacc.c:1646  */
    {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_UnionBranch *b = 0;
          AST_Field *f = (yyvsp[-2].ffval);
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemCompleted);

          /*
           * Create several nodes representing branches of a union.
           * Add them to the enclosing scope (the union scope)
           */
          if (s != 0 && (yyvsp[-5].llval) != 0 && (yyvsp[-2].ffval) != 0)
            {
              b =
                idl_global->gen ()->create_union_branch (
                                        (yyvsp[-5].llval),
                                        f->field_type (),
                                        f->name ()
                                      );
              b->annotations ((yyvsp[-3].annotations));
              (void) s->fe_add_union_branch (b);

              // f has passed its field type to the union branch,
              // but the rest still needs to be cleaned up.
              f->AST_Decl::destroy ();
              delete f;
              f = 0;
            }
        }
#line 5841 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 316:
#line 3312 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 5849 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 317:
#line 3316 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 5858 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 318:
#line 3324 "fe/idl.ypp" /* yacc.c:1646  */
    {
          ACE_NEW_RETURN ((yyval.llval),
                          UTL_LabelList ((yyvsp[-1].ulval),
                                         (yyvsp[0].llval)),
                          1);
        }
#line 5869 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 319:
#line 3334 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 5891 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 320:
#line 3352 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.llval) = 0;
        }
#line 5899 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 321:
#line 3359 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
#line 5907 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 322:
#line 3363 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
#line 5920 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 323:
#line 3372 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
#line 5928 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 324:
#line 3376 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
#line 5936 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 325:
#line 3380 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      (yyvsp[-2].exval)
                                    );
        }
#line 5949 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 326:
#line 3392 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
#line 5957 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 327:
#line 3396 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 6005 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 328:
#line 3443 "fe/idl.ypp" /* yacc.c:1646  */
    {
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

          (yyval.dcval) = d;
        }
#line 6031 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 329:
#line 3468 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 6055 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 330:
#line 3491 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
#line 6063 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 331:
#line 3495 "fe/idl.ypp" /* yacc.c:1646  */
    {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n ((yyvsp[0].idval), 0);
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
#line 6098 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 332:
#line 3526 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
#line 6106 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 333:
#line 3530 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
#line 6114 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 334:
#line 3534 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 6137 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 336:
#line 3559 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
#line 6145 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 339:
#line 3568 "fe/idl.ypp" /* yacc.c:1646  */
    {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          Identifier id ((yyvsp[0].strval));
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
          UTL_ScopedName n (&id, 0);
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
                  e->annotations ((yyvsp[-1].annotations));
                }

              (void) s->fe_add_enum_val (e);
            }
        }
#line 6180 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 340:
#line 3603 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
#line 6188 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 341:
#line 3607 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
#line 6196 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 342:
#line 3611 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 6276 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 343:
#line 3688 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 6335 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 344:
#line 3746 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
#line 6348 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 345:
#line 3755 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
#line 6356 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 346:
#line 3759 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 6365 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 347:
#line 3767 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyvsp[-1].exval)->evaluate (AST_Expression::EK_positive_int);
          (yyval.dcval) = idl_global->gen ()->create_fixed ((yyvsp[-3].exval), (yyvsp[-1].exval));
        }
#line 6374 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 348:
#line 3776 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 6382 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 349:
#line 3780 "fe/idl.ypp" /* yacc.c:1646  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 6390 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 350:
#line 3784 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 6436 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 351:
#line 3826 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 6462 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 352:
#line 3851 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 6470 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 353:
#line 3859 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 6478 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 354:
#line 3863 "fe/idl.ypp" /* yacc.c:1646  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 6486 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 355:
#line 3867 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 6521 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 356:
#line 3898 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 6546 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 357:
#line 3922 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 6554 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 358:
#line 3929 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
#line 6562 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 359:
#line 3933 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 6599 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 360:
#line 3969 "fe/idl.ypp" /* yacc.c:1646  */
    {
          ACE_NEW_RETURN ((yyval.elval),
                          UTL_ExprList ((yyvsp[-1].exval),
                                        (yyvsp[0].elval)),
                          1);
        }
#line 6610 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 361:
#line 3979 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 6632 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 362:
#line 3997 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.elval) = 0;
        }
#line 6640 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 363:
#line 4004 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
#line 6648 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 364:
#line 4008 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
#line 6656 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 365:
#line 4012 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 6714 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 368:
#line 4074 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
#line 6722 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 369:
#line 4078 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 6730 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 370:
#line 4082 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 6738 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 371:
#line 4086 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 6746 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 372:
#line 4090 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 6810 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 373:
#line 4153 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 6818 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 374:
#line 4157 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 6826 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 375:
#line 4161 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 6834 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 376:
#line 4165 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
#line 6842 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 377:
#line 4169 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 6915 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 378:
#line 4241 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
#line 6923 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 379:
#line 4245 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 6956 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 380:
#line 4274 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
#line 6964 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 381:
#line 4278 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
#line 6972 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 382:
#line 4282 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();
        }
#line 6984 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 383:
#line 4294 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 6992 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 384:
#line 4298 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 7060 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 385:
#line 4362 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7068 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 386:
#line 4366 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
#line 7076 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 387:
#line 4370 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 7109 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 388:
#line 4402 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_oneway;
        }
#line 7118 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 389:
#line 4407 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_idempotent;
        }
#line 7127 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 390:
#line 4412 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.ofval) = AST_Operation::OP_noflags;
        }
#line 7135 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 392:
#line 4420 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
#line 7146 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 393:
#line 4430 "fe/idl.ypp" /* yacc.c:1646  */
    {
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7155 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 394:
#line 4435 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 7200 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 395:
#line 4476 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7208 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 396:
#line 4480 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          if ((yyvsp[0].nlval) != 0)
            {
              UTL_Scope *s = idl_global->scopes ().top_non_null ();
              AST_Factory *f = AST_Factory::narrow_from_scope (s);
              (void) f->fe_add_exceptions ((yyvsp[0].nlval));
            }

          idl_global->scopes ().pop ();
        }
#line 7225 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 397:
#line 4496 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7233 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 398:
#line 4500 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7241 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 399:
#line 4504 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7249 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 400:
#line 4509 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7257 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 402:
#line 4519 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7265 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 405:
#line 4528 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7273 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 406:
#line 4532 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7281 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 407:
#line 4536 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 7317 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 408:
#line 4571 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7325 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 409:
#line 4575 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7333 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 410:
#line 4579 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7341 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 411:
#line 4584 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7349 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 413:
#line 4594 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7357 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 416:
#line 4603 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7365 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 417:
#line 4607 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7373 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 418:
#line 4611 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 7416 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 419:
#line 4653 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 7427 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 422:
#line 4662 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 7535 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 423:
#line 4769 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dival) = AST_Argument::dir_IN;
        }
#line 7543 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 424:
#line 4773 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dival) = AST_Argument::dir_OUT;
        }
#line 7551 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 425:
#line 4777 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.dival) = AST_Argument::dir_INOUT;
        }
#line 7559 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 426:
#line 4784 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
#line 7567 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 427:
#line 4788 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
#line 7575 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 428:
#line 4793 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7584 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 429:
#line 4798 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.nlval) = 0;
        }
#line 7592 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 430:
#line 4805 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
#line 7600 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 431:
#line 4809 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
#line 7608 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 432:
#line 4814 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7617 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 433:
#line 4819 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.nlval) = 0;
        }
#line 7625 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 434:
#line 4826 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
#line 7633 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 435:
#line 4830 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
#line 7641 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 436:
#line 4835 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7650 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 437:
#line 4840 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.nlval) = 0;
        }
#line 7658 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 438:
#line 4847 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
#line 7666 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 439:
#line 4851 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
        }
#line 7674 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 440:
#line 4856 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (yyval.slval) = (yyvsp[-1].slval);
        }
#line 7683 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 441:
#line 4861 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.slval) = 0;
        }
#line 7691 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 442:
#line 4868 "fe/idl.ypp" /* yacc.c:1646  */
    {
          ACE_NEW_RETURN ((yyval.slval),
                          UTL_StrList ((yyvsp[-1].sval),
                                       (yyvsp[0].slval)),
                          1);
        }
#line 7702 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 443:
#line 4879 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
#line 7710 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 444:
#line 4883 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 7732 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 445:
#line 4901 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.slval) = 0;
        }
#line 7740 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 446:
#line 4908 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 7769 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 447:
#line 4936 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 7807 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 450:
#line 4979 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 7835 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 451:
#line 5006 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 7878 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 452:
#line 5045 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
#line 7886 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 453:
#line 5049 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
#line 7894 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 454:
#line 5053 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 7907 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 455:
#line 5066 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
#line 7915 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 456:
#line 5070 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 7923 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 457:
#line 5074 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 7961 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 458:
#line 5111 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 7969 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 459:
#line 5115 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 7977 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 460:
#line 5119 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.idlist) = 0;
        }
#line 7985 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 463:
#line 5131 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 7993 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 464:
#line 5135 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8001 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 465:
#line 5139 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 8009 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 466:
#line 5143 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8017 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 467:
#line 5147 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
        }
#line 8025 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 468:
#line 5151 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8033 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 469:
#line 5155 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
        }
#line 8041 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 470:
#line 5159 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8049 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 471:
#line 5163 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
        }
#line 8057 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 472:
#line 5167 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8065 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 473:
#line 5171 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 8073 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 474:
#line 5175 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8081 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 475:
#line 5179 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 8089 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 476:
#line 5183 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8097 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 477:
#line 5189 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 8188 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 478:
#line 5279 "fe/idl.ypp" /* yacc.c:1646  */
    {
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8198 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 479:
#line 5285 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 8225 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 480:
#line 5310 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 8332 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 481:
#line 5416 "fe/idl.ypp" /* yacc.c:1646  */
    {
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          (yyval.bval) = (yyvsp[0].bval);
        }
#line 8342 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 482:
#line 5425 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.bval) = true;
        }
#line 8350 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 483:
#line 5429 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.bval) = false;
        }
#line 8358 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 484:
#line 5435 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 8432 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 485:
#line 5507 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 8506 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 486:
#line 5579 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 8580 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 487:
#line 5652 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 8621 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 488:
#line 5689 "fe/idl.ypp" /* yacc.c:1646  */
    {
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 8632 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 489:
#line 5699 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
#line 8640 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 490:
#line 5703 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
#line 8648 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 491:
#line 5707 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 8656 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 492:
#line 5711 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
#line 8664 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 493:
#line 5715 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
#line 8672 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 494:
#line 5719 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
#line 8680 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 495:
#line 5723 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 8730 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 496:
#line 5772 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 8738 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 497:
#line 5776 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8746 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 498:
#line 5780 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.idlist) = 0;
        }
#line 8754 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 499:
#line 5788 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8762 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 500:
#line 5792 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.idlist) = 0;
        }
#line 8770 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 501:
#line 5799 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
#line 8778 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 502:
#line 5803 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
#line 8786 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 503:
#line 5807 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
#line 8794 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 507:
#line 5820 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
#line 8802 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 508:
#line 5824 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8810 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 509:
#line 5828 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
#line 8818 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 510:
#line 5832 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8826 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 511:
#line 5840 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 8853 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 512:
#line 5863 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 8861 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 513:
#line 5867 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 8883 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 514:
#line 5889 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 8914 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 515:
#line 5916 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 8922 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 516:
#line 5920 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 8944 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 522:
#line 5953 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 8971 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 523:
#line 5981 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 8998 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 524:
#line 6008 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 9046 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 525:
#line 6052 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9054 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 526:
#line 6056 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9062 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 527:
#line 6060 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9075 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 528:
#line 6074 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9083 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 529:
#line 6083 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 9100 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 530:
#line 6100 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9110 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 531:
#line 6109 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 9118 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 532:
#line 6113 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 9151 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 533:
#line 6146 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 9204 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 534:
#line 6195 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9212 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 535:
#line 6199 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9220 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 536:
#line 6203 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9233 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 537:
#line 6215 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9241 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 538:
#line 6219 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9249 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 539:
#line 6226 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.ntval) = AST_Decl::NT_type;
        }
#line 9257 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 540:
#line 6230 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.ntval) = AST_Decl::NT_struct;
        }
#line 9265 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 541:
#line 6234 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.ntval) = AST_Decl::NT_union;
        }
#line 9273 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 542:
#line 6238 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.ntval) = AST_Decl::NT_eventtype;
        }
#line 9281 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 543:
#line 6242 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.ntval) = AST_Decl::NT_sequence;
        }
#line 9289 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 544:
#line 6246 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.ntval) = AST_Decl::NT_interface;
        }
#line 9297 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 545:
#line 6250 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.ntval) = AST_Decl::NT_valuetype;
        }
#line 9305 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 546:
#line 6254 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.ntval) = AST_Decl::NT_enum;
        }
#line 9313 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 547:
#line 6258 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.ntval) = AST_Decl::NT_except;
        }
#line 9321 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 548:
#line 6262 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.ntval) = AST_Decl::NT_const;
          t_param_const_type = (yyvsp[0].etval);
        }
#line 9330 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 549:
#line 6270 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 9364 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 550:
#line 6303 "fe/idl.ypp" /* yacc.c:1646  */
    {
          // Maybe add a new parse state to set here.
        }
#line 9372 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 551:
#line 6307 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 9391 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 552:
#line 6322 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.plval) = 0;
        }
#line 9399 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 553:
#line 6329 "fe/idl.ypp" /* yacc.c:1646  */
    {

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
#line 9428 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 554:
#line 6354 "fe/idl.ypp" /* yacc.c:1646  */
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
#line 9447 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 555:
#line 6372 "fe/idl.ypp" /* yacc.c:1646  */
    {
          ACE_NEW_RETURN ((yyval.slval),
                          UTL_StrList ((yyvsp[-1].sval),
                                       (yyvsp[0].slval)),
                          1);
        }
#line 9458 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 556:
#line 6382 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 9480 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 557:
#line 6400 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.slval) = 0;
        }
#line 9488 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 558:
#line 6407 "fe/idl.ypp" /* yacc.c:1646  */
    {
          ACE_NEW_RETURN ((yyval.sval),
                          UTL_String ((yyvsp[0].strval), true),
                          1);
        }
#line 9498 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 559:
#line 6416 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSeen);
        }
#line 9506 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 560:
#line 6420 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 9528 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 561:
#line 6438 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSqSeen);
        }
#line 9536 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 562:
#line 6442 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeBodySeen);
        }
#line 9544 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 563:
#line 6446 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeQsSeen);

          // Done with this port type - pop it off the scopes stack.
          idl_global->scopes ().pop ();
        }
#line 9555 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 564:
#line 6456 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 9562 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 565:
#line 6462 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 9569 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 566:
#line 6465 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 9576 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 567:
#line 6471 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 9583 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 568:
#line 6474 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 9590 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 569:
#line 6477 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 9597 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 570:
#line 6480 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 9604 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 571:
#line 6483 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 9612 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 572:
#line 6487 "fe/idl.ypp" /* yacc.c:1646  */
    {
        }
#line 9619 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 573:
#line 6493 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 9692 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 574:
#line 6562 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 9743 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 575:
#line 6612 "fe/idl.ypp" /* yacc.c:1646  */
    {
          if ((yyvsp[0].alval) == 0)
            {
              ACE_NEW_RETURN ((yyvsp[0].alval),
                              FE_Utils::T_ARGLIST,
                              1);
            }

          (yyvsp[0].alval)->enqueue_head ((yyvsp[-1].dcval));
          (yyval.alval) = (yyvsp[0].alval);
        }
#line 9759 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 576:
#line 6627 "fe/idl.ypp" /* yacc.c:1646  */
    {
          if ((yyvsp[-3].alval) == 0)
            {
              ACE_NEW_RETURN ((yyvsp[-3].alval),
                              FE_Utils::T_ARGLIST,
                              1);
            }

          (yyvsp[-3].alval)->enqueue_tail ((yyvsp[0].dcval));
          (yyval.alval) = (yyvsp[-3].alval);
        }
#line 9775 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 577:
#line 6639 "fe/idl.ypp" /* yacc.c:1646  */
    {
          (yyval.alval) = 0;
        }
#line 9783 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 578:
#line 6646 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 9843 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 580:
#line 6709 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSeen);
        }
#line 9851 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 581:
#line 6713 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorIDSeen);
        }
#line 9859 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 582:
#line 6717 "fe/idl.ypp" /* yacc.c:1646  */
    {
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
#line 9915 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 583:
#line 6772 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSqSeen);
        }
#line 9923 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 584:
#line 6776 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorBodySeen);
        }
#line 9931 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 585:
#line 6780 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorQsSeen);

          // Done with this connector - pop it off the scope stack.
          idl_global->scopes ().pop ();
        }
#line 9942 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 588:
#line 6795 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 9950 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 589:
#line 6799 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9958 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 590:
#line 6803 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 9966 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 591:
#line 6807 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9974 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 592:
#line 6811 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 9982 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 593:
#line 6815 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9990 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 594:
#line 6819 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 9998 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;

  case 595:
#line 6823 "fe/idl.ypp" /* yacc.c:1646  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10006 "fe/idl.tab.cpp" /* yacc.c:1646  */
    break;


#line 10010 "fe/idl.tab.cpp" /* yacc.c:1646  */
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
#line 6828 "fe/idl.ypp" /* yacc.c:1906  */

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
