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
  AST_Decl			*dcval;		/* Decl value		*/
  UTL_StrList			*slval;		/* String list		*/
  UTL_NameList			*nlval;		/* Name list		*/
  UTL_ExprList			*elval;		/* Expression list	*/
  UTL_LabelList			*llval;		/* Label list		*/
  UTL_DeclList			*dlval;		/* Declaration list	*/
  FE_InterfaceHeader		*ihval;		/* Interface header	*/
  FE_obv_header         	*vhval;         /* Valuetype header     */
  AST_Expression		*exval;		/* Expression value	*/
  AST_UnionLabel		*ulval;		/* Union label		*/
  AST_Field			*ffval;		/* Field value		*/
  AST_Field::Visibility  	vival;         	/* N/A, pub or priv     */
  AST_Expression::ExprType 	etval;		/* Expression type	*/
  AST_Argument::Direction 	dival;		/* Argument direction	*/
  AST_Operation::Flags		ofval;		/* Operation flags	*/
  FE_Declarator			*deval;		/* Declarator value	*/
  idl_bool			bval;		/* Boolean value	*/
  long				ival;		/* Long value		*/
  unsigned long			uival;		/* Unsigned long value	*/
  double			dval;		/* Double value		*/
  float				fval;		/* Float value		*/
  char				cval;		/* Char value		*/
  ACE_CDR::WChar		wcval;		/* WChar value		*/
  UTL_String			*sval;		/* String value		*/
  char				*wsval;		/* WString value	*/
  char				*strval;	/* char * value		*/
  Identifier			*idval;		/* Identifier		*/
  UTL_IdList			*idlist;	/* Identifier list	*/
} TAO_YYSTYPE;
#line 55 "y.tab.cpp"
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
#define IDL_FIXED 271
#define IDL_ANY 272
#define IDL_STRUCT 273
#define IDL_UNION 274
#define IDL_SWITCH 275
#define IDL_ENUM 276
#define IDL_SEQUENCE 277
#define IDL_STRING 278
#define IDL_WSTRING 279
#define IDL_EXCEPTION 280
#define IDL_CASE 281
#define IDL_DEFAULT 282
#define IDL_READONLY 283
#define IDL_ATTRIBUTE 284
#define IDL_ONEWAY 285
#define IDL_IDEMPOTENT 286
#define IDL_VOID 287
#define IDL_IN 288
#define IDL_OUT 289
#define IDL_INOUT 290
#define IDL_RAISES 291
#define IDL_CONTEXT 292
#define IDL_NATIVE 293
#define IDL_LOCAL 294
#define IDL_ABSTRACT 295
#define IDL_CUSTOM 296
#define IDL_INIT 297
#define IDL_PRIVATE 298
#define IDL_PUBLIC 299
#define IDL_SUPPORTS 300
#define IDL_TRUNCATABLE 301
#define IDL_VALUETYPE 302
#define IDL_INTEGER_LITERAL 303
#define IDL_UINTEGER_LITERAL 304
#define IDL_STRING_LITERAL 305
#define IDL_CHARACTER_LITERAL 306
#define IDL_FLOATING_PT_LITERAL 307
#define IDL_TRUETOK 308
#define IDL_FALSETOK 309
#define IDL_SCOPE_DELIMITOR 310
#define IDL_LEFT_SHIFT 311
#define IDL_RIGHT_SHIFT 312
#define IDL_WCHAR_LITERAL 313
#define IDL_WSTRING_LITERAL 314
#define TAO_YYERRCODE 256
short tao_yylhs[] = {                                        -1,
    0,   72,   72,   74,   73,   76,   73,   78,   73,   80,
   73,   82,   73,   84,   73,   85,   73,   86,   87,   88,
   89,   81,   79,   79,   92,   94,   95,   90,   96,   68,
   33,   33,   33,   97,   22,   22,   83,   83,   83,   83,
   98,   98,  103,  105,  106,  102,  107,  108,  109,   99,
   34,  110,   69,   67,   67,   24,   24,  100,  100,  101,
  104,  104,  111,  111,  111,  116,  112,  117,  112,   93,
   93,  118,  113,  119,  113,  120,  113,  122,  113,  124,
  113,  125,  113,   20,  126,   21,   21,   16,  127,   16,
  128,   16,   70,   91,   91,   91,  129,  130,  131,  132,
   75,   50,   50,   50,   50,   50,   50,   50,   50,   50,
   35,   36,   37,   37,   38,   38,   39,   39,   40,   40,
   40,   41,   41,   41,   42,   42,   42,   42,   43,   43,
   43,   43,   44,   44,   44,   45,   45,   45,   45,   45,
   45,   45,   45,   45,   46,  134,   71,   71,   71,   71,
   71,  135,  133,    1,    1,    2,    2,    2,   59,   59,
   59,   59,   59,   59,   59,    4,    4,    4,    3,    3,
    3,   29,  136,   30,   30,   63,   63,   31,  137,   32,
   32,   64,   65,   51,   51,   57,   57,   57,   58,   58,
   58,   54,   54,   54,   56,   52,   52,   60,   53,   55,
  138,  139,  141,  142,    7,  140,  140,  145,  140,  144,
  144,  146,  144,  147,  143,  148,  149,  115,  150,  115,
  151,  152,  153,  154,  155,  156,  158,  159,   10,    9,
    9,    9,    9,    9,    9,  157,  161,  161,  162,  163,
  160,  164,  160,   27,   28,   28,  165,   48,  166,  167,
   48,  168,   49,  169,  170,  172,  173,    8,  171,  176,
  175,  175,  174,  177,  178,    5,    5,  179,  180,   13,
  182,  183,    6,    6,  181,  185,  186,   14,   14,  184,
  187,   11,   25,   26,   26,  188,  189,   47,  190,  191,
  121,   66,   66,  192,  193,  194,  195,   77,  196,  198,
  199,  200,  123,   62,   62,   62,   12,   12,  114,  201,
  197,  203,  197,  202,  206,  205,  205,  207,  208,  204,
   15,   15,   15,   15,   61,   61,   61,  209,  210,   23,
   23,  211,  212,   17,   17,   18,  213,   19,   19,
};
short tao_yylen[] = {                                         2,
    1,    2,    0,    0,    3,    0,    3,    0,    3,    0,
    3,    0,    3,    0,    3,    0,    3,    0,    0,    0,
    0,    9,    1,    1,    0,    0,    0,    7,    0,    3,
    2,    3,    3,    0,    3,    0,    1,    1,    1,    1,
    2,    1,    0,    0,    0,    7,    0,    0,    0,    8,
    4,    0,    3,    1,    0,    2,    0,    2,    1,    2,
    2,    0,    1,    1,    1,    0,    3,    0,    3,    2,
    0,    0,    3,    0,    3,    0,    3,    0,    3,    0,
    3,    0,    3,    2,    0,    4,    0,    1,    0,    3,
    0,    4,    1,    1,    2,    2,    0,    0,    0,    0,
    9,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    3,    1,    3,    1,    3,    1,    3,
    3,    1,    3,    3,    1,    3,    3,    3,    1,    2,
    2,    2,    1,    1,    3,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    0,    3,    1,    1,    1,
    2,    0,    3,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    2,    0,    4,    0,    1,    1,    2,    0,    4,
    0,    1,    1,    1,    1,    1,    2,    1,    2,    3,
    2,    1,    1,    2,    1,    1,    1,    1,    1,    1,
    0,    0,    0,    0,    9,    2,    0,    0,    3,    2,
    0,    0,    3,    0,    2,    0,    0,    5,    0,    3,
    0,    0,    0,    0,    0,    0,    0,    0,   17,    1,
    1,    1,    1,    1,    1,    2,    2,    0,    0,    0,
    5,    0,    3,    2,    2,    0,    0,    3,    0,    0,
    5,    0,    3,    0,    0,    0,    0,    9,    2,    0,
    4,    0,    1,    0,    0,    6,    2,    0,    0,    5,
    0,    0,    6,    1,    1,    0,    0,    6,    1,    1,
    0,    3,    2,    2,    0,    0,    0,    5,    0,    0,
    6,    1,    0,    0,    0,    0,    0,    9,    0,    0,
    0,    0,   10,    1,    1,    0,    1,    1,    1,    0,
    3,    0,    4,    2,    0,    4,    0,    0,    0,    5,
    1,    1,    1,    1,    1,    1,    1,    0,    0,    6,
    0,    0,    0,    6,    0,    2,    0,    4,    0,
};
short tao_yydefred[] = {                                      3,
    0,    0,   16,   97,   18,   29,  146,  201,  221,  254,
  294,    0,    0,    0,    0,   52,  148,  150,  149,   25,
   43,    0,    0,    4,    2,    6,    8,   10,   12,   14,
   23,   24,   37,   38,   39,   40,   42,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   93,  151,  182,    0,
   47,    0,    0,    0,   41,    0,    0,    0,   34,   31,
    0,  188,    0,  192,  193,  196,  197,  198,  199,  195,
  200,  268,  275,  280,   54,   89,   60,  154,  155,  157,
  166,  167,  169,  171,  170,    0,  168,    0,  159,  161,
  162,  160,  165,  164,  184,  185,  156,  163,    0,   88,
    0,    0,    0,    0,    0,    0,    0,    0,   17,  108,
  109,    0,   98,  102,  103,  105,  106,  107,  104,   19,
   30,  152,  147,  202,  222,  255,  295,   32,    0,   33,
   53,   26,   44,    0,  187,  194,    0,  191,    0,    0,
  264,  267,   91,    0,  271,  276,    5,    7,    9,   11,
   13,   15,    0,    0,    0,    0,    0,    0,    0,   48,
   71,   62,    0,   35,  190,  269,   90,    0,    0,    0,
   51,    0,    0,   99,   20,  183,  153,  175,  176,  177,
    0,  203,  223,  256,  296,   71,    0,    0,    0,    0,
  136,  137,  138,  140,  142,  143,  144,  141,  139,    0,
    0,    0,    0,    0,  145,    0,    0,    0,    0,    0,
    0,  125,  129,  134,  265,   92,    0,  272,  277,    0,
    3,    0,    0,    0,    0,    0,    0,    0,   82,  292,
  304,  305,    0,    0,   72,   74,   76,    0,   70,   78,
   80,  309,   68,   66,    0,   61,   63,   64,   65,   85,
  270,  130,  131,  132,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  100,    0,
  173,  286,  282,  285,  208,  204,    0,    0,  224,  263,
  257,  262,  212,    0,    0,    0,  308,  322,  299,  323,
  307,    0,  321,  289,    0,    0,    0,   28,    0,    0,
    0,    0,   46,    0,  135,    0,    0,    0,    0,    0,
    0,    0,  126,  127,  128,  266,  273,  278,    0,    0,
    0,    0,    0,    0,    0,    0,  219,  216,  215,    0,
    0,    0,    0,  210,    0,   50,   83,    0,    0,   73,
   75,   77,   79,   81,   69,   67,    0,  101,  111,   22,
  174,  287,  284,  209,  205,    0,    0,    0,  234,  225,
    0,  230,  231,  233,  232,  258,  260,  213,  298,  300,
  290,    0,  220,  217,    0,    0,    0,    0,  288,    0,
  226,  261,    0,  301,  291,  181,  218,    0,    0,    0,
    0,    0,  227,  311,  325,  326,  327,  318,    0,  317,
  328,  302,  179,    0,    0,  313,    0,    0,    0,    0,
  242,  249,  247,  239,  246,  228,  238,  319,  315,  329,
  332,  303,  180,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  243,  250,  248,  252,  240,  245,
  229,  237,  320,  316,    0,  333,    0,    0,    0,  330,
    0,  251,  253,  241,  339,    0,    0,  334,  337,    0,
  338,
};
short tao_yydgoto[] = {                                       1,
  328,   78,   79,   80,   81,   82,   83,   84,  360,   85,
  176,  289,   86,   87,  291,  204,  422,  456,  457,  164,
  189,   60,  402,  171,  273,  323,  414,  428,  177,  222,
  385,  392,   20,   21,  348,  205,  206,  207,  208,  209,
  210,  211,  212,  213,  214,  215,  274,  415,  439,  113,
   89,   90,   91,   92,   93,   94,   95,   96,   97,   98,
  398,  233,  178,  179,  180,  234,   99,   22,   23,  100,
  235,    2,   25,  103,  236,  104,  237,  105,   28,  106,
   29,  107,   30,  108,   38,   40,  154,  221,  320,   31,
   32,   57,  187,  161,  238,   41,  134,   33,   34,   35,
   36,   37,   58,  188,  162,  245,  129,  186,  285,   56,
  246,  247,  239,  249,  329,  302,  301,  295,  296,  297,
  240,  299,  241,  300,  286,  304,  140,  169,   39,  153,
  220,  319,  123,   42,  155,  321,  410,   43,  156,  276,
  224,  325,  334,  284,  324,  333,  278,  357,  380,  356,
   44,  157,  225,  330,  375,  388,  416,  404,  429,  417,
  430,  427,  449,  424,  426,  425,  447,  448,   45,  158,
  281,  226,  331,  282,  332,  376,  168,  266,  139,  190,
  101,  172,  267,  102,  173,  268,  223,  322,  372,  339,
  378,   46,  159,  227,  335,  338,  384,  377,  391,  409,
  389,  399,  390,  400,  407,  432,  405,  431,  408,  433,
  434,  451,  460,
};
short tao_yysindex[] = {                                      0,
    0,  609,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -220, -219, -166, -252,    0,    0,    0,    0,    0,
    0,   -4,  437,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   39,  585, -194,
 -220,  463, -220, -220, -220, -220,    0,    0,    0,   -4,
    0,   -4, -214, -214,    0, -220,  -28,    2,    0,    0,
 -138,    0,  -84,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   20,    0, -199,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   -4,    0,
   77,   89,  100,  113,  127,  129,  135,  138,    0,    0,
    0, -199,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   93,    0,
    0,    0,    0, -222,    0,    0,  -40,    0,  171, -220,
    0,    0,    0,  -67,    0,    0,    0,    0,    0,    0,
    0,    0, -220,  117, -220,  120,  -39,  134,  136,    0,
    0,    0, -199,    0,    0,    0,    0,   16, -220, -222,
    0,   16,   16,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  651,  601,  227,  512,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   33,
   33,   33,   16, -199,    0,  163,  194,  253,  -86,   35,
  168,    0,    0,    0,    0,    0, -199,    0,    0,  228,
    0,  250,  205,   41,  270,   54,   56,  651,    0,    0,
    0,    0,  536,   29,    0,    0,    0,  189,    0,    0,
    0,    0,    0,    0,  190,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  277,   16,   16,   16,   16,   16,
   16,   16,   16,   16,   16,  269,  271,  273,    0,  609,
    0,    0,    0,    0,    0,    0,   56,  413,    0,    0,
    0,    0,    0,    0,  207,  289,    0,    0,    0,    0,
    0, -199,    0,    0,  293,  294,  295,    0,  296,  297,
  413,  413,    0, -222,    0,  194,  253,  -86,   35,   35,
  168,  168,    0,    0,    0,    0,    0,    0,   16,  233,
 -220,   16,  205,  300,  235,    0,    0,    0,    0,  153,
  236,  320,  308,    0,  240,    0,    0,  111,  562,    0,
    0,    0,    0,    0,    0,    0, -199,    0,    0,    0,
    0,    0,    0,    0,    0,  315, -220,  116,    0,    0,
 -199,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  282,    0,    0,  336,   54,  339, -220,    0,  321,
    0,    0,    0,    0,    0,    0,    0,  259,  343,  -60,
   94,  342,    0,    0,    0,    0,    0,    0,  348,    0,
    0,    0,    0,  -92,  562,    0,  346,  351,  101, -220,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  333,   16,  337,  463,  -47,  272,  -92,
 -220,  -60, -222,  354,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  355,    0,  344, -220,  345,    0,
  102,    0,    0,    0,    0,  367,  365,    0,    0,  107,
    0,
};
short tao_yyrindex[] = {                                      0,
    0,  414,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   -1,   57,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   42,
    0,   48,   81,  -46,    0,    0,    0,    0,    0,    0,
   -2,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    3,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  -93,    0,
    9,   59,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  156,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    8,    0,    0,    0,
    0,    0,    0,  301,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    7,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  162,    0,    0,    0,    0,    0,  278,  306,   44,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  -19,    0,  155,  179,  550,  125,   51,
   38,    0,    0,    0,    0,    0,  302,    0,    0,    0,
    0,  359,    0,  226,    0,    0,  251,  335,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  303,
    0,    0,    0,    0,    0,    0,  251,    0,    0,    0,
    0,    0,    0,  177,    0,    0,    0,    0,    0,    0,
    0,  173,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  574,  569,  493,   76,   88,
   61,  115,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   49,    0,    0,  202,    0,    0,    0,    0,
    0,  307,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   28,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  394,    0,    0,
  396,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -27,    0,    0,    0,    0,    0,    0,    0,
  -48,  379,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  411,    0,  398,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  486,    0,  352,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  420,    0,    0,    0,
    0,
};
short tao_yygindex[] = {                                      0,
  -21,  292,    0,    0,    0,  -35,   14,    4,    0,   36,
    0,    0,    0,  -34, -314,  -22,    0,    0,    0,   52,
    0,   34,    0,    0,    0,    0,    0,    0,  105,    0,
    0,    0,    0,  470,    0, -167,    0,  230,  244,  248,
   10,    6,  -18,   50,    0, -127,  184,   82,    0,    0,
  -32,  -31,  -30,  472,    0,  487,    0,    0, -178,  -29,
    0,    0, -255,   -9,    0,    0,    0,  262,  263,  -12,
   11,  288,    0,    0,   13,    0,   25,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  347,    0,    0,    0,    0,    0,    0,    0,
    0,  517,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  349,    0,  -16,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  314,  276,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  109,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  178,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  126,    0,    0,    0,    0,    0,    0,
    0,    0,    0,
};
#define TAO_YYTABLESIZE 944
short tao_yytable[] = {                                      49,
   88,   77,   48,  110,  111,   18,  114,  115,  116,  119,
  331,   55,   24,  310,   26,   17,  112,  133,  133,   88,
  122,  133,  133,  133,  371,  133,   27,  133,  121,   36,
  124,  125,  126,  127,   47,  255,   47,   19,  133,  133,
    6,  186,  133,  131,  218,  219,  158,   87,  189,   16,
   87,  189,  274,   59,  293,  203,  186,   94,  200,  186,
  201,  158,  120,  141,  158,  351,  189,  274,   86,  189,
  274,   86,  203,  133,  133,  122,   55,  261,  122,  262,
  122,  142,  122,  128,   84,  130,   75,   76,  119,  283,
  418,  119,  283,    6,  132,  122,  122,  109,  123,  122,
   95,  123,  279,  123,  133,  123,   96,  283,  119,  119,
  143,  163,  119,  120,   55,   59,  120,  279,  123,  123,
  279,   36,  123,  135,  133,  121,  136,  167,  121,   87,
  122,  122,  144,  120,  120,   16,  145,  120,   55,   58,
  174,  202,  181,  119,  119,  121,  121,  217,  146,  121,
   86,  349,  124,  123,  123,  124,  216,  124,  147,  124,
  293,  122,  117,  411,   36,  117,   84,   88,  120,  120,
   36,  148,  124,  124,  119,  443,  124,  137,  138,   55,
  121,  121,  117,  117,  123,  149,  117,  150,  412,  413,
   18,   18,  453,  151,  352,  112,  152,  288,  290,  120,
   17,   17,  182,   55,  265,  182,   36,  124,  124,  263,
  292,  121,  112,  112,  264,  160,  112,  117,  117,  113,
  182,  165,   19,   19,  259,  260,  293,  395,  396,  397,
  166,   18,  170,  412,  413,  183,  113,  113,  124,  175,
  113,   17,  182,  331,  313,  314,  315,  112,  117,  252,
  253,  254,  281,   55,  186,   88,  184,  436,  185,  158,
  312,  312,  312,   19,  189,  274,  311,  312,  309,  310,
  250,  113,   47,   18,   50,   52,   53,   54,   88,   88,
   24,  347,   26,   17,  345,  346,  256,  257,  269,   47,
  258,  133,  133,  271,   27,  272,  275,  362,  363,  364,
  365,  297,  113,  288,  290,   19,   87,  361,  181,  279,
  280,  283,  294,  298,  303,  279,  292,  305,  191,  192,
  193,  194,  195,  196,  197,   76,  206,   86,  198,  199,
  316,  336,  317,  359,  318,  191,  192,  193,  194,  195,
  196,  197,   76,   84,  181,  198,  199,  337,  122,  122,
  207,  340,  341,  342,  343,  344,   55,  350,  354,  355,
  366,  119,  119,  367,  369,   49,  368,  370,  386,  288,
  290,  123,  123,  373,  379,  211,  381,  135,  383,  387,
   55,  393,  292,  394,  401,  403,  120,  120,  406,  419,
  420,  435,  421,  446,  437,  450,  441,   49,  121,  121,
  423,  452,   27,  454,   88,  438,  455,  458,  459,   47,
  163,  461,  110,    1,  358,   62,   63,  172,  181,   66,
   67,   68,   69,   57,   56,  124,  124,   21,   10,  324,
   45,  259,  214,  214,  186,  181,  235,  178,  214,  214,
  214,  214,  214,  214,  214,  214,  214,  214,  214,  214,
  214,  314,  214,  214,  214,  214,  335,  214,  214,   49,
  336,  374,   76,  214,  214,  214,  214,  214,  214,  214,
  214,  214,  214,  214,  214,  214,  236,  214,  214,  214,
  214,  251,  214,   51,  445,  306,  214,  214,  214,  214,
  214,  214,  214,  214,  214,  214,  214,  214,  214,  214,
  307,  214,  214,  214,  214,  308,  353,  211,  270,  440,
  117,  214,  211,  211,  211,  211,  211,  211,  211,  211,
  211,  211,  211,  211,  211,  118,  211,  211,  211,  211,
  118,   55,  228,  118,  306,  214,  248,  277,  442,  306,
  306,  306,  306,  306,  306,  306,  306,  306,  306,  306,
  118,  118,  326,  382,  118,  306,  306,  444,    0,    0,
  211,  293,  306,    0,  306,    0,    0,  306,  306,  306,
  306,  306,  306,  306,  306,  306,  306,  306,    0,    0,
    0,    0,    0,  306,  306,  118,  118,  306,    0,  293,
  115,  306,  306,    0,    0,    0,  306,  306,  306,  306,
  306,  306,  306,  306,  306,  306,  306,  115,  115,  116,
    0,  115,  306,  306,  114,  306,  118,    0,  293,    0,
    0,  306,    0,    0,    0,    0,  116,  116,    0,    0,
  116,  114,  114,    0,    0,  114,    0,    0,    0,    0,
    0,    0,  115,  115,  306,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  116,  116,    0,    0,    0,  114,    0,  327,   47,
    0,    0,    0,  115,   61,   62,   63,   64,   65,   66,
   67,   68,   69,   70,   71,    8,    9,    0,   10,   72,
   73,   74,  116,   47,    0,    0,    0,  114,   61,   62,
   63,   64,   65,   66,   67,   68,   69,   70,   71,    8,
    9,    0,   10,   72,   73,   74,    0,    0,    0,   47,
    0,    0,   76,    0,   61,   62,   63,   64,   65,   66,
   67,   68,   69,   70,   71,    8,    9,   75,   10,   72,
   73,   74,  244,    0,    0,    0,   76,  244,  244,  244,
  244,  244,  244,  244,  244,  244,  244,  244,  244,  244,
    0,  244,  244,  244,  244,    0,    0,    0,   47,    0,
    0,    0,   76,   61,   62,   63,   64,   65,   66,   67,
   68,   69,   70,   71,    0,    0,    0,    0,   72,   73,
   74,    0,   47,    0,    0,  244,    0,   61,   62,   63,
   64,   65,   66,   67,   68,   69,   70,   71,    0,    0,
    0,    0,    0,   73,   74,    0,    0,    0,   47,    0,
    0,   76,  287,   61,   62,   63,   64,   65,   66,   67,
   68,   69,   70,   71,    0,    0,    0,    0,    0,   73,
   74,   47,    0,    0,    0,   76,   61,   62,   63,   64,
   65,   66,   67,   68,   69,   70,  229,    0,    4,    0,
    0,    7,   73,   74,    3,    0,    4,    5,    6,    7,
    0,   76,    0,    8,    9,    0,   10,    0,    0,    0,
   11,    8,    9,  230,   10,  231,  232,    0,   11,    0,
    0,    0,    0,   12,   76,    0,    0,  242,  243,  244,
    0,   12,   13,   14,   15,    0,  229,    0,    4,    0,
   16,    7,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    8,    9,    0,   10,    0,    0,    0,
   11,    0,    0,  230,    0,  231,  232,    0,    0,    0,
    0,    0,    0,   12,
};
short tao_yycheck[] = {                                      12,
   23,   23,   12,   39,   39,    2,   39,   39,   39,   39,
   59,   58,    2,   41,    2,    2,   39,   37,   38,   42,
   42,   41,   42,   43,  339,   45,    2,   47,   41,  123,
   43,   44,   45,   46,  257,  203,  257,    2,   58,   59,
  260,   44,   62,   56,  172,  173,   44,   41,   41,  302,
   44,   44,   44,   58,  233,   40,   59,   59,   43,   62,
   45,   59,  257,   44,   62,  321,   59,   59,   41,   62,
   62,   44,   40,   93,   94,   38,  123,   43,   41,   45,
   43,   62,   45,   50,   41,   52,  301,  310,   38,   41,
  405,   41,   44,  260,  123,   58,   59,   59,   38,   62,
   59,   41,   44,   43,  124,   45,   59,   59,   58,   59,
  310,  134,   62,   38,   58,   59,   41,   59,   58,   59,
   62,  123,   62,  262,  123,   38,  265,  140,   41,  123,
   93,   94,   99,   58,   59,  302,   60,   62,   58,   59,
  153,  126,  155,   93,   94,   58,   59,  170,   60,   62,
  123,  319,   38,   93,   94,   41,  169,   43,   59,   45,
  339,  124,   38,  256,  123,   41,  123,  190,   93,   94,
  123,   59,   58,   59,  124,  431,   62,  262,  263,  123,
   93,   94,   58,   59,  124,   59,   62,   59,  281,  282,
  187,  188,  448,   59,  322,   41,   59,  233,  233,  124,
  187,  188,   41,  123,   37,   44,  300,   93,   94,   42,
  233,  124,   58,   59,   47,  123,   62,   93,   94,   41,
   59,  262,  187,  188,  311,  312,  405,  288,  289,  290,
   60,  228,  300,  281,  282,  275,   58,   59,  124,  123,
   62,  228,  123,  292,  263,  264,  265,   93,  124,  200,
  201,  202,   91,  300,  257,  278,  123,  425,  123,  257,
  288,  289,  290,  228,  257,  257,  261,  262,  259,  260,
   44,   93,  257,  270,   13,   14,   14,   15,  301,  302,
  270,  304,  270,  270,  301,  302,  124,   94,   61,  257,
   38,  311,  312,   44,  270,   91,  256,  330,  330,  330,
  330,  125,  124,  339,  339,  270,  300,  330,  321,   40,
  257,  256,  284,  125,  125,  257,  339,   41,  303,  304,
  305,  306,  307,  308,  309,  310,  125,  300,  313,  314,
   62,  125,   62,  330,   62,  303,  304,  305,  306,  307,
  308,  309,  310,  300,  357,  313,  314,   59,  311,  312,
  125,   59,   59,   59,   59,   59,  300,  125,   59,  125,
  125,  311,  312,   44,  125,  378,   59,  257,  378,  405,
  405,  311,  312,   59,   93,  125,   41,  262,   40,   59,
  300,  123,  405,   41,  291,   44,  311,  312,   41,   44,
   40,   59,  292,   40,   58,   41,  125,  410,  311,  312,
  410,   58,  125,   59,  427,  427,  305,   41,   44,  257,
  433,  305,  257,    0,  262,  263,  264,   59,  431,  267,
  268,  269,  270,  123,  123,  311,  312,  125,  276,  257,
  125,  125,  256,  257,   41,  448,   41,   59,  262,  263,
  264,  265,  266,  267,  268,  269,  270,  271,  272,  273,
  274,   41,  276,  277,  278,  279,   59,  256,  257,  125,
   41,  357,  310,  262,  263,  264,  265,  266,  267,  268,
  269,  270,  271,  272,  273,  274,  125,  276,  277,  278,
  279,  190,  257,   14,  433,  256,  310,  262,  263,  264,
  265,  266,  267,  268,  269,  270,  271,  272,  273,  274,
  257,  276,  277,  278,  279,  258,  323,  257,  221,  428,
   39,  310,  262,  263,  264,  265,  266,  267,  268,  269,
  270,  271,  272,  273,  274,   39,  276,  277,  278,  279,
   38,   15,  186,   41,  257,  310,  188,  224,  430,  262,
  263,  264,  265,  266,  267,  268,  269,  270,  271,  272,
   58,   59,  277,  376,   62,  278,  279,  432,   -1,   -1,
  310,  284,  257,   -1,  287,   -1,   -1,  262,  263,  264,
  265,  266,  267,  268,  269,  270,  271,  272,   -1,   -1,
   -1,   -1,   -1,  278,  279,   93,   94,  310,   -1,  284,
   41,  257,  287,   -1,   -1,   -1,  262,  263,  264,  265,
  266,  267,  268,  269,  270,  271,  272,   58,   59,   41,
   -1,   62,  278,  279,   41,  310,  124,   -1,  284,   -1,
   -1,  287,   -1,   -1,   -1,   -1,   58,   59,   -1,   -1,
   62,   58,   59,   -1,   -1,   62,   -1,   -1,   -1,   -1,
   -1,   -1,   93,   94,  310,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   93,   94,   -1,   -1,   -1,   93,   -1,  256,  257,
   -1,   -1,   -1,  124,  262,  263,  264,  265,  266,  267,
  268,  269,  270,  271,  272,  273,  274,   -1,  276,  277,
  278,  279,  124,  257,   -1,   -1,   -1,  124,  262,  263,
  264,  265,  266,  267,  268,  269,  270,  271,  272,  273,
  274,   -1,  276,  277,  278,  279,   -1,   -1,   -1,  257,
   -1,   -1,  310,   -1,  262,  263,  264,  265,  266,  267,
  268,  269,  270,  271,  272,  273,  274,  301,  276,  277,
  278,  279,  257,   -1,   -1,   -1,  310,  262,  263,  264,
  265,  266,  267,  268,  269,  270,  271,  272,  273,  274,
   -1,  276,  277,  278,  279,   -1,   -1,   -1,  257,   -1,
   -1,   -1,  310,  262,  263,  264,  265,  266,  267,  268,
  269,  270,  271,  272,   -1,   -1,   -1,   -1,  277,  278,
  279,   -1,  257,   -1,   -1,  310,   -1,  262,  263,  264,
  265,  266,  267,  268,  269,  270,  271,  272,   -1,   -1,
   -1,   -1,   -1,  278,  279,   -1,   -1,   -1,  257,   -1,
   -1,  310,  287,  262,  263,  264,  265,  266,  267,  268,
  269,  270,  271,  272,   -1,   -1,   -1,   -1,   -1,  278,
  279,  257,   -1,   -1,   -1,  310,  262,  263,  264,  265,
  266,  267,  268,  269,  270,  271,  256,   -1,  258,   -1,
   -1,  261,  278,  279,  256,   -1,  258,  259,  260,  261,
   -1,  310,   -1,  273,  274,   -1,  276,   -1,   -1,   -1,
  280,  273,  274,  283,  276,  285,  286,   -1,  280,   -1,
   -1,   -1,   -1,  293,  310,   -1,   -1,  297,  298,  299,
   -1,  293,  294,  295,  296,   -1,  256,   -1,  258,   -1,
  302,  261,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  273,  274,   -1,  276,   -1,   -1,   -1,
  280,   -1,   -1,  283,   -1,  285,  286,   -1,   -1,   -1,
   -1,   -1,   -1,  293,
};
#define TAO_YYFINAL 1
#ifndef TAO_YYDEBUG
#define TAO_YYDEBUG 0
#endif
#define TAO_YYMAXTOKEN 314
#if TAO_YYDEBUG
const char *tao_yyname[] = {
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
"IDL_BOOLEAN","IDL_FIXED","IDL_ANY","IDL_STRUCT","IDL_UNION","IDL_SWITCH",
"IDL_ENUM","IDL_SEQUENCE","IDL_STRING","IDL_WSTRING","IDL_EXCEPTION","IDL_CASE",
"IDL_DEFAULT","IDL_READONLY","IDL_ATTRIBUTE","IDL_ONEWAY","IDL_IDEMPOTENT",
"IDL_VOID","IDL_IN","IDL_OUT","IDL_INOUT","IDL_RAISES","IDL_CONTEXT",
"IDL_NATIVE","IDL_LOCAL","IDL_ABSTRACT","IDL_CUSTOM","IDL_INIT","IDL_PRIVATE",
"IDL_PUBLIC","IDL_SUPPORTS","IDL_TRUNCATABLE","IDL_VALUETYPE",
"IDL_INTEGER_LITERAL","IDL_UINTEGER_LITERAL","IDL_STRING_LITERAL",
"IDL_CHARACTER_LITERAL","IDL_FLOATING_PT_LITERAL","IDL_TRUETOK","IDL_FALSETOK",
"IDL_SCOPE_DELIMITOR","IDL_LEFT_SHIFT","IDL_RIGHT_SHIFT","IDL_WCHAR_LITERAL",
"IDL_WSTRING_LITERAL",
};
const char *tao_yyrule[] = {
"$accept : start",
"start : definitions",
"definitions : definitions definition",
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
"definition : value_def $$6 ';'",
"$$7 :",
"definition : error $$7 ';'",
"$$8 :",
"$$9 :",
"$$10 :",
"$$11 :",
"module : IDL_MODULE $$8 IDENTIFIER $$9 '{' $$10 definitions $$11 '}'",
"interface_def : interface",
"interface_def : forward",
"$$12 :",
"$$13 :",
"$$14 :",
"interface : interface_header $$12 '{' $$13 exports $$14 '}'",
"$$15 :",
"interface_decl : IDL_INTERFACE $$15 id",
"interface_header : interface_decl inheritance_spec",
"interface_header : IDL_LOCAL interface_decl inheritance_spec",
"interface_header : IDL_ABSTRACT interface_decl inheritance_spec",
"$$16 :",
"inheritance_spec : ':' $$16 at_least_one_scoped_name",
"inheritance_spec :",
"value_def : valuetype",
"value_def : value_abs_decl",
"value_def : value_forward_decl",
"value_def : value_box_decl",
"valuetype : IDL_CUSTOM value_concrete_decl",
"valuetype : value_concrete_decl",
"$$17 :",
"$$18 :",
"$$19 :",
"value_concrete_decl : value_header $$17 '{' $$18 value_elements $$19 '}'",
"$$20 :",
"$$21 :",
"$$22 :",
"value_abs_decl : IDL_ABSTRACT value_header $$20 '{' $$21 exports $$22 '}'",
"value_header : value_decl opt_truncatable inheritance_spec supports_spec",
"$$23 :",
"value_decl : IDL_VALUETYPE $$23 id",
"opt_truncatable : IDL_TRUNCATABLE",
"opt_truncatable :",
"supports_spec : IDL_SUPPORTS scoped_name",
"supports_spec :",
"value_forward_decl : IDL_ABSTRACT value_decl",
"value_forward_decl : value_decl",
"value_box_decl : value_decl type_spec",
"value_elements : value_elements value_element",
"value_elements :",
"value_element : state_member",
"value_element : export",
"value_element : init_decl",
"$$24 :",
"state_member : IDL_PUBLIC $$24 member_i",
"$$25 :",
"state_member : IDL_PRIVATE $$25 member_i",
"exports : exports export",
"exports :",
"$$26 :",
"export : type_dcl $$26 ';'",
"$$27 :",
"export : const_dcl $$27 ';'",
"$$28 :",
"export : exception $$28 ';'",
"$$29 :",
"export : attribute $$29 ';'",
"$$30 :",
"export : operation $$30 ';'",
"$$31 :",
"export : error $$31 ';'",
"at_least_one_scoped_name : scoped_name scoped_names",
"$$32 :",
"scoped_names : scoped_names ',' $$32 scoped_name",
"scoped_names :",
"scoped_name : id",
"$$33 :",
"scoped_name : IDL_SCOPE_DELIMITOR $$33 id",
"$$34 :",
"scoped_name : scoped_name IDL_SCOPE_DELIMITOR $$34 id",
"id : IDENTIFIER",
"forward : interface_decl",
"forward : IDL_LOCAL interface_decl",
"forward : IDL_ABSTRACT interface_decl",
"$$35 :",
"$$36 :",
"$$37 :",
"$$38 :",
"const_dcl : IDL_CONST $$35 const_type $$36 id $$37 '=' $$38 expression",
"const_type : integer_type",
"const_type : char_type",
"const_type : octet_type",
"const_type : boolean_type",
"const_type : floating_pt_type",
"const_type : fixed_type",
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
"literal : IDL_UINTEGER_LITERAL",
"literal : IDL_STRING_LITERAL",
"literal : IDL_WSTRING_LITERAL",
"literal : IDL_CHARACTER_LITERAL",
"literal : IDL_WCHAR_LITERAL",
"literal : IDL_FLOATING_PT_LITERAL",
"literal : IDL_TRUETOK",
"literal : IDL_FALSETOK",
"positive_int_expr : const_expr",
"$$39 :",
"type_dcl : IDL_TYPEDEF $$39 type_declarator",
"type_dcl : struct_type",
"type_dcl : union_type",
"type_dcl : enum_type",
"type_dcl : IDL_NATIVE simple_declarator",
"$$40 :",
"type_declarator : type_spec $$40 at_least_one_declarator",
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
"base_type_spec : fixed_type",
"base_type_spec : any_type",
"template_type_spec : sequence_type_spec",
"template_type_spec : string_type_spec",
"template_type_spec : wstring_type_spec",
"constructed_type_spec : struct_type",
"constructed_type_spec : union_type",
"constructed_type_spec : enum_type",
"at_least_one_declarator : declarator declarators",
"$$41 :",
"declarators : declarators ',' $$41 declarator",
"declarators :",
"declarator : simple_declarator",
"declarator : complex_declarator",
"at_least_one_simple_declarator : simple_declarator simple_declarators",
"$$42 :",
"simple_declarators : simple_declarators ',' $$42 simple_declarator",
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
"fixed_type : IDL_FIXED",
"char_type : IDL_CHAR",
"char_type : IDL_WCHAR",
"octet_type : IDL_OCTET",
"boolean_type : IDL_BOOLEAN",
"any_type : IDL_ANY",
"$$43 :",
"$$44 :",
"$$45 :",
"$$46 :",
"struct_type : IDL_STRUCT $$43 id $$44 '{' $$45 at_least_one_member $$46 '}'",
"at_least_one_member : member members",
"at_least_one_member :",
"$$47 :",
"at_least_one_member : error $$47 ';'",
"members : members member",
"members :",
"$$48 :",
"members : error $$48 ';'",
"$$49 :",
"member : $$49 member_i",
"$$50 :",
"$$51 :",
"member_i : type_spec $$50 at_least_one_declarator $$51 ';'",
"$$52 :",
"member_i : error $$52 ';'",
"$$53 :",
"$$54 :",
"$$55 :",
"$$56 :",
"$$57 :",
"$$58 :",
"$$59 :",
"$$60 :",
"union_type : IDL_UNION $$53 id $$54 IDL_SWITCH $$55 '(' $$56 switch_type_spec $$57 ')' $$58 '{' $$59 at_least_one_case_branch $$60 '}'",
"switch_type_spec : integer_type",
"switch_type_spec : char_type",
"switch_type_spec : octet_type",
"switch_type_spec : boolean_type",
"switch_type_spec : enum_type",
"switch_type_spec : scoped_name",
"at_least_one_case_branch : case_branch case_branches",
"case_branches : case_branches case_branch",
"case_branches :",
"$$61 :",
"$$62 :",
"case_branch : at_least_one_case_label $$61 element_spec $$62 ';'",
"$$63 :",
"case_branch : error $$63 ';'",
"at_least_one_case_label : case_label case_labels",
"case_labels : case_labels case_label",
"case_labels :",
"$$64 :",
"case_label : IDL_DEFAULT $$64 ':'",
"$$65 :",
"$$66 :",
"case_label : IDL_CASE $$65 const_expr $$66 ':'",
"$$67 :",
"element_spec : type_spec $$67 declarator",
"$$68 :",
"$$69 :",
"$$70 :",
"$$71 :",
"enum_type : IDL_ENUM $$68 id $$69 '{' $$70 at_least_one_enumerator $$71 '}'",
"at_least_one_enumerator : enumerator enumerators",
"$$72 :",
"enumerators : enumerators ',' $$72 enumerator",
"enumerators :",
"enumerator : IDENTIFIER",
"$$73 :",
"$$74 :",
"sequence_type_spec : seq_head ',' $$73 positive_int_expr $$74 '>'",
"sequence_type_spec : seq_head '>'",
"$$75 :",
"$$76 :",
"seq_head : IDL_SEQUENCE $$75 '<' $$76 simple_type_spec",
"$$77 :",
"$$78 :",
"string_type_spec : string_head '<' $$77 positive_int_expr $$78 '>'",
"string_type_spec : string_head",
"string_head : IDL_STRING",
"$$79 :",
"$$80 :",
"wstring_type_spec : wstring_head '<' $$79 positive_int_expr $$80 '>'",
"wstring_type_spec : wstring_head",
"wstring_head : IDL_WSTRING",
"$$81 :",
"array_declarator : id $$81 at_least_one_array_dim",
"at_least_one_array_dim : array_dim array_dims",
"array_dims : array_dims array_dim",
"array_dims :",
"$$82 :",
"$$83 :",
"array_dim : '[' $$82 positive_int_expr $$83 ']'",
"$$84 :",
"$$85 :",
"attribute : opt_readonly IDL_ATTRIBUTE $$84 param_type_spec $$85 at_least_one_simple_declarator",
"opt_readonly : IDL_READONLY",
"opt_readonly :",
"$$86 :",
"$$87 :",
"$$88 :",
"$$89 :",
"exception : IDL_EXCEPTION $$86 id $$87 '{' $$88 members $$89 '}'",
"$$90 :",
"$$91 :",
"$$92 :",
"$$93 :",
"operation : opt_op_attribute op_type_spec $$90 IDENTIFIER $$91 parameter_list $$92 opt_raises $$93 opt_context",
"opt_op_attribute : IDL_ONEWAY",
"opt_op_attribute : IDL_IDEMPOTENT",
"opt_op_attribute :",
"op_type_spec : param_type_spec",
"op_type_spec : IDL_VOID",
"init_decl : IDL_INIT",
"$$94 :",
"parameter_list : '(' $$94 ')'",
"$$95 :",
"parameter_list : '(' $$95 at_least_one_parameter ')'",
"at_least_one_parameter : parameter parameters",
"$$96 :",
"parameters : parameters ',' $$96 parameter",
"parameters :",
"$$97 :",
"$$98 :",
"parameter : direction $$97 param_type_spec $$98 declarator",
"param_type_spec : base_type_spec",
"param_type_spec : string_type_spec",
"param_type_spec : wstring_type_spec",
"param_type_spec : scoped_name",
"direction : IDL_IN",
"direction : IDL_OUT",
"direction : IDL_INOUT",
"$$99 :",
"$$100 :",
"opt_raises : IDL_RAISES $$99 '(' $$100 at_least_one_scoped_name ')'",
"opt_raises :",
"$$101 :",
"$$102 :",
"opt_context : IDL_CONTEXT $$101 '(' $$102 at_least_one_string_literal ')'",
"opt_context :",
"at_least_one_string_literal : IDL_STRING_LITERAL string_literals",
"$$103 :",
"string_literals : string_literals ',' $$103 IDL_STRING_LITERAL",
"string_literals :",
};
#endif
#define tao_yyclearin (tao_yychar=(-1))
#define tao_yyerrok (tao_yyerrflag=0)
#ifndef TAO_YYSTACKSIZE
#ifdef TAO_YYMAXDEPTH
#define TAO_YYSTACKSIZE TAO_YYMAXDEPTH
#else
#define TAO_YYSTACKSIZE 10000
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
#line 2676 "fe/idl.tao_yy"
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
 */
