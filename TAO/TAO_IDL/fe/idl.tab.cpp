/* A Bison parser, made by GNU Bison 3.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.1"

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
#include "ast_annotation_appl.h"
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
  AST_Annotation_Appl *annotation_val;
  AST_Annotation_Appls *annotations_val;
  AST_Annotation_Appl::Param *annotation_param_val;
  AST_Annotation_Appl::Params *annotation_params_val;
  AST_Decls *decls_val;
  Decl_Annotations_Pair *decl_annotations_pair_val;

#line 326 "fe/idl.tab.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE tao_yylval;

int tao_yyparse (void);

#endif /* !YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 343 "fe/idl.tab.cpp" /* yacc.c:358  */

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
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
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
#  define YYSIZE_T unsigned
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

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
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
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1630

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  109
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  398
/* YYNRULES -- Number of rules.  */
#define YYNRULES  594
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  872

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   342

#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

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
       2,   107,     2,   108,    97,     2,     2,     2,     2,     2,
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
      85,    86,    87
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   382,   382,   385,   386,   394,   407,   413,   417,   421,
     429,   428,   438,   437,   447,   446,   456,   455,   465,   464,
     474,   473,   483,   482,   492,   491,   501,   500,   510,   509,
     519,   518,   528,   527,   537,   536,   546,   545,   555,   554,
     568,   567,   579,   618,   622,   578,   638,   646,   660,   670,
     700,   704,   645,   727,   731,   732,   736,   737,   742,   747,
     741,   833,   838,   832,   907,   908,   913,   951,   955,   912,
     971,   970,   982,  1019,  1049,  1082,  1081,  1090,  1097,  1098,
    1099,  1100,  1104,  1114,  1119,  1162,  1166,  1118,  1193,  1236,
    1240,  1191,  1257,  1255,  1295,  1294,  1306,  1310,  1317,  1322,
    1330,  1355,  1381,  1445,  1446,  1450,  1451,  1452,  1458,  1457,
    1466,  1465,  1476,  1477,  1482,  1481,  1490,  1489,  1498,  1497,
    1506,  1505,  1514,  1513,  1522,  1521,  1530,  1529,  1538,  1537,
    1549,  1561,  1559,  1584,  1591,  1601,  1600,  1626,  1624,  1649,
    1659,  1670,  1714,  1741,  1771,  1775,  1779,  1783,  1770,  1845,
    1846,  1847,  1848,  1849,  1850,  1851,  1855,  1859,  1927,  1929,
    1931,  1932,  1944,  1945,  1957,  1958,  1970,  1971,  1980,  1992,
    1993,  2002,  2014,  2015,  2024,  2033,  2045,  2046,  2055,  2064,
    2076,  2126,  2127,  2134,  2138,  2143,  2150,  2154,  2158,  2163,
    2167,  2171,  2175,  2182,  2250,  2277,  2278,  2282,  2283,  2284,
    2288,  2292,  2293,  2297,  2301,  2302,  2306,  2315,  2321,  2328,
    2341,  2345,  2352,  2361,  2368,  2376,  2382,  2389,  2400,  2399,
    2408,  2412,  2416,  2420,  2446,  2454,  2453,  2525,  2526,  2530,
    2537,  2538,  2564,  2565,  2566,  2567,  2568,  2569,  2570,  2571,
    2575,  2576,  2577,  2578,  2582,  2583,  2584,  2588,  2589,  2593,
    2605,  2603,  2628,  2635,  2636,  2640,  2652,  2650,  2675,  2682,
    2698,  2716,  2717,  2721,  2725,  2729,  2736,  2740,  2744,  2751,
    2755,  2759,  2766,  2773,  2777,  2784,  2791,  2798,  2805,  2813,
    2812,  2826,  2857,  2861,  2825,  2878,  2881,  2882,  2886,  2886,
    2905,  2909,  2904,  2973,  2972,  2985,  2984,  2997,  3001,  3034,
    3038,  3094,  3098,  2996,  3120,  3127,  3140,  3149,  3156,  3157,
    3263,  3266,  3267,  3272,  3276,  3271,  3309,  3308,  3320,  3330,
    3348,  3356,  3355,  3369,  3373,  3368,  3389,  3388,  3439,  3464,
    3488,  3492,  3523,  3527,  3487,  3551,  3556,  3554,  3560,  3564,
    3600,  3604,  3598,  3691,  3761,  3770,  3760,  3784,  3794,  3798,
    3792,  3843,  3868,  3877,  3881,  3875,  3915,  3939,  3947,  3946,
    3986,  3996,  4014,  4022,  4026,  4021,  4086,  4087,  4092,  4096,
    4100,  4104,  4091,  4171,  4175,  4179,  4183,  4170,  4259,  4263,
    4292,  4296,  4258,  4312,  4316,  4380,  4384,  4310,  4419,  4424,
    4429,  4436,  4437,  4448,  4453,  4494,  4447,  4514,  4513,  4522,
    4521,  4532,  4537,  4535,  4541,  4546,  4550,  4545,  4589,  4588,
    4597,  4596,  4607,  4612,  4610,  4616,  4621,  4625,  4620,  4670,
    4677,  4678,  4679,  4786,  4790,  4794,  4802,  4806,  4801,  4815,
    4823,  4827,  4822,  4836,  4844,  4848,  4843,  4857,  4865,  4869,
    4864,  4878,  4885,  4897,  4895,  4918,  4925,  4953,  4990,  4991,
    4995,  5024,  5063,  5067,  5023,  5084,  5088,  5082,  5129,  5128,
    5136,  5143,  5144,  5149,  5148,  5157,  5156,  5165,  5164,  5173,
    5172,  5181,  5180,  5189,  5188,  5197,  5196,  5206,  5296,  5302,
    5327,  5433,  5442,  5446,  5452,  5524,  5596,  5670,  5669,  5717,
    5721,  5725,  5729,  5733,  5737,  5716,  5790,  5789,  5797,  5804,
    5809,  5817,  5821,  5816,  5831,  5832,  5836,  5838,  5837,  5846,
    5845,  5858,  5881,  5856,  5907,  5934,  5905,  5958,  5959,  5960,
    5964,  5965,  5969,  5996,  6026,  6070,  6074,  6024,  6089,  6098,
    6116,  6127,  6126,  6164,  6213,  6217,  6162,  6232,  6236,  6243,
    6247,  6251,  6255,  6259,  6263,  6267,  6271,  6275,  6279,  6287,
    6318,  6331,  6338,  6363,  6381,  6388,  6403,  6410,  6420,  6424,
    6442,  6446,  6419,  6459,  6465,  6468,  6475,  6474,  6481,  6480,
    6487,  6486,  6496,  6565,  6615,  6630,  6642,  6649,  6708,  6713,
    6717,  6712,  6776,  6780,  6775,  6793,  6794,  6799,  6798,  6807,
    6806,  6815,  6814,  6823,  6822
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
  "'*'", "'/'", "'%'", "'~'", "'('", "')'", "'['", "']'", "$accept",
  "start", "definitions", "at_least_one_definition", "definition",
  "fixed_definition", "$@1", "$@2", "$@3", "$@4", "$@5", "$@6", "$@7",
  "$@8", "$@9", "$@10", "$@11", "$@12", "$@13", "$@14", "$@15",
  "module_header", "$@16", "module", "@17", "$@18", "$@19",
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
  "formal_parameters", "formal_parameter",
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
     335,   336,   337,   338,   339,   340,   341,   342,    59,   123,
     125,    60,    62,    58,    44,    61,   124,    94,    38,    43,
      45,    42,    47,    37,   126,    40,    41,    91,    93
};
# endif

#define YYPACT_NINF -600

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-600)))

