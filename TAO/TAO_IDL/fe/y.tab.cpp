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
#define IDL_ABSTRACT 293
#define IDL_CUSTOM 294
#define IDL_INIT 295
#define IDL_PRIVATE 296
#define IDL_PUBLIC 297
#define IDL_SUPPORTS 298
#define IDL_TRUNCATABLE 299
#define IDL_VALUETYPE 300
#define IDL_INTEGER_LITERAL 301
#define IDL_UINTEGER_LITERAL 302
#define IDL_STRING_LITERAL 303
#define IDL_CHARACTER_LITERAL 304
#define IDL_FLOATING_PT_LITERAL 305
#define IDL_TRUETOK 306
#define IDL_FALSETOK 307
#define IDL_SCOPE_DELIMITOR 308
#define IDL_LEFT_SHIFT 309
#define IDL_RIGHT_SHIFT 310
#define IDL_WCHAR_LITERAL 311
#define IDL_WSTRING_LITERAL 312
#define TAO_YYERRCODE 256
short tao_yylhs[] = {                                        -1,
    0,   71,   71,   73,   72,   75,   72,   77,   72,   79,
   72,   81,   72,   83,   72,   84,   72,   85,   86,   87,
   88,   80,   78,   78,   91,   93,   94,   89,   95,   67,
   33,   96,   22,   22,   82,   82,   82,   82,   97,   97,
  102,  104,  105,  101,  106,  107,  108,   98,   34,  109,
   68,   66,   66,   24,   24,   99,   99,  100,  103,  103,
  110,  110,  110,  115,  111,  116,  111,   92,   92,  117,
  112,  118,  112,  119,  112,  121,  112,  123,  112,  124,
  112,   20,  125,   21,   21,   16,  126,   16,  127,   16,
   69,   90,  128,  129,  130,  131,   74,   50,   50,   50,
   50,   50,   50,   50,   50,   35,   36,   37,   37,   38,
   38,   39,   39,   40,   40,   40,   41,   41,   41,   42,
   42,   42,   42,   43,   43,   43,   43,   44,   44,   44,
   45,   45,   45,   45,   45,   45,   45,   45,   45,   46,
  133,   70,   70,   70,   70,   70,  134,  132,    1,    1,
    2,    2,    2,   58,   58,   58,   58,   58,   58,    4,
    4,    4,    3,    3,    3,   29,  135,   30,   30,   62,
   62,   31,  136,   32,   32,   63,   64,   51,   51,   56,
   56,   56,   57,   57,   57,   54,   54,   54,   52,   52,
   59,   53,   55,  137,  138,  140,  141,    7,  139,  143,
  143,  144,  142,  145,  146,  114,  147,  114,  148,  149,
  150,  151,  152,  153,  155,  156,   10,    9,    9,    9,
    9,    9,    9,  154,  158,  158,  159,  160,  157,  161,
  157,   27,   28,   28,  162,   48,  163,  164,   48,  165,
   49,  166,  167,  169,  170,    8,  168,  173,  172,  172,
  171,  174,  175,    5,    5,  176,  177,   13,  179,  180,
    6,    6,  178,  182,  183,   14,   14,  181,  184,   11,
   25,   26,   26,  185,  186,   47,  187,  188,  120,   65,
   65,  189,  190,  191,  192,   76,  193,  195,  196,  197,
  122,   61,   61,   61,   12,   12,  113,  198,  194,  200,
  194,  199,  203,  202,  202,  204,  205,  201,   15,   15,
   15,   15,   60,   60,   60,  206,  207,   23,   23,  208,
  209,   17,   17,   18,  210,   19,   19,
};
short tao_yylen[] = {                                         2,
    1,    2,    0,    0,    3,    0,    3,    0,    3,    0,
    3,    0,    3,    0,    3,    0,    3,    0,    0,    0,
    0,    9,    1,    1,    0,    0,    0,    7,    0,    3,
    2,    0,    3,    0,    1,    1,    1,    1,    2,    1,
    0,    0,    0,    7,    0,    0,    0,    8,    4,    0,
    3,    1,    0,    2,    0,    2,    1,    2,    2,    0,
    1,    1,    1,    0,    3,    0,    3,    2,    0,    0,
    3,    0,    3,    0,    3,    0,    3,    0,    3,    0,
    3,    2,    0,    4,    0,    1,    0,    3,    0,    4,
    1,    1,    0,    0,    0,    0,    9,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    3,    1,
    3,    1,    3,    1,    3,    3,    1,    3,    3,    1,
    3,    3,    3,    1,    2,    2,    2,    1,    1,    3,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    0,    3,    1,    1,    1,    2,    0,    3,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    2,    0,    4,    0,    1,
    1,    2,    0,    4,    0,    1,    1,    1,    1,    1,
    2,    1,    2,    3,    2,    1,    1,    2,    1,    1,
    1,    1,    1,    0,    0,    0,    0,    9,    2,    2,
    0,    0,    2,    0,    0,    5,    0,    3,    0,    0,
    0,    0,    0,    0,    0,    0,   17,    1,    1,    1,
    1,    1,    1,    2,    2,    0,    0,    0,    5,    0,
    3,    2,    2,    0,    0,    3,    0,    0,    5,    0,
    3,    0,    0,    0,    0,    9,    2,    0,    4,    0,
    1,    0,    0,    6,    2,    0,    0,    5,    0,    0,
    6,    1,    1,    0,    0,    6,    1,    1,    0,    3,
    2,    2,    0,    0,    0,    5,    0,    0,    6,    1,
    0,    0,    0,    0,    0,    9,    0,    0,    0,    0,
   10,    1,    1,    0,    1,    1,    1,    0,    3,    0,
    4,    2,    0,    4,    0,    0,    0,    5,    1,    1,
    1,    1,    1,    1,    1,    0,    0,    6,    0,    0,
    0,    6,    0,    2,    0,    4,    0,
};
short tao_yydefred[] = {                                      0,
   16,   93,   18,   29,  141,  194,  209,  242,  282,    0,
    0,    0,   50,    0,  143,  145,  144,   25,   41,    0,
    0,    4,    1,    0,    6,    8,   10,   12,   14,   23,
   24,   35,   36,   37,   38,   40,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   91,  146,  176,   45,    0,
    0,   39,    0,    0,    0,   32,   31,    0,  182,    0,
  186,  187,  189,  190,  191,  192,  193,  256,  263,  268,
   52,   87,   58,  149,  150,  152,  160,  161,  163,  165,
  164,    0,  162,    0,  154,  156,  157,  155,  159,  178,
  179,  151,  158,    0,   86,    0,    0,    0,    2,    0,
    0,    0,    0,    0,   17,  103,  104,    0,   94,   98,
   99,  101,  102,  100,   19,   30,  147,  142,  195,  210,
  243,  283,    0,   51,   26,   42,    0,  181,  188,    0,
  185,    0,    0,  252,  255,   89,    0,  259,  264,    5,
    7,    9,   11,   13,   15,    0,    0,    0,    0,    0,
    0,    0,   46,   69,   60,    0,   33,  184,  257,   88,
    0,    0,    0,   49,    0,    0,   95,   20,  177,  148,
  169,  170,  171,    0,  196,  211,  244,  284,   69,    0,
    0,    0,    0,  131,  132,  133,  135,  137,  138,  139,
  136,  134,    0,    0,    0,    0,    0,  140,    0,    0,
    0,    0,    0,    0,  120,  124,  129,  253,   90,    0,
  260,  265,    0,    0,    0,    0,  202,    0,    0,  201,
    0,   80,  280,  292,  293,    0,    0,   70,   72,   74,
    0,   68,   76,   78,  297,   66,   64,    0,   59,   61,
   62,   63,   83,  258,  125,  126,  127,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   96,   21,  167,  274,  270,  273,  197,  201,    0,
  212,  251,  245,  250,    0,    0,    0,  296,  310,  287,
  311,  295,    0,  309,  277,    0,    0,    0,   28,    0,
    0,    0,    0,   44,    0,  130,    0,    0,    0,    0,
    0,    0,    0,  121,  122,  123,  254,  261,  266,    0,
    0,    0,    0,    0,    0,    0,  207,  204,  203,    0,
    0,    0,  200,    0,   48,   81,    0,    0,   71,   73,
   75,   77,   79,   67,   65,    0,   97,  106,   22,  168,
  275,  272,  198,    0,    0,    0,  222,  213,    0,  218,
  219,  221,  220,  246,  248,  286,  288,  278,    0,  208,
  205,    0,    0,    0,    0,  276,    0,  214,  249,    0,
  289,  279,  175,  206,    0,    0,    0,    0,    0,  215,
  299,  313,  314,  315,  306,    0,  305,  316,  290,  173,
    0,    0,  301,    0,    0,    0,    0,  230,  237,  235,
  227,  234,  216,  226,  307,  303,  317,  320,  291,  174,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  231,  238,  236,  240,  228,  233,  217,  225,  308,
  304,    0,  321,    0,    0,    0,  318,    0,  239,  241,
  229,  327,    0,    0,  322,  325,    0,  326,
};
short tao_yydgoto[] = {                                      14,
  318,   74,   75,   76,   77,   78,   15,   16,  348,   17,
  169,  280,   82,   83,  282,  197,  409,  443,  444,  157,
  182,   57,  389,  164,  266,  314,  401,  415,  170,  215,
  372,  379,   18,   19,  337,  198,  199,  200,  201,  202,
  203,  204,  205,  206,  207,  208,  267,  402,  426,  109,
   85,   86,   87,   88,   89,   90,   91,   92,   93,  385,
  226,  171,  172,  173,  227,   94,   20,   21,   95,   22,
   23,   24,   98,   25,  100,   26,  101,   27,  102,   28,
  103,   29,  104,   37,   39,  147,  214,  311,   30,   31,
   54,  180,  154,  231,   40,  127,   32,   33,   34,   35,
   36,   55,  181,  155,  238,  123,  179,  276,   53,  239,
  240,  232,  242,  319,  293,  292,  286,  287,  288,  233,
  290,  234,  291,  277,  295,  133,  162,   38,  146,  213,
  310,  118,   41,  148,  312,  397,   42,  149,  268,  217,
  315,  323,  275,  270,  345,  367,  344,   43,  150,  218,
  320,  362,  375,  403,  391,  416,  404,  417,  414,  436,
  411,  413,  412,  434,  435,   44,  151,  273,  219,  321,
  274,  322,  363,  161,  259,  132,  183,   96,  165,  260,
   97,  166,  261,  216,  313,  359,  328,  365,   45,  152,
  220,  324,  327,  371,  364,  378,  396,  376,  386,  377,
  387,  394,  419,  392,  418,  395,  420,  421,  438,  447,
};
short tao_yysindex[] = {                                    555,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -219,
 -253, -253,    0,    0,    0,    0,    0,    0,    0,    1,
  398,    0,    0,  555,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    4,  348, -203, -219,
  423, -219, -219, -219, -219,    0,    0,    0,    0, -196,
 -196,    0, -219,  -15,   -8,    0,    0, -152,    0, -183,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    8,    0, -191,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    1,    0,   80,   89,   83,    0,  124,
  126,  134,  137,  165,    0,    0,    0, -191,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  104,    0,    0,    0, -208,    0,    0,  -24,
    0,  170, -219,    0,    0,    0,  -63,    0,    0,    0,
    0,    0,    0,    0,    0, -219,  122, -219,  128,  -21,
  133,  135,    0,    0,    0, -191,    0,    0,    0,    0,
   13, -219, -208,    0,   13,   13,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  609,
  579,  199,  470,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   26,   26,   26,   13, -191,    0,  140,  172,
  230, -164,   28,   45,    0,    0,    0,    0,    0, -191,
    0,    0,  208,  555,  227,  183,    0,  238,   25,    0,
  609,    0,    0,    0,    0,  493,    6,    0,    0,    0,
  159,    0,    0,    0,    0,    0,    0,  160,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  249,   13,   13,
   13,   13,   13,   13,   13,   13,   13,   13,  236,  241,
  243,    0,    0,    0,    0,    0,    0,    0,    0,  376,
    0,    0,    0,    0,    0,  181,  250,    0,    0,    0,
    0,    0, -191,    0,    0,  251,  252,  263,    0,  264,
  277,  376,  376,    0, -208,    0,  172,  230, -164,   28,
   28,   45,   45,    0,    0,    0,    0,    0,    0,   13,
  217, -219,   13,  183,  218,    0,    0,    0,    0,  535,
  219,  302,    0,  222,    0,    0,   91,  518,    0,    0,
    0,    0,    0,    0,    0, -191,    0,    0,    0,    0,
    0,    0,    0,  292, -219,   92,    0,    0, -191,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  266,    0,
    0,  315,   25,  317, -219,    0,  303,    0,    0,    0,
    0,    0,    0,    0,  240,  323,  -69,   75,  322,    0,
    0,    0,    0,    0,    0,  326,    0,    0,    0,    0,
 -151,  518,    0,  324,  331,   84, -219,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  318,   13,  316,  423, -107,  253, -151, -219,  -69, -208,
  336,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  338,    0,  327, -219,  329,    0,   78,    0,    0,
    0,    0,  345,  347,    0,    0,   86,    0,
};
short tao_yyrindex[] = {                                    392,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    5,
  -46,    0,    0,   29,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  -39,
   -3,    0,    0,    0,    0,    0,    0,   42,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   50,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -106,    0,   82,  127,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  136,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   -2,
    0,    0,    0,    0,    0,    0,  273,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  -17,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  116,    0,    0,    0,    0,    0,  228,
  255,   -4,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    3,    0,  167,  442,
  143,  499,  110,   31,    0,    0,    0,    0,    0,  274,
    0,    0,    0,  278,  340,    0,    0,    0,    0,    0,
  282,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  176,    0,    0,    0,    0,    0,
    0,    0,  145,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  513,  311,  526,  118,
  120,   40,   73,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  106,    0,  201,    0,    0,    0,    0,
    0,  281,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   47,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  369,    0,    0,  370,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  -26,
    0,    0,    0,    0,    0,    0,    0,  -45,  353,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  372,    0,  355,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  445,    0,  290,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  375,    0,    0,    0,    0,
};
short tao_yygindex[] = {                                      0,
  -19,  234,    0,    0,    0,  -32,  -13,  -16,    0,   -5,
    0,    0,    0,  -31, -297,  -20,    0,    0,    0,   11,
    0,  328,    0,    0,    0,    0,    0,    0,   76,    0,
    0,    0,    0,  407,    0, -173,    0,  174,  184,  173,
  -53,  -22,  -35,   54,    0,  -98,  112,   21,    0,    0,
  -34,  -29,  -28,  399,    0,    0,    0, -175,  -27,    0,
    0, -237,   -7,    0,    0,    0,    0,  198,  -10,  -59,
    2,    0,    0,    7,    0,   10,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  271,    0,    0,    0,    0,    0,    0,    0,    0,
  443,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  275,    0,  -52,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  242,  191,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   44,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   99,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   56,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
#define TAO_YYTABLESIZE 901
short tao_yytable[] = {                                      48,
   84,   73,   47,  110,   80,  106,  107,   79,  111,  112,
  114,   53,   57,  319,  298,   81,   34,  108,   53,   56,
   84,  117,  248,   85,   80,   99,   85,   79,    3,  116,
  358,  119,  120,  121,  122,   81,   82,   46,  183,  128,
  128,  183,  124,  128,  128,  128,   13,  128,   46,  128,
  284,  134,  196,  115,   53,  193,  183,  194,   56,  183,
  128,  128,  105,   92,  128,  196,  211,  212,  117,  135,
  254,  117,  255,  117,  340,  117,   53,  118,  130,  131,
  118,  258,  118,   53,  118,  180,  256,   84,  117,  117,
   84,  257,  117,  153,  405,  128,  128,  118,  118,   72,
  180,  118,   71,  180,  398,   85,  156,  125,  153,  128,
  119,  153,  129,  119,  126,  119,  136,  119,   82,   53,
  228,  228,  160,  117,  117,  262,  128,   34,  399,  400,
  119,  119,  118,  118,  119,  167,  338,  174,  195,  138,
  262,  140,  210,  262,  252,  253,  271,  114,  139,  271,
  114,  209,  284,    3,  117,  115,  176,  116,  115,  176,
  116,  228,   84,  118,  271,  119,  119,  114,  114,   84,
  267,  114,  399,  400,  176,  115,  115,  116,  116,  115,
  430,  116,  141,  110,  142,  267,  229,  229,  267,  230,
  230,   34,  143,  279,  281,  144,  119,  440,  300,  301,
  110,  110,  114,  114,  110,  283,  269,  107,   50,   51,
  115,  115,  116,  116,  341,  263,  284,  382,  383,  384,
  304,  305,  306,  145,  107,  107,  153,  229,  107,  159,
  230,  302,  303,  114,  163,  110,  110,  158,  423,  334,
  335,  115,  243,  116,  168,  319,  245,  246,  247,   84,
  175,   53,  176,   80,  183,  177,   79,  178,   53,  107,
  300,  300,  300,  249,   81,  250,  110,  251,  262,   46,
  264,   84,   84,  265,  336,   80,   80,  271,   79,   79,
   85,  272,   46,  289,  294,  350,   81,   81,  285,  296,
  351,  352,  353,   82,   53,  279,  281,  307,  180,  349,
  285,  174,  308,  347,  309,  325,  153,  283,  326,  329,
  330,  128,  128,  184,  185,  186,  187,  188,  189,  190,
   72,  331,  332,  191,  192,  199,  184,  185,  186,  187,
  188,  189,  190,   72,  174,  333,  191,  192,  262,  117,
  117,  339,  343,  354,   84,  355,  356,  357,  118,  118,
  360,  111,   27,  128,   48,  368,  370,  373,  366,  279,
  281,  374,  380,  381,  388,  390,  393,  406,  111,  111,
  407,  283,  111,  424,  408,  433,  422,  428,  437,   43,
  442,  119,  119,  267,  439,  445,   48,  441,  448,  410,
  446,    3,  105,   84,  425,   55,   54,   80,  166,  156,
   79,  312,    3,  111,  111,  247,   47,  174,   81,  180,
  223,  172,  302,  323,  224,  324,  244,   49,  114,  114,
  361,  137,  297,  299,  174,  342,  115,  115,  116,  116,
  432,  202,  202,  298,  111,  427,  113,  202,  202,  202,
  202,  202,  202,  202,  202,  202,  202,  202,  202,  221,
  202,  202,  202,  202,   52,  241,  202,  202,  269,  316,
  429,  369,  202,  202,  202,  202,  202,  202,  202,  202,
  202,  202,  202,  202,  431,  202,  202,  202,  202,    0,
    0,    0,  108,  202,  294,    0,    0,    0,    0,  294,
  294,  294,  294,  294,  294,  294,  294,  294,  294,  108,
  108,    0,    0,  108,  294,  294,    0,    0,  202,    0,
  281,  294,    0,  294,    0,    0,  294,  294,  294,  294,
  294,  294,  294,  294,  294,  294,    0,    0,    0,    0,
    0,  294,  294,    0,  108,  294,  112,  281,  294,  112,
  294,    0,    0,  294,  294,  294,  294,  294,  294,  294,
  294,  294,  294,  109,    0,    0,  112,  112,  294,  294,
  112,    0,  294,  113,  281,  108,  113,  294,    0,    0,
  109,  109,    0,    0,  109,    0,    0,    0,    0,    0,
    0,    0,    0,  113,  113,    0,    0,  113,    0,  294,
    0,  112,  112,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   46,  109,    0,    0,    0,   58,
   59,   60,   61,   62,   63,   64,   65,   66,  113,  113,
    0,    0,  112,    0,   69,   70,    0,    0,    0,    0,
    0,  317,   46,    0,    0,    0,  109,   58,   59,   60,
   61,   62,   63,   64,   65,   66,   67,    6,    7,  113,
    8,   68,   69,   70,   46,   72,    0,    0,    0,   58,
   59,   60,   61,   62,   63,   64,   65,   66,   67,    6,
    7,    0,    8,   68,   69,   70,    0,    0,    0,   46,
    0,    0,    0,   72,   58,   59,   60,   61,   62,   63,
   64,   65,   66,   67,    6,    7,   71,    8,   68,   69,
   70,  232,    0,    0,    0,   72,  232,  232,  232,  232,
  232,  232,  232,  232,  232,  232,  232,  232,    0,  232,
  232,  232,  232,    0,    0,    0,   46,    0,    0,    0,
   72,   58,   59,   60,   61,   62,   63,   64,   65,   66,
   67,    0,    0,    0,    0,   68,   69,   70,    0,   46,
    0,    0,  232,    0,   58,   59,   60,   61,   62,   63,
   64,   65,   66,   67,    0,    0,    0,    0,    0,   69,
   70,    0,    0,    0,   46,    0,    0,   72,  278,   58,
   59,   60,   61,   62,   63,   64,   65,   66,   67,    0,
    0,   46,    0,    0,   69,   70,  346,   59,   60,    0,
   72,   63,   64,   65,   66,    0,    0,    0,    0,    8,
    1,    0,    2,    3,    4,    5,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   72,    6,    7,    0,    8,
    0,    0,    0,    9,  222,    0,    2,    0,    0,    5,
    0,    0,   72,    0,    0,    0,   10,   11,   12,    0,
    6,    7,    0,    8,   13,    0,    0,    9,    0,    0,
  223,    0,  224,  225,  222,    0,    2,    0,    0,    5,
   10,    0,    0,  235,  236,  237,    0,    0,    0,    0,
    6,    7,    0,    8,    0,    0,    0,    9,    0,    0,
  223,    0,  224,  225,    0,    0,    0,    0,    0,    0,
   10,
};
short tao_yycheck[] = {                                      10,
   21,   21,   10,   38,   21,   38,   38,   21,   38,   38,
   38,   58,   59,   59,   41,   21,  123,   38,   58,   59,
   41,   41,  196,   41,   41,   24,   44,   41,    0,   40,
  328,   42,   43,   44,   45,   41,   41,  257,   41,   37,
   38,   44,   53,   41,   42,   43,  300,   45,  257,   47,
  226,   44,   40,  257,   58,   43,   59,   45,   58,   62,
   58,   59,   59,   59,   62,   40,  165,  166,   38,   62,
   43,   41,   45,   43,  312,   45,  123,   38,  262,  263,
   41,   37,   43,  123,   45,   44,   42,   41,   58,   59,
   44,   47,   62,   44,  392,   93,   94,   58,   59,  308,
   59,   62,  299,   62,  256,  123,  127,  123,   59,  262,
   38,   62,  265,   41,  123,   43,  308,   45,  123,  123,
  180,  181,  133,   93,   94,   44,  124,  123,  280,  281,
   58,   59,   93,   94,   62,  146,  310,  148,  126,   60,
   59,   59,  163,   62,  309,  310,   41,   38,   60,   44,
   41,  162,  328,  125,  124,   38,   41,   38,   41,   44,
   41,  221,  183,  124,   59,   93,   94,   58,   59,  123,
   44,   62,  280,  281,   59,   58,   59,   58,   59,   62,
  418,   62,   59,   41,   59,   59,  180,  181,   62,  180,
  181,  298,   59,  226,  226,   59,  124,  435,  252,  253,
   58,   59,   93,   94,   62,  226,   91,   41,   11,   12,
   93,   94,   93,   94,  313,  214,  392,  287,  288,  289,
  256,  257,  258,   59,   58,   59,  123,  221,   62,   60,
  221,  254,  255,  124,  298,   93,   94,  262,  412,  292,
  293,  124,   44,  124,  123,  291,  193,  194,  195,  270,
  123,  298,  274,  270,  257,  123,  270,  123,  298,   93,
  287,  288,  289,  124,  270,   94,  124,   38,   61,  257,
   44,  292,  293,   91,  295,  292,  293,   40,  292,  293,
  298,  257,  257,  125,  125,  320,  292,  293,  283,   41,
  320,  320,  320,  298,  298,  328,  328,   62,  257,  320,
  125,  312,   62,  320,   62,  125,  257,  328,   59,   59,
   59,  309,  310,  301,  302,  303,  304,  305,  306,  307,
  308,   59,   59,  311,  312,  125,  301,  302,  303,  304,
  305,  306,  307,  308,  345,   59,  311,  312,  257,  309,
  310,  125,  125,  125,  298,   44,  125,  257,  309,  310,
   59,   41,  125,  262,  365,   41,   40,  365,   93,  392,
  392,   59,  123,   41,  290,   44,   41,   44,   58,   59,
   40,  392,   62,   58,  291,   40,   59,  125,   41,  125,
  303,  309,  310,  257,   58,   41,  397,   59,  303,  397,
   44,    0,  257,  414,  414,  123,  123,  414,   59,  420,
  414,  257,  125,   93,   94,  125,  125,  418,  414,   41,
   41,   59,   41,   59,  125,   41,  183,   11,  309,  310,
  345,   94,  249,  251,  435,  314,  309,  310,  309,  310,
  420,  256,  257,  250,  124,  415,   38,  262,  263,  264,
  265,  266,  267,  268,  269,  270,  271,  272,  273,  179,
  275,  276,  277,  278,   12,  181,  256,  257,  217,  269,
  417,  363,  262,  263,  264,  265,  266,  267,  268,  269,
  270,  271,  272,  273,  419,  275,  276,  277,  278,   -1,
   -1,   -1,   41,  308,  257,   -1,   -1,   -1,   -1,  262,
  263,  264,  265,  266,  267,  268,  269,  270,  271,   58,
   59,   -1,   -1,   62,  277,  278,   -1,   -1,  308,   -1,
  283,  257,   -1,  286,   -1,   -1,  262,  263,  264,  265,
  266,  267,  268,  269,  270,  271,   -1,   -1,   -1,   -1,
   -1,  277,  278,   -1,   93,  308,   38,  283,  257,   41,
  286,   -1,   -1,  262,  263,  264,  265,  266,  267,  268,
  269,  270,  271,   41,   -1,   -1,   58,   59,  277,  278,
   62,   -1,  308,   38,  283,  124,   41,  286,   -1,   -1,
   58,   59,   -1,   -1,   62,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   58,   59,   -1,   -1,   62,   -1,  308,
   -1,   93,   94,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  257,   93,   -1,   -1,   -1,  262,
  263,  264,  265,  266,  267,  268,  269,  270,   93,   94,
   -1,   -1,  124,   -1,  277,  278,   -1,   -1,   -1,   -1,
   -1,  256,  257,   -1,   -1,   -1,  124,  262,  263,  264,
  265,  266,  267,  268,  269,  270,  271,  272,  273,  124,
  275,  276,  277,  278,  257,  308,   -1,   -1,   -1,  262,
  263,  264,  265,  266,  267,  268,  269,  270,  271,  272,
  273,   -1,  275,  276,  277,  278,   -1,   -1,   -1,  257,
   -1,   -1,   -1,  308,  262,  263,  264,  265,  266,  267,
  268,  269,  270,  271,  272,  273,  299,  275,  276,  277,
  278,  257,   -1,   -1,   -1,  308,  262,  263,  264,  265,
  266,  267,  268,  269,  270,  271,  272,  273,   -1,  275,
  276,  277,  278,   -1,   -1,   -1,  257,   -1,   -1,   -1,
  308,  262,  263,  264,  265,  266,  267,  268,  269,  270,
  271,   -1,   -1,   -1,   -1,  276,  277,  278,   -1,  257,
   -1,   -1,  308,   -1,  262,  263,  264,  265,  266,  267,
  268,  269,  270,  271,   -1,   -1,   -1,   -1,   -1,  277,
  278,   -1,   -1,   -1,  257,   -1,   -1,  308,  286,  262,
  263,  264,  265,  266,  267,  268,  269,  270,  271,   -1,
   -1,  257,   -1,   -1,  277,  278,  262,  263,  264,   -1,
  308,  267,  268,  269,  270,   -1,   -1,   -1,   -1,  275,
  256,   -1,  258,  259,  260,  261,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  308,  272,  273,   -1,  275,
   -1,   -1,   -1,  279,  256,   -1,  258,   -1,   -1,  261,
   -1,   -1,  308,   -1,   -1,   -1,  292,  293,  294,   -1,
  272,  273,   -1,  275,  300,   -1,   -1,  279,   -1,   -1,
  282,   -1,  284,  285,  256,   -1,  258,   -1,   -1,  261,
  292,   -1,   -1,  295,  296,  297,   -1,   -1,   -1,   -1,
  272,  273,   -1,  275,   -1,   -1,   -1,  279,   -1,   -1,
  282,   -1,  284,  285,   -1,   -1,   -1,   -1,   -1,   -1,
  292,
};
#define TAO_YYFINAL 14
#ifndef TAO_YYDEBUG
#define TAO_YYDEBUG 0
#endif
#define TAO_YYMAXTOKEN 312
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
"IDL_BOOLEAN","IDL_ANY","IDL_STRUCT","IDL_UNION","IDL_SWITCH","IDL_ENUM",
"IDL_SEQUENCE","IDL_STRING","IDL_WSTRING","IDL_EXCEPTION","IDL_CASE",
"IDL_DEFAULT","IDL_READONLY","IDL_ATTRIBUTE","IDL_ONEWAY","IDL_IDEMPOTENT",
"IDL_VOID","IDL_IN","IDL_OUT","IDL_INOUT","IDL_RAISES","IDL_CONTEXT",
"IDL_NATIVE","IDL_ABSTRACT","IDL_CUSTOM","IDL_INIT","IDL_PRIVATE","IDL_PUBLIC",
"IDL_SUPPORTS","IDL_TRUNCATABLE","IDL_VALUETYPE","IDL_INTEGER_LITERAL",
"IDL_UINTEGER_LITERAL","IDL_STRING_LITERAL","IDL_CHARACTER_LITERAL",
"IDL_FLOATING_PT_LITERAL","IDL_TRUETOK","IDL_FALSETOK","IDL_SCOPE_DELIMITOR",
"IDL_LEFT_SHIFT","IDL_RIGHT_SHIFT","IDL_WCHAR_LITERAL","IDL_WSTRING_LITERAL",
};
const char *tao_yyrule[] = {
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
"members : members member",
"members :",
"$$47 :",
"member : $$47 member_i",
"$$48 :",
"$$49 :",
"member_i : type_spec $$48 at_least_one_declarator $$49 ';'",
"$$50 :",
"member_i : error $$50 ';'",
"$$51 :",
"$$52 :",
"$$53 :",
"$$54 :",
"$$55 :",
"$$56 :",
"$$57 :",
"$$58 :",
"union_type : IDL_UNION $$51 id $$52 IDL_SWITCH $$53 '(' $$54 switch_type_spec $$55 ')' $$56 '{' $$57 at_least_one_case_branch $$58 '}'",
"switch_type_spec : integer_type",
"switch_type_spec : char_type",
"switch_type_spec : octet_type",
"switch_type_spec : boolean_type",
"switch_type_spec : enum_type",
"switch_type_spec : scoped_name",
"at_least_one_case_branch : case_branch case_branches",
"case_branches : case_branches case_branch",
"case_branches :",
"$$59 :",
"$$60 :",
"case_branch : at_least_one_case_label $$59 element_spec $$60 ';'",
"$$61 :",
"case_branch : error $$61 ';'",
"at_least_one_case_label : case_label case_labels",
"case_labels : case_labels case_label",
"case_labels :",
"$$62 :",
"case_label : IDL_DEFAULT $$62 ':'",
"$$63 :",
"$$64 :",
"case_label : IDL_CASE $$63 const_expr $$64 ':'",
"$$65 :",
"element_spec : type_spec $$65 declarator",
"$$66 :",
"$$67 :",
"$$68 :",
"$$69 :",
"enum_type : IDL_ENUM $$66 id $$67 '{' $$68 at_least_one_enumerator $$69 '}'",
"at_least_one_enumerator : enumerator enumerators",
"$$70 :",
"enumerators : enumerators ',' $$70 enumerator",
"enumerators :",
"enumerator : IDENTIFIER",
"$$71 :",
"$$72 :",
"sequence_type_spec : seq_head ',' $$71 positive_int_expr $$72 '>'",
"sequence_type_spec : seq_head '>'",
"$$73 :",
"$$74 :",
"seq_head : IDL_SEQUENCE $$73 '<' $$74 simple_type_spec",
"$$75 :",
"$$76 :",
"string_type_spec : string_head '<' $$75 positive_int_expr $$76 '>'",
"string_type_spec : string_head",
"string_head : IDL_STRING",
"$$77 :",
"$$78 :",
"wstring_type_spec : wstring_head '<' $$77 positive_int_expr $$78 '>'",
"wstring_type_spec : wstring_head",
"wstring_head : IDL_WSTRING",
"$$79 :",
"array_declarator : id $$79 at_least_one_array_dim",
"at_least_one_array_dim : array_dim array_dims",
"array_dims : array_dims array_dim",
"array_dims :",
"$$80 :",
"$$81 :",
"array_dim : '[' $$80 positive_int_expr $$81 ']'",
"$$82 :",
"$$83 :",
"attribute : opt_readonly IDL_ATTRIBUTE $$82 param_type_spec $$83 at_least_one_simple_declarator",
"opt_readonly : IDL_READONLY",
"opt_readonly :",
"$$84 :",
"$$85 :",
"$$86 :",
"$$87 :",
"exception : IDL_EXCEPTION $$84 id $$85 '{' $$86 members $$87 '}'",
"$$88 :",
"$$89 :",
"$$90 :",
"$$91 :",
"operation : opt_op_attribute op_type_spec $$88 IDENTIFIER $$89 parameter_list $$90 opt_raises $$91 opt_context",
"opt_op_attribute : IDL_ONEWAY",
"opt_op_attribute : IDL_IDEMPOTENT",
"opt_op_attribute :",
"op_type_spec : param_type_spec",
"op_type_spec : IDL_VOID",
"init_decl : IDL_INIT",
"$$92 :",
"parameter_list : '(' $$92 ')'",
"$$93 :",
"parameter_list : '(' $$93 at_least_one_parameter ')'",
"at_least_one_parameter : parameter parameters",
"$$94 :",
"parameters : parameters ',' $$94 parameter",
"parameters :",
"$$95 :",
"$$96 :",
"parameter : direction $$95 param_type_spec $$96 declarator",
"param_type_spec : base_type_spec",
"param_type_spec : string_type_spec",
"param_type_spec : wstring_type_spec",
"param_type_spec : scoped_name",
"direction : IDL_IN",
"direction : IDL_OUT",
"direction : IDL_INOUT",
"$$97 :",
"$$98 :",
"opt_raises : IDL_RAISES $$97 '(' $$98 at_least_one_scoped_name ')'",
"opt_raises :",
"$$99 :",
"$$100 :",
"opt_context : IDL_CONTEXT $$99 '(' $$100 at_least_one_string_literal ')'",
"opt_context :",
"at_least_one_string_literal : IDL_STRING_LITERAL string_literals",
"$$101 :",
"string_literals : string_literals ',' $$101 IDL_STRING_LITERAL",
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
#line 2553 "fe/idl.tao_yy"
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
#line 957 "y.tab.cpp"
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

    tao_yyerror("syntax error");
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
#line 258 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        }
break;
case 5:
#line 262 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 6:
#line 266 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        }
break;
case 7:
#line 270 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 8:
#line 274 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        }
break;
case 9:
#line 278 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 10:
#line 282 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceDeclSeen);
        }
