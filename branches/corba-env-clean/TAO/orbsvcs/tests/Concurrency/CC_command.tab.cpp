//$Id$
#ifndef lint
static char const
ace_cc_yyrcsid[] = "$FreeBSD: src/usr.bin/yacc/skeleton.c,v 1.28 2000/01/17 02:04:06 bde Exp $";
#endif
#include <stdlib.h>
#define ACE_CC_YYBYACC 1
#define ACE_CC_YYMAJOR 1
#define ACE_CC_YYMINOR 9
#define ACE_CC_YYLEX ace_cc_yylex()
#define ACE_CC_YYEMPTY -1
#define ace_cc_yyclearin (ace_cc_yychar=(ACE_CC_YYEMPTY))
#define ace_cc_yyerrok (ace_cc_yyerrflag=0)
#define ACE_CC_YYRECOVERING() (ace_cc_yyerrflag!=0)
static int ace_cc_yygrowstack();
#define ACE_CC_YYPREFIX "ace_cc_yy"
#include "CC_command.h"
#include "ace/ACE.h"
#include "orbsvcs/CosConcurrencyControlC.h"

void ace_cc_yyerror (const char* s);
int ace_cc_yylex(void);

extern int line_no;
extern char line_buf[500];
extern char *ace_cc_yytext;
extern CC_CommandList *cmdlist;
typedef union {
  CC_Command *command;
  CosConcurrencyControl::lock_mode lock_mode;
  char *id;
  int num;
} ACE_CC_YYSTYPE;
#define ACE_CC_YYERRCODE 256
#define T_START_CMD 257
#define T_CREATE_CMD 258
#define T_LOCK_CMD 259
#define T_UNLOCK_CMD 260
#define T_TRYLOCK_CMD 261
#define T_CHANGEMODE_CMD 262
#define T_SLEEP_CMD 263
#define T_WAIT_CMD 264
#define T_REPEAT_CMD 265
#define T_EXCEP_CMD 266
#define T_PRINT_CMD 267
#define T_LOOKUP_CMD 268
#define T_TERM 269
#define T_READ 270
#define T_IREAD 271
#define T_UPGRADE 272
#define T_WRITE 273
#define T_IWRITE 274
#define T_IDENT 275
#define Q_STRING 276
#define T_NUM 277
const short ace_cc_yylhs[] = {                                        -1,
    0,    0,    6,    6,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    2,    3,    4,    5,    5,    5,    5,    5,
};
const short ace_cc_yylen[] = {                                         2,
    0,    1,    2,    1,    3,    3,    2,    4,    3,    4,
    3,    4,    3,    5,    4,    3,    2,    3,    3,    3,
    3,    3,    1,    1,    1,    1,    1,    1,    1,    1,
};
const short ace_cc_yydefred[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    4,    0,   23,    0,    7,   24,    0,
   26,   27,   28,   29,   30,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   17,    0,    0,   25,    0,    0,
    0,    3,    5,    6,    0,    9,    0,   11,    0,   13,
    0,    0,   16,   18,   21,   22,   19,   20,    8,   10,
   12,    0,   15,   14,
};
const short ace_cc_yydgoto[] = {                                      13,
   14,   17,   20,   39,   27,   15,
};
const short ace_cc_yysindex[] = {                                   -254,
 -260, -239, -233, -233, -233, -233, -256, -249, -246, -252,
 -244, -242,    0,    0, -254,    0, -235,    0,    0, -234,
    0,    0,    0,    0,    0, -222, -226, -222, -225, -222,
 -224, -222, -222, -223,    0, -216, -215,    0, -214, -213,
 -212,    0,    0,    0, -211,    0, -210,    0, -209,    0,
 -222, -208,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -207,    0,    0,
};
const short ace_cc_yyrindex[] = {                                     63,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   64,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,
};
const short ace_cc_yygindex[] = {                                      0,
   50,    0,   13,    0,   -4,    0,
};
#define ACE_CC_YYTABLESIZE 65
const short ace_cc_yytable[] = {                                      29,
   31,   33,    1,    2,    3,    4,    5,    6,    7,    8,
    9,   10,   11,   12,   16,   26,   28,   30,   32,   35,
   34,   45,   38,   47,   41,   49,   36,   51,   52,   18,
   37,   40,   19,   43,   44,   19,   21,   22,   23,   24,
   25,   19,   46,   48,   50,   53,   62,   21,   22,   23,
   24,   25,   54,   55,   56,   57,   58,   59,   60,   61,
   63,   64,    1,    2,   42,
};
const short ace_cc_yycheck[] = {                                       4,
    5,    6,  257,  258,  259,  260,  261,  262,  263,  264,
  265,  266,  267,  268,  275,    3,    4,    5,    6,  269,
  277,   26,  275,   28,   12,   30,  276,   32,   33,  269,
  277,  276,  275,  269,  269,  275,  270,  271,  272,  273,
  274,  275,  269,  269,  269,  269,   51,  270,  271,  272,
  273,  274,  269,  269,  269,  269,  269,  269,  269,  269,
  269,  269,    0,    0,   15,
};
#define ACE_CC_YYFINAL 13
#ifndef ACE_CC_YYDEBUG
#define ACE_CC_YYDEBUG 0
#endif
#define ACE_CC_YYMAXTOKEN 277
#if ACE_CC_YYDEBUG
const char * const ace_cc_yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"T_START_CMD","T_CREATE_CMD",
"T_LOCK_CMD","T_UNLOCK_CMD","T_TRYLOCK_CMD","T_CHANGEMODE_CMD","T_SLEEP_CMD",
"T_WAIT_CMD","T_REPEAT_CMD","T_EXCEP_CMD","T_PRINT_CMD","T_LOOKUP_CMD","T_TERM",
"T_READ","T_IREAD","T_UPGRADE","T_WRITE","T_IWRITE","T_IDENT","Q_STRING",
"T_NUM",
};
const char * const ace_cc_yyrule[] = {
"$accept : start",
"start :",
"start : cmd_list",
"cmd_list : cmd_list cmd",
"cmd_list : cmd",
"cmd : T_START_CMD config_file_name T_TERM",
"cmd : T_CREATE_CMD lock_set_name T_TERM",
"cmd : T_CREATE_CMD T_TERM",
"cmd : T_LOCK_CMD lock_set_name lock_mode T_TERM",
"cmd : T_LOCK_CMD lock_mode T_TERM",
"cmd : T_UNLOCK_CMD lock_set_name lock_mode T_TERM",
"cmd : T_UNLOCK_CMD lock_mode T_TERM",
"cmd : T_TRYLOCK_CMD lock_set_name lock_mode T_TERM",
"cmd : T_TRYLOCK_CMD lock_mode T_TERM",
"cmd : T_CHANGEMODE_CMD lock_set_name lock_mode lock_mode T_TERM",
"cmd : T_CHANGEMODE_CMD lock_mode lock_mode T_TERM",
"cmd : T_SLEEP_CMD T_NUM T_TERM",
"cmd : T_WAIT_CMD T_TERM",
"cmd : T_WAIT_CMD Q_STRING T_TERM",
"cmd : T_PRINT_CMD Q_STRING T_TERM",
"cmd : T_LOOKUP_CMD lock_set_name T_TERM",
"cmd : T_REPEAT_CMD T_NUM T_TERM",
"cmd : T_EXCEP_CMD exception_name T_TERM",
"config_file_name : T_IDENT",
"lock_set_name : T_IDENT",
"exception_name : T_IDENT",
"lock_mode : T_READ",
"lock_mode : T_IREAD",
"lock_mode : T_UPGRADE",
"lock_mode : T_WRITE",
"lock_mode : T_IWRITE",
};
#endif
#if ACE_CC_YYDEBUG
#include <stdio.h>
#endif
#ifdef ACE_CC_YYSTACKSIZE
#undef ACE_CC_YYMAXDEPTH
#define ACE_CC_YYMAXDEPTH ACE_CC_YYSTACKSIZE
#else
#ifdef ACE_CC_YYMAXDEPTH
#define ACE_CC_YYSTACKSIZE ACE_CC_YYMAXDEPTH
#else
#define ACE_CC_YYSTACKSIZE 10000
#define ACE_CC_YYMAXDEPTH 10000
#endif
#endif
#define ACE_CC_YYINITSTACKSIZE 200
int ace_cc_yydebug;
int ace_cc_yynerrs;
int ace_cc_yyerrflag;
int ace_cc_yychar;
short *ace_cc_yyssp;
ACE_CC_YYSTYPE *ace_cc_yyvsp;
ACE_CC_YYSTYPE ace_cc_yyval;
ACE_CC_YYSTYPE ace_cc_yylval;
short *ace_cc_yyss;
short *ace_cc_yysslim;
ACE_CC_YYSTYPE *ace_cc_yyvs;
int ace_cc_yystacksize;