#define YYTABLE_NINF -536

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -600,    68,  1292,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,    46,    80,   106,    56,  -600,    46,
      46,  -600,    53,    53,  -600,  -600,    94,  -600,  -600,    13,
    -600,  1555,    25,    92,  -600,  -600,    18,  -600,  -600,  -600,
    -600,  -600,  -600,   569,  -600,  -600,  -600,  -600,  -600,  1423,
      97,  -600,  -600,   102,  -600,   136,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,    99,  -600,  -600,  -600,    99,  -600,
    -600,   114,   126,   336,    53,    46,  1532,    46,    46,    46,
      46,  -600,  -600,  -600,    86,    46,   119,  -600,   130,    46,
    -600,    99,    46,   141,   145,    46,  -600,  -600,   -18,  -600,
      15,  -600,  -600,   133,  -600,   147,   160,  1563,  -600,  -600,
    -600,   172,   214,  -600,   200,   212,   220,   152,  -600,    85,
    -600,  -600,  -600,  -600,  -600,  -600,    82,  -600,  -600,  -600,
    -600,  -600,  -600,   230,  -600,  -600,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
    -600,   136,  -600,  -600,  -600,    38,  -600,  -600,   221,  -600,
     223,   227,   228,  -600,    53,   231,   235,   233,  -600,   237,
     238,   239,   240,   245,   247,   248,   250,  -600,  -600,  -600,
     253,   255,  -600,  -600,  -600,  -600,   230,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,   230,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,   266,  -600,   251,  -600,  -600,
     226,  -600,   333,  -600,  -600,  -600,    40,    44,  -600,  -600,
    -600,   336,  -600,  -600,  -600,  -600,   267,  -600,  -600,  -600,
    -600,   356,  -600,  -600,    41,   268,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,   355,  -600,   120,   273,   318,  -600,
    -600,  -600,  -600,  -600,  -600,     2,  -600,  -600,   262,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,   318,   279,
     281,  -600,  -600,  -600,    46,    46,   282,   283,  -600,  -600,
    -600,   280,  -600,   333,  -600,  -600,  -600,   510,  -600,  -600,
     371,  -600,   284,   285,  -600,  -600,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,   203,   203,   203,   120,   230,  -600,
    -600,   286,   278,   288,   157,   155,   125,  -600,  -600,  -600,
    -600,  -600,    53,  -600,  -600,  -600,  -600,   287,  -600,    53,
    -600,   120,   120,   120,   120,  -600,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,   201,  -600,   271,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,    53,   318,  -600,  -600,  -600,   291,
     226,  -600,  -600,  -600,   296,   358,    46,  -600,  -600,  -600,
    -600,  1345,   298,   297,  -600,  1563,  -600,  -600,  -600,   293,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     306,    46,  -600,   230,  1074,   822,   120,  -600,  -600,  -600,
    -600,  -600,   295,  -600,   300,  -600,   309,  -600,  -600,  -600,
    -600,   234,   990,  -600,  -600,  -600,  -600,    84,   334,    53,
      53,  -600,  -600,  -600,  -600,  -600,    84,  -600,   314,  -600,
     315,   302,   317,  -600,  -600,  1109,   230,  -600,    53,   318,
    -600,  -600,  -600,   408,  -600,   323,   412,   329,  -600,  -600,
     278,   288,   157,   155,   155,   125,   125,  -600,  -600,  -600,
    -600,  -600,   325,  -600,  -600,  -600,   331,  -600,  -600,  -600,
    -600,  -600,  -600,   672,  -600,  -600,  -600,  -600,  -600,   332,
    -600,  -600,  -600,   335,   337,   880,   338,   339,   340,   120,
    -600,   333,  -600,   343,   344,   623,   134,    53,    53,    53,
    -600,   345,  -600,  -600,  -600,  -600,  -600,  -600,  -600,    46,
      46,  -600,   346,  -600,  -600,  -600,  1193,   906,   393,   706,
    -600,   230,   333,  -600,  -600,    70,    73,   349,   350,   351,
     333,   352,  -600,  -600,  -600,  -600,  -600,  -600,    48,  -600,
    -600,   353,   354,   230,  -600,   151,   336,  -600,  -600,  -600,
      41,  -600,   357,  -600,   359,   360,   361,   366,   367,  -600,
     230,  -600,  -600,  -600,  -600,  -600,   368,   369,  -600,  1513,
    1513,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,   295,
     309,  -600,  -600,  -600,  -600,  -600,   417,   230,  -600,  -600,
    -600,  -600,  -600,  -600,    81,    81,    81,   370,  -600,   372,
     375,   380,   381,   382,   383,  -600,  -600,  -600,   384,   387,
     386,   388,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
    -600,  -600,   120,   120,   302,    46,  -600,   390,   347,   391,
    -600,   362,  -600,  -600,  -600,  -600,  -600,  -600,  1370,  -600,
      53,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
      49,  -600,  -600,  -600,   394,    46,   321,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,  -600,   341,   341,  -600,
    -600,  -600,  -600,   706,    46,  -600,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,   400,   402,   404,   405,   151,    53,  -600,
    -600,  -600,  -600,   406,   230,    51,  -600,  -600,  -600,  -600,
     407,  -600,  -600,  -600,  -600,  -600,   389,  -600,    53,  -600,
    -600,  -600,  -600,  -600,   230,   409,  1238,  -600,  -600,   341,
     410,   415,   423,   424,   458,   458,    46,   457,   421,  -600,
    -600,   230,   425,  -600,  -600,   427,  -600,  -600,  -600,  -600,
    -600,   434,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
     484,   541,   439,  -600,   458,   209,   706,  -600,   452,   442,
     458,   450,   494,    46,    53,  -600,  -600,   467,  -600,   454,
     210,   458,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,   230,  -600,
     468,  -600,  -600,  -600,  -600,   455,  -600,  -600,  -600,   475,
     120,   471,   476,    71,  -600,   222,    46,   424,    53,    53,
     460,    46,   541,  -600,   473,   706,   529,  -600,  -600,  -600,
    -600,  -600,  1487,  -600,  -600,  -600,   463,   465,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,   480,  -600,  -600,  -600,  -600,
      53,   210,    46,   470,  -600,    46,   488,   490,  -600,  -600,
    -600,  -600,  -600,  -600,   522,  -600,   492,   505,  -600,  -600,
     526,  -600
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,     0,     3,     1,    38,   144,    40,    70,   218,
     279,   295,   330,   378,     0,     0,     0,     0,    94,     0,
       0,   489,     0,     0,   558,   579,     0,     6,     7,    42,
      24,    61,     0,     0,    22,    64,    77,    66,    26,    78,
      83,    79,    84,    77,    80,    81,    65,    18,    10,     0,
       0,    12,   224,   281,   220,   329,   221,   247,   248,   222,
      20,    14,    16,    28,   449,   448,   451,    30,   487,    32,
     519,   521,   520,   518,    77,   537,   538,   517,    77,    34,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   140,   259,   223,    77,     0,    77,    88,    77,     0,
      82,    77,     0,   455,   530,     0,   139,   135,     0,   134,
       0,   208,   208,     0,    46,     0,     0,     0,   208,     8,
       9,     0,    97,    72,     0,     0,     0,   263,   265,     0,
     269,   270,   273,   274,   275,   276,   272,   277,   278,   344,
     352,   357,    92,   231,   102,   227,   229,   230,   228,   232,
     261,   262,   233,   237,   234,   236,   235,   238,   239,   281,
     244,     0,   245,   246,   240,     0,   243,   241,   351,   242,
     356,     0,     0,     5,     0,   206,     0,     0,   297,     0,
       0,     0,     0,     0,     0,     0,     0,   531,   524,   533,
       0,     0,   582,   578,    39,   272,   157,   145,   149,   153,
     154,   150,   151,   152,   155,   156,    41,    71,   219,   225,
     280,   296,   331,   379,    73,   528,    74,     0,   529,    95,
     460,   490,     0,   446,   137,   447,     0,     0,   196,    43,
      25,     0,   544,   540,   541,   546,   543,   547,   545,   542,
     539,     0,    48,   551,     0,     0,    23,    96,    75,    67,
      27,    85,   264,   271,   266,   268,     0,     0,    99,   343,
     340,   348,   353,    19,    11,   211,    13,   282,     0,    21,
      15,    17,    29,   452,    31,   501,   488,    33,    99,     0,
       0,    35,    37,   586,     0,     0,     0,     0,    89,   458,
     456,   498,   136,     0,   559,   207,   580,     0,     4,   548,
       0,   552,     0,   549,   183,   184,   185,   187,   190,   189,
     191,   192,   188,   186,     0,     0,     0,     0,   180,   577,
     158,   159,   160,   162,   164,   166,   169,   172,   176,   181,
     576,    62,     0,   113,   104,   267,   193,     0,   345,     0,
      93,     0,     0,     0,     0,   209,   288,   298,   462,   505,
     532,   525,   534,   583,   146,   259,   226,   252,   253,   254,
     260,   332,   380,   113,     0,    99,   496,   491,   138,     0,
     460,   194,   198,   201,     0,   204,     0,   205,   197,   199,
     202,     0,     3,     0,    49,     0,   177,   178,   179,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     574,     0,    76,   133,     0,     0,     0,   208,    98,   341,
     349,   354,   134,   212,     0,   213,   216,   283,   287,   208,
     208,   453,     0,   113,   104,   368,   373,     0,   483,     0,
       0,   591,   366,   367,   587,   589,     0,   593,     0,   585,
       0,     0,   249,   208,   287,     0,   459,   457,     0,    99,
     560,   581,   195,     0,   200,     0,     0,     0,   550,   182,
     161,   163,   165,   167,   168,   170,   171,   173,   174,   175,
     208,    63,   130,   128,   388,   389,     0,   112,   120,   114,
     124,   122,   126,     0,   116,   118,   393,   110,   108,     0,
     103,   105,   106,     0,     0,     0,     0,     0,     0,     0,
     210,     0,   214,     0,   288,     0,     0,     0,     0,     0,
     473,     0,   461,   463,   465,   467,   469,   471,   475,     0,
       0,   506,     0,   504,   507,   509,     0,     0,     0,     0,
     479,   478,     0,   482,   481,     0,     0,     0,     0,     0,
       0,     0,   584,   147,   363,   359,   362,   250,     0,   333,
     338,   288,     0,   497,   492,     0,     0,    45,   553,    50,
       0,   131,     0,    69,     0,     0,     0,     0,     0,   392,
     422,   419,   420,   421,   383,   391,     0,     0,   208,     0,
       0,    87,   107,   347,   346,   342,   350,   355,   217,     0,
     216,   284,   286,   293,   290,   289,   263,   309,   304,   305,
     306,   307,   299,   308,     0,     0,     0,     0,   454,     0,
       0,     0,     0,     0,     0,   511,   514,   503,     0,     0,
       0,     0,   369,   374,   477,   572,   573,   592,   588,   590,
     480,   594,     0,     0,   360,     0,   339,     0,   335,     0,
      91,     0,   570,   566,   568,   561,   565,   203,     0,   575,
       0,   129,   121,   115,   125,   123,   127,   208,   117,   119,
       0,   111,   109,   215,     0,     0,     0,   486,   484,   485,
     474,   464,   466,   468,   470,   472,   476,     0,     0,   508,
     510,   527,   536,     0,     0,   148,   364,   361,   251,   334,
     336,   382,   493,     0,     0,     0,     0,   563,     0,    56,
      42,    51,    55,     0,   132,     0,   394,   294,   291,   300,
     397,   512,   515,   370,   375,   258,     0,   208,     0,   571,
     567,   569,   562,   564,    58,     0,     0,    57,   384,     0,
       0,     0,     0,     0,   429,   429,     0,   433,   255,   365,
     337,   494,     0,    52,    54,     0,   395,   292,   301,   398,
     405,     0,   404,   426,   513,   516,   371,   430,   376,   256,
     500,     0,   410,   385,   429,     0,     0,   400,   401,     0,
     429,     0,   437,     0,     0,   495,   557,     0,   556,     0,
       0,   429,   396,   316,   323,   321,   302,   312,   313,   320,
     406,   402,   427,   372,   431,   434,   377,   257,   499,    59,
     554,   409,   423,   424,   425,     0,   415,   416,   386,     0,
       0,     0,     0,     0,   208,   318,     0,     0,     0,     0,
       0,     0,     0,   411,   412,     0,   441,   317,   324,   322,
     303,   311,     0,   319,   407,   403,     0,     0,   435,    60,
     555,   413,   417,   438,   387,     0,   326,   314,   428,   432,
       0,     0,     0,     0,   325,     0,     0,     0,   414,   418,
     439,   327,   315,   436,     0,   445,     0,   442,   440,   443,
       0,   444
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -600,  -600,   304,   305,   555,  -560,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
    -600,  -557,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,  -121,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,   252,  -600,
    -600,    93,  -600,  -600,  -600,   589,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,   591,  -600,   256,  -600,  -600,  -241,  -600,
    -600,   184,  -600,  -600,  -600,  -600,  -349,  -355,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,  -313,  -600,  -600,   -22,
    -600,  -600,  -211,   -10,  -600,     4,  -600,  -600,  -600,  -600,
    -199,   -21,  -236,  -600,   219,   224,   218,   -96,   -91,  -140,
     -23,  -600,  -302,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
    -600,   -87,   562,  -600,  -600,  -600,    23,   113,    16,  -600,
    -600,  -600,   -40,   122,  -470,  -600,  -600,  -600,   -47,  -600,
    -600,  -599,  -117,  -600,  -600,    -7,  -600,   -56,  -600,  -600,
     -48,   -45,   -55,   -54,   -53,   324,  -600,   -38,  -600,   -31,
    -600,  -600,  -600,  -600,   178,   277,  -600,  -273,  -600,  -600,
    -600,   -28,  -600,   -26,  -600,  -600,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -188,  -600,  -600,  -600,  -600,  -600,  -186,
    -600,  -600,  -600,  -600,  -600,  -600,  -600,   -41,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,   -90,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,   -67,  -600,  -600,  -600,   -63,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,    11,  -600,  -600,  -320,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,    17,
    -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,  -576,  -600,  -600,  -600,  -600,
    -600,  -166,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
    -198,  -600,  -600,  -495,  -600,  -579,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
      20,    21,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
    -600,   289,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
    -600,  -600,  -311,   236,  -282,  -600,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,   576,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,   270,  -600,  -600,  -164,
    -600,  -600,  -600,  -600,  -600,  -600,  -600,   -37,  -600,  -600,
    -600,   242,  -600,  -600,    96,  -600,  -600,  -600,  -600,  -600,
    -600,  -600,  -600,  -600,  -600,  -600,  -600,  -600
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     3,    27,    28,   172,   176,   180,   181,
     171,   179,   121,   116,   125,   182,   184,   186,   190,   191,
      82,    29,    84,    30,   115,   298,   455,    31,    32,   117,
     302,   457,   648,   725,   701,   726,   702,   703,   742,   821,
      33,   118,   401,    34,    35,   124,   333,   476,    36,    85,
      37,   142,   332,    38,    39,    40,   126,   334,   489,    41,
     217,   363,   552,    42,   258,    43,   102,   248,   340,    44,
      45,   405,   490,   491,   580,   579,   404,   477,   565,   576,
     577,   564,   567,   566,   568,   562,   402,   472,   650,   318,
     222,   293,   109,   355,    46,   478,    83,   284,   440,   632,
     197,   319,   336,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   337,    48,   297,   374,   375,   376,   377,   378,
      49,    50,   295,   345,   414,   415,   502,   416,   479,    86,
     208,   285,   594,   145,   146,   147,   148,    52,   356,   442,
     635,   357,   714,   738,   773,   358,   359,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    53,    87,    54,
     177,   346,   503,   417,   504,   592,   419,   595,   665,   730,
     664,    55,    88,    56,   268,   420,   666,   731,   765,   812,
     602,   786,   813,   787,   814,   856,   809,   788,   815,   789,
     811,   810,   845,   847,   855,    57,    58,    59,    89,   286,
     443,   637,   549,   638,   717,   550,   164,   341,   496,   165,
     257,   407,   166,   167,   342,   497,   168,   169,   343,   498,
     170,   360,   441,   545,   634,   546,   633,   716,   480,   432,
     528,   683,   736,   770,   433,   529,   684,   737,   772,   481,
      90,   287,   444,   639,   482,   657,   745,   781,   826,   483,
     574,   493,   578,   729,   764,   711,   732,   733,   751,   768,
     817,   752,   766,   816,   763,   779,   780,   805,   824,   851,
     806,   825,   852,   575,   807,   754,   769,   818,   758,   771,
     819,   796,   820,   850,   844,   853,   864,   866,   867,   870,
     484,   485,    63,    64,    65,   183,   348,   511,    66,   220,
     365,   290,   364,   421,   512,   609,   610,   611,   612,   613,
     607,   614,   643,   532,   644,   436,   534,   515,   516,   517,
      67,   185,    68,   105,   291,   449,   641,   718,   760,   367,
     448,   775,   276,   349,   522,   422,   523,   618,   619,   524,
     677,   734,   525,   678,   735,    69,    70,    71,    72,    73,
     279,   423,   620,    74,    75,    76,   188,   278,    77,   280,
     424,   621,    78,   241,   242,   303,   243,   777,   800,   778,
      79,   111,   369,   555,   696,   645,   697,   646,   694,   695,
     693,   437,   245,   400,   330,    80,    81,   112,   370,   193,
     283,   438,   353,   439,   538,   539,   537,   541
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     108,   110,   163,   144,    92,   159,    47,    93,   320,   103,
     104,   292,   160,   571,   445,   161,   204,   162,    51,    60,
     205,   143,    61,    62,   226,   227,   408,   198,   201,   202,
     203,   244,   299,   431,   623,   199,   688,   350,   200,   409,
     410,   411,   434,   294,   106,   163,   209,   296,   159,    91,
     492,   636,   706,    47,   728,   160,   106,   223,   161,   571,
     162,   196,   206,   224,   143,    51,    60,   521,     4,    61,
      62,   435,   783,   625,   526,   207,   626,   210,   211,   212,
     213,   389,   368,   224,   106,   215,     8,   106,   699,   218,
     225,   700,   219,   254,   255,   221,   224,   113,   373,   784,
     785,   510,   712,   530,   114,    18,  -141,   344,   413,    99,
     513,   122,     8,   119,   304,   305,   306,   307,   308,   309,
     310,   311,   107,   106,   447,   312,   313,   174,   174,   123,
     259,   174,   260,   412,   107,   174,   174,   106,   174,   514,
     314,   315,   596,   128,   129,   316,   317,   132,   133,   134,
     135,   224,   265,   746,   224,    18,   755,    12,   178,    95,
     252,  -310,   224,   253,   204,   107,   699,   187,   205,   700,
     494,   187,   492,   256,  -142,   198,   201,   202,   203,   122,
     120,   425,   426,   199,   174,   782,   200,   214,   713,   216,
    -328,   793,   122,   304,   305,   306,   307,   308,   309,   310,
     311,   107,   808,   192,   312,   313,   106,  -143,   554,   196,
     783,   427,   122,   571,   194,   107,   428,   834,  -100,   314,
     315,   174,   228,   122,   316,   317,   397,   398,   399,  -450,
     204,   425,   426,  -522,   205,   642,   229,   784,   785,   393,
     394,   198,   201,   202,   203,   802,   803,   804,   230,   199,
     784,   785,   200,   859,   395,   396,   861,   467,   468,   469,
     246,   427,   247,   588,   425,   426,   428,    94,    96,   429,
     430,   790,    98,   101,   354,   196,   304,   305,   306,   307,
     308,   309,   310,   311,   107,   507,   508,   312,   313,   249,
     589,   386,   387,   388,   427,   509,   571,   463,   464,   428,
     250,   372,   429,   430,   465,   466,   661,   662,   317,   251,
     403,   224,   261,   379,   262,   263,   264,   403,  -207,   289,
     495,   624,   267,   266,   320,   269,   270,   271,   272,   630,
     842,   686,   505,   506,   273,   274,   106,   275,   277,   106,
     288,   281,   446,   282,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   195,  -523,   571,   548,   647,   300,   301,
     331,   140,   141,   335,   338,   339,    92,   347,   351,   454,
     352,   361,   362,   366,   383,   391,   384,   642,  -358,   385,
     450,   406,   390,   560,   452,    47,   392,   453,   -44,   456,
     499,   471,   533,   667,   668,   669,   320,    51,    60,   459,
     470,    61,    62,   501,   542,   531,   500,   535,   536,   544,
     543,   547,   556,   557,   531,   558,   572,   107,   559,   561,
     573,   563,   581,   582,   622,   252,   553,   709,   692,   583,
     585,   586,   587,   591,  -285,   608,   617,   627,   628,   629,
     631,   690,  -399,  -381,   640,   651,   710,   652,   653,   654,
     598,   599,   600,   601,   655,   656,   658,   659,   670,   750,
     671,   570,   572,   672,   163,   603,   573,   159,   673,   674,
     675,   676,   679,   143,   160,   680,   681,   161,   682,   162,
     689,   691,   707,   143,   597,   604,   605,   606,   719,   204,
     720,   660,   721,   205,   727,   722,   753,   739,   747,   743,
     198,   201,   202,   203,   748,   836,   837,   570,   199,   615,
     616,   200,   757,   106,     6,   759,   761,     9,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   195,   137,   749,
      10,    11,   762,    12,   196,   140,   141,   857,   163,   163,
     767,   159,   159,   774,   776,  -408,   791,   792,   160,   160,
      14,   161,   161,   162,   162,   794,   795,   143,   143,   799,
     801,   823,   822,   827,   829,   838,   830,   841,   843,   848,
     705,   849,   106,   854,   828,   860,   862,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,    10,
      11,   107,    12,   139,   140,   141,   863,   865,   868,   869,
     371,   871,   381,   382,   173,   744,   100,    97,   527,   460,
     462,   685,   175,   663,   590,   461,   572,   584,   708,   756,
     573,   380,   551,   418,   593,   831,   106,   740,   704,   833,
     548,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    10,    11,   687,    12,   139,   140,   141,
     107,   835,    47,   858,   189,   458,   649,  -101,   840,   451,
     723,   570,   122,   518,    51,    60,     0,     0,    61,    62,
       0,     0,   540,     0,    92,   106,   724,   715,     0,     0,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   195,
     137,   138,     0,     0,     0,     0,   741,   140,   141,   572,
       0,     0,     0,   573,   107,     0,   569,     0,     0,   106,
     174,     0,     0,     0,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   195,   137,   138,    92,   832,     0,   715,
      47,   140,   141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,    60,   570,     0,    61,    62,     0,     0,
       0,     0,   798,   107,     0,     0,     0,     0,   572,     0,
       0,     0,   573,    92,     0,     0,   797,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   107,     0,     0,
       0,   163,   846,     0,   159,     0,   403,   403,     0,     0,
       0,   160,     0,   570,   161,     0,   162,     0,     0,     0,
     143,   839,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   473,     0,  -390,     6,     0,   403,     9,
    -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,
    -390,  -390,    10,    11,     0,    12,     0,  -390,  -390,    13,
       0,     0,   425,   426,   474,   475,  -390,     0,     0,     0,
       0,     0,    14,     0,     0,     0,   486,   487,   488,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   106,     0,    22,    23,     0,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
       0,     0,     0,  -390,   139,   140,   141,   473,     0,  -390,
       6,     0,   -86,     9,  -390,  -390,  -390,  -390,  -390,  -390,
    -390,  -390,  -390,  -390,  -390,  -390,    10,    11,     0,    12,
       0,  -390,  -390,    13,     0,     0,   425,   426,   474,   475,
    -390,     0,     0,     0,     0,     0,    14,     0,     0,     0,
     486,   487,   488,     0,     0,     0,     0,     0,     0,     0,
       0,   107,     0,     0,     0,     0,     0,   174,     0,    22,
      23,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -390,     0,     0,
       0,   473,     0,  -390,     6,     0,  -535,     9,  -390,  -390,
    -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,
      10,    11,     0,    12,     0,  -390,  -390,    13,     0,     0,
     425,   426,   474,   475,  -390,     0,     0,     0,     0,     0,
      14,     0,     0,     0,   519,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   520,     0,     0,     0,     0,     0,
       0,     0,     0,    22,    23,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -390,     0,     0,     0,   473,     0,  -390,     6,     0,
    -502,     9,  -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,
    -390,  -390,  -390,  -390,    10,    11,     0,    12,     0,  -390,
    -390,    13,     0,     0,   425,   426,   474,   475,  -390,     0,
     473,     0,  -390,     6,    14,     0,     9,  -390,  -390,  -390,
    -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,    10,
      11,     0,    12,     0,  -390,  -390,    13,    22,    23,   425,
     426,   474,   475,  -390,     0,     0,     0,     0,     0,    14,
       0,     0,     0,     0,     0,  -390,     0,     0,     0,     0,
       0,     0,     0,     0,   -68,     0,     0,     0,     0,     0,
       0,     0,    22,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -390,     0,     0,     0,   473,     0,  -390,     6,     0,   -90,
       9,  -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,  -390,
    -390,  -390,  -390,    10,    11,     0,    12,     0,  -390,  -390,
      13,     0,     0,   425,   426,   474,   475,  -390,     0,     0,
       0,     0,     0,    14,     0,     0,     0,     0,     0,     5,
       0,     0,     6,     7,     8,     9,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    22,    23,    10,    11,
       0,    12,     0,     0,     0,    13,     0,     0,     0,     0,
       0,     0,     0,     0,  -390,     0,     0,     0,    14,    15,
      16,    17,     0,  -526,     0,     0,     0,    18,    19,     0,
       0,    20,    -2,     5,    21,     0,     6,     7,     8,     9,
       0,    22,    23,     0,     0,     0,     0,     0,    24,    25,
     698,     0,    10,    11,     0,    12,     0,     0,     0,    13,
       0,     0,     0,     0,    26,     0,     0,     0,   -53,     0,
       0,     0,    14,    15,    16,    17,     0,     0,     0,     0,
       0,    18,    19,     0,     0,    20,     5,     0,    21,     6,
       7,     8,     9,     0,     0,    22,    23,     0,     0,     0,
       0,     0,    24,    25,     0,    10,    11,     0,    12,     0,
       0,     5,    13,     0,     6,     7,     8,     9,    26,  -208,
       0,     0,     0,     0,     0,    14,    15,    16,    17,     0,
      10,    11,     0,    12,    18,    19,     0,    13,    20,     0,
       0,    21,     0,     0,     0,     0,     0,     0,    22,    23,
      14,    15,    16,    17,     0,    24,    25,     0,     0,    18,
      19,     0,     0,    20,     5,     0,    21,     6,     7,     8,
       9,    26,  -208,    22,    23,     0,     0,     0,     0,     0,
      24,    25,   698,    10,    11,     0,    12,     0,     0,     0,
      13,     0,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,     0,    14,    15,    16,    17,     0,     0,     0,
       0,     0,    18,    19,     0,     0,    20,     0,     0,    21,
       0,     0,     0,     0,     0,     0,    22,    23,     0,     0,
     106,     0,     0,    24,    25,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    10,    11,    26,
      12,   139,   140,   141,   593,     0,   106,     0,     0,     0,
       0,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    10,    11,   106,    12,   139,   140,   141,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,    10,    11,     0,    12,   139,   140,   141,   -47,
       0,   -47,     0,     0,     0,     0,     0,   231,   107,   232,
       0,     0,     0,     0,   174,   -47,   -47,     0,   -47,   -47,
       0,     0,   -47,   233,   234,     0,   235,   236,     0,     0,
     237,     0,     0,     0,   107,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   -47,     0,     0,     0,   -47,     0,
       0,     0,   238,   107,     0,     0,   239,     0,     0,     0,
       0,     0,   -47,     0,     0,     0,     0,     0,     0,     0,
     240
};