void
tao_yyerror(const char *msg)
{
  ACE_ERROR ((LM_ERROR,
              "%s\n",
              msg));
}
#line 990 "y.tab.cpp"
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
void TAO_YYDEBUG_REDUCE(int, int, const char *tao_yyrule_string, int tao_yynew_indent, int tao_yyrhs_count)
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
void TAO_YYDEBUG_SHIFT_LEXEME(int, int, const char *tao_yytoken_string, int tao_yynew_indent)
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
void TAO_YYDEBUG_LOOK_AHEAD(int, int tao_yytoken_num, const char *tao_yytoken_string, int tao_yyindent)
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
void TAO_YYDEBUG_DISCARD_STATE(int, int tao_yyindent)
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
void TAO_YYDEBUG_DISCARD_TOKEN(int, int, const char *tao_yytoken_string, int tao_yyindent)
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
void TAO_YYDEBUG_SHIFT_ERROR_LEXEME(int, int, int tao_yyindent)
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
    register const char *tao_yys;
#ifndef __cplusplus
    extern char *ace_foo();
#endif

    if ((tao_yys = ACE_OS::getenv("TAO_YYDEBUG")))
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
    if ((tao_yyn = tao_yydefred[tao_yystate])) goto tao_yyreduce;
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

//    tao_yyerror("syntax error");
#ifdef lint
    goto tao_yyerrlab;
