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
#include "ast_annotation_member.h"

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
AST_String *tao_string_decl = 0;
AST_Expression::ExprType t_param_const_type = AST_Expression::EV_none;
#define TAO_YYDEBUG_LEXER_TEXT (tao_yytext[tao_yyleng] = '\0', tao_yytext)
// Compile Optional Tracing Output for Parser, can be enabled with --bison-trace
#define YYDEBUG 1

#line 156 "fe/idl.tab.cpp" /* yacc.c:339  */

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
#line 158 "fe/idl.ypp" /* yacc.c:355  */

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

#line 328 "fe/idl.tab.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE tao_yylval;

int tao_yyparse (void);

#endif /* !YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 345 "fe/idl.tab.cpp" /* yacc.c:358  */

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
#define YYLAST   1679

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  109
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  399
/* YYNRULES -- Number of rules.  */
#define YYNRULES  596
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  877

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
       0,   385,   385,   388,   389,   397,   410,   416,   420,   424,
     432,   431,   441,   440,   450,   449,   459,   458,   468,   467,
     477,   476,   486,   485,   495,   494,   504,   503,   513,   512,
     522,   521,   531,   530,   540,   539,   549,   548,   558,   557,
     571,   570,   582,   621,   625,   581,   641,   649,   663,   673,
     703,   707,   648,   730,   734,   735,   739,   740,   745,   750,
     744,   836,   841,   835,   910,   911,   916,   954,   958,   915,
     974,   973,   985,  1022,  1052,  1085,  1084,  1093,  1100,  1101,
    1102,  1103,  1107,  1111,  1116,  1159,  1163,  1115,  1190,  1233,
    1237,  1188,  1254,  1252,  1292,  1291,  1303,  1307,  1314,  1319,
    1327,  1352,  1378,  1442,  1443,  1447,  1448,  1449,  1455,  1454,
    1463,  1462,  1473,  1474,  1479,  1478,  1487,  1486,  1495,  1494,
    1503,  1502,  1511,  1510,  1519,  1518,  1527,  1526,  1535,  1534,
    1546,  1558,  1556,  1581,  1588,  1598,  1597,  1623,  1621,  1646,
    1656,  1667,  1711,  1738,  1768,  1772,  1776,  1780,  1767,  1842,
    1843,  1844,  1845,  1846,  1847,  1848,  1852,  1856,  1924,  1926,
    1928,  1929,  1941,  1942,  1954,  1955,  1967,  1968,  1977,  1989,
    1990,  1999,  2011,  2012,  2021,  2030,  2042,  2043,  2052,  2061,
    2073,  2123,  2124,  2131,  2135,  2140,  2147,  2151,  2155,  2160,
    2164,  2168,  2172,  2179,  2248,  2247,  2274,  2275,  2279,  2280,
    2281,  2283,  2282,  2291,  2292,  2296,  2347,  2351,  2358,  2371,
    2381,  2388,  2420,  2424,  2431,  2440,  2447,  2455,  2461,  2468,
    2479,  2478,  2487,  2491,  2495,  2499,  2525,  2533,  2532,  2604,
    2605,  2609,  2616,  2617,  2643,  2644,  2645,  2646,  2647,  2648,
    2649,  2650,  2654,  2655,  2656,  2657,  2661,  2662,  2663,  2667,
    2668,  2672,  2684,  2682,  2707,  2714,  2715,  2719,  2731,  2729,
    2754,  2761,  2777,  2795,  2796,  2800,  2804,  2808,  2815,  2819,
    2823,  2830,  2834,  2838,  2845,  2852,  2856,  2863,  2870,  2877,
    2884,  2892,  2891,  2905,  2936,  2940,  2904,  2957,  2960,  2961,
    2965,  2965,  2984,  2988,  2983,  3052,  3051,  3064,  3063,  3076,
    3080,  3113,  3117,  3173,  3177,  3075,  3199,  3206,  3219,  3228,
    3235,  3236,  3342,  3345,  3346,  3351,  3355,  3350,  3388,  3387,
    3399,  3409,  3427,  3435,  3434,  3448,  3452,  3447,  3468,  3467,
    3518,  3543,  3567,  3571,  3602,  3606,  3566,  3630,  3635,  3633,
    3639,  3643,  3679,  3683,  3677,  3770,  3840,  3849,  3839,  3863,
    3873,  3877,  3871,  3922,  3946,  3955,  3959,  3953,  3995,  4019,
    4027,  4026,  4066,  4076,  4094,  4102,  4106,  4101,  4166,  4167,
    4172,  4176,  4180,  4184,  4171,  4251,  4255,  4259,  4263,  4250,
    4339,  4343,  4372,  4376,  4338,  4392,  4396,  4460,  4464,  4390,
    4499,  4504,  4509,  4516,  4517,  4528,  4533,  4574,  4527,  4594,
    4593,  4602,  4601,  4612,  4617,  4615,  4621,  4626,  4630,  4625,
    4669,  4668,  4677,  4676,  4687,  4692,  4690,  4696,  4701,  4705,
    4700,  4750,  4757,  4758,  4759,  4866,  4870,  4874,  4882,  4886,
    4881,  4895,  4903,  4907,  4902,  4916,  4924,  4928,  4923,  4937,
    4945,  4949,  4944,  4958,  4965,  4977,  4975,  4998,  5005,  5033,
    5070,  5071,  5075,  5104,  5143,  5147,  5103,  5164,  5168,  5162,
    5209,  5208,  5216,  5223,  5224,  5229,  5228,  5237,  5236,  5245,
    5244,  5253,  5252,  5261,  5260,  5269,  5268,  5277,  5276,  5286,
    5376,  5382,  5407,  5513,  5522,  5526,  5532,  5604,  5676,  5750,
    5749,  5797,  5801,  5805,  5809,  5813,  5817,  5796,  5870,  5869,
    5877,  5884,  5889,  5897,  5901,  5896,  5911,  5912,  5916,  5918,
    5917,  5926,  5925,  5938,  5961,  5936,  5987,  6014,  5985,  6038,
    6039,  6040,  6044,  6045,  6049,  6076,  6106,  6150,  6154,  6104,
    6169,  6178,  6196,  6207,  6206,  6244,  6293,  6297,  6242,  6312,
    6316,  6323,  6327,  6331,  6335,  6339,  6343,  6347,  6351,  6355,
    6359,  6367,  6398,  6411,  6418,  6443,  6461,  6468,  6483,  6490,
    6500,  6504,  6522,  6526,  6499,  6539,  6545,  6548,  6555,  6554,
    6561,  6560,  6567,  6566,  6576,  6645,  6695,  6710,  6722,  6729,
    6788,  6793,  6797,  6792,  6856,  6860,  6855,  6873,  6874,  6879,
    6878,  6887,  6886,  6895,  6894,  6903,  6902
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
  "primary_expr", "literal", "positive_int_expr", "annotation_dcl", "$@59",
  "annotation_body", "annotation_statement", "$@60",
  "annotation_member_type", "annotation_member",
  "annotation_member_default", "at_least_one_annotation",
  "annotations_maybe", "annotation_appl", "annotation_appl_params_maybe",
  "annotation_appl_params", "named_annotation_appl_params",
  "more_named_annotation_appl_params", "named_annotation_appl_param",
  "type_dcl", "$@61", "type_declarator", "$@62", "type_spec",
  "simple_type_spec", "base_type_spec", "template_type_spec",
  "constructed_type_spec", "constructed_forward_type_spec",
  "at_least_one_declarator", "declarators", "$@63", "declarator",
  "at_least_one_simple_declarator", "simple_declarators", "$@64",
  "simple_declarator", "complex_declarator", "integer_type", "signed_int",
  "unsigned_int", "floating_pt_type", "fixed_type", "char_type",
  "octet_type", "boolean_type", "any_type", "object_type", "struct_decl",
  "$@65", "struct_type", "$@66", "$@67", "$@68", "at_least_one_member",
  "members", "member", "@69", "member_i", "$@70", "$@71", "$@72",
  "union_decl", "$@73", "union_type", "$@74", "$@75", "$@76", "$@77",
  "$@78", "$@79", "switch_type_spec", "at_least_one_case_branch",
  "case_branches", "case_branch", "$@80", "$@81", "$@82",
  "at_least_one_case_label", "case_labels", "case_label", "$@83", "$@84",
  "$@85", "element_spec", "$@86", "struct_forward_type",
  "union_forward_type", "enum_type", "$@87", "$@88", "$@89", "$@90",
  "at_least_one_enumerator", "enumerators", "$@91", "enumerator",
  "sequence_type_spec", "$@92", "$@93", "seq_head", "$@94", "$@95",
  "fixed_type_spec", "string_type_spec", "$@96", "$@97", "string_head",
  "wstring_type_spec", "$@98", "$@99", "wstring_head", "array_declarator",
  "$@100", "at_least_one_array_dim", "array_dims", "array_dim", "$@101",
  "$@102", "attribute", "attribute_readonly", "$@103", "$@104", "$@105",
  "$@106", "attribute_readwrite", "$@107", "$@108", "$@109", "$@110",
  "exception", "$@111", "$@112", "$@113", "$@114", "operation", "$@115",
  "$@116", "$@117", "$@118", "opt_op_attribute", "op_type_spec",
  "init_decl", "$@119", "$@120", "$@121", "init_parameter_list", "$@122",
  "$@123", "at_least_one_in_parameter", "in_parameters", "$@124",
  "in_parameter", "$@125", "$@126", "parameter_list", "$@127", "$@128",
  "at_least_one_parameter", "parameters", "$@129", "parameter", "$@130",
  "$@131", "param_type_spec", "direction", "opt_raises", "$@132", "$@133",
  "opt_getraises", "$@134", "$@135", "opt_setraises", "$@136", "$@137",
  "opt_context", "$@138", "$@139", "at_least_one_string_literal",
  "string_literals", "$@140", "typeid_dcl", "typeprefix_dcl", "component",
  "component_forward_decl", "component_decl", "$@141", "$@142", "$@143",
  "component_header", "$@144", "$@145", "component_inheritance_spec",
  "$@146", "component_exports", "component_export", "$@147", "$@148",
  "$@149", "$@150", "$@151", "$@152", "$@153", "provides_decl",
  "interface_type", "uses_decl", "uses_opt_multiple", "opt_multiple",
  "emits_decl", "publishes_decl", "consumes_decl", "home_decl", "$@154",
  "home_header", "$@155", "$@156", "$@157", "$@158", "$@159", "$@160",
  "home_inheritance_spec", "$@161", "primary_key_spec", "home_body",
  "$@162", "$@163", "home_exports", "home_export", "$@164", "$@165",
  "factory_decl", "$@166", "$@167", "finder_decl", "$@168", "$@169",
  "event", "event_forward_decl", "event_concrete_forward_decl",
  "event_abs_forward_decl", "event_abs_decl", "$@170", "$@171", "$@172",
  "event_abs_header", "event_custom_header", "event_plain_header",
  "event_rest_of_header", "$@173", "event_decl", "$@174", "$@175", "$@176",
  "event_header", "formal_parameter_type", "at_least_one_formal_parameter",
  "formal_parameters", "formal_parameter",
  "at_least_one_formal_parameter_name", "formal_parameter_names",
  "formal_parameter_name", "porttype_decl", "$@177", "$@178", "$@179",
  "$@180", "at_least_one_port_export", "port_exports", "port_export",
  "$@181", "$@182", "$@183", "extended_port_decl",
  "at_least_one_actual_parameter", "actual_parameters", "actual_parameter",
  "connector_decl", "connector_header", "$@184", "$@185", "connector_body",
  "$@186", "$@187", "connector_exports", "connector_export", "$@188",
  "$@189", "$@190", "$@191", YY_NULLPTR
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

