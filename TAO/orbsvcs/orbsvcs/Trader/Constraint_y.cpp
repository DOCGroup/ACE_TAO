/* original parser id follows */
/* yysccsid[] = "@(#)yaccpar        1.9 (Berkeley) 02/21/93" */
/* (use YYMAJOR/YYMINOR for ifdefs dependent on parser version) */

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20140715

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)
#define YYENOMEM       (-2)
#define YYEOF          0
#define YYPREFIX "yy"

#define YYPURE 0

  /* ========================================================================*/
/**/
/* = LIBRARY*/
/*    orbsvcs*/
/* */
/* = FILENAME*/
/*    constraint.y*/
/**/
/* = AUTHOR*/
/*    Seth Widoff <sbw1@cs.wustl.edu>*/
/**/
/* ========================================================================*/

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdlib.h"
#include "orbsvcs/Trader/Constraint_Interpreter.h"
#include "orbsvcs/Trader/Constraint_Nodes.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/*#define YYDEBUG 1*/

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define TAO_GT 257
#define TAO_GE 258
#define TAO_LT 259
#define TAO_LE 260
#define TAO_EQ 261
#define TAO_NE 262
#define TAO_EXIST 263
#define TAO_AND 264
#define TAO_OR 265
#define TAO_NOT 266
#define TAO_IN 267
#define TAO_TWIDDLE 268
#define TAO_BOOLEAN 269
#define TAO_PLUS 270
#define TAO_MINUS 271
#define TAO_MULT 272
#define TAO_DIV 273
#define TAO_UMINUS 274
#define TAO_NUMBER 275
#define TAO_RPAREN 276
#define TAO_LPAREN 277
#define TAO_IDENT 278
#define TAO_STRING 279
#define TAO_UNKNOWN 280
#define TAO_SIGNED 281
#define TAO_UNSIGNED 282
#define TAO_DOUBLE 283
#define TAO_CONSTRAINT 284
#define TAO_SEQUENCE 285
#define TAO_WITH 286
#define TAO_MAX 287
#define TAO_MIN 288
#define TAO_FIRST 289
#define TAO_RANDOM 290
#define YYERRCODE 256
typedef short YYINT;
static const YYINT yylhs[] = {                           -1,
    0,    0,    1,    1,    1,    1,    1,    2,    2,    3,
    3,    4,    4,    4,    4,    4,    4,    4,    5,    5,
    6,    6,    7,    7,    7,    8,    8,    8,    9,    9,
   10,   10,   10,   10,   10,   10,   10,
};
static const YYINT yylen[] = {                            2,
    1,    1,    2,    2,    2,    1,    1,    3,    1,    3,
    1,    3,    3,    3,    3,    3,    3,    1,    3,    1,
    3,    1,    3,    3,    1,    3,    3,    1,    2,    1,
    3,    2,    1,    1,    2,    1,    1,
};
static const YYINT yydefred[] = {                         0,
    0,    0,   37,    0,   34,    0,   33,   36,    0,    0,
    0,    6,    7,    0,    2,    0,    0,   11,    0,    0,
    0,    0,   28,   30,   32,   29,   35,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   31,    0,   10,   14,   15,
   16,   17,   12,   13,   19,    0,    0,    0,   26,   27,
};
static const YYINT yydgoto[] = {                         14,
   15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
};
static const YYINT yysindex[] = {                      -253,
 -278, -219,    0, -271,    0, -224,    0,    0, -224, -224,
 -224,    0,    0,    0,    0, -256, -252,    0, -195, -244,
 -251, -243,    0,    0,    0,    0,    0, -262, -256, -256,
 -256, -224, -224, -224, -224, -224, -224, -224, -224, -263,
 -224, -224, -224, -224, -224,    0, -252,    0,    0,    0,
    0,    0,    0,    0,    0, -239, -243, -243,    0,    0,
};
static const YYINT yyrindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   27,    8,    0,   11,   87,
   61,    1,    0,    0,    0,    0,    0,    0,   38,   40,
   43,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   28,    0,    0,    0,
    0,    0,    0,    0,    0,   74,   21,   41,    0,    0,
};
static const YYINT yygindex[] = {                         0,
    0,   -4,   14,   24,   34,    0,   36,    6,   31,   76,
};
#define YYTABLESIZE 363
static const YYINT yytable[] = {                         25,
   25,   28,   32,   27,   29,   30,   31,    9,   32,    1,
   18,   33,    2,   46,   55,    3,   41,    4,   42,   43,
   23,    5,   40,    6,    7,    8,    1,    8,   44,   45,
   42,   43,    9,   10,   11,   12,   13,    5,    1,    4,
   24,    2,    3,    1,    3,   47,    4,   57,   58,    3,
    5,    4,    6,    7,    8,    5,   48,    6,    7,    8,
   22,   34,   35,   36,   37,   38,   39,   49,   50,   51,
   52,   53,   54,   21,   59,   60,   56,   26,    0,    0,
    0,    0,    0,    0,    0,    0,   20,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   25,   25,   25,
   25,   25,   25,    0,   25,   25,    0,   25,   25,    0,
   25,   25,    9,    0,   18,   18,   25,   23,   23,   23,
   23,   23,   23,    9,   23,   23,   18,   23,   23,    0,
   23,   23,    8,    0,    0,    0,   23,   24,   24,   24,
   24,   24,   24,    8,   24,   24,    0,   24,   24,    0,
   24,   24,    0,    0,    0,    0,   24,   22,   22,   22,
   22,   22,   22,    0,   22,   22,    0,   22,    0,    0,
   21,   21,   21,   21,   21,   21,   22,   21,   21,    0,
   21,    0,    0,   20,   20,   20,   20,   20,   20,   21,
   20,   20,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   20,
};
static const YYINT yycheck[] = {                        278,
    0,    6,  265,  275,    9,   10,   11,    0,  265,  263,
    0,  264,  266,  276,  278,  269,  268,  271,  270,  271,
    0,  275,  267,  277,  278,  279,    0,    0,  272,  273,
  270,  271,  286,  287,  288,  289,  290,    0,  263,    0,
    0,  266,    0,  263,  269,   32,  271,   42,   43,  269,
  275,  271,  277,  278,  279,  275,   33,  277,  278,  279,
    0,  257,  258,  259,  260,  261,  262,   34,   35,   36,
   37,   38,   39,    0,   44,   45,   41,    2,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,    0,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  257,  258,  259,
  260,  261,  262,   -1,  264,  265,   -1,  267,  268,   -1,
  270,  271,  265,   -1,  264,  265,  276,  257,  258,  259,
  260,  261,  262,  276,  264,  265,  276,  267,  268,   -1,
  270,  271,  265,   -1,   -1,   -1,  276,  257,  258,  259,
  260,  261,  262,  276,  264,  265,   -1,  267,  268,   -1,
  270,  271,   -1,   -1,   -1,   -1,  276,  257,  258,  259,
  260,  261,  262,   -1,  264,  265,   -1,  267,   -1,   -1,
  257,  258,  259,  260,  261,  262,  276,  264,  265,   -1,
  267,   -1,   -1,  257,  258,  259,  260,  261,  262,  276,
  264,  265,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,  276,
};
#define YYFINAL 14
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 290
#define YYUNDFTOKEN 303
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? YYUNDFTOKEN : (a))
#if YYDEBUG
static const char *const yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"TAO_GT","TAO_GE","TAO_LT",
"TAO_LE","TAO_EQ","TAO_NE","TAO_EXIST","TAO_AND","TAO_OR","TAO_NOT","TAO_IN",
"TAO_TWIDDLE","TAO_BOOLEAN","TAO_PLUS","TAO_MINUS","TAO_MULT","TAO_DIV",
"TAO_UMINUS","TAO_NUMBER","TAO_RPAREN","TAO_LPAREN","TAO_IDENT","TAO_STRING",
"TAO_UNKNOWN","TAO_SIGNED","TAO_UNSIGNED","TAO_DOUBLE","TAO_CONSTRAINT",
"TAO_SEQUENCE","TAO_WITH","TAO_MAX","TAO_MIN","TAO_FIRST","TAO_RANDOM",0,0,0,0,
0,0,0,0,0,0,0,0,"illegal-symbol",
};
static const char *const yyrule[] = {
"$accept : constraint",
"constraint : bool_or",
"constraint : preference",
"preference : TAO_MIN bool_or",
"preference : TAO_MAX bool_or",
"preference : TAO_WITH bool_or",
"preference : TAO_FIRST",
"preference : TAO_RANDOM",
"bool_or : bool_or TAO_OR bool_and",
"bool_or : bool_and",
"bool_and : bool_and TAO_AND bool_compare",
"bool_and : bool_compare",
"bool_compare : expr_in TAO_EQ expr_in",
"bool_compare : expr_in TAO_NE expr_in",
"bool_compare : expr_in TAO_GT expr_in",
"bool_compare : expr_in TAO_GE expr_in",
"bool_compare : expr_in TAO_LT expr_in",
"bool_compare : expr_in TAO_LE expr_in",
"bool_compare : expr_in",
"expr_in : expr_twiddle TAO_IN TAO_IDENT",
"expr_in : expr_twiddle",
"expr_twiddle : expr TAO_TWIDDLE expr",
"expr_twiddle : expr",
"expr : expr TAO_PLUS term",
"expr : expr TAO_MINUS term",
"expr : term",
"term : term TAO_MULT factor_not",
"term : term TAO_DIV factor_not",
"term : factor_not",
"factor_not : TAO_NOT factor",
"factor_not : factor",
"factor : TAO_LPAREN bool_or TAO_RPAREN",
"factor : TAO_EXIST TAO_IDENT",
"factor : TAO_IDENT",
"factor : TAO_NUMBER",
"factor : TAO_MINUS TAO_NUMBER",
"factor : TAO_STRING",
"factor : TAO_BOOLEAN",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#define YYINITSTACKSIZE 200

typedef struct {
    unsigned stacksize;
    YYINT    *s_base;
    YYINT    *s_mark;
    YYINT    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;

//extern int yydebug = 1;

#if YYDEBUG
#include <stdio.h>                /* needed for printf */
#endif

#include <stdlib.h>        /* needed for malloc, etc */
#include <string.h>        /* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    YYINT *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return YYENOMEM;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (YYINT *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return YYENOMEM;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return YYENOMEM;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == 0 && yygrowstack(&yystack) == YYENOMEM) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = YYEOF;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
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

    YYERROR_CALL("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == YYEOF) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
        { yyval.constraint_ = new TAO_Unary_Constraint(TAO_CONSTRAINT, yystack.l_mark[0].constraint_); }
break;
case 2:
        { yyval.constraint_ = yystack.l_mark[0].constraint_; }
break;
case 3:
        { yyval.constraint_ = new TAO_Unary_Constraint(TAO_MIN, yystack.l_mark[0].constraint_); }
break;
case 4:
        { yyval.constraint_ = new TAO_Unary_Constraint(TAO_MAX, yystack.l_mark[0].constraint_); }
break;
case 5:
        { yyval.constraint_ = new TAO_Unary_Constraint(TAO_WITH, yystack.l_mark[0].constraint_); }
break;
case 6:
        { yyval.constraint_ = new TAO_Noop_Constraint(TAO_FIRST); }
break;
case 7:
        { yyval.constraint_ = new TAO_Noop_Constraint(TAO_RANDOM); }
break;
case 8:
        { yyval.constraint_ = new TAO_Binary_Constraint(TAO_OR, yystack.l_mark[-2].constraint_, yystack.l_mark[0].constraint_); }
break;
case 9:
        { yyval.constraint_ = yystack.l_mark[0].constraint_; }
break;
case 10:
        { yyval.constraint_ = new TAO_Binary_Constraint(TAO_AND, yystack.l_mark[-2].constraint_, yystack.l_mark[0].constraint_); }
break;
case 11:
        { yyval.constraint_ = yystack.l_mark[0].constraint_; }
break;
case 12:
        { yyval.constraint_ = new TAO_Binary_Constraint(TAO_EQ, yystack.l_mark[-2].constraint_, yystack.l_mark[0].constraint_); }
break;
case 13:
        { yyval.constraint_ = new TAO_Binary_Constraint(TAO_NE, yystack.l_mark[-2].constraint_, yystack.l_mark[0].constraint_); }
break;
case 14:
        { yyval.constraint_ = new TAO_Binary_Constraint(TAO_GT, yystack.l_mark[-2].constraint_, yystack.l_mark[0].constraint_); }
break;
case 15:
        { yyval.constraint_ = new TAO_Binary_Constraint(TAO_GE, yystack.l_mark[-2].constraint_, yystack.l_mark[0].constraint_); }
break;
case 16:
        { yyval.constraint_ = new TAO_Binary_Constraint(TAO_LT, yystack.l_mark[-2].constraint_, yystack.l_mark[0].constraint_); }
break;
case 17:
        { yyval.constraint_ = new TAO_Binary_Constraint(TAO_LE, yystack.l_mark[-2].constraint_, yystack.l_mark[0].constraint_); }
break;
case 18:
        { yyval.constraint_ = yystack.l_mark[0].constraint_; }
break;
case 19:
        { yyval.constraint_ = new TAO_Binary_Constraint(TAO_IN, yystack.l_mark[-2].constraint_, yystack.l_mark[0].constraint_); }
break;
case 20:
        { yyval.constraint_ = yystack.l_mark[0].constraint_; }
break;
case 21:
        { yyval.constraint_ = new TAO_Binary_Constraint(TAO_TWIDDLE, yystack.l_mark[-2].constraint_, yystack.l_mark[0].constraint_); }
break;
case 22:
        { yyval.constraint_ = yystack.l_mark[0].constraint_; }
break;
case 23:
        { yyval.constraint_ = new TAO_Binary_Constraint(TAO_PLUS, yystack.l_mark[-2].constraint_, yystack.l_mark[0].constraint_); }
break;
case 24:
        { yyval.constraint_ = new TAO_Binary_Constraint(TAO_MINUS, yystack.l_mark[-2].constraint_, yystack.l_mark[0].constraint_); }
break;
case 25:
        { yyval.constraint_ = yystack.l_mark[0].constraint_; }
break;
case 26:
        { yyval.constraint_ = new TAO_Binary_Constraint(TAO_MULT, yystack.l_mark[-2].constraint_, yystack.l_mark[0].constraint_); }
break;
case 27:
        { yyval.constraint_ = new TAO_Binary_Constraint(TAO_DIV, yystack.l_mark[-2].constraint_, yystack.l_mark[0].constraint_); }
break;
case 28:
        { yyval.constraint_ = yystack.l_mark[0].constraint_; }
break;
case 29:
        { yyval.constraint_ = new TAO_Unary_Constraint(TAO_NOT, yystack.l_mark[0].constraint_); }
break;
case 30:
        { yyval.constraint_ = yystack.l_mark[0].constraint_; }
break;
case 31:
        { yyval.constraint_ = yystack.l_mark[-1].constraint_; }
break;
case 32:
        { yyval.constraint_ = new TAO_Unary_Constraint(TAO_EXIST, yystack.l_mark[0].constraint_); }
break;
case 33:
        { yyval.constraint_ = yystack.l_mark[0].constraint_; }
break;
case 34:
        { yyval.constraint_ = yystack.l_mark[0].constraint_; }
break;
case 35:
        { yyval.constraint_ = new TAO_Unary_Constraint(TAO_UMINUS, yystack.l_mark[0].constraint_); }
break;
case 36:
        { yyval.constraint_ = yystack.l_mark[0].constraint_; }
break;
case 37:
        { yyval.constraint_ = yystack.l_mark[0].constraint_; }
break;
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = YYEOF;
#if YYDEBUG
            if (yydebug)
            {
                yys = yyname[YYTRANSLATE(yychar)];
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == YYEOF) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack) == YYENOMEM)
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (YYINT) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    YYERROR_CALL("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
TAO_END_VERSIONED_NAMESPACE_DECL