#endif
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
#line 260 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        }
break;
case 5:
#line 264 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 6:
#line 268 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        }
break;
case 7:
#line 272 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 8:
#line 276 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        }
break;
case 9:
#line 280 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 10:
#line 284 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceDeclSeen);
        }
break;
case 11:
#line 288 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 12:
#line 292 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ModuleDeclSeen);
        }
break;
case 13:
#line 296 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 14:
#line 300 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ValuetypeDeclSeen);
        }
break;
case 15:
#line 304 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 16:
#line 308 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 17:
#line 312 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 18:
#line 319 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSeen);
	  }
break;
case 19:
#line 323 "fe/idl.tao_yy"
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
	      m = idl_global->gen ()->create_module (s, n, p);
	      (void) s->fe_add_module (m);
	    }
	    /*
	     * Push it on the stack
	     */
	    idl_global->scopes()->push(m);
	  }
break;
case 20:
#line 344 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSqSeen);
	  }
break;
case 21:
#line 348 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleBodySeen);
	  }
break;
case 22:
#line 352 "fe/idl.tao_yy"
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
case 25:
#line 374 "fe/idl.tao_yy"
{
	  UTL_Scope     *s = idl_global->scopes ()->top_non_null ();
	  AST_Interface *i = NULL;
	  AST_Decl	*v = NULL;
	  UTL_StrList   *p = idl_global->pragmas ();
	  ACE_UNUSED_ARG (v);

	  /*
	   * Make a new interface node and add it to its enclosing scope
	   */
	  if (s != NULL && tao_yyvsp[0].ihval != NULL) {
	    i = idl_global->gen ()->create_interface (
                                       tao_yyvsp[0].ihval->interface_name (),
                                       tao_yyvsp[0].ihval->inherits (),
                                       tao_yyvsp[0].ihval->n_inherits (),
                                       tao_yyvsp[0].ihval->inherits_flat (),
                                       tao_yyvsp[0].ihval->n_inherits_flat (),
                                       p,
                                       tao_yyvsp[0].ihval->is_local (),
                                       tao_yyvsp[0].ihval->is_abstract ()
                                     );
            AST_Interface::fwd_redefinition_helper (i,s,p);
	    /*
	     * Add the interface to its definition scope
	     */
	    (void) s->fe_add_interface (i);
	  }
	  /*
	   * Push it on the scope stack
	   */
	  idl_global->scopes ()->push (i);
        }
