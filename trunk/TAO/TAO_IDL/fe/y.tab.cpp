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
#define IDL_ABSTRACT 294
#define IDL_CUSTOM 295
#define IDL_INIT 296
#define IDL_PRIVATE 297
#define IDL_PUBLIC 298
#define IDL_SUPPORTS 299
#define IDL_TRUNCATABLE 300
#define IDL_VALUETYPE 301
#define IDL_INTEGER_LITERAL 302
#define IDL_UINTEGER_LITERAL 303
#define IDL_STRING_LITERAL 304
#define IDL_CHARACTER_LITERAL 305
#define IDL_FLOATING_PT_LITERAL 306
#define IDL_TRUETOK 307
#define IDL_FALSETOK 308
#define IDL_SCOPE_DELIMITOR 309
#define IDL_LEFT_SHIFT 310
#define IDL_RIGHT_SHIFT 311
#define IDL_WCHAR_LITERAL 312
#define IDL_WSTRING_LITERAL 313
#define TAO_YYERRCODE 256
short tao_yylhs[] = {                                        -1,
    0,   72,   72,   74,   73,   76,   73,   78,   73,   80,
   73,   82,   73,   84,   73,   85,   73,   86,   87,   88,
   89,   81,   79,   79,   92,   94,   95,   90,   96,   68,
   33,   97,   22,   22,   83,   83,   83,   83,   98,   98,
  103,  105,  106,  102,  107,  108,  109,   99,   34,  110,
   69,   67,   67,   24,   24,  100,  100,  101,  104,  104,
  111,  111,  111,  116,  112,  117,  112,   93,   93,  118,
  113,  119,  113,  120,  113,  122,  113,  124,  113,  125,
  113,   20,  126,   21,   21,   16,  127,   16,  128,   16,
   70,   91,  129,  130,  131,  132,   75,   50,   50,   50,
   50,   50,   50,   50,   50,   50,   35,   36,   37,   37,
   38,   38,   39,   39,   40,   40,   40,   41,   41,   41,
   42,   42,   42,   42,   43,   43,   43,   43,   44,   44,
   44,   45,   45,   45,   45,   45,   45,   45,   45,   45,
   46,  134,   71,   71,   71,   71,   71,  135,  133,    1,
    1,    2,    2,    2,   59,   59,   59,   59,   59,   59,
   59,    4,    4,    4,    3,    3,    3,   29,  136,   30,
   30,   63,   63,   31,  137,   32,   32,   64,   65,   51,
   51,   57,   57,   57,   58,   58,   58,   54,   54,   54,
   56,   52,   52,   60,   53,   55,  138,  139,  141,  142,
    7,  140,  140,  145,  140,  144,  144,  146,  144,  147,
  143,  148,  149,  115,  150,  115,  151,  152,  153,  154,
  155,  156,  158,  159,   10,    9,    9,    9,    9,    9,
    9,  157,  161,  161,  162,  163,  160,  164,  160,   27,
   28,   28,  165,   48,  166,  167,   48,  168,   49,  169,
  170,  172,  173,    8,  171,  176,  175,  175,  174,  177,
  178,    5,    5,  179,  180,   13,  182,  183,    6,    6,
  181,  185,  186,   14,   14,  184,  187,   11,   25,   26,
   26,  188,  189,   47,  190,  191,  121,   66,   66,  192,
  193,  194,  195,   77,  196,  198,  199,  200,  123,   62,
   62,   62,   12,   12,  114,  201,  197,  203,  197,  202,
  206,  205,  205,  207,  208,  204,   15,   15,   15,   15,
   61,   61,   61,  209,  210,   23,   23,  211,  212,   17,
   17,   18,  213,   19,   19,
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
    1,    1,    1,    1,    1,    1,    1,    1,    1,    3,
    1,    3,    1,    3,    1,    3,    3,    1,    3,    3,
    1,    3,    3,    3,    1,    2,    2,    2,    1,    1,
    3,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    0,    3,    1,    1,    1,    2,    0,    3,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    2,    0,    4,
    0,    1,    1,    2,    0,    4,    0,    1,    1,    1,
    1,    1,    2,    1,    2,    3,    2,    1,    1,    2,
    1,    1,    1,    1,    1,    1,    0,    0,    0,    0,
    9,    2,    0,    0,    3,    2,    0,    0,    3,    0,
    2,    0,    0,    5,    0,    3,    0,    0,    0,    0,
    0,    0,    0,    0,   17,    1,    1,    1,    1,    1,
    1,    2,    2,    0,    0,    0,    5,    0,    3,    2,
    2,    0,    0,    3,    0,    0,    5,    0,    3,    0,
    0,    0,    0,    9,    2,    0,    4,    0,    1,    0,
    0,    6,    2,    0,    0,    5,    0,    0,    6,    1,
    1,    0,    0,    6,    1,    1,    0,    3,    2,    2,
    0,    0,    0,    5,    0,    0,    6,    1,    0,    0,
    0,    0,    0,    9,    0,    0,    0,    0,   10,    1,
    1,    0,    1,    1,    1,    0,    3,    0,    4,    2,
    0,    4,    0,    0,    0,    5,    1,    1,    1,    1,
    1,    1,    1,    0,    0,    6,    0,    0,    0,    6,
    0,    2,    0,    4,    0,
};
short tao_yydefred[] = {                                      3,
    0,    0,   16,   93,   18,   29,  142,  197,  217,  250,
  290,    0,    0,    0,   50,  144,  146,  145,   25,   41,
    0,    0,    4,    2,    6,    8,   10,   12,   14,   23,
   24,   35,   36,   37,   38,   40,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   91,  147,  178,   45,    0,
    0,   39,    0,    0,    0,   32,   31,    0,  184,    0,
  188,  189,  192,  193,  194,  195,  191,  196,  264,  271,
  276,   52,   87,   58,  150,  151,  153,  162,  163,  165,
  167,  166,    0,  164,    0,  155,  157,  158,  156,  161,
  160,  180,  181,  152,  159,    0,   86,    0,    0,    0,
    0,    0,    0,    0,    0,   17,  104,  105,    0,   94,
   98,   99,  101,  102,  103,  100,   19,   30,  148,  143,
  198,  218,  251,  291,    0,   51,   26,   42,    0,  183,
  190,    0,  187,    0,    0,  260,  263,   89,    0,  267,
  272,    5,    7,    9,   11,   13,   15,    0,    0,    0,
    0,    0,    0,    0,   46,   69,   60,    0,   33,  186,
  265,   88,    0,    0,    0,   49,    0,    0,   95,   20,
  179,  149,  171,  172,  173,    0,  199,  219,  252,  292,
   69,    0,    0,    0,    0,  132,  133,  134,  136,  138,
  139,  140,  137,  135,    0,    0,    0,    0,    0,  141,
    0,    0,    0,    0,    0,    0,  121,  125,  130,  261,
   90,    0,  268,  273,    0,    3,    0,    0,    0,    0,
    0,    0,    0,   80,  288,  300,  301,    0,    0,   70,
   72,   74,    0,   68,   76,   78,  305,   66,   64,    0,
   59,   61,   62,   63,   83,  266,  126,  127,  128,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   96,    0,  169,  282,  278,  281,  204,
  200,    0,    0,  220,  259,  253,  258,  208,    0,    0,
    0,  304,  318,  295,  319,  303,    0,  317,  285,    0,
    0,    0,   28,    0,    0,    0,    0,   44,    0,  131,
    0,    0,    0,    0,    0,    0,    0,  122,  123,  124,
  262,  269,  274,    0,    0,    0,    0,    0,    0,    0,
    0,  215,  212,  211,    0,    0,    0,    0,  206,    0,
   48,   81,    0,    0,   71,   73,   75,   77,   79,   67,
   65,    0,   97,  107,   22,  170,  283,  280,  205,  201,
    0,    0,    0,  230,  221,    0,  226,  227,  229,  228,
  254,  256,  209,  294,  296,  286,    0,  216,  213,    0,
    0,    0,    0,  284,    0,  222,  257,    0,  297,  287,
  177,  214,    0,    0,    0,    0,    0,  223,  307,  321,
  322,  323,  314,    0,  313,  324,  298,  175,    0,    0,
  309,    0,    0,    0,    0,  238,  245,  243,  235,  242,
  224,  234,  315,  311,  325,  328,  299,  176,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  239,
  246,  244,  248,  236,  241,  225,  233,  316,  312,    0,
  329,    0,    0,    0,  326,    0,  247,  249,  237,  335,
    0,    0,  330,  333,    0,  334,
};
short tao_yydgoto[] = {                                       1,
  323,   75,   76,   77,   78,   79,   80,   81,  355,   82,
  171,  284,   83,   84,  286,  199,  417,  451,  452,  159,
  184,   57,  397,  166,  268,  318,  409,  423,  172,  217,
  380,  387,   19,   20,  343,  200,  201,  202,  203,  204,
  205,  206,  207,  208,  209,  210,  269,  410,  434,  110,
   86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
  393,  228,  173,  174,  175,  229,   96,   21,   22,   97,
  230,    2,   24,  100,  231,  101,  232,  102,   27,  103,
   28,  104,   29,  105,   37,   39,  149,  216,  315,   30,
   31,   54,  182,  156,  233,   40,  129,   32,   33,   34,
   35,   36,   55,  183,  157,  240,  125,  181,  280,   53,
  241,  242,  234,  244,  324,  297,  296,  290,  291,  292,
  235,  294,  236,  295,  281,  299,  135,  164,   38,  148,
  215,  314,  120,   41,  150,  316,  405,   42,  151,  271,
  219,  320,  329,  279,  319,  328,  273,  352,  375,  351,
   43,  152,  220,  325,  370,  383,  411,  399,  424,  412,
  425,  422,  444,  419,  421,  420,  442,  443,   44,  153,
  276,  221,  326,  277,  327,  371,  163,  261,  134,  185,
   98,  167,  262,   99,  168,  263,  218,  317,  367,  334,
  373,   45,  154,  222,  330,  333,  379,  372,  386,  404,
  384,  394,  385,  395,  402,  427,  400,  426,  403,  428,
  429,  446,  455,
};
short tao_yysindex[] = {                                      0,
    0,  583,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -231, -254, -254,    0,    0,    0,    0,    0,    0,
   16,  407,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -10,  559, -186, -231,
  455, -231, -231, -231, -231,    0,    0,    0,    0, -224,
 -224,    0, -231,  -43,  -41,    0,    0, -205,    0, -226,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -19,    0, -206,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   16,    0,   55,   57,   65,
   71,   73,   94,   96,  100,    0,    0,    0, -206,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   51,    0,    0,    0, -184,    0,
    0,  -79,    0,  125, -231,    0,    0,    0, -109,    0,
    0,    0,    0,    0,    0,    0,    0, -231,   86, -231,
   90,  -52,  103,  105,    0,    0,    0, -206,    0,    0,
    0,    0,   18, -231, -184,    0,   18,   18,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  632,  606,  191,  491,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   30,   30,   30,   18, -206,    0,
  112,  154,  213, -191,   19,  124,    0,    0,    0,    0,
    0, -206,    0,    0,  193,    0,  208,  169,   10,  225,
   14,   11,  632,    0,    0,    0,    0,  509,  -14,    0,
    0,    0,  147,    0,    0,    0,    0,    0,    0,  148,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  242,
   18,   18,   18,   18,   18,   18,   18,   18,   18,   18,
  222,  226,  227,    0,  583,    0,    0,    0,    0,    0,
    0,   11,  383,    0,    0,    0,    0,    0,    0,  165,
  232,    0,    0,    0,    0,    0, -206,    0,    0,  234,
  243,  244,    0,  247,  250,  383,  383,    0, -184,    0,
  154,  213, -191,   19,   19,  124,  124,    0,    0,    0,
    0,    0,    0,   18,  185, -231,   18,  169,  254,  190,
    0,    0,    0,    0,  158,  194,  272,  282,    0,  203,
    0,    0,   91,  541,    0,    0,    0,    0,    0,    0,
    0, -206,    0,    0,    0,    0,    0,    0,    0,    0,
  288, -231,   87,    0,    0, -206,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  261,    0,    0,  314,
   14,  322, -231,    0,  304,    0,    0,    0,    0,    0,
    0,    0,  249,  324,  -49,   83,  331,    0,    0,    0,
    0,    0,    0,  335,    0,    0,    0,    0, -119,  541,
    0,  338,  339,   92, -231,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  326,   18,
  325,  455, -133,  262, -119, -231,  -49, -184,  346,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  349,
    0,  333, -231,  336,    0,   99,    0,    0,    0,    0,
  359,  360,    0,    0,  101,    0,
};
short tao_yyrindex[] = {                                      0,
    0,  406,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   -5,  -35,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   54,
   -7,    0,    0,    0,    0,    0,    0,  140,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  152,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -110,    0,  156,  160,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  150,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  116,    0,    0,    0,    0,    0,    0,  287,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   53,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  110,    0,    0,    0,    0,
    0,  293,  328,   12,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   -3,    0,
  319,  175,  162,  330,   46,    7,    0,    0,    0,    0,
    0,  296,    0,    0,    0,    0,  352,    0,  245,    0,
    0,  269,  354,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  291,    0,    0,    0,    0,    0,
    0,  269,    0,    0,    0,    0,    0,    0,  173,    0,
    0,    0,    0,    0,    0,    0,  176,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  188,  572,  511,   48,   88,   34,   40,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  166,    0,    0,
  221,    0,    0,    0,    0,    0,  307,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   70,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  414,    0,    0,  415,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  -27,    0,    0,
    0,    0,    0,    0,    0,  -47,  389,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  416,    0,  399,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  473,    0,  334,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  419,    0,    0,    0,    0,
};
short tao_yygindex[] = {                                      0,
  -21,  276,    0,    0,    0,  -34,    9,    4,    0,   15,
    0,    0,    0,  -33, -305,  -20,    0,    0,    0,   35,
    0,  366,    0,    0,    0,    0,    0,    0,  113,    0,
    0,    0,    0,  451,    0, -171,    0,  215,  216,  217,
  -87,  -77,  -16,   62,    0, -100,  151,   49,    0,    0,
  -31,  -30,  -29,  433,    0,  435,    0,    0, -212,  -28,
    0,    0, -270,   -9,    0,    0,    0,    0,  192,  -12,
   13,  258,    0,    0,   17,    0,   20,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  294,    0,    0,    0,    0,    0,    0,    0,
    0,  462,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  297,    0,  -66,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  277,  209,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   76,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  132,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   77,    0,    0,    0,    0,    0,    0,
    0,    0,    0,
};
#define TAO_YYTABLESIZE 925
short tao_yytable[] = {                                      48,
   74,   85,   47,  107,  108,   17,  111,  112,  113,  116,
   16,  327,   34,  306,   23,  288,   18,  109,   25,  119,
   85,   26,   53,   57,  136,   46,  250,  118,  366,  121,
  122,  123,  124,  129,  129,  132,  133,  129,  129,  129,
  126,  129,  137,  129,  118,  346,   15,  118,  106,  118,
   53,  118,   82,   92,  129,  129,  130,  198,  129,  131,
  195,  256,  196,  257,  118,  118,  213,  214,  118,  198,
  117,  119,   46,   56,  119,   72,  119,  120,  119,  127,
  120,  128,  120,  115,  120,  116,  115,   53,  116,  129,
  129,  119,  119,   85,  413,  119,   85,  120,  120,  118,
  118,  120,  138,  115,  115,  116,  116,  115,  158,  116,
   84,   53,   56,   84,  140,   53,  141,   34,  254,  255,
  129,  288,  162,  142,   73,  117,  119,  119,  117,  143,
  118,  144,  120,  120,   82,  169,  406,  176,  115,  115,
  116,  116,  344,  197,  212,  117,  117,  407,  408,  117,
  178,  211,  145,  178,  146,  438,  185,  119,  147,  185,
  260,  407,  408,  120,   85,  258,  304,  305,  178,  115,
  259,  116,  448,  155,  185,   85,   53,  185,  306,  307,
  117,  117,  160,  182,  161,   17,   17,  288,   34,  165,
   16,   16,   84,  283,  285,  154,   18,   18,  182,  270,
  277,  182,  111,  275,   50,   51,  279,  287,  170,  279,
  154,  117,  177,  154,  270,  109,  347,  270,  275,  111,
  111,  275,  178,  111,  279,  179,   17,  180,  110,  340,
  341,   16,  109,  109,  245,  251,  109,   18,  390,  391,
  392,  308,  309,  310,  327,  110,  110,  252,  431,  110,
  253,  266,   85,  264,  111,  111,  247,  248,  249,  267,
  308,  308,  308,   53,  274,  270,  278,  109,   17,  289,
  275,  293,  298,   16,   46,   85,   85,   23,  342,   18,
  110,   25,  300,  311,   26,  111,   46,  312,  313,  331,
  332,   53,  335,  357,  358,  359,  360,  293,  109,  283,
  285,  336,  337,  176,  356,  338,  129,  129,  339,  345,
   82,  110,  349,  287,  350,  362,  118,  118,  361,  186,
  187,  188,  189,  190,  191,  192,   73,  364,  354,  193,
  194,  186,  187,  188,  189,  190,  191,  192,   73,  176,
  363,  193,  194,  119,  119,  202,  368,  365,  130,  120,
  120,   85,   53,  374,  376,  115,  115,  116,  116,  108,
   48,  378,  382,  381,  389,  283,  285,  113,   84,  203,
  113,  388,  185,  396,  398,  401,  108,  108,  415,  287,
  108,  414,  432,  416,  430,  441,  436,  113,  113,  445,
  447,  113,   48,  207,  449,  418,  182,  117,  117,  453,
  433,   85,  450,  454,  456,    1,  106,  158,  154,   55,
  168,  108,  270,  176,   46,   21,  275,   27,   54,  353,
   59,   60,  113,  113,   63,   64,   65,   66,  210,  210,
  176,  255,  320,   10,  210,  210,  210,  210,  210,  210,
  210,  210,  210,  210,  210,  210,  210,  174,  210,  210,
  210,  210,   43,  113,  182,  231,  310,  331,  232,  332,
  246,  139,  440,   49,  369,  301,   73,  302,  348,  303,
  114,  435,  115,  265,  223,   52,  210,  210,   47,  243,
  321,  210,  210,  210,  210,  210,  210,  210,  210,  210,
  210,  210,  210,  210,  210,  272,  210,  210,  210,  210,
  437,  210,  377,  439,    0,    0,  210,  210,  210,  210,
  210,  210,  210,  210,  210,  210,  210,  210,  210,    0,
  210,  210,  210,  210,    0,  207,    0,    0,    0,  210,
  207,  207,  207,  207,  207,  207,  207,  207,  207,  207,
  207,  207,  207,    0,  207,  207,  207,  207,  114,  302,
    0,  114,    0,  210,  302,  302,  302,  302,  302,  302,
  302,  302,  302,  302,  302,    0,    0,    0,  114,  114,
  302,  302,  114,    0,    0,    0,  289,  207,    0,  302,
    0,    0,    0,    0,  302,    0,    0,    0,    0,  302,
  302,  302,  302,  302,  302,  302,  302,  302,  302,  302,
    0,  302,    0,  114,  114,  302,  302,    0,    0,    0,
  302,  289,  112,    0,  302,  302,  302,  302,  302,  302,
  302,  302,  302,  302,  302,  302,    0,    0,    0,  112,
  112,  302,  302,  112,  114,    0,  302,  289,  322,   46,
  302,    0,    0,    0,   58,   59,   60,   61,   62,   63,
   64,   65,   66,   67,   68,    8,    9,    0,   10,   69,
   70,   71,  302,   46,  112,  112,    0,    0,   58,   59,
   60,   61,   62,   63,   64,   65,   66,   67,   68,    8,
    9,    0,   10,   69,   70,   71,    0,    0,    0,    0,
    0,   73,    0,    0,    0,  112,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   72,    0,    0,    0,
    0,   46,    0,    0,    0,   73,   58,   59,   60,   61,
   62,   63,   64,   65,   66,   67,   68,    8,    9,  240,
   10,   69,   70,   71,  240,  240,  240,  240,  240,  240,
  240,  240,  240,  240,  240,  240,  240,   46,  240,  240,
  240,  240,   58,   59,   60,   61,   62,   63,   64,   65,
   66,   67,   68,   73,    0,   46,    0,   69,   70,   71,
   58,   59,   60,   61,   62,   63,   64,   65,   66,   67,
   68,  240,    0,    0,    0,    0,   70,   71,    0,    0,
    0,    0,    0,    0,    0,  282,    0,   46,    0,   73,
    0,    0,   58,   59,   60,   61,   62,   63,   64,   65,
   66,   67,   68,    0,    0,   46,    0,   73,   70,   71,
   58,   59,   60,   61,   62,   63,   64,   65,   66,   67,
    0,    0,    0,    0,    0,    0,   70,   71,    3,    0,
    4,    5,    6,    7,    0,    0,    0,    0,    0,   73,
    0,    0,    0,    0,    0,    8,    9,    0,   10,    0,
    0,  224,   11,    4,    0,    0,    7,   73,    0,    0,
    0,    0,    0,    0,    0,   12,   13,   14,    8,    9,
    0,   10,    0,   15,    0,   11,    0,  224,  225,    4,
  226,  227,    7,    0,    0,    0,    0,    0,   12,    0,
    0,  237,  238,  239,    8,    9,    0,   10,    0,    0,
    0,   11,    0,    0,  225,    0,  226,  227,    0,    0,
    0,    0,    0,    0,   12,
};
short tao_yycheck[] = {                                      12,
   22,   22,   12,   38,   38,    2,   38,   38,   38,   38,
    2,   59,  123,   41,    2,  228,    2,   38,    2,   41,
   41,    2,   58,   59,   44,  257,  198,   40,  334,   42,
   43,   44,   45,   37,   38,  262,  263,   41,   42,   43,
   53,   45,   62,   47,   38,  316,  301,   41,   59,   43,
   58,   45,   41,   59,   58,   59,  262,   40,   62,  265,
   43,   43,   45,   45,   58,   59,  167,  168,   62,   40,
  257,   38,  257,   58,   41,  300,   43,   38,   45,  123,
   41,  123,   43,   38,   45,   38,   41,  123,   41,   93,
   94,   58,   59,   41,  400,   62,   44,   58,   59,   93,
   94,   62,  309,   58,   59,   58,   59,   62,  129,   62,
   41,   58,   59,   44,   60,  123,   60,  123,  310,  311,
  124,  334,  135,   59,  309,   38,   93,   94,   41,   59,
  124,   59,   93,   94,  123,  148,  256,  150,   93,   94,
   93,   94,  314,  126,  165,   58,   59,  281,  282,   62,
   41,  164,   59,   44,   59,  426,   41,  124,   59,   44,
   37,  281,  282,  124,  185,   42,  254,  255,   59,  124,
   47,  124,  443,  123,   59,  123,  123,   62,  256,  257,
   93,   94,  262,   44,   60,  182,  183,  400,  299,  299,
  182,  183,  123,  228,  228,   44,  182,  183,   59,   44,
   91,   62,   41,   44,   13,   14,   41,  228,  123,   44,
   59,  124,  123,   62,   59,   41,  317,   62,   59,   58,
   59,   62,  275,   62,   59,  123,  223,  123,   41,  296,
  297,  223,   58,   59,   44,  124,   62,  223,  288,  289,
  290,  258,  259,  260,  292,   58,   59,   94,  420,   62,
   38,   44,  273,   61,   93,   94,  195,  196,  197,   91,
  288,  289,  290,  299,   40,  256,  256,   93,  265,  284,
  257,  125,  125,  265,  257,  296,  297,  265,  299,  265,
   93,  265,   41,   62,  265,  124,  257,   62,   62,  125,
   59,  299,   59,  325,  325,  325,  325,  125,  124,  334,
  334,   59,   59,  316,  325,   59,  310,  311,   59,  125,
  299,  124,   59,  334,  125,   44,  310,  311,  125,  302,
  303,  304,  305,  306,  307,  308,  309,  125,  325,  312,
  313,  302,  303,  304,  305,  306,  307,  308,  309,  352,
   59,  312,  313,  310,  311,  125,   59,  257,  262,  310,
  311,  299,  299,   93,   41,  310,  311,  310,  311,   41,
  373,   40,   59,  373,   41,  400,  400,   38,  299,  125,
   41,  123,  257,  291,   44,   41,   58,   59,   40,  400,
   62,   44,   58,  292,   59,   40,  125,   58,   59,   41,
   58,   62,  405,  125,   59,  405,  257,  310,  311,   41,
  422,  422,  304,   44,  304,    0,  257,  428,  257,  123,
   59,   93,  257,  426,  257,  125,  257,  125,  123,  262,
  263,  264,   93,   94,  267,  268,  269,  270,  256,  257,
  443,  125,  257,  276,  262,  263,  264,  265,  266,  267,
  268,  269,  270,  271,  272,  273,  274,   59,  276,  277,
  278,  279,  125,  124,   41,   41,   41,   59,  125,   41,
  185,   96,  428,   13,  352,  251,  309,  252,  318,  253,
   38,  423,   38,  216,  181,   14,  256,  257,  125,  183,
  272,  309,  262,  263,  264,  265,  266,  267,  268,  269,
  270,  271,  272,  273,  274,  219,  276,  277,  278,  279,
  425,  257,  371,  427,   -1,   -1,  262,  263,  264,  265,
  266,  267,  268,  269,  270,  271,  272,  273,  274,   -1,
  276,  277,  278,  279,   -1,  257,   -1,   -1,   -1,  309,
  262,  263,  264,  265,  266,  267,  268,  269,  270,  271,
  272,  273,  274,   -1,  276,  277,  278,  279,   38,  257,
   -1,   41,   -1,  309,  262,  263,  264,  265,  266,  267,
  268,  269,  270,  271,  272,   -1,   -1,   -1,   58,   59,
  278,  279,   62,   -1,   -1,   -1,  284,  309,   -1,  287,
   -1,   -1,   -1,   -1,  257,   -1,   -1,   -1,   -1,  262,
  263,  264,  265,  266,  267,  268,  269,  270,  271,  272,
   -1,  309,   -1,   93,   94,  278,  279,   -1,   -1,   -1,
  257,  284,   41,   -1,  287,  262,  263,  264,  265,  266,
  267,  268,  269,  270,  271,  272,   -1,   -1,   -1,   58,
   59,  278,  279,   62,  124,   -1,  309,  284,  256,  257,
  287,   -1,   -1,   -1,  262,  263,  264,  265,  266,  267,
  268,  269,  270,  271,  272,  273,  274,   -1,  276,  277,
  278,  279,  309,  257,   93,   94,   -1,   -1,  262,  263,
  264,  265,  266,  267,  268,  269,  270,  271,  272,  273,
  274,   -1,  276,  277,  278,  279,   -1,   -1,   -1,   -1,
   -1,  309,   -1,   -1,   -1,  124,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  300,   -1,   -1,   -1,
   -1,  257,   -1,   -1,   -1,  309,  262,  263,  264,  265,
  266,  267,  268,  269,  270,  271,  272,  273,  274,  257,
  276,  277,  278,  279,  262,  263,  264,  265,  266,  267,
  268,  269,  270,  271,  272,  273,  274,  257,  276,  277,
  278,  279,  262,  263,  264,  265,  266,  267,  268,  269,
  270,  271,  272,  309,   -1,  257,   -1,  277,  278,  279,
  262,  263,  264,  265,  266,  267,  268,  269,  270,  271,
  272,  309,   -1,   -1,   -1,   -1,  278,  279,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  287,   -1,  257,   -1,  309,
   -1,   -1,  262,  263,  264,  265,  266,  267,  268,  269,
  270,  271,  272,   -1,   -1,  257,   -1,  309,  278,  279,
  262,  263,  264,  265,  266,  267,  268,  269,  270,  271,
   -1,   -1,   -1,   -1,   -1,   -1,  278,  279,  256,   -1,
  258,  259,  260,  261,   -1,   -1,   -1,   -1,   -1,  309,
   -1,   -1,   -1,   -1,   -1,  273,  274,   -1,  276,   -1,
   -1,  256,  280,  258,   -1,   -1,  261,  309,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,  293,  294,  295,  273,  274,
   -1,  276,   -1,  301,   -1,  280,   -1,  256,  283,  258,
  285,  286,  261,   -1,   -1,   -1,   -1,   -1,  293,   -1,
   -1,  296,  297,  298,  273,  274,   -1,  276,   -1,   -1,
   -1,  280,   -1,   -1,  283,   -1,  285,  286,   -1,   -1,
   -1,   -1,   -1,   -1,  293,
};
#define TAO_YYFINAL 1
#ifndef TAO_YYDEBUG
#define TAO_YYDEBUG 0
#endif
#define TAO_YYMAXTOKEN 313
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
"IDL_NATIVE","IDL_ABSTRACT","IDL_CUSTOM","IDL_INIT","IDL_PRIVATE","IDL_PUBLIC",
"IDL_SUPPORTS","IDL_TRUNCATABLE","IDL_VALUETYPE","IDL_INTEGER_LITERAL",
"IDL_UINTEGER_LITERAL","IDL_STRING_LITERAL","IDL_CHARACTER_LITERAL",
"IDL_FLOATING_PT_LITERAL","IDL_TRUETOK","IDL_FALSETOK","IDL_SCOPE_DELIMITOR",
"IDL_LEFT_SHIFT","IDL_RIGHT_SHIFT","IDL_WCHAR_LITERAL","IDL_WSTRING_LITERAL",
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
#line 2605 "fe/idl.tao_yy"
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
#line 977 "y.tab.cpp"
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
#line 259 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        }
break;
case 5:
#line 263 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 6:
#line 267 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        }
break;
case 7:
#line 271 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 8:
#line 275 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        }
break;
case 9:
#line 279 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 10:
#line 283 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceDeclSeen);
        }
