// $Id$
# line 74 "fe/idl.tao_yy"
#include "idl.h"
#include "idl_extern.h"
#include "fe_private.h"
#include <stdio.h>

#if (defined(apollo) || defined(hpux)) && defined(__cplusplus)
extern	"C" int tao_yywrap();
#endif	// (defined(apollo) || defined(hpux)) && defined(__cplusplus)

void tao_yyerror (const char *);
int tao_yylex (void);
extern "C" int tao_yywrap (void);
extern char tao_yytext[];
extern int tao_yyleng;
#define TAO_YYDEBUG_LEXER_TEXT (tao_yytext[tao_yyleng] = '\0', tao_yytext)
// Force the pretty debugging code to compile.
#define TAO_YYDEBUG 1

# line 97 "fe/idl.tao_yy"
typedef union
#ifdef __cplusplus
	TAO_YYSTYPE
#endif
 {
  AST_Decl		*dcval;		/* Decl value		*/
  UTL_StrList		*slval;		/* String list		*/
  UTL_NameList		*nlval;		/* Name list		*/
  UTL_ExprList		*elval;		/* Expression list	*/
  UTL_LabelList		*llval;		/* Label list		*/
  UTL_DeclList		*dlval;		/* Declaration list	*/
  FE_InterfaceHeader	*ihval;		/* Interface header	*/
  AST_Expression	*exval;		/* Expression value	*/
  AST_UnionLabel	*ulval;		/* Union label		*/
  AST_Field		*ffval;		/* Field value		*/
  AST_Expression::ExprType etval;	/* Expression type	*/
  AST_Argument::Direction dival;	/* Argument direction	*/
  AST_Operation::Flags	ofval;		/* Operation flags	*/
  FE_Declarator		*deval;		/* Declarator value	*/
  idl_bool		bval;		/* Boolean value	*/
  long			ival;		/* Long value		*/
  double		dval;		/* Double value		*/
  float			fval;		/* Float value		*/
  char			cval;		/* Char value		*/

  String		*sval;		/* String value		*/
  char			*strval;	/* char * value		*/
  Identifier		*idval;		/* Identifier		*/
  UTL_IdList		*idlist;	/* Identifier list	*/
} TAO_YYSTYPE;
# define IDENTIFIER 257
# define IDL_CONST 258
# define IDL_MODULE 259
# define IDL_INTERFACE 260
# define IDL_TYPEDEF 261
# define IDL_LONG 262
# define IDL_SHORT 263
# define IDL_UNSIGNED 264
# define IDL_DOUBLE 265
# define IDL_FLOAT 266
# define IDL_CHAR 267
# define IDL_WCHAR 268
# define IDL_OCTET 269
# define IDL_BOOLEAN 270
# define IDL_ANY 271
# define IDL_STRUCT 272
# define IDL_UNION 273
# define IDL_SWITCH 274
# define IDL_ENUM 275
# define IDL_SEQUENCE 276
# define IDL_STRING 277
# define IDL_WSTRING 278
# define IDL_EXCEPTION 279
# define IDL_CASE 280
# define IDL_DEFAULT 281
# define IDL_READONLY 282
# define IDL_ATTRIBUTE 283
# define IDL_ONEWAY 284
# define IDL_IDEMPOTENT 285
# define IDL_VOID 286
# define IDL_IN 287
# define IDL_OUT 288
# define IDL_INOUT 289
# define IDL_RAISES 290
# define IDL_CONTEXT 291
# define IDL_NATIVE 292
# define IDL_INTEGER_LITERAL 293
# define IDL_STRING_LITERAL 294
# define IDL_CHARACTER_LITERAL 295
# define IDL_FLOATING_PT_LITERAL 296
# define IDL_TRUETOK 297
# define IDL_FALSETOK 298
# define IDL_SCOPE_DELIMITOR 299
# define IDL_LEFT_SHIFT 300
# define IDL_RIGHT_SHIFT 301

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#else
#include <malloc.h>
#include <memory.h>
#endif


#ifdef __cplusplus

#ifndef tao_yyerror
	void tao_yyerror(const char *);
#endif

#ifndef tao_yylex
#ifdef __EXTERN_C__
	extern "C" { int tao_yylex(void); }
#else
	int tao_yylex(void);
#endif
#endif
	int tao_yyparse(void);

#endif
#define tao_yyclearin tao_yychar = -1
#define tao_yyerrok tao_yyerrflag = 0
extern int tao_yychar;
extern int tao_yyerrflag;
TAO_YYSTYPE tao_yylval;
TAO_YYSTYPE tao_yyval;
typedef int tao_yytabelem;
#ifndef TAO_YYMAXDEPTH
#define TAO_YYMAXDEPTH 150
#endif
#if TAO_YYMAXDEPTH > 0
int tao_yy_tao_yys[TAO_YYMAXDEPTH], *tao_yys = tao_yy_tao_yys;
TAO_YYSTYPE tao_yy_tao_yyv[TAO_YYMAXDEPTH], *tao_yyv = tao_yy_tao_yyv;
#else	/* user does initial allocation */
int *tao_yys;
TAO_YYSTYPE *tao_yyv;
#endif
static int tao_yymaxdepth = TAO_YYMAXDEPTH;
# define TAO_YYERRCODE 256

# line 2289 "fe/idl.tao_yy"

/* programs */

/*
 * ???
 */
int
tao_yywrap()
{
  return 1;
}

/*
 * Report an error situation discovered in a production
 *
 * This does not do anything since we report all error situations through
 * idl_global->err() operations
 */
void
tao_yyerror(const char *)
{
}
static const tao_yytabelem tao_yyexca[] ={
-1, 0,
	0, 3,
	-2, 0,
-1, 1,
	0, -1,
	-2, 0,
-1, 3,
	0, 3,
	125, 3,
	-2, 0,
-1, 24,
	123, 32,
	-2, 57,
-1, 134,
	91, 229,
	-2, 138,
-1, 149,
	257, 254,
	262, 254,
	263, 254,
	264, 254,
	265, 254,
	266, 254,
	267, 254,
	268, 254,
	269, 254,
	270, 254,
	271, 254,
	277, 254,
	283, 241,
	286, 254,
	299, 254,
	125, 25,
	-2, 0,
-1, 181,
	125, 3,
	-2, 0,
-1, 222,
	125, 245,
	-2, 0,
-1, 269,
	125, 161,
	-2, 0,
-1, 319,
	41, 257,
	-2, 259,
-1, 357,
	125, 184,
	-2, 0,
	};
# define TAO_YYNPROD 286
# define TAO_YYLAST 528
static const tao_yytabelem tao_yyact[]={

    72,   131,   347,   331,   344,    57,   167,   155,   128,   241,
    70,   230,   132,   207,   208,    35,   226,   110,   393,   388,
   333,   334,   335,   352,   328,   239,   147,    34,   349,   348,
   130,   112,   118,   119,   113,   112,   141,    36,    36,   101,
    59,   103,   104,   105,   100,   227,    36,   108,    98,   169,
    54,    78,    87,    88,    76,    77,    79,    80,    82,    81,
    83,    20,    21,    36,    22,    89,    90,    91,    78,    87,
    88,    76,    77,    79,    80,    82,    81,    83,    20,    21,
    73,    22,    89,    90,    91,   231,   305,    36,    73,   102,
   201,   346,    78,    87,    88,    76,    77,    79,    80,    82,
    81,    83,   162,   222,   160,    73,    89,    90,    91,   161,
   134,   154,   137,    36,   129,   349,   348,   159,   301,    87,
    88,    67,   143,    79,    80,    82,    81,   158,     2,    73,
   157,    22,    26,    36,   265,     6,     5,   153,    78,    87,
    88,    76,    77,    79,    80,    82,    81,    83,    55,   169,
    66,   367,   164,    90,   165,    73,   302,   291,   290,    95,
    36,   289,   242,   232,   204,    78,    87,    88,    76,    77,
    79,    80,    82,    81,    83,    73,    65,   217,    62,    13,
    90,    63,    13,   325,     4,    59,    36,   148,    96,    59,
   163,    78,    87,    88,    76,    77,    79,    80,    82,    81,
   243,   245,    73,   146,   145,   244,    90,    91,   192,   144,
    15,   106,   205,    10,    94,   312,   202,   263,   262,    93,
   250,   114,   221,   142,    20,    21,   117,    22,    73,    59,
   116,    16,   384,   371,   195,   166,   196,   197,     9,   115,
    15,    19,    25,    10,    14,   313,    61,    12,   134,   284,
    12,   176,   177,   283,    20,    21,   293,    22,   280,    60,
    11,    16,   279,    11,   278,   277,    36,   276,   275,    52,
    51,   134,   288,    50,    14,    49,    59,    48,    47,   300,
   386,   373,    45,   391,   213,   189,   188,   243,   245,   211,
   354,   304,   244,   209,   212,   210,   326,   303,   199,   198,
   389,   292,   170,   171,   172,   173,   174,   175,    73,   383,
   223,    35,   254,   255,   258,   259,   260,   225,   340,   256,
   257,   315,   329,   317,   253,   314,   261,   220,    13,   269,
   375,   353,   319,   252,   187,   251,   228,   206,    35,   285,
    64,    71,   392,   382,   363,   364,   339,   366,   243,   245,
   350,   342,   355,   244,   365,   214,   215,   216,   341,   330,
    13,   286,   368,   372,   324,    59,    36,   134,   378,   377,
   374,   129,   323,   376,   338,   370,   322,   318,    97,    99,
   311,   134,   385,   282,   266,   180,   122,    38,   310,   281,
   306,   249,   152,   219,   297,   140,    12,    86,   218,   139,
    85,   178,   170,   171,   172,   173,   174,   175,    73,    11,
   120,   203,   138,   309,   274,   273,   229,   184,   126,    42,
   380,   381,   362,   298,   361,   359,   379,   358,    12,   357,
   356,   343,   336,   320,   308,   272,   183,   125,    41,   271,
   307,    11,   270,   268,   224,   182,   124,    40,   337,   296,
   247,   299,   109,    53,   295,    33,   264,   179,   121,    37,
   136,   111,   246,   238,   237,   191,   236,   190,   235,   234,
   233,   186,   107,    46,   185,   149,   127,    43,    18,    17,
   267,   181,   123,    39,    32,    31,     8,    30,     7,    29,
    28,    27,     3,     1,    24,   193,   133,   194,   332,    75,
    74,    68,    92,   369,   168,   156,   287,    23,   321,   316,
   151,   360,   345,   248,   200,   327,    44,   150,   390,   387,
   351,    84,   240,   135,   294,    69,    58,    56 };
static const tao_yytabelem tao_yypact[]={

   -18,-10000000,-10000000,   -18,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,  -220,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,   224,-10000000,-10000000,   219,   218,   216,
   214,   211,   210,  -194,-10000000,-10000000,-10000000,   -71,  -220,  -168,
  -220,  -220,  -220,    88,-10000000,-10000000,  -220,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -282,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -231,-10000000,
-10000000,-10000000,-10000000,-10000000,   177,   170,   166,-10000000,  -230,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
  -282,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -219,-10000000,  -220,
-10000000,  -220,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -226,-10000000,
   163,  -220,    86,    81,    80,  -248,    64,-10000000,-10000000,  -282,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -220,-10000000,   109,   109,
   109,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,   -48,
   255,   254,   125,-10000000,-10000000,-10000000,    40,   118,   299,  -287,
   250,   247,-10000000,-10000000,     9,     9,     9,  -282,-10000000,   109,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -170,   161,
-10000000,   -18,  -211,   296,  -172,    38,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,  -258,  -124,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,   158,   109,   109,   109,   109,   109,   109,
   109,   109,   109,   109,-10000000,-10000000,-10000000,   285,   156,   155,
-10000000,-10000000,  -211,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,   209,   208,   206,   205,   203,   199,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,  -282,  -219,  -220,   125,   109,
-10000000,   118,   299,  -287,   250,   250,   247,   247,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,   109,-10000000,    36,    33,    32,  -211,
  -220,   197,  -144,    31,   253,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,   -97,  -171,  -282,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
  -282,  -227,-10000000,-10000000,-10000000,-10000000,   122,   186,   284,  -172,
  -220,   292,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
    60,   252,  -266,   281,  -267,-10000000,-10000000,-10000000,-10000000,-10000000,
   277,-10000000,-10000000,-10000000,-10000000,-10000000,  -165,  -220,  -268,   291,
-10000000,   246,   -97,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,    26,  -165,  -194,   174,
  -252,   223,   109,   290,  -219,  -267,  -220,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,   268,-10000000,-10000000,   173,
  -220,   222,  -275,-10000000,-10000000,-10000000,-10000000,   259,-10000000,-10000000,
   239,-10000000,  -276,-10000000 };