break;
case 26:
#line 407 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSqSeen);
	}
break;
case 27:
#line 411 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceBodySeen);
	}
break;
case 28:
#line 415 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceQsSeen);
	  /*
	   * Done with this interface - pop it off the scopes stack
	   */
	  UTL_Scope* s = idl_global->scopes ()->top();
	  AST_Interface* m = AST_Interface::narrow_from_scope (s);
          if (m != NULL)
            {
	      m->inherited_name_clash ();
	      UTL_StrList *p = m->pragmas ();
	      if (p != 0)
                {
	          p = (UTL_StrList*) p->copy ();
                }
	      idl_global->set_pragmas (p);
            }
	  idl_global->scopes ()->pop();
	}
break;
case 29:
#line 438 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSeen);
	 }
break;
case 30:
#line 442 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceIDSeen);
	   tao_yyval.idval = tao_yyvsp[0].idval;
	 }
break;
case 31:
#line 450 "fe/idl.tao_yy"
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
case 32:
#line 462 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InheritSpecSeen);
	  /*
	   * Create an AST representation of the information in the header
	   * part of an interface - this representation contains a computed
	   * list of all interfaces which this interface inherits from,
	   * recursively
	   */
	  tao_yyval.ihval = new FE_Local_InterfaceHeader(new UTL_ScopedName(tao_yyvsp[-1].idval, NULL), tao_yyvsp[0].nlval);
	}
