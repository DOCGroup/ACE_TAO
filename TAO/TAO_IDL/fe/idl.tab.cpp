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
  AST_Annotation_Decl *annotation_decl_val;
  AST_Decls *decls_val;
  Decl_Annotations_Pair *decl_annotations_pair_val;

#line 329 "fe/idl.tab.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE tao_yylval;

int tao_yyparse (void);

#endif /* !YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 346 "fe/idl.tab.cpp" /* yacc.c:358  */

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
#define YYLAST   1537

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  109
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  398
/* YYNRULES -- Number of rules.  */
#define YYNRULES  595
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  876

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
       0,   387,   387,   390,   391,   399,   414,   420,   424,   428,
     436,   435,   445,   444,   454,   453,   463,   462,   472,   471,
     481,   480,   490,   489,   499,   498,   508,   507,   517,   516,
     526,   525,   535,   534,   544,   543,   553,   552,   562,   561,
     575,   574,   586,   625,   629,   585,   645,   653,   667,   677,
     707,   711,   652,   734,   738,   739,   743,   744,   749,   754,
     748,   840,   845,   839,   914,   915,   920,   958,   962,   919,
     978,   977,   989,  1026,  1056,  1089,  1088,  1097,  1104,  1105,
    1106,  1107,  1111,  1115,  1120,  1163,  1167,  1119,  1194,  1237,
    1241,  1192,  1258,  1256,  1296,  1295,  1307,  1311,  1318,  1323,
    1331,  1356,  1382,  1446,  1447,  1451,  1452,  1453,  1458,  1462,
    1469,  1488,  1489,  1494,  1493,  1502,  1501,  1510,  1509,  1518,
    1517,  1526,  1525,  1534,  1533,  1542,  1541,  1550,  1549,  1561,
    1573,  1571,  1596,  1603,  1613,  1612,  1638,  1636,  1661,  1671,
    1682,  1726,  1753,  1783,  1787,  1791,  1795,  1782,  1857,  1858,
    1859,  1860,  1861,  1862,  1863,  1867,  1871,  1939,  1941,  1943,
    1944,  1956,  1957,  1969,  1970,  1982,  1983,  1992,  2004,  2005,
    2014,  2026,  2027,  2036,  2045,  2057,  2058,  2067,  2076,  2088,
    2146,  2147,  2154,  2158,  2163,  2170,  2174,  2178,  2183,  2187,
    2191,  2195,  2202,  2271,  2270,  2297,  2298,  2302,  2303,  2304,
    2306,  2305,  2314,  2315,  2319,  2375,  2379,  2386,  2399,  2409,
    2417,  2416,  2499,  2503,  2510,  2519,  2526,  2534,  2540,  2547,
    2560,  2559,  2568,  2572,  2576,  2580,  2606,  2614,  2613,  2685,
    2686,  2690,  2697,  2698,  2724,  2725,  2726,  2727,  2728,  2729,
    2730,  2731,  2735,  2736,  2737,  2738,  2742,  2743,  2744,  2748,
    2749,  2753,  2765,  2763,  2788,  2795,  2796,  2800,  2812,  2810,
    2835,  2842,  2858,  2876,  2877,  2881,  2885,  2889,  2896,  2900,
    2904,  2911,  2915,  2919,  2926,  2933,  2937,  2944,  2951,  2958,
    2965,  2973,  2972,  2986,  3017,  3021,  2985,  3038,  3041,  3042,
    3046,  3064,  3068,  3063,  3126,  3125,  3138,  3137,  3150,  3154,
    3187,  3191,  3250,  3254,  3149,  3276,  3283,  3296,  3305,  3312,
    3313,  3422,  3425,  3426,  3431,  3435,  3430,  3471,  3470,  3482,
    3492,  3510,  3518,  3517,  3531,  3535,  3530,  3551,  3550,  3601,
    3626,  3650,  3654,  3685,  3689,  3649,  3713,  3718,  3716,  3722,
    3726,  3766,  3770,  3764,  3858,  3929,  3938,  3928,  3952,  3962,
    3966,  3960,  4011,  4037,  4046,  4050,  4044,  4086,  4112,  4120,
    4119,  4166,  4176,  4194,  4202,  4206,  4201,  4266,  4267,  4272,
    4276,  4280,  4284,  4271,  4351,  4355,  4359,  4363,  4350,  4439,
    4443,  4472,  4476,  4438,  4492,  4496,  4562,  4566,  4490,  4601,
    4606,  4611,  4618,  4619,  4630,  4635,  4678,  4629,  4698,  4697,
    4706,  4705,  4716,  4721,  4719,  4725,  4730,  4734,  4729,  4773,
    4772,  4781,  4780,  4791,  4796,  4794,  4800,  4805,  4809,  4804,
    4854,  4861,  4862,  4863,  4970,  4974,  4978,  4986,  4990,  4985,
    4999,  5007,  5011,  5006,  5020,  5028,  5032,  5027,  5041,  5049,
    5053,  5048,  5062,  5069,  5081,  5079,  5102,  5109,  5137,  5174,
    5175,  5179,  5208,  5247,  5251,  5207,  5268,  5272,  5266,  5313,
    5312,  5320,  5327,  5328,  5333,  5332,  5341,  5340,  5349,  5348,
    5357,  5356,  5365,  5364,  5373,  5372,  5381,  5380,  5390,  5480,
    5486,  5511,  5617,  5626,  5630,  5636,  5708,  5780,  5854,  5853,
    5901,  5905,  5909,  5913,  5917,  5921,  5900,  5974,  5973,  5981,
    5988,  5993,  6001,  6005,  6000,  6015,  6016,  6020,  6022,  6021,
    6030,  6029,  6042,  6065,  6040,  6091,  6118,  6089,  6142,  6143,
    6144,  6148,  6149,  6153,  6180,  6210,  6254,  6258,  6208,  6273,
    6282,  6300,  6311,  6310,  6348,  6397,  6401,  6346,  6416,  6420,
    6427,  6431,  6435,  6439,  6443,  6447,  6451,  6455,  6459,  6463,
    6471,  6502,  6515,  6522,  6547,  6565,  6572,  6587,  6594,  6604,
    6608,  6628,  6632,  6603,  6645,  6651,  6654,  6661,  6660,  6667,
    6666,  6673,  6672,  6682,  6751,  6801,  6817,  6830,  6837,  6896,
    6901,  6905,  6900,  6966,  6970,  6965,  6983,  6984,  6989,  6988,
    6997,  6996,  7005,  7004,  7013,  7012
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
  "value_box_decl", "value_elements", "value_element", "visibility",
  "state_member", "exports", "export", "$@42", "$@43", "$@44", "$@45",
  "$@46", "$@47", "$@48", "$@49", "at_least_one_scoped_name",
  "scoped_names", "$@50", "scoped_name", "$@51", "$@52", "id",
  "defining_id", "interface_forward", "const_dcl", "$@53", "$@54", "$@55",
  "$@56", "const_type", "expression", "const_expr", "or_expr", "xor_expr",
  "and_expr", "shift_expr", "add_expr", "mult_expr", "unary_expr",
  "primary_expr", "literal", "positive_int_expr", "annotation_dcl", "$@57",
  "annotation_body", "annotation_statement", "$@58",
  "annotation_member_type", "annotation_member",
  "annotation_member_default", "at_least_one_annotation",
  "annotations_maybe", "annotation_appl", "@59",
  "annotation_appl_params_maybe", "annotation_appl_params",
  "named_annotation_appl_params", "more_named_annotation_appl_params",
  "named_annotation_appl_param", "type_dcl", "$@60", "type_declarator",
  "$@61", "type_spec", "simple_type_spec", "base_type_spec",
  "template_type_spec", "constructed_type_spec",
  "constructed_forward_type_spec", "at_least_one_declarator",
  "declarators", "$@62", "declarator", "at_least_one_simple_declarator",
  "simple_declarators", "$@63", "simple_declarator", "complex_declarator",
  "integer_type", "signed_int", "unsigned_int", "floating_pt_type",
  "fixed_type", "char_type", "octet_type", "boolean_type", "any_type",
  "object_type", "struct_decl", "$@64", "struct_type", "$@65", "$@66",
  "$@67", "at_least_one_member", "members", "member", "member_i", "$@68",
  "$@69", "$@70", "union_decl", "$@71", "union_type", "$@72", "$@73",
  "$@74", "$@75", "$@76", "$@77", "switch_type_spec",
  "at_least_one_case_branch", "case_branches", "case_branch", "$@78",
  "$@79", "$@80", "at_least_one_case_label", "case_labels", "case_label",
  "$@81", "$@82", "$@83", "element_spec", "$@84", "struct_forward_type",
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

#define YYPACT_NINF -611

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-611)))

