/* A Bison parser, made by GNU Bison 3.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.4"

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

/* First part of user prologue.  */
#line 72 "fe/idl.ypp"

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

#line 160 "fe/idl.tab.cpp"

# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
#line 158 "fe/idl.ypp"

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

#line 336 "fe/idl.tab.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE tao_yylval;

int tao_yyparse (void);

#endif /* !YY_TAO_YY_FE_IDL_TAB_HPP_INCLUDED  */



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


#define YY_ASSERT(E) ((void) (0 && (E)))

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
#define YYLAST   1567

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  109
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  399
/* YYNRULES -- Number of rules.  */
#define YYNRULES  597
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  878

#define YYUNDEFTOK  2
#define YYMAXUTOK   342

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
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
       0,   389,   389,   392,   393,   401,   416,   422,   426,   430,
     438,   437,   447,   446,   456,   455,   465,   464,   474,   473,
     483,   482,   492,   491,   501,   500,   510,   509,   519,   518,
     528,   527,   537,   536,   546,   545,   555,   554,   564,   563,
     577,   576,   588,   627,   631,   587,   647,   655,   669,   679,
     709,   713,   654,   736,   740,   741,   745,   746,   751,   756,
     750,   842,   847,   841,   916,   920,   928,   966,   970,   927,
     987,   986,   998,  1035,  1065,  1098,  1097,  1106,  1113,  1114,
    1115,  1116,  1120,  1124,  1129,  1172,  1176,  1128,  1203,  1246,
    1250,  1201,  1267,  1265,  1305,  1304,  1316,  1320,  1327,  1332,
    1340,  1365,  1391,  1455,  1456,  1460,  1461,  1462,  1467,  1471,
    1478,  1497,  1498,  1502,  1517,  1524,  1523,  1533,  1532,  1542,
    1541,  1551,  1550,  1560,  1559,  1569,  1568,  1578,  1577,  1587,
    1586,  1599,  1611,  1609,  1634,  1641,  1651,  1650,  1676,  1674,
    1699,  1709,  1720,  1764,  1791,  1823,  1827,  1831,  1835,  1822,
    1897,  1898,  1899,  1900,  1901,  1902,  1903,  1907,  1911,  1979,
    1981,  1983,  1984,  1996,  1997,  2009,  2010,  2022,  2023,  2032,
    2044,  2045,  2054,  2066,  2067,  2076,  2085,  2097,  2098,  2107,
    2116,  2128,  2186,  2187,  2194,  2198,  2203,  2210,  2214,  2218,
    2223,  2227,  2231,  2235,  2242,  2311,  2310,  2337,  2338,  2342,
    2343,  2344,  2346,  2345,  2354,  2355,  2359,  2415,  2419,  2426,
    2439,  2449,  2457,  2456,  2541,  2545,  2552,  2561,  2568,  2576,
    2582,  2589,  2602,  2601,  2610,  2614,  2618,  2622,  2650,  2658,
    2657,  2729,  2730,  2734,  2741,  2742,  2768,  2769,  2770,  2771,
    2772,  2773,  2774,  2775,  2779,  2780,  2781,  2782,  2786,  2787,
    2788,  2792,  2793,  2797,  2809,  2807,  2832,  2839,  2840,  2844,
    2856,  2854,  2879,  2886,  2902,  2920,  2921,  2925,  2929,  2933,
    2940,  2944,  2948,  2955,  2959,  2963,  2970,  2977,  2981,  2988,
    2995,  3002,  3009,  3017,  3016,  3030,  3061,  3065,  3029,  3082,
    3085,  3086,  3090,  3108,  3112,  3107,  3170,  3169,  3182,  3181,
    3194,  3198,  3231,  3235,  3294,  3298,  3193,  3320,  3327,  3340,
    3349,  3356,  3357,  3466,  3469,  3470,  3475,  3479,  3474,  3515,
    3514,  3526,  3536,  3554,  3562,  3561,  3575,  3579,  3574,  3595,
    3594,  3645,  3670,  3694,  3698,  3729,  3733,  3693,  3757,  3762,
    3760,  3766,  3770,  3810,  3814,  3808,  3902,  3973,  3982,  3972,
    3996,  4006,  4010,  4004,  4055,  4081,  4090,  4094,  4088,  4130,
    4156,  4164,  4163,  4210,  4220,  4238,  4246,  4250,  4245,  4310,
    4311,  4316,  4320,  4324,  4328,  4315,  4387,  4391,  4395,  4399,
    4386,  4467,  4471,  4500,  4504,  4466,  4519,  4523,  4584,  4588,
    4518,  4625,  4630,  4635,  4642,  4643,  4654,  4659,  4702,  4653,
    4722,  4721,  4730,  4729,  4740,  4745,  4743,  4749,  4754,  4758,
    4753,  4797,  4796,  4805,  4804,  4815,  4820,  4818,  4824,  4829,
    4833,  4828,  4878,  4885,  4886,  4887,  4994,  4998,  5002,  5010,
    5014,  5009,  5023,  5031,  5035,  5030,  5044,  5052,  5056,  5051,
    5065,  5073,  5077,  5072,  5086,  5093,  5105,  5103,  5126,  5133,
    5161,  5198,  5199,  5203,  5232,  5271,  5275,  5231,  5292,  5296,
    5290,  5337,  5336,  5344,  5351,  5352,  5357,  5356,  5365,  5364,
    5373,  5372,  5381,  5380,  5389,  5388,  5397,  5396,  5405,  5404,
    5414,  5504,  5510,  5535,  5641,  5650,  5654,  5660,  5732,  5804,
    5878,  5877,  5925,  5929,  5933,  5937,  5941,  5945,  5924,  5998,
    5997,  6005,  6012,  6017,  6025,  6029,  6024,  6039,  6040,  6044,
    6046,  6045,  6054,  6053,  6066,  6089,  6064,  6115,  6142,  6113,
    6166,  6167,  6168,  6172,  6173,  6177,  6204,  6234,  6278,  6282,
    6232,  6297,  6306,  6324,  6335,  6334,  6372,  6421,  6425,  6370,
    6440,  6444,  6451,  6455,  6459,  6463,  6467,  6471,  6475,  6479,
    6483,  6487,  6495,  6526,  6539,  6546,  6571,  6589,  6596,  6611,
    6618,  6628,  6632,  6652,  6656,  6627,  6669,  6675,  6678,  6685,
    6684,  6691,  6690,  6697,  6696,  6706,  6775,  6825,  6841,  6854,
    6861,  6920,  6925,  6929,  6924,  6990,  6994,  6989,  7007,  7008,
    7013,  7012,  7021,  7020,  7029,  7028,  7037,  7036
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
  "state_member", "exports", "at_least_one_export", "export", "$@42",
  "$@43", "$@44", "$@45", "$@46", "$@47", "$@48", "$@49",
  "at_least_one_scoped_name", "scoped_names", "$@50", "scoped_name",
  "$@51", "$@52", "id", "defining_id", "interface_forward", "const_dcl",
  "$@53", "$@54", "$@55", "$@56", "const_type", "expression", "const_expr",
  "or_expr", "xor_expr", "and_expr", "shift_expr", "add_expr", "mult_expr",
  "unary_expr", "primary_expr", "literal", "positive_int_expr",
  "annotation_dcl", "$@57", "annotation_body", "annotation_statement",
  "$@58", "annotation_member_type", "annotation_member",
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

#define YYPACT_NINF -656

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-656)))

