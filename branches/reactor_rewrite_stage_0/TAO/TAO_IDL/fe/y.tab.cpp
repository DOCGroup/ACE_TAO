// $Id$
#ifndef lint
char tao_yysccsid[] = "@(#)yaccpar	1.4 (Berkeley) 02/25/90 \n\
 Modified 5/2/90 by J. Roskind to support graphic debugging modes";
#endif
#include "idl.h"
#include "idl_extern.h"
#include "fe_private.h"
#include <stdio.h>

#if (defined(apollo) || defined(hpux)) && defined(__cplusplus)
extern  "C" int tao_yywrap();
#endif  /* (defined(apollo) || defined(hpux)) && defined(__cplusplus)*/


void tao_yyerror (const char *);
int tao_yylex (void);
extern "C" int tao_yywrap (void);
extern char tao_yytext[];
extern int tao_yyleng;
#define TAO_YYDEBUG_LEXER_TEXT (tao_yytext[tao_yyleng] = '\0', tao_yytext)
/* Force the pretty debugging code to compile.*/
#define TAO_YYDEBUG 1
typedef union {
  AST_Decl                      *dcval;         /* Decl value           */
  UTL_StrList                   *slval;         /* String list          */
  UTL_NameList                  *nlval;         /* Name list            */
  UTL_ExprList                  *elval;         /* Expression list      */
  UTL_LabelList                 *llval;         /* Label list           */
  UTL_DeclList                  *dlval;         /* Declaration list     */
  FE_InterfaceHeader            *ihval;         /* Interface header     */
  FE_obv_header                 *vhval;         /* Valuetype header     */
  AST_Expression                *exval;         /* Expression value     */
  AST_UnionLabel                *ulval;         /* Union label          */
  AST_Field                     *ffval;         /* Field value          */
  AST_Field::Visibility         vival;          /* N/A, pub or priv     */
  AST_Expression::ExprType      etval;          /* Expression type      */
  AST_Argument::Direction       dival;          /* Argument direction   */
  AST_Operation::Flags          ofval;          /* Operation flags      */
  FE_Declarator                 *deval;         /* Declarator value     */
  idl_bool                      bval;           /* Boolean value        */
  long                          ival;           /* Long value           */
  unsigned long                 uival;          /* Unsigned long value  */
  double                        dval;           /* Double value         */
  float                         fval;           /* Float value          */
  char                          cval;           /* Char value           */
  ACE_CDR::WChar                wcval;          /* WChar value          */
  UTL_String                    *sval;          /* String value         */
  char                          *wsval;         /* WString value        */
  char                          *strval;        /* char * value         */
  Identifier                    *idval;         /* Identifier           */
  UTL_IdList                    *idlist;        /* Identifier list      */
} TAO_YYSTYPE;
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
#define IDL_FACTORY 297
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
  138,  139,  141,  142,    7,  140,  144,  144,  145,  143,
  146,  147,  115,  148,  115,  149,  150,  151,  152,  153,
  154,  156,  157,   10,    9,    9,    9,    9,    9,    9,
  155,  159,  159,  160,  161,  158,  162,  158,   27,   28,
   28,  163,   48,  164,  165,   48,  166,   49,  167,  168,
  170,  171,    8,  169,  174,  173,  173,  172,  175,  176,
    5,    5,  177,  178,   13,  180,  181,    6,    6,  179,
  183,  184,   14,   14,  182,  185,   11,   25,   26,   26,
  186,  187,   47,  188,  189,  121,   66,   66,  190,  191,
  192,  193,   77,  194,  196,  197,  198,  123,   62,   62,
   62,   12,   12,  114,  199,  195,  201,  195,  200,  204,
  203,  203,  205,  206,  202,   15,   15,   15,   15,   61,
   61,   61,  207,  208,   23,   23,  209,  210,   17,   17,
   18,  211,   19,   19,
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
    0,    0,    0,    0,    9,    2,    2,    0,    0,    2,
    0,    0,    5,    0,    3,    0,    0,    0,    0,    0,
    0,    0,    0,   17,    1,    1,    1,    1,    1,    1,
    2,    2,    0,    0,    0,    5,    0,    3,    2,    2,
    0,    0,    3,    0,    0,    5,    0,    3,    0,    0,
    0,    0,    9,    2,    0,    4,    0,    1,    0,    0,
    6,    2,    0,    0,    5,    0,    0,    6,    1,    1,
    0,    0,    6,    1,    1,    0,    3,    2,    2,    0,
    0,    0,    5,    0,    0,    6,    1,    0,    0,    0,
    0,    0,    9,    0,    0,    0,    0,   10,    1,    1,
    0,    1,    1,    3,    0,    3,    0,    4,    2,    0,
    4,    0,    0,    0,    5,    1,    1,    1,    1,    1,
    1,    1,    0,    0,    6,    0,    0,    0,    6,    0,
    2,    0,    4,    0,
};
short tao_yydefred[] = {                                      3,
    0,    0,   16,   97,   18,   29,  146,  201,  216,  249,
  289,    0,    0,    0,    0,   52,  148,  150,  149,   25,
   43,    0,    0,    4,    2,    6,    8,   10,   12,   14,
   23,   24,   37,   38,   39,   40,   42,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   93,  151,  182,    0,
   47,    0,    0,    0,   41,    0,    0,    0,   34,   31,
    0,  188,    0,  192,  193,  196,  197,  198,  199,  195,
  200,  263,  270,  275,   54,   89,   60,  154,  155,  157,
  166,  167,  169,  171,  170,    0,  168,    0,  159,  161,
  162,  160,  165,  164,  184,  185,  156,  163,    0,   88,
    0,    0,    0,    0,    0,    0,    0,    0,   17,  108,
  109,    0,   98,  102,  103,  105,  106,  107,  104,   19,
   30,  152,  147,  202,  217,  250,  290,   32,    0,   33,
   53,   26,   44,    0,  187,  194,    0,  191,    0,    0,
  259,  262,   91,    0,  266,  271,    5,    7,    9,   11,
   13,   15,    0,    0,    0,    0,    0,    0,    0,   48,
   71,   62,    0,   35,  190,  264,   90,    0,    0,    0,
   51,    0,    0,   99,   20,  183,  153,  175,  176,  177,
    0,  203,  218,  251,  291,   71,    0,    0,    0,    0,
  136,  137,  138,  140,  142,  143,  144,  141,  139,    0,
    0,    0,    0,    0,  145,    0,    0,    0,    0,    0,
    0,  125,  129,  134,  260,   92,    0,  267,  272,    0,
    3,    0,    0,  209,    0,    0,  208,    0,   82,  287,
  299,  300,    0,    0,   72,   74,   76,    0,   70,   78,
   80,    0,   68,   66,    0,   61,   63,   64,   65,   85,
  265,  130,  131,  132,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  100,    0,
  173,  281,  277,  280,  204,  208,    0,  219,  258,  252,
  257,    0,    0,    0,  303,  317,  294,  318,  302,    0,
  316,  284,    0,    0,    0,   28,    0,    0,    0,    0,
    0,   46,    0,  135,    0,    0,    0,    0,    0,    0,
    0,  126,  127,  128,  261,  268,  273,    0,    0,    0,
    0,    0,    0,    0,  214,  211,  210,    0,    0,    0,
  207,    0,   50,   83,    0,    0,   73,   75,   77,   79,
   81,    0,  304,   69,   67,    0,  101,  111,   22,  174,
  282,  279,  205,    0,    0,    0,  229,  220,    0,  225,
  226,  228,  227,  253,  255,  293,  295,  285,    0,    0,
    0,  215,  212,    0,    0,    0,    0,  306,  320,  321,
  322,  313,    0,  312,  283,    0,  221,  256,  296,  286,
  181,    0,  308,    0,  213,    0,    0,    0,  314,  310,
  222,  323,  297,  179,    0,    0,    0,    0,    0,    0,
  315,  311,  237,  244,  242,  234,  241,  223,  233,  324,
  327,  298,  180,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  238,  245,  243,  247,  235,  240,  224,  232,
    0,  328,    0,    0,    0,  325,    0,  246,  248,  236,
  334,    0,    0,  329,  332,    0,  333,
};
short tao_yydgoto[] = {                                       1,
  326,   78,   79,   80,   81,   82,   83,   84,  358,   85,
  176,  287,   86,   87,  289,  204,  422,  452,  453,  164,
  189,   60,  403,  171,  273,  322,  416,  428,  177,  222,
  390,  398,   20,   21,  347,  205,  206,  207,  208,  209,
  210,  211,  212,  213,  214,  215,  274,  417,  437,  113,
   89,   90,   91,   92,   93,   94,   95,   96,   97,   98,
  382,  233,  178,  179,  180,  234,   99,   22,   23,  100,
  235,    2,   25,  103,  236,  104,  237,  105,   28,  106,
   29,  107,   30,  108,   38,   40,  154,  221,  319,   31,
   32,   57,  187,  161,  238,   41,  134,   33,   34,   35,
   36,   37,   58,  188,  162,  245,  129,  186,  283,   56,
  246,  247,  239,  249,  327,  301,  300,  293,  294,  295,
  240,  297,  241,  298,  284,  303,  140,  169,   39,  153,
  220,  318,  123,   42,  155,  320,  410,   43,  156,  275,
  224,  323,  331,  282,  277,  355,  386,  354,   44,  157,
  225,  328,  374,  396,  418,  407,  429,  419,  430,  427,
  445,  424,  426,  425,  443,  444,   45,  158,  280,  226,
  329,  281,  330,  375,  168,  266,  139,  190,  101,  172,
  267,  102,  173,  268,  223,  321,  371,  336,  377,   46,
  159,  227,  332,  335,  343,  376,  397,  409,  369,  383,
  370,  384,  394,  406,  392,  405,  408,  431,  432,  447,
  456,
};
short tao_yysindex[] = {                                      0,
    0,  590,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -208, -197, -175, -224,    0,    0,    0,    0,    0,
    0,   26,  430,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   49,  566, -146,
 -208,  448, -208, -208, -208, -208,    0,    0,    0,   26,
    0,   26, -186, -186,    0, -208,   -5,   17,    0,    0,
 -209,    0, -138,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   59,    0, -177,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   26,    0,
   77,   87,   99,  105,  108,  113,  115,  134,    0,    0,
    0, -177,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   73,    0,
    0,    0,    0, -221,    0,    0,  -59,    0,  149, -208,
    0,    0,    0,  -88,    0,    0,    0,    0,    0,    0,
    0,    0, -208,  117, -208,  125,  -51,  126,  127,    0,
    0,    0, -177,    0,    0,    0,    0,   12, -208, -221,
    0,   12,   12,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  648,  617,  207,  357,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   31,
   31,   31,   12, -177,    0,  130,  162,  220, -131,  132,
  129,    0,    0,    0,    0,    0, -177,    0,    0,  198,
    0,  221,  173,    0,  226,   16,    0,  648,    0,    0,
    0,    0,  515,  -16,    0,    0,    0,  145,    0,    0,
    0,   18,    0,    0,  146,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  235,   12,   12,   12,   12,   12,
   12,   12,   12,   12,   12,  227,  229,  238,    0,  590,
    0,    0,    0,    0,    0,    0,  381,    0,    0,    0,
    0,    0,  152,  223,    0,    0,    0,    0,    0, -177,
    0,    0,  251,  253,  264,    0,  265,  268,  288,  381,
  381,    0, -221,    0,  162,  220, -131,  132,  132,  129,
  129,    0,    0,    0,    0,    0,    0,   12,  204, -208,
   12,  173,  205,    0,    0,    0,    0,  157,  206,  289,
    0,  217,    0,    0,   89,  548,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -177,    0,    0,    0,    0,
    0,    0,    0,  292, -208,   85,    0,    0, -177,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  311,  -74,
  261,    0,    0,  314,   16,  288, -208,    0,    0,    0,
    0,    0,  315,    0,    0,  300,    0,    0,    0,    0,
    0,  548,    0,  316,    0,  239,   70,  319,    0,    0,
    0,    0,    0,    0, -208,  -74, -188,  324,   74, -208,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  308,   12,  313,  448,  -94,  244, -188,
 -221,  336,    0,    0,    0,    0,    0,    0,    0,    0,
  337,    0,  321, -208,  318,    0,   94,    0,    0,    0,
    0,  359,  360,    0,    0,   98,    0,
};
short tao_yyrindex[] = {                                      0,
    0,  407,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   -1,  -13,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   11,
    0,   42,    3,  -43,    0,    0,    0,    0,    0,    0,
   29,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   33,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  -98,    0,
   54,   91,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  151,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   28,    0,    0,    0,
    0,    0,    0,  287,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   -6,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   23,    0,    0,    0,    0,    0,  228,  277,   13,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  -19,    0,  120,  470,  537,  464,  111,
   38,    0,    0,    0,    0,    0,  293,    0,    0,    0,
    0,  352,    0,    0,    0,    0,    0,  305,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  290,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  118,    0,    0,    0,    0,    0,    0,    0,  160,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  549,  539,  492,  148,  159,   61,
  101,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    6,    0,  179,    0,    0,    0,    0,    0,  304,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  -28,    0,    0,    0,    7,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  377,    0,    0,  390,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  393,    0,    0,  -48,  378,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  379,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  497,    0,  329,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  398,    0,    0,    0,    0,
};
short tao_yygindex[] = {                                      0,
  -21,  250,    0,    0,    0,  -35,   35,    4,    0,   39,
    0,    0,    0,  -34, -306,  -22,    0,    0,    0,   30,
    0,   57,    0,    0,    0,    0,    0,    0,  107,    0,
    0,    0,    0,  449,    0, -189,    0,  208,  209,  210,
  -76,  -42,  -26,   45,    0, -113,  143,   41,    0,    0,
  -32,  -31,  -30,  433,    0,  434,    0,    0, -191,  -29,
    0,    0, -254,   -9,    0,    0,    0,  215,  216,  -12,
   10,  254,    0,    0,   14,    0,   25,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  291,    0,    0,    0,    0,    0,    0,    0,
    0,  459,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  294,    0,  -67,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  252,  202,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   50,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  104,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  110,    0,    0,    0,    0,    0,
    0,   75,    0,    0,    0,    0,    0,    0,    0,    0,
    0,
};
#define TAO_YYTABLESIZE 941
short tao_yytable[] = {                                      49,
   88,   77,   48,  110,  111,   18,  114,  115,  116,  119,
  326,   24,  305,  255,   55,   26,  112,  133,  133,   88,
  122,  133,  133,  133,   36,  133,   27,  133,  121,  368,
  124,  125,  126,  127,   87,   47,   17,   87,  133,  133,
   19,  291,  133,  131,   55,   59,  278,   86,   47,  278,
   86,  203,  135,   84,  200,  136,  201,   94,  218,  219,
   55,   58,    6,  182,  278,  350,  182,  413,  189,   95,
  203,  189,  186,  133,  133,  122,  158,   16,  122,   55,
  122,  182,  122,   59,    6,  399,  189,  186,   76,  189,
  186,  158,  414,  415,  158,  122,  122,  269,  123,  122,
   96,  123,  141,  123,  133,  123,  128,  109,  130,   55,
  120,  163,  269,  276,   75,  269,   87,  132,  123,  123,
  142,   36,  123,  137,  138,   55,   16,  167,  348,   86,
  122,  122,  143,   36,  274,   84,  145,  202,  124,  133,
  174,  124,  181,  124,  291,  124,  146,  217,  119,  274,
  411,  119,  274,  123,  123,  144,  216,  147,  124,  124,
  112,  122,  124,  148,   36,  265,  149,   88,  119,  119,
  263,  150,  119,  151,  261,  264,  262,  112,  112,  259,
  260,  112,  308,  309,  123,  120,  414,  415,  120,  449,
   18,   18,  152,  124,  124,  160,  121,  286,  288,  121,
  291,   36,  165,  119,  119,  120,  120,  351,  166,  120,
  290,  170,  112,  379,  380,  381,  121,  121,  310,  311,
  121,   17,   17,  183,  124,   19,   19,   50,   52,   53,
   54,   18,  344,  345,  119,  434,  312,  313,  314,  175,
  120,  120,  292,  326,  252,  253,  254,  182,  184,  185,
  250,  121,  121,  256,   88,  257,   55,  258,  269,  307,
  307,  307,   17,  272,  271,  278,   19,  292,   47,  296,
  302,  120,  279,   18,  299,  304,  333,   88,   88,   24,
  346,  334,  121,   26,  189,  186,   55,   47,  315,  158,
  316,  133,  133,   87,   27,  360,  361,  362,  363,  317,
  286,  288,   55,  206,   17,  359,   86,  181,   19,  337,
  269,  338,   84,  290,  191,  192,  193,  194,  195,  196,
  197,   76,  339,  340,  198,  199,  341,  342,  349,  353,
  364,  357,  365,  191,  192,  193,  194,  195,  196,  197,
   76,  366,  181,  198,  199,  367,  135,  274,  122,  122,
  372,  378,   27,  385,  387,  393,  286,  288,  395,  400,
  402,  401,  404,  420,   49,  421,  433,  391,  439,  290,
  435,  123,  123,  209,  209,  442,  450,  446,  448,  209,
  209,  209,  209,  209,  209,  209,  209,  209,  209,  209,
  209,  209,  181,  209,  209,  209,  209,   49,  451,  454,
  423,   45,  457,  455,   88,  436,    1,  110,  163,   57,
  172,  124,  124,   47,   21,   56,  319,  186,  356,   62,
   63,  119,  119,   66,   67,   68,   69,  209,  254,   49,
  230,  181,   10,  309,  209,  209,  178,  330,  331,  251,
  209,  209,  209,  209,  209,  209,  209,  209,  209,  209,
  209,  209,  209,  231,  209,  209,  209,  209,  120,  120,
  441,  373,   51,  305,  352,  306,   76,  307,  438,  121,
  121,  117,  118,   55,  270,  276,  228,  324,  388,  440,
  412,  248,    0,    0,  301,  389,    0,    0,  209,  301,
  301,  301,  301,  301,  301,  301,  301,  301,  301,  301,
    0,  117,    0,    0,  117,  301,  301,    0,    0,    0,
  113,  288,    0,    0,  301,    0,    0,    0,    0,    0,
    0,  117,  117,    0,    0,  117,    0,  113,  113,  118,
    0,  113,  118,  301,    0,    0,    0,  301,  301,  301,
  301,  301,  301,  301,  301,  301,  301,  301,  301,  118,
  118,    0,    0,  118,  301,  301,  117,  117,    0,    0,
  288,  301,  113,  301,    0,    0,  301,  301,  301,  301,
  301,  301,  301,  301,  301,  301,  301,  115,    0,  116,
    0,    0,  301,  301,  118,  118,  301,  117,  288,  114,
    0,  301,    0,  113,  115,  115,  116,  116,  115,    0,
  116,    0,    0,    0,    0,    0,  114,  114,    0,    0,
  114,    0,    0,   47,  301,  118,    0,    0,   61,   62,
   63,   64,   65,   66,   67,   68,   69,   70,   71,  115,
  115,  116,  116,   72,   73,   74,  325,   47,    0,    0,
    0,  114,   61,   62,   63,   64,   65,   66,   67,   68,
   69,   70,   71,    8,    9,    0,   10,   72,   73,   74,
  115,    0,  116,    0,    0,    0,   76,    0,    0,    0,
    0,    0,  114,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   47,    0,    0,    0,
   76,   61,   62,   63,   64,   65,   66,   67,   68,   69,
   70,   71,    8,    9,   47,   10,   72,   73,   74,   61,
   62,   63,   64,   65,   66,   67,   68,   69,   70,   71,
    8,    9,    0,   10,   72,   73,   74,    0,    0,    0,
   75,    0,    0,    0,    0,    0,    0,    0,    0,   76,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  239,    0,    0,    0,   76,  239,  239,
  239,  239,  239,  239,  239,  239,  239,  239,  239,  239,
  239,   47,  239,  239,  239,  239,   61,   62,   63,   64,
   65,   66,   67,   68,   69,   70,   71,    0,    0,    0,
    0,    0,   73,   74,    0,    0,    0,    0,    0,    0,
    0,  285,    0,    0,   47,    0,  239,    0,    0,   61,
   62,   63,   64,   65,   66,   67,   68,   69,   70,   71,
    0,    0,   47,    0,   76,   73,   74,   61,   62,   63,
   64,   65,   66,   67,   68,   69,   70,    0,    0,    0,
    0,    0,    0,   73,   74,    3,    0,    4,    5,    6,
    7,    0,    0,    0,    0,    0,    0,   76,    0,    0,
    0,    0,    8,    9,    0,   10,    0,    0,    0,   11,
    0,    0,  229,    0,    4,   76,    0,    7,    0,    0,
    0,    0,   12,   13,   14,   15,    0,    0,    0,    8,
    9,   16,   10,    0,    0,    0,   11,    0,    0,  230,
    0,  231,  232,  229,    0,    4,    0,    0,    7,   12,
    0,    0,    0,  242,  243,  244,    0,    0,    0,    0,
    8,    9,    0,   10,    0,    0,    0,   11,    0,    0,
  230,    0,  231,  232,    0,    0,    0,    0,    0,    0,
   12,
};
short tao_yycheck[] = {                                      12,
   23,   23,   12,   39,   39,    2,   39,   39,   39,   39,
   59,    2,   41,  203,   58,    2,   39,   37,   38,   42,
   42,   41,   42,   43,  123,   45,    2,   47,   41,  336,
   43,   44,   45,   46,   41,  257,    2,   44,   58,   59,
    2,  233,   62,   56,   58,   59,   41,   41,  257,   44,
   44,   40,  262,   41,   43,  265,   45,   59,  172,  173,
   58,   59,  260,   41,   59,  320,   44,  256,   41,   59,
   40,   44,   44,   93,   94,   38,   44,  302,   41,  123,
   43,   59,   45,   58,  260,  392,   59,   59,  310,   62,
   62,   59,  281,  282,   62,   58,   59,   44,   38,   62,
   59,   41,   44,   43,  124,   45,   50,   59,   52,  123,
  257,  134,   59,   91,  301,   62,  123,  123,   58,   59,
   62,  123,   62,  262,  263,  123,  302,  140,  318,  123,
   93,   94,  310,  123,   44,  123,   60,  126,   38,  123,
  153,   41,  155,   43,  336,   45,   60,  170,   38,   59,
  405,   41,   62,   93,   94,   99,  169,   59,   58,   59,
   41,  124,   62,   59,  123,   37,   59,  190,   58,   59,
   42,   59,   62,   59,   43,   47,   45,   58,   59,  311,
  312,   62,  259,  260,  124,   38,  281,  282,   41,  444,
  187,  188,   59,   93,   94,  123,   38,  233,  233,   41,
  392,  300,  262,   93,   94,   58,   59,  321,   60,   62,
  233,  300,   93,  288,  289,  290,   58,   59,  261,  262,
   62,  187,  188,  275,  124,  187,  188,   13,   14,   14,
   15,  228,  300,  301,  124,  425,  263,  264,  265,  123,
   93,   94,  125,  292,  200,  201,  202,  123,  123,  123,
   44,   93,   94,  124,  277,   94,  300,   38,   61,  288,
  289,  290,  228,   91,   44,   40,  228,  284,  257,  125,
  125,  124,  257,  270,  257,   41,  125,  300,  301,  270,
  303,   59,  124,  270,  257,  257,  300,  257,   62,  257,
   62,  311,  312,  300,  270,  328,  328,  328,  328,   62,
  336,  336,  300,  125,  270,  328,  300,  320,  270,   59,
  257,   59,  300,  336,  303,  304,  305,  306,  307,  308,
  309,  310,   59,   59,  313,  314,   59,   40,  125,  125,
  125,  328,   44,  303,  304,  305,  306,  307,  308,  309,
  310,  125,  355,  313,  314,  257,  262,  257,  311,  312,
   59,   41,  125,   93,   41,   41,  392,  392,   59,   44,
  291,  123,   44,   40,  377,  292,   59,  377,  125,  392,
   58,  311,  312,  256,  257,   40,   59,   41,   58,  262,
  263,  264,  265,  266,  267,  268,  269,  270,  271,  272,
  273,  274,  405,  276,  277,  278,  279,  410,  305,   41,
  410,  125,  305,   44,  427,  427,    0,  257,  431,  123,
   59,  311,  312,  257,  125,  123,  257,   41,  262,  263,
  264,  311,  312,  267,  268,  269,  270,  310,  125,  125,
   41,  444,  276,   41,  256,  257,   59,   59,   41,  190,
  262,  263,  264,  265,  266,  267,  268,  269,  270,  271,
  272,  273,  274,  125,  276,  277,  278,  279,  311,  312,
  431,  355,   14,  256,  322,  257,  310,  258,  428,  311,
  312,   39,   39,   15,  221,  224,  186,  276,  375,  430,
  406,  188,   -1,   -1,  257,  376,   -1,   -1,  310,  262,
  263,  264,  265,  266,  267,  268,  269,  270,  271,  272,
   -1,   38,   -1,   -1,   41,  278,  279,   -1,   -1,   -1,
   41,  284,   -1,   -1,  287,   -1,   -1,   -1,   -1,   -1,
   -1,   58,   59,   -1,   -1,   62,   -1,   58,   59,   38,
   -1,   62,   41,  257,   -1,   -1,   -1,  310,  262,  263,
  264,  265,  266,  267,  268,  269,  270,  271,  272,   58,
   59,   -1,   -1,   62,  278,  279,   93,   94,   -1,   -1,
  284,  257,   93,  287,   -1,   -1,  262,  263,  264,  265,
  266,  267,  268,  269,  270,  271,  272,   41,   -1,   41,
   -1,   -1,  278,  279,   93,   94,  310,  124,  284,   41,
   -1,  287,   -1,  124,   58,   59,   58,   59,   62,   -1,
   62,   -1,   -1,   -1,   -1,   -1,   58,   59,   -1,   -1,
   62,   -1,   -1,  257,  310,  124,   -1,   -1,  262,  263,
  264,  265,  266,  267,  268,  269,  270,  271,  272,   93,
   94,   93,   94,  277,  278,  279,  256,  257,   -1,   -1,
   -1,   93,  262,  263,  264,  265,  266,  267,  268,  269,
  270,  271,  272,  273,  274,   -1,  276,  277,  278,  279,
  124,   -1,  124,   -1,   -1,   -1,  310,   -1,   -1,   -1,
   -1,   -1,  124,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  257,   -1,   -1,   -1,
  310,  262,  263,  264,  265,  266,  267,  268,  269,  270,
  271,  272,  273,  274,  257,  276,  277,  278,  279,  262,
  263,  264,  265,  266,  267,  268,  269,  270,  271,  272,
  273,  274,   -1,  276,  277,  278,  279,   -1,   -1,   -1,
  301,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  310,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,  257,   -1,   -1,   -1,  310,  262,  263,
  264,  265,  266,  267,  268,  269,  270,  271,  272,  273,
  274,  257,  276,  277,  278,  279,  262,  263,  264,  265,
  266,  267,  268,  269,  270,  271,  272,   -1,   -1,   -1,
   -1,   -1,  278,  279,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  287,   -1,   -1,  257,   -1,  310,   -1,   -1,  262,
  263,  264,  265,  266,  267,  268,  269,  270,  271,  272,
   -1,   -1,  257,   -1,  310,  278,  279,  262,  263,  264,
  265,  266,  267,  268,  269,  270,  271,   -1,   -1,   -1,
   -1,   -1,   -1,  278,  279,  256,   -1,  258,  259,  260,
  261,   -1,   -1,   -1,   -1,   -1,   -1,  310,   -1,   -1,
   -1,   -1,  273,  274,   -1,  276,   -1,   -1,   -1,  280,
   -1,   -1,  256,   -1,  258,  310,   -1,  261,   -1,   -1,
   -1,   -1,  293,  294,  295,  296,   -1,   -1,   -1,  273,
  274,  302,  276,   -1,   -1,   -1,  280,   -1,   -1,  283,
   -1,  285,  286,  256,   -1,  258,   -1,   -1,  261,  293,
   -1,   -1,   -1,  297,  298,  299,   -1,   -1,   -1,   -1,
  273,  274,   -1,  276,   -1,   -1,   -1,  280,   -1,   -1,
  283,   -1,  285,  286,   -1,   -1,   -1,   -1,   -1,   -1,
  293,
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
"IDL_NATIVE","IDL_LOCAL","IDL_ABSTRACT","IDL_CUSTOM","IDL_FACTORY",
"IDL_PRIVATE","IDL_PUBLIC","IDL_SUPPORTS","IDL_TRUNCATABLE","IDL_VALUETYPE",
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
"init_decl : IDL_FACTORY IDENTIFIER parameter_list",
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
{
          idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        }
break;
case 5:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 6:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        }
break;
case 7:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 8:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        }
break;
case 9:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 10:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceDeclSeen);
        }