#define YYTABLE_NINF -537

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -611,    63,   739,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,    41,    66,    98,   107,  -611,    41,
      41,  -611,    38,    38,  -611,  -611,    41,  -611,  -611,     7,
    -611,   674,    29,    56,  -611,  -611,    70,  -611,  -611,  -611,
    -611,  -611,  -611,   270,  -611,  -611,  -611,  -611,  -611,  1312,
      93,  -611,  -611,    69,  -611,   128,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,   110,  -611,  -611,  -611,   110,  -611,
    -611,   124,   132,   586,    38,    41,  1421,    41,    41,    41,
      41,  -611,  -611,  -611,    71,    41,   112,  -611,   118,    41,
    -611,   110,    41,   135,   146,    41,  -611,  -611,    16,  -611,
      21,  -611,  -611,   149,  -611,   151,   168,   705,  -611,  -611,
    -611,   181,   228,  -611,   188,   204,   217,   166,  -611,   159,
    -611,  -611,  -611,  -611,  -611,  -611,   216,  -611,  -611,  -611,
    -611,  -611,  -611,   227,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,   128,  -611,  -611,  -611,    15,  -611,  -611,   218,  -611,
     220,   225,   226,  -611,    38,   229,   231,   233,  -611,   235,
     236,   237,   239,   241,   240,   246,   243,  -611,  -611,  -611,
     248,   249,  -611,  -611,  -611,  -611,   227,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,   227,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,   250,  -611,   251,  -611,  -611,
     253,  -611,   340,  -611,  -611,  -611,    34,    44,  -611,  -611,
    -611,   586,  -611,  -611,  -611,  -611,   254,  -611,  -611,  -611,
    -611,   341,  -611,  -611,    49,   255,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,   344,  -611,   117,   258,   306,  -611,
    -611,  -611,  -611,  -611,  -611,   227,  -611,  -611,   257,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,   306,   265,
     266,  -611,  -611,  -611,    41,    41,   267,   268,  -611,  -611,
    -611,   272,  -611,   340,  -611,  -611,  -611,  -611,  -611,  -611,
     357,  -611,   276,   275,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,   152,   152,   152,   117,   227,  -611,
    -611,   274,   277,   279,   104,   145,   140,  -611,  -611,  -611,
    -611,  -611,    38,  -611,  -611,  -611,  -611,   281,  -611,    38,
    -611,   117,   117,   117,   273,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,   139,  -611,     1,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,    38,   306,  -611,  -611,  -611,   290,   253,
    1158,  1181,   294,   288,  -611,   705,  -611,  -611,  -611,   280,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
     295,    41,  -611,   227,   910,   523,   117,  -611,  -611,  -611,
    -611,  -611,   117,  -611,  1261,  -611,  -611,  -611,   428,   622,
    -611,  -611,  -611,  -611,    62,   332,    38,    38,  -611,  -611,
    -611,  -611,  -611,    62,  -611,   309,  -611,   307,  -611,   310,
    -611,  -611,   945,   227,  -611,    38,   306,  -611,  -611,  -611,
    -611,   315,  -611,  -611,    41,  -611,  -611,   318,   319,   405,
     323,  -611,  -611,   277,   279,   104,   145,   145,   140,   140,
    -611,  -611,  -611,  -611,  -611,   324,  -611,  -611,  -611,   327,
    -611,  -611,  -611,  -611,  -611,  -611,   866,  -611,  -611,  -611,
    -611,  -611,   329,  -611,  1402,  -611,  -611,   333,   328,  1003,
     331,   334,   335,   330,  -611,   325,  -611,   338,  -611,  -611,
    -611,   339,   345,   252,    38,    38,    38,  -611,   346,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,    41,    41,  -611,   347,
    -611,  -611,  -611,  1029,   826,   393,  1456,  -611,   227,   340,
    -611,  -611,    57,    64,   350,   351,   352,   340,   353,  -611,
    -611,     3,  -611,    48,  -611,  -611,   359,   360,   227,  -611,
     154,  1421,  -611,   413,  -611,  -611,  -611,  -611,    49,  -611,
     355,  -611,   367,   369,   376,   378,   380,  -611,   227,  -611,
    -611,  -611,  -611,  -611,   381,   382,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,   117,  -611,   340,  -611,   383,
      41,  -611,  -611,   464,   227,  -611,  -611,  -611,  -611,  -611,
    -611,    65,    65,    65,   385,  -611,   386,   387,   388,   389,
     390,   397,  -611,  -611,  -611,   398,   399,   400,   404,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,   117,
    -611,  -611,  -611,    41,  -611,   408,   401,   409,  -611,   434,
    -611,  -611,  -611,  -611,  -611,   414,   117,   415,  1287,  -611,
      38,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
      50,  -611,   330,   338,  -611,  -611,   395,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,   403,   403,  -611,
    -611,  -611,  -611,  1456,    41,  -611,   117,   402,  -611,  -611,
    -611,  -611,  -611,   417,   422,   424,   425,   154,  -611,  -611,
    -611,    38,  -611,  -611,  -611,  -611,   429,   227,    54,  -611,
    -611,   430,  -611,   481,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,    38,  -611,  -611,  -611,  -611,  -611,   227,   431,
    1074,  -611,  -611,   403,  -611,   433,   419,   493,   507,   507,
      41,   492,   457,   444,  -611,   227,   467,  -611,  -611,   454,
    -611,  -611,  -611,  -611,   455,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,   501,   559,   458,  -611,   507,    85,
    1456,  -611,   471,   461,   507,   465,   509,    41,    38,  -611,
    -611,   480,  -611,   468,   215,   507,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,   227,  -611,   479,  -611,  -611,  -611,  -611,   469,
    -611,  -611,  -611,   489,   117,   485,   490,    72,  -611,   219,
      41,   493,    38,    38,   474,    41,   559,  -611,   487,  1456,
     543,  -611,  -611,  -611,  -611,  -611,  1376,  -611,  -611,  -611,
     477,   478,  -611,  -611,  -611,  -611,  -611,  -611,  -611,   495,
    -611,  -611,  -611,  -611,    38,   215,    41,   486,  -611,    41,
     497,   499,  -611,  -611,  -611,  -611,  -611,  -611,   517,  -611,
     500,   513,  -611,  -611,   518,  -611
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,     0,     3,     1,    38,   143,    40,    70,   220,
     281,   296,   331,   379,     0,     0,     0,     0,    94,     0,
       0,   490,     0,     0,   559,   580,     0,     6,     7,    42,
      24,    61,     0,     0,    22,    64,    77,    66,    26,    78,
      83,    79,    84,    77,    80,    81,    65,    18,    10,     0,
       0,    12,   226,   283,   222,   330,   223,   249,   250,   224,
      20,    14,    16,    28,   450,   449,   452,    30,   488,    32,
     520,   522,   521,   519,    77,   538,   539,   518,    77,    34,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   139,   261,   225,    77,     0,    77,    88,    77,     0,
      82,    77,     0,   456,   531,     0,   138,   134,     0,   133,
       0,   209,   209,     0,    46,     0,     0,     0,   209,     8,
       9,     0,    97,    72,     0,     0,     0,   265,   267,     0,
     271,   272,   275,   276,   277,   278,   274,   279,   280,   345,
     353,   358,    92,   233,   102,   229,   231,   232,   230,   234,
     263,   264,   235,   239,   236,   238,   237,   240,   241,   283,
     246,     0,   247,   248,   242,     0,   245,   243,   352,   244,
     357,     0,     0,     5,     0,   207,     0,     0,   298,     0,
       0,     0,     0,     0,     0,     0,     0,   532,   525,   534,
       0,     0,   583,   579,    39,   274,   156,   144,   148,   152,
     153,   149,   150,   151,   154,   155,    41,    71,   221,   227,
     282,   297,   332,   380,    73,   529,    74,     0,   530,    95,
     461,   491,     0,   447,   136,   448,     0,     0,   193,    43,
      25,     0,   545,   541,   542,   547,   544,   548,   546,   543,
     540,     0,    48,   552,     0,     0,    23,    96,    75,    67,
      27,    85,   266,   273,   268,   270,     0,     0,    99,   344,
     341,   349,   354,    19,    11,   210,    13,   284,     0,    21,
      15,    17,    29,   453,    31,   502,   489,    33,    99,     0,
       0,    35,    37,   587,     0,     0,     0,     0,    89,   459,
     457,   499,   135,     0,   560,   208,   581,   196,     4,   549,
       0,   553,     0,   550,   182,   183,   184,   186,   189,   188,
     190,   191,   187,   185,     0,     0,     0,     0,   179,   578,
     157,   158,   159,   161,   163,   165,   168,   171,   175,   180,
     577,    62,     0,   112,   104,   269,   192,     0,   346,     0,
      93,     0,     0,     0,   213,   209,   299,   463,   506,   533,
     526,   535,   584,   145,   261,   228,   254,   255,   256,   262,
     333,   381,   112,     0,    99,   497,   492,   137,     0,   461,
       0,     0,     3,     0,    49,     0,   176,   177,   178,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     575,     0,    76,   132,     0,     0,     0,   209,    98,   342,
     350,   355,     0,   211,     0,   285,   289,   209,   454,     0,
     112,   104,   369,   374,     0,   484,     0,     0,   592,   367,
     368,   588,   590,     0,   594,     0,   586,     0,   209,   251,
     209,   289,     0,   460,   458,     0,    99,   561,   582,   200,
     194,     0,   202,   195,     0,   197,   203,     0,     0,     0,
       0,   551,   181,   160,   162,   164,   166,   167,   169,   170,
     172,   173,   174,   209,    63,   129,   127,   389,   390,     0,
     111,   119,   113,   123,   121,   125,     0,   115,   117,   394,
     109,   108,     0,   103,     0,   105,   106,     0,     0,     0,
       0,     0,     0,   133,   214,     0,   215,   218,   294,   291,
     290,     0,   209,     0,     0,     0,     0,   474,     0,   462,
     464,   466,   468,   470,   472,   476,     0,     0,   507,     0,
     505,   508,   510,     0,     0,     0,     0,   480,   479,     0,
     483,   482,     0,     0,     0,     0,     0,     0,     0,   585,
     146,     0,   252,     0,   334,   339,   209,     0,   498,   493,
       0,     0,   198,   206,   199,    45,   554,    50,     0,   130,
       0,    69,     0,     0,     0,     0,     0,   393,   423,   420,
     421,   422,   384,   392,     0,     0,   209,    87,   110,   107,
     348,   347,   343,   351,   356,     0,   212,     0,   216,     0,
       0,   286,   288,   265,   310,   305,   306,   307,   308,   300,
     309,     0,     0,     0,     0,   455,     0,     0,     0,     0,
       0,     0,   512,   515,   504,     0,     0,     0,     0,   370,
     375,   478,   573,   574,   593,   589,   591,   481,   595,     0,
     364,   360,   363,     0,   340,     0,   336,     0,    91,     0,
     571,   567,   569,   562,   566,     0,     0,     0,     0,   576,
       0,   128,   120,   114,   124,   122,   126,   209,   116,   118,
       0,   219,     0,   218,   295,   292,     0,   487,   485,   486,
     475,   465,   467,   469,   471,   473,   477,     0,     0,   509,
     511,   528,   537,     0,     0,   147,     0,   361,   253,   335,
     337,   383,   494,     0,     0,     0,     0,   564,   201,   205,
     204,     0,    56,    42,    51,    55,     0,   131,     0,   395,
     217,     0,   301,   398,   513,   516,   371,   376,   260,   365,
     362,   209,     0,   572,   568,   570,   563,   565,    58,     0,
       0,    57,   385,     0,   293,     0,     0,     0,   430,   430,
       0,   434,   257,     0,   338,   495,     0,    52,    54,     0,
     396,   302,   399,   406,     0,   405,   427,   514,   517,   372,
     431,   377,   258,   366,   501,     0,   411,   386,   430,     0,
       0,   401,   402,     0,   430,     0,   438,     0,     0,   496,
     558,     0,   557,     0,     0,   430,   397,   317,   324,   322,
     303,   313,   314,   321,   407,   403,   428,   373,   432,   435,
     378,   259,   500,    59,   555,   410,   424,   425,   426,     0,
     416,   417,   387,     0,     0,     0,     0,     0,   209,   319,
       0,     0,     0,     0,     0,     0,     0,   412,   413,     0,
     442,   318,   325,   323,   304,   312,     0,   320,   408,   404,
       0,     0,   436,    60,   556,   414,   418,   439,   388,     0,
     327,   315,   429,   433,     0,     0,     0,     0,   326,     0,
       0,     0,   415,   419,   440,   328,   316,   437,     0,   446,
       0,   443,   441,   444,     0,   445
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -611,  -611,   311,   312,   565,  -598,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -590,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -122,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,   238,  -611,
    -611,     9,  -611,  -611,  -611,   598,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,   600,  -611,   242,  -611,  -611,  -248,  -611,
    -611,   208,  -611,  -611,  -611,  -268,  -353,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -331,  -611,  -611,   -22,  -611,
    -611,  -187,   -10,  -611,    17,  -611,  -611,  -611,  -611,  -195,
      -9,  -224,  -611,   244,   263,   245,  -120,  -114,   -90,   -13,
    -611,  -303,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,     0,   571,  -611,  -611,  -611,  -611,   -41,    59,    20,
    -611,    99,  -611,   -28,   162,  -444,  -611,  -611,  -611,    68,
    -611,  -611,  -610,   -81,  -611,  -611,    -7,  -611,   -70,  -611,
    -611,   -50,   -49,   -58,   -55,   -52,   293,  -611,   -40,  -611,
     -37,  -611,  -611,  -611,  -611,   256,   326,   176,  -611,  -611,
    -611,   -32,  -611,   -31,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -153,  -611,  -611,  -611,  -611,  -611,  -150,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,   -38,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,   -39,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,   -66,  -611,  -611,  -611,   -65,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,   -15,  -611,  -611,  -338,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,    22,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -594,  -611,  -611,  -611,  -611,
    -611,  -140,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -172,  -611,  -611,  -500,  -611,  -550,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
      25,    27,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,   320,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -309,   261,  -293,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,   610,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,   316,  -611,  -611,  -136,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,    -5,  -611,  -611,
    -611,   285,  -611,  -611,   138,  -611,  -611,  -611,  -611,  -611,
    -611,  -611,  -611,  -611,  -611,  -611,  -611,  -611
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     3,    27,    28,   172,   176,   180,   181,
     171,   179,   121,   116,   125,   182,   184,   186,   190,   191,
      82,    29,    84,    30,   115,   298,   448,    31,    32,   117,
     302,   450,   648,   729,   704,   730,   705,   706,   746,   825,
      33,   118,   391,    34,    35,   124,   333,   469,    36,    85,
      37,   142,   332,    38,    39,    40,   126,   334,   482,    41,
     217,   362,   547,    42,   258,    43,   102,   248,   340,    44,
      45,   395,   483,   484,   485,   394,   470,   563,   574,   575,
     562,   565,   564,   566,   560,   392,   465,   650,   318,   222,
     293,   109,   354,    46,   471,    83,   284,   427,   629,   197,
     319,   336,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   337,    48,   297,   370,   443,   551,   444,   445,   647,
      49,   404,   295,   344,   403,   495,   496,   588,   497,   472,
      86,   208,   285,   209,   145,   146,   147,   148,    52,   355,
     429,   633,   356,   717,   742,   777,   357,   358,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    53,    87,
      54,   177,   345,   501,   405,   502,   592,   500,   590,   711,
     589,    55,    88,    56,   268,   407,   666,   735,   769,   816,
     599,   790,   817,   791,   818,   860,   813,   792,   819,   793,
     815,   814,   849,   851,   859,    57,    58,    59,    89,   286,
     430,   635,   544,   636,   721,   545,   164,   341,   490,   165,
     257,   397,   166,   167,   342,   491,   168,   169,   343,   492,
     170,   359,   428,   631,   687,   632,   686,   743,   473,   419,
     525,   683,   740,   774,   420,   526,   684,   741,   776,   474,
      90,   287,   431,   637,   475,   657,   749,   785,   830,   476,
     572,   487,   576,   733,   768,   714,   736,   737,   754,   772,
     821,   755,   770,   820,   767,   783,   784,   809,   828,   855,
     810,   829,   856,   573,   811,   757,   773,   822,   761,   775,
     823,   800,   824,   854,   848,   857,   868,   870,   871,   874,
     477,   478,    63,    64,    65,   183,   347,   508,    66,   220,
     364,   290,   363,   408,   509,   606,   607,   608,   609,   610,
     604,   611,   641,   529,   642,   423,   531,   512,   513,   514,
      67,   185,    68,   105,   291,   436,   639,   722,   764,   366,
     435,   779,   276,   348,   519,   409,   520,   615,   616,   521,
     677,   738,   522,   678,   739,    69,    70,    71,    72,    73,
     279,   410,   617,    74,    75,    76,   188,   278,    77,   280,
     411,   618,    78,   241,   242,   303,   243,   781,   804,   782,
      79,   111,   368,   550,   696,   643,   697,   644,   694,   695,
     693,   424,   245,   390,   330,    80,    81,   112,   369,   193,
     283,   425,   352,   426,   535,   536,   534,   538
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     108,   110,    50,   159,    92,   163,   160,    93,   398,   103,
     104,   161,   162,   198,   418,   144,   113,   204,   205,    47,
     320,   143,    51,   688,    60,   201,   620,    61,   202,    62,
     349,   203,   569,   199,   200,   292,   299,   294,   399,   400,
     401,   106,   486,   421,    91,   123,   159,   296,   163,   160,
     702,   634,   106,   709,   161,   162,   518,   732,   703,   422,
     622,   196,   206,     4,   143,   106,    47,   623,   106,    51,
     507,    60,     8,   787,    61,   207,    62,   210,   211,   212,
     213,   527,   569,   187,   715,   215,   787,   187,  -359,   218,
     174,   223,   219,   379,   432,   221,   225,   224,   114,   510,
     788,   789,   224,   214,     8,   216,   367,   259,  -359,   260,
     630,   226,   227,   788,   789,   511,   434,   119,   244,   107,
     106,   174,   304,   305,   306,   307,   308,   309,   310,   311,
     107,   174,   702,   312,   313,   174,   174,   174,   224,   750,
     703,   174,   523,   107,   120,   224,   224,    18,   314,   315,
     178,    95,   265,   316,   317,   106,    18,  -329,  -140,  -141,
      99,   198,  -311,   122,   122,   204,   205,   254,   255,   412,
     413,   486,   488,   201,   252,   442,   202,   253,   494,   203,
     174,   199,   200,   716,   412,   413,   383,   384,   549,   758,
     304,   305,   306,   307,   308,   309,   310,   311,   107,   414,
    -142,   312,   313,   122,   415,   122,  -100,   416,   417,   196,
     838,   122,   640,   192,   414,   493,   314,   315,   786,   415,
     194,   316,   317,  -451,   797,   304,   305,   306,   307,   308,
     309,   310,   311,   107,  -523,   812,   312,   313,   228,   569,
     229,   387,   388,   389,   385,   386,   863,   788,   789,   865,
     806,   807,   808,    94,    96,   106,   230,   317,    98,   101,
     593,   128,   129,   456,   457,   132,   133,   134,   135,   246,
     794,   458,   459,   106,   353,    12,   247,   249,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
      10,    11,   250,    12,   139,   140,   141,   460,   461,   462,
     198,   376,   377,   378,   204,   205,   251,   256,   224,   261,
     393,   262,   201,   263,   264,   202,  -208,   393,   203,   266,
     199,   200,   267,   269,   270,   271,   569,   272,   274,   846,
     273,   277,   447,   107,   320,   275,   281,   282,  -524,   174,
     288,   433,   621,   106,   301,   300,   289,   331,   196,   338,
     627,   107,   335,   339,   350,   351,   360,   361,  -101,   640,
     373,   661,   346,   122,   159,   365,   163,   160,   374,   375,
     380,    50,   161,   162,   381,   396,   499,   382,   402,   437,
     449,   464,   143,   719,   -44,   569,   452,   441,    47,   463,
     530,    51,   528,    60,   532,   533,    61,   489,    62,   539,
     662,   528,   540,   552,   542,   320,   554,   503,   556,   555,
     570,   571,   557,   548,   667,   668,   669,   561,   559,   577,
     580,   579,   699,   582,   619,   585,   583,   584,   541,   591,
     543,   586,   587,   595,   553,  -287,   605,   614,   624,   625,
     626,   628,   646,   651,   159,   596,   163,   160,   597,  -382,
     638,   598,   161,   162,   568,   652,   499,   653,   412,   413,
     570,   571,   143,   558,   654,   600,   655,   143,   656,   658,
     659,   664,   252,   670,   671,   672,   673,   674,   675,   504,
     505,   594,   601,   602,   603,   676,   679,   680,   414,   506,
     681,   840,   841,   415,   682,   690,   416,   417,   689,   691,
     692,   712,   698,   700,   568,   723,   612,   613,   713,   630,
     724,   159,   725,   163,   160,   726,  -400,   731,   734,   161,
     162,   747,   751,   861,   466,   752,  -391,     6,   753,   143,
       9,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,
    -391,  -391,  -391,    10,    11,   756,    12,   760,  -391,  -391,
      13,   762,   763,   412,   413,   467,   468,  -391,   765,   766,
     778,   771,   780,    14,  -409,   795,   796,   479,   480,   481,
     798,   799,   803,   826,   805,   827,   660,   831,   833,   842,
     834,   845,   847,   852,   853,   866,    22,    23,   858,   106,
     832,   864,   869,   875,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   195,  -391,   867,   872,   873,   748,   371,
     372,   140,   141,   -86,   173,   100,    97,   570,   571,   524,
     685,   175,   710,   466,   453,  -391,     6,   455,   707,     9,
    -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,
    -391,  -391,    10,    11,   454,    12,   663,  -391,  -391,    13,
     645,   581,   412,   413,   467,   468,  -391,   708,   665,   759,
     578,   568,    14,   446,   835,    47,   516,   107,    51,   837,
      60,   406,   720,    61,    92,    62,   517,   718,   -47,   728,
     -47,   839,   744,   862,   537,    22,    23,   546,   189,   438,
     844,   451,   727,   515,   -47,   -47,   649,   -47,   -47,     0,
     745,   -47,     0,  -391,   570,   571,     0,     0,     0,   231,
       0,   232,  -503,     0,     0,     0,     0,     0,     0,     0,
       0,   543,     0,   -47,     0,   233,   234,   -47,   235,   236,
      92,     0,   237,   718,     0,     0,     0,     0,     0,    -2,
       5,   -47,     0,     6,     7,     8,     9,    47,   568,     0,
      51,     0,    60,     0,   238,    61,   802,    62,   239,    10,
      11,     0,    12,   570,   571,     0,    13,    92,     0,     0,
     801,     0,   240,     0,     0,     0,     0,     0,     0,    14,
      15,    16,    17,     0,     0,     0,     0,     0,    18,    19,
       0,     0,    20,     0,     0,    21,   159,     0,   163,   160,
     393,   393,    22,    23,   161,   162,     0,   568,   850,    24,
      25,     0,     0,     0,   143,   843,     0,     0,   836,     0,
       0,     0,     0,     0,     0,    26,  -209,   466,     0,  -391,
       6,     0,   393,     9,  -391,  -391,  -391,  -391,  -391,  -391,
    -391,  -391,  -391,  -391,  -391,  -391,    10,    11,     0,    12,
       0,  -391,  -391,    13,     0,     0,   412,   413,   467,   468,
    -391,     0,     0,     0,     0,     0,    14,     0,     0,   106,
     479,   480,   481,     0,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   195,   137,   138,     0,     0,     0,    22,
      23,   140,   141,     0,     0,     0,     0,     0,     0,     0,
     567,     0,     0,     0,     0,     0,     0,  -391,     0,     0,
       0,   466,     0,  -391,     6,     0,  -536,     9,  -391,  -391,
    -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,
      10,    11,     0,    12,     0,  -391,  -391,    13,     0,     0,
     412,   413,   467,   468,  -391,     0,   466,   107,  -391,     6,
      14,     0,     9,  -391,  -391,  -391,  -391,  -391,  -391,  -391,
    -391,  -391,  -391,  -391,  -391,    10,    11,     0,    12,     0,
    -391,  -391,    13,    22,    23,   412,   413,   467,   468,  -391,
       0,     0,     0,     0,     0,    14,     0,     0,     0,     0,
       0,  -391,     0,     0,     0,     0,     0,     0,     0,     0,
     -68,     0,     0,     0,     0,     0,   106,     0,    22,    23,
       0,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,     0,     0,     0,  -391,   139,   140,   141,
     466,     0,  -391,     6,     0,   -90,     9,  -391,  -391,  -391,
    -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,    10,
      11,     0,    12,     0,  -391,  -391,    13,     0,     0,   412,
     413,   467,   468,  -391,     0,     0,     0,     0,     0,    14,
       0,     0,     0,     0,     0,     5,     0,     0,     6,     7,
       8,     9,     0,     0,   107,     0,     0,     0,     0,     0,
     174,     0,    22,    23,    10,    11,     0,    12,     0,     0,
       0,    13,     0,     0,     0,     0,     0,     0,     0,     0,
    -391,     0,     0,     0,    14,    15,    16,    17,     0,  -527,
       0,     0,     0,    18,    19,     0,     0,    20,     0,     0,
      21,     0,     0,     0,     0,     0,     0,    22,    23,     0,
       0,     0,     0,     0,    24,    25,   701,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      26,   106,     6,     0,   -53,   439,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   195,   137,     0,     0,     0,
       0,    12,     5,   140,   141,     6,     7,     8,     9,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    10,    11,     0,    12,     0,     0,     0,    13,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    14,    15,    16,    17,     0,     0,     0,     0,     0,
      18,    19,     0,     0,    20,     0,     0,    21,     0,   107,
       0,     0,     0,     0,    22,    23,     0,     0,   440,     0,
       0,    24,    25,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   498,     0,   106,     0,     0,    26,  -209,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,    10,    11,     0,    12,   139,   140,   141,     5,     0,
       0,     6,     7,     8,     9,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    10,    11,     0,
      12,     0,     0,     5,    13,     0,     6,     7,     8,     9,
       0,     0,     0,     0,     0,     0,     0,    14,    15,    16,
      17,     0,    10,    11,     0,    12,    18,    19,     0,    13,
      20,     0,   107,    21,     0,     0,     0,     0,   174,     0,
      22,    23,    14,    15,    16,    17,     0,    24,    25,   701,
       0,    18,    19,     0,     0,    20,     0,     0,    21,     0,
       0,     0,     0,    26,     0,    22,    23,     0,     0,   106,
       0,     0,    24,    25,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,    10,    11,    26,    12,
     139,   140,   141,   498,     0,   106,     0,     0,     0,     0,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,    10,    11,   106,    12,   139,   140,   141,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,    10,    11,     0,    12,   139,   140,   141,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   107,     0,   106,
       0,     0,     0,   174,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   195,   137,   138,     0,     0,     0,     0,
       0,   140,   141,   107,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   107
};

