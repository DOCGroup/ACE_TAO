// $Id$

#ifndef lint
char tao_yysccsid[] = "@(#)yaccpar	1.4 (Berkeley) 02/25/90 \n\
 Modified 5/2/90 by J. Roskind to support graphic debugging modes";
#endif
#line 74 "fe/idl.tao_yy"
#include "idl.h"
#include "idl_extern.h"
#include "fe_private.h"
#include <stdio.h>

#if (defined(apollo) || defined(hpux)) && defined(__cplusplus)
extern	"C" int tao_yywrap();
#endif	/* (defined(apollo) || defined(hpux)) && defined(__cplusplus)*/

void tao_yyerror (const char *);
int tao_yylex (void);
extern "C" int tao_yywrap (void);
extern char tao_yytext[];
extern int tao_yyleng;
#define TAO_YYDEBUG_LEXER_TEXT (tao_yytext[tao_yyleng] = '\0', tao_yytext)
/* Force the pretty debugging code to compile.*/
#define TAO_YYDEBUG 1
#line 97 "fe/idl.tao_yy"
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
} TAO_YYSTYPE;
#line 51 "y.tab.cpp"
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
#define IDL_NATIVE 292
#define IDL_INTEGER_LITERAL 293
#define IDL_STRING_LITERAL 294
#define IDL_CHARACTER_LITERAL 295
#define IDL_FLOATING_PT_LITERAL 296
#define IDL_TRUETOK 297
#define IDL_FALSETOK 298
#define IDL_SCOPE_DELIMITOR 299
#define IDL_LEFT_SHIFT 300
#define IDL_RIGHT_SHIFT 301
#define TAO_YYERRCODE 256
short tao_yylhs[] = {                                        -1,
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
   43,   44,  108,   66,   66,   66,   66,   66,  109,  107,
    1,    1,    2,    2,    2,   56,   56,   56,   56,   56,
   56,    4,    4,    4,    3,    3,    3,   28,  110,   29,
   29,   60,   60,   30,  111,   31,   31,   61,   62,   49,
   49,   54,   54,   54,   55,   55,   55,   52,   52,   52,
   50,   50,   57,   51,   53,  112,  113,  115,  116,    7,
  114,  118,  118,  119,  120,  117,  121,  117,  122,  123,
  124,  125,  126,  127,  129,  130,   10,    9,    9,    9,
    9,    9,    9,  128,  132,  132,  133,  134,  131,  135,
  131,   26,   27,   27,  136,   46,  137,  138,   46,  139,
   47,  140,  141,  143,  144,    8,  142,  147,  146,  146,
  145,  148,  149,    5,    5,  150,  151,   13,  153,  154,
    6,    6,  152,  156,  157,   14,   14,  155,  158,   11,
   24,   25,   25,  159,  160,   45,  161,  162,   95,   63,
   63,  163,  164,  165,  166,   72,  167,  169,  170,  171,
   97,   59,   59,   59,   12,   12,  172,  168,  174,  168,
  173,  177,  176,  176,  178,  179,  175,   15,   15,   15,
   58,   58,   58,  180,  181,   23,   23,  182,  183,   17,
   17,   18,  184,   19,   19,
};
short tao_yylen[] = {                                         2,
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
    1,    1,    0,    3,    1,    1,    1,    2,    0,    3,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    2,    0,    4,
    0,    1,    1,    2,    0,    4,    0,    1,    1,    1,
    1,    1,    2,    1,    2,    3,    2,    1,    1,    2,
    1,    1,    1,    1,    1,    0,    0,    0,    0,    9,
    2,    2,    0,    0,    0,    5,    0,    3,    0,    0,
    0,    0,    0,    0,    0,    0,   17,    1,    1,    1,
    1,    1,    1,    2,    2,    0,    0,    0,    5,    0,
    3,    2,    2,    0,    0,    3,    0,    0,    5,    0,
    3,    0,    0,    0,    0,    9,    2,    0,    4,    0,
    1,    0,    0,    6,    2,    0,    0,    5,    0,    0,
    6,    1,    1,    0,    0,    6,    1,    1,    0,    3,
    2,    2,    0,    0,    0,    5,    0,    0,    6,    1,
    0,    0,    0,    0,    0,    9,    0,    0,    0,    0,
   10,    1,    1,    0,    1,    1,    0,    3,    0,    4,
    2,    0,    4,    0,    0,    0,    5,    1,    1,    1,
    1,    1,    1,    0,    0,    6,    0,    0,    0,    6,
    0,    2,    0,    4,    0,
};
short tao_yydefred[] = {                                      0,
   14,   58,   16,   27,  103,  156,  169,  202,  242,    0,
    0,  105,  107,  106,   23,    0,    4,    1,    0,    6,
    8,   10,   12,   21,   22,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   56,  108,  138,    0,   30,   29,
    0,    2,    0,    0,    0,    0,   15,    0,  144,    0,
  148,  149,  151,  152,  153,  154,  223,  228,   52,   68,
   69,    0,   59,   63,   64,   66,   67,  140,  141,   65,
   51,    0,    0,   17,   28,  155,  216,  109,  111,  112,
  114,  122,  123,  125,  127,  126,    0,  124,    0,  116,
  118,  119,  117,  121,  113,  120,  104,  157,  170,  203,
  243,   24,    0,    5,    7,    9,   11,   13,  143,  150,
    0,  147,    0,   54,    0,  219,  224,    0,    0,    0,
  212,  215,    0,    0,    0,    0,   34,    0,   31,  146,
   53,    0,   60,    0,    0,   18,  217,  139,  110,  131,
  132,  133,    0,    0,  158,  171,  204,  244,    0,    0,
   55,    0,   96,   97,   98,   99,  100,  101,    0,    0,
    0,    0,    0,  102,    0,    0,    0,    0,    0,    0,
   85,   89,   94,  220,  225,    0,    0,    0,    0,  213,
    0,    0,    0,  163,   45,  240,  252,  253,    0,    0,
   35,   37,   39,    0,   33,   41,   43,   48,   61,   90,
   91,   92,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   19,  218,  129,  234,  230,
  233,    0,  167,  164,  159,  163,  172,  211,  205,  210,
    0,    0,  256,  269,  247,  255,    0,  268,  237,    0,
    0,    0,   26,    0,    0,    0,    0,   95,    0,    0,
    0,    0,    0,    0,    0,   86,   87,   88,  221,  226,
    0,    0,    0,    0,  214,    0,    0,    0,    0,    0,
    0,    0,  162,    0,   46,    0,    0,   36,   38,   40,
   42,   44,    0,   62,   71,   20,  130,  235,  232,  168,
  165,  160,    0,  182,  173,    0,  178,  179,  181,  180,
  206,  208,  246,  248,  238,    0,    0,    0,    0,    0,
    0,  236,  166,  174,  209,    0,  249,  239,  137,    0,
    0,    0,    0,    0,  175,  258,  271,  272,  273,  265,
    0,  264,  274,  250,  135,    0,    0,  260,    0,    0,
    0,    0,  190,  197,  195,  187,  194,  176,  186,  266,
  262,  275,  278,  251,  136,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  191,  198,  196,  200,
  188,  193,  177,  185,  267,  263,    0,  279,    0,    0,
    0,  276,    0,  199,  201,  189,  285,    0,    0,  280,
  283,    0,  284,
};
short tao_yydgoto[] = {                                      11,
  224,   79,   80,   81,   82,   83,   84,   85,  295,   86,
  138,  235,   87,   88,  236,  163,  354,  388,  389,  129,
  150,   40,  334,  220,  264,  346,  360,  139,  178,  318,
  324,   15,  284,  164,  165,  166,  167,  168,  169,  170,
  171,  172,  173,  174,  221,  347,  371,   63,   90,   91,
   92,   93,   94,   68,   69,   95,   96,  330,  189,  140,
  141,  142,  190,   16,   71,   17,   18,   19,   41,   20,
   43,   21,   44,   22,   45,   23,   46,   26,   28,  118,
  176,  261,   24,   25,   38,  149,  127,  194,   29,  103,
  195,  240,  241,  242,  196,  244,  197,  245,  232,  246,
  113,  132,   27,  115,  152,  247,   97,   30,  120,  262,
  342,   31,  123,  225,  181,  268,  273,  231,  267,  307,
  266,   32,  124,  182,  270,  308,  320,  348,  336,  361,
  349,  362,  359,  381,  356,  358,  357,  379,  380,   33,
  125,  229,  183,  271,  230,  272,  309,  144,  222,  119,
  177,   72,  134,  214,   73,  135,  215,  179,  263,  306,
  277,  311,   34,  126,  184,  274,  276,  317,  310,  323,
  341,  321,  331,  322,  332,  339,  364,  337,  363,  340,
  365,  366,  383,  392,
};
short tao_yysindex[] = {                                    333,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -194,
    0,    0,    0,    0,    0,   10,    0,    0,  333,    0,
    0,    0,    0,    0,    0,   25,  254, -177, -194,  162,
 -194, -194, -194, -194,    0,    0,    0,  -34,    0,    0,
   47,    0,   52,   61,   78,  103,    0,  -54,    0, -187,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -131,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  126,  150,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   77,    0, -131,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -183,    0,    0,    0,    0,    0,    0,    0,
  -90,    0, -194,    0, -194,    0,    0,   72,  152, -194,
    0,    0,   95,  -53,  119,  122,    0, -131,    0,    0,
    0, -194,    0,   26,   26,    0,    0,    0,    0,    0,
    0,    0,    0,   26,    0,    0,    0,    0,  303,  212,
    0,  209,    0,    0,    0,    0,    0,    0,  -30,  -30,
  -30,   26, -131,    0,  156,  182,  246, -154,  159,   89,
    0,    0,    0,    0,    0,  333,  211,  242,  196,    0,
  137,  248,   32,    0,    0,    0,    0,    0,  228,   13,
    0,    0,    0,  165,    0,    0,    0,    0,    0,    0,
    0,    0,  250,   26,   26,   26,   26,   26,   26,   26,
   26,   26,   26,  230,  235,    0,    0,    0,    0,    0,
    0,  236,    0,    0,    0,    0,    0,    0,    0,    0,
  137,  243,    0,    0,    0,    0, -131,    0,    0,  245,
  247,  249,    0,  251,  253, -183,   26,    0,  182,  246,
 -154,  159,  159,   89,   89,    0,    0,    0,    0,    0,
  180, -194,   26,  196,    0,  257, -194,  188,  137,  287,
  192,  263,    0,  193,    0,   71,  271,    0,    0,    0,
    0,    0, -131,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   67,    0,    0, -131,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  238,  277,  296,   32,  298,
 -194,    0,    0,    0,    0,    0,    0,    0,    0,  219,
  302,  -63,   55,  300,    0,    0,    0,    0,    0,    0,
  305,    0,    0,    0,    0,  -74,  271,    0,  304,  307,
   58, -194,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  293,   26,  297,  162, -102,
  229,  -74, -194,  -63, -183,  318,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  319,    0,  301, -194,
  306,    0,   69,    0,    0,    0,    0,  323,  324,    0,
    0,   79,    0,
};
short tao_yyrindex[] = {                                    371,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  -27,    0,    0,   17,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  -19,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  115,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   28,   38,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   57,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    5,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    4,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   94,    0,    0,    0,    0,    0,  112,    9,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -29,    0,   99,  129,  155,  107,   50,   -7,
    0,    0,    0,    0,    0,  259,    0,  326,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  261,    0,    0,    0,    0,    0,  130,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  157,  268,
  135,   56,   66,   -1,   40,    0,    0,    0,    0,    0,
    0,    0,    0,  140,    0,    0,    0,    0,  265,    0,
    0,  266,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   18,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  347,    0,    0,  355,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  -14,    0,    0,    0,    0,
    0,    0,  -56,  338,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  375,    0,
  251,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  187,
    0,  292,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  377,    0,
    0,    0,    0,
};
short tao_yygindex[] = {                                      0,
  -24,  244,    0,    0,    0,  -22,   54,    7,    0,   60,
    0,    0,    0,  393, -207,  -26,    0,    0,    0,   76,
    0,    0,    0,    0,    0,    0,    0,  175,    0,    0,
    0,    0,    0, -134,    0,  218,  240,  217,   39,   51,
   21,   80,    0,  -88,  179,   86,    0,    0,  -16,  -12,
    8,  420,    0,    0,    0, -148,   12,    0,    0, -209,
   -8,    0,    0,    0,  -10,  299,    1,    0,    0,  317,
    0,  320,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  286,  258,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  108,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  163,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  136,    0,    0,    0,    0,    0,
    0,    0,    0,    0,
};
#define TAO_YYTABLESIZE 625
short tao_yytable[] = {                                      37,
   62,   36,  277,   89,   60,   78,   13,   93,   93,  162,
   64,   93,   93,   93,   65,   93,    3,   93,   75,   42,
   98,   99,  100,  101,  142,   13,  257,  203,   93,   93,
   82,   57,   93,   82,   66,   82,   83,   82,   70,   83,
  238,   83,  142,   83,   50,  145,  175,   50,  145,   47,
   82,   82,  287,   12,   82,  180,   83,   83,   49,   14,
   83,   49,   35,   93,   93,  162,  145,   39,  159,  305,
  160,  222,   12,   35,  111,  112,  128,   84,   14,   74,
   84,  227,   84,   47,   84,   82,   82,   79,  102,  222,
   79,   83,   83,   80,   93,   32,   80,   84,   84,  227,
  115,   84,  131,   81,  133,  104,   81,   79,   79,  143,
  105,   79,  285,   80,   80,   59,   82,   80,  115,  106,
  121,  151,   83,   81,   81,  213,   50,   81,  238,  350,
  211,   47,   84,   84,  138,  212,  107,  138,  122,   72,
   49,    3,   79,   79,   77,  207,  208,   77,   80,   80,
   89,  161,  138,  375,   89,   13,   72,   72,   81,   81,
   72,  108,  237,   84,   77,   77,  234,  114,   77,   73,
  385,  130,   78,   79,  288,   78,  216,  344,  345,   80,
  231,  343,   13,  231,  229,  116,   73,   73,  238,   81,
   73,   72,   78,   78,  136,   75,   78,   74,  231,   77,
   77,  209,   12,  210,   89,  344,  345,  109,   14,  117,
  110,  137,   75,   75,   74,   74,   75,  145,   74,  283,
  146,   73,  368,  327,  328,  329,   35,   78,   78,   12,
   77,  256,  257,  258,  277,   14,   25,  142,  200,  201,
  202,  147,   89,  296,  148,  252,  253,   75,   75,   74,
  237,  143,   73,  297,  234,  198,  143,  298,   78,  254,
  255,  145,  153,  154,  155,  156,  157,  158,   59,  199,
   93,   93,  259,  259,  259,  205,  294,  299,   75,  204,
   74,  300,   35,  206,  222,  218,  219,  227,  228,  243,
  248,  259,   82,   82,  227,  239,  260,  265,   83,   83,
   37,  275,  319,  278,  286,  279,  302,  280,   76,  281,
  237,  282,  292,  115,  234,  290,  301,  303,  153,  154,
  155,  156,  157,  158,   59,   76,   76,  304,  109,   76,
  312,   37,   89,  355,  370,  313,  314,  316,  128,   84,
   84,  325,  326,  335,  333,  338,  352,  351,  353,   79,
   79,  367,  143,  373,  369,   80,   80,  378,  384,  382,
   76,   76,  387,  390,  386,   81,   81,  391,  254,  143,
    3,   70,  393,  254,  254,  254,  254,  254,  254,  254,
  254,  254,  254,    3,  128,  245,  270,  142,  254,  161,
  207,   76,  223,   35,  241,  183,  134,  254,   48,   49,
   50,   51,   52,   53,   54,   55,   56,   76,    6,    7,
  254,    8,   77,   57,   58,  261,  184,  282,   35,   61,
  217,  249,  251,   48,   49,   50,   51,   52,   53,   54,
   55,   56,   76,    6,    7,   59,    8,   77,   57,   58,
  377,  291,  289,  192,  250,  372,   67,  191,  192,  192,
  192,  192,  192,  192,  192,  192,  192,  192,  192,  192,
   59,  192,  192,  192,  192,  192,  226,   35,  193,  374,
    0,  315,   48,   49,   50,   51,   52,   53,   54,   55,
   56,   76,    0,  269,   35,  192,   77,   57,   58,   48,
   49,   50,   51,   52,   53,   54,   55,   56,   76,  376,
    0,    0,    0,    0,   57,    0,    0,    0,    0,   59,
   35,    0,    0,  233,    0,   48,   49,   50,   51,   52,
   53,   54,   55,   56,    0,    0,   59,   35,    0,    0,
   57,   58,   48,   49,   50,   51,   52,   53,   54,   55,
   56,   76,    0,   35,    0,    0,    0,   57,  293,   49,
   50,    0,   59,   53,   54,   55,   56,    0,  185,    0,
    2,    8,    0,    5,    0,    0,    0,    0,    0,   59,
    0,    0,    0,    0,    6,    7,    0,    8,    0,    0,
    0,    9,    0,    0,  186,   59,  187,  188,    1,    0,
    2,    3,    4,    5,   10,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    6,    7,    0,    8,    0,    0,
    0,    9,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   10,
};
short tao_yycheck[] = {                                      10,
   27,   10,   59,   30,   27,   30,    0,   37,   38,   40,
   27,   41,   42,   43,   27,   45,    0,   47,   29,   19,
   31,   32,   33,   34,   44,   19,   41,  162,   58,   59,
   38,   59,   62,   41,   27,   43,   38,   45,   27,   41,
  189,   43,   62,   45,   41,   41,  135,   44,   44,   41,
   58,   59,  262,    0,   62,  144,   58,   59,   41,    0,
   62,   44,  257,   93,   94,   40,   62,   58,   43,  277,
   45,   44,   19,  257,  262,  263,  103,   38,   19,  257,
   41,   44,   43,   59,   45,   93,   94,   38,  123,   62,
   41,   93,   94,   38,  124,  123,   41,   58,   59,   62,
   44,   62,  113,   38,  115,   59,   41,   58,   59,  120,
   59,   62,  247,   58,   59,  299,  124,   62,   62,   59,
   44,  132,  124,   58,   59,   37,  123,   62,  277,  337,
   42,  123,   93,   94,   41,   47,   59,   44,   62,   41,
  123,  125,   93,   94,   38,  300,  301,   41,   93,   94,
  177,  126,   59,  363,  181,  149,   58,   59,   93,   94,
   62,   59,  189,  124,   58,   59,  189,  299,   62,   41,
  380,  262,   38,  124,  263,   41,  176,  280,  281,  124,
   41,  256,  176,   44,   91,   60,   58,   59,  337,  124,
   62,   93,   58,   59,  123,   41,   62,   41,   59,   93,
   94,   43,  149,   45,  231,  280,  281,  262,  149,   60,
  265,   60,   58,   59,   58,   59,   62,  123,   62,  246,
  274,   93,  357,  287,  288,  289,  257,   93,   94,  176,
  124,  211,  212,  213,  291,  176,  125,  257,  159,  160,
  161,  123,  269,  270,  123,  207,  208,   93,   94,   93,
  277,  262,  124,  270,  277,   44,  267,  270,  124,  209,
  210,  257,  293,  294,  295,  296,  297,  298,  299,   61,
  300,  301,  287,  288,  289,   94,  270,  270,  124,  124,
  124,  270,  257,   38,  257,   44,   91,   40,  257,  125,
   41,   62,  300,  301,  257,  283,   62,   62,  300,  301,
  311,   59,  311,   59,  125,   59,   44,   59,   41,   59,
  337,   59,  125,  257,  337,   59,  125,  125,  293,  294,
  295,  296,  297,  298,  299,   58,   59,  257,  262,   62,
   93,  342,  359,  342,  359,   59,   41,   40,  365,  300,
  301,  123,   41,   44,  290,   41,   40,   44,  291,  300,
  301,   59,  363,  125,   58,  300,  301,   40,   58,   41,
   93,   94,  294,   41,   59,  300,  301,   44,  257,  380,
    0,  257,  294,  262,  263,  264,  265,  266,  267,  268,
  269,  270,  271,  125,   59,  125,  257,   41,  277,  125,
  125,  124,  256,  257,  283,   41,   59,  286,  262,  263,
  264,  265,  266,  267,  268,  269,  270,  271,  272,  273,
  299,  275,  276,  277,  278,   41,  125,   41,  257,   27,
  177,  204,  206,  262,  263,  264,  265,  266,  267,  268,
  269,  270,  271,  272,  273,  299,  275,  276,  277,  278,
  365,  267,  264,  257,  205,  360,   27,  149,  262,  263,
  264,  265,  266,  267,  268,  269,  270,  271,  272,  273,
  299,  275,  276,  277,  278,  149,  181,  257,  149,  362,
   -1,  309,  262,  263,  264,  265,  266,  267,  268,  269,
  270,  271,   -1,  226,  257,  299,  276,  277,  278,  262,
  263,  264,  265,  266,  267,  268,  269,  270,  271,  364,
   -1,   -1,   -1,   -1,  277,   -1,   -1,   -1,   -1,  299,
  257,   -1,   -1,  286,   -1,  262,  263,  264,  265,  266,
  267,  268,  269,  270,   -1,   -1,  299,  257,   -1,   -1,
  277,  278,  262,  263,  264,  265,  266,  267,  268,  269,
  270,  271,   -1,  257,   -1,   -1,   -1,  277,  262,  263,
  264,   -1,  299,  267,  268,  269,  270,   -1,  256,   -1,
  258,  275,   -1,  261,   -1,   -1,   -1,   -1,   -1,  299,
   -1,   -1,   -1,   -1,  272,  273,   -1,  275,   -1,   -1,
   -1,  279,   -1,   -1,  282,  299,  284,  285,  256,   -1,
  258,  259,  260,  261,  292,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  272,  273,   -1,  275,   -1,   -1,
   -1,  279,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  292,
};
#define TAO_YYFINAL 11
#ifndef TAO_YYDEBUG
#define TAO_YYDEBUG 0
#endif
#define TAO_YYMAXTOKEN 301
#if TAO_YYDEBUG
char *tao_yyname[] = {
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
"IDL_NATIVE","IDL_INTEGER_LITERAL","IDL_STRING_LITERAL","IDL_CHARACTER_LITERAL",
"IDL_FLOATING_PT_LITERAL","IDL_TRUETOK","IDL_FALSETOK","IDL_SCOPE_DELIMITOR",
"IDL_LEFT_SHIFT","IDL_RIGHT_SHIFT",
};
char *tao_yyrule[] = {
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
"type_dcl : IDL_NATIVE simple_declarator",
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
#define tao_yyclearin (tao_yychar=(-1))
#define tao_yyerrok (tao_yyerrflag=0)
#ifndef TAO_YYSTACKSIZE
#ifdef TAO_YYMAXDEPTH
#define TAO_YYSTACKSIZE TAO_YYMAXDEPTH
#else
#define TAO_YYSTACKSIZE 300
#endif
#endif
int tao_yydebug;
int tao_yynerrs;
int tao_yyerrflag;
int tao_yychar;
short *tao_yyssp;
TAO_YYSTYPE *tao_yyvsp;
TAO_YYSTYPE tao_yyval;
TAO_YYSTYPE tao_yylval;
#define tao_yystacksize TAO_YYSTACKSIZE
short tao_yyss[TAO_YYSTACKSIZE];
TAO_YYSTYPE tao_yyvs[TAO_YYSTACKSIZE];
#line 2290 "fe/idl.tao_yy"
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
#line 815 "y.tab.cpp"
#define TAO_YYABORT goto tao_yyabort
#define TAO_YYACCEPT goto tao_yyaccept
#define TAO_YYERROR goto tao_yyerrlab
#ifdef TAO_YYDEBUG
#ifndef TAO_YYDEBUG_LEXER_TEXT /* pointer to the text isolated by the lexer*/
#define TAO_YYDEBUG_LEXER_TEXT "TAO_YYDEBUG_LEXER_TEXT not defined"
#endif
#ifndef TAO_YYDEBUG_INDENT_STRING
#define TAO_YYDEBUG_INDENT_STRING  "|       "
#endif
#ifndef TAO_YYDEBUG_REDUCE_STRING
#define TAO_YYDEBUG_REDUCE_STRING  "+-------"
#endif
#ifndef TAO_YYDEBUG_INDENT
#ifdef __cplusplus
void TAO_YYDEBUG_INDENT(int tao_yyindent)
#else
TAO_YYDEBUG_INDENT(tao_yyindent)
int tao_yyindent;
#endif
{
    while(tao_yyindent-- > 0)
        printf("%s", TAO_YYDEBUG_INDENT_STRING);
}
#endif /* TAO_YYDEBUG_INDENT */
#ifndef TAO_YYDEBUG_REDUCE
#ifdef __cplusplus
void TAO_YYDEBUG_REDUCE(int tao_yynew_state, int tao_yyrule_num, char *tao_yyrule_string, int tao_yynew_indent, int tao_yyrhs_count)
#else
TAO_YYDEBUG_REDUCE(tao_yynew_state, tao_yyrule_num, tao_yyrule_string, tao_yynew_indent, tao_yyrhs_count)
int tao_yynew_state;
int tao_yyrule_num;
char * tao_yyrule_string;
int tao_yynew_indent;
int tao_yyrhs_count;
#endif
{
    if (1 < tao_yyrhs_count)
    {  /* draw the graphics for the reduction */
        TAO_YYDEBUG_INDENT(tao_yynew_indent);
        while(1 < tao_yyrhs_count--)
            printf("%s", TAO_YYDEBUG_REDUCE_STRING);
        putchar('+'); /* left rotated L would look nice */
        putchar('\n');
        TAO_YYDEBUG_INDENT(tao_yynew_indent);
        putchar('|'); /* down arrow would look nice */
        putchar('\n');
    }
    TAO_YYDEBUG_INDENT(tao_yynew_indent);
    /* Only print the resulting token name */
    while (*tao_yyrule_string)
        putchar(*tao_yyrule_string++);
    putchar('\n');
}
#endif /* TAO_YYDEBUG_REDUCE */
#ifndef TAO_YYDEBUG_SHIFT_LEXEME
#ifdef __cplusplus
void TAO_YYDEBUG_SHIFT_LEXEME(int tao_yyold_state, int tao_yynew_state, char *tao_yytoken_string, int tao_yynew_indent)
#else
TAO_YYDEBUG_SHIFT_LEXEME(tao_yyold_state, tao_yynew_state, tao_yytoken_string, tao_yynew_indent)
int tao_yyold_state;
int tao_yynew_state;
char * tao_yytoken_string;
int tao_yynew_indent;
#endif
{
    TAO_YYDEBUG_INDENT(tao_yynew_indent);
    printf("%s <-- `%s'\n", tao_yytoken_string, TAO_YYDEBUG_LEXER_TEXT);
}
#endif /*  TAO_YYDEBUG_SHIFT_LEXEME */
#ifndef TAO_YYDEBUG_LOOK_AHEAD
#ifdef __cplusplus
void TAO_YYDEBUG_LOOK_AHEAD(int tao_yynew_state, int tao_yytoken_num, char *tao_yytoken_string, int tao_yyindent)
#else
TAO_YYDEBUG_LOOK_AHEAD(tao_yynew_state, tao_yytoken_num, tao_yytoken_string, tao_yyindent)
int tao_yynew_state;
int tao_yytoken_num;
char * tao_yytoken_string;
int tao_yyindent;
#endif
{
    TAO_YYDEBUG_INDENT(tao_yyindent);
    printf("          .... look ahead at %s   `%s'\n",
           tao_yytoken_string,
           (0 == tao_yytoken_num)? "\0": TAO_YYDEBUG_LEXER_TEXT);
}
#endif /* TAO_YYDEBUG_LOOK_AHEAD */
#ifndef TAO_YYDEBUG_DISCARD_STATE
#ifdef __cplusplus
void TAO_YYDEBUG_DISCARD_STATE(int tao_yynew_state, int tao_yyindent)
#else
TAO_YYDEBUG_DISCARD_STATE(tao_yynew_state, tao_yyindent)
int tao_yynew_state;
int tao_yyindent;
#endif
{
    if (0 < tao_yyindent)
    {  /* draw the graphics for the reduction */
        TAO_YYDEBUG_INDENT(tao_yyindent-1);
        printf("%s", TAO_YYDEBUG_REDUCE_STRING);
        putchar('+'); /* left rotated L would look nice */
        printf("  discarding state\n");
        TAO_YYDEBUG_INDENT(tao_yyindent-1);
        putchar('|'); /* down arrow would look nice */
        putchar('\n');
    }
    else
    {
        if (0 == tao_yyindent)
            printf("discarding state\n");
        else
            printf("no more states to discard: parser will abort\n");
    }
}
#endif /* TAO_YYDEBUG_DISCARD_STATE */
#ifndef TAO_YYDEBUG_DISCARD_TOKEN
#ifdef __cplusplus
void TAO_YYDEBUG_DISCARD_TOKEN(int tao_yynew_state, int tao_yytoken_num, char *tao_yytoken_string, int tao_yyindent)
#else
TAO_YYDEBUG_DISCARD_TOKEN(tao_yynew_state, tao_yytoken_num, tao_yytoken_string, tao_yyindent)
int tao_yynew_state;
int tao_yytoken_num;
char * tao_yytoken_string;
int tao_yyindent;
#endif
{
    TAO_YYDEBUG_INDENT(tao_yyindent);
    printf("discarding token %s\n", tao_yytoken_string);
}
#endif /* TAO_YYDEBUG_DISCARD_TOKEN */
#ifndef TAO_YYDEBUG_SHIFT_ERROR_LEXEME
#ifdef __cplusplus
void TAO_YYDEBUG_SHIFT_ERROR_LEXEME(int tao_yyold_state, int tao_yynew_state, int tao_yyindent)
#else
TAO_YYDEBUG_SHIFT_ERROR_LEXEME(tao_yyold_state, tao_yynew_state, tao_yyindent)
int tao_yyold_state;
int tao_yynew_state;
int tao_yyindent;
#endif
{
    TAO_YYDEBUG_INDENT(tao_yyindent);
    printf("error\n");
}
#endif /* TAO_YYDEBUG_SHIFT_ERROR_LEXEME */
#endif /* TAO_YYDEBUG */
#ifdef __cplusplus
extern "C" { extern char *ace_foo(const char *); }
#endif
int
tao_yyparse()
{
    register int tao_yym, tao_yyn, tao_yystate;
#if TAO_YYDEBUG
    register char *tao_yys;
#ifndef __cplusplus
    extern char *ace_foo();
#endif

    if (tao_yys = ACE_OS::getenv("TAO_YYDEBUG"))
    {
        tao_yyn = *tao_yys;
        if (tao_yyn >= '0' && tao_yyn <= '9')
            tao_yydebug = tao_yyn - '0';
    }
#endif

    tao_yynerrs = 0;
    tao_yyerrflag = 0;
    tao_yychar = (-1);

    tao_yyssp = tao_yyss;
    tao_yyvsp = tao_yyvs;
    *tao_yyssp = tao_yystate = 0;

tao_yyloop:
    if (tao_yyn = tao_yydefred[tao_yystate]) goto tao_yyreduce;
    if (tao_yychar < 0)
    {
        if ((tao_yychar = tao_yylex()) < 0) tao_yychar = 0;
#if TAO_YYDEBUG
        if (tao_yydebug)
        {
            tao_yys = 0;
            if (tao_yychar <= TAO_YYMAXTOKEN) tao_yys = tao_yyname[tao_yychar];
            if (!tao_yys) tao_yys = "illegal-symbol";
            if (5 < tao_yydebug)
                printf("tao_yydebug: state %d, reading %d (%s)\n", tao_yystate,
                        tao_yychar, tao_yys);
            else
                TAO_YYDEBUG_LOOK_AHEAD(tao_yystate, tao_yychar, tao_yys, tao_yyssp-tao_yyss);
        }
#endif
    }
    if ((tao_yyn = tao_yysindex[tao_yystate]) && (tao_yyn += tao_yychar) >= 0 &&
            tao_yyn <= TAO_YYTABLESIZE && tao_yycheck[tao_yyn] == tao_yychar)
    {
#if TAO_YYDEBUG
        if (tao_yydebug)
            if (5 < tao_yydebug)
                printf("tao_yydebug: state %d, shifting to state %d\n",
                        tao_yystate, tao_yytable[tao_yyn]);
            else
                TAO_YYDEBUG_SHIFT_LEXEME(tao_yystate, tao_yytable[tao_yyn], tao_yys, tao_yyssp-tao_yyss);
#endif
        if (tao_yyssp >= tao_yyss + tao_yystacksize - 1)
        {
            goto tao_yyoverflow;
        }
        *++tao_yyssp = tao_yystate = tao_yytable[tao_yyn];
        *++tao_yyvsp = tao_yylval;
        tao_yychar = (-1);
        if (tao_yyerrflag > 0)  --tao_yyerrflag;
        goto tao_yyloop;
    }
    if ((tao_yyn = tao_yyrindex[tao_yystate]) && (tao_yyn += tao_yychar) >= 0 &&
            tao_yyn <= TAO_YYTABLESIZE && tao_yycheck[tao_yyn] == tao_yychar)
    {
        tao_yyn = tao_yytable[tao_yyn];
        goto tao_yyreduce;
    }
    if (tao_yyerrflag) goto tao_yyinrecovery;
#ifdef lint
    goto tao_yynewerror;
#endif

    tao_yyerror("syntax error");
#ifdef lint
    goto tao_yyerrlab;
#endif
tao_yyerrlab:
    ++tao_yynerrs;
tao_yyinrecovery:
    if (tao_yyerrflag < 3)
    {
        tao_yyerrflag = 3;
        for (;;)
        {
            if ((tao_yyn = tao_yysindex[*tao_yyssp]) && (tao_yyn += TAO_YYERRCODE) >= 0 &&
                    tao_yyn <= TAO_YYTABLESIZE && tao_yycheck[tao_yyn] == TAO_YYERRCODE)
            {
#if TAO_YYDEBUG
                if (tao_yydebug)
                    if (5 < tao_yydebug)
                        printf("tao_yydebug: state %d, error recovery shifting\
 to state %d\n", *tao_yyssp, tao_yytable[tao_yyn]);
                    else
                        TAO_YYDEBUG_SHIFT_ERROR_LEXEME(*tao_yyssp, tao_yytable[tao_yyn], tao_yyssp-tao_yyss);
#endif
                if (tao_yyssp >= tao_yyss + tao_yystacksize - 1)
                {
                    goto tao_yyoverflow;
                }
                *++tao_yyssp = tao_yystate = tao_yytable[tao_yyn];
                *++tao_yyvsp = tao_yylval;
                goto tao_yyloop;
            }
            else
            {
#if TAO_YYDEBUG
                if (tao_yydebug)
                    if (5 < tao_yydebug)
                        printf("tao_yydebug: error recovery discarding state %d\
",
                            *tao_yyssp);
                    else
                        TAO_YYDEBUG_DISCARD_STATE(*tao_yyssp, tao_yyssp-tao_yyss-1);
#endif
                if (tao_yyssp <= tao_yyss) goto tao_yyabort;
                --tao_yyssp;
                --tao_yyvsp;
            }
        }
    }
    else
    {
        if (tao_yychar == 0) goto tao_yyabort;
#if TAO_YYDEBUG
        if (tao_yydebug)
        {
            tao_yys = 0;
            if (tao_yychar <= TAO_YYMAXTOKEN) tao_yys = tao_yyname[tao_yychar];
            if (!tao_yys) tao_yys = "illegal-symbol";
            if (5 < tao_yydebug)
                printf("tao_yydebug: state %d, error recovery discards token %d (%s)\n",
                    tao_yystate, tao_yychar, tao_yys);
            else 
                TAO_YYDEBUG_DISCARD_TOKEN(tao_yystate, tao_yychar, tao_yys, tao_yyssp-tao_yyss);
        }
#endif
        tao_yychar = (-1);
        goto tao_yyloop;
    }
tao_yyreduce:
    tao_yym = tao_yylen[tao_yyn];
    tao_yyval = tao_yyvsp[1-tao_yym];
#if TAO_YYDEBUG
    if (tao_yydebug)
        if (5 < tao_yydebug)
            printf("tao_yydebug: state %d, reducing by rule %d (%s)\n",
                    tao_yystate, tao_yyn, tao_yyrule[tao_yyn]);
        else
            TAO_YYDEBUG_REDUCE(tao_yystate, tao_yyn, tao_yyrule[tao_yyn], tao_yyssp-tao_yyss-tao_yym, tao_yym);
#endif
    switch (tao_yyn)
    {
case 4:
#line 240 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        }
break;
case 5:
#line 244 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 6:
#line 248 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        }
break;
case 7:
#line 252 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 8:
#line 256 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        }
break;
case 9:
#line 260 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 10:
#line 264 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceDeclSeen);
        }
break;
case 11:
#line 268 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 12:
#line 272 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ModuleDeclSeen);
        }
break;
case 13:
#line 276 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 14:
#line 280 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 15:
#line 284 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 16:
#line 291 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSeen);
	  }
