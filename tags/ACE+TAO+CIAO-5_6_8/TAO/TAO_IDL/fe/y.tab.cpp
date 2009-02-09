// $Id$
/*  A Bison parser, made by GNU Bison 1.875d.  */

/*   Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003 Free Software Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with tao_yy or TAO_YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define TAO_YYBISON 1

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
     IDL_INTEGER_LITERAL = 322,
     IDL_UINTEGER_LITERAL = 323,
     IDL_STRING_LITERAL = 324,
     IDL_CHARACTER_LITERAL = 325,
     IDL_FLOATING_PT_LITERAL = 326,
     IDL_TRUETOK = 327,
     IDL_FALSETOK = 328,
     IDL_SCOPE_DELIMITOR = 329,
     IDL_LEFT_SHIFT = 330,
     IDL_RIGHT_SHIFT = 331,
     IDL_WCHAR_LITERAL = 332,
     IDL_WSTRING_LITERAL = 333
   };
#endif
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
#define IDL_INTEGER_LITERAL 322
#define IDL_UINTEGER_LITERAL 323
#define IDL_STRING_LITERAL 324
#define IDL_CHARACTER_LITERAL 325
#define IDL_FLOATING_PT_LITERAL 326
#define IDL_TRUETOK 327
#define IDL_FALSETOK 328
#define IDL_SCOPE_DELIMITOR 329
#define IDL_LEFT_SHIFT 330
#define IDL_RIGHT_SHIFT 331
#define IDL_WCHAR_LITERAL 332
#define IDL_WSTRING_LITERAL 333




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

#if ! defined (TAO_YYSTYPE) && ! defined (TAO_YYSTYPE_IS_DECLARED)

typedef union TAO_YYSTYPE {
  AST_Decl                      *dcval;         /* Decl value           */
  UTL_StrList                   *slval;         /* String list          */
  UTL_NameList                  *nlval;         /* Name list            */
  UTL_ExprList                  *elval;         /* Expression list      */
  UTL_LabelList                 *llval;         /* Label list           */
  UTL_DeclList                  *dlval;         /* Declaration list     */
  FE_InterfaceHeader            *ihval;         /* Interface header     */
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
} TAO_YYSTYPE;
/* Line 191 of yacc.c.  */

# define tao_yystype TAO_YYSTYPE /* obsolescent; will be withdrawn */
# define TAO_YYSTYPE_IS_DECLARED 1
# define TAO_YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 214 of yacc.c.  */


#if ! defined (tao_yyoverflow) || TAO_YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef TAO_YYSTACK_USE_ALLOCA
#  if TAO_YYSTACK_USE_ALLOCA
#   define TAO_YYSTACK_ALLOC alloca
#  endif
# else
#  if defined (alloca) || defined (_ALLOCA_H)
#   define TAO_YYSTACK_ALLOC alloca
#  else
#   ifdef __GNUC__
#    define TAO_YYSTACK_ALLOC __builtin_alloca
#   endif
#  endif
# endif

# ifdef TAO_YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define TAO_YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define TAO_YYSIZE_T size_t
#  endif
#  define TAO_YYSTACK_ALLOC malloc
#  define TAO_YYSTACK_FREE free
# endif
#endif /* ! defined (tao_yyoverflow) || TAO_YYERROR_VERBOSE */


#if (! defined (tao_yyoverflow) \
     && (! defined (__cplusplus) \
         || (defined (TAO_YYSTYPE_IS_TRIVIAL) && TAO_YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union tao_yyalloc
{
  short tao_yyss;
  TAO_YYSTYPE tao_yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define TAO_YYSTACK_GAP_MAXIMUM (sizeof (union tao_yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define TAO_YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (TAO_YYSTYPE))                                \
      + TAO_YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef TAO_YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define TAO_YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define TAO_YYCOPY(To, From, Count)                \
      do                                        \
        {                                        \
          register TAO_YYSIZE_T tao_yyi;                \
          for (tao_yyi = 0; tao_yyi < (Count); tao_yyi++)        \
            (To)[tao_yyi] = (From)[tao_yyi];                \
        }                                        \
      while (0)
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
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char tao_yysigned_char;
#else
   typedef short tao_yysigned_char;
#endif

/* TAO_YYFINAL -- State number of the termination state. */
#define TAO_YYFINAL  3
/* TAO_YYLAST -- Last index in TAO_YYTABLE.  */
#define TAO_YYLAST   1111

/* TAO_YYNTOKENS -- Number of terminals. */
#define TAO_YYNTOKENS  100
/* TAO_YYNNTS -- Number of nonterminals. */
#define TAO_YYNNTS  322
/* TAO_YYNRULES -- Number of rules. */
#define TAO_YYNRULES  477
/* TAO_YYNRULES -- Number of states. */
#define TAO_YYNSTATES  675

/* TAO_YYTRANSLATE(TAO_YYLEX) -- Bison symbol number corresponding to TAO_YYLEX.  */
#define TAO_YYUNDEFTOK  2
#define TAO_YYMAXUTOK   333

#define TAO_YYTRANSLATE(TAO_YYX)                                                 \
  ((unsigned int) (TAO_YYX) <= TAO_YYMAXUTOK ? tao_yytranslate[TAO_YYX] : TAO_YYUNDEFTOK)

/* TAO_YYTRANSLATE[TAO_YYLEX] -- Bison symbol number corresponding to TAO_YYLEX.  */
static const unsigned char tao_yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    92,    87,     2,
      94,    95,    90,    88,    83,    89,     2,    91,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    82,    79,
      97,    84,    96,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    98,     2,    99,    86,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    80,    85,    81,    93,     2,     2,     2,
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
      75,    76,    77,    78
};

#if TAO_YYDEBUG
/* TAO_YYPRHS[TAO_YYN] -- Index of the first RHS symbol of rule number TAO_YYN in
   TAO_YYRHS.  */
static const unsigned short tao_yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    10,    14,    15,    19,
      20,    24,    25,    29,    30,    34,    35,    39,    40,    44,
      45,    49,    50,    54,    55,    59,    60,    64,    65,    69,
      70,    71,    72,    73,    83,    85,    87,    88,    89,    90,
      98,    99,   103,   106,   110,   114,   115,   120,   121,   123,
     125,   127,   129,   132,   134,   135,   136,   137,   145,   146,
     147,   148,   157,   158,   163,   164,   168,   170,   171,   174,
     175,   178,   180,   183,   186,   187,   189,   191,   194,   195,
     199,   200,   204,   207,   208,   209,   213,   214,   218,   219,
     223,   224,   228,   229,   233,   234,   238,   239,   243,   244,
     248,   251,   252,   257,   258,   260,   261,   265,   266,   271,
     273,   275,   278,   281,   282,   283,   284,   285,   295,   297,
     299,   301,   303,   305,   307,   309,   311,   313,   315,   317,
     319,   323,   325,   329,   331,   335,   337,   341,   345,   347,
     351,   355,   357,   361,   365,   369,   371,   374,   377,   380,
     382,   384,   388,   390,   392,   394,   396,   398,   400,   402,
     404,   406,   408,   409,   413,   415,   417,   419,   422,   424,
     425,   429,   431,   433,   435,   437,   439,   441,   443,   445,
     447,   449,   451,   453,   455,   457,   459,   461,   463,   465,
     467,   469,   471,   474,   475,   480,   481,   483,   485,   488,
     489,   494,   495,   497,   499,   501,   503,   505,   508,   510,
     513,   517,   520,   522,   524,   527,   529,   531,   533,   535,
     537,   539,   541,   542,   546,   547,   548,   549,   557,   560,
     563,   564,   565,   568,   569,   570,   576,   577,   581,   582,
     586,   587,   588,   589,   590,   591,   592,   607,   609,   611,
     613,   615,   617,   619,   622,   625,   626,   627,   628,   634,
     635,   639,   642,   645,   646,   647,   651,   652,   653,   659,
     660,   664,   666,   668,   669,   670,   671,   672,   682,   685,
     686,   691,   692,   694,   695,   696,   703,   706,   707,   708,
     714,   715,   716,   723,   725,   727,   728,   729,   736,   738,
     740,   741,   745,   748,   751,   752,   753,   754,   760,   762,
     764,   765,   766,   767,   768,   778,   779,   780,   781,   782,
     792,   793,   794,   795,   796,   806,   807,   808,   809,   810,
     821,   823,   825,   826,   828,   830,   831,   832,   833,   841,
     842,   846,   847,   852,   855,   856,   861,   862,   863,   864,
     870,   871,   875,   876,   881,   884,   885,   890,   891,   892,
     893,   899,   901,   903,   905,   907,   909,   911,   913,   914,
     915,   922,   923,   924,   925,   932,   933,   934,   935,   942,
     943,   944,   945,   952,   953,   956,   957,   962,   963,   967,
     971,   973,   975,   978,   979,   980,   981,   989,   990,   991,
     998,   999,  1003,  1004,  1007,  1008,  1009,  1013,  1014,  1018,
    1019,  1023,  1024,  1028,  1029,  1033,  1034,  1038,  1042,  1044,
    1046,  1051,  1053,  1054,  1058,  1062,  1066,  1067,  1071,  1072,
    1073,  1074,  1075,  1076,  1077,  1091,  1092,  1096,  1097,  1100,
    1101,  1102,  1103,  1109,  1112,  1113,  1115,  1116,  1120,  1121,
    1125,  1126,  1127,  1134,  1135,  1136,  1143,  1145,  1147,  1149,
    1151,  1153,  1156,  1160,  1161,  1162,  1163,  1172,  1176,  1180,
    1183,  1184,  1188,  1189,  1190,  1191,  1200,  1202
};

/* TAO_YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short tao_yyrhs[] =
{
     101,     0,    -1,   102,    -1,   102,   103,    -1,    -1,    -1,
     190,   104,    79,    -1,    -1,   351,   105,    79,    -1,    -1,
     352,   106,    79,    -1,    -1,   172,   107,    79,    -1,    -1,
     300,   108,    79,    -1,    -1,   121,   109,    79,    -1,    -1,
     116,   110,    79,    -1,    -1,   131,   111,    79,    -1,    -1,
     353,   112,    79,    -1,    -1,   379,   113,    79,    -1,    -1,
     404,   114,    79,    -1,    -1,     1,   115,    79,    -1,    -1,
      -1,    -1,    -1,     5,   117,     3,   118,    80,   119,   102,
     120,    81,    -1,   122,    -1,   171,    -1,    -1,    -1,    -1,
     128,   123,    80,   124,   154,   125,    81,    -1,    -1,     6,
     127,   170,    -1,   126,   129,    -1,    41,   126,   129,    -1,
      42,   126,   129,    -1,    -1,    82,   145,   130,   164,    -1,
      -1,   132,    -1,   137,    -1,   147,    -1,   148,    -1,    43,
     133,    -1,   133,    -1,    -1,    -1,    -1,   141,   134,    80,
     135,   149,   136,    81,    -1,    -1,    -1,    -1,    42,   141,
     138,    80,   139,   154,   140,    81,    -1,    -1,   143,   129,
     142,   146,    -1,    -1,    49,   144,   170,    -1,    48,    -1,
      -1,    47,   164,    -1,    -1,    42,   143,    -1,   143,    -1,
     143,   194,    -1,   149,   150,    -1,    -1,   151,    -1,   155,
      -1,   312,    79,    -1,    -1,    46,   152,   229,    -1,    -1,
      45,   153,   229,    -1,   154,   155,    -1,    -1,    -1,   190,
     156,    79,    -1,    -1,   351,   157,    79,    -1,    -1,   352,
     158,    79,    -1,    -1,   172,   159,    79,    -1,    -1,   300,
     160,    79,    -1,    -1,   289,   161,    79,    -1,    -1,   305,
     162,    79,    -1,    -1,     1,   163,    79,    -1,   167,   165,
      -1,    -1,   165,    83,   166,   167,    -1,    -1,   170,    -1,
      -1,    74,   168,   170,    -1,    -1,   167,    74,   169,   170,
      -1,     3,    -1,   126,    -1,    41,   126,    -1,    42,   126,
      -1,    -1,    -1,    -1,    -1,     4,   173,   177,   174,   170,
     175,    84,   176,   178,    -1,   209,    -1,   214,    -1,   215,
      -1,   216,    -1,   212,    -1,   213,    -1,   274,    -1,   278,
      -1,   167,    -1,   179,    -1,   180,    -1,   181,    -1,   180,
      85,   181,    -1,   182,    -1,   181,    86,   182,    -1,   183,
      -1,   182,    87,   183,    -1,   184,    -1,   183,    75,   184,
      -1,   183,    76,   184,    -1,   185,    -1,   184,    88,   185,
      -1,   184,    89,   185,    -1,   186,    -1,   185,    90,   186,
      -1,   185,    91,   186,    -1,   185,    92,   186,    -1,   187,
      -1,    88,   187,    -1,    89,   187,    -1,    93,   187,    -1,
     167,    -1,   188,    -1,    94,   179,    95,    -1,    67,    -1,
      68,    -1,    69,    -1,    78,    -1,    70,    -1,    77,    -1,
      71,    -1,    72,    -1,    73,    -1,   179,    -1,    -1,     7,
     191,   192,    -1,   221,    -1,   235,    -1,   259,    -1,    40,
     207,    -1,   199,    -1,    -1,   194,   193,   200,    -1,   195,
      -1,   198,    -1,   196,    -1,   197,    -1,   167,    -1,   209,
      -1,   212,    -1,   214,    -1,   216,    -1,   215,    -1,   213,
      -1,   217,    -1,   218,    -1,   268,    -1,   274,    -1,   278,
      -1,   221,    -1,   235,    -1,   259,    -1,   257,    -1,   258,
      -1,   203,   201,    -1,    -1,   201,    83,   202,   203,    -1,
      -1,   207,    -1,   208,    -1,   207,   205,    -1,    -1,   205,
      83,   206,   207,    -1,    -1,   170,    -1,   282,    -1,   210,
      -1,   211,    -1,     8,    -1,     8,     8,    -1,     9,    -1,
      10,     8,    -1,    10,     8,     8,    -1,    10,     9,    -1,
      11,    -1,    12,    -1,     8,    11,    -1,    17,    -1,    13,
      -1,    14,    -1,    15,    -1,    16,    -1,    18,    -1,    19,
      -1,    -1,    20,   220,   170,    -1,    -1,    -1,    -1,   219,
     222,    80,   223,   225,   224,    81,    -1,   227,   226,    -1,
     226,   227,    -1,    -1,    -1,   228,   229,    -1,    -1,    -1,
     194,   230,   200,   231,    79,    -1,    -1,     1,   232,    79,
      -1,    -1,    21,   234,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   233,    22,   236,    94,   237,   242,   238,    95,
     239,    80,   240,   243,   241,    81,    -1,   209,    -1,   214,
      -1,   215,    -1,   216,    -1,   259,    -1,   167,    -1,   245,
     244,    -1,   244,   245,    -1,    -1,    -1,    -1,   249,   246,
     255,   247,    79,    -1,    -1,     1,   248,    79,    -1,   251,
     250,    -1,   250,   251,    -1,    -1,    -1,    29,   252,    82,
      -1,    -1,    -1,    28,   253,   179,   254,    82,    -1,    -1,
     194,   256,   203,    -1,   219,    -1,   233,    -1,    -1,    -1,
      -1,    -1,    23,   260,   170,   261,    80,   262,   264,   263,
      81,    -1,   267,   265,    -1,    -1,   265,    83,   266,   267,
      -1,    -1,     3,    -1,    -1,    -1,   271,    83,   269,   189,
     270,    96,    -1,   271,    96,    -1,    -1,    -1,    24,   272,
      97,   273,   195,    -1,    -1,    -1,   277,    97,   275,   189,
     276,    96,    -1,   277,    -1,    25,    -1,    -1,    -1,   281,
      97,   279,   189,   280,    96,    -1,   281,    -1,    26,    -1,
      -1,   170,   283,   284,    -1,   286,   285,    -1,   285,   286,
      -1,    -1,    -1,    -1,    98,   287,   189,   288,    99,    -1,
     290,    -1,   295,    -1,    -1,    -1,    -1,    -1,    30,   291,
      31,   292,   334,   293,   204,   294,   336,    -1,    -1,    -1,
      -1,    -1,    31,   296,   334,   297,   204,   298,   339,   299,
     342,    -1,    -1,    -1,    -1,    -1,    27,   301,   170,   302,
      80,   303,   226,   304,    81,    -1,    -1,    -1,    -1,    -1,
     310,   311,   306,     3,   307,   325,   308,   336,   309,   345,
      -1,    32,    -1,    33,    -1,    -1,   334,    -1,    34,    -1,
      -1,    -1,    -1,    44,   313,     3,   314,   316,   315,   336,
      -1,    -1,    94,   317,    95,    -1,    -1,    94,   318,   319,
      95,    -1,   322,   320,    -1,    -1,   320,    83,   321,   322,
      -1,    -1,    -1,    -1,    35,   323,   334,   324,   203,    -1,
      -1,    94,   326,    95,    -1,    -1,    94,   327,   328,    95,
      -1,   331,   329,    -1,    -1,   329,    83,   330,   331,    -1,
      -1,    -1,    -1,   335,   332,   334,   333,   203,    -1,   196,
      -1,   274,    -1,   278,    -1,   167,    -1,    35,    -1,    36,
      -1,    37,    -1,    -1,    -1,    38,   337,    94,   338,   164,
      95,    -1,    -1,    -1,    -1,    55,   340,    94,   341,   164,
      95,    -1,    -1,    -1,    -1,    62,   343,    94,   344,   164,
      95,    -1,    -1,    -1,    -1,    39,   346,    94,   347,   348,
      95,    -1,    -1,    69,   349,    -1,    -1,   349,    83,   350,
      69,    -1,    -1,    63,   167,    69,    -1,    64,   167,    69,
      -1,   355,    -1,   354,    -1,    50,   170,    -1,    -1,    -1,
      -1,   359,   356,    80,   357,   364,   358,    81,    -1,    -1,
      -1,    50,   170,   360,   362,   361,   146,    -1,    -1,    82,
     363,   167,    -1,    -1,   364,   365,    -1,    -1,    -1,   372,
     366,    79,    -1,    -1,   374,   367,    79,    -1,    -1,   376,
     368,    79,    -1,    -1,   377,   369,    79,    -1,    -1,   378,
     370,    79,    -1,    -1,   289,   371,    79,    -1,    60,   373,
     170,    -1,   167,    -1,    19,    -1,    65,   375,   373,   170,
      -1,    58,    -1,    -1,    52,   167,   170,    -1,    61,   167,
     170,    -1,    51,   167,   170,    -1,    -1,   381,   380,   391,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,   382,   170,
     383,   388,   384,   146,   385,    66,   386,   167,   387,   390,
      -1,    -1,    82,   389,   167,    -1,    -1,    59,   167,    -1,
      -1,    -1,    -1,    80,   392,   394,   393,    81,    -1,   394,
     395,    -1,    -1,   155,    -1,    -1,   398,   396,    79,    -1,
      -1,   401,   397,    79,    -1,    -1,    -1,    44,   170,   399,
     316,   400,   336,    -1,    -1,    -1,    54,   170,   402,   316,
     403,   336,    -1,   417,    -1,   408,    -1,   405,    -1,   407,
      -1,   406,    -1,    53,   170,    -1,    42,    53,   170,    -1,
      -1,    -1,    -1,   412,   415,   409,    80,   410,   154,   411,
      81,    -1,    42,    53,   170,    -1,    43,    53,   170,    -1,
      53,   170,    -1,    -1,   129,   416,   146,    -1,    -1,    -1,
      -1,   421,   415,   418,    80,   419,   149,   420,    81,    -1,
     413,    -1,   414,    -1
};

/* TAO_YYRLINE[TAO_YYN] -- source line where rule number TAO_YYN was defined.  */
static const unsigned short tao_yyrline[] =
{
       0,   320,   320,   323,   324,   329,   328,   339,   338,   349,
     348,   359,   358,   369,   368,   379,   378,   389,   388,   399,
     398,   409,   408,   419,   418,   429,   428,   439,   438,   452,
     457,   483,   488,   451,   504,   505,   510,   549,   554,   509,
     572,   571,   585,   623,   654,   688,   687,   699,   706,   707,
     708,   709,   713,   724,   729,   773,   778,   728,   807,   846,
     851,   805,   870,   868,   919,   918,   932,   938,   945,   952,
     959,   985,  1012,  1067,  1068,  1072,  1073,  1074,  1080,  1079,
    1087,  1086,  1096,  1097,  1102,  1101,  1112,  1111,  1122,  1121,
    1132,  1131,  1142,  1141,  1152,  1151,  1162,  1161,  1172,  1171,
    1185,  1198,  1196,  1226,  1233,  1244,  1243,  1271,  1269,  1296,
    1308,  1354,  1382,  1413,  1418,  1423,  1428,  1412,  1481,  1482,
    1483,  1484,  1485,  1486,  1487,  1499,  1504,  1579,  1581,  1583,
    1584,  1598,  1599,  1613,  1614,  1627,  1628,  1638,  1651,  1652,
    1662,  1675,  1676,  1686,  1696,  1709,  1710,  1720,  1730,  1743,
    1778,  1779,  1788,  1793,  1800,  1808,  1813,  1818,  1824,  1829,
    1834,  1842,  1914,  1913,  1923,  1928,  1933,  1938,  1965,  1974,
    1973,  2032,  2033,  2037,  2045,  2046,  2074,  2075,  2076,  2077,
    2078,  2079,  2080,  2081,  2085,  2086,  2087,  2091,  2092,  2093,
    2097,  2098,  2102,  2115,  2113,  2143,  2150,  2151,  2155,  2168,
    2166,  2196,  2203,  2220,  2239,  2240,  2244,  2249,  2254,  2262,
    2267,  2272,  2280,  2285,  2290,  2298,  2312,  2317,  2325,  2333,
    2341,  2349,  2358,  2357,  2373,  2407,  2412,  2372,  2431,  2434,
    2435,  2439,  2439,  2449,  2454,  2448,  2517,  2516,  2531,  2530,
    2545,  2550,  2555,  2560,  2607,  2612,  2544,  2636,  2644,  2658,
    2668,  2676,  2677,  2785,  2788,  2789,  2794,  2799,  2793,  2835,
    2834,  2848,  2859,  2881,  2889,  2888,  2904,  2909,  2903,  2926,
    2925,  2978,  3002,  3027,  3032,  3065,  3070,  3026,  3096,  3101,
    3099,  3106,  3110,  3147,  3152,  3145,  3219,  3274,  3284,  3273,
    3299,  3304,  3297,  3345,  3369,  3379,  3384,  3377,  3414,  3437,
    3446,  3445,  3481,  3492,  3514,  3522,  3527,  3521,  3564,  3565,
    3570,  3575,  3580,  3585,  3569,  3654,  3659,  3664,  3669,  3653,
    3747,  3752,  3782,  3787,  3746,  3805,  3810,  3863,  3868,  3803,
    3905,  3911,  3918,  3925,  3926,  3938,  3944,  3986,  3937,  4008,
    4007,  4018,  4017,  4030,  4035,  4033,  4040,  4045,  4050,  4044,
    4091,  4090,  4101,  4100,  4113,  4118,  4116,  4123,  4128,  4133,
    4127,  4180,  4188,  4189,  4190,  4233,  4238,  4243,  4252,  4257,
    4251,  4269,  4277,  4282,  4276,  4294,  4302,  4307,  4301,  4319,
    4327,  4332,  4326,  4344,  4351,  4364,  4362,  4390,  4397,  4426,
    4464,  4465,  4469,  4499,  4539,  4544,  4498,  4563,  4568,  4561,
    4610,  4609,  4620,  4627,  4628,  4633,  4632,  4643,  4642,  4653,
    4652,  4663,  4662,  4673,  4672,  4683,  4682,  4695,  4759,  4766,
    4791,  4864,  4870,  4877,  4935,  4993,  5052,  5051,  5101,  5106,
    5111,  5116,  5121,  5126,  5100,  5180,  5179,  5190,  5197,  5204,
    5212,  5217,  5211,  5229,  5230,  5234,  5236,  5235,  5246,  5245,
    5260,  5296,  5258,  5330,  5366,  5328,  5398,  5399,  5400,  5404,
    5405,  5409,  5437,  5468,  5513,  5518,  5466,  5535,  5545,  5564,
    5576,  5575,  5626,  5676,  5681,  5624,  5698,  5703
};
#endif

#if TAO_YYDEBUG || TAO_YYERROR_VERBOSE
/* TAO_YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at TAO_YYNTOKENS, nonterminals. */
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
  "IDL_USES", "IDL_MANAGES", "IDL_INTEGER_LITERAL", "IDL_UINTEGER_LITERAL",
  "IDL_STRING_LITERAL", "IDL_CHARACTER_LITERAL", "IDL_FLOATING_PT_LITERAL",
  "IDL_TRUETOK", "IDL_FALSETOK", "IDL_SCOPE_DELIMITOR", "IDL_LEFT_SHIFT",
  "IDL_RIGHT_SHIFT", "IDL_WCHAR_LITERAL", "IDL_WSTRING_LITERAL", "';'",
  "'{'", "'}'", "':'", "','", "'='", "'|'", "'^'", "'&'", "'+'", "'-'",
  "'*'", "'/'", "'%'", "'~'", "'('", "')'", "'>'", "'<'", "'['", "']'",
  "$accept", "start", "definitions", "definition", "@1", "@2", "@3", "@4",
  "@5", "@6", "@7", "@8", "@9", "@10", "@11", "@12", "module", "@13",
  "@14", "@15", "@16", "interface_def", "interface", "@17", "@18", "@19",
  "interface_decl", "@20", "interface_header", "inheritance_spec", "@21",
  "value_def", "valuetype", "value_concrete_decl", "@22", "@23", "@24",
  "value_abs_decl", "@25", "@26", "@27", "value_header", "@28",
  "value_decl", "@29", "opt_truncatable", "supports_spec",
  "value_forward_decl", "value_box_decl", "value_elements",
  "value_element", "state_member", "@30", "@31", "exports", "export",
  "@32", "@33", "@34", "@35", "@36", "@37", "@38", "@39",
  "at_least_one_scoped_name", "scoped_names", "@40", "scoped_name", "@41",
  "@42", "id", "interface_forward", "const_dcl", "@43", "@44", "@45",
  "@46", "const_type", "expression", "const_expr", "or_expr", "xor_expr",
  "and_expr", "shift_expr", "add_expr", "mult_expr", "unary_expr",
  "primary_expr", "literal", "positive_int_expr", "type_dcl", "@47",
  "type_declarator", "@48", "type_spec", "simple_type_spec",
  "base_type_spec", "template_type_spec", "constructed_type_spec",
  "constructed_forward_type_spec", "at_least_one_declarator",
  "declarators", "@49", "declarator", "at_least_one_simple_declarator",
  "simple_declarators", "@50", "simple_declarator", "complex_declarator",
  "integer_type", "signed_int", "unsigned_int", "floating_pt_type",
  "fixed_type", "char_type", "octet_type", "boolean_type", "any_type",
  "object_type", "struct_decl", "@51", "struct_type", "@52", "@53", "@54",
  "at_least_one_member", "members", "member", "@55", "member_i", "@56",
  "@57", "@58", "union_decl", "@59", "union_type", "@60", "@61", "@62",
  "@63", "@64", "@65", "switch_type_spec", "at_least_one_case_branch",
  "case_branches", "case_branch", "@66", "@67", "@68",
  "at_least_one_case_label", "case_labels", "case_label", "@69", "@70",
  "@71", "element_spec", "@72", "struct_forward_type",
  "union_forward_type", "enum_type", "@73", "@74", "@75", "@76",
  "at_least_one_enumerator", "enumerators", "@77", "enumerator",
  "sequence_type_spec", "@78", "@79", "seq_head", "@80", "@81",
  "string_type_spec", "@82", "@83", "string_head", "wstring_type_spec",
  "@84", "@85", "wstring_head", "array_declarator", "@86",
  "at_least_one_array_dim", "array_dims", "array_dim", "@87", "@88",
  "attribute", "attribute_readonly", "@89", "@90", "@91", "@92",
  "attribute_readwrite", "@93", "@94", "@95", "@96", "exception", "@97",
  "@98", "@99", "@100", "operation", "@101", "@102", "@103", "@104",
  "opt_op_attribute", "op_type_spec", "init_decl", "@105", "@106", "@107",
  "init_parameter_list", "@108", "@109", "at_least_one_in_parameter",
  "in_parameters", "@110", "in_parameter", "@111", "@112",
  "parameter_list", "@113", "@114", "at_least_one_parameter", "parameters",
  "@115", "parameter", "@116", "@117", "param_type_spec", "direction",
  "opt_raises", "@118", "@119", "opt_getraises", "@120", "@121",
  "opt_setraises", "@122", "@123", "opt_context", "@124", "@125",
  "at_least_one_string_literal", "string_literals", "@126", "typeid_dcl",
  "typeprefix_dcl", "component", "component_forward_decl",
  "component_decl", "@127", "@128", "@129", "component_header", "@130",
  "@131", "component_inheritance_spec", "@132", "component_exports",
  "component_export", "@133", "@134", "@135", "@136", "@137", "@138",
  "provides_decl", "interface_type", "uses_decl", "opt_multiple",
  "emits_decl", "publishes_decl", "consumes_decl", "home_decl", "@139",
  "home_header", "@140", "@141", "@142", "@143", "@144", "@145",
  "home_inheritance_spec", "@146", "primary_key_spec", "home_body", "@147",
  "@148", "home_exports", "home_export", "@149", "@150", "factory_decl",
  "@151", "@152", "finder_decl", "@153", "@154", "event",
  "event_forward_decl", "event_concrete_forward_decl",
  "event_abs_forward_decl", "event_abs_decl", "@155", "@156", "@157",
  "event_abs_header", "event_custom_header", "event_plain_header",
  "event_rest_of_header", "@158", "event_decl", "@159", "@160", "@161",
  "event_header", 0
};
#endif

# ifdef TAO_YYPRINT
/* TAO_YYTOKNUM[TAO_YYLEX-NUM] -- Internal token number corresponding to
   token TAO_YYLEX-NUM.  */
static const unsigned short tao_yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,    59,
     123,   125,    58,    44,    61,   124,    94,    38,    43,    45,
      42,    47,    37,   126,    40,    41,    62,    60,    91,    93
};
# endif