break;
case 11:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 12:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_ModuleDeclSeen);
        }
break;
case 13:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 14:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_ValuetypeDeclSeen);
        }
break;
case 15:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 16:
{
          idl_global->err()->syntax_error(idl_global->parse_state());
        }
break;
case 17:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
break;
case 18:
{
            idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSeen);
          }
break;
case 19:
{
            UTL_ScopedName      *n =
                new UTL_ScopedName(new Identifier(tao_yyvsp[0].strval), NULL);
            AST_Module          *m = NULL;
            UTL_Scope           *s = idl_global->scopes()->top_non_null();
            UTL_StrList         *p = idl_global->pragmas();

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
{
            idl_global->set_parse_state(IDL_GlobalData::PS_ModuleSqSeen);
          }
break;
case 21:
{
            idl_global->set_parse_state(IDL_GlobalData::PS_ModuleBodySeen);
          }
break;
case 22:
{
            idl_global->set_parse_state(IDL_GlobalData::PS_ModuleQsSeen);
            /*
             * Finished with this module - pop it from the scope stack
             */
            UTL_Scope* s = idl_global->scopes()->top();
            AST_Module* m = AST_Module::narrow_from_scope (s);
            if (m != 0)
              {
                UTL_StrList *p = m->pragmas ();
                if (p != 0)
                  p = (UTL_StrList*)p->copy ();
                idl_global->set_pragmas (p);
              }
            idl_global->scopes()->pop();
          }
break;
case 25:
{
          UTL_Scope     *s = idl_global->scopes ()->top_non_null ();
          AST_Interface *i = NULL;
          AST_Decl      *v = NULL;
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
{
          idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSqSeen);
        }
break;
case 27:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceBodySeen);
        }