#define YYTABLE_NINF -539

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -656,   127,  1208,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,    83,   105,   115,   155,  -656,    83,
      83,  -656,    44,    44,  -656,  -656,    83,  -656,  -656,    98,
    -656,   285,    57,   110,  -656,  -656,    26,  -656,  -656,  -656,
    -656,  -656,  -656,   527,  -656,  -656,  -656,  -656,  -656,  1339,
     130,  -656,  -656,   131,  -656,   199,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,   147,  -656,  -656,  -656,   147,  -656,
    -656,   134,   157,  1486,    44,    83,  1462,    83,    83,    83,
      83,  -656,  -656,  -656,    53,    83,    86,  -656,   100,    83,
    -656,   147,    83,   167,   169,    83,  -656,  -656,   109,  -656,
     111,  -656,  -656,   181,  -656,   186,   189,   692,  -656,  -656,
    -656,   194,   242,  -656,   203,   206,   215,   161,  -656,    42,
    -656,  -656,  -656,  -656,  -656,  -656,   216,  -656,  -656,  -656,
    -656,  -656,  -656,   230,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,   199,  -656,  -656,  -656,   103,  -656,  -656,   229,  -656,
     238,   243,   244,  -656,    44,   249,   251,   248,  -656,   252,
     254,   255,   257,   258,   261,   262,   265,  -656,  -656,  -656,
     266,   267,  -656,  -656,  -656,  -656,   230,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,   230,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,   269,  -656,   270,  -656,  -656,
     263,  -656,   357,  -656,  -656,  -656,    50,    56,  -656,  -656,
    -656,  1486,  -656,  -656,  -656,  -656,   271,  -656,  -656,  -656,
    -656,   360,  -656,  -656,    55,   274,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,   361,  -656,   154,   279,   324,  -656,
    -656,  -656,  -656,  -656,  -656,   230,  -656,  -656,   275,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,   324,   286,
     287,  -656,  -656,  -656,    83,    83,   293,   295,  -656,  -656,
    -656,   292,  -656,   357,  -656,  -656,  -656,  -656,  -656,  -656,
     387,  -656,   305,   304,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,   188,   188,   188,   154,   230,  -656,
    -656,   303,   307,   302,   143,   137,   113,  -656,  -656,  -656,
    -656,  -656,    44,  -656,  -656,  -656,  -656,   312,  -656,    44,
    -656,   154,   154,   154,   296,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,   211,  -656,   -11,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,    44,   324,  -656,  -656,  -656,   318,   263,
      90,  1261,   320,   319,  -656,   692,  -656,  -656,  -656,   306,
     154,   154,   154,   154,   154,   154,   154,   154,   154,   154,
     321,    83,  -656,   230,   983,  -656,   554,   154,  -656,  -656,
    -656,  -656,  -656,   154,  -656,   637,  -656,  -656,  -656,   410,
     899,  -656,  -656,  -656,  -656,    41,   355,    44,    44,  -656,
    -656,  -656,  -656,  -656,    41,  -656,   326,  -656,   322,  -656,
     327,  -656,  -656,  1018,   230,  -656,    44,   324,  -656,  -656,
    -656,  -656,   330,  -656,  -656,    83,  -656,  -656,   332,   336,
     424,   339,  -656,  -656,   307,   302,   143,   137,   137,   113,
     113,  -656,  -656,  -656,  -656,  -656,   335,  -656,  -656,  -656,
     340,  -656,  -656,  1429,  -656,  -656,  -656,  -656,   676,  -656,
    -656,  -656,  -656,  -656,   341,  -656,   773,  -656,  -656,   344,
     342,   873,   346,   347,   350,   338,  -656,   337,  -656,   353,
    -656,  -656,  -656,   354,   362,   197,    44,    44,    44,  -656,
     370,  -656,  -656,  -656,  -656,  -656,  -656,  -656,    83,    83,
    -656,   376,  -656,  -656,  -656,  1103,   815,   405,  1075,  -656,
     230,   357,  -656,  -656,    68,    70,   379,   380,   384,   357,
     385,  -656,  -656,    71,  -656,    60,  -656,  -656,   386,   390,
     230,  -656,   136,  1462,  -656,   421,  -656,  -656,  -656,  -656,
      55,  -656,   389,  -656,   393,  -656,   394,   399,   400,   401,
    -656,   230,  -656,  -656,  -656,  -656,  -656,   402,   403,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,   154,  -656,
     357,  -656,   404,    83,  -656,  -656,   466,   230,  -656,  -656,
    -656,  -656,  -656,  -656,    81,    81,    81,   407,  -656,   408,
     409,   411,   412,   413,   414,  -656,  -656,  -656,   417,   419,
     420,   427,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,   154,  -656,  -656,  -656,    83,  -656,   428,   425,
     430,  -656,   432,  -656,  -656,  -656,  -656,  -656,   423,   154,
     426,  1286,  -656,    44,  -656,  -656,  -656,  -656,  -656,  -656,
     490,  -656,  -656,    66,  -656,   338,   353,  -656,  -656,   416,
    -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
     418,   418,  -656,  -656,  -656,  -656,  1075,    83,  -656,   154,
     422,  -656,  -656,  -656,  -656,  -656,   436,   437,   438,   442,
     136,  -656,  -656,  -656,    44,  -656,  -656,  -656,  -656,   439,
     230,  -656,  -656,  -656,   440,  -656,   498,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,    44,  -656,  -656,  -656,  -656,
    -656,   230,   459,  1154,  -656,   449,   418,  -656,   467,   453,
     525,   538,   538,    83,   523,   488,   475,  -656,   230,   499,
    -656,  -656,   483,  -656,  -656,  -656,  -656,  -656,   485,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,   533,   592,
     491,   212,   538,   538,   174,  1075,  -656,   502,   496,   538,
     497,   541,    83,    44,  -656,  -656,   512,  -656,  -656,  -656,
    -656,  -656,   500,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,   230,  -656,   511,  -656,   513,  1075,   570,   522,
     154,   519,   524,    54,  -656,   268,    83,   525,    44,    44,
     508,    83,   592,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,  1403,  -656,  -656,  -656,   515,   516,  -656,  -656,
    -656,   212,    83,   514,   530,  -656,  -656,  -656,  -656,    44,
    -656,  -656,  -656,  -656,    83,   536,   520,   550,  -656,  -656,
    -656,  -656,   521,   534,  -656,  -656,   555,  -656
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,     0,     3,     1,    38,   145,    40,    70,   222,
     283,   298,   333,   381,     0,     0,     0,     0,    94,     0,
       0,   492,     0,     0,   561,   582,     0,     6,     7,    42,
      24,    61,     0,     0,    22,    64,    77,    66,    26,    78,
      83,    79,    84,    77,    80,    81,    65,    18,    10,     0,
       0,    12,   228,   285,   224,   332,   225,   251,   252,   226,
      20,    14,    16,    28,   452,   451,   454,    30,   490,    32,
     522,   524,   523,   521,    77,   540,   541,   520,    77,    34,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   141,   263,   227,    77,     0,    77,    88,    77,     0,
      82,    77,     0,   458,   533,     0,   140,   136,     0,   135,
       0,   211,   211,     0,    46,     0,     0,     0,   211,     8,
       9,     0,    97,    72,     0,     0,     0,   267,   269,     0,
     273,   274,   277,   278,   279,   280,   276,   281,   282,   347,
     355,   360,    92,   235,   102,   231,   233,   234,   232,   236,
     265,   266,   237,   241,   238,   240,   239,   242,   243,   285,
     248,     0,   249,   250,   244,     0,   247,   245,   354,   246,
     359,     0,     0,     5,     0,   209,     0,     0,   300,     0,
       0,     0,     0,     0,     0,     0,     0,   534,   527,   536,
       0,     0,   585,   581,    39,   276,   158,   146,   150,   154,
     155,   151,   152,   153,   156,   157,    41,    71,   223,   229,
     284,   299,   334,   382,    73,   531,    74,     0,   532,    95,
     463,   493,     0,   449,   138,   450,     0,     0,   195,    43,
      25,     0,   547,   543,   544,   549,   546,   550,   548,   545,
     542,     0,    48,   554,     0,     0,    23,    96,    75,    67,
      27,    85,   268,   275,   270,   272,     0,     0,    99,   346,
     343,   351,   356,    19,    11,   212,    13,   286,     0,    21,
      15,    17,    29,   455,    31,   504,   491,    33,    99,     0,
       0,    35,    37,   589,     0,     0,     0,     0,    89,   461,
     459,   501,   137,     0,   562,   210,   583,   198,     4,   551,
       0,   555,     0,   552,   184,   185,   186,   188,   191,   190,
     192,   193,   189,   187,     0,     0,     0,     0,   181,   580,
     159,   160,   161,   163,   165,   167,   170,   173,   177,   182,
     579,    62,     0,   112,   104,   271,   194,     0,   348,     0,
      93,     0,     0,     0,   215,   211,   301,   465,   508,   535,
     528,   537,   586,   147,   263,   230,   256,   257,   258,   264,
     335,   383,   112,     0,    99,   499,   494,   139,     0,   463,
       0,     0,     3,     0,    49,     0,   178,   179,   180,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     577,     0,    76,   134,     0,   111,     0,     0,   211,    98,
     344,   352,   357,     0,   213,     0,   287,   291,   211,   456,
       0,   112,   104,   371,   376,     0,   486,     0,     0,   594,
     369,   370,   590,   592,     0,   596,     0,   588,     0,   211,
     253,   211,   291,     0,   462,   460,     0,    99,   563,   584,
     202,   196,     0,   204,   197,     0,   199,   205,     0,     0,
       0,     0,   553,   183,   162,   164,   166,   168,   169,   171,
     172,   174,   175,   176,   211,    63,   131,   129,   391,   392,
       0,   114,   121,     0,   115,   125,   123,   127,     0,   117,
     119,   396,   109,   108,     0,   103,     0,   105,   106,     0,
       0,     0,     0,     0,     0,   135,   216,     0,   217,   220,
     296,   293,   292,     0,   211,     0,     0,     0,     0,   476,
       0,   464,   466,   468,   470,   472,   474,   478,     0,     0,
     509,     0,   507,   510,   512,     0,     0,     0,     0,   482,
     481,     0,   485,   484,     0,     0,     0,     0,     0,     0,
       0,   587,   148,     0,   254,     0,   336,   341,   211,     0,
     500,   495,     0,     0,   200,   208,   201,    45,   556,    50,
       0,   132,     0,    69,     0,   113,     0,     0,     0,     0,
     395,   425,   422,   423,   424,   386,   394,     0,     0,   211,
      87,   110,   107,   350,   349,   345,   353,   358,     0,   214,
       0,   218,     0,     0,   288,   290,   267,   312,   307,   308,
     309,   310,   302,   311,     0,     0,     0,     0,   457,     0,
       0,     0,     0,     0,     0,   514,   517,   506,     0,     0,
       0,     0,   372,   377,   480,   575,   576,   595,   591,   593,
     483,   597,     0,   366,   362,   365,     0,   342,     0,   338,
       0,    91,     0,   573,   569,   571,   564,   568,     0,     0,
       0,     0,   578,     0,   130,   122,   116,   126,   124,   128,
       0,   118,   120,     0,   221,     0,   220,   297,   294,     0,
     489,   487,   488,   477,   467,   469,   471,   473,   475,   479,
       0,     0,   511,   513,   530,   539,     0,     0,   149,     0,
     363,   255,   337,   339,   385,   496,     0,     0,     0,     0,
     566,   203,   207,   206,     0,    56,    42,    51,    55,     0,
     133,   387,   397,   219,     0,   303,   400,   515,   518,   373,
     378,   262,   367,   364,   211,     0,   574,   570,   572,   565,
     567,    58,     0,     0,    57,     0,     0,   295,     0,     0,
       0,   432,   432,     0,   436,   259,     0,   340,   497,     0,
      52,    54,   413,   388,   398,   304,   401,   408,     0,   407,
     429,   516,   519,   374,   433,   379,   260,   368,   503,     0,
       0,     0,   432,   432,     0,     0,   403,   404,     0,   432,
       0,   440,     0,     0,   498,   560,     0,   559,   412,   426,
     427,   428,     0,   418,   419,   389,   399,   319,   326,   324,
     305,   315,   316,   323,   409,   405,   430,   375,   434,   437,
     380,   261,   502,    59,   557,   414,   415,     0,   444,     0,
       0,     0,     0,     0,   211,   321,     0,     0,     0,     0,
       0,     0,     0,   416,   420,   441,   390,   320,   327,   325,
     306,   314,     0,   322,   410,   406,     0,     0,   438,    60,
     558,     0,     0,     0,     0,   329,   317,   431,   435,     0,
     417,   421,   442,   328,     0,     0,     0,     0,   330,   318,
     439,   448,     0,   445,   443,   446,     0,   447
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -656,  -656,   331,   334,   584,  -595,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,  -583,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,   -99,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,   283,  -656,
    -656,   207,  -656,  -656,  -656,   619,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,   621,  -656,   298,  -656,  -656,  -238,  -656,
    -656,   231,  -656,  -656,  -656,  -288,  -656,  -353,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,  -325,  -656,  -656,   -22,
    -656,  -656,  -181,   -10,  -656,    16,  -656,  -656,  -656,  -656,
    -189,     9,  -227,  -656,   290,   278,   260,   -65,   -58,  -137,
     -28,  -656,  -311,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,    20,   -85,   615,  -656,  -656,  -656,  -656,     0,    85,
      17,  -656,   118,  -656,   -31,   182,  -440,  -656,  -656,  -656,
      88,  -656,  -656,  -608,   -67,  -656,  -656,    -7,  -656,   -68,
    -656,  -656,   -49,   -44,   -54,   -48,   -46,   308,  -656,   -41,
    -656,   -38,  -656,  -656,  -656,  -656,   272,   352,   213,  -656,
    -656,  -656,   -37,  -656,   -32,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,  -140,  -656,  -656,  -656,  -656,  -656,
    -119,  -656,  -656,  -656,  -656,  -656,  -656,  -656,   -40,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,   -17,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,   -75,  -656,  -656,  -656,   -70,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,    19,  -656,  -656,  -328,
    -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
      18,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,  -571,  -656,  -656,  -656,
    -656,  -656,  -116,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,  -143,  -656,  -656,  -492,  -656,  -655,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,    21,    23,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,   345,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,  -300,   297,  -261,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,  -656,   639,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,  -656,   348,  -656,  -656,
    -112,  -656,  -656,  -656,  -656,  -656,  -656,  -656,    22,  -656,
    -656,  -656,   316,  -656,  -656,   166,  -656,  -656,  -656,  -656,
    -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656,  -656
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     3,    27,    28,   172,   176,   180,   181,
     171,   179,   121,   116,   125,   182,   184,   186,   190,   191,
      82,    29,    84,    30,   115,   298,   449,    31,    32,   117,
     302,   451,   651,   732,   707,   733,   708,   709,   749,   831,
      33,   118,   391,    34,    35,   124,   333,   470,    36,    85,
      37,   142,   332,    38,    39,    40,   126,   334,   484,    41,
     217,   362,   549,    42,   258,    43,   102,   248,   340,    44,
      45,   396,   485,   486,   487,   394,   395,   471,   566,   577,
     578,   564,   568,   567,   569,   562,   392,   466,   653,   318,
     222,   293,   109,   354,    46,   472,    83,   284,   428,   632,
     197,   319,   336,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   337,    48,   297,   370,   444,   553,   445,   446,
     650,   473,    50,   295,   344,   404,   497,   498,   591,   499,
     474,    86,   208,   285,   209,   145,   146,   147,   148,    52,
     355,   430,   636,   356,   720,   745,   782,   357,   358,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    53,
      87,    54,   177,   345,   503,   406,   504,   595,   502,   593,
     714,   592,    55,    88,    56,   268,   408,   669,   738,   774,
     822,   602,   800,   823,   801,   824,   865,   819,   802,   825,
     803,   821,   820,   854,   856,   864,    57,    58,    59,    89,
     286,   431,   638,   546,   639,   724,   547,   164,   341,   492,
     165,   257,   398,   166,   167,   342,   493,   168,   169,   343,
     494,   170,   359,   429,   634,   690,   635,   689,   746,   475,
     420,   527,   686,   743,   779,   421,   528,   687,   744,   781,
     476,    90,   287,   432,   640,   477,   660,   735,   772,   818,
     478,   575,   489,   579,   736,   773,   717,   739,   740,   758,
     777,   827,   759,   775,   826,   753,   770,   771,   792,   816,
     851,   793,   817,   852,   576,   794,   761,   778,   828,   765,
     780,   829,   810,   830,   859,   836,   853,   867,   872,   873,
     876,   479,   480,    63,    64,    65,   183,   347,   510,    66,
     220,   364,   290,   363,   409,   511,   609,   610,   611,   612,
     613,   607,   614,   644,   531,   645,   424,   533,   514,   515,
     516,    67,   185,    68,   105,   291,   437,   642,   725,   768,
     366,   436,   784,   276,   348,   521,   410,   522,   618,   619,
     523,   680,   741,   524,   681,   742,    69,    70,    71,    72,
      73,   279,   411,   620,    74,    75,    76,   188,   278,    77,
     280,   412,   621,    78,   241,   242,   303,   243,   786,   814,
     787,    79,   111,   368,   552,   699,   646,   700,   647,   697,
     698,   696,   425,   245,   390,   330,    80,    81,   112,   369,
     193,   283,   426,   352,   427,   537,   538,   536,   540
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     108,   110,   159,   163,    92,   160,   161,    93,   204,   103,
     104,   162,   144,   205,   399,   198,   113,   320,    47,    51,
      60,   143,    49,    61,   419,    62,   226,   227,   691,   201,
     400,   401,   402,   244,   199,   202,   623,   203,   572,   200,
     349,   292,   299,   488,   106,   159,   163,   106,   160,   161,
     254,   255,   422,   294,   162,   797,   705,   520,   106,   296,
     529,   196,   206,   637,   143,    47,    51,    60,   706,   712,
      61,   625,    62,   626,   433,   207,  -361,   210,   211,   212,
     213,   509,   798,   799,   106,   215,    91,   762,   572,   218,
     379,   423,   219,   106,     6,   221,  -361,   440,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   195,   137,   512,
     718,     8,   367,    12,  -142,   140,   141,   795,   796,   122,
     565,     8,   107,   525,   807,   107,   435,     4,   304,   305,
     306,   307,   308,   309,   310,   311,   107,   174,   705,   312,
     313,  -143,   174,   174,  -313,   119,   122,   174,   513,   224,
     706,   224,   265,   174,   314,   315,   204,   106,   174,   316,
     317,   205,   224,   198,    18,   754,   413,   414,    95,   252,
     490,   107,   253,   488,  -144,   797,   496,   201,   633,   122,
     441,   443,   199,   202,   223,   203,   225,   200,  -100,   114,
     224,   106,   224,   122,   719,   259,   415,   260,   120,   551,
     106,   416,   798,   799,    18,   596,   128,   129,    99,   196,
     132,   133,   134,   135,   387,   388,   389,   174,   844,  -331,
      12,   178,   495,   192,   643,   383,   384,   304,   305,   306,
     307,   308,   309,   310,   311,   107,   385,   386,   312,   313,
     122,   413,   414,   123,   861,   194,   572,   789,   790,   791,
     461,   462,   463,   314,   315,  -453,   868,  -525,   316,   317,
     405,   304,   305,   306,   307,   308,   309,   310,   311,   107,
     228,   415,   312,   313,   353,   229,   416,   230,   107,   417,
     418,   187,   246,   804,   174,   187,   376,   377,   378,   -47,
     247,   -47,   249,   317,   250,   204,   798,   799,    94,    96,
     205,   214,   198,   216,   251,   -47,   -47,   256,   -47,   -47,
     393,   224,   -47,   491,    98,   101,   201,   393,   457,   458,
     261,   199,   202,   505,   203,   834,   200,   459,   460,   262,
     448,   263,   264,   320,   -47,   572,  -210,   267,   -47,   266,
     269,   434,   270,   271,   543,   272,   545,   273,   196,   274,
     624,   275,   -47,   277,   281,   282,   289,  -526,   630,   288,
     106,   664,   300,   301,   159,   163,   331,   160,   161,   335,
     338,   339,   643,   162,   501,   350,   351,   572,   722,   560,
     346,   465,   360,   143,   361,   365,   442,    47,    51,    60,
     373,    49,    61,   530,    62,   534,   535,   374,   375,   380,
     382,   403,   530,   573,   381,   320,   397,   438,   574,   665,
     -44,   450,   453,   532,   550,   464,   541,   542,   554,   405,
     556,   544,   702,   670,   671,   672,   557,   558,   559,   561,
     563,   580,   582,   588,   583,   555,   622,   598,   585,   586,
     413,   414,   587,   589,   594,   159,   163,   590,   160,   161,
     649,   599,  -289,   573,   162,   501,   571,   600,   574,   601,
     608,   506,   507,   405,   143,   603,   617,   627,   628,   143,
     415,   508,   629,   631,   252,   416,  -384,   654,   417,   418,
     641,   655,   656,   597,   604,   605,   606,   657,   658,   659,
     661,   662,   667,   711,   663,   673,   674,   675,   695,   676,
     677,   678,   679,   846,   847,   682,   571,   683,   615,   616,
     684,   701,   159,   163,   703,   160,   161,   685,   692,   693,
     694,   162,   715,   716,   726,   727,   728,   734,   737,   633,
     106,   143,   729,  -402,   866,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    10,    11,   750,
      12,   139,   140,   141,   752,   467,   755,  -393,     6,   756,
     757,     9,  -393,  -393,  -393,  -393,  -393,  -393,  -393,  -393,
    -393,  -393,  -393,  -393,    10,    11,   760,    12,   764,  -393,
    -393,    13,   766,   767,   413,   414,   468,   469,  -393,  -411,
     769,   776,   783,   838,    14,   785,   805,   788,   481,   482,
     483,   806,   808,   809,   813,   832,   815,   833,   107,   835,
     837,   573,   839,   848,   840,  -101,   574,    22,    23,   862,
     122,   857,   858,   863,   869,   871,   870,   874,   875,   371,
     877,   710,   372,   173,   751,  -393,   100,    97,   500,   545,
     106,   688,   456,   526,   -86,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    10,    11,   455,
      12,   139,   140,   141,   571,   175,   713,    47,    51,    60,
     454,   648,    61,   584,    62,   666,   763,    92,   447,   106,
     721,   668,   731,   841,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   195,   137,   138,   231,   407,   232,   581,
     573,   140,   141,   748,   548,   574,   843,   747,   860,   723,
     570,   845,   233,   234,   439,   235,   236,   189,   107,   237,
     850,   539,   730,   452,   174,   517,   652,     0,     0,     0,
       0,     0,     0,    92,     0,     0,   721,     0,     0,   842,
       0,   238,   573,     0,     0,   239,     0,   574,     0,    47,
      51,    60,     0,   571,    61,     0,    62,   107,     0,   240,
       0,   812,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,   500,   811,   106,     0,     0,     0,
       0,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    10,    11,   571,    12,   139,   140,   141,
       0,   159,   163,     0,   160,   161,   393,   393,     0,     0,
     162,   855,     0,     0,     0,     0,   467,     0,  -393,     6,
     143,   849,     9,  -393,  -393,  -393,  -393,  -393,  -393,  -393,
    -393,  -393,  -393,  -393,  -393,    10,    11,   393,    12,     0,
    -393,  -393,    13,     0,     0,   413,   414,   468,   469,  -393,
       0,     0,     0,     0,   107,    14,     0,     0,     0,   481,
     482,   483,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   106,     0,    22,    23,
       0,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,     0,     0,     0,  -393,   139,   140,   141,
     467,     0,  -393,     6,     0,  -538,     9,  -393,  -393,  -393,
    -393,  -393,  -393,  -393,  -393,  -393,  -393,  -393,  -393,    10,
      11,     0,    12,     0,  -393,  -393,    13,     0,     0,   413,
     414,   468,   469,  -393,     0,     0,     0,     0,     0,    14,
       0,     0,     0,   518,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   519,   107,     0,     0,     0,     0,     0,
     174,     0,    22,    23,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -393,     0,     0,     0,   467,     0,  -393,     6,     0,  -505,
       9,  -393,  -393,  -393,  -393,  -393,  -393,  -393,  -393,  -393,
    -393,  -393,  -393,    10,    11,     0,    12,     0,  -393,  -393,
      13,     0,     0,   413,   414,   468,   469,  -393,     0,   467,
       0,  -393,     6,    14,     0,     9,  -393,  -393,  -393,  -393,
    -393,  -393,  -393,  -393,  -393,  -393,  -393,  -393,    10,    11,
       0,    12,     0,  -393,  -393,    13,    22,    23,   413,   414,
     468,   469,  -393,     0,     0,     0,     0,     0,    14,     0,
       0,     0,     0,     0,  -393,     0,     0,     0,     0,     0,
    -211,     0,     0,   -68,     0,     0,     0,     0,   106,     0,
       0,    22,    23,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   195,   137,   138,     0,     0,     0,     0,  -393,
     140,   141,     0,     0,   467,  -211,  -393,     6,   -90,     0,
       9,  -393,  -393,  -393,  -393,  -393,  -393,  -393,  -393,  -393,
    -393,  -393,  -393,    10,    11,     0,    12,     0,  -393,  -393,
      13,     0,     0,   413,   414,   468,   469,  -393,     0,     0,
       0,     0,     0,    14,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     5,   107,     0,     6,     7,
       8,     9,     0,     0,     0,     0,    22,    23,     0,     0,
       0,     0,     0,     0,    10,    11,     0,    12,     0,     0,
       0,    13,     0,     0,  -393,     0,     0,     0,     0,     0,
    -211,     0,     0,  -529,    14,    15,    16,    17,     0,     0,
       0,     0,     0,    18,    19,     0,     0,    20,    -2,     5,
      21,     0,     6,     7,     8,     9,     0,    22,    23,     0,
       0,     0,     0,     0,    24,    25,   704,     0,    10,    11,
       0,    12,     0,     0,     0,    13,     0,     0,     0,     0,
      26,     0,     0,     0,   -53,     0,     0,     0,    14,    15,
      16,    17,     0,     0,     0,     0,     0,    18,    19,     0,
       0,    20,     5,     0,    21,     6,     7,     8,     9,     0,
       0,    22,    23,     0,     0,     0,     0,     0,    24,    25,
       0,    10,    11,     0,    12,     0,     0,     5,    13,     0,
       6,     7,     8,     9,    26,  -211,     0,     0,     0,     0,
       0,    14,    15,    16,    17,     0,    10,    11,     0,    12,
      18,    19,     0,    13,    20,     0,     0,    21,     0,     0,
       0,     0,     0,     0,    22,    23,    14,    15,    16,    17,
       0,    24,    25,     0,     0,    18,    19,     0,     0,    20,
       5,     0,    21,     6,     7,     8,     9,    26,  -211,    22,
      23,     0,     0,     0,     0,     0,    24,    25,   704,    10,
      11,     0,    12,     0,     0,     0,    13,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,     0,    14,
      15,    16,    17,     0,     0,     0,     0,     0,    18,    19,
       0,     0,    20,     0,     0,    21,     0,     0,     0,     0,
       0,     0,    22,    23,     0,     0,   106,     0,     0,    24,
      25,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    10,    11,    26,    12,   139,   140,   141,
     467,     0,  -393,     6,     0,     0,     9,  -393,  -393,  -393,
    -393,  -393,  -393,  -393,  -393,  -393,  -393,  -393,  -393,    10,
      11,     0,    12,     0,  -393,  -393,    13,     0,     0,   413,
     414,   468,   469,  -393,     0,   106,     0,     0,     0,    14,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,    10,    11,   107,    12,   139,   140,   141,   106,
     174,     0,    22,    23,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   195,     0,     0,     0,     0,     0,     0,
    -393,   140,   141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   107,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   107
};