static const yytype_int16 yycheck[] =
{
      22,    23,    43,    43,    14,    43,     2,    14,   244,    19,
      20,   222,    43,   483,   363,    43,    83,    43,     2,     2,
      83,    43,     2,     2,   111,   112,   339,    83,    83,    83,
      83,   118,   231,   353,   529,    83,   635,   278,    83,   341,
     342,   343,   353,     3,     3,    86,    86,     3,    86,     3,
     405,     3,     3,    49,     3,    86,     3,    75,    86,   529,
      86,    83,    84,    81,    86,    49,    49,   422,     0,    49,
      49,   353,     1,     3,   423,    85,     3,    87,    88,    89,
      90,   317,   293,    81,     3,    95,     6,     3,   648,    99,
      75,   648,   102,     8,     9,   105,    81,     3,   297,    28,
      29,   421,   678,    19,    91,    49,    88,   105,   344,    53,
     421,    93,     6,    88,    73,    74,    75,    76,    77,    78,
      79,    80,    81,     3,   365,    84,    85,    87,    87,    36,
      92,    87,    94,   344,    81,    87,    87,     3,    87,   421,
      99,   100,     8,     9,    10,   104,   105,    13,    14,    15,
      16,    81,   174,   729,    81,    49,   735,    23,    22,    53,
       8,    90,    81,    11,   231,    81,   726,    74,   231,   726,
     406,    78,   527,    91,    88,   231,   231,   231,   231,    93,
      88,    30,    31,   231,    87,   764,   231,    94,   683,    96,
      88,   770,    93,    73,    74,    75,    76,    77,    78,    79,
      80,    81,   781,    89,    84,    85,     3,    88,   449,   231,
       1,    60,    93,   683,    88,    81,    65,   816,    88,    99,
     100,    87,    89,    93,   104,   105,   101,   102,   103,    88,
     297,    30,    31,    88,   297,   555,    89,    28,    29,    82,
      83,   297,   297,   297,   297,    35,    36,    37,    88,   297,
      28,    29,   297,   852,    99,   100,   855,   397,   398,   399,
      88,    60,    48,   499,    30,    31,    65,    15,    16,    68,
      69,   766,    16,    17,   284,   297,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    51,    52,    84,    85,    89,
     501,   314,   315,   316,    60,    61,   766,   393,   394,    65,
      88,   297,    68,    69,   395,   396,   579,   580,   105,    89,
     332,    81,    91,   297,    91,    88,    88,   339,    87,    93,
     407,   532,    89,    88,   560,    88,    88,    88,    88,   540,
     825,   633,   419,   420,    89,    88,     3,    89,    88,     3,
      89,    88,   364,    88,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    88,   825,   443,   556,    91,     3,
      92,    25,    26,     8,    91,    47,   376,   105,    89,   376,
      89,    89,    89,    93,     3,    97,    92,   697,   107,    94,
      89,    94,    96,   470,    88,   381,    98,    29,    90,    92,
      95,   401,    58,   604,   605,   606,   632,   381,   381,   106,
      94,   381,   381,    94,    90,   427,   106,   429,   430,   107,
      95,    94,     4,    90,   436,     3,   483,    81,    89,    94,
     483,    90,    90,    88,    31,     8,   448,   106,    66,    92,
      92,    92,    92,    90,    90,    90,    90,    88,    88,    88,
      88,    94,    35,    90,    90,    88,   105,    88,    88,    88,
     506,   506,   506,   506,    88,    88,    88,    88,    88,    35,
      88,   483,   529,    88,   505,   506,   529,   505,    88,    88,
      88,    88,    88,   495,   505,    88,    90,   505,    90,   505,
      90,    90,    88,   505,   506,   507,   508,   509,    88,   556,
      88,   578,    88,   556,    88,    90,    38,   108,    88,    90,
     556,   556,   556,   556,    89,   818,   819,   529,   556,   519,
     520,   556,    55,     3,     4,    94,    91,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,   106,
      20,    21,   105,    23,   556,    25,    26,   850,   579,   580,
     106,   579,   580,    59,     3,   106,    94,   105,   579,   580,
      40,   579,   580,   579,   580,   105,    62,   579,   580,    92,
     106,   106,    94,    88,    93,   105,    90,    94,    39,   106,
     657,   106,     3,    93,   810,   105,    88,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    81,    23,    24,    25,    26,   106,    75,   106,    94,
      90,    75,   298,   298,    49,   726,    17,    16,   424,   390,
     392,   632,    50,   590,   501,   391,   683,   495,   665,   736,
     683,   297,   444,   346,     1,   813,     3,   717,   650,   815,
     717,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   634,    23,    24,    25,    26,
      81,   817,   648,   851,    78,   385,   560,    88,   822,   370,
     697,   683,    93,   421,   648,   648,    -1,    -1,   648,   648,
      -1,    -1,   436,    -1,   684,     3,   698,   684,    -1,    -1,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    -1,   718,    25,    26,   766,
      -1,    -1,    -1,   766,    81,    -1,    34,    -1,    -1,     3,
      87,    -1,    -1,    -1,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,   736,   814,    -1,   736,
     726,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   726,   726,   766,    -1,   726,   726,    -1,    -1,
      -1,    -1,   774,    81,    -1,    -1,    -1,    -1,   825,    -1,
      -1,    -1,   825,   773,    -1,    -1,   773,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,
      -1,   832,   832,    -1,   832,    -1,   818,   819,    -1,    -1,
      -1,   832,    -1,   825,   832,    -1,   832,    -1,    -1,    -1,
     832,   821,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,     4,    -1,   850,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    23,    -1,    25,    26,    27,
      -1,    -1,    30,    31,    32,    33,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    44,    45,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    -1,    63,    64,    -1,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    81,    24,    25,    26,     1,    -1,     3,
       4,    -1,    90,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      -1,    25,    26,    27,    -1,    -1,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    87,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,
      -1,     1,    -1,     3,     4,    -1,    90,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    23,    -1,    25,    26,    27,    -1,    -1,
      30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,     1,    -1,     3,     4,    -1,
      90,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    -1,    25,
      26,    27,    -1,    -1,    30,    31,    32,    33,    34,    -1,
       1,    -1,     3,     4,    40,    -1,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    -1,    25,    26,    27,    63,    64,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,     1,    -1,     3,     4,    -1,    90,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    23,    -1,    25,    26,
      27,    -1,    -1,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,     1,
      -1,    -1,     4,     5,     6,     7,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    20,    21,
      -1,    23,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    40,    41,
      42,    43,    -1,    90,    -1,    -1,    -1,    49,    50,    -1,
      -1,    53,     0,     1,    56,    -1,     4,     5,     6,     7,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,    71,
      72,    -1,    20,    21,    -1,    23,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    40,    41,    42,    43,    -1,    -1,    -1,    -1,
      -1,    49,    50,    -1,    -1,    53,     1,    -1,    56,     4,
       5,     6,     7,    -1,    -1,    63,    64,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    20,    21,    -1,    23,    -1,
      -1,     1,    27,    -1,     4,     5,     6,     7,    86,    87,
      -1,    -1,    -1,    -1,    -1,    40,    41,    42,    43,    -1,
      20,    21,    -1,    23,    49,    50,    -1,    27,    53,    -1,
      -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      40,    41,    42,    43,    -1,    70,    71,    -1,    -1,    49,
      50,    -1,    -1,    53,     1,    -1,    56,     4,     5,     6,
       7,    86,    87,    63,    64,    -1,    -1,    -1,    -1,    -1,
      70,    71,    72,    20,    21,    -1,    23,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    41,    42,    43,    -1,    -1,    -1,
      -1,    -1,    49,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
       3,    -1,    -1,    70,    71,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    86,
      23,    24,    25,    26,     1,    -1,     3,    -1,    -1,    -1,
      -1,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,     3,    23,    24,    25,    26,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    -1,    23,    24,    25,    26,     4,
      -1,     6,    -1,    -1,    -1,    -1,    -1,     4,    81,     6,
      -1,    -1,    -1,    -1,    87,    20,    21,    -1,    23,    24,
      -1,    -1,    27,    20,    21,    -1,    23,    24,    -1,    -1,
      27,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    49,    81,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      67
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   110,   111,   112,     0,     1,     4,     5,     6,     7,
      20,    21,    23,    27,    40,    41,    42,    43,    49,    50,
      53,    56,    63,    64,    70,    71,    86,   113,   114,   130,
     132,   136,   137,   149,   152,   153,   157,   159,   162,   163,
     164,   168,   172,   174,   178,   179,   203,   204,   222,   229,
     230,   237,   246,   266,   268,   280,   282,   304,   305,   306,
     348,   399,   400,   401,   402,   403,   407,   429,   431,   454,
     455,   456,   457,   458,   462,   463,   464,   467,   471,   479,
     494,   495,   129,   205,   131,   158,   238,   267,   281,   307,
     349,     3,   202,   254,   157,    53,   157,   172,   174,    53,
     164,   174,   175,   202,   202,   432,     3,    81,   198,   201,
     198,   480,   496,     3,    91,   133,   122,   138,   150,    88,
      88,   121,    93,   160,   154,   123,   165,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    24,
      25,    26,   160,   198,   241,   242,   243,   244,   245,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     268,   280,   282,   306,   315,   318,   321,   322,   325,   326,
     329,   119,   115,   113,    87,   231,   116,   269,    22,   120,
     117,   118,   124,   404,   125,   430,   126,   160,   465,   465,
     127,   128,    89,   498,    88,    17,   198,   209,   256,   259,
     260,   261,   262,   263,   322,   326,   198,   202,   239,   241,
     202,   202,   202,   202,   160,   202,   160,   169,   202,   202,
     408,   202,   199,    75,    81,    75,   230,   230,    89,    89,
      88,     4,     6,    20,    21,    23,    24,    27,    49,    53,
      67,   472,   473,   475,   230,   491,    88,    48,   176,    89,
      88,    89,     8,    11,     8,     9,    91,   319,   173,    92,
      94,    91,    91,    88,    88,   198,    88,    89,   283,    88,
      88,    88,    88,    89,    88,    89,   441,    88,   466,   459,
     468,    88,    88,   499,   206,   240,   308,   350,    89,    93,
     410,   433,   201,   200,     3,   231,     3,   223,   134,   209,
      91,     3,   139,   474,    73,    74,    75,    76,    77,    78,
      79,    80,    84,    85,    99,   100,   104,   105,   198,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     493,    92,   161,   155,   166,     8,   211,   221,    91,    47,
     177,   316,   323,   327,   105,   232,   270,   105,   405,   442,
     177,    89,    89,   501,   202,   202,   247,   250,   254,   255,
     330,    89,    89,   170,   411,   409,    93,   438,   201,   481,
     497,    90,   204,   209,   224,   225,   226,   227,   228,   237,
     264,   111,   112,     3,    92,    94,   219,   219,   219,   211,
      96,    97,    98,    82,    83,    99,   100,   101,   102,   103,
     492,   151,   195,   198,   185,   180,    94,   320,   195,   221,
     221,   221,   201,   211,   233,   234,   236,   272,   274,   275,
     284,   412,   444,   460,   469,    30,    31,    60,    65,    68,
      69,   337,   338,   343,   421,   423,   424,   490,   500,   502,
     207,   331,   248,   309,   351,   185,   198,   177,   439,   434,
      89,   410,    88,    29,   254,   135,    92,   140,   475,   106,
     213,   214,   215,   216,   216,   217,   217,   218,   218,   218,
      94,   202,   196,     1,    32,    33,   156,   186,   204,   237,
     337,   348,   353,   358,   399,   400,    44,    45,    46,   167,
     181,   182,   186,   360,   211,   230,   317,   324,   328,    95,
     106,    94,   235,   271,   273,   230,   230,    51,    52,    61,
     337,   406,   413,   421,   423,   426,   427,   428,   490,    44,
      54,   186,   443,   445,   448,   451,   185,   180,   339,   344,
      19,   198,   422,    58,   425,   198,   198,   505,   503,   504,
     422,   506,    90,    95,   107,   332,   334,    94,   230,   311,
     314,   273,   171,   198,   177,   482,     4,    90,     3,    89,
     230,    94,   194,    90,   190,   187,   192,   191,   193,    34,
     198,   243,   322,   326,   359,   382,   188,   189,   361,   184,
     183,    90,    88,    92,   242,    92,    92,    92,   211,   201,
     236,    90,   274,     1,   241,   276,     8,   198,   256,   261,
     262,   263,   289,   306,   198,   198,   198,   419,    90,   414,
     415,   416,   417,   418,   420,   202,   202,    90,   446,   447,
     461,   470,    31,   382,   201,     3,     3,    88,    88,    88,
     201,    88,   208,   335,   333,   249,     3,   310,   312,   352,
      90,   435,   337,   421,   423,   484,   486,   209,   141,   493,
     197,    88,    88,    88,    88,    88,    88,   354,    88,    88,
     230,   276,   276,   235,   279,   277,   285,   201,   201,   201,
      88,    88,    88,    88,    88,    88,    88,   449,   452,    88,
      88,    90,    90,   340,   345,   210,   221,   334,   250,    90,
      94,    90,    66,   489,   487,   488,   483,   485,    72,   114,
     130,   143,   145,   146,   198,   230,     3,    88,   247,   106,
     105,   364,   364,   382,   251,   254,   336,   313,   436,    88,
      88,    88,    90,   486,   198,   142,   144,    88,     3,   362,
     278,   286,   365,   366,   450,   453,   341,   346,   252,   108,
     314,   198,   147,    90,   145,   355,   364,    88,    89,   106,
      35,   367,   370,    38,   384,   384,   251,    55,   387,    94,
     437,    91,   105,   373,   363,   287,   371,   106,   368,   385,
     342,   388,   347,   253,    59,   440,     3,   476,   478,   374,
     375,   356,   384,     1,    28,    29,   290,   292,   296,   298,
     382,    94,   105,   384,   105,    62,   390,   254,   198,    92,
     477,   106,    35,    36,    37,   376,   379,   383,   384,   295,
     300,   299,   288,   291,   293,   297,   372,   369,   386,   389,
     391,   148,    94,   106,   377,   380,   357,    88,   211,    93,
      90,   292,   230,   298,   250,   370,   195,   195,   105,   202,
     478,    94,   382,    39,   393,   301,   241,   302,   106,   106,
     392,   378,   381,   394,    93,   303,   294,   195,   379,   250,
     105,   250,    88,   106,   395,    75,   396,   397,   106,    94,
     398,    75
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   109,   110,   111,   111,   112,   112,   113,   113,   113,
     115,   114,   116,   114,   117,   114,   118,   114,   119,   114,
     120,   114,   121,   114,   122,   114,   123,   114,   124,   114,
     125,   114,   126,   114,   127,   114,   128,   114,   129,   114,
     131,   130,   133,   134,   135,   132,   136,   138,   139,   140,
     141,   142,   137,   143,   144,   144,   145,   145,   147,   148,
     146,   150,   151,   149,   152,   152,   154,   155,   156,   153,
     158,   157,   159,   159,   159,   161,   160,   160,   162,   162,
     162,   162,   163,   163,   165,   166,   167,   164,   169,   170,
     171,   168,   173,   172,   175,   174,   176,   176,   177,   177,
     178,   178,   179,   180,   180,   181,   181,   181,   183,   182,
     184,   182,   185,   185,   187,   186,   188,   186,   189,   186,
     190,   186,   191,   186,   192,   186,   193,   186,   194,   186,
     195,   197,   196,   196,   198,   199,   198,   200,   198,   201,
     202,   203,   203,   203,   205,   206,   207,   208,   204,   209,
     209,   209,   209,   209,   209,   209,   209,   209,   210,   211,
     212,   212,   213,   213,   214,   214,   215,   215,   215,   216,
     216,   216,   217,   217,   217,   217,   218,   218,   218,   218,
     219,   219,   219,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   221,   222,   223,   223,   224,   224,   224,
     225,   226,   226,   227,   228,   228,   229,   230,   230,   231,
     232,   232,   233,   233,   234,   235,   235,   236,   238,   237,
     237,   237,   237,   237,   237,   240,   239,   241,   241,   242,
     242,   242,   243,   243,   243,   243,   243,   243,   243,   243,
     244,   244,   244,   244,   245,   245,   245,   246,   246,   247,
     249,   248,   248,   250,   250,   251,   253,   252,   252,   254,
     255,   256,   256,   257,   257,   257,   258,   258,   258,   259,
     259,   259,   260,   261,   261,   262,   263,   264,   265,   267,
     266,   269,   270,   271,   268,   272,   273,   273,   275,   274,
     277,   278,   276,   279,   276,   281,   280,   283,   284,   285,
     286,   287,   288,   282,   289,   289,   289,   289,   289,   289,
     290,   291,   291,   293,   294,   292,   295,   292,   296,   297,
     297,   299,   298,   300,   301,   298,   303,   302,   304,   305,
     307,   308,   309,   310,   306,   311,   313,   312,   312,   314,
     316,   317,   315,   315,   319,   320,   318,   321,   323,   324,
     322,   322,   325,   327,   328,   326,   326,   329,   331,   330,
     332,   333,   333,   335,   336,   334,   337,   337,   339,   340,
     341,   342,   338,   344,   345,   346,   347,   343,   349,   350,
     351,   352,   348,   354,   355,   356,   357,   353,   358,   358,
     358,   359,   359,   361,   362,   363,   360,   365,   364,   366,
     364,   367,   369,   368,   368,   371,   372,   370,   374,   373,
     375,   373,   376,   378,   377,   377,   380,   381,   379,   382,
     382,   382,   382,   383,   383,   383,   385,   386,   384,   384,
     388,   389,   387,   387,   391,   392,   390,   390,   394,   395,
     393,   393,   396,   398,   397,   397,   399,   400,   401,   401,
     402,   404,   405,   406,   403,   408,   409,   407,   411,   410,
     410,   412,   412,   414,   413,   415,   413,   416,   413,   417,
     413,   418,   413,   419,   413,   420,   413,   421,   422,   422,
     423,   424,   425,   425,   426,   427,   428,   430,   429,   432,
     433,   434,   435,   436,   437,   431,   439,   438,   438,   440,
     440,   442,   443,   441,   444,   444,   445,   446,   445,   447,
     445,   449,   450,   448,   452,   453,   451,   454,   454,   454,
     455,   455,   456,   457,   459,   460,   461,   458,   462,   463,
     464,   466,   465,   468,   469,   470,   467,   471,   471,   472,
     472,   472,   472,   472,   472,   472,   472,   472,   472,   473,
     474,   474,   475,   475,   476,   477,   477,   478,   480,   481,
     482,   483,   479,   484,   485,   485,   487,   486,   488,   486,
     489,   486,   490,   490,   491,   492,   492,   493,   494,   496,
     497,   495,   499,   500,   498,   501,   501,   503,   502,   504,
     502,   505,   502,   506,   502
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     0,     3,     2,     1,     2,     2,
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
       0,     0,     6,     2,     0,     0,     6,     6,     0,     0,
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
       3,     0,     2,     5,     2,     3,     0,     1,     0,     0,
       0,     0,    10,     2,     2,     0,     0,     3,     0,     3,
       0,     3,     3,     3,     3,     4,     0,     1,     2,     0,
       0,     6,     0,     0,     5,     2,     0,     0,     3,     0,
       3,     0,     3,     0,     3
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
  unsigned long yylno = yyrline[yyrule];
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
    default: /* Avoid compiler warnings. */
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
                  (unsigned long) yystacksize));

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
#line 395 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Decl *d = (yyvsp[0].dcval);
          if (d)
            {
              d->annotation_appls ((yyvsp[-1].annotations_val));
            }
          else
            {
              idl_global->err ()-> unsupported_warning(
                "Annotating this type of definition is not supported");
            }
        }