break;
case 17:
#line 295 "fe/idl.tao_yy"
{
	    UTL_ScopedName 	*n =
		new UTL_ScopedName(new Identifier(tao_yyvsp[0].strval, 1, 0, I_FALSE), NULL);
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
#line 316 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSqSeen);
	  }
break;
case 19:
#line 320 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleBodySeen);
	  }
break;
case 20:
#line 324 "fe/idl.tao_yy"
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
	  }
break;
case 23:
#line 346 "fe/idl.tao_yy"
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
	  if (s != NULL && tao_yyvsp[0].ihval != NULL) {
	    i = idl_global->gen()->create_interface(tao_yyvsp[0].ihval->interface_name(),
						    tao_yyvsp[0].ihval->inherits(),
						    tao_yyvsp[0].ihval->n_inherits(),
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
		    fd->set_inherits(tao_yyvsp[0].ihval->inherits());
		    fd->set_n_inherits(tao_yyvsp[0].ihval->n_inherits());
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
#line 430 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSqSeen);
	}
break;
case 25:
#line 434 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceBodySeen);
	}
break;
case 26:
#line 438 "fe/idl.tao_yy"
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
	}
break;
case 27:
#line 455 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSeen);
	 }
break;
case 28:
#line 459 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceIDSeen);
	   tao_yyval.idval = tao_yyvsp[0].idval;
	 }