static const yytype_int16 yycheck[] =
{
      22,    23,     2,    43,    14,    43,    43,    14,   339,    19,
      20,    43,    43,    83,   352,    43,    26,    83,    83,     2,
     244,    43,     2,   633,     2,    83,   526,     2,    83,     2,
     278,    83,   476,    83,    83,   222,   231,     3,   341,   342,
     343,     3,   395,   352,     3,    36,    86,     3,    86,    86,
     648,     3,     3,     3,    86,    86,   409,     3,   648,   352,
       3,    83,    84,     0,    86,     3,    49,     3,     3,    49,
     408,    49,     6,     1,    49,    85,    49,    87,    88,    89,
      90,    19,   526,    74,   678,    95,     1,    78,    87,    99,
      87,    75,   102,   317,   362,   105,    75,    81,    91,   408,
      28,    29,    81,    94,     6,    96,   293,    92,   107,    94,
     107,   111,   112,    28,    29,   408,   364,    88,   118,    81,
       3,    87,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    87,   730,    84,    85,    87,    87,    87,    81,   733,
     730,    87,   410,    81,    88,    81,    81,    49,    99,   100,
      22,    53,   174,   104,   105,     3,    49,    88,    88,    88,
      53,   231,    90,    93,    93,   231,   231,     8,     9,    30,
      31,   524,   396,   231,     8,   370,   231,    11,   402,   231,
      87,   231,   231,   683,    30,    31,    82,    83,   436,   739,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    60,
      88,    84,    85,    93,    65,    93,    88,    68,    69,   231,
     820,    93,   550,    89,    60,   402,    99,   100,   768,    65,
      88,   104,   105,    88,   774,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    88,   785,    84,    85,    89,   683,
      89,   101,   102,   103,    99,   100,   856,    28,    29,   859,
      35,    36,    37,    15,    16,     3,    88,   105,    16,    17,
       8,     9,    10,   383,   384,    13,    14,    15,    16,    88,
     770,   385,   386,     3,   284,    23,    48,    89,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    88,    23,    24,    25,    26,   387,   388,   389,
     370,   314,   315,   316,   370,   370,    89,    91,    81,    91,
     332,    91,   370,    88,    88,   370,    87,   339,   370,    88,
     370,   370,    89,    88,    88,    88,   770,    88,    88,   829,
      89,    88,   370,    81,   558,    89,    88,    88,    88,    87,
      89,   363,   529,     3,     3,    91,    93,    92,   370,    91,
     537,    81,     8,    47,    89,    89,    89,    89,    88,   697,
       3,   585,   105,    93,   404,    93,   404,   404,    92,    94,
      96,   371,   404,   404,    97,    94,   404,    98,   105,    89,
      92,   391,   404,   686,    90,   829,   106,   370,   371,    94,
      58,   371,   414,   371,   416,   417,   371,   397,   371,    90,
     587,   423,    95,    88,    94,   629,    88,   407,     3,    90,
     476,   476,    89,   435,   601,   602,   603,    90,    94,    90,
      92,    88,   646,    92,    31,    95,    92,    92,   428,    90,
     430,   106,    94,   503,   444,    90,    90,    90,    88,    88,
      88,    88,    29,    88,   484,   503,   484,   484,   503,    90,
      90,   503,   484,   484,   476,    88,   484,    88,    30,    31,
     526,   526,   484,   463,    88,   503,    88,   489,    88,    88,
      88,    88,     8,    88,    88,    88,    88,    88,    88,    51,
      52,   503,   504,   505,   506,    88,    88,    88,    60,    61,
      90,   822,   823,    65,    90,    94,    68,    69,    90,    90,
      66,   106,    88,    88,   526,    88,   516,   517,   105,   107,
      88,   551,    88,   551,   551,    90,    35,    88,    88,   551,
     551,    90,    89,   854,     1,   106,     3,     4,    35,   551,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    38,    23,    55,    25,    26,
      27,    94,   108,    30,    31,    32,    33,    34,    91,   105,
      59,   106,     3,    40,   106,    94,   105,    44,    45,    46,
     105,    62,    92,    94,   106,   106,   576,    88,    93,   105,
      90,    94,    39,   106,   106,    88,    63,    64,    93,     3,
     814,   105,    75,    75,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    81,   106,   106,    94,   730,   298,
     298,    25,    26,    90,    49,    17,    16,   683,   683,   411,
     629,    50,   663,     1,   380,     3,     4,   382,   650,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,   381,    23,   587,    25,    26,    27,
     551,   489,    30,    31,    32,    33,    34,   657,   590,   740,
     484,   683,    40,   370,   817,   648,    44,    81,   648,   819,
     648,   345,   687,   648,   684,   648,    54,   684,     4,   701,
       6,   821,   721,   855,   423,    63,    64,   431,    78,   369,
     826,   375,   697,   408,    20,    21,   558,    23,    24,    -1,
     722,    27,    -1,    81,   770,   770,    -1,    -1,    -1,     4,
      -1,     6,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   721,    -1,    49,    -1,    20,    21,    53,    23,    24,
     740,    -1,    27,   740,    -1,    -1,    -1,    -1,    -1,     0,
       1,    67,    -1,     4,     5,     6,     7,   730,   770,    -1,
     730,    -1,   730,    -1,    49,   730,   778,   730,    53,    20,
      21,    -1,    23,   829,   829,    -1,    27,   777,    -1,    -1,
     777,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      41,    42,    43,    -1,    -1,    -1,    -1,    -1,    49,    50,
      -1,    -1,    53,    -1,    -1,    56,   836,    -1,   836,   836,
     822,   823,    63,    64,   836,   836,    -1,   829,   836,    70,
      71,    -1,    -1,    -1,   836,   825,    -1,    -1,   818,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    87,     1,    -1,     3,
       4,    -1,   854,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      -1,    25,    26,    27,    -1,    -1,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,     3,
      44,    45,    46,    -1,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    63,
      64,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,
      -1,     1,    -1,     3,     4,    -1,    90,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    -1,    23,    -1,    25,    26,    27,    -1,    -1,
      30,    31,    32,    33,    34,    -1,     1,    81,     3,     4,
      40,    -1,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    23,    -1,
      25,    26,    27,    63,    64,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    -1,    -1,     3,    -1,    63,    64,
      -1,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    81,    24,    25,    26,
       1,    -1,     3,     4,    -1,    90,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    -1,    25,    26,    27,    -1,    -1,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,     1,    -1,    -1,     4,     5,
       6,     7,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    63,    64,    20,    21,    -1,    23,    -1,    -1,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,    40,    41,    42,    43,    -1,    90,
      -1,    -1,    -1,    49,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,     3,     4,    -1,    90,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      -1,    23,     1,    25,    26,     4,     5,     6,     7,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    20,    21,    -1,    23,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    40,    41,    42,    43,    -1,    -1,    -1,    -1,    -1,
      49,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    81,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    90,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,    -1,    -1,    86,    87,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    24,    25,    26,     1,    -1,
      -1,     4,     5,     6,     7,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      23,    -1,    -1,     1,    27,    -1,     4,     5,     6,     7,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,
      43,    -1,    20,    21,    -1,    23,    49,    50,    -1,    27,
      53,    -1,    81,    56,    -1,    -1,    -1,    -1,    87,    -1,
      63,    64,    40,    41,    42,    43,    -1,    70,    71,    72,
      -1,    49,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    -1,    -1,    86,    -1,    63,    64,    -1,    -1,     3,
      -1,    -1,    70,    71,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    86,    23,
      24,    25,    26,     1,    -1,     3,    -1,    -1,    -1,    -1,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,     3,    23,    24,    25,    26,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    24,    25,    26,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81,    -1,     3,
      -1,    -1,    -1,    87,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      -1,    25,    26,    81,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    81
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   110,   111,   112,     0,     1,     4,     5,     6,     7,
      20,    21,    23,    27,    40,    41,    42,    43,    49,    50,
      53,    56,    63,    64,    70,    71,    86,   113,   114,   130,
     132,   136,   137,   149,   152,   153,   157,   159,   162,   163,
     164,   168,   172,   174,   178,   179,   202,   203,   221,   229,
     230,   238,   247,   267,   269,   280,   282,   304,   305,   306,
     348,   399,   400,   401,   402,   403,   407,   429,   431,   454,
     455,   456,   457,   458,   462,   463,   464,   467,   471,   479,
     494,   495,   129,   204,   131,   158,   239,   268,   281,   307,
     349,     3,   201,   255,   157,    53,   157,   172,   174,    53,
     164,   174,   175,   201,   201,   432,     3,    81,   197,   200,
     197,   480,   496,   201,    91,   133,   122,   138,   150,    88,
      88,   121,    93,   160,   154,   123,   165,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    24,
      25,    26,   160,   197,   242,   243,   244,   245,   246,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     269,   280,   282,   306,   315,   318,   321,   322,   325,   326,
     329,   119,   115,   113,    87,   231,   116,   270,    22,   120,
     117,   118,   124,   404,   125,   430,   126,   160,   465,   465,
     127,   128,    89,   498,    88,    17,   197,   208,   257,   260,
     261,   262,   263,   264,   322,   326,   197,   201,   240,   242,
     201,   201,   201,   201,   160,   201,   160,   169,   201,   201,
     408,   201,   198,    75,    81,    75,   230,   230,    89,    89,
      88,     4,     6,    20,    21,    23,    24,    27,    49,    53,
      67,   472,   473,   475,   230,   491,    88,    48,   176,    89,
      88,    89,     8,    11,     8,     9,    91,   319,   173,    92,
      94,    91,    91,    88,    88,   197,    88,    89,   283,    88,
      88,    88,    88,    89,    88,    89,   441,    88,   466,   459,
     468,    88,    88,   499,   205,   241,   308,   350,    89,    93,
     410,   433,   200,   199,     3,   231,     3,   222,   134,   208,
      91,     3,   139,   474,    73,    74,    75,    76,    77,    78,
      79,    80,    84,    85,    99,   100,   104,   105,   197,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     493,    92,   161,   155,   166,     8,   210,   220,    91,    47,
     177,   316,   323,   327,   232,   271,   105,   405,   442,   177,
      89,    89,   501,   201,   201,   248,   251,   255,   256,   330,
      89,    89,   170,   411,   409,    93,   438,   200,   481,   497,
     223,   111,   112,     3,    92,    94,   218,   218,   218,   210,
      96,    97,    98,    82,    83,    99,   100,   101,   102,   103,
     492,   151,   194,   197,   184,   180,    94,   320,   194,   220,
     220,   220,   105,   233,   230,   273,   275,   284,   412,   444,
     460,   469,    30,    31,    60,    65,    68,    69,   337,   338,
     343,   421,   423,   424,   490,   500,   502,   206,   331,   249,
     309,   351,   184,   197,   177,   439,   434,    89,   410,     7,
      90,   203,   208,   224,   226,   227,   265,   306,   135,    92,
     140,   475,   106,   212,   213,   214,   215,   215,   216,   216,
     217,   217,   217,    94,   201,   195,     1,    32,    33,   156,
     185,   203,   238,   337,   348,   353,   358,   399,   400,    44,
      45,    46,   167,   181,   182,   183,   185,   360,   210,   230,
     317,   324,   328,   200,   210,   234,   235,   237,     1,   242,
     276,   272,   274,   230,    51,    52,    61,   337,   406,   413,
     421,   423,   426,   427,   428,   490,    44,    54,   185,   443,
     445,   448,   451,   184,   180,   339,   344,    19,   197,   422,
      58,   425,   197,   197,   505,   503,   504,   422,   506,    90,
      95,   230,    94,   230,   311,   314,   274,   171,   197,   177,
     482,   225,    88,   201,    88,    90,     3,    89,   230,    94,
     193,    90,   189,   186,   191,   190,   192,    34,   197,   244,
     322,   326,   359,   382,   187,   188,   361,    90,   276,    88,
      92,   243,    92,    92,    92,    95,   106,    94,   236,   279,
     277,    90,   275,     8,   197,   257,   262,   263,   264,   289,
     306,   197,   197,   197,   419,    90,   414,   415,   416,   417,
     418,   420,   201,   201,    90,   446,   447,   461,   470,    31,
     382,   200,     3,     3,    88,    88,    88,   200,    88,   207,
     107,   332,   334,   250,     3,   310,   312,   352,    90,   435,
     337,   421,   423,   484,   486,   240,    29,   228,   141,   493,
     196,    88,    88,    88,    88,    88,    88,   354,    88,    88,
     230,   210,   200,   237,    88,   248,   285,   200,   200,   200,
      88,    88,    88,    88,    88,    88,    88,   449,   452,    88,
      88,    90,    90,   340,   345,   209,   335,   333,   251,    90,
      94,    90,    66,   489,   487,   488,   483,   485,    88,   210,
      88,    72,   114,   130,   143,   145,   146,   197,   230,     3,
     236,   278,   106,   105,   364,   364,   382,   252,   255,   220,
     334,   313,   436,    88,    88,    88,    90,   486,   197,   142,
     144,    88,     3,   362,    88,   286,   365,   366,   450,   453,
     341,   346,   253,   336,   314,   197,   147,    90,   145,   355,
     364,    89,   106,    35,   367,   370,    38,   384,   384,   252,
      55,   387,    94,   108,   437,    91,   105,   373,   363,   287,
     371,   106,   368,   385,   342,   388,   347,   254,    59,   440,
       3,   476,   478,   374,   375,   356,   384,     1,    28,    29,
     290,   292,   296,   298,   382,    94,   105,   384,   105,    62,
     390,   255,   197,    92,   477,   106,    35,    36,    37,   376,
     379,   383,   384,   295,   300,   299,   288,   291,   293,   297,
     372,   369,   386,   389,   391,   148,    94,   106,   377,   380,
     357,    88,   210,    93,    90,   292,   230,   298,   251,   370,
     194,   194,   105,   201,   478,    94,   382,    39,   393,   301,
     242,   302,   106,   106,   392,   378,   381,   394,    93,   303,
     294,   194,   379,   251,   105,   251,    88,   106,   395,    75,
     396,   397,   106,    94,   398,    75
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
     178,   178,   179,   180,   180,   181,   181,   181,   182,   182,
     183,   184,   184,   186,   185,   187,   185,   188,   185,   189,
     185,   190,   185,   191,   185,   192,   185,   193,   185,   194,
     196,   195,   195,   197,   198,   197,   199,   197,   200,   201,
     202,   202,   202,   204,   205,   206,   207,   203,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   209,   210,   211,
     211,   212,   212,   213,   213,   214,   214,   214,   215,   215,
     215,   216,   216,   216,   216,   217,   217,   217,   217,   218,
     218,   218,   219,   219,   219,   219,   219,   219,   219,   219,
     219,   219,   220,   222,   221,   223,   223,   224,   224,   224,
     225,   224,   226,   226,   227,   228,   228,   229,   230,   230,
     232,   231,   233,   233,   234,   234,   235,   236,   236,   237,
     239,   238,   238,   238,   238,   238,   238,   241,   240,   242,
     242,   243,   243,   243,   244,   244,   244,   244,   244,   244,
     244,   244,   245,   245,   245,   245,   246,   246,   246,   247,
     247,   248,   250,   249,   249,   251,   251,   252,   254,   253,
     253,   255,   256,   257,   257,   258,   258,   258,   259,   259,
     259,   260,   260,   260,   261,   262,   262,   263,   264,   265,
     266,   268,   267,   270,   271,   272,   269,   273,   274,   274,
     275,   277,   278,   276,   279,   276,   281,   280,   283,   284,
     285,   286,   287,   288,   282,   289,   289,   289,   289,   289,
     289,   290,   291,   291,   293,   294,   292,   295,   292,   296,
     297,   297,   299,   298,   300,   301,   298,   303,   302,   304,
     305,   307,   308,   309,   310,   306,   311,   313,   312,   312,
     314,   316,   317,   315,   315,   319,   320,   318,   321,   323,
     324,   322,   322,   325,   327,   328,   326,   326,   329,   331,
     330,   332,   333,   333,   335,   336,   334,   337,   337,   339,
     340,   341,   342,   338,   344,   345,   346,   347,   343,   349,
     350,   351,   352,   348,   354,   355,   356,   357,   353,   358,
     358,   358,   359,   359,   361,   362,   363,   360,   365,   364,
     366,   364,   367,   369,   368,   368,   371,   372,   370,   374,
     373,   375,   373,   376,   378,   377,   377,   380,   381,   379,
     382,   382,   382,   382,   383,   383,   383,   385,   386,   384,
     384,   388,   389,   387,   387,   391,   392,   390,   390,   394,
     395,   393,   393,   396,   398,   397,   397,   399,   400,   401,
     401,   402,   404,   405,   406,   403,   408,   409,   407,   411,
     410,   410,   412,   412,   414,   413,   415,   413,   416,   413,
     417,   413,   418,   413,   419,   413,   420,   413,   421,   422,
     422,   423,   424,   425,   425,   426,   427,   428,   430,   429,
     432,   433,   434,   435,   436,   437,   431,   439,   438,   438,
     440,   440,   442,   443,   441,   444,   444,   445,   446,   445,
     447,   445,   449,   450,   448,   452,   453,   451,   454,   454,
     454,   455,   455,   456,   457,   459,   460,   461,   458,   462,
     463,   464,   466,   465,   468,   469,   470,   467,   471,   471,
     472,   472,   472,   472,   472,   472,   472,   472,   472,   472,
     473,   474,   474,   475,   475,   476,   477,   477,   478,   480,
     481,   482,   483,   479,   484,   485,   485,   487,   486,   488,
     486,   489,   486,   490,   490,   491,   492,   492,   493,   494,
     496,   497,   495,   499,   500,   498,   501,   501,   503,   502,
     504,   502,   505,   502,   506,   502
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
       2,     1,     2,     2,     0,     1,     1,     2,     1,     1,
       2,     2,     0,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     2,
       0,     4,     0,     1,     0,     3,     0,     4,     1,     1,
       1,     2,     2,     0,     0,     0,     0,     9,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     3,     1,     3,     1,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     1,     2,     2,     2,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     6,     2,     0,     1,     2,     2,
       0,     4,     1,     1,     4,     2,     0,     2,     2,     0,
       0,     4,     3,     0,     1,     1,     2,     3,     0,     3,
       0,     3,     1,     1,     1,     2,     1,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     0,     4,     0,     1,     1,     2,     0,     4,
       0,     1,     1,     1,     1,     1,     2,     1,     2,     3,
       2,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     0,     0,     0,     7,     2,     2,     0,
       2,     0,     0,     5,     0,     3,     0,     3,     0,     0,
       0,     0,     0,     0,    15,     1,     1,     1,     1,     1,
       1,     2,     2,     0,     0,     0,     6,     0,     3,     2,
       2,     0,     0,     3,     0,     0,     5,     0,     3,     1,
       1,     0,     0,     0,     0,     9,     2,     0,     4,     0,
       2,     0,     0,     6,     2,     0,     0,     6,     6,     0,
       0,     6,     1,     1,     0,     0,     6,     1,     1,     0,
       4,     2,     2,     0,     0,     0,     5,     1,     1,     0,
       0,     0,     0,     9,     0,     0,     0,     0,     9,     0,
       0,     0,     0,     9,     0,     0,     0,     0,    11,     1,
       1,     0,     1,     1,     0,     0,     0,     8,     0,     3,
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
       2,     3,     0,     2,     5,     2,     3,     0,     1,     0,
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
#line 400 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Decl *d = (yyvsp[0].dcval);
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          if (d)
            {
              d->annotation_appls (*annotations);
            }
          else
            {
              idl_global->err ()-> unsupported_warning(
                "Annotating this type of definition is not supported");
            }
          delete annotations;
        }