#line 2386 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 6:
#line 408 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 2393 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 7:
#line 414 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 2401 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 8:
#line 418 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = 0;
        }
#line 2409 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 9:
#line 422 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = 0;
        }
#line 2417 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 10:
#line 429 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AnnotationDeclSeen);
        }
#line 2425 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 11:
#line 433 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2434 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 12:
#line 438 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 2442 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 13:
#line 442 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 2451 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 14:
#line 447 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 2459 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 15:
#line 451 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2468 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 16:
#line 456 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 2476 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 17:
#line 460 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2485 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 18:
#line 465 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 2493 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 19:
#line 469 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 2502 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 20:
#line 474 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 2510 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 21:
#line 478 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2519 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 22:
#line 483 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
        }
#line 2527 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 23:
#line 487 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2536 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 24:
#line 492 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
        }
#line 2544 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 25:
#line 496 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 2553 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 26:
#line 501 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeDeclSeen);
        }
#line 2561 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 27:
#line 505 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2570 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 28:
#line 510 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentDeclSeen);
        }
#line 2578 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 29:
#line 514 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2587 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 30:
#line 519 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeDeclSeen);
        }
#line 2595 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 31:
#line 523 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2604 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 32:
#line 528 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventDeclSeen);
        }
#line 2612 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 33:
#line 532 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2621 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 34:
#line 537 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeDeclSeen);
        }
