#ifndef lint
char yysccsid[] = "@(#)yaccpar	1.4 (Berkeley) 02/25/90 \n\
 Modified 5/2/90 by J. Roskind to support graphic debugging modes";
#endif
#line 73 "idl.yy"
#include "idl.h"
#include "idl_extern.h"

#include "fe_private.h"

#include <stdio.h>

#if (defined(apollo) || defined(hpux)) && defined(__cplusplus)
extern	"C" int yywrap();
#endif	/* (defined(apollo) || defined(hpux)) && defined(__cplusplus)*/

void yyerror (char *);
int yylex (void);
extern "C" int yywrap (void);
#line 93 "idl.yy"
typedef union {
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
#line 48 "y.tab.c"
#define IDENTIFIER 257
#define IDL_CONST 258
#define IDL_MODULE 259
#define IDL_INTERFACE 260
#define IDL_TYPEDEF 261
#define IDL_LONG 262
#define IDL_SHORT 263
#define IDL_UNSIGNED 264
#define IDL_DOUBLE 265
#define IDL_FLOAT 266
#define IDL_CHAR 267
#define IDL_WCHAR 268
#define IDL_OCTET 269
#define IDL_BOOLEAN 270
#define IDL_ANY 271
#define IDL_STRUCT 272
#define IDL_UNION 273
#define IDL_SWITCH 274
#define IDL_ENUM 275
#define IDL_SEQUENCE 276
#define IDL_STRING 277
#define IDL_WSTRING 278
#define IDL_EXCEPTION 279
#define IDL_CASE 280
#define IDL_DEFAULT 281
#define IDL_READONLY 282
#define IDL_ATTRIBUTE 283
#define IDL_ONEWAY 284
#define IDL_IDEMPOTENT 285
#define IDL_VOID 286
#define IDL_IN 287
#define IDL_OUT 288
#define IDL_INOUT 289
#define IDL_RAISES 290
#define IDL_CONTEXT 291
#define IDL_INTEGER_LITERAL 292
#define IDL_STRING_LITERAL 293
#define IDL_CHARACTER_LITERAL 294
#define IDL_FLOATING_PT_LITERAL 295
#define IDL_TRUETOK 296
#define IDL_FALSETOK 297
#define IDL_SCOPE_DELIMITOR 298
#define IDL_LEFT_SHIFT 299
#define IDL_RIGHT_SHIFT 300
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,   67,   67,   69,   68,   71,   68,   73,   68,   75,
   68,   77,   68,   78,   68,   79,   80,   81,   82,   76,
   74,   74,   85,   87,   88,   83,   89,   64,   32,   90,
   22,   22,   86,   86,   92,   91,   93,   91,   94,   91,
   96,   91,   98,   91,   99,   91,   20,  100,   21,   21,
   16,  101,   16,  102,   16,   65,   84,  103,  104,  105,
  106,   70,   48,   48,   48,   48,   48,   48,   48,   48,
   33,   34,   35,   35,   36,   36,   37,   37,   38,   38,
   38,   39,   39,   39,   40,   40,   40,   40,   41,   41,
   41,   41,   42,   42,   42,   43,   43,   43,   43,   43,
   43,   44,  108,   66,   66,   66,   66,  109,  107,    1,
    1,    2,    2,    2,   56,   56,   56,   56,   56,   56,
    4,    4,    4,    3,    3,    3,   28,  110,   29,   29,
   60,   60,   30,  111,   31,   31,   61,   62,   49,   49,
   54,   54,   54,   55,   55,   55,   52,   52,   52,   50,
   50,   57,   51,   53,  112,  113,  115,  116,    7,  114,
  118,  118,  119,  120,  117,  121,  117,  122,  123,  124,
  125,  126,  127,  129,  130,   10,    9,    9,    9,    9,
    9,    9,  128,  132,  132,  133,  134,  131,  135,  131,
   26,   27,   27,  136,   46,  137,  138,   46,  139,   47,
  140,  141,  143,  144,    8,  142,  147,  146,  146,  145,
  148,  149,    5,    5,  150,  151,   13,  153,  154,    6,
    6,  152,  156,  157,   14,   14,  155,  158,   11,   24,
   25,   25,  159,  160,   45,  161,  162,   95,   63,   63,
  163,  164,  165,  166,   72,  167,  169,  170,  171,   97,
   59,   59,   59,   12,   12,  172,  168,  174,  168,  173,
  177,  176,  176,  178,  179,  175,   15,   15,   15,   58,
   58,   58,  180,  181,   23,   23,  182,  183,   17,   17,
   18,  184,   19,   19,
};
short yylen[] = {                                         2,
    1,    2,    0,    0,    3,    0,    3,    0,    3,    0,
    3,    0,    3,    0,    3,    0,    0,    0,    0,    9,
    1,    1,    0,    0,    0,    7,    0,    3,    2,    0,
    3,    0,    2,    0,    0,    3,    0,    3,    0,    3,
    0,    3,    0,    3,    0,    3,    2,    0,    4,    0,
    1,    0,    3,    0,    4,    1,    1,    0,    0,    0,
    0,    9,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    3,    1,    3,    1,    3,    1,    3,
    3,    1,    3,    3,    1,    3,    3,    3,    1,    2,
    2,    2,    1,    1,    3,    1,    1,    1,    1,    1,
    1,    1,    0,    3,    1,    1,    1,    0,    3,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    2,    0,    4,    0,
    1,    1,    2,    0,    4,    0,    1,    1,    1,    1,
    1,    2,    1,    2,    3,    2,    1,    1,    2,    1,
    1,    1,    1,    1,    0,    0,    0,    0,    9,    2,
    2,    0,    0,    0,    5,    0,    3,    0,    0,    0,
    0,    0,    0,    0,    0,   17,    1,    1,    1,    1,
    1,    1,    2,    2,    0,    0,    0,    5,    0,    3,
    2,    2,    0,    0,    3,    0,    0,    5,    0,    3,
    0,    0,    0,    0,    9,    2,    0,    4,    0,    1,
    0,    0,    6,    2,    0,    0,    5,    0,    0,    6,
    1,    1,    0,    0,    6,    1,    1,    0,    3,    2,
    2,    0,    0,    0,    5,    0,    0,    6,    1,    0,
    0,    0,    0,    0,    9,    0,    0,    0,    0,   10,
    1,    1,    0,    1,    1,    0,    3,    0,    4,    2,
    0,    4,    0,    0,    0,    5,    1,    1,    1,    1,
    1,    1,    0,    0,    6,    0,    0,    0,    6,    0,
    2,    0,    4,    0,
};
short yydefred[] = {                                      0,
   14,   58,   16,   27,  103,  155,  168,  201,  241,    0,
  105,  107,  106,   23,    0,    4,    1,    0,    6,    8,
   10,   12,   21,   22,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   30,   29,    0,    2,    0,    0,
    0,    0,   15,   56,    0,  143,    0,  147,  148,  150,
  151,  152,  153,  222,  227,   52,   68,   69,    0,   59,
   63,   64,   66,   67,  139,  140,   65,   51,    0,    0,
   17,   28,  154,  215,  108,  110,  111,  113,  121,  122,
  124,  126,  125,    0,  123,    0,  115,  117,  118,  116,
  120,  112,  119,  104,  156,  169,  202,  242,   24,    0,
    5,    7,    9,   11,   13,  142,  149,    0,  146,    0,
   54,    0,  218,  223,    0,    0,    0,  211,  214,    0,
    0,    0,    0,   34,    0,   31,  145,   53,    0,   60,
    0,    0,   18,  216,  138,  109,  130,  131,  132,    0,
    0,  157,  170,  203,  243,    0,    0,   55,    0,   96,
   97,   98,   99,  100,  101,    0,    0,    0,    0,    0,
  102,    0,    0,    0,    0,    0,    0,   85,   89,   94,
  219,  224,    0,    0,    0,    0,  212,    0,    0,    0,
  162,   45,  239,  251,  252,    0,    0,   35,   37,   39,
    0,   33,   41,   43,   48,   61,   90,   91,   92,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   19,  217,  128,  233,  229,  232,    0,  166,
  163,  158,  162,  171,  210,  204,  209,    0,    0,  255,
  268,  246,  254,    0,  267,  236,    0,    0,    0,   26,
    0,    0,    0,    0,   95,    0,    0,    0,    0,    0,
    0,    0,   86,   87,   88,  220,  225,    0,    0,    0,
    0,  213,    0,    0,    0,    0,    0,    0,    0,  161,
    0,   46,    0,    0,   36,   38,   40,   42,   44,    0,
   62,   71,   20,  129,  234,  231,  167,  164,  159,    0,
  181,  172,    0,  177,  178,  180,  179,  205,  207,  245,
  247,  237,    0,    0,    0,    0,    0,    0,  235,  165,
  173,  208,    0,  248,  238,  136,  137,    0,    0,    0,
    0,    0,  174,  257,  270,  271,  272,  264,    0,  263,
  273,  249,  134,    0,    0,  259,    0,    0,    0,    0,
  189,  196,  194,  186,  193,  175,  185,  265,  261,  274,
  277,  250,  135,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  190,  197,  195,  199,  187,  192,
  176,  184,  266,  262,    0,  278,    0,    0,    0,  275,
    0,  198,  200,  188,  284,    0,    0,  279,  282,    0,
  283,
};
short yydgoto[] = {                                      10,
  221,   76,   77,   78,   79,   80,   81,   82,  292,   83,
  135,  232,   84,   85,  233,  160,  352,  386,  387,  126,
  147,   36,  332,  217,  261,  344,  358,  136,  175,  315,
  322,   14,  281,  161,  162,  163,  164,  165,  166,  167,
  168,  169,  170,  171,  218,  345,  369,   60,   87,   88,
   89,   90,   91,   65,   66,   92,   93,  328,  186,  137,
  138,  139,  187,   15,   68,   16,   17,   18,   37,   19,
   39,   20,   40,   21,   41,   22,   42,   25,   27,  115,
  173,  258,   23,   24,   34,  146,  124,  191,   28,  100,
  192,  237,  238,  239,  193,  241,  194,  242,  229,  243,
  110,  129,   26,  112,  149,  244,   94,   29,  117,  259,
  340,   30,  120,  222,  178,  265,  270,  228,  264,  304,
  263,   31,  121,  179,  267,  305,  318,  346,  334,  359,
  347,  360,  357,  379,  354,  356,  355,  377,  378,   32,
  122,  226,  180,  268,  227,  269,  306,  141,  219,  116,
  174,   69,  131,  211,   70,  132,  212,  176,  260,  303,
  274,  308,   33,  123,  181,  271,  273,  314,  307,  321,
  339,  319,  329,  320,  330,  337,  362,  335,  361,  338,
  363,  364,  381,  390,
};
short yysindex[] = {                                    320,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   44,    0,    0,  320,    0,    0,
    0,    0,    0,    0,   51,  256, -112, -106,  167, -106,
 -106, -106, -106,   16,    0,    0,  108,    0,  110,  120,
  126,  154,    0,    0, -118,    0,  -86,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  -84,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  157,  161,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   41,    0,  -84,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -198,
    0,    0,    0,    0,    0,    0,    0,  -38,    0, -106,
    0, -106,    0,    0,  103,  172, -106,    0,    0,  112,
  -37,  115,  116,    0,  -84,    0,    0,    0, -106,    0,
   -6,   -6,    0,    0,    0,    0,    0,    0,    0,    0,
   -6,    0,    0,    0,    0,  290,  196,    0,  180,    0,
    0,    0,    0,    0,    0,  -32,  -32,  -32,   -6,  -84,
    0,  128,  150,  212,  -95,  119,  131,    0,    0,    0,
    0,    0,  320,  214,  213,  176,    0,  143,  235,   26,
    0,    0,    0,    0,    0,  231,    2,    0,    0,    0,
  168,    0,    0,    0,    0,    0,    0,    0,    0,  253,
   -6,   -6,   -6,   -6,   -6,   -6,   -6,   -6,   -6,   -6,
  233,  236,    0,    0,    0,    0,    0,    0,  239,    0,
    0,    0,    0,    0,    0,    0,    0,  143,  244,    0,
    0,    0,    0,  -84,    0,    0,  245,  247,  248,    0,
  249,  250, -198,   -6,    0,  150,  212,  -95,  119,  119,
  131,  131,    0,    0,    0,    0,    0,  186, -106,   -6,
  176,    0,  254, -106,  189,  143,  127,  199,  281,    0,
  204,    0,   73,  273,    0,    0,    0,    0,    0,  -84,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   74,
    0,    0,  -84,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  241,  282,  299,   26,  304, -106,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  222,  306, -158,
   58,  309,    0,    0,    0,    0,    0,    0,  308,    0,
    0,    0,    0, -139,  273,    0,  310,  319,   69, -106,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  302,   -6,  311,  167,  -99,  243, -139,
 -106, -158, -198,  330,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  331,    0,  313, -106,  315,    0,
   82,    0,    0,    0,    0,  335,  336,    0,    0,   86,
    0,
};
short yyrindex[] = {                                    381,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   40,    0,    0,   49,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   -9,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  129,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   -1,    1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   21,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  -15,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   11,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   64,
    0,    0,    0,    0,    0,  -71,    9,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  -26,
    0,  305,  160,   33,  238,   56,  -18,    0,    0,    0,
    0,    0,  260,    0,  324,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  262,    0,    0,
    0,    0,    0,  135,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  280,  258,  264,   66,   78,
   19,   28,    0,    0,    0,    0,    0,    0,    0,    0,
   94,    0,    0,    0,    0,  268,    0,    0,  276,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   52,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  362,
    0,    0,  376,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -19,    0,    0,    0,    0,    0,    0,    0,
  -46,  363,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  382,    0,  367,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  191,    0,  303,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  386,    0,    0,    0,
    0,
};
short yygindex[] = {                                      0,
  -22,  272,    0,    0,    0,  -25,   38,   10,    0,   70,
    0,    0,    0,  421, -202,  -20,    0,    0,    0,   87,
    0,    0,    0,    0,    0,    0,    0,  185,    0,    0,
    0,    0,    0, -135,    0,  251,  270,  267,   29,    3,
  -21,   72,    0,  -90,  190,  117,    0,    0,  -12,   -8,
    4,  447,    0,    0,    0, -128,    5,    0,    0, -213,
 -229,    0,    0,    0,  -28,  328,   30,    0,    0,  340,
    0,  341,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  325,  283,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  144,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  201,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  147,    0,    0,    0,    0,    0,
    0,    0,    0,    0,
};
#define YYTABLESIZE 599
short yytable[] = {                                      72,
   57,   95,   96,   97,   98,   59,   75,  159,   86,   12,
   93,   93,  276,   61,   93,   93,   93,   62,   93,   82,
   93,  256,   82,  200,   82,  144,   82,   12,  144,   63,
   67,   93,   93,  159,  141,   93,  156,   11,  157,   82,
   82,  172,  221,   82,  226,  284,  144,   38,    3,   47,
  177,   50,  141,   25,   50,   11,   83,  235,   44,   83,
  221,   83,  226,   83,  114,   84,   93,   93,   84,   13,
   84,  302,   84,   75,   82,   82,   83,   83,  316,  125,
   83,  128,  114,  130,  118,   84,   84,   13,  140,   84,
   75,   75,   49,   79,   75,   49,   79,   93,   57,   56,
  148,   35,  119,   80,  137,   82,   80,  137,  282,   43,
  353,   83,   83,   79,   79,   81,  341,   79,   81,  158,
   84,   84,  137,   80,   80,   75,   75,   80,  325,  326,
  327,   47,  348,   50,  230,   81,   81,  230,   99,   81,
  342,  343,   83,  106,   71,  235,  107,  373,   79,   79,
   44,   84,  230,   86,  228,   12,   75,   86,   80,   80,
  231,  206,   32,  207,  383,  234,  101,  210,  102,  285,
   81,   81,  208,    3,   49,  108,  109,  209,  103,   79,
  342,  343,   12,   11,  104,  253,  253,  254,  255,   80,
  253,  253,  253,  253,  253,  253,  253,  253,  253,  253,
   73,   81,  213,  204,  205,  253,  235,   86,  251,  252,
   11,  240,  105,  111,  253,   13,  113,   73,   73,  366,
  114,   73,  280,  127,   44,  133,  253,  197,  198,  199,
  140,  134,  249,  250,  142,  140,  143,  144,  145,  195,
  196,  144,   13,  202,  276,   86,  293,  141,  231,  203,
   44,  201,   73,  234,  294,  221,  215,  226,  295,  150,
  151,  152,  153,  154,  155,   56,  216,  258,  258,  258,
  296,  297,   93,   93,  224,   77,  291,  114,   77,  317,
   82,   82,  225,   73,  236,  150,  151,  152,  153,  154,
  155,   56,  240,  245,  256,   77,   77,  257,   76,   77,
  262,   78,  272,  275,   78,  276,  277,  278,  279,  231,
  283,  317,  287,  289,  234,   76,   76,   83,   83,   76,
   74,   78,   78,  298,  299,   78,   84,   84,  300,  301,
   77,   77,  140,  309,  368,  106,   86,   74,   74,  311,
  310,   74,  125,  313,  323,   72,  324,  331,  336,  140,
   76,   76,  333,  349,   79,   79,   78,   78,  350,  351,
  365,   77,   72,   72,   80,   80,   72,  371,  367,  376,
  382,  380,   74,  384,  385,  388,   81,   81,  391,  389,
    3,   76,  127,   44,    3,   70,  244,   78,  290,   46,
   47,  269,  160,   50,   51,   52,   53,   72,  220,   44,
  206,    8,  141,   74,   45,   46,   47,   48,   49,   50,
   51,   52,   53,   73,    6,    7,  182,    8,   74,   54,
   55,  133,  260,   44,   56,  280,  281,  183,   45,   46,
   47,   48,   49,   50,   51,   52,   53,   73,    6,    7,
   56,    8,   74,   54,   55,  214,   58,  191,  288,  375,
  286,  246,  191,  191,  191,  191,  191,  191,  191,  191,
  191,  191,  191,  191,   56,  191,  191,  191,  191,  248,
   44,  247,   64,  188,  370,   45,   46,   47,   48,   49,
   50,   51,   52,   53,   73,  189,  190,   44,  191,   74,
   54,   55,   45,   46,   47,   48,   49,   50,   51,   52,
   53,   73,  223,  372,    0,  266,  312,   54,  374,    0,
    0,   56,   44,    0,    0,    0,  230,   45,   46,   47,
   48,   49,   50,   51,   52,   53,    0,    0,   56,   44,
    0,    0,   54,   55,   45,   46,   47,   48,   49,   50,
   51,   52,   53,   73,    0,  182,    0,    2,    0,   54,
    5,    0,    0,   56,    0,    0,    0,    0,    0,    0,
    0,    6,    7,    0,    8,    0,    0,    0,    9,    0,
   56,  183,    0,  184,  185,    1,    0,    2,    3,    4,
    5,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    6,    7,    0,    8,    0,    0,    0,    9,
};
short yycheck[] = {                                      28,
   26,   30,   31,   32,   33,   26,   29,   40,   29,    0,
   37,   38,   59,   26,   41,   42,   43,   26,   45,   38,
   47,   41,   41,  159,   43,   41,   45,   18,   44,   26,
   26,   58,   59,   40,   44,   62,   43,    0,   45,   58,
   59,  132,   44,   62,   44,  259,   62,   18,    0,   41,
  141,   41,   62,  125,   44,   18,   38,  186,  257,   41,
   62,   43,   62,   45,   44,   38,   93,   94,   41,    0,
   43,  274,   45,   41,   93,   94,   58,   59,  308,  100,
   62,  110,   62,  112,   44,   58,   59,   18,  117,   62,
   58,   59,   41,   38,   62,   44,   41,  124,   59,  298,
  129,   58,   62,   38,   41,  124,   41,   44,  244,   59,
  340,   93,   94,   58,   59,   38,  256,   62,   41,  126,
   93,   94,   59,   58,   59,   93,   94,   62,  287,  288,
  289,  123,  335,  123,   41,   58,   59,   44,  123,   62,
  280,  281,  124,  262,  257,  274,  265,  361,   93,   94,
  257,  124,   59,  174,   91,  146,  124,  178,   93,   94,
  186,   43,  123,   45,  378,  186,   59,   37,   59,  260,
   93,   94,   42,  125,  123,  262,  263,   47,   59,  124,
  280,  281,  173,  146,   59,  257,  208,  209,  210,  124,
  262,  263,  264,  265,  266,  267,  268,  269,  270,  271,
   41,  124,  173,  299,  300,  277,  335,  228,  206,  207,
  173,  283,   59,  298,  286,  146,   60,   58,   59,  355,
   60,   62,  243,  262,  257,  123,  298,  156,  157,  158,
  259,   60,  204,  205,  123,  264,  274,  123,  123,   44,
   61,  257,  173,   94,  291,  266,  267,  257,  274,   38,
  257,  124,   93,  274,  267,  257,   44,  257,  267,  292,
  293,  294,  295,  296,  297,  298,   91,  287,  288,  289,
  267,  267,  299,  300,   40,   38,  267,  257,   41,  308,
  299,  300,  257,  124,  283,  292,  293,  294,  295,  296,
  297,  298,  125,   41,   62,   58,   59,   62,   41,   62,
   62,   38,   59,   59,   41,   59,   59,   59,   59,  335,
  125,  340,   59,  125,  335,   58,   59,  299,  300,   62,
   41,   58,   59,  125,   44,   62,  299,  300,  125,  257,
   93,   94,  361,   93,  357,  262,  357,   58,   59,   41,
   59,   62,  363,   40,  123,   41,   41,  290,   41,  378,
   93,   94,   44,   44,  299,  300,   93,   94,   40,  291,
   59,  124,   58,   59,  299,  300,   62,  125,   58,   40,
   58,   41,   93,   59,  293,   41,  299,  300,  293,   44,
    0,  124,   59,  257,  125,  257,  125,  124,  262,  263,
  264,  257,  125,  267,  268,  269,  270,   93,  256,  257,
  125,  275,   41,  124,  262,  263,  264,  265,  266,  267,
  268,  269,  270,  271,  272,  273,   41,  275,  276,  277,
  278,   59,   41,  257,  298,   59,   41,  125,  262,  263,
  264,  265,  266,  267,  268,  269,  270,  271,  272,  273,
  298,  275,  276,  277,  278,  174,   26,  257,  264,  363,
  261,  201,  262,  263,  264,  265,  266,  267,  268,  269,
  270,  271,  272,  273,  298,  275,  276,  277,  278,  203,
  257,  202,   26,  146,  358,  262,  263,  264,  265,  266,
  267,  268,  269,  270,  271,  146,  146,  257,  298,  276,
  277,  278,  262,  263,  264,  265,  266,  267,  268,  269,
  270,  271,  178,  360,   -1,  223,  306,  277,  362,   -1,
   -1,  298,  257,   -1,   -1,   -1,  286,  262,  263,  264,
  265,  266,  267,  268,  269,  270,   -1,   -1,  298,  257,
   -1,   -1,  277,  278,  262,  263,  264,  265,  266,  267,
  268,  269,  270,  271,   -1,  256,   -1,  258,   -1,  277,
  261,   -1,   -1,  298,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  272,  273,   -1,  275,   -1,   -1,   -1,  279,   -1,
  298,  282,   -1,  284,  285,  256,   -1,  258,  259,  260,
  261,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  272,  273,   -1,  275,   -1,   -1,   -1,  279,
};
#define YYFINAL 10
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#define YYMAXTOKEN 300
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,"'%'","'&'",0,"'('","')'","'*'","'+'","','","'-'",0,"'/'",0,0,0,0,0,0,0,0,
0,0,"':'","';'","'<'","'='","'>'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,"'['",0,"']'","'^'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,"'{'","'|'","'}'","'~'",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"IDENTIFIER","IDL_CONST",
"IDL_MODULE","IDL_INTERFACE","IDL_TYPEDEF","IDL_LONG","IDL_SHORT",
"IDL_UNSIGNED","IDL_DOUBLE","IDL_FLOAT","IDL_CHAR","IDL_WCHAR","IDL_OCTET",
"IDL_BOOLEAN","IDL_ANY","IDL_STRUCT","IDL_UNION","IDL_SWITCH","IDL_ENUM",
"IDL_SEQUENCE","IDL_STRING","IDL_WSTRING","IDL_EXCEPTION","IDL_CASE",
"IDL_DEFAULT","IDL_READONLY","IDL_ATTRIBUTE","IDL_ONEWAY","IDL_IDEMPOTENT",
"IDL_VOID","IDL_IN","IDL_OUT","IDL_INOUT","IDL_RAISES","IDL_CONTEXT",
"IDL_INTEGER_LITERAL","IDL_STRING_LITERAL","IDL_CHARACTER_LITERAL",
"IDL_FLOATING_PT_LITERAL","IDL_TRUETOK","IDL_FALSETOK","IDL_SCOPE_DELIMITOR",
"IDL_LEFT_SHIFT","IDL_RIGHT_SHIFT",
};
char *yyrule[] = {
"$accept : start",
"start : definitions",
"definitions : definition definitions",
"definitions :",
"$$1 :",
"definition : type_dcl $$1 ';'",
"$$2 :",
"definition : const_dcl $$2 ';'",
"$$3 :",
"definition : exception $$3 ';'",
"$$4 :",
"definition : interface_def $$4 ';'",
"$$5 :",
"definition : module $$5 ';'",
"$$6 :",
"definition : error $$6 ';'",
"$$7 :",
"$$8 :",
"$$9 :",
"$$10 :",
"module : IDL_MODULE $$7 IDENTIFIER $$8 '{' $$9 definitions $$10 '}'",
"interface_def : interface",
"interface_def : forward",
"$$11 :",
"$$12 :",
"$$13 :",
"interface : interface_header $$11 '{' $$12 exports $$13 '}'",
"$$14 :",
"interface_decl : IDL_INTERFACE $$14 id",
"interface_header : interface_decl inheritance_spec",
"$$15 :",
"inheritance_spec : ':' $$15 at_least_one_scoped_name",
"inheritance_spec :",
"exports : exports export",
"exports :",
"$$16 :",
"export : type_dcl $$16 ';'",
"$$17 :",
"export : const_dcl $$17 ';'",
"$$18 :",
"export : exception $$18 ';'",
"$$19 :",
"export : attribute $$19 ';'",
"$$20 :",
"export : operation $$20 ';'",
"$$21 :",
"export : error $$21 ';'",
"at_least_one_scoped_name : scoped_name scoped_names",
"$$22 :",
"scoped_names : scoped_names ',' $$22 scoped_name",
"scoped_names :",
"scoped_name : id",
"$$23 :",
"scoped_name : IDL_SCOPE_DELIMITOR $$23 id",
"$$24 :",
"scoped_name : scoped_name IDL_SCOPE_DELIMITOR $$24 id",
"id : IDENTIFIER",
"forward : interface_decl",
"$$25 :",
"$$26 :",
"$$27 :",
"$$28 :",
"const_dcl : IDL_CONST $$25 const_type $$26 id $$27 '=' $$28 expression",
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
"$$29 :",
"type_dcl : IDL_TYPEDEF $$29 type_declarator",
"type_dcl : struct_type",
"type_dcl : union_type",
"type_dcl : enum_type",
"$$30 :",
"type_declarator : type_spec $$30 at_least_one_declarator",
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
"$$31 :",
"declarators : declarators ',' $$31 declarator",
"declarators :",
"declarator : simple_declarator",
"declarator : complex_declarator",
"at_least_one_simple_declarator : simple_declarator simple_declarators",
"$$32 :",
"simple_declarators : simple_declarators ',' $$32 simple_declarator",
"simple_declarators :",
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
"$$33 :",
"$$34 :",
"$$35 :",
"$$36 :",
"struct_type : IDL_STRUCT $$33 id $$34 '{' $$35 at_least_one_member $$36 '}'",
"at_least_one_member : member members",
"members : members member",
"members :",
"$$37 :",
"$$38 :",
"member : type_spec $$37 at_least_one_declarator $$38 ';'",
"$$39 :",
"member : error $$39 ';'",
"$$40 :",
"$$41 :",
"$$42 :",
"$$43 :",
"$$44 :",
"$$45 :",
"$$46 :",
"$$47 :",
"union_type : IDL_UNION $$40 id $$41 IDL_SWITCH $$42 '(' $$43 switch_type_spec $$44 ')' $$45 '{' $$46 at_least_one_case_branch $$47 '}'",
"switch_type_spec : integer_type",
"switch_type_spec : char_type",
"switch_type_spec : octet_type",
"switch_type_spec : boolean_type",
"switch_type_spec : enum_type",
"switch_type_spec : scoped_name",
"at_least_one_case_branch : case_branch case_branches",
"case_branches : case_branches case_branch",
"case_branches :",
"$$48 :",
"$$49 :",
"case_branch : at_least_one_case_label $$48 element_spec $$49 ';'",
"$$50 :",
"case_branch : error $$50 ';'",
"at_least_one_case_label : case_label case_labels",
"case_labels : case_labels case_label",
"case_labels :",
"$$51 :",
"case_label : IDL_DEFAULT $$51 ':'",
"$$52 :",
"$$53 :",
"case_label : IDL_CASE $$52 const_expr $$53 ':'",
"$$54 :",
"element_spec : type_spec $$54 declarator",
"$$55 :",
"$$56 :",
"$$57 :",
"$$58 :",
"enum_type : IDL_ENUM $$55 id $$56 '{' $$57 at_least_one_enumerator $$58 '}'",
"at_least_one_enumerator : enumerator enumerators",
"$$59 :",
"enumerators : enumerators ',' $$59 enumerator",
"enumerators :",
"enumerator : IDENTIFIER",
"$$60 :",
"$$61 :",
"sequence_type_spec : seq_head ',' $$60 positive_int_expr $$61 '>'",
"sequence_type_spec : seq_head '>'",
"$$62 :",
"$$63 :",
"seq_head : IDL_SEQUENCE $$62 '<' $$63 simple_type_spec",
"$$64 :",
"$$65 :",
"string_type_spec : string_head '<' $$64 positive_int_expr $$65 '>'",
"string_type_spec : string_head",
"string_head : IDL_STRING",
"$$66 :",
"$$67 :",
"wstring_type_spec : wstring_head '<' $$66 positive_int_expr $$67 '>'",
"wstring_type_spec : wstring_head",
"wstring_head : IDL_WSTRING",
"$$68 :",
"array_declarator : id $$68 at_least_one_array_dim",
"at_least_one_array_dim : array_dim array_dims",
"array_dims : array_dims array_dim",
"array_dims :",
"$$69 :",
"$$70 :",
"array_dim : '[' $$69 positive_int_expr $$70 ']'",
"$$71 :",
"$$72 :",
"attribute : opt_readonly IDL_ATTRIBUTE $$71 param_type_spec $$72 at_least_one_simple_declarator",
"opt_readonly : IDL_READONLY",
"opt_readonly :",
"$$73 :",
"$$74 :",
"$$75 :",
"$$76 :",
"exception : IDL_EXCEPTION $$73 id $$74 '{' $$75 members $$76 '}'",
"$$77 :",
"$$78 :",
"$$79 :",
"$$80 :",
"operation : opt_op_attribute op_type_spec $$77 IDENTIFIER $$78 parameter_list $$79 opt_raises $$80 opt_context",
"opt_op_attribute : IDL_ONEWAY",
"opt_op_attribute : IDL_IDEMPOTENT",
"opt_op_attribute :",
"op_type_spec : param_type_spec",
"op_type_spec : IDL_VOID",
"$$81 :",
"parameter_list : '(' $$81 ')'",
"$$82 :",
"parameter_list : '(' $$82 at_least_one_parameter ')'",
"at_least_one_parameter : parameter parameters",
"$$83 :",
"parameters : parameters ',' $$83 parameter",
"parameters :",
"$$84 :",
"$$85 :",
"parameter : direction $$84 param_type_spec $$85 declarator",
"param_type_spec : base_type_spec",
"param_type_spec : string_type_spec",
"param_type_spec : scoped_name",
"direction : IDL_IN",
"direction : IDL_OUT",
"direction : IDL_INOUT",
"$$86 :",
"$$87 :",
"opt_raises : IDL_RAISES $$86 '(' $$87 at_least_one_scoped_name ')'",
"opt_raises :",
"$$88 :",
"$$89 :",
"opt_context : IDL_CONTEXT $$88 '(' $$89 at_least_one_string_literal ')'",
"opt_context :",
"at_least_one_string_literal : IDL_STRING_LITERAL string_literals",
"$$90 :",
"string_literals : string_literals ',' $$90 IDL_STRING_LITERAL",
"string_literals :",
};
#endif
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#ifndef YYSTACKSIZE
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 300
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
#define yystacksize YYSTACKSIZE
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#line 2240 "idl.yy"
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
#line 804 "y.tab.c"
#define YYABORT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
#ifdef YYDEBUG
#ifndef YYDEBUG_LEXER_TEXT /* pointer to the text isolated by the lexer*/
#define YYDEBUG_LEXER_TEXT "YYDEBUG_LEXER_TEXT not defined"
#endif
#ifndef YYDEBUG_INDENT_STRING
#define YYDEBUG_INDENT_STRING  "|       "
#endif
#ifndef YYDEBUG_REDUCE_STRING
#define YYDEBUG_REDUCE_STRING  "+-------"
#endif
#ifndef YYDEBUG_INDENT
#ifdef __cplusplus
void YYDEBUG_INDENT(int yyindent)
#else
YYDEBUG_INDENT(yyindent)
int yyindent;
#endif
{
    while(yyindent-- > 0)
        printf("%s", YYDEBUG_INDENT_STRING);
}
#endif /* YYDEBUG_INDENT */
#ifndef YYDEBUG_REDUCE
#ifdef __cplusplus
void YYDEBUG_REDUCE(int yynew_state, int yyrule_num, char *yyrule_string, int yynew_indent, int yyrhs_count)
#else
YYDEBUG_REDUCE(yynew_state, yyrule_num, yyrule_string, yynew_indent, yyrhs_count)
int yynew_state;
int yyrule_num;
char * yyrule_string;
int yynew_indent;
int yyrhs_count;
#endif
{
    if (1 < yyrhs_count)
    {  /* draw the graphics for the reduction */
        YYDEBUG_INDENT(yynew_indent);
        while(1 < yyrhs_count--)
            printf("%s", YYDEBUG_REDUCE_STRING);
        putchar('+'); /* left rotated L would look nice */
        putchar('\n');
        YYDEBUG_INDENT(yynew_indent);
        putchar('|'); /* down arrow would look nice */
        putchar('\n');
    }
    YYDEBUG_INDENT(yynew_indent);
    /* Only print the resulting token name */
    while (*yyrule_string)
        putchar(*yyrule_string++);
    putchar('\n');
}
#endif /* YYDEBUG_REDUCE */
#ifndef YYDEBUG_SHIFT_LEXEME
#ifdef __cplusplus
void YYDEBUG_SHIFT_LEXEME(int yyold_state, int yynew_state, char *yytoken_string, int yynew_indent)
#else
YYDEBUG_SHIFT_LEXEME(yyold_state, yynew_state, yytoken_string, yynew_indent)
int yyold_state;
int yynew_state;
char * yytoken_string;
int yynew_indent;
#endif
{
    YYDEBUG_INDENT(yynew_indent);
    printf("%s <-- `%s'\n", yytoken_string, YYDEBUG_LEXER_TEXT);
}
#endif /*  YYDEBUG_SHIFT_LEXEME */
#ifndef YYDEBUG_LOOK_AHEAD
#ifdef __cplusplus
void YYDEBUG_LOOK_AHEAD(int yynew_state, int yytoken_num, char *yytoken_string, int yyindent)
#else
YYDEBUG_LOOK_AHEAD(yynew_state, yytoken_num, yytoken_string, yyindent)
int yynew_state;
int yytoken_num;
char * yytoken_string;
int yyindent;
#endif
{
    YYDEBUG_INDENT(yyindent);
    printf("          .... look ahead at %s   `%s'\n",
           yytoken_string,
           (0 == yytoken_num)? "\0": YYDEBUG_LEXER_TEXT);
}
#endif /* YYDEBUG_LOOK_AHEAD */
#ifndef YYDEBUG_DISCARD_STATE
#ifdef __cplusplus
void YYDEBUG_DISCARD_STATE(int yynew_state, int yyindent)
#else
YYDEBUG_DISCARD_STATE(yynew_state, yyindent)
int yynew_state;
int yyindent;
#endif
{
    if (0 < yyindent)
    {  /* draw the graphics for the reduction */
        YYDEBUG_INDENT(yyindent-1);
        printf("%s", YYDEBUG_REDUCE_STRING);
        putchar('+'); /* left rotated L would look nice */
        printf("  discarding state\n");
        YYDEBUG_INDENT(yyindent-1);
        putchar('|'); /* down arrow would look nice */
        putchar('\n');
    }
    else
    {
        if (0 == yyindent)
            printf("discarding state\n");
        else
            printf("no more states to discard: parser will abort\n");
    }
}
#endif /* YYDEBUG_DISCARD_STATE */
#ifndef YYDEBUG_DISCARD_TOKEN
#ifdef __cplusplus
void YYDEBUG_DISCARD_TOKEN(int yynew_state, int yytoken_num, char *yytoken_string, int yyindent)
#else
YYDEBUG_DISCARD_TOKEN(yynew_state, yytoken_num, yytoken_string, yyindent)
int yynew_state;
int yytoken_num;
char * yytoken_string;
int yyindent;
#endif
{
    YYDEBUG_INDENT(yyindent);
    printf("discarding token %s\n", yytoken_string);
}
#endif /* YYDEBUG_DISCARD_TOKEN */
#ifndef YYDEBUG_SHIFT_ERROR_LEXEME
#ifdef __cplusplus
void YYDEBUG_SHIFT_ERROR_LEXEME(int yyold_state, int yynew_state, int yyindent)
#else
YYDEBUG_SHIFT_ERROR_LEXEME(yyold_state, yynew_state, yyindent)
int yyold_state;
int yynew_state;
int yyindent;
#endif
{
    YYDEBUG_INDENT(yyindent);
    printf("error\n");
}
#endif /* YYDEBUG_SHIFT_ERROR_LEXEME */
#endif /* YYDEBUG */
#ifdef __cplusplus
extern "C" { extern char *getenv(const char *); }
#endif
int
yyparse()
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;
#ifndef __cplusplus
    extern char *getenv();
