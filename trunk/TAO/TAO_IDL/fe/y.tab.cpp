
# line 72 "idl.yy"
#include "idl.h"
#include "idl_extern.h"

#include "fe_private.h"

#include <stdio.h>

#if (defined(apollo) || defined(hpux)) && defined(__cplusplus)
extern	"C" int yywrap();
#endif	// (defined(apollo) || defined(hpux)) && defined(__cplusplus)


# line 89 "idl.yy"
typedef union
#ifdef __cplusplus
	YYSTYPE
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
} YYSTYPE;
# define IDENTIFIER 257
# define CONST 258
# define MODULE 259
# define INTERFACE 260
# define TYPEDEF 261
# define LONG 262
# define SHORT 263
# define UNSIGNED 264
# define DOUBLE 265
# define FLOAT 266
# define CHAR 267
# define WCHAR 268
# define OCTET 269
# define BOOLEAN 270
# define ANY 271
# define STRUCT 272
# define UNION 273
# define SWITCH 274
# define ENUM 275
# define SEQUENCE 276
# define STRING 277
# define WSTRING 278
# define EXCEPTION 279
# define CASE 280
# define DEFAULT 281
# define READONLY 282
# define ATTRIBUTE 283
# define ONEWAY 284
# define IDEMPOTENT 285
# define VOID 286
# define IN 287
# define OUT 288
# define INOUT 289
# define RAISES 290
# define CONTEXT 291
# define INTEGER_LITERAL 292
# define STRING_LITERAL 293
# define CHARACTER_LITERAL 294
# define FLOATING_PT_LITERAL 295
# define TRUETOK 296
# define FALSETOK 297
# define SCOPE_DELIMITOR 298
# define LEFT_SHIFT 299
# define RIGHT_SHIFT 300

#ifdef __STDC__
#include <stdlib.h>
#include <string.h>
#else
#include <malloc.h>
#include <memory.h>
#endif

#include <values.h>

#ifdef __cplusplus

#ifndef yyerror
	void yyerror(const char *);
#endif

#ifndef yylex
#ifdef __EXTERN_C__
	extern "C" { int yylex(void); }
#else
	int yylex(void);
#endif
#endif
	int yyparse(void);

#endif
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
YYSTYPE yylval;
YYSTYPE yyval;
typedef int yytabelem;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
#if YYMAXDEPTH > 0
int yy_yys[YYMAXDEPTH], *yys = yy_yys;
YYSTYPE yy_yyv[YYMAXDEPTH], *yyv = yy_yyv;
#else	/* user does initial allocation */
int *yys;
YYSTYPE *yyv;
#endif
static int yymaxdepth = YYMAXDEPTH;
# define YYERRCODE 256

# line 2220 "idl.yy"

/* programs */

/*
 * ???
 */
int
yywrap()
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
yyerror(char *)
{
}
yytabelem yyexca[] ={
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
-1, 23,
	123, 32,
	-2, 57,
-1, 131,
	91, 228,
	-2, 137,
-1, 146,
	257, 253,
	262, 253,
	263, 253,
	264, 253,
	265, 253,
	266, 253,
	267, 253,
	268, 253,
	269, 253,
	270, 253,
	271, 253,
	277, 253,
	283, 240,
	286, 253,
	298, 253,
	125, 25,
	-2, 0,
-1, 178,
	125, 3,
	-2, 0,
-1, 219,
	125, 244,
	-2, 0,
-1, 266,
	125, 160,
	-2, 0,
-1, 317,
	41, 256,
	-2, 258,
-1, 355,
	125, 183,
	-2, 0,
	};
# define YYNPROD 285
# define YYLAST 524
yytabelem yyact[]={

    68,   128,   152,   329,   342,   125,    53,   164,    66,   238,
   345,   224,    83,   227,   223,   129,   107,    74,    84,    85,
    72,    73,    75,    76,    78,    77,    79,    19,    20,   391,
    21,    86,    87,    88,    83,    98,   386,   100,   101,   102,
    55,    97,    95,   105,   350,   166,   326,    50,   161,    83,
   162,   204,   205,    69,    74,    84,    85,    72,    73,    75,
    76,    78,    77,    79,    19,    20,   236,    21,    86,    87,
    88,   144,    83,   166,   344,    69,    63,    74,    84,    85,
    72,    73,    75,    76,    78,    77,    79,   331,   332,   333,
    69,    86,    87,    88,   347,   346,   109,   109,   347,   346,
   110,   115,   116,    62,    61,    59,   138,   131,   127,   134,
    92,    83,   126,    69,   228,    83,   302,    99,   219,   140,
    74,    84,    85,    72,    73,    75,    76,    78,    77,    79,
   158,   163,    58,    13,   150,    87,    13,    93,    91,    90,
    83,   198,   155,   151,   239,    74,    84,    85,    72,    73,
    75,    76,    78,    77,    79,   156,    69,    51,     6,   154,
    87,   365,     5,    57,    12,    83,     4,    12,   299,   214,
    74,    84,    85,    72,    73,    75,    76,    78,    77,   157,
   288,    69,   159,    55,    83,    87,    88,    55,   287,   298,
    84,    85,   262,   286,    75,    76,    78,    77,   240,   242,
   241,   229,    21,   189,   201,    14,    69,     2,    10,    56,
    11,    25,   323,    11,   145,   143,   142,   141,   103,    19,
    20,   202,    21,   309,   199,    69,    15,    55,   260,   192,
   111,   193,   194,     9,   160,    14,    18,    24,    10,   259,
   247,   218,   139,   114,   113,   131,   281,   382,   112,    19,
    20,   280,    21,   369,   310,   290,    15,   277,   276,   275,
   274,   273,    83,   272,   285,    48,    47,    46,   131,    45,
    44,    43,   384,   371,    55,    41,   206,   297,   207,    13,
   173,   174,   389,   352,   210,   240,   242,   241,   301,   208,
    83,   387,   324,   300,   209,   196,   195,   167,   168,   169,
   170,   171,   172,    69,   381,   186,   338,   327,   315,   185,
    12,    13,   311,   184,   258,   373,   351,   317,   225,   203,
   312,    60,   390,   314,    67,   167,   168,   169,   170,   171,
   172,    69,   380,   217,   361,   362,   315,   253,   254,   337,
   364,   266,    12,   340,   363,   249,   294,   240,   242,   241,
   353,   348,   339,   328,   322,    94,    11,   321,    96,   250,
   366,   248,   336,   372,    55,   131,   376,   375,   374,   370,
   126,   368,   222,   295,   293,   292,   289,   320,   316,   131,
   383,   308,   279,   263,   251,   252,   220,   282,    11,   177,
   283,   255,   256,   257,   119,    34,   211,   212,   213,   307,
   278,   303,   296,   246,   149,   216,   137,    82,   215,   136,
    81,   175,   117,   200,   135,   306,   271,   270,   226,   181,
   123,    38,   378,   379,   360,   359,   357,   377,   356,   355,
   354,   341,   334,   318,   305,   269,   180,   122,    37,   268,
   304,   267,   265,   221,   179,   121,    36,   335,   244,   106,
    49,    32,   261,   176,   118,    33,   133,   108,   243,   235,
   234,   188,   233,   187,   232,   231,   230,   183,   104,    42,
   182,   146,   124,    39,    17,    16,   264,   178,   120,    35,
    31,    30,     8,    29,     7,    28,    27,    26,     3,     1,
    23,   190,   130,   191,   330,    71,    70,    64,    89,   367,
   165,   153,   284,    22,   319,   313,   148,   358,   343,   245,
   197,   325,    40,   147,   388,   385,   349,    80,   237,   132,
   291,    65,    54,    52 };
yytabelem yypact[]={

   -23,-10000000,-10000000,   -23,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,   217,-10000000,-10000000,   212,   211,   210,   208,
   207,   206,  -208,   -92,  -146,  -140,  -146,  -146,  -146,    95,
-10000000,-10000000,  -146,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,  -282,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,  -165,-10000000,-10000000,-10000000,-10000000,-10000000,
   186,   184,   183,-10000000,-10000000,  -161,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -282,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,  -223,-10000000,  -146,-10000000,  -146,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,  -156,-10000000,   182,  -146,    94,
    93,    92,  -203,    91,-10000000,-10000000,  -282,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,  -146,-10000000,     5,     5,     5,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,   -53,   252,   251,   133,
-10000000,-10000000,-10000000,    80,   127,   281,  -248,   233,   247,-10000000,
-10000000,    33,    33,    33,  -282,-10000000,     5,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,  -185,   180,-10000000,   -23,  -245,
   278,  -143,    76,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
  -217,  -142,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
   178,     5,     5,     5,     5,     5,     5,     5,     5,     5,
     5,-10000000,-10000000,-10000000,   273,   177,   166,-10000000,-10000000,  -245,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
   204,   202,   201,   200,   199,   198,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,  -282,  -223,  -146,   133,     5,-10000000,   127,   281,
  -248,   233,   233,   247,   247,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,     5,-10000000,    68,    63,    55,  -245,  -146,   196,   -73,
    43,   249,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -117,  -141,
  -282,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,  -282,  -166,-10000000,
-10000000,-10000000,-10000000,   130,   195,   271,  -143,  -146,   277,-10000000,
-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,    89,   248,
  -244,   266,  -200,-10000000,-10000000,-10000000,-10000000,-10000000,   265,-10000000,
-10000000,-10000000,-10000000,-10000000,  -182,  -146,  -247,   276,-10000000,   239,
  -117,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,    36,  -182,  -208,   194,  -186,   215,
     5,   275,  -223,  -200,  -146,-10000000,-10000000,-10000000,-10000000,-10000000,
-10000000,-10000000,-10000000,-10000000,   263,-10000000,-10000000,   188,  -146,   214,
  -257,-10000000,-10000000,-10000000,-10000000,   250,-10000000,-10000000,   238,-10000000,
  -264,-10000000 };