break;
case 11:
#line 287 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 12:
#line 291 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ModuleDeclSeen);
        }
break;
case 13:
#line 295 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 14:
#line 299 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ValuetypeDeclSeen);
        }
break;
case 15:
#line 303 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 16:
#line 307 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 17:
#line 311 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 18:
#line 318 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSeen);
	  }
break;
case 19:
#line 322 "fe/idl.tao_yy"
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
#line 343 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSqSeen);
	  }
break;
case 21:
#line 347 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_ModuleBodySeen);
	  }
break;
case 22:
#line 351 "fe/idl.tao_yy"
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
#line 373 "fe/idl.tao_yy"
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
                                       p
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
#line 404 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSqSeen);
	}
break;
case 27:
#line 408 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceBodySeen);
	}
break;
case 28:
#line 412 "fe/idl.tao_yy"
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
#line 435 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSeen);
	 }
break;
case 30:
#line 439 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceIDSeen);
	   tao_yyval.idval = tao_yyvsp[0].idval;
	 }
break;
case 31:
#line 447 "fe/idl.tao_yy"
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
#line 461 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InheritColonSeen);
        }
break;
case 33:
#line 465 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = tao_yyvsp[0].nlval;
	}
break;
case 34:
#line 469 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = NULL;
	}
break;
case 39:
#line 483 "fe/idl.tao_yy"
{
           cerr << "error in " << idl_global->filename()->get_string()
		<< " line " << idl_global->lineno() << ":\n" ;
           cerr << "Sorry, I (TAO_IDL) can't handle custom yet\n";
            /* set custom (if not truncatable) */
        }