#define YYPACT_NINF -629

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-629)))

#define YYTABLE_NINF -538

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -629,    57,  1243,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,    71,    81,    50,    73,  -629,    71,
      71,  -629,    40,    40,  -629,  -629,    71,  -629,  -629,     6,
    -629,   223,    25,    72,  -629,  -629,    12,  -629,  -629,  -629,
    -629,  -629,  -629,   344,  -629,  -629,  -629,  -629,  -629,  1427,
      42,  -629,  -629,    75,  -629,   144,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,    80,  -629,  -629,  -629,    80,  -629,
    -629,    96,   115,  1598,    40,    71,  1536,    71,    71,    71,
      71,  -629,  -629,  -629,    79,    71,   122,  -629,   125,    71,
    -629,    80,    71,   123,   126,    71,  -629,  -629,    15,  -629,
      27,  -629,  -629,   128,  -629,   132,   135,   570,  -629,  -629,
    -629,   138,   186,  -629,   153,   157,   160,   142,  -629,   168,
    -629,  -629,  -629,  -629,  -629,  -629,   176,  -629,  -629,  -629,
    -629,  -629,  -629,   192,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,   144,  -629,  -629,  -629,    97,  -629,  -629,   184,  -629,
     188,   212,   221,  -629,    40,   202,   225,   222,  -629,   227,
     228,   230,   234,   235,   237,   239,   243,  -629,  -629,  -629,
     244,   245,  -629,  -629,  -629,  -629,   192,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,   192,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,   246,  -629,   247,  -629,  -629,
     251,  -629,   332,  -629,  -629,  -629,    41,    44,  -629,  -629,
    -629,  1598,  -629,  -629,  -629,  -629,   248,  -629,  -629,  -629,
    -629,   335,  -629,  -629,   120,   253,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,   338,  -629,   181,   249,   301,  -629,
    -629,  -629,  -629,  -629,  -629,    13,  -629,  -629,   261,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,   301,   282,
     283,  -629,  -629,  -629,    71,    71,   284,   285,  -629,  -629,
    -629,   286,  -629,   332,  -629,  -629,  -629,  -629,  -629,  -629,
     373,  -629,   289,   290,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,    60,    60,    60,   181,   192,  -629,
    -629,   281,   288,   280,   170,   164,   129,  -629,  -629,  -629,
    -629,  -629,    40,  -629,  -629,  -629,  -629,   292,  -629,    40,
    -629,   181,   181,   181,   181,  -629,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,   238,  -629,   293,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,    40,   301,  -629,  -629,  -629,   302,
     251,   699,  1296,   300,   303,  -629,   570,  -629,  -629,  -629,
     291,   181,   181,   181,   181,   181,   181,   181,   181,   181,
     181,   307,    71,  -629,   192,  1079,   827,   181,  -629,  -629,
    -629,  -629,  -629,   308,  -629,   296,  -629,   312,  -629,  -629,
    -629,  -629,   384,   995,  -629,  -629,  -629,  -629,    65,   349,
      40,    40,  -629,  -629,  -629,  -629,  -629,    65,  -629,   319,
    -629,   315,   304,   318,  -629,  -629,  1114,   192,  -629,    40,
     301,  -629,  -629,  -629,  -629,   328,  -629,  -629,    71,  -629,
    -629,   333,   334,   419,   337,  -629,  -629,   288,   280,   170,
     164,   164,   129,   129,  -629,  -629,  -629,  -629,  -629,   329,
    -629,  -629,  -629,   340,  -629,  -629,  -629,  -629,  -629,  -629,
    1571,  -629,  -629,  -629,  -629,  -629,   341,  -629,  -629,  -629,
     339,   336,   885,   347,   350,   351,   181,  -629,   332,  -629,
     356,   357,  1376,   101,    40,    40,    40,  -629,   358,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,    71,    71,  -629,   365,
    -629,  -629,  -629,  1198,   911,   410,  1171,  -629,   192,   332,
    -629,  -629,    83,    88,   368,   369,   371,   332,   372,  -629,
    -629,  -629,  -629,  -629,  -629,    64,  -629,  -629,   375,   376,
     192,  -629,    89,  1536,  -629,   434,  -629,  -629,  -629,  -629,
     120,  -629,   379,  -629,   380,   381,   385,   387,   389,  -629,
     192,  -629,  -629,  -629,  -629,  -629,   390,   391,  -629,  1517,
    1517,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,   308,
     312,  -629,  -629,  -629,  -629,  -629,   464,   192,  -629,  -629,
    -629,  -629,  -629,  -629,    90,    90,    90,   397,  -629,   398,
     399,   400,   401,   402,   403,  -629,  -629,  -629,   407,   408,
     409,   411,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,  -629,   181,   181,   304,    71,  -629,   412,   404,   413,
    -629,   431,  -629,  -629,  -629,  -629,  -629,   417,   181,   420,
    1402,  -629,    40,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,  -629,    69,  -629,  -629,  -629,   421,    71,   394,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,   405,
     405,  -629,  -629,  -629,  -629,  1171,    71,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,   423,   426,   428,   427,    89,
    -629,  -629,  -629,    40,  -629,  -629,  -629,  -629,   430,   192,
      70,  -629,  -629,  -629,  -629,   484,  -629,  -629,  -629,  -629,
    -629,   415,  -629,    40,  -629,  -629,  -629,  -629,  -629,   192,
     436,   561,  -629,  -629,   405,   432,   439,   424,   486,   491,
     491,    71,   477,   440,  -629,  -629,   192,   442,  -629,  -629,
     435,  -629,  -629,  -629,  -629,  -629,   429,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,   478,   533,   437,  -629,   491,
     146,  1171,  -629,   450,   441,   491,   445,   483,    71,    40,
    -629,  -629,   455,  -629,   449,   201,   491,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,   192,  -629,   462,  -629,  -629,  -629,  -629,
     453,  -629,  -629,  -629,   472,   181,   468,   473,    53,  -629,
     242,    71,   486,    40,    40,   459,    71,   533,  -629,   475,
    1171,   531,  -629,  -629,  -629,  -629,  -629,  1491,  -629,  -629,
    -629,   465,   466,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
     480,  -629,  -629,  -629,  -629,    40,   201,    71,   474,  -629,
      71,   489,   479,  -629,  -629,  -629,  -629,  -629,  -629,   503,
    -629,   481,   492,  -629,  -629,   505,  -629
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,     0,     3,     1,    38,   144,    40,    70,   220,
     281,   297,   332,   380,     0,     0,     0,     0,    94,     0,
       0,   491,     0,     0,   560,   581,     0,     6,     7,    42,
      24,    61,     0,     0,    22,    64,    77,    66,    26,    78,
      83,    79,    84,    77,    80,    81,    65,    18,    10,     0,
       0,    12,   226,   283,   222,   331,   223,   249,   250,   224,
      20,    14,    16,    28,   451,   450,   453,    30,   489,    32,
     521,   523,   522,   520,    77,   539,   540,   519,    77,    34,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   140,   261,   225,    77,     0,    77,    88,    77,     0,
      82,    77,     0,   457,   532,     0,   139,   135,     0,   134,
       0,   210,   210,     0,    46,     0,     0,     0,   210,     8,
       9,     0,    97,    72,     0,     0,     0,   265,   267,     0,
     271,   272,   275,   276,   277,   278,   274,   279,   280,   346,
     354,   359,    92,   233,   102,   229,   231,   232,   230,   234,
     263,   264,   235,   239,   236,   238,   237,   240,   241,   283,
     246,     0,   247,   248,   242,     0,   245,   243,   353,   244,
     358,     0,     0,     5,     0,   208,     0,     0,   299,     0,
       0,     0,     0,     0,     0,     0,     0,   533,   526,   535,
       0,     0,   584,   580,    39,   274,   157,   145,   149,   153,
     154,   150,   151,   152,   155,   156,    41,    71,   221,   227,
     282,   298,   333,   381,    73,   530,    74,     0,   531,    95,
     462,   492,     0,   448,   137,   449,     0,     0,   194,    43,
      25,     0,   546,   542,   543,   548,   545,   549,   547,   544,
     541,     0,    48,   553,     0,     0,    23,    96,    75,    67,
      27,    85,   266,   273,   268,   270,     0,     0,    99,   345,
     342,   350,   355,    19,    11,   213,    13,   284,     0,    21,
      15,    17,    29,   454,    31,   503,   490,    33,    99,     0,
       0,    35,    37,   588,     0,     0,     0,     0,    89,   460,
     458,   500,   136,     0,   561,   209,   582,   197,     4,   550,
       0,   554,     0,   551,   183,   184,   185,   187,   190,   189,
     191,   192,   188,   186,     0,     0,     0,     0,   180,   579,
     158,   159,   160,   162,   164,   166,   169,   172,   176,   181,
     578,    62,     0,   113,   104,   269,   193,     0,   347,     0,
      93,     0,     0,     0,     0,   211,   290,   300,   464,   507,
     534,   527,   536,   585,   146,   261,   228,   254,   255,   256,
     262,   334,   382,   113,     0,    99,   498,   493,   138,     0,
     462,     0,     0,     3,     0,    49,     0,   177,   178,   179,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   576,     0,    76,   133,     0,     0,     0,   210,    98,
     343,   351,   356,   134,   214,     0,   215,   218,   285,   289,
     210,   210,   455,     0,   113,   104,   370,   375,     0,   485,
       0,     0,   593,   368,   369,   589,   591,     0,   595,     0,
     587,     0,     0,   251,   210,   289,     0,   461,   459,     0,
      99,   562,   583,   201,   195,     0,   203,   196,     0,   198,
     204,     0,     0,     0,     0,   552,   182,   161,   163,   165,
     167,   168,   170,   171,   173,   174,   175,   210,    63,   130,
     128,   390,   391,     0,   112,   120,   114,   124,   122,   126,
       0,   116,   118,   395,   110,   108,     0,   103,   105,   106,
       0,     0,     0,     0,     0,     0,     0,   212,     0,   216,
       0,   290,     0,     0,     0,     0,     0,   475,     0,   463,
     465,   467,   469,   471,   473,   477,     0,     0,   508,     0,
     506,   509,   511,     0,     0,     0,     0,   481,   480,     0,
     484,   483,     0,     0,     0,     0,     0,     0,     0,   586,
     147,   365,   361,   364,   252,     0,   335,   340,   290,     0,
     499,   494,     0,     0,   199,   207,   200,    45,   555,    50,
       0,   131,     0,    69,     0,     0,     0,     0,     0,   394,
     424,   421,   422,   423,   385,   393,     0,     0,   210,     0,
       0,    87,   107,   349,   348,   344,   352,   357,   219,     0,
     218,   286,   288,   295,   292,   291,   265,   311,   306,   307,
     308,   309,   301,   310,     0,     0,     0,     0,   456,     0,
       0,     0,     0,     0,     0,   513,   516,   505,     0,     0,
       0,     0,   371,   376,   479,   574,   575,   594,   590,   592,
     482,   596,     0,     0,   362,     0,   341,     0,   337,     0,
      91,     0,   572,   568,   570,   563,   567,     0,     0,     0,
       0,   577,     0,   129,   121,   115,   125,   123,   127,   210,
     117,   119,     0,   111,   109,   217,     0,     0,     0,   488,
     486,   487,   476,   466,   468,   470,   472,   474,   478,     0,
       0,   510,   512,   529,   538,     0,     0,   148,   366,   363,
     253,   336,   338,   384,   495,     0,     0,     0,     0,   565,
     202,   206,   205,     0,    56,    42,    51,    55,     0,   132,
       0,   396,   296,   293,   302,   399,   514,   517,   372,   377,
     260,     0,   210,     0,   573,   569,   571,   564,   566,    58,
       0,     0,    57,   386,     0,     0,     0,     0,     0,   431,
     431,     0,   435,   257,   367,   339,   496,     0,    52,    54,
       0,   397,   294,   303,   400,   407,     0,   406,   428,   515,
     518,   373,   432,   378,   258,   502,     0,   412,   387,   431,
       0,     0,   402,   403,     0,   431,     0,   439,     0,     0,
     497,   559,     0,   558,     0,     0,   431,   398,   318,   325,
     323,   304,   314,   315,   322,   408,   404,   429,   374,   433,
     436,   379,   259,   501,    59,   556,   411,   425,   426,   427,
       0,   417,   418,   388,     0,     0,     0,     0,     0,   210,
     320,     0,     0,     0,     0,     0,     0,     0,   413,   414,
       0,   443,   319,   326,   324,   305,   313,     0,   321,   409,
     405,     0,     0,   437,    60,   557,   415,   419,   440,   389,
       0,   328,   316,   430,   434,     0,     0,     0,     0,   327,
       0,     0,     0,   416,   420,   441,   329,   317,   438,     0,
     447,     0,   444,   442,   445,     0,   446
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -629,  -629,   294,   297,   534,  -601,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,  -599,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,  -142,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,   272,  -629,
    -629,   112,  -629,  -629,  -629,   581,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,   583,  -629,   275,  -629,  -629,  -238,  -629,
    -629,   185,  -629,  -629,  -629,  -629,  -313,  -354,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,  -331,  -629,  -629,   -22,
    -629,  -629,  -186,   -10,  -629,    16,  -629,  -629,  -629,  -629,
    -190,   -27,  -219,  -629,   226,   224,   229,   -91,   -90,  -149,
     -32,  -629,  -304,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,  -629,   -84,   558,  -629,  -629,  -629,    26,   117,    17,
    -629,    67,  -629,   -40,   130,  -450,  -629,  -629,  -629,   -49,
    -629,  -629,  -609,  -120,  -629,  -629,    -8,  -629,   -70,  -629,
    -629,   -51,   -48,   -69,   -63,   -52,   255,  -629,   -38,  -629,
     -31,  -629,  -629,  -629,  -629,   193,   295,  -629,  -275,  -629,
    -629,  -629,   -28,  -629,   -26,  -629,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -191,  -629,  -629,  -629,  -629,  -629,
    -185,  -629,  -629,  -629,  -629,  -629,  -629,  -629,   -41,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,   -93,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,   -76,  -629,  -629,  -629,   -72,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,     0,  -629,  -629,  -324,
    -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
      20,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,  -628,  -629,  -629,  -629,
    -629,  -629,  -183,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,  -220,  -629,  -629,  -493,  -629,  -553,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,    21,    22,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,  -629,   270,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,  -300,   215,  -270,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,  -629,   565,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,  -629,   268,  -629,  -629,
    -182,  -629,  -629,  -629,  -629,  -629,  -629,  -629,   -53,  -629,
    -629,  -629,   236,  -629,  -629,    92,  -629,  -629,  -629,  -629,
    -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629,  -629
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     3,    27,    28,   172,   176,   180,   181,
     171,   179,   121,   116,   125,   182,   184,   186,   190,   191,
      82,    29,    84,    30,   115,   298,   452,    31,    32,   117,
     302,   454,   650,   730,   706,   731,   707,   708,   747,   826,
      33,   118,   392,    34,    35,   124,   333,   473,    36,    85,
      37,   142,   332,    38,    39,    40,   126,   334,   486,    41,
     217,   363,   549,    42,   258,    43,   102,   248,   340,    44,
      45,   396,   487,   488,   580,   579,   395,   474,   565,   576,
     577,   564,   567,   566,   568,   562,   393,   469,   652,   318,
     222,   293,   109,   355,    46,   475,    83,   284,   431,   632,
     197,   319,   336,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   337,    48,   297,   371,   447,   553,   448,   449,
     649,    49,    50,   295,   345,   405,   406,   499,   407,   476,
      86,   208,   285,   594,   145,   146,   147,   148,    52,   356,
     433,   635,   357,   719,   743,   778,   358,   359,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    53,    87,
      54,   177,   346,   500,   408,   501,   592,   410,   595,   667,
     735,   666,    55,    88,    56,   268,   411,   668,   736,   770,
     817,   602,   791,   818,   792,   819,   861,   814,   793,   820,
     794,   816,   815,   850,   852,   860,    57,    58,    59,    89,
     286,   434,   637,   546,   638,   722,   547,   164,   341,   493,
     165,   257,   398,   166,   167,   342,   494,   168,   169,   343,
     495,   170,   360,   432,   542,   634,   543,   633,   721,   477,
     423,   525,   685,   741,   775,   424,   526,   686,   742,   777,
     478,    90,   287,   435,   639,   479,   659,   750,   786,   831,
     480,   574,   490,   578,   734,   769,   716,   737,   738,   756,
     773,   822,   757,   771,   821,   768,   784,   785,   810,   829,
     856,   811,   830,   857,   575,   812,   759,   774,   823,   763,
     776,   824,   801,   825,   855,   849,   858,   869,   871,   872,
     875,   481,   482,    63,    64,    65,   183,   348,   508,    66,
     220,   365,   290,   364,   412,   509,   609,   610,   611,   612,
     613,   607,   614,   643,   529,   644,   427,   531,   512,   513,
     514,    67,   185,    68,   105,   291,   440,   641,   723,   765,
     367,   439,   780,   276,   349,   519,   413,   520,   618,   619,
     521,   679,   739,   522,   680,   740,    69,    70,    71,    72,
      73,   279,   414,   620,    74,    75,    76,   188,   278,    77,
     280,   415,   621,    78,   241,   242,   303,   243,   782,   805,
     783,    79,   111,   369,   552,   698,   645,   699,   646,   696,
     697,   695,   428,   245,   391,   330,    80,    81,   112,   370,
     193,   283,   429,   353,   430,   535,   536,   534,   538
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     108,   110,   163,   144,    92,   159,    93,   204,   399,   103,
     104,   205,   160,   198,   201,   161,   113,   162,    47,    51,
     202,   143,    60,    61,    62,   320,   690,   226,   227,   422,
     571,   203,   199,   623,   244,   200,   292,   400,   401,   402,
     350,   299,   489,   106,   294,   163,   209,   296,   159,   704,
     436,   705,   717,   425,   788,   160,     8,     4,   161,   518,
     162,   196,   206,   106,   143,    47,    51,   636,   106,    60,
      61,    62,   711,   733,    91,   207,   571,   210,   211,   212,
     213,   789,   790,   426,   527,   215,   625,     8,   507,   218,
     223,   626,   219,   106,   224,   221,   224,   114,   380,    18,
    -141,   523,   225,    95,   106,   122,   751,   368,   224,   596,
     128,   129,   510,   119,   132,   133,   134,   135,   344,   416,
     417,   107,    18,   106,    12,   404,    99,   438,   174,   174,
     704,   174,   705,   304,   305,   306,   307,   308,   309,   310,
     311,   107,   511,  -312,   312,   313,   107,   788,   123,   418,
     252,   174,   265,   253,   419,   204,   174,   174,   403,   205,
     120,   198,   201,  -330,   224,   317,   178,  -142,   202,   224,
     489,   224,   122,   122,   789,   790,   254,   255,   491,   203,
     199,   446,   107,   200,   106,   192,   187,   760,   174,   259,
     187,   260,   718,   304,   305,   306,   307,   308,   309,   310,
     311,   107,   551,   194,   312,   313,   214,   174,   216,   196,
    -143,  -452,   839,  -100,  -524,   122,   787,   228,   122,   314,
     315,   229,   798,   230,   316,   317,   246,   -47,   642,   -47,
     388,   389,   390,   813,   247,   571,   807,   808,   809,   464,
     465,   466,   249,   -47,   -47,   250,   -47,   -47,   864,   251,
     -47,   866,   384,   385,   304,   305,   306,   307,   308,   309,
     310,   311,   107,   386,   387,   312,   313,   256,   416,   417,
     789,   790,   -47,   224,   354,   261,   -47,   588,   795,   262,
     314,   315,   377,   378,   379,   316,   317,    94,    96,  -209,
     -47,    98,   101,   460,   461,   204,   462,   463,   418,   205,
     263,   198,   201,   419,   663,   664,   420,   421,   202,   264,
     394,   267,   589,   266,   492,   269,   270,   394,   271,   203,
     199,   571,   272,   200,   273,   274,   502,   503,   275,   688,
     451,   277,   281,   282,  -525,   106,   288,   847,   301,   300,
     338,   320,   437,   624,   289,   331,   335,   106,   339,   196,
     545,   630,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,    10,    11,   347,    12,   139,   140,
     141,   351,   352,   361,   362,   642,   374,   381,   383,   366,
     571,   375,   468,   560,   376,   382,   397,   445,    47,    51,
     -44,   441,    60,    61,    62,   453,   528,   456,   532,   533,
    -360,   467,   497,   496,   572,   528,   498,   530,   573,   539,
     540,   541,   544,   320,   416,   417,   554,   550,   669,   670,
     671,   556,   558,   561,   557,   107,   559,   582,   583,   701,
     563,   581,  -101,   598,   599,   504,   505,   122,   555,   585,
     600,   622,   586,   587,   418,   506,   591,  -287,   608,   419,
     572,   601,   420,   421,   573,   617,   627,   628,   570,   629,
     631,   163,   603,   648,   159,  -383,   640,   653,   654,   655,
     143,   160,   252,   656,   161,   657,   162,   658,   660,   661,
     143,   597,   604,   605,   606,   672,   673,   674,   675,   676,
     677,   678,   841,   842,   662,   681,   682,   694,   692,   683,
     714,   684,   691,   693,   570,   700,   615,   616,   702,   712,
     715,   724,   163,   209,   725,   159,   726,   727,   732,  -401,
     752,   755,   160,   744,   862,   161,   748,   162,   753,   758,
     754,   143,   762,   766,   764,   772,   781,   779,   163,   163,
     767,   159,   159,  -410,   796,   800,   797,   804,   160,   160,
     799,   161,   161,   162,   162,   806,   827,   143,   143,   828,
     832,   834,     5,   835,   843,     6,     7,     8,     9,   846,
     848,   853,   854,   859,   231,   710,   232,   867,   870,   865,
     876,    10,    11,   173,    12,   868,   874,   873,    13,   749,
     233,   234,   372,   235,   236,   373,   833,   237,   100,    97,
     524,    14,    15,    16,    17,   687,   458,   457,   175,   572,
      18,    19,   459,   573,    20,   590,   665,    21,   713,   238,
     647,   761,   584,   239,    22,    23,   450,   836,   548,   745,
     709,    24,    25,   703,   689,   838,   863,   240,   545,   840,
     442,   409,   537,   189,   455,   845,   728,    26,   515,     0,
       0,   -53,   651,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   570,     0,     0,    47,    51,     0,     0,
      60,    61,    62,     0,     0,     0,    92,     0,   720,     0,
       0,   729,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   572,     0,     0,     0,   573,
       0,   746,   106,     6,     0,     0,   443,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   195,   137,     0,     0,
       0,     0,    12,     0,   140,   141,     0,     0,     0,     0,
       0,    92,     0,   720,     0,   837,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    47,    51,   570,
       0,    60,    61,    62,   572,     0,     0,   803,   573,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
     802,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     107,     0,     0,     0,     0,     0,     0,     0,     0,   444,
       0,     0,     0,     0,     0,     0,   163,   851,     0,   159,
       0,   394,   394,     0,     0,     0,   160,     0,   570,   161,
       0,   162,     0,     0,     0,   143,   844,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   470,     0,
    -392,     6,     0,   394,     9,  -392,  -392,  -392,  -392,  -392,
    -392,  -392,  -392,  -392,  -392,  -392,  -392,    10,    11,     0,
      12,     0,  -392,  -392,    13,     0,     0,   416,   417,   471,
     472,  -392,     0,     0,     0,     0,     0,    14,     0,     0,
       0,   483,   484,   485,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   106,     0,
      22,    23,     0,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,     0,     0,     0,  -392,   139,
     140,   141,   470,     0,  -392,     6,     0,   -86,     9,  -392,
    -392,  -392,  -392,  -392,  -392,  -392,  -392,  -392,  -392,  -392,
    -392,    10,    11,     0,    12,     0,  -392,  -392,    13,     0,
       0,   416,   417,   471,   472,  -392,     0,     0,     0,     0,
       0,    14,     0,     0,     0,   483,   484,   485,     0,     0,
       0,     0,     0,     0,     0,     0,   107,     0,     0,     0,
       0,     0,   174,     0,    22,    23,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -392,     0,     0,     0,   470,     0,  -392,     6,
       0,  -537,     9,  -392,  -392,  -392,  -392,  -392,  -392,  -392,
    -392,  -392,  -392,  -392,  -392,    10,    11,     0,    12,     0,
    -392,  -392,    13,     0,     0,   416,   417,   471,   472,  -392,
       0,     0,     0,     0,     0,    14,     0,     0,     0,   516,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   517,
       0,     0,     0,     0,     0,     0,     0,     0,    22,    23,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -392,     0,     0,     0,
     470,     0,  -392,     6,     0,  -504,     9,  -392,  -392,  -392,
    -392,  -392,  -392,  -392,  -392,  -392,  -392,  -392,  -392,    10,
      11,     0,    12,     0,  -392,  -392,    13,     0,     0,   416,
     417,   471,   472,  -392,     0,   470,     0,  -392,     6,    14,
       0,     9,  -392,  -392,  -392,  -392,  -392,  -392,  -392,  -392,
    -392,  -392,  -392,  -392,    10,    11,     0,    12,     0,  -392,
    -392,    13,    22,    23,   416,   417,   471,   472,  -392,     0,
       0,     0,     0,     0,    14,     0,     0,     0,     0,     0,
    -392,     0,     0,     0,     0,     0,     0,     0,     0,   -68,
       0,     0,     0,     0,   106,     0,     0,    22,    23,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   195,   137,
     138,     0,     0,     0,     0,  -392,   140,   141,     0,   470,
       0,  -392,     6,     0,   -90,     9,  -392,  -392,  -392,  -392,
    -392,  -392,  -392,  -392,  -392,  -392,  -392,  -392,    10,    11,
       0,    12,     0,  -392,  -392,    13,     0,     0,   416,   417,
     471,   472,  -392,     0,     0,     0,     0,     0,    14,     0,
       0,     0,     0,    -2,     5,     0,     0,     6,     7,     8,
       9,     0,   107,     0,     0,     0,     0,     0,     0,     0,
       0,    22,    23,    10,    11,     0,    12,     0,     0,     0,
      13,     0,     0,     0,     0,     0,     0,     0,     0,  -392,
       0,     0,     0,    14,    15,    16,    17,     0,  -528,     0,
       0,     0,    18,    19,     0,     0,    20,     5,     0,    21,
       6,     7,     8,     9,     0,     0,    22,    23,     0,     0,
       0,     0,     0,    24,    25,     0,    10,    11,     0,    12,
       0,     0,     0,    13,     0,     0,     0,     0,     0,    26,
    -210,     0,     0,     0,     0,     0,    14,    15,    16,    17,
       0,     0,     0,     0,     0,    18,    19,     0,     0,    20,
       0,     0,    21,     0,     0,     0,     0,     0,     0,    22,
      23,     0,     0,     0,     0,     0,    24,    25,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   593,     0,   106,
       0,     0,    26,  -210,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,    10,    11,     0,    12,
     139,   140,   141,     5,     0,     0,     6,     7,     8,     9,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    10,    11,     0,    12,     0,     0,     5,    13,
       0,     6,     7,     8,     9,     0,     0,     0,     0,     0,
       0,     0,    14,    15,    16,    17,     0,    10,    11,     0,
      12,    18,    19,     0,    13,    20,     0,   107,    21,     0,
       0,     0,     0,   174,     0,    22,    23,    14,    15,    16,
      17,     0,    24,    25,   703,     0,    18,    19,     0,     0,
      20,     0,     0,    21,     0,     0,     0,     0,    26,     0,
      22,    23,     0,     0,   106,     0,     0,    24,    25,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,    10,    11,    26,    12,   139,   140,   141,   593,     0,
     106,     0,     0,     0,     0,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    10,    11,   106,
      12,   139,   140,   141,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,    10,    11,     0,    12,
     139,   140,   141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   107,     0,   106,     0,     0,     0,   174,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   195,   137,
     138,     0,     0,     0,     0,     0,   140,   141,   107,     0,
       0,   106,     0,     0,     0,   569,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   195,     0,   107,     0,     0,
       0,     0,     0,   140,   141,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   107
};