yytabelem yypgo[]={

     0,    14,   157,   523,   522,   521,     8,   209,   132,   520,
   163,   519,   518,   517,   324,     9,     7,   516,   515,   514,
     5,   513,   512,   511,   510,   509,   508,   507,   108,   506,
   505,   504,   503,   502,     2,   501,   159,   142,   155,   179,
   130,   182,   234,   500,   143,   141,    10,   499,   498,   105,
   104,   103,   321,   497,   496,   495,     6,    76,   494,   493,
     1,    15,   492,   491,   490,     0,   489,   207,   488,   166,
   487,   162,   486,   158,   485,   484,   483,   482,   481,   480,
   479,   478,   477,   476,   475,   474,   473,   472,   471,   470,
   469,   468,   467,   466,   465,   464,   463,   462,   461,   460,
   459,   458,   457,   456,   455,   454,   453,   452,   451,   450,
   449,   448,   447,   446,   445,   444,   443,   442,   192,   118,
   441,   440,   439,   438,   437,   436,   435,   434,   433,   432,
   431,   430,     4,   429,   428,   427,   426,   425,   424,   423,
   422,   421,   420,   419,   418,   417,    13,   416,   415,   414,
   413,   412,   411,   410,   409,   408,   407,   406,   405,   404,
   403,   401,   400,   399,   395,   394,   389,   383,   382,   381,
   378,   377,   362,   357,   354,   353,     3,   352,   344,   343,
   340,   339,   335,   334,   332,   322 };
yytabelem yyr1[]={

     0,    66,    67,    67,    70,    68,    72,    68,    74,    68,
    76,    68,    78,    68,    79,    68,    80,    81,    82,    83,
    77,    75,    75,    86,    87,    89,    84,    90,    64,    32,
    91,    22,    22,    88,    88,    93,    92,    94,    92,    95,
    92,    97,    92,    99,    92,   100,    92,    20,   101,    21,
    21,    16,   102,    16,   103,    16,    65,    85,   104,   105,
   106,   107,    71,    48,    48,    48,    48,    48,    48,    48,
    48,    33,    34,    35,    35,    36,    36,    37,    37,    38,
    38,    38,    39,    39,    39,    40,    40,    40,    40,    41,
    41,    41,    41,    42,    42,    42,    43,    43,    43,    43,
    43,    43,    44,   108,    69,    69,    69,    69,   110,   109,
     1,     1,     2,     2,     2,    56,    56,    56,    56,    56,
    56,     4,     4,     4,     3,     3,     3,    28,   111,    29,
    29,    60,    60,    30,   112,    31,    31,    61,    62,    49,
    49,    54,    54,    54,    55,    55,    55,    52,    52,    52,
    50,    50,    57,    51,    53,   113,   114,   115,   117,     7,
   116,   119,   119,   120,   121,   118,   122,   118,   123,   124,
   125,   126,   127,   128,   129,   131,    10,     9,     9,     9,
     9,     9,     9,   130,   133,   133,   134,   135,   132,   136,
   132,    26,    27,    27,   137,    46,   138,   139,    46,   140,
    47,   141,   142,   143,   145,     8,   144,   148,   147,   147,
   146,   149,   150,     5,     5,   151,   152,    13,   154,   155,
     6,     6,   153,   157,   158,    14,    14,   156,   159,    11,
    24,    25,    25,   160,   161,    45,   162,   163,    96,    63,
    63,   164,   165,   166,   167,    73,   168,   169,   171,   172,
    98,    59,    59,    59,    12,    12,   173,   170,   174,   170,
   175,   178,   177,   177,   179,   180,   176,    15,    15,    15,
    58,    58,    58,   181,   182,    23,    23,   183,   184,    17,
    17,    18,   185,    19,    19 };
yytabelem yyr2[]={

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
     3,     3,     3,     1,     6,     2,     2,     2,     1,     7,
     2,     2,     3,     2,     3,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     5,     1,     9,
     1,     2,     2,     5,     1,     9,     1,     3,     3,     2,
     2,     3,     5,     3,     5,     7,     5,     3,     3,     5,
     3,     3,     3,     3,     3,     1,     1,     1,     1,    19,
     4,     4,     0,     1,     1,    11,     1,     7,     1,     1,
     1,     1,     1,     1,     1,     1,    35,     3,     3,     3,
     3,     2,     3,     4,     4,     0,     1,     1,    11,     1,
     7,     5,     5,     1,     1,     7,     1,     1,    11,     1,
     7,     1,     1,     1,     1,    19,     4,     1,     8,     0,
     3,     1,     1,    13,     5,     1,     1,    11,     1,     1,
    13,     3,     3,     1,     1,    13,     3,     3,     1,     7,
     5,     5,     1,     1,     1,    11,     1,     1,    13,     3,
     1,     1,     1,     1,     1,    19,     1,     1,     1,     1,
    21,     3,     3,     1,     2,     3,     1,     7,     1,     9,
     4,     1,     8,     0,     1,     1,    11,     3,     2,     3,
     3,     3,     3,     1,     1,    13,     1,     1,     1,    13,
     1,     5,     1,     9,     1 };
yytabelem yychk[]={

-10000000,   -66,   -67,   -68,   -69,   -71,   -73,   -75,   -77,   256,
   261,    -7,   -10,    -8,   258,   279,   -84,   -85,   259,   272,
   273,   275,   -32,   -64,   260,   -67,   -70,   -72,   -74,   -76,
   -78,   -79,  -108,  -104,  -164,   -80,  -113,  -123,  -141,   -86,
   -22,    58,   -90,    59,    59,    59,    59,    59,    59,  -109,
    -1,    -2,    -3,   -56,    -4,   -16,    -7,   -10,    -8,   -49,
   -52,   -50,   -51,   -57,   -53,    -5,    -6,   -14,   -65,   298,
   -54,   -55,   265,   266,   262,   267,   268,   270,   269,   271,
   -13,  -153,  -156,   257,   263,   264,   276,   277,   278,   -48,
   -49,   -50,   -57,   -51,   -52,    -6,   -14,   -16,   -65,   257,
   -65,   -65,   -65,   123,   -91,   -65,  -110,   298,  -102,   262,
   265,    44,    62,    60,    60,   262,   263,  -151,  -105,  -165,
   -81,  -114,  -124,  -142,   -87,   -20,   -16,   -28,   -60,   -61,
   -62,   -65,   -11,  -103,   -65,  -149,  -154,  -157,   262,    60,
   -65,   123,   123,   123,   274,   123,   -88,   -21,   -29,  -159,
   -65,   -44,   -34,   -35,   -36,   -37,   -38,   -39,   -40,   -41,
   -42,    43,    45,   126,   -16,   -43,    40,   292,   293,   294,
   295,   296,   297,   -44,   -44,  -152,  -106,  -166,   -82,  -115,
  -125,  -143,   -89,   -92,   -69,   -71,   -73,   -96,   -98,   256,
   -63,   -59,   282,   284,   285,    44,    44,   -24,   -45,    91,
  -150,   124,    94,    38,   299,   300,    43,    45,    42,    47,
    37,   -42,   -42,   -42,   -34,  -155,  -158,    -2,    61,  -119,
   -67,  -116,  -118,    -1,   256,    40,  -144,  -146,   257,   125,
   -93,   -94,   -95,   -97,   -99,  -100,   283,   -12,   -15,   286,
   -56,    -6,   -16,  -101,  -111,   -25,  -160,    62,   -36,   -37,
   -38,   -39,   -39,   -40,   -40,   -41,   -41,   -41,    41,    62,
    62,  -107,  -118,  -167,   -83,  -117,  -119,  -120,  -122,  -126,
  -145,  -147,    59,    59,    59,    59,    59,    59,  -162,  -168,
   -16,   -60,   -45,   -44,   -33,   -34,   125,   125,   125,   -28,
    59,    -9,   -49,   -50,   -57,   -51,    -8,   -16,   262,   125,
    44,   -15,   257,  -161,  -121,  -127,  -148,  -163,  -169,    93,
    59,    41,  -146,   -30,   -61,   -65,  -170,    40,  -128,   -31,
  -171,  -173,  -174,   123,    44,   -23,   290,    41,  -175,  -176,
   -58,   287,   288,   289,  -129,  -112,  -172,  -181,    41,  -177,
  -179,  -130,  -132,   -26,   256,   -46,   281,   280,   -61,   -17,
   291,    40,    44,   -15,  -131,  -133,  -134,  -136,   -27,  -137,
  -138,  -183,  -182,  -178,  -180,   125,  -132,   -47,    -1,    59,
   -46,    58,   -34,    40,   -20,  -176,   -60,  -135,  -140,  -139,
  -184,    41,    59,   -60,    58,   -18,   293,    41,   -19,    44,
  -185,   293 };
