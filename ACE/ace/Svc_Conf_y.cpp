/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with ACE_YY_ or ace_yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with ace_yy or ACE_YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define ACE_YYBISON 30802

/* Bison version string.  */
#define ACE_YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define ACE_YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define ACE_YYPURE 1

/* Push parsers.  */
#define ACE_YYPUSH 0

/* Pull parsers.  */
#define ACE_YYPULL 1




/* First part of user prologue.  */

#include "ace/Svc_Conf.h"

#if (ACE_USES_CLASSIC_SVC_CONF == 1)

#include "ace/Module.h"
#include "ace/Stream.h"
#include "ace/Service_Types.h"
#include "ace/ace_wchar.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Prototypes.

static ACE_Module_Type *
  ace_get_module (ACE_Service_Type const * sr,
                  ACE_TCHAR const * svc_name,
                  int & ace_ace_yyerrno);

#define ACE_YYDEBUG_LEXER_TEXT (ace_yytext[ace_yyleng] = '\0', ace_yytext)

// Force the pretty debugging code to compile.
// #define ACE_YYDEBUG 1

// Bison 2.3 template contains switch statement with a "default:", but
// without a "case:" label. Suppressing a compiler warning for Visual
// C++.
#if defined (_MSC_VER)
#   pragma warning ( disable : 4065 )
#endif

// Normalize the message literal's type to match ace_yyerror() prototype
#define ACE_YY_ ACE_TEXT

// Prevent yacc(1) from declaring a trivial ACE_YYSTYPE just because
// ACE_YYSTYPE is not a macro definition. On the other hand we want
// ACE_YYSTYPE_IS_DECLARED to be as localized as possible to avoid
// poluting the global namespace - there may be other yacc(1) parsers
// that want to play nice with ACE
#define ACE_YYSTYPE_IS_DECLARED

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4702)
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfree-nonheap-object"
#endif

ACE_END_VERSIONED_NAMESPACE_DECL



# ifndef ACE_YY_CAST
#  ifdef __cplusplus
#   define ACE_YY_CAST(Type, Val) static_cast<Type> (Val)
#   define ACE_YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define ACE_YY_CAST(Type, Val) ((Type) (Val))
#   define ACE_YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef ACE_YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define ACE_YY_NULLPTR nullptr
#   else
#    define ACE_YY_NULLPTR 0
#   endif
#  else
#   define ACE_YY_NULLPTR ((void*)0)
#  endif
# endif

#include "Svc_Conf_Token_Table.h"
/* Symbol kind.  */
enum ace_yysymbol_kind_t
{
  ACE_YYSYMBOL_ACE_YYEMPTY = -2,
  ACE_YYSYMBOL_ACE_YYEOF = 0,                      /* "end of file"  */
  ACE_YYSYMBOL_ACE_YYerror = 1,                    /* error  */
  ACE_YYSYMBOL_ACE_YYUNDEF = 2,                    /* "invalid token"  */
  ACE_YYSYMBOL_ACE_DYNAMIC = 3,                /* ACE_DYNAMIC  */
  ACE_YYSYMBOL_ACE_STATIC = 4,                 /* ACE_STATIC  */
  ACE_YYSYMBOL_ACE_SUSPEND = 5,                /* ACE_SUSPEND  */
  ACE_YYSYMBOL_ACE_RESUME = 6,                 /* ACE_RESUME  */
  ACE_YYSYMBOL_ACE_REMOVE = 7,                 /* ACE_REMOVE  */
  ACE_YYSYMBOL_ACE_USTREAM = 8,                /* ACE_USTREAM  */
  ACE_YYSYMBOL_ACE_MODULE_T = 9,               /* ACE_MODULE_T  */
  ACE_YYSYMBOL_ACE_STREAM_T = 10,              /* ACE_STREAM_T  */
  ACE_YYSYMBOL_ACE_SVC_OBJ_T = 11,             /* ACE_SVC_OBJ_T  */
  ACE_YYSYMBOL_ACE_ACTIVE = 12,                /* ACE_ACTIVE  */
  ACE_YYSYMBOL_ACE_INACTIVE = 13,              /* ACE_INACTIVE  */
  ACE_YYSYMBOL_ACE_PATHNAME = 14,              /* ACE_PATHNAME  */
  ACE_YYSYMBOL_ACE_IDENT = 15,                 /* ACE_IDENT  */
  ACE_YYSYMBOL_ACE_STRING = 16,                /* ACE_STRING  */
  ACE_YYSYMBOL_17_ = 17,                       /* '{'  */
  ACE_YYSYMBOL_18_ = 18,                       /* '}'  */
  ACE_YYSYMBOL_19_ = 19,                       /* ':'  */
  ACE_YYSYMBOL_20_ = 20,                       /* '('  */
  ACE_YYSYMBOL_21_ = 21,                       /* ')'  */
  ACE_YYSYMBOL_22_ = 22,                       /* '*'  */
  ACE_YYSYMBOL_ACE_YYACCEPT = 23,                  /* $accept  */
  ACE_YYSYMBOL_svc_config_entries = 24,        /* svc_config_entries  */
  ACE_YYSYMBOL_svc_config_entry = 25,          /* svc_config_entry  */
  ACE_YYSYMBOL_dynamic = 26,                   /* dynamic  */
  ACE_YYSYMBOL_static = 27,                    /* static  */
  ACE_YYSYMBOL_suspend = 28,                   /* suspend  */
  ACE_YYSYMBOL_resume = 29,                    /* resume  */
  ACE_YYSYMBOL_remove = 30,                    /* remove  */
  ACE_YYSYMBOL_stream = 31,                    /* stream  */
  ACE_YYSYMBOL_32_1 = 32,                      /* @1  */
  ACE_YYSYMBOL_stream_ops = 33,                /* stream_ops  */
  ACE_YYSYMBOL_stream_modules = 34,            /* stream_modules  */
  ACE_YYSYMBOL_35_2 = 35,                      /* @2  */
  ACE_YYSYMBOL_module_list = 36,               /* module_list  */
  ACE_YYSYMBOL_module = 37,                    /* module  */
  ACE_YYSYMBOL_svc_location = 38,              /* svc_location  */
  ACE_YYSYMBOL_status = 39,                    /* status  */
  ACE_YYSYMBOL_svc_initializer = 40,           /* svc_initializer  */
  ACE_YYSYMBOL_type = 41,                      /* type  */
  ACE_YYSYMBOL_parameters_opt = 42,            /* parameters_opt  */
  ACE_YYSYMBOL_pathname = 43                   /* pathname  */
};
typedef enum ace_yysymbol_kind_t ace_yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define ACE_YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ ace_yytype_int8;
#elif defined ACE_YY_STDINT_H
typedef int_least8_t ace_yytype_int8;
#else
typedef signed char ace_yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ ace_yytype_int16;
#elif defined ACE_YY_STDINT_H
typedef int_least16_t ace_yytype_int16;
#else
typedef short ace_yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ ace_yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined ACE_YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t ace_yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char ace_yytype_uint8;
#else
typedef short ace_yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ ace_yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined ACE_YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t ace_yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short ace_yytype_uint16;
#else
typedef int ace_yytype_uint16;
#endif