#line 2371 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 6:
#line 415 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 2378 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 7:
#line 421 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 2386 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 8:
#line 425 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = 0;
        }
#line 2394 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 9:
#line 429 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = 0;
        }
#line 2402 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 10:
#line 436 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AnnotationDeclSeen);
        }
#line 2410 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 11:
#line 440 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2419 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 12:
#line 445 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 2427 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 13:
#line 449 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 2436 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 14:
#line 454 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 2444 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 15:
#line 458 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2453 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 16:
#line 463 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 2461 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 17:
#line 467 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2470 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 18:
#line 472 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 2478 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 19:
#line 476 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 2487 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 20:
#line 481 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 2495 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 21:
#line 485 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2504 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 22:
#line 490 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
        }
#line 2512 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 23:
#line 494 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2521 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 24:
#line 499 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
        }
#line 2529 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 25:
#line 503 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 2538 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 26:
#line 508 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeDeclSeen);
        }
#line 2546 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 27:
#line 512 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2555 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 28:
#line 517 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentDeclSeen);
        }
#line 2563 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 29:
#line 521 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2572 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 30:
#line 526 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeDeclSeen);
        }
#line 2580 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 31:
#line 530 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2589 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 32:
#line 535 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventDeclSeen);
        }
#line 2597 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 33:
#line 539 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2606 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 34:
#line 544 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeDeclSeen);
        }