static const tao_yytabelem tao_yypgo[]={

     0,    16,   148,   527,   526,   525,    10,   259,   178,   524,
   246,   523,   522,   521,   341,     9,     6,   520,   519,   518,
     8,   517,   516,   515,   514,   513,   512,   511,    30,   510,
   509,   508,   507,   506,     7,   505,   130,   127,   117,   104,
   109,   102,   190,   504,   111,    90,     2,   503,   502,   181,
   176,   150,   340,   501,   500,   499,     5,   121,   498,   497,
     1,    12,   496,   495,   494,     0,   184,   493,   128,   492,
   491,   136,   490,   135,   489,   488,   487,   486,   485,   484,
   483,   482,   481,   480,   479,   478,   477,   476,   475,   474,
   473,   472,   471,   470,   469,   468,   467,   466,   465,   464,
   463,   462,   461,   460,   459,   458,   457,   456,   455,   453,
   452,   450,   448,   447,   446,   445,   444,   443,   134,   103,
   442,   440,   439,   438,   437,   436,   435,   434,   433,   432,
   431,   430,     4,   429,   427,   426,   425,   424,   422,   421,
   420,   419,   418,   417,   416,   415,    11,   414,   413,   412,
   411,   410,   401,   400,   399,   398,   397,   395,   393,   392,
   391,   390,   389,   388,   387,   386,   385,   384,   383,   380,
   377,   376,   374,   372,   364,   359,     3,   358,   354,   351,
   347,   346,   345,   344,   343,   342 };
static const tao_yytabelem tao_yyr1[]={

     0,    67,    68,    68,    70,    69,    72,    69,    74,    69,
    76,    69,    78,    69,    79,    69,    80,    81,    82,    83,
    77,    75,    75,    86,    87,    89,    84,    90,    64,    32,
    91,    22,    22,    88,    88,    93,    92,    94,    92,    95,
    92,    97,    92,    99,    92,   100,    92,    20,   101,    21,
    21,    16,   102,    16,   103,    16,    65,    85,   104,   105,
   106,   107,    71,    48,    48,    48,    48,    48,    48,    48,
    48,    33,    34,    35,    35,    36,    36,    37,    37,    38,
    38,    38,    39,    39,    39,    40,    40,    40,    40,    41,
    41,    41,    41,    42,    42,    42,    43,    43,    43,    43,
    43,    43,    44,   108,    66,    66,    66,    66,    66,   110,
   109,     1,     1,     2,     2,     2,    56,    56,    56,    56,
    56,    56,     4,     4,     4,     3,     3,     3,    28,   111,
    29,    29,    60,    60,    30,   112,    31,    31,    61,    62,
    49,    49,    54,    54,    54,    55,    55,    55,    52,    52,
    52,    50,    50,    57,    51,    53,   113,   114,   115,   117,
     7,   116,   119,   119,   120,   121,   118,   122,   118,   123,
   124,   125,   126,   127,   128,   129,   131,    10,     9,     9,
     9,     9,     9,     9,   130,   133,   133,   134,   135,   132,
   136,   132,    26,    27,    27,   137,    46,   138,   139,    46,
   140,    47,   141,   142,   143,   145,     8,   144,   148,   147,
   147,   146,   149,   150,     5,     5,   151,   152,    13,   154,
   155,     6,     6,   153,   157,   158,    14,    14,   156,   159,
    11,    24,    25,    25,   160,   161,    45,   162,   163,    96,
    63,    63,   164,   165,   166,   167,    73,   168,   169,   171,
   172,    98,    59,    59,    59,    12,    12,   173,   170,   174,
   170,   175,   178,   177,   177,   179,   180,   176,    15,    15,
    15,    58,    58,    58,   181,   182,    23,    23,   183,   184,
    17,    17,    18,   185,    19,    19 };
static const tao_yytabelem tao_yyr2[]={

     0,     2,     4,     0,     1,     7,     1,     7,     1,     7,
     1,     7,     1,     7,     1,     7,     1,     1,     1,     1,
    19,     2,     2,     1,     1,     1,    15,     1,     7,     5,
     1,     7,     1,     4,     0,     1,     7,     1,     7,     1,
     7,     1,     7,     1,     7,     1,     7,     5,     1,     9,
     1,     3,     1,     7,     1,     9,     3,     3,     1,     1,
     1,     1,    19,     2,     2,     2,     2,     2,     3,     3,
     3,     2,     2,     2,     7,     2,     7,     2,     7,     2,
     7,     7,     2,     7,     7,     2,     7,     7,     7,     2,
     5,     5,     5,     3,     2,     7,     3,     3,     3,     3,
     3,     3,     3,     1,     7,     3,     3,     3,     5,     1,
     7,     2,     2,     3,     2,     3,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     5,     1,
     9,     1,     2,     2,     5,     1,     9,     1,     3,     3,
     2,     2,     3,     5,     3,     5,     7,     5,     3,     3,
     5,     3,     3,     3,     3,     3,     1,     1,     1,     1,
    19,     4,     4,     0,     1,     1,    11,     1,     7,     1,
     1,     1,     1,     1,     1,     1,     1,    35,     3,     3,
     3,     3,     2,     3,     4,     4,     0,     1,     1,    11,
     1,     7,     5,     5,     1,     1,     7,     1,     1,    11,
     1,     7,     1,     1,     1,     1,    19,     4,     1,     8,
     0,     3,     1,     1,    13,     5,     1,     1,    11,     1,
     1,    13,     3,     3,     1,     1,    13,     3,     3,     1,
     7,     5,     5,     1,     1,     1,    11,     1,     1,    13,
     3,     1,     1,     1,     1,     1,    19,     1,     1,     1,
     1,    21,     3,     3,     1,     2,     3,     1,     7,     1,
     9,     4,     1,     8,     0,     1,     1,    11,     3,     2,
     3,     3,     3,     3,     1,     1,    13,     1,     1,     1,
    13,     1,     5,     1,     9,     1 };
static const tao_yytabelem tao_yychk[]={

-10000000,   -67,   -68,   -69,   -66,   -71,   -73,   -75,   -77,   256,
   261,    -7,   -10,    -8,   292,   258,   279,   -84,   -85,   259,
   272,   273,   275,   -32,   -64,   260,   -68,   -70,   -72,   -74,
   -76,   -78,   -79,  -108,   -61,   -65,   257,  -104,  -164,   -80,
  -113,  -123,  -141,   -86,   -22,    58,   -90,    59,    59,    59,
    59,    59,    59,  -109,    -1,    -2,    -3,   -56,    -4,   -16,
    -7,   -10,    -8,   -49,   -52,   -50,   -51,   -57,   -53,    -5,
    -6,   -14,   -65,   299,   -54,   -55,   265,   266,   262,   267,
   268,   270,   269,   271,   -13,  -153,  -156,   263,   264,   276,
   277,   278,   -48,   -49,   -50,   -57,   -51,   -52,    -6,   -14,
   -16,   -65,   257,   -65,   -65,   -65,   123,   -91,   -65,  -110,
   299,  -102,   262,   265,    44,    62,    60,    60,   262,   263,
  -151,  -105,  -165,   -81,  -114,  -124,  -142,   -87,   -20,   -16,
   -28,   -60,   -61,   -62,   -65,   -11,  -103,   -65,  -149,  -154,
  -157,   262,    60,   -65,   123,   123,   123,   274,   123,   -88,
   -21,   -29,  -159,   -65,   -44,   -34,   -35,   -36,   -37,   -38,
   -39,   -40,   -41,   -42,    43,    45,   126,   -16,   -43,    40,
   293,   294,   295,   296,   297,   298,   -44,   -44,  -152,  -106,
  -166,   -82,  -115,  -125,  -143,   -89,   -92,   -66,   -71,   -73,
   -96,   -98,   256,   -63,   -59,   282,   284,   285,    44,    44,
   -24,   -45,    91,  -150,   124,    94,    38,   300,   301,    43,
    45,    42,    47,    37,   -42,   -42,   -42,   -34,  -155,  -158,
    -2,    61,  -119,   -68,  -116,  -118,    -1,   256,    40,  -144,
  -146,   257,   125,   -93,   -94,   -95,   -97,   -99,  -100,   283,
   -12,   -15,   286,   -56,    -6,   -16,  -101,  -111,   -25,  -160,
    62,   -36,   -37,   -38,   -39,   -39,   -40,   -40,   -41,   -41,
   -41,    41,    62,    62,  -107,  -118,  -167,   -83,  -117,  -119,
  -120,  -122,  -126,  -145,  -147,    59,    59,    59,    59,    59,
    59,  -162,  -168,   -16,   -60,   -45,   -44,   -33,   -34,   125,
   125,   125,   -28,    59,    -9,   -49,   -50,   -57,   -51,    -8,
   -16,   262,   125,    44,   -15,   257,  -161,  -121,  -127,  -148,
  -163,  -169,    93,    59,    41,  -146,   -30,   -61,  -170,    40,
  -128,   -31,  -171,  -173,  -174,   123,    44,   -23,   290,    41,
  -175,  -176,   -58,   287,   288,   289,  -129,  -112,  -172,  -181,
    41,  -177,  -179,  -130,  -132,   -26,   256,   -46,   281,   280,
   -61,   -17,   291,    40,    44,   -15,  -131,  -133,  -134,  -136,
   -27,  -137,  -138,  -183,  -182,  -178,  -180,   125,  -132,   -47,
    -1,    59,   -46,    58,   -34,    40,   -20,  -176,   -60,  -135,
  -140,  -139,  -184,    41,    59,   -60,    58,   -18,   294,    41,
   -19,    44,  -185,   294 };
static const tao_yytabelem tao_yydef[]={

    -2,    -2,     1,    -2,     4,     6,     8,    10,    12,    14,
   103,   105,   106,   107,     0,    58,   242,    21,    22,    16,
   156,   169,   202,    23,    -2,    27,     2,     0,     0,     0,
     0,     0,     0,     0,   108,   138,    56,     0,     0,     0,
     0,     0,     0,     0,    29,    30,     0,     5,     7,     9,
    11,    13,    15,   104,   109,   111,   112,   113,   114,   115,
   125,   126,   127,   116,   117,   118,   119,   120,   121,   122,
   123,   124,    51,    52,   140,   141,   148,   149,   142,   151,
   152,   154,   153,   155,     0,   222,   227,   144,     0,   216,
   223,   228,    59,    63,    64,    65,    66,    67,    68,    69,
    70,   243,    17,   157,   170,   203,    24,     0,    28,     0,
    54,     0,   143,   150,   212,   215,   219,   224,   145,   147,
     0,     0,     0,     0,     0,     0,     0,    34,    31,    50,
   110,   131,   132,   133,    -2,   139,     0,    53,     0,     0,
     0,   146,   217,    60,   244,    18,   158,   171,   204,    -2,
    47,   128,     0,    55,   213,   102,    72,    73,    75,    77,
    79,    82,    85,    89,     0,     0,     0,    93,    94,     0,
    96,    97,    98,    99,   100,   101,   220,   225,     0,     0,
   163,    -2,     0,     0,     0,     0,    33,    35,    37,    39,
    41,    43,    45,     0,     0,   240,   252,   253,    48,   129,
   230,   233,   234,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    90,    91,    92,     0,     0,     0,
   218,    61,    -2,    19,   159,   163,   164,   167,   172,   205,
   210,   211,    26,     0,     0,     0,     0,     0,     0,   237,
   247,   255,   256,   268,   269,   270,     0,     0,   231,     0,
   214,    74,    76,    78,    80,    81,    83,    84,    86,    87,
    88,    95,   221,   226,     0,   162,     0,     0,     0,    -2,
     0,     0,     0,     0,   207,    36,    38,    40,    42,    44,
    46,     0,     0,    49,   130,   232,   235,    62,    71,   246,
    20,   160,   165,   168,   173,   178,   179,   180,   181,   182,
   183,   142,   206,   208,   238,   248,     0,     0,     0,     0,
     0,     0,   236,   166,   174,   209,   239,   137,   249,    -2,
     0,   134,   277,     0,     0,   175,   135,   250,   274,   258,
     0,   264,   265,   271,   272,   273,     0,     0,   281,     0,
   260,   261,     0,   176,   186,   187,   190,   194,   195,   197,
   136,   251,   278,   275,   262,   266,     0,    -2,     0,     0,
   192,     0,     0,     0,     0,     0,     0,   177,   185,   188,
   200,   191,   193,   196,   198,   279,     0,   263,   267,     0,
     0,     0,     0,   276,   189,   201,   199,     0,   285,   280,
   282,   283,     0,   284 };
typedef struct
#ifdef __cplusplus
	tao_yytoktype
#endif
{ char *t_name; int t_val; } tao_yytoktype;
#ifndef TAO_YYDEBUG
#	define TAO_YYDEBUG	0	/* don't allow debugging */
#endif