#endif

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            if (5 < yydebug)
                printf("yydebug: state %d, reading %d (%s)\n", yystate,
                        yychar, yys);
            else
                YYDEBUG_LOOK_AHEAD(yystate, yychar, yys, yyssp-yyss);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            if (5 < yydebug)
                printf("yydebug: state %d, shifting to state %d\n",
                        yystate, yytable[yyn]);
            else
                YYDEBUG_SHIFT_LEXEME(yystate, yytable[yyn], yys, yyssp-yyss);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    if (5 < yydebug)
                        printf("yydebug: state %d, error recovery shifting\
 to state %d\n", *yyssp, yytable[yyn]);
                    else
                        YYDEBUG_SHIFT_ERROR_LEXEME(*yyssp, yytable[yyn], yyssp-yyss);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    if (5 < yydebug)
                        printf("yydebug: error recovery discarding state %d\
",
                            *yyssp);
                    else
                        YYDEBUG_DISCARD_STATE(*yyssp, yyssp-yyss-1);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            if (5 < yydebug)
                printf("yydebug: state %d, error recovery discards token %d (%s)\n",
                    yystate, yychar, yys);
            else 
                YYDEBUG_DISCARD_TOKEN(yystate, yychar, yys, yyssp-yyss);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
#if YYDEBUG
    if (yydebug)
        if (5 < yydebug)
            printf("yydebug: state %d, reducing by rule %d (%s)\n",
                    yystate, yyn, yyrule[yyn]);
        else
            YYDEBUG_REDUCE(yystate, yyn, yyrule[yyn], yyssp-yyss-yym, yym);