#line 2629 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 35:
#line 541 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2638 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 36:
#line 546 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorDeclSeen);
        }
#line 2646 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 37:
#line 550 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2655 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 38:
#line 555 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 2663 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 39:
#line 559 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
          (yyval.dcval) = 0;
        }
#line 2673 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 40:
#line 568 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
        }
#line 2681 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 41:
#line 572 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 2689 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 42:
#line 579 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 2732 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 43:
#line 618 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
        }
#line 2740 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 44:
#line 622 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
        }
#line 2748 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 45:
#line 626 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
          /*
           * Finished with this module - pop it from the scope stack.
           */

          idl_global->scopes ().pop ();
          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 2762 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 46:
#line 639 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleIDSeen);
        }
#line 2770 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 47:
#line 646 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 2788 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 48:
#line 660 "fe/idl.ypp" /* yacc.c:1651  */
    {
          if (FE_Utils::duplicate_param_id ((yyvsp[0].plval)))
            {
              idl_global->err ()->duplicate_param_id (
                (yyvsp[-2].idlist));

              return 1;
            }
        }
#line 2802 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 49:
#line 670 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 2836 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 50:
#line 700 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleSqSeen);
        }
#line 2844 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 51:
#line 704 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleBodySeen);
        }
#line 2852 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 52:
#line 708 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 2873 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 58:
#line 742 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefSeen);
        }