#if TAO_YYDEBUG

tao_yytoktype tao_yytoks[] =
{
	{"IDENTIFIER",	257},
	{"IDL_CONST",	258},
	{"IDL_MODULE",	259},
	{"IDL_INTERFACE",	260},
	{"IDL_TYPEDEF",	261},
	{"IDL_LONG",	262},
	{"IDL_SHORT",	263},
	{"IDL_UNSIGNED",	264},
	{"IDL_DOUBLE",	265},
	{"IDL_FLOAT",	266},
	{"IDL_CHAR",	267},
	{"IDL_WCHAR",	268},
	{"IDL_OCTET",	269},
	{"IDL_BOOLEAN",	270},
	{"IDL_ANY",	271},
	{"IDL_STRUCT",	272},
	{"IDL_UNION",	273},
	{"IDL_SWITCH",	274},
	{"IDL_ENUM",	275},
	{"IDL_SEQUENCE",	276},
	{"IDL_STRING",	277},
	{"IDL_WSTRING",	278},
	{"IDL_EXCEPTION",	279},
	{"IDL_CASE",	280},
	{"IDL_DEFAULT",	281},
	{"IDL_READONLY",	282},
	{"IDL_ATTRIBUTE",	283},
	{"IDL_ONEWAY",	284},
	{"IDL_IDEMPOTENT",	285},
	{"IDL_VOID",	286},
	{"IDL_IN",	287},
	{"IDL_OUT",	288},
	{"IDL_INOUT",	289},
	{"IDL_RAISES",	290},
	{"IDL_CONTEXT",	291},
	{"IDL_NATIVE",	292},
	{"IDL_INTEGER_LITERAL",	293},
	{"IDL_STRING_LITERAL",	294},
	{"IDL_CHARACTER_LITERAL",	295},
	{"IDL_FLOATING_PT_LITERAL",	296},
	{"IDL_TRUETOK",	297},
	{"IDL_FALSETOK",	298},
	{"IDL_SCOPE_DELIMITOR",	299},
	{"IDL_LEFT_SHIFT",	300},
	{"IDL_RIGHT_SHIFT",	301},
	{"-unknown-",	-1}	/* ends search */
};

char * tao_yyreds[] =
{
	"-no such reduction-",
	"start : definitions",
	"definitions : definition definitions",
	"definitions : /* empty */",
	"definition : type_dcl",
	"definition : type_dcl ';'",
	"definition : const_dcl",
	"definition : const_dcl ';'",
	"definition : exception",
	"definition : exception ';'",
	"definition : interface_def",
	"definition : interface_def ';'",
	"definition : module",
	"definition : module ';'",
	"definition : error",
	"definition : error ';'",
	"module : IDL_MODULE",
	"module : IDL_MODULE IDENTIFIER",
	"module : IDL_MODULE IDENTIFIER '{'",
	"module : IDL_MODULE IDENTIFIER '{' definitions",
	"module : IDL_MODULE IDENTIFIER '{' definitions '}'",
	"interface_def : interface",
	"interface_def : forward",
	"interface : interface_header",
	"interface : interface_header '{'",
	"interface : interface_header '{' exports",
	"interface : interface_header '{' exports '}'",
	"interface_decl : IDL_INTERFACE",
	"interface_decl : IDL_INTERFACE id",
	"interface_header : interface_decl inheritance_spec",
	"inheritance_spec : ':'",
	"inheritance_spec : ':' at_least_one_scoped_name",
	"inheritance_spec : /* empty */",
	"exports : exports export",
	"exports : /* empty */",
	"export : type_dcl",
	"export : type_dcl ';'",
	"export : const_dcl",
	"export : const_dcl ';'",
	"export : exception",
	"export : exception ';'",
	"export : attribute",
	"export : attribute ';'",
	"export : operation",
	"export : operation ';'",
	"export : error",
	"export : error ';'",
	"at_least_one_scoped_name : scoped_name scoped_names",
	"scoped_names : scoped_names ','",
	"scoped_names : scoped_names ',' scoped_name",
	"scoped_names : /* empty */",
	"scoped_name : id",
	"scoped_name : IDL_SCOPE_DELIMITOR",
	"scoped_name : IDL_SCOPE_DELIMITOR id",
	"scoped_name : scoped_name IDL_SCOPE_DELIMITOR",
	"scoped_name : scoped_name IDL_SCOPE_DELIMITOR id",
	"id : IDENTIFIER",
	"forward : interface_decl",
	"const_dcl : IDL_CONST",
	"const_dcl : IDL_CONST const_type",
	"const_dcl : IDL_CONST const_type id",
	"const_dcl : IDL_CONST const_type id '='",
	"const_dcl : IDL_CONST const_type id '=' expression",
	"const_type : integer_type",
	"const_type : char_type",
	"const_type : octet_type",
	"const_type : boolean_type",
	"const_type : floating_pt_type",
	"const_type : string_type_spec",
	"const_type : wstring_type_spec",
	"const_type : scoped_name",
	"expression : const_expr",
	"const_expr : or_expr",
	"or_expr : xor_expr",
	"or_expr : or_expr '|' xor_expr",
	"xor_expr : and_expr",
	"xor_expr : xor_expr '^' and_expr",
	"and_expr : shift_expr",
	"and_expr : and_expr '&' shift_expr",
	"shift_expr : add_expr",
	"shift_expr : shift_expr IDL_LEFT_SHIFT add_expr",
	"shift_expr : shift_expr IDL_RIGHT_SHIFT add_expr",
	"add_expr : mult_expr",
	"add_expr : add_expr '+' mult_expr",
	"add_expr : add_expr '-' mult_expr",
	"mult_expr : unary_expr",
	"mult_expr : mult_expr '*' unary_expr",
	"mult_expr : mult_expr '/' unary_expr",
	"mult_expr : mult_expr '%' unary_expr",
	"unary_expr : primary_expr",
	"unary_expr : '+' primary_expr",
	"unary_expr : '-' primary_expr",
	"unary_expr : '~' primary_expr",
	"primary_expr : scoped_name",
	"primary_expr : literal",
	"primary_expr : '(' const_expr ')'",
	"literal : IDL_INTEGER_LITERAL",
	"literal : IDL_STRING_LITERAL",
	"literal : IDL_CHARACTER_LITERAL",
	"literal : IDL_FLOATING_PT_LITERAL",
	"literal : IDL_TRUETOK",
	"literal : IDL_FALSETOK",
	"positive_int_expr : const_expr",
	"type_dcl : IDL_TYPEDEF",
	"type_dcl : IDL_TYPEDEF type_declarator",
	"type_dcl : struct_type",
	"type_dcl : union_type",
	"type_dcl : enum_type",
	"type_dcl : IDL_NATIVE simple_declarator",
	"type_declarator : type_spec",
	"type_declarator : type_spec at_least_one_declarator",
	"type_spec : simple_type_spec",
	"type_spec : constructed_type_spec",
	"simple_type_spec : base_type_spec",
	"simple_type_spec : template_type_spec",
	"simple_type_spec : scoped_name",
	"base_type_spec : integer_type",
	"base_type_spec : floating_pt_type",
	"base_type_spec : char_type",
	"base_type_spec : boolean_type",
	"base_type_spec : octet_type",
	"base_type_spec : any_type",
	"template_type_spec : sequence_type_spec",
	"template_type_spec : string_type_spec",
	"template_type_spec : wstring_type_spec",
	"constructed_type_spec : struct_type",
	"constructed_type_spec : union_type",
	"constructed_type_spec : enum_type",
	"at_least_one_declarator : declarator declarators",
	"declarators : declarators ','",
	"declarators : declarators ',' declarator",
	"declarators : /* empty */",
	"declarator : simple_declarator",
	"declarator : complex_declarator",
	"at_least_one_simple_declarator : simple_declarator simple_declarators",
	"simple_declarators : simple_declarators ','",
	"simple_declarators : simple_declarators ',' simple_declarator",
	"simple_declarators : /* empty */",
	"simple_declarator : id",
	"complex_declarator : array_declarator",
	"integer_type : signed_int",
	"integer_type : unsigned_int",
	"signed_int : IDL_LONG",
	"signed_int : IDL_LONG IDL_LONG",
	"signed_int : IDL_SHORT",
	"unsigned_int : IDL_UNSIGNED IDL_LONG",
	"unsigned_int : IDL_UNSIGNED IDL_LONG IDL_LONG",
	"unsigned_int : IDL_UNSIGNED IDL_SHORT",
	"floating_pt_type : IDL_DOUBLE",
	"floating_pt_type : IDL_FLOAT",
	"floating_pt_type : IDL_LONG IDL_DOUBLE",
	"char_type : IDL_CHAR",
	"char_type : IDL_WCHAR",
	"octet_type : IDL_OCTET",
	"boolean_type : IDL_BOOLEAN",
	"any_type : IDL_ANY",
	"struct_type : IDL_STRUCT",
	"struct_type : IDL_STRUCT id",
	"struct_type : IDL_STRUCT id '{'",
	"struct_type : IDL_STRUCT id '{' at_least_one_member",
	"struct_type : IDL_STRUCT id '{' at_least_one_member '}'",
	"at_least_one_member : member members",
	"members : members member",
	"members : /* empty */",
	"member : type_spec",
	"member : type_spec at_least_one_declarator",
	"member : type_spec at_least_one_declarator ';'",
	"member : error",
	"member : error ';'",
	"union_type : IDL_UNION",
	"union_type : IDL_UNION id",
	"union_type : IDL_UNION id IDL_SWITCH",
	"union_type : IDL_UNION id IDL_SWITCH '('",
	"union_type : IDL_UNION id IDL_SWITCH '(' switch_type_spec",
	"union_type : IDL_UNION id IDL_SWITCH '(' switch_type_spec ')'",
	"union_type : IDL_UNION id IDL_SWITCH '(' switch_type_spec ')' '{'",
	"union_type : IDL_UNION id IDL_SWITCH '(' switch_type_spec ')' '{' at_least_one_case_branch",
	"union_type : IDL_UNION id IDL_SWITCH '(' switch_type_spec ')' '{' at_least_one_case_branch '}'",
	"switch_type_spec : integer_type",
	"switch_type_spec : char_type",
	"switch_type_spec : octet_type",
	"switch_type_spec : boolean_type",
	"switch_type_spec : enum_type",
	"switch_type_spec : scoped_name",
	"at_least_one_case_branch : case_branch case_branches",
	"case_branches : case_branches case_branch",
	"case_branches : /* empty */",
	"case_branch : at_least_one_case_label",
	"case_branch : at_least_one_case_label element_spec",
	"case_branch : at_least_one_case_label element_spec ';'",
	"case_branch : error",
	"case_branch : error ';'",
	"at_least_one_case_label : case_label case_labels",
	"case_labels : case_labels case_label",
	"case_labels : /* empty */",
	"case_label : IDL_DEFAULT",
	"case_label : IDL_DEFAULT ':'",
	"case_label : IDL_CASE",
	"case_label : IDL_CASE const_expr",
	"case_label : IDL_CASE const_expr ':'",
	"element_spec : type_spec",
	"element_spec : type_spec declarator",
	"enum_type : IDL_ENUM",
	"enum_type : IDL_ENUM id",
	"enum_type : IDL_ENUM id '{'",
	"enum_type : IDL_ENUM id '{' at_least_one_enumerator",
	"enum_type : IDL_ENUM id '{' at_least_one_enumerator '}'",
	"at_least_one_enumerator : enumerator enumerators",
	"enumerators : enumerators ','",
	"enumerators : enumerators ',' enumerator",
	"enumerators : /* empty */",
	"enumerator : IDENTIFIER",
	"sequence_type_spec : seq_head ','",
	"sequence_type_spec : seq_head ',' positive_int_expr",
	"sequence_type_spec : seq_head ',' positive_int_expr '>'",
	"sequence_type_spec : seq_head '>'",
	"seq_head : IDL_SEQUENCE",
	"seq_head : IDL_SEQUENCE '<'",
	"seq_head : IDL_SEQUENCE '<' simple_type_spec",
	"string_type_spec : string_head '<'",
	"string_type_spec : string_head '<' positive_int_expr",
	"string_type_spec : string_head '<' positive_int_expr '>'",
	"string_type_spec : string_head",
	"string_head : IDL_STRING",
	"wstring_type_spec : wstring_head '<'",
	"wstring_type_spec : wstring_head '<' positive_int_expr",
	"wstring_type_spec : wstring_head '<' positive_int_expr '>'",
	"wstring_type_spec : wstring_head",
	"wstring_head : IDL_WSTRING",
	"array_declarator : id",
	"array_declarator : id at_least_one_array_dim",
	"at_least_one_array_dim : array_dim array_dims",
	"array_dims : array_dims array_dim",
	"array_dims : /* empty */",
	"array_dim : '['",
	"array_dim : '[' positive_int_expr",
	"array_dim : '[' positive_int_expr ']'",
	"attribute : opt_readonly IDL_ATTRIBUTE",
	"attribute : opt_readonly IDL_ATTRIBUTE param_type_spec",
	"attribute : opt_readonly IDL_ATTRIBUTE param_type_spec at_least_one_simple_declarator",
	"opt_readonly : IDL_READONLY",
	"opt_readonly : /* empty */",
	"exception : IDL_EXCEPTION",
	"exception : IDL_EXCEPTION id",
	"exception : IDL_EXCEPTION id '{'",
	"exception : IDL_EXCEPTION id '{' members",
	"exception : IDL_EXCEPTION id '{' members '}'",
	"operation : opt_op_attribute op_type_spec",
	"operation : opt_op_attribute op_type_spec IDENTIFIER",
	"operation : opt_op_attribute op_type_spec IDENTIFIER parameter_list",
	"operation : opt_op_attribute op_type_spec IDENTIFIER parameter_list opt_raises",
	"operation : opt_op_attribute op_type_spec IDENTIFIER parameter_list opt_raises opt_context",
	"opt_op_attribute : IDL_ONEWAY",
	"opt_op_attribute : IDL_IDEMPOTENT",
	"opt_op_attribute : /* empty */",
	"op_type_spec : param_type_spec",
	"op_type_spec : IDL_VOID",
	"parameter_list : '('",
	"parameter_list : '(' ')'",
	"parameter_list : '('",
	"parameter_list : '(' at_least_one_parameter ')'",
	"at_least_one_parameter : parameter parameters",
	"parameters : parameters ','",
	"parameters : parameters ',' parameter",
	"parameters : /* empty */",
	"parameter : direction",
	"parameter : direction param_type_spec",
	"parameter : direction param_type_spec declarator",
	"param_type_spec : base_type_spec",
	"param_type_spec : string_type_spec",
	"param_type_spec : scoped_name",
	"direction : IDL_IN",
	"direction : IDL_OUT",
	"direction : IDL_INOUT",
	"opt_raises : IDL_RAISES",
	"opt_raises : IDL_RAISES '('",
	"opt_raises : IDL_RAISES '(' at_least_one_scoped_name ')'",
	"opt_raises : /* empty */",
	"opt_context : IDL_CONTEXT",
	"opt_context : IDL_CONTEXT '('",
	"opt_context : IDL_CONTEXT '(' at_least_one_string_literal ')'",
	"opt_context : /* empty */",
	"at_least_one_string_literal : IDL_STRING_LITERAL string_literals",
	"string_literals : string_literals ','",
	"string_literals : string_literals ',' IDL_STRING_LITERAL",
	"string_literals : /* empty */",
};
#endif /* TAO_YYDEBUG */
# line	1 "/usr/ccs/bin/yaccpar"
/*
 * Copyright (c) 1993 by Sun Microsystems, Inc.
 */