#endif
    switch (yyn)
    {
case 4:
#line 235 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        }
break;
case 5:
#line 239 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 6:
#line 243 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        }
break;
case 7:
#line 247 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 8:
#line 251 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        }
break;
case 9:
#line 255 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 10:
#line 259 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceDeclSeen);
        }
break;
case 11:
#line 263 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 12:
#line 267 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ModuleDeclSeen);
        }
break;
case 13:
#line 271 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 14:
#line 275 "idl.yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 15:
#line 279 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  yyerrok;
	}
break;
case 16:
#line 286 "idl.yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSeen);
	  }
break;
case 17:
#line 290 "idl.yy"
{
	    UTL_ScopedName 	*n =
		new UTL_ScopedName(new Identifier(yyvsp[0].strval, 1, 0, I_FALSE), NULL);
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
	  }
break;
case 18:
#line 311 "idl.yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSqSeen);
	  }
break;
case 19:
#line 315 "idl.yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleBodySeen);
	  }
break;
case 20:
#line 319 "idl.yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleQsSeen);
	    /*
	     * Finished with this module - pop it from the scope stack
	     */
	    idl_global->scopes()->pop();
	  }
break;
case 23:
#line 335 "idl.yy"
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
	  if (s != NULL && yyvsp[0].ihval != NULL) {
	    i = idl_global->gen()->create_interface(yyvsp[0].ihval->interface_name(),
						    yyvsp[0].ihval->inherits(),
						    yyvsp[0].ihval->n_inherits(),
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
		    fd->set_inherits(yyvsp[0].ihval->inherits());
		    fd->set_n_inherits(yyvsp[0].ihval->n_inherits());
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
        }
break;
case 24:
#line 419 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSqSeen);
	}