break;
case 41:
#line 494 "fe/idl.tao_yy"
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
#line 516 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSqSeen);
	}
break;
case 43:
#line 520 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceBodySeen);
	}
break;
case 44:
#line 524 "fe/idl.tao_yy"
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
#line 542 "fe/idl.tao_yy"
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
#line 570 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSqSeen);
	}
break;
case 47:
#line 574 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceBodySeen);
	}
break;
case 48:
#line 578 "fe/idl.tao_yy"
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
#line 599 "fe/idl.tao_yy"
{
          tao_yyval.vhval = new FE_obv_header (new UTL_ScopedName (tao_yyvsp[-3].idval, NULL), tao_yyvsp[-1].nlval, tao_yyvsp[0].nlval);
        }
break;
case 50:
#line 606 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_ValuetypeSeen);
        }
break;
case 51:
#line 610 "fe/idl.tao_yy"
{
           idl_global->set_parse_state(IDL_GlobalData::PS_ValuetypeIDSeen);
	   tao_yyval.idval = tao_yyvsp[0].idval;
        }
break;
case 52:
#line 618 "fe/idl.tao_yy"
{
            cerr << "warning in " << idl_global->filename()->get_string()
	         << " line " << idl_global->lineno() << ":\n" ;
            cerr << "truncatable modifier not supported and is ignored\n";
	    tao_yyval.bval = I_FALSE;
	    /* $$ = I_TRUE; */
	  }