void
ace_cc_yyerror (const char *s)
{
  ACE_OS::printf ("%d: %s at %s in:\n%s\n",
                  line_no,
                  s,
                  ace_cc_yytext,
                  line_buf);
  ACE_OS::exit (-1);
}
/* allocate initial stack or double stack size, up to ACE_CC_YYMAXDEPTH */
static int ace_cc_yygrowstack()
{
    int newsize, i;
    short *newss;
    ACE_CC_YYSTYPE *newvs;

    if ((newsize = ace_cc_yystacksize) == 0)
        newsize = ACE_CC_YYINITSTACKSIZE;
    else if (newsize >= ACE_CC_YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > ACE_CC_YYMAXDEPTH)
        newsize = ACE_CC_YYMAXDEPTH;
    i = ace_cc_yyssp - ace_cc_yyss;
    newss = ace_cc_yyss ? (short *)realloc(ace_cc_yyss, newsize * sizeof *newss) :
      (short *)malloc(newsize * sizeof *newss);
    if (newss == NULL)
        return -1;
    ace_cc_yyss = newss;
    ace_cc_yyssp = newss + i;
    newvs = ace_cc_yyvs ? (ACE_CC_YYSTYPE *)realloc(ace_cc_yyvs, newsize * sizeof *newvs) :
      (ACE_CC_YYSTYPE *)malloc(newsize * sizeof *newvs);
    if (newvs == NULL)
        return -1;
    ace_cc_yyvs = newvs;
    ace_cc_yyvsp = newvs + i;
    ace_cc_yystacksize = newsize;
    ace_cc_yysslim = ace_cc_yyss + newsize - 1;
    return 0;
}