static const yytype_int16 yycheck[] =
{
      22,    23,    43,    43,    14,    43,    43,    14,    83,    19,
      20,    43,    43,    83,   339,    83,    26,   244,     2,     2,
       2,    43,     2,     2,   352,     2,   111,   112,   636,    83,
     341,   342,   343,   118,    83,    83,   528,    83,   478,    83,
     278,   222,   231,   396,     3,    86,    86,     3,    86,    86,
       8,     9,   352,     3,    86,     1,   651,   410,     3,     3,
      19,    83,    84,     3,    86,    49,    49,    49,   651,     3,
      49,     3,    49,     3,   362,    85,    87,    87,    88,    89,
      90,   409,    28,    29,     3,    95,     3,   742,   528,    99,
     317,   352,   102,     3,     4,   105,   107,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,   409,
     681,     6,   293,    23,    88,    25,    26,   772,   773,    93,
     473,     6,    81,   411,   779,    81,   364,     0,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    87,   733,    84,
      85,    88,    87,    87,    90,    88,    93,    87,   409,    81,
     733,    81,   174,    87,    99,   100,   231,     3,    87,   104,
     105,   231,    81,   231,    49,   736,    30,    31,    53,     8,
     397,    81,    11,   526,    88,     1,   403,   231,   107,    93,
      90,   370,   231,   231,    75,   231,    75,   231,    88,    91,
      81,     3,    81,    93,   686,    92,    60,    94,    88,   437,
       3,    65,    28,    29,    49,     8,     9,    10,    53,   231,
      13,    14,    15,    16,   101,   102,   103,    87,   826,    88,
      23,    22,   403,    89,   552,    82,    83,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    99,   100,    84,    85,
      93,    30,    31,    36,   852,    88,   686,    35,    36,    37,
     387,   388,   389,    99,   100,    88,   864,    88,   104,   105,
     345,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      89,    60,    84,    85,   284,    89,    65,    88,    81,    68,
      69,    74,    88,   775,    87,    78,   314,   315,   316,     4,
      48,     6,    89,   105,    88,   370,    28,    29,    15,    16,
     370,    94,   370,    96,    89,    20,    21,    91,    23,    24,
     332,    81,    27,   398,    16,    17,   370,   339,   383,   384,
      91,   370,   370,   408,   370,   817,   370,   385,   386,    91,
     370,    88,    88,   560,    49,   775,    87,    89,    53,    88,
      88,   363,    88,    88,   429,    88,   431,    89,   370,    88,
     531,    89,    67,    88,    88,    88,    93,    88,   539,    89,
       3,   588,    91,     3,   405,   405,    92,   405,   405,     8,
      91,    47,   700,   405,   405,    89,    89,   817,   689,   464,
     105,   391,    89,   405,    89,    93,   370,   371,   371,   371,
       3,   371,   371,   415,   371,   417,   418,    92,    94,    96,
      98,   105,   424,   478,    97,   632,    94,    89,   478,   590,
      90,    92,   106,    58,   436,    94,    90,    95,    88,   504,
      88,    94,   649,   604,   605,   606,    90,     3,    89,    94,
      90,    90,    88,    95,    92,   445,    31,   505,    92,    92,
      30,    31,    92,   106,    90,   486,   486,    94,   486,   486,
      29,   505,    90,   528,   486,   486,   478,   505,   528,   505,
      90,    51,    52,   548,   486,   505,    90,    88,    88,   491,
      60,    61,    88,    88,     8,    65,    90,    88,    68,    69,
      90,    88,    88,   505,   506,   507,   508,    88,    88,    88,
      88,    88,    88,     3,   579,    88,    88,    88,    66,    88,
      88,    88,    88,   828,   829,    88,   528,    88,   518,   519,
      90,    88,   553,   553,    88,   553,   553,    90,    90,    94,
      90,   553,   106,   105,    88,    88,    88,    88,    88,   107,
       3,   553,    90,    35,   859,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    90,
      23,    24,    25,    26,   105,     1,    89,     3,     4,   106,
      35,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    38,    23,    55,    25,
      26,    27,    94,   108,    30,    31,    32,    33,    34,   106,
      91,   106,    59,   820,    40,     3,    94,   106,    44,    45,
      46,   105,   105,    62,    92,    94,   106,    94,    81,    39,
      88,   686,    93,   105,    90,    88,   686,    63,    64,   105,
      93,   106,   106,    93,    88,    75,   106,   106,    94,   298,
      75,   653,   298,    49,   733,    81,    17,    16,     1,   724,
       3,   632,   382,   412,    90,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,   381,
      23,    24,    25,    26,   686,    50,   666,   651,   651,   651,
     380,   553,   651,   491,   651,   590,   743,   687,   370,     3,
     687,   593,   704,   823,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,     4,   345,     6,   486,
     775,    25,    26,   725,   432,   775,   825,   724,   851,   690,
      34,   827,    20,    21,   369,    23,    24,    78,    81,    27,
     832,   424,   700,   375,    87,   409,   560,    -1,    -1,    -1,
      -1,    -1,    -1,   743,    -1,    -1,   743,    -1,    -1,   824,
      -1,    49,   817,    -1,    -1,    53,    -1,   817,    -1,   733,
     733,   733,    -1,   775,   733,    -1,   733,    81,    -1,    67,
      -1,   783,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   782,    -1,     1,   782,     3,    -1,    -1,    -1,
      -1,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,   817,    23,    24,    25,    26,
      -1,   842,   842,    -1,   842,   842,   828,   829,    -1,    -1,
     842,   842,    -1,    -1,    -1,    -1,     1,    -1,     3,     4,
     842,   831,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   859,    23,    -1,
      25,    26,    27,    -1,    -1,    30,    31,    32,    33,    34,
      -1,    -1,    -1,    -1,    81,    40,    -1,    -1,    -1,    44,
      45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    63,    64,
      -1,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    -1,    -1,    -1,    81,    24,    25,    26,
       1,    -1,     3,     4,    -1,    90,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    -1,    25,    26,    27,    -1,    -1,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    54,    81,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    -1,    -1,    -1,     1,    -1,     3,     4,    -1,    90,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    23,    -1,    25,    26,
      27,    -1,    -1,    30,    31,    32,    33,    34,    -1,     1,
      -1,     3,     4,    40,    -1,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      -1,    23,    -1,    25,    26,    27,    63,    64,    30,    31,
      32,    33,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    90,    -1,    -1,    -1,    -1,     3,    -1,
      -1,    63,    64,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,    81,
      25,    26,    -1,    -1,     1,    87,     3,     4,    90,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    -1,    23,    -1,    25,    26,
      27,    -1,    -1,    30,    31,    32,    33,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    81,    -1,     4,     5,
       6,     7,    -1,    -1,    -1,    -1,    63,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    -1,    23,    -1,    -1,
      -1,    27,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    90,    40,    41,    42,    43,    -1,    -1,
      -1,    -1,    -1,    49,    50,    -1,    -1,    53,     0,     1,
      56,    -1,     4,     5,     6,     7,    -1,    63,    64,    -1,
      -1,    -1,    -1,    -1,    70,    71,    72,    -1,    20,    21,
      -1,    23,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,
      86,    -1,    -1,    -1,    90,    -1,    -1,    -1,    40,    41,
      42,    43,    -1,    -1,    -1,    -1,    -1,    49,    50,    -1,
      -1,    53,     1,    -1,    56,     4,     5,     6,     7,    -1,
      -1,    63,    64,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    20,    21,    -1,    23,    -1,    -1,     1,    27,    -1,
       4,     5,     6,     7,    86,    87,    -1,    -1,    -1,    -1,
      -1,    40,    41,    42,    43,    -1,    20,    21,    -1,    23,
      49,    50,    -1,    27,    53,    -1,    -1,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    40,    41,    42,    43,
      -1,    70,    71,    -1,    -1,    49,    50,    -1,    -1,    53,
       1,    -1,    56,     4,     5,     6,     7,    86,    87,    63,
      64,    -1,    -1,    -1,    -1,    -1,    70,    71,    72,    20,
      21,    -1,    23,    -1,    -1,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    40,
      41,    42,    43,    -1,    -1,    -1,    -1,    -1,    49,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,     3,    -1,    -1,    70,
      71,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    86,    23,    24,    25,    26,
       1,    -1,     3,     4,    -1,    -1,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    -1,    23,    -1,    25,    26,    27,    -1,    -1,    30,
      31,    32,    33,    34,    -1,     3,    -1,    -1,    -1,    40,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    81,    23,    24,    25,    26,     3,
      87,    -1,    63,    64,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    -1,    -1,    -1,    -1,    -1,    -1,
      81,    25,    26,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    -1,    -1,    -1,    -1,    -1,    -1,
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
     164,   168,   172,   174,   178,   179,   203,   204,   222,   230,
     231,   239,   248,   268,   270,   281,   283,   305,   306,   307,
     349,   400,   401,   402,   403,   404,   408,   430,   432,   455,
     456,   457,   458,   459,   463,   464,   465,   468,   472,   480,
     495,   496,   129,   205,   131,   158,   240,   269,   282,   308,
     350,     3,   202,   256,   157,    53,   157,   172,   174,    53,
     164,   174,   175,   202,   202,   433,     3,    81,   198,   201,
     198,   481,   497,   202,    91,   133,   122,   138,   150,    88,
      88,   121,    93,   160,   154,   123,   165,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    24,
      25,    26,   160,   198,   243,   244,   245,   246,   247,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     270,   281,   283,   307,   316,   319,   322,   323,   326,   327,
     330,   119,   115,   113,    87,   232,   116,   271,    22,   120,
     117,   118,   124,   405,   125,   431,   126,   160,   466,   466,
     127,   128,    89,   499,    88,    17,   198,   209,   258,   261,
     262,   263,   264,   265,   323,   327,   198,   202,   241,   243,
     202,   202,   202,   202,   160,   202,   160,   169,   202,   202,
     409,   202,   199,    75,    81,    75,   231,   231,    89,    89,
      88,     4,     6,    20,    21,    23,    24,    27,    49,    53,
      67,   473,   474,   476,   231,   492,    88,    48,   176,    89,
      88,    89,     8,    11,     8,     9,    91,   320,   173,    92,
      94,    91,    91,    88,    88,   198,    88,    89,   284,    88,
      88,    88,    88,    89,    88,    89,   442,    88,   467,   460,
     469,    88,    88,   500,   206,   242,   309,   351,    89,    93,
     411,   434,   201,   200,     3,   232,     3,   223,   134,   209,
      91,     3,   139,   475,    73,    74,    75,    76,    77,    78,
      79,    80,    84,    85,    99,   100,   104,   105,   198,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     494,    92,   161,   155,   166,     8,   211,   221,    91,    47,
     177,   317,   324,   328,   233,   272,   105,   406,   443,   177,
      89,    89,   502,   202,   202,   249,   252,   256,   257,   331,
      89,    89,   170,   412,   410,    93,   439,   201,   482,   498,
     224,   111,   112,     3,    92,    94,   219,   219,   219,   211,
      96,    97,    98,    82,    83,    99,   100,   101,   102,   103,
     493,   151,   195,   198,   184,   185,   180,    94,   321,   195,
     221,   221,   221,   105,   234,   231,   274,   276,   285,   413,
     445,   461,   470,    30,    31,    60,    65,    68,    69,   338,
     339,   344,   422,   424,   425,   491,   501,   503,   207,   332,
     250,   310,   352,   184,   198,   177,   440,   435,    89,   411,
       7,    90,   204,   209,   225,   227,   228,   266,   307,   135,
      92,   140,   476,   106,   213,   214,   215,   216,   216,   217,
     217,   218,   218,   218,    94,   202,   196,     1,    32,    33,
     156,   186,   204,   230,   239,   338,   349,   354,   359,   400,
     401,    44,    45,    46,   167,   181,   182,   183,   186,   361,
     211,   231,   318,   325,   329,   201,   211,   235,   236,   238,
       1,   243,   277,   273,   275,   231,    51,    52,    61,   338,
     407,   414,   422,   424,   427,   428,   429,   491,    44,    54,
     186,   444,   446,   449,   452,   184,   180,   340,   345,    19,
     198,   423,    58,   426,   198,   198,   506,   504,   505,   423,
     507,    90,    95,   231,    94,   231,   312,   315,   275,   171,
     198,   177,   483,   226,    88,   202,    88,    90,     3,    89,
     231,    94,   194,    90,   190,   186,   187,   192,   191,   193,
      34,   198,   245,   323,   327,   360,   383,   188,   189,   362,
      90,   277,    88,    92,   244,    92,    92,    92,    95,   106,
      94,   237,   280,   278,    90,   276,     8,   198,   258,   263,
     264,   265,   290,   307,   198,   198,   198,   420,    90,   415,
     416,   417,   418,   419,   421,   202,   202,    90,   447,   448,
     462,   471,    31,   383,   201,     3,     3,    88,    88,    88,
     201,    88,   208,   107,   333,   335,   251,     3,   311,   313,
     353,    90,   436,   338,   422,   424,   485,   487,   241,    29,
     229,   141,   494,   197,    88,    88,    88,    88,    88,    88,
     355,    88,    88,   231,   211,   201,   238,    88,   249,   286,
     201,   201,   201,    88,    88,    88,    88,    88,    88,    88,
     450,   453,    88,    88,    90,    90,   341,   346,   210,   336,
     334,   252,    90,    94,    90,    66,   490,   488,   489,   484,
     486,    88,   211,    88,    72,   114,   130,   143,   145,   146,
     198,     3,     3,   237,   279,   106,   105,   365,   365,   383,
     253,   256,   221,   335,   314,   437,    88,    88,    88,    90,
     487,   198,   142,   144,    88,   356,   363,    88,   287,   366,
     367,   451,   454,   342,   347,   254,   337,   315,   198,   147,
      90,   145,   105,   374,   365,    89,   106,    35,   368,   371,
      38,   385,   385,   253,    55,   388,    94,   108,   438,    91,
     375,   376,   357,   364,   288,   372,   106,   369,   386,   343,
     389,   348,   255,    59,   441,     3,   477,   479,   106,    35,
      36,    37,   377,   380,   384,   385,   385,     1,    28,    29,
     291,   293,   297,   299,   383,    94,   105,   385,   105,    62,
     391,   256,   198,    92,   478,   106,   378,   381,   358,   296,
     301,   300,   289,   292,   294,   298,   373,   370,   387,   390,
     392,   148,    94,    94,   383,    39,   394,    88,   211,    93,
      90,   293,   231,   299,   252,   371,   195,   195,   105,   202,
     479,   379,   382,   395,   302,   243,   303,   106,   106,   393,
     380,   252,   105,    93,   304,   295,   195,   396,   252,    88,
     106,    75,   397,   398,   106,    94,   399,    75
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
     183,   184,   184,   185,   185,   187,   186,   188,   186,   189,
     186,   190,   186,   191,   186,   192,   186,   193,   186,   194,
     186,   195,   197,   196,   196,   198,   199,   198,   200,   198,
     201,   202,   203,   203,   203,   205,   206,   207,   208,   204,
     209,   209,   209,   209,   209,   209,   209,   209,   209,   210,
     211,   212,   212,   213,   213,   214,   214,   215,   215,   215,
     216,   216,   216,   217,   217,   217,   217,   218,   218,   218,
     218,   219,   219,   219,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   221,   223,   222,   224,   224,   225,
     225,   225,   226,   225,   227,   227,   228,   229,   229,   230,
     231,   231,   233,   232,   234,   234,   235,   235,   236,   237,
     237,   238,   240,   239,   239,   239,   239,   239,   239,   242,
     241,   243,   243,   244,   244,   244,   245,   245,   245,   245,
     245,   245,   245,   245,   246,   246,   246,   246,   247,   247,
     247,   248,   248,   249,   251,   250,   250,   252,   252,   253,
     255,   254,   254,   256,   257,   258,   258,   259,   259,   259,
     260,   260,   260,   261,   261,   261,   262,   263,   263,   264,
     265,   266,   267,   269,   268,   271,   272,   273,   270,   274,
     275,   275,   276,   278,   279,   277,   280,   277,   282,   281,
     284,   285,   286,   287,   288,   289,   283,   290,   290,   290,
     290,   290,   290,   291,   292,   292,   294,   295,   293,   296,
     293,   297,   298,   298,   300,   299,   301,   302,   299,   304,
     303,   305,   306,   308,   309,   310,   311,   307,   312,   314,
     313,   313,   315,   317,   318,   316,   316,   320,   321,   319,
     322,   324,   325,   323,   323,   326,   328,   329,   327,   327,
     330,   332,   331,   333,   334,   334,   336,   337,   335,   338,
     338,   340,   341,   342,   343,   339,   345,   346,   347,   348,
     344,   350,   351,   352,   353,   349,   355,   356,   357,   358,
     354,   359,   359,   359,   360,   360,   362,   363,   364,   361,
     366,   365,   367,   365,   368,   370,   369,   369,   372,   373,
     371,   375,   374,   376,   374,   377,   379,   378,   378,   381,
     382,   380,   383,   383,   383,   383,   384,   384,   384,   386,
     387,   385,   385,   389,   390,   388,   388,   392,   393,   391,
     391,   395,   396,   394,   394,   397,   399,   398,   398,   400,
     401,   402,   402,   403,   405,   406,   407,   404,   409,   410,
     408,   412,   411,   411,   413,   413,   415,   414,   416,   414,
     417,   414,   418,   414,   419,   414,   420,   414,   421,   414,
     422,   423,   423,   424,   425,   426,   426,   427,   428,   429,
     431,   430,   433,   434,   435,   436,   437,   438,   432,   440,
     439,   439,   441,   441,   443,   444,   442,   445,   445,   446,
     447,   446,   448,   446,   450,   451,   449,   453,   454,   452,
     455,   455,   455,   456,   456,   457,   458,   460,   461,   462,
     459,   463,   464,   465,   467,   466,   469,   470,   471,   468,
     472,   472,   473,   473,   473,   473,   473,   473,   473,   473,
     473,   473,   474,   475,   475,   476,   476,   477,   478,   478,
     479,   481,   482,   483,   484,   480,   485,   486,   486,   488,
     487,   489,   487,   490,   487,   491,   491,   492,   493,   493,
     494,   495,   497,   498,   496,   500,   501,   499,   502,   502,
     504,   503,   505,   503,   506,   503,   507,   503
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
       2,     1,     0,     3,     2,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     2,     0,     4,     0,     1,     0,     3,     0,     4,
       1,     1,     1,     2,     2,     0,     0,     0,     0,     9,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     3,     1,     3,     1,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     1,     2,     2,
       2,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     6,     2,     0,     1,
       2,     2,     0,     4,     1,     1,     4,     2,     0,     2,
       2,     0,     0,     4,     3,     0,     1,     1,     2,     3,
       0,     3,     0,     3,     1,     1,     1,     2,     1,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     0,     4,     0,     1,     1,     2,
       0,     4,     0,     1,     1,     1,     1,     1,     2,     1,
       2,     3,     2,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     0,     3,     0,     0,     0,     7,     2,
       2,     0,     2,     0,     0,     5,     0,     3,     0,     3,
       0,     0,     0,     0,     0,     0,    15,     1,     1,     1,
       1,     1,     1,     2,     2,     0,     0,     0,     6,     0,
       3,     2,     2,     0,     0,     3,     0,     0,     5,     0,
       3,     1,     1,     0,     0,     0,     0,     9,     2,     0,
       4,     0,     2,     0,     0,     6,     2,     0,     0,     6,
       6,     0,     0,     6,     1,     1,     0,     0,     6,     1,
       1,     0,     4,     2,     2,     0,     0,     0,     5,     1,
       1,     0,     0,     0,     0,     9,     0,     0,     0,     0,
       9,     0,     0,     0,     0,     9,     0,     0,     0,     0,
      10,     1,     1,     0,     1,     1,     0,     0,     0,     8,
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
       1,     2,     2,     3,     0,     2,     5,     2,     3,     0,
       1,     0,     0,     0,     0,    10,     2,     2,     0,     0,
       3,     0,     3,     0,     3,     3,     3,     3,     4,     0,
       1,     2,     0,     0,     6,     0,     0,     5,     2,     0,
       0,     3,     0,     3,     0,     3,     0,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
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
                       &yyvsp[(yyi + 1) - (yynrhs)]
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
            else
              goto append;

          append:
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

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
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
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
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
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
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
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
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
# else /* defined YYSTACK_RELOCATE */
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
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
| yyreduce -- do a reduction.  |
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
#line 402 "fe/idl.ypp"
    {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Decl *d = (yyvsp[0].dcval);
          if (d)
            {
              d->annotation_appls (*annotations);
            }
          else
            {
              idl_global->err ()-> unsupported_warning(
                "Annotating this is not supported");
            }
          delete annotations;
        }
#line 2382 "fe/idl.tab.cpp"
    break;

  case 6:
#line 417 "fe/idl.ypp"
    {
        }
#line 2389 "fe/idl.tab.cpp"
    break;

  case 7:
#line 423 "fe/idl.ypp"
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 2397 "fe/idl.tab.cpp"
    break;

  case 8:
#line 427 "fe/idl.ypp"
    {
          (yyval.dcval) = 0;
        }
#line 2405 "fe/idl.tab.cpp"
    break;

  case 9:
#line 431 "fe/idl.ypp"
    {
          (yyval.dcval) = 0;
        }
#line 2413 "fe/idl.tab.cpp"
    break;

  case 10:
#line 438 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AnnotationDeclSeen);
        }