// #pragma ident	"@(#)yaccpar	6.14	97/01/16 SMI"

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define TAO_YYERROR		goto tao_yyerrlab
#define TAO_YYACCEPT	return(0)
#define TAO_YYABORT		return(1)
#define TAO_YYBACKUP( newtoken, newvalue )\
{\
	if ( tao_yychar >= 0 || ( tao_yyr2[ tao_yytmp ] >> 1 ) != 1 )\
	{\
		tao_yyerror( "syntax error - cannot backup" );\
		goto tao_yyerrlab;\
	}\
	tao_yychar = newtoken;\
	tao_yystate = *tao_yyps;\
	tao_yylval = newvalue;\
	goto tao_yynewstate;\
}
#define TAO_YYRECOVERING()	(!!tao_yyerrflag)
#define TAO_YYNEW(type)	malloc(sizeof(type) * tao_yynewmax)
#define TAO_YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, tao_yymaxdepth * sizeof (type))
#define TAO_YYENLARGE( from, type) \
	(type *) realloc((char *) from, tao_yynewmax * sizeof(type))
#ifndef TAO_YYDEBUG
#	define TAO_YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int tao_yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define TAO_YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
TAO_YYSTYPE *tao_yypv;			/* top of value stack */
int *tao_yyps;			/* top of state stack */

int tao_yystate;			/* current state */
int tao_yytmp;			/* extra var (lasts between blocks) */

int tao_yynerrs;			/* number of errors */
int tao_yyerrflag;			/* error recovery flag */
int tao_yychar;			/* current input token number */



#ifdef TAO_YYNMBCHARS
#define TAO_YYLEX()		tao_yycvtok(tao_yylex())
/*
** tao_yycvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
int tao_yycvtok(int i)
#else
int tao_yycvtok(i) int i;
#endif
{
	int first = 0;
	int last = TAO_YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if(i&0x60000000){/*Must convert to a token. */
		if( tao_yymbchars[last].character < i ){
			return i;/*Giving up*/
		}
		while ((last>=first)&&(first>=0)) {/*Binary search loop*/
			mid = (first+last)/2;
			j = tao_yymbchars[mid].character;
			if( j==i ){/*Found*/
				return tao_yymbchars[mid].tvalue;
			}else if( j<i ){
				first = mid + 1;
			}else{
				last = mid -1;
			}
		}
		/*No entry in the table.*/
		return i;/* Giving up.*/
	}else{/* i is already a token. */
		return i;
	}
}
#else/*!TAO_YYNMBCHARS*/
#define TAO_YYLEX()		tao_yylex()
#endif/*!TAO_YYNMBCHARS*/

/*
** tao_yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int tao_yyparse(void)
#else
int tao_yyparse()
#endif
{
	register TAO_YYSTYPE *tao_yypvt = 0;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside
	switch should never be executed
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto tao_yyerrlab;
		case 2: goto tao_yynewstate;
	}
#endif

	/*
	** Initialize externals - tao_yyparse may be called more than once
	*/
	tao_yypv = &tao_yyv[-1];
	tao_yyps = &tao_yys[-1];
	tao_yystate = 0;
	tao_yytmp = 0;
	tao_yynerrs = 0;
	tao_yyerrflag = 0;
	tao_yychar = -1;

#if TAO_YYMAXDEPTH <= 0
	if (tao_yymaxdepth <= 0)
	{
		if ((tao_yymaxdepth = TAO_YYEXPAND(0)) <= 0)
		{
			tao_yyerror("yacc initialization error");
			TAO_YYABORT;
		}
	}