break;
case 25:
#line 423 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceBodySeen);
	}
break;
case 26:
#line 427 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceQsSeen);
	  /*
	   * Done with this interface - pop it off the scopes stack
	   */
	  idl_global->scopes()->pop();
	}
break;
case 27:
#line 438 "idl.yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSeen);
	 }
break;
case 28:
#line 442 "idl.yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceIDSeen);
	   yyval.idval = yyvsp[0].idval;
	 }
break;
case 29:
#line 450 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InheritSpecSeen);
	  /*
	   * Create an AST representation of the information in the header
	   * part of an interface - this representation contains a computed
	   * list of all interfaces which this interface inherits from,
	   * recursively
	   */
	  yyval.ihval = new FE_InterfaceHeader(new UTL_ScopedName(yyvsp[-1].idval, NULL), yyvsp[0].nlval);
	}
break;
case 30:
#line 464 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InheritColonSeen);
        }
break;
case 31:
#line 468 "idl.yy"
{
	  yyval.nlval = yyvsp[0].nlval;
	}
break;
case 32:
#line 472 "idl.yy"
{
	  yyval.nlval = NULL;
	}
break;
case 35:
#line 484 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        }
break;
case 36:
#line 488 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 37:
#line 492 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        }
break;
case 38:
#line 496 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 39:
#line 500 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        }
break;
case 40:
#line 504 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 41:
#line 508 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrDeclSeen);
        }