#line 2882 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 59:
#line 747 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefParamsSeen);
        }
#line 2891 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 60:
#line 752 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 2973 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 61:
#line 833 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleSeen);
        }
#line 2982 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 62:
#line 838 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleArgsSeen);
        }
#line 2991 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 63:
#line 843 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3057 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 66:
#line 913 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3099 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 67:
#line 951 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
#line 3107 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 68:
#line 955 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
#line 3115 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 69:
#line 959 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
#line 3128 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 70:
#line 971 "fe/idl.ypp" /* yacc.c:1651  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
#line 3136 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 71:
#line 975 "fe/idl.ypp" /* yacc.c:1651  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (yyval.idval) = (yyvsp[0].idval);
         }
#line 3145 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 72:
#line 983 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3185 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 73:
#line 1020 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3218 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 74:
#line 1050 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3251 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 75:
#line 1082 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 3259 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 76:
#line 1086 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyvsp[0].nlval)->truncatable ((yyvsp[-2].bval));
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3268 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 77:
#line 1091 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = 0;
        }
#line 3276 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 82:
#line 1105 "fe/idl.ypp" /* yacc.c:1651  */
    {
           ACE_DEBUG ((LM_DEBUG,
                       ACE_TEXT ("error in %C line %d\n"),
                       idl_global->filename ()->get_string (),
                       idl_global->lineno ()));
           ACE_DEBUG ((LM_DEBUG,
                       ACE_TEXT ("Sorry, I (TAO_IDL) can't handle")
                       ACE_TEXT (" custom yet\n")));
        }
#line 3290 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 84:
#line 1119 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3337 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 85:
#line 1162 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3345 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 86:
#line 1166 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3353 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 87:
#line 1170 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3376 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 88:
#line 1193 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3423 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 89:
#line 1236 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3431 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 90:
#line 1240 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3439 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 91:
#line 1244 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 3452 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 92:
#line 1257 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 3460 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 93:
#line 1261 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3495 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 94:
#line 1295 "fe/idl.ypp" /* yacc.c:1651  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
#line 3503 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 95:
#line 1299 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 3512 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 96:
#line 1307 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.bval) = true;
        }
#line 3520 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 97:
#line 1311 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.bval) = false;
        }
#line 3528 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 98:
#line 1319 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3536 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 99:
#line 1323 "fe/idl.ypp" /* yacc.c:1651  */
    {
/*      |    EMPTY */
          (yyval.nlval) = 0;
        }
#line 3545 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 100:
#line 1332 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3572 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 101:
#line 1356 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3599 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 102:
#line 1382 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3664 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 108:
#line 1458 "fe/idl.ypp" /* yacc.c:1651  */
    {
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_PUBLIC;
        }
#line 3673 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 109:
#line 1463 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 3680 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 110:
#line 1466 "fe/idl.ypp" /* yacc.c:1651  */
    {
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_PRIVATE;
        }
#line 3689 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 111:
#line 1471 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 3696 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 114:
#line 1482 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 3704 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 115:
#line 1486 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3712 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 116:
#line 1490 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 3720 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 117:
#line 1494 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3728 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 118:
#line 1498 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 3736 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 119:
#line 1502 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3744 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 120:
#line 1506 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 3752 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 121:
#line 1510 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
#line 3760 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 122:
#line 1514 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 3768 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 123:
#line 1518 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3776 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 124:
#line 1522 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 3784 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 125:
#line 1526 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3792 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 126:
#line 1530 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
#line 3800 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 127:
#line 1534 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3808 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 128:
#line 1538 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 3816 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 129:
#line 1542 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 3825 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 130:
#line 1550 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.nlval),
                          UTL_NameList ((yyvsp[-1].idlist),
                                        (yyvsp[0].nlval)),
                          1);
        }
#line 3836 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 131:
#line 1561 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
#line 3844 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 132:
#line 1565 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3868 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 133:
#line 1585 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = 0;
        }
#line 3876 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 134:
#line 1592 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN ((yyval.idlist),
                          UTL_IdList ((yyvsp[0].idval),
                                      0),
                          1);
        }
#line 3889 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 135:
#line 1601 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
#line 3897 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 136:
#line 1605 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3921 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 137:
#line 1626 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);

          // This cleans up all the non-global "::"s in scoped names.
          // If there is a global one, it gets put into the UTL_IdList,
          // so we clean it up in the case above.
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3935 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 138:
#line 1636 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3951 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 139:
#line 1650 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.idval),
                          Identifier ((yyvsp[0].strval)),
                          1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3963 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 140:
#line 1660 "fe/idl.ypp" /* yacc.c:1651  */
    {
          /* defining_id is a defining identifier
             whereas id is usually a reference to a defining identifier */
          ACE_NEW_RETURN ((yyval.idval), Identifier ((yyvsp[0].strval)), 1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3975 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 141:
#line 1671 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4022 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 142:
#line 1715 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4052 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 143:
#line 1742 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4082 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 144:
#line 1771 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
#line 4090 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 145:
#line 1775 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
#line 4098 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 146:
#line 1779 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
#line 4106 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 147:
#line 1783 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
#line 4114 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 148:
#line 1787 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4174 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 155:
#line 1852 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_string;
        }
#line 4182 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 156:
#line 1856 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_wstring;
        }
#line 4190 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 157:
#line 1860 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4260 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 161:
#line 1933 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_or,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4273 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 163:
#line 1946 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_xor,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4286 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 165:
#line 1959 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_and,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4299 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 167:
#line 1972 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_left,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4312 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 168:
#line 1981 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_right,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4325 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 170:
#line 1994 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_add,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4338 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 171:
#line 2003 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_minus,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4351 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 173:
#line 2016 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mul,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4364 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 174:
#line 2025 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_div,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4377 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 175:
#line 2034 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mod,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4390 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 177:
#line 2047 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_u_plus,
                                    (yyvsp[0].exval),
                                    0
                                  );
        }
#line 4403 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 178:
#line 2056 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_u_minus,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4416 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 179:
#line 2065 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_bit_neg,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4429 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 180:
#line 2077 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4483 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 182:
#line 2128 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = (yyvsp[-1].exval);
        }
#line 4491 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 183:
#line 2135 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].ival));
        }
#line 4499 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 184:
#line 2139 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr ((yyvsp[0].uival));
        }
#line 4508 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 185:
#line 2144 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].sval));
          (yyvsp[0].sval)->destroy ();
          delete (yyvsp[0].sval);
          (yyvsp[0].sval) = 0;
        }
#line 4519 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 186:
#line 2151 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].wsval));
        }
#line 4527 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 187:
#line 2155 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].cval));
        }
#line 4535 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 188:
#line 2159 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_OutputCDR::from_wchar wc ((yyvsp[0].wcval));
          (yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
#line 4544 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 189:
#line 2164 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].fixval));
        }
#line 4552 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 190:
#line 2168 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].dval));
        }
#line 4560 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 191:
#line 2172 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr (true);
        }
#line 4568 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 192:
#line 2176 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr (false);
        }
#line 4576 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 193:
#line 2183 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4645 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 194:
#line 2250 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4674 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 206:
#line 2307 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          annotations->push_back ((yyvsp[0].annotation_val));
          (yyval.annotations_val) = annotations;
        }
#line 4684 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 207:
#line 2316 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          annotations->push_back ((yyvsp[0].annotation_val));
          (yyval.annotations_val) = annotations;
        }
#line 4694 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 208:
#line 2322 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.annotations_val) = new AST_Annotation_Appls ();
        }
#line 4702 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 209:
#line 2329 "fe/idl.ypp" /* yacc.c:1651  */
    {
          if (idl_global->idl_version_ < IDL_VERSION_4)
            {
              idl_global->err ()->idl_version_error (
                "Annotations are an IDL4 feature");
            }

          (yyval.annotation_val) = idl_global->gen ()->create_annotation_appl ((yyvsp[-1].idlist), (yyvsp[0].annotation_params_val));
        }
#line 4716 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 210:
#line 2342 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.annotation_params_val) = (yyvsp[-1].annotation_params_val);
        }
#line 4724 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 211:
#line 2346 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.annotation_params_val) = 0;
        }
#line 4732 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 212:
#line 2353 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appl::Params *params = new AST_Annotation_Appl::Params;
          AST_Annotation_Appl::Param *param = new AST_Annotation_Appl::Param;
          param->id = 0;
          param->expr = (yyvsp[0].exval);
          params->push (param);
          (yyval.annotation_params_val) = params;
        }