#define ACE_CC_YYABORT goto ace_cc_yyabort
#define ACE_CC_YYREJECT goto ace_cc_yyabort
#define ACE_CC_YYACCEPT goto ace_cc_yyaccept
#define ACE_CC_YYERROR goto ace_cc_yyerrlab

#ifndef ACE_CC_YYPARSE_PARAM
#if defined(__cplusplus) || __STDC__
#define ACE_CC_YYPARSE_PARAM_ARG void
#define ACE_CC_YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define ACE_CC_YYPARSE_PARAM_ARG
#define ACE_CC_YYPARSE_PARAM_DECL
#endif	/* ANSI-C/C++ */
#else	/* ACE_CC_YYPARSE_PARAM */
#ifndef ACE_CC_YYPARSE_PARAM_TYPE
#define ACE_CC_YYPARSE_PARAM_TYPE void *
#endif
#if defined(__cplusplus) || __STDC__
#define ACE_CC_YYPARSE_PARAM_ARG ACE_CC_YYPARSE_PARAM_TYPE ACE_CC_YYPARSE_PARAM
#define ACE_CC_YYPARSE_PARAM_DECL
#else	/* ! ANSI-C/C++ */
#define ACE_CC_YYPARSE_PARAM_ARG ACE_CC_YYPARSE_PARAM
#define ACE_CC_YYPARSE_PARAM_DECL ACE_CC_YYPARSE_PARAM_TYPE ACE_CC_YYPARSE_PARAM;
#endif	/* ANSI-C/C++ */
#endif	/* ! ACE_CC_YYPARSE_PARAM */