break;
case 28:
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
{
           idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSeen);
         }
break;
case 30:
{
           idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceIDSeen);
           tao_yyval.idval = tao_yyvsp[0].idval;
         }
break;
case 31:
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
{
          idl_global->set_parse_state(IDL_GlobalData::PS_InheritColonSeen);
        }
break;
case 35:
{
          tao_yyval.nlval = tao_yyvsp[0].nlval;
        }
break;
case 36:
{
          tao_yyval.nlval = NULL;
        }
break;
case 41:
{
           cerr << "error in " << idl_global->filename()->get_string()
                << " line " << idl_global->lineno() << ":\n" ;
           cerr << "Sorry, I (TAO_IDL) can't handle custom yet\n";
            /* set custom (if not truncatable) */
        }
break;
case 43:
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
{
          idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSqSeen);
        }
break;
case 45:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceBodySeen);
        }
break;
case 46:
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
{
          idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceSqSeen);
        }
break;
case 49:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_InterfaceBodySeen);
        }
break;
case 50:
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
{
          tao_yyval.vhval = new FE_obv_header (new UTL_ScopedName (tao_yyvsp[-3].idval, NULL), tao_yyvsp[-1].nlval, tao_yyvsp[0].nlval);
        }
break;
case 52:
{
           idl_global->set_parse_state(IDL_GlobalData::PS_ValuetypeSeen);
        }