break;
case 11:
#line 286 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 12:
#line 290 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ModuleDeclSeen);
        }
break;
case 13:
#line 294 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 14:
#line 298 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ValuetypeDeclSeen);
        }
break;
case 15:
#line 302 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 16:
#line 306 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 17:
#line 310 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 18:
#line 317 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSeen);
	  }
break;
case 19:
#line 321 "fe/idl.tao_yy"
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
case 20:
#line 342 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSqSeen);
	  }
break;
case 21:
#line 346 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleBodySeen);
	  }
break;
case 22:
#line 350 "fe/idl.tao_yy"
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
#line 372 "fe/idl.tao_yy"
{
	  UTL_Scope     *s = idl_global->scopes()->top_non_null();
	  AST_Interface *i = NULL;
	  AST_Decl	*v = NULL;
	  UTL_StrList   *p = idl_global->pragmas();
	  ACE_UNUSED_ARG (v);

	  /*
	   * Make a new interface node and add it to its enclosing scope
	   */
	  if (s != NULL && tao_yyvsp[0].ihval != NULL) {
	    i = idl_global->gen()->create_interface(tao_yyvsp[0].ihval->interface_name(),
						    tao_yyvsp[0].ihval->inherits(),
						    tao_yyvsp[0].ihval->n_inherits(),
						    p);
            AST_Interface::fwd_redefinition_helper (i,s,p);
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
case 26:
#line 399 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSqSeen);
	}
break;
case 27:
#line 403 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceBodySeen);
	}