#line 2614 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 35:
#line 548 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2623 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 36:
#line 553 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorDeclSeen);
        }
#line 2631 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 37:
#line 557 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2640 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 38:
#line 562 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 2648 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 39:
#line 566 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
          (yyval.dcval) = 0;
        }
#line 2658 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 40:
#line 575 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
        }
#line 2666 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 41:
#line 579 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 2674 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 42:
#line 586 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 2717 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 43:
#line 625 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
        }
#line 2725 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 44:
#line 629 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
        }
#line 2733 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 45:
#line 633 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
          /*
           * Finished with this module - pop it from the scope stack.
           */

          idl_global->scopes ().pop ();
          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 2747 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 46:
#line 646 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleIDSeen);
        }
#line 2755 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 47:
#line 653 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 2773 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 48:
#line 667 "fe/idl.ypp" /* yacc.c:1651  */
    {
          if (FE_Utils::duplicate_param_id ((yyvsp[0].plval)))
            {
              idl_global->err ()->duplicate_param_id (
                (yyvsp[-2].idlist));

              return 1;
            }
        }
#line 2787 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 49:
#line 677 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 2821 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 50:
#line 707 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleSqSeen);
        }
#line 2829 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 51:
#line 711 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleBodySeen);
        }
#line 2837 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 52:
#line 715 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 2858 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 58:
#line 749 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefSeen);
        }
#line 2867 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 59:
#line 754 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefParamsSeen);
        }
#line 2876 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 60:
#line 759 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 2958 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 61:
#line 840 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleSeen);
        }
#line 2967 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 62:
#line 845 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleArgsSeen);
        }
#line 2976 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 63:
#line 850 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3042 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 66:
#line 920 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3084 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 67:
#line 958 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
#line 3092 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 68:
#line 962 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
#line 3100 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 69:
#line 966 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
#line 3113 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 70:
#line 978 "fe/idl.ypp" /* yacc.c:1651  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
#line 3121 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 71:
#line 982 "fe/idl.ypp" /* yacc.c:1651  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (yyval.idval) = (yyvsp[0].idval);
         }
#line 3130 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 72:
#line 990 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3170 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 73:
#line 1027 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3203 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 74:
#line 1057 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3236 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 75:
#line 1089 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 3244 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 76:
#line 1093 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyvsp[0].nlval)->truncatable ((yyvsp[-2].bval));
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3253 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 77:
#line 1098 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = 0;
        }