break;
case 42:
#line 512 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 43:
#line 516 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpDeclSeen);
        }
break;
case 44:
#line 520 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 45:
#line 524 "idl.yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 46:
#line 528 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  yyerrok;
	}
break;
case 47:
#line 536 "idl.yy"
{
	  yyval.nlval = new UTL_NameList(yyvsp[-1].idlist, yyvsp[0].nlval);
	}
break;
case 48:
#line 544 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SNListCommaSeen);
        }
break;
case 49:
#line 548 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopedNameSeen);

	  if (yyvsp[-3].nlval == NULL)
	    yyval.nlval = new UTL_NameList(yyvsp[0].idlist, NULL);
	  else {
	    yyvsp[-3].nlval->nconc(new UTL_NameList(yyvsp[0].idlist, NULL));
	    yyval.nlval = yyvsp[-3].nlval;
	  }
	}
break;
case 50:
#line 559 "idl.yy"
{
	  yyval.nlval = NULL;
	}
break;
case 51:
#line 566 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  yyval.idlist = new UTL_IdList(yyvsp[0].idval, NULL);
	}
break;
case 52:
#line 572 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        }
break;
case 53:
#line 576 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  yyval.idlist = new UTL_IdList(new Identifier(yyvsp[-2].strval, 1, 0, I_FALSE),
			      new UTL_IdList(yyvsp[0].idval, NULL));
	}
break;
case 54:
#line 584 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        }
break;
case 55:
#line 588 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  yyvsp[-3].idlist->nconc(new UTL_IdList(yyvsp[0].idval, NULL));
	  yyval.idlist = yyvsp[-3].idlist;
	}
break;
case 56:
#line 597 "idl.yy"
{
            yyval.idval = new Identifier(yyvsp[0].strval, 1, 0, I_FALSE);
        }
break;
case 57:
#line 604 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(yyvsp[0].idval, NULL);
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
	}
break;
case 58:
#line 624 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstSeen);
        }
break;
case 59:
#line 628 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstTypeSeen);
        }
break;
case 60:
#line 632 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstIDSeen);
        }
break;
case 61:
#line 636 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstAssignSeen);
        }
break;
case 62:
#line 640 "idl.yy"
{
	  UTL_ScopedName	*n = new UTL_ScopedName(yyvsp[-4].idval, NULL);
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
	  if (yyvsp[0].exval != NULL && s != NULL) {
	    if (yyvsp[0].exval->coerce(yyvsp[-6].etval) == NULL)
	      idl_global->err()->coercion_error(yyvsp[0].exval, yyvsp[-6].etval);
	    else {
	      c =
		idl_global->gen()->create_constant(yyvsp[-6].etval, yyvsp[0].exval, n, p);
	      (void) s->fe_add_constant(c);
	    }
	  }
	}
break;
case 68:
#line 673 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_string;
	}
break;
case 69:
#line 677 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_wstring;
        }