static const yytype_int16 yycheck[] =
{
      22,    23,    43,    43,    14,    43,    14,    83,   339,    19,
      20,    83,    43,    83,    83,    43,    26,    43,     2,     2,
      83,    43,     2,     2,     2,   244,   635,   111,   112,   353,
     480,    83,    83,   526,   118,    83,   222,   341,   342,   343,
     278,   231,   396,     3,     3,    86,    86,     3,    86,   650,
     363,   650,   680,   353,     1,    86,     6,     0,    86,   413,
      86,    83,    84,     3,    86,    49,    49,     3,     3,    49,
      49,    49,     3,     3,     3,    85,   526,    87,    88,    89,
      90,    28,    29,   353,    19,    95,     3,     6,   412,    99,
      75,     3,   102,     3,    81,   105,    81,    91,   317,    49,
      88,   414,    75,    53,     3,    93,   734,   293,    81,     8,
       9,    10,   412,    88,    13,    14,    15,    16,   105,    30,
      31,    81,    49,     3,    23,   344,    53,   365,    87,    87,
     731,    87,   731,    73,    74,    75,    76,    77,    78,    79,
      80,    81,   412,    90,    84,    85,    81,     1,    36,    60,
       8,    87,   174,    11,    65,   231,    87,    87,   344,   231,
      88,   231,   231,    88,    81,   105,    22,    88,   231,    81,
     524,    81,    93,    93,    28,    29,     8,     9,   397,   231,
     231,   371,    81,   231,     3,    89,    74,   740,    87,    92,
      78,    94,   685,    73,    74,    75,    76,    77,    78,    79,
      80,    81,   440,    88,    84,    85,    94,    87,    96,   231,
      88,    88,   821,    88,    88,    93,   769,    89,    93,    99,
     100,    89,   775,    88,   104,   105,    88,     4,   552,     6,
     101,   102,   103,   786,    48,   685,    35,    36,    37,   388,
     389,   390,    89,    20,    21,    88,    23,    24,   857,    89,
      27,   860,    82,    83,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    99,   100,    84,    85,    91,    30,    31,
      28,    29,    49,    81,   284,    91,    53,   496,   771,    91,
      99,   100,   314,   315,   316,   104,   105,    15,    16,    87,
      67,    16,    17,   384,   385,   371,   386,   387,    60,   371,
      88,   371,   371,    65,   579,   580,    68,    69,   371,    88,
     332,    89,   498,    88,   398,    88,    88,   339,    88,   371,
     371,   771,    88,   371,    89,    88,   410,   411,    89,   633,
     371,    88,    88,    88,    88,     3,    89,   830,     3,    91,
      91,   560,   364,   529,    93,    92,     8,     3,    47,   371,
     434,   537,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,   105,    23,    24,    25,
      26,    89,    89,    89,    89,   699,     3,    96,    98,    93,
     830,    92,   392,   467,    94,    97,    94,   371,   372,   372,
      90,    89,   372,   372,   372,    92,   418,   106,   420,   421,
     107,    94,   106,    95,   480,   427,    94,    58,   480,    90,
      95,   107,    94,   632,    30,    31,    88,   439,   604,   605,
     606,    88,     3,    94,    90,    81,    89,    88,    92,   648,
      90,    90,    88,   503,   503,    51,    52,    93,   448,    92,
     503,    31,    92,    92,    60,    61,    90,    90,    90,    65,
     526,   503,    68,    69,   526,    90,    88,    88,   480,    88,
      88,   502,   503,    29,   502,    90,    90,    88,    88,    88,
     492,   502,     8,    88,   502,    88,   502,    88,    88,    88,
     502,   503,   504,   505,   506,    88,    88,    88,    88,    88,
      88,    88,   823,   824,   578,    88,    88,    66,    94,    90,
     106,    90,    90,    90,   526,    88,   516,   517,    88,    88,
     105,    88,   553,   553,    88,   553,    88,    90,    88,    35,
      88,    35,   553,   108,   855,   553,    90,   553,    89,    38,
     106,   553,    55,    91,    94,   106,     3,    59,   579,   580,
     105,   579,   580,   106,    94,    62,   105,    92,   579,   580,
     105,   579,   580,   579,   580,   106,    94,   579,   580,   106,
      88,    93,     1,    90,   105,     4,     5,     6,     7,    94,
      39,   106,   106,    93,     4,   659,     6,    88,    75,   105,
      75,    20,    21,    49,    23,   106,    94,   106,    27,   731,
      20,    21,   298,    23,    24,   298,   815,    27,    17,    16,
     415,    40,    41,    42,    43,   632,   382,   381,    50,   685,
      49,    50,   383,   685,    53,   498,   590,    56,   667,    49,
     553,   741,   492,    53,    63,    64,   371,   818,   435,   722,
     652,    70,    71,    72,   634,   820,   856,    67,   722,   822,
     370,   346,   427,    78,   376,   827,   699,    86,   412,    -1,
      -1,    90,   560,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   685,    -1,    -1,   650,   650,    -1,    -1,
     650,   650,   650,    -1,    -1,    -1,   686,    -1,   686,    -1,
      -1,   703,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   771,    -1,    -1,    -1,   771,
      -1,   723,     3,     4,    -1,    -1,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    23,    -1,    25,    26,    -1,    -1,    -1,    -1,
      -1,   741,    -1,   741,    -1,   819,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   731,   731,   771,
      -1,   731,   731,   731,   830,    -1,    -1,   779,   830,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   778,    -1,
     778,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    -1,   837,   837,    -1,   837,
      -1,   823,   824,    -1,    -1,    -1,   837,    -1,   830,   837,
      -1,   837,    -1,    -1,    -1,   837,   826,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,    -1,   855,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    -1,
      23,    -1,    25,    26,    27,    -1,    -1,    30,    31,    32,
      33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      63,    64,    -1,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    81,    24,
      25,    26,     1,    -1,     3,     4,    -1,    90,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    -1,    25,    26,    27,    -1,
      -1,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    44,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,     1,    -1,     3,     4,
      -1,    90,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    23,    -1,
      25,    26,    27,    -1,    -1,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,
       1,    -1,     3,     4,    -1,    90,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    -1,    25,    26,    27,    -1,    -1,    30,
      31,    32,    33,    34,    -1,     1,    -1,     3,     4,    40,
      -1,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    -1,    23,    -1,    25,
      26,    27,    63,    64,    30,    31,    32,    33,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    -1,     3,    -1,    -1,    63,    64,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    -1,    81,    25,    26,    -1,     1,
      -1,     3,     4,    -1,    90,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    23,    -1,    25,    26,    27,    -1,    -1,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,     0,     1,    -1,    -1,     4,     5,     6,
       7,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    64,    20,    21,    -1,    23,    -1,    -1,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,
      -1,    -1,    -1,    40,    41,    42,    43,    -1,    90,    -1,
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
      -1,     3,    -1,    -1,    -1,    34,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    81
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   110,   111,   112,     0,     1,     4,     5,     6,     7,
      20,    21,    23,    27,    40,    41,    42,    43,    49,    50,
      53,    56,    63,    64,    70,    71,    86,   113,   114,   130,
     132,   136,   137,   149,   152,   153,   157,   159,   162,   163,
     164,   168,   172,   174,   178,   179,   203,   204,   222,   230,
     231,   238,   247,   267,   269,   281,   283,   305,   306,   307,
     349,   400,   401,   402,   403,   404,   408,   430,   432,   455,
     456,   457,   458,   459,   463,   464,   465,   468,   472,   480,
     495,   496,   129,   205,   131,   158,   239,   268,   282,   308,
     350,     3,   202,   255,   157,    53,   157,   172,   174,    53,
     164,   174,   175,   202,   202,   433,     3,    81,   198,   201,
     198,   481,   497,   202,    91,   133,   122,   138,   150,    88,
      88,   121,    93,   160,   154,   123,   165,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    24,
      25,    26,   160,   198,   242,   243,   244,   245,   246,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     269,   281,   283,   307,   316,   319,   322,   323,   326,   327,
     330,   119,   115,   113,    87,   232,   116,   270,    22,   120,
     117,   118,   124,   405,   125,   431,   126,   160,   466,   466,
     127,   128,    89,   499,    88,    17,   198,   209,   257,   260,
     261,   262,   263,   264,   323,   327,   198,   202,   240,   242,
     202,   202,   202,   202,   160,   202,   160,   169,   202,   202,
     409,   202,   199,    75,    81,    75,   231,   231,    89,    89,
      88,     4,     6,    20,    21,    23,    24,    27,    49,    53,
      67,   473,   474,   476,   231,   492,    88,    48,   176,    89,
      88,    89,     8,    11,     8,     9,    91,   320,   173,    92,
      94,    91,    91,    88,    88,   198,    88,    89,   284,    88,
      88,    88,    88,    89,    88,    89,   442,    88,   467,   460,
     469,    88,    88,   500,   206,   241,   309,   351,    89,    93,
     411,   434,   201,   200,     3,   232,     3,   223,   134,   209,
      91,     3,   139,   475,    73,    74,    75,    76,    77,    78,
      79,    80,    84,    85,    99,   100,   104,   105,   198,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     494,    92,   161,   155,   166,     8,   211,   221,    91,    47,
     177,   317,   324,   328,   105,   233,   271,   105,   406,   443,
     177,    89,    89,   502,   202,   202,   248,   251,   255,   256,
     331,    89,    89,   170,   412,   410,    93,   439,   201,   482,
     498,   224,   111,   112,     3,    92,    94,   219,   219,   219,
     211,    96,    97,    98,    82,    83,    99,   100,   101,   102,
     103,   493,   151,   195,   198,   185,   180,    94,   321,   195,
     221,   221,   221,   201,   211,   234,   235,   237,   273,   275,
     276,   285,   413,   445,   461,   470,    30,    31,    60,    65,
      68,    69,   338,   339,   344,   422,   424,   425,   491,   501,
     503,   207,   332,   249,   310,   352,   185,   198,   177,   440,
     435,    89,   411,     7,    90,   204,   209,   225,   227,   228,
     265,   307,   135,    92,   140,   476,   106,   213,   214,   215,
     216,   216,   217,   217,   218,   218,   218,    94,   202,   196,
       1,    32,    33,   156,   186,   204,   238,   338,   349,   354,
     359,   400,   401,    44,    45,    46,   167,   181,   182,   186,
     361,   211,   231,   318,   325,   329,    95,   106,    94,   236,
     272,   274,   231,   231,    51,    52,    61,   338,   407,   414,
     422,   424,   427,   428,   429,   491,    44,    54,   186,   444,
     446,   449,   452,   185,   180,   340,   345,    19,   198,   423,
      58,   426,   198,   198,   506,   504,   505,   423,   507,    90,
      95,   107,   333,   335,    94,   231,   312,   315,   274,   171,
     198,   177,   483,   226,    88,   202,    88,    90,     3,    89,
     231,    94,   194,    90,   190,   187,   192,   191,   193,    34,
     198,   244,   323,   327,   360,   383,   188,   189,   362,   184,
     183,    90,    88,    92,   243,    92,    92,    92,   211,   201,
     237,    90,   275,     1,   242,   277,     8,   198,   257,   262,
     263,   264,   290,   307,   198,   198,   198,   420,    90,   415,
     416,   417,   418,   419,   421,   202,   202,    90,   447,   448,
     462,   471,    31,   383,   201,     3,     3,    88,    88,    88,
     201,    88,   208,   336,   334,   250,     3,   311,   313,   353,
      90,   436,   338,   422,   424,   485,   487,   240,    29,   229,
     141,   494,   197,    88,    88,    88,    88,    88,    88,   355,
      88,    88,   231,   277,   277,   236,   280,   278,   286,   201,
     201,   201,    88,    88,    88,    88,    88,    88,    88,   450,
     453,    88,    88,    90,    90,   341,   346,   210,   221,   335,
     251,    90,    94,    90,    66,   490,   488,   489,   484,   486,
      88,   211,    88,    72,   114,   130,   143,   145,   146,   198,
     231,     3,    88,   248,   106,   105,   365,   365,   383,   252,
     255,   337,   314,   437,    88,    88,    88,    90,   487,   198,
     142,   144,    88,     3,   363,   279,   287,   366,   367,   451,
     454,   342,   347,   253,   108,   315,   198,   147,    90,   145,
     356,   365,    88,    89,   106,    35,   368,   371,    38,   385,
     385,   252,    55,   388,    94,   438,    91,   105,   374,   364,
     288,   372,   106,   369,   386,   343,   389,   348,   254,    59,
     441,     3,   477,   479,   375,   376,   357,   385,     1,    28,
      29,   291,   293,   297,   299,   383,    94,   105,   385,   105,
      62,   391,   255,   198,    92,   478,   106,    35,    36,    37,
     377,   380,   384,   385,   296,   301,   300,   289,   292,   294,
     298,   373,   370,   387,   390,   392,   148,    94,   106,   378,
     381,   358,    88,   211,    93,    90,   293,   231,   299,   251,
     371,   195,   195,   105,   202,   479,    94,   383,    39,   394,
     302,   242,   303,   106,   106,   393,   379,   382,   395,    93,
     304,   295,   195,   380,   251,   105,   251,    88,   106,   396,
      75,   397,   398,   106,    94,   399,    75
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
     220,   220,   220,   221,   223,   222,   224,   224,   225,   225,
     225,   226,   225,   227,   227,   228,   229,   229,   230,   231,
     231,   232,   233,   233,   234,   234,   235,   236,   236,   237,
     239,   238,   238,   238,   238,   238,   238,   241,   240,   242,
     242,   243,   243,   243,   244,   244,   244,   244,   244,   244,
     244,   244,   245,   245,   245,   245,   246,   246,   246,   247,
     247,   248,   250,   249,   249,   251,   251,   252,   254,   253,
     253,   255,   256,   257,   257,   258,   258,   258,   259,   259,
     259,   260,   260,   260,   261,   262,   262,   263,   264,   265,
     266,   268,   267,   270,   271,   272,   269,   273,   274,   274,
     276,   275,   278,   279,   277,   280,   277,   282,   281,   284,
     285,   286,   287,   288,   289,   283,   290,   290,   290,   290,
     290,   290,   291,   292,   292,   294,   295,   293,   296,   293,
     297,   298,   298,   300,   299,   301,   302,   299,   304,   303,
     305,   306,   308,   309,   310,   311,   307,   312,   314,   313,
     313,   315,   317,   318,   316,   316,   320,   321,   319,   322,
     324,   325,   323,   323,   326,   328,   329,   327,   327,   330,
     332,   331,   333,   334,   334,   336,   337,   335,   338,   338,
     340,   341,   342,   343,   339,   345,   346,   347,   348,   344,
     350,   351,   352,   353,   349,   355,   356,   357,   358,   354,
     359,   359,   359,   360,   360,   362,   363,   364,   361,   366,
     365,   367,   365,   368,   370,   369,   369,   372,   373,   371,
     375,   374,   376,   374,   377,   379,   378,   378,   381,   382,
     380,   383,   383,   383,   383,   384,   384,   384,   386,   387,
     385,   385,   389,   390,   388,   388,   392,   393,   391,   391,
     395,   396,   394,   394,   397,   399,   398,   398,   400,   401,
     402,   402,   403,   405,   406,   407,   404,   409,   410,   408,
     412,   411,   411,   413,   413,   415,   414,   416,   414,   417,
     414,   418,   414,   419,   414,   420,   414,   421,   414,   422,
     423,   423,   424,   425,   426,   426,   427,   428,   429,   431,
     430,   433,   434,   435,   436,   437,   438,   432,   440,   439,
     439,   441,   441,   443,   444,   442,   445,   445,   446,   447,
     446,   448,   446,   450,   451,   449,   453,   454,   452,   455,
     455,   455,   456,   456,   457,   458,   460,   461,   462,   459,
     463,   464,   465,   467,   466,   469,   470,   471,   468,   472,
     472,   473,   473,   473,   473,   473,   473,   473,   473,   473,
     473,   474,   475,   475,   476,   476,   477,   478,   478,   479,
     481,   482,   483,   484,   480,   485,   486,   486,   488,   487,
     489,   487,   490,   487,   491,   491,   492,   493,   493,   494,
     495,   497,   498,   496,   500,   501,   499,   502,   502,   504,
     503,   505,   503,   506,   503,   507,   503
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
       1,     1,     1,     1,     0,     6,     2,     0,     1,     2,
       2,     0,     4,     1,     1,     4,     2,     0,     2,     2,
       0,     3,     3,     0,     1,     1,     2,     3,     0,     3,
       0,     3,     1,     1,     1,     2,     1,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     0,     4,     0,     1,     1,     2,     0,     4,
       0,     1,     1,     1,     1,     1,     2,     1,     2,     3,
       2,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     0,     0,     0,     7,     2,     2,     0,
       0,     3,     0,     0,     5,     0,     3,     0,     3,     0,
       0,     0,     0,     0,     0,    15,     1,     1,     1,     1,
       1,     1,     2,     2,     0,     0,     0,     6,     0,     3,
       2,     2,     0,     0,     3,     0,     0,     5,     0,     3,
       1,     1,     0,     0,     0,     0,     9,     2,     0,     4,
       0,     2,     0,     0,     6,     2,     0,     0,     6,     6,
       0,     0,     6,     1,     1,     0,     0,     6,     1,     1,
       0,     3,     2,     2,     0,     0,     0,     5,     1,     1,
       0,     0,     0,     0,     9,     0,     0,     0,     0,     9,
       0,     0,     0,     0,     9,     0,     0,     0,     0,    11,
       1,     1,     0,     1,     1,     0,     0,     0,     8,     0,
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     0,     2,     5,     2,     3,     0,     1,
       0,     0,     0,     0,    10,     2,     2,     0,     0,     3,
       0,     3,     0,     3,     3,     3,     3,     4,     0,     1,
       2,     0,     0,     6,     0,     0,     5,     2,     0,     0,
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
#line 398 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 2396 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 6:
#line 411 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 2403 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 7:
#line 417 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 2411 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 8:
#line 421 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = 0;
        }