break;
case 33:
#line 474 "fe/idl.tao_yy"
{
           cerr << "error in " << idl_global->filename()->get_string()
		<< " line " << idl_global->lineno() << ":\n" ;
           cerr << "Sorry, I (TAO_IDL) can't handle abstract interface yet\n";
            /* (if not truncatable) */
#if 0
	  idl_global->set_parse_state(IDL_GlobalData::PS_InheritSpecSeen);
	  /*
	   * Create an AST representation of the information in the header
	   * part of an interface - this representation contains a computed
	   * list of all interfaces which this interface inherits from,
	   * recursively
	   */
	  tao_yyval.ihval = new FE_Abstract_InterfaceHeader(new UTL_ScopedName(tao_yyvsp[-1].idval, NULL), tao_yyvsp[0].nlval);
#endif
	}
break;
case 34:
#line 494 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InheritColonSeen);
        }
break;
case 35:
#line 498 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = tao_yyvsp[0].nlval;
	}
break;
case 36:
#line 502 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = NULL;
	}
break;
case 41:
#line 516 "fe/idl.tao_yy"
{
           cerr << "error in " << idl_global->filename()->get_string()
		<< " line " << idl_global->lineno() << ":\n" ;
           cerr << "Sorry, I (TAO_IDL) can't handle custom yet\n";
            /* set custom (if not truncatable) */
        }
break;
case 43:
#line 527 "fe/idl.tao_yy"
{
	  UTL_Scope     *s = idl_global->scopes()->top_non_null();
	  AST_Interface *i = NULL;
	  UTL_StrList   *p = idl_global->pragmas();

	  if (s != NULL && tao_yyvsp[0].vhval != NULL) {
	    i = idl_global->gen()->create_valuetype(tao_yyvsp[0].vhval->interface_name(),
						    tao_yyvsp[0].vhval->inherits(),
						    tao_yyvsp[0].vhval->n_inherits(),
						    p);
            AST_Interface::fwd_redefinition_helper (i,s,p);
	    /*
	     * Add the valuetype to its definition scope
	     */
	    (void) s->fe_add_interface(i);
	  }
	  /*
	   * Push it on the scope stack
	   */
	  idl_global->scopes()->push(i);
        }
break;
case 44:
#line 549 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSqSeen);
	}
break;
case 45:
#line 553 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceBodySeen);
	}