break;
case 70:
#line 681 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Decl		*d = NULL;
	  AST_PredefinedType	*c = NULL;
	  AST_Typedef		*t = NULL;

	  /*
	   * If the constant's type is a scoped name, it must resolve
	   * to a scalar constant type
	   */
	  if (s != NULL && (d = s->lookup_by_name(yyvsp[0].idlist, I_TRUE)) != NULL) {
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
	}
break;
case 74:
#line 723 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_or, yyvsp[-2].exval, yyvsp[0].exval);
	}
break;
case 76:
#line 731 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_xor, yyvsp[-2].exval, yyvsp[0].exval);
	}
break;
case 78:
#line 739 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_and, yyvsp[-2].exval, yyvsp[0].exval);
	}
break;
case 80:
#line 747 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_right,yyvsp[-2].exval,yyvsp[0].exval);
	}
break;
case 81:
#line 751 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_left,yyvsp[-2].exval,yyvsp[0].exval);
	}
break;
case 83:
#line 759 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_add, yyvsp[-2].exval, yyvsp[0].exval);
	}
break;
case 84:
#line 763 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_minus,yyvsp[-2].exval,yyvsp[0].exval);
	}
break;
case 86:
#line 771 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_mul, yyvsp[-2].exval, yyvsp[0].exval);
	}
break;
case 87:
#line 775 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_div, yyvsp[-2].exval, yyvsp[0].exval);
	}
break;
case 88:
#line 779 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_mod, yyvsp[-2].exval, yyvsp[0].exval);
	}
break;
case 90:
#line 787 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_u_plus,
					      yyvsp[0].exval,
					      NULL);
	}
break;
case 91:
#line 793 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_u_minus,
					      yyvsp[0].exval,
					      NULL);
	}
break;
case 92:
#line 799 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_bit_neg,
					      yyvsp[0].exval,
					      NULL);
	}
break;
case 93:
#line 808 "idl.yy"
{
	  /*
	   * An expression which is a scoped name is not resolved now,
	   * but only when it is evaluated (such as when it is assigned
	   * as a constant value)
	   */
	  yyval.exval = idl_global->gen()->create_expr(yyvsp[0].idlist);
	}
break;
case 95:
#line 818 "idl.yy"
{
	  yyval.exval = yyvsp[-1].exval;
	}
break;
case 96:
#line 825 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(yyvsp[0].ival);
	}
break;
case 97:
#line 829 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(yyvsp[0].sval);
	}
break;
case 98:
#line 833 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(yyvsp[0].cval);
	}
break;
case 99:
#line 837 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr(yyvsp[0].dval);
	}
break;
case 100:
#line 841 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr((idl_bool) I_TRUE,
					    AST_Expression::EV_bool);
	}
break;
case 101:
#line 846 "idl.yy"
{
	  yyval.exval = idl_global->gen()->create_expr((idl_bool) I_FALSE,
					    AST_Expression::EV_bool);
	}
break;
case 102:
#line 854 "idl.yy"
{
	    yyvsp[0].exval->evaluate(AST_Expression::EK_const);
	    yyval.exval = idl_global->gen()->create_expr(yyvsp[0].exval, AST_Expression::EV_ulong);
	}
break;
case 103:
#line 862 "idl.yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_TypedefSeen);
	  }
break;
case 104:
#line 865 "idl.yy"
{yyval.ival = 0;}
break;
case 105:
#line 866 "idl.yy"
{ yyval.ival = 0;}
break;
case 106:
#line 867 "idl.yy"
{ yyval.ival = 0;}
break;
case 107:
#line 868 "idl.yy"
{ yyval.ival = 0;}
break;
case 108:
#line 873 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeSpecSeen);
        }
break;
case 109:
#line 877 "idl.yy"
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
	  if (s != NULL && yyvsp[-2].dcval != NULL && yyvsp[0].dlval != NULL) {
	    l = new UTL_DecllistActiveIterator(yyvsp[0].dlval);
	    for (;!(l->is_done()); l->next()) {
	      d = l->item();
	      if (d == NULL)
		continue;
              AST_Type * tp = d->compose(yyvsp[-2].dcval);
              if (tp == NULL)
		continue;
	      t = idl_global->gen()->create_typedef(tp, d->name(), p);
	      (void) s->fe_add_typedef(t);
	    }
	    delete l;
	  }
	}
break;
case 112:
#line 916 "idl.yy"
{
	  yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(yyvsp[0].etval);
	}
break;
case 114:
#line 921 "idl.yy"
{
	  UTL_Scope	*s = idl_global->scopes()->top_non_null();
	  AST_Decl	*d = NULL;

	  if (s != NULL)
	    d = s->lookup_by_name(yyvsp[0].idlist, I_TRUE);
	  if (d == NULL)
	    idl_global->err()->lookup_error(yyvsp[0].idlist);
	  yyval.dcval = d;
	}
break;
case 127:
#line 956 "idl.yy"
{
	  yyval.dlval = new UTL_DeclList(yyvsp[-1].deval, yyvsp[0].dlval);
	}
break;
case 128:
#line 964 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        }
break;
case 129:
#line 968 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsDeclSeen);

	  if (yyvsp[-3].dlval == NULL)
	    yyval.dlval = new UTL_DeclList(yyvsp[0].deval, NULL);
	  else {
	    yyvsp[-3].dlval->nconc(new UTL_DeclList(yyvsp[0].deval, NULL));
	    yyval.dlval = yyvsp[-3].dlval;
	  }
	}
break;
case 130:
#line 979 "idl.yy"
{
	  yyval.dlval = NULL;
	}
break;
case 133:
#line 991 "idl.yy"
{
	  yyval.dlval = new UTL_DeclList(yyvsp[-1].deval, yyvsp[0].dlval);
	}
break;
case 134:
#line 999 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        }
break;
case 135:
#line 1003 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsDeclSeen);

	  if (yyvsp[-3].dlval == NULL)
	    yyval.dlval = new UTL_DeclList(yyvsp[0].deval, NULL);
	  else {
	    yyvsp[-3].dlval->nconc(new UTL_DeclList(yyvsp[0].deval, NULL));
	    yyval.dlval = yyvsp[-3].dlval;
	  }
	}
break;
case 136:
#line 1014 "idl.yy"
{
	  yyval.dlval = NULL;
	}
break;
case 137:
#line 1021 "idl.yy"
{
	  yyval.deval = new FE_Declarator(new UTL_ScopedName(yyvsp[0].idval, NULL),
				 FE_Declarator::FD_simple, NULL);
	}
break;
case 138:
#line 1029 "idl.yy"
{
	  yyval.deval = new FE_Declarator(new UTL_ScopedName(yyvsp[0].dcval->local_name(), NULL),
				 FE_Declarator::FD_complex,
				 yyvsp[0].dcval);
	}
break;
case 141:
#line 1043 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_long;
	}
break;
case 142:
#line 1047 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_longlong;
        }
break;
case 143:
#line 1051 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_short;
	}
break;
case 144:
#line 1058 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_ulong;
	}
break;
case 145:
#line 1062 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_ulonglong;
        }
break;
case 146:
#line 1066 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_ushort;
	}
break;
case 147:
#line 1073 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_double;
	}
break;
case 148:
#line 1077 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_float;
	}
break;
case 149:
#line 1081 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_longdouble;
        }
break;
case 150:
#line 1088 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_char;
	}
break;
case 151:
#line 1092 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_wchar;
        }
break;
case 152:
#line 1099 "idl.yy"
{
          yyval.etval = AST_Expression::EV_octet;
	}
break;
case 153:
#line 1106 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_bool;
        }
break;
case 154:
#line 1113 "idl.yy"
{
	  yyval.etval = AST_Expression::EV_any;
	}
break;
case 155:
#line 1120 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructSeen);
        }
break;
case 156:
#line 1124 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(yyvsp[0].idval, NULL);
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
	}
break;
case 157:
#line 1148 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructSqSeen);
        }
break;
case 158:
#line 1152 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructBodySeen);
        }
break;
case 159:
#line 1156 "idl.yy"
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
	}
break;
case 163:
#line 1181 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberTypeSeen);
        }
break;
case 164:
#line 1185 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberDeclsSeen);
        }
break;
case 165:
#line 1189 "idl.yy"
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
	  if (yyvsp[-4].dcval != NULL && AST_illegal_recursive_type(yyvsp[-4].dcval))
	    idl_global->err()->error1(UTL_Error::EIDL_RECURSIVE_TYPE, yyvsp[-4].dcval);
	  /*
	   * Create a node representing a struct or exception member
	   * Add it to the enclosing scope
	   */
	  else if (s != NULL && yyvsp[-4].dcval != NULL && yyvsp[-2].dlval != NULL) {
	    l = new UTL_DecllistActiveIterator(yyvsp[-2].dlval);
	    for (;!(l->is_done()); l->next()) {
	      d = l->item();
	      if (d == NULL)
		continue;
 	      AST_Type *tp = d->compose(yyvsp[-4].dcval);
	      if (tp == NULL)
		continue;
	      f = idl_global->gen()->create_field(tp, d->name(), p);
	      (void) s->fe_add_field(f);
	    }
	    delete l;
	  }
	}
break;
case 166:
#line 1222 "idl.yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 167:
#line 1226 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  yyerrok;
	}
break;
case 168:
#line 1234 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionSeen);
        }