/* TAO_YYR1[TAO_YYN] -- Symbol number of symbol that rule TAO_YYN derives.  */
static const unsigned short tao_yyr1[] =
{
       0,   100,   101,   102,   102,   104,   103,   105,   103,   106,
     103,   107,   103,   108,   103,   109,   103,   110,   103,   111,
     103,   112,   103,   113,   103,   114,   103,   115,   103,   117,
     118,   119,   120,   116,   121,   121,   123,   124,   125,   122,
     127,   126,   128,   128,   128,   130,   129,   129,   131,   131,
     131,   131,   132,   132,   134,   135,   136,   133,   138,   139,
     140,   137,   142,   141,   144,   143,   145,   145,   146,   146,
     147,   147,   148,   149,   149,   150,   150,   150,   152,   151,
     153,   151,   154,   154,   156,   155,   157,   155,   158,   155,
     159,   155,   160,   155,   161,   155,   162,   155,   163,   155,
     164,   166,   165,   165,   167,   168,   167,   169,   167,   170,
     171,   171,   171,   173,   174,   175,   176,   172,   177,   177,
     177,   177,   177,   177,   177,   177,   177,   178,   179,   180,
     180,   181,   181,   182,   182,   183,   183,   183,   184,   184,
     184,   185,   185,   185,   185,   186,   186,   186,   186,   187,
     187,   187,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   189,   191,   190,   190,   190,   190,   190,   190,   193,
     192,   194,   194,   195,   195,   195,   196,   196,   196,   196,
     196,   196,   196,   196,   197,   197,   197,   198,   198,   198,
     199,   199,   200,   202,   201,   201,   203,   203,   204,   206,
     205,   205,   207,   208,   209,   209,   210,   210,   210,   211,
     211,   211,   212,   212,   212,   213,   214,   214,   215,   216,
     217,   218,   220,   219,   222,   223,   224,   221,   225,   226,
     226,   228,   227,   230,   231,   229,   232,   229,   234,   233,
     236,   237,   238,   239,   240,   241,   235,   242,   242,   242,
     242,   242,   242,   243,   244,   244,   246,   247,   245,   248,
     245,   249,   250,   250,   252,   251,   253,   254,   251,   256,
     255,   257,   258,   260,   261,   262,   263,   259,   264,   266,
     265,   265,   267,   269,   270,   268,   268,   272,   273,   271,
     275,   276,   274,   274,   277,   279,   280,   278,   278,   281,
     283,   282,   284,   285,   285,   287,   288,   286,   289,   289,
     291,   292,   293,   294,   290,   296,   297,   298,   299,   295,
     301,   302,   303,   304,   300,   306,   307,   308,   309,   305,
     310,   310,   310,   311,   311,   313,   314,   315,   312,   317,
     316,   318,   316,   319,   321,   320,   320,   323,   324,   322,
     326,   325,   327,   325,   328,   330,   329,   329,   332,   333,
     331,   334,   334,   334,   334,   335,   335,   335,   337,   338,
     336,   336,   340,   341,   339,   339,   343,   344,   342,   342,
     346,   347,   345,   345,   348,   350,   349,   349,   351,   352,
     353,   353,   354,   356,   357,   358,   355,   360,   361,   359,
     363,   362,   362,   364,   364,   366,   365,   367,   365,   368,
     365,   369,   365,   370,   365,   371,   365,   372,   373,   373,
     374,   375,   375,   376,   377,   378,   380,   379,   382,   383,
     384,   385,   386,   387,   381,   389,   388,   388,   390,   390,
     392,   393,   391,   394,   394,   395,   396,   395,   397,   395,
     399,   400,   398,   402,   403,   401,   404,   404,   404,   405,
     405,   406,   407,   409,   410,   411,   408,   412,   413,   414,
     416,   415,   418,   419,   420,   417,   421,   421
};

/* TAO_YYR2[TAO_YYN] -- Number of symbols composing right hand side of rule TAO_YYN.  */
static const unsigned char tao_yyr2[] =
{
       0,     2,     1,     2,     0,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       0,     0,     0,     9,     1,     1,     0,     0,     0,     7,
       0,     3,     2,     3,     3,     0,     4,     0,     1,     1,
       1,     1,     2,     1,     0,     0,     0,     7,     0,     0,
       0,     8,     0,     4,     0,     3,     1,     0,     2,     0,
       2,     1,     2,     2,     0,     1,     1,     2,     0,     3,
       0,     3,     2,     0,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       2,     0,     4,     0,     1,     0,     3,     0,     4,     1,
       1,     2,     2,     0,     0,     0,     0,     9,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     1,     3,     1,     3,     1,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     1,     2,     2,     2,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     3,     1,     1,     1,     2,     1,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     0,     4,     0,     1,     1,     2,     0,
       4,     0,     1,     1,     1,     1,     1,     2,     1,     2,
       3,     2,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     0,     3,     0,     0,     0,     7,     2,     2,
       0,     0,     2,     0,     0,     5,     0,     3,     0,     3,
       0,     0,     0,     0,     0,     0,    14,     1,     1,     1,
       1,     1,     1,     2,     2,     0,     0,     0,     5,     0,
       3,     2,     2,     0,     0,     3,     0,     0,     5,     0,
       3,     1,     1,     0,     0,     0,     0,     9,     2,     0,
       4,     0,     1,     0,     0,     6,     2,     0,     0,     5,
       0,     0,     6,     1,     1,     0,     0,     6,     1,     1,
       0,     3,     2,     2,     0,     0,     0,     5,     1,     1,
       0,     0,     0,     0,     9,     0,     0,     0,     0,     9,
       0,     0,     0,     0,     9,     0,     0,     0,     0,    10,
       1,     1,     0,     1,     1,     0,     0,     0,     7,     0,
       3,     0,     4,     2,     0,     4,     0,     0,     0,     5,
       0,     3,     0,     4,     2,     0,     4,     0,     0,     0,
       5,     1,     1,     1,     1,     1,     1,     1,     0,     0,
       6,     0,     0,     0,     6,     0,     0,     0,     6,     0,
       0,     0,     6,     0,     2,     0,     4,     0,     3,     3,
       1,     1,     2,     0,     0,     0,     7,     0,     0,     6,
       0,     3,     0,     2,     0,     0,     3,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     3,     1,     1,
       4,     1,     0,     3,     3,     3,     0,     3,     0,     0,
       0,     0,     0,     0,    13,     0,     3,     0,     2,     0,
       0,     0,     5,     2,     0,     1,     0,     3,     0,     3,
       0,     0,     6,     0,     0,     6,     1,     1,     1,     1,
       1,     2,     3,     0,     0,     0,     8,     3,     3,     2,
       0,     3,     0,     0,     0,     8,     1,     1
};

/* TAO_YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when TAO_YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short tao_yydefact[] =
{
       4,     0,     0,     1,    27,   113,    29,    40,   162,   222,
     238,   273,   320,     0,     0,     0,     0,    64,     0,     0,
     428,     0,     0,     3,    17,    15,    34,    47,    36,    19,
      48,    53,    49,    54,    47,    50,    51,    35,    11,     5,
     168,   224,   164,   272,   165,   190,   191,   166,    13,     7,
       9,    21,   391,   390,   393,    23,   426,    25,   458,   460,
     459,   457,    47,   476,   477,   456,    47,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   109,   202,   167,    47,
       0,    47,    58,    47,     0,    52,    47,     0,   397,   469,
       0,   105,     0,   104,     0,     0,     0,    67,    42,     0,
       0,     0,   206,   208,     0,   212,   213,   216,   217,   218,
     219,   215,   220,   221,   287,   294,   299,    62,   175,    72,
     171,   173,   174,   172,   176,   204,   205,   177,   181,   178,
     180,   179,   182,   183,   224,   187,     0,   188,   189,   184,
       0,   185,   293,   186,   298,     0,     0,     0,   240,     0,
       0,     0,     0,     0,     0,     0,     0,   470,   463,   472,
      28,   126,   114,   118,   122,   123,   119,   120,   121,   124,
     125,    30,    41,   163,   169,   223,   239,   274,   321,    43,
     467,    44,     0,   468,    65,   402,   429,     0,   388,   107,
     389,    18,    16,    66,    45,    37,    20,    55,   207,   214,
     209,   211,     0,    69,   283,   286,   290,   295,    12,     6,
     225,     0,    14,     8,    10,    22,   394,    24,   440,   427,
      26,    69,     0,     0,     0,     0,     0,     0,     0,    59,
     400,   398,   437,   106,     0,     0,    83,    74,   210,   288,
       0,    63,     0,     0,     0,   231,   241,   404,   444,   471,
     464,   473,   115,    31,   202,   170,   195,   196,   197,   203,
     275,   322,    83,     0,    69,   435,   430,   108,    46,   103,
       0,     0,     0,    68,   152,   153,   154,   156,   158,   159,
     160,   157,   155,     0,     0,     0,     0,   149,   161,   128,
     129,   131,   133,   135,   138,   141,   145,   150,   284,   291,
     296,   226,   230,     0,     0,   395,     0,    83,    74,     0,
       4,     0,   192,     0,   230,     0,   401,   399,     0,    69,
     100,    98,   310,   315,   330,   331,     0,    82,    90,    84,
      94,   308,   309,    92,    96,     0,    86,    88,   335,    80,
      78,     0,    73,    75,    76,     0,   289,   146,   147,   148,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   231,   236,   233,   232,   206,
     252,   247,   248,   249,   250,   242,   251,     0,     0,     0,
       0,   422,   415,     0,   403,   405,   407,   409,   411,   413,
       0,     0,   445,     0,   443,   446,   448,     0,     0,   116,
       0,   305,   301,   304,   193,   282,   276,   281,   231,     0,
     436,   431,   101,     0,     0,     0,    39,     0,     0,     0,
       0,     0,   334,   364,   361,   362,   363,   325,   333,     0,
       0,     0,     0,     0,    57,    77,   151,   130,   132,   134,
     136,   137,   139,   140,   142,   143,   144,   285,   292,   297,
     227,   229,     0,     0,     0,     0,     0,   419,   418,     0,
       0,   421,     0,     0,   396,     0,     0,     0,     0,     0,
     450,   453,   442,     0,     0,     0,     0,     0,     0,     0,
     302,     0,     0,   278,     0,    61,     0,     0,    99,   311,
     316,    91,    85,    95,    93,    97,     0,    87,    89,   336,
      81,    79,   237,   234,   243,   425,   423,   417,   424,     0,
     416,   406,   408,   410,   412,   414,     0,     0,   447,   449,
     466,   475,   117,   127,    33,   306,   303,   194,   277,   279,
     324,   432,   102,     0,     0,   326,     0,     0,     0,   420,
     339,   451,   454,     0,     0,     0,   312,   317,   201,     0,
     337,   235,   244,     0,     0,   371,   371,   307,   280,   433,
       0,   375,   198,   352,   327,   371,     0,   340,   347,     0,
     346,   368,   452,   455,   439,   313,   372,   318,   199,     0,
       0,   371,   338,   259,   266,   264,   245,   255,   256,   263,
       0,   342,   343,     0,     0,   434,   371,     0,   379,     0,
     351,   365,   366,   367,     0,   357,   358,   328,     0,     0,
       0,     0,     0,     0,   261,   348,   344,   369,   438,   314,
     373,   376,   319,   200,   353,   354,     0,   383,   260,   267,
     265,   246,   254,   269,   257,   262,     0,     0,     0,     0,
       0,   355,   359,   380,   329,     0,     0,     0,   349,   345,
       0,     0,   377,     0,     0,     0,   268,   270,   258,   370,
     374,     0,   356,   360,   381,     0,     0,   378,   387,     0,
     384,   382,   385,     0,   386
};

/* TAO_YYDEFGOTO[NTERM-NUM]. */
static const short tao_yydefgoto[] =
{
      -1,     1,     2,    23,   146,   150,   151,   145,   149,    96,
      95,   100,   152,   154,   156,    67,    24,    69,   225,   310,
     478,    25,    26,    99,   236,   326,    27,    70,    28,   117,
     235,    29,    30,    31,   101,   237,   341,    32,   182,   262,
     409,    33,   203,    34,    87,   194,   241,    35,    36,   271,
     342,   343,   433,   432,   270,   327,   418,   429,   430,   417,
     420,   419,   421,   413,   268,   320,   487,   287,   187,   234,
      93,    37,   328,    68,   224,   309,   477,   162,   522,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     329,    71,   173,   226,   367,   120,   121,   122,   123,    40,
     255,   312,   481,   256,   547,   562,   599,   257,   258,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,    41,
      72,    42,   147,   245,   364,   301,   365,   451,   303,   368,
     453,   537,   452,    43,    73,    44,   211,   304,   454,   538,
     566,   611,   375,   586,   612,   587,   613,   647,   608,   588,
     614,   589,   610,   609,   645,   634,   646,    45,    46,    47,
      74,   227,   313,   482,   406,   483,   544,   407,   139,   242,
     361,   140,   202,   272,   141,   243,   362,   142,   143,   244,
     363,   144,   259,   311,   402,   480,   403,   479,   543,   330,
     331,   414,   533,   560,   596,   332,   415,   534,   561,   598,
     333,    75,   228,   314,   484,   334,   496,   549,   581,   627,
     335,   427,   345,   431,   536,   565,   541,   553,   554,   569,
     592,   637,   570,   590,   636,   564,   579,   580,   604,   625,
     653,   605,   626,   654,   428,   606,   572,   593,   638,   577,
     597,   639,   622,   640,   661,   644,   655,   666,   669,   670,
     673,   336,   337,    51,    52,    53,   153,   247,   383,    54,
     185,   264,   231,   263,   305,   384,   465,   466,   467,   468,
     469,   463,   385,   459,   386,   462,   387,   388,   389,    55,
     155,    56,    90,   232,   319,   486,   545,   574,   266,   318,
     595,   219,   248,   393,   306,   394,   473,   474,   395,   516,
     555,   396,   517,   556,    57,    58,    59,    60,    61,   222,
     307,   475,    62,    63,    64,   158,   221,    65,   223,   308,
     476,    66
};