yytabelem yydef[]={

    -2,    -2,     1,    -2,     4,     6,     8,    10,    12,    14,
   103,   105,   106,   107,    58,   241,    21,    22,    16,   155,
   168,   201,    23,    -2,    27,     2,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    29,    30,     0,     5,     7,     9,    11,    13,    15,   104,
   108,   110,   111,   112,   113,   114,   124,   125,   126,   115,
   116,   117,   118,   119,   120,   121,   122,   123,    51,    52,
   139,   140,   147,   148,   141,   150,   151,   153,   152,   154,
     0,   221,   226,    56,   143,     0,   215,   222,   227,    59,
    63,    64,    65,    66,    67,    68,    69,    70,   242,    17,
   156,   169,   202,    24,     0,    28,     0,    54,     0,   142,
   149,   211,   214,   218,   223,   144,   146,     0,     0,     0,
     0,     0,     0,     0,    34,    31,    50,   109,   130,   131,
   132,    -2,   138,     0,    53,     0,     0,     0,   145,   216,
    60,   243,    18,   157,   170,   203,    -2,    47,   127,     0,
    55,   212,   102,    72,    73,    75,    77,    79,    82,    85,
    89,     0,     0,     0,    93,    94,     0,    96,    97,    98,
    99,   100,   101,   219,   224,     0,     0,   162,    -2,     0,
     0,     0,     0,    33,    35,    37,    39,    41,    43,    45,
     0,     0,   239,   251,   252,    48,   128,   229,   232,   233,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,    90,    91,    92,     0,     0,     0,   217,    61,    -2,
    19,   158,   162,   163,   166,   171,   204,   209,   210,    26,
     0,     0,     0,     0,     0,     0,   236,   246,   254,   255,
   267,   268,   269,     0,     0,   230,     0,   213,    74,    76,
    78,    80,    81,    83,    84,    86,    87,    88,    95,   220,
   225,     0,   161,     0,     0,     0,    -2,     0,     0,     0,
     0,   206,    36,    38,    40,    42,    44,    46,     0,     0,
    49,   129,   231,   234,    62,    71,   245,    20,   159,   164,
   167,   172,   177,   178,   179,   180,   181,   182,   141,   205,
   207,   237,   247,     0,     0,     0,     0,     0,     0,   235,
   165,   173,   208,   238,   136,   137,   248,    -2,     0,   133,
   276,     0,     0,   174,   134,   249,   273,   257,     0,   263,
   264,   270,   271,   272,     0,     0,   280,     0,   259,   260,
     0,   175,   185,   186,   189,   193,   194,   196,   135,   250,
   277,   274,   261,   265,     0,    -2,     0,     0,   191,     0,
     0,     0,     0,     0,     0,   176,   184,   187,   199,   190,
   192,   195,   197,   278,     0,   262,   266,     0,     0,     0,
     0,   275,   188,   200,   198,     0,   284,   279,   281,   282,
     0,   283 };
typedef struct
#ifdef __cplusplus
	yytoktype
#endif
{ char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	1	/* allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"IDENTIFIER",	257,
	"CONST",	258,
	"MODULE",	259,
	"INTERFACE",	260,
	"TYPEDEF",	261,
	"LONG",	262,
	"SHORT",	263,
	"UNSIGNED",	264,
	"DOUBLE",	265,
	"FLOAT",	266,
	"CHAR",	267,
	"WCHAR",	268,
	"OCTET",	269,
	"BOOLEAN",	270,
	"ANY",	271,
	"STRUCT",	272,
	"UNION",	273,
	"SWITCH",	274,
	"ENUM",	275,
	"SEQUENCE",	276,
	"STRING",	277,
	"WSTRING",	278,
	"EXCEPTION",	279,
	"CASE",	280,
	"DEFAULT",	281,
	"READONLY",	282,
	"ATTRIBUTE",	283,
	"ONEWAY",	284,
	"IDEMPOTENT",	285,
	"VOID",	286,
	"IN",	287,
	"OUT",	288,
	"INOUT",	289,
	"RAISES",	290,
	"CONTEXT",	291,
	"INTEGER_LITERAL",	292,
	"STRING_LITERAL",	293,
	"CHARACTER_LITERAL",	294,
	"FLOATING_PT_LITERAL",	295,
	"TRUETOK",	296,
	"FALSETOK",	297,
	"SCOPE_DELIMITOR",	298,
	"LEFT_SHIFT",	299,
	"RIGHT_SHIFT",	300,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
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
	"module : MODULE",
	"module : MODULE IDENTIFIER",
	"module : MODULE IDENTIFIER '{'",
	"module : MODULE IDENTIFIER '{' definitions",
	"module : MODULE IDENTIFIER '{' definitions '}'",
	"interface_def : interface",
	"interface_def : forward",
	"interface : interface_header",
	"interface : interface_header '{'",
	"interface : interface_header '{' exports",
	"interface : interface_header '{' exports '}'",
	"interface_decl : INTERFACE",
	"interface_decl : INTERFACE id",
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
	"scoped_name : SCOPE_DELIMITOR",
	"scoped_name : SCOPE_DELIMITOR id",
	"scoped_name : scoped_name SCOPE_DELIMITOR",
	"scoped_name : scoped_name SCOPE_DELIMITOR id",
	"id : IDENTIFIER",
	"forward : interface_decl",
	"const_dcl : CONST",
	"const_dcl : CONST const_type",
	"const_dcl : CONST const_type id",
	"const_dcl : CONST const_type id '='",
	"const_dcl : CONST const_type id '=' expression",
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
	"shift_expr : shift_expr LEFT_SHIFT add_expr",
	"shift_expr : shift_expr RIGHT_SHIFT add_expr",
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
	"literal : INTEGER_LITERAL",
	"literal : STRING_LITERAL",
	"literal : CHARACTER_LITERAL",
	"literal : FLOATING_PT_LITERAL",
	"literal : TRUETOK",
	"literal : FALSETOK",
	"positive_int_expr : const_expr",
	"type_dcl : TYPEDEF",
	"type_dcl : TYPEDEF type_declarator",
	"type_dcl : struct_type",
	"type_dcl : union_type",
	"type_dcl : enum_type",
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
	"signed_int : LONG",
	"signed_int : LONG LONG",
	"signed_int : SHORT",
	"unsigned_int : UNSIGNED LONG",
	"unsigned_int : UNSIGNED LONG LONG",
	"unsigned_int : UNSIGNED SHORT",
	"floating_pt_type : DOUBLE",
	"floating_pt_type : FLOAT",
	"floating_pt_type : LONG DOUBLE",
	"char_type : CHAR",
	"char_type : WCHAR",
	"octet_type : OCTET",
	"boolean_type : BOOLEAN",
	"any_type : ANY",
	"struct_type : STRUCT",
	"struct_type : STRUCT id",
	"struct_type : STRUCT id '{'",
	"struct_type : STRUCT id '{' at_least_one_member",
	"struct_type : STRUCT id '{' at_least_one_member '}'",
	"at_least_one_member : member members",
	"members : members member",
	"members : /* empty */",
	"member : type_spec",
	"member : type_spec at_least_one_declarator",
	"member : type_spec at_least_one_declarator ';'",
	"member : error",
	"member : error ';'",
	"union_type : UNION",
	"union_type : UNION id",
	"union_type : UNION id SWITCH",
	"union_type : UNION id SWITCH '('",
	"union_type : UNION id SWITCH '(' switch_type_spec",
	"union_type : UNION id SWITCH '(' switch_type_spec ')'",
	"union_type : UNION id SWITCH '(' switch_type_spec ')' '{'",
	"union_type : UNION id SWITCH '(' switch_type_spec ')' '{' at_least_one_case_branch",
	"union_type : UNION id SWITCH '(' switch_type_spec ')' '{' at_least_one_case_branch '}'",
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
	"case_label : DEFAULT",
	"case_label : DEFAULT ':'",
	"case_label : CASE",
	"case_label : CASE const_expr",
	"case_label : CASE const_expr ':'",
	"element_spec : type_spec",
	"element_spec : type_spec declarator",
	"enum_type : ENUM",
	"enum_type : ENUM id",
	"enum_type : ENUM id '{'",
	"enum_type : ENUM id '{' at_least_one_enumerator",
	"enum_type : ENUM id '{' at_least_one_enumerator '}'",
	"at_least_one_enumerator : enumerator enumerators",
	"enumerators : enumerators ','",
	"enumerators : enumerators ',' enumerator",
	"enumerators : /* empty */",
	"enumerator : IDENTIFIER",
	"sequence_type_spec : seq_head ','",
	"sequence_type_spec : seq_head ',' positive_int_expr",
	"sequence_type_spec : seq_head ',' positive_int_expr '>'",
	"sequence_type_spec : seq_head '>'",
	"seq_head : SEQUENCE",
	"seq_head : SEQUENCE '<'",
	"seq_head : SEQUENCE '<' simple_type_spec",
	"string_type_spec : string_head '<'",
	"string_type_spec : string_head '<' positive_int_expr",
	"string_type_spec : string_head '<' positive_int_expr '>'",
	"string_type_spec : string_head",
	"string_head : STRING",
	"wstring_type_spec : wstring_head '<'",
	"wstring_type_spec : wstring_head '<' positive_int_expr",
	"wstring_type_spec : wstring_head '<' positive_int_expr '>'",
	"wstring_type_spec : wstring_head",
	"wstring_head : WSTRING",
	"array_declarator : id",
	"array_declarator : id at_least_one_array_dim",
	"at_least_one_array_dim : array_dim array_dims",
	"array_dims : array_dims array_dim",
	"array_dims : /* empty */",
	"array_dim : '['",
	"array_dim : '[' positive_int_expr",
	"array_dim : '[' positive_int_expr ']'",
	"attribute : opt_readonly ATTRIBUTE",
	"attribute : opt_readonly ATTRIBUTE param_type_spec",
	"attribute : opt_readonly ATTRIBUTE param_type_spec at_least_one_simple_declarator",
	"opt_readonly : READONLY",
	"opt_readonly : /* empty */",
	"exception : EXCEPTION",
	"exception : EXCEPTION id",
	"exception : EXCEPTION id '{'",
	"exception : EXCEPTION id '{' members",
	"exception : EXCEPTION id '{' members '}'",
	"operation : opt_op_attribute op_type_spec",
	"operation : opt_op_attribute op_type_spec IDENTIFIER",
	"operation : opt_op_attribute op_type_spec IDENTIFIER parameter_list",
	"operation : opt_op_attribute op_type_spec IDENTIFIER parameter_list opt_raises",
	"operation : opt_op_attribute op_type_spec IDENTIFIER parameter_list opt_raises opt_context",
	"opt_op_attribute : ONEWAY",
	"opt_op_attribute : IDEMPOTENT",
	"opt_op_attribute : /* empty */",
	"op_type_spec : param_type_spec",
	"op_type_spec : VOID",
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
	"direction : IN",
	"direction : OUT",
	"direction : INOUT",
	"opt_raises : RAISES",
	"opt_raises : RAISES '('",
	"opt_raises : RAISES '(' at_least_one_scoped_name ')'",
	"opt_raises : /* empty */",
	"opt_context : CONTEXT",
	"opt_context : CONTEXT '('",
	"opt_context : CONTEXT '(' at_least_one_string_literal ')'",
	"opt_context : /* empty */",
	"at_least_one_string_literal : STRING_LITERAL string_literals",
	"string_literals : string_literals ','",
	"string_literals : string_literals ',' STRING_LITERAL",
	"string_literals : /* empty */",
};
#endif /* YYDEBUG */
# line	1 "/usr/ccs/bin/yaccpar"
/*
 * Copyright (c) 1993 by Sun Microsystems, Inc.
 */