break;
case 169:
#line 1238 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionIDSeen);
        }
break;
case 170:
#line 1242 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchSeen);
        }
break;
case 171:
#line 1246 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchOpenParSeen);
        }
break;
case 172:
#line 1250 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchTypeSeen);
        }
break;
case 173:
#line 1254 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(yyvsp[-8].idval, NULL);
	  AST_Union		*u = NULL;
	  AST_Decl		*v = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

          ACE_UNUSED_ARG (v);

	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchCloseParSeen);
	  /*
	   * Create a node representing a union. Add it to its enclosing
	   * scope
	   */
	  if (yyvsp[-2].dcval != NULL && s != NULL) {
 	    AST_ConcreteType    *tp = AST_ConcreteType::narrow_from_decl(yyvsp[-2].dcval);
            if (tp == NULL) {
              idl_global->err()->not_a_type(yyvsp[-2].dcval);
            } else {
	      u = idl_global->gen()->create_union(tp, n, p);
	      (void) s->fe_add_union(u);
 	    }
	  }
	  /*
	   * Push the scope of the union on the scopes stack
	   */
	  idl_global->scopes()->push(u);
	}
break;
case 174:
#line 1283 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionSqSeen);
        }
break;
case 175:
#line 1287 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionBodySeen);
        }
break;
case 176:
#line 1291 "idl.yy"
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
	}
break;
case 177:
#line 1309 "idl.yy"
{
	  yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(yyvsp[0].etval);
	}
break;
case 178:
#line 1313 "idl.yy"
{
	  yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(yyvsp[0].etval);
	}
break;
case 179:
#line 1317 "idl.yy"
{
	  yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(yyvsp[0].etval);
	}
break;
case 180:
#line 1321 "idl.yy"
{
	  yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(yyvsp[0].etval);
	}
break;
case 182:
#line 1326 "idl.yy"
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
	  if (s != NULL && (d = s->lookup_by_name(yyvsp[0].idlist, I_TRUE)) != NULL) {
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
	    idl_global->err()->lookup_error(yyvsp[0].idlist);
	}
break;
case 186:
#line 1396 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionLabelSeen);
        }
break;
case 187:
#line 1400 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemSeen);
        }
break;
case 188:
#line 1404 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_LabellistActiveIterator *l = NULL;
	  AST_UnionLabel	*d = NULL;
	  AST_UnionBranch	*b = NULL;
	  AST_Field		*f = yyvsp[-2].ffval;

	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemCompleted);
	  /*
	   * Create several nodes representing branches of a union.
	   * Add them to the enclosing scope (the union scope)
	   */
	  if (s != NULL && yyvsp[-4].llval != NULL && yyvsp[-2].ffval != NULL) {
	    l = new UTL_LabellistActiveIterator(yyvsp[-4].llval);
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
	}
break;
case 189:
#line 1432 "idl.yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 190:
#line 1436 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  yyerrok;
	}
break;
case 191:
#line 1444 "idl.yy"
{
	  yyval.llval = new UTL_LabelList(yyvsp[-1].ulval, yyvsp[0].llval);
	}
break;
case 192:
#line 1451 "idl.yy"
{
	  if (yyvsp[-1].llval == NULL)
	    yyval.llval = new UTL_LabelList(yyvsp[0].ulval, NULL);
	  else {
	    yyvsp[-1].llval->nconc(new UTL_LabelList(yyvsp[0].ulval, NULL));
	    yyval.llval = yyvsp[-1].llval;
	  }
	}
break;
case 193:
#line 1460 "idl.yy"
{
	  yyval.llval = NULL;
	}
break;
case 194:
#line 1467 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DefaultSeen);
        }
break;
case 195:
#line 1471 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

	  yyval.ulval = idl_global->gen()->
	            create_union_label(AST_UnionLabel::UL_default,
				       NULL);
	}
break;
case 196:
#line 1479 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_CaseSeen);
        }
break;
case 197:
#line 1483 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelExprSeen);
        }
break;
case 198:
#line 1487 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

	  yyval.ulval = idl_global->gen()->create_union_label(AST_UnionLabel::UL_label,
						     yyvsp[-2].exval);
	}
break;
case 199:
#line 1497 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemTypeSeen);
        }
break;
case 200:
#line 1501 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemDeclSeen);
	  /*
	   * Check for illegal recursive use of type
	   */
	  if (yyvsp[-2].dcval != NULL && AST_illegal_recursive_type(yyvsp[-2].dcval))
	    idl_global->err()->error1(UTL_Error::EIDL_RECURSIVE_TYPE, yyvsp[-2].dcval);
	  /*
	   * Create a field in a union branch
	   */
	  else if (yyvsp[-2].dcval == NULL || yyvsp[0].deval == NULL)
	    yyval.ffval = NULL;
	  else {
	    AST_Type *tp = yyvsp[0].deval->compose(yyvsp[-2].dcval);
	    if (tp == NULL)
	      yyval.ffval = NULL;
 	    else
	      yyval.ffval = idl_global->gen()->create_field(tp,
			        		   yyvsp[0].deval->name(),
			        		   idl_global->pragmas());
	  }
	}
break;
case 201:
#line 1527 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumSeen);
        }
break;
case 202:
#line 1531 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(yyvsp[0].idval, NULL);
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
	}
break;
case 203:
#line 1558 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumSqSeen);
        }
break;
case 204:
#line 1562 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumBodySeen);
        }
break;
case 205:
#line 1566 "idl.yy"
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
	}
break;
case 207:
#line 1585 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumCommaSeen);
        }
break;
case 210:
#line 1594 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n =
		new UTL_ScopedName(new Identifier(yyvsp[0].strval, 1, 0, I_FALSE), NULL);
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
	}
break;
case 211:
#line 1618 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceCommaSeen);
        }
break;
case 212:
#line 1622 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceExprSeen);
        }
break;
case 213:
#line 1626 "idl.yy"
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
	  if (yyvsp[-2].exval == NULL || yyvsp[-2].exval->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error(yyvsp[-2].exval, AST_Expression::EV_ulong);
	    yyval.dcval = NULL;
	  } else if (yyvsp[-5].dcval == NULL) {
	    yyval.dcval = NULL;
	  } else {
 	    AST_Type *tp = AST_Type::narrow_from_decl(yyvsp[-5].dcval);
	    if (tp == NULL)
	      yyval.dcval = NULL;
	    else {
	      yyval.dcval = idl_global->gen()->create_sequence(yyvsp[-2].exval, tp);
	      /*
	       * Add this AST_Sequence to the types defined in the global scope
	       */
	      (void) idl_global->root()
		        ->fe_add_sequence(AST_Sequence::narrow_from_decl(yyval.dcval));
 	    }
	  }
	}
break;
case 214:
#line 1657 "idl.yy"
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
	  if (yyvsp[-1].dcval == NULL)
	    yyval.dcval = NULL;
	  else {
	    AST_Type *tp = AST_Type::narrow_from_decl(yyvsp[-1].dcval);
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
	}
break;
case 215:
#line 1690 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSeen);
	  /*
	   * Push a sequence marker on scopes stack
	   */
	  idl_global->scopes()->push(NULL);
	}
break;
case 216:
#line 1698 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSqSeen);
        }
break;
case 217:
#line 1702 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceTypeSeen);
	  yyval.dcval = yyvsp[0].dcval;
        }
break;
case 218:
#line 1711 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        }
break;
case 219:
#line 1715 "idl.yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        }
break;
case 220:
#line 1719 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringQsSeen);
	  /*
	   * Create a node representing a string
	   */
	  if (yyvsp[-2].exval == NULL || yyvsp[-2].exval->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error(yyvsp[-2].exval, AST_Expression::EV_ulong);
	    yyval.dcval = NULL;
	  } else {
	    yyval.dcval = idl_global->gen()->create_string(yyvsp[-2].exval);
	    /*
	     * Add this AST_String to the types defined in the global scope
	     */
	    (void) idl_global->root()
		      ->fe_add_string(AST_String::narrow_from_decl(yyval.dcval));
	  }
	}
break;
case 221:
#line 1737 "idl.yy"
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
	}
break;
case 222:
#line 1755 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        }
break;
case 223:
#line 1763 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        }
break;
case 224:
#line 1767 "idl.yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        }
break;
case 225:
#line 1771 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringQsSeen);
	  /*
	   * Create a node representing a string
	   */
	  if (yyvsp[-2].exval == NULL || yyvsp[-2].exval->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error(yyvsp[-2].exval, AST_Expression::EV_ulong);
	    yyval.dcval = NULL;
	  } else {
	    yyval.dcval = idl_global->gen()->create_wstring(yyvsp[-2].exval);
	    /*
	     * Add this AST_String to the types defined in the global scope
	     */
	    (void) idl_global->root()
		      ->fe_add_string(AST_String::narrow_from_decl(yyval.dcval));
	  }
	}
break;
case 226:
#line 1789 "idl.yy"
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
	}
break;
case 227:
#line 1807 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        }
break;
case 228:
#line 1814 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ArrayIDSeen);
        }
break;
case 229:
#line 1818 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ArrayCompleted);
	  /*
	   * Create a node representing an array
	   */
	  if (yyvsp[0].elval != NULL) {
	     yyval.dcval = idl_global->gen()->create_array(new UTL_ScopedName(yyvsp[-2].idval, NULL),
						  yyvsp[0].elval->length(), yyvsp[0].elval);
	  }
	}