break;
case 46:
#line 557 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceQsSeen);
	  /*
	   * Done with this interface - pop it off the scopes stack
	   */
	  UTL_Scope* s = idl_global->scopes()->top();
	  AST_Interface* m = AST_Interface::narrow_from_scope (s);
	  m->inherited_name_clash ();
	  UTL_StrList *p = m->pragmas ();
	  if (p != 0) p = (UTL_StrList*)p->copy ();
	  idl_global->set_pragmas (p);
	  idl_global->scopes()->pop();
        }
break;
case 47:
#line 575 "fe/idl.tao_yy"
{
	  UTL_Scope     *s = idl_global->scopes()->top_non_null();
	  AST_Interface *i = NULL;
	  UTL_StrList   *p = idl_global->pragmas();

	  if (s != NULL && tao_yyvsp[0].vhval != NULL) {

            if (tao_yyvsp[0].vhval->n_concrete() > 0) {
              idl_global->err()->abstract_inheritance_error (tao_yyvsp[0].vhval->interface_name ());
            }

	    i = idl_global->gen()->create_valuetype(tao_yyvsp[0].vhval->interface_name(),
						    tao_yyvsp[0].vhval->inherits(),
						    tao_yyvsp[0].vhval->n_inherits(),
						    p);
            i->set_abstract_valuetype ();
            AST_Interface::fwd_redefinition_helper (i,s,p);
	    /*
	     * Add the valuetype to its definition scope
	     */
	    (void) s->fe_add_interface(i);
	  }
	  /*
	   * Push it on the scope stack
	   */
	  idl_global->scopes()->push(i);
        }
break;
case 48:
#line 603 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSqSeen);
	}
break;
case 49:
#line 607 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceBodySeen);
	}
break;
case 50:
#line 611 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceQsSeen);
	  /*
	   * Done with this interface - pop it off the scopes stack
	   */
	  UTL_Scope* s = idl_global->scopes()->top();
	  AST_Interface* m = AST_Interface::narrow_from_scope (s);
	  m->inherited_name_clash ();
	  UTL_StrList *p = m->pragmas ();
	  if (p != 0) p = (UTL_StrList*)p->copy ();
	  idl_global->set_pragmas (p);
	  idl_global->scopes()->pop();
        }
break;
case 51:
#line 632 "fe/idl.tao_yy"
{
          tao_yyval.vhval = new FE_obv_header (new UTL_ScopedName (tao_yyvsp[-3].idval, NULL), tao_yyvsp[-1].nlval, tao_yyvsp[0].nlval);
        }
break;
case 52:
#line 639 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_ValuetypeSeen);
        }
break;
case 53:
#line 643 "fe/idl.tao_yy"
{
           idl_global->set_parse_state(IDL_GlobalData::PS_ValuetypeIDSeen);
	   tao_yyval.idval = tao_yyvsp[0].idval;
        }
break;
case 54:
#line 651 "fe/idl.tao_yy"
{
            cerr << "warning in " << idl_global->filename()->get_string()
	         << " line " << idl_global->lineno() << ":\n" ;
            cerr << "truncatable modifier not supported and is ignored\n";
	    tao_yyval.bval = I_FALSE;
	    /* $$ = I_TRUE; */
	  }
break;
case 55:
#line 659 "fe/idl.tao_yy"
{
	    tao_yyval.bval = I_FALSE;
	  }
break;
case 56:
#line 667 "fe/idl.tao_yy"
{
              tao_yyval.nlval = new UTL_NameList(tao_yyvsp[0].idlist, NULL);
            }
break;
case 57:
#line 671 "fe/idl.tao_yy"
{
              tao_yyval.nlval = NULL;
            }
break;
case 58:
#line 679 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(tao_yyvsp[0].idval, NULL);
	  AST_InterfaceFwd	*f = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_ForwardDeclSeen);
	  /*
	   * Create a node representing a forward declaration of an
	   * valuetype. Store it in the enclosing scope
	   */
	  if (s != NULL) {
	    f = idl_global->gen()->create_valuetype_fwd(n, p);
         /*   if ($1)*/
            f->set_abstract_valuetype ();
	    (void) s->fe_add_interface_fwd(f);
	  }
          idl_global->set_pragmas (p);
	}
break;
case 59:
#line 700 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_ScopedName	*n = new UTL_ScopedName(tao_yyvsp[0].idval, NULL);
	  AST_InterfaceFwd	*f = NULL;
	  UTL_StrList		*p = idl_global->pragmas();

	  idl_global->set_parse_state(IDL_GlobalData::PS_ForwardDeclSeen);
	  /*
	   * Create a node representing a forward declaration of an
	   * valuetype. Store it in the enclosing scope
	   */
	  if (s != NULL) {
	    f = idl_global->gen()->create_valuetype_fwd(n, p);
	    (void) s->fe_add_interface_fwd(f);
	  }
          idl_global->set_pragmas (p);
	}
break;
case 60:
#line 722 "fe/idl.tao_yy"
{
           cerr << "error in " << idl_global->filename()->get_string()
		<< " line " << idl_global->lineno() << ":\n" ;
           cerr << "Sorry, I (TAO_IDL) can't handle boxes yet\n";
        }
break;
case 66:
#line 742 "fe/idl.tao_yy"
{
            /* is $0 to member_i */
            tao_yyval.vival = AST_Field::vis_PUBLIC;
          }
break;
case 68:
#line 748 "fe/idl.tao_yy"
{
            /* is $0 to member_i */
            tao_yyval.vival = AST_Field::vis_PRIVATE;
          }
break;
case 72:
#line 762 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        }
break;
case 73:
#line 766 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 74:
#line 770 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        }
break;
case 75:
#line 774 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 76:
#line 778 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        }
break;
case 77:
#line 782 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 78:
#line 786 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrDeclSeen);
        }
break;
case 79:
#line 790 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 80:
#line 794 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpDeclSeen);
        }
break;
case 81:
#line 798 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 82:
#line 802 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 83:
#line 806 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 84:
#line 814 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = new UTL_NameList(tao_yyvsp[-1].idlist, tao_yyvsp[0].nlval);
	}
break;
case 85:
#line 822 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SNListCommaSeen);
        }
break;
case 86:
#line 826 "fe/idl.tao_yy"
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
case 87:
#line 837 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = NULL;
	}
break;
case 88:
#line 844 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  tao_yyval.idlist = new UTL_IdList(tao_yyvsp[0].idval, NULL);
	}
break;
case 89:
#line 850 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        }
break;
case 90:
#line 854 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  tao_yyval.idlist = new UTL_IdList(new Identifier(tao_yyvsp[-2].strval, 1, 0, I_FALSE),
			      new UTL_IdList(tao_yyvsp[0].idval, NULL));
	}
break;
case 91:
#line 862 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        }
break;
case 92:
#line 866 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  tao_yyvsp[-3].idlist->nconc(new UTL_IdList(tao_yyvsp[0].idval, NULL));
	  tao_yyval.idlist = tao_yyvsp[-3].idlist;
	}
break;
case 93:
#line 875 "fe/idl.tao_yy"
{
            tao_yyval.idval = new Identifier(tao_yyvsp[0].strval, 1, 0, I_FALSE);
        }
break;
case 94:
#line 882 "fe/idl.tao_yy"
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
	    f = idl_global->gen()->create_interface_fwd(n, p, 0, 0);
	    (void) s->fe_add_interface_fwd(f);
	  }
          idl_global->set_pragmas (p);
	}
break;
case 95:
#line 901 "fe/idl.tao_yy"
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
	    f = idl_global->gen()->create_interface_fwd(n, p, 1, 0);
	    (void) s->fe_add_interface_fwd(f);
	  }
          idl_global->set_pragmas (p);
	}
break;
case 96:
#line 920 "fe/idl.tao_yy"
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
	    f = idl_global->gen()->create_interface_fwd(n, p, 0, 1);
	    (void) s->fe_add_interface_fwd(f);
	  }
          idl_global->set_pragmas (p);
	}
break;
case 97:
#line 941 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstSeen);
        }
break;
case 98:
#line 945 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstTypeSeen);
        }
break;
case 99:
#line 949 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstIDSeen);
        }
break;
case 100:
#line 953 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstAssignSeen);
        }
break;
case 101:
#line 957 "fe/idl.tao_yy"
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
          idl_global->set_pragmas (p);
	}
break;
case 108:
#line 992 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_string;
	}
break;
case 109:
#line 996 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_wstring;
        }
break;
case 110:
#line 1000 "fe/idl.tao_yy"
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
	    } else if (d->node_type () == AST_Decl::NT_string) {
                tao_yyval.etval = AST_Expression::EV_string;
            } else if (d->node_type () == AST_Decl::NT_wstring) {
		tao_yyval.etval = AST_Expression::EV_wstring;
            } else
	      tao_yyval.etval = AST_Expression::EV_any;
	  } else
	    tao_yyval.etval = AST_Expression::EV_any;
	}
break;
case 114:
#line 1046 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_or, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 116:
#line 1054 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_xor, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 118:
#line 1062 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_and, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 120:
#line 1070 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_left,tao_yyvsp[-2].exval,tao_yyvsp[0].exval);
	}
break;
case 121:
#line 1074 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_right,tao_yyvsp[-2].exval,tao_yyvsp[0].exval);
	}
break;
case 123:
#line 1082 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_add, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 124:
#line 1086 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_minus,tao_yyvsp[-2].exval,tao_yyvsp[0].exval);
	}
break;
case 126:
#line 1094 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_mul, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 127:
#line 1098 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_div, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 128:
#line 1102 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_mod, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 130:
#line 1110 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_u_plus,
					      tao_yyvsp[0].exval,
					      NULL);
	}
break;
case 131:
#line 1116 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_u_minus,
					      tao_yyvsp[0].exval,
					      NULL);
	}
break;
case 132:
#line 1122 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_bit_neg,
					      tao_yyvsp[0].exval,
					      NULL);
	}
break;
case 133:
#line 1131 "fe/idl.tao_yy"
{
	  /*
	   * An expression which is a scoped name is not resolved now,
	   * but only when it is evaluated (such as when it is assigned
	   * as a constant value)
	   */
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].idlist);
	}
break;
case 135:
#line 1141 "fe/idl.tao_yy"
{
	  tao_yyval.exval = tao_yyvsp[-1].exval;
	}
break;
case 136:
#line 1148 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].ival);
	}
break;
case 137:
#line 1152 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].uival);
	}
break;
case 138:
#line 1156 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].sval);
	}
break;
case 139:
#line 1160 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].wsval);
	}
break;
case 140:
#line 1164 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].cval);
	}