#pragma ident	"@(#)yaccpar	6.12	93/06/07 SMI"

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	return(0)
#define YYABORT		return(1)
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#define YYNEW(type)	malloc(sizeof(type) * yynewmax)
#define YYCOPY(to, from, type) \
	(type *) memcpy(to, (char *) from, yynewmax * sizeof(type))
#define YYENLARGE( from, type) \
	(type *) realloc((char *) from, yynewmax * sizeof(type))
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-10000000)

/*
** global variables used by the parser
*/
YYSTYPE *yypv;			/* top of value stack */
int *yyps;			/* top of state stack */

int yystate;			/* current state */
int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */
int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */



#ifdef YYNMBCHARS
#define YYLEX()		yycvtok(yylex())
/*
** yycvtok - return a token if i is a wchar_t value that exceeds 255.
**	If i<255, i itself is the token.  If i>255 but the neither 
**	of the 30th or 31st bit is on, i is already a token.
*/
#if defined(__STDC__) || defined(__cplusplus)
int yycvtok(int i)
#else
int yycvtok(i) int i;
#endif
{
	int first = 0;
	int last = YYNMBCHARS - 1;
	int mid;
	wchar_t j;

	if(i&0x60000000){/*Must convert to a token. */
		if( yymbchars[last].character < i ){
			return i;/*Giving up*/
		}
		while ((last>=first)&&(first>=0)) {/*Binary search loop*/
			mid = (first+last)/2;
			j = yymbchars[mid].character;
			if( j==i ){/*Found*/ 
				return yymbchars[mid].tvalue;
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
#else/*!YYNMBCHARS*/
#define YYLEX()		yylex()
#endif/*!YYNMBCHARS*/

/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
#if defined(__STDC__) || defined(__cplusplus)
int yyparse(void)
#else
int yyparse()
#endif
{
	register YYSTYPE *yypvt;	/* top of value stack for $vars */

#if defined(__cplusplus) || defined(lint)
/*
	hacks to please C++ and lint - goto's inside switch should never be
	executed; yypvt is set to 0 to avoid "used before set" warning.
*/
	static int __yaccpar_lint_hack__ = 0;
	switch (__yaccpar_lint_hack__)
	{
		case 1: goto yyerrlab;
		case 2: goto yynewstate;
	}
	yypvt = 0;
#endif

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

#if YYMAXDEPTH <= 0
	if (yymaxdepth <= 0)
	{
		if ((yymaxdepth = YYEXPAND(0)) <= 0)
		{
			yyerror("yacc initialization error");
			YYABORT;
		}
	}
#endif

	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */
	goto yystack;	/* moved from 6 lines above to here to please C++ */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ yymaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			int yyps_index = (yy_ps - yys);
			int yypv_index = (yy_pv - yyv);
			int yypvt_index = (yypvt - yyv);
			int yynewmax;
#ifdef YYEXPAND
			yynewmax = YYEXPAND(yymaxdepth);
#else
			yynewmax = 2 * yymaxdepth;	/* double table size */
			if (yymaxdepth == YYMAXDEPTH)	/* first time growth */
			{
				char *newyys = (char *)YYNEW(int);
				char *newyyv = (char *)YYNEW(YYSTYPE);
				if (newyys != 0 && newyyv != 0)
				{
					yys = YYCOPY(newyys, yys, int);
					yyv = YYCOPY(newyyv, yyv, YYSTYPE);
				}
				else
					yynewmax = 0;	/* failed */
			}
			else				/* not first time */
			{
				yys = YYENLARGE(yys, int);
				yyv = YYENLARGE(yyv, YYSTYPE);
				if (yys == 0 || yyv == 0)
					yynewmax = 0;	/* failed */
			}
#endif
			if (yynewmax <= yymaxdepth)	/* tables not expanded */
			{
				yyerror( "yacc stack overflow" );
				YYABORT;
			}
			yymaxdepth = yynewmax;

			yy_ps = yys + yyps_index;
			yy_pv = yyv + yypv_index;
			yypvt = yyv + yypvt_index;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			printf( "Received token " );
			if ( yychar == 0 )
				printf( "end-of-file\n" );
			else if ( yychar < 0 )
				printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = YYLEX() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				printf( "Received token " );
				if ( yychar == 0 )
					printf( "end-of-file\n" );
				else if ( yychar < 0 )
					printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
			skip_init:
				yynerrs++;
				/* FALLTHRU */
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					printf( "Error recovery discards " );
					if ( yychar == 0 )
						printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 4:
# line 230 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        } break;
case 5:
# line 234 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 6:
# line 238 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        } break;
case 7:
# line 242 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 8:
# line 246 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        } break;
case 9:
# line 250 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 10:
# line 254 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceDeclSeen);
        } break;
case 11:
# line 258 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 12:
# line 262 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ModuleDeclSeen);
        } break;
case 13:
# line 266 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 14:
# line 270 "idl.yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	} break;
case 15:
# line 274 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  yyerrok;
	} break;
case 16:
# line 281 "idl.yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSeen);
	  } break;
case 17:
# line 285 "idl.yy"
{
	    UTL_ScopedName 	*n =
		new UTL_ScopedName(new Identifier(yypvt[-0].strval, 1, 0, I_FALSE), NULL);
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
# line 306 "idl.yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSqSeen);
	  } break;
case 19:
# line 310 "idl.yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleBodySeen);
	  } break;