break;
case 28:
#line 407 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceQsSeen);
	  /*
	   * Done with this interface - pop it off the scopes stack
	   */
	  UTL_Scope* s = idl_global->scopes()->top();
	  AST_Interface* m = AST_Interface::narrow_from_scope (s);
	  m->inherited_name_clash ();
	  UTL_StrList *p = m->pragmas ();
	  if (p != 0)
		p = (UTL_StrList*)p->copy ();
	  idl_global->set_pragmas (p);
	  idl_global->scopes()->pop();
	}
break;
case 29:
#line 425 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSeen);
	 }
break;
case 30:
#line 429 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceIDSeen);
	   tao_yyval.idval = tao_yyvsp[0].idval;
	 }
break;
case 31:
#line 437 "fe/idl.tao_yy"
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
#line 451 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InheritColonSeen);
        }
break;
case 33:
#line 455 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = tao_yyvsp[0].nlval;
	}
break;
case 34:
#line 459 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = NULL;
	}
break;
case 39:
#line 473 "fe/idl.tao_yy"
{
           cerr << "error in " << idl_global->filename()->get_string()
		<< " line " << idl_global->lineno() << ":\n" ;
           cerr << "Sorry, I (TAO_IDL) can't handle custom yet\n";
            /* set custom (if not truncatable) */
        }