#line 4745 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 213:
#line 2362 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.annotation_params_val) = (yyvsp[0].annotation_params_val);
        }
#line 4753 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 214:
#line 2369 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 4763 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 215:
#line 2377 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 4773 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 216:
#line 2383 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.annotation_params_val) = new AST_Annotation_Appl::Params;
        }
#line 4781 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 217:
#line 2390 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appl::Param *param = new AST_Annotation_Appl::Param;
          param->id = (yyvsp[-2].idval);
          param->expr = (yyvsp[0].exval);
          (yyval.annotation_param_val) = param;
        }
#line 4792 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 218:
#line 2400 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 4801 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 219:
#line 2405 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 4809 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 220:
#line 2409 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 4817 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 221:
#line 2413 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 4825 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 222:
#line 2417 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 4833 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 223:
#line 2421 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4863 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 224:
#line 2447 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = 0;
        }
#line 4871 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 225:
#line 2454 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
#line 4879 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 226:
#line 2458 "fe/idl.ypp" /* yacc.c:1651  */
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

          (yyval.dcval) = t;
        }
#line 4948 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 229:
#line 2531 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 4959 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 231:
#line 2539 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4986 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 249:
#line 2594 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 4997 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 250:
#line 2605 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5005 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 251:
#line 2609 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5029 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 252:
#line 2629 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dlval) = 0;
        }
#line 5037 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 255:
#line 2641 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5048 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 256:
#line 2652 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5056 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 257:
#line 2656 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5080 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 258:
#line 2676 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dlval) = 0;
        }
#line 5088 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 259:
#line 2683 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5105 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 260:
#line 2699 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5124 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 263:
#line 2722 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_long;
        }
#line 5132 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 264:
#line 2726 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_longlong;
        }
#line 5140 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 265:
#line 2730 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_short;
        }
#line 5148 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 266:
#line 2737 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_ulong;
        }
#line 5156 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 267:
#line 2741 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_ulonglong;
        }
#line 5164 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 268:
#line 2745 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_ushort;
        }
#line 5172 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 269:
#line 2752 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_double;
        }
#line 5180 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 270:
#line 2756 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_float;
        }
#line 5188 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 271:
#line 2760 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_longdouble;
        }
#line 5196 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 272:
#line 2767 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_fixed;
        }
#line 5204 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 273:
#line 2774 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_char;
        }
#line 5212 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 274:
#line 2778 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_wchar;
        }
#line 5220 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 275:
#line 2785 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_octet;
        }
#line 5228 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 276:
#line 2792 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_bool;
        }
#line 5236 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 277:
#line 2799 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_any;
        }
#line 5244 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 278:
#line 2806 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_object;
        }
#line 5252 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 279:
#line 2813 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
#line 5260 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 280:
#line 2817 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5269 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 281:
#line 2826 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5304 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 282:
#line 2857 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
#line 5312 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 283:
#line 2861 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
#line 5320 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 284:
#line 2865 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5336 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 288:
#line 2886 "fe/idl.ypp" /* yacc.c:1651  */
    {
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_NA;
        }
#line 5345 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 289:
#line 2891 "fe/idl.ypp" /* yacc.c:1651  */
    {
          if ((yyvsp[0].decls_val))
            {
              AST_Decls & decls = *(yyvsp[0].decls_val);
              for (size_t i = 0; i < decls.size (); i++)
                {
                  decls[i]->annotation_appls ((yyvsp[-1].annotations_val));
                }
            }
        }
#line 5360 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 290:
#line 2905 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
#line 5368 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 291:
#line 2909 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
#line 5376 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 292:
#line 2913 "fe/idl.ypp" /* yacc.c:1651  */
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
                  members->push_back (f);
                  (void) s->fe_add_field (f);
                }
            }

          (yyvsp[-2].dlval)->destroy ();
          delete (yyvsp[-2].dlval);
          (yyvsp[-2].dlval) = 0;

          (yyval.decls_val) = members;
        }
#line 5440 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 293:
#line 2973 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->err()->syntax_error (idl_global->parse_state ());
        }
#line 5448 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 294:
#line 2977 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 5457 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 295:
#line 2985 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
#line 5465 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 296:
#line 2989 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5474 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 297:
#line 2997 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
#line 5482 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 298:
#line 3001 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5519 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 299:
#line 3034 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
#line 5527 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 300:
#line 3038 "fe/idl.ypp" /* yacc.c:1651  */
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
                  e->disc_annotations ((yyvsp[-3].annotations_val)); // Set Discriminator Annotations

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
#line 5587 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 301:
#line 3094 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
#line 5595 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 302:
#line 3098 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
#line 5603 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 303:
#line 3102 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5623 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 304:
#line 3121 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5634 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 305:
#line 3128 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5651 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 306:
#line 3141 "fe/idl.ypp" /* yacc.c:1651  */
    {
          /* octets are not allowed. */
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5664 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 307:
#line 3150 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5675 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 309:
#line 3158 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5783 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 313:
#line 3272 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
#line 5791 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 314:
#line 3276 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
#line 5799 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 315:
#line 3280 "fe/idl.ypp" /* yacc.c:1651  */
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
              b->annotation_appls ((yyvsp[-3].annotations_val));
              (void) s->fe_add_union_branch (b);

              // f has passed its field type to the union branch,
              // but the rest still needs to be cleaned up.
              f->AST_Decl::destroy ();
              delete f;
              f = 0;
            }
        }
#line 5832 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 316:
#line 3309 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 5840 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 317:
#line 3313 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 5849 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 318:
#line 3321 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.llval),
                          UTL_LabelList ((yyvsp[-1].ulval),
                                         (yyvsp[0].llval)),
                          1);
        }
#line 5860 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 319:
#line 3331 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5882 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 320:
#line 3349 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.llval) = 0;
        }
#line 5890 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 321:
#line 3356 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
#line 5898 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 322:
#line 3360 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
#line 5911 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 323:
#line 3369 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
#line 5919 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 324:
#line 3373 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
#line 5927 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 325:
#line 3377 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      (yyvsp[-2].exval)
                                    );
        }
#line 5940 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 326:
#line 3389 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
#line 5948 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 327:
#line 3393 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5996 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 328:
#line 3440 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6022 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 329:
#line 3465 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6046 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 330:
#line 3488 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
#line 6054 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 331:
#line 3492 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6089 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 332:
#line 3523 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
#line 6097 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 333:
#line 3527 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
#line 6105 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 334:
#line 3531 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6128 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 336:
#line 3556 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
#line 6136 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 339:
#line 3565 "fe/idl.ypp" /* yacc.c:1651  */
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
                  e->annotation_appls ((yyvsp[-1].annotations_val));
                }

              (void) s->fe_add_enum_val (e);
            }
        }
#line 6171 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 340:
#line 3600 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
#line 6179 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 341:
#line 3604 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
#line 6187 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 342:
#line 3608 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceQsSeen);

          AST_Sequence *seq = 0;
          Decl_Annotations_Pair *seq_head = (yyvsp[-5].decl_annotations_pair_val);
          AST_Decl *type = 0;
          AST_Annotation_Appls *type_annotations = 0;
          if (seq_head)
            {
              type = seq_head->decl;
              type_annotations = seq_head->annotations;
            }
          delete seq_head;

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
            }
          else if (type)
            {
              AST_Type *tp = AST_Type::narrow_from_decl (type);

              if (0 == tp)
                {
                  ; // Error will be caught in FE_Declarator.
                }
              else
                {
                  Identifier id ("sequence");
                  UTL_ScopedName sn (&id,
                                     0);

                  seq =
                    idl_global->gen ()->create_sequence (
                                            (yyvsp[-2].exval),
                                            tp,
                                            &sn,
                                            s->is_local (),
                                            s->is_abstract ()
                                          );
                  seq->base_type_annotations (type_annotations);

                  if (!idl_global->in_typedef ()
                      && !idl_global->anon_silent ())
                    {
                      idl_global->err ()->anonymous_type_diagnostic ();
                    }
                }
            }

          delete ev;
          ev = 0;
          (yyval.dcval) = seq;
        }
#line 6275 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 343:
#line 3693 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceQsSeen);

          AST_Sequence *seq = 0;
          Decl_Annotations_Pair *seq_head = (yyvsp[-1].decl_annotations_pair_val);
          AST_Decl *type = 0;
          AST_Annotation_Appls *type_annotations = 0;
          if (seq_head)
            {
              type = seq_head->decl;
              type_annotations = seq_head->annotations;
            }
          delete seq_head;

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
          if (type)
            {
              AST_Type *tp = AST_Type::narrow_from_decl (type);

              if (tp == 0)
                {
                  ; // Error will be caught in FE_Declarator.
                }
              else
                {
                  Identifier id ("sequence");
                  UTL_ScopedName sn (&id, 0);
                  ACE_CDR::ULong bound = 0UL;

                  seq =
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
                  seq->base_type_annotations (type_annotations);

                  if (!idl_global->in_typedef ()
                      && !idl_global->anon_silent ())
                    {
                      idl_global->err ()->anonymous_type_diagnostic ();
                    }
                }
            }

          (yyval.dcval) = seq;
        }
#line 6344 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 344:
#line 3761 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
#line 6357 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 345:
#line 3770 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
#line 6365 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 346:
#line 3774 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          Decl_Annotations_Pair *seq_head = new Decl_Annotations_Pair;
          seq_head->decl = (yyvsp[0].dcval);
          seq_head->annotations = (yyvsp[-1].annotations_val);
          (yyval.decl_annotations_pair_val) = seq_head;
        }
#line 6377 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 347:
#line 3785 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyvsp[-1].exval)->evaluate (AST_Expression::EK_positive_int);
          (yyval.dcval) = idl_global->gen ()->create_fixed ((yyvsp[-3].exval), (yyvsp[-1].exval));
        }
#line 6386 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 348:
#line 3794 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 6394 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 349:
#line 3798 "fe/idl.ypp" /* yacc.c:1651  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 6402 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 350:
#line 3802 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6448 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 351:
#line 3844 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6474 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 352:
#line 3869 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 6482 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 353:
#line 3877 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 6490 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 354:
#line 3881 "fe/idl.ypp" /* yacc.c:1651  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 6498 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 355:
#line 3885 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6533 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 356:
#line 3916 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6558 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 357:
#line 3940 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 6566 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 358:
#line 3947 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
#line 6574 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 359:
#line 3951 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6611 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 360:
#line 3987 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.elval),
                          UTL_ExprList ((yyvsp[-1].exval),
                                        (yyvsp[0].elval)),
                          1);
        }
#line 6622 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 361:
#line 3997 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6644 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 362:
#line 4015 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.elval) = 0;
        }
#line 6652 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 363:
#line 4022 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
#line 6660 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 364:
#line 4026 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
#line 6668 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 365:
#line 4030 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6726 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 368:
#line 4092 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
#line 6734 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 369:
#line 4096 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 6742 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 370:
#line 4100 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 6750 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 371:
#line 4104 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 6758 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 372:
#line 4108 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6822 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 373:
#line 4171 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 6830 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 374:
#line 4175 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 6838 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 375:
#line 4179 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 6846 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 376:
#line 4183 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
#line 6854 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 377:
#line 4187 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6927 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 378:
#line 4259 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
#line 6935 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 379:
#line 4263 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6968 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 380:
#line 4292 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
#line 6976 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 381:
#line 4296 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
#line 6984 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 382:
#line 4300 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();
        }