case 20:
# line 314 "idl.yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleQsSeen);
	    /*
	     * Finished with this module - pop it from the scope stack
	     */
	    idl_global->scopes()->pop();
	  } break;
case 23:
# line 330 "idl.yy"
{
	  UTL_Scope     *s = idl_global->scopes()->top_non_null();
	  AST_Interface *i = NULL;
	  AST_Decl	*v = NULL;
	  UTL_StrList   *p = idl_global->pragmas();
	  AST_Decl	*d = NULL;
	  AST_Interface *fd = NULL;

	  /*
	   * Make a new interface node and add it to its enclosing scope
	   */
	  if (s != NULL && yypvt[-0].ihval != NULL) {
	    i = idl_global->gen()->create_interface(yypvt[-0].ihval->interface_name(),
						    yypvt[-0].ihval->inherits(),
						    yypvt[-0].ihval->n_inherits(),
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
		    fd->set_inherits(yypvt[-0].ihval->inherits());
		    fd->set_n_inherits(yypvt[-0].ihval->n_inherits());
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
# line 412 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSqSeen);
	} break;
case 25:
# line 416 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceBodySeen);
	} break;
case 26:
# line 420 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceQsSeen);
	  /*
	   * Done with this interface - pop it off the scopes stack
	   */
	  idl_global->scopes()->pop();
	} break;
case 27:
# line 431 "idl.yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSeen);
	 } break;
case 28:
# line 435 "idl.yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceIDSeen);
	   yyval.idval = yypvt[-0].idval;
	 } break;
case 29:
# line 443 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InheritSpecSeen);
	  /*
	   * Create an AST representation of the information in the header
	   * part of an interface - this representation contains a computed
	   * list of all interfaces which this interface inherits from,
	   * recursively
	   */
	  yyval.ihval = new FE_InterfaceHeader(new UTL_ScopedName(yypvt[-1].idval, NULL), yypvt[-0].nlval);
	} break;
case 30:
# line 457 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InheritColonSeen);
        } break;
case 31:
# line 461 "idl.yy"
{
	  yyval.nlval = yypvt[-0].nlval;
	} break;
case 32:
# line 465 "idl.yy"
{
	  yyval.nlval = NULL;
	} break;
case 35:
# line 477 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        } break;
case 36:
# line 481 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 37:
# line 485 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        } break;
case 38:
# line 489 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 39:
# line 493 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        } break;
case 40:
# line 497 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 41:
# line 501 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrDeclSeen);
        } break;
case 42:
# line 505 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 43:
# line 509 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpDeclSeen);
        } break;
case 44:
# line 513 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        } break;
case 45:
# line 517 "idl.yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	} break;
case 46:
# line 521 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  yyerrok;
	} break;
case 47:
# line 529 "idl.yy"
{
	  yyval.nlval = new UTL_NameList(yypvt[-1].idlist, yypvt[-0].nlval);
	} break;
case 48:
# line 537 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SNListCommaSeen);
        } break;
case 49:
# line 541 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopedNameSeen);

	  if (yypvt[-3].nlval == NULL)
	    yyval.nlval = new UTL_NameList(yypvt[-0].idlist, NULL);
	  else {
	    yypvt[-3].nlval->nconc(new UTL_NameList(yypvt[-0].idlist, NULL));
	    yyval.nlval = yypvt[-3].nlval;
	  }
	} break;
case 50:
# line 552 "idl.yy"
{
	  yyval.nlval = NULL;
	} break;
case 51:
# line 559 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  yyval.idlist = new UTL_IdList(yypvt[-0].idval, NULL);
	} break;
case 52:
# line 565 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        } break;
case 53:
# line 569 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  yyval.idlist = new UTL_IdList(new Identifier(yypvt[-2].strval, 1, 0, I_FALSE),
			      new UTL_IdList(yypvt[-0].idval, NULL));
	} break;
case 54:
# line 577 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        } break;
case 55:
# line 581 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  yypvt[-3].idlist->nconc(new UTL_IdList(yypvt[-0].idval, NULL));
	  yyval.idlist = yypvt[-3].idlist;
	} break;
case 56:
# line 590 "idl.yy"
{
            yyval.idval = new Identifier(yypvt[-0].strval, 1, 0, I_FALSE);
        } break;
case 57:
# line 597 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(yypvt[-0].idval, NULL);
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
# line 617 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstSeen);
        } break;
case 59:
# line 621 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstTypeSeen);
        } break;
case 60:
# line 625 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstIDSeen);
        } break;
case 61:
# line 629 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstAssignSeen);
        } break;
case 62:
# line 633 "idl.yy"
{
	  UTL_ScopedName	*n = new UTL_ScopedName(yypvt[-4].idval, NULL);
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Constant		*c = NULL;
	  UTL_StrList		*p = idl_global->pragmas();
	  AST_Decl		*v = NULL;

	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstExprSeen);
	  /*
	   * Create a node representing a constant declaration. Store
	   * it in the enclosing scope
	   */
	  if (yypvt[-0].exval != NULL && s != NULL) {
	    if (yypvt[-0].exval->coerce(yypvt[-6].etval) == NULL)
	      idl_global->err()->coercion_error(yypvt[-0].exval, yypvt[-6].etval);
	    else {
	      c =
		idl_global->gen()->create_constant(yypvt[-6].etval, yypvt[-0].exval, n, p);
	      (void) s->fe_add_constant(c);
	    }
	  }
	} break;
case 68:
# line 664 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_string;
	} break;
case 69:
# line 668 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_wstring;
        } break;
case 70:
# line 672 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Decl		*d = NULL;
	  AST_PredefinedType	*c = NULL;
	  AST_Typedef		*t = NULL;

	  /*
	   * If the constant's type is a scoped name, it must resolve
	   * to a scalar constant type
	   */
	  if (s != NULL && (d = s->lookup_by_name(yypvt[-0].idlist, I_TRUE)) != NULL) {
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
	      yyval.etval = AST_Expression::EV_any;
	    else if (d->node_type() == AST_Decl::NT_pre_defined) {
	      c = AST_PredefinedType::narrow_from_decl(d);
	      if (c != NULL) {
	         yyval.etval = idl_global->PredefinedTypeToExprType(c->pt());
	      } else {
	         yyval.etval = AST_Expression::EV_any;
	      }
	    } else
	      yyval.etval = AST_Expression::EV_any;
	  } else
	    yyval.etval = AST_Expression::EV_any;
	} break;
case 74:
# line 714 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_or, yypvt[-2].exval, yypvt[-0].exval);
	} break;
case 76:
# line 722 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_xor, yypvt[-2].exval, yypvt[-0].exval);
	} break;
case 78:
# line 730 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_and, yypvt[-2].exval, yypvt[-0].exval);
	} break;
case 80:
# line 738 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_right,yypvt[-2].exval,yypvt[-0].exval);
	} break;
case 81:
# line 742 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_left,yypvt[-2].exval,yypvt[-0].exval);
	} break;
case 83:
# line 750 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_add, yypvt[-2].exval, yypvt[-0].exval);
	} break;
case 84:
# line 754 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_minus,yypvt[-2].exval,yypvt[-0].exval);
	} break;
case 86:
# line 762 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_mul, yypvt[-2].exval, yypvt[-0].exval);
	} break;
case 87:
# line 766 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_div, yypvt[-2].exval, yypvt[-0].exval);
	} break;
case 88:
# line 770 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_mod, yypvt[-2].exval, yypvt[-0].exval);
	} break;
case 90:
# line 778 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_u_plus,
					      yypvt[-0].exval,
					      NULL);
	} break;
case 91:
# line 784 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_u_minus,
					      yypvt[-0].exval,
					      NULL);
	} break;
case 92:
# line 790 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_bit_neg,
					      yypvt[-0].exval,
					      NULL);
	} break;
case 93:
# line 799 "idl.yy"
{
	  /*
	   * An expression which is a scoped name is not resolved now,
	   * but only when it is evaluated (such as when it is assigned
	   * as a constant value)
	   */
	  yyval.exval = idl_global->gen()->create_expr(yypvt[-0].idlist);
	} break;
case 95:
# line 809 "idl.yy"
{
	  yyval.exval = yypvt[-1].exval;
	} break;
case 96:
# line 816 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(yypvt[-0].ival);
	} break;
case 97:
# line 820 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(yypvt[-0].sval);
	} break;
case 98:
# line 824 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(yypvt[-0].cval);
	} break;
case 99:
# line 828 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(yypvt[-0].dval);
	} break;
case 100:
# line 832 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr((idl_bool) I_TRUE,
					    AST_Expression::EV_bool);
	} break;
case 101:
# line 837 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr((idl_bool) I_FALSE,
					    AST_Expression::EV_bool);
	} break;
case 102:
# line 845 "idl.yy"
{
	    yypvt[-0].exval->evaluate(AST_Expression::EK_const);
	    yyval.exval = idl_global->gen()->create_expr(yypvt[-0].exval, AST_Expression::EV_ulong);
	} break;