#line 2419 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 9:
#line 425 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = 0;
        }
#line 2427 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 10:
#line 432 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AnnotationDeclSeen);
        }
#line 2435 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 11:
#line 436 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2444 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 12:
#line 441 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 2452 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 13:
#line 445 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 2461 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 14:
#line 450 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 2469 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 15:
#line 454 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2478 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 16:
#line 459 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 2486 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 17:
#line 463 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2495 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 18:
#line 468 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 2503 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 19:
#line 472 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 2512 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 20:
#line 477 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 2520 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 21:
#line 481 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2529 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 22:
#line 486 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
        }
#line 2537 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 23:
#line 490 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2546 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 24:
#line 495 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
        }
#line 2554 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 25:
#line 499 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 2563 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 26:
#line 504 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeDeclSeen);
        }
#line 2571 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 27:
#line 508 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2580 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 28:
#line 513 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentDeclSeen);
        }
#line 2588 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 29:
#line 517 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2597 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 30:
#line 522 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeDeclSeen);
        }
#line 2605 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 31:
#line 526 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2614 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 32:
#line 531 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventDeclSeen);
        }
#line 2622 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 33:
#line 535 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2631 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 34:
#line 540 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeDeclSeen);
        }
#line 2639 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 35:
#line 544 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2648 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 36:
#line 549 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorDeclSeen);
        }