break;
case 53:
{
           idl_global->set_parse_state(IDL_GlobalData::PS_ValuetypeIDSeen);
           tao_yyval.idval = tao_yyvsp[0].idval;
        }
break;
case 54:
{
            cerr << "warning in " << idl_global->filename()->get_string()
                 << " line " << idl_global->lineno() << ":\n" ;
            cerr << "truncatable modifier not supported and is ignored\n";
            tao_yyval.bval = I_FALSE;
            /* $$ = I_TRUE; */
          }
break;
case 55:
{
            tao_yyval.bval = I_FALSE;
          }
break;
case 56:
{
              tao_yyval.nlval = new UTL_NameList(tao_yyvsp[0].idlist, NULL);
            }
break;
case 57:
{
              tao_yyval.nlval = NULL;
            }
break;
case 58:
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          UTL_ScopedName        *n = new UTL_ScopedName(tao_yyvsp[0].idval, NULL);
          AST_InterfaceFwd      *f = NULL;
          UTL_StrList           *p = idl_global->pragmas();

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
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          UTL_ScopedName        *n = new UTL_ScopedName(tao_yyvsp[0].idval, NULL);
          AST_InterfaceFwd      *f = NULL;
          UTL_StrList           *p = idl_global->pragmas();

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
{
           cerr << "error in " << idl_global->filename()->get_string()
                << " line " << idl_global->lineno() << ":\n" ;
           cerr << "Sorry, I (TAO_IDL) can't handle boxes yet\n";
        }