case 103:
# line 853 "idl.yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_TypedefSeen);
	  } break;
case 108:
# line 864 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeSpecSeen);
        } break;
case 109:
# line 868 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_DecllistActiveIterator *l;
	  FE_Declarator		*d = NULL;
	  AST_Typedef		*t = NULL;
	  AST_Decl		*v = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclaratorsSeen);
	  /*
	   * Create a list of type renamings. Add them to the
	   * enclosing scope
	   */
	  if (s != NULL && yypvt[-2].dcval != NULL && yypvt[-0].dlval != NULL) {
	    l = new UTL_DecllistActiveIterator(yypvt[-0].dlval);
	    for (;!(l->is_done()); l->next()) {
	      d = l->item();
	      if (d == NULL) 
		continue;
              AST_Type * tp = d->compose(yypvt[-2].dcval);
              if (tp == NULL) 
		continue;
	      t = idl_global->gen()->create_typedef(tp, d->name(), p);
	      (void) s->fe_add_typedef(t);
	    }
	    delete l;
	  }
	} break;
case 112:
# line 905 "idl.yy"
{
	  yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(yypvt[-0].etval);
	} break;
case 114:
# line 910 "idl.yy"
{
	  UTL_Scope	*s = idl_global->scopes()->top_non_null();
	  AST_Decl	*d = NULL;

	  if (s != NULL)
	    d = s->lookup_by_name(yypvt[-0].idlist, I_TRUE);
	  if (d == NULL)
	    idl_global->err()->lookup_error(yypvt[-0].idlist);
	  yyval.dcval = d;
	} break;
case 127:
# line 945 "idl.yy"
{
	  yyval.dlval = new UTL_DeclList(yypvt[-1].deval, yypvt[-0].dlval);
	} break;
case 128:
# line 953 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        } break;
case 129:
# line 957 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsDeclSeen);

	  if (yypvt[-3].dlval == NULL)
	    yyval.dlval = new UTL_DeclList(yypvt[-0].deval, NULL);
	  else {
	    yypvt[-3].dlval->nconc(new UTL_DeclList(yypvt[-0].deval, NULL));
	    yyval.dlval = yypvt[-3].dlval;
	  }
	} break;
case 130:
# line 968 "idl.yy"
{
	  yyval.dlval = NULL;
	} break;
case 133:
# line 980 "idl.yy"
{
	  yyval.dlval = new UTL_DeclList(yypvt[-1].deval, yypvt[-0].dlval);
	} break;
case 134:
# line 988 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        } break;
case 135:
# line 992 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsDeclSeen);

	  if (yypvt[-3].dlval == NULL)
	    yyval.dlval = new UTL_DeclList(yypvt[-0].deval, NULL);
	  else {
	    yypvt[-3].dlval->nconc(new UTL_DeclList(yypvt[-0].deval, NULL));
	    yyval.dlval = yypvt[-3].dlval;
	  }
	} break;
case 136:
# line 1003 "idl.yy"
{
	  yyval.dlval = NULL;
	} break;
case 137:
# line 1010 "idl.yy"
{
	  yyval.deval = new FE_Declarator(new UTL_ScopedName(yypvt[-0].idval, NULL),
				 FE_Declarator::FD_simple, NULL);
	} break;
case 138:
# line 1018 "idl.yy"
{
	  yyval.deval = new FE_Declarator(new UTL_ScopedName(yypvt[-0].dcval->local_name(), NULL),
				 FE_Declarator::FD_complex,
				 yypvt[-0].dcval);
	} break;
case 141:
# line 1032 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_long;
	} break;
case 142:
# line 1036 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_longlong;
        } break;
case 143:
# line 1040 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_short;
	} break;
case 144:
# line 1047 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_ulong;
	} break;
case 145:
# line 1051 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_ulonglong;
        } break;
case 146:
# line 1055 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_ushort;
	} break;
case 147:
# line 1062 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_double;
	} break;
case 148:
# line 1066 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_float;
	} break;
case 149:
# line 1070 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_longdouble;
        } break;
case 150:
# line 1077 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_char;
	} break;
case 151:
# line 1081 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_wchar;
        } break;
case 152:
# line 1088 "idl.yy"
{ 
          yyval.etval = AST_Expression::EV_octet;
	} break;
case 153:
# line 1095 "idl.yy"
{ 
	  yyval.etval = AST_Expression::EV_bool;
        } break;
case 154:
# line 1102 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_any;
	} break;
case 155:
# line 1109 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructSeen);
        } break;
case 156:
# line 1113 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(yypvt[-0].idval, NULL);
	  AST_Structure		*d = NULL;
	  UTL_StrList		*p = idl_global->pragmas();
	  AST_Decl		*v = NULL;

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
case 157:
# line 1135 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructSqSeen);
        } break;
case 158:
# line 1139 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructBodySeen);
        } break;
case 159:
# line 1143 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructQsSeen);
	  /*
	   * Done with this struct. Pop its scope off the scopes stack
	   */
	  if (idl_global->scopes()->top() == NULL)
	    yyval.dcval = NULL;
	  else {
	    yyval.dcval =
	      AST_Structure::narrow_from_scope(
				   idl_global->scopes()->top_non_null());
	    idl_global->scopes()->pop();
	  }
	} break;
case 163:
# line 1168 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberTypeSeen);
        } break;
case 164:
# line 1172 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberDeclsSeen);
        } break;
case 165:
# line 1176 "idl.yy"
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
	  if (yypvt[-4].dcval != NULL && AST_illegal_recursive_type(yypvt[-4].dcval))
	    idl_global->err()->error1(UTL_Error::EIDL_RECURSIVE_TYPE, yypvt[-4].dcval);
	  /*
	   * Create a node representing a struct or exception member
	   * Add it to the enclosing scope
	   */
	  else if (s != NULL && yypvt[-4].dcval != NULL && yypvt[-2].dlval != NULL) {
	    l = new UTL_DecllistActiveIterator(yypvt[-2].dlval);
	    for (;!(l->is_done()); l->next()) {
	      d = l->item();
	      if (d == NULL) 
		continue;
 	      AST_Type *tp = d->compose(yypvt[-4].dcval);
	      if (tp == NULL) 
		continue;
	      f = idl_global->gen()->create_field(tp, d->name(), p);
	      (void) s->fe_add_field(f);
	    }
	    delete l;
	  }
	} break;
case 166:
# line 1209 "idl.yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	} break;
case 167:
# line 1213 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  yyerrok;
	} break;
case 168:
# line 1221 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionSeen);
        } break;
case 169:
# line 1225 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionIDSeen);
        } break;
case 170:
# line 1229 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchSeen);
        } break;
case 171:
# line 1233 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchOpenParSeen);
        } break;
case 172:
# line 1237 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchTypeSeen);
        } break;
case 173:
# line 1241 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(yypvt[-8].idval, NULL);
	  AST_Union		*u = NULL;
	  AST_Decl		*v = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchCloseParSeen);
	  /*
	   * Create a node representing a union. Add it to its enclosing
	   * scope
	   */
	  if (yypvt[-2].dcval != NULL && s != NULL) {
 	    AST_ConcreteType    *tp = AST_ConcreteType::narrow_from_decl(yypvt[-2].dcval);
            if (tp == NULL) {
              idl_global->err()->not_a_type(yypvt[-2].dcval);
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
case 174:
# line 1268 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionSqSeen);
        } break;
case 175:
# line 1272 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionBodySeen);
        } break;
case 176:
# line 1276 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionQsSeen);
	  /*
	   * Done with this union. Pop its scope from the scopes stack
	   */
	  if (idl_global->scopes()->top() == NULL)
	    yyval.dcval = NULL;
	  else {
	    yyval.dcval =
	      AST_Union::narrow_from_scope(
				idl_global->scopes()->top_non_null());
	    idl_global->scopes()->pop();
	  }
	} break;
case 177:
# line 1294 "idl.yy"
{
	  yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(yypvt[-0].etval);
	} break;
case 178:
# line 1298 "idl.yy"
{
	  yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(yypvt[-0].etval);
	} break;
case 179:
# line 1302 "idl.yy"
{
	  yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(yypvt[-0].etval);
	} break;
case 180:
# line 1306 "idl.yy"
{
	  yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(yypvt[-0].etval);
	} break;
case 182:
# line 1311 "idl.yy"
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
	  if (s != NULL && (d = s->lookup_by_name(yypvt[-0].idlist, I_TRUE)) != NULL) {
	    while (!found) {
	      switch (d->node_type()) {
	      case AST_Decl::NT_enum:
		yyval.dcval = d;
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
		  case AST_PredefinedType::PT_octet:
		  case AST_PredefinedType::PT_boolean:
		    yyval.dcval = p;
		    found = I_TRUE;
		    break;
		  default:
		    yyval.dcval = NULL;
		    found = I_TRUE;
		    break;
		  }
		}
		break;
	      case AST_Decl::NT_typedef:
		t = AST_Typedef::narrow_from_decl(d);
		if (t != NULL) d = t->base_type();
		break;
	      default:
		yyval.dcval = NULL;
		found = I_TRUE;
		break;
	      }
	    }
	  } else
	    yyval.dcval = NULL;

	  if (yyval.dcval == NULL)
	    idl_global->err()->lookup_error(yypvt[-0].idlist);
	} break;