#line 2656 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 37:
#line 553 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2665 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 38:
#line 558 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 2673 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 39:
#line 562 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
          (yyval.dcval) = 0;
        }
#line 2683 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 40:
#line 571 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
        }
#line 2691 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 41:
#line 575 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 2699 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 42:
#line 582 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 2742 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 43:
#line 621 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
        }
#line 2750 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 44:
#line 625 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
        }
#line 2758 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 45:
#line 629 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
          /*
           * Finished with this module - pop it from the scope stack.
           */

          idl_global->scopes ().pop ();
          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 2772 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 46:
#line 642 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleIDSeen);
        }
#line 2780 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 47:
#line 649 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 2798 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 48:
#line 663 "fe/idl.ypp" /* yacc.c:1651  */
    {
          if (FE_Utils::duplicate_param_id ((yyvsp[0].plval)))
            {
              idl_global->err ()->duplicate_param_id (
                (yyvsp[-2].idlist));

              return 1;
            }
        }
#line 2812 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 49:
#line 673 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 2846 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 50:
#line 703 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleSqSeen);
        }
#line 2854 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 51:
#line 707 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleBodySeen);
        }
#line 2862 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 52:
#line 711 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 2883 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 58:
#line 745 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefSeen);
        }
#line 2892 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 59:
#line 750 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefParamsSeen);
        }