break;
case 53:
#line 626 "fe/idl.tao_yy"
{
	    tao_yyval.bval = I_FALSE;
	  }
break;
case 54:
#line 634 "fe/idl.tao_yy"
{ 
              tao_yyval.nlval = new UTL_NameList(tao_yyvsp[0].idlist, NULL); 
            }
break;
case 55:
#line 638 "fe/idl.tao_yy"
{ 
              tao_yyval.nlval = NULL; 
            }
break;
case 56:
#line 646 "fe/idl.tao_yy"
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
#line 667 "fe/idl.tao_yy"
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
#line 689 "fe/idl.tao_yy"
{
           cerr << "error in " << idl_global->filename()->get_string()
		<< " line " << idl_global->lineno() << ":\n" ;
           cerr << "Sorry, I (TAO_IDL) can't handle boxes yet\n";
        }
break;
case 64:
#line 709 "fe/idl.tao_yy"
{
            /* is $0 to member_i */
            tao_yyval.vival = AST_Field::vis_PUBLIC;
          }
break;
case 66:
#line 715 "fe/idl.tao_yy"
{
            /* is $0 to member_i */
            tao_yyval.vival = AST_Field::vis_PRIVATE;
          }
break;
case 70:
#line 729 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        }
break;
case 71:
#line 733 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 72:
#line 737 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        }
break;
case 73:
#line 741 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 74:
#line 745 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        }
break;
case 75:
#line 749 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 76:
#line 753 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrDeclSeen);
        }