case 186:
# line 1381 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionLabelSeen);
        } break;
case 187:
# line 1385 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemSeen);
        } break;
case 188:
# line 1389 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_LabellistActiveIterator *l = NULL;
	  AST_UnionLabel	*d = NULL;
	  AST_UnionBranch	*b = NULL;
	  AST_Field		*f = yypvt[-2].ffval;

	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemCompleted);
	  /*
	   * Create several nodes representing branches of a union.
	   * Add them to the enclosing scope (the union scope)
	   */
	  if (s != NULL && yypvt[-4].llval != NULL && yypvt[-2].ffval != NULL) {
	    l = new UTL_LabellistActiveIterator(yypvt[-4].llval);
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
case 189:
# line 1417 "idl.yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	} break;
case 190:
# line 1421 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  yyerrok;
	} break;
case 191:
# line 1429 "idl.yy"
{
	  yyval.llval = new UTL_LabelList(yypvt[-1].ulval, yypvt[-0].llval);
	} break;
case 192:
# line 1436 "idl.yy"
{
	  if (yypvt[-1].llval == NULL)
	    yyval.llval = new UTL_LabelList(yypvt[-0].ulval, NULL);
	  else {
	    yypvt[-1].llval->nconc(new UTL_LabelList(yypvt[-0].ulval, NULL));
	    yyval.llval = yypvt[-1].llval;
	  }
	} break;
case 193:
# line 1445 "idl.yy"
{
	  yyval.llval = NULL;
	} break;
case 194:
# line 1452 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DefaultSeen);
        } break;
case 195:
# line 1456 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

	  yyval.ulval = idl_global->gen()->
	            create_union_label(AST_UnionLabel::UL_default,
				       NULL);
	} break;
case 196:
# line 1464 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_CaseSeen);
        } break;
case 197:
# line 1468 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelExprSeen);
        } break;
case 198:
# line 1472 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

	  yyval.ulval = idl_global->gen()->create_union_label(AST_UnionLabel::UL_label,
						     yypvt[-2].exval);
	} break;
case 199:
# line 1482 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemTypeSeen);
        } break;
case 200:
# line 1486 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemDeclSeen);
	  /*
	   * Check for illegal recursive use of type
	   */
	  if (yypvt[-2].dcval != NULL && AST_illegal_recursive_type(yypvt[-2].dcval))
	    idl_global->err()->error1(UTL_Error::EIDL_RECURSIVE_TYPE, yypvt[-2].dcval);
	  /*
	   * Create a field in a union branch
	   */
	  else if (yypvt[-2].dcval == NULL || yypvt[-0].deval == NULL)
	    yyval.ffval = NULL;
	  else {
	    AST_Type *tp = yypvt[-0].deval->compose(yypvt[-2].dcval);
	    if (tp == NULL)
	      yyval.ffval = NULL;
 	    else
	      yyval.ffval = idl_global->gen()->create_field(tp,
			        		   yypvt[-0].deval->name(),
			        		   idl_global->pragmas());
	  }
	} break;
case 201:
# line 1512 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumSeen);
        } break;
case 202:
# line 1516 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(yypvt[-0].idval, NULL);
	  AST_Enum		*e = NULL;
	  AST_Decl		*v = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

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
case 203:
# line 1541 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumSqSeen);
        } break;
case 204:
# line 1545 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumBodySeen);
        } break;
case 205:
# line 1549 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumQsSeen);
	  /*
	   * Done with this enum. Pop its scope from the scopes stack
	   */
	  if (idl_global->scopes()->top() == NULL)
	    yyval.dcval = NULL;
	  else {
	    yyval.dcval = AST_Enum::narrow_from_scope(idl_global->scopes()->top_non_null());
	    idl_global->scopes()->pop();
	  }
	} break;
case 207:
# line 1568 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumCommaSeen);
        } break;
case 210:
# line 1577 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n =
		new UTL_ScopedName(new Identifier(yypvt[-0].strval, 1, 0, I_FALSE), NULL);
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
case 211:
# line 1601 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceCommaSeen);
        } break;
case 212:
# line 1605 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceExprSeen);
        } break;
case 213:
# line 1609 "idl.yy"
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
	  if (yypvt[-2].exval == NULL || yypvt[-2].exval->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error(yypvt[-2].exval, AST_Expression::EV_ulong);
	    yyval.dcval = NULL;
	  } else if (yypvt[-5].dcval == NULL) {
	    yyval.dcval = NULL;
	  } else {
 	    AST_Type *tp = AST_Type::narrow_from_decl(yypvt[-5].dcval); 
	    if (tp == NULL) 
	      yyval.dcval = NULL;
	    else {
	      yyval.dcval = idl_global->gen()->create_sequence(yypvt[-2].exval, tp);
	      /*
	       * Add this AST_Sequence to the types defined in the global scope
	       */
	      (void) idl_global->root()
		        ->fe_add_sequence(AST_Sequence::narrow_from_decl(yyval.dcval));
 	    }
	  }
	} break;
case 214:
# line 1640 "idl.yy"
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
	  if (yypvt[-1].dcval == NULL)
	    yyval.dcval = NULL;
	  else {
	    AST_Type *tp = AST_Type::narrow_from_decl(yypvt[-1].dcval);
	    if (tp == NULL)
	      yyval.dcval = NULL;
            else {
	      yyval.dcval =
	        idl_global->gen()->create_sequence(
		  	     idl_global->gen()->create_expr((unsigned long) 0),
			     tp);
	      /*
	       * Add this AST_Sequence to the types defined in the global scope
	       */
	      (void) idl_global->root()
		        ->fe_add_sequence(AST_Sequence::narrow_from_decl(yyval.dcval));
	    }
	  }
	} break;
case 215:
# line 1673 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSeen);
	  /*
	   * Push a sequence marker on scopes stack
	   */
	  idl_global->scopes()->push(NULL);
	} break;
case 216:
# line 1681 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSqSeen);
        } break;
case 217:
# line 1685 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceTypeSeen);
	  yyval.dcval = yypvt[-0].dcval;
        } break;
case 218:
# line 1694 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        } break;
case 219:
# line 1698 "idl.yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        } break;
case 220:
# line 1702 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringQsSeen);
	  /*
	   * Create a node representing a string
	   */
	  if (yypvt[-2].exval == NULL || yypvt[-2].exval->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error(yypvt[-2].exval, AST_Expression::EV_ulong);
	    yyval.dcval = NULL;
	  } else {
	    yyval.dcval = idl_global->gen()->create_string(yypvt[-2].exval);
	    /*
	     * Add this AST_String to the types defined in the global scope
	     */
	    (void) idl_global->root()
		      ->fe_add_string(AST_String::narrow_from_decl(yyval.dcval));
	  }
	} break;
case 221:
# line 1720 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringCompleted);
	  /*
	   * Create a node representing a string
	   */
	  yyval.dcval =
	    idl_global->gen()->create_string(
			 idl_global->gen()->create_expr((unsigned long) 0));
	  /*
	   * Add this AST_String to the types defined in the global scope
	   */
	  (void) idl_global->root()
                    ->fe_add_string(AST_String::narrow_from_decl(yyval.dcval));
	} break;
case 222:
# line 1738 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        } break;
case 223:
# line 1746 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        } break;
case 224:
# line 1750 "idl.yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        } break;
case 225:
# line 1754 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringQsSeen);
	  /*
	   * Create a node representing a string
	   */
	  if (yypvt[-2].exval == NULL || yypvt[-2].exval->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error(yypvt[-2].exval, AST_Expression::EV_ulong);
	    yyval.dcval = NULL;
	  } else {
	    yyval.dcval = idl_global->gen()->create_wstring(yypvt[-2].exval);
	    /*
	     * Add this AST_String to the types defined in the global scope
	     */
	    (void) idl_global->root()
		      ->fe_add_string(AST_String::narrow_from_decl(yyval.dcval));
	  }
	} break;
case 226:
# line 1772 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringCompleted);
	  /*
	   * Create a node representing a string
	   */
	  yyval.dcval =
	    idl_global->gen()->create_wstring(
			 idl_global->gen()->create_expr((unsigned long) 0));
	  /*
	   * Add this AST_String to the types defined in the global scope
	   */
	  (void) idl_global->root()
                    ->fe_add_string(AST_String::narrow_from_decl(yyval.dcval));
	} break;
case 227:
# line 1790 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        } break;
case 228:
# line 1797 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ArrayIDSeen);
        } break;