#line 2901 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 60:
#line 755 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 2983 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 61:
#line 836 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleSeen);
        }
#line 2992 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 62:
#line 841 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleArgsSeen);
        }
#line 3001 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 63:
#line 846 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3067 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 66:
#line 916 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3109 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 67:
#line 954 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
#line 3117 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 68:
#line 958 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
#line 3125 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 69:
#line 962 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
#line 3138 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 70:
#line 974 "fe/idl.ypp" /* yacc.c:1651  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
#line 3146 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 71:
#line 978 "fe/idl.ypp" /* yacc.c:1651  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (yyval.idval) = (yyvsp[0].idval);
         }
#line 3155 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 72:
#line 986 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3195 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 73:
#line 1023 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3228 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 74:
#line 1053 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3261 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 75:
#line 1085 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 3269 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 76:
#line 1089 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyvsp[0].nlval)->truncatable ((yyvsp[-2].bval));
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3278 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 77:
#line 1094 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = 0;
        }
#line 3286 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 82:
#line 1108 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->err ()->unsupported_error ("custom is not supported");
        }
#line 3294 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 84:
#line 1116 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3341 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 85:
#line 1159 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3349 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 86:
#line 1163 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3357 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 87:
#line 1167 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3380 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 88:
#line 1190 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3427 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 89:
#line 1233 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3435 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 90:
#line 1237 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3443 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 91:
#line 1241 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 3456 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 92:
#line 1254 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 3464 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 93:
#line 1258 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3499 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 94:
#line 1292 "fe/idl.ypp" /* yacc.c:1651  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
#line 3507 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 95:
#line 1296 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 3516 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 96:
#line 1304 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.bval) = true;
        }
#line 3524 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 97:
#line 1308 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.bval) = false;
        }
#line 3532 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 98:
#line 1316 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3540 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 99:
#line 1320 "fe/idl.ypp" /* yacc.c:1651  */
    {
/*      |    EMPTY */
          (yyval.nlval) = 0;
        }
#line 3549 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 100:
#line 1329 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3576 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 101:
#line 1353 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3603 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 102:
#line 1379 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3668 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 108:
#line 1455 "fe/idl.ypp" /* yacc.c:1651  */
    {
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_PUBLIC;
        }
#line 3677 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 109:
#line 1460 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 3684 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 110:
#line 1463 "fe/idl.ypp" /* yacc.c:1651  */
    {
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_PRIVATE;
        }
#line 3693 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 111:
#line 1468 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 3700 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 114:
#line 1479 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 3708 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 115:
#line 1483 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3716 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 116:
#line 1487 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 3724 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 117:
#line 1491 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3732 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 118:
#line 1495 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 3740 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 119:
#line 1499 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3748 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 120:
#line 1503 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 3756 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 121:
#line 1507 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
#line 3764 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 122:
#line 1511 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 3772 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 123:
#line 1515 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3780 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 124:
#line 1519 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 3788 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 125:
#line 1523 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3796 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 126:
#line 1527 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
#line 3804 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 127:
#line 1531 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3812 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 128:
#line 1535 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 3820 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 129:
#line 1539 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 3829 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 130:
#line 1547 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.nlval),
                          UTL_NameList ((yyvsp[-1].idlist),
                                        (yyvsp[0].nlval)),
                          1);
        }
#line 3840 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 131:
#line 1558 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
#line 3848 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 132:
#line 1562 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3872 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 133:
#line 1582 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = 0;
        }
#line 3880 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 134:
#line 1589 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN ((yyval.idlist),
                          UTL_IdList ((yyvsp[0].idval),
                                      0),
                          1);
        }
#line 3893 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 135:
#line 1598 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
#line 3901 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 136:
#line 1602 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3925 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 137:
#line 1623 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);

          // This cleans up all the non-global "::"s in scoped names.
          // If there is a global one, it gets put into the UTL_IdList,
          // so we clean it up in the case above.
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3939 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 138:
#line 1633 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3955 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 139:
#line 1647 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.idval),
                          Identifier ((yyvsp[0].strval)),
                          1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3967 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 140:
#line 1657 "fe/idl.ypp" /* yacc.c:1651  */
    {
          /* defining_id is a defining identifier
             whereas id is usually a reference to a defining identifier */
          ACE_NEW_RETURN ((yyval.idval), Identifier ((yyvsp[0].strval)), 1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3979 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 141:
#line 1668 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4026 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 142:
#line 1712 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4056 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 143:
#line 1739 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4086 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 144:
#line 1768 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
#line 4094 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 145:
#line 1772 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
#line 4102 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 146:
#line 1776 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
#line 4110 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 147:
#line 1780 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
#line 4118 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 148:
#line 1784 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4178 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 155:
#line 1849 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_string;
        }
#line 4186 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 156:
#line 1853 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_wstring;
        }
#line 4194 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 157:
#line 1857 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4264 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 161:
#line 1930 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_or,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4277 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 163:
#line 1943 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_xor,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4290 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 165:
#line 1956 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_and,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4303 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 167:
#line 1969 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_left,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4316 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 168:
#line 1978 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_right,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4329 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 170:
#line 1991 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_add,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4342 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 171:
#line 2000 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_minus,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4355 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 173:
#line 2013 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mul,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4368 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 174:
#line 2022 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_div,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4381 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 175:
#line 2031 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mod,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4394 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 177:
#line 2044 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_u_plus,
                                    (yyvsp[0].exval),
                                    0
                                  );
        }
#line 4407 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 178:
#line 2053 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_u_minus,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4420 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 179:
#line 2062 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_bit_neg,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4433 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 180:
#line 2074 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4487 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 182:
#line 2125 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = (yyvsp[-1].exval);
        }
#line 4495 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 183:
#line 2132 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].ival));
        }
#line 4503 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 184:
#line 2136 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr ((yyvsp[0].uival));
        }
#line 4512 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 185:
#line 2141 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].sval));
          (yyvsp[0].sval)->destroy ();
          delete (yyvsp[0].sval);
          (yyvsp[0].sval) = 0;
        }
#line 4523 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 186:
#line 2148 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].wsval));
        }
#line 4531 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 187:
#line 2152 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].cval));
        }
#line 4539 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 188:
#line 2156 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_OutputCDR::from_wchar wc ((yyvsp[0].wcval));
          (yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
#line 4548 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 189:
#line 2161 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].fixval));
        }
#line 4556 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 190:
#line 2165 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].dval));
        }
#line 4564 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 191:
#line 2169 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr (true);
        }
#line 4572 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 192:
#line 2173 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr (false);
        }
#line 4580 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 193:
#line 2180 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4649 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 194:
#line 2248 "fe/idl.ypp" /* yacc.c:1651  */
    {
          if (idl_global->idl_version_ < IDL_VERSION_4)
            {
              idl_global->err ()->idl_version_error (
                "Annotations are an IDL4 feature");
            }

          Identifier *id = (yyvsp[-1].idval);
          UTL_ScopedName name (id, 0);
          AST_Annotation_Decl::escape_name (&name);
          AST_Annotation_Decl *annotation_decl = idl_global->gen ()->
            create_annotation_decl (&name);
          idl_global->scopes ().top_non_null ()->
            fe_add_annotation_decl (annotation_decl);
          idl_global->scopes ().push (annotation_decl);
        }
#line 4670 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 195:
#line 2265 "fe/idl.ypp" /* yacc.c:1651  */
    {
          Identifier *id = (yyvsp[-4].idval);
          idl_global->scopes ().pop ();
          id->destroy ();
          delete id;
        }