/* TAO_YYPACT[STATE-NUM] -- Index in TAO_YYTABLE of the portion describing
   STATE-NUM.  */
#define TAO_YYPACT_NINF -483
static const short tao_yypact[] =
{
    -483,    49,  1047,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,    61,    60,    66,    68,  -483,    61,    61,
    -483,    40,    40,  -483,  -483,  -483,  -483,   -45,  -483,  -483,
    -483,  -483,  -483,  -483,    70,  -483,  -483,  -483,  -483,  -483,
    -483,    -4,  -483,    88,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,    10,  -483,  -483,  -483,    10,    39,   221,   147,
      61,   467,    61,    61,    61,    61,  -483,  -483,  -483,    48,
      61,    69,  -483,   101,    61,  -483,    10,    61,    75,    77,
      61,  -483,   -33,  -483,   -11,    79,   109,    72,  -483,   118,
     131,   132,   185,  -483,    91,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,   144,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,    88,  -483,  -483,  -483,
     -52,  -483,   123,  -483,   126,   160,   170,   172,  -483,   181,
     188,   191,   194,   197,   200,   201,   203,  -483,  -483,  -483,
    -483,   144,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
     205,  -483,   216,  -483,  -483,   215,  -483,    61,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
     293,  -483,   207,   255,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,   211,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,   255,   226,   229,    61,   231,    61,   233,   235,  -483,
    -483,  -483,   234,  -483,    61,    40,  -483,  -483,  -483,  -483,
      40,  -483,    35,    35,    35,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,   220,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,    40,   255,  -483,  -483,  -483,  -483,   144,
     866,   641,   494,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,    65,    65,    65,    35,   144,  -483,   237,
     239,   236,   106,    52,   110,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,   152,   176,   238,   791,  -483,  -483,   240,
    -483,   228,   248,   329,  -483,   901,   144,  -483,    40,   255,
     250,  -483,  -483,  -483,  -483,  -483,   253,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  1009,  -483,  -483,  -483,  -483,
    -483,   257,  -483,  -483,  -483,   260,  -483,  -483,  -483,  -483,
     246,    35,    35,    35,    35,    35,    35,    35,    35,    35,
      35,   249,   251,   252,   261,   263,  -483,  -483,  -483,   338,
     144,  -483,  -483,  -483,  -483,  -483,  -483,    40,    40,    37,
      40,   291,  -483,   271,  -483,  -483,  -483,  -483,  -483,  -483,
      61,    61,  -483,   272,  -483,  -483,  -483,   976,   716,  -483,
     287,  -483,  -483,  -483,  -483,  -483,  -483,  -483,   275,   276,
     144,  -483,  -483,   279,   332,   377,  -483,   285,   286,   288,
     290,   292,  -483,   144,  -483,  -483,  -483,  -483,  -483,   294,
     295,   363,   152,   152,  -483,  -483,  -483,   239,   236,   106,
      52,    52,   110,   110,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,   296,    61,   277,    42,    42,  -483,   144,    61,
      42,  -483,    37,   297,  -483,   300,   302,   303,   304,   305,
    -483,  -483,  -483,   319,   320,   289,   323,    35,   324,    35,
     228,    61,   325,   341,   344,  -483,   360,    40,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,   424,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,    61,
    -483,  -483,  -483,  -483,  -483,  -483,   334,   334,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,   144,   377,    61,  -483,   334,   350,   351,  -483,
     395,  -483,  -483,   333,   329,    40,  -483,  -483,  -483,   339,
    -483,  -483,  -483,   340,   399,   398,   398,  -483,  -483,   144,
      61,   382,   355,   353,  -483,   398,    97,  -483,  -483,   354,
    -483,  -483,  -483,  -483,   380,  -483,  -483,  -483,  -483,   357,
     171,   398,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
     377,  -483,   362,   356,    40,  -483,   398,   359,   392,    61,
    -483,  -483,  -483,  -483,   361,  -483,  -483,  -483,   376,    35,
     375,   378,    41,   467,   117,  -483,  -483,  -483,   144,  -483,
    -483,  -483,  -483,  -483,  -483,   379,   377,   419,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,    61,   399,    40,    40,
     366,  -483,  -483,  -483,  -483,   381,    61,   385,  -483,  -483,
     370,   371,  -483,   171,    61,   367,  -483,  -483,  -483,  -483,
    -483,    40,  -483,  -483,  -483,   372,   402,  -483,  -483,   394,
     391,  -483,  -483,   425,  -483
};

/* TAO_YYPGOTO[NTERM-NUM].  */
static const short tao_yypgoto[] =
{
    -483,  -483,   189,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,   180,  -483,  -483,   182,
    -483,  -483,  -483,   479,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,   483,  -483,   199,  -483,  -483,  -188,  -483,  -483,   192,
    -483,  -483,  -483,  -483,  -210,  -241,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,  -220,  -483,  -483,   -18,  -483,  -483,
     -13,  -483,    11,  -483,  -483,  -483,  -483,  -483,  -483,  -274,
    -483,   150,   162,   163,  -127,  -114,  -101,    36,  -483,  -217,
      20,  -483,  -483,  -483,   -24,   245,  -314,  -483,  -483,  -483,
      71,  -483,  -483,  -449,   -37,  -483,  -483,   -12,  -483,   -53,
    -483,  -483,   457,   458,   -51,   -49,   -40,  -483,  -483,   -25,
    -483,   -23,  -483,  -483,  -483,  -483,   214,   298,  -483,  -167,
    -483,  -483,  -483,   -20,  -483,   -16,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,   -81,  -483,  -483,  -483,  -483,
    -483,   -82,  -483,  -483,  -483,  -483,  -483,  -483,  -483,   -32,
    -483,  -483,  -483,  -483,  -483,  -483,  -483,   -10,  -483,  -483,
    -483,  -483,  -483,  -483,   -61,  -483,  -483,  -483,   -60,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,    53,  -483,  -483,   230,
    -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
      21,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,  -482,  -483,  -483,  -483,
    -483,  -483,  -100,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -117,  -483,  -483,  -386,  -483,  -340,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,    22,    23,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,    76,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,  -483,
    -483,  -483,  -483,  -483,  -483,   473,  -483,  -483,  -483,  -483,
    -483,  -483
};

/* TAO_YYTABLE[TAO_YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what TAO_YYDEFACT says.
   If TAO_YYTABLE_NINF, syntax error.  */
#define TAO_YYTABLE_NINF -475
static const short tao_yytable[] =
{
      77,    78,   138,    92,    94,    88,    89,   169,   170,   134,
     119,   135,   350,    38,   136,   163,   118,   166,   137,   167,
     273,   424,    39,    48,    49,    50,   299,   300,   168,   490,
     344,   204,   527,   249,  -110,   542,   188,    97,    76,   138,
      76,   189,   583,    76,   205,    76,   134,   174,   135,     3,
     161,   136,   315,   118,   550,   137,   457,   172,   190,   175,
     176,   177,   178,   189,    76,   392,     7,   180,    76,   584,
     585,   183,     7,    76,   184,  -271,   317,   186,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
       9,    10,    97,    11,   114,   115,   116,   397,   583,   200,
     201,   424,   274,   275,   276,   277,   278,   279,   280,    91,
     148,    91,   281,   282,    91,    17,   189,    17,   160,    80,
     193,    84,  -253,   283,   284,   584,   585,  -111,   285,   286,
      97,   411,   274,   275,   276,   277,   278,   279,   280,    91,
     356,   357,   281,   282,    91,   584,   585,   546,  -112,   -71,
     171,    97,    97,   366,  -392,    76,  -461,   344,   191,   286,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,     9,    10,   233,    11,   114,   115,   116,    76,
     -70,   354,   355,    97,   369,   103,   104,   648,   192,   107,
     108,   109,   110,   198,    79,    81,   199,   657,   195,    11,
     358,   359,   360,   523,   615,   663,   601,   602,   603,    98,
     196,   252,   197,   254,    83,    86,   573,   269,   189,   424,
     206,   267,   269,   207,    76,   582,    91,   440,   441,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   208,
     642,   607,   442,   443,   157,   316,   115,   116,   157,   209,
      91,   371,   210,   372,   118,   373,   619,   444,   445,   446,
     212,   179,   525,   181,   374,   500,   501,   213,   322,   323,
     214,   138,   376,   215,   425,   426,   424,   216,   134,   217,
     135,   218,   220,   136,  -462,   118,   370,   137,     4,   377,
     378,     5,     6,     7,     8,    91,   229,   230,   379,   380,
     410,   238,   240,   381,   239,   246,   250,     9,    10,   251,
      11,   253,   424,   260,    12,   261,   265,   423,  -300,   347,
     348,   349,   351,   353,   399,   352,   401,    13,    14,    15,
      16,   404,   405,   412,   416,   629,    17,    18,   434,   435,
      19,   436,   450,    20,  -228,   447,   198,   448,   449,   461,
      21,    22,   464,   472,   425,   426,  -323,   485,   488,   455,
     456,   458,   460,   489,   491,   492,   499,   493,   -32,   494,
     520,   495,   504,   497,   498,   502,   510,   470,   471,   511,
      76,   512,   513,   514,   515,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   423,   518,   519,
     138,   138,   115,   116,   521,   524,   528,   134,   134,   135,
     135,    38,   136,   136,   118,   118,   137,   137,   650,   651,
      39,    48,    49,    50,   529,   530,   531,   535,   540,   551,
    -341,   552,   557,   563,   568,   567,   571,   576,   578,   594,
     254,   665,   505,   506,   458,   616,   507,   508,  -350,   591,
     617,    91,   600,   620,   621,   628,   624,   630,   643,   631,
     652,   664,   641,   656,   658,   659,   660,   667,   254,   532,
      76,   668,   425,   426,   672,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,     9,    10,   671,
      11,   114,   115,   116,   674,    85,   539,    76,    82,   400,
     398,   437,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   438,   423,   439,   346,   114,   115,
     116,    77,   548,   575,   503,   164,   165,   559,   408,   425,
     426,   632,   635,   526,   558,   382,   662,   649,   509,   159,
       0,    91,     0,   302,     0,     0,     0,    77,   548,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   425,   426,     0,    91,     0,
       0,     0,   423,     0,     0,     0,   618,     0,     0,     0,
       0,   138,     0,     0,     0,     0,    77,   623,   134,   633,
     135,     0,     0,   136,     0,   118,     0,   137,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     269,   269,     0,   254,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   254,     0,     0,     0,     0,     0,     0,
       0,   254,   321,   269,  -332,     5,     0,     0,     8,  -332,
    -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,     9,    10,     0,    11,     0,  -332,  -332,    12,     0,
       0,   322,   323,   324,   325,  -332,     0,     0,     0,     0,
       0,    13,     0,     0,     0,   338,   339,   340,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,    22,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -332,     0,   321,     0,  -332,
       5,     0,   -56,     8,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,  -332,  -332,  -332,  -332,  -332,     9,    10,     0,    11,
       0,  -332,  -332,    12,     0,     0,   322,   323,   324,   325,
    -332,     0,     0,     0,     0,     0,    13,     0,     0,     0,
     338,   339,   340,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    21,
      22,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -332,     0,   321,     0,  -332,     5,     0,  -474,     8,  -332,
    -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,     9,    10,     0,    11,     0,  -332,  -332,    12,     0,
       0,   322,   323,   324,   325,  -332,     0,     0,     0,     0,
       0,    13,     0,     0,     0,   390,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   391,     0,     0,     0,     0,
       0,     0,     0,     0,    21,    22,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -332,     0,   321,     0,  -332,
       5,     0,  -441,     8,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,  -332,  -332,  -332,  -332,  -332,     9,    10,     0,    11,
       0,  -332,  -332,    12,     0,     0,   322,   323,   324,   325,
    -332,     0,   321,     0,  -332,     5,    13,     0,     8,  -332,
    -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,     9,    10,     0,    11,     0,  -332,  -332,    12,    21,
      22,   322,   323,   324,   325,  -332,     0,     0,     0,     0,
    -332,    13,     0,     0,     0,     0,     0,   -38,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    21,    22,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -332,     0,   321,     0,  -332,
       5,     0,   -60,     8,  -332,  -332,  -332,  -332,  -332,  -332,
    -332,  -332,  -332,  -332,  -332,  -332,     9,    10,     0,    11,
       0,  -332,  -332,    12,     0,     0,   322,   323,   324,   325,
    -332,     0,    76,     0,     0,     0,    13,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,     0,
       0,     0,     0,     0,   115,   116,     0,     0,     0,    21,
      22,     0,     0,   422,     0,     0,     0,    -2,     4,     0,
    -332,     5,     6,     7,     8,     0,     0,  -465,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     9,    10,     0,
      11,     0,     0,     0,    12,     0,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,    13,    14,    15,
      16,     0,     0,     0,     0,     0,    17,    18,     0,     0,
      19,     0,     0,    20,     0,     0,     0,     0,     0,     0,
      21,    22
};

static const short tao_yycheck[] =
{
      13,    13,    34,    21,    22,    18,    19,    68,    68,    34,
      34,    34,   286,     2,    34,    68,    34,    68,    34,    68,
     240,   335,     2,     2,     2,     2,   243,   244,    68,   415,
     271,    83,   481,   221,    79,   517,    69,    82,     3,    71,
       3,    74,     1,     3,    96,     3,    71,    71,    71,     0,
      68,    71,   262,    71,   536,    71,    19,    70,    69,    72,
      73,    74,    75,    74,     3,   306,     6,    80,     3,    28,
      29,    84,     6,     3,    87,    79,   264,    90,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    82,    23,    24,    25,    26,   307,     1,     8,
       9,   415,    67,    68,    69,    70,    71,    72,    73,    74,
      22,    74,    77,    78,    74,    49,    74,    49,    79,    53,
      48,    53,    81,    88,    89,    28,    29,    79,    93,    94,
      82,   319,    67,    68,    69,    70,    71,    72,    73,    74,
      88,    89,    77,    78,    74,    28,    29,   533,    79,    79,
       3,    82,    82,     1,    79,     3,    79,   398,    79,    94,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,   187,    23,    24,    25,    26,     3,
      79,    75,    76,    82,     8,     9,    10,   636,    79,    13,
      14,    15,    16,     8,    14,    15,    11,   646,    80,    23,
      90,    91,    92,   477,   590,   654,    35,    36,    37,    27,
      79,   224,    80,   226,    15,    16,   556,   235,    74,   533,
      97,   234,   240,    97,     3,   565,    74,   354,   355,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    79,
     626,   581,   356,   357,    62,   263,    25,    26,    66,    79,
      74,   304,    80,   304,   272,   304,   596,   358,   359,   360,
      79,    79,   479,    81,   304,   432,   433,    79,    30,    31,
      79,   303,   304,    79,   335,   335,   590,    80,   303,    79,
     303,    80,    79,   303,    79,   303,   304,   303,     1,    51,
      52,     4,     5,     6,     7,    74,    80,    82,    60,    61,
     318,     8,    47,    65,    97,    94,    80,    20,    21,    80,
      23,    80,   626,    80,    27,    80,    82,   335,    98,   283,
     284,   285,    85,    87,    84,    86,    98,    40,    41,    42,
      43,    83,     3,    83,    81,   609,    49,    50,    81,    79,
      53,    95,    81,    56,    81,    96,     8,    96,    96,    58,
      63,    64,    81,    81,   415,   415,    81,    81,    79,   377,
     378,   379,   380,    31,    79,    79,     3,    79,    81,    79,
      81,    79,    95,    79,    79,    79,    79,   390,   391,    79,
       3,    79,    79,    79,    79,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,   415,    79,    79,
     432,   433,    25,    26,    81,    81,    81,   432,   433,   432,
     433,   400,   432,   433,   432,   433,   432,   433,   638,   639,
     400,   400,   400,   400,    83,    81,    66,     3,    94,    79,
      35,    80,    99,    94,    35,    95,    38,    55,    83,    59,
     453,   661,   455,   456,   462,    83,   459,   460,    95,    95,
      94,    74,    95,    94,    62,    79,    95,    82,    39,    81,
      94,    94,    83,    82,    79,    95,    95,    95,   481,   487,
       3,    69,   533,   533,    83,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    95,
      23,    24,    25,    26,    69,    16,   509,     3,    15,   310,
     308,   351,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,   352,   533,   353,   272,    24,    25,
      26,   534,   534,   560,   453,    68,    68,   545,   314,   590,
     590,   612,   614,   480,   544,   305,   653,   637,   462,    66,
      -1,    74,    -1,   245,    -1,    -1,    -1,   560,   560,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   626,   626,    -1,    74,    -1,
      -1,    -1,   590,    -1,    -1,    -1,   594,    -1,    -1,    -1,
      -1,   613,    -1,    -1,    -1,    -1,   599,   599,   613,   613,
     613,    -1,    -1,   613,    -1,   613,    -1,   613,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   626,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     638,   639,    -1,   636,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   646,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   654,     1,   661,     3,     4,    -1,    -1,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    -1,    25,    26,    27,    -1,
      -1,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    44,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    -1,     1,    -1,     3,
       4,    -1,    81,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      -1,    25,    26,    27,    -1,    -1,    30,    31,    32,    33,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    -1,     1,    -1,     3,     4,    -1,    81,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    -1,    25,    26,    27,    -1,
      -1,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    -1,     1,    -1,     3,
       4,    -1,    81,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      -1,    25,    26,    27,    -1,    -1,    30,    31,    32,    33,
      34,    -1,     1,    -1,     3,     4,    40,    -1,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    23,    -1,    25,    26,    27,    63,
      64,    30,    31,    32,    33,    34,    -1,    -1,    -1,    -1,
      74,    40,    -1,    -1,    -1,    -1,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    -1,     1,    -1,     3,
       4,    -1,    81,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    -1,    23,
      -1,    25,    26,    27,    -1,    -1,    30,    31,    32,    33,
      34,    -1,     3,    -1,    -1,    -1,    40,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    -1,    -1,    25,    26,    -1,    -1,    -1,    63,
      64,    -1,    -1,    34,    -1,    -1,    -1,     0,     1,    -1,
      74,     4,     5,     6,     7,    -1,    -1,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    20,    21,    -1,
      23,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    74,    -1,    -1,    -1,    40,    41,    42,
      43,    -1,    -1,    -1,    -1,    -1,    49,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64
};