break;
case 230:
#line 1832 "idl.yy"
{
	  yyval.elval = new UTL_ExprList(yyvsp[-1].exval, yyvsp[0].elval);
	}
break;
case 231:
#line 1839 "idl.yy"
{
	  if (yyvsp[-1].elval == NULL)
	    yyval.elval = new UTL_ExprList(yyvsp[0].exval, NULL);
	  else {
	    yyvsp[-1].elval->nconc(new UTL_ExprList(yyvsp[0].exval, NULL));
	    yyval.elval = yyvsp[-1].elval;
	  }
	}
break;
case 232:
#line 1848 "idl.yy"
{
	  yyval.elval = NULL;
	}
break;
case 233:
#line 1855 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimSqSeen);
        }
break;
case 234:
#line 1859 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimExprSeen);
        }
break;
case 235:
#line 1863 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimQsSeen);
	  /*
	   * Array dimensions are expressions which must be coerced to
	   * positive integers
	   */
	  if (yyvsp[-2].exval == NULL || yyvsp[-2].exval->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error(yyvsp[-2].exval, AST_Expression::EV_ulong);
	    yyval.exval = NULL;
	  } else
	    yyval.exval = yyvsp[-2].exval;
	}
break;
case 236:
#line 1880 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrSeen);
        }
break;
case 237:
#line 1884 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrTypeSeen);
        }
break;
case 238:
#line 1888 "idl.yy"
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
	  if (s != NULL && yyvsp[-2].dcval != NULL && yyvsp[0].dlval != NULL) {
	    l = new UTL_DecllistActiveIterator(yyvsp[0].dlval);
	    for (;!(l->is_done()); l->next()) {
	      d = l->item();
	      if (d == NULL)
		continue;
	      AST_Type *tp = d->compose(yyvsp[-2].dcval);
 	      if (tp == NULL)
	 	continue;
	      a = idl_global->gen()->create_attribute(yyvsp[-5].bval, tp, d->name(), p);
	      /*
	       * Add one attribute to the enclosing scope
	       */
	      (void) s->fe_add_attribute(a);
	    }
	    delete l;
	  }
	}
break;
case 239:
#line 1922 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrROSeen);
	  yyval.bval = I_TRUE;
	}
break;
case 240:
#line 1927 "idl.yy"
{
	  yyval.bval = I_FALSE;
	}
break;
case 241:
#line 1934 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSeen);
	}
break;
case 242:
#line 1938 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(yyvsp[0].idval, NULL);
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
	}
break;
case 243:
#line 1962 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSqSeen);
        }
break;
case 244:
#line 1966 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptBodySeen);
        }
break;
case 245:
#line 1970 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptQsSeen);
	  /*
	   * Done with this exception. Pop its scope from the scope stack
	   */
	  idl_global->scopes()->pop();
	}
break;
case 246:
#line 1982 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpTypeSeen);
        }
break;
case 247:
#line 1986 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n =
		new UTL_ScopedName(new Identifier(yyvsp[0].strval, 1, 0, I_FALSE), NULL);
	  AST_Operation		*o = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_OpIDSeen);
	  /*
	   * Create a node representing an operation on an interface
	   * and add it to its enclosing scope
	   */
	  if (s != NULL && yyvsp[-2].dcval != NULL) {
	    AST_Type *tp = AST_Type::narrow_from_decl(yyvsp[-2].dcval);
            if (tp == NULL) {
              idl_global->err()->not_a_type(yyvsp[-2].dcval);
            } else if (tp->node_type() == AST_Decl::NT_except) {
              idl_global->err()->not_a_type(yyvsp[-2].dcval);
            } else {
	      o = idl_global->gen()->create_operation(tp, yyvsp[-3].ofval, n, p);
	      (void) s->fe_add_operation(o);
	    }
	  }
	  /*
	   * Push the operation scope onto the scopes stack
	   */
	  idl_global->scopes()->push(o);
	}
break;
case 248:
#line 2015 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParsCompleted);
        }
break;
case 249:
#line 2019 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseCompleted);
        }
break;
case 250:
#line 2023 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Operation		*o = NULL;

	  idl_global->set_parse_state(IDL_GlobalData::PS_OpCompleted);
	  /*
	   * Add exceptions and context to the operation
	   */
	  if (s != NULL && s->scope_node_type() == AST_Decl::NT_op) {
	    o = AST_Operation::narrow_from_scope(s);

	    if (yyvsp[-2].nlval != NULL && o != NULL)
	      (void) o->fe_add_exceptions(yyvsp[-2].nlval);
	    if (yyvsp[0].slval != NULL)
	      (void) o->fe_add_context(yyvsp[0].slval);
	  }
	  /*
	   * Done with this operation. Pop its scope from the scopes stack
	   */
	  idl_global->scopes()->pop();
	}
break;
case 251:
#line 2048 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
	  yyval.ofval = AST_Operation::OP_oneway;
	}
break;
case 252:
#line 2053 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
	  yyval.ofval = AST_Operation::OP_idempotent;
	}
break;
case 253:
#line 2058 "idl.yy"
{
	  yyval.ofval = AST_Operation::OP_noflags;
	}
break;
case 255:
#line 2066 "idl.yy"
{
	  yyval.dcval =
	    idl_global->scopes()->bottom()
	       ->lookup_primitive_type(AST_Expression::EV_void);
	}
break;
case 256:
#line 2075 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        }
break;
case 257:
#line 2079 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        }
break;
case 258:
#line 2083 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        }
break;
case 259:
#line 2088 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        }
break;
case 261:
#line 2098 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParCommaSeen);
        }
break;
case 264:
#line 2107 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParDirSeen);
        }
break;
case 265:
#line 2111 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParTypeSeen);
        }
break;
case 266:
#line 2115 "idl.yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Argument		*a = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParDeclSeen);
	  /*
	   * Create a node representing an argument to an operation
	   * Add it to the enclosing scope (the operation scope)
	   */
	  if (yyvsp[-2].dcval != NULL && yyvsp[0].deval != NULL && s != NULL) {
	    AST_Type *tp = yyvsp[0].deval->compose(yyvsp[-2].dcval);
	    if (tp != NULL) {
	      a = idl_global->gen()->create_argument(yyvsp[-4].dival, tp, yyvsp[0].deval->name(), p);
	      (void) s->fe_add_argument(a);
	    }
	  }
	}
break;
case 267:
#line 2137 "idl.yy"
{
	  yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(yyvsp[0].etval);
	}
break;
case 269:
#line 2142 "idl.yy"
{
	  UTL_Scope	*s = idl_global->scopes()->top_non_null();
	  AST_Decl	*d = NULL;

	  if (s != NULL)
	    d = s->lookup_by_name(yyvsp[0].idlist, I_TRUE);
	  if (d == NULL)
	    idl_global->err()->lookup_error(yyvsp[0].idlist);
	  yyval.dcval = d;
	}
break;
case 270:
#line 2156 "idl.yy"
{
	  yyval.dival = AST_Argument::dir_IN;
	}
break;
case 271:
#line 2160 "idl.yy"
{
	  yyval.dival = AST_Argument::dir_OUT;
	}
break;
case 272:
#line 2164 "idl.yy"
{
	  yyval.dival = AST_Argument::dir_INOUT;
	}
break;
case 273:
#line 2171 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSeen);
        }
break;
case 274:
#line 2175 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSqSeen);
        }
break;
case 275:
#line 2180 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseQsSeen);
	  yyval.nlval = yyvsp[-1].nlval;
	}
break;
case 276:
#line 2185 "idl.yy"
{
	  yyval.nlval = NULL;
	}
break;
case 277:
#line 2192 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSeen);
        }
break;
case 278:
#line 2196 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSqSeen);
        }
break;
case 279:
#line 2201 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextQsSeen);
	  yyval.slval = yyvsp[-1].slval;
	}
break;
case 280:
#line 2206 "idl.yy"
{
	  yyval.slval = NULL;
	}
break;
case 281:
#line 2213 "idl.yy"
{
	  yyval.slval = new UTL_StrList(yyvsp[-1].sval, yyvsp[0].slval);
	}
break;
case 282:
#line 2221 "idl.yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextCommaSeen);
        }
break;
case 283:
#line 2225 "idl.yy"
{
	  if (yyvsp[-3].slval == NULL)
	    yyval.slval = new UTL_StrList(yyvsp[0].sval, NULL);
	  else {
	    yyvsp[-3].slval->nconc(new UTL_StrList(yyvsp[0].sval, NULL));
	    yyval.slval = yyvsp[-3].slval;
	  }
	}
break;
case 284:
#line 2234 "idl.yy"
{
	  yyval.slval = NULL;
	}
break;
#line 3242 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#ifdef YYDEBUG
        if (5 < yydebug)
            printf("yydebug: after reduction, shifting from state 0 to\
 state %d\n", YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                if (5 < yydebug)
                    printf("yydebug: state %d, reading %d (%s)\n",
                        YYFINAL, yychar, yys);
                else
                    YYDEBUG_LOOK_AHEAD(YYFINAL, yychar, yys, yyssp-yyss);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#ifdef YYDEBUG
    if (5 < yydebug)
        printf("yydebug: after reduction, shifting from state %d \
to state %d\n", *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