break;
case 41:
#line 484 "fe/idl.tao_yy"
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
case 42:
#line 506 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSqSeen);
	}
break;
case 43:
#line 510 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceBodySeen);
	}
break;
case 44:
#line 514 "fe/idl.tao_yy"
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
case 45:
#line 532 "fe/idl.tao_yy"
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
case 46:
#line 560 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSqSeen);
	}
break;
case 47:
#line 564 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceBodySeen);
	}
break;
case 48:
#line 568 "fe/idl.tao_yy"
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
case 49:
#line 589 "fe/idl.tao_yy"
{
          tao_yyval.vhval = new FE_obv_header (new UTL_ScopedName (tao_yyvsp[-3].idval, NULL), tao_yyvsp[-1].nlval, tao_yyvsp[0].nlval);
        }
break;
case 50:
#line 596 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_ValuetypeSeen);
        }
break;
case 51:
#line 600 "fe/idl.tao_yy"
{
           idl_global->set_parse_state(IDL_GlobalData::PS_ValuetypeIDSeen);
	   tao_yyval.idval = tao_yyvsp[0].idval;
        }
break;
case 52:
#line 608 "fe/idl.tao_yy"
{
            cerr << "warning in " << idl_global->filename()->get_string()
	         << " line " << idl_global->lineno() << ":\n" ;
            cerr << "truncatable modifier not supported and is ignored\n";
	    tao_yyval.bval = I_FALSE;
	    /* $$ = I_TRUE; */
	  }