/* TAO_YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short tao_yystos[] =
{
       0,   101,   102,     0,     1,     4,     5,     6,     7,    20,
      21,    23,    27,    40,    41,    42,    43,    49,    50,    53,
      56,    63,    64,   103,   116,   121,   122,   126,   128,   131,
     132,   133,   137,   141,   143,   147,   148,   171,   172,   190,
     199,   219,   221,   233,   235,   257,   258,   259,   300,   351,
     352,   353,   354,   355,   359,   379,   381,   404,   405,   406,
     407,   408,   412,   413,   414,   417,   421,   115,   173,   117,
     127,   191,   220,   234,   260,   301,     3,   170,   207,   126,
      53,   126,   141,   143,    53,   133,   143,   144,   170,   170,
     382,    74,   167,   170,   167,   110,   109,    82,   129,   123,
     111,   134,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    24,    25,    26,   129,   167,   194,
     195,   196,   197,   198,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   221,   233,   235,   259,   268,
     271,   274,   277,   278,   281,   107,   104,   222,    22,   108,
     105,   106,   112,   356,   113,   380,   114,   129,   415,   415,
      79,   167,   177,   209,   212,   213,   214,   215,   216,   274,
     278,     3,   170,   192,   194,   170,   170,   170,   170,   129,
     170,   129,   138,   170,   170,   360,   170,   168,    69,    74,
      69,    79,    79,    48,   145,    80,    79,    80,     8,    11,
       8,     9,   272,   142,    83,    96,    97,    97,    79,    79,
      80,   236,    79,    79,    79,    79,    80,    79,    80,   391,
      79,   416,   409,   418,   174,   118,   193,   261,   302,    80,
      82,   362,   383,   170,   169,   130,   124,   135,     8,    97,
      47,   146,   269,   275,   279,   223,    94,   357,   392,   146,
      80,    80,   170,    80,   170,   200,   203,   207,   208,   282,
      80,    80,   139,   363,   361,    82,   388,   170,   164,   167,
     154,   149,   273,   164,    67,    68,    69,    70,    71,    72,
      73,    77,    78,    88,    89,    93,    94,   167,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   189,
     189,   225,   227,   228,   237,   364,   394,   410,   419,   175,
     119,   283,   201,   262,   303,   154,   167,   146,   389,   384,
     165,     1,    30,    31,    32,    33,   125,   155,   172,   190,
     289,   290,   295,   300,   305,   310,   351,   352,    44,    45,
      46,   136,   150,   151,   155,   312,   195,   187,   187,   187,
     179,    85,    86,    87,    75,    76,    88,    89,    90,    91,
      92,   270,   276,   280,   224,   226,     1,   194,   229,     8,
     167,   209,   214,   215,   216,   242,   259,    51,    52,    60,
      61,    65,   289,   358,   365,   372,   374,   376,   377,   378,
      44,    54,   155,   393,   395,   398,   401,   154,   149,    84,
     102,    98,   284,   286,    83,     3,   264,   267,   226,   140,
     167,   146,    83,   163,   291,   296,    81,   159,   156,   161,
     160,   162,    34,   167,   196,   274,   278,   311,   334,   157,
     158,   313,   153,   152,    81,    79,    95,   181,   182,   183,
     184,   184,   185,   185,   186,   186,   186,    96,    96,    96,
      81,   227,   232,   230,   238,   167,   167,    19,   167,   373,
     167,    58,   375,   371,    81,   366,   367,   368,   369,   370,
     170,   170,    81,   396,   397,   411,   420,   176,   120,   287,
     285,   202,   263,   265,   304,    81,   385,   166,    79,    31,
     334,    79,    79,    79,    79,    79,   306,    79,    79,     3,
     229,   229,    79,   200,    95,   170,   170,   170,   170,   373,
      79,    79,    79,    79,    79,    79,   399,   402,    79,    79,
      81,    81,   178,   179,    81,   189,   286,   203,    81,    83,
      81,    66,   167,   292,   297,     3,   314,   231,   239,   170,
      94,   316,   316,   288,   266,   386,   334,   204,   207,   307,
     316,    79,    80,   317,   318,   400,   403,    99,   267,   167,
     293,   298,   205,    94,   325,   315,   240,    95,    35,   319,
     322,    38,   336,   336,   387,   204,    55,   339,    83,   326,
     327,   308,   336,     1,    28,    29,   243,   245,   249,   251,
     323,    95,   320,   337,    59,   390,   294,   340,   299,   206,
      95,    35,    36,    37,   328,   331,   335,   336,   248,   253,
     252,   241,   244,   246,   250,   334,    83,    94,   167,   336,
      94,    62,   342,   207,    95,   329,   332,   309,    79,   179,
      82,    81,   245,   194,   255,   251,   324,   321,   338,   341,
     343,    83,   334,    39,   345,   254,   256,   247,   203,   322,
     164,   164,    94,   330,   333,   346,    82,   203,    79,    95,
      95,   344,   331,   203,    94,   164,   347,    95,    69,   348,
     349,    95,    83,   350,    69
};

#if ! defined (TAO_YYSIZE_T) && defined (__SIZE_TYPE__)
# define TAO_YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (TAO_YYSIZE_T) && defined (size_t)
# define TAO_YYSIZE_T size_t
#endif
#if ! defined (TAO_YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define TAO_YYSIZE_T size_t
# endif
#endif
#if ! defined (TAO_YYSIZE_T)
# define TAO_YYSIZE_T unsigned int
#endif

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
      TAO_YYPOPSTACK;                                                \
      goto tao_yybackup;                                                \
    }                                                                \
  else                                                                \
    {                                                                 \
      tao_yyerror ("syntax error: cannot back up");\
      TAO_YYERROR;                                                        \
    }                                                                \
while (0)

#define TAO_YYTERROR        1
#define TAO_YYERRCODE        256

/* TAO_YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef TAO_YYLLOC_DEFAULT
# define TAO_YYLLOC_DEFAULT(Current, Rhs, N)                \
   ((Current).first_line   = (Rhs)[1].first_line,        \
    (Current).first_column = (Rhs)[1].first_column,        \
    (Current).last_line    = (Rhs)[N].last_line,        \
    (Current).last_column  = (Rhs)[N].last_column)
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
} while (0)

# define TAO_YYDSYMPRINT(Args)                        \
do {                                                \
  if (tao_yydebug)                                        \
    tao_yysymprint Args;                                \
} while (0)

# define TAO_YYDSYMPRINTF(Title, Token, Value, Location)                \
do {                                                                \
  if (tao_yydebug)                                                        \
    {                                                                \
      TAO_YYFPRINTF (stderr, "%s ", Title);                                \
      tao_yysymprint (stderr,                                         \
                  Token, Value);        \
      TAO_YYFPRINTF (stderr, "\n");                                        \
    }                                                                \
} while (0)

/*------------------------------------------------------------------.
| tao_yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
tao_yy_stack_print (short *bottom, short *top)
#else
static void
tao_yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  TAO_YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    TAO_YYFPRINTF (stderr, " %d", *bottom);
  TAO_YYFPRINTF (stderr, "\n");
}

# define TAO_YY_STACK_PRINT(Bottom, Top)                                \
do {                                                                \
  if (tao_yydebug)                                                        \
    tao_yy_stack_print ((Bottom), (Top));                                \
} while (0)


/*------------------------------------------------.
| Report that the TAO_YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
tao_yy_reduce_print (int tao_yyrule)
#else
static void
tao_yy_reduce_print (tao_yyrule)
    int tao_yyrule;
#endif
{
  int tao_yyi;
  unsigned int tao_yylno = tao_yyrline[tao_yyrule];
  TAO_YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             tao_yyrule - 1, tao_yylno);
  /* Print the symbols being reduced, and their result.  */
  for (tao_yyi = tao_yyprhs[tao_yyrule]; 0 <= tao_yyrhs[tao_yyi]; tao_yyi++)
    TAO_YYFPRINTF (stderr, "%s ", tao_yytname [tao_yyrhs[tao_yyi]]);
  TAO_YYFPRINTF (stderr, "-> %s\n", tao_yytname [tao_yyr1[tao_yyrule]]);
}