break;
case 29:
#line 467 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InheritSpecSeen);
	  /*
	   * Create an AST representation of the information in the header
	   * part of an interface - this representation contains a computed
	   * list of all interfaces which this interface inherits from,
	   * recursively
	   */
	  tao_yyval.ihval = new FE_InterfaceHeader(new UTL_ScopedName(tao_yyvsp[-1].idval, NULL), tao_yyvsp[0].nlval);
	}
break;
case 30:
#line 481 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InheritColonSeen);
        }
break;
case 31:
#line 485 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = tao_yyvsp[0].nlval;
	}
break;
case 32:
#line 489 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = NULL;
	}
break;
case 35:
#line 501 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        }
break;
case 36:
#line 505 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 37:
#line 509 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        }
break;
case 38:
#line 513 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 39:
#line 517 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        }
break;
case 40:
#line 521 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 41:
#line 525 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrDeclSeen);
        }
break;
case 42:
#line 529 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 43:
#line 533 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpDeclSeen);
        }
break;
case 44:
#line 537 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 45:
#line 541 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 46:
#line 545 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 47:
#line 553 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = new UTL_NameList(tao_yyvsp[-1].idlist, tao_yyvsp[0].nlval);
	}
break;
case 48:
#line 561 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SNListCommaSeen);
        }