#ifndef ACE_YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define ACE_YYPTRDIFF_T __PTRDIFF_TYPE__
#  define ACE_YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define ACE_YYPTRDIFF_T ptrdiff_t
#  define ACE_YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define ACE_YYPTRDIFF_T long
#  define ACE_YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef ACE_YYSIZE_T
# ifdef __SIZE_TYPE__
#  define ACE_YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define ACE_YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define ACE_YYSIZE_T size_t
# else
#  define ACE_YYSIZE_T unsigned
# endif
#endif

#define ACE_YYSIZE_MAXIMUM                                  \
  ACE_YY_CAST (ACE_YYPTRDIFF_T,                                 \
           (ACE_YYPTRDIFF_MAXIMUM < ACE_YY_CAST (ACE_YYSIZE_T, -1)  \
            ? ACE_YYPTRDIFF_MAXIMUM                         \
            : ACE_YY_CAST (ACE_YYSIZE_T, -1)))

#define ACE_YYSIZEOF(X) ACE_YY_CAST (ACE_YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef ace_yytype_int8 ace_yy_state_t;

/* State numbers in computations.  */
typedef int ace_yy_state_fast_t;

#ifndef ACE_YY_
# if defined ACE_YYENABLE_NLS && ACE_YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define ACE_YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef ACE_YY_
#  define ACE_YY_(Msgid) Msgid
# endif
#endif


#ifndef ACE_YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define ACE_YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define ACE_YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef ACE_YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define ACE_YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define ACE_YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define ACE_YY_USE(E) ((void) (E))
#else
# define ACE_YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about ace_yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define ACE_YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define ACE_YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define ACE_YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define ACE_YY_INITIAL_VALUE(Value) Value
#endif
#ifndef ACE_YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define ACE_YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define ACE_YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef ACE_YY_INITIAL_VALUE
# define ACE_YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define ACE_YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define ACE_YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef ACE_YY_IGNORE_USELESS_CAST_BEGIN
# define ACE_YY_IGNORE_USELESS_CAST_BEGIN
# define ACE_YY_IGNORE_USELESS_CAST_END
#endif


#define ACE_YY_ASSERT(E) ((void) (0 && (E)))

#if !defined ace_yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef ACE_YYSTACK_USE_ALLOCA
#  if ACE_YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define ACE_YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define ACE_YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define ACE_YYSTACK_ALLOC alloca
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