int
ace_cc_yyparse (ACE_CC_YYPARSE_PARAM_ARG)
    ACE_CC_YYPARSE_PARAM_DECL
{
    register int ace_cc_yym, ace_cc_yyn, ace_cc_yystate;
#if ACE_CC_YYDEBUG
    register const char *ace_cc_yys;

    if ((ace_cc_yys = ACE_OS::getenv("ACE_CC_YYDEBUG")))
    {
        ace_cc_yyn = *ace_cc_yys;
        if (ace_cc_yyn >= '0' && ace_cc_yyn <= '9')
            ace_cc_yydebug = ace_cc_yyn - '0';
    }
#endif

    ace_cc_yynerrs = 0;
    ace_cc_yyerrflag = 0;
    ace_cc_yychar = (-1);

    if (ace_cc_yyss == NULL && ace_cc_yygrowstack()) goto ace_cc_yyoverflow;
    ace_cc_yyssp = ace_cc_yyss;
    ace_cc_yyvsp = ace_cc_yyvs;
    *ace_cc_yyssp = ace_cc_yystate = 0;

ace_cc_yyloop:
    if ((ace_cc_yyn = ace_cc_yydefred[ace_cc_yystate])) goto ace_cc_yyreduce;
    if (ace_cc_yychar < 0)
    {
        if ((ace_cc_yychar = ace_cc_yylex()) < 0) ace_cc_yychar = 0;
#if ACE_CC_YYDEBUG
        if (ace_cc_yydebug)
        {
            ace_cc_yys = 0;
            if (ace_cc_yychar <= ACE_CC_YYMAXTOKEN) ace_cc_yys = ace_cc_yyname[ace_cc_yychar];
            if (!ace_cc_yys) ace_cc_yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    ACE_CC_YYPREFIX, ace_cc_yystate, ace_cc_yychar, ace_cc_yys);
        }
#endif
    }
    if ((ace_cc_yyn = ace_cc_yysindex[ace_cc_yystate]) && (ace_cc_yyn += ace_cc_yychar) >= 0 &&
            ace_cc_yyn <= ACE_CC_YYTABLESIZE && ace_cc_yycheck[ace_cc_yyn] == ace_cc_yychar)
    {
#if ACE_CC_YYDEBUG
        if (ace_cc_yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    ACE_CC_YYPREFIX, ace_cc_yystate, ace_cc_yytable[ace_cc_yyn]);
#endif
        if (ace_cc_yyssp >= ace_cc_yysslim && ace_cc_yygrowstack())
        {
            goto ace_cc_yyoverflow;
        }
        *++ace_cc_yyssp = ace_cc_yystate = ace_cc_yytable[ace_cc_yyn];
        *++ace_cc_yyvsp = ace_cc_yylval;
        ace_cc_yychar = (-1);
        if (ace_cc_yyerrflag > 0)  --ace_cc_yyerrflag;
        goto ace_cc_yyloop;
    }
    if ((ace_cc_yyn = ace_cc_yyrindex[ace_cc_yystate]) && (ace_cc_yyn += ace_cc_yychar) >= 0 &&
            ace_cc_yyn <= ACE_CC_YYTABLESIZE && ace_cc_yycheck[ace_cc_yyn] == ace_cc_yychar)
    {
        ace_cc_yyn = ace_cc_yytable[ace_cc_yyn];
        goto ace_cc_yyreduce;
    }
    if (ace_cc_yyerrflag) goto ace_cc_yyinrecovery;
#if defined(lint) || defined(__GNUC__)

#endif

    ace_cc_yyerror("syntax error");
#if defined(lint) || defined(__GNUC__)
    goto ace_cc_yyerrlab;
#endif
ace_cc_yyerrlab:
    ++ace_cc_yynerrs;