#endif

	{
		register TAO_YYSTYPE *tao_yy_pv;	/* top of value stack */
		register int *tao_yy_ps;		/* top of state stack */
		register int tao_yy_state;		/* current state */
		register int  tao_yy_n;		/* internal state number info */
	goto tao_yystack;	/* moved from 6 lines above to here to please C++ */

		/*
		** get globals into registers.
		** branch to here only if TAO_YYBACKUP was called.
		*/
	tao_yynewstate:
		tao_yy_pv = tao_yypv;
		tao_yy_ps = tao_yyps;
		tao_yy_state = tao_yystate;
		goto tao_yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	tao_yystack:
		tao_yy_pv = tao_yypv;
		tao_yy_ps = tao_yyps;
		tao_yy_state = tao_yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	tao_yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if TAO_YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( tao_yydebug )
		{
			register int tao_yy_i;

			printf( "State %d, token ", tao_yy_state );
			if ( tao_yychar == 0 )
				printf( "end-of-file\n" );
			else if ( tao_yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( tao_yy_i = 0; tao_yytoks[tao_yy_i].t_val >= 0;
					tao_yy_i++ )
				{
					if ( tao_yytoks[tao_yy_i].t_val == tao_yychar )
						break;
				}
				printf( "%s\n", tao_yytoks[tao_yy_i].t_name );
			}
		}
#endif /* TAO_YYDEBUG */
		if ( ++tao_yy_ps >= &tao_yys[ tao_yymaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			int tao_yyps_index = (tao_yy_ps - tao_yys);
			int tao_yypv_index = (tao_yy_pv - tao_yyv);
			int tao_yypvt_index = (tao_yypvt - tao_yyv);
			int tao_yynewmax;
#ifdef TAO_YYEXPAND
			tao_yynewmax = TAO_YYEXPAND(tao_yymaxdepth);
#else
			tao_yynewmax = 2 * tao_yymaxdepth;	/* double table size */
			if (tao_yymaxdepth == TAO_YYMAXDEPTH)	/* first time growth */
			{
				char *newtao_yys = (char *)TAO_YYNEW(int);
				char *newtao_yyv = (char *)TAO_YYNEW(TAO_YYSTYPE);
				if (newtao_yys != 0 && newtao_yyv != 0)
				{
					tao_yys = TAO_YYCOPY(newtao_yys, tao_yys, int);
					tao_yyv = TAO_YYCOPY(newtao_yyv, tao_yyv, TAO_YYSTYPE);
				}
				else
					tao_yynewmax = 0;	/* failed */
			}
			else				/* not first time */
			{
				tao_yys = TAO_YYENLARGE(tao_yys, int);
				tao_yyv = TAO_YYENLARGE(tao_yyv, TAO_YYSTYPE);
				if (tao_yys == 0 || tao_yyv == 0)
					tao_yynewmax = 0;	/* failed */
			}
#endif
			if (tao_yynewmax <= tao_yymaxdepth)	/* tables not expanded */
			{
				tao_yyerror( "yacc stack overflow" );
				TAO_YYABORT;
			}
			tao_yymaxdepth = tao_yynewmax;

			tao_yy_ps = tao_yys + tao_yyps_index;
			tao_yy_pv = tao_yyv + tao_yypv_index;
			tao_yypvt = tao_yyv + tao_yypvt_index;
		}
		*tao_yy_ps = tao_yy_state;
		*++tao_yy_pv = tao_yyval;

		/*
		** we have a new state - find out what to do
		*/
	tao_yy_newstate:
		if ( ( tao_yy_n = tao_yypact[ tao_yy_state ] ) <= TAO_YYFLAG )
			goto tao_yydefault;		/* simple state */
#if TAO_YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		tao_yytmp = tao_yychar < 0;
#endif
		if ( ( tao_yychar < 0 ) && ( ( tao_yychar = TAO_YYLEX() ) < 0 ) )
			tao_yychar = 0;		/* reached EOF */
#if TAO_YYDEBUG
		if ( tao_yydebug && tao_yytmp )
		{
			register int tao_yy_i;

			printf( "Received token " );
			if ( tao_yychar == 0 )
				printf( "end-of-file\n" );
			else if ( tao_yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( tao_yy_i = 0; tao_yytoks[tao_yy_i].t_val >= 0;
					tao_yy_i++ )
				{
					if ( tao_yytoks[tao_yy_i].t_val == tao_yychar )
						break;
				}
				printf( "%s\n", tao_yytoks[tao_yy_i].t_name );
			}
		}
#endif /* TAO_YYDEBUG */
		if ( ( ( tao_yy_n += tao_yychar ) < 0 ) || ( tao_yy_n >= TAO_YYLAST ) )
			goto tao_yydefault;
		if ( tao_yychk[ tao_yy_n = tao_yyact[ tao_yy_n ] ] == tao_yychar )	/*valid shift*/
		{
			tao_yychar = -1;
			tao_yyval = tao_yylval;
			tao_yy_state = tao_yy_n;
			if ( tao_yyerrflag > 0 )
				tao_yyerrflag--;
			goto tao_yy_stack;
		}

	tao_yydefault:
		if ( ( tao_yy_n = tao_yydef[ tao_yy_state ] ) == -2 )
		{
#if TAO_YYDEBUG
			tao_yytmp = tao_yychar < 0;
#endif
			if ( ( tao_yychar < 0 ) && ( ( tao_yychar = TAO_YYLEX() ) < 0 ) )
				tao_yychar = 0;		/* reached EOF */
#if TAO_YYDEBUG
			if ( tao_yydebug && tao_yytmp )
			{
				register int tao_yy_i;

				printf( "Received token " );
				if ( tao_yychar == 0 )
					printf( "end-of-file\n" );
				else if ( tao_yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( tao_yy_i = 0;
						tao_yytoks[tao_yy_i].t_val >= 0;
						tao_yy_i++ )
					{
						if ( tao_yytoks[tao_yy_i].t_val
							== tao_yychar )
						{
							break;
						}
					}
					printf( "%s\n", tao_yytoks[tao_yy_i].t_name );
				}
			}
#endif /* TAO_YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register const int *tao_yyxi = tao_yyexca;

				while ( ( *tao_yyxi != -1 ) ||
					( tao_yyxi[1] != tao_yy_state ) )
				{
					tao_yyxi += 2;
				}
				while ( ( *(tao_yyxi += 2) >= 0 ) &&
					( *tao_yyxi != tao_yychar ) )
					;
				if ( ( tao_yy_n = tao_yyxi[1] ) < 0 )
					TAO_YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( tao_yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( tao_yyerrflag )
			{
			case 0:		/* new error */
				tao_yyerror( "syntax error" );
				goto skip_init;
			tao_yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				tao_yy_pv = tao_yypv;
				tao_yy_ps = tao_yyps;
				tao_yy_state = tao_yystate;
			skip_init:
				tao_yynerrs++;
				/* FALLTHRU */
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				tao_yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( tao_yy_ps >= tao_yys )
				{
					tao_yy_n = tao_yypact[ *tao_yy_ps ] + TAO_YYERRCODE;
					if ( tao_yy_n >= 0 && tao_yy_n < TAO_YYLAST &&
						tao_yychk[tao_yyact[tao_yy_n]] == TAO_YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						tao_yy_state = tao_yyact[ tao_yy_n ];
						goto tao_yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if TAO_YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( tao_yydebug )
						printf( _POP_, *tao_yy_ps,
							tao_yy_ps[-1] );
#	undef _POP_
#endif
					tao_yy_ps--;
					tao_yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				TAO_YYABORT;
			case 3:		/* no shift yet; eat a token */
#if TAO_YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( tao_yydebug )
				{
					register int tao_yy_i;

					printf( "Error recovery discards " );
					if ( tao_yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( tao_yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( tao_yy_i = 0;
							tao_yytoks[tao_yy_i].t_val >= 0;
							tao_yy_i++ )
						{
							if ( tao_yytoks[tao_yy_i].t_val
								== tao_yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							tao_yytoks[tao_yy_i].t_name );
					}
				}
#endif /* TAO_YYDEBUG */
				if ( tao_yychar == 0 )	/* reached EOF. quit */
					TAO_YYABORT;
				tao_yychar = -1;
				goto tao_yy_newstate;
			}
		}/* end if ( tao_yy_n == 0 ) */
		/*
		** reduction by production tao_yy_n
		** put stack tops, etc. so things right after switch
		*/
#if TAO_YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( tao_yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				tao_yy_n, tao_yyreds[ tao_yy_n ] );
#endif
		tao_yytmp = tao_yy_n;			/* value to switch over */
		tao_yypvt = tao_yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using tao_yy_state here as temporary
		** register variable, but why not, if it works...
		** If tao_yyr2[ tao_yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto tao_yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int tao_yy_len = tao_yyr2[ tao_yy_n ];

			if ( !( tao_yy_len & 01 ) )
			{
				tao_yy_len >>= 1;
				tao_yyval = ( tao_yy_pv -= tao_yy_len )[1];	/* $$ = $1 */
				tao_yy_state = tao_yypgo[ tao_yy_n = tao_yyr1[ tao_yy_n ] ] +
					*( tao_yy_ps -= tao_yy_len ) + 1;
				if ( tao_yy_state >= TAO_YYLAST ||
					tao_yychk[ tao_yy_state =
					tao_yyact[ tao_yy_state ] ] != -tao_yy_n )
				{
					tao_yy_state = tao_yyact[ tao_yypgo[ tao_yy_n ] ];
				}
				goto tao_yy_stack;
			}
			tao_yy_len >>= 1;
			tao_yyval = ( tao_yy_pv -= tao_yy_len )[1];	/* $$ = $1 */
			tao_yy_state = tao_yypgo[ tao_yy_n = tao_yyr1[ tao_yy_n ] ] +
				*( tao_yy_ps -= tao_yy_len ) + 1;
			if ( tao_yy_state >= TAO_YYLAST ||
				tao_yychk[ tao_yy_state = tao_yyact[ tao_yy_state ] ] != -tao_yy_n )
			{
				tao_yy_state = tao_yyact[ tao_yypgo[ tao_yy_n ] ];
			}
		}
					/* save until reenter driver code */
		tao_yystate = tao_yy_state;
		tao_yyps = tao_yy_ps;
		tao_yypv = tao_yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( tao_yytmp )
	{

case 4:
# line 240 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        } break;
case 5:
# line 244 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 6:
# line 248 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        } break;
case 7:
# line 252 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 8:
# line 256 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        } break;
case 9:
# line 260 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 10:
# line 264 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceDeclSeen);
        } break;
case 11:
# line 268 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 12:
# line 272 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ModuleDeclSeen);
        } break;
case 13:
# line 276 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 14:
# line 280 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	} break;
case 15:
# line 284 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	} break;
case 16:
# line 291 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSeen);
	  } break;
case 17:
# line 295 "fe/idl.tao_yy"
{
	    UTL_ScopedName 	*n =
		new UTL_ScopedName(new Identifier(tao_yypvt[-0].strval, 1, 0, I_FALSE), NULL);
	    AST_Module		*m = NULL;
	    UTL_Scope		*s = idl_global->scopes()->top_non_null();
	    UTL_StrList		*p = idl_global->pragmas();

	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleIDSeen);
	    /*
	     * Make a new module and add it to the enclosing scope
	     */
	    if (s != NULL) {
	      m = idl_global->gen()->create_module(n, p);
	      (void) s->fe_add_module(m);
	    }
	    /*
	     * Push it on the stack
	     */
	    idl_global->scopes()->push(m);
	  } break;
case 18:
# line 316 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSqSeen);
	  } break;
case 19:
# line 320 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleBodySeen);
	  } break;
case 20:
# line 324 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleQsSeen);
	    /*
	     * Finished with this module - pop it from the scope stack
	     */
	    UTL_Scope* s = idl_global->scopes()->top();
	    AST_Module* m = AST_Module::narrow_from_scope (s);
	    UTL_StrList *p = m->pragmas ();
	    if (p != 0)
		p = (UTL_StrList*)p->copy ();
	    idl_global->set_pragmas (p);
	    idl_global->scopes()->pop();
	  } break;
case 23:
# line 346 "fe/idl.tao_yy"
{
	  UTL_Scope     *s = idl_global->scopes()->top_non_null();
	  AST_Interface *i = NULL;
	  AST_Decl	*v = NULL;
	  UTL_StrList   *p = idl_global->pragmas();
	  AST_Decl	*d = NULL;
	  AST_Interface *fd = NULL;

	  ACE_UNUSED_ARG (v);

	  /*
	   * Make a new interface node and add it to its enclosing scope
	   */
	  if (s != NULL && tao_yypvt[-0].ihval != NULL) {
	    i = idl_global->gen()->create_interface(tao_yypvt[-0].ihval->interface_name(),
						    tao_yypvt[-0].ihval->inherits(),
						    tao_yypvt[-0].ihval->n_inherits(),
						    p);
	    if (i != NULL &&
		(d = s->lookup_by_name(i->name(), I_FALSE)) != NULL) {
	      /*
	       * See if we're defining a forward declared interface.
	       */
	      if (d->node_type() == AST_Decl::NT_interface) {
		/*
		 * Narrow to an interface
		 */
		fd = AST_Interface::narrow_from_decl(d);
		/*
		 * Successful?
		 */
		if (fd == NULL) {
		  /*
		   * Should we give an error here?
		   */
		}
		/*
		 * If it is a forward declared interface..
		 */
		else if (!fd->is_defined()) {
		  /*
		   * Check if redefining in same scope
		   */
		  if (fd->defined_in() != s) {
		    idl_global->err()
		       ->error3(UTL_Error::EIDL_SCOPE_CONFLICT,
				i,
				fd,
				ScopeAsDecl(s));
		  }
		  /*
		   * All OK, do the redefinition
		   */
		  else {
		    fd->set_inherits(tao_yypvt[-0].ihval->inherits());
		    fd->set_n_inherits(tao_yypvt[-0].ihval->n_inherits());
		    /*
		     * Update place of definition
		     */
		    fd->set_imported(idl_global->imported());
		    fd->set_in_main_file(idl_global->in_main_file());
		    fd->set_line(idl_global->lineno());
		    fd->set_file_name(idl_global->filename());
		    fd->add_pragmas(p);
		    /*
		     * Use full definition node
		     */
		    delete i;
		    i = fd;
		  }
	        }
	      }
	    }
	    /*
	     * Add the interface to its definition scope
	     */
	    (void) s->fe_add_interface(i);
	  }
	  /*
	   * Push it on the scope stack
	   */
	  idl_global->scopes()->push(i);
        } break;
case 24:
# line 430 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSqSeen);
	} break;
case 25:
# line 434 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceBodySeen);
	} break;
case 26:
# line 438 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceQsSeen);
	  /*
	   * Done with this interface - pop it off the scopes stack
	   */
	  UTL_Scope* s = idl_global->scopes()->top();
	  AST_Interface* m = AST_Interface::narrow_from_scope (s);
	  UTL_StrList *p = m->pragmas ();
	  if (p != 0)
		p = (UTL_StrList*)p->copy ();
	  idl_global->set_pragmas (p);
	  idl_global->scopes()->pop();
	} break;
case 27:
# line 455 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSeen);
	 } break;
case 28:
# line 459 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceIDSeen);
	   tao_yyval.idval = tao_yypvt[-0].idval;
	 } break;
case 29:
# line 467 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InheritSpecSeen);
	  /*
	   * Create an AST representation of the information in the header
	   * part of an interface - this representation contains a computed
	   * list of all interfaces which this interface inherits from,
	   * recursively
	   */
	  tao_yyval.ihval = new FE_InterfaceHeader(new UTL_ScopedName(tao_yypvt[-1].idval, NULL), tao_yypvt[-0].nlval);
	} break;
case 30:
# line 481 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InheritColonSeen);
        } break;
case 31:
# line 485 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = tao_yypvt[-0].nlval;
	} break;
case 32:
# line 489 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = NULL;
	} break;
case 35:
# line 501 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        } break;
case 36:
# line 505 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 37:
# line 509 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        } break;
case 38:
# line 513 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 39:
# line 517 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        } break;
case 40:
# line 521 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 41:
# line 525 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrDeclSeen);
        } break;
case 42:
# line 529 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 43:
# line 533 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpDeclSeen);
        } break;
case 44:
# line 537 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 45:
# line 541 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	} break;
case 46:
# line 545 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	} break;
case 47:
# line 553 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = new UTL_NameList(tao_yypvt[-1].idlist, tao_yypvt[-0].nlval);
	} break;
case 48:
# line 561 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SNListCommaSeen);
        } break;
case 49:
# line 565 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopedNameSeen);

	  if (tao_yypvt[-3].nlval == NULL)
	    tao_yyval.nlval = new UTL_NameList(tao_yypvt[-0].idlist, NULL);
	  else {
	    tao_yypvt[-3].nlval->nconc(new UTL_NameList(tao_yypvt[-0].idlist, NULL));
	    tao_yyval.nlval = tao_yypvt[-3].nlval;
	  }
	} break;
case 50:
# line 576 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = NULL;
	} break;
case 51:
# line 583 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  tao_yyval.idlist = new UTL_IdList(tao_yypvt[-0].idval, NULL);
	} break;
case 52:
# line 589 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        } break;
case 53:
# line 593 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  tao_yyval.idlist = new UTL_IdList(new Identifier(tao_yypvt[-2].strval, 1, 0, I_FALSE),
			      new UTL_IdList(tao_yypvt[-0].idval, NULL));
	} break;
case 54:
# line 601 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        } break;
case 55:
# line 605 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  tao_yypvt[-3].idlist->nconc(new UTL_IdList(tao_yypvt[-0].idval, NULL));
	  tao_yyval.idlist = tao_yypvt[-3].idlist;
	} break;
case 56:
# line 614 "fe/idl.tao_yy"
{
            tao_yyval.idval = new Identifier(tao_yypvt[-0].strval, 1, 0, I_FALSE);
        } break;
case 57:
# line 621 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(tao_yypvt[-0].idval, NULL);
	  AST_InterfaceFwd	*f = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_ForwardDeclSeen);
	  /*
	   * Create a node representing a forward declaration of an
	   * interface. Store it in the enclosing scope
	   */
	  if (s != NULL) {
	    f = idl_global->gen()->create_interface_fwd(n, p);
	    (void) s->fe_add_interface_fwd(f);
	  }
	} break;
case 58:
# line 641 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstSeen);
        } break;
case 59:
# line 645 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstTypeSeen);
        } break;
case 60:
# line 649 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstIDSeen);
        } break;
case 61:
# line 653 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstAssignSeen);
        } break;