break;
case 77:
#line 757 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 78:
#line 761 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpDeclSeen);
        }
break;
case 79:
#line 765 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 80:
#line 769 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 81:
#line 773 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 82:
#line 781 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = new UTL_NameList(tao_yyvsp[-1].idlist, tao_yyvsp[0].nlval);
	}
break;
case 83:
#line 789 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SNListCommaSeen);
        }
break;
case 84:
#line 793 "fe/idl.tao_yy"
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
#line 804 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = NULL;
	}
break;
case 86:
#line 811 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  tao_yyval.idlist = new UTL_IdList(tao_yyvsp[0].idval, NULL);
	}
break;
case 87:
#line 817 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        }
break;
case 88:
#line 821 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  tao_yyval.idlist = new UTL_IdList(new Identifier(tao_yyvsp[-2].strval, 1, 0, I_FALSE),
			      new UTL_IdList(tao_yyvsp[0].idval, NULL));
	}
break;
case 89:
#line 829 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        }
break;
case 90:
#line 833 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

	  tao_yyvsp[-3].idlist->nconc(new UTL_IdList(tao_yyvsp[0].idval, NULL));
	  tao_yyval.idlist = tao_yyvsp[-3].idlist;
	}
break;
case 91:
#line 842 "fe/idl.tao_yy"
{
            tao_yyval.idval = new Identifier(tao_yyvsp[0].strval, 1, 0, I_FALSE);
        }