#line 2421 "fe/idl.tab.cpp"
    break;

  case 11:
#line 442 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2430 "fe/idl.tab.cpp"
    break;

  case 12:
#line 447 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 2438 "fe/idl.tab.cpp"
    break;

  case 13:
#line 451 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 2447 "fe/idl.tab.cpp"
    break;

  case 14:
#line 456 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 2455 "fe/idl.tab.cpp"
    break;

  case 15:
#line 460 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2464 "fe/idl.tab.cpp"
    break;

  case 16:
#line 465 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 2472 "fe/idl.tab.cpp"
    break;

  case 17:
#line 469 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2481 "fe/idl.tab.cpp"
    break;

  case 18:
#line 474 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 2489 "fe/idl.tab.cpp"
    break;

  case 19:
#line 478 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 2498 "fe/idl.tab.cpp"
    break;

  case 20:
#line 483 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 2506 "fe/idl.tab.cpp"
    break;

  case 21:
#line 487 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2515 "fe/idl.tab.cpp"
    break;

  case 22:
#line 492 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceDeclSeen);
        }
#line 2523 "fe/idl.tab.cpp"
    break;

  case 23:
#line 496 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 2532 "fe/idl.tab.cpp"
    break;

  case 24:
#line 501 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
        }
#line 2540 "fe/idl.tab.cpp"
    break;

  case 25:
#line 505 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 2549 "fe/idl.tab.cpp"
    break;

  case 26:
#line 510 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeDeclSeen);
        }
#line 2557 "fe/idl.tab.cpp"
    break;

  case 27:
#line 514 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2566 "fe/idl.tab.cpp"
    break;

  case 28:
#line 519 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentDeclSeen);
        }
#line 2574 "fe/idl.tab.cpp"
    break;

  case 29:
#line 523 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2583 "fe/idl.tab.cpp"
    break;

  case 30:
#line 528 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeDeclSeen);
        }
#line 2591 "fe/idl.tab.cpp"
    break;

  case 31:
#line 532 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2600 "fe/idl.tab.cpp"
    break;

  case 32:
#line 537 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventDeclSeen);
        }
#line 2608 "fe/idl.tab.cpp"
    break;

  case 33:
#line 541 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2617 "fe/idl.tab.cpp"
    break;

  case 34:
#line 546 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeDeclSeen);
        }
#line 2625 "fe/idl.tab.cpp"
    break;

  case 35:
#line 550 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2634 "fe/idl.tab.cpp"
    break;

  case 36:
#line 555 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorDeclSeen);
        }
#line 2642 "fe/idl.tab.cpp"
    break;

  case 37:
#line 559 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 2651 "fe/idl.tab.cpp"
    break;

  case 38:
#line 564 "fe/idl.ypp"
    {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 2659 "fe/idl.tab.cpp"
    break;

  case 39:
#line 568 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
          (yyval.dcval) = 0;
        }
#line 2669 "fe/idl.tab.cpp"
    break;

  case 40:
#line 577 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
        }
#line 2677 "fe/idl.tab.cpp"
    break;

  case 41:
#line 581 "fe/idl.ypp"
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 2685 "fe/idl.tab.cpp"
    break;

  case 42:
#line 588 "fe/idl.ypp"
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
#line 2728 "fe/idl.tab.cpp"
    break;

  case 43:
#line 627 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
        }
#line 2736 "fe/idl.tab.cpp"
    break;

  case 44:
#line 631 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
        }
#line 2744 "fe/idl.tab.cpp"
    break;

  case 45:
#line 635 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
          /*
           * Finished with this module - pop it from the scope stack.
           */

          idl_global->scopes ().pop ();
          (yyval.dcval) = (yyvsp[-5].dcval);
        }
#line 2758 "fe/idl.tab.cpp"
    break;

  case 46:
#line 648 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleIDSeen);
        }
#line 2766 "fe/idl.tab.cpp"
    break;

  case 47:
#line 655 "fe/idl.ypp"
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
#line 2784 "fe/idl.tab.cpp"
    break;

  case 48:
#line 669 "fe/idl.ypp"
    {
          if (FE_Utils::duplicate_param_id ((yyvsp[0].plval)))
            {
              idl_global->err ()->duplicate_param_id (
                (yyvsp[-2].idlist));

              return 1;
            }
        }
#line 2798 "fe/idl.tab.cpp"
    break;

  case 49:
#line 679 "fe/idl.ypp"
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
#line 2832 "fe/idl.tab.cpp"
    break;

  case 50:
#line 709 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleSqSeen);
        }
#line 2840 "fe/idl.tab.cpp"
    break;

  case 51:
#line 713 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TmplModuleBodySeen);
        }
#line 2848 "fe/idl.tab.cpp"
    break;

  case 52:
#line 717 "fe/idl.ypp"
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
#line 2869 "fe/idl.tab.cpp"
    break;

  case 58:
#line 751 "fe/idl.ypp"
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefSeen);
        }
#line 2878 "fe/idl.tab.cpp"
    break;

  case 59:
#line 756 "fe/idl.ypp"
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_ModuleRefParamsSeen);
        }
#line 2887 "fe/idl.tab.cpp"
    break;

  case 60:
#line 761 "fe/idl.ypp"
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
#line 2969 "fe/idl.tab.cpp"
    break;

  case 61:
#line 842 "fe/idl.ypp"
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleSeen);
        }
#line 2978 "fe/idl.tab.cpp"
    break;

  case 62:
#line 847 "fe/idl.ypp"
    {
          idl_global->set_parse_state (
            IDL_GlobalData::PS_InstModuleArgsSeen);
        }
#line 2987 "fe/idl.tab.cpp"
    break;

  case 63:
#line 852 "fe/idl.ypp"
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
#line 3053 "fe/idl.tab.cpp"
    break;

  case 64:
#line 917 "fe/idl.ypp"
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 3061 "fe/idl.tab.cpp"
    break;

  case 65:
#line 921 "fe/idl.ypp"
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 3069 "fe/idl.tab.cpp"
    break;

  case 66:
#line 928 "fe/idl.ypp"
    {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Interface *i = 0;

          FE_InterfaceHeader *header = (yyvsp[0].ihval);
          (yyvsp[0].ihval) = 0;

          /*
           * Make a new interface node and add it to its enclosing scope.
           */
          if (s != 0 && header)
            {
              i = idl_global->gen ()->create_interface (
                header->name (),
                header->inherits (),
                header->n_inherits (),
                header->inherits_flat (),
                header->n_inherits_flat (),
                header->is_local (),
                header->is_abstract ()
              );
              AST_Interface::fwd_redefinition_helper (i, s);
              /*
               * Add the interface to its definition scope.
               */
              s->fe_add_interface (i);

              // This FE_InterfaceHeader class isn't destroyed with the AST.
              header->destroy ();
              delete header;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (i);
        }
#line 3111 "fe/idl.tab.cpp"
    break;

  case 67:
#line 966 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
#line 3119 "fe/idl.tab.cpp"
    break;

  case 68:
#line 970 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
#line 3127 "fe/idl.tab.cpp"
    break;

  case 69:
#line 974 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          (yyval.dcval) = dynamic_cast<AST_Decl*> (idl_global->scopes ().top_non_null ());
          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
#line 3141 "fe/idl.tab.cpp"
    break;

  case 70:
#line 987 "fe/idl.ypp"
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
#line 3149 "fe/idl.tab.cpp"
    break;

  case 71:
#line 991 "fe/idl.ypp"
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           (yyval.idval) = (yyvsp[0].idval);
         }
#line 3158 "fe/idl.tab.cpp"
    break;

  case 72:
#line 999 "fe/idl.ypp"
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
#line 3198 "fe/idl.tab.cpp"
    break;

  case 73:
#line 1036 "fe/idl.ypp"
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
#line 3231 "fe/idl.tab.cpp"
    break;

  case 74:
#line 1066 "fe/idl.ypp"
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
#line 3264 "fe/idl.tab.cpp"
    break;

  case 75:
#line 1098 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 3272 "fe/idl.tab.cpp"
    break;

  case 76:
#line 1102 "fe/idl.ypp"
    {
          (yyvsp[0].nlval)->truncatable ((yyvsp[-2].bval));
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3281 "fe/idl.tab.cpp"
    break;

  case 77:
#line 1107 "fe/idl.ypp"
    {
          (yyval.nlval) = 0;
        }
#line 3289 "fe/idl.tab.cpp"
    break;

  case 82:
#line 1121 "fe/idl.ypp"
    {
          idl_global->err ()->unsupported_error ("custom is not supported");
        }
#line 3297 "fe/idl.tab.cpp"
    break;

  case 84:
#line 1129 "fe/idl.ypp"
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
#line 3344 "fe/idl.tab.cpp"
    break;

  case 85:
#line 1172 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3352 "fe/idl.tab.cpp"
    break;

  case 86:
#line 1176 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3360 "fe/idl.tab.cpp"
    break;

  case 87:
#line 1180 "fe/idl.ypp"
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
#line 3383 "fe/idl.tab.cpp"
    break;

  case 88:
#line 1203 "fe/idl.ypp"
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
#line 3430 "fe/idl.tab.cpp"
    break;

  case 89:
#line 1246 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
#line 3438 "fe/idl.tab.cpp"
    break;

  case 90:
#line 1250 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
#line 3446 "fe/idl.tab.cpp"
    break;

  case 91:
#line 1254 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 3459 "fe/idl.tab.cpp"
    break;

  case 92:
#line 1267 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 3467 "fe/idl.tab.cpp"
    break;

  case 93:
#line 1271 "fe/idl.ypp"
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
#line 3502 "fe/idl.tab.cpp"
    break;

  case 94:
#line 1305 "fe/idl.ypp"
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
#line 3510 "fe/idl.tab.cpp"
    break;

  case 95:
#line 1309 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 3519 "fe/idl.tab.cpp"
    break;

  case 96:
#line 1317 "fe/idl.ypp"
    {
          (yyval.bval) = true;
        }
#line 3527 "fe/idl.tab.cpp"
    break;

  case 97:
#line 1321 "fe/idl.ypp"
    {
          (yyval.bval) = false;
        }
#line 3535 "fe/idl.tab.cpp"
    break;

  case 98:
#line 1329 "fe/idl.ypp"
    {
          (yyval.nlval) = (yyvsp[0].nlval);
        }
#line 3543 "fe/idl.tab.cpp"
    break;

  case 99:
#line 1333 "fe/idl.ypp"
    {
/*      |    EMPTY */
          (yyval.nlval) = 0;
        }
#line 3552 "fe/idl.tab.cpp"
    break;

  case 100:
#line 1342 "fe/idl.ypp"
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
#line 3579 "fe/idl.tab.cpp"
    break;

  case 101:
#line 1366 "fe/idl.ypp"
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
#line 3606 "fe/idl.tab.cpp"
    break;

  case 102:
#line 1392 "fe/idl.ypp"
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
#line 3671 "fe/idl.tab.cpp"
    break;

  case 108:
#line 1468 "fe/idl.ypp"
    {
          (yyval.vival) = AST_Field::vis_PUBLIC;
        }
#line 3679 "fe/idl.tab.cpp"
    break;

  case 109:
#line 1472 "fe/idl.ypp"
    {
          (yyval.vival) = AST_Field::vis_PRIVATE;
        }
#line 3687 "fe/idl.tab.cpp"
    break;

  case 110:
#line 1479 "fe/idl.ypp"
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
#line 3707 "fe/idl.tab.cpp"
    break;

  case 113:
#line 1503 "fe/idl.ypp"
    {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Decl *d = (yyvsp[0].dcval);
          if (d)
            {
              d->annotation_appls (*annotations);
            }
          else
            {
              idl_global->err ()-> unsupported_warning(
                "Annotating this is not supported");
            }
          delete annotations;
        }
#line 3726 "fe/idl.tab.cpp"
    break;

  case 114:
#line 1518 "fe/idl.ypp"
    {
        }
#line 3733 "fe/idl.tab.cpp"
    break;

  case 115:
#line 1524 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
#line 3741 "fe/idl.tab.cpp"
    break;

  case 116:
#line 1528 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 3750 "fe/idl.tab.cpp"
    break;

  case 117:
#line 1533 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
#line 3758 "fe/idl.tab.cpp"
    break;

  case 118:
#line 1537 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 3767 "fe/idl.tab.cpp"
    break;

  case 119:
#line 1542 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
        }
#line 3775 "fe/idl.tab.cpp"
    break;

  case 120:
#line 1546 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 3784 "fe/idl.tab.cpp"
    break;

  case 121:
#line 1551 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
#line 3792 "fe/idl.tab.cpp"
    break;

  case 122:
#line 1555 "fe/idl.ypp"
    {
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 3801 "fe/idl.tab.cpp"
    break;

  case 123:
#line 1560 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
        }
#line 3809 "fe/idl.tab.cpp"
    break;

  case 124:
#line 1564 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = 0;
        }
#line 3818 "fe/idl.tab.cpp"
    break;

  case 125:
#line 1569 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 3826 "fe/idl.tab.cpp"
    break;

  case 126:
#line 1573 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 3835 "fe/idl.tab.cpp"
    break;

  case 127:
#line 1578 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
        }
#line 3843 "fe/idl.tab.cpp"
    break;

  case 128:
#line 1582 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          (yyval.dcval) = (yyvsp[-2].dcval);
        }
#line 3852 "fe/idl.tab.cpp"
    break;

  case 129:
#line 1587 "fe/idl.ypp"
    {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 3860 "fe/idl.tab.cpp"
    break;

  case 130:
#line 1591 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
          (yyval.dcval) = 0;
        }
#line 3870 "fe/idl.tab.cpp"
    break;

  case 131:
#line 1600 "fe/idl.ypp"
    {
          ACE_NEW_RETURN ((yyval.nlval),
                          UTL_NameList ((yyvsp[-1].idlist),
                                        (yyvsp[0].nlval)),
                          1);
        }
#line 3881 "fe/idl.tab.cpp"
    break;

  case 132:
#line 1611 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
#line 3889 "fe/idl.tab.cpp"
    break;

  case 133:
#line 1615 "fe/idl.ypp"
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
#line 3913 "fe/idl.tab.cpp"
    break;

  case 134:
#line 1635 "fe/idl.ypp"
    {
          (yyval.nlval) = 0;
        }
#line 3921 "fe/idl.tab.cpp"
    break;

  case 135:
#line 1642 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN ((yyval.idlist),
                          UTL_IdList ((yyvsp[0].idval),
                                      0),
                          1);
        }
#line 3934 "fe/idl.tab.cpp"
    break;

  case 136:
#line 1651 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
#line 3942 "fe/idl.tab.cpp"
    break;

  case 137:
#line 1655 "fe/idl.ypp"
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
#line 3966 "fe/idl.tab.cpp"
    break;

  case 138:
#line 1676 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);

          // This cleans up all the non-global "::"s in scoped names.
          // If there is a global one, it gets put into the UTL_IdList,
          // so we clean it up in the case above.
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 3980 "fe/idl.tab.cpp"
    break;

  case 139:
#line 1686 "fe/idl.ypp"
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
#line 3996 "fe/idl.tab.cpp"
    break;

  case 140:
#line 1700 "fe/idl.ypp"
    {
          ACE_NEW_RETURN ((yyval.idval),
                          Identifier ((yyvsp[0].strval)),
                          1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 4008 "fe/idl.tab.cpp"
    break;

  case 141:
#line 1710 "fe/idl.ypp"
    {
          /* defining_id is a defining identifier
             whereas id is usually a reference to a defining identifier */
          ACE_NEW_RETURN ((yyval.idval), Identifier ((yyvsp[0].strval)), 1);
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;
        }
#line 4020 "fe/idl.tab.cpp"
    break;

  case 142:
#line 1721 "fe/idl.ypp"
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
#line 4067 "fe/idl.tab.cpp"
    break;

  case 143:
#line 1765 "fe/idl.ypp"
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
#line 4097 "fe/idl.tab.cpp"
    break;

  case 144:
#line 1792 "fe/idl.ypp"
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

          (yyval.dcval) = dynamic_cast<AST_Decl*>(f);
        }
#line 4129 "fe/idl.tab.cpp"
    break;

  case 145:
#line 1823 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
#line 4137 "fe/idl.tab.cpp"
    break;

  case 146:
#line 1827 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
#line 4145 "fe/idl.tab.cpp"
    break;

  case 147:
#line 1831 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
#line 4153 "fe/idl.tab.cpp"
    break;

  case 148:
#line 1835 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
#line 4161 "fe/idl.tab.cpp"
    break;

  case 149:
#line 1839 "fe/idl.ypp"
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
#line 4221 "fe/idl.tab.cpp"
    break;

  case 156:
#line 1904 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_string;
        }
#line 4229 "fe/idl.tab.cpp"
    break;

  case 157:
#line 1908 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_wstring;
        }
#line 4237 "fe/idl.tab.cpp"
    break;

  case 158:
#line 1912 "fe/idl.ypp"
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
#line 4307 "fe/idl.tab.cpp"
    break;

  case 162:
#line 1985 "fe/idl.ypp"
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_or,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4320 "fe/idl.tab.cpp"
    break;

  case 164:
#line 1998 "fe/idl.ypp"
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_xor,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4333 "fe/idl.tab.cpp"
    break;

  case 166:
#line 2011 "fe/idl.ypp"
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_and,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4346 "fe/idl.tab.cpp"
    break;

  case 168:
#line 2024 "fe/idl.ypp"
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_left,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4359 "fe/idl.tab.cpp"
    break;

  case 169:
#line 2033 "fe/idl.ypp"
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_right,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4372 "fe/idl.tab.cpp"
    break;

  case 171:
#line 2046 "fe/idl.ypp"
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_add,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4385 "fe/idl.tab.cpp"
    break;

  case 172:
#line 2055 "fe/idl.ypp"
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_minus,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4398 "fe/idl.tab.cpp"
    break;

  case 174:
#line 2068 "fe/idl.ypp"
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mul,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4411 "fe/idl.tab.cpp"
    break;

  case 175:
#line 2077 "fe/idl.ypp"
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_div,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4424 "fe/idl.tab.cpp"
    break;

  case 176:
#line 2086 "fe/idl.ypp"
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mod,
                                    (yyvsp[-2].exval),
                                    (yyvsp[0].exval)
                                  );
        }
#line 4437 "fe/idl.tab.cpp"
    break;

  case 178:
#line 2099 "fe/idl.ypp"
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_u_plus,
                                    (yyvsp[0].exval),
                                    0
                                  );
        }
#line 4450 "fe/idl.tab.cpp"
    break;

  case 179:
#line 2108 "fe/idl.ypp"
    {
          (yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_u_minus,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4463 "fe/idl.tab.cpp"
    break;

  case 180:
#line 2117 "fe/idl.ypp"
    {
          (yyval.exval) =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_bit_neg,
                                   (yyvsp[0].exval),
                                   0
                                 );
        }
#line 4476 "fe/idl.tab.cpp"
    break;

  case 181:
#line 2129 "fe/idl.ypp"
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
#line 4538 "fe/idl.tab.cpp"
    break;

  case 183:
#line 2188 "fe/idl.ypp"
    {
          (yyval.exval) = (yyvsp[-1].exval);
        }
#line 4546 "fe/idl.tab.cpp"
    break;

  case 184:
#line 2195 "fe/idl.ypp"
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].ival));
        }
#line 4554 "fe/idl.tab.cpp"
    break;

  case 185:
#line 2199 "fe/idl.ypp"
    {
          (yyval.exval) =
            idl_global->gen ()->create_expr ((yyvsp[0].uival));
        }
#line 4563 "fe/idl.tab.cpp"
    break;

  case 186:
#line 2204 "fe/idl.ypp"
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].sval));
          (yyvsp[0].sval)->destroy ();
          delete (yyvsp[0].sval);
          (yyvsp[0].sval) = 0;
        }
#line 4574 "fe/idl.tab.cpp"
    break;

  case 187:
#line 2211 "fe/idl.ypp"
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].wsval));
        }
#line 4582 "fe/idl.tab.cpp"
    break;

  case 188:
#line 2215 "fe/idl.ypp"
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].cval));
        }
#line 4590 "fe/idl.tab.cpp"
    break;

  case 189:
#line 2219 "fe/idl.ypp"
    {
          ACE_OutputCDR::from_wchar wc ((yyvsp[0].wcval));
          (yyval.exval) = idl_global->gen ()->create_expr (wc);
        }
#line 4599 "fe/idl.tab.cpp"
    break;

  case 190:
#line 2224 "fe/idl.ypp"
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].fixval));
        }
#line 4607 "fe/idl.tab.cpp"
    break;

  case 191:
#line 2228 "fe/idl.ypp"
    {
          (yyval.exval) = idl_global->gen ()->create_expr ((yyvsp[0].dval));
        }
#line 4615 "fe/idl.tab.cpp"
    break;

  case 192:
#line 2232 "fe/idl.ypp"
    {
          (yyval.exval) = idl_global->gen ()->create_expr (true);
        }
#line 4623 "fe/idl.tab.cpp"
    break;

  case 193:
#line 2236 "fe/idl.ypp"
    {
          (yyval.exval) = idl_global->gen ()->create_expr (false);
        }
#line 4631 "fe/idl.tab.cpp"
    break;

  case 194:
#line 2243 "fe/idl.ypp"
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
#line 4700 "fe/idl.tab.cpp"
    break;

  case 195:
#line 2311 "fe/idl.ypp"
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
#line 4721 "fe/idl.tab.cpp"
    break;

  case 196:
#line 2328 "fe/idl.ypp"
    {
          Identifier *id = (yyvsp[-4].idval);
          idl_global->scopes ().pop ();
          id->destroy ();
          delete id;
        }