#line 3261 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 82:
#line 1112 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->err ()->unsupported_error ("custom is not supported");
        }
#line 3269 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 84:
#line 1120 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3316 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 85:
#line 1163 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3324 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 86:
#line 1167 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3332 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 87:
#line 1171 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3355 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 88:
#line 1194 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3402 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 89:
#line 1237 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3410 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 90:
#line 1241 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3418 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 91:
#line 1245 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 3431 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 92:
#line 1258 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 3439 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 93:
#line 1262 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3474 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 94:
#line 1296 "fe/idl.ypp" /* yacc.c:1651  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
#line 3482 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 95:
#line 1300 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 3491 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 96:
#line 1308 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.bval) = true;
        }
#line 3499 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 97:
#line 1312 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.bval) = false;
        }
#line 3507 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 98:
#line 1320 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3515 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 99:
#line 1324 "fe/idl.ypp" /* yacc.c:1651  */
    {
/*      |    EMPTY */
          (yyval.nlval) = 0;
        }
#line 3524 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 100:
#line 1333 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3551 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 101:
#line 1357 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3578 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 102:
#line 1383 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3643 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 108:
#line 1459 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.vival) = AST_Field::vis_PUBLIC;
        }
#line 3651 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 109:
#line 1463 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.vival) = AST_Field::vis_PRIVATE;
        }
#line 3659 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 110:
#line 1470 "fe/idl.ypp" /* yacc.c:1651  */
    {
          if ((yyvsp[0].decls_val))
            {
              AST_Decls &decls = *(yyvsp[0].decls_val);
              for (size_t i = 0; i < decls.size (); i++)
                {
                  AST_Field * field = dynamic_cast<AST_Field*>(decls[i]);
                  if (field)
                    {
                      field->visibility ((yyvsp[-1].vival));
                    }
                }
            }
          delete (yyvsp[0].decls_val);
        }
#line 3679 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 113:
#line 1494 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 3687 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 114:
#line 1498 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3695 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 115:
#line 1502 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 3703 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 116:
#line 1506 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3711 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 117:
#line 1510 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 3719 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 118:
#line 1514 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3727 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 119:
#line 1518 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 3735 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 120:
#line 1522 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
#line 3743 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 121:
#line 1526 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 3751 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 122:
#line 1530 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3759 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 123:
#line 1534 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 3767 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 124:
#line 1538 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3775 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 125:
#line 1542 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
#line 3783 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 126:
#line 1546 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 3791 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 127:
#line 1550 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 3799 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 128:
#line 1554 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 3808 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 129:
#line 1562 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.nlval),
                          UTL_NameList ((yyvsp[-1].idlist),
                                        (yyvsp[0].nlval)),
                          1);
        }
#line 3819 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 130:
#line 1573 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
#line 3827 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 131:
#line 1577 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3851 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 132:
#line 1597 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = 0;
        }
#line 3859 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 133:
#line 1604 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN ((yyval.idlist),
                          UTL_IdList ((yyvsp[0].idval),
                                      0),
                          1);
        }
#line 3872 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 134:
#line 1613 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
#line 3880 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 135:
#line 1617 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3904 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 136:
#line 1638 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);

          // This cleans up all the non-global "::"s in scoped names.
          // If there is a global one, it gets put into the UTL_IdList,
          // so we clean it up in the case above.
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3918 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 137:
#line 1648 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 3934 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 138:
#line 1662 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.idval),
                          Identifier ((yyvsp[0].strval)),
                          1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3946 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 139:
#line 1672 "fe/idl.ypp" /* yacc.c:1651  */
    {
          /* defining_id is a defining identifier
             whereas id is usually a reference to a defining identifier */
          ACE_NEW_RETURN ((yyval.idval), Identifier ((yyvsp[0].strval)), 1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3958 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 140:
#line 1683 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4005 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 141:
#line 1727 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4035 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 142:
#line 1754 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4065 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 143:
#line 1783 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
#line 4073 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 144:
#line 1787 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
#line 4081 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 145:
#line 1791 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
#line 4089 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 146:
#line 1795 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
#line 4097 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 147:
#line 1799 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4157 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 154:
#line 1864 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_string;
        }
#line 4165 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 155:
#line 1868 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_wstring;
        }
#line 4173 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 156:
#line 1872 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4243 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 160:
#line 1945 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_or,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4256 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 162:
#line 1958 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_xor,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4269 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 164:
#line 1971 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_and,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4282 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 166:
#line 1984 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_left,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4295 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 167:
#line 1993 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_right,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4308 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 169:
#line 2006 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_add,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4321 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 170:
#line 2015 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_minus,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4334 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 172:
#line 2028 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mul,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4347 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 173:
#line 2037 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_div,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4360 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 174:
#line 2046 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mod,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4373 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 176:
#line 2059 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_u_plus,
                                    (yyvsp[0].exval),
                                    0
                                  );
        }
#line 4386 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 177:
#line 2068 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_u_minus,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4399 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 178:
#line 2077 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_bit_neg,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4412 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 179:
#line 2089 "fe/idl.ypp" /* yacc.c:1651  */
    {
          /*
           * An expression which is a scoped name is not resolved now,
           * but only when it is evaluated (such as when it is assigned
           * as a constant value).
           */
          AST_Expression *expr = 0;
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d =
            s->lookup_by_name ((yyvsp[0].idlist));

          if (d == 0)
            {
              if (!idl_global->ignore_lookup_errors_)
                {
                  idl_global->err ()->lookup_error ((yyvsp[0].idlist));
                  return 1;
                }
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

              expr = idl_global->gen ()->create_expr (
                  c->constant_value (),
                  c->et ());
            }
          else
            {
              // An AST_Expression owns the scoped name
              // passed in this constructor, so we copy it
              // and destroy it below no matter which case
              // is followed.
              expr = idl_global->gen ()->create_expr (
                  (yyvsp[0].idlist)->copy ());
              if (d->node_type () == AST_Decl::NT_enum_val)
                {
                  AST_EnumVal *enumval = AST_EnumVal::narrow_from_decl (d);
                  expr->enum_parent (enumval->enum_parent ());
                }
            }

          (yyval.exval) = expr;
          (yyvsp[0].idlist)->destroy ();
          delete (yyvsp[0].idlist);
          (yyvsp[0].idlist) = 0;
        }
#line 4474 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 181:
#line 2148 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = (yyvsp[-1].exval);
        }
#line 4482 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 182:
#line 2155 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].ival));
        }
#line 4490 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 183:
#line 2159 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr ((yyvsp[0].uival));
        }
#line 4499 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 184:
#line 2164 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].sval));
          (yyvsp[0].sval)->destroy ();
          delete (yyvsp[0].sval);
          (yyvsp[0].sval) = 0;
        }
#line 4510 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 185:
#line 2171 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].wsval));
        }
#line 4518 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 186:
#line 2175 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].cval));
        }
#line 4526 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 187:
#line 2179 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_OutputCDR::from_wchar wc ((yyvsp[0].wcval));
          (yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
#line 4535 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 188:
#line 2184 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].fixval));
        }
#line 4543 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 189:
#line 2188 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].dval));
        }
#line 4551 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 190:
#line 2192 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr (true);
        }
#line 4559 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 191:
#line 2196 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = idl_global->gen ()->create_expr (false);
        }
#line 4567 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 192:
#line 2203 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4636 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 193:
#line 2271 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 4657 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 194:
#line 2288 "fe/idl.ypp" /* yacc.c:1651  */
    {
          Identifier *id = (yyvsp[-4].idval);
          idl_global->scopes ().pop ();
          id->destroy ();
          delete id;
        }
#line 4668 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 200:
#line 2306 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 4677 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 204:
#line 2320 "fe/idl.ypp" /* yacc.c:1651  */
    {
          UTL_Scope *scope = idl_global->scopes ().top_non_null ();
          UTL_Scope *root = idl_global->scopes ().bottom ();

          AST_Expression::ExprType type = (yyvsp[-3].etval);
          Identifier *name = (yyvsp[-2].idval);
          // Annotation Member Names Can't Clash with C++ keywords
          FE_Utils::original_local_name (name);
          UTL_ScopedName *scoped_name = new UTL_ScopedName (name, 0);
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
            scoped_name, FE_Declarator::FD_simple, 0);
          AST_Annotation_Member *member = idl_global->gen ()->
            create_annotation_member (
              type, declarator->compose (type_obj), declarator->name ());
          scope->fe_add_annotation_member (member);

          declarator->destroy ();
          delete declarator;

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
#line 4734 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 205:
#line 2376 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = (yyvsp[0].exval);
        }
#line 4742 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 206:
#line 2380 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.exval) = 0;
        }
#line 4750 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 207:
#line 2387 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Annotation_Appl *annotation = (yyvsp[0].annotation_val);
          if (annotation)
            {
              annotations->add (annotation);
            }
          (yyval.annotations_val) = annotations;
        }
#line 4764 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 208:
#line 2400 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Annotation_Appl *annotation = (yyvsp[0].annotation_val);
          if (annotation)
            {
              annotations->add (annotation);
            }
          (yyval.annotations_val) = annotations;
        }
#line 4778 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 209:
#line 2410 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.annotations_val) = new AST_Annotation_Appls ();
        }
#line 4786 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 210:
#line 2417 "fe/idl.ypp" /* yacc.c:1651  */
    {
          if (idl_global->idl_version_ < IDL_VERSION_4)
            {
              idl_global->err ()->idl_version_error (
                "Annotations are an IDL4 feature");
            }

          AST_Annotation_Decl *decl = 0;
          UTL_ScopedName *name = (yyvsp[0].idlist);
          AST_Annotation_Decl::escape_name (name);

          // Check for Matching Annotation Declaration
          AST_Decl *d = idl_global->scopes ().top_non_null ()->
            lookup_by_name (name);
          if (d)
            {
              decl = AST_Annotation_Decl::narrow_from_decl (d);
              if (decl)
                {
                  idl_global->scopes ().push (decl);
                }
            }
          else
            {
              Identifier &id = *name->last_component ();
              switch (idl_global->unknown_annotations_)
                {
                default:
                case IDL_GlobalData::UNKNOWN_ANNOTATIONS_WARN_ONCE:
                  if (idl_global->unknown_annotations_seen_.insert (id) == 1)
                    {
                      break;
                    }
                  idl_global->err ()->lookup_warning (name);
                  break;
                case IDL_GlobalData::UNKNOWN_ANNOTATIONS_WARN_ALL:
                  idl_global->err ()->lookup_warning (name);
                  break;
                case IDL_GlobalData::UNKNOWN_ANNOTATIONS_ERROR:
                  idl_global->err ()->lookup_error (name);
                  break;
                case IDL_GlobalData::UNKNOWN_ANNOTATIONS_IGNORE:
                  break;
                }

              // Ignore lookup errors for parameter values that might depend
              // on constants or enums from the annotation declaration.
              idl_global->ignore_lookup_errors_ = true;
            }

          (yyval.annotation_decl_val) = decl;
        }
#line 4843 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 211:
#line 2470 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->ignore_lookup_errors_ = false;
          AST_Annotation_Appl *appl = 0;
          UTL_ScopedName *name = (yyvsp[-2].idlist);
          AST_Annotation_Decl *decl = (yyvsp[-1].annotation_decl_val);
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);

          if (decl)
            {
              appl = idl_global->gen ()->create_annotation_appl (name, params);
              appl->apply_from (decl);
              idl_global->scopes ().pop ();
            }
          else
            {
              AST_Annotation_Appl::delete_params (params);
            }

          if (name)
            {
              name->destroy ();
              delete name;
            }

          (yyval.annotation_val) = appl;
        }
#line 4874 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 212:
#line 2500 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.annotation_params_val) = (yyvsp[-1].annotation_params_val);
        }
#line 4882 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 213:
#line 2504 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.annotation_params_val) = 0;
        }