break;
case 53:
#line 616 "fe/idl.tao_yy"
{
	    tao_yyval.bval = I_FALSE;
	  }
break;
case 54:
#line 624 "fe/idl.tao_yy"
{ 
              tao_yyval.nlval = new UTL_NameList(tao_yyvsp[0].idlist, NULL); 
            }
break;
case 55:
#line 628 "fe/idl.tao_yy"
{ 
              tao_yyval.nlval = NULL; 
            }
break;
case 56:
#line 636 "fe/idl.tao_yy"
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
case 57:
#line 657 "fe/idl.tao_yy"
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
case 58:
#line 679 "fe/idl.tao_yy"
{
           cerr << "error in " << idl_global->filename()->get_string()
		<< " line " << idl_global->lineno() << ":\n" ;
           cerr << "Sorry, I (TAO_IDL) can't handle boxes yet\n";
        }
break;
case 64:
#line 699 "fe/idl.tao_yy"
{
            /* is $0 to member_i */
            tao_yyval.vival = AST_Field::vis_PUBLIC;
          }
break;
case 66:
#line 705 "fe/idl.tao_yy"
{
            /* is $0 to member_i */
            tao_yyval.vival = AST_Field::vis_PRIVATE;
          }
break;
case 70:
#line 719 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        }
break;
case 71:
#line 723 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 72:
#line 727 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        }
break;
case 73:
#line 731 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 74:
#line 735 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        }
break;
case 75:
#line 739 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 76:
#line 743 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrDeclSeen);
        }