break;
case 49:
#line 565 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopedNameSeen);

	  if (tao_yyvsp[-3].nlval == NULL)
	    tao_yyval.nlval = new UTL_NameList(tao_yyvsp[0].idlist, NULL);
	  else {
	    tao_yyvsp[-3].nlval->nconc(new UTL_NameList(tao_yyvsp[0].idlist, NULL));
	    tao_yyval.nlval = tao_yyvsp[-3].nlval;
	  }
	}
break;
case 50:
#line 576 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = NULL;
	}
break;
case 51:
#line 583 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  tao_yyval.idlist = new UTL_IdList(tao_yyvsp[0].idval, NULL);
	}
break;
case 52:
#line 589 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        }
break;
case 53:
#line 593 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  tao_yyval.idlist = new UTL_IdList(new Identifier(tao_yyvsp[-2].strval, 1, 0, I_FALSE),
			      new UTL_IdList(tao_yyvsp[0].idval, NULL));
	}
break;
case 54:
#line 601 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        }
break;
case 55:
#line 605 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  tao_yyvsp[-3].idlist->nconc(new UTL_IdList(tao_yyvsp[0].idval, NULL));
	  tao_yyval.idlist = tao_yyvsp[-3].idlist;
	}
break;
case 56:
#line 614 "fe/idl.tao_yy"
{
            tao_yyval.idval = new Identifier(tao_yyvsp[0].strval, 1, 0, I_FALSE);
        }