break;
case 92:
#line 849 "fe/idl.tao_yy"
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
#line 870 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstSeen);
        }
break;
case 94:
#line 874 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstTypeSeen);
        }
break;
case 95:
#line 878 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstIDSeen);
        }
break;
case 96:
#line 882 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ConstAssignSeen);
        }
break;
case 97:
#line 886 "fe/idl.tao_yy"
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
case 104:
#line 921 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_string;
	}
break;
case 105:
#line 925 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_wstring;
        }
break;
case 106:
#line 929 "fe/idl.tao_yy"
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
case 110:
#line 975 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_or, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 112:
#line 983 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_xor, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 114:
#line 991 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_and, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 116:
#line 999 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_left,tao_yyvsp[-2].exval,tao_yyvsp[0].exval);
	}
break;
case 117:
#line 1003 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_right,tao_yyvsp[-2].exval,tao_yyvsp[0].exval);
	}
break;
case 119:
#line 1011 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_add, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 120:
#line 1015 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_minus,tao_yyvsp[-2].exval,tao_yyvsp[0].exval);
	}
break;
case 122:
#line 1023 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_mul, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 123:
#line 1027 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_div, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 124:
#line 1031 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_mod, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
	}
break;
case 126:
#line 1039 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_u_plus,
					      tao_yyvsp[0].exval,
					      NULL);
	}
break;
case 127:
#line 1045 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_u_minus,
					      tao_yyvsp[0].exval,
					      NULL);
	}
break;
case 128:
#line 1051 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_bit_neg,
					      tao_yyvsp[0].exval,
					      NULL);
	}
break;
case 129:
#line 1060 "fe/idl.tao_yy"
{
	  /*
	   * An expression which is a scoped name is not resolved now,
	   * but only when it is evaluated (such as when it is assigned
	   * as a constant value)
	   */
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].idlist);
	}
break;
case 131:
#line 1070 "fe/idl.tao_yy"
{
	  tao_yyval.exval = tao_yyvsp[-1].exval;
	}
break;
case 132:
#line 1077 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].ival);
	}
break;
case 133:
#line 1081 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].uival);
	}
break;
case 134:
#line 1085 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].sval);
	}
break;
case 135:
#line 1089 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].wsval);
	}
break;
case 136:
#line 1093 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].cval);
	}
break;
case 137:
#line 1097 "fe/idl.tao_yy"
{
	  ACE_OutputCDR::from_wchar wc (tao_yyvsp[0].wcval);
	  tao_yyval.exval = idl_global->gen()->create_expr(wc);
	}
break;
case 138:
#line 1102 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].dval);
	}
break;
case 139:
#line 1106 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr((idl_bool) I_TRUE,
					      AST_Expression::EV_bool);
	}
break;
case 140:
#line 1111 "fe/idl.tao_yy"
{
	  tao_yyval.exval = idl_global->gen()->create_expr((idl_bool) I_FALSE,
					      AST_Expression::EV_bool);
	}
break;
case 141:
#line 1119 "fe/idl.tao_yy"
{
	    tao_yyvsp[0].exval->evaluate(AST_Expression::EK_const);
	    tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].exval, AST_Expression::EV_ulong);
	}
break;
case 142:
#line 1127 "fe/idl.tao_yy"
{
	    idl_global->set_parse_state(IDL_GlobalData::PS_TypedefSeen);
	  }
break;
case 143:
#line 1130 "fe/idl.tao_yy"
{tao_yyval.ival = 0;}
break;
case 144:
#line 1131 "fe/idl.tao_yy"
{ tao_yyval.ival = 0;}
break;
case 145:
#line 1132 "fe/idl.tao_yy"
{ tao_yyval.ival = 0;}
break;
case 146:
#line 1133 "fe/idl.tao_yy"
{ tao_yyval.ival = 0;}
break;
case 147:
#line 1135 "fe/idl.tao_yy"
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
case 148:
#line 1161 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_TypeSpecSeen);
        }
break;
case 149:
#line 1165 "fe/idl.tao_yy"
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
case 152:
#line 1205 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 154:
#line 1210 "fe/idl.tao_yy"
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
case 168:
#line 1246 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = new UTL_DeclList(tao_yyvsp[-1].deval, tao_yyvsp[0].dlval);
	}
break;
case 169:
#line 1254 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        }
break;
case 170:
#line 1258 "fe/idl.tao_yy"
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
case 171:
#line 1269 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = NULL;
	}
break;
case 174:
#line 1281 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = new UTL_DeclList(tao_yyvsp[-1].deval, tao_yyvsp[0].dlval);
	}
break;
case 175:
#line 1289 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        }
break;
case 176:
#line 1293 "fe/idl.tao_yy"
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
case 177:
#line 1304 "fe/idl.tao_yy"
{
	  tao_yyval.dlval = NULL;
	}