#line 6996 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 383:
#line 4312 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7004 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 384:
#line 4316 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7072 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 385:
#line 4380 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7080 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 386:
#line 4384 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
#line 7088 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 387:
#line 4388 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7121 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 388:
#line 4420 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_oneway;
        }
#line 7130 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 389:
#line 4425 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_idempotent;
        }
#line 7139 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 390:
#line 4430 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ofval) = AST_Operation::OP_noflags;
        }
#line 7147 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 392:
#line 4438 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
#line 7158 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 393:
#line 4448 "fe/idl.ypp" /* yacc.c:1651  */
    {
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7167 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 394:
#line 4453 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7212 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 395:
#line 4494 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7220 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 396:
#line 4498 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7237 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 397:
#line 4514 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7245 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 398:
#line 4518 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7253 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 399:
#line 4522 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7261 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 400:
#line 4527 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7269 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 402:
#line 4537 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7277 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 405:
#line 4546 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7285 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 406:
#line 4550 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7293 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 407:
#line 4554 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7329 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 408:
#line 4589 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7337 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 409:
#line 4593 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7345 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 410:
#line 4597 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7353 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 411:
#line 4602 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7361 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 413:
#line 4612 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7369 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 416:
#line 4621 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7377 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 417:
#line 4625 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7385 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 418:
#line 4629 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7428 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 419:
#line 4671 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 7439 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 422:
#line 4680 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7547 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 423:
#line 4787 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dival) = AST_Argument::dir_IN;
        }
#line 7555 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 424:
#line 4791 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dival) = AST_Argument::dir_OUT;
        }
#line 7563 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 425:
#line 4795 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dival) = AST_Argument::dir_INOUT;
        }
#line 7571 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 426:
#line 4802 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
#line 7579 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 427:
#line 4806 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
#line 7587 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 428:
#line 4811 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7596 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 429:
#line 4816 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = 0;
        }
#line 7604 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 430:
#line 4823 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
#line 7612 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 431:
#line 4827 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
#line 7620 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 432:
#line 4832 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7629 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 433:
#line 4837 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = 0;
        }
#line 7637 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 434:
#line 4844 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
#line 7645 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 435:
#line 4848 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
#line 7653 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 436:
#line 4853 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7662 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 437:
#line 4858 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = 0;
        }
#line 7670 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 438:
#line 4865 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
#line 7678 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 439:
#line 4869 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
        }
#line 7686 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 440:
#line 4874 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (yyval.slval) = (yyvsp[-1].slval);
        }
#line 7695 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 441:
#line 4879 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.slval) = 0;
        }
#line 7703 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 442:
#line 4886 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.slval),
                          UTL_StrList ((yyvsp[-1].sval),
                                       (yyvsp[0].slval)),
                          1);
        }
#line 7714 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 443:
#line 4897 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
#line 7722 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 444:
#line 4901 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7744 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 445:
#line 4919 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.slval) = 0;
        }
#line 7752 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 446:
#line 4926 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7781 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 447:
#line 4954 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7819 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 450:
#line 4997 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7847 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 451:
#line 5024 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7890 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 452:
#line 5063 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
#line 7898 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 453:
#line 5067 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
#line 7906 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 454:
#line 5071 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 7919 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 455:
#line 5084 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
#line 7927 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 456:
#line 5088 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 7935 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 457:
#line 5092 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7973 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 458:
#line 5129 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 7981 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 459:
#line 5133 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 7989 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 460:
#line 5137 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = 0;
        }
#line 7997 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 463:
#line 5149 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 8005 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 464:
#line 5153 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8013 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 465:
#line 5157 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 8021 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 466:
#line 5161 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8029 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 467:
#line 5165 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
        }
#line 8037 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 468:
#line 5169 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8045 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 469:
#line 5173 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
        }
#line 8053 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 470:
#line 5177 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8061 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 471:
#line 5181 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
        }
#line 8069 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 472:
#line 5185 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8077 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 473:
#line 5189 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 8085 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 474:
#line 5193 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8093 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 475:
#line 5197 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 8101 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 476:
#line 5201 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8109 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 477:
#line 5207 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8200 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 478:
#line 5297 "fe/idl.ypp" /* yacc.c:1651  */
    {
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8210 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 479:
#line 5303 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8237 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 480:
#line 5328 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8344 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 481:
#line 5434 "fe/idl.ypp" /* yacc.c:1651  */
    {
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          (yyval.bval) = (yyvsp[0].bval);
        }
#line 8354 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 482:
#line 5443 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.bval) = true;
        }
#line 8362 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 483:
#line 5447 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.bval) = false;
        }
#line 8370 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 484:
#line 5453 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8444 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 485:
#line 5525 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8518 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 486:
#line 5597 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8592 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 487:
#line 5670 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8633 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 488:
#line 5707 "fe/idl.ypp" /* yacc.c:1651  */
    {
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 8644 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 489:
#line 5717 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
#line 8652 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 490:
#line 5721 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
#line 8660 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 491:
#line 5725 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 8668 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 492:
#line 5729 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
#line 8676 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 493:
#line 5733 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
#line 8684 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 494:
#line 5737 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
#line 8692 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 495:
#line 5741 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8742 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 496:
#line 5790 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 8750 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 497:
#line 5794 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8758 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 498:
#line 5798 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = 0;
        }
#line 8766 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 499:
#line 5806 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8774 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 500:
#line 5810 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = 0;
        }
#line 8782 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 501:
#line 5817 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
#line 8790 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 502:
#line 5821 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
#line 8798 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 503:
#line 5825 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
#line 8806 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 507:
#line 5838 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
#line 8814 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 508:
#line 5842 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8822 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 509:
#line 5846 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
#line 8830 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 510:
#line 5850 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8838 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 511:
#line 5858 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8865 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 512:
#line 5881 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 8873 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 513:
#line 5885 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8895 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 514:
#line 5907 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8926 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 515:
#line 5934 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 8934 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 516:
#line 5938 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8956 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 522:
#line 5971 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8983 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 523:
#line 5999 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9010 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 524:
#line 6026 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9058 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 525:
#line 6070 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9066 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 526:
#line 6074 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9074 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 527:
#line 6078 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9087 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 528:
#line 6092 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9095 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 529:
#line 6101 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9112 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 530:
#line 6118 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9122 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 531:
#line 6127 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 9130 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 532:
#line 6131 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9163 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 533:
#line 6164 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9216 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 534:
#line 6213 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9224 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 535:
#line 6217 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9232 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 536:
#line 6221 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9245 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 537:
#line 6233 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9253 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 538:
#line 6237 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9261 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 539:
#line 6244 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_type;
        }
#line 9269 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 540:
#line 6248 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_struct;
        }
#line 9277 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 541:
#line 6252 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_union;
        }
#line 9285 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 542:
#line 6256 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_eventtype;
        }
#line 9293 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 543:
#line 6260 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_sequence;
        }
#line 9301 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 544:
#line 6264 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_interface;
        }
#line 9309 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 545:
#line 6268 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_valuetype;
        }
#line 9317 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 546:
#line 6272 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_enum;
        }
#line 9325 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 547:
#line 6276 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_except;
        }
#line 9333 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 548:
#line 6280 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_const;
          t_param_const_type = (yyvsp[0].etval);
        }
#line 9342 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 549:
#line 6288 "fe/idl.ypp" /* yacc.c:1651  */
    {
          if ((yyvsp[0].plval) == 0)
            {
              ACE_NEW_RETURN ((yyvsp[0].plval), FE_Utils::T_PARAMLIST_INFO, 1);
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
#line 9374 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 550:
#line 6319 "fe/idl.ypp" /* yacc.c:1651  */
    {
          if ((yyvsp[-2].plval) == 0)
            {
              ACE_NEW_RETURN ((yyvsp[-2].plval), FE_Utils::T_PARAMLIST_INFO, 1);
            }

          (yyvsp[-2].plval)->enqueue_tail (*(yyvsp[0].pival));
          (yyval.plval) = (yyvsp[-2].plval);

          delete (yyvsp[0].pival);
          (yyvsp[0].pival) = 0;
        }
#line 9391 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 551:
#line 6332 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.plval) = 0;
        }
#line 9399 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 552:
#line 6339 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9428 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 553:
#line 6364 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9447 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 554:
#line 6382 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.slval), UTL_StrList ((yyvsp[-1].sval), (yyvsp[0].slval)), 1);
        }
#line 9455 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 555:
#line 6389 "fe/idl.ypp" /* yacc.c:1651  */
    {
          UTL_StrList *sl = 0;
          ACE_NEW_RETURN (sl, UTL_StrList ((yyvsp[0].sval), 0), 1);

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
#line 9474 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 556:
#line 6404 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.slval) = 0;
        }
#line 9482 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 557:
#line 6411 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.sval),
                          UTL_String ((yyvsp[0].strval), true),
                          1);
        }
#line 9492 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 558:
#line 6420 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSeen);
        }
#line 9500 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 559:
#line 6424 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9522 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 560:
#line 6442 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSqSeen);
        }
#line 9530 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 561:
#line 6446 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeBodySeen);
        }
#line 9538 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 562:
#line 6450 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeQsSeen);

          // Done with this port type - pop it off the scopes stack.
          idl_global->scopes ().pop ();
        }
#line 9549 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 563:
#line 6460 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9556 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 564:
#line 6466 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9563 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 565:
#line 6469 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9570 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 566:
#line 6475 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9577 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 567:
#line 6478 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9584 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 568:
#line 6481 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9591 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 569:
#line 6484 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9598 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 570:
#line 6487 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 9606 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 571:
#line 6491 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9613 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 572:
#line 6497 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9686 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 573:
#line 6566 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9737 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 574:
#line 6616 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9753 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 575:
#line 6631 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9769 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 576:
#line 6643 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.alval) = 0;
        }
#line 9777 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 577:
#line 6650 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9837 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 579:
#line 6713 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSeen);
        }
#line 9845 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 580:
#line 6717 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorIDSeen);
        }
#line 9853 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 581:
#line 6721 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9909 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 582:
#line 6776 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSqSeen);
        }
#line 9917 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 583:
#line 6780 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorBodySeen);
        }
#line 9925 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 584:
#line 6784 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorQsSeen);

          // Done with this connector - pop it off the scope stack.
          idl_global->scopes ().pop ();
        }
#line 9936 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 587:
#line 6799 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 9944 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 588:
#line 6803 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9952 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 589:
#line 6807 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 9960 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 590:
#line 6811 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9968 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 591:
#line 6815 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 9976 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 592:
#line 6819 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9984 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 593:
#line 6823 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 9992 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 594:
#line 6827 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10000 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;


#line 10004 "fe/idl.tab.cpp" /* yacc.c:1651  */
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
#line 6832 "fe/idl.ypp" /* yacc.c:1910  */

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