break;
case 141:
#line 1168 "fe/idl.tao_yy"
{
	  ACE_OutputCDR::from_wchar wc (tao_yyvsp[0].wcval);
	  tao_yyval.exval = idl_global->gen()->create_expr(wc);
	}
break;
case 142:
#line 1173 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].dval);
	}
break;
case 143:
#line 1177 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr((idl_bool) I_TRUE,
					      AST_Expression::EV_bool);
	}
break;
case 144:
#line 1182 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr((idl_bool) I_FALSE,
					      AST_Expression::EV_bool);
	}
break;
case 145:
#line 1190 "fe/idl.tao_yy"
{
	    tao_yyvsp[0].exval->evaluate(AST_Expression::EK_const);
	    tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].exval, AST_Expression::EV_ulong);
	}
break;
case 146:
#line 1198 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_TypedefSeen);
	  }
break;
case 147:
#line 1201 "fe/idl.tao_yy"
{tao_yyval.ival = 0;}
break;
case 148:
#line 1202 "fe/idl.tao_yy"
{ tao_yyval.ival = 0;}
break;
case 149:
#line 1203 "fe/idl.tao_yy"
{ tao_yyval.ival = 0;}
break;
case 150:
#line 1204 "fe/idl.tao_yy"
{ tao_yyval.ival = 0;}
break;
case 151:
#line 1206 "fe/idl.tao_yy"
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
          idl_global->set_pragmas (p);
	}
break;
case 152:
#line 1232 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeSpecSeen);
        }
break;
case 153:
#line 1236 "fe/idl.tao_yy"
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
	      t = idl_global->gen()->create_typedef(tp, d->name(), p,
                                                    s->is_local (),
                                                    s->is_abstract ());
	      (void) s->fe_add_typedef(t);
	    }
	    delete l;
	  }
          idl_global->set_pragmas (p);
	}
break;
case 156:
#line 1276 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 158:
#line 1281 "fe/idl.tao_yy"
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
case 172:
#line 1317 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = new UTL_DeclList(tao_yyvsp[-1].deval, tao_yyvsp[0].dlval);
	}
break;
case 173:
#line 1325 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        }
break;
case 174:
#line 1329 "fe/idl.tao_yy"
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
case 175:
#line 1340 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = NULL;
	}
break;
case 178:
#line 1352 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = new UTL_DeclList(tao_yyvsp[-1].deval, tao_yyvsp[0].dlval);
	}
break;
case 179:
#line 1360 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        }
break;
case 180:
#line 1364 "fe/idl.tao_yy"
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
case 181:
#line 1375 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = NULL;
	}
break;
case 182:
#line 1382 "fe/idl.tao_yy"
{
	  tao_yyval.deval = new FE_Declarator(new UTL_ScopedName(tao_yyvsp[0].idval, NULL),
				 FE_Declarator::FD_simple, NULL);
	}
break;
case 183:
#line 1390 "fe/idl.tao_yy"
{
	  tao_yyval.deval = new FE_Declarator(new UTL_ScopedName(tao_yyvsp[0].dcval->local_name(), NULL),
				 FE_Declarator::FD_complex,
				 tao_yyvsp[0].dcval);
	}
break;
case 186:
#line 1404 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_long;
	}
break;
case 187:
#line 1408 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_longlong;
        }
break;
case 188:
#line 1412 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_short;
	}
break;
case 189:
#line 1419 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_ulong;
	}
break;
case 190:
#line 1423 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_ulonglong;
        }
break;
case 191:
#line 1427 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_ushort;
	}
break;
case 192:
#line 1434 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_double;
	}
break;
case 193:
#line 1438 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_float;
	}
break;
case 194:
#line 1442 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_longdouble;
        }
break;
case 195:
#line 1449 "fe/idl.tao_yy"
{
           cerr << "error in " << idl_global->filename()->get_string()
		<< " line " << idl_global->lineno() << ":\n" ;
           cerr << "Sorry, I (TAO_IDL) can't handle fixed types yet\n";
	}
break;
case 196:
#line 1458 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_char;
	}
break;
case 197:
#line 1462 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_wchar;
        }
break;
case 198:
#line 1469 "fe/idl.tao_yy"
{
          tao_yyval.etval = AST_Expression::EV_octet;
	}
break;
case 199:
#line 1476 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_bool;
        }
break;
case 200:
#line 1483 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_any;
	}
break;
case 201:
#line 1490 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructSeen);
        }
break;
case 202:
#line 1494 "fe/idl.tao_yy"
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
	    d = idl_global->gen()->create_structure(n,
                                                    p,
                                                    s->is_local (),
                                                    s->is_abstract ());
	    (void) s->fe_add_structure(d);
	  }
	  /*
	   * Push the scope of the struct on the scopes stack
	   */
	  idl_global->scopes()->push(d);
	}
break;
case 203:
#line 1518 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructSqSeen);
        }
break;
case 204:
#line 1522 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructBodySeen);
        }
break;
case 205:
#line 1526 "fe/idl.tao_yy"
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
case 207:
#line 1545 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 208:
#line 1551 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 209:
#line 1555 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 212:
#line 1565 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 213:
#line 1569 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 214:
#line 1576 "fe/idl.tao_yy"
{
          /* is $0 to member_i */
          tao_yyval.vival = AST_Field::vis_NA;
        }
break;
case 216:
#line 1585 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberTypeSeen);
        }
break;
case 217:
#line 1589 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberDeclsSeen);
        }
break;
case 218:
#line 1593 "fe/idl.tao_yy"
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
              /* $0 denotes Visibility, must be on yacc reduction stack */
	      f = idl_global->gen()->create_field(tp, d->name(), p, tao_yyvsp[-5].vival);
	      (void) s->fe_add_field(f);
	    }
	    delete l;
	  }
	}
break;
case 219:
#line 1627 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 220:
#line 1631 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 221:
#line 1639 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionSeen);
        }
break;
case 222:
#line 1643 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionIDSeen);
        }
break;
case 223:
#line 1647 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchSeen);
        }
break;
case 224:
#line 1651 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchOpenParSeen);
        }
break;
case 225:
#line 1655 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchTypeSeen);
        }
break;
case 226:
#line 1659 "fe/idl.tao_yy"
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
	      u = idl_global->gen()->create_union(tp,
                                                  n,
                                                  p,
                                                  s->is_local (),
                                                  s->is_abstract ());
	      (void) s->fe_add_union(u);
 	    }
	  }
	  /*
	   * Push the scope of the union on the scopes stack
	   */
	  idl_global->scopes()->push(u);
	}
break;
case 227:
#line 1688 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionSqSeen);
        }
break;
case 228:
#line 1692 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionBodySeen);
        }
break;
case 229:
#line 1696 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionQsSeen);
	  /*
	   * Done with this union. Pop its scope from the scopes stack
	   */
	  tao_yyval.dcval =
	     AST_Union::narrow_from_scope (
                 idl_global->scopes()->top_non_null()
               );
          if (tao_yyval.dcval != 0)
            {
	      UTL_StrList *p = tao_yyval.dcval->pragmas ();
	      if (p != 0)
	         p = (UTL_StrList*)p->copy ();
	      idl_global->set_pragmas (p);
	      idl_global->scopes()->pop();
            }
	}
break;
case 230:
#line 1718 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 231:
#line 1722 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 232:
#line 1726 "fe/idl.tao_yy"
{
          /* octets are not allowed*/
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 233:
#line 1732 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 235:
#line 1737 "fe/idl.tao_yy"
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
case 239:
#line 1816 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionLabelSeen);
        }
break;
case 240:
#line 1820 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemSeen);
        }
break;
case 241:
#line 1824 "fe/idl.tao_yy"
{
	  UTL_Scope		*s = idl_global->scopes()->top_non_null();
	  UTL_LabellistActiveIterator *l = NULL;
	  AST_UnionLabel	*d = NULL;
	  AST_UnionBranch	*b = NULL;
	  AST_Field		*f = tao_yyvsp[-2].ffval;

          ACE_UNUSED_ARG (l);
          ACE_UNUSED_ARG (d);

	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemCompleted);
	  /*
	   * Create several nodes representing branches of a union.
	   * Add them to the enclosing scope (the union scope)
	   */
	  if (s != NULL && tao_yyvsp[-4].llval != NULL && tao_yyvsp[-2].ffval != NULL) {
	      b = idl_global->gen()->create_union_branch(tao_yyvsp[-4].llval,
						      f->field_type(),
						      f->name(),
						      f->pragmas());
	      (void) s->fe_add_union_branch(b);
	  }
	}
break;
case 242:
#line 1848 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 243:
#line 1853 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 244:
#line 1861 "fe/idl.tao_yy"
{
	  tao_yyval.llval = new UTL_LabelList(tao_yyvsp[-1].ulval, tao_yyvsp[0].llval);
	}
break;
case 245:
#line 1868 "fe/idl.tao_yy"
{
	  if (tao_yyvsp[-1].llval == NULL)
	    tao_yyval.llval = new UTL_LabelList(tao_yyvsp[0].ulval, NULL);
	  else {
	    tao_yyvsp[-1].llval->nconc(new UTL_LabelList(tao_yyvsp[0].ulval, NULL));
	    tao_yyval.llval = tao_yyvsp[-1].llval;
	  }
	}
break;
case 246:
#line 1877 "fe/idl.tao_yy"
{
	  tao_yyval.llval = NULL;
	}
break;
case 247:
#line 1884 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DefaultSeen);
        }
break;
case 248:
#line 1888 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

	  tao_yyval.ulval = idl_global->gen()->
	            create_union_label(AST_UnionLabel::UL_default,
				       NULL);
	}
break;
case 249:
#line 1896 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_CaseSeen);
        }
break;
case 250:
#line 1900 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelExprSeen);
        }
break;
case 251:
#line 1904 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

	  tao_yyval.ulval = idl_global->gen()->create_union_label(AST_UnionLabel::UL_label,
						     tao_yyvsp[-2].exval);
	}
break;
case 252:
#line 1914 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemTypeSeen);
        }
break;
case 253:
#line 1918 "fe/idl.tao_yy"
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
case 254:
#line 1944 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumSeen);
        }
break;
case 255:
#line 1948 "fe/idl.tao_yy"
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
	    e = idl_global->gen()->create_enum(n,
                                               p,
                                               s->is_local (),
                                               s->is_abstract ());
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
case 256:
#line 1975 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumSqSeen);
        }
break;
case 257:
#line 1979 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumBodySeen);
        }