break;
case 57:
#line 621 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(tao_yyvsp[0].idval, NULL);
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
#line 641 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstSeen);
        }
break;
case 59:
#line 645 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstTypeSeen);
        }
break;
case 60:
#line 649 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstIDSeen);
        }
break;
case 61:
#line 653 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstAssignSeen);
        }
break;
case 62:
#line 657 "fe/idl.tao_yy"
{
	  UTL_ScopedName	*n = new UTL_ScopedName(tao_yyvsp[-4].idval, NULL);
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
	  if (tao_yyvsp[0].exval != NULL && s != NULL) {
	    if (tao_yyvsp[0].exval->coerce(tao_yyvsp[-6].etval) == NULL)
	      idl_global->err()->coercion_error(tao_yyvsp[0].exval, tao_yyvsp[-6].etval);
	    else {
	      c =
		idl_global->gen()->create_constant(tao_yyvsp[-6].etval, tao_yyvsp[0].exval, n, p);
	      (void) s->fe_add_constant(c);
	    }
	  }
	}
break;
case 68:
#line 690 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_string;
	}
break;
case 69:
#line 694 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_wstring;
        }
break;
case 70:
#line 698 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Decl		*d = NULL;
	  AST_PredefinedType	*c = NULL;
	  AST_Typedef		*t = NULL;

	  /*
	   * If the constant's type is a scoped name, it must resolve
	   * to a scalar constant type
	   */
	  if (s != NULL && (d = s->lookup_by_name(tao_yyvsp[0].idlist, I_TRUE)) != NULL) {
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
	}
break;
case 74:
#line 740 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_or, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 76:
#line 748 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_xor, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 78:
#line 756 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_and, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 80:
#line 764 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_right,tao_yyvsp[-2].exval,tao_yyvsp[0].exval);
	}
break;
case 81:
#line 768 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_left,tao_yyvsp[-2].exval,tao_yyvsp[0].exval);
	}
break;
case 83:
#line 776 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_add, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 84:
#line 780 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_minus,tao_yyvsp[-2].exval,tao_yyvsp[0].exval);
	}
break;
case 86:
#line 788 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_mul, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 87:
#line 792 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_div, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 88:
#line 796 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_mod, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 90:
#line 804 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_u_plus,
					      tao_yyvsp[0].exval,
					      NULL);
	}
break;
case 91:
#line 810 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_u_minus,
					      tao_yyvsp[0].exval,
					      NULL);
	}
break;
case 92:
#line 816 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_bit_neg,
					      tao_yyvsp[0].exval,
					      NULL);
	}
break;
case 93:
#line 825 "fe/idl.tao_yy"
{
	  /*
	   * An expression which is a scoped name is not resolved now,
	   * but only when it is evaluated (such as when it is assigned
	   * as a constant value)
	   */
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].idlist);
	}
break;
case 95:
#line 835 "fe/idl.tao_yy"
{
	  tao_yyval.exval = tao_yyvsp[-1].exval;
	}
break;
case 96:
#line 842 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].ival);
	}
break;
case 97:
#line 846 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].sval);
	}
break;
case 98:
#line 850 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].cval);
	}
break;
case 99:
#line 854 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].dval);
	}
break;
case 100:
#line 858 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr((idl_bool) I_TRUE,
					    AST_Expression::EV_bool);
	}
break;
case 101:
#line 863 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr((idl_bool) I_FALSE,
					    AST_Expression::EV_bool);
	}
break;
case 102:
#line 871 "fe/idl.tao_yy"
{
	    tao_yyvsp[0].exval->evaluate(AST_Expression::EK_const);
	    tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].exval, AST_Expression::EV_ulong);
	}
break;
case 103:
#line 879 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_TypedefSeen);
	  }
break;
case 104:
#line 882 "fe/idl.tao_yy"
{tao_yyval.ival = 0;}
break;
case 105:
#line 883 "fe/idl.tao_yy"
{ tao_yyval.ival = 0;}
break;
case 106:
#line 884 "fe/idl.tao_yy"
{ tao_yyval.ival = 0;}
break;
case 107:
#line 885 "fe/idl.tao_yy"
{ tao_yyval.ival = 0;}
break;
case 108:
#line 887 "fe/idl.tao_yy"
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
	    node = idl_global->gen()->create_native (tao_yyvsp[0].deval->name (), p);
	    /*
	     * Add it to its defining scope
	     */
	    (void) s->fe_add_native (node);
	  }
	}