case 62:
# line 657 "fe/idl.tao_yy"
{
	  UTL_ScopedName	*n = new UTL_ScopedName(tao_yypvt[-4].idval, NULL);
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Constant		*c = NULL;
	  UTL_StrList		*p = idl_global->pragmas();
	  AST_Decl		*v = NULL;

	  ACE_UNUSED_ARG (v);

	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstExprSeen);
	  /*
	   * Create a node representing a constant declaration. Store
	   * it in the enclosing scope
	   */
	  if (tao_yypvt[-0].exval != NULL && s != NULL) {
	    if (tao_yypvt[-0].exval->coerce(tao_yypvt[-6].etval) == NULL)
	      idl_global->err()->coercion_error(tao_yypvt[-0].exval, tao_yypvt[-6].etval);
	    else {
	      c =
		idl_global->gen()->create_constant(tao_yypvt[-6].etval, tao_yypvt[-0].exval, n, p);
	      (void) s->fe_add_constant(c);
	    }
	  }
	} break;
case 68:
# line 690 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_string;
	} break;
case 69:
# line 694 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_wstring;
        } break;
case 70:
# line 698 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Decl		*d = NULL;
	  AST_PredefinedType	*c = NULL;
	  AST_Typedef		*t = NULL;

	  /*
	   * If the constant's type is a scoped name, it must resolve
	   * to a scalar constant type
	   */
	  if (s != NULL && (d = s->lookup_by_name(tao_yypvt[-0].idlist, I_TRUE)) != NULL) {
	    /*
	     * Look through typedefs
	     */
	    while (d->node_type() == AST_Decl::NT_typedef) {
	      t = AST_Typedef::narrow_from_decl(d);
	      if (t == NULL)
	        break;
	      d = t->base_type();
	    }
	    if (d == NULL)
	      tao_yyval.etval = AST_Expression::EV_any;
	    else if (d->node_type() == AST_Decl::NT_pre_defined) {
	      c = AST_PredefinedType::narrow_from_decl(d);
	      if (c != NULL) {
	         tao_yyval.etval = idl_global->PredefinedTypeToExprType(c->pt());
	      } else {
	         tao_yyval.etval = AST_Expression::EV_any;
	      }
	    } else
	      tao_yyval.etval = AST_Expression::EV_any;
	  } else
	    tao_yyval.etval = AST_Expression::EV_any;
	} break;
case 74:
# line 740 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_or, tao_yypvt[-2].exval, tao_yypvt[-0].exval);
	} break;
case 76:
# line 748 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_xor, tao_yypvt[-2].exval, tao_yypvt[-0].exval);
	} break;
case 78:
# line 756 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_and, tao_yypvt[-2].exval, tao_yypvt[-0].exval);
	} break;
case 80:
# line 764 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_right,tao_yypvt[-2].exval,tao_yypvt[-0].exval);
	} break;
case 81:
# line 768 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_left,tao_yypvt[-2].exval,tao_yypvt[-0].exval);
	} break;
case 83:
# line 776 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_add, tao_yypvt[-2].exval, tao_yypvt[-0].exval);
	} break;
case 84:
# line 780 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_minus,tao_yypvt[-2].exval,tao_yypvt[-0].exval);
	} break;
case 86:
# line 788 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_mul, tao_yypvt[-2].exval, tao_yypvt[-0].exval);
	} break;
case 87:
# line 792 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_div, tao_yypvt[-2].exval, tao_yypvt[-0].exval);
	} break;
case 88:
# line 796 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_mod, tao_yypvt[-2].exval, tao_yypvt[-0].exval);
	} break;
case 90:
# line 804 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_u_plus,
					      tao_yypvt[-0].exval,
					      NULL);
	} break;
case 91:
# line 810 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_u_minus,
					      tao_yypvt[-0].exval,
					      NULL);
	} break;
case 92:
# line 816 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_bit_neg,
					      tao_yypvt[-0].exval,
					      NULL);
	} break;
case 93:
# line 825 "fe/idl.tao_yy"
{
	  /*
	   * An expression which is a scoped name is not resolved now,
	   * but only when it is evaluated (such as when it is assigned
	   * as a constant value)
	   */
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yypvt[-0].idlist);
	} break;
case 95:
# line 835 "fe/idl.tao_yy"
{
	  tao_yyval.exval = tao_yypvt[-1].exval;
	} break;
case 96:
# line 842 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yypvt[-0].ival);
	} break;
case 97:
# line 846 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yypvt[-0].sval);
	} break;
case 98:
# line 850 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yypvt[-0].cval);
	} break;
case 99:
# line 854 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yypvt[-0].dval);
	} break;
case 100:
# line 858 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr((idl_bool) I_TRUE,
					    AST_Expression::EV_bool);
	} break;
case 101:
# line 863 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr((idl_bool) I_FALSE,
					    AST_Expression::EV_bool);
	} break;
case 102:
# line 871 "fe/idl.tao_yy"
{
	    tao_yypvt[-0].exval->evaluate(AST_Expression::EK_const);
	    tao_yyval.exval = idl_global->gen()->create_expr(tao_yypvt[-0].exval, AST_Expression::EV_ulong);
	} break;
case 103:
# line 879 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_TypedefSeen);
	  } break;
case 104:
# line 882 "fe/idl.tao_yy"
{tao_yyval.ival = 0;} break;
case 105:
# line 883 "fe/idl.tao_yy"
{ tao_yyval.ival = 0;} break;
case 106:
# line 884 "fe/idl.tao_yy"
{ tao_yyval.ival = 0;} break;
case 107:
# line 885 "fe/idl.tao_yy"
{ tao_yyval.ival = 0;} break;
case 108:
# line 887 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Native		*node = NULL;
	  AST_Decl		*v = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

          ACE_UNUSED_ARG (v);

	  idl_global->set_parse_state(IDL_GlobalData::PS_NativeSeen);
	  /*
	   * Create a node representing a Native and add it to its
	   * enclosing scope
	   */
	  if (s != NULL) {
	    node = idl_global->gen()->create_native (tao_yypvt[-0].deval->name (), p);
	    /*
	     * Add it to its defining scope
	     */
	    (void) s->fe_add_native (node);
	  }
	} break;
case 109:
# line 912 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeSpecSeen);
        } break;
case 110:
# line 916 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_DecllistActiveIterator *l;
	  FE_Declarator		*d = NULL;
	  AST_Typedef		*t = NULL;
	  AST_Decl		*v = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

          ACE_UNUSED_ARG (v);

	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclaratorsSeen);
	  /*
	   * Create a list of type renamings. Add them to the
	   * enclosing scope
	   */
	  if (s != NULL && tao_yypvt[-2].dcval != NULL && tao_yypvt[-0].dlval != NULL) {
	    l = new UTL_DecllistActiveIterator(tao_yypvt[-0].dlval);
	    for (;!(l->is_done()); l->next()) {
	      d = l->item();
	      if (d == NULL)
		      continue;
              AST_Type * tp = d->compose(tao_yypvt[-2].dcval);
              if (tp == NULL)
		     continue;
	      t = idl_global->gen()->create_typedef(tp, d->name(), p);
	      (void) s->fe_add_typedef(t);
	    }
	    delete l;
	  }
	} break;
case 113:
# line 955 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yypvt[-0].etval);
	} break;
case 115:
# line 960 "fe/idl.tao_yy"
{
	  UTL_Scope	*s = idl_global->scopes()->top_non_null();
	  AST_Decl	*d = NULL;

	  if (s != NULL)
	    d = s->lookup_by_name(tao_yypvt[-0].idlist, I_TRUE);
	  if (d == NULL)
	    idl_global->err()->lookup_error(tao_yypvt[-0].idlist);
	  tao_yyval.dcval = d;
	} break;
case 128:
# line 995 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = new UTL_DeclList(tao_yypvt[-1].deval, tao_yypvt[-0].dlval);
	} break;
case 129:
# line 1003 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        } break;
case 130:
# line 1007 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsDeclSeen);

	  if (tao_yypvt[-3].dlval == NULL)
	    tao_yyval.dlval = new UTL_DeclList(tao_yypvt[-0].deval, NULL);
	  else {
	    tao_yypvt[-3].dlval->nconc(new UTL_DeclList(tao_yypvt[-0].deval, NULL));
	    tao_yyval.dlval = tao_yypvt[-3].dlval;
	  }
	} break;
case 131:
# line 1018 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = NULL;
	} break;
case 134:
# line 1030 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = new UTL_DeclList(tao_yypvt[-1].deval, tao_yypvt[-0].dlval);
	} break;
case 135:
# line 1038 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        } break;
case 136:
# line 1042 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsDeclSeen);

	  if (tao_yypvt[-3].dlval == NULL)
	    tao_yyval.dlval = new UTL_DeclList(tao_yypvt[-0].deval, NULL);
	  else {
	    tao_yypvt[-3].dlval->nconc(new UTL_DeclList(tao_yypvt[-0].deval, NULL));
	    tao_yyval.dlval = tao_yypvt[-3].dlval;
	  }
	} break;
case 137:
# line 1053 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = NULL;
	} break;
case 138:
# line 1060 "fe/idl.tao_yy"
{
	  tao_yyval.deval = new FE_Declarator(new UTL_ScopedName(tao_yypvt[-0].idval, NULL),
				 FE_Declarator::FD_simple, NULL);
	} break;
case 139:
# line 1068 "fe/idl.tao_yy"
{
	  tao_yyval.deval = new FE_Declarator(new UTL_ScopedName(tao_yypvt[-0].dcval->local_name(), NULL),
				 FE_Declarator::FD_complex,
				 tao_yypvt[-0].dcval);
	} break;
case 142:
# line 1082 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_long;
	} break;
case 143:
# line 1086 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_longlong;
        } break;
case 144:
# line 1090 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_short;
	} break;
case 145:
# line 1097 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_ulong;
	} break;
case 146:
# line 1101 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_ulonglong;
        } break;
case 147:
# line 1105 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_ushort;
	} break;
case 148:
# line 1112 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_double;
	} break;
case 149:
# line 1116 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_float;
	} break;
case 150:
# line 1120 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_longdouble;
        } break;
case 151:
# line 1127 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_char;
	} break;
case 152:
# line 1131 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_wchar;
        } break;
case 153:
# line 1138 "fe/idl.tao_yy"
{
          tao_yyval.etval = AST_Expression::EV_octet;
	} break;
case 154:
# line 1145 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_bool;
        } break;
case 155:
# line 1152 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_any;
	} break;
case 156:
# line 1159 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructSeen);
        } break;
case 157:
# line 1163 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(tao_yypvt[-0].idval, NULL);
	  AST_Structure		*d = NULL;
	  UTL_StrList		*p = idl_global->pragmas();
	  AST_Decl		*v = NULL;

          ACE_UNUSED_ARG (v);

	  idl_global->set_parse_state(IDL_GlobalData::PS_StructIDSeen);
	  /*
	   * Create a node representing a struct declaration. Add it
	   * to the enclosing scope
	   */
	  if (s != NULL) {
	    d = idl_global->gen()->create_structure(n, p);
	    (void) s->fe_add_structure(d);
	  }
	  /*
	   * Push the scope of the struct on the scopes stack
	   */
	  idl_global->scopes()->push(d);
	} break;
case 158:
# line 1187 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructSqSeen);
        } break;
case 159:
# line 1191 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructBodySeen);
        } break;
case 160:
# line 1195 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructQsSeen);
	  /*
	   * Done with this struct. Pop its scope off the scopes stack
	   */
          tao_yyval.dcval =
	      AST_Structure::narrow_from_scope(
				   idl_global->scopes()->top_non_null());
	   UTL_StrList *p = tao_yyval.dcval->pragmas ();
	   if (p != 0)
	     p = (UTL_StrList*)p->copy ();
	  idl_global->set_pragmas (p);
          idl_global->scopes()->pop();
	} break;
case 164:
# line 1220 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberTypeSeen);
        } break;
case 165:
# line 1224 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberDeclsSeen);
        } break;
case 166:
# line 1228 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_DecllistActiveIterator *l = NULL;
	  FE_Declarator		*d = NULL;
	  AST_Field		*f = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberDeclsCompleted);
	  /*
	   * Check for illegal recursive use of type
	   */
	  if (tao_yypvt[-4].dcval != NULL && AST_illegal_recursive_type(tao_yypvt[-4].dcval))
	    idl_global->err()->error1(UTL_Error::EIDL_RECURSIVE_TYPE, tao_yypvt[-4].dcval);
	  /*
	   * Create a node representing a struct or exception member
	   * Add it to the enclosing scope
	   */
	  else if (s != NULL && tao_yypvt[-4].dcval != NULL && tao_yypvt[-2].dlval != NULL) {
	    l = new UTL_DecllistActiveIterator(tao_yypvt[-2].dlval);
	    for (;!(l->is_done()); l->next()) {
	      d = l->item();
	      if (d == NULL)
		continue;
 	      AST_Type *tp = d->compose(tao_yypvt[-4].dcval);
	      if (tp == NULL)
		continue;
	      f = idl_global->gen()->create_field(tp, d->name(), p);
	      (void) s->fe_add_field(f);
	    }
	    delete l;
	  }
	} break;