break;
case 77:
#line 747 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 78:
#line 751 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpDeclSeen);
        }
break;
case 79:
#line 755 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 80:
#line 759 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 81:
#line 763 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 82:
#line 771 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = new UTL_NameList(tao_yyvsp[-1].idlist, tao_yyvsp[0].nlval);
	}
break;
case 83:
#line 779 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SNListCommaSeen);
        }
break;
case 84:
#line 783 "fe/idl.tao_yy"
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
case 85:
#line 794 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = NULL;
	}
break;
case 86:
#line 801 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  tao_yyval.idlist = new UTL_IdList(tao_yyvsp[0].idval, NULL);
	}
break;
case 87:
#line 807 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        }
break;
case 88:
#line 811 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  tao_yyval.idlist = new UTL_IdList(new Identifier(tao_yyvsp[-2].strval, 1, 0, I_FALSE),
			      new UTL_IdList(tao_yyvsp[0].idval, NULL));
	}
break;
case 89:
#line 819 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        }
break;
case 90:
#line 823 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  tao_yyvsp[-3].idlist->nconc(new UTL_IdList(tao_yyvsp[0].idval, NULL));
	  tao_yyval.idlist = tao_yyvsp[-3].idlist;
	}
break;
case 91:
#line 832 "fe/idl.tao_yy"
{
            tao_yyval.idval = new Identifier(tao_yyvsp[0].strval, 1, 0, I_FALSE);
        }
break;
case 92:
#line 839 "fe/idl.tao_yy"
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
          idl_global->set_pragmas (p);
	}
break;
case 93:
#line 860 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstSeen);
        }
break;
case 94:
#line 864 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstTypeSeen);
        }
break;
case 95:
#line 868 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstIDSeen);
        }
break;
case 96:
#line 872 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstAssignSeen);
        }
break;
case 97:
#line 876 "fe/idl.tao_yy"
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
case 103:
#line 910 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_string;
	}
break;
case 104:
#line 914 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_wstring;
        }
break;
case 105:
#line 918 "fe/idl.tao_yy"
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
case 109:
#line 964 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_or, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 111:
#line 972 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_xor, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 113:
#line 980 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_and, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 115:
#line 988 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_right,tao_yyvsp[-2].exval,tao_yyvsp[0].exval);
	}
break;
case 116:
#line 992 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_left,tao_yyvsp[-2].exval,tao_yyvsp[0].exval);
	}
break;
case 118:
#line 1000 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_add, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 119:
#line 1004 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_minus,tao_yyvsp[-2].exval,tao_yyvsp[0].exval);
	}
break;
case 121:
#line 1012 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_mul, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 122:
#line 1016 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_div, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 123:
#line 1020 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_mod, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 125:
#line 1028 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_u_plus,
					      tao_yyvsp[0].exval,
					      NULL);
	}
break;
case 126:
#line 1034 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_u_minus,
					      tao_yyvsp[0].exval,
					      NULL);
	}
break;
case 127:
#line 1040 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_bit_neg,
					      tao_yyvsp[0].exval,
					      NULL);
	}
break;
case 128:
#line 1049 "fe/idl.tao_yy"
{
	  /*
	   * An expression which is a scoped name is not resolved now,
	   * but only when it is evaluated (such as when it is assigned
	   * as a constant value)
	   */
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].idlist);
	}