break;
case 66:
{
            /* is $0 to member_i */
            tao_yyval.vival = AST_Field::vis_PUBLIC;
          }
break;
case 68:
{
            /* is $0 to member_i */
            tao_yyval.vival = AST_Field::vis_PRIVATE;
          }
break;
case 72:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_TypeDeclSeen);
        }
break;
case 73:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 74:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_ConstDeclSeen);
        }
break;
case 75:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 76:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_ExceptDeclSeen);
        }
break;
case 77:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 78:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_AttrDeclSeen);
        }
break;
case 79:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 80:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpDeclSeen);
        }
break;
case 81:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
        }
break;
case 82:
{
          idl_global->err()->syntax_error(idl_global->parse_state());
        }
break;
case 83:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
break;
case 84:
{
          tao_yyval.nlval = new UTL_NameList(tao_yyvsp[-1].idlist, tao_yyvsp[0].nlval);
        }
break;
case 85:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_SNListCommaSeen);
        }
break;
case 86:
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
{
          tao_yyval.nlval = NULL;
        }
break;
case 88:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

          tao_yyval.idlist = new UTL_IdList(tao_yyvsp[0].idval, NULL);
        }
break;
case 89:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        }
break;
case 90:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

          tao_yyval.idlist = new UTL_IdList(new Identifier(tao_yyvsp[-2].strval),
                              new UTL_IdList(tao_yyvsp[0].idval, NULL));
        }
break;
case 91:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_ScopeDelimSeen);
        }
break;
case 92:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_SN_IDSeen);

          tao_yyvsp[-3].idlist->nconc(new UTL_IdList(tao_yyvsp[0].idval, NULL));
          tao_yyval.idlist = tao_yyvsp[-3].idlist;
        }
break;
case 93:
{
            tao_yyval.idval = new Identifier(tao_yyvsp[0].strval);
        }
break;
case 94:
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          UTL_ScopedName        *n = new UTL_ScopedName(tao_yyvsp[0].idval, NULL);
          AST_InterfaceFwd      *f = NULL;
          UTL_StrList           *p = idl_global->pragmas();

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
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          UTL_ScopedName        *n = new UTL_ScopedName(tao_yyvsp[0].idval, NULL);
          AST_InterfaceFwd      *f = NULL;
          UTL_StrList           *p = idl_global->pragmas();

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
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          UTL_ScopedName        *n = new UTL_ScopedName(tao_yyvsp[0].idval, NULL);
          AST_InterfaceFwd      *f = NULL;
          UTL_StrList           *p = idl_global->pragmas();

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
{
          idl_global->set_parse_state(IDL_GlobalData::PS_ConstSeen);
        }