# define TAO_YY_REDUCE_PRINT(Rule)                \
do {                                        \
  if (tao_yydebug)                                \
    tao_yy_reduce_print (Rule);                \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int tao_yydebug;
#else /* !TAO_YYDEBUG */
# define TAO_YYDPRINTF(Args)
# define TAO_YYDSYMPRINT(Args)
# define TAO_YYDSYMPRINTF(Title, Token, Value, Location)
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
   SIZE_MAX < TAO_YYSTACK_BYTES (TAO_YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if defined (TAO_YYMAXDEPTH) && TAO_YYMAXDEPTH == 0
# undef TAO_YYMAXDEPTH
#endif

#ifndef TAO_YYMAXDEPTH
# define TAO_YYMAXDEPTH 10000
#endif



#if TAO_YYERROR_VERBOSE

# ifndef tao_yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define tao_yystrlen strlen
#  else
/* Return the length of TAO_YYSTR.  */
static TAO_YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
tao_yystrlen (const char *tao_yystr)
#   else
tao_yystrlen (tao_yystr)
     const char *tao_yystr;
#   endif
{
  register const char *tao_yys = tao_yystr;

  while (*tao_yys++ != '\0')
    continue;

  return tao_yys - tao_yystr - 1;
}
#  endif
# endif

# ifndef tao_yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define tao_yystpcpy stpcpy
#  else
/* Copy TAO_YYSRC to TAO_YYDEST, returning the address of the terminating '\0' in
   TAO_YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
tao_yystpcpy (char *tao_yydest, const char *tao_yysrc)
#   else
tao_yystpcpy (tao_yydest, tao_yysrc)
     char *tao_yydest;
     const char *tao_yysrc;
#   endif
{
  register char *tao_yyd = tao_yydest;
  register const char *tao_yys = tao_yysrc;

  while ((*tao_yyd++ = *tao_yys++) != '\0')
    continue;

  return tao_yyd - 1;
}
#  endif
# endif

#endif /* !TAO_YYERROR_VERBOSE */



#if TAO_YYDEBUG
/*--------------------------------.
| Print this symbol on TAO_YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
tao_yysymprint (FILE *tao_yyoutput, int tao_yytype, TAO_YYSTYPE *tao_yyvaluep)
#else
static void
tao_yysymprint (tao_yyoutput, tao_yytype, tao_yyvaluep)
    FILE *tao_yyoutput;
    int tao_yytype;
    TAO_YYSTYPE *tao_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) tao_yyvaluep;

  if (tao_yytype < TAO_YYNTOKENS)
    {
      TAO_YYFPRINTF (tao_yyoutput, "token %s (", tao_yytname[tao_yytype]);
# ifdef TAO_YYPRINT
      TAO_YYPRINT (tao_yyoutput, tao_yytoknum[tao_yytype], *tao_yyvaluep);
# endif
    }
  else
    TAO_YYFPRINTF (tao_yyoutput, "nterm %s (", tao_yytname[tao_yytype]);

  TAO_YYFPRINTF (tao_yyoutput, ")");
}

#endif /* ! TAO_YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
tao_yydestruct (int tao_yytype, TAO_YYSTYPE *tao_yyvaluep)
#else
static void
tao_yydestruct (tao_yytype, tao_yyvaluep)
    int tao_yytype;
    TAO_YYSTYPE *tao_yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) tao_yytype;
  (void) tao_yyvaluep;

}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef TAO_YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int tao_yyparse (void *TAO_YYPARSE_PARAM);
# else
int tao_yyparse ();
# endif
#else /* ! TAO_YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int tao_yyparse (void);
#else
int tao_yyparse ();
#endif
#endif /* ! TAO_YYPARSE_PARAM */



/* The lookahead symbol.  */
int tao_yychar;

/* The semantic value of the lookahead symbol.  */
TAO_YYSTYPE tao_yylval;

/* Number of syntax errors so far.  */
int tao_yynerrs;



/*----------.
| tao_yyparse.  |
`----------*/

#ifdef TAO_YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int tao_yyparse (void *TAO_YYPARSE_PARAM)
# else
int tao_yyparse (TAO_YYPARSE_PARAM)
  void *TAO_YYPARSE_PARAM;
# endif
#else /* ! TAO_YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
tao_yyparse (void)
#else
int
tao_yyparse ()

#endif
#endif
{

  register int tao_yystate;
  register int tao_yyn;
  int tao_yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int tao_yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int tao_yytoken = 0;

  /* Three stacks and their tools:
     `tao_yyss': related to states,
     `tao_yyvs': related to semantic values,
     `tao_yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow tao_yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short        tao_yyssa[TAO_YYINITDEPTH];
  short *tao_yyss = tao_yyssa;
  register short *tao_yyssp;

  /* The semantic value stack.  */
  TAO_YYSTYPE tao_yyvsa[TAO_YYINITDEPTH];
  TAO_YYSTYPE *tao_yyvs = tao_yyvsa;
  register TAO_YYSTYPE *tao_yyvsp;



#define TAO_YYPOPSTACK   (tao_yyvsp--, tao_yyssp--)

  TAO_YYSIZE_T tao_yystacksize = TAO_YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  TAO_YYSTYPE tao_yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int tao_yylen;

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
     have just been pushed. so pushing a state here evens the stacks.
     */
  tao_yyssp++;

 tao_yysetstate:
  *tao_yyssp = tao_yystate;

  if (tao_yyss + tao_yystacksize - 1 <= tao_yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      TAO_YYSIZE_T tao_yysize = tao_yyssp - tao_yyss + 1;

#ifdef tao_yyoverflow
      {
        /* Give user a chance to reallocate the stack. Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        TAO_YYSTYPE *tao_yyvs1 = tao_yyvs;
        short *tao_yyss1 = tao_yyss;


        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if tao_yyoverflow is a macro.  */
        tao_yyoverflow ("parser stack overflow",
                    &tao_yyss1, tao_yysize * sizeof (*tao_yyssp),
                    &tao_yyvs1, tao_yysize * sizeof (*tao_yyvsp),

                    &tao_yystacksize);

        tao_yyss = tao_yyss1;
        tao_yyvs = tao_yyvs1;
      }
#else /* no tao_yyoverflow */
# ifndef TAO_YYSTACK_RELOCATE
      goto tao_yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (TAO_YYMAXDEPTH <= tao_yystacksize)
        goto tao_yyoverflowlab;
      tao_yystacksize *= 2;
      if (TAO_YYMAXDEPTH < tao_yystacksize)
        tao_yystacksize = TAO_YYMAXDEPTH;

      {
        short *tao_yyss1 = tao_yyss;
        union tao_yyalloc *tao_yyptr =
          (union tao_yyalloc *) TAO_YYSTACK_ALLOC (TAO_YYSTACK_BYTES (tao_yystacksize));
        if (! tao_yyptr)
          goto tao_yyoverflowlab;
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

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* tao_yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  tao_yyn = tao_yypact[tao_yystate];
  if (tao_yyn == TAO_YYPACT_NINF)
    goto tao_yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* TAO_YYCHAR is either TAO_YYEMPTY or TAO_YYEOF or a valid lookahead symbol.  */
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
      TAO_YYDSYMPRINTF ("Next token is", tao_yytoken, &tao_yylval, &tao_yylloc);
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

  /* Shift the lookahead token.  */
  TAO_YYDPRINTF ((stderr, "Shifting token %s, ", tao_yytname[tao_yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (tao_yychar != TAO_YYEOF)
    tao_yychar = TAO_YYEMPTY;

  *++tao_yyvsp = tao_yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (tao_yyerrstatus)
    tao_yyerrstatus--;

  tao_yystate = tao_yyn;
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
//      | module
          idl_global->set_parse_state (IDL_GlobalData::PS_ModuleDeclSeen);
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
//      | value_def
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeDeclSeen);
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
//      | component
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentDeclSeen);
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
//      | home_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeDeclSeen);
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
//      | event
          idl_global->set_parse_state (IDL_GlobalData::PS_EventDeclSeen);
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
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
    break;

  case 28:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 29:

    {
// module  : IDL_MODULE
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSeen);
          }
    break;

  case 30:

    {
//        IDENTIFIER
            Identifier id (tao_yyvsp[0].strval);
            ACE::strdelete (tao_yyvsp[0].strval);
            tao_yyvsp[0].strval = 0;
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

  case 31:

    {
//        '{'
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleSqSeen);
          }
    break;

  case 32:

    {
//        definitions
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleBodySeen);
          }
    break;

  case 33:

    {
//        '}'
            idl_global->set_parse_state (IDL_GlobalData::PS_ModuleQsSeen);
            /*
             * Finished with this module - pop it from the scope stack.
             */
            idl_global->scopes ().pop ();
          }
    break;

  case 36:

    {
// interface : interface_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Interface *i = 0;

          /*
           * Make a new interface node and add it to its enclosing scope.
           */
          if (s != 0 && tao_yyvsp[0].ihval != 0)
            {
              i =
                idl_global->gen ()->create_interface (
                                        tao_yyvsp[0].ihval->name (),
                                        tao_yyvsp[0].ihval->inherits (),
                                        tao_yyvsp[0].ihval->n_inherits (),
                                        tao_yyvsp[0].ihval->inherits_flat (),
                                        tao_yyvsp[0].ihval->n_inherits_flat (),
                                        tao_yyvsp[0].ihval->is_local (),
                                        tao_yyvsp[0].ihval->is_abstract ()
                                      );
              AST_Interface::fwd_redefinition_helper (i,
                                                      s);
              /*
               * Add the interface to its definition scope.
               */
              (void) s->fe_add_interface (i);

              // This FE_InterfaceHeader class isn't destroyed with the AST.
              tao_yyvsp[0].ihval->destroy ();
              delete tao_yyvsp[0].ihval;
              tao_yyvsp[0].ihval = 0;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (i);
        }
    break;

  case 37:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSqSeen);
        }
    break;

  case 38:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceBodySeen);
        }
    break;

  case 39:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceQsSeen);

          /*
           * Done with this interface - pop it off the scopes stack
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 40:

    {
// interface_decl : IDL_INTERFACE
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceSeen);
         }
    break;

  case 41:

    {
//       id
           idl_global->set_parse_state (IDL_GlobalData::PS_InterfaceIDSeen);
           tao_yyval.idval = tao_yyvsp[0].idval;
         }
    break;

  case 42:

    {
// interface_header : interface_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);

          if (tao_yyvsp[0].nlval != 0 && tao_yyvsp[0].nlval->truncatable ())
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
                          UTL_ScopedName (tao_yyvsp[-1].idval, 0),
                          1);
          ACE_NEW_RETURN (tao_yyval.ihval,
                          FE_InterfaceHeader (n,
                                              tao_yyvsp[0].nlval,
                                              false,
                                              false,
                                              true),
                          1);

          if (0 != tao_yyvsp[0].nlval)
            {
              tao_yyvsp[0].nlval->destroy ();
              delete tao_yyvsp[0].nlval;
              tao_yyvsp[0].nlval = 0;
            }
        }
    break;

  case 43:

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
                          UTL_ScopedName (tao_yyvsp[-1].idval, 0),
                          1);
          ACE_NEW_RETURN (tao_yyval.ihval,
                          FE_InterfaceHeader (n,
                                              tao_yyvsp[0].nlval,
                                              true,
                                              false,
                                              true),
                          1);

          if (0 != tao_yyvsp[0].nlval)
            {
              tao_yyvsp[0].nlval->destroy ();
              delete tao_yyvsp[0].nlval;
              tao_yyvsp[0].nlval = 0;
            }
        }
    break;

  case 44:

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
                          UTL_ScopedName (tao_yyvsp[-1].idval, 0),
                          1);
          ACE_NEW_RETURN (tao_yyval.ihval,
                          FE_InterfaceHeader (n,
                                              tao_yyvsp[0].nlval,
                                              false,
                                              true,
                                              true),
                          1);

          if (0 != tao_yyvsp[0].nlval)
            {
              tao_yyvsp[0].nlval->destroy ();
              delete tao_yyvsp[0].nlval;
              tao_yyvsp[0].nlval = 0;
            }
        }
    break;

  case 45:

    {
// inheritance_spec : ':' opt_truncatable
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
    break;

  case 46:

    {
//      at_least_one_scoped_name
          tao_yyvsp[0].nlval->truncatable (tao_yyvsp[-2].bval);
          tao_yyval.nlval = tao_yyvsp[0].nlval;
        }
    break;

  case 47:

    {
/*      |  EMPTY */
          tao_yyval.nlval = 0;
        }
    break;

  case 52:

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

  case 54:

    {
// value_concrete_decl : value_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_ValueType *v = 0;
          AST_Interface *i = 0;

          if (s != 0 && tao_yyvsp[0].vhval != 0)
            {
              v =
                idl_global->gen ()->create_valuetype (
                    tao_yyvsp[0].vhval->name (),
                    tao_yyvsp[0].vhval->inherits (),
                    tao_yyvsp[0].vhval->n_inherits (),
                    tao_yyvsp[0].vhval->inherits_concrete (),
                    tao_yyvsp[0].vhval->inherits_flat (),
                    tao_yyvsp[0].vhval->n_inherits_flat (),
                    tao_yyvsp[0].vhval->supports (),
                    tao_yyvsp[0].vhval->n_supports (),
                    tao_yyvsp[0].vhval->supports_concrete (),
                    false,
                    tao_yyvsp[0].vhval->truncatable (),
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
              tao_yyvsp[0].vhval->destroy ();
              delete tao_yyvsp[0].vhval;
              tao_yyvsp[0].vhval = 0;
            }

          /*
           * Push it on the scope stack
           */
          idl_global->scopes ().push (v);
        }
    break;

  case 55:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
    break;

  case 56:

    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
    break;

  case 57:

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

  case 58:

    {
// value_abs_decl : IDL_ABSTRACT value_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_ValueType *v = 0;
          AST_Interface *i = 0;

          if (s != 0 && tao_yyvsp[0].vhval != 0)
            {
              v =
                idl_global->gen ()->create_valuetype (
                    tao_yyvsp[0].vhval->name (),
                    tao_yyvsp[0].vhval->inherits (),
                    tao_yyvsp[0].vhval->n_inherits (),
                    tao_yyvsp[0].vhval->inherits_concrete (),
                    tao_yyvsp[0].vhval->inherits_flat (),
                    tao_yyvsp[0].vhval->n_inherits_flat (),
                    tao_yyvsp[0].vhval->supports (),
                    tao_yyvsp[0].vhval->n_supports (),
                    tao_yyvsp[0].vhval->supports_concrete (),
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

  case 59:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSqSeen);
        }
    break;

  case 60:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeBodySeen);
        }
    break;

  case 61:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeQsSeen);

          /*
           * Done with this valuetype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 62:

    {
// value_header : value_decl inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 63:

    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);

          UTL_ScopedName *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_ScopedName (tao_yyvsp[-3].idval, 0),
                          1);
          ACE_NEW_RETURN (tao_yyval.vhval,
                          FE_OBVHeader (sn,
                                        tao_yyvsp[-2].nlval,
                                        tao_yyvsp[0].nlval,
                                        tao_yyvsp[-2].nlval ? tao_yyvsp[-2].nlval->truncatable () : false),
                          1);

          if (0 != tao_yyvsp[0].nlval)
            {
              tao_yyvsp[0].nlval->destroy ();
              delete tao_yyvsp[0].nlval;
              tao_yyvsp[0].nlval = 0;
            }

          if (0 != tao_yyvsp[-2].nlval)
            {
              tao_yyvsp[-2].nlval->destroy ();
              delete tao_yyvsp[-2].nlval;
              tao_yyvsp[-2].nlval = 0;
            }
        }
    break;

  case 64:

    {
// value_decl : IDL_VALUETYPE
           idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeSeen);
        }
    break;

  case 65:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueTypeIDSeen);
          tao_yyval.idval = tao_yyvsp[0].idval;
        }
    break;

  case 66:

    {
// opt_truncatable : IDL_TRUNCATABLE
          tao_yyval.bval = true;
        }
    break;

  case 67:

    {
/*      |  EMPTY */
          tao_yyval.bval = false;
        }
    break;

  case 68:

    {
// supports_spec : IDL_SUPPORTS at_least_one_scoped_name
          tao_yyval.nlval = tao_yyvsp[0].nlval;
        }
    break;

  case 69:

    {
/*      |    EMPTY */
          tao_yyval.nlval = 0;
        }
    break;

  case 70:

    {
// value_forward_decl : IDL_ABSTRACT value_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n (tao_yyvsp[0].idval,
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

          tao_yyvsp[0].idval->destroy ();
          delete tao_yyvsp[0].idval;
          tao_yyvsp[0].idval = 0;
        }
    break;

  case 71:

    {
//      | value_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n (tao_yyvsp[0].idval,
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

          tao_yyvsp[0].idval->destroy ();
          delete tao_yyvsp[0].idval;
          tao_yyvsp[0].idval = 0;
        }
    break;

  case 72:

    {
// value_box_decl : value_decl type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_ValueBoxDeclSeen);

          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n (tao_yyvsp[-1].idval,
                            0);

          if (s != 0 && tao_yyvsp[0].dcval != 0)
            {
              /*
              * Get the type_spec associated with the valuebox
              */
              AST_Type *tp = 0;
              AST_Typedef *td
                = AST_Typedef::narrow_from_decl (tao_yyvsp[0].dcval);

              if (td == 0)
                {
                  tp = AST_Type::narrow_from_decl (tao_yyvsp[0].dcval);
                }
              else
                {
                  tp = td->primitive_base_type ();
                }

              if (tp == 0)
                {
                  // The <type_spec> given is a valid type
                  idl_global->err ()->not_a_type (tao_yyvsp[0].dcval);
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

          tao_yyvsp[-1].idval->destroy ();
          delete tao_yyvsp[-1].idval;
          tao_yyvsp[-1].idval = 0;
        }
    break;

  case 78:

    {
// state_member : IDL_PUBLIC
          /* is $0 to member_i */
          tao_yyval.vival = AST_Field::vis_PUBLIC;
        }
    break;

  case 80:

    {
//      IDL_PRIVATE
          /* is $0 to member_i */
          tao_yyval.vival = AST_Field::vis_PRIVATE;
        }
    break;

  case 84:

    {
// export : type_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeDeclSeen);
        }
    break;

  case 85:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 86:

    {
//      | typeid_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeIdDeclSeen);
        }
    break;

  case 87:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 88:

    {
//      | typeprefix_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_TypePrefixDeclSeen);
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
//      | const_dcl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstDeclSeen);
        }
    break;

  case 91:

    {
//      ';'
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
    break;

  case 92:

    {
//      | exception
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptDeclSeen);
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
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
        }
    break;

  case 95:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 96:

    {
//      | operation
          idl_global->set_parse_state (IDL_GlobalData::PS_OpDeclSeen);
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
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
    break;

  case 99:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 100:

    {
// at_least_one_scoped_name : scoped_name scoped_names
          ACE_NEW_RETURN (tao_yyval.nlval,
                          UTL_NameList (tao_yyvsp[-1].idlist,
                                        tao_yyvsp[0].nlval),
                          1);
        }
    break;

  case 101:

    {
// scoped_names : scoped_names ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SNListCommaSeen);
        }
    break;

  case 102:

    {
//      scoped_name
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopedNameSeen);

          if (tao_yyvsp[-3].nlval == 0)
            {
              ACE_NEW_RETURN (tao_yyval.nlval,
                              UTL_NameList (tao_yyvsp[0].idlist,
                                            0),
                              1);
            }
          else
            {
              UTL_NameList *nl = 0;
              ACE_NEW_RETURN (nl,
                              UTL_NameList (tao_yyvsp[0].idlist,
                                            0),
                              1);
              tao_yyvsp[-3].nlval->nconc (nl);
              tao_yyval.nlval = tao_yyvsp[-3].nlval;
            }
        }
    break;

  case 103:

    {
/*      |  EMPTY */
          tao_yyval.nlval = 0;
        }
    break;

  case 104:

    {
// scoped_name : id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          ACE_NEW_RETURN (tao_yyval.idlist,
                          UTL_IdList (tao_yyvsp[0].idval,
                                      0),
                          1);
        }
    break;

  case 105:

    {
//      | IDL_SCOPE_DELIMITOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);
        }
    break;

  case 106:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          Identifier *id = 0;
          ACE_NEW_RETURN (id,
                          Identifier (tao_yyvsp[-2].strval),
                          1);
          ACE::strdelete (tao_yyvsp[-2].strval);
          tao_yyvsp[-2].strval = 0;
          UTL_IdList *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_IdList (tao_yyvsp[0].idval,
                                      0),
                          1);
          ACE_NEW_RETURN (tao_yyval.idlist,
                          UTL_IdList (id,
                                      sn),
                          1);
        }
    break;

  case 107:

    {
//      | scoped_name IDL_SCOPE_DELIMITOR
          idl_global->set_parse_state (IDL_GlobalData::PS_ScopeDelimSeen);

          // This cleans up all the non-global "::"s in scoped names.
          // If there is a global one, it gets put into the UTL_IdList,
          // so we clean it up in the case above.
          ACE::strdelete (tao_yyvsp[0].strval);
          tao_yyvsp[0].strval = 0;
        }
    break;

  case 108:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_SN_IDSeen);

          UTL_IdList *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_IdList (tao_yyvsp[0].idval,
                                      0),
                          1);
          tao_yyvsp[-3].idlist->nconc (sn);
          tao_yyval.idlist = tao_yyvsp[-3].idlist;
        }
    break;

  case 109:

    {
// id: IDENTIFIER
          ACE_NEW_RETURN (tao_yyval.idval,
                          Identifier (tao_yyvsp[0].strval),
                          1);
          ACE::strdelete (tao_yyvsp[0].strval);
          tao_yyvsp[0].strval = 0;
        }
    break;

  case 110:

    {
// interface_forward : interface_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n (tao_yyvsp[0].idval,
                            0);

          if (ACE_OS::strcmp (tao_yyvsp[0].idval->get_string (),
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

              tao_yyvsp[0].idval->destroy ();
              delete tao_yyvsp[0].idval;
              tao_yyvsp[0].idval = 0;

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

          tao_yyvsp[0].idval->destroy ();
          delete tao_yyvsp[0].idval;
          tao_yyvsp[0].idval = 0;
        }
    break;

  case 111:

    {
//      | IDL_LOCAL interface_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n (tao_yyvsp[0].idval,
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

          tao_yyvsp[0].idval->destroy ();
          delete tao_yyvsp[0].idval;
          tao_yyvsp[0].idval = 0;
        }
    break;

  case 112:

    {
//      | IDL_ABSTRACT interface_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n (tao_yyvsp[0].idval,
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

          tao_yyvsp[0].idval->destroy ();
          delete tao_yyvsp[0].idval;
          tao_yyvsp[0].idval = 0;
        }
    break;

  case 113:

    {
// const_dcl : IDL_CONST
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstSeen);
        }
    break;

  case 114:

    {
//      const_type
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstTypeSeen);
        }
    break;

  case 115:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstIDSeen);
        }
    break;

  case 116:

    {
//      '='
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstAssignSeen);
        }
    break;

  case 117:

    {
//      expression
          UTL_ScopedName n (tao_yyvsp[-4].idval,
                            0);
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Constant *c = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_ConstExprSeen);

          /*
           * Create a node representing a constant declaration. Store
           * it in the enclosing scope.
           */
          if (tao_yyvsp[0].exval != 0 && s != 0)
            {
              AST_Expression::AST_ExprValue *result =
                tao_yyvsp[0].exval->check_and_coerce (tao_yyvsp[-6].etval,
                                      tao_enum_constant_decl);
              tao_enum_constant_decl = 0;

              if (result == 0)
                {
                  idl_global->err ()->coercion_error (tao_yyvsp[0].exval,
                                                      tao_yyvsp[-6].etval);
                  tao_yyvsp[0].exval->destroy ();
                  delete tao_yyvsp[0].exval;
                  tao_yyvsp[0].exval = 0;
                }
              else
                {
                  c =
                    idl_global->gen ()->create_constant (
                                            tao_yyvsp[-6].etval,
                                            tao_yyvsp[0].exval,
                                            &n
                                          );
                  (void) s->fe_add_constant (c);
                  delete result;
                  result = 0;
                }

              tao_yyvsp[-4].idval->destroy ();
              delete tao_yyvsp[-4].idval;
              tao_yyvsp[-4].idval = 0;
            }
        }
    break;

  case 124:

    {
// const_type
//      : integer_type
//      | char_type
//      | octet_type
//      | boolean_type
//      | floating_pt_type
//      | fixed_type
//      | string_type_spec
          tao_yyval.etval = AST_Expression::EV_string;
        }
    break;

  case 125:

    {
//      | wstring_type_spec
          tao_yyval.etval = AST_Expression::EV_wstring;
        }
    break;

  case 126:

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
            s->lookup_by_name (tao_yyvsp[0].idlist,
                               true);

          tao_yyvsp[0].idlist->destroy ();
          delete tao_yyvsp[0].idlist;
          tao_yyvsp[0].idlist = 0;

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
                  tao_yyval.etval = AST_Expression::EV_enum;
                }
              else if (d->node_type () == AST_Decl::NT_pre_defined)
                {
                  c = AST_PredefinedType::narrow_from_decl (d);

                  if (c != 0)
                    {
                      tao_yyval.etval = idl_global->PredefinedTypeToExprType (c->pt ());
                    }
                  else
                    {
                      tao_yyval.etval = AST_Expression::EV_enum;
                    }
                }
              else if (d->node_type () == AST_Decl::NT_string)
                {
                  tao_yyval.etval = AST_Expression::EV_string;
                }
              else if (d->node_type () == AST_Decl::NT_wstring)
                {
                  tao_yyval.etval = AST_Expression::EV_wstring;
                }
              else
                {
                  tao_yyval.etval = AST_Expression::EV_enum;
                }
            }
          else
            {
              tao_yyval.etval = AST_Expression::EV_enum;
            }
        }
    break;

  case 130:

    {
// or_expr : xor_expr
//      | or_expr '|' xor_expr
          tao_yyval.exval =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_or,
                                    tao_yyvsp[-2].exval,
                                    tao_yyvsp[0].exval
                                  );
        }
    break;

  case 132:

    {
// xor_expr : and_expr
//      | xor_expr '^' and_expr
          tao_yyval.exval =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_xor,
                                    tao_yyvsp[-2].exval,
                                    tao_yyvsp[0].exval
                                  );
        }
    break;

  case 134:

    {
// and_expr : shift_expr | and_expr '&' shift_expr
          tao_yyval.exval =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_and,
                                    tao_yyvsp[-2].exval,
                                    tao_yyvsp[0].exval
                                  );
        }
    break;

  case 136:

    {
// shift_expr : add_expr | shift_expr IDL_LEFT_SHIFT add_expr
          tao_yyval.exval =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_left,
                                    tao_yyvsp[-2].exval,
                                    tao_yyvsp[0].exval
                                  );
        }
    break;

  case 137:

    {
//      | shift_expr IDL_RIGHT_SHIFT add_expr
          tao_yyval.exval =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_right,
                                    tao_yyvsp[-2].exval,
                                    tao_yyvsp[0].exval
                                  );
        }
    break;

  case 139:

    {
// add_expr : mult_expr | add_expr '+' mult_expr
          tao_yyval.exval =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_add,
                                    tao_yyvsp[-2].exval,
                                    tao_yyvsp[0].exval
                                  );
        }
    break;

  case 140:

    {
//      | add_expr '-' mult_expr
          tao_yyval.exval =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_minus,
                                    tao_yyvsp[-2].exval,
                                    tao_yyvsp[0].exval
                                  );
        }
    break;

  case 142:

    {
// mult_expr : unary_expr | mult_expr '*' unary_expr
          tao_yyval.exval =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mul,
                                    tao_yyvsp[-2].exval,
                                    tao_yyvsp[0].exval
                                  );
        }
    break;

  case 143:

    {
//      | mult_expr '/' unary_expr
          tao_yyval.exval =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_div,
                                    tao_yyvsp[-2].exval,
                                    tao_yyvsp[0].exval
                                  );
        }
    break;

  case 144:

    {
//      | mult_expr '%' unary_expr
          tao_yyval.exval =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_mod,
                                    tao_yyvsp[-2].exval,
                                    tao_yyvsp[0].exval
                                  );
        }
    break;

  case 146:

    {
// unary_expr : primary_expr | '+' primary_expr
          tao_yyval.exval =
            idl_global->gen ()->create_expr (
                                    AST_Expression::EC_u_plus,
                                    tao_yyvsp[0].exval,
                                    0
                                  );
        }
    break;

  case 147:

    {
//      | '-' primary_expr
          tao_yyval.exval =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_u_minus,
                                   tao_yyvsp[0].exval,
                                   0
                                 );
        }
    break;

  case 148:

    {
//      | '~' primary_expr
          tao_yyval.exval =
            idl_global->gen()->create_expr (
                                   AST_Expression::EC_bit_neg,
                                   tao_yyvsp[0].exval,
                                   0
                                 );
        }
    break;

  case 149:

    {
// primary_expr : scoped_name
          /*
           * An expression which is a scoped name is not resolved now,
           * but only when it is evaluated (such as when it is assigned
           * as a constant value).
           */
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name (tao_yyvsp[0].idlist,
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
              tao_yyval.exval =
                idl_global->gen ()->create_expr (c->constant_value (),
                                                 c->et ());

              tao_yyvsp[0].idlist->destroy ();
              delete tao_yyvsp[0].idlist;
              tao_yyvsp[0].idlist = 0;
            }
          else
            {
              tao_yyval.exval = idl_global->gen ()->create_expr (tao_yyvsp[0].idlist);
            }
        }
    break;

  case 151:

    {
//      | literal
//      | '(' const_expr ')'
          tao_yyval.exval = tao_yyvsp[-1].exval;
        }
    break;

  case 152:

    {
// literal : IDL_INTEGER_LITERAL
          tao_yyval.exval = idl_global->gen ()->create_expr (tao_yyvsp[0].ival);
        }
    break;

  case 153:

    {
//      | IDL_UINTEGER_LITERAL
          tao_yyval.exval =
            idl_global->gen ()->create_expr (tao_yyvsp[0].uival,
                                             AST_Expression::EV_ulonglong);
        }
    break;

  case 154:

    {
//      | IDL_STRING_LITERAL
          tao_yyval.exval = idl_global->gen ()->create_expr (tao_yyvsp[0].sval);
          tao_yyvsp[0].sval->destroy ();
          delete tao_yyvsp[0].sval;
          tao_yyvsp[0].sval = 0;
        }
    break;

  case 155:

    {
//      | IDL_WSTRING_LITERAL
          tao_yyval.exval = idl_global->gen ()->create_expr (tao_yyvsp[0].wsval);
        }
    break;

  case 156:

    {
//      | IDL_CHARACTER_LITERAL
          tao_yyval.exval = idl_global->gen ()->create_expr (tao_yyvsp[0].cval);
        }
    break;

  case 157:

    {
//      | IDL_WCHAR_LITERAL
          ACE_OutputCDR::from_wchar wc (tao_yyvsp[0].wcval);
          tao_yyval.exval = idl_global->gen ()->create_expr (wc);
        }
    break;

  case 158:

    {
//      | IDL_FLOATING_PT_LITERAL
          tao_yyval.exval = idl_global->gen ()->create_expr (tao_yyvsp[0].dval);
        }
    break;

  case 159:

    {
//      | IDL_TRUETOK
          tao_yyval.exval = idl_global->gen ()->create_expr (true);
        }
    break;

  case 160:

    {
//      | IDL_FALSETOK
          tao_yyval.exval = idl_global->gen ()->create_expr (false);
        }
    break;

  case 161:

    {
// positive_int_expr : const_expr
          int good_expression = 1;
          tao_yyvsp[0].exval->evaluate (AST_Expression::EK_positive_int);
          AST_Expression::AST_ExprValue *ev = tao_yyvsp[0].exval->ev ();

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
              tao_yyval.exval = tao_yyvsp[0].exval;
            }
          else
            {
              idl_global->err ()->syntax_error (idl_global->parse_state ());
            }
        }
    break;

  case 162:

    {
// type_dcl : IDL_TYPEDEF
          idl_global->set_parse_state (IDL_GlobalData::PS_TypedefSeen);
        }
    break;

  case 163:

    {
//      type_declarator
          tao_yyval.ival = 0;
        }
    break;

  case 164:

    {
//      | struct_type
          tao_yyval.ival = 0;
        }
    break;

  case 165:

    {
//      | union_type
          tao_yyval.ival = 0;
        }
    break;

  case 166:

    {
//      | enum_type
          tao_yyval.ival = 0;
        }
    break;

  case 167:

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
                                        tao_yyvsp[0].deval->name ()
                                      );
              /*
               * Add it to its defining scope
               */
              (void) s->fe_add_native (node);
            }

          tao_yyvsp[0].deval->destroy ();
          delete tao_yyvsp[0].deval;
          tao_yyvsp[0].deval = 0;
        }
    break;

  case 168:

    {
//      | constructed_forward_type_spec
          tao_yyval.ival = 0;
        }
    break;

  case 169:

    {
// type_declarator : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_TypeSpecSeen);
        }
    break;

  case 170:

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
          if (s != 0 && tao_yyvsp[-2].dcval != 0 && tao_yyvsp[0].dlval != 0)
            {
              for (UTL_DecllistActiveIterator l (tao_yyvsp[0].dlval);
                   !l.is_done ();
                   l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type * tp = d->compose (tao_yyvsp[-2].dcval);

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
              tao_yyvsp[0].dlval->destroy ();
              delete tao_yyvsp[0].dlval;
              tao_yyvsp[0].dlval = 0;
            }
        }
    break;

  case 173:

    {
// simple_type_spec : base_type_spec
          tao_yyval.dcval =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 tao_yyvsp[0].etval
                                               );
        }
    break;

  case 175:

    {
//      | template_type_spec
//      | scoped_name
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = 0;

          if (s != 0)
            {
              d = s->lookup_by_name (tao_yyvsp[0].idlist,
                                     true);
            }

          if (d == 0)
            {
              idl_global->err ()->lookup_error (tao_yyvsp[0].idlist);
            }
          else
            {
              d->last_referenced_as (tao_yyvsp[0].idlist);
            }


          tao_yyval.dcval = d;
        }
    break;

  case 192:

    {
// at_least_one_declarator : declarator declarators
          ACE_NEW_RETURN (tao_yyval.dlval,
                          UTL_DeclList (tao_yyvsp[-1].deval,
                                        tao_yyvsp[0].dlval),
                          1);
        }
    break;

  case 193:

    {
// declarators : declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
    break;

  case 194:

    {
//      declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsDeclSeen);

          if (tao_yyvsp[-3].dlval == 0)
            {
              ACE_NEW_RETURN (tao_yyval.dlval,
                              UTL_DeclList (tao_yyvsp[0].deval,
                                            0),
                              1);
            }
          else
            {
              UTL_DeclList *dl = 0;
              ACE_NEW_RETURN (dl,
                              UTL_DeclList (tao_yyvsp[0].deval,
                                            0),
                              1);
              tao_yyvsp[-3].dlval->nconc (dl);
              tao_yyval.dlval = tao_yyvsp[-3].dlval;
            }
        }
    break;

  case 195:

    {
/*      |  EMPTY */
          tao_yyval.dlval = 0;
        }
    break;

  case 198:

    {
// at_least_one_simple_declarator : simple_declarator simple_declarators
          ACE_NEW_RETURN (tao_yyval.dlval,
                          UTL_DeclList (tao_yyvsp[-1].deval,
                                        tao_yyvsp[0].dlval),
                          1);
        }
    break;

  case 199:

    {
// simple_declarators : simple_declarators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsCommaSeen);
        }
    break;

  case 200:

    {
//      simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_DeclsDeclSeen);

          if (tao_yyvsp[-3].dlval == 0)
            {
              ACE_NEW_RETURN (tao_yyval.dlval,
                              UTL_DeclList (tao_yyvsp[0].deval,
                                            0),
                              1);
            }
          else
            {
              UTL_DeclList *dl = 0;
              ACE_NEW_RETURN (dl,
                              UTL_DeclList (tao_yyvsp[0].deval,
                                            0),
                              1);
              tao_yyvsp[-3].dlval->nconc (dl);
              tao_yyval.dlval = tao_yyvsp[-3].dlval;
            }
        }
    break;

  case 201:

    {
/*      |  EMPTY */
          tao_yyval.dlval = 0;
        }
    break;

  case 202:

    {
// simple_declarator : id
          UTL_ScopedName *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_ScopedName (tao_yyvsp[0].idval,
                                          0),
                          1);
          ACE_NEW_RETURN (tao_yyval.deval,
                          FE_Declarator (sn,
                                         FE_Declarator::FD_simple,
                                         0),
                          1);
        }
    break;

  case 203:

    {
// complex_declarator : array_declarator
          UTL_ScopedName *sn = 0;
          ACE_NEW_RETURN (sn,
                          UTL_ScopedName (
                              tao_yyvsp[0].dcval->local_name ()->copy (),
                              0
                            ),
                          1);
          ACE_NEW_RETURN (tao_yyval.deval,
                          FE_Declarator (sn,
                                         FE_Declarator::FD_complex,
                                         tao_yyvsp[0].dcval),
                          1);
        }
    break;

  case 206:

    {
// signed_int : IDL_LONG
          tao_yyval.etval = AST_Expression::EV_long;
        }
    break;

  case 207:

    {
//      | IDL_LONG IDL_LONG
          tao_yyval.etval = AST_Expression::EV_longlong;
        }
    break;

  case 208:

    {
//      | IDL_SHORT
          tao_yyval.etval = AST_Expression::EV_short;
        }
    break;

  case 209:

    {
// unsigned_int : IDL_UNSIGNED IDL_LONG
          tao_yyval.etval = AST_Expression::EV_ulong;
        }
    break;

  case 210:

    {
//      | IDL_UNSIGNED IDL_LONG IDL_LONG
          tao_yyval.etval = AST_Expression::EV_ulonglong;
        }
    break;

  case 211:

    {
//      | IDL_UNSIGNED IDL_SHORT
          tao_yyval.etval = AST_Expression::EV_ushort;
        }
    break;

  case 212:

    {
// floating_pt_type : IDL_DOUBLE
          tao_yyval.etval = AST_Expression::EV_double;
        }
    break;

  case 213:

    {
//      | IDL_FLOAT
          tao_yyval.etval = AST_Expression::EV_float;
        }
    break;

  case 214:

    {
//      | IDL_LONG IDL_DOUBLE
          tao_yyval.etval = AST_Expression::EV_longdouble;
        }
    break;

  case 215:

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

  case 216:

    {
// char_type : IDL_CHAR
          tao_yyval.etval = AST_Expression::EV_char;
        }
    break;

  case 217:

    {
//      | IDL_WCHAR
          tao_yyval.etval = AST_Expression::EV_wchar;
        }
    break;

  case 218:

    {
// octet_type : IDL_OCTET
          tao_yyval.etval = AST_Expression::EV_octet;
        }
    break;

  case 219:

    {
// boolean_type : IDL_BOOLEAN
          tao_yyval.etval = AST_Expression::EV_bool;
        }
    break;

  case 220:

    {
// any_type : IDL_ANY
          tao_yyval.etval = AST_Expression::EV_any;
        }
    break;

  case 221:

    {
// object_type : IDL_OBJECT
          tao_yyval.etval = AST_Expression::EV_object;
        }
    break;

  case 222:

    {
// struct_decl : IDL_STRUCT
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSeen);
        }
    break;

  case 223:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_StructIDSeen);
          tao_yyval.idval = tao_yyvsp[0].idval;
        }
    break;

  case 224:

    {
// struct_type : struct_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n (tao_yyvsp[0].idval,
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

          tao_yyvsp[0].idval->destroy ();
          delete tao_yyvsp[0].idval;
          tao_yyvsp[0].idval = 0;
        }
    break;

  case 225:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_StructSqSeen);
        }
    break;

  case 226:

    {
//      at_least_one_member
          idl_global->set_parse_state (IDL_GlobalData::PS_StructBodySeen);
        }
    break;

  case 227:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_StructQsSeen);

          /*
           * Done with this struct. Pop its scope off the scopes stack.
           */
          tao_yyval.dcval = AST_Structure::narrow_from_scope (
                   idl_global->scopes ().top_non_null ()
                 );
          idl_global->scopes ().pop ();
        }
    break;

  case 231:

    {
// member  :
          /* is $0 to member_i */
          tao_yyval.vival = AST_Field::vis_NA;
        }
    break;

  case 233:

    {
// member_i : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberTypeSeen);
        }
    break;

  case 234:

    {
//      at_least_one_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsSeen);
        }
    break;

  case 235:

    {
//      ';'
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          FE_Declarator *d = 0;
          AST_Field *f = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_MemberDeclsCompleted);

          /*
           * Check for illegal recursive use of type.
           */
          if (tao_yyvsp[-4].dcval != 0
              && AST_illegal_recursive_type (tao_yyvsp[-4].dcval))
            {
              idl_global->err ()->error1 (UTL_Error::EIDL_RECURSIVE_TYPE,
                                          tao_yyvsp[-4].dcval);
            }
          /*
           * Create a node representing a struct or exception member
           * Add it to the enclosing scope.
           */
          else if (s != 0
                   && tao_yyvsp[-4].dcval != 0
                   && tao_yyvsp[-2].dlval != 0)
            {
              for (UTL_DecllistActiveIterator l (tao_yyvsp[-2].dlval);
                   !l.is_done ();
                   l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type *tp = d->compose (tao_yyvsp[-4].dcval);

                  if (tp == 0)
                    {
                      continue;
                    }

                  /* $0 denotes Visibility, must be on yacc reduction stack. */
                  f =
                    idl_global->gen ()->create_field (
                                            tp,
                                            d->name (),
                                            tao_yyvsp[-5].vival
                                          );
                  (void) s->fe_add_field (f);
                }
            }

          tao_yyvsp[-2].dlval->destroy ();
          delete tao_yyvsp[-2].dlval;
          tao_yyvsp[-2].dlval = 0;
        }
    break;

  case 236:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state ());
        }
    break;

  case 237:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 238:

    {
// union_decl : IDL_UNION
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSeen);
        }
    break;

  case 239:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionIDSeen);
          tao_yyval.idval = tao_yyvsp[0].idval;
        }
    break;

  case 240:

    {
// union_type : union_decl IDL_SWITCH
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchSeen);
        }
    break;

  case 241:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchOpenParSeen);
        }
    break;

  case 242:

    {
//      switch_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchTypeSeen);
        }
    break;

  case 243:

    {
//      ')'
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n (tao_yyvsp[-7].idval,
                            0);
          AST_Union *u = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_SwitchCloseParSeen);

          /*
           * Create a node representing a union. Add it to its enclosing
           * scope.
           */
          if (tao_yyvsp[-2].dcval != 0
              && s != 0)
            {
              AST_ConcreteType *tp =
                AST_ConcreteType::narrow_from_decl (tao_yyvsp[-2].dcval);

              if (tp == 0)
                {
                  idl_global->err ()->not_a_type (tao_yyvsp[-2].dcval);
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

          tao_yyvsp[-7].idval->destroy ();
          delete tao_yyvsp[-7].idval;
          tao_yyvsp[-7].idval = 0;
        }
    break;

  case 244:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionSqSeen);
        }
    break;

  case 245:

    {
//      at_least_one_case_branch
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionBodySeen);
        }
    break;

  case 246:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionQsSeen);

          /*
           * Done with this union. Pop its scope from the scopes stack.
           */
          tao_yyval.dcval = AST_Union::narrow_from_scope (
                   idl_global->scopes ().top_non_null ()
                 );

          if (tao_yyval.dcval != 0)
            {
              idl_global->scopes ().pop ();
            }
        }
    break;

  case 247:

    {
// switch_type_spec : integer_type
          tao_yyval.dcval =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 tao_yyvsp[0].etval
                                               );
        }
    break;

  case 248:

    {
//      | char_type
          /* wchars are not allowed. */
          if (tao_yyvsp[0].etval == AST_Expression::EV_wchar)
            {
              idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
            }

          tao_yyval.dcval =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 tao_yyvsp[0].etval
                                               );
        }
    break;

  case 249:

    {
//      | octet_type
          /* octets are not allowed. */
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
          tao_yyval.dcval =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 tao_yyvsp[0].etval
                                               );
        }
    break;

  case 250:

    {
//      | boolean_type
          tao_yyval.dcval =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 tao_yyvsp[0].etval
                                               );
        }
    break;

  case 252:

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
          d = s->lookup_by_name (tao_yyvsp[0].idlist,
                                 true);

          if (s != 0 && d != 0)
            {
              while (!found)
                {
                  switch (d->node_type ())
                  {
                    case AST_Decl::NT_enum:
                      tao_yyval.dcval = d;
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
                              tao_yyval.dcval = p;
                              found = true;
                              break;
                            case AST_PredefinedType::PT_wchar:
                            case AST_PredefinedType::PT_octet:
                              /* octets and wchars are not allowed */
                              idl_global->err ()->error0 (
                                  UTL_Error::EIDL_DISC_TYPE
                                );
                              tao_yyval.dcval = 0;
                              found = true;
                              break;
                            default:
                              tao_yyval.dcval = 0;
                              found = true;
                              break;
                          }
                        }
                      else
                        {
                          tao_yyval.dcval = 0;
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
                      tao_yyval.dcval = 0;
                      found = true;
                      break;
                  }
                }
            }
          else
            {
              tao_yyval.dcval = 0;
            }

          if (tao_yyval.dcval == 0)
            {
              idl_global->err ()->lookup_error (tao_yyvsp[0].idlist);

              tao_yyvsp[0].idlist->destroy ();
              delete tao_yyvsp[0].idlist;
              tao_yyvsp[0].idlist = 0;

              /* If we don't return here, we'll crash later.*/
              return 1;
            }

          tao_yyvsp[0].idlist->destroy ();
          delete tao_yyvsp[0].idlist;
          tao_yyvsp[0].idlist = 0;
        }
    break;

  case 256:

    {
// case_branch : at_least_one_case_label
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionLabelSeen);
        }
    break;

  case 257:

    {
//      element_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemSeen);
        }
    break;

  case 258:

    {
//      ';'
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_UnionBranch *b = 0;
          AST_Field *f = tao_yyvsp[-2].ffval;
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemCompleted);

          /*
           * Create several nodes representing branches of a union.
           * Add them to the enclosing scope (the union scope)
           */
          if (s != 0
              &&  tao_yyvsp[-4].llval != 0
              && tao_yyvsp[-2].ffval != 0)
            {
              b =
                idl_global->gen ()->create_union_branch (
                                        tao_yyvsp[-4].llval,
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

  case 259:

    {
//      | error
          idl_global->err()->syntax_error (idl_global->parse_state());
        }
    break;

  case 260:

    {
//      ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
    break;

  case 261:

    {
// at_least_one_case_label : case_label case_labels
          ACE_NEW_RETURN (tao_yyval.llval,
                          UTL_LabelList (tao_yyvsp[-1].ulval,
                                         tao_yyvsp[0].llval),
                          1);
        }
    break;

  case 262:

    {
// case_labels : case_labels case_label
          if (tao_yyvsp[-1].llval == 0)
            {
              ACE_NEW_RETURN (tao_yyval.llval,
                              UTL_LabelList (tao_yyvsp[0].ulval,
                                             0),
                              1);
            }
          else
            {
              UTL_LabelList *ll = 0;
              ACE_NEW_RETURN (ll,
                              UTL_LabelList (tao_yyvsp[0].ulval,
                                             0),
                              1);
              tao_yyvsp[-1].llval->nconc (ll);
              tao_yyval.llval = tao_yyvsp[-1].llval;
            }
        }
    break;

  case 263:

    {
/*      |  EMPTY */
          tao_yyval.llval = 0;
        }
    break;

  case 264:

    {
// case_label : IDL_DEFAULT
          idl_global->set_parse_state (IDL_GlobalData::PS_DefaultSeen);
        }
    break;

  case 265:

    {
//      ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          tao_yyval.ulval = idl_global->gen ()->create_union_label (
                                       AST_UnionLabel::UL_default,
                                       0
                                     );
        }
    break;

  case 266:

    {
//      | IDL_CASE
          idl_global->set_parse_state (IDL_GlobalData::PS_CaseSeen);
        }
    break;

  case 267:

    {
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelExprSeen);
        }
    break;

  case 268:

    {
//      const_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_LabelColonSeen);

          tao_yyval.ulval = idl_global->gen()->create_union_label (
                                      AST_UnionLabel::UL_label,
                                      tao_yyvsp[-2].exval
                                    );
        }
    break;

  case 269:

    {
// element_spec : type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemTypeSeen);
        }
    break;

  case 270:

    {
//      declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_UnionElemDeclSeen);

          /*
           * Check for illegal recursive use of type
           */
          if (tao_yyvsp[-2].dcval != 0
              && AST_illegal_recursive_type (tao_yyvsp[-2].dcval))
            {
              idl_global->err()->error1 (UTL_Error::EIDL_RECURSIVE_TYPE,
                                         tao_yyvsp[-2].dcval);

              tao_yyval.ffval = 0;
            }
          /*
           * Create a field in a union branch
           */
          else if (tao_yyvsp[-2].dcval == 0
                   || tao_yyvsp[0].deval == 0)
            {
              tao_yyval.ffval = 0;
            }
          else
            {
              AST_Type *tp = tao_yyvsp[0].deval->compose (tao_yyvsp[-2].dcval);

              if (tp == 0)
                {
                  tao_yyval.ffval = 0;
                }
              else
                {
                  tao_yyval.ffval = idl_global->gen ()->create_field (
                                               tp,
                                               tao_yyvsp[0].deval->name ()
                                             );
                }

              tao_yyvsp[0].deval->destroy ();
              delete tao_yyvsp[0].deval;
              tao_yyvsp[0].deval = 0;
            }
        }
    break;

  case 271:

    {
// struct_forward_type : struct_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n (tao_yyvsp[0].idval,
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

          tao_yyvsp[0].idval->destroy ();
          delete tao_yyvsp[0].idval;
          tao_yyvsp[0].idval = 0;
        }
    break;

  case 272:

    {
// union_forward_type : union_decl
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n (tao_yyvsp[0].idval,
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

          tao_yyvsp[0].idval->destroy ();
          delete tao_yyvsp[0].idval;
          tao_yyvsp[0].idval = 0;
        }
    break;

  case 273:

    {
// enum_type : IDL_ENUM
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSeen);
        }
    break;

  case 274:

    {
//      id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n (tao_yyvsp[0].idval,
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

          tao_yyvsp[0].idval->destroy ();
          delete tao_yyvsp[0].idval;
          tao_yyvsp[0].idval = 0;
        }
    break;

  case 275:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumSqSeen);
        }
    break;

  case 276:

    {
//      at_least_one_enumerator
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumBodySeen);
        }
    break;

  case 277:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumQsSeen);

          /*
           * Done with this enum. Pop its scope from the scopes stack.
           */
          if (idl_global->scopes ().top () == 0)
            {
              tao_yyval.dcval = 0;
            }
          else
            {
              tao_yyval.dcval = AST_Enum::narrow_from_scope (
                       idl_global->scopes ().top_non_null ()
                     );
              idl_global->scopes ().pop ();
            }
        }
    break;

  case 279:

    {
// enumerators : enumerators ','
          idl_global->set_parse_state (IDL_GlobalData::PS_EnumCommaSeen);
        }
    break;

  case 282:

    {
// enumerator : IDENTIFIER
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          Identifier id (tao_yyvsp[0].strval);
          ACE::strdelete (tao_yyvsp[0].strval);
          tao_yyvsp[0].strval = 0;
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

  case 283:

    {
// sequence_type_spec : seq_head ','
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceCommaSeen);
        }
    break;

  case 284:

    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceExprSeen);
        }
    break;

  case 285:

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

          if (tao_yyvsp[-2].exval != 0)
            {
              ev = tao_yyvsp[-2].exval->coerce (AST_Expression::EV_ulong);
            }

          if (0 == tao_yyvsp[-2].exval || 0 == ev)
            {
              idl_global->err ()->coercion_error (tao_yyvsp[-2].exval,
                                                  AST_Expression::EV_ulong);
              tao_yyval.dcval = 0;
            }
          else if (0 == tao_yyvsp[-5].dcval)
            {
              tao_yyval.dcval = 0;
            }
          else
            {
              AST_Type *tp = AST_Type::narrow_from_decl (tao_yyvsp[-5].dcval);

              if (0 == tp)
                {
                  ; // Error will be caught in FE_Declarator.
                }
              else
                {
                  Identifier id ("sequence");
                  UTL_ScopedName sn (&id,
                                     0);

                  tao_yyval.dcval =
                    idl_global->gen ()->create_sequence (
                                            tao_yyvsp[-2].exval,
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

  case 286:

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
          if (tao_yyvsp[-1].dcval == 0)
            {
              tao_yyval.dcval = 0;
            }
          else
            {
              AST_Type *tp = AST_Type::narrow_from_decl (tao_yyvsp[-1].dcval);

              if (tp == 0)
                {
                  ; // Error will be caught in FE_Declarator.
                }
              else
                {
                  Identifier id ("sequence");
                  UTL_ScopedName sn (&id, 0);
                  ACE_CDR::ULong bound = 0UL;

                  tao_yyval.dcval =
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

  case 287:

    {
// seq_head : IDL_SEQUENCE
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSeen);

          /*
           * Push a sequence marker on scopes stack.
           */
          idl_global->scopes ().push (0);
        }
    break;

  case 288:

    {
//      '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceSqSeen);
        }
    break;

  case 289:

    {
//      simple_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SequenceTypeSeen);
          tao_yyval.dcval = tao_yyvsp[0].dcval;
        }
    break;

  case 290:

    {
// string_type_spec : string_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
    break;

  case 291:

    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
    break;

  case 292:

    {
//      '>'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringQsSeen);

          /*
           * Create a node representing a string.
           */
          AST_Expression::AST_ExprValue *ev = 0;

          if (tao_yyvsp[-2].exval != 0)
            {
              ev = tao_yyvsp[-2].exval->coerce (AST_Expression::EV_ulong);
            }

          if (0 == tao_yyvsp[-2].exval || 0 == ev)
            {
              idl_global->err ()->coercion_error (tao_yyvsp[-2].exval,
                                                  AST_Expression::EV_ulong);
              tao_yyval.dcval = 0;
            }
          else
            {
              tao_yyval.dcval = idl_global->gen ()->create_string (tao_yyvsp[-2].exval);
              /*
               * Add this AST_String to the types defined in the global scope.
               */
              (void) idl_global->root ()->fe_add_string (
                                              AST_String::narrow_from_decl (
                                                  tao_yyval.dcval
                                                )
                                            );
            }

          delete ev;
          ev = 0;
        }
    break;

  case 293:

    {
//      | string_head
          idl_global->set_parse_state (IDL_GlobalData::PS_StringCompleted);
          /*
           * Create a node representing a string.
           */
          ACE_CDR::ULong bound = 0UL;
          
          tao_yyval.dcval =
            idl_global->gen ()->create_string (
                idl_global->gen ()->create_expr (bound,
                                                 AST_Expression::EV_ulong)
              );
          /*
           * Add this AST_String to the types defined in the global scope.
           */
          (void) idl_global->root ()->fe_add_string (
                                          AST_String::narrow_from_decl (
                                              tao_yyval.dcval
                                            )
                                        );
        }
    break;

  case 294:

    {
// string_head : IDL_STRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
    break;

  case 295:

    {
// wstring_type_spec : wstring_head '<'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSqSeen);
        }
    break;

  case 296:

    {
//      positive_int_expr
           idl_global->set_parse_state (IDL_GlobalData::PS_StringExprSeen);
        }
    break;

  case 297:

    {
//      '>'
          idl_global->set_parse_state (IDL_GlobalData::PS_StringQsSeen);

          /*
           * Create a node representing a string.
           */
          if (tao_yyvsp[-2].exval == 0
              || tao_yyvsp[-2].exval->coerce (AST_Expression::EV_ulong) == 0)
            {
              idl_global->err ()->coercion_error (tao_yyvsp[-2].exval,
                                                  AST_Expression::EV_ulong);
              tao_yyval.dcval = 0;
            }
          else
            {
              tao_yyval.dcval = idl_global->gen ()->create_wstring (tao_yyvsp[-2].exval);
              /*
               * Add this AST_String to the types defined in the global scope.
               */
              (void) idl_global->root ()->fe_add_string (
                                              AST_String::narrow_from_decl (tao_yyval.dcval)
                                            );
            }
        }
    break;

  case 298:

    {
//      | wstring_head
          idl_global->set_parse_state (IDL_GlobalData::PS_StringCompleted);

          /*
           * Create a node representing a wstring.
           */
          ACE_CDR::ULong bound = 0UL;
          
          tao_yyval.dcval =
            idl_global->gen ()->create_wstring (
                idl_global->gen ()->create_expr (bound,
                                                 AST_Expression::EV_ulong)
              );
          /*
           * Add this AST_String to the types defined in the global scope.
           */
          (void) idl_global->root ()->fe_add_string (
                                          AST_String::narrow_from_decl (tao_yyval.dcval)
                                        );
        }
    break;

  case 299:

    {
// wstring_head : IDL_WSTRING
          idl_global->set_parse_state (IDL_GlobalData::PS_StringSeen);
        }
    break;

  case 300:

    {
// array_declarator : id
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayIDSeen);
        }
    break;

  case 301:

    {
//      at_least_one_array_dim
          idl_global->set_parse_state (IDL_GlobalData::PS_ArrayCompleted);

          /*
           * Create a node representing an array.
           */
          if (tao_yyvsp[0].elval != 0)
            {
              UTL_ScopedName sn (tao_yyvsp[-2].idval,
                                 0);
              tao_yyval.dcval =
                idl_global->gen ()->create_array (
                                        &sn,
                                        tao_yyvsp[0].elval->length (),
                                        tao_yyvsp[0].elval,
                                        0,
                                        0
                                      );

              tao_yyvsp[0].elval->destroy ();
              delete tao_yyvsp[0].elval;
              tao_yyvsp[0].elval = 0;

              sn.destroy ();
            }
        }
    break;

  case 302:

    {
// at_least_one_array_dim : array_dim array_dims
          ACE_NEW_RETURN (tao_yyval.elval,
                          UTL_ExprList (tao_yyvsp[-1].exval,
                                        tao_yyvsp[0].elval),
                          1);
        }
    break;

  case 303:

    {
// array_dims : array_dims array_dim
          if (tao_yyvsp[-1].elval == 0)
            {
              ACE_NEW_RETURN (tao_yyval.elval,
                              UTL_ExprList (tao_yyvsp[0].exval,
                                            0),
                              1);
            }
          else
            {
              UTL_ExprList *el = 0;
              ACE_NEW_RETURN (el,
                              UTL_ExprList (tao_yyvsp[0].exval,
                                            0),
                              1);
              tao_yyvsp[-1].elval->nconc (el);
              tao_yyval.elval = tao_yyvsp[-1].elval;
            }
        }
    break;

  case 304:

    {
/*      |  EMPTY */
          tao_yyval.elval = 0;
        }
    break;

  case 305:

    {
// array_dim : '['
          idl_global->set_parse_state (IDL_GlobalData::PS_DimSqSeen);
        }
    break;

  case 306:

    {
//      positive_int_expr
          idl_global->set_parse_state (IDL_GlobalData::PS_DimExprSeen);
        }
    break;

  case 307:

    {
//      ']'
          idl_global->set_parse_state (IDL_GlobalData::PS_DimQsSeen);

          /*
           * Array dimensions are expressions which must be coerced to
           * positive integers.
           */
          AST_Expression::AST_ExprValue *ev = 0;

          if (tao_yyvsp[-2].exval != 0)
            {
              ev = tao_yyvsp[-2].exval->coerce (AST_Expression::EV_ulong);
            }

          if (0 == tao_yyvsp[-2].exval || 0 == ev)
            {
              idl_global->err ()->coercion_error (tao_yyvsp[-2].exval,
                                                  AST_Expression::EV_ulong);
              tao_yyval.exval = 0;
            }
          else
            {
              tao_yyval.exval = tao_yyvsp[-2].exval;
            }

          delete ev;
          ev = 0;
        }
    break;

  case 310:

    {
// attribute_readonly : IDL_READONLY
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrROSeen);
        }
    break;

  case 311:

    {
//      IDL_ATTRIBUTE
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrSeen);
        }
    break;

  case 312:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrTypeSeen);
        }
    break;

  case 313:

    {
//      at_least_one_simple_declarator
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclsSeen);
        }
    break;

  case 314:

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
              && tao_yyvsp[-4].dcval != 0
              && tao_yyvsp[-2].dlval != 0)
            {
              for (UTL_DecllistActiveIterator l (tao_yyvsp[-2].dlval);
                   !l.is_done ();
                   l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type *tp = d->compose (tao_yyvsp[-4].dcval);

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

                  if (tao_yyvsp[0].nlval != 0)
                    {
                      (void) a->fe_add_get_exceptions (tao_yyvsp[0].nlval);

                      tao_yyvsp[0].nlval->destroy ();
                      delete tao_yyvsp[0].nlval;
                      tao_yyvsp[0].nlval = 0;
                    }

                  (void) s->fe_add_attribute (a);
                }
            }

          tao_yyvsp[-2].dlval->destroy ();
          delete tao_yyvsp[-2].dlval;
          tao_yyvsp[-2].dlval = 0;
        }
    break;

  case 315:

    {
// attribute_readonly : IDL_ATTRIBUTE
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
//      opt_getraises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseCompleted);
        }
    break;

  case 319:

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
              && tao_yyvsp[-6].dcval != 0
              && tao_yyvsp[-4].dlval != 0)
            {
              for (UTL_DecllistActiveIterator l (tao_yyvsp[-4].dlval);
                   !l.is_done ();
                   l.next ())
                {
                  d = l.item ();

                  if (d == 0)
                    {
                      continue;
                    }

                  AST_Type *tp = d->compose (tao_yyvsp[-6].dcval);

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

                  if (tao_yyvsp[-2].nlval != 0)
                    {
                      (void) a->fe_add_get_exceptions (tao_yyvsp[-2].nlval);

                      tao_yyvsp[-2].nlval->destroy ();
                      delete tao_yyvsp[-2].nlval;
                      tao_yyvsp[-2].nlval = 0;
                    }

                  if (tao_yyvsp[0].nlval != 0)
                    {
                      (void) a->fe_add_set_exceptions (tao_yyvsp[0].nlval);

                      tao_yyvsp[0].nlval->destroy ();
                      delete tao_yyvsp[0].nlval;
                      tao_yyvsp[0].nlval = 0;
                    }

                  (void) s->fe_add_attribute (a);
                }
            }

          tao_yyvsp[-4].dlval->destroy ();
          delete tao_yyvsp[-4].dlval;
          tao_yyvsp[-4].dlval = 0;
        }
    break;

  case 320:

    {
// exception : IDL_EXCEPTION
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSeen);
        }
    break;

  case 321:

    {
//      id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n (tao_yyvsp[0].idval,
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

          tao_yyvsp[0].idval->destroy ();
          delete tao_yyvsp[0].idval;
          tao_yyvsp[0].idval = 0;
        }
    break;

  case 322:

    {
//       '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptSqSeen);
        }
    break;

  case 323:

    {
//      members
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptBodySeen);
        }
    break;

  case 324:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ExceptQsSeen);
          /*
           * Done with this exception. Pop its scope from the scope stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 325:

    {
// operation : opt_op_attribute op_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
    break;

  case 326:

    {
//      IDENTIFIER
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          Identifier id (tao_yyvsp[0].strval);
          ACE::strdelete (tao_yyvsp[0].strval);
          tao_yyvsp[0].strval = 0;

          UTL_ScopedName n (&id,
                            0);
          AST_Operation *o = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpIDSeen);

          /*
           * Create a node representing an operation on an interface
           * and add it to its enclosing scope.
           */
          if (s != 0 && tao_yyvsp[-2].dcval != 0)
            {
              AST_Type *tp =
                AST_Type::narrow_from_decl (tao_yyvsp[-2].dcval);

              if (tp == 0)
                {
                  idl_global->err ()->not_a_type (tao_yyvsp[-2].dcval);
                }
              else if (tp->node_type () == AST_Decl::NT_except)
                {
                  idl_global->err ()->not_a_type (tao_yyvsp[-2].dcval);
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
                                                          tao_yyvsp[-3].ofval,
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

  case 327:

    {
//      parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 328:

    {
//      opt_raises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);
        }
    break;

  case 329:

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

              if (tao_yyvsp[-2].nlval != 0 && o != 0)
                {
                  (void) o->fe_add_exceptions (tao_yyvsp[-2].nlval);
                }

              if (tao_yyvsp[0].slval != 0)
                {
                  (void) o->fe_add_context (tao_yyvsp[0].slval);
                }
            }

          /*
           * Done with this operation. Pop its scope from the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 330:

    {
// opt_op_attribute : IDL_ONEWAY
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          tao_yyval.ofval = AST_Operation::OP_oneway;
        }
    break;

  case 331:

    {
//      | IDL_IDEMPOTENT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpAttrSeen);
          tao_yyval.ofval = AST_Operation::OP_idempotent;
        }
    break;

  case 332:

    {
/*      |  EMPTY */
          tao_yyval.ofval = AST_Operation::OP_noflags;
        }
    break;

  case 334:

    {
// op_type_spec : param_type_spec | IDL_VOID
          tao_yyval.dcval =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                  AST_Expression::EV_void
                                                );
        }
    break;

  case 335:

    {
// init_decl : IDL_FACTORY
          //@@ PS_FactorySeen?
          idl_global->set_parse_state (IDL_GlobalData::PS_OpTypeSeen);
        }
    break;

  case 336:

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

          Identifier id (tao_yyvsp[0].strval);
          ACE::strdelete (tao_yyvsp[0].strval);
          tao_yyvsp[0].strval = 0;

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

  case 337:

    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 338:

    {
//      opt_raises
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseCompleted);

          if (tao_yyvsp[0].nlval != 0)
            {
              UTL_Scope *s = idl_global->scopes ().top_non_null ();
              AST_Factory *f = AST_Factory::narrow_from_scope (s);
              (void) f->fe_add_exceptions (tao_yyvsp[0].nlval);
            }

          idl_global->scopes ().pop ();
        }
    break;

  case 339:

    {
// init_parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 340:

    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 341:

    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 342:

    {
//      at_least_one_in_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 344:

    {
// in_parameters : in_parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
    break;

  case 347:

    {
// in_parameter : IDL_IN
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
    break;

  case 348:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
    break;

  case 349:

    {
//      declarator
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Argument *a = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDeclSeen);

          /*
           * Create a node representing an argument to an operation
           * Add it to the enclosing scope (the operation scope).
           */
          if (tao_yyvsp[-2].dcval != 0
              && tao_yyvsp[0].deval != 0 &&
              s != 0)
            {
              AST_Type *tp = tao_yyvsp[0].deval->compose (tao_yyvsp[-2].dcval);

              if (tp != 0)
                {
                  a = idl_global->gen ()->create_argument (
                          AST_Argument::dir_IN,
                          tp,
                          tao_yyvsp[0].deval->name ()
                        );

                  (void) s->fe_add_argument (a);
                }
            }

          tao_yyvsp[0].deval->destroy ();
          delete tao_yyvsp[0].deval;
          tao_yyvsp[0].deval = 0;
        }
    break;

  case 350:

    {
// parameter_list : '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 351:

    {
//      ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 352:

    {
//      | '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSqSeen);
        }
    break;

  case 353:

    {
//      at_least_one_parameter ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpQsSeen);
        }
    break;

  case 355:

    {
// parameters : parameters ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParCommaSeen);
        }
    break;

  case 358:

    {
// parameter : direction
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDirSeen);
        }
    break;

  case 359:

    {
//      param_type_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParTypeSeen);
        }
    break;

  case 360:

    {
//      declarator
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Argument *a = 0;
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParDeclSeen);

          /*
           * Create a node representing an argument to an operation
           * Add it to the enclosing scope (the operation scope).
           */
          if (tao_yyvsp[-2].dcval != 0
              && tao_yyvsp[0].deval != 0
              && s != 0)
            {
              AST_Type *tp = tao_yyvsp[0].deval->compose (tao_yyvsp[-2].dcval);

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
                            tao_yyvsp[-4].dival,
                            tp,
                            tao_yyvsp[0].deval->name ()
                          );
                      (void) s->fe_add_argument (a);
                    }
                }
            }

          tao_yyvsp[0].deval->destroy ();
          delete tao_yyvsp[0].deval;
          tao_yyvsp[0].deval = 0;
        }
    break;

  case 361:

    {
// param_type_spec : base_type_spec
          tao_yyval.dcval =
            idl_global->scopes ().bottom ()->lookup_primitive_type (
                                                 tao_yyvsp[0].etval
                                               );
        }
    break;

  case 364:

    {
//      | string_type_spec
//      | wstring_type_spec
//      | scoped_name
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = 0;

          if (s != 0)
            {
              d = s->lookup_by_name (tao_yyvsp[0].idlist,
                                     true);
            }

          if (d == 0)
            {
              idl_global->err ()->lookup_error (tao_yyvsp[0].idlist);
            }
          else
            {
              d->last_referenced_as (tao_yyvsp[0].idlist);
              AST_Decl::NodeType nt = d->node_type ();

              if (nt == AST_Decl::NT_struct_fwd
                  || nt == AST_Decl::NT_union_fwd
                  || nt == AST_Decl::NT_struct
                  || nt == AST_Decl::NT_union)
                {
                  if (! AST_Type::narrow_from_decl (d)->is_defined ())
                    {
                      idl_global->err ()->error1 (UTL_Error::EIDL_ILLEGAL_ADD,
                                                  d);

                      /* If we don't return here, we'll crash later.*/
                      return 1;
                    }
                }
            }


          tao_yyval.dcval = d;
        }
    break;

  case 365:

    {
// direction : IDL_IN
          tao_yyval.dival = AST_Argument::dir_IN;
        }
    break;

  case 366:

    {
//      | IDL_OUT
          tao_yyval.dival = AST_Argument::dir_OUT;
        }
    break;

  case 367:

    {
//      | IDL_INOUT
          tao_yyval.dival = AST_Argument::dir_INOUT;
        }
    break;

  case 368:

    {
// opt_raises : IDL_RAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSeen);
        }
    break;

  case 369:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseSqSeen);
        }
    break;

  case 370:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpRaiseQsSeen);
          tao_yyval.nlval = tao_yyvsp[-1].nlval;
        }
    break;

  case 371:

    {
          tao_yyval.nlval = 0;
/*      |  EMPTY */
        }
    break;

  case 372:

    {
// opt_getraises : IDL_GETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSeen);
        }
    break;

  case 373:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseSqSeen);
        }
    break;

  case 374:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpGetRaiseQsSeen);
          tao_yyval.nlval = tao_yyvsp[-1].nlval;
        }
    break;

  case 375:

    {
          tao_yyval.nlval = 0;
/*      |  EMPTY */
        }
    break;

  case 376:

    {
// opt_setraises : IDL_SETRAISES
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSeen);
        }
    break;

  case 377:

    {
//      '('
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseSqSeen);
        }
    break;

  case 378:

    {
//      at_least_one_scoped_name ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpSetRaiseQsSeen);
          tao_yyval.nlval = tao_yyvsp[-1].nlval;
        }
    break;

  case 379:

    {
          tao_yyval.nlval = 0;
/*      |  EMPTY */
        }
    break;

  case 380:

    {
// opt_context : IDL_CONTEXT
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSeen);
        }
    break;

  case 381:

    {
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextSqSeen);
//      '('
        }
    break;

  case 382:

    {
//      at_least_one_string_literal ')'
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextQsSeen);
          tao_yyval.slval = tao_yyvsp[-1].slval;
        }
    break;

  case 383:

    {
/*      |  EMPTY */
          tao_yyval.slval = 0;
        }
    break;

  case 384:

    {
// at_least_one_string_literal : IDL_STRING_LITERAL string_literals
          ACE_NEW_RETURN (tao_yyval.slval,
                          UTL_StrList (tao_yyvsp[-1].sval,
                                       tao_yyvsp[0].slval),
                          1);
        }
    break;

  case 385:

    {
// string_literals : string_literals ','
          idl_global->set_parse_state (IDL_GlobalData::PS_OpContextCommaSeen);
        }
    break;

  case 386:

    {
//      IDL_STRING_LITERAL
          if (tao_yyvsp[-3].slval == 0)
            {
              ACE_NEW_RETURN (tao_yyval.slval,
                              UTL_StrList (tao_yyvsp[0].sval,
                                           0),
                              1);
            }
          else
            {
              UTL_StrList *sl = 0;
              ACE_NEW_RETURN (sl,
                              UTL_StrList (tao_yyvsp[0].sval,
                                           0),
                              1);
              tao_yyvsp[-3].slval->nconc (sl);
              tao_yyval.slval = tao_yyvsp[-3].slval;
            }
        }
    break;

  case 387:

    {
/*      |  EMPTY */
          tao_yyval.slval = 0;
        }
    break;

  case 388:

    {
// typeid_dcl : IDL_TYPEID scoped_name IDL_STRING_LITERAL
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name (tao_yyvsp[-1].idlist,
                                           true);

          if (d == 0)
            {
              idl_global->err ()->lookup_error (tao_yyvsp[-1].idlist);
            }
          else
            {
              d->set_id_with_typeid (
                     tao_yyvsp[0].sval->get_string ()
                   );
            }

          tao_yyvsp[-1].idlist->destroy ();
          delete tao_yyvsp[-1].idlist;
          tao_yyvsp[-1].idlist = 0;

          tao_yyvsp[0].sval->destroy ();
          delete tao_yyvsp[0].sval;
          tao_yyvsp[0].sval = 0;
        }
    break;

  case 389:

    {
// typeprefix_dcl : IDL_TYPEPREFIX scoped_name IDL_STRING_LITERAL
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = ScopeAsDecl (s);

          // If we are in a module, we want to avoid a lookup unless the
          // typeprefix is to be applied to some other scope, since we
          // might get a previous opening of the module, and the prefix
          // of this opening would never get checked or set.
          if (d->name ()->compare (tao_yyvsp[-1].idlist) != 0)
            {
              d = s->lookup_by_name (tao_yyvsp[-1].idlist,
                                     true);
            }

          if (d == 0)
            {
              idl_global->err ()->lookup_error (tao_yyvsp[-1].idlist);
            }
          else
            {
              d->set_prefix_with_typeprefix (
                     tao_yyvsp[0].sval->get_string ()
                   );
            }

          tao_yyvsp[-1].idlist->destroy ();
          delete tao_yyvsp[-1].idlist;
          tao_yyvsp[-1].idlist = 0;

          tao_yyvsp[0].sval->destroy ();
          delete tao_yyvsp[0].sval;
          tao_yyvsp[0].sval = 0;
        }
    break;

  case 392:

    {
// component_forward_decl : IDL_COMPONENT id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n (tao_yyvsp[0].idval,
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

          tao_yyvsp[0].idval->destroy ();
          delete tao_yyvsp[0].idval;
          tao_yyvsp[0].idval = 0;
        }
    break;

  case 393:

    {
// component_decl : component_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Component *c = 0;

          /*
           * Make a new component node and add it to the enclosing scope.
           */
          if (s != 0 && tao_yyvsp[0].chval != 0)
            {
              c =
                idl_global->gen ()->create_component (
                                        tao_yyvsp[0].chval->name (),
                                        tao_yyvsp[0].chval->base_component (),
                                        tao_yyvsp[0].chval->supports (),
                                        tao_yyvsp[0].chval->n_supports (),
                                        tao_yyvsp[0].chval->supports_flat (),
                                        tao_yyvsp[0].chval->n_supports_flat ()
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
              tao_yyvsp[0].chval->destroy ();
              delete tao_yyvsp[0].chval;
              tao_yyvsp[0].chval = 0;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (c);
        }
    break;

  case 394:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentSqSeen);
        }
    break;

  case 395:

    {
//      component_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentBodySeen);
        }
    break;

  case 396:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentQsSeen);

          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 397:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_ComponentIDSeen);
        }
    break;

  case 398:

    {
//      component_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 399:

    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);

          /*
           * Create an AST representation of the information in the header
           * part of a component.
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName (tao_yyvsp[-4].idval, 0),
                          1);
          ACE_NEW_RETURN (tao_yyval.chval,
                          FE_ComponentHeader (n,
                                              tao_yyvsp[-2].idlist,
                                              tao_yyvsp[0].nlval,
                                              false),
                          1);

          if (0 != tao_yyvsp[0].nlval)
            {
              tao_yyvsp[0].nlval->destroy ();
              delete tao_yyvsp[0].nlval;
              tao_yyvsp[0].nlval = 0;
            }

          if (0 != tao_yyvsp[-2].idlist)
            {
              tao_yyvsp[-2].idlist->destroy ();
              delete tao_yyvsp[-2].idlist;
              tao_yyvsp[-2].idlist = 0;
            }
        }
    break;

  case 400:

    {
// component_inheritance_spec : ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
    break;

  case 401:

    {
//      scoped_name
          tao_yyval.idlist = tao_yyvsp[0].idlist;
        }
    break;

  case 402:

    {
/*      |  EMPTY */
          tao_yyval.idlist = 0;
        }
    break;

  case 405:

    {
// component_export : provides_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ProvidesDeclSeen);
        }
    break;

  case 406:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 407:

    {
//      | uses_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_UsesDeclSeen);
        }
    break;

  case 408:

    {
//        ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 409:

    {
//      | emits_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_EmitsDeclSeen);
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
//      | publishes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_PublishesDeclSeen);
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
//      | consumes_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_ConsumesDeclSeen);
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
//      | attribute
          idl_global->set_parse_state (IDL_GlobalData::PS_AttrDeclSeen);
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
// provides_decl : IDL_PROVIDES interface_type id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Component *c = AST_Component::narrow_from_scope (s);

          if (c != 0)
            {
              AST_Decl *d = s->lookup_by_name (tao_yyvsp[-1].idlist,
                                               true);
              if (0 == d)
                {
                  idl_global->err ()->lookup_error (tao_yyvsp[-1].idlist);

                  tao_yyvsp[-1].idlist->destroy ();
                  delete tao_yyvsp[-1].idlist;
                  tao_yyvsp[-1].idlist = 0;

                  tao_yyvsp[0].idval->destroy ();
                  delete tao_yyvsp[0].idval;
                  tao_yyvsp[0].idval = 0;

                  break;
                }
              else if (d->node_type () != AST_Decl::NT_interface)
                {
                  // Nothing else but CORBA::Object can have
                  // this identifier.
                  if (ACE_OS::strcmp (d->local_name ()->get_string (),
                                      "Object")
                        != 0)
                    {
                      idl_global->err ()->interface_expected (d);

                      tao_yyvsp[-1].idlist->destroy ();
                      delete tao_yyvsp[-1].idlist;
                      tao_yyvsp[-1].idlist = 0;

                      tao_yyvsp[0].idval->destroy ();
                      delete tao_yyvsp[0].idval;
                      tao_yyvsp[0].idval = 0;

                      break;
                    }
                }

              AST_Type *interface_type =
                AST_Type::narrow_from_decl (d);

              AST_Component::port_description pd;
              pd.id = tao_yyvsp[0].idval;
              pd.impl = interface_type;
              pd.line_number = idl_global->lineno ();
              c->provides ().enqueue_tail (pd);
            }

          tao_yyvsp[-1].idlist->destroy ();
          delete tao_yyvsp[-1].idlist;
          tao_yyvsp[-1].idlist = 0;
        }
    break;

  case 418:

    {
// interface_type : scoped_name
          // Lookups and checking are done where the 'interface_type'
          // token is used, in 'provides_decl' and 'uses_decl'.
          tao_yyval.idlist = tao_yyvsp[0].idlist;
        }
    break;

  case 419:

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
          tao_yyval.idlist = corba_name;
        }
    break;

  case 420:

    {
// uses_decl : IDL_USES opt_multiple interface_type id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name (tao_yyvsp[-1].idlist,
                                           true);

          if (0 == d)
            {
              idl_global->err ()->lookup_error (tao_yyvsp[-1].idlist);

              tao_yyvsp[-1].idlist->destroy ();
              delete tao_yyvsp[-1].idlist;
              tao_yyvsp[-1].idlist = 0;

              tao_yyvsp[0].idval->destroy ();
              delete tao_yyvsp[0].idval;
              tao_yyvsp[0].idval = 0;

              break;
            }
          else if (d->node_type () != AST_Decl::NT_interface)
            {
              if (ACE_OS::strcmp (d->local_name ()->get_string (),
                                  "Object")
                    != 0)
                {
                  idl_global->err ()->interface_expected (d);

                  tao_yyvsp[-1].idlist->destroy ();
                  delete tao_yyvsp[-1].idlist;
                  tao_yyvsp[-1].idlist = 0;

                  tao_yyvsp[0].idval->destroy ();
                  delete tao_yyvsp[0].idval;
                  tao_yyvsp[0].idval = 0;

                  break;
                }
            }

          AST_Type *interface_type = AST_Type::narrow_from_decl (d);
          AST_Component *c = AST_Component::narrow_from_scope (s);

          if (c != 0)
            {
              AST_Component::port_description ud;
              ud.id = tao_yyvsp[0].idval;
              ud.impl = interface_type;
              ud.is_multiple = tao_yyvsp[-2].bval;
              ud.line_number = idl_global->lineno ();
              c->uses ().enqueue_tail (ud);

              if (ud.is_multiple == true
                  && !idl_global->using_ifr_backend ()
                  && !idl_global->ignore_idl3 ())
                {
                  // These datatypes must be created in the
                  // front end so they can be looked up
                  // when compiling the generated executor IDL.
                  idl_global->create_uses_multiple_stuff (c, ud);
                }
            }

          tao_yyvsp[-1].idlist->destroy ();
          delete tao_yyvsp[-1].idlist;
          tao_yyvsp[-1].idlist = 0;
        }
    break;

  case 421:

    {
// opt_multiple : IDL_MULTIPLE
          tao_yyval.bval = true;
        }
    break;

  case 422:

    {
/*      |  EMPTY */
          tao_yyval.bval = false;
        }
    break;

  case 423:

    {
// emits_decl : IDL_EMITS scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name (tao_yyvsp[-1].idlist,
                                           true);

          if (0 == d)
            {
              idl_global->err ()->lookup_error (tao_yyvsp[-1].idlist);

              tao_yyvsp[-1].idlist->destroy ();
              delete tao_yyvsp[-1].idlist;
              tao_yyvsp[-1].idlist = 0;

              tao_yyvsp[0].idval->destroy ();
              delete tao_yyvsp[0].idval;
              tao_yyvsp[0].idval = 0;

              break;
            }
          else if (d->node_type () != AST_Decl::NT_eventtype)
            {
              idl_global->err ()->eventtype_expected (d);

              tao_yyvsp[-1].idlist->destroy ();
              delete tao_yyvsp[-1].idlist;
              tao_yyvsp[-1].idlist = 0;

              tao_yyvsp[0].idval->destroy ();
              delete tao_yyvsp[0].idval;
              tao_yyvsp[0].idval = 0;

              break;
            }
          else
            {
              AST_Type *event_type = AST_Type::narrow_from_decl (d);
              AST_Component *c = AST_Component::narrow_from_scope (s);

              if (c != 0)
                {
                  AST_Component::port_description pd;
                  pd.id = tao_yyvsp[0].idval;
                  pd.impl = event_type;
                  pd.line_number = idl_global->lineno ();
                 c->emits ().enqueue_tail (pd);
                }
            }

          tao_yyvsp[-1].idlist->destroy ();
          delete tao_yyvsp[-1].idlist;
          tao_yyvsp[-1].idlist = 0;
        }
    break;

  case 424:

    {
// publishes_decl : IDL_PUBLISHES scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name (tao_yyvsp[-1].idlist,
                                           true);

          if (0 == d)
            {
              idl_global->err ()->lookup_error (tao_yyvsp[-1].idlist);

              tao_yyvsp[-1].idlist->destroy ();
              delete tao_yyvsp[-1].idlist;
              tao_yyvsp[-1].idlist = 0;

              tao_yyvsp[0].idval->destroy ();
              delete tao_yyvsp[0].idval;
              tao_yyvsp[0].idval = 0;

              break;
            }
          else if (d->node_type () != AST_Decl::NT_eventtype)
            {
              idl_global->err ()->eventtype_expected (d);

              tao_yyvsp[-1].idlist->destroy ();
              delete tao_yyvsp[-1].idlist;
              tao_yyvsp[-1].idlist = 0;

              tao_yyvsp[0].idval->destroy ();
              delete tao_yyvsp[0].idval;
              tao_yyvsp[0].idval = 0;

              break;
            }
          else
            {
              AST_Type *event_type = AST_Type::narrow_from_decl (d);
              AST_Component *c = AST_Component::narrow_from_scope (s);

              if (c != 0)
                {
                  AST_Component::port_description pd;
                  pd.id = tao_yyvsp[0].idval;
                  pd.impl = event_type;
                  pd.line_number = idl_global->lineno ();
                  c->publishes ().enqueue_tail (pd);
                }
            }

          tao_yyvsp[-1].idlist->destroy ();
          delete tao_yyvsp[-1].idlist;
          tao_yyvsp[-1].idlist = 0;
        }
    break;

  case 425:

    {
// consumes_decl : IDL_CONSUMES scoped_name id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Decl *d = s->lookup_by_name (tao_yyvsp[-1].idlist,
                                           true);

          if (0 == d)
            {
              idl_global->err ()->lookup_error (tao_yyvsp[-1].idlist);

              tao_yyvsp[-1].idlist->destroy ();
              delete tao_yyvsp[-1].idlist;
              tao_yyvsp[-1].idlist = 0;

              tao_yyvsp[0].idval->destroy ();
              delete tao_yyvsp[0].idval;
              tao_yyvsp[0].idval = 0;

              break;
            }
          else if (d->node_type () != AST_Decl::NT_eventtype)
            {
              idl_global->err ()->eventtype_expected (d);

              tao_yyvsp[-1].idlist->destroy ();
              delete tao_yyvsp[-1].idlist;
              tao_yyvsp[-1].idlist = 0;

              tao_yyvsp[0].idval->destroy ();
              delete tao_yyvsp[0].idval;
              tao_yyvsp[0].idval = 0;

              break;
            }
          else
            {
              AST_Type *event_type = AST_Type::narrow_from_decl (d);
              AST_Component *c = AST_Component::narrow_from_scope (s);

              if (c != 0)
                {
                  AST_Component::port_description pd;
                  pd.id = tao_yyvsp[0].idval;
                  pd.impl = event_type;
                  pd.line_number = idl_global->lineno ();
                  c->consumes ().enqueue_tail (pd);
                }
            }

          tao_yyvsp[-1].idlist->destroy ();
          delete tao_yyvsp[-1].idlist;
          tao_yyvsp[-1].idlist = 0;
        }
    break;

  case 426:

    {
// home_decl : home_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_Home *h = 0;

          /*
           * Make a new home node and add it to the enclosing scope.
           */
          if (s != 0 && tao_yyvsp[0].hhval != 0)
            {
              h =
                idl_global->gen ()->create_home (
                                        tao_yyvsp[0].hhval->name (),
                                        tao_yyvsp[0].hhval->base_home (),
                                        tao_yyvsp[0].hhval->managed_component (),
                                        tao_yyvsp[0].hhval->primary_key (),
                                        tao_yyvsp[0].hhval->supports (),
                                        tao_yyvsp[0].hhval->n_supports (),
                                        tao_yyvsp[0].hhval->supports_flat (),
                                        tao_yyvsp[0].hhval->n_supports_flat ()
                                      );
              /*
               * Add the home to its definition scope.
               */
              (void) s->fe_add_home (h);

              // This FE_HomeHeader class isn't destroyed with the AST.
              tao_yyvsp[0].hhval->destroy ();
              delete tao_yyvsp[0].hhval;
              tao_yyvsp[0].hhval = 0;
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (h);
        }
    break;

  case 427:

    {
//      home_body
          /*
           * Done with this component - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 428:

    {
// home_header : IDL_HOME
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSeen);
        }
    break;

  case 429:

    {
//      id
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeIDSeen);
        }
    break;

  case 430:

    {
//      home_inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);
        }
    break;

  case 431:

    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);
        }
    break;

  case 432:

    {
//      IDL_MANAGES
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesSeen);
        }
    break;

  case 433:

    {
//      scoped_name
          idl_global->set_parse_state (IDL_GlobalData::PS_ManagesIDSeen);
        }
    break;

  case 434:

    {
//      primary_key_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_PrimaryKeySpecSeen);

          /*
           * Create an AST representation of the information in the header
           * part of a component home.
           */
          UTL_ScopedName *n = 0;
          ACE_NEW_RETURN (n,
                          UTL_ScopedName (tao_yyvsp[-10].idval, 0),
                          1);
          ACE_NEW_RETURN (tao_yyval.hhval,
                          FE_HomeHeader (n,
                                         tao_yyvsp[-8].idlist,
                                         tao_yyvsp[-6].nlval,
                                         tao_yyvsp[-2].idlist,
                                         tao_yyvsp[0].idlist),
                          1);

          tao_yyvsp[-2].idlist->destroy ();
          delete tao_yyvsp[-2].idlist;
          tao_yyvsp[-2].idlist = 0;

          if (0 != tao_yyvsp[-8].idlist)
            {
              tao_yyvsp[-8].idlist->destroy ();
              delete tao_yyvsp[-8].idlist;
              tao_yyvsp[-8].idlist = 0;
            }

          if (0 != tao_yyvsp[0].idlist)
            {
              tao_yyvsp[0].idlist->destroy ();
              delete tao_yyvsp[0].idlist;
              tao_yyvsp[0].idlist = 0;
            }

          if (0 != tao_yyvsp[-6].nlval)
            {
              tao_yyvsp[-6].nlval->destroy ();
              delete tao_yyvsp[-6].nlval;
              tao_yyvsp[-6].nlval = 0;
            }
        }
    break;

  case 435:

    {
// home_inheritance_spec ':'
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritColonSeen);
        }
    break;

  case 436:

    {
//      scoped_name
          tao_yyval.idlist = tao_yyvsp[0].idlist;
        }
    break;

  case 437:

    {
/*      |  EMPTY */
          tao_yyval.idlist = 0;
        }
    break;

  case 438:

    {
// primary_key_spec : IDL_PRIMARYKEY scoped_name
          tao_yyval.idlist = tao_yyvsp[0].idlist;
        }
    break;

  case 439:

    {
/*      |  EMPTY */
          tao_yyval.idlist = 0;
        }
    break;

  case 440:

    {
// home_body : '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeSqSeen);
        }
    break;

  case 441:

    {
//      home_exports
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeBodySeen);
        }
    break;

  case 442:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_HomeQsSeen);
        }
    break;

  case 446:

    {
// home_export : factory_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FactoryDeclSeen);
        }
    break;

  case 447:

    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 448:

    {
//      | finder_decl
          idl_global->set_parse_state (IDL_GlobalData::PS_FinderDeclSeen);
        }
    break;

  case 449:

    {
//      | ';'
          idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
        }
    break;

  case 450:

    {
// factory_decl : IDL_FACTORY id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n (tao_yyvsp[0].idval,
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

          tao_yyvsp[0].idval->destroy ();
          delete tao_yyvsp[0].idval;
          tao_yyvsp[0].idval = 0;

          /*
           * Push the operation scope onto the scopes stack.
           */
          idl_global->scopes ().push (o);
        }
    break;

  case 451:

    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 452:

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

              if (tao_yyvsp[0].nlval != 0 && o != 0)
                {
                  (void) o->fe_add_exceptions (tao_yyvsp[0].nlval);
                }
            }

          /*
           * Done with this operation. Pop its scope from the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 453:

    {
// finder_decl : IDL_FINDER id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n (tao_yyvsp[0].idval,
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

          tao_yyvsp[0].idval->destroy ();
          delete tao_yyvsp[0].idval;
          tao_yyvsp[0].idval = 0;

          /*
           * Push the operation scope onto the scopes stack.
           */
          idl_global->scopes ().push (o);
        }
    break;

  case 454:

    {
//      init_parameter_list
          idl_global->set_parse_state (IDL_GlobalData::PS_OpParsCompleted);
        }
    break;

  case 455:

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

              if (tao_yyvsp[0].nlval != 0 && o != 0)
                {
                  (void) o->fe_add_exceptions (tao_yyvsp[0].nlval);
                }
            }

          /*
           * Done with this operation. Pop its scope from the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 461:

    {
// event_concrete_forward_decl : IDL_EVENTTYPE id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n (tao_yyvsp[0].idval,
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

          tao_yyvsp[0].idval->destroy ();
          delete tao_yyvsp[0].idval;
          tao_yyvsp[0].idval = 0;
        }
    break;

  case 462:

    {
// event_abs_forward_decl : IDL_ABSTRACT IDL_EVENTTYPE id
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          UTL_ScopedName n (tao_yyvsp[0].idval,
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

          tao_yyvsp[0].idval->destroy ();
          delete tao_yyvsp[0].idval;
          tao_yyvsp[0].idval = 0;
        }
    break;

  case 463:

    {
// event_abs_decl : event_abs_header event_rest_of_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_EventType *e = 0;
          AST_Interface *i = 0;

          if (s != 0 && tao_yyvsp[-1].idval != 0)
            {
              UTL_ScopedName sn (tao_yyvsp[-1].idval,
                                 0);
              e =
                idl_global->gen ()->create_eventtype (
                    &sn,
                    tao_yyvsp[0].ehval->inherits (),
                    tao_yyvsp[0].ehval->n_inherits (),
                    tao_yyvsp[0].ehval->inherits_concrete (),
                    tao_yyvsp[0].ehval->inherits_flat (),
                    tao_yyvsp[0].ehval->n_inherits_flat (),
                    tao_yyvsp[0].ehval->supports (),
                    tao_yyvsp[0].ehval->n_supports (),
                    tao_yyvsp[0].ehval->supports_concrete (),
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

          tao_yyvsp[-1].idval->destroy ();
          delete tao_yyvsp[-1].idval;
          tao_yyvsp[-1].idval = 0;
        }
    break;

  case 464:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
    break;

  case 465:

    {
//      exports
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
    break;

  case 466:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 467:

    {
// event_abs_header : IDL_ABSTRACT IDL_EVENTTYPE id
          tao_yyval.idval = tao_yyvsp[0].idval;
        }
    break;

  case 468:

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
          tao_yyval.idval = 0;
        }
    break;

  case 469:

    {
// event_plain_header : IDL_EVENTTYPE id
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeIDSeen);

          tao_yyval.idval = tao_yyvsp[0].idval;
        }
    break;

  case 470:

    {
// event_rest_of_header : inheritance_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_InheritSpecSeen);

        }
    break;

  case 471:

    {
//      supports_spec
          idl_global->set_parse_state (IDL_GlobalData::PS_SupportSpecSeen);

          ACE_NEW_RETURN (tao_yyval.ehval,
                          FE_EventHeader (
                              0,
                              tao_yyvsp[-2].nlval,
                              tao_yyvsp[0].nlval,
                              tao_yyvsp[-2].nlval
                                ? tao_yyvsp[-2].nlval->truncatable ()
                                : false
                            ),
                          1);

          if (0 != tao_yyvsp[0].nlval)
            {
              tao_yyvsp[0].nlval->destroy ();
              delete tao_yyvsp[0].nlval;
              tao_yyvsp[0].nlval = 0;
            }

          if (0 != tao_yyvsp[-2].nlval)
            {
              tao_yyvsp[-2].nlval->destroy ();
              delete tao_yyvsp[-2].nlval;
              tao_yyvsp[-2].nlval = 0;
            }
        }
    break;

  case 472:

    {
// event_decl : event_header event_rest_of_header
          UTL_Scope *s = idl_global->scopes ().top_non_null ();
          AST_EventType *e = 0;
          AST_Interface *i = 0;

          if (s != 0 && tao_yyvsp[-1].idval != 0)
            {
              // We create the scoped name here instead of with the
              // FE_EventHeader because there is a token for it only here.
              UTL_ScopedName sn (tao_yyvsp[-1].idval,
                                 0);
              e =
                idl_global->gen ()->create_eventtype (
                    &sn,
                    tao_yyvsp[0].ehval->inherits (),
                    tao_yyvsp[0].ehval->n_inherits (),
                    tao_yyvsp[0].ehval->inherits_concrete (),
                    tao_yyvsp[0].ehval->inherits_flat (),
                    tao_yyvsp[0].ehval->n_inherits_flat (),
                    tao_yyvsp[0].ehval->supports (),
                    tao_yyvsp[0].ehval->n_supports (),
                    tao_yyvsp[0].ehval->supports_concrete (),
                    false,
                    tao_yyvsp[0].ehval->truncatable (),
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
              tao_yyvsp[0].ehval->destroy ();
              delete tao_yyvsp[0].ehval;
              tao_yyvsp[0].ehval = 0;

              sn.destroy ();
            }

          /*
           * Push it on the scope stack.
           */
          idl_global->scopes ().push (e);
        }
    break;

  case 473:

    {
//      '{'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeSqSeen);
        }
    break;

  case 474:

    {
//      value_elements
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeBodySeen);
        }
    break;

  case 475:

    {
//      '}'
          idl_global->set_parse_state (IDL_GlobalData::PS_EventTypeQsSeen);

          /*
           * Done with this eventtype - pop it off the scopes stack.
           */
          idl_global->scopes ().pop ();
        }
    break;

  case 476:

    {
// event_header : event_custom_header
          tao_yyval.idval = tao_yyvsp[0].idval;
        }
    break;

  case 477:

    {
// event_header : event_plain_header
          tao_yyval.idval = tao_yyvsp[0].idval;
        }
    break;


    }