break;
case 130:
#line 1059 "fe/idl.tao_yy"
{
	  tao_yyval.exval = tao_yyvsp[-1].exval;
	}
break;
case 131:
#line 1066 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].ival);
	}
break;
case 132:
#line 1070 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].uival);
	}
break;
case 133:
#line 1074 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].sval);
	}
break;
case 134:
#line 1078 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].wsval);
	}
break;
case 135:
#line 1082 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].cval);
	}
break;
case 136:
#line 1086 "fe/idl.tao_yy"
{
	  ACE_OutputCDR::from_wchar wc (tao_yyvsp[0].wcval);
	  tao_yyval.exval = idl_global->gen()->create_expr(wc);
	}
break;
case 137:
#line 1091 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].dval);
	}
break;
case 138:
#line 1095 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr((idl_bool) I_TRUE,
					      AST_Expression::EV_bool);
	}
break;
case 139:
#line 1100 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr((idl_bool) I_FALSE,
					      AST_Expression::EV_bool);
	}
break;
case 140:
#line 1108 "fe/idl.tao_yy"
{
	    tao_yyvsp[0].exval->evaluate(AST_Expression::EK_const);
	    tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].exval, AST_Expression::EV_ulong);
	}
break;
case 141:
#line 1116 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_TypedefSeen);
	  }
break;
case 142:
#line 1119 "fe/idl.tao_yy"
{tao_yyval.ival = 0;}
break;
case 143:
#line 1120 "fe/idl.tao_yy"
{ tao_yyval.ival = 0;}
break;
case 144:
#line 1121 "fe/idl.tao_yy"
{ tao_yyval.ival = 0;}
break;
case 145:
#line 1122 "fe/idl.tao_yy"
{ tao_yyval.ival = 0;}
break;
case 146:
#line 1124 "fe/idl.tao_yy"
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
case 147:
#line 1150 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeSpecSeen);
        }
break;
case 148:
#line 1154 "fe/idl.tao_yy"
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
          idl_global->set_pragmas (p);
	}
break;
case 151:
#line 1194 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 153:
#line 1199 "fe/idl.tao_yy"
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
case 166:
#line 1234 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = new UTL_DeclList(tao_yyvsp[-1].deval, tao_yyvsp[0].dlval);
	}
break;
case 167:
#line 1242 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        }
break;
case 168:
#line 1246 "fe/idl.tao_yy"
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
case 169:
#line 1257 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = NULL;
	}
break;
case 172:
#line 1269 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = new UTL_DeclList(tao_yyvsp[-1].deval, tao_yyvsp[0].dlval);
	}
break;
case 173:
#line 1277 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        }
break;
case 174:
#line 1281 "fe/idl.tao_yy"
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
#line 1292 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = NULL;
	}
break;
case 176:
#line 1299 "fe/idl.tao_yy"
{
	  tao_yyval.deval = new FE_Declarator(new UTL_ScopedName(tao_yyvsp[0].idval, NULL),
				 FE_Declarator::FD_simple, NULL);
	}
break;
case 177:
#line 1307 "fe/idl.tao_yy"
{
	  tao_yyval.deval = new FE_Declarator(new UTL_ScopedName(tao_yyvsp[0].dcval->local_name(), NULL),
				 FE_Declarator::FD_complex,
				 tao_yyvsp[0].dcval);
	}
break;
case 180:
#line 1321 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_long;
	}
break;
case 181:
#line 1325 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_longlong;
        }
break;
case 182:
#line 1329 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_short;
	}
break;
case 183:
#line 1336 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_ulong;
	}
break;
case 184:
#line 1340 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_ulonglong;
        }
break;
case 185:
#line 1344 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_ushort;
	}
break;
case 186:
#line 1351 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_double;
	}
break;
case 187:
#line 1355 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_float;
	}
break;
case 188:
#line 1359 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_longdouble;
        }
break;
case 189:
#line 1366 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_char;
	}
break;
case 190:
#line 1370 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_wchar;
        }
break;
case 191:
#line 1377 "fe/idl.tao_yy"
{
          tao_yyval.etval = AST_Expression::EV_octet;
	}
break;
case 192:
#line 1384 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_bool;
        }
break;
case 193:
#line 1391 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_any;
	}
break;
case 194:
#line 1398 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructSeen);
        }
break;
case 195:
#line 1402 "fe/idl.tao_yy"
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
case 196:
#line 1426 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructSqSeen);
        }
break;
case 197:
#line 1430 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructBodySeen);
        }
break;
case 198:
#line 1434 "fe/idl.tao_yy"
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
case 202:
#line 1458 "fe/idl.tao_yy"
{ 
          /* is $0 to member_i */
          tao_yyval.vival = AST_Field::vis_NA;
        }
break;
case 204:
#line 1467 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberTypeSeen);
        }
break;
case 205:
#line 1471 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberDeclsSeen);
        }
break;
case 206:
#line 1475 "fe/idl.tao_yy"
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
case 207:
#line 1509 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 208:
#line 1513 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 209:
#line 1521 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionSeen);
        }
break;
case 210:
#line 1525 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionIDSeen);
        }
break;
case 211:
#line 1529 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchSeen);
        }
break;
case 212:
#line 1533 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchOpenParSeen);
        }
break;
case 213:
#line 1537 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchTypeSeen);
        }
break;
case 214:
#line 1541 "fe/idl.tao_yy"
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
case 215:
#line 1570 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionSqSeen);
        }
break;
case 216:
#line 1574 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionBodySeen);
        }
break;
case 217:
#line 1578 "fe/idl.tao_yy"
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
case 218:
#line 1596 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 219:
#line 1600 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 220:
#line 1604 "fe/idl.tao_yy"
{
          /* octets are not allowed*/
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
	}
break;
case 221:
#line 1609 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 223:
#line 1614 "fe/idl.tao_yy"
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
case 227:
#line 1693 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionLabelSeen);
        }
break;
case 228:
#line 1697 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemSeen);
        }
break;
case 229:
#line 1701 "fe/idl.tao_yy"
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
case 230:
#line 1725 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 231:
#line 1730 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 232:
#line 1738 "fe/idl.tao_yy"
{
	  tao_yyval.llval = new UTL_LabelList(tao_yyvsp[-1].ulval, tao_yyvsp[0].llval);
	}
break;
case 233:
#line 1745 "fe/idl.tao_yy"
{
	  if (tao_yyvsp[-1].llval == NULL)
	    tao_yyval.llval = new UTL_LabelList(tao_yyvsp[0].ulval, NULL);
	  else {
	    tao_yyvsp[-1].llval->nconc(new UTL_LabelList(tao_yyvsp[0].ulval, NULL));
	    tao_yyval.llval = tao_yyvsp[-1].llval;
	  }
	}
break;
case 234:
#line 1754 "fe/idl.tao_yy"
{
	  tao_yyval.llval = NULL;
	}
break;
case 235:
#line 1761 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DefaultSeen);
        }
break;
case 236:
#line 1765 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

	  tao_yyval.ulval = idl_global->gen()->
	            create_union_label(AST_UnionLabel::UL_default,
				       NULL);
	}
break;
case 237:
#line 1773 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_CaseSeen);
        }
break;
case 238:
#line 1777 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelExprSeen);
        }
break;
case 239:
#line 1781 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

	  tao_yyval.ulval = idl_global->gen()->create_union_label(AST_UnionLabel::UL_label,
						     tao_yyvsp[-2].exval);
	}