#line 4681 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 201:
#line 2283 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 4690 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 205:
#line 2297 "fe/idl.ypp" /* yacc.c:1651  */
    {
          UTL_Scope *scope = idl_global->scopes ().top_non_null ();
          UTL_Scope *root = idl_global->scopes ().bottom ();

          AST_Expression::ExprType type = (yyvsp[-3].etval);
          Identifier *name = (yyvsp[-2].idval);
          UTL_ScopedName scoped_name (name, 0);
          AST_Expression *default_value = (yyvsp[-1].exval);

          AST_Decl * type_obj;
          switch (type)
            {
            case AST_Expression::EV_string:
            case AST_Expression::EV_wstring:
              type_obj = tao_string_decl;
              break;
            case AST_Expression::EV_enum:
              type_obj = tao_enum_constant_decl;
              break;
            default:
              type_obj = root->lookup_primitive_type (type);
            }

          FE_Declarator *declarator = new FE_Declarator (
            &scoped_name, FE_Declarator::FD_simple, 0);
          AST_Annotation_Member *member = idl_global->gen ()->
            create_annotation_member (
              type, declarator->compose (type_obj), declarator->name ());
          scope->fe_add_annotation_member (member);

          if (default_value)
            {
              // Check if types are compatible, else report error
              AST_Expression::AST_ExprValue *result =
                default_value->check_and_coerce (type, tao_enum_constant_decl);
              tao_enum_constant_decl = 0;
              if (result)
                {
                  member->value (default_value);
                }
              else
                {
                  idl_global->err ()->coercion_error (default_value, type);
                }
              delete result;
            }
        }
#line 4742 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 206:
#line 2348 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = (yyvsp[0].exval);
        }
#line 4750 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 207:
#line 2352 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = 0;
        }
#line 4758 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 208:
#line 2359 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Annotation_Appl *annotation = (yyvsp[0].annotation_val);
          if (annotation)
            {
              annotations->push_back (annotation);
            }
          (yyval.annotations_val) = annotations;
        }
#line 4772 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 209:
#line 2372 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Annotation_Appl *annotation = (yyvsp[0].annotation_val);
          if (annotation)
            {
              annotations->push_back (annotation);
            }
          (yyval.annotations_val) = annotations;
        }
#line 4786 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 210:
#line 2382 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.annotations_val) = new AST_Annotation_Appls ();
        }
#line 4794 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 211:
#line 2389 "fe/idl.ypp" /* yacc.c:1651  */
    {
          if (idl_global->idl_version_ < IDL_VERSION_4)
            {
              idl_global->err ()->idl_version_error (
                "Annotations are an IDL4 feature");
            }

          AST_Annotation_Appl *appl = 0;
          UTL_ScopedName *name = (yyvsp[-1].idlist);
          AST_Annotation_Decl::escape_name (name);
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);

          // Check for Annotation Declaration
          AST_Decl *d = idl_global->scopes ().top_non_null ()->
            lookup_by_name (name);
          if (!d /*&& !idl_global->ignore_missing_annotations_*/)
            {
              idl_global->err ()->lookup_error (name);
            }
          else
            {
              AST_Annotation_Decl *decl = AST_Annotation_Decl::narrow_from_decl (d);
              appl = idl_global->gen ()->create_annotation_appl (name, params);
              appl->apply_from (decl);
            }

          (yyval.annotation_val) = appl;
        }
#line 4827 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 212:
#line 2421 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.annotation_params_val) = (yyvsp[-1].annotation_params_val);
        }
#line 4835 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 213:
#line 2425 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.annotation_params_val) = 0;
        }
#line 4843 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 214:
#line 2432 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appl::Params *params = new AST_Annotation_Appl::Params;
          AST_Annotation_Appl::Param *param = new AST_Annotation_Appl::Param;
          param->id = 0;
          param->expr = (yyvsp[0].exval);
          params->push (param);
          (yyval.annotation_params_val) = params;
        }
#line 4856 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 215:
#line 2441 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.annotation_params_val) = (yyvsp[0].annotation_params_val);
        }
#line 4864 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 216:
#line 2448 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 4874 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 217:
#line 2456 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 4884 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 218:
#line 2462 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.annotation_params_val) = new AST_Annotation_Appl::Params;
        }
#line 4892 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 219:
#line 2469 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appl::Param *param = new AST_Annotation_Appl::Param;
          param->id = (yyvsp[-2].idval);
          param->expr = (yyvsp[0].exval);
          (yyval.annotation_param_val) = param;
        }
#line 4903 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 220:
#line 2479 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 4912 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 221:
#line 2484 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 4920 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 222:
#line 2488 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 4928 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 223:
#line 2492 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 4936 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 224:
#line 2496 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 4944 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 225:
#line 2500 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4974 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 226:
#line 2526 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = 0;
        }
#line 4982 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 227:
#line 2533 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
#line 4990 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 228:
#line 2537 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5059 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 231:
#line 2610 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5070 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 233:
#line 2618 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5097 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 251:
#line 2673 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5108 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 252:
#line 2684 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5116 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 253:
#line 2688 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5140 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 254:
#line 2708 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dlval) = 0;
        }
#line 5148 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 257:
#line 2720 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5159 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 258:
#line 2731 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5167 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 259:
#line 2735 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5191 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 260:
#line 2755 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dlval) = 0;
        }
#line 5199 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 261:
#line 2762 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5216 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 262:
#line 2778 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5235 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 265:
#line 2801 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_long;
        }
#line 5243 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 266:
#line 2805 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_longlong;
        }
#line 5251 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 267:
#line 2809 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_short;
        }
#line 5259 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 268:
#line 2816 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_ulong;
        }
#line 5267 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 269:
#line 2820 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_ulonglong;
        }
#line 5275 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 270:
#line 2824 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_ushort;
        }
#line 5283 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 271:
#line 2831 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_double;
        }
#line 5291 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 272:
#line 2835 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_float;
        }
#line 5299 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 273:
#line 2839 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_longdouble;
        }
#line 5307 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 274:
#line 2846 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_fixed;
        }
#line 5315 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 275:
#line 2853 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_char;
        }
#line 5323 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 276:
#line 2857 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_wchar;
        }
#line 5331 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 277:
#line 2864 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_octet;
        }
#line 5339 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 278:
#line 2871 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_bool;
        }
#line 5347 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 279:
#line 2878 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_any;
        }
#line 5355 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 280:
#line 2885 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_object;
        }
#line 5363 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 281:
#line 2892 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
#line 5371 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 282:
#line 2896 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5380 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 283:
#line 2905 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5415 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 284:
#line 2936 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
#line 5423 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 285:
#line 2940 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
#line 5431 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 286:
#line 2944 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5447 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 290:
#line 2965 "fe/idl.ypp" /* yacc.c:1651  */
    {
          /* is $0 to member_i */
          (yyval.vival) = AST_Field::vis_NA;
        }
#line 5456 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 291:
#line 2970 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5471 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 292:
#line 2984 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
#line 5479 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 293:
#line 2988 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
#line 5487 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 294:
#line 2992 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5551 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 295:
#line 3052 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->err()->syntax_error (idl_global->parse_state ());
        }
#line 5559 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 296:
#line 3056 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 5568 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 297:
#line 3064 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
#line 5576 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 298:
#line 3068 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5585 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 299:
#line 3076 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
#line 5593 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 300:
#line 3080 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5630 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 301:
#line 3113 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
#line 5638 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 302:
#line 3117 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5698 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 303:
#line 3173 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
#line 5706 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 304:
#line 3177 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
#line 5714 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 305:
#line 3181 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5734 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 306:
#line 3200 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5745 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 307:
#line 3207 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5762 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 308:
#line 3220 "fe/idl.ypp" /* yacc.c:1651  */
    {
          /* octets are not allowed. */
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5775 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 309:
#line 3229 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5786 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 311:
#line 3237 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5894 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 315:
#line 3351 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
#line 5902 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 316:
#line 3355 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
#line 5910 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 317:
#line 3359 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5943 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 318:
#line 3388 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 5951 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 319:
#line 3392 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 5960 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 320:
#line 3400 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.llval),
                          UTL_LabelList ((yyvsp[-1].ulval),
                                         (yyvsp[0].llval)),
                          1);
        }
#line 5971 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 321:
#line 3410 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5993 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 322:
#line 3428 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.llval) = 0;
        }
#line 6001 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 323:
#line 3435 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
#line 6009 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 324:
#line 3439 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
#line 6022 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 325:
#line 3448 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
#line 6030 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 326:
#line 3452 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
#line 6038 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 327:
#line 3456 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      (yyvsp[-2].exval)
                                    );
        }
#line 6051 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 328:
#line 3468 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
#line 6059 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 329:
#line 3472 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6107 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 330:
#line 3519 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6133 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 331:
#line 3544 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6157 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 332:
#line 3567 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
#line 6165 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 333:
#line 3571 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6200 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 334:
#line 3602 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
#line 6208 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 335:
#line 3606 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
#line 6216 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 336:
#line 3610 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6239 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 338:
#line 3635 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
#line 6247 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 341:
#line 3644 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6282 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 342:
#line 3679 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
#line 6290 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 343:
#line 3683 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
#line 6298 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 344:
#line 3687 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6386 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 345:
#line 3772 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6455 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 346:
#line 3840 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
#line 6468 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 347:
#line 3849 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
#line 6476 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 348:
#line 3853 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          Decl_Annotations_Pair *seq_head = new Decl_Annotations_Pair;
          seq_head->decl = (yyvsp[0].dcval);
          seq_head->annotations = (yyvsp[-1].annotations_val);
          (yyval.decl_annotations_pair_val) = seq_head;
        }
#line 6488 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 349:
#line 3864 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyvsp[-1].exval)->evaluate (AST_Expression::EK_positive_int);
          (yyval.dcval) = idl_global->gen ()->create_fixed ((yyvsp[-3].exval), (yyvsp[-1].exval));
        }
#line 6497 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 350:
#line 3873 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 6505 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 351:
#line 3877 "fe/idl.ypp" /* yacc.c:1651  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 6513 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 352:
#line 3881 "fe/idl.ypp" /* yacc.c:1651  */
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
              tao_string_decl = idl_global->gen ()->create_string ((yyvsp[-2].exval));

              /*
               * Add this AST_String to the types defined in the global scope.
               */
              idl_global->root ()->fe_add_string (
                AST_String::narrow_from_decl (
                  tao_string_decl));

              if (!idl_global->in_typedef () && !idl_global->anon_silent ())
                {
                  idl_global->err ()->anonymous_type_diagnostic ();
                }

              (yyval.dcval) = tao_string_decl;
            }

          delete ev;
          ev = 0;
        }
#line 6559 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 353:
#line 3923 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringCompleted);

          /*
           * Create a node representing a string.
           */
          ACE_CDR::ULong bound = 0UL;
          tao_string_decl =
            idl_global->gen ()->create_string (
                idl_global->gen ()->create_expr (bound,
                                                 AST_Expression::EV_ulong)
              );

          /*
           * Add this AST_String to the types defined in the global scope.
           */
          idl_global->root ()->fe_add_string (
            AST_String::narrow_from_decl (
              tao_string_decl));
        }
#line 6584 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 354:
#line 3947 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 6592 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 355:
#line 3955 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 6600 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 356:
#line 3959 "fe/idl.ypp" /* yacc.c:1651  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 6608 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 357:
#line 3963 "fe/idl.ypp" /* yacc.c:1651  */
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
              AST_String *string = idl_global->gen ()->create_wstring ((yyvsp[-2].exval));

              /*
               * Add this AST_String to the types defined in the global scope.
               */
              idl_global->root ()->fe_add_string (
                AST_String::narrow_from_decl (
                  string));

              if (!idl_global->in_typedef () && !idl_global->anon_silent ())
                {
                  idl_global->err ()->anonymous_type_diagnostic ();
                }

              (yyval.dcval) = string;
            }
        }