break;
case 98:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_ConstTypeSeen);
        }
break;
case 99:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_ConstIDSeen);
        }
break;
case 100:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_ConstAssignSeen);
        }
break;
case 101:
{
          UTL_ScopedName        *n = new UTL_ScopedName(tao_yyvsp[-4].idval, NULL);
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          AST_Constant          *c = NULL;
          UTL_StrList           *p = idl_global->pragmas();
          AST_Decl              *v = NULL;

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
{
          tao_yyval.etval = AST_Expression::EV_string;
        }
break;
case 109:
{
          tao_yyval.etval = AST_Expression::EV_wstring;
        }
break;
case 110:
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          AST_Decl              *d = NULL;
          AST_PredefinedType    *c = NULL;
          AST_Typedef           *t = NULL;

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
{
          tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_or, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
        }
break;
case 116:
{
          tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_xor, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
        }
break;
case 118:
{
          tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_and, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
        }
break;
case 120:
{
          tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_left,tao_yyvsp[-2].exval,tao_yyvsp[0].exval);
        }
break;
case 121:
{
          tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_right,tao_yyvsp[-2].exval,tao_yyvsp[0].exval);
        }
break;
case 123:
{
          tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_add, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
        }
break;
case 124:
{
          tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_minus,tao_yyvsp[-2].exval,tao_yyvsp[0].exval);
        }
break;
case 126:
{
          tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_mul, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
        }
break;
case 127:
{
          tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_div, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
        }
break;
case 128:
{
          tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_mod, tao_yyvsp[-2].exval, tao_yyvsp[0].exval);
        }
break;
case 130:
{
          tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_u_plus,
                                              tao_yyvsp[0].exval,
                                              NULL);
        }
break;
case 131:
{
          tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_u_minus,
                                              tao_yyvsp[0].exval,
                                              NULL);
        }
break;
case 132:
{
          tao_yyval.exval = idl_global->gen()->create_expr(AST_Expression::EC_bit_neg,
                                              tao_yyvsp[0].exval,
                                              NULL);
        }
break;
case 133:
{
          /*
           * An expression which is a scoped name is not resolved now,
           * but only when it is evaluated (such as when it is assigned
           * as a constant value).
           */
          UTL_Scope *s = idl_global->scopes()->top_non_null ();

          AST_Decl *d = s->lookup_by_name (tao_yyvsp[0].idlist,
                                            1);

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
              tao_yyval.exval = idl_global->gen ()->create_expr (c->constant_value (),
                                                    c->et ());
            }
          else
            {
              tao_yyval.exval = idl_global->gen ()->create_expr (tao_yyvsp[0].idlist);
            }
        }
break;
case 135:
{
          tao_yyval.exval = tao_yyvsp[-1].exval;
        }
break;
case 136:
{
          tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].ival);
        }
break;
case 137:
{
          tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].uival);
        }
break;
case 138:
{
          tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].sval);
        }
break;
case 139:
{
          tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].wsval);
        }
break;
case 140:
{
          tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].cval);
        }
break;
case 141:
{
          ACE_OutputCDR::from_wchar wc (tao_yyvsp[0].wcval);
          tao_yyval.exval = idl_global->gen()->create_expr(wc);
        }
break;
case 142:
{
          tao_yyval.exval = idl_global->gen()->create_expr(tao_yyvsp[0].dval);
        }
break;
case 143:
{
          tao_yyval.exval = idl_global->gen()->create_expr((idl_bool) I_TRUE,
                                              AST_Expression::EV_bool);
        }
break;
case 144:
{
          tao_yyval.exval = idl_global->gen()->create_expr((idl_bool) I_FALSE,
                                              AST_Expression::EV_bool);
        }
break;
case 145:
{
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
              tao_yyval.exval = 
                idl_global->gen()->create_expr (tao_yyvsp[0].exval, 
                                                AST_Expression::EV_ulong);
            }
          else
            {
              idl_global->err ()->syntax_error (idl_global->parse_state ());
            }
        }
break;
case 146:
{
            idl_global->set_parse_state(IDL_GlobalData::PS_TypedefSeen);
          }
break;
case 147:
{tao_yyval.ival = 0;}
break;
case 148:
{ tao_yyval.ival = 0;}
break;
case 149:
{ tao_yyval.ival = 0;}
break;
case 150:
{ tao_yyval.ival = 0;}
break;
case 151:
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          AST_Native            *node = NULL;
          AST_Decl              *v = NULL;
          UTL_StrList           *p = idl_global->pragmas();

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
{
          idl_global->set_parse_state(IDL_GlobalData::PS_TypeSpecSeen);
        }
break;
case 153:
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          UTL_DecllistActiveIterator *l;
          FE_Declarator         *d = NULL;
          AST_Typedef           *t = NULL;
          AST_Decl              *v = NULL;
          UTL_StrList           *p = idl_global->pragmas();

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
{
          tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
        }
break;
case 158:
{
          UTL_Scope     *s = idl_global->scopes()->top_non_null();
          AST_Decl      *d = NULL;

          if (s != NULL)
            d = s->lookup_by_name(tao_yyvsp[0].idlist, I_TRUE);
          if (d == NULL)
            idl_global->err()->lookup_error(tao_yyvsp[0].idlist);
          tao_yyval.dcval = d;
        }
break;
case 172:
{
          tao_yyval.dlval = new UTL_DeclList(tao_yyvsp[-1].deval, tao_yyvsp[0].dlval);
        }
break;
case 173:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        }
break;
case 174:
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
{
          tao_yyval.dlval = NULL;
        }
break;
case 178:
{
          tao_yyval.dlval = new UTL_DeclList(tao_yyvsp[-1].deval, tao_yyvsp[0].dlval);
        }
break;
case 179:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_DeclsCommaSeen);
        }
break;
case 180:
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
{
          tao_yyval.dlval = NULL;
        }
break;
case 182:
{
          tao_yyval.deval = new FE_Declarator(new UTL_ScopedName(tao_yyvsp[0].idval, NULL),
                                 FE_Declarator::FD_simple, NULL);
        }
break;
case 183:
{
          tao_yyval.deval = new FE_Declarator(new UTL_ScopedName(tao_yyvsp[0].dcval->local_name(), NULL),
                                 FE_Declarator::FD_complex,
                                 tao_yyvsp[0].dcval);
        }
break;
case 186:
{
          tao_yyval.etval = AST_Expression::EV_long;
        }
break;
case 187:
{
          tao_yyval.etval = AST_Expression::EV_longlong;
        }
break;
case 188:
{
          tao_yyval.etval = AST_Expression::EV_short;
        }
break;
case 189:
{
          tao_yyval.etval = AST_Expression::EV_ulong;
        }
break;
case 190:
{
          tao_yyval.etval = AST_Expression::EV_ulonglong;
        }
break;
case 191:
{
          tao_yyval.etval = AST_Expression::EV_ushort;
        }