break;
case 240:
#line 1791 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemTypeSeen);
        }
break;
case 241:
#line 1795 "fe/idl.tao_yy"
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
case 242:
#line 1821 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumSeen);
        }
break;
case 243:
#line 1825 "fe/idl.tao_yy"
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
case 244:
#line 1852 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumSqSeen);
        }
break;
case 245:
#line 1856 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumBodySeen);
        }
break;
case 246:
#line 1860 "fe/idl.tao_yy"
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
case 248:
#line 1882 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumCommaSeen);
        }
break;
case 251:
#line 1891 "fe/idl.tao_yy"
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
case 252:
#line 1915 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceCommaSeen);
        }
break;
case 253:
#line 1919 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceExprSeen);
        }
break;
case 254:
#line 1923 "fe/idl.tao_yy"
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
case 255:
#line 1954 "fe/idl.tao_yy"
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
case 256:
#line 1987 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSeen);
	  /*
	   * Push a sequence marker on scopes stack
	   */
	  idl_global->scopes()->push(NULL);
	}
break;
case 257:
#line 1995 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSqSeen);
        }
break;
case 258:
#line 1999 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceTypeSeen);
	  tao_yyval.dcval = tao_yyvsp[0].dcval;
        }
break;
case 259:
#line 2008 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        }
break;
case 260:
#line 2012 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        }
break;
case 261:
#line 2016 "fe/idl.tao_yy"
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
case 262:
#line 2034 "fe/idl.tao_yy"
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
case 263:
#line 2052 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        }
break;
case 264:
#line 2060 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        }
break;
case 265:
#line 2064 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        }
break;
case 266:
#line 2068 "fe/idl.tao_yy"
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
case 267:
#line 2086 "fe/idl.tao_yy"
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
case 268:
#line 2104 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        }
break;
case 269:
#line 2111 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ArrayIDSeen);
        }
break;
case 270:
#line 2115 "fe/idl.tao_yy"
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
case 271:
#line 2129 "fe/idl.tao_yy"
{
	  tao_yyval.elval = new UTL_ExprList(tao_yyvsp[-1].exval, tao_yyvsp[0].elval);
	}
break;
case 272:
#line 2136 "fe/idl.tao_yy"
{
	  if (tao_yyvsp[-1].elval == NULL)
	    tao_yyval.elval = new UTL_ExprList(tao_yyvsp[0].exval, NULL);
	  else {
	    tao_yyvsp[-1].elval->nconc(new UTL_ExprList(tao_yyvsp[0].exval, NULL));
	    tao_yyval.elval = tao_yyvsp[-1].elval;
	  }
	}
break;
case 273:
#line 2145 "fe/idl.tao_yy"
{
	  tao_yyval.elval = NULL;
	}
break;
case 274:
#line 2152 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimSqSeen);
        }
break;
case 275:
#line 2156 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimExprSeen);
        }
break;
case 276:
#line 2160 "fe/idl.tao_yy"
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
case 277:
#line 2177 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrSeen);
        }
break;
case 278:
#line 2181 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrTypeSeen);
        }
break;
case 279:
#line 2185 "fe/idl.tao_yy"
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
          idl_global->set_pragmas (p);
	}
break;
case 280:
#line 2220 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrROSeen);
	  tao_yyval.bval = I_TRUE;
	}
break;
case 281:
#line 2225 "fe/idl.tao_yy"
{
	  tao_yyval.bval = I_FALSE;
	}
break;
case 282:
#line 2232 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSeen);
	}
break;
case 283:
#line 2236 "fe/idl.tao_yy"
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
case 284:
#line 2260 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSqSeen);
        }
break;
case 285:
#line 2264 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptBodySeen);
        }
break;
case 286:
#line 2268 "fe/idl.tao_yy"
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
case 287:
#line 2284 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpTypeSeen);
        }
break;
case 288:
#line 2288 "fe/idl.tao_yy"
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
case 289:
#line 2317 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParsCompleted);
        }
break;
case 290:
#line 2321 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseCompleted);
        }
break;
case 291:
#line 2325 "fe/idl.tao_yy"
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
case 292:
#line 2350 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
	  tao_yyval.ofval = AST_Operation::OP_oneway;
	}
break;
case 293:
#line 2355 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
	  tao_yyval.ofval = AST_Operation::OP_idempotent;
	}
break;
case 294:
#line 2360 "fe/idl.tao_yy"
{
	  tao_yyval.ofval = AST_Operation::OP_noflags;
	}
break;
case 296:
#line 2368 "fe/idl.tao_yy"
{
	  tao_yyval.dcval =
	    idl_global->scopes()->bottom()
	       ->lookup_primitive_type(AST_Expression::EV_void);
	}
break;
case 297:
#line 2377 "fe/idl.tao_yy"
{
           cerr << "error in " << idl_global->filename()->get_string()
		<< " line " << idl_global->lineno() << ":\n" ;
           cerr << "Sorry, I (TAO_IDL) can't handle init yet\n";
        }
break;
case 298:
#line 2387 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        }
break;
case 299:
#line 2391 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        }
break;
case 300:
#line 2395 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        }
break;
case 301:
#line 2400 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        }
break;
case 303:
#line 2410 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParCommaSeen);
        }
break;
case 306:
#line 2419 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParDirSeen);
        }
break;
case 307:
#line 2423 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParTypeSeen);
        }
break;
case 308:
#line 2427 "fe/idl.tao_yy"
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
case 309:
#line 2449 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 312:
#line 2455 "fe/idl.tao_yy"
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
case 313:
#line 2469 "fe/idl.tao_yy"
{
	  tao_yyval.dival = AST_Argument::dir_IN;
	}
break;
case 314:
#line 2473 "fe/idl.tao_yy"
{
	  tao_yyval.dival = AST_Argument::dir_OUT;
	}
break;
case 315:
#line 2477 "fe/idl.tao_yy"
{
	  tao_yyval.dival = AST_Argument::dir_INOUT;
	}
break;
case 316:
#line 2484 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSeen);
        }
break;
case 317:
#line 2488 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSqSeen);
        }
break;
case 318:
#line 2493 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseQsSeen);
	  tao_yyval.nlval = tao_yyvsp[-1].nlval;
	}
break;
case 319:
#line 2498 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = NULL;
	}
break;
case 320:
#line 2505 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSeen);
        }
break;
case 321:
#line 2509 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSqSeen);
        }
break;
case 322:
#line 2514 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextQsSeen);
	  tao_yyval.slval = tao_yyvsp[-1].slval;
	}
break;
case 323:
#line 2519 "fe/idl.tao_yy"
{
	  tao_yyval.slval = NULL;
	}
break;
case 324:
#line 2526 "fe/idl.tao_yy"
{
	  tao_yyval.slval = new UTL_StrList(tao_yyvsp[-1].sval, tao_yyvsp[0].slval);
	}
break;
case 325:
#line 2534 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextCommaSeen);
        }
break;
case 326:
#line 2538 "fe/idl.tao_yy"
{
	  if (tao_yyvsp[-3].slval == NULL)
	    tao_yyval.slval = new UTL_StrList(tao_yyvsp[0].sval, NULL);
	  else {
	    tao_yyvsp[-3].slval->nconc(new UTL_StrList(tao_yyvsp[0].sval, NULL));
	    tao_yyval.slval = tao_yyvsp[-3].slval;
	  }
	}
break;
case 327:
#line 2547 "fe/idl.tao_yy"
{
	  tao_yyval.slval = NULL;
	}
break;
#line 3674 "y.tab.cpp"
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