case 167:
# line 1261 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	} break;
case 168:
# line 1265 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	} break;
case 169:
# line 1273 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionSeen);
        } break;
case 170:
# line 1277 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionIDSeen);
        } break;
case 171:
# line 1281 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchSeen);
        } break;
case 172:
# line 1285 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchOpenParSeen);
        } break;
case 173:
# line 1289 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchTypeSeen);
        } break;
case 174:
# line 1293 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(tao_yypvt[-8].idval, NULL);
	  AST_Union		*u = NULL;
	  AST_Decl		*v = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

          ACE_UNUSED_ARG (v);

	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchCloseParSeen);
	  /*
	   * Create a node representing a union. Add it to its enclosing
	   * scope
	   */
	  if (tao_yypvt[-2].dcval != NULL && s != NULL) {
 	    AST_ConcreteType    *tp = AST_ConcreteType::narrow_from_decl(tao_yypvt[-2].dcval);
            if (tp == NULL) {
              idl_global->err()->not_a_type(tao_yypvt[-2].dcval);
            } else {
	      u = idl_global->gen()->create_union(tp, n, p);
	      (void) s->fe_add_union(u);
 	    }
	  }
	  /*
	   * Push the scope of the union on the scopes stack
	   */
	  idl_global->scopes()->push(u);
	} break;
case 175:
# line 1322 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionSqSeen);
        } break;
case 176:
# line 1326 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionBodySeen);
        } break;
case 177:
# line 1330 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionQsSeen);
	  /*
	   * Done with this union. Pop its scope from the scopes stack
	   */
	  tao_yyval.dcval =
	     AST_Union::narrow_from_scope(
				idl_global->scopes()->top_non_null());
	  UTL_StrList *p = tao_yyval.dcval->pragmas ();
	  if (p != 0)
	     p = (UTL_StrList*)p->copy ();
	  idl_global->set_pragmas (p);
	  idl_global->scopes()->pop();
	} break;
case 178:
# line 1348 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yypvt[-0].etval);
	} break;
case 179:
# line 1352 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yypvt[-0].etval);
	} break;
case 180:
# line 1356 "fe/idl.tao_yy"
{
          // octets are not allowed
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
	} break;
case 181:
# line 1361 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yypvt[-0].etval);
	} break;
case 183:
# line 1366 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Decl		*d = NULL;
	  AST_PredefinedType	*p = NULL;
	  AST_Typedef		*t = NULL;
	  long			found = I_FALSE;

	  /*
	   * The discriminator is a scoped name. Try to resolve to
	   * one of the scalar types or to an enum. Thread through
	   * typedef's to arrive at the base type at the end of the
	   * chain
	   */
	  if (s != NULL && (d = s->lookup_by_name(tao_yypvt[-0].idlist, I_TRUE)) != NULL) {
	    while (!found) {
	      switch (d->node_type()) {
	      case AST_Decl::NT_enum:
		tao_yyval.dcval = d;
		found = I_TRUE;
		break;
	      case AST_Decl::NT_pre_defined:
		p = AST_PredefinedType::narrow_from_decl(d);
		if (p != NULL) {
		  switch (p->pt()) {
		  case AST_PredefinedType::PT_long:
		  case AST_PredefinedType::PT_ulong:
		  case AST_PredefinedType::PT_longlong:
		  case AST_PredefinedType::PT_ulonglong:
		  case AST_PredefinedType::PT_short:
		  case AST_PredefinedType::PT_char:
		  case AST_PredefinedType::PT_wchar:
		  case AST_PredefinedType::PT_boolean:
		    tao_yyval.dcval = p;
		    found = I_TRUE;
		    break;
		  case AST_PredefinedType::PT_octet:
                    // octets are not allowed
                    idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
                    tao_yyval.dcval = NULL;
		    found = I_TRUE;
		    break;
		  default:
		    tao_yyval.dcval = NULL;
		    found = I_TRUE;
		    break;
		  }
		} else
                {
		    tao_yyval.dcval = NULL;
		    found = I_TRUE;
                }
		break;
	      case AST_Decl::NT_typedef:
		t = AST_Typedef::narrow_from_decl(d);
		if (t != NULL) d = t->base_type();
		break;
	      default:
		tao_yyval.dcval = NULL;
		found = I_TRUE;
		break;
	      }
	    }
	  } else
	    tao_yyval.dcval = NULL;

	  if (tao_yyval.dcval == NULL)
	    idl_global->err()->lookup_error(tao_yypvt[-0].idlist);
	} break;
case 187:
# line 1445 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionLabelSeen);
        } break;
case 188:
# line 1449 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemSeen);
        } break;
case 189:
# line 1453 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_LabellistActiveIterator *l = NULL;
	  AST_UnionLabel	*d = NULL;
	  AST_UnionBranch	*b = NULL;
	  AST_Field		*f = tao_yypvt[-2].ffval;

	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemCompleted);
	  /*
	   * Create several nodes representing branches of a union.
	   * Add them to the enclosing scope (the union scope)
	   */
	  if (s != NULL && tao_yypvt[-4].llval != NULL && tao_yypvt[-2].ffval != NULL) {
	    l = new UTL_LabellistActiveIterator(tao_yypvt[-4].llval);
	    for (;!(l->is_done()); l->next()) {
	      d = l->item();
	      if (d == NULL)
		continue;
	      b = idl_global->gen()->create_union_branch(d,
						      f->field_type(),
						      f->name(),
						      f->pragmas());
	      (void) s->fe_add_union_branch(b);
	    }
	    delete l;
	  }
	} break;
case 190:
# line 1481 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	} break;
case 191:
# line 1486 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	} break;
case 192:
# line 1494 "fe/idl.tao_yy"
{
	  tao_yyval.llval = new UTL_LabelList(tao_yypvt[-1].ulval, tao_yypvt[-0].llval);
	} break;
case 193:
# line 1501 "fe/idl.tao_yy"
{
	  if (tao_yypvt[-1].llval == NULL)
	    tao_yyval.llval = new UTL_LabelList(tao_yypvt[-0].ulval, NULL);
	  else {
	    tao_yypvt[-1].llval->nconc(new UTL_LabelList(tao_yypvt[-0].ulval, NULL));
	    tao_yyval.llval = tao_yypvt[-1].llval;
	  }
	} break;
case 194:
# line 1510 "fe/idl.tao_yy"
{
	  tao_yyval.llval = NULL;
	} break;
case 195:
# line 1517 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DefaultSeen);
        } break;
case 196:
# line 1521 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

	  tao_yyval.ulval = idl_global->gen()->
	            create_union_label(AST_UnionLabel::UL_default,
				       NULL);
	} break;
case 197:
# line 1529 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_CaseSeen);
        } break;
case 198:
# line 1533 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelExprSeen);
        } break;
case 199:
# line 1537 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

	  tao_yyval.ulval = idl_global->gen()->create_union_label(AST_UnionLabel::UL_label,
						     tao_yypvt[-2].exval);
	} break;
case 200:
# line 1547 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemTypeSeen);
        } break;
case 201:
# line 1551 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemDeclSeen);
	  /*
	   * Check for illegal recursive use of type
	   */
	  if (tao_yypvt[-2].dcval != NULL && AST_illegal_recursive_type(tao_yypvt[-2].dcval))
	    idl_global->err()->error1(UTL_Error::EIDL_RECURSIVE_TYPE, tao_yypvt[-2].dcval);
	  /*
	   * Create a field in a union branch
	   */
	  else if (tao_yypvt[-2].dcval == NULL || tao_yypvt[-0].deval == NULL)
	    tao_yyval.ffval = NULL;
	  else {
	    AST_Type *tp = tao_yypvt[-0].deval->compose(tao_yypvt[-2].dcval);
	    if (tp == NULL)
	      tao_yyval.ffval = NULL;
 	    else
	      tao_yyval.ffval = idl_global->gen()->create_field(tp,
			        		   tao_yypvt[-0].deval->name(),
			        		   idl_global->pragmas());
	  }
	} break;
case 202:
# line 1577 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumSeen);
        } break;
case 203:
# line 1581 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(tao_yypvt[-0].idval, NULL);
	  AST_Enum		*e = NULL;
	  AST_Decl		*v = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

          ACE_UNUSED_ARG (v);

	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumIDSeen);
	  /*
	   * Create a node representing an enum and add it to its
	   * enclosing scope
	   */
	  if (s != NULL) {
	    e = idl_global->gen()->create_enum(n, p);
	    /*
	     * Add it to its defining scope
	     */
	    (void) s->fe_add_enum(e);
	  }
	  /*
	   * Push the enum scope on the scopes stack
	   */
	  idl_global->scopes()->push(e);
	} break;
case 204:
# line 1608 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumSqSeen);
        } break;
case 205:
# line 1612 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumBodySeen);
        } break;
case 206:
# line 1616 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumQsSeen);
	  /*
	   * Done with this enum. Pop its scope from the scopes stack
	   */
	  if (idl_global->scopes()->top() == NULL)
	    tao_yyval.dcval = NULL;
	  else {
	    tao_yyval.dcval = AST_Enum::narrow_from_scope(idl_global->scopes()->top_non_null());
	    idl_global->scopes()->pop();
	  }
	} break;
case 208:
# line 1635 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumCommaSeen);
        } break;
case 211:
# line 1644 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n =
		new UTL_ScopedName(new Identifier(tao_yypvt[-0].strval, 1, 0, I_FALSE), NULL);
	  AST_EnumVal		*e = NULL;
	  AST_Enum		*c = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  /*
	   * Create a node representing one enumerator in an enum
	   * Add it to the enclosing scope (the enum scope)
	   */
	  if (s != NULL && s->scope_node_type() == AST_Decl::NT_enum) {
	    c = AST_Enum::narrow_from_scope(s);
	    if (c != NULL)
	      e = idl_global->gen()->create_enum_val(c->next_enum_val(), n, p);
	    (void) s->fe_add_enum_val(e);
	  }
	} break;
case 212:
# line 1668 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceCommaSeen);
        } break;
case 213:
# line 1672 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceExprSeen);
        } break;
case 214:
# line 1676 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceQsSeen);
	  /*
	   * Remove sequence marker from scopes stack
	   */
	  if (idl_global->scopes()->top() == NULL)
	    idl_global->scopes()->pop();
	  /*
	   * Create a node representing a sequence
	   */
	  if (tao_yypvt[-2].exval == NULL || tao_yypvt[-2].exval->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error(tao_yypvt[-2].exval, AST_Expression::EV_ulong);
	    tao_yyval.dcval = NULL;
	  } else if (tao_yypvt[-5].dcval == NULL) {
	    tao_yyval.dcval = NULL;
	  } else {
 	    AST_Type *tp = AST_Type::narrow_from_decl(tao_yypvt[-5].dcval);
	    if (tp == NULL)
	      tao_yyval.dcval = NULL;
	    else {
	      tao_yyval.dcval = idl_global->gen()->create_sequence(tao_yypvt[-2].exval, tp);
	      /*
	       * Add this AST_Sequence to the types defined in the global scope
	       */
	      (void) idl_global->root()
		        ->fe_add_sequence(AST_Sequence::narrow_from_decl(tao_yyval.dcval));
 	    }
	  }
	} break;
case 215:
# line 1707 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceQsSeen);
	  /*
	   * Remove sequence marker from scopes stack
	   */
	  if (idl_global->scopes()->top() == NULL)
	    idl_global->scopes()->pop();
	  /*
	   * Create a node representing a sequence
	   */
	  if (tao_yypvt[-1].dcval == NULL)
	    tao_yyval.dcval = NULL;
	  else {
	    AST_Type *tp = AST_Type::narrow_from_decl(tao_yypvt[-1].dcval);
	    if (tp == NULL)
	      tao_yyval.dcval = NULL;
            else {
	      tao_yyval.dcval =
	        idl_global->gen()->create_sequence(
		  	     idl_global->gen()->create_expr((unsigned long) 0),
			     tp);
	      /*
	       * Add this AST_Sequence to the types defined in the global scope
	       */
	      (void) idl_global->root()
		        ->fe_add_sequence(AST_Sequence::narrow_from_decl(tao_yyval.dcval));
	    }
	  }
	} break;
case 216:
# line 1740 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSeen);
	  /*
	   * Push a sequence marker on scopes stack
	   */
	  idl_global->scopes()->push(NULL);
	} break;
case 217:
# line 1748 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSqSeen);
        } break;
case 218:
# line 1752 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceTypeSeen);
	  tao_yyval.dcval = tao_yypvt[-0].dcval;
        } break;