break;
case 178:
#line 1311 "fe/idl.tao_yy"
{
	  tao_yyval.deval = new FE_Declarator(new UTL_ScopedName(tao_yyvsp[0].idval, NULL),
				 FE_Declarator::FD_simple, NULL);
	}
break;
case 179:
#line 1319 "fe/idl.tao_yy"
{
	  tao_yyval.deval = new FE_Declarator(new UTL_ScopedName(tao_yyvsp[0].dcval->local_name(), NULL),
				 FE_Declarator::FD_complex,
				 tao_yyvsp[0].dcval);
	}
break;
case 182:
#line 1333 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_long;
	}
break;
case 183:
#line 1337 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_longlong;
        }
break;
case 184:
#line 1341 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_short;
	}
break;
case 185:
#line 1348 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_ulong;
	}
break;
case 186:
#line 1352 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_ulonglong;
        }
break;
case 187:
#line 1356 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_ushort;
	}
break;
case 188:
#line 1363 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_double;
	}
break;
case 189:
#line 1367 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_float;
	}
break;
case 190:
#line 1371 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_longdouble;
        }
break;
case 191:
#line 1378 "fe/idl.tao_yy"
{	  
           cerr << "error in " << idl_global->filename()->get_string()
		<< " line " << idl_global->lineno() << ":\n" ;
           cerr << "Sorry, I (TAO_IDL) can't handle fixed types yet\n";
	}
break;
case 192:
#line 1387 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_char;
	}
break;
case 193:
#line 1391 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_wchar;
        }
break;
case 194:
#line 1398 "fe/idl.tao_yy"
{
          tao_yyval.etval = AST_Expression::EV_octet;
	}
break;
case 195:
#line 1405 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_bool;
        }
break;
case 196:
#line 1412 "fe/idl.tao_yy"
{
	  tao_yyval.etval = AST_Expression::EV_any;
	}
break;
case 197:
#line 1419 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructSeen);
        }
break;
case 198:
#line 1423 "fe/idl.tao_yy"
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
case 199:
#line 1447 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructSqSeen);
        }
break;
case 200:
#line 1451 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StructBodySeen);
        }
break;
case 201:
#line 1455 "fe/idl.tao_yy"
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
case 203:
#line 1474 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 204:
#line 1480 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 205:
#line 1484 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 208:
#line 1494 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 209:
#line 1498 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 210:
#line 1505 "fe/idl.tao_yy"
{ 
          /* is $0 to member_i */
          tao_yyval.vival = AST_Field::vis_NA;
        }
break;
case 212:
#line 1514 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberTypeSeen);
        }
break;
case 213:
#line 1518 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_MemberDeclsSeen);
        }
break;
case 214:
#line 1522 "fe/idl.tao_yy"
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
case 215:
#line 1556 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 216:
#line 1560 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 217:
#line 1568 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionSeen);
        }
break;
case 218:
#line 1572 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionIDSeen);
        }
break;
case 219:
#line 1576 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchSeen);
        }
break;
case 220:
#line 1580 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchOpenParSeen);
        }
break;
case 221:
#line 1584 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SwitchTypeSeen);
        }
break;
case 222:
#line 1588 "fe/idl.tao_yy"
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
case 223:
#line 1617 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionSqSeen);
        }
break;
case 224:
#line 1621 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionBodySeen);
        }
break;
case 225:
#line 1625 "fe/idl.tao_yy"
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
case 226:
#line 1647 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 227:
#line 1651 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 228:
#line 1655 "fe/idl.tao_yy"
{
          /* octets are not allowed*/
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 229:
#line 1661 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 231:
#line 1666 "fe/idl.tao_yy"
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
case 235:
#line 1745 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionLabelSeen);
        }
break;
case 236:
#line 1749 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemSeen);
        }
break;
case 237:
#line 1753 "fe/idl.tao_yy"
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
case 238:
#line 1777 "fe/idl.tao_yy"
{
	  idl_global->err()->syntax_error(idl_global->parse_state());
	}
break;
case 239:
#line 1782 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
	  tao_yyerrok;
	}
break;
case 240:
#line 1790 "fe/idl.tao_yy"
{
	  tao_yyval.llval = new UTL_LabelList(tao_yyvsp[-1].ulval, tao_yyvsp[0].llval);
	}
break;
case 241:
#line 1797 "fe/idl.tao_yy"
{
	  if (tao_yyvsp[-1].llval == NULL)
	    tao_yyval.llval = new UTL_LabelList(tao_yyvsp[0].ulval, NULL);
	  else {
	    tao_yyvsp[-1].llval->nconc(new UTL_LabelList(tao_yyvsp[0].ulval, NULL));
	    tao_yyval.llval = tao_yyvsp[-1].llval;
	  }
	}
break;
case 242:
#line 1806 "fe/idl.tao_yy"
{
	  tao_yyval.llval = NULL;
	}
break;
case 243:
#line 1813 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DefaultSeen);
        }
break;
case 244:
#line 1817 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

	  tao_yyval.ulval = idl_global->gen()->
	            create_union_label(AST_UnionLabel::UL_default,
				       NULL);
	}
break;
case 245:
#line 1825 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_CaseSeen);
        }
break;
case 246:
#line 1829 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelExprSeen);
        }
break;
case 247:
#line 1833 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

	  tao_yyval.ulval = idl_global->gen()->create_union_label(AST_UnionLabel::UL_label,
						     tao_yyvsp[-2].exval);
	}
break;
case 248:
#line 1843 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemTypeSeen);
        }
break;
case 249:
#line 1847 "fe/idl.tao_yy"
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
case 250:
#line 1873 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumSeen);
        }
break;
case 251:
#line 1877 "fe/idl.tao_yy"
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
case 252:
#line 1904 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumSqSeen);
        }
break;
case 253:
#line 1908 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumBodySeen);
        }
break;
case 254:
#line 1912 "fe/idl.tao_yy"
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
case 256:
#line 1934 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_EnumCommaSeen);
        }
break;
case 259:
#line 1943 "fe/idl.tao_yy"
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
case 260:
#line 1967 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceCommaSeen);
        }
break;
case 261:
#line 1971 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceExprSeen);
        }
break;
case 262:
#line 1975 "fe/idl.tao_yy"
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
	      ; /* Error will be caught in FE_Declarator.*/
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
case 263:
#line 2006 "fe/idl.tao_yy"
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
	      ; /* Error will be caught in FE_Declarator.*/
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
case 264:
#line 2039 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSeen);
	  /*
	   * Push a sequence marker on scopes stack
	   */
	  idl_global->scopes()->push(NULL);
	}
break;
case 265:
#line 2047 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSqSeen);
        }
break;
case 266:
#line 2051 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_SequenceTypeSeen);
	  tao_yyval.dcval = tao_yyvsp[0].dcval;
        }