case 229:
# line 1801 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ArrayCompleted);
	  /*
	   * Create a node representing an array
	   */
	  if (yypvt[-0].elval != NULL) {
	     yyval.dcval = idl_global->gen()->create_array(new UTL_ScopedName(yypvt[-2].idval, NULL),
						  yypvt[-0].elval->length(), yypvt[-0].elval);
	  }
	} break;
case 230:
# line 1815 "idl.yy"
{
	  yyval.elval = new UTL_ExprList(yypvt[-1].exval, yypvt[-0].elval);
	} break;
case 231:
# line 1822 "idl.yy"
{
	  if (yypvt[-1].elval == NULL)
	    yyval.elval = new UTL_ExprList(yypvt[-0].exval, NULL);
	  else {
	    yypvt[-1].elval->nconc(new UTL_ExprList(yypvt[-0].exval, NULL));
	    yyval.elval = yypvt[-1].elval;
	  }
	} break;
case 232:
# line 1831 "idl.yy"
{
	  yyval.elval = NULL;
	} break;
case 233:
# line 1838 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimSqSeen);
        } break;
case 234:
# line 1842 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimExprSeen);
        } break;
case 235:
# line 1846 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimQsSeen);
	  /*
	   * Array dimensions are expressions which must be coerced to
	   * positive integers
	   */
	  if (yypvt[-2].exval == NULL || yypvt[-2].exval->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error(yypvt[-2].exval, AST_Expression::EV_ulong);
	    yyval.exval = NULL;
	  } else
	    yyval.exval = yypvt[-2].exval;
	} break;
case 236:
# line 1863 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrSeen);
        } break;
case 237:
# line 1867 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrTypeSeen);
        } break;
case 238:
# line 1871 "idl.yy"
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
	  if (s != NULL && yypvt[-2].dcval != NULL && yypvt[-0].dlval != NULL) {
	    l = new UTL_DecllistActiveIterator(yypvt[-0].dlval);
	    for (;!(l->is_done()); l->next()) {
	      d = l->item();
	      if (d == NULL)
		continue;
	      AST_Type *tp = d->compose(yypvt[-2].dcval);
 	      if (tp == NULL)
	 	continue;
	      a = idl_global->gen()->create_attribute(yypvt[-5].bval, tp, d->name(), p);
	      /*
	       * Add one attribute to the enclosing scope
	       */
	      (void) s->fe_add_attribute(a);
	    }
	    delete l;
	  }
	} break;
case 239:
# line 1905 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrROSeen);
	  yyval.bval = I_TRUE;
	} break;
case 240:
# line 1910 "idl.yy"
{
	  yyval.bval = I_FALSE;
	} break;
case 241:
# line 1917 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSeen);
	} break;
case 242:
# line 1921 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(yypvt[-0].idval, NULL);
	  AST_Exception		*e = NULL;
	  UTL_StrList		*p = idl_global->pragmas();
	  AST_Decl		*v = NULL;

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
case 243:
# line 1943 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSqSeen);
        } break;
case 244:
# line 1947 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptBodySeen);
        } break;
case 245:
# line 1951 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptQsSeen);
	  /*
	   * Done with this exception. Pop its scope from the scope stack
	   */
	  idl_global->scopes()->pop();
	} break;
case 246:
# line 1963 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpTypeSeen);
        } break;
case 247:
# line 1967 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n =
		new UTL_ScopedName(new Identifier(yypvt[-0].strval, 1, 0, I_FALSE), NULL);
	  AST_Operation		*o = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_OpIDSeen);
	  /*
	   * Create a node representing an operation on an interface
	   * and add it to its enclosing scope
	   */
	  if (s != NULL && yypvt[-2].dcval != NULL) {
	    AST_Type *tp = AST_Type::narrow_from_decl(yypvt[-2].dcval);
            if (tp == NULL) {
              idl_global->err()->not_a_type(yypvt[-2].dcval);
            } else if (tp->node_type() == AST_Decl::NT_except) {
              idl_global->err()->not_a_type(yypvt[-2].dcval);
            } else {
	      o = idl_global->gen()->create_operation(tp, yypvt[-3].ofval, n, p);
	      (void) s->fe_add_operation(o);
	    }
	  }
	  /*
	   * Push the operation scope onto the scopes stack
	   */
	  idl_global->scopes()->push(o);
	} break;
case 248:
# line 1996 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParsCompleted);
        } break;
case 249:
# line 2000 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseCompleted);
        } break;
case 250:
# line 2004 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Operation		*o = NULL;

	  idl_global->set_parse_state(IDL_GlobalData::PS_OpCompleted);
	  /*
	   * Add exceptions and context to the operation
	   */
	  if (s != NULL && s->scope_node_type() == AST_Decl::NT_op) {
	    o = AST_Operation::narrow_from_scope(s);

	    if (yypvt[-2].nlval != NULL && o != NULL)
	      (void) o->fe_add_exceptions(yypvt[-2].nlval);
	    if (yypvt[-0].slval != NULL)
	      (void) o->fe_add_context(yypvt[-0].slval);
	  }
	  /*
	   * Done with this operation. Pop its scope from the scopes stack
	   */
	  idl_global->scopes()->pop();
	} break;
case 251:
# line 2029 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
	  yyval.ofval = AST_Operation::OP_oneway;
	} break;
case 252:
# line 2034 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
	  yyval.ofval = AST_Operation::OP_idempotent;
	} break;
case 253:
# line 2039 "idl.yy"
{
	  yyval.ofval = AST_Operation::OP_noflags;
	} break;
case 255:
# line 2047 "idl.yy"
{
	  yyval.dcval =
	    idl_global->scopes()->bottom()
	       ->lookup_primitive_type(AST_Expression::EV_void);
	} break;
case 256:
# line 2056 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        } break;
case 257:
# line 2060 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        } break;
case 258:
# line 2064 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        } break;
case 259:
# line 2069 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        } break;
case 261:
# line 2079 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParCommaSeen);
        } break;
case 264:
# line 2088 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParDirSeen);
        } break;
case 265:
# line 2092 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParTypeSeen);
        } break;
case 266:
# line 2096 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Argument		*a = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParDeclSeen);
	  /*
	   * Create a node representing an argument to an operation
	   * Add it to the enclosing scope (the operation scope)
	   */
	  if (yypvt[-2].dcval != NULL && yypvt[-0].deval != NULL && s != NULL) {
	    AST_Type *tp = yypvt[-0].deval->compose(yypvt[-2].dcval);
	    if (tp != NULL) {
	      a = idl_global->gen()->create_argument(yypvt[-4].dival, tp, yypvt[-0].deval->name(), p);
	      (void) s->fe_add_argument(a);
	    }
	  }
	} break;
case 267:
# line 2118 "idl.yy"
{
	  yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(yypvt[-0].etval);
	} break;
case 269:
# line 2123 "idl.yy"
{
	  UTL_Scope	*s = idl_global->scopes()->top_non_null();
	  AST_Decl	*d = NULL;

	  if (s != NULL)
	    d = s->lookup_by_name(yypvt[-0].idlist, I_TRUE);
	  if (d == NULL)
	    idl_global->err()->lookup_error(yypvt[-0].idlist);
	  yyval.dcval = d;
	} break;
case 270:
# line 2137 "idl.yy"
{
	  yyval.dival = AST_Argument::dir_IN;
	} break;
case 271:
# line 2141 "idl.yy"
{
	  yyval.dival = AST_Argument::dir_OUT;
	} break;
case 272:
# line 2145 "idl.yy"
{
	  yyval.dival = AST_Argument::dir_INOUT;
	} break;
case 273:
# line 2152 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSeen);
        } break;
case 274:
# line 2156 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSqSeen);
        } break;
case 275:
# line 2161 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseQsSeen);
	  yyval.nlval = yypvt[-1].nlval;
	} break;
case 276:
# line 2166 "idl.yy"
{
	  yyval.nlval = NULL;
	} break;
case 277:
# line 2173 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSeen);
        } break;
case 278:
# line 2177 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSqSeen);
        } break;
case 279:
# line 2182 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextQsSeen);
	  yyval.slval = yypvt[-1].slval;
	} break;
case 280:
# line 2187 "idl.yy"
{
	  yyval.slval = NULL;
	} break;
case 281:
# line 2194 "idl.yy"
{
	  yyval.slval = new UTL_StrList(yypvt[-1].sval, yypvt[-0].slval);
	} break;
case 282:
# line 2202 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextCommaSeen);
        } break;
case 283:
# line 2206 "idl.yy"
{
	  if (yypvt[-3].slval == NULL)
	    yyval.slval = new UTL_StrList(yypvt[-0].sval, NULL);
	  else {
	    yypvt[-3].slval->nconc(new UTL_StrList(yypvt[-0].sval, NULL));
	    yyval.slval = yypvt[-3].slval;
	  }
	} break;
case 284:
# line 2215 "idl.yy"
{
	  yyval.slval = NULL;
	} break;
# line	532 "/usr/ccs/bin/yaccpar"
	}
	goto yystack;		/* reset registers in driver code */
}