#line 6645 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 358:
#line 3996 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringCompleted);

          /*
           * Create a node representing a wstring.
           */
          ACE_CDR::ULong bound = 0UL;
          AST_String *string =
            idl_global->gen ()->create_wstring (
                idl_global->gen ()->create_expr (bound,
                                                 AST_Expression::EV_ulong)
              );

          /*
           * Add this AST_String to the types defined in the global scope.
           */
          idl_global->root ()->fe_add_string (
            AST_String::narrow_from_decl (
              string));
        }
#line 6670 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 359:
#line 4020 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 6678 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 360:
#line 4027 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
#line 6686 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 361:
#line 4031 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6723 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 362:
#line 4067 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.elval),
                          UTL_ExprList ((yyvsp[-1].exval),
                                        (yyvsp[0].elval)),
                          1);
        }
#line 6734 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 363:
#line 4077 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6756 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 364:
#line 4095 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.elval) = 0;
        }
#line 6764 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 365:
#line 4102 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
#line 6772 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 366:
#line 4106 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
#line 6780 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 367:
#line 4110 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6838 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 370:
#line 4172 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
#line 6846 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 371:
#line 4176 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 6854 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 372:
#line 4180 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 6862 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 373:
#line 4184 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 6870 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 374:
#line 4188 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6934 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 375:
#line 4251 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 6942 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 376:
#line 4255 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 6950 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 377:
#line 4259 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 6958 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 378:
#line 4263 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
#line 6966 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 379:
#line 4267 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7039 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 380:
#line 4339 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
#line 7047 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 381:
#line 4343 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7080 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 382:
#line 4372 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
#line 7088 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 383:
#line 4376 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
#line 7096 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 384:
#line 4380 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();
        }
#line 7108 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 385:
#line 4392 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7116 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 386:
#line 4396 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7184 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 387:
#line 4460 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7192 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 388:
#line 4464 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
#line 7200 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 389:
#line 4468 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7233 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 390:
#line 4500 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_oneway;
        }
#line 7242 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 391:
#line 4505 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_idempotent;
        }
#line 7251 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 392:
#line 4510 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ofval) = AST_Operation::OP_noflags;
        }
#line 7259 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 394:
#line 4518 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
#line 7270 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 395:
#line 4528 "fe/idl.ypp" /* yacc.c:1651  */
    {
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7279 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 396:
#line 4533 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7324 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 397:
#line 4574 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7332 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 398:
#line 4578 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7349 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 399:
#line 4594 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7357 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 400:
#line 4598 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7365 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 401:
#line 4602 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7373 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 402:
#line 4607 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7381 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 404:
#line 4617 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7389 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 407:
#line 4626 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7397 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 408:
#line 4630 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7405 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 409:
#line 4634 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7441 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 410:
#line 4669 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7449 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 411:
#line 4673 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7457 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 412:
#line 4677 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7465 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 413:
#line 4682 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7473 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 415:
#line 4692 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7481 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 418:
#line 4701 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7489 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 419:
#line 4705 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7497 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 420:
#line 4709 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7540 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 421:
#line 4751 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 7551 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 424:
#line 4760 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7659 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 425:
#line 4867 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dival) = AST_Argument::dir_IN;
        }
#line 7667 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 426:
#line 4871 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dival) = AST_Argument::dir_OUT;
        }
#line 7675 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 427:
#line 4875 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dival) = AST_Argument::dir_INOUT;
        }
#line 7683 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 428:
#line 4882 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
#line 7691 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 429:
#line 4886 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
#line 7699 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 430:
#line 4891 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7708 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 431:
#line 4896 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = 0;
        }
#line 7716 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 432:
#line 4903 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
#line 7724 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 433:
#line 4907 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
#line 7732 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 434:
#line 4912 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7741 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 435:
#line 4917 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = 0;
        }
#line 7749 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 436:
#line 4924 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
#line 7757 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 437:
#line 4928 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
#line 7765 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 438:
#line 4933 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7774 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 439:
#line 4938 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = 0;
        }
#line 7782 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 440:
#line 4945 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
#line 7790 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 441:
#line 4949 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
        }
#line 7798 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 442:
#line 4954 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (yyval.slval) = (yyvsp[-1].slval);
        }
#line 7807 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 443:
#line 4959 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.slval) = 0;
        }
#line 7815 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 444:
#line 4966 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.slval),
                          UTL_StrList ((yyvsp[-1].sval),
                                       (yyvsp[0].slval)),
                          1);
        }
#line 7826 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 445:
#line 4977 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
#line 7834 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 446:
#line 4981 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7856 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 447:
#line 4999 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.slval) = 0;
        }
#line 7864 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 448:
#line 5006 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7893 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 449:
#line 5034 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7931 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 452:
#line 5077 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7959 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 453:
#line 5104 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8002 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 454:
#line 5143 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
#line 8010 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 455:
#line 5147 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
#line 8018 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 456:
#line 5151 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 8031 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 457:
#line 5164 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
#line 8039 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 458:
#line 5168 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 8047 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 459:
#line 5172 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8085 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 460:
#line 5209 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 8093 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 461:
#line 5213 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8101 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 462:
#line 5217 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = 0;
        }
#line 8109 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 465:
#line 5229 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 8117 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 466:
#line 5233 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8125 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 467:
#line 5237 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 8133 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 468:
#line 5241 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8141 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 469:
#line 5245 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
        }
#line 8149 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 470:
#line 5249 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8157 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 471:
#line 5253 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
        }
#line 8165 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 472:
#line 5257 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8173 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 473:
#line 5261 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
        }
#line 8181 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 474:
#line 5265 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8189 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 475:
#line 5269 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 8197 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 476:
#line 5273 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8205 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 477:
#line 5277 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 8213 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 478:
#line 5281 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8221 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 479:
#line 5287 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8312 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 480:
#line 5377 "fe/idl.ypp" /* yacc.c:1651  */
    {
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8322 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 481:
#line 5383 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8349 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 482:
#line 5408 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8456 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 483:
#line 5514 "fe/idl.ypp" /* yacc.c:1651  */
    {
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          (yyval.bval) = (yyvsp[0].bval);
        }
#line 8466 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 484:
#line 5523 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.bval) = true;
        }
#line 8474 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 485:
#line 5527 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.bval) = false;
        }
#line 8482 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 486:
#line 5533 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8556 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 487:
#line 5605 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8630 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 488:
#line 5677 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8704 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 489:
#line 5750 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8745 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 490:
#line 5787 "fe/idl.ypp" /* yacc.c:1651  */
    {
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 8756 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 491:
#line 5797 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
#line 8764 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 492:
#line 5801 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
#line 8772 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 493:
#line 5805 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 8780 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 494:
#line 5809 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
#line 8788 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 495:
#line 5813 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
#line 8796 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 496:
#line 5817 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
#line 8804 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 497:
#line 5821 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8854 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 498:
#line 5870 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 8862 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 499:
#line 5874 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8870 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 500:
#line 5878 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = 0;
        }
#line 8878 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 501:
#line 5886 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8886 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 502:
#line 5890 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = 0;
        }
#line 8894 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 503:
#line 5897 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
#line 8902 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 504:
#line 5901 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
#line 8910 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 505:
#line 5905 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
#line 8918 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 509:
#line 5918 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
#line 8926 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 510:
#line 5922 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8934 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 511:
#line 5926 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
#line 8942 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 512:
#line 5930 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8950 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 513:
#line 5938 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8977 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 514:
#line 5961 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 8985 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 515:
#line 5965 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9007 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 516:
#line 5987 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9038 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 517:
#line 6014 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 9046 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 518:
#line 6018 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9068 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 524:
#line 6051 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9095 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 525:
#line 6079 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9122 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 526:
#line 6106 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9170 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 527:
#line 6150 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9178 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 528:
#line 6154 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9186 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 529:
#line 6158 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9199 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 530:
#line 6172 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9207 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 531:
#line 6181 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9224 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 532:
#line 6198 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9234 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 533:
#line 6207 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 9242 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 534:
#line 6211 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9275 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 535:
#line 6244 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9328 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 536:
#line 6293 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9336 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 537:
#line 6297 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9344 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 538:
#line 6301 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9357 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 539:
#line 6313 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9365 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 540:
#line 6317 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9373 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 541:
#line 6324 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_type;
        }
#line 9381 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 542:
#line 6328 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_struct;
        }
#line 9389 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 543:
#line 6332 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_union;
        }
#line 9397 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 544:
#line 6336 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_eventtype;
        }
#line 9405 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 545:
#line 6340 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_sequence;
        }
#line 9413 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 546:
#line 6344 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_interface;
        }
#line 9421 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 547:
#line 6348 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_valuetype;
        }
#line 9429 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 548:
#line 6352 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_enum;
        }
#line 9437 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 549:
#line 6356 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_except;
        }
#line 9445 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 550:
#line 6360 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_const;
          t_param_const_type = (yyvsp[0].etval);
        }
#line 9454 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 551:
#line 6368 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9486 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 552:
#line 6399 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9503 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 553:
#line 6412 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.plval) = 0;
        }
#line 9511 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 554:
#line 6419 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9540 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 555:
#line 6444 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9559 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 556:
#line 6462 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.slval), UTL_StrList ((yyvsp[-1].sval), (yyvsp[0].slval)), 1);
        }
#line 9567 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 557:
#line 6469 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9586 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 558:
#line 6484 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.slval) = 0;
        }
#line 9594 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 559:
#line 6491 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.sval),
                          UTL_String ((yyvsp[0].strval), true),
                          1);
        }
#line 9604 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 560:
#line 6500 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSeen);
        }
#line 9612 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 561:
#line 6504 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9634 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 562:
#line 6522 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSqSeen);
        }
#line 9642 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 563:
#line 6526 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeBodySeen);
        }
#line 9650 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 564:
#line 6530 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeQsSeen);

          // Done with this port type - pop it off the scopes stack.
          idl_global->scopes ().pop ();
        }
#line 9661 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 565:
#line 6540 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9668 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 566:
#line 6546 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9675 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 567:
#line 6549 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9682 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 568:
#line 6555 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9689 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 569:
#line 6558 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9696 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 570:
#line 6561 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9703 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 571:
#line 6564 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9710 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 572:
#line 6567 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 9718 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 573:
#line 6571 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9725 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 574:
#line 6577 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9798 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 575:
#line 6646 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9849 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 576:
#line 6696 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9865 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 577:
#line 6711 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9881 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 578:
#line 6723 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.alval) = 0;
        }
#line 9889 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 579:
#line 6730 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9949 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 581:
#line 6793 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSeen);
        }
#line 9957 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 582:
#line 6797 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorIDSeen);
        }
#line 9965 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 583:
#line 6801 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 10021 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 584:
#line 6856 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSqSeen);
        }
#line 10029 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 585:
#line 6860 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorBodySeen);
        }
#line 10037 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 586:
#line 6864 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorQsSeen);

          // Done with this connector - pop it off the scope stack.
          idl_global->scopes ().pop ();
        }
#line 10048 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 589:
#line 6879 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 10056 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 590:
#line 6883 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10064 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 591:
#line 6887 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 10072 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 592:
#line 6891 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10080 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 593:
#line 6895 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 10088 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 594:
#line 6899 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10096 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 595:
#line 6903 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 10104 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 596:
#line 6907 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10112 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;


#line 10116 "fe/idl.tab.cpp" /* yacc.c:1651  */
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
#line 6912 "fe/idl.ypp" /* yacc.c:1910  */

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