#line 4732 "fe/idl.tab.cpp"
    break;

  case 202:
#line 2346 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 4741 "fe/idl.tab.cpp"
    break;

  case 206:
#line 2360 "fe/idl.ypp"
    {
          UTL_Scope *scope = idl_global->scopes ().top_non_null ();
          UTL_Scope *root = idl_global->scopes ().bottom ();

          AST_Expression::ExprType type = (yyvsp[-3].etval);
          Identifier *name = (yyvsp[-2].idval);
          // Annotation member names can't clash with C++ keywords
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
#line 4798 "fe/idl.tab.cpp"
    break;

  case 207:
#line 2416 "fe/idl.ypp"
    {
          (yyval.exval) = (yyvsp[0].exval);
        }
#line 4806 "fe/idl.tab.cpp"
    break;

  case 208:
#line 2420 "fe/idl.ypp"
    {
          (yyval.exval) = 0;
        }
#line 4814 "fe/idl.tab.cpp"
    break;

  case 209:
#line 2427 "fe/idl.ypp"
    {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Annotation_Appl *annotation = (yyvsp[0].annotation_val);
          if (annotation)
            {
              annotations->add (annotation);
            }
          (yyval.annotations_val) = annotations;
        }
#line 4828 "fe/idl.tab.cpp"
    break;

  case 210:
#line 2440 "fe/idl.ypp"
    {
          AST_Annotation_Appls *annotations = (yyvsp[-1].annotations_val);
          AST_Annotation_Appl *annotation = (yyvsp[0].annotation_val);
          if (annotation)
            {
              annotations->add (annotation);
            }
          (yyval.annotations_val) = annotations;
        }
#line 4842 "fe/idl.tab.cpp"
    break;

  case 211:
#line 2450 "fe/idl.ypp"
    {
          (yyval.annotations_val) = new AST_Annotation_Appls ();
        }
#line 4850 "fe/idl.tab.cpp"
    break;

  case 212:
#line 2457 "fe/idl.ypp"
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

              /*
               * Ignore lookup errors for parameter values that might depend on
               * constants or enums from the missing annotation declaration.
               */
              idl_global->ignore_lookup_errors_ = true;
            }

          (yyval.annotation_decl_val) = decl;
        }
#line 4909 "fe/idl.tab.cpp"
    break;

  case 213:
#line 2512 "fe/idl.ypp"
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
#line 4940 "fe/idl.tab.cpp"
    break;

  case 214:
#line 2542 "fe/idl.ypp"
    {
          (yyval.annotation_params_val) = (yyvsp[-1].annotation_params_val);
        }
#line 4948 "fe/idl.tab.cpp"
    break;

  case 215:
#line 2546 "fe/idl.ypp"
    {
          (yyval.annotation_params_val) = 0;
        }
#line 4956 "fe/idl.tab.cpp"
    break;

  case 216:
#line 2553 "fe/idl.ypp"
    {
          AST_Annotation_Appl::Params *params = new AST_Annotation_Appl::Params;
          AST_Annotation_Appl::Param *param = new AST_Annotation_Appl::Param;
          param->id = 0;
          param->expr = (yyvsp[0].exval);
          params->push (param);
          (yyval.annotation_params_val) = params;
        }
#line 4969 "fe/idl.tab.cpp"
    break;

  case 217:
#line 2562 "fe/idl.ypp"
    {
          (yyval.annotation_params_val) = (yyvsp[0].annotation_params_val);
        }
#line 4977 "fe/idl.tab.cpp"
    break;

  case 218:
#line 2569 "fe/idl.ypp"
    {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 4987 "fe/idl.tab.cpp"
    break;

  case 219:
#line 2577 "fe/idl.ypp"
    {
          AST_Annotation_Appl::Params *params = (yyvsp[0].annotation_params_val);
          params->push ((yyvsp[-1].annotation_param_val));
          (yyval.annotation_params_val) = params;
        }
#line 4997 "fe/idl.tab.cpp"
    break;

  case 220:
#line 2583 "fe/idl.ypp"
    {
          (yyval.annotation_params_val) = new AST_Annotation_Appl::Params;
        }
#line 5005 "fe/idl.tab.cpp"
    break;

  case 221:
#line 2590 "fe/idl.ypp"
    {
          AST_Annotation_Appl::Param *param = new AST_Annotation_Appl::Param;
          param->id = (yyvsp[-2].idval);
          // Annotation Member Names Can't Clash with C++ keywords
          FE_Utils::original_local_name (param->id);
          param->expr = (yyvsp[0].exval);
          (yyval.annotation_param_val) = param;
        }
#line 5018 "fe/idl.tab.cpp"
    break;

  case 222:
#line 2602 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
          idl_global->in_typedef (true);
        }
#line 5027 "fe/idl.tab.cpp"
    break;

  case 223:
#line 2607 "fe/idl.ypp"
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5035 "fe/idl.tab.cpp"
    break;

  case 224:
#line 2611 "fe/idl.ypp"
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5043 "fe/idl.tab.cpp"
    break;

  case 225:
#line 2615 "fe/idl.ypp"
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5051 "fe/idl.tab.cpp"
    break;

  case 226:
#line 2619 "fe/idl.ypp"
    {
          (yyval.dcval) = (yyvsp[0].dcval);
        }
#line 5059 "fe/idl.tab.cpp"
    break;

  case 227:
#line 2623 "fe/idl.ypp"
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

          (yyval.dcval) = 0;
        }
#line 5091 "fe/idl.tab.cpp"
    break;

  case 228:
#line 2651 "fe/idl.ypp"
    {
          (yyval.dcval) = 0;
        }
#line 5099 "fe/idl.tab.cpp"
    break;

  case 229:
#line 2658 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
#line 5107 "fe/idl.tab.cpp"
    break;

  case 230:
#line 2662 "fe/idl.ypp"
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
#line 5176 "fe/idl.tab.cpp"
    break;

  case 233:
#line 2735 "fe/idl.ypp"
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5187 "fe/idl.tab.cpp"
    break;

  case 235:
#line 2743 "fe/idl.ypp"
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
#line 5214 "fe/idl.tab.cpp"
    break;

  case 253:
#line 2798 "fe/idl.ypp"
    {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5225 "fe/idl.tab.cpp"
    break;

  case 254:
#line 2809 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5233 "fe/idl.tab.cpp"
    break;

  case 255:
#line 2813 "fe/idl.ypp"
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
#line 5257 "fe/idl.tab.cpp"
    break;

  case 256:
#line 2833 "fe/idl.ypp"
    {
          (yyval.dlval) = 0;
        }
#line 5265 "fe/idl.tab.cpp"
    break;

  case 259:
#line 2845 "fe/idl.ypp"
    {
          ACE_NEW_RETURN ((yyval.dlval),
                          UTL_DeclList ((yyvsp[-1].deval),
                                        (yyvsp[0].dlval)),
                          1);
        }
#line 5276 "fe/idl.tab.cpp"
    break;

  case 260:
#line 2856 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
#line 5284 "fe/idl.tab.cpp"
    break;

  case 261:
#line 2860 "fe/idl.ypp"
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
#line 5308 "fe/idl.tab.cpp"
    break;

  case 262:
#line 2880 "fe/idl.ypp"
    {
          (yyval.dlval) = 0;
        }
#line 5316 "fe/idl.tab.cpp"
    break;

  case 263:
#line 2887 "fe/idl.ypp"
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
#line 5333 "fe/idl.tab.cpp"
    break;

  case 264:
#line 2903 "fe/idl.ypp"
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
#line 5352 "fe/idl.tab.cpp"
    break;

  case 267:
#line 2926 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_long;
        }
#line 5360 "fe/idl.tab.cpp"
    break;

  case 268:
#line 2930 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_longlong;
        }
#line 5368 "fe/idl.tab.cpp"
    break;

  case 269:
#line 2934 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_short;
        }
#line 5376 "fe/idl.tab.cpp"
    break;

  case 270:
#line 2941 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_ulong;
        }
#line 5384 "fe/idl.tab.cpp"
    break;

  case 271:
#line 2945 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_ulonglong;
        }
#line 5392 "fe/idl.tab.cpp"
    break;

  case 272:
#line 2949 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_ushort;
        }
#line 5400 "fe/idl.tab.cpp"
    break;

  case 273:
#line 2956 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_double;
        }
#line 5408 "fe/idl.tab.cpp"
    break;

  case 274:
#line 2960 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_float;
        }
#line 5416 "fe/idl.tab.cpp"
    break;

  case 275:
#line 2964 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_longdouble;
        }
#line 5424 "fe/idl.tab.cpp"
    break;

  case 276:
#line 2971 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_fixed;
        }
#line 5432 "fe/idl.tab.cpp"
    break;

  case 277:
#line 2978 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_char;
        }
#line 5440 "fe/idl.tab.cpp"
    break;

  case 278:
#line 2982 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_wchar;
        }
#line 5448 "fe/idl.tab.cpp"
    break;

  case 279:
#line 2989 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_octet;
        }
#line 5456 "fe/idl.tab.cpp"
    break;

  case 280:
#line 2996 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_bool;
        }
#line 5464 "fe/idl.tab.cpp"
    break;

  case 281:
#line 3003 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_any;
        }
#line 5472 "fe/idl.tab.cpp"
    break;

  case 282:
#line 3010 "fe/idl.ypp"
    {
          (yyval.etval) = AST_Expression::EV_object;
        }
#line 5480 "fe/idl.tab.cpp"
    break;

  case 283:
#line 3017 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
#line 5488 "fe/idl.tab.cpp"
    break;

  case 284:
#line 3021 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5497 "fe/idl.tab.cpp"
    break;

  case 285:
#line 3030 "fe/idl.ypp"
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
#line 5532 "fe/idl.tab.cpp"
    break;

  case 286:
#line 3061 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
#line 5540 "fe/idl.tab.cpp"
    break;

  case 287:
#line 3065 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
#line 5548 "fe/idl.tab.cpp"
    break;

  case 288:
#line 3069 "fe/idl.ypp"
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
#line 5564 "fe/idl.tab.cpp"
    break;

  case 292:
#line 3091 "fe/idl.ypp"
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
#line 5582 "fe/idl.tab.cpp"
    break;

  case 293:
#line 3108 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
#line 5590 "fe/idl.tab.cpp"
    break;

  case 294:
#line 3112 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
#line 5598 "fe/idl.tab.cpp"
    break;

  case 295:
#line 3116 "fe/idl.ypp"
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
#line 5656 "fe/idl.tab.cpp"
    break;

  case 296:
#line 3170 "fe/idl.ypp"
    {
          idl_global->err()->syntax_error (idl_global->parse_state ());
        }
#line 5664 "fe/idl.tab.cpp"
    break;

  case 297:
#line 3174 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 5673 "fe/idl.tab.cpp"
    break;

  case 298:
#line 3182 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
#line 5681 "fe/idl.tab.cpp"
    break;

  case 299:
#line 3186 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 5690 "fe/idl.tab.cpp"
    break;

  case 300:
#line 3194 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
#line 5698 "fe/idl.tab.cpp"
    break;

  case 301:
#line 3198 "fe/idl.ypp"
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
#line 5735 "fe/idl.tab.cpp"
    break;

  case 302:
#line 3231 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
#line 5743 "fe/idl.tab.cpp"
    break;

  case 303:
#line 3235 "fe/idl.ypp"
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
#line 5806 "fe/idl.tab.cpp"
    break;

  case 304:
#line 3294 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
#line 5814 "fe/idl.tab.cpp"
    break;

  case 305:
#line 3298 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
#line 5822 "fe/idl.tab.cpp"
    break;

  case 306:
#line 3302 "fe/idl.ypp"
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
#line 5842 "fe/idl.tab.cpp"
    break;

  case 307:
#line 3321 "fe/idl.ypp"
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5853 "fe/idl.tab.cpp"
    break;

  case 308:
#line 3328 "fe/idl.ypp"
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
#line 5870 "fe/idl.tab.cpp"
    break;

  case 309:
#line 3341 "fe/idl.ypp"
    {
          /* octets are not allowed. */
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5883 "fe/idl.tab.cpp"
    break;

  case 310:
#line 3350 "fe/idl.ypp"
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 5894 "fe/idl.tab.cpp"
    break;

  case 312:
#line 3358 "fe/idl.ypp"
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
#line 6005 "fe/idl.tab.cpp"
    break;

  case 316:
#line 3475 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
#line 6013 "fe/idl.tab.cpp"
    break;

  case 317:
#line 3479 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
#line 6021 "fe/idl.tab.cpp"
    break;

  case 318:
#line 3483 "fe/idl.ypp"
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
#line 6057 "fe/idl.tab.cpp"
    break;

  case 319:
#line 3515 "fe/idl.ypp"
    {
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
#line 6065 "fe/idl.tab.cpp"
    break;

  case 320:
#line 3519 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          yyerrok;
        }
#line 6074 "fe/idl.tab.cpp"
    break;

  case 321:
#line 3527 "fe/idl.ypp"
    {
          ACE_NEW_RETURN ((yyval.llval),
                          UTL_LabelList ((yyvsp[-1].ulval),
                                         (yyvsp[0].llval)),
                          1);
        }
#line 6085 "fe/idl.tab.cpp"
    break;

  case 322:
#line 3537 "fe/idl.ypp"
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
#line 6107 "fe/idl.tab.cpp"
    break;

  case 323:
#line 3555 "fe/idl.ypp"
    {
          (yyval.llval) = 0;
        }
#line 6115 "fe/idl.tab.cpp"
    break;

  case 324:
#line 3562 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
#line 6123 "fe/idl.tab.cpp"
    break;

  case 325:
#line 3566 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
#line 6136 "fe/idl.tab.cpp"
    break;

  case 326:
#line 3575 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
#line 6144 "fe/idl.tab.cpp"
    break;

  case 327:
#line 3579 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
#line 6152 "fe/idl.tab.cpp"
    break;

  case 328:
#line 3583 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          (yyval.ulval) = idl_global->gen()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      (yyvsp[-2].exval)
                                    );
        }
#line 6165 "fe/idl.tab.cpp"
    break;

  case 329:
#line 3595 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
#line 6173 "fe/idl.tab.cpp"
    break;

  case 330:
#line 3599 "fe/idl.ypp"
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
#line 6221 "fe/idl.tab.cpp"
    break;

  case 331:
#line 3646 "fe/idl.ypp"
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
#line 6247 "fe/idl.tab.cpp"
    break;

  case 332:
#line 3671 "fe/idl.ypp"
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
#line 6271 "fe/idl.tab.cpp"
    break;

  case 333:
#line 3694 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
#line 6279 "fe/idl.tab.cpp"
    break;

  case 334:
#line 3698 "fe/idl.ypp"
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
#line 6314 "fe/idl.tab.cpp"
    break;

  case 335:
#line 3729 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
#line 6322 "fe/idl.tab.cpp"
    break;

  case 336:
#line 3733 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
#line 6330 "fe/idl.tab.cpp"
    break;

  case 337:
#line 3737 "fe/idl.ypp"
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
#line 6353 "fe/idl.tab.cpp"
    break;

  case 339:
#line 3762 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
#line 6361 "fe/idl.tab.cpp"
    break;

  case 342:
#line 3771 "fe/idl.ypp"
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
#line 6400 "fe/idl.tab.cpp"
    break;

  case 343:
#line 3810 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
#line 6408 "fe/idl.tab.cpp"
    break;

  case 344:
#line 3814 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
#line 6416 "fe/idl.tab.cpp"
    break;

  case 345:
#line 3818 "fe/idl.ypp"
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
#line 6505 "fe/idl.tab.cpp"
    break;

  case 346:
#line 3904 "fe/idl.ypp"
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
#line 6575 "fe/idl.tab.cpp"
    break;

  case 347:
#line 3973 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
#line 6588 "fe/idl.tab.cpp"
    break;

  case 348:
#line 3982 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
#line 6596 "fe/idl.tab.cpp"
    break;

  case 349:
#line 3986 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          Decl_Annotations_Pair *seq_head = new Decl_Annotations_Pair;
          seq_head->decl = (yyvsp[0].dcval);
          seq_head->annotations = (yyvsp[-1].annotations_val);
          (yyval.decl_annotations_pair_val) = seq_head;
        }
#line 6608 "fe/idl.tab.cpp"
    break;

  case 350:
#line 3997 "fe/idl.ypp"
    {
          (yyvsp[-1].exval)->evaluate (AST_Expression::EK_positive_int);
          (yyval.dcval) = idl_global->gen ()->create_fixed ((yyvsp[-3].exval), (yyvsp[-1].exval));
        }
#line 6617 "fe/idl.tab.cpp"
    break;

  case 351:
#line 4006 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 6625 "fe/idl.tab.cpp"
    break;

  case 352:
#line 4010 "fe/idl.ypp"
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 6633 "fe/idl.tab.cpp"
    break;

  case 353:
#line 4014 "fe/idl.ypp"
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
#line 6679 "fe/idl.tab.cpp"
    break;

  case 354:
#line 4056 "fe/idl.ypp"
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
#line 6706 "fe/idl.tab.cpp"
    break;

  case 355:
#line 4082 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 6714 "fe/idl.tab.cpp"
    break;

  case 356:
#line 4090 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
#line 6722 "fe/idl.tab.cpp"
    break;

  case 357:
#line 4094 "fe/idl.ypp"
    {
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
#line 6730 "fe/idl.tab.cpp"
    break;

  case 358:
#line 4098 "fe/idl.ypp"
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
#line 6767 "fe/idl.tab.cpp"
    break;

  case 359:
#line 4131 "fe/idl.ypp"
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
#line 6794 "fe/idl.tab.cpp"
    break;

  case 360:
#line 4157 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
#line 6802 "fe/idl.tab.cpp"
    break;

  case 361:
#line 4164 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
#line 6810 "fe/idl.tab.cpp"
    break;

  case 362:
#line 4168 "fe/idl.ypp"
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
#line 6854 "fe/idl.tab.cpp"
    break;

  case 363:
#line 4211 "fe/idl.ypp"
    {
          ACE_NEW_RETURN ((yyval.elval),
                          UTL_ExprList ((yyvsp[-1].exval),
                                        (yyvsp[0].elval)),
                          1);
        }
#line 6865 "fe/idl.tab.cpp"
    break;

  case 364:
#line 4221 "fe/idl.ypp"
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
#line 6887 "fe/idl.tab.cpp"
    break;

  case 365:
#line 4239 "fe/idl.ypp"
    {
          (yyval.elval) = 0;
        }
#line 6895 "fe/idl.tab.cpp"
    break;

  case 366:
#line 4246 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
#line 6903 "fe/idl.tab.cpp"
    break;

  case 367:
#line 4250 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
#line 6911 "fe/idl.tab.cpp"
    break;

  case 368:
#line 4254 "fe/idl.ypp"
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
#line 6969 "fe/idl.tab.cpp"
    break;

  case 371:
#line 4316 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
#line 6977 "fe/idl.tab.cpp"
    break;

  case 372:
#line 4320 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 6985 "fe/idl.tab.cpp"
    break;

  case 373:
#line 4324 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 6993 "fe/idl.tab.cpp"
    break;

  case 374:
#line 4328 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 7001 "fe/idl.tab.cpp"
    break;

  case 375:
#line 4332 "fe/idl.ypp"
    {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Attribute *a = 0;
          FE_Declarator *d = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          /*
           * Create nodes representing attributes and add them to the
           * enclosing scope.
           */
          if (s != 0 && (yyvsp[-4].dcval) != 0 && (yyvsp[-2].dlval) != 0)
            {
              for (UTL_DecllistActiveIterator l ((yyvsp[-2].dlval)); !l.is_done (); l.next ())
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

                  a = idl_global->gen ()->create_attribute (
                    true, tp, d->name (), s->is_local (), s->is_abstract ());

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

          (yyval.dcval) = a;
        }
#line 7057 "fe/idl.tab.cpp"
    break;

  case 376:
#line 4387 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
#line 7065 "fe/idl.tab.cpp"
    break;

  case 377:
#line 4391 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
#line 7073 "fe/idl.tab.cpp"
    break;

  case 378:
#line 4395 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
#line 7081 "fe/idl.tab.cpp"
    break;

  case 379:
#line 4399 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
#line 7089 "fe/idl.tab.cpp"
    break;

  case 380:
#line 4403 "fe/idl.ypp"
    {
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Attribute *a = 0;
          FE_Declarator *d = 0;

          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseCompleted);

          /*
           * Create nodes representing attributes and add them to the
           * enclosing scope.
           */
          if (s != 0 && (yyvsp[-6].dcval) != 0 && (yyvsp[-4].dlval) != 0)
            {
              for (UTL_DecllistActiveIterator l ((yyvsp[-4].dlval)); !l.is_done (); l.next ())
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

                  a = idl_global->gen ()->create_attribute (
                   false, tp, d->name (), s->is_local (), s->is_abstract ());

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

          (yyval.dcval) = a;
        }
#line 7154 "fe/idl.tab.cpp"
    break;

  case 381:
#line 4467 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
#line 7162 "fe/idl.tab.cpp"
    break;

  case 382:
#line 4471 "fe/idl.ypp"
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
#line 7195 "fe/idl.tab.cpp"
    break;

  case 383:
#line 4500 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
#line 7203 "fe/idl.tab.cpp"
    break;

  case 384:
#line 4504 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
#line 7211 "fe/idl.tab.cpp"
    break;

  case 385:
#line 4508 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();
        }
#line 7223 "fe/idl.tab.cpp"
    break;

  case 386:
#line 4519 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7231 "fe/idl.tab.cpp"
    break;

  case 387:
#line 4523 "fe/idl.ypp"
    {
          AST_Operation *op = 0;
          UTL_Scope *scope = idl_global->scopes ().top_non_null ();

          AST_Operation::Flags op_flags = (yyvsp[-3].ofval);
          AST_Decl *type_node = (yyvsp[-2].dcval);
          (yyvsp[-2].dcval) = 0;
          Identifier id ((yyvsp[0].strval));
          ACE::strdelete ((yyvsp[0].strval));
          (yyvsp[0].strval) = 0;

          UTL_ScopedName name (&id, 0);
          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing an operation on an interface
           * and add it to its enclosing scope.
           */
          if (scope != 0 && type_node != 0)
            {
              AST_Type *type = AST_Type::narrow_from_decl (type_node);

              if (type == 0)
                {
                  idl_global->err ()->not_a_type (type_node);
                }
              else if (type->node_type () == AST_Decl::NT_except)
                {
                  idl_global->err ()->not_a_type (type_node);
                }
              else
                {
                  AST_Decl *d = ScopeAsDecl (scope);
                  AST_Decl::NodeType nt = d->node_type ();
                  const bool local = scope->is_local ()
                    || nt == AST_Decl::NT_valuetype
                    || nt == AST_Decl::NT_eventtype;

                  op = idl_global->gen ()->create_operation (
                    type, op_flags, &name, local, scope->is_abstract ());

                  if (!local && type->is_local ())
                    {
                      idl_global->err ()->local_remote_mismatch (type, op);
                      op->destroy ();
                      delete op;
                      op = 0;
                    }
                  else
                    {
                      scope->fe_add_operation (op);
                    }
                }
            }

          /*
           * Push the operation scope onto the scopes stack.
           */
          idl_global->scopes ().push (op);
        }
#line 7296 "fe/idl.tab.cpp"
    break;

  case 388:
#line 4584 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7304 "fe/idl.tab.cpp"
    break;

  case 389:
#line 4588 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
#line 7312 "fe/idl.tab.cpp"
    break;

  case 390:
#line 4592 "fe/idl.ypp"
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

          (yyval.dcval) = o;
        }
#line 7347 "fe/idl.tab.cpp"
    break;

  case 391:
#line 4626 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_oneway;
        }
#line 7356 "fe/idl.tab.cpp"
    break;

  case 392:
#line 4631 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          (yyval.ofval) = AST_Operation::OP_idempotent;
        }
#line 7365 "fe/idl.tab.cpp"
    break;

  case 393:
#line 4636 "fe/idl.ypp"
    {
          (yyval.ofval) = AST_Operation::OP_noflags;
        }
#line 7373 "fe/idl.tab.cpp"
    break;

  case 395:
#line 4644 "fe/idl.ypp"
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
#line 7384 "fe/idl.tab.cpp"
    break;

  case 396:
#line 4654 "fe/idl.ypp"
    {
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
#line 7393 "fe/idl.tab.cpp"
    break;

  case 397:
#line 4659 "fe/idl.ypp"
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
#line 7440 "fe/idl.tab.cpp"
    break;

  case 398:
#line 4702 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 7448 "fe/idl.tab.cpp"
    break;

  case 399:
#line 4706 "fe/idl.ypp"
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
#line 7465 "fe/idl.tab.cpp"
    break;

  case 400:
#line 4722 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7473 "fe/idl.tab.cpp"
    break;

  case 401:
#line 4726 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7481 "fe/idl.tab.cpp"
    break;

  case 402:
#line 4730 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7489 "fe/idl.tab.cpp"
    break;

  case 403:
#line 4735 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7497 "fe/idl.tab.cpp"
    break;

  case 405:
#line 4745 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7505 "fe/idl.tab.cpp"
    break;

  case 408:
#line 4754 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7513 "fe/idl.tab.cpp"
    break;

  case 409:
#line 4758 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7521 "fe/idl.tab.cpp"
    break;

  case 410:
#line 4762 "fe/idl.ypp"
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
#line 7557 "fe/idl.tab.cpp"
    break;

  case 411:
#line 4797 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7565 "fe/idl.tab.cpp"
    break;

  case 412:
#line 4801 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7573 "fe/idl.tab.cpp"
    break;

  case 413:
#line 4805 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
#line 7581 "fe/idl.tab.cpp"
    break;

  case 414:
#line 4810 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
#line 7589 "fe/idl.tab.cpp"
    break;

  case 416:
#line 4820 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
#line 7597 "fe/idl.tab.cpp"
    break;

  case 419:
#line 4829 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
#line 7605 "fe/idl.tab.cpp"
    break;

  case 420:
#line 4833 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
#line 7613 "fe/idl.tab.cpp"
    break;

  case 421:
#line 4837 "fe/idl.ypp"
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
#line 7656 "fe/idl.tab.cpp"
    break;

  case 422:
#line 4879 "fe/idl.ypp"
    {
          (yyval.dcval) =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 (yyvsp[0].etval)
                                               );
        }
#line 7667 "fe/idl.tab.cpp"
    break;

  case 425:
#line 4888 "fe/idl.ypp"
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
#line 7775 "fe/idl.tab.cpp"
    break;

  case 426:
#line 4995 "fe/idl.ypp"
    {
          (yyval.dival) = AST_Argument::dir_IN;
        }
#line 7783 "fe/idl.tab.cpp"
    break;

  case 427:
#line 4999 "fe/idl.ypp"
    {
          (yyval.dival) = AST_Argument::dir_OUT;
        }
#line 7791 "fe/idl.tab.cpp"
    break;

  case 428:
#line 5003 "fe/idl.ypp"
    {
          (yyval.dival) = AST_Argument::dir_INOUT;
        }
#line 7799 "fe/idl.tab.cpp"
    break;

  case 429:
#line 5010 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
#line 7807 "fe/idl.tab.cpp"
    break;

  case 430:
#line 5014 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
#line 7815 "fe/idl.tab.cpp"
    break;

  case 431:
#line 5019 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7824 "fe/idl.tab.cpp"
    break;

  case 432:
#line 5024 "fe/idl.ypp"
    {
          (yyval.nlval) = 0;
        }
#line 7832 "fe/idl.tab.cpp"
    break;

  case 433:
#line 5031 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
#line 7840 "fe/idl.tab.cpp"
    break;

  case 434:
#line 5035 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
#line 7848 "fe/idl.tab.cpp"
    break;

  case 435:
#line 5040 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7857 "fe/idl.tab.cpp"
    break;

  case 436:
#line 5045 "fe/idl.ypp"
    {
          (yyval.nlval) = 0;
        }
#line 7865 "fe/idl.tab.cpp"
    break;

  case 437:
#line 5052 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
#line 7873 "fe/idl.tab.cpp"
    break;

  case 438:
#line 5056 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
#line 7881 "fe/idl.tab.cpp"
    break;

  case 439:
#line 5061 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          (yyval.nlval) = (yyvsp[-1].nlval);
        }
#line 7890 "fe/idl.tab.cpp"
    break;

  case 440:
#line 5066 "fe/idl.ypp"
    {
          (yyval.nlval) = 0;
        }
#line 7898 "fe/idl.tab.cpp"
    break;

  case 441:
#line 5073 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
#line 7906 "fe/idl.tab.cpp"
    break;

  case 442:
#line 5077 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
        }
#line 7914 "fe/idl.tab.cpp"
    break;

  case 443:
#line 5082 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          (yyval.slval) = (yyvsp[-1].slval);
        }
#line 7923 "fe/idl.tab.cpp"
    break;

  case 444:
#line 5087 "fe/idl.ypp"
    {
          (yyval.slval) = 0;
        }
#line 7931 "fe/idl.tab.cpp"
    break;

  case 445:
#line 5094 "fe/idl.ypp"
    {
          ACE_NEW_RETURN ((yyval.slval),
                          UTL_StrList ((yyvsp[-1].sval),
                                       (yyvsp[0].slval)),
                          1);
        }
#line 7942 "fe/idl.tab.cpp"
    break;

  case 446:
#line 5105 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
#line 7950 "fe/idl.tab.cpp"
    break;

  case 447:
#line 5109 "fe/idl.ypp"
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
#line 7972 "fe/idl.tab.cpp"
    break;

  case 448:
#line 5127 "fe/idl.ypp"
    {
          (yyval.slval) = 0;
        }
#line 7980 "fe/idl.tab.cpp"
    break;

  case 449:
#line 5134 "fe/idl.ypp"
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
#line 8009 "fe/idl.tab.cpp"
    break;

  case 450:
#line 5162 "fe/idl.ypp"
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
#line 8047 "fe/idl.tab.cpp"
    break;

  case 453:
#line 5205 "fe/idl.ypp"
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
#line 8075 "fe/idl.tab.cpp"
    break;

  case 454:
#line 5232 "fe/idl.ypp"
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
#line 8118 "fe/idl.tab.cpp"
    break;

  case 455:
#line 5271 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
#line 8126 "fe/idl.tab.cpp"
    break;

  case 456:
#line 5275 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
#line 8134 "fe/idl.tab.cpp"
    break;

  case 457:
#line 5279 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 8147 "fe/idl.tab.cpp"
    break;

  case 458:
#line 5292 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
#line 8155 "fe/idl.tab.cpp"
    break;

  case 459:
#line 5296 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 8163 "fe/idl.tab.cpp"
    break;

  case 460:
#line 5300 "fe/idl.ypp"
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
#line 8201 "fe/idl.tab.cpp"
    break;

  case 461:
#line 5337 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 8209 "fe/idl.tab.cpp"
    break;

  case 462:
#line 5341 "fe/idl.ypp"
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8217 "fe/idl.tab.cpp"
    break;

  case 463:
#line 5345 "fe/idl.ypp"
    {
          (yyval.idlist) = 0;
        }
#line 8225 "fe/idl.tab.cpp"
    break;

  case 466:
#line 5357 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 8233 "fe/idl.tab.cpp"
    break;

  case 467:
#line 5361 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8241 "fe/idl.tab.cpp"
    break;

  case 468:
#line 5365 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 8249 "fe/idl.tab.cpp"
    break;

  case 469:
#line 5369 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8257 "fe/idl.tab.cpp"
    break;

  case 470:
#line 5373 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
        }
#line 8265 "fe/idl.tab.cpp"
    break;

  case 471:
#line 5377 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8273 "fe/idl.tab.cpp"
    break;

  case 472:
#line 5381 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
        }
#line 8281 "fe/idl.tab.cpp"
    break;

  case 473:
#line 5385 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8289 "fe/idl.tab.cpp"
    break;

  case 474:
#line 5389 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
        }
#line 8297 "fe/idl.tab.cpp"
    break;

  case 475:
#line 5393 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8305 "fe/idl.tab.cpp"
    break;

  case 476:
#line 5397 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 8313 "fe/idl.tab.cpp"
    break;

  case 477:
#line 5401 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8321 "fe/idl.tab.cpp"
    break;

  case 478:
#line 5405 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 8329 "fe/idl.tab.cpp"
    break;

  case 479:
#line 5409 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 8337 "fe/idl.tab.cpp"
    break;

  case 480:
#line 5415 "fe/idl.ypp"
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
#line 8428 "fe/idl.tab.cpp"
    break;

  case 481:
#line 5505 "fe/idl.ypp"
    {
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8438 "fe/idl.tab.cpp"
    break;

  case 482:
#line 5511 "fe/idl.ypp"
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
#line 8465 "fe/idl.tab.cpp"
    break;

  case 483:
#line 5536 "fe/idl.ypp"
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
#line 8572 "fe/idl.tab.cpp"
    break;

  case 484:
#line 5642 "fe/idl.ypp"
    {
          // We use this extra rule here to use in both uses_decl and
          // extended_uses_decl, so the LALR(1) parser can avoid conflicts.
          (yyval.bval) = (yyvsp[0].bval);
        }
#line 8582 "fe/idl.tab.cpp"
    break;

  case 485:
#line 5651 "fe/idl.ypp"
    {
          (yyval.bval) = true;
        }
#line 8590 "fe/idl.tab.cpp"
    break;

  case 486:
#line 5655 "fe/idl.ypp"
    {
          (yyval.bval) = false;
        }
#line 8598 "fe/idl.tab.cpp"
    break;

  case 487:
#line 5661 "fe/idl.ypp"
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
#line 8672 "fe/idl.tab.cpp"
    break;

  case 488:
#line 5733 "fe/idl.ypp"
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
#line 8746 "fe/idl.tab.cpp"
    break;

  case 489:
#line 5805 "fe/idl.ypp"
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
#line 8820 "fe/idl.tab.cpp"
    break;

  case 490:
#line 5878 "fe/idl.ypp"
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
#line 8861 "fe/idl.tab.cpp"
    break;

  case 491:
#line 5915 "fe/idl.ypp"
    {
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 8872 "fe/idl.tab.cpp"
    break;

  case 492:
#line 5925 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
#line 8880 "fe/idl.tab.cpp"
    break;

  case 493:
#line 5929 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
#line 8888 "fe/idl.tab.cpp"
    break;

  case 494:
#line 5933 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 8896 "fe/idl.tab.cpp"
    break;

  case 495:
#line 5937 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
#line 8904 "fe/idl.tab.cpp"
    break;

  case 496:
#line 5941 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
#line 8912 "fe/idl.tab.cpp"
    break;

  case 497:
#line 5945 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
#line 8920 "fe/idl.tab.cpp"
    break;

  case 498:
#line 5949 "fe/idl.ypp"
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
#line 8970 "fe/idl.tab.cpp"
    break;

  case 499:
#line 5998 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
#line 8978 "fe/idl.tab.cpp"
    break;

  case 500:
#line 6002 "fe/idl.ypp"
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 8986 "fe/idl.tab.cpp"
    break;

  case 501:
#line 6006 "fe/idl.ypp"
    {
          (yyval.idlist) = 0;
        }
#line 8994 "fe/idl.tab.cpp"
    break;

  case 502:
#line 6014 "fe/idl.ypp"
    {
          (yyval.idlist) = (yyvsp[0].idlist);
        }
#line 9002 "fe/idl.tab.cpp"
    break;

  case 503:
#line 6018 "fe/idl.ypp"
    {
          (yyval.idlist) = 0;
        }
#line 9010 "fe/idl.tab.cpp"
    break;

  case 504:
#line 6025 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
#line 9018 "fe/idl.tab.cpp"
    break;

  case 505:
#line 6029 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
#line 9026 "fe/idl.tab.cpp"
    break;

  case 506:
#line 6033 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
#line 9034 "fe/idl.tab.cpp"
    break;

  case 510:
#line 6046 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
#line 9042 "fe/idl.tab.cpp"
    break;

  case 511:
#line 6050 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9050 "fe/idl.tab.cpp"
    break;

  case 512:
#line 6054 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
#line 9058 "fe/idl.tab.cpp"
    break;

  case 513:
#line 6058 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 9066 "fe/idl.tab.cpp"
    break;

  case 514:
#line 6066 "fe/idl.ypp"
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
#line 9093 "fe/idl.tab.cpp"
    break;

  case 515:
#line 6089 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 9101 "fe/idl.tab.cpp"
    break;

  case 516:
#line 6093 "fe/idl.ypp"
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
#line 9123 "fe/idl.tab.cpp"
    break;

  case 517:
#line 6115 "fe/idl.ypp"
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
#line 9154 "fe/idl.tab.cpp"
    break;

  case 518:
#line 6142 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
#line 9162 "fe/idl.tab.cpp"
    break;

  case 519:
#line 6146 "fe/idl.ypp"
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
#line 9184 "fe/idl.tab.cpp"
    break;

  case 525:
#line 6179 "fe/idl.ypp"
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
#line 9211 "fe/idl.tab.cpp"
    break;

  case 526:
#line 6207 "fe/idl.ypp"
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
#line 9238 "fe/idl.tab.cpp"
    break;

  case 527:
#line 6234 "fe/idl.ypp"
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
#line 9286 "fe/idl.tab.cpp"
    break;

  case 528:
#line 6278 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9294 "fe/idl.tab.cpp"
    break;

  case 529:
#line 6282 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9302 "fe/idl.tab.cpp"
    break;

  case 530:
#line 6286 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9315 "fe/idl.tab.cpp"
    break;

  case 531:
#line 6300 "fe/idl.ypp"
    {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9323 "fe/idl.tab.cpp"
    break;

  case 532:
#line 6309 "fe/idl.ypp"
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
#line 9340 "fe/idl.tab.cpp"
    break;

  case 533:
#line 6326 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9350 "fe/idl.tab.cpp"
    break;

  case 534:
#line 6335 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
#line 9358 "fe/idl.tab.cpp"
    break;

  case 535:
#line 6339 "fe/idl.ypp"
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
#line 9391 "fe/idl.tab.cpp"
    break;

  case 536:
#line 6372 "fe/idl.ypp"
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
#line 9444 "fe/idl.tab.cpp"
    break;

  case 537:
#line 6421 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
#line 9452 "fe/idl.tab.cpp"
    break;

  case 538:
#line 6425 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
#line 9460 "fe/idl.tab.cpp"
    break;

  case 539:
#line 6429 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
#line 9473 "fe/idl.tab.cpp"
    break;

  case 540:
#line 6441 "fe/idl.ypp"
    {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9481 "fe/idl.tab.cpp"
    break;

  case 541:
#line 6445 "fe/idl.ypp"
    {
          (yyval.idval) = (yyvsp[0].idval);
        }
#line 9489 "fe/idl.tab.cpp"
    break;

  case 542:
#line 6452 "fe/idl.ypp"
    {
          (yyval.ntval) = AST_Decl::NT_type;
        }
#line 9497 "fe/idl.tab.cpp"
    break;

  case 543:
#line 6456 "fe/idl.ypp"
    {
          (yyval.ntval) = AST_Decl::NT_struct;
        }
#line 9505 "fe/idl.tab.cpp"
    break;

  case 544:
#line 6460 "fe/idl.ypp"
    {
          (yyval.ntval) = AST_Decl::NT_union;
        }
#line 9513 "fe/idl.tab.cpp"
    break;

  case 545:
#line 6464 "fe/idl.ypp"
    {
          (yyval.ntval) = AST_Decl::NT_eventtype;
        }
#line 9521 "fe/idl.tab.cpp"
    break;

  case 546:
#line 6468 "fe/idl.ypp"
    {
          (yyval.ntval) = AST_Decl::NT_sequence;
        }
#line 9529 "fe/idl.tab.cpp"
    break;

  case 547:
#line 6472 "fe/idl.ypp"
    {
          (yyval.ntval) = AST_Decl::NT_interface;
        }
#line 9537 "fe/idl.tab.cpp"
    break;

  case 548:
#line 6476 "fe/idl.ypp"
    {
          (yyval.ntval) = AST_Decl::NT_valuetype;
        }
#line 9545 "fe/idl.tab.cpp"
    break;

  case 549:
#line 6480 "fe/idl.ypp"
    {
          (yyval.ntval) = AST_Decl::NT_enum;
        }
#line 9553 "fe/idl.tab.cpp"
    break;

  case 550:
#line 6484 "fe/idl.ypp"
    {
          (yyval.ntval) = AST_Decl::NT_except;
        }
#line 9561 "fe/idl.tab.cpp"
    break;

  case 551:
#line 6488 "fe/idl.ypp"
    {
          (yyval.ntval) = AST_Decl::NT_const;
          t_param_const_type = (yyvsp[0].etval);
        }
#line 9570 "fe/idl.tab.cpp"
    break;

  case 552:
#line 6496 "fe/idl.ypp"
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
#line 9602 "fe/idl.tab.cpp"
    break;

  case 553:
#line 6527 "fe/idl.ypp"
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
#line 9619 "fe/idl.tab.cpp"
    break;

  case 554:
#line 6540 "fe/idl.ypp"
    {
          (yyval.plval) = 0;
        }
#line 9627 "fe/idl.tab.cpp"
    break;

  case 555:
#line 6547 "fe/idl.ypp"
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
#line 9656 "fe/idl.tab.cpp"
    break;

  case 556:
#line 6572 "fe/idl.ypp"
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
#line 9675 "fe/idl.tab.cpp"
    break;

  case 557:
#line 6590 "fe/idl.ypp"
    {
          ACE_NEW_RETURN ((yyval.slval), UTL_StrList ((yyvsp[-1].sval), (yyvsp[0].slval)), 1);
        }
#line 9683 "fe/idl.tab.cpp"
    break;

  case 558:
#line 6597 "fe/idl.ypp"
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
#line 9702 "fe/idl.tab.cpp"
    break;

  case 559:
#line 6612 "fe/idl.ypp"
    {
          (yyval.slval) = 0;
        }
#line 9710 "fe/idl.tab.cpp"
    break;

  case 560:
#line 6619 "fe/idl.ypp"
    {
          ACE_NEW_RETURN ((yyval.sval),
                          UTL_String ((yyvsp[0].strval), true),
                          1);
        }
#line 9720 "fe/idl.tab.cpp"
    break;

  case 561:
#line 6628 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSeen);
        }
#line 9728 "fe/idl.tab.cpp"
    break;

  case 562:
#line 6632 "fe/idl.ypp"
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
#line 9752 "fe/idl.tab.cpp"
    break;

  case 563:
#line 6652 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeSqSeen);
        }
#line 9760 "fe/idl.tab.cpp"
    break;

  case 564:
#line 6656 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeBodySeen);
        }
#line 9768 "fe/idl.tab.cpp"
    break;

  case 565:
#line 6660 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_PorttypeQsSeen);

          // Done with this port type - pop it off the scopes stack.
          idl_global->scopes ().pop ();
        }
#line 9779 "fe/idl.tab.cpp"
    break;

  case 566:
#line 6670 "fe/idl.ypp"
    {
        }
#line 9786 "fe/idl.tab.cpp"
    break;

  case 567:
#line 6676 "fe/idl.ypp"
    {
        }
#line 9793 "fe/idl.tab.cpp"
    break;

  case 568:
#line 6679 "fe/idl.ypp"
    {
        }
#line 9800 "fe/idl.tab.cpp"
    break;

  case 569:
#line 6685 "fe/idl.ypp"
    {
        }
#line 9807 "fe/idl.tab.cpp"
    break;

  case 570:
#line 6688 "fe/idl.ypp"
    {
        }
#line 9814 "fe/idl.tab.cpp"
    break;

  case 571:
#line 6691 "fe/idl.ypp"
    {
        }
#line 9821 "fe/idl.tab.cpp"
    break;

  case 572:
#line 6694 "fe/idl.ypp"
    {
        }
#line 9828 "fe/idl.tab.cpp"
    break;

  case 573:
#line 6697 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 9836 "fe/idl.tab.cpp"
    break;

  case 574:
#line 6701 "fe/idl.ypp"
    {
        }
#line 9843 "fe/idl.tab.cpp"
    break;

  case 575:
#line 6707 "fe/idl.ypp"
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
#line 9916 "fe/idl.tab.cpp"
    break;

  case 576:
#line 6776 "fe/idl.ypp"
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
#line 9967 "fe/idl.tab.cpp"
    break;

  case 577:
#line 6826 "fe/idl.ypp"
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
#line 9984 "fe/idl.tab.cpp"
    break;

  case 578:
#line 6842 "fe/idl.ypp"
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
#line 10001 "fe/idl.tab.cpp"
    break;

  case 579:
#line 6855 "fe/idl.ypp"
    {
          (yyval.alval) = 0;
        }
#line 10009 "fe/idl.tab.cpp"
    break;

  case 580:
#line 6862 "fe/idl.ypp"
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
#line 10069 "fe/idl.tab.cpp"
    break;

  case 582:
#line 6925 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSeen);
        }
#line 10077 "fe/idl.tab.cpp"
    break;

  case 583:
#line 6929 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorIDSeen);
        }
#line 10085 "fe/idl.tab.cpp"
    break;

  case 584:
#line 6933 "fe/idl.ypp"
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
#line 10143 "fe/idl.tab.cpp"
    break;

  case 585:
#line 6990 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorSqSeen);
        }
#line 10151 "fe/idl.tab.cpp"
    break;

  case 586:
#line 6994 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorBodySeen);
        }
#line 10159 "fe/idl.tab.cpp"
    break;

  case 587:
#line 6998 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ConnectorQsSeen);

          // Done with this connector - pop it off the scope stack.
          idl_global->scopes ().pop ();
        }
#line 10170 "fe/idl.tab.cpp"
    break;

  case 590:
#line 7013 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
#line 10178 "fe/idl.tab.cpp"
    break;

  case 591:
#line 7017 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10186 "fe/idl.tab.cpp"
    break;

  case 592:
#line 7021 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
#line 10194 "fe/idl.tab.cpp"
    break;

  case 593:
#line 7025 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10202 "fe/idl.tab.cpp"
    break;

  case 594:
#line 7029 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
#line 10210 "fe/idl.tab.cpp"
    break;

  case 595:
#line 7033 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10218 "fe/idl.tab.cpp"
    break;

  case 596:
#line 7037 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_ExtendedPortDeclSeen);
        }
#line 10226 "fe/idl.tab.cpp"
    break;

  case 597:
#line 7041 "fe/idl.ypp"
    {
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
#line 10234 "fe/idl.tab.cpp"
    break;


#line 10238 "fe/idl.tab.cpp"

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
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
#line 7046 "fe/idl.ypp"

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
  ACE_ERROR ((LM_ERROR, "%C\n", msg));
}