break;
case 109:
#line 912 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeSpecSeen);
        }
break;
case 110:
#line 916 "fe/idl.tao_yy"
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
	  if (s != NULL && tao_yyvsp[-2].dcval != NULL && tao_yyvsp[0].dlval != NULL) {
	    l = new UTL_DecllistActiveIterator(tao_yyvsp[0].dlval);
	    for (;!(l->is_done()); l->next()) {
	      d = l->item();
	      if (d == NULL)
		      continue;
              AST_Type * tp = d->compose(tao_yyvsp[-2].dcval);
              if (tp == NULL)
		     continue;
	      t = idl_global->gen()->create_typedef(tp, d->name(), p);
	      (void) s->fe_add_typedef(t);
	    }
	    delete l;
	  }
	}
break;
case 113:
#line 955 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 115:
#line 960 "fe/idl.tao_yy"
{
	  UTL_Scope	*s = idl_global->scopes()->top_non_null();
	  AST_Decl	*d = NULL;

	  if (s != NULL)
	    d = s->lookup_by_name(tao_yyvsp[0].idlist, I_TRUE);
	  if (d == NULL)
	    idl_global->err()->lookup_error(tao_yyvsp[0].idlist);
	  tao_yyval.dcval = d;
	}
break;
case 128:
#line 995 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = new UTL_DeclList(tao_yyvsp[-1].deval, tao_yyvsp[0].dlval);
	}
break;
case 129:
#line 1003 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        }
break;
case 130:
#line 1007 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsDeclSeen);

	  if (tao_yyvsp[-3].dlval == NULL)
	    tao_yyval.dlval = new UTL_DeclList(tao_yyvsp[0].deval, NULL);
	  else {
	    tao_yyvsp[-3].dlval->nconc(new UTL_DeclList(tao_yyvsp[0].deval, NULL));
	    tao_yyval.dlval = tao_yyvsp[-3].dlval;
	  }
	}
break;
case 131:
#line 1018 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = NULL;
	}
break;
case 134:
#line 1030 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = new UTL_DeclList(tao_yyvsp[-1].deval, tao_yyvsp[0].dlval);
	}
break;
case 135:
#line 1038 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        }
break;
case 136:
#line 1042 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsDeclSeen);

	  if (tao_yyvsp[-3].dlval == NULL)
	    tao_yyval.dlval = new UTL_DeclList(tao_yyvsp[0].deval, NULL);
	  else {
	    tao_yyvsp[-3].dlval->nconc(new UTL_DeclList(tao_yyvsp[0].deval, NULL));
	    tao_yyval.dlval = tao_yyvsp[-3].dlval;
	  }
	}
break;
case 137:
#line 1053 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = NULL;
	}
break;
case 138:
#line 1060 "fe/idl.tao_yy"
{
	  tao_yyval.deval = new FE_Declarator(new UTL_ScopedName(tao_yyvsp[0].idval, NULL),
				 FE_Declarator::FD_simple, NULL);
	}
break;
case 139:
#line 1068 "fe/idl.tao_yy"
{
	  tao_yyval.deval = new FE_Declarator(new UTL_ScopedName(tao_yyvsp[0].dcval->local_name(), NULL),
				 FE_Declarator::FD_complex,
				 tao_yyvsp[0].dcval);
	}
break;
case 142:
#line 1082 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_long;
	}
break;
case 143:
#line 1086 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_longlong;
        }
break;
case 144:
#line 1090 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_short;
	}
break;
case 145:
#line 1097 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_ulong;
	}
break;
case 146:
#line 1101 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_ulonglong;
        }
break;
case 147:
#line 1105 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_ushort;
	}
break;
case 148:
#line 1112 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_double;
	}
break;
case 149:
#line 1116 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_float;
	}
break;
case 150:
#line 1120 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_longdouble;
        }
break;
case 151:
#line 1127 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_char;
	}
break;
case 152:
#line 1131 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_wchar;
        }
break;
case 153:
#line 1138 "fe/idl.tao_yy"
{
          tao_yyval.etval = AST_Expression::EV_octet;
	}
break;
case 154:
#line 1145 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_bool;
        }
break;
case 155:
#line 1152 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_any;
	}
break;
case 156:
#line 1159 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructSeen);
        }
break;
case 157:
#line 1163 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(tao_yyvsp[0].idval, NULL);
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
case 158:
#line 1187 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructSqSeen);
        }
break;
case 159:
#line 1191 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructBodySeen);
        }
break;
case 160:
#line 1195 "fe/idl.tao_yy"
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
	}
break;
case 164:
#line 1220 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberTypeSeen);
        }
break;
case 165:
#line 1224 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberDeclsSeen);
        }
break;
case 166:
#line 1228 "fe/idl.tao_yy"
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
	  if (tao_yyvsp[-4].dcval != NULL && AST_illegal_recursive_type(tao_yyvsp[-4].dcval))
	    idl_global->err()->error1(UTL_Error::EIDL_RECURSIVE_TYPE, tao_yyvsp[-4].dcval);
	  /*
	   * Create a node representing a struct or exception member
	   * Add it to the enclosing scope
	   */
	  else if (s != NULL && tao_yyvsp[-4].dcval != NULL && tao_yyvsp[-2].dlval != NULL) {
	    l = new UTL_DecllistActiveIterator(tao_yyvsp[-2].dlval);
	    for (;!(l->is_done()); l->next()) {
	      d = l->item();
	      if (d == NULL)
		continue;
 	      AST_Type *tp = d->compose(tao_yyvsp[-4].dcval);
	      if (tp == NULL)
		continue;
	      f = idl_global->gen()->create_field(tp, d->name(), p);
	      (void) s->fe_add_field(f);
	    }
	    delete l;
	  }
	}
break;
case 167:
#line 1261 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 168:
#line 1265 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 169:
#line 1273 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionSeen);
        }
break;
case 170:
#line 1277 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionIDSeen);
        }
break;
case 171:
#line 1281 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchSeen);
        }
break;
case 172:
#line 1285 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchOpenParSeen);
        }
break;
case 173:
#line 1289 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchTypeSeen);
        }
break;
case 174:
#line 1293 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(tao_yyvsp[-8].idval, NULL);
	  AST_Union		*u = NULL;
	  AST_Decl		*v = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

          ACE_UNUSED_ARG (v);

	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchCloseParSeen);
	  /*
	   * Create a node representing a union. Add it to its enclosing
	   * scope
	   */
	  if (tao_yyvsp[-2].dcval != NULL && s != NULL) {
 	    AST_ConcreteType    *tp = AST_ConcreteType::narrow_from_decl(tao_yyvsp[-2].dcval);
            if (tp == NULL) {
              idl_global->err()->not_a_type(tao_yyvsp[-2].dcval);
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
case 175:
#line 1322 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionSqSeen);
        }
break;
case 176:
#line 1326 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionBodySeen);
        }
break;
case 177:
#line 1330 "fe/idl.tao_yy"
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
	}
break;
case 178:
#line 1348 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 179:
#line 1352 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 180:
#line 1356 "fe/idl.tao_yy"
{
          /* octets are not allowed*/
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
	}
break;
case 181:
#line 1361 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 183:
#line 1366 "fe/idl.tao_yy"
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
	  if (s != NULL && (d = s->lookup_by_name(tao_yyvsp[0].idlist, I_TRUE)) != NULL) {
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
                    /* octets are not allowed*/
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
	    idl_global->err()->lookup_error(tao_yyvsp[0].idlist);
	}
break;
case 187:
#line 1445 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionLabelSeen);
        }
break;
case 188:
#line 1449 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemSeen);
        }
break;
case 189:
#line 1453 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_LabellistActiveIterator *l = NULL;
	  AST_UnionLabel	*d = NULL;
	  AST_UnionBranch	*b = NULL;
	  AST_Field		*f = tao_yyvsp[-2].ffval;

	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemCompleted);
	  /*
	   * Create several nodes representing branches of a union.
	   * Add them to the enclosing scope (the union scope)
	   */
	  if (s != NULL && tao_yyvsp[-4].llval != NULL && tao_yyvsp[-2].ffval != NULL) {
	    l = new UTL_LabellistActiveIterator(tao_yyvsp[-4].llval);
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
case 190:
#line 1481 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 191:
#line 1486 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 192:
#line 1494 "fe/idl.tao_yy"
{
	  tao_yyval.llval = new UTL_LabelList(tao_yyvsp[-1].ulval, tao_yyvsp[0].llval);
	}
break;
case 193:
#line 1501 "fe/idl.tao_yy"
{
	  if (tao_yyvsp[-1].llval == NULL)
	    tao_yyval.llval = new UTL_LabelList(tao_yyvsp[0].ulval, NULL);
	  else {
	    tao_yyvsp[-1].llval->nconc(new UTL_LabelList(tao_yyvsp[0].ulval, NULL));
	    tao_yyval.llval = tao_yyvsp[-1].llval;
	  }
	}
break;
case 194:
#line 1510 "fe/idl.tao_yy"
{
	  tao_yyval.llval = NULL;
	}
break;
case 195:
#line 1517 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DefaultSeen);
        }