ace_cc_yyinrecovery:
    if (ace_cc_yyerrflag < 3)
    {
        ace_cc_yyerrflag = 3;
        for (;;)
        {
            if ((ace_cc_yyn = ace_cc_yysindex[*ace_cc_yyssp]) && (ace_cc_yyn += ACE_CC_YYERRCODE) >= 0 &&
                    ace_cc_yyn <= ACE_CC_YYTABLESIZE && ace_cc_yycheck[ace_cc_yyn] == ACE_CC_YYERRCODE)
            {
#if ACE_CC_YYDEBUG
                if (ace_cc_yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", ACE_CC_YYPREFIX, *ace_cc_yyssp, ace_cc_yytable[ace_cc_yyn]);
#endif
                if (ace_cc_yyssp >= ace_cc_yysslim && ace_cc_yygrowstack())
                {
                    goto ace_cc_yyoverflow;
                }
                *++ace_cc_yyssp = ace_cc_yystate = ace_cc_yytable[ace_cc_yyn];
                *++ace_cc_yyvsp = ace_cc_yylval;
                goto ace_cc_yyloop;
            }
            else
            {
#if ACE_CC_YYDEBUG
                if (ace_cc_yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            ACE_CC_YYPREFIX, *ace_cc_yyssp);
#endif
                if (ace_cc_yyssp <= ace_cc_yyss) goto ace_cc_yyabort;
                --ace_cc_yyssp;
                --ace_cc_yyvsp;
            }
        }
    }
    else
    {
        if (ace_cc_yychar == 0) goto ace_cc_yyabort;
#if ACE_CC_YYDEBUG
        if (ace_cc_yydebug)
        {
            ace_cc_yys = 0;
            if (ace_cc_yychar <= ACE_CC_YYMAXTOKEN) ace_cc_yys = ace_cc_yyname[ace_cc_yychar];
            if (!ace_cc_yys) ace_cc_yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    ACE_CC_YYPREFIX, ace_cc_yystate, ace_cc_yychar, ace_cc_yys);
        }
#endif
        ace_cc_yychar = (-1);
        goto ace_cc_yyloop;
    }
ace_cc_yyreduce:
#if ACE_CC_YYDEBUG
    if (ace_cc_yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                ACE_CC_YYPREFIX, ace_cc_yystate, ace_cc_yyn, ace_cc_yyrule[ace_cc_yyn]);