break;
case 258:
#line 1983 "fe/idl.tao_yy"
{
          UTL_StrList *p = 0;
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumQsSeen);
	  /*
	   * Done with this enum. Pop its scope from the scopes stack
	   */
	  if (idl_global->scopes()->top() == NULL)
	    tao_yyval.dcval = NULL;
	  else {
	    tao_yyval.dcval = AST_Enum::narrow_from_scope(idl_global->scopes()->top_non_null());
            p = tao_yyval.dcval->pragmas ();
	    idl_global->scopes()->pop();
	  }
          idl_global->set_pragmas (p);
	}
break;
case 260:
#line 2005 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumCommaSeen);
        }
break;
case 263:
#line 2014 "fe/idl.tao_yy"
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
case 264:
#line 2038 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceCommaSeen);
        }
break;
case 265:
#line 2042 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceExprSeen);
        }
break;
case 266:
#line 2046 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceQsSeen);
	  /*
	   * Remove sequence marker from scopes stack
	   */
	  if (idl_global->scopes()->top() == NULL)
	    idl_global->scopes()->pop();
          UTL_Scope *s = idl_global->scopes()->top ();
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
	      ; /* Error will be caught in FE_Declarator.*/
	    else {
	      tao_yyval.dcval = idl_global->gen()->create_sequence(tao_yyvsp[-2].exval, tp, s->is_local (), s->is_abstract ());
	      /*
	       * Add this AST_Sequence to the types defined in the global scope
	       */
	      (void) idl_global->root()
		        ->fe_add_sequence(AST_Sequence::narrow_from_decl(tao_yyval.dcval));
 	    }
	  }
	}
break;
case 267:
#line 2077 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceQsSeen);
	  /*
	   * Remove sequence marker from scopes stack
	   */
	  if (idl_global->scopes()->top() == NULL)
	    idl_global->scopes()->pop();
          UTL_Scope *s = idl_global->scopes()->top_non_null ();
	  /*
	   * Create a node representing a sequence
	   */
	  if (tao_yyvsp[-1].dcval == NULL)
	    tao_yyval.dcval = NULL;
	  else {
	    AST_Type *tp = AST_Type::narrow_from_decl(tao_yyvsp[-1].dcval);
	    if (tp == NULL)
	      ; /* Error will be caught in FE_Declarator.*/
            else {
	      tao_yyval.dcval =
	        idl_global->gen()->create_sequence(
		  	     idl_global->gen()->create_expr((unsigned long) 0),
			     tp,
                             s->is_local (),
                             s->is_abstract ());
	      /*
	       * Add this AST_Sequence to the types defined in the global scope
	       */
	      (void) idl_global->root()
		        ->fe_add_sequence(AST_Sequence::narrow_from_decl(tao_yyval.dcval));
	    }
	  }
	}
break;
case 268:
#line 2110 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSeen);
	  /*
	   * Push a sequence marker on scopes stack
	   */
	  idl_global->scopes()->push(NULL);
	}
break;
case 269:
#line 2118 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSqSeen);
        }
break;
case 270:
#line 2122 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceTypeSeen);
	  tao_yyval.dcval = tao_yyvsp[0].dcval;
        }
break;
case 271:
#line 2131 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        }
break;
case 272:
#line 2135 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        }
break;
case 273:
#line 2139 "fe/idl.tao_yy"
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
case 274:
#line 2157 "fe/idl.tao_yy"
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
case 275:
#line 2175 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        }
break;
case 276:
#line 2183 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        }
break;
case 277:
#line 2187 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        }
break;
case 278:
#line 2191 "fe/idl.tao_yy"
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
case 279:
#line 2209 "fe/idl.tao_yy"
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
case 280:
#line 2227 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        }
break;
case 281:
#line 2234 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ArrayIDSeen);
        }
break;
case 282:
#line 2238 "fe/idl.tao_yy"
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
case 283:
#line 2252 "fe/idl.tao_yy"
{
	  tao_yyval.elval = new UTL_ExprList(tao_yyvsp[-1].exval, tao_yyvsp[0].elval);
	}
break;
case 284:
#line 2259 "fe/idl.tao_yy"
{
	  if (tao_yyvsp[-1].elval == NULL)
	    tao_yyval.elval = new UTL_ExprList(tao_yyvsp[0].exval, NULL);
	  else {
	    tao_yyvsp[-1].elval->nconc(new UTL_ExprList(tao_yyvsp[0].exval, NULL));
	    tao_yyval.elval = tao_yyvsp[-1].elval;
	  }
	}
break;
case 285:
#line 2268 "fe/idl.tao_yy"
{
	  tao_yyval.elval = NULL;
	}
break;
case 286:
#line 2275 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimSqSeen);
        }
break;
case 287:
#line 2279 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimExprSeen);
        }
break;
case 288:
#line 2283 "fe/idl.tao_yy"
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
case 289:
#line 2300 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrSeen);
        }
break;
case 290:
#line 2304 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrTypeSeen);
        }
break;
case 291:
#line 2308 "fe/idl.tao_yy"
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
	      a = idl_global->gen()->create_attribute(tao_yyvsp[-5].bval, tp, (UTL_IdList *) d->name()->copy (), p);
	      /*
	       * Add one attribute to the enclosing scope
	       */
	      (void) s->fe_add_attribute(a);
	    }
	    delete l;
	  }
          idl_global->set_pragmas (p);
	}
break;
case 292:
#line 2343 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrROSeen);
	  tao_yyval.bval = I_TRUE;
	}
break;
case 293:
#line 2348 "fe/idl.tao_yy"
{
	  tao_yyval.bval = I_FALSE;
	}
break;
case 294:
#line 2355 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSeen);
	}
break;
case 295:
#line 2359 "fe/idl.tao_yy"
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
	    e = idl_global->gen()->create_exception(n,
                                                    p,
                                                    s->is_local (),
                                                    s->is_abstract ());
	    (void) s->fe_add_exception(e);
	  }
	  /*
	   * Push the exception scope on the scope stack
	   */
	  idl_global->scopes()->push(e);
	}
break;
case 296:
#line 2383 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSqSeen);
        }
break;
case 297:
#line 2387 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptBodySeen);
        }
break;
case 298:
#line 2391 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptQsSeen);
	  /*
	   * Done with this exception. Pop its scope from the scope stack
	   */
          AST_Exception *ex =
            AST_Exception::narrow_from_scope (idl_global->scopes ()->top_non_null ());
          UTL_StrList *p = ex->pragmas ();
	  idl_global->scopes()->pop();
          idl_global->set_pragmas (p);
	}
break;
case 299:
#line 2407 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpTypeSeen);
        }
break;
case 300:
#line 2411 "fe/idl.tao_yy"
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
case 301:
#line 2440 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParsCompleted);
        }
break;
case 302:
#line 2444 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseCompleted);
        }
break;
case 303:
#line 2448 "fe/idl.tao_yy"
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
case 304:
#line 2473 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
	  tao_yyval.ofval = AST_Operation::OP_oneway;
	}
break;
case 305:
#line 2478 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
	  tao_yyval.ofval = AST_Operation::OP_idempotent;
	}
break;
case 306:
#line 2483 "fe/idl.tao_yy"
{
	  tao_yyval.ofval = AST_Operation::OP_noflags;
	}
break;
case 308:
#line 2491 "fe/idl.tao_yy"
{
	  tao_yyval.dcval =
	    idl_global->scopes()->bottom()
	       ->lookup_primitive_type(AST_Expression::EV_void);
	}
break;
case 309:
#line 2500 "fe/idl.tao_yy"
{
           cerr << "error in " << idl_global->filename()->get_string()
		<< " line " << idl_global->lineno() << ":\n" ;
           cerr << "Sorry, I (TAO_IDL) can't handle init yet\n";
        }
break;
case 310:
#line 2510 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        }
break;
case 311:
#line 2514 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        }
break;
case 312:
#line 2518 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        }
break;
case 313:
#line 2523 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        }
break;
case 315:
#line 2533 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParCommaSeen);
        }
break;
case 318:
#line 2542 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParDirSeen);
        }
break;
case 319:
#line 2546 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParTypeSeen);
        }
break;
case 320:
#line 2550 "fe/idl.tao_yy"
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
	      a = idl_global->gen()->create_argument(tao_yyvsp[-4].dival, tp, (UTL_IdList *) tao_yyvsp[0].deval->name ()->copy (), p);
	      (void) s->fe_add_argument(a);
	    }
	  }
	}
break;
case 321:
#line 2572 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 324:
#line 2578 "fe/idl.tao_yy"
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
case 325:
#line 2592 "fe/idl.tao_yy"
{
	  tao_yyval.dival = AST_Argument::dir_IN;
	}
break;
case 326:
#line 2596 "fe/idl.tao_yy"
{
	  tao_yyval.dival = AST_Argument::dir_OUT;
	}
break;
case 327:
#line 2600 "fe/idl.tao_yy"
{
	  tao_yyval.dival = AST_Argument::dir_INOUT;
	}
break;
case 328:
#line 2607 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSeen);
        }
break;
case 329:
#line 2611 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSqSeen);
        }
break;
case 330:
#line 2616 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseQsSeen);
	  tao_yyval.nlval = tao_yyvsp[-1].nlval;
	}
break;
case 331:
#line 2621 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = NULL;
	}
break;
case 332:
#line 2628 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSeen);
        }
break;
case 333:
#line 2632 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSqSeen);
        }
break;
case 334:
#line 2637 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextQsSeen);
	  tao_yyval.slval = tao_yyvsp[-1].slval;
	}
break;
case 335:
#line 2642 "fe/idl.tao_yy"
{
	  tao_yyval.slval = NULL;
	}
break;
case 336:
#line 2649 "fe/idl.tao_yy"
{
	  tao_yyval.slval = new UTL_StrList(tao_yyvsp[-1].sval, tao_yyvsp[0].slval);
	}
break;
case 337:
#line 2657 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextCommaSeen);
        }
break;
case 338:
#line 2661 "fe/idl.tao_yy"
{
	  if (tao_yyvsp[-3].slval == NULL)
	    tao_yyval.slval = new UTL_StrList(tao_yyvsp[0].sval, NULL);
	  else {
	    tao_yyvsp[-3].slval->nconc(new UTL_StrList(tao_yyvsp[0].sval, NULL));
	    tao_yyval.slval = tao_yyvsp[-3].slval;
	  }
	}
break;
case 339:
#line 2670 "fe/idl.tao_yy"
{
	  tao_yyval.slval = NULL;
	}
break;
#line 3837 "y.tab.cpp"
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