break;
case 192:
{
          tao_yyval.etval = AST_Expression::EV_double;
        }
break;
case 193:
{
          tao_yyval.etval = AST_Expression::EV_float;
        }
break;
case 194:
{
          tao_yyval.etval = AST_Expression::EV_longdouble;
        }
break;
case 195:
{
           cerr << "error in " << idl_global->filename()->get_string()
                << " line " << idl_global->lineno() << ":\n" ;
           cerr << "Sorry, I (TAO_IDL) can't handle fixed types yet\n";
        }
break;
case 196:
{
          tao_yyval.etval = AST_Expression::EV_char;
        }
break;
case 197:
{
          tao_yyval.etval = AST_Expression::EV_wchar;
        }
break;
case 198:
{
          tao_yyval.etval = AST_Expression::EV_octet;
        }
break;
case 199:
{
          tao_yyval.etval = AST_Expression::EV_bool;
        }
break;
case 200:
{
          tao_yyval.etval = AST_Expression::EV_any;
        }
break;
case 201:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_StructSeen);
        }
break;
case 202:
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          UTL_ScopedName        *n = new UTL_ScopedName(tao_yyvsp[0].idval, NULL);
          AST_Structure         *d = NULL;
          UTL_StrList           *p = idl_global->pragmas();
          AST_Decl              *v = NULL;

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
{
          idl_global->set_parse_state(IDL_GlobalData::PS_StructSqSeen);
        }
break;
case 204:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_StructBodySeen);
        }
break;
case 205:
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
case 209:
{
          /* is $0 to member_i */
          tao_yyval.vival = AST_Field::vis_NA;
        }
break;
case 211:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_MemberTypeSeen);
        }
break;
case 212:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_MemberDeclsSeen);
        }
break;
case 213:
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          UTL_DecllistActiveIterator *l = NULL;
          FE_Declarator         *d = NULL;
          AST_Field             *f = NULL;
          UTL_StrList           *p = idl_global->pragmas();

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
case 214:
{
          idl_global->err()->syntax_error(idl_global->parse_state());
        }
break;
case 215:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
break;
case 216:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_UnionSeen);
        }
break;
case 217:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_UnionIDSeen);
        }
break;
case 218:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_SwitchSeen);
        }
break;
case 219:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_SwitchOpenParSeen);
        }
break;
case 220:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_SwitchTypeSeen);
        }
break;
case 221:
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          UTL_ScopedName        *n = new UTL_ScopedName(tao_yyvsp[-8].idval, NULL);
          AST_Union             *u = NULL;
          AST_Decl              *v = NULL;
          UTL_StrList           *p = idl_global->pragmas();

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
case 222:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_UnionSqSeen);
        }
break;
case 223:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_UnionBodySeen);
        }
break;
case 224:
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
case 225:
{
          tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
        }
break;
case 226:
{
          tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
        }
break;
case 227:
{
          /* octets are not allowed*/
          idl_global->err ()->error0 (UTL_Error::EIDL_DISC_TYPE);
          tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
        }
break;
case 228:
{
          tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
        }
break;
case 230:
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          AST_Decl              *d = NULL;
          AST_PredefinedType    *p = NULL;
          AST_Typedef           *t = NULL;
          long                  found = I_FALSE;

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
                  case AST_PredefinedType::PT_ushort:
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
case 234:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_UnionLabelSeen);
        }
break;
case 235:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemSeen);
        }
break;
case 236:
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          UTL_LabellistActiveIterator *l = NULL;
          AST_UnionLabel        *d = NULL;
          AST_UnionBranch       *b = NULL;
          AST_Field             *f = tao_yyvsp[-2].ffval;

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
case 237:
{
          idl_global->err()->syntax_error(idl_global->parse_state());
        }
break;
case 238:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_NoState);
          tao_yyerrok;
        }
break;
case 239:
{
          tao_yyval.llval = new UTL_LabelList(tao_yyvsp[-1].ulval, tao_yyvsp[0].llval);
        }
break;
case 240:
{
          if (tao_yyvsp[-1].llval == NULL)
            tao_yyval.llval = new UTL_LabelList(tao_yyvsp[0].ulval, NULL);
          else {
            tao_yyvsp[-1].llval->nconc(new UTL_LabelList(tao_yyvsp[0].ulval, NULL));
            tao_yyval.llval = tao_yyvsp[-1].llval;
          }
        }
break;
case 241:
{
          tao_yyval.llval = NULL;
        }
break;
case 242:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_DefaultSeen);
        }
break;
case 243:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

          tao_yyval.ulval = idl_global->gen()->
                    create_union_label(AST_UnionLabel::UL_default,
                                       NULL);
        }
break;
case 244:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_CaseSeen);
        }
break;
case 245:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_LabelExprSeen);
        }
break;
case 246:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_LabelColonSeen);

          tao_yyval.ulval = idl_global->gen()->create_union_label(AST_UnionLabel::UL_label,
                                                     tao_yyvsp[-2].exval);
        }
break;
case 247:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_UnionElemTypeSeen);
        }
break;
case 248:
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
case 249:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_EnumSeen);
        }
break;
case 250:
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          UTL_ScopedName        *n = new UTL_ScopedName(tao_yyvsp[0].idval, NULL);
          AST_Enum              *e = NULL;
          AST_Decl              *v = NULL;
          UTL_StrList           *p = idl_global->pragmas();

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
case 251:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_EnumSqSeen);
        }
break;
case 252:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_EnumBodySeen);
        }
break;
case 253:
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
case 255:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_EnumCommaSeen);
        }
break;
case 258:
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          UTL_ScopedName        *n =
                new UTL_ScopedName(new Identifier(tao_yyvsp[0].strval), NULL);
          AST_EnumVal           *e = NULL;
          AST_Enum              *c = NULL;
          UTL_StrList           *p = idl_global->pragmas();

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
case 259:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_SequenceCommaSeen);
        }
break;
case 260:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_SequenceExprSeen);
        }
break;
case 261:
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
              tao_yyval.dcval = idl_global->gen()->create_sequence(tao_yyvsp[-2].exval,
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
case 262:
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
case 263:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSeen);
          /*
           * Push a sequence marker on scopes stack
           */
          idl_global->scopes()->push(NULL);
        }
break;
case 264:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_SequenceSqSeen);
        }
break;
case 265:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_SequenceTypeSeen);
          tao_yyval.dcval = tao_yyvsp[0].dcval;
        }
break;
case 266:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        }
break;
case 267:
{
           idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        }
break;
case 268:
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
case 269:
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
case 270:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        }
break;
case 271:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_StringSqSeen);
        }
break;
case 272:
{
           idl_global->set_parse_state(IDL_GlobalData::PS_StringExprSeen);
        }
break;
case 273:
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
case 274:
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
case 275:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_StringSeen);
        }
break;
case 276:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_ArrayIDSeen);
        }
break;
case 277:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_ArrayCompleted);
          /*
           * Create a node representing an array
           */
          if (tao_yyvsp[0].elval != NULL) {
             tao_yyval.dcval = idl_global->gen()->create_array(new UTL_ScopedName(tao_yyvsp[-2].idval, NULL),
                                                  tao_yyvsp[0].elval->length(), tao_yyvsp[0].elval, 0, 0);
          }
        }