break;
case 196:
#line 1521 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

	  tao_yyval.ulval = idl_global->gen()->
	            create_union_label(AST_UnionLabel::UL_default,
				       NULL);
	}
break;
case 197:
#line 1529 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_CaseSeen);
        }
break;
case 198:
#line 1533 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelExprSeen);
        }
break;
case 199:
#line 1537 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

	  tao_yyval.ulval = idl_global->gen()->create_union_label(AST_UnionLabel::UL_label,
						     tao_yyvsp[-2].exval);
	}
break;
case 200:
#line 1547 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemTypeSeen);
        }
break;
case 201:
#line 1551 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemDeclSeen);
	  /*
	   * Check for illegal recursive use of type
	   */
	  if (tao_yyvsp[-2].dcval != NULL && AST_illegal_recursive_type(tao_yyvsp[-2].dcval))
	    idl_global->err()->error1(UTL_Error::EIDL_RECURSIVE_TYPE, tao_yyvsp[-2].dcval);
	  /*
	   * Create a field in a union branch
	   */
	  else if (tao_yyvsp[-2].dcval == NULL || tao_yyvsp[0].deval == NULL)
	    tao_yyval.ffval = NULL;
	  else {
	    AST_Type *tp = tao_yyvsp[0].deval->compose(tao_yyvsp[-2].dcval);
	    if (tp == NULL)
	      tao_yyval.ffval = NULL;
 	    else
	      tao_yyval.ffval = idl_global->gen()->create_field(tp,
			        		   tao_yyvsp[0].deval->name(),
			        		   idl_global->pragmas());
	  }
	}
break;
case 202:
#line 1577 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumSeen);
        }
break;
case 203:
#line 1581 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(tao_yyvsp[0].idval, NULL);
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
case 204:
#line 1608 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumSqSeen);
        }
break;
case 205:
#line 1612 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumBodySeen);
        }
break;
case 206:
#line 1616 "fe/idl.tao_yy"
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
	}
break;
case 208:
#line 1635 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumCommaSeen);
        }
break;
case 211:
#line 1644 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n =
		new UTL_ScopedName(new Identifier(tao_yyvsp[0].strval, 1, 0, I_FALSE), NULL);
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
case 212:
#line 1668 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceCommaSeen);
        }
break;
case 213:
#line 1672 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceExprSeen);
        }
break;
case 214:
#line 1676 "fe/idl.tao_yy"
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
	  if (tao_yyvsp[-2].exval == NULL || tao_yyvsp[-2].exval->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error(tao_yyvsp[-2].exval, AST_Expression::EV_ulong);
	    tao_yyval.dcval = NULL;
	  } else if (tao_yyvsp[-5].dcval == NULL) {
	    tao_yyval.dcval = NULL;
	  } else {
 	    AST_Type *tp = AST_Type::narrow_from_decl(tao_yyvsp[-5].dcval);
	    if (tp == NULL)
	      tao_yyval.dcval = NULL;
	    else {
	      tao_yyval.dcval = idl_global->gen()->create_sequence(tao_yyvsp[-2].exval, tp);
	      /*
	       * Add this AST_Sequence to the types defined in the global scope
	       */
	      (void) idl_global->root()
		        ->fe_add_sequence(AST_Sequence::narrow_from_decl(tao_yyval.dcval));
 	    }
	  }
	}
break;
case 215:
#line 1707 "fe/idl.tao_yy"
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
	  if (tao_yyvsp[-1].dcval == NULL)
	    tao_yyval.dcval = NULL;
	  else {
	    AST_Type *tp = AST_Type::narrow_from_decl(tao_yyvsp[-1].dcval);
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
	}
break;
case 216:
#line 1740 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSeen);
	  /*
	   * Push a sequence marker on scopes stack
	   */
	  idl_global->scopes()->push(NULL);
	}
break;
case 217:
#line 1748 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSqSeen);
        }
break;
case 218:
#line 1752 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceTypeSeen);
	  tao_yyval.dcval = tao_yyvsp[0].dcval;
        }
break;
case 219:
#line 1761 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        }
break;
case 220:
#line 1765 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        }
break;
case 221:
#line 1769 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringQsSeen);
	  /*
	   * Create a node representing a string
	   */
	  if (tao_yyvsp[-2].exval == NULL || tao_yyvsp[-2].exval->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error(tao_yyvsp[-2].exval, AST_Expression::EV_ulong);
	    tao_yyval.dcval = NULL;
	  } else {
	    tao_yyval.dcval = idl_global->gen()->create_string(tao_yyvsp[-2].exval);
	    /*
	     * Add this AST_String to the types defined in the global scope
	     */
	    (void) idl_global->root()
		      ->fe_add_string(AST_String::narrow_from_decl(tao_yyval.dcval));
	  }
	}
break;
case 222:
#line 1787 "fe/idl.tao_yy"
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
	}
break;
case 223:
#line 1805 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        }
break;
case 224:
#line 1813 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        }
break;
case 225:
#line 1817 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        }
break;
case 226:
#line 1821 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringQsSeen);
	  /*
	   * Create a node representing a string
	   */
	  if (tao_yyvsp[-2].exval == NULL || tao_yyvsp[-2].exval->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error(tao_yyvsp[-2].exval, AST_Expression::EV_ulong);
	    tao_yyval.dcval = NULL;
	  } else {
	    tao_yyval.dcval = idl_global->gen()->create_wstring(tao_yyvsp[-2].exval);
	    /*
	     * Add this AST_String to the types defined in the global scope
	     */
	    (void) idl_global->root()
		      ->fe_add_string(AST_String::narrow_from_decl(tao_yyval.dcval));
	  }
	}
break;
case 227:
#line 1839 "fe/idl.tao_yy"
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
	}
break;
case 228:
#line 1857 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        }
break;
case 229:
#line 1864 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ArrayIDSeen);
        }
break;
case 230:
#line 1868 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ArrayCompleted);
	  /*
	   * Create a node representing an array
	   */
	  if (tao_yyvsp[0].elval != NULL) {
	     tao_yyval.dcval = idl_global->gen()->create_array(new UTL_ScopedName(tao_yyvsp[-2].idval, NULL),
						  tao_yyvsp[0].elval->length(), tao_yyvsp[0].elval);
	  }
	}
break;
case 231:
#line 1882 "fe/idl.tao_yy"
{
	  tao_yyval.elval = new UTL_ExprList(tao_yyvsp[-1].exval, tao_yyvsp[0].elval);
	}
break;
case 232:
#line 1889 "fe/idl.tao_yy"
{
	  if (tao_yyvsp[-1].elval == NULL)
	    tao_yyval.elval = new UTL_ExprList(tao_yyvsp[0].exval, NULL);
	  else {
	    tao_yyvsp[-1].elval->nconc(new UTL_ExprList(tao_yyvsp[0].exval, NULL));
	    tao_yyval.elval = tao_yyvsp[-1].elval;
	  }
	}
break;
case 233:
#line 1898 "fe/idl.tao_yy"
{
	  tao_yyval.elval = NULL;
	}
break;
case 234:
#line 1905 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimSqSeen);
        }
break;
case 235:
#line 1909 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimExprSeen);
        }
break;
case 236:
#line 1913 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimQsSeen);
	  /*
	   * Array dimensions are expressions which must be coerced to
	   * positive integers
	   */
	  if (tao_yyvsp[-2].exval == NULL || tao_yyvsp[-2].exval->coerce(AST_Expression::EV_ulong) == NULL) {
	    idl_global->err()->coercion_error(tao_yyvsp[-2].exval, AST_Expression::EV_ulong);
	    tao_yyval.exval = NULL;
	  } else
	    tao_yyval.exval = tao_yyvsp[-2].exval;
	}
break;
case 237:
#line 1930 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrSeen);
        }
break;
case 238:
#line 1934 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrTypeSeen);
        }
break;
case 239:
#line 1938 "fe/idl.tao_yy"
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
	  if (s != NULL && tao_yyvsp[-2].dcval != NULL && tao_yyvsp[0].dlval != NULL) {
	    l = new UTL_DecllistActiveIterator(tao_yyvsp[0].dlval);
	    for (;!(l->is_done()); l->next()) {
	      d = l->item();
	      if (d == NULL)
		continue;
	      AST_Type *tp = d->compose(tao_yyvsp[-2].dcval);
 	      if (tp == NULL)
	 	continue;
	      a = idl_global->gen()->create_attribute(tao_yyvsp[-5].bval, tp, d->name(), p);
	      /*
	       * Add one attribute to the enclosing scope
	       */
	      (void) s->fe_add_attribute(a);
	    }
	    delete l;
	  }
	}
break;
case 240:
#line 1972 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrROSeen);
	  tao_yyval.bval = I_TRUE;
	}
break;
case 241:
#line 1977 "fe/idl.tao_yy"
{
	  tao_yyval.bval = I_FALSE;
	}
break;
case 242:
#line 1984 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSeen);
	}