# ifdef ACE_YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define ACE_YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef ACE_YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define ACE_YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define ACE_YYSTACK_ALLOC ACE_YYMALLOC
#  define ACE_YYSTACK_FREE ACE_YYFREE
#  ifndef ACE_YYSTACK_ALLOC_MAXIMUM
#   define ACE_YYSTACK_ALLOC_MAXIMUM ACE_YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined ACE_YYMALLOC || defined malloc) \
             && (defined ACE_YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef ACE_YYMALLOC
#   define ACE_YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (ACE_YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef ACE_YYFREE
#   define ACE_YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined ace_yyoverflow */

#if (! defined ace_yyoverflow \
     && (! defined __cplusplus \
         || (defined ACE_YYSTYPE_IS_TRIVIAL && ACE_YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union ace_yyalloc
{
  ace_yy_state_t ace_yyss_alloc;
  ACE_YYSTYPE ace_yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define ACE_YYSTACK_GAP_MAXIMUM (ACE_YYSIZEOF (union ace_yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define ACE_YYSTACK_BYTES(N) \
     ((N) * (ACE_YYSIZEOF (ace_yy_state_t) + ACE_YYSIZEOF (ACE_YYSTYPE)) \
      + ACE_YYSTACK_GAP_MAXIMUM)

# define ACE_YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables ACE_YYSIZE and ACE_YYSTACKSIZE give the old and new number of
   elements in the stack, and ACE_YYPTR gives the new location of the
   stack.  Advance ACE_YYPTR to a properly aligned location for the next
   stack.  */
# define ACE_YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        ACE_YYPTRDIFF_T ace_yynewbytes;                                         \
        ACE_YYCOPY (&ace_yyptr->Stack_alloc, Stack, ace_yysize);                    \
        Stack = &ace_yyptr->Stack_alloc;                                    \
        ace_yynewbytes = ace_yystacksize * ACE_YYSIZEOF (*Stack) + ACE_YYSTACK_GAP_MAXIMUM; \
        ace_yyptr += ace_yynewbytes / ACE_YYSIZEOF (*ace_yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined ACE_YYCOPY_NEEDED && ACE_YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef ACE_YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define ACE_YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, ACE_YY_CAST (ACE_YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define ACE_YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          ACE_YYPTRDIFF_T ace_yyi;                      \
          for (ace_yyi = 0; ace_yyi < (Count); ace_yyi++)   \
            (Dst)[ace_yyi] = (Src)[ace_yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !ACE_YYCOPY_NEEDED */

/* ACE_YYFINAL -- State number of the termination state.  */
#define ACE_YYFINAL  2
/* ACE_YYLAST -- Last index in ACE_YYTABLE.  */
#define ACE_YYLAST   62

/* ACE_YYNTOKENS -- Number of terminals.  */
#define ACE_YYNTOKENS  23
/* ACE_YYNNTS -- Number of nonterminals.  */
#define ACE_YYNNTS  21
/* ACE_YYNRULES -- Number of rules.  */
#define ACE_YYNRULES  45
/* ACE_YYNSTATES -- Number of states.  */
#define ACE_YYNSTATES  66

/* ACE_YYMAXUTOK -- Last valid token kind.  */
#define ACE_YYMAXUTOK   271


/* ACE_YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by ace_yylex, with out-of-bounds checking.  */
#define ACE_YYTRANSLATE(ACE_YYX)                                \
  (0 <= (ACE_YYX) && (ACE_YYX) <= ACE_YYMAXUTOK                     \
   ? ACE_YY_CAST (ace_yysymbol_kind_t, ace_yytranslate[ACE_YYX])        \
   : ACE_YYSYMBOL_ACE_YYUNDEF)

/* ACE_YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by ace_yylex.  */
static const ace_yytype_int8 ace_yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      20,    21,    22,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    19,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    17,     2,    18,     2,     2,     2,     2,
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
      15,    16
};

#if ACE_YYDEBUG
/* ACE_YYRLINE[ACE_YYN] -- Source line where rule number ACE_YYN was defined.  */
static const ace_yytype_int16 ace_yyrline[] =
{
       0,    77,    77,    86,    90,    94,    95,    96,    97,    98,
      99,   103,   113,   120,   127,   134,   141,   145,   145,   152,
     155,   162,   161,   170,   174,   182,   186,   189,   202,   211,
     220,   242,   249,   253,   258,   264,   268,   272,   279,   283,
     287,   294,   295,   299,   300,   301
};
#endif

/** Accessing symbol of state STATE.  */
#define ACE_YY_ACCESSING_SYMBOL(State) ACE_YY_CAST (ace_yysymbol_kind_t, ace_yystos[State])

#if ACE_YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   ACE_YYSYMBOL.  No bounds checking.  */
static const char *ace_yysymbol_name (ace_yysymbol_kind_t ace_yysymbol) ACE_YY_ATTRIBUTE_UNUSED;

/* ACE_YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at ACE_YYNTOKENS, nonterminals.  */
static const char *const ace_yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ACE_DYNAMIC",
  "ACE_STATIC", "ACE_SUSPEND", "ACE_RESUME", "ACE_REMOVE", "ACE_USTREAM",
  "ACE_MODULE_T", "ACE_STREAM_T", "ACE_SVC_OBJ_T", "ACE_ACTIVE",
  "ACE_INACTIVE", "ACE_PATHNAME", "ACE_IDENT", "ACE_STRING", "'{'", "'}'",
  "':'", "'('", "')'", "'*'", "$accept", "svc_config_entries",
  "svc_config_entry", "dynamic", "static", "suspend", "resume", "remove",
  "stream", "@1", "stream_ops", "stream_modules", "@2", "module_list",
  "module", "svc_location", "status", "svc_initializer", "type",
  "parameters_opt", "pathname", ACE_YY_NULLPTR
};

static const char *
ace_yysymbol_name (ace_yysymbol_kind_t ace_yysymbol)
{
  return ace_yytname[ace_yysymbol];
}
#endif

#define ACE_YYPACT_NINF (-13)

#define ace_yypact_value_is_default(Yyn) \
  ((Yyn) == ACE_YYPACT_NINF)

#define ACE_YYTABLE_NINF (-1)

#define ace_yytable_value_is_error(Yyn) \
  0

/* ACE_YYPACT[STATE-NUM] -- Index in ACE_YYTABLE of the portion describing
   STATE-NUM.  */
static const ace_yytype_int8 ace_yypact[] =
{
     -13,    20,   -13,   -13,     1,     3,     7,    14,    18,     4,
     -13,   -13,   -13,   -13,   -13,   -13,   -13,    21,    19,    19,
     -13,   -13,   -13,   -13,   -13,   -13,    -2,    12,    15,    16,
      -5,   -13,   -13,   -13,    -2,   -13,   -13,   -13,   -13,   -13,
     -13,   -13,   -13,    24,     0,    17,   -13,   -13,    22,   -13,
     -13,   -13,    25,    -1,    26,    23,   -13,   -13,   -13,   -13,
     -13,   -13,   -13,   -13,    27,   -13
};

/* ACE_YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when ACE_YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const ace_yytype_int8 ace_yydefact[] =
{
       4,     0,     1,     3,     0,     0,     0,     0,     0,     0,
       2,     5,     6,     7,     8,     9,    10,     0,    42,    42,
      13,    14,    15,    17,    19,    20,    23,     0,     0,     0,
       0,    41,    11,    12,    23,    21,    16,    38,    40,    39,
      43,    44,    45,     0,    34,     0,    18,    25,     0,    32,
      33,    31,     0,     0,     0,    35,    22,    26,    27,    28,
      29,    30,    24,    37,     0,    36
};

/* ACE_YYPGOTO[NTERM-NUM].  */
static const ace_yytype_int8 ace_yypgoto[] =
{
     -13,   -13,   -13,    -9,    -8,   -12,    -7,    -4,   -13,   -13,
     -13,    28,   -13,   -13,   -13,   -13,   -13,   -13,   -13,    31,
     -13
};

/* ACE_YYDEFGOTO[NTERM-NUM].  */
static const ace_yytype_int8 ace_yydefgoto[] =
{
       0,     1,    10,    11,    12,    13,    14,    15,    16,    34,
      26,    36,    47,    53,    62,    18,    51,    44,    30,    32,
      45
};

/* ACE_YYTABLE[ACE_YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If ACE_YYTABLE_NINF, syntax error.  */
static const ace_yytype_int8 ace_yytable[] =
{
      24,    25,     4,     5,     6,     7,     8,     4,     5,    40,
      41,    42,    49,    50,    43,    35,    17,    56,    19,    23,
       2,     3,    20,     4,     5,     6,     7,     8,     9,    21,
      27,    28,    29,    22,    37,    31,    52,    38,    39,    48,
      55,    59,    54,    64,    57,    58,    60,    63,    65,    61,
      33,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    46
};

static const ace_yytype_int8 ace_yycheck[] =
{
       9,     9,     3,     4,     5,     6,     7,     3,     4,    14,
      15,    16,    12,    13,    19,    17,    15,    18,    15,    15,
       0,     1,    15,     3,     4,     5,     6,     7,     8,    15,
       9,    10,    11,    15,    22,    16,    19,    22,    22,    15,
      15,    53,    20,    20,    53,    53,    53,    21,    21,    53,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34
};

/* ACE_YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const ace_yytype_int8 ace_yystos[] =
{
       0,    24,     0,     1,     3,     4,     5,     6,     7,     8,
      25,    26,    27,    28,    29,    30,    31,    15,    38,    15,
      15,    15,    15,    15,    26,    27,    33,     9,    10,    11,
      41,    16,    42,    42,    32,    17,    34,    22,    22,    22,
      14,    15,    16,    19,    40,    43,    34,    35,    15,    12,
      13,    39,    19,    36,    20,    15,    18,    26,    27,    28,
      29,    30,    37,    21,    20,    21
};

/* ACE_YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const ace_yytype_int8 ace_yyr1[] =
{
       0,    23,    24,    24,    24,    25,    25,    25,    25,    25,
      25,    26,    27,    28,    29,    30,    31,    32,    31,    33,
      33,    35,    34,    34,    36,    36,    37,    37,    37,    37,
      37,    38,    39,    39,    39,    40,    40,    40,    41,    41,
      41,    42,    42,    43,    43,    43
};

/* ACE_YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const ace_yytype_int8 ace_yyr2[] =
{
       0,     2,     2,     2,     0,     1,     1,     1,     1,     1,
       1,     3,     3,     2,     2,     2,     3,     0,     4,     1,
       1,     0,     4,     0,     2,     0,     1,     1,     1,     1,
       1,     4,     1,     1,     0,     3,     5,     4,     2,     2,
       2,     1,     0,     1,     1,     1
};


enum { ACE_YYENOMEM = -2 };

#define ace_yyerrok         (ace_yyerrstatus = 0)
#define ace_yyclearin       (ace_yychar = ACE_YYEMPTY)

#define ACE_YYACCEPT        goto ace_yyacceptlab
#define ACE_YYABORT         goto ace_yyabortlab
#define ACE_YYERROR         goto ace_yyerrorlab
#define ACE_YYNOMEM         goto ace_yyexhaustedlab


#define ACE_YYRECOVERING()  (!!ace_yyerrstatus)

#define ACE_YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (ace_yychar == ACE_YYEMPTY)                                        \
      {                                                           \
        ace_yychar = (Token);                                         \
        ace_yylval = (Value);                                         \
        ACE_YYPOPSTACK (ace_yylen);                                       \
        ace_yystate = *ace_yyssp;                                         \
        goto ace_yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        ace_yyerror (ACE_YYLEX_PARAM, ACE_YY_("syntax error: cannot back up")); \
        ACE_YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use ACE_YYerror or ACE_YYUNDEF. */
#define ACE_YYERRCODE ACE_YYUNDEF


/* Enable debugging if requested.  */
#if ACE_YYDEBUG

# ifndef ACE_YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define ACE_YYFPRINTF ACE_OS::fprintf
# endif

# define ACE_YYDPRINTF(Args)                        \
do {                                            \
  if (ace_yydebug)                                  \
    ACE_YYFPRINTF Args;                             \
} while (0)




# define ACE_YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (ace_yydebug)                                                            \
    {                                                                     \
      ACE_YYFPRINTF (stderr, "%s ", Title);                                   \
      ace_yy_symbol_print (stderr,                                            \
                  Kind, Value, ACE_YYLEX_PARAM); \
      ACE_YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on ACE_YYO.  |
`-----------------------------------*/

static void
ace_yy_symbol_value_print (FILE *ace_yyo,
                       ace_yysymbol_kind_t ace_yykind, ACE_YYSTYPE const * const ace_yyvaluep, void *ACE_YYLEX_PARAM)
{
  FILE *ace_yyoutput = ace_yyo;
  ACE_YY_USE (ace_yyoutput);
  ACE_YY_USE (ACE_YYLEX_PARAM);
  if (!ace_yyvaluep)
    return;
  ACE_YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  ACE_YY_USE (ace_yykind);
  ACE_YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on ACE_YYO.  |
`---------------------------*/

static void
ace_yy_symbol_print (FILE *ace_yyo,
                 ace_yysymbol_kind_t ace_yykind, ACE_YYSTYPE const * const ace_yyvaluep, void *ACE_YYLEX_PARAM)
{
  ACE_YYFPRINTF (ace_yyo, "%s %s (",
             ace_yykind < ACE_YYNTOKENS ? "token" : "nterm", ace_yysymbol_name (ace_yykind));

  ace_yy_symbol_value_print (ace_yyo, ace_yykind, ace_yyvaluep, ACE_YYLEX_PARAM);
  ACE_YYFPRINTF (ace_yyo, ")");
}

/*------------------------------------------------------------------.
| ace_yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
ace_yy_stack_print (ace_yy_state_t *ace_yybottom, ace_yy_state_t *ace_yytop)
{
  ACE_YYFPRINTF (stderr, "Stack now");
  for (; ace_yybottom <= ace_yytop; ace_yybottom++)
    {
      int ace_yybot = *ace_yybottom;
      ACE_YYFPRINTF (stderr, " %d", ace_yybot);
    }
  ACE_YYFPRINTF (stderr, "\n");
}

# define ACE_YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (ace_yydebug)                                                  \
    ace_yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the ACE_YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
ace_yy_reduce_print (ace_yy_state_t *ace_yyssp, ACE_YYSTYPE *ace_yyvsp,
                 int ace_yyrule, void *ACE_YYLEX_PARAM)
{
  int ace_yylno = ace_yyrline[ace_yyrule];
  int ace_yynrhs = ace_yyr2[ace_yyrule];
  int ace_yyi;
  ACE_YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             ace_yyrule - 1, ace_yylno);
  /* The symbols being reduced.  */
  for (ace_yyi = 0; ace_yyi < ace_yynrhs; ace_yyi++)
    {
      ACE_YYFPRINTF (stderr, "   $%d = ", ace_yyi + 1);
      ace_yy_symbol_print (stderr,
                       ACE_YY_ACCESSING_SYMBOL (+ace_yyssp[ace_yyi + 1 - ace_yynrhs]),
                       &ace_yyvsp[(ace_yyi + 1) - (ace_yynrhs)], ACE_YYLEX_PARAM);
      ACE_YYFPRINTF (stderr, "\n");
    }
}

# define ACE_YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (ace_yydebug)                          \
    ace_yy_reduce_print (ace_yyssp, ace_yyvsp, Rule, ACE_YYLEX_PARAM); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int ace_yydebug;
#else /* !ACE_YYDEBUG */
# define ACE_YYDPRINTF(Args) ((void) 0)
# define ACE_YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define ACE_YY_STACK_PRINT(Bottom, Top)
# define ACE_YY_REDUCE_PRINT(Rule)
#endif /* !ACE_YYDEBUG */


/* ACE_YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef ACE_YYINITDEPTH
# define ACE_YYINITDEPTH 200
#endif

/* ACE_YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   ACE_YYSTACK_ALLOC_MAXIMUM < ACE_YYSTACK_BYTES (ACE_YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef ACE_YYMAXDEPTH
# define ACE_YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
ace_yydestruct (const char *ace_yymsg,
            ace_yysymbol_kind_t ace_yykind, ACE_YYSTYPE *ace_yyvaluep, void *ACE_YYLEX_PARAM)
{
  ACE_YY_USE (ace_yyvaluep);
  ACE_YY_USE (ACE_YYLEX_PARAM);
  if (!ace_yymsg)
    ace_yymsg = "Deleting";
  ACE_YY_SYMBOL_PRINT (ace_yymsg, ace_yykind, ace_yyvaluep, ace_yylocationp);

  ACE_YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  ACE_YY_USE (ace_yykind);
  ACE_YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| ace_yyparse.  |
`----------*/

int
ace_yyparse (void *ACE_YYLEX_PARAM)
{
/* Lookahead token kind.  */
int ace_yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
ACE_YY_INITIAL_VALUE (static ACE_YYSTYPE ace_yyval_default;)
ACE_YYSTYPE ace_yylval ACE_YY_INITIAL_VALUE (= ace_yyval_default);

    /* Number of syntax errors so far.  */
    int ace_yynerrs = 0;

    ace_yy_state_fast_t ace_yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int ace_yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow ace_yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    ACE_YYPTRDIFF_T ace_yystacksize = ACE_YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    ace_yy_state_t ace_yyssa[ACE_YYINITDEPTH];
    ace_yy_state_t *ace_yyss = ace_yyssa;
    ace_yy_state_t *ace_yyssp = ace_yyss;

    /* The semantic value stack: array, bottom, top.  */
    ACE_YYSTYPE ace_yyvsa[ACE_YYINITDEPTH];
    ACE_YYSTYPE *ace_yyvs = ace_yyvsa;
    ACE_YYSTYPE *ace_yyvsp = ace_yyvs;

  int ace_yyn;
  /* The return value of ace_yyparse.  */
  int ace_yyresult;
  /* Lookahead symbol kind.  */
  ace_yysymbol_kind_t ace_yytoken = ACE_YYSYMBOL_ACE_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  ACE_YYSTYPE ace_yyval;



#define ACE_YYPOPSTACK(N)   (ace_yyvsp -= (N), ace_yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int ace_yylen = 0;

  ACE_YYDPRINTF ((stderr, "Starting parse\n"));

  ace_yychar = ACE_YYEMPTY; /* Cause a token to be read.  */

  goto ace_yysetstate;


/*------------------------------------------------------------.
| ace_yynewstate -- push a new state, which is found in ace_yystate.  |
`------------------------------------------------------------*/
ace_yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  ace_yyssp++;


/*--------------------------------------------------------------------.
| ace_yysetstate -- set current state (the top of the stack) to ace_yystate.  |
`--------------------------------------------------------------------*/
ace_yysetstate:
  ACE_YYDPRINTF ((stderr, "Entering state %d\n", ace_yystate));
  ACE_YY_ASSERT (0 <= ace_yystate && ace_yystate < ACE_YYNSTATES);
  ACE_YY_IGNORE_USELESS_CAST_BEGIN
  *ace_yyssp = ACE_YY_CAST (ace_yy_state_t, ace_yystate);
  ACE_YY_IGNORE_USELESS_CAST_END
  ACE_YY_STACK_PRINT (ace_yyss, ace_yyssp);

  if (ace_yyss + ace_yystacksize - 1 <= ace_yyssp)
#if !defined ace_yyoverflow && !defined ACE_YYSTACK_RELOCATE
    ACE_YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      ACE_YYPTRDIFF_T ace_yysize = ace_yyssp - ace_yyss + 1;

# if defined ace_yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        ace_yy_state_t *ace_yyss1 = ace_yyss;
        ACE_YYSTYPE *ace_yyvs1 = ace_yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if ace_yyoverflow is a macro.  */
        ace_yyoverflow (ACE_YY_("memory exhausted"),
                    &ace_yyss1, ace_yysize * ACE_YYSIZEOF (*ace_yyssp),
                    &ace_yyvs1, ace_yysize * ACE_YYSIZEOF (*ace_yyvsp),
                    &ace_yystacksize);
        ace_yyss = ace_yyss1;
        ace_yyvs = ace_yyvs1;
      }
# else /* defined ACE_YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (ACE_YYMAXDEPTH <= ace_yystacksize)
        ACE_YYNOMEM;
      ace_yystacksize *= 2;
      if (ACE_YYMAXDEPTH < ace_yystacksize)
        ace_yystacksize = ACE_YYMAXDEPTH;

      {
        ace_yy_state_t *ace_yyss1 = ace_yyss;
        union ace_yyalloc *ace_yyptr =
          ACE_YY_CAST (union ace_yyalloc *,
                   ACE_YYSTACK_ALLOC (ACE_YY_CAST (ACE_YYSIZE_T, ACE_YYSTACK_BYTES (ace_yystacksize))));
        if (! ace_yyptr)
          ACE_YYNOMEM;
        ACE_YYSTACK_RELOCATE (ace_yyss_alloc, ace_yyss);
        ACE_YYSTACK_RELOCATE (ace_yyvs_alloc, ace_yyvs);
#  undef ACE_YYSTACK_RELOCATE
        if (ace_yyss1 != ace_yyssa)
          ACE_YYSTACK_FREE (ace_yyss1);
      }
# endif

      ace_yyssp = ace_yyss + ace_yysize - 1;
      ace_yyvsp = ace_yyvs + ace_yysize - 1;

      ACE_YY_IGNORE_USELESS_CAST_BEGIN
      ACE_YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  ACE_YY_CAST (long, ace_yystacksize)));
      ACE_YY_IGNORE_USELESS_CAST_END

      if (ace_yyss + ace_yystacksize - 1 <= ace_yyssp)
        ACE_YYABORT;
    }
#endif /* !defined ace_yyoverflow && !defined ACE_YYSTACK_RELOCATE */


  if (ace_yystate == ACE_YYFINAL)
    ACE_YYACCEPT;

  goto ace_yybackup;


/*-----------.
| ace_yybackup.  |
`-----------*/
ace_yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  ace_yyn = ace_yypact[ace_yystate];
  if (ace_yypact_value_is_default (ace_yyn))
    goto ace_yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* ACE_YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (ace_yychar == ACE_YYEMPTY)
    {
      ACE_YYDPRINTF ((stderr, "Reading a token\n"));
      ace_yychar = ace_yylex (&ace_yylval, ACE_YYLEX_PARAM);
    }

  if (ace_yychar <= ACE_YYEOF)
    {
      ace_yychar = ACE_YYEOF;
      ace_yytoken = ACE_YYSYMBOL_ACE_YYEOF;
      ACE_YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (ace_yychar == ACE_YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      ace_yychar = ACE_YYUNDEF;
      ace_yytoken = ACE_YYSYMBOL_ACE_YYerror;
      goto ace_yyerrlab1;
    }
  else
    {
      ace_yytoken = ACE_YYTRANSLATE (ace_yychar);
      ACE_YY_SYMBOL_PRINT ("Next token is", ace_yytoken, &ace_yylval, &ace_yylloc);
    }

  /* If the proper action on seeing token ACE_YYTOKEN is to reduce or to
     detect an error, take that action.  */
  ace_yyn += ace_yytoken;
  if (ace_yyn < 0 || ACE_YYLAST < ace_yyn || ace_yycheck[ace_yyn] != ace_yytoken)
    goto ace_yydefault;
  ace_yyn = ace_yytable[ace_yyn];
  if (ace_yyn <= 0)
    {
      if (ace_yytable_value_is_error (ace_yyn))
        goto ace_yyerrlab;
      ace_yyn = -ace_yyn;
      goto ace_yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (ace_yyerrstatus)
    ace_yyerrstatus--;

  /* Shift the lookahead token.  */
  ACE_YY_SYMBOL_PRINT ("Shifting", ace_yytoken, &ace_yylval, &ace_yylloc);
  ace_yystate = ace_yyn;
  ACE_YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++ace_yyvsp = ace_yylval;
  ACE_YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  ace_yychar = ACE_YYEMPTY;
  goto ace_yynewstate;


/*-----------------------------------------------------------.
| ace_yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
ace_yydefault:
  ace_yyn = ace_yydefact[ace_yystate];
  if (ace_yyn == 0)
    goto ace_yyerrlab;
  goto ace_yyreduce;


/*-----------------------------.
| ace_yyreduce -- do a reduction.  |
`-----------------------------*/
ace_yyreduce:
  /* ace_yyn is the number of a rule to reduce with.  */
  ace_yylen = ace_yyr2[ace_yyn];

  /* If ACE_YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets ACE_YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to ACE_YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that ACE_YYVAL may be used uninitialized.  */
  ace_yyval = ace_yyvsp[1-ace_yylen];


  ACE_YY_REDUCE_PRINT (ace_yyn);
  switch (ace_yyn)
    {
  case 2: /* svc_config_entries: svc_config_entries svc_config_entry  */
    {
      if ((ace_yyvsp[0].parse_node_) != 0)
      {
        (ace_yyvsp[0].parse_node_)->apply (ACE_SVC_CONF_PARAM->config, ACE_SVC_CONF_PARAM->yyerrno);
        delete (ace_yyvsp[0].parse_node_);
      }
      ACE_SVC_CONF_PARAM->obstack.release ();
    }
    break;

  case 3: /* svc_config_entries: svc_config_entries error  */
    {
      ACE_SVC_CONF_PARAM->obstack.release ();
    }
    break;

  case 11: /* dynamic: ACE_DYNAMIC svc_location parameters_opt  */
    {
      if ((ace_yyvsp[-1].svc_record_) != 0)
        (ace_yyval.parse_node_) = new ACE_Dynamic_Node ((ace_yyvsp[-1].svc_record_), (ace_yyvsp[0].ident_));
      else
        (ace_yyval.parse_node_) = 0;
    }
    break;

  case 12: /* static: ACE_STATIC ACE_IDENT parameters_opt  */
    {
      (ace_yyval.parse_node_) = new ACE_Static_Node ((ace_yyvsp[-1].ident_), (ace_yyvsp[0].ident_));
    }
    break;

  case 13: /* suspend: ACE_SUSPEND ACE_IDENT  */
    {
      (ace_yyval.parse_node_) = new ACE_Suspend_Node ((ace_yyvsp[0].ident_));
    }
    break;

  case 14: /* resume: ACE_RESUME ACE_IDENT  */
    {
      (ace_yyval.parse_node_) = new ACE_Resume_Node ((ace_yyvsp[0].ident_));
    }
    break;

  case 15: /* remove: ACE_REMOVE ACE_IDENT  */
    {
      (ace_yyval.parse_node_) = new ACE_Remove_Node ((ace_yyvsp[0].ident_));
    }
    break;

  case 16: /* stream: ACE_USTREAM stream_ops stream_modules  */
    {
      (ace_yyval.parse_node_) = new ACE_Stream_Node ((ace_yyvsp[-1].static_node_), (ace_yyvsp[0].parse_node_));
    }
    break;

  case 17: /* @1: %empty  */
                          { (ace_yyval.static_node_) = new ACE_Static_Node ((ace_yyvsp[0].ident_)); }
    break;

  case 18: /* stream: ACE_USTREAM ACE_IDENT @1 stream_modules  */
    {
      (ace_yyval.parse_node_) = new ACE_Dummy_Node ((ace_yyvsp[-1].static_node_), (ace_yyvsp[0].parse_node_));
    }
    break;

  case 19: /* stream_ops: dynamic  */
    {
    }
    break;

  case 20: /* stream_ops: static  */
    {
    }
    break;

  case 21: /* @2: %empty  */
    {
      // Initialize left context...
      (ace_yyval.static_node_) = (ace_yyvsp[-1].static_node_);
    }
    break;

  case 22: /* stream_modules: '{' @2 module_list '}'  */
    {
      (ace_yyval.parse_node_) = (ace_yyvsp[-1].parse_node_);
    }
    break;

  case 23: /* stream_modules: %empty  */
                { (ace_yyval.parse_node_) = 0; }
    break;

  case 24: /* module_list: module_list module  */
    {
      if ((ace_yyvsp[0].parse_node_) != 0)
        {
          (ace_yyvsp[0].parse_node_)->link ((ace_yyvsp[-1].parse_node_));
          (ace_yyval.parse_node_) = (ace_yyvsp[0].parse_node_);
        }
    }
    break;

  case 25: /* module_list: %empty  */
                { (ace_yyval.parse_node_) = 0; }
    break;

  case 26: /* module: dynamic  */
    {
            }
    break;

  case 27: /* module: static  */
    {
      ACE_Static_Node *sn = (ace_yyvsp[(-1) - (1)].static_node_);
      ACE_Module_Type *mt = ace_get_module (sn->record (ACE_SVC_CONF_PARAM->config),
                                            (ace_yyvsp[0].static_node_)->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);

      if (((ACE_Stream_Type *) sn->record (ACE_SVC_CONF_PARAM->config)->type ())->push (mt) == -1)
        {
          ACELIB_ERROR ((LM_ERROR, ACE_TEXT ("Problem with static\n")));
          ACE_SVC_CONF_PARAM->yyerrno++;
        }
    }
    break;

  case 28: /* module: suspend  */
    {
      ACE_Static_Node *sn = (ace_yyvsp[(-1) - (1)].static_node_);
      ACE_Module_Type *mt = ace_get_module (sn->record (ACE_SVC_CONF_PARAM->config),
                                            sn->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);
      if (mt != 0)
        mt->suspend ();
    }
    break;

  case 29: /* module: resume  */
    {
      ACE_Static_Node *sn = (ace_yyvsp[(-1) - (1)].static_node_);
      ACE_Module_Type *mt = ace_get_module (sn->record (ACE_SVC_CONF_PARAM->config),
                                            (ace_yyvsp[0].static_node_)->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);
      if (mt != 0)
        mt->resume ();
    }
    break;

  case 30: /* module: remove  */
    {
      ACE_Static_Node *stream = (ace_yyvsp[(-1) - (1)].static_node_);
      ACE_Static_Node *module = (ace_yyvsp[0].static_node_);
      ACE_Module_Type *mt = ace_get_module (stream->record (ACE_SVC_CONF_PARAM->config),
                                            module->name (),
                                            ACE_SVC_CONF_PARAM->yyerrno);

      ACE_Stream_Type *st =
        dynamic_cast<ACE_Stream_Type *> (const_cast<ACE_Service_Type_Impl *> (stream->record (ACE_SVC_CONF_PARAM->config)->type ()));
      if (!st || (mt != 0 && st->remove (mt) == -1))
        {
          ACELIB_ERROR ((LM_ERROR,
                         ACE_TEXT ("cannot remove Module_Type %s from STREAM_Type %s\n"),
                         module->name (),
                         stream->name ()));
          ACE_SVC_CONF_PARAM->yyerrno++;
        }
    }
    break;

  case 31: /* svc_location: ACE_IDENT type svc_initializer status  */
    {
      (ace_yyval.svc_record_) = new ACE_Service_Type_Factory ((ace_yyvsp[-3].ident_), (ace_yyvsp[-2].type_), (ace_yyvsp[-1].location_node_), (ace_yyvsp[0].type_));
    }
    break;

  case 32: /* status: ACE_ACTIVE  */
    {
      (ace_yyval.type_) = 1;
    }
    break;

  case 33: /* status: ACE_INACTIVE  */
    {
      (ace_yyval.type_) = 0;
    }
    break;

  case 34: /* status: %empty  */
    {
      (ace_yyval.type_) = 1;
    }
    break;

  case 35: /* svc_initializer: pathname ':' ACE_IDENT  */
    {
      (ace_yyval.location_node_) = new ACE_Object_Node ((ace_yyvsp[-2].ident_), (ace_yyvsp[0].ident_));
    }
    break;

  case 36: /* svc_initializer: pathname ':' ACE_IDENT '(' ')'  */
    {
      (ace_yyval.location_node_) = new ACE_Function_Node ((ace_yyvsp[-4].ident_), (ace_yyvsp[-2].ident_));
    }
    break;

  case 37: /* svc_initializer: ':' ACE_IDENT '(' ')'  */
    {
      (ace_yyval.location_node_) = new ACE_Static_Function_Node ((ace_yyvsp[-2].ident_));
    }
    break;

  case 38: /* type: ACE_MODULE_T '*'  */
    {
      (ace_yyval.type_) = ACE_MODULE_T;
    }
    break;

  case 39: /* type: ACE_SVC_OBJ_T '*'  */
    {
      (ace_yyval.type_) = ACE_SVC_OBJ_T;
    }
    break;

  case 40: /* type: ACE_STREAM_T '*'  */
    {
      (ace_yyval.type_) = ACE_STREAM_T;
    }
    break;

  case 42: /* parameters_opt: %empty  */
                { (ace_yyval.ident_) = 0; }
    break;



      default: break;
    }
  /* User semantic actions sometimes alter ace_yychar, and that requires
     that ace_yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of ace_yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     ACE_YYABORT, ACE_YYACCEPT, or ACE_YYERROR immediately after altering ace_yychar or
     if it invokes ACE_YYBACKUP.  In the case of ACE_YYABORT or ACE_YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of ACE_YYERROR or ACE_YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  ACE_YY_SYMBOL_PRINT ("-> $$ =", ACE_YY_CAST (ace_yysymbol_kind_t, ace_yyr1[ace_yyn]), &ace_yyval, &ace_yyloc);

  ACE_YYPOPSTACK (ace_yylen);
  ace_yylen = 0;

  *++ace_yyvsp = ace_yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int ace_yylhs = ace_yyr1[ace_yyn] - ACE_YYNTOKENS;
    const int ace_yyi = ace_yypgoto[ace_yylhs] + *ace_yyssp;
    ace_yystate = (0 <= ace_yyi && ace_yyi <= ACE_YYLAST && ace_yycheck[ace_yyi] == *ace_yyssp
               ? ace_yytable[ace_yyi]
               : ace_yydefgoto[ace_yylhs]);
  }

  goto ace_yynewstate;


/*--------------------------------------.
| ace_yyerrlab -- here on detecting error.  |
`--------------------------------------*/
ace_yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  ace_yytoken = ace_yychar == ACE_YYEMPTY ? ACE_YYSYMBOL_ACE_YYEMPTY : ACE_YYTRANSLATE (ace_yychar);
  /* If not already recovering from an error, report this error.  */
  if (!ace_yyerrstatus)
    {
      ++ace_yynerrs;
      ace_yyerror (ACE_YYLEX_PARAM, ACE_YY_("syntax error"));
    }

  if (ace_yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (ace_yychar <= ACE_YYEOF)
        {
          /* Return failure if at end of input.  */
          if (ace_yychar == ACE_YYEOF)
            ACE_YYABORT;
        }
      else
        {
          ace_yydestruct ("Error: discarding",
                      ace_yytoken, &ace_yylval, ACE_YYLEX_PARAM);
          ace_yychar = ACE_YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto ace_yyerrlab1;


/*---------------------------------------------------.
| ace_yyerrorlab -- error raised explicitly by ACE_YYERROR.  |
`---------------------------------------------------*/
ace_yyerrorlab:
  /* Pacify compilers when the user code never invokes ACE_YYERROR and the
     label ace_yyerrorlab therefore never appears in user code.  */
  if (0)
    ACE_YYERROR;
  ++ace_yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this ACE_YYERROR.  */
  ACE_YYPOPSTACK (ace_yylen);
  ace_yylen = 0;
  ACE_YY_STACK_PRINT (ace_yyss, ace_yyssp);
  ace_yystate = *ace_yyssp;
  goto ace_yyerrlab1;


/*-------------------------------------------------------------.
| ace_yyerrlab1 -- common code for both syntax error and ACE_YYERROR.  |
`-------------------------------------------------------------*/
ace_yyerrlab1:
  ace_yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      ace_yyn = ace_yypact[ace_yystate];
      if (!ace_yypact_value_is_default (ace_yyn))
        {
          ace_yyn += ACE_YYSYMBOL_ACE_YYerror;
          if (0 <= ace_yyn && ace_yyn <= ACE_YYLAST && ace_yycheck[ace_yyn] == ACE_YYSYMBOL_ACE_YYerror)
            {
              ace_yyn = ace_yytable[ace_yyn];
              if (0 < ace_yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (ace_yyssp == ace_yyss)
        ACE_YYABORT;


      ace_yydestruct ("Error: popping",
                  ACE_YY_ACCESSING_SYMBOL (ace_yystate), ace_yyvsp, ACE_YYLEX_PARAM);
      ACE_YYPOPSTACK (1);
      ace_yystate = *ace_yyssp;
      ACE_YY_STACK_PRINT (ace_yyss, ace_yyssp);
    }

  ACE_YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++ace_yyvsp = ace_yylval;
  ACE_YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  ACE_YY_SYMBOL_PRINT ("Shifting", ACE_YY_ACCESSING_SYMBOL (ace_yyn), ace_yyvsp, ace_yylsp);

  ace_yystate = ace_yyn;
  goto ace_yynewstate;


/*-------------------------------------.
| ace_yyacceptlab -- ACE_YYACCEPT comes here.  |
`-------------------------------------*/
ace_yyacceptlab:
  ace_yyresult = 0;
  goto ace_yyreturnlab;


/*-----------------------------------.
| ace_yyabortlab -- ACE_YYABORT comes here.  |
`-----------------------------------*/
ace_yyabortlab:
  ace_yyresult = 1;
  goto ace_yyreturnlab;


/*-----------------------------------------------------------.
| ace_yyexhaustedlab -- ACE_YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
ace_yyexhaustedlab:
  ace_yyerror (ACE_YYLEX_PARAM, ACE_YY_("memory exhausted"));
  ace_yyresult = 2;
  goto ace_yyreturnlab;


/*----------------------------------------------------------.
| ace_yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
ace_yyreturnlab:
  if (ace_yychar != ACE_YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      ace_yytoken = ACE_YYTRANSLATE (ace_yychar);
      ace_yydestruct ("Cleanup: discarding lookahead",
                  ace_yytoken, &ace_yylval, ACE_YYLEX_PARAM);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this ACE_YYABORT or ACE_YYACCEPT.  */
  ACE_YYPOPSTACK (ace_yylen);
  ACE_YY_STACK_PRINT (ace_yyss, ace_yyssp);
  while (ace_yyssp != ace_yyss)
    {
      ace_yydestruct ("Cleanup: popping",
                  ACE_YY_ACCESSING_SYMBOL (+*ace_yyssp), ace_yyvsp, ACE_YYLEX_PARAM);
      ACE_YYPOPSTACK (1);
    }
#ifndef ace_yyoverflow
  if (ace_yyss != ace_yyssa)
    ACE_YYSTACK_FREE (ace_yyss);
#endif

  return ace_yyresult;
}



ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Prints the error string to standard output.  Cleans up the error
// messages.

void
ace_yyerror (int ace_yyerrno, int ace_yylineno, ACE_TCHAR const * s)
{
#if defined (ACE_NLOGGING)
  ACE_UNUSED_ARG (ace_yyerrno);
  ACE_UNUSED_ARG (ace_yylineno);
  ACE_UNUSED_ARG (s);
#endif /* ACE_NLOGGING */

  ACELIB_ERROR ((LM_ERROR,
                 ACE_TEXT ("ACE (%P|%t) [error %d] on line %d: %C\n"),
                 ace_yyerrno,
                 ace_yylineno,
                 s));
}

void
ace_yyerror (void *, ACE_TCHAR const * s)
{
  ace_yyerror (-1, -1, s);
}

// Note that SRC_REC represents left context, which is the STREAM *
// record.

static ACE_Module_Type *
ace_get_module (ACE_Service_Type const * sr,
                ACE_TCHAR const * svc_name,
                int & ace_yyerrno)
{
  ACE_Stream_Type const * const st =
    (sr == 0
     ? 0
     : dynamic_cast<ACE_Stream_Type const *> (sr->type ()));
  ACE_Module_Type const * const mt = (st == 0 ? 0 : st->find (svc_name));

  if (sr == 0 || st == 0 || mt == 0)
    {
      ACELIB_ERROR ((LM_ERROR,
                     ACE_TEXT ("cannot locate Module_Type %s ")
                     ACE_TEXT ("in STREAM_Type %s\n"),
                     svc_name,
                     (sr ? sr->name () : ACE_TEXT ("(nil)"))));
      ++ace_yyerrno;
    }

  return const_cast<ACE_Module_Type *> (mt);
}

#if defined (SVC_CONF_Y_DEBUGGING)
// Main driver program.

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Svc_Conf_Param param (0, stdin);

  // Try to reopen any filename argument to use ACE_YYIN.
  if (argc > 1 && (ace_yyin = freopen (argv[1], "r", stdin)) == 0)
    (void) ACE_OS::ACE_OS::fprintf (stderr, ACE_TEXT ("usage: %s [file]\n"), argv[0]), ACE_OS::exit (1);

  return ::ace_yyparse (&param);
}
#endif /* SVC_CONF_Y_DEBUGGING */

ACE_END_VERSIONED_NAMESPACE_DECL

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#endif  /* ACE_USES_CLASSIC_SVC_CONF == 1 */