#endif
    ace_cc_yym = ace_cc_yylen[ace_cc_yyn];
    ace_cc_yyval = ace_cc_yyvsp[1-ace_cc_yym];
    switch (ace_cc_yyn)
    {
case 2:
{ cmdlist->add(new CC_Sleep_Cmd(0));
			   /* dummy to check exeption in the last command */
		  if(cmdlist->execute()!=0) /*CC_FAIL*/
                    { printf(" ** Test succeded!!\n"); }
                  else
                    { printf(" ** Test FAILED!!\n"); } }
break;
case 3:
{ cmdlist->add(ace_cc_yyvsp[0].command); }
break;
case 4:
{ cmdlist->add(ace_cc_yyvsp[0].command); }
break;
case 5:
{ ace_cc_yyval.command = new CC_Start_Cmd(ace_cc_yyvsp[-1].id); }
break;
case 6:
{ ace_cc_yyval.command = new CC_CreateLockSet_Cmd(ace_cc_yyvsp[-1].id); }
break;
case 7:
{ ace_cc_yyval.command = new CC_CreateLockSet_Cmd(""); }
break;
case 8:
{ ace_cc_yyval.command = new CC_Lock_Cmd(ace_cc_yyvsp[-2].id, ace_cc_yyvsp[-1].lock_mode); }
break;
case 9:
{ ace_cc_yyval.command = new CC_Lock_Cmd("", ace_cc_yyvsp[-1].lock_mode); }
break;
case 10:
{ ace_cc_yyval.command = new CC_UnLock_Cmd(ace_cc_yyvsp[-2].id, ace_cc_yyvsp[-1].lock_mode); }
break;
case 11:
{ ace_cc_yyval.command = new CC_UnLock_Cmd("", ace_cc_yyvsp[-1].lock_mode); }
break;
case 12:
{ ace_cc_yyval.command = new CC_TryLock_Cmd(ace_cc_yyvsp[-2].id, ace_cc_yyvsp[-1].lock_mode); }
break;
case 13:
{ ace_cc_yyval.command = new CC_TryLock_Cmd("", ace_cc_yyvsp[-1].lock_mode); }
break;
case 14:
{ ace_cc_yyval.command = new CC_ChangeMode_Cmd(ace_cc_yyvsp[-3].id, ace_cc_yyvsp[-2].lock_mode, ace_cc_yyvsp[-1].lock_mode); }
break;
case 15:
{ ace_cc_yyval.command = new CC_ChangeMode_Cmd("", ace_cc_yyvsp[-2].lock_mode, ace_cc_yyvsp[-1].lock_mode); }
break;
case 16:
{ ace_cc_yyval.command = new CC_Sleep_Cmd(ace_cc_yyvsp[-1].num); }
break;
case 17:
{ ace_cc_yyval.command = new CC_Wait_Cmd(""); }
break;
case 18:
{ ace_cc_yyval.command = new CC_Wait_Cmd(ace_cc_yyvsp[-1].id); }
break;
case 19:
{ ace_cc_yyval.command = new CC_Print_Cmd(ace_cc_yyvsp[-1].id); }
break;
case 20:
{ ace_cc_yyval.command = new CC_Lookup_Cmd(ace_cc_yyvsp[-1].id); }
break;
case 21:
{ cmdlist->setrepeat(ace_cc_yyvsp[-1].num);
                          ace_cc_yyval.command = new CC_Dummy_Cmd();}
break;
case 22:
{ ace_cc_yyval.command = new CC_Excep_Cmd(ace_cc_yyvsp[-1].id); }
break;
case 23:
{ace_cc_yyval.id = ace_cc_yyvsp[0].id; }
break;
case 24:
{ace_cc_yyval.id = ace_cc_yyvsp[0].id; }
break;
case 25:
{ace_cc_yyval.id = ace_cc_yyvsp[0].id; }
break;
case 26:
{ ace_cc_yyval.lock_mode = CosConcurrencyControl::read; }
break;
case 27:
{ ace_cc_yyval.lock_mode = CosConcurrencyControl::intention_read; }
break;
case 28:
{ ace_cc_yyval.lock_mode = CosConcurrencyControl::upgrade; }
break;
case 29:
{ ace_cc_yyval.lock_mode = CosConcurrencyControl::write; }
break;
case 30:
{ ace_cc_yyval.lock_mode = CosConcurrencyControl::intention_write; }
break;
    }
    ace_cc_yyssp -= ace_cc_yym;
    ace_cc_yystate = *ace_cc_yyssp;
    ace_cc_yyvsp -= ace_cc_yym;
    ace_cc_yym = ace_cc_yylhs[ace_cc_yyn];
    if (ace_cc_yystate == 0 && ace_cc_yym == 0)
    {
#if ACE_CC_YYDEBUG
        if (ace_cc_yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", ACE_CC_YYPREFIX, ACE_CC_YYFINAL);
#endif
        ace_cc_yystate = ACE_CC_YYFINAL;
        *++ace_cc_yyssp = ACE_CC_YYFINAL;
        *++ace_cc_yyvsp = ace_cc_yyval;
        if (ace_cc_yychar < 0)
        {
            if ((ace_cc_yychar = ace_cc_yylex()) < 0) ace_cc_yychar = 0;
#if ACE_CC_YYDEBUG
            if (ace_cc_yydebug)
            {
                ace_cc_yys = 0;
                if (ace_cc_yychar <= ACE_CC_YYMAXTOKEN) ace_cc_yys = ace_cc_yyname[ace_cc_yychar];
                if (!ace_cc_yys) ace_cc_yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        ACE_CC_YYPREFIX, ACE_CC_YYFINAL, ace_cc_yychar, ace_cc_yys);
            }
#endif
        }
        if (ace_cc_yychar == 0) goto ace_cc_yyaccept;
        goto ace_cc_yyloop;
    }
    if ((ace_cc_yyn = ace_cc_yygindex[ace_cc_yym]) && (ace_cc_yyn += ace_cc_yystate) >= 0 &&
            ace_cc_yyn <= ACE_CC_YYTABLESIZE && ace_cc_yycheck[ace_cc_yyn] == ace_cc_yystate)
        ace_cc_yystate = ace_cc_yytable[ace_cc_yyn];
    else
        ace_cc_yystate = ace_cc_yydgoto[ace_cc_yym];
#if ACE_CC_YYDEBUG
    if (ace_cc_yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", ACE_CC_YYPREFIX, *ace_cc_yyssp, ace_cc_yystate);
#endif
    if (ace_cc_yyssp >= ace_cc_yysslim && ace_cc_yygrowstack())
    {
        goto ace_cc_yyoverflow;
    }
    *++ace_cc_yyssp = ace_cc_yystate;
    *++ace_cc_yyvsp = ace_cc_yyval;
    goto ace_cc_yyloop;
ace_cc_yyoverflow:
    ace_cc_yyerror("yacc stack overflow");
ace_cc_yyabort:
    return (1);
ace_cc_yyaccept:
    return (0);
}