#line 4890 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 214:
#line 2511 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appl::Params *params = new AST_Annotation_Appl::Params;
          AST_Annotation_Appl::Param *param = new AST_Annotation_Appl::Param;
          param->id = 0;
          param->expr = (yyvsp[0].exval);
          params->push (param);
          (yyval.annotation_params_val) = params;
        }
#line 4903 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 215:
#line 2520 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.annotation_params_val) = (yyvsp[0].annotation_params_val);
        }
#line 4911 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 216:
#line 2527 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 4921 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 217:
#line 2535 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 4931 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 218:
#line 2541 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.annotation_params_val) = new AST_Annotation_Appl::Params;
        }
#line 4939 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 219:
#line 2548 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appl::Param *param = new AST_Annotation_Appl::Param;
          param->id = (yyvsp[-2].idval);
          // Annotation Member Names Can't Clash with C++ keywords
          FE_Utils::original_local_name (param->id);
          param->expr = (yyvsp[0].exval);
          (yyval.annotation_param_val) = param;
        }
#line 4952 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 220:
#line 2560 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 4961 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 221:
#line 2565 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 4969 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 222:
#line 2569 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 4977 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 223:
#line 2573 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 4985 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 224:
#line 2577 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 4993 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 225:
#line 2581 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5023 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 226:
#line 2607 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) = 0;
        }
#line 5031 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 227:
#line 2614 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
#line 5039 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 228:
#line 2618 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5108 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 231:
#line 2691 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5119 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 233:
#line 2699 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5146 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 251:
#line 2754 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5157 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 252:
#line 2765 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5165 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 253:
#line 2769 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5189 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 254:
#line 2789 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dlval) = 0;
        }
#line 5197 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 257:
#line 2801 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5208 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 258:
#line 2812 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5216 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 259:
#line 2816 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5240 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 260:
#line 2836 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dlval) = 0;
        }
#line 5248 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 261:
#line 2843 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5265 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 262:
#line 2859 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5284 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 265:
#line 2882 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_long;
        }
#line 5292 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 266:
#line 2886 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_longlong;
        }
#line 5300 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 267:
#line 2890 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_short;
        }
#line 5308 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 268:
#line 2897 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_ulong;
        }
#line 5316 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 269:
#line 2901 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_ulonglong;
        }
#line 5324 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 270:
#line 2905 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_ushort;
        }
#line 5332 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 271:
#line 2912 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_double;
        }
#line 5340 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 272:
#line 2916 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_float;
        }
#line 5348 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 273:
#line 2920 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_longdouble;
        }
#line 5356 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 274:
#line 2927 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_fixed;
        }
#line 5364 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 275:
#line 2934 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_char;
        }
#line 5372 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 276:
#line 2938 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_wchar;
        }
#line 5380 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 277:
#line 2945 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_octet;
        }
#line 5388 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 278:
#line 2952 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_bool;
        }
#line 5396 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 279:
#line 2959 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_any;
        }
#line 5404 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 280:
#line 2966 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.etval) = AST_Expression::EV_object;
        }
#line 5412 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 281:
#line 2973 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
#line 5420 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 282:
#line 2977 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5429 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 283:
#line 2986 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5464 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 284:
#line 3017 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
#line 5472 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 285:
#line 3021 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
#line 5480 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 286:
#line 3025 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5496 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 290:
#line 3047 "fe/idl.ypp" /* yacc.c:1651  */
    {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Decls *members = (yyvsp[0].decls_val);
          if (annotations && members)
            {
              for (size_t i = 0; i < members->size (); i++)
                {
                  (*members)[i]->annotation_appls (*annotations);
                }
            }
          delete annotations;
          delete members;
        }
#line 5514 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 291:
#line 3064 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
#line 5522 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 292:
#line 3068 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
#line 5530 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 293:
#line 3072 "fe/idl.ypp" /* yacc.c:1651  */
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

                  f = idl_global->gen ()->create_field (tp, d->name ());
                  members->push_back (f);
                  (void) s->fe_add_field (f);
                }
            }

          (yyvsp[-2].dlval)->destroy ();
          delete (yyvsp[-2].dlval);
          (yyvsp[-2].dlval) = 0;

          (yyval.decls_val) = members;
        }
#line 5588 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 294:
#line 3126 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->err()->syntax_error (idl_global->parse_state ());
        }
#line 5596 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 295:
#line 3130 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 5605 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 296:
#line 3138 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
#line 5613 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 297:
#line 3142 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5622 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 298:
#line 3150 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
#line 5630 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 299:
#line 3154 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5667 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 300:
#line 3187 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
#line 5675 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 301:
#line 3191 "fe/idl.ypp" /* yacc.c:1651  */
    {
          /*
           * The top of the scopes must be an empty union we added after we
           * encountered 'union <id> switch ('. Now we are ready to add a
           * correct one. Temporarily remove the top so that we setup the
           * correct union in a right scope.
           */
          UTL_Scope *top = idl_global->scopes ().top_non_null ();
          idl_global->scopes ().pop ();
          AST_Annotation_Appls *disc_annotations = (yyvsp[-3].annotations_val);

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
                  e->disc_annotations (*disc_annotations); // Set Discriminator Annotations

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

          delete disc_annotations;
        }
#line 5738 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 302:
#line 3250 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
#line 5746 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 303:
#line 3254 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
#line 5754 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 304:
#line 3258 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5774 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 305:
#line 3277 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5785 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 306:
#line 3284 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 5802 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 307:
#line 3297 "fe/idl.ypp" /* yacc.c:1651  */
    {
          /* octets are not allowed. */
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5815 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 308:
#line 3306 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5826 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 310:
#line 3314 "fe/idl.ypp" /* yacc.c:1651  */
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
              if (!idl_global->ignore_lookup_errors_)
                {
                  return 1;
                }
            }

          (yyvsp[0].idlist)->destroy ();
          delete (yyvsp[0].idlist);
          (yyvsp[0].idlist) = 0;
        }
#line 5937 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 314:
#line 3431 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
#line 5945 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 315:
#line 3435 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
#line 5953 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 316:
#line 3439 "fe/idl.ypp" /* yacc.c:1651  */
    {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_UnionBranch *b = 0;
          AST_Annotation_Appls *annotations = (yyvsp[-3].annotations_val);
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
              b->annotation_appls (*annotations);
              (void) s->fe_add_union_branch (b);

              // f has passed its field type to the union branch,
              // but the rest still needs to be cleaned up.
              f->AST_Decl::destroy ();
              delete f;
              f = 0;
            }

          delete annotations;
        }
#line 5989 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 317:
#line 3471 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 5997 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 318:
#line 3475 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 6006 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 319:
#line 3483 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.llval),
                          UTL_LabelList ((yyvsp[-1].ulval),
                                         (yyvsp[0].llval)),
                          1);
        }
#line 6017 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 320:
#line 3493 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6039 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 321:
#line 3511 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.llval) = 0;
        }
#line 6047 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 322:
#line 3518 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
#line 6055 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 323:
#line 3522 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
#line 6068 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 324:
#line 3531 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
#line 6076 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 325:
#line 3535 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
#line 6084 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 326:
#line 3539 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      (yyvsp[-2].exval)
                                    );
        }
#line 6097 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 327:
#line 3551 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
#line 6105 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 328:
#line 3555 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6153 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 329:
#line 3602 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6179 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 330:
#line 3627 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6203 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 331:
#line 3650 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
#line 6211 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 332:
#line 3654 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6246 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 333:
#line 3685 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
#line 6254 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 334:
#line 3689 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
#line 6262 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 335:
#line 3693 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6285 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 337:
#line 3718 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
#line 6293 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 340:
#line 3727 "fe/idl.ypp" /* yacc.c:1651  */
    {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
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
                  e->enum_parent (c);
                  e->annotation_appls (*annotations);
                }

              (void) s->fe_add_enum_val (e);
            }

          delete annotations;
        }
#line 6332 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 341:
#line 3766 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
#line 6340 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 342:
#line 3770 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
#line 6348 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 343:
#line 3774 "fe/idl.ypp" /* yacc.c:1651  */
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
                  seq->base_type_annotations (*type_annotations);

                  if (!idl_global->in_typedef ()
                      && !idl_global->anon_silent ())
                    {
                      idl_global->err ()->anonymous_type_diagnostic ();
                    }
                }
            }

          delete type_annotations;
          delete ev;
          ev = 0;
          (yyval.dcval) = seq;
        }
#line 6437 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 344:
#line 3860 "fe/idl.ypp" /* yacc.c:1651  */
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
                  seq->base_type_annotations (*type_annotations);

                  if (!idl_global->in_typedef ()
                      && !idl_global->anon_silent ())
                    {
                      idl_global->err ()->anonymous_type_diagnostic ();
                    }
                }
            }

          delete type_annotations;
          (yyval.dcval) = seq;
        }
#line 6507 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 345:
#line 3929 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
#line 6520 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 346:
#line 3938 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
#line 6528 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 347:
#line 3942 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          Decl_Annotations_Pair *seq_head = new Decl_Annotations_Pair;
          seq_head->decl = (yyvsp[0].dcval);
          seq_head->annotations = (yyvsp[-1].annotations_val);
          (yyval.decl_annotations_pair_val) = seq_head;
        }
#line 6540 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 348:
#line 3953 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyvsp[-1].exval)->evaluate (AST_Expression::EK_positive_int);
          (yyval.dcval) = idl_global->gen ()->create_fixed ((yyvsp[-3].exval), (yyvsp[-1].exval));
        }
#line 6549 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 349:
#line 3962 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 6557 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 350:
#line 3966 "fe/idl.ypp" /* yacc.c:1651  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 6565 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 351:
#line 3970 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6611 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 352:
#line 4012 "fe/idl.ypp" /* yacc.c:1651  */
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

          (yyval.dcval) = tao_string_decl;
        }
#line 6638 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 353:
#line 4038 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 6646 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 354:
#line 4046 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 6654 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 355:
#line 4050 "fe/idl.ypp" /* yacc.c:1651  */
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 6662 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 356:
#line 4054 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6699 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 357:
#line 4087 "fe/idl.ypp" /* yacc.c:1651  */
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

          (yyval.dcval) = string;
        }
#line 6726 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 358:
#line 4113 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 6734 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 359:
#line 4120 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
#line 6742 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 360:
#line 4124 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayCompleted);

          AST_Array *array = 0;
          AST_Annotation_Appls *base_type_annotations = (yyvsp[-1].annotations_val);
          UTL_ExprList *array_dims = (yyvsp[0].elval);

          /*
           * Create a node representing an array.
           */
          if (array_dims)
            {
              UTL_ScopedName sn ((yyvsp[-3].idval),
                                 0);
              array =
                idl_global->gen ()->create_array (
                                        &sn,
                                        array_dims->length (),
                                        array_dims,
                                        0,
                                        0
                                      );
              array->base_type_annotations (*base_type_annotations);
              sn.destroy ();

              if (!idl_global->in_typedef ()
                  && !idl_global->anon_silent ())
                {
                  idl_global->err ()->anonymous_type_diagnostic ();
                }
            }

          array_dims->destroy ();
          delete array_dims;

          delete base_type_annotations;

          (yyval.dcval) = array;
        }
#line 6786 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 361:
#line 4167 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.elval),
                          UTL_ExprList ((yyvsp[-1].exval),
                                        (yyvsp[0].elval)),
                          1);
        }
#line 6797 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 362:
#line 4177 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6819 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 363:
#line 4195 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.elval) = 0;
        }
#line 6827 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 364:
#line 4202 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
#line 6835 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 365:
#line 4206 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
#line 6843 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 366:
#line 4210 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6901 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 369:
#line 4272 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
#line 6909 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 370:
#line 4276 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 6917 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 371:
#line 4280 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 6925 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 372:
#line 4284 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 6933 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 373:
#line 4288 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 6997 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 374:
#line 4351 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 7005 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 375:
#line 4355 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 7013 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 376:
#line 4359 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 7021 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 377:
#line 4363 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
#line 7029 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 378:
#line 4367 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7102 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 379:
#line 4439 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
#line 7110 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 380:
#line 4443 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7143 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 381:
#line 4472 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
#line 7151 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 382:
#line 4476 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
#line 7159 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 383:
#line 4480 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();
        }