/* Line 993 of yacc.c.  */


  tao_yyvsp -= tao_yylen;
  tao_yyssp -= tao_yylen;


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
#if TAO_YYERROR_VERBOSE
      tao_yyn = tao_yypact[tao_yystate];

      if (TAO_YYPACT_NINF < tao_yyn && tao_yyn < TAO_YYLAST)
        {
          TAO_YYSIZE_T tao_yysize = 0;
          int tao_yytype = TAO_YYTRANSLATE (tao_yychar);
          const char* tao_yyprefix;
          char *tao_yymsg;
          int tao_yyx;

          /* Start TAO_YYX at -TAO_YYN if negative to avoid negative indexes in
             TAO_YYCHECK.  */
          int tao_yyxbegin = tao_yyn < 0 ? -tao_yyn : 0;

          /* Stay within bounds of both tao_yycheck and tao_yytname.  */
          int tao_yychecklim = TAO_YYLAST - tao_yyn;
          int tao_yyxend = tao_yychecklim < TAO_YYNTOKENS ? tao_yychecklim : TAO_YYNTOKENS;
          int tao_yycount = 0;

          tao_yyprefix = ", expecting ";
          for (tao_yyx = tao_yyxbegin; tao_yyx < tao_yyxend; ++tao_yyx)
            if (tao_yycheck[tao_yyx + tao_yyn] == tao_yyx && tao_yyx != TAO_YYTERROR)
              {
                tao_yysize += tao_yystrlen (tao_yyprefix) + tao_yystrlen (tao_yytname [tao_yyx]);
                tao_yycount += 1;
                if (tao_yycount == 5)
                  {
                    tao_yysize = 0;
                    break;
                  }
              }
          tao_yysize += (sizeof ("syntax error, unexpected ")
                     + tao_yystrlen (tao_yytname[tao_yytype]));
          tao_yymsg = (char *) TAO_YYSTACK_ALLOC (tao_yysize);
          if (tao_yymsg != 0)
            {
              char *tao_yyp = tao_yystpcpy (tao_yymsg, "syntax error, unexpected ");
              tao_yyp = tao_yystpcpy (tao_yyp, tao_yytname[tao_yytype]);

              if (tao_yycount < 5)
                {
                  tao_yyprefix = ", expecting ";
                  for (tao_yyx = tao_yyxbegin; tao_yyx < tao_yyxend; ++tao_yyx)
                    if (tao_yycheck[tao_yyx + tao_yyn] == tao_yyx && tao_yyx != TAO_YYTERROR)
                      {
                        tao_yyp = tao_yystpcpy (tao_yyp, tao_yyprefix);
                        tao_yyp = tao_yystpcpy (tao_yyp, tao_yytname[tao_yyx]);
                        tao_yyprefix = " or ";
                      }
                }
              tao_yyerror (tao_yymsg);
              TAO_YYSTACK_FREE (tao_yymsg);
            }
          else
            tao_yyerror ("syntax error; also virtual memory exhausted");
        }
      else