break;
case 278:
{
          tao_yyval.elval = new UTL_ExprList(tao_yyvsp[-1].exval, tao_yyvsp[0].elval);
        }
break;
case 279:
{
          if (tao_yyvsp[-1].elval == NULL)
            tao_yyval.elval = new UTL_ExprList(tao_yyvsp[0].exval, NULL);
          else {
            tao_yyvsp[-1].elval->nconc(new UTL_ExprList(tao_yyvsp[0].exval, NULL));
            tao_yyval.elval = tao_yyvsp[-1].elval;
          }
        }
break;
case 280:
{
          tao_yyval.elval = NULL;
        }
break;
case 281:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_DimSqSeen);
        }
break;
case 282:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_DimExprSeen);
        }
break;
case 283:
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
case 284:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_AttrSeen);
        }
break;
case 285:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_AttrTypeSeen);
        }
break;
case 286:
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          UTL_DecllistActiveIterator *l = NULL;
          AST_Attribute         *a = NULL;
          FE_Declarator         *d = NULL;
          UTL_StrList           *p = idl_global->pragmas();

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
              a = idl_global->gen()->create_attribute(tao_yyvsp[-5].bval,
                                                      tp,
                                                      (UTL_IdList *) d->name()->copy (),
                                                      p,
                                                      s->is_local (),
                                                      s->is_abstract ());
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
case 287:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_AttrROSeen);
          tao_yyval.bval = I_TRUE;
        }
break;
case 288:
{
          tao_yyval.bval = I_FALSE;
        }
break;
case 289:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSeen);
        }
break;
case 290:
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          UTL_ScopedName        *n = new UTL_ScopedName(tao_yyvsp[0].idval, NULL);
          AST_Exception         *e = NULL;
          UTL_StrList           *p = idl_global->pragmas();
          AST_Decl              *v = NULL;

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
case 291:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_ExceptSqSeen);
        }
break;
case 292:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_ExceptBodySeen);
        }
break;
case 293:
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
case 294:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpTypeSeen);
        }
break;
case 295:
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          UTL_ScopedName        *n =
                new UTL_ScopedName(new Identifier(tao_yyvsp[0].strval), NULL);
          AST_Operation         *o = NULL;
          UTL_StrList           *p = idl_global->pragmas();

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
              o = idl_global->gen()->create_operation(tp,
                                                      tao_yyvsp[-3].ofval,
                                                      n,
                                                      p,
                                                      s->is_local (),
                                                      s->is_abstract ());
              (void) s->fe_add_operation(o);
            }
          }
          /*
           * Push the operation scope onto the scopes stack
           */
          idl_global->scopes()->push(o);
        }
break;
case 296:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpParsCompleted);
        }
break;
case 297:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseCompleted);
        }
break;
case 298:
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          AST_Operation         *o = NULL;

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
case 299:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
          tao_yyval.ofval = AST_Operation::OP_oneway;
        }
break;
case 300:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpAttrSeen);
          tao_yyval.ofval = AST_Operation::OP_idempotent;
        }
break;
case 301:
{
          tao_yyval.ofval = AST_Operation::OP_noflags;
        }
break;
case 303:
{
          tao_yyval.dcval =
            idl_global->scopes()->bottom()
               ->lookup_primitive_type(AST_Expression::EV_void);
        }
break;
case 304:
{
           /* TODO: replace parameter_list with rule that accepts only IN args*/
           cerr << "error in " << idl_global->filename()->get_string()
                << " line " << idl_global->lineno() << ":\n" ;
           cerr << "Sorry, I (TAO_IDL) can't handle factory yet\n";
        }
break;
case 305:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        }
break;
case 306:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        }
break;
case 307:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpSqSeen);
        }
break;
case 308:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpQsSeen);
        }
break;
case 310:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpParCommaSeen);
        }
break;
case 313:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpParDirSeen);
        }
break;
case 314:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpParTypeSeen);
        }
break;
case 315:
{
          UTL_Scope             *s = idl_global->scopes()->top_non_null();
          AST_Argument          *a = NULL;
          UTL_StrList           *p = idl_global->pragmas();

          idl_global->set_parse_state(IDL_GlobalData::PS_OpParDeclSeen);
          /*
           * Create a node representing an argument to an operation
           * Add it to the enclosing scope (the operation scope)
           */
          if (tao_yyvsp[-2].dcval != NULL && tao_yyvsp[0].deval != NULL && s != NULL) {
            AST_Type *tp = tao_yyvsp[0].deval->compose(tao_yyvsp[-2].dcval);
            if (tp != NULL) {
              if (!s->is_local () && tp->is_local ())
                {
                  cerr << "error in " << idl_global->filename()->get_string()
                       << " line " << idl_global->lineno() << ":\n" ;
                  cerr << "Cannot use a local type as an argument of a remote interface operation\n";
                }
              else
                {
                  a = idl_global->gen()->create_argument(tao_yyvsp[-4].dival, tp, (UTL_IdList *) tao_yyvsp[0].deval->name ()->copy (), p);
                  (void) s->fe_add_argument(a);
                }
            }
          }
        }
break;
case 316:
{
          tao_yyval.dcval = idl_global->scopes()->bottom()->lookup_primitive_type(tao_yyvsp[0].etval);
        }
break;
case 319:
{
          UTL_Scope     *s = idl_global->scopes()->top_non_null();
          AST_Decl      *d = NULL;

          if (s != NULL)
            d = s->lookup_by_name(tao_yyvsp[0].idlist, I_TRUE);
          if (d == NULL)
            idl_global->err()->lookup_error(tao_yyvsp[0].idlist);
          tao_yyval.dcval = d;
        }
break;
case 320:
{
          tao_yyval.dival = AST_Argument::dir_IN;
        }
break;
case 321:
{
          tao_yyval.dival = AST_Argument::dir_OUT;
        }
break;
case 322:
{
          tao_yyval.dival = AST_Argument::dir_INOUT;
        }
break;
case 323:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSeen);
        }
break;
case 324:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseSqSeen);
        }
break;
case 325:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpRaiseQsSeen);
          tao_yyval.nlval = tao_yyvsp[-1].nlval;
        }
break;
case 326:
{
          tao_yyval.nlval = NULL;
        }
break;
case 327:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSeen);
        }
break;
case 328:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpContextSqSeen);
        }
break;
case 329:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpContextQsSeen);
          tao_yyval.slval = tao_yyvsp[-1].slval;
        }
break;
case 330:
{
          tao_yyval.slval = NULL;
        }
break;
case 331:
{
          tao_yyval.slval = new UTL_StrList(tao_yyvsp[-1].sval, tao_yyvsp[0].slval);
        }
break;
case 332:
{
          idl_global->set_parse_state(IDL_GlobalData::PS_OpContextCommaSeen);
        }
break;
case 333:
{
          if (tao_yyvsp[-3].slval == NULL)
            tao_yyval.slval = new UTL_StrList(tao_yyvsp[0].sval, NULL);
          else {
            tao_yyvsp[-3].slval->nconc(new UTL_StrList(tao_yyvsp[0].sval, NULL));
            tao_yyval.slval = tao_yyvsp[-3].slval;
          }
        }
break;
case 334:
{
          tao_yyval.slval = NULL;
        }
break;
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