#line 7171 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 384:
#line 4492 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7179 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 385:
#line 4496 "fe/idl.ypp" /* yacc.c:1651  */
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

          delete (yyvsp[-1].annotations_val);
        }
#line 7249 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 386:
#line 4562 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7257 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 387:
#line 4566 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
#line 7265 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 388:
#line 4570 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7298 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 389:
#line 4602 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_oneway;
        }
#line 7307 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 390:
#line 4607 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_idempotent;
        }
#line 7316 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 391:
#line 4612 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ofval) = AST_Operation::OP_noflags;
        }
#line 7324 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 393:
#line 4620 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
#line 7335 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 394:
#line 4630 "fe/idl.ypp" /* yacc.c:1651  */
    {
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7344 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 395:
#line 4635 "fe/idl.ypp" /* yacc.c:1651  */
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

          delete (yyvsp[-1].annotations_val);
        }
#line 7391 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 396:
#line 4678 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7399 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 397:
#line 4682 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7416 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 398:
#line 4698 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7424 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 399:
#line 4702 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7432 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 400:
#line 4706 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7440 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 401:
#line 4711 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7448 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 403:
#line 4721 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7456 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 406:
#line 4730 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7464 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 407:
#line 4734 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7472 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 408:
#line 4738 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7508 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 409:
#line 4773 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7516 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 410:
#line 4777 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7524 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 411:
#line 4781 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7532 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 412:
#line 4786 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7540 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 414:
#line 4796 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7548 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 417:
#line 4805 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7556 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 418:
#line 4809 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7564 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 419:
#line 4813 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7607 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 420:
#line 4855 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 7618 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 423:
#line 4864 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7726 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 424:
#line 4971 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dival) = AST_Argument::dir_IN;
        }
#line 7734 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 425:
#line 4975 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dival) = AST_Argument::dir_OUT;
        }
#line 7742 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 426:
#line 4979 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.dival) = AST_Argument::dir_INOUT;
        }
#line 7750 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 427:
#line 4986 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
#line 7758 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 428:
#line 4990 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
#line 7766 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 429:
#line 4995 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7775 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 430:
#line 5000 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = 0;
        }
#line 7783 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 431:
#line 5007 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
#line 7791 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 432:
#line 5011 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
#line 7799 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 433:
#line 5016 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7808 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 434:
#line 5021 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = 0;
        }
#line 7816 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 435:
#line 5028 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
#line 7824 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 436:
#line 5032 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
#line 7832 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 437:
#line 5037 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7841 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 438:
#line 5042 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.nlval) = 0;
        }
#line 7849 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 439:
#line 5049 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
#line 7857 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 440:
#line 5053 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
        }
#line 7865 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 441:
#line 5058 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (yyval.slval) = (yyvsp[-1].slval);
        }
#line 7874 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 442:
#line 5063 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.slval) = 0;
        }
#line 7882 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 443:
#line 5070 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.slval),
                          UTL_StrList ((yyvsp[-1].sval),
                                       (yyvsp[0].slval)),
                          1);
        }
#line 7893 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 444:
#line 5081 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
#line 7901 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 445:
#line 5085 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7923 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 446:
#line 5103 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.slval) = 0;
        }
#line 7931 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 447:
#line 5110 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7960 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 448:
#line 5138 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 7998 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 451:
#line 5181 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8026 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 452:
#line 5208 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8069 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 453:
#line 5247 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
#line 8077 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 454:
#line 5251 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
#line 8085 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 455:
#line 5255 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 8098 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 456:
#line 5268 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
#line 8106 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 457:
#line 5272 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 8114 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 458:
#line 5276 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8152 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 459:
#line 5313 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 8160 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 460:
#line 5317 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8168 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 461:
#line 5321 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = 0;
        }
#line 8176 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 464:
#line 5333 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 8184 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 465:
#line 5337 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8192 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 466:
#line 5341 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 8200 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 467:
#line 5345 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8208 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 468:
#line 5349 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
        }
#line 8216 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 469:
#line 5353 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8224 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 470:
#line 5357 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
        }
#line 8232 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 471:
#line 5361 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8240 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 472:
#line 5365 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
        }
#line 8248 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 473:
#line 5369 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8256 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 474:
#line 5373 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 8264 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 475:
#line 5377 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8272 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 476:
#line 5381 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 8280 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 477:
#line 5385 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8288 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 478:
#line 5391 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8379 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 479:
#line 5481 "fe/idl.ypp" /* yacc.c:1651  */
    {
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8389 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 480:
#line 5487 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8416 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 481:
#line 5512 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8523 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 482:
#line 5618 "fe/idl.ypp" /* yacc.c:1651  */
    {
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          (yyval.bval) = (yyvsp[0].bval);
        }
#line 8533 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 483:
#line 5627 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.bval) = true;
        }
#line 8541 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 484:
#line 5631 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.bval) = false;
        }
#line 8549 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 485:
#line 5637 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8623 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 486:
#line 5709 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8697 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 487:
#line 5781 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8771 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 488:
#line 5854 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8812 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 489:
#line 5891 "fe/idl.ypp" /* yacc.c:1651  */
    {
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 8823 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 490:
#line 5901 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
#line 8831 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 491:
#line 5905 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
#line 8839 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 492:
#line 5909 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 8847 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 493:
#line 5913 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
#line 8855 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 494:
#line 5917 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
#line 8863 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 495:
#line 5921 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
#line 8871 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 496:
#line 5925 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 8921 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 497:
#line 5974 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 8929 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 498:
#line 5978 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8937 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 499:
#line 5982 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = 0;
        }
#line 8945 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 500:
#line 5990 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8953 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 501:
#line 5994 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idlist) = 0;
        }
#line 8961 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 502:
#line 6001 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
#line 8969 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 503:
#line 6005 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
#line 8977 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 504:
#line 6009 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
#line 8985 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 508:
#line 6022 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
#line 8993 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 509:
#line 6026 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9001 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 510:
#line 6030 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
#line 9009 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 511:
#line 6034 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9017 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 512:
#line 6042 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9044 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 513:
#line 6065 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 9052 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 514:
#line 6069 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9074 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 515:
#line 6091 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9105 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 516:
#line 6118 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 9113 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 517:
#line 6122 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9135 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 523:
#line 6155 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9162 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 524:
#line 6183 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9189 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 525:
#line 6210 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9237 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 526:
#line 6254 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9245 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 527:
#line 6258 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9253 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 528:
#line 6262 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9266 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 529:
#line 6276 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9274 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 530:
#line 6285 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9291 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 531:
#line 6302 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9301 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 532:
#line 6311 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 9309 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 533:
#line 6315 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9342 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 534:
#line 6348 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9395 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 535:
#line 6397 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9403 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 536:
#line 6401 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9411 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 537:
#line 6405 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9424 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 538:
#line 6417 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9432 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 539:
#line 6421 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9440 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 540:
#line 6428 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_type;
        }
#line 9448 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 541:
#line 6432 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_struct;
        }
#line 9456 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 542:
#line 6436 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_union;
        }
#line 9464 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 543:
#line 6440 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_eventtype;
        }
#line 9472 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 544:
#line 6444 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_sequence;
        }
#line 9480 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 545:
#line 6448 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_interface;
        }
#line 9488 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 546:
#line 6452 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_valuetype;
        }
#line 9496 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 547:
#line 6456 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_enum;
        }
#line 9504 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 548:
#line 6460 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_except;
        }
#line 9512 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 549:
#line 6464 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.ntval) = AST_Decl::NT_const;
          t_param_const_type = (yyvsp[0].etval);
        }
#line 9521 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 550:
#line 6472 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9553 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 551:
#line 6503 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9570 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 552:
#line 6516 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.plval) = 0;
        }
#line 9578 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 553:
#line 6523 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9607 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 554:
#line 6548 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9626 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 555:
#line 6566 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.slval), UTL_StrList ((yyvsp[-1].sval), (yyvsp[0].slval)), 1);
        }
#line 9634 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 556:
#line 6573 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9653 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 557:
#line 6588 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.slval) = 0;
        }
#line 9661 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 558:
#line 6595 "fe/idl.ypp" /* yacc.c:1651  */
    {
          ACE_NEW_RETURN ((yyval.sval),
                          UTL_String ((yyvsp[0].strval), true),
                          1);
        }
#line 9671 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 559:
#line 6604 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSeen);
        }
#line 9679 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 560:
#line 6608 "fe/idl.ypp" /* yacc.c:1651  */
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

          delete (yyvsp[-1].annotations_val);
        }
#line 9703 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 561:
#line 6628 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSqSeen);
        }
#line 9711 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 562:
#line 6632 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeBodySeen);
        }
#line 9719 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 563:
#line 6636 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeQsSeen);

          // Done with this port type - pop it off the scopes stack.
          idl_global->scopes ().pop ();
        }
#line 9730 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 564:
#line 6646 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9737 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 565:
#line 6652 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9744 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 566:
#line 6655 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9751 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 567:
#line 6661 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9758 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 568:
#line 6664 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9765 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 569:
#line 6667 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9772 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 570:
#line 6670 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9779 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 571:
#line 6673 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 9787 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 572:
#line 6677 "fe/idl.ypp" /* yacc.c:1651  */
    {
        }
#line 9794 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 573:
#line 6683 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9867 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 574:
#line 6752 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 9918 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 575:
#line 6802 "fe/idl.ypp" /* yacc.c:1651  */
    {
          if ((yyvsp[0].alval) == 0)
            {
              ACE_NEW_RETURN ((yyvsp[0].alval),
                              FE_Utils::T_ARGLIST,
                              1);
            }

          delete (yyvsp[-2].annotations_val);
          (yyvsp[0].alval)->enqueue_head ((yyvsp[-1].dcval));
          (yyval.alval) = (yyvsp[0].alval);
        }
#line 9935 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 576:
#line 6818 "fe/idl.ypp" /* yacc.c:1651  */
    {
          if ((yyvsp[-3].alval) == 0)
            {
              ACE_NEW_RETURN ((yyvsp[-3].alval),
                              FE_Utils::T_ARGLIST,
                              1);
            }

          delete (yyvsp[-1].annotations_val);
          (yyvsp[-3].alval)->enqueue_tail ((yyvsp[0].dcval));
          (yyval.alval) = (yyvsp[-3].alval);
        }
#line 9952 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 577:
#line 6831 "fe/idl.ypp" /* yacc.c:1651  */
    {
          (yyval.alval) = 0;
        }
#line 9960 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 578:
#line 6838 "fe/idl.ypp" /* yacc.c:1651  */
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
#line 10020 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 580:
#line 6901 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSeen);
        }
#line 10028 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 581:
#line 6905 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorIDSeen);
        }
#line 10036 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 582:
#line 6909 "fe/idl.ypp" /* yacc.c:1651  */
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

          delete (yyvsp[-3].annotations_val);
        }
#line 10094 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 583:
#line 6966 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSqSeen);
        }
#line 10102 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 584:
#line 6970 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorBodySeen);
        }
#line 10110 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 585:
#line 6974 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorQsSeen);

          // Done with this connector - pop it off the scope stack.
          idl_global->scopes ().pop ();
        }
#line 10121 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 588:
#line 6989 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 10129 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 589:
#line 6993 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10137 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 590:
#line 6997 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 10145 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 591:
#line 7001 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10153 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 592:
#line 7005 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 10161 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 593:
#line 7009 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10169 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 594:
#line 7013 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 10177 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;

  case 595:
#line 7017 "fe/idl.ypp" /* yacc.c:1651  */
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10185 "fe/idl.tab.cpp" /* yacc.c:1651  */
    break;


#line 10189 "fe/idl.tab.cpp" /* yacc.c:1651  */
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
#line 7022 "fe/idl.ypp" /* yacc.c:1910  */

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