#endif /* TAO_YYERROR_VERBOSE */
        tao_yyerror ("syntax error");
    }



  if (tao_yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (tao_yychar <= TAO_YYEOF)
        {
          /* If at end of input, pop the error token,
             then the rest of the stack, then return failure.  */
          if (tao_yychar == TAO_YYEOF)
             for (;;)
               {
                 TAO_YYPOPSTACK;
                 if (tao_yyssp == tao_yyss)
                   TAO_YYABORT;
                 TAO_YYDSYMPRINTF ("Error: popping", tao_yystos[*tao_yyssp], tao_yyvsp, tao_yylsp);
                 tao_yydestruct (tao_yystos[*tao_yyssp], tao_yyvsp);
               }
        }
      else
        {
          TAO_YYDSYMPRINTF ("Error: discarding", tao_yytoken, &tao_yylval, &tao_yylloc);
          tao_yydestruct (tao_yytoken, &tao_yylval);
          tao_yychar = TAO_YYEMPTY;

        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto tao_yyerrlab1;


/*---------------------------------------------------.
| tao_yyerrorlab -- error raised explicitly by TAO_YYERROR.  |
`---------------------------------------------------*/
tao_yyerrorlab:

#if defined (__GNUC__) || defined (WIN32) || defined (__HP_aCC)
  /* Pacify GCC when the user code never invokes TAO_YYERROR and the label
     tao_yyerrorlab therefore never appears in user code.  */
  if (0)
     goto tao_yyerrorlab;
#endif

  tao_yyvsp -= tao_yylen;
  tao_yyssp -= tao_yylen;
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

      TAO_YYDSYMPRINTF ("Error: popping", tao_yystos[*tao_yyssp], tao_yyvsp, tao_yylsp);
      tao_yydestruct (tao_yystos[tao_yystate], tao_yyvsp);
      TAO_YYPOPSTACK;
      tao_yystate = *tao_yyssp;
      TAO_YY_STACK_PRINT (tao_yyss, tao_yyssp);
    }

  if (tao_yyn == TAO_YYFINAL)
    TAO_YYACCEPT;

  TAO_YYDPRINTF ((stderr, "Shifting error token, "));

  *++tao_yyvsp = tao_yylval;


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
/*----------------------------------------------.
| tao_yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
tao_yyoverflowlab:
  tao_yyerror ("parser stack overflow");
  tao_yyresult = 2;
  /* Fall through.  */
#endif

tao_yyreturn:
#ifndef tao_yyoverflow
  if (tao_yyss != tao_yyssa)
    TAO_YYSTACK_FREE (tao_yyss);
#endif
  return tao_yyresult;
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