break;
case 267:
#line 2060 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        }
break;
case 268:
#line 2064 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        }
break;
case 269:
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
	    tao_yyval.dcval = idl_global->gen()->create_string(tao_yyvsp[-2].exval);
	    /*
	     * Add this AST_String to the types defined in the global scope
	     */
	    (void) idl_global->root()
		      ->fe_add_string(AST_String::narrow_from_decl(tao_yyval.dcval));
	  }
	}
break;
case 270:
#line 2086 "fe/idl.tao_yy"
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
case 271:
#line 2104 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        }
break;
case 272:
#line 2112 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        }
break;
case 273:
#line 2116 "fe/idl.tao_yy"
{
	   idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        }
break;
case 274:
#line 2120 "fe/idl.tao_yy"
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
case 275:
#line 2138 "fe/idl.tao_yy"
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
case 276:
#line 2156 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        }
break;
case 277:
#line 2163 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ArrayIDSeen);
        }
break;
case 278:
#line 2167 "fe/idl.tao_yy"
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
case 279:
#line 2181 "fe/idl.tao_yy"
{
	  tao_yyval.elval = new UTL_ExprList(tao_yyvsp[-1].exval, tao_yyvsp[0].elval);
	}
break;
case 280:
#line 2188 "fe/idl.tao_yy"
{
	  if (tao_yyvsp[-1].elval == NULL)
	    tao_yyval.elval = new UTL_ExprList(tao_yyvsp[0].exval, NULL);
	  else {
	    tao_yyvsp[-1].elval->nconc(new UTL_ExprList(tao_yyvsp[0].exval, NULL));
	    tao_yyval.elval = tao_yyvsp[-1].elval;
	  }
	}
break;
case 281:
#line 2197 "fe/idl.tao_yy"
{
	  tao_yyval.elval = NULL;
	}
break;
case 282:
#line 2204 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimSqSeen);
        }
break;
case 283:
#line 2208 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_DimExprSeen);
        }
break;
case 284:
#line 2212 "fe/idl.tao_yy"
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
case 285:
#line 2229 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrSeen);
        }
break;
case 286:
#line 2233 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrTypeSeen);
        }
break;
case 287:
#line 2237 "fe/idl.tao_yy"
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
case 288:
#line 2272 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_AttrROSeen);
	  tao_yyval.bval = I_TRUE;
	}
break;
case 289:
#line 2277 "fe/idl.tao_yy"
{
	  tao_yyval.bval = I_FALSE;
	}
break;
case 290:
#line 2284 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSeen);
	}
break;
case 291:
#line 2288 "fe/idl.tao_yy"
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
case 292:
#line 2312 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSqSeen);
        }
break;
case 293:
#line 2316 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_ExceptBodySeen);
        }
break;
case 294:
#line 2320 "fe/idl.tao_yy"
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
case 295:
#line 2336 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpTypeSeen);
        }
break;
case 296:
#line 2340 "fe/idl.tao_yy"
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
case 297:
#line 2369 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParsCompleted);
        }
break;
case 298:
#line 2373 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseCompleted);
        }
break;
case 299:
#line 2377 "fe/idl.tao_yy"
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
case 300:
#line 2402 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
	  tao_yyval.ofval = AST_Operation::OP_oneway;
	}
break;
case 301:
#line 2407 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
	  tao_yyval.ofval = AST_Operation::OP_idempotent;
	}
break;
case 302:
#line 2412 "fe/idl.tao_yy"
{
	  tao_yyval.ofval = AST_Operation::OP_noflags;
	}
break;
case 304:
#line 2420 "fe/idl.tao_yy"
{
	  tao_yyval.dcval =
	    idl_global->scopes()->bottom()
	       ->lookup_primitive_type(AST_Expression::EV_void);
	}
break;
case 305:
#line 2429 "fe/idl.tao_yy"
{
           cerr << "error in " << idl_global->filename()->get_string()
		<< " line " << idl_global->lineno() << ":\n" ;
           cerr << "Sorry, I (TAO_IDL) can't handle init yet\n";
        }
break;
case 306:
#line 2439 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        }
break;
case 307:
#line 2443 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        }
break;
case 308:
#line 2447 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        }
break;
case 309:
#line 2452 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        }
break;
case 311:
#line 2462 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParCommaSeen);
        }
break;
case 314:
#line 2471 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParDirSeen);
        }
break;
case 315:
#line 2475 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpParTypeSeen);
        }
break;
case 316:
#line 2479 "fe/idl.tao_yy"
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
case 317:
#line 2501 "fe/idl.tao_yy"
{
	  tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
	}
break;
case 320:
#line 2507 "fe/idl.tao_yy"
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
case 321:
#line 2521 "fe/idl.tao_yy"
{
	  tao_yyval.dival = AST_Argument::dir_IN;
	}
break;
case 322:
#line 2525 "fe/idl.tao_yy"
{
	  tao_yyval.dival = AST_Argument::dir_OUT;
	}
break;
case 323:
#line 2529 "fe/idl.tao_yy"
{
	  tao_yyval.dival = AST_Argument::dir_INOUT;
	}
break;
case 324:
#line 2536 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSeen);
        }
break;
case 325:
#line 2540 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSqSeen);
        }
break;
case 326:
#line 2545 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseQsSeen);
	  tao_yyval.nlval = tao_yyvsp[-1].nlval;
	}
break;
case 327:
#line 2550 "fe/idl.tao_yy"
{
	  tao_yyval.nlval = NULL;
	}
break;
case 328:
#line 2557 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSeen);
        }
break;
case 329:
#line 2561 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSqSeen);
        }
break;
case 330:
#line 2566 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextQsSeen);
	  tao_yyval.slval = tao_yyvsp[-1].slval;
	}
break;
case 331:
#line 2571 "fe/idl.tao_yy"
{
	  tao_yyval.slval = NULL;
	}
break;
case 332:
#line 2578 "fe/idl.tao_yy"
{
	  tao_yyval.slval = new UTL_StrList(tao_yyvsp[-1].sval, tao_yyvsp[0].slval);
	}
break;
case 333:
#line 2586 "fe/idl.tao_yy"
{
	  idl_global->set_parse_state(IDL_GlobalData::PS_OpContextCommaSeen);
        }
break;
case 334:
#line 2590 "fe/idl.tao_yy"
{
	  if (tao_yyvsp[-3].slval == NULL)
	    tao_yyval.slval = new UTL_StrList(tao_yyvsp[0].sval, NULL);
	  else {
	    tao_yyvsp[-3].slval->nconc(new UTL_StrList(tao_yyvsp[0].sval, NULL));
	    tao_yyval.slval = tao_yyvsp[-3].slval;
	  }
	}
break;
case 335:
#line 2599 "fe/idl.tao_yy"
{
	  tao_yyval.slval = NULL;
	}
break;
#line 3750 "y.tab.cpp"
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