case 219:
# line 1761 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        } break;
case 220:
# line 1765 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        } break;
case 221:
# line 1769 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringQsSeen);
	  /*
	   * Create a node representing a string
	   */
	  if (tao_yypvt[-2].exval == NULL || tao_yypvt[-2].exval->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error(tao_yypvt[-2].exval, AST_Expression::EV_ulong);
	    tao_yyval.dcval = NULL;
	  } else {
	    tao_yyval.dcval = idl_global->gen()->create_string(tao_yypvt[-2].exval);
	    /*
	     * Add this AST_String to the types defined in the global scope
	     */
	    (void) idl_global->root()
		      ->fe_add_string(AST_String::narrow_from_decl(tao_yyval.dcval));
	  }
	} break;
case 222:
# line 1787 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringCompleted);
	  /*
	   * Create a node representing a string
	   */
	  tao_yyval.dcval =
	    idl_global->gen()->create_string(
			 idl_global->gen()->create_expr((unsigned long) 0));
	  /*
	   * Add this AST_String to the types defined in the global scope
	   */
	  (void) idl_global->root()
                    ->fe_add_string(AST_String::narrow_from_decl(tao_yyval.dcval));
	} break;
case 223:
# line 1805 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        } break;
case 224:
# line 1813 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        } break;
case 225:
# line 1817 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        } break;
case 226:
# line 1821 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringQsSeen);
	  /*
	   * Create a node representing a string
	   */
	  if (tao_yypvt[-2].exval == NULL || tao_yypvt[-2].exval->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error(tao_yypvt[-2].exval, AST_Expression::EV_ulong);
	    tao_yyval.dcval = NULL;
	  } else {
	    tao_yyval.dcval = idl_global->gen()->create_wstring(tao_yypvt[-2].exval);
	    /*
	     * Add this AST_String to the types defined in the global scope
	     */
	    (void) idl_global->root()
		      ->fe_add_string(AST_String::narrow_from_decl(tao_yyval.dcval));
	  }
	} break;
case 227:
# line 1839 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringCompleted);
	  /*
	   * Create a node representing a string
	   */
	  tao_yyval.dcval =
	    idl_global->gen()->create_wstring(
			 idl_global->gen()->create_expr((unsigned long) 0));
	  /*
	   * Add this AST_String to the types defined in the global scope
	   */
	  (void) idl_global->root()
                    ->fe_add_string(AST_String::narrow_from_decl(tao_yyval.dcval));
	} break;
case 228:
# line 1857 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        } break;
case 229:
# line 1864 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ArrayIDSeen);
        } break;
case 230:
# line 1868 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ArrayCompleted);
	  /*
	   * Create a node representing an array
	   */
	  if (tao_yypvt[-0].elval != NULL) {
	     tao_yyval.dcval = idl_global->gen()->create_array(new UTL_ScopedName(tao_yypvt[-2].idval, NULL),
						  tao_yypvt[-0].elval->length(), tao_yypvt[-0].elval);
	  }
	} break;
case 231:
# line 1882 "fe/idl.tao_yy"
{
	  tao_yyval.elval = new UTL_ExprList(tao_yypvt[-1].exval, tao_yypvt[-0].elval);
	} break;
case 232:
# line 1889 "fe/idl.tao_yy"
{
	  if (tao_yypvt[-1].elval == NULL)
	    tao_yyval.elval = new UTL_ExprList(tao_yypvt[-0].exval, NULL);
	  else {
	    tao_yypvt[-1].elval->nconc(new UTL_ExprList(tao_yypvt[-0].exval, NULL));
	    tao_yyval.elval = tao_yypvt[-1].elval;
	  }
	} break;
case 233:
# line 1898 "fe/idl.tao_yy"
{
	  tao_yyval.elval = NULL;
	} break;
case 234:
# line 1905 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimSqSeen);
        } break;
case 235:
# line 1909 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimExprSeen);
        } break;
case 236:
# line 1913 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimQsSeen);
	  /*
	   * Array dimensions are expressions which must be coerced to
	   * positive integers
	   */
	  if (tao_yypvt[-2].exval == NULL || tao_yypvt[-2].exval->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error(tao_yypvt[-2].exval, AST_Expression::EV_ulong);
	    tao_yyval.exval = NULL;
	  } else
	    tao_yyval.exval = tao_yypvt[-2].exval;
	} break;
case 237:
# line 1930 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrSeen);
        } break;
case 238:
# line 1934 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrTypeSeen);
        } break;
case 239:
# line 1938 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_DecllistActiveIterator *l = NULL;
	  AST_Attribute		*a = NULL;
	  FE_Declarator		*d = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrCompleted);
	  /*
	   * Create nodes representing attributes and add them to the
	   * enclosing scope
	   */
	  if (s != NULL && tao_yypvt[-2].dcval != NULL && tao_yypvt[-0].dlval != NULL) {
	    l = new UTL_DecllistActiveIterator(tao_yypvt[-0].dlval);
	    for (;!(l->is_done()); l->next()) {
	      d = l->item();
	      if (d == NULL)
		continue;
	      AST_Type *tp = d->compose(tao_yypvt[-2].dcval);
 	      if (tp == NULL)
	 	continue;
	      a = idl_global->gen()->create_attribute(tao_yypvt[-5].bval, tp, d->name(), p);
	      /*
	       * Add one attribute to the enclosing scope
	       */
	      (void) s->fe_add_attribute(a);
	    }
	    delete l;
	  }
	} break;
case 240:
# line 1972 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrROSeen);
	  tao_yyval.bval = I_TRUE;
	} break;
case 241:
# line 1977 "fe/idl.tao_yy"
{
	  tao_yyval.bval = I_FALSE;
	} break;
case 242:
# line 1984 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSeen);
	} break;
case 243:
# line 1988 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(tao_yypvt[-0].idval, NULL);
	  AST_Exception		*e = NULL;
	  UTL_StrList		*p = idl_global->pragmas();
	  AST_Decl		*v = NULL;

          ACE_UNUSED_ARG (v);

	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptIDSeen);
	  /*
	   * Create a node representing an exception and add it to
	   * the enclosing scope
	   */
	  if (s != NULL) {
	    e = idl_global->gen()->create_exception(n, p);
	    (void) s->fe_add_exception(e);
	  }
	  /*
	   * Push the exception scope on the scope stack
	   */
	  idl_global->scopes()->push(e);
	} break;
case 244:
# line 2012 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSqSeen);
        } break;
case 245:
# line 2016 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptBodySeen);
        } break;
case 246:
# line 2020 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptQsSeen);
	  /*
	   * Done with this exception. Pop its scope from the scope stack
	   */
	  idl_global->scopes()->pop();
	} break;
case 247:
# line 2032 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpTypeSeen);
        } break;
case 248:
# line 2036 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n =
		new UTL_ScopedName(new Identifier(tao_yypvt[-0].strval, 1, 0, I_FALSE), NULL);
	  AST_Operation		*o = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_OpIDSeen);
	  /*
	   * Create a node representing an operation on an interface
	   * and add it to its enclosing scope
	   */
	  if (s != NULL && tao_yypvt[-2].dcval != NULL) {
	    AST_Type *tp = AST_Type::narrow_from_decl(tao_yypvt[-2].dcval);
            if (tp == NULL) {
              idl_global->err()->not_a_type(tao_yypvt[-2].dcval);
            } else if (tp->node_type() == AST_Decl::NT_except) {
              idl_global->err()->not_a_type(tao_yypvt[-2].dcval);
            } else {
	      o = idl_global->gen()->create_operation(tp, tao_yypvt[-3].ofval, n, p);
	      (void) s->fe_add_operation(o);
	    }
	  }
	  /*
	   * Push the operation scope onto the scopes stack
	   */
	  idl_global->scopes()->push(o);
	} break;
case 249:
# line 2065 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParsCompleted);
        } break;
case 250:
# line 2069 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseCompleted);
        } break;
case 251:
# line 2073 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Operation		*o = NULL;

	  idl_global->set_parse_state(IDL_GlobalData::PS_OpCompleted);
	  /*
	   * Add exceptions and context to the operation
	   */
	  if (s != NULL && s->scope_node_type() == AST_Decl::NT_op) {
	    o = AST_Operation::narrow_from_scope(s);

	    if (tao_yypvt[-2].nlval != NULL && o != NULL)
	      (void) o->fe_add_exceptions(tao_yypvt[-2].nlval);
	    if (tao_yypvt[-0].slval != NULL)
	      (void) o->fe_add_context(tao_yypvt[-0].slval);
	  }
	  /*
	   * Done with this operation. Pop its scope from the scopes stack
	   */
	  idl_global->scopes()->pop();
	} break;
case 252:
# line 2098 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
	  tao_yyval.ofval = AST_Operation::OP_oneway;
	} break;
case 253:
# line 2103 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
	  tao_yyval.ofval = AST_Operation::OP_idempotent;
	} break;
case 254:
# line 2108 "fe/idl.tao_yy"
{
	  tao_yyval.ofval = AST_Operation::OP_noflags;
	} break;
case 256:
# line 2116 "fe/idl.tao_yy"
{
	  tao_yyval.dcval =
	    idl_global->scopes()->bottom()
	       ->lookup_primitive_type(AST_Expression::EV_void);
	} break;
case 257:
# line 2125 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        } break;
case 258:
# line 2129 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        } break;
case 259:
# line 2133 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        } break;
case 260:
# line 2138 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        } break;
case 262:
# line 2148 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParCommaSeen);
        } break;
case 265:
# line 2157 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParDirSeen);
        } break;
case 266:
# line 2161 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParTypeSeen);
        } break;
case 267:
# line 2165 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Argument		*a = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParDeclSeen);
	  /*
	   * Create a node representing an argument to an operation
	   * Add it to the enclosing scope (the operation scope)
	   */
	  if (tao_yypvt[-2].dcval != NULL && tao_yypvt[-0].deval != NULL && s != NULL) {
	    AST_Type *tp = tao_yypvt[-0].deval->compose(tao_yypvt[-2].dcval);
	    if (tp != NULL) {
	      a = idl_global->gen()->create_argument(tao_yypvt[-4].dival, tp, tao_yypvt[-0].deval->name(), p);
	      (void) s->fe_add_argument(a);
	    }
	  }
	} break;
case 268:
# line 2187 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yypvt[-0].etval);
	} break;
case 270:
# line 2192 "fe/idl.tao_yy"
{
	  UTL_Scope	*s = idl_global->scopes()->top_non_null();
	  AST_Decl	*d = NULL;

	  if (s != NULL)
	    d = s->lookup_by_name(tao_yypvt[-0].idlist, I_TRUE);
	  if (d == NULL)
	    idl_global->err()->lookup_error(tao_yypvt[-0].idlist);
	  tao_yyval.dcval = d;
	} break;
case 271:
# line 2206 "fe/idl.tao_yy"
{
	  tao_yyval.dival = AST_Argument::dir_IN;
	} break;
case 272:
# line 2210 "fe/idl.tao_yy"
{
	  tao_yyval.dival = AST_Argument::dir_OUT;
	} break;
case 273:
# line 2214 "fe/idl.tao_yy"
{
	  tao_yyval.dival = AST_Argument::dir_INOUT;
	} break;
case 274:
# line 2221 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSeen);
        } break;
case 275:
# line 2225 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSqSeen);
        } break;
case 276:
# line 2230 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseQsSeen);
	  tao_yyval.nlval = tao_yypvt[-1].nlval;
	} break;
case 277:
# line 2235 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = NULL;
	} break;
case 278:
# line 2242 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSeen);
        } break;
case 279:
# line 2246 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSqSeen);
        } break;
case 280:
# line 2251 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextQsSeen);
	  tao_yyval.slval = tao_yypvt[-1].slval;
	} break;
case 281:
# line 2256 "fe/idl.tao_yy"
{
	  tao_yyval.slval = NULL;
	} break;
case 282:
# line 2263 "fe/idl.tao_yy"
{
	  tao_yyval.slval = new UTL_StrList(tao_yypvt[-1].sval, tao_yypvt[-0].slval);
	} break;
case 283:
# line 2271 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextCommaSeen);
        } break;
case 284:
# line 2275 "fe/idl.tao_yy"
{
	  if (tao_yypvt[-3].slval == NULL)
	    tao_yyval.slval = new UTL_StrList(tao_yypvt[-0].sval, NULL);
	  else {
	    tao_yypvt[-3].slval->nconc(new UTL_StrList(tao_yypvt[-0].sval, NULL));
	    tao_yyval.slval = tao_yypvt[-3].slval;
	  }
	} break;
case 285:
# line 2284 "fe/idl.tao_yy"
{
	  tao_yyval.slval = NULL;
	} break;
# line	531 "/usr/ccs/bin/yaccpar"
	}
	goto tao_yystack;		/* reset registers in driver code */
}