break;
case 243:
#line 1988 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(tao_yyvsp[0].idval, NULL);
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
case 244:
#line 2012 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSqSeen);
        }
break;
case 245:
#line 2016 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptBodySeen);
        }
break;
case 246:
#line 2020 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptQsSeen);
	  /*
	   * Done with this exception. Pop its scope from the scope stack
	   */
	  idl_global->scopes()->pop();
	}
break;
case 247:
#line 2032 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpTypeSeen);
        }
break;
case 248:
#line 2036 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n =
		new UTL_ScopedName(new Identifier(tao_yyvsp[0].strval, 1, 0, I_FALSE), NULL);
	  AST_Operation		*o = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_OpIDSeen);
	  /*
	   * Create a node representing an operation on an interface
	   * and add it to its enclosing scope
	   */
	  if (s != NULL && tao_yyvsp[-2].dcval != NULL) {
	    AST_Type *tp = AST_Type::narrow_from_decl(tao_yyvsp[-2].dcval);
            if (tp == NULL) {
              idl_global->err()->not_a_type(tao_yyvsp[-2].dcval);
            } else if (tp->node_type() == AST_Decl::NT_except) {
              idl_global->err()->not_a_type(tao_yyvsp[-2].dcval);
            } else {
	      o = idl_global->gen()->create_operation(tp, tao_yyvsp[-3].ofval, n, p);
	      (void) s->fe_add_operation(o);
	    }
	  }
	  /*
	   * Push the operation scope onto the scopes stack
	   */
	  idl_global->scopes()->push(o);
	}
break;
case 249:
#line 2065 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParsCompleted);
        }
break;
case 250:
#line 2069 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseCompleted);
        }
break;
case 251:
#line 2073 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Operation		*o = NULL;

	  idl_global->set_parse_state(IDL_GlobalData::PS_OpCompleted);
	  /*
	   * Add exceptions and context to the operation
	   */
	  if (s != NULL && s->scope_node_type() == AST_Decl::NT_op) {
	    o = AST_Operation::narrow_from_scope(s);

	    if (tao_yyvsp[-2].nlval != NULL && o != NULL)
	      (void) o->fe_add_exceptions(tao_yyvsp[-2].nlval);
	    if (tao_yyvsp[0].slval != NULL)
	      (void) o->fe_add_context(tao_yyvsp[0].slval);
	  }
	  /*
	   * Done with this operation. Pop its scope from the scopes stack
	   */
	  idl_global->scopes()->pop();
	}
break;
case 252:
#line 2098 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
	  tao_yyval.ofval = AST_Operation::OP_oneway;
	}
break;
case 253:
#line 2103 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
	  tao_yyval.ofval = AST_Operation::OP_idempotent;
	}
break;
case 254:
#line 2108 "fe/idl.tao_yy"
{
	  tao_yyval.ofval = AST_Operation::OP_noflags;
	}
break;
case 256:
#line 2116 "fe/idl.tao_yy"
{
	  tao_yyval.dcval =
	    idl_global->scopes()->bottom()
	       ->lookup_primitive_type(AST_Expression::EV_void);
	}
break;
case 257:
#line 2125 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        }
break;
case 258:
#line 2129 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        }
break;
case 259:
#line 2133 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        }
break;
case 260:
#line 2138 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        }
break;
case 262:
#line 2148 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParCommaSeen);
        }
break;
case 265:
#line 2157 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParDirSeen);
        }
break;
case 266:
#line 2161 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParTypeSeen);
        }
break;
case 267:
#line 2165 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  AST_Argument		*a = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParDeclSeen);
	  /*
	   * Create a node representing an argument to an operation
	   * Add it to the enclosing scope (the operation scope)
	   */
	  if (tao_yyvsp[-2].dcval != NULL && tao_yyvsp[0].deval != NULL && s != NULL) {
	    AST_Type *tp = tao_yyvsp[0].deval->compose(tao_yyvsp[-2].dcval);
	    if (tp != NULL) {
	      a = idl_global->gen()->create_argument(tao_yyvsp[-4].dival, tp, tao_yyvsp[0].deval->name(), p);
	      (void) s->fe_add_argument(a);
	    }
	  }
	}
break;
case 268:
#line 2187 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 270:
#line 2192 "fe/idl.tao_yy"
{
	  UTL_Scope	*s = idl_global->scopes()->top_non_null();
	  AST_Decl	*d = NULL;

	  if (s != NULL)
	    d = s->lookup_by_name(tao_yyvsp[0].idlist, I_TRUE);
	  if (d == NULL)
	    idl_global->err()->lookup_error(tao_yyvsp[0].idlist);
	  tao_yyval.dcval = d;
	}
break;
case 271:
#line 2206 "fe/idl.tao_yy"
{
	  tao_yyval.dival = AST_Argument::dir_IN;
	}
break;
case 272:
#line 2210 "fe/idl.tao_yy"
{
	  tao_yyval.dival = AST_Argument::dir_OUT;
	}
break;
case 273:
#line 2214 "fe/idl.tao_yy"
{
	  tao_yyval.dival = AST_Argument::dir_INOUT;
	}
break;
case 274:
#line 2221 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSeen);
        }
break;
case 275:
#line 2225 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSqSeen);
        }
break;
case 276:
#line 2230 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseQsSeen);
	  tao_yyval.nlval = tao_yyvsp[-1].nlval;
	}
break;
case 277:
#line 2235 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = NULL;
	}
break;
case 278:
#line 2242 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSeen);
        }
break;
case 279:
#line 2246 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSqSeen);
        }
break;
case 280:
#line 2251 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextQsSeen);
	  tao_yyval.slval = tao_yyvsp[-1].slval;
	}
break;
case 281:
#line 2256 "fe/idl.tao_yy"
{
	  tao_yyval.slval = NULL;
	}
break;
case 282:
#line 2263 "fe/idl.tao_yy"
{
	  tao_yyval.slval = new UTL_StrList(tao_yyvsp[-1].sval, tao_yyvsp[0].slval);
	}
break;
case 283:
#line 2271 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextCommaSeen);
        }
break;
case 284:
#line 2275 "fe/idl.tao_yy"
{
	  if (tao_yyvsp[-3].slval == NULL)
	    tao_yyval.slval = new UTL_StrList(tao_yyvsp[0].sval, NULL);
	  else {
	    tao_yyvsp[-3].slval->nconc(new UTL_StrList(tao_yyvsp[0].sval, NULL));
	    tao_yyval.slval = tao_yyvsp[-3].slval;
	  }
	}
break;
case 285:
#line 2284 "fe/idl.tao_yy"
{
	  tao_yyval.slval = NULL;
	}
break;
#line 3299 "y.tab.cpp"
    }
    tao_yyssp -= tao_yym;
    tao_yystate = *tao_yyssp;
    tao_yyvsp -= tao_yym;
    tao_yym = tao_yylhs[tao_yyn];
    if (tao_yystate == 0 && tao_yym == 0)
    {
#ifdef TAO_YYDEBUG
        if (5 < tao_yydebug)
            printf("tao_yydebug: after reduction, shifting from state 0 to\
 state %d\n", TAO_YYFINAL);
#endif
        tao_yystate = TAO_YYFINAL;
        *++tao_yyssp = TAO_YYFINAL;
        *++tao_yyvsp = tao_yyval;
        if (tao_yychar < 0)
        {
            if ((tao_yychar = tao_yylex()) < 0) tao_yychar = 0;
#if TAO_YYDEBUG
            if (tao_yydebug)
            {
                tao_yys = 0;
                if (tao_yychar <= TAO_YYMAXTOKEN) tao_yys = tao_yyname[tao_yychar];
                if (!tao_yys) tao_yys = "illegal-symbol";
                if (5 < tao_yydebug)
                    printf("tao_yydebug: state %d, reading %d (%s)\n",
                        TAO_YYFINAL, tao_yychar, tao_yys);
                else
                    TAO_YYDEBUG_LOOK_AHEAD(TAO_YYFINAL, tao_yychar, tao_yys, tao_yyssp-tao_yyss);
            }
#endif
        }
        if (tao_yychar == 0) goto tao_yyaccept;
        goto tao_yyloop;
    }
    if ((tao_yyn = tao_yygindex[tao_yym]) && (tao_yyn += tao_yystate) >= 0 &&
            tao_yyn <= TAO_YYTABLESIZE && tao_yycheck[tao_yyn] == tao_yystate)
        tao_yystate = tao_yytable[tao_yyn];
    else
        tao_yystate = tao_yydgoto[tao_yym];
#ifdef TAO_YYDEBUG
    if (5 < tao_yydebug)
        printf("tao_yydebug: after reduction, shifting from state %d \
to state %d\n", *tao_yyssp, tao_yystate);
#endif
    if (tao_yyssp >= tao_yyss + tao_yystacksize - 1)
    {
        goto tao_yyoverflow;
    }
    *++tao_yyssp = tao_yystate;
    *++tao_yyvsp = tao_yyval;
    goto tao_yyloop;
tao_yyoverflow:
    tao_yyerror("yacc stack overflow");
tao_yyabort:
    return (1);
tao_yyaccept:
    return (0);
}
