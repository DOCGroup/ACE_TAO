#ifndef lint
char ace_yysccsid[] = "@(#)yaccpar	1.4 (Berkeley) 02/25/90 \n\
 Modified 5/2/90 by J. Roskind to support graphic debugging modes";
#endif
#line 2 "Svc_Conf.y"
#define ACE_BUILD_DLL
#include "ace/ARGV.h"
#include "ace/Svc_Conf.h"
#include "ace/Module.h"
#include "ace/Stream.h"

static ACE_Module_Type *get_module (ACE_Static_Node *str_rec, ACE_Static_Node *svc_type);
static ACE_Module_Type *get_module (ACE_Static_Node *str_rec, const char *svc_name);

#define YYDEBUG_LEXER_TEXT (ace_yytext[ace_yyleng] = '\0', ace_yytext) 
/* Force the pretty debugging code to compile.*/
#define YYDEBUG 1 

/* Efficient memory allocation technique.*/
ACE_Obstack *ace_obstack;

#line 23 "y.tab.c"
#define ACE_DYNAMIC 257
#define ACE_STATIC 258
#define ACE_SUSPEND 259
#define ACE_RESUME 260
#define ACE_REMOVE 261
#define ACE_USTREAM 262
#define ACE_MODULE_T 263
#define ACE_STREAM_T 264
#define ACE_SVC_OBJ_T 265
#define ACE_ACTIVE 266
#define ACE_INACTIVE 267
#define ACE_PATHNAME 268
#define ACE_IDENT 269
#define ACE_STRING 270
#define ACE_LPAREN 271
#define ACE_RPAREN 272
#define ACE_LBRACE 273
#define ACE_RBRACE 274
#define ACE_STAR 275
#define ACE_COLON 276
#define YYERRCODE 256
short ace_yylhs[] = {                                        -1,
    0,    0,    0,   14,   14,   14,   14,   14,   14,    5,
    6,    7,    8,    9,   11,   18,   11,   15,   15,   19,
   12,   12,   10,   10,   13,   13,   13,   13,   13,   16,
    4,    4,    4,   17,   17,    3,    3,    3,    2,    2,
    1,    1,
};
short ace_yylen[] = {                                         2,
    2,    2,    0,    1,    1,    1,    1,    1,    1,    3,
    3,    2,    2,    2,    3,    0,    4,    1,    1,    0,
    4,    0,    2,    0,    1,    1,    1,    1,    1,    4,
    1,    1,    0,    3,    5,    2,    2,    2,    1,    0,
    1,    1,
};
short ace_yydefred[] = {                                      3,
    0,    2,    0,    0,    0,    0,    0,    0,    4,    5,
    6,    7,    8,    9,    1,    0,    0,    0,   12,   13,
   14,   16,   18,   19,    0,    0,    0,    0,    0,   39,
   10,   11,    0,   20,   15,   36,   38,   37,   41,   42,
    0,    0,   17,   24,    0,   31,   32,   30,    0,    0,
   21,   25,   26,   27,   28,   29,   23,    0,   35,
};
short ace_yydgoto[] = {                                       1,
   41,   31,   29,   48,    9,   10,   11,   12,   13,   49,
   14,   35,   57,   15,   25,   17,   42,   33,   44,
};
short ace_yysindex[] = {                                      0,
 -244,    0, -266, -260, -250, -243, -241, -247,    0,    0,
    0,    0,    0,    0,    0, -240, -237, -237,    0,    0,
    0,    0,    0,    0, -238, -236, -233, -231, -239,    0,
    0,    0, -238,    0,    0,    0,    0,    0,    0,    0,
 -242, -235,    0,    0, -232,    0,    0,    0, -253, -226,
    0,    0,    0,    0,    0,    0,    0, -234,    0,
};
short ace_yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   27,   27,    0,    0,
    0,    0,    0,    0,   46,    0,    0,    0,    0,    0,
    0,    0,   46,    0,    0,    0,    0,    0,    0,    0,
    0,   20,    0,    0,    0,    0,    0,    0,    0,    1,
    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
short ace_yygindex[] = {                                      0,
    0,   18,    0,    0,   -8,   -6,   -9,   -2,   -1,    0,
    0,   16,    0,    0,    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 308
short ace_yytable[] = {                                      23,
   34,   24,   16,    3,    4,    5,    6,    7,   18,    3,
    4,    2,    3,    4,    5,    6,    7,    8,   19,   33,
   51,   22,   26,   27,   28,   20,   40,   21,   39,   40,
   46,   47,   30,   45,   34,   32,   50,   59,   36,   54,
   52,   37,   53,   38,   58,   22,   55,   56,   43,    0,
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
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   34,   34,   34,   34,
   34,   34,   34,    0,    0,    0,   34,   34,    0,    0,
   34,    0,    0,   34,   34,   33,   33,   33,   33,   33,
   33,   33,   40,   40,   40,   40,   40,   40,   40,   33,
    0,    0,   33,   33,    0,    0,    0,    0,    0,   40,
   40,   22,   22,   22,   22,   22,   22,   22,
};
short ace_yycheck[] = {                                       8,
    0,    8,  269,  257,  258,  259,  260,  261,  269,  257,
  258,  256,  257,  258,  259,  260,  261,  262,  269,    0,
  274,  269,  263,  264,  265,  269,    0,  269,  268,  269,
  266,  267,  270,  276,  273,   18,  269,  272,  275,   49,
   49,  275,   49,  275,  271,    0,   49,   49,   33,   -1,
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
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,  256,  257,  258,  259,
  260,  261,  262,   -1,   -1,   -1,  266,  267,   -1,   -1,
  270,   -1,   -1,  273,  274,  256,  257,  258,  259,  260,
  261,  262,  256,  257,  258,  259,  260,  261,  262,  270,
   -1,   -1,  273,  274,   -1,   -1,   -1,   -1,   -1,  273,
  274,  256,  257,  258,  259,  260,  261,  262,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 276
#if YYDEBUG
char *ace_yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"ACE_DYNAMIC","ACE_STATIC",
"ACE_SUSPEND","ACE_RESUME","ACE_REMOVE","ACE_USTREAM","ACE_MODULE_T",
"ACE_STREAM_T","ACE_SVC_OBJ_T","ACE_ACTIVE","ACE_INACTIVE","ACE_PATHNAME",
"ACE_IDENT","ACE_STRING","ACE_LPAREN","ACE_RPAREN","ACE_LBRACE","ACE_RBRACE",
"ACE_STAR","ACE_COLON",
};
char *ace_yyrule[] = {
"$accept : svc_config_entries",
"svc_config_entries : svc_config_entries svc_config_entry",
"svc_config_entries : svc_config_entries error",
"svc_config_entries :",
"svc_config_entry : dynamic",
"svc_config_entry : static",
"svc_config_entry : suspend",
"svc_config_entry : resume",
"svc_config_entry : remove",
"svc_config_entry : stream",
"dynamic : ACE_DYNAMIC svc_location parameters_opt",
"static : ACE_STATIC ACE_IDENT parameters_opt",
"suspend : ACE_SUSPEND ACE_IDENT",
"resume : ACE_RESUME ACE_IDENT",
"remove : ACE_REMOVE ACE_IDENT",
"stream : ACE_USTREAM stream_ops stream_modules",
"$$1 :",
"stream : ACE_USTREAM ACE_IDENT $$1 stream_modules",
"stream_ops : dynamic",
"stream_ops : static",
"$$2 :",
"stream_modules : ACE_LBRACE $$2 module_list ACE_RBRACE",
"stream_modules :",
"module_list : module_list module",
"module_list :",
"module : dynamic",
"module : static",
"module : suspend",
"module : resume",
"module : remove",
"svc_location : ACE_IDENT type svc_initializer status",
"status : ACE_ACTIVE",
"status : ACE_INACTIVE",
"status :",
"svc_initializer : pathname ACE_COLON ACE_IDENT",
"svc_initializer : pathname ACE_COLON ACE_IDENT ACE_LPAREN ACE_RPAREN",
"type : ACE_MODULE_T ACE_STAR",
"type : ACE_SVC_OBJ_T ACE_STAR",
"type : ACE_STREAM_T ACE_STAR",
"parameters_opt : ACE_STRING",
"parameters_opt :",
"pathname : ACE_PATHNAME",
"pathname : ACE_IDENT",
};
#endif
#define ace_yyclearin (ace_yychar=(-1))
#define ace_yyerrok (ace_yyerrflag=0)
#ifndef YYSTACKSIZE
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 300
#endif
#endif
int ace_yydebug;
int ace_yynerrs;
int ace_yyerrflag;
int ace_yychar;
short *ace_yyssp;
YYSTYPE *ace_yyvsp;
YYSTYPE ace_yyval;
YYSTYPE ace_yylval;
#define ace_yystacksize YYSTACKSIZE
short ace_yyss[YYSTACKSIZE];
YYSTYPE ace_yyvs[YYSTACKSIZE];
#line 246 "Svc_Conf.y"
// Prints the error string to standard output.  Cleans up the error
// messages.

void
ace_yyerror (char *s)
{
  ACE_ERROR ((LM_ERROR, "[error %d] on line %d: %s\n", 
	      ++ace_yyerrno, ace_yylineno, s));
}

// Note that SRC_REC represents left context, which is the STREAM *
// record.

static ACE_Module_Type *
get_module (ACE_Static_Node *str_rec, const char *svc_name)
{
  const ACE_Service_Record *sr = str_rec->record ();
  const ACE_Service_Type *type = sr->type ();
  ACE_Stream_Type *st = sr == 0 ? 0 : (ACE_Stream_Type *) type;
  ACE_Module_Type *mt = st == 0 ? 0 : st->find (svc_name);

  if (sr == 0 || st == 0 || mt == 0)
    {
      ACE_ERROR ((LM_ERROR, "cannot locate Module_Type %s in STREAM_Type %s\n",
		 svc_name, str_rec->name ()));
      ace_yyerrno++;
    }
  return mt;
}

static ACE_Module_Type *
get_module (ACE_Static_Node *str_rec, ACE_Static_Node *svc_type)
{
  const ACE_Service_Record *sr = str_rec->record ();
  const ACE_Service_Type *type = sr->type ();
  ACE_Stream_Type *st = sr == 0 ? 0 : (ACE_Stream_Type *) type;
  const ACE_Service_Record *sv = svc_type->record ();
  type = sv->type ();
  ACE_Module_Type *mt = (ACE_Module_Type *) type;

  if (sr == 0 || st == 0 || mt == 0)
    {
      ACE_ERROR ((LM_ERROR, "cannot locate Module_Type %s or STREAM_Type %s\n",
		 svc_type->name (), str_rec->name ()));
      ace_yyerrno++;
    }
  return mt;
}

ACE_Service_Type *
ace_create_service_type (const char *name, 
			 int type, 
			 const void *symbol, 
			 unsigned int flags)
{
  ACE_Service_Type *stp = 0;

  // Note, the only place we need to put a case statement.  This is
  // also the place where we'd put the RTTI tests, if the compiler
  // actually supported them!

  switch (type)
    {
    case ACE_SVC_OBJ_T:
      stp = new ACE_Service_Object_Type ((ACE_Service_Object *) symbol, name, flags);
      break;
    case ACE_MODULE_T:
      stp = new ACE_Module_Type ((MT_Module *) symbol, name, flags);
      break;
    case ACE_STREAM_T:
      stp = new ACE_Stream_Type ((MT_Stream *) symbol, name, flags);
      break;
    default:
      ACE_ERROR ((LM_ERROR, "unknown case\n"));
      ace_yyerrno++;
      break;
    }
  return stp;
}

#if defined (DEBUGGING)
// Current line number.
int ace_yylineno = 1;

// Name given on the command-line to envoke the program.
char *program_name;

// Main driver program. 

int 
main (int argc, char *argv[])
{ 
  ace_yyin = stdin;
  ace_obstack = new ACE_Obstack;

  // Try to reopen any filename argument to use YYIN.
  if (argc > 1 && (ace_yyin = freopen (argv[1], "r", stdin)) == 0)
    (void) ::fprintf (stderr, "usage: %s [file]\n", argv[0]), exit (1);

  return ace_yyparse ();
}
#endif /* DEBUGGING */
#line 346 "y.tab.c"
#define YYABORT goto ace_yyabort
#define YYACCEPT goto ace_yyaccept
#define YYERROR goto ace_yyerrlab
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
void YYDEBUG_INDENT(int ace_yyindent)
#else
YYDEBUG_INDENT(ace_yyindent)
int ace_yyindent;
#endif
{
    while(ace_yyindent-- > 0)
        printf("%s", YYDEBUG_INDENT_STRING);
}
#endif /* YYDEBUG_INDENT */
#ifndef YYDEBUG_REDUCE
#ifdef __cplusplus
void YYDEBUG_REDUCE(int /* ace_yynew_state */, int /* ace_yyrule_num */, char *ace_yyrule_string, int ace_yynew_indent, int ace_yyrhs_count)
#else
YYDEBUG_REDUCE(ace_yynew_state, ace_yyrule_num, ace_yyrule_string, ace_yynew_indent, ace_yyrhs_count)
int ace_yynew_state;
int ace_yyrule_num;
char * ace_yyrule_string;
int ace_yynew_indent;
int ace_yyrhs_count;
#endif
{
    if (1 < ace_yyrhs_count)
    {  /* draw the graphics for the reduction */
        YYDEBUG_INDENT(ace_yynew_indent);
        while(1 < ace_yyrhs_count--)
            printf("%s", YYDEBUG_REDUCE_STRING);
        putchar('+'); /* left rotated L would look nice */
        putchar('\n');
        YYDEBUG_INDENT(ace_yynew_indent);
        putchar('|'); /* down arrow would look nice */
        putchar('\n');
    }
    YYDEBUG_INDENT(ace_yynew_indent);
    /* Only print the resulting token name */
    while (*ace_yyrule_string)
        putchar(*ace_yyrule_string++);
    putchar('\n');
}
#endif /* YYDEBUG_REDUCE */
#ifndef YYDEBUG_SHIFT_LEXEME
#ifdef __cplusplus
void YYDEBUG_SHIFT_LEXEME(int /* ace_yyold_state */, int /* ace_yynew_state */, char *ace_yytoken_string, int ace_yynew_indent)
#else
YYDEBUG_SHIFT_LEXEME(ace_yyold_state, ace_yynew_state, ace_yytoken_string, ace_yynew_indent)
int ace_yyold_state;
int ace_yynew_state;
char * ace_yytoken_string;
int ace_yynew_indent;
#endif
{
    YYDEBUG_INDENT(ace_yynew_indent);
    printf("%s <-- `%s'\n", ace_yytoken_string, YYDEBUG_LEXER_TEXT);
}
#endif /*  YYDEBUG_SHIFT_LEXEME */
#ifndef YYDEBUG_LOOK_AHEAD
#ifdef __cplusplus
void YYDEBUG_LOOK_AHEAD(int /* ace_yynew_state */, int ace_yytoken_num, char *ace_yytoken_string, int ace_yyindent)
#else
YYDEBUG_LOOK_AHEAD(ace_yynew_state, ace_yytoken_num, ace_yytoken_string, ace_yyindent)
int ace_yynew_state;
int ace_yytoken_num;
char * ace_yytoken_string;
int ace_yyindent;
#endif
{
    YYDEBUG_INDENT(ace_yyindent);
    printf("          .... look ahead at %s   `%s'\n",
           ace_yytoken_string,
           (0 == ace_yytoken_num)? "\0": YYDEBUG_LEXER_TEXT);
}
#endif /* YYDEBUG_LOOK_AHEAD */
#ifndef YYDEBUG_DISCARD_STATE
#ifdef __cplusplus
void YYDEBUG_DISCARD_STATE(int /* ace_yynew_state */, int ace_yyindent)
#else
YYDEBUG_DISCARD_STATE(ace_yynew_state, ace_yyindent)
int ace_yynew_state;
int ace_yyindent;
#endif
{
    if (0 < ace_yyindent)
    {  /* draw the graphics for the reduction */
        YYDEBUG_INDENT(ace_yyindent-1);
        printf("%s", YYDEBUG_REDUCE_STRING);
        putchar('+'); /* left rotated L would look nice */
        printf("  discarding state\n");
        YYDEBUG_INDENT(ace_yyindent-1);
        putchar('|'); /* down arrow would look nice */
        putchar('\n');
    }
    else
    {
        if (0 == ace_yyindent)
            printf("discarding state\n");
        else
            printf("no more states to discard: parser will abort\n");
    }
}
#endif /* YYDEBUG_DISCARD_STATE */
#ifndef YYDEBUG_DISCARD_TOKEN
#ifdef __cplusplus
void YYDEBUG_DISCARD_TOKEN(int /* ace_yynew_state */, int /* ace_yytoken_num */, char *ace_yytoken_string, int ace_yyindent)
#else
YYDEBUG_DISCARD_TOKEN(ace_yynew_state, ace_yytoken_num, ace_yytoken_string, ace_yyindent)
int ace_yynew_state;
int ace_yytoken_num;
char * ace_yytoken_string;
int ace_yyindent;
#endif
{
    YYDEBUG_INDENT(ace_yyindent);
    printf("discarding token %s\n", ace_yytoken_string);
}
#endif /* YYDEBUG_DISCARD_TOKEN */
#ifndef YYDEBUG_SHIFT_ERROR_LEXEME
#ifdef __cplusplus
void YYDEBUG_SHIFT_ERROR_LEXEME(int /* ace_yyold_state */, int /* ace_yynew_state */, int ace_yyindent)
#else
YYDEBUG_SHIFT_ERROR_LEXEME(ace_yyold_state, ace_yynew_state, ace_yyindent)
int ace_yyold_state;
int ace_yynew_state;
int ace_yyindent;
#endif
{
    YYDEBUG_INDENT(ace_yyindent);
    printf("error\n");
}
#endif /* YYDEBUG_SHIFT_ERROR_LEXEME */
#endif /* YYDEBUG */
#ifdef __cplusplus
extern "C" { extern char *ace_foo(const char *); }
#endif
int
ace_yyparse()
{
    register int ace_yym, ace_yyn, ace_yystate;
#if YYDEBUG
    register char *ace_yys;
#ifndef __cplusplus
    extern char *ace_foo();
#endif

    if (ace_yys = ACE_OS::getenv("YYDEBUG"))
    {
        ace_yyn = *ace_yys;
        if (ace_yyn >= '0' && ace_yyn <= '9')
            ace_yydebug = ace_yyn - '0';
    }
#endif

    ace_yynerrs = 0;
    ace_yyerrflag = 0;
    ace_yychar = (-1);

    ace_yyssp = ace_yyss;
    ace_yyvsp = ace_yyvs;
    *ace_yyssp = ace_yystate = 0;

ace_yyloop:
    if (ace_yyn = ace_yydefred[ace_yystate]) goto ace_yyreduce;
    if (ace_yychar < 0)
    {
        if ((ace_yychar = ace_yylex()) < 0) ace_yychar = 0;
#if YYDEBUG
        if (ace_yydebug)
        {
            ace_yys = 0;
            if (ace_yychar <= YYMAXTOKEN) ace_yys = ace_yyname[ace_yychar];
            if (!ace_yys) ace_yys = "illegal-symbol";
            if (5 < ace_yydebug)
                printf("ace_yydebug: state %d, reading %d (%s)\n", ace_yystate,
                        ace_yychar, ace_yys);
            else
                YYDEBUG_LOOK_AHEAD(ace_yystate, ace_yychar, ace_yys, ace_yyssp-ace_yyss);
        }
#endif
    }
    if ((ace_yyn = ace_yysindex[ace_yystate]) && (ace_yyn += ace_yychar) >= 0 &&
            ace_yyn <= YYTABLESIZE && ace_yycheck[ace_yyn] == ace_yychar)
    {
#if YYDEBUG
        if (ace_yydebug)
            if (5 < ace_yydebug)
                printf("ace_yydebug: state %d, shifting to state %d\n",
                        ace_yystate, ace_yytable[ace_yyn]);
            else
                YYDEBUG_SHIFT_LEXEME(ace_yystate, ace_yytable[ace_yyn], ace_yys, ace_yyssp-ace_yyss);
#endif
        if (ace_yyssp >= ace_yyss + ace_yystacksize - 1)
        {
            goto ace_yyoverflow;
        }
        *++ace_yyssp = ace_yystate = ace_yytable[ace_yyn];
        *++ace_yyvsp = ace_yylval;
        ace_yychar = (-1);
        if (ace_yyerrflag > 0)  --ace_yyerrflag;
        goto ace_yyloop;
    }
    if ((ace_yyn = ace_yyrindex[ace_yystate]) && (ace_yyn += ace_yychar) >= 0 &&
            ace_yyn <= YYTABLESIZE && ace_yycheck[ace_yyn] == ace_yychar)
    {
        ace_yyn = ace_yytable[ace_yyn];
        goto ace_yyreduce;
    }
    if (ace_yyerrflag) goto ace_yyinrecovery;
#ifdef lint
    goto ace_yynewerror;
#endif

    ace_yyerror("syntax error");
#ifdef lint
    goto ace_yyerrlab;
#endif

    ++ace_yynerrs;
ace_yyinrecovery:
    if (ace_yyerrflag < 3)
    {
        ace_yyerrflag = 3;
        for (;;)
        {
            if ((ace_yyn = ace_yysindex[*ace_yyssp]) && (ace_yyn += YYERRCODE) >= 0 &&
                    ace_yyn <= YYTABLESIZE && ace_yycheck[ace_yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (ace_yydebug)
                    if (5 < ace_yydebug)
                        printf("ace_yydebug: state %d, error recovery shifting\
 to state %d\n", *ace_yyssp, ace_yytable[ace_yyn]);
                    else
                        YYDEBUG_SHIFT_ERROR_LEXEME(*ace_yyssp, ace_yytable[ace_yyn], ace_yyssp-ace_yyss);
#endif
                if (ace_yyssp >= ace_yyss + ace_yystacksize - 1)
                {
                    goto ace_yyoverflow;
                }
                *++ace_yyssp = ace_yystate = ace_yytable[ace_yyn];
                *++ace_yyvsp = ace_yylval;
                goto ace_yyloop;
            }
            else
            {
#if YYDEBUG
                if (ace_yydebug)
                    if (5 < ace_yydebug)
                        printf("ace_yydebug: error recovery discarding state %d\
",
                            *ace_yyssp);
                    else
                        YYDEBUG_DISCARD_STATE(*ace_yyssp, ace_yyssp-ace_yyss-1);
#endif
                if (ace_yyssp <= ace_yyss) goto ace_yyabort;
                --ace_yyssp;
                --ace_yyvsp;
            }
        }
    }
    else
    {
        if (ace_yychar == 0) goto ace_yyabort;
#if YYDEBUG
        if (ace_yydebug)
        {
            ace_yys = 0;
            if (ace_yychar <= YYMAXTOKEN) ace_yys = ace_yyname[ace_yychar];
            if (!ace_yys) ace_yys = "illegal-symbol";
            if (5 < ace_yydebug)
                printf("ace_yydebug: state %d, error recovery discards token %d (%s)\n",
                    ace_yystate, ace_yychar, ace_yys);
            else 
                YYDEBUG_DISCARD_TOKEN(ace_yystate, ace_yychar, ace_yys, ace_yyssp-ace_yyss);
        }
#endif
        ace_yychar = (-1);
        goto ace_yyloop;
    }
ace_yyreduce:
    ace_yym = ace_yylen[ace_yyn];
    ace_yyval = ace_yyvsp[1-ace_yym];
#if YYDEBUG
    if (ace_yydebug)
        if (5 < ace_yydebug)
            printf("ace_yydebug: state %d, reducing by rule %d (%s)\n",
                    ace_yystate, ace_yyn, ace_yyrule[ace_yyn]);
        else
            YYDEBUG_REDUCE(ace_yystate, ace_yyn, ace_yyrule[ace_yyn], ace_yyssp-ace_yyss-ace_yym, ace_yym);
#endif
    switch (ace_yyn)
    {
case 1:
#line 38 "Svc_Conf.y"
{
      ace_yyvsp[0].parse_node_->apply (); delete ace_yyvsp[0].parse_node_; ace_obstack->release (); 
    }
break;
case 2:
#line 42 "Svc_Conf.y"
{ 
      ace_obstack->release (); 
    }
break;
case 10:
#line 59 "Svc_Conf.y"
{ 
      ace_yyval.parse_node_ = new ACE_Dynamic_Node (ace_yyvsp[-1].svc_record_, ace_yyvsp[0].ident_);
    }
break;
case 11:
#line 66 "Svc_Conf.y"
{ 
      ace_yyval.parse_node_ = new ACE_Static_Node (ace_yyvsp[-1].ident_, ace_yyvsp[0].ident_);
    }
break;
case 12:
#line 73 "Svc_Conf.y"
{ 
      ace_yyval.parse_node_ = new ACE_Suspend_Node (ace_yyvsp[0].ident_);
    }
break;
case 13:
#line 80 "Svc_Conf.y"
{ 
      ace_yyval.parse_node_ = new ACE_Resume_Node (ace_yyvsp[0].ident_);
    }
break;
case 14:
#line 87 "Svc_Conf.y"
{ 
      ace_yyval.parse_node_ = new ACE_Remove_Node (ace_yyvsp[0].ident_);
    }
break;
case 15:
#line 94 "Svc_Conf.y"
{
      ace_yyval.parse_node_ = new ACE_Stream_Node (ace_yyvsp[-1].ACE_Static_Node_, ace_yyvsp[0].parse_node_);
    }
break;
case 16:
#line 97 "Svc_Conf.y"
{ ace_yyval.ACE_Static_Node_ = new ACE_Static_Node (ace_yyvsp[0].ident_); }
break;
case 17:
#line 98 "Svc_Conf.y"
{
      ace_yyval.parse_node_ = new ACE_Dummy_Node (ace_yyvsp[-1].ACE_Static_Node_, ace_yyvsp[0].parse_node_);
    }
break;
case 18:
#line 105 "Svc_Conf.y"
{
    }
break;
case 19:
#line 108 "Svc_Conf.y"
{
    }
break;
case 20:
#line 114 "Svc_Conf.y"
{ 
      /* Initialize left context...*/
      ace_yyval.ACE_Static_Node_ = ace_yyvsp[-1].ACE_Static_Node_; 
    }
break;
case 21:
#line 119 "Svc_Conf.y"
{
      ace_yyval.parse_node_ = ace_yyvsp[-1].parse_node_;
    }
break;
case 22:
#line 122 "Svc_Conf.y"
{ ace_yyval.parse_node_ = 0; }
break;
case 23:
#line 126 "Svc_Conf.y"
{ ace_yyvsp[0].parse_node_->link (ace_yyvsp[-1].parse_node_); ace_yyval.parse_node_ = ace_yyvsp[0].parse_node_; }
break;
case 24:
#line 127 "Svc_Conf.y"
{ ace_yyval.parse_node_ = 0; }
break;
case 25:
#line 132 "Svc_Conf.y"
{
      ACE_ARGV args (ace_yyvsp[0].ACE_Static_Node_->parameters ());
      ACE_Module_Type *mt = get_module (ace_yyvsp[-2].ACE_Static_Node_, ace_yyvsp[0].ACE_Static_Node_);

      if (::strcmp (ace_yyvsp[0].ACE_Static_Node_->name (), 
		    ((MT_Module *) mt->object ())->name ()) != 0)
	ACE_ERROR ((LM_ERROR, "warning, service name %s is different from Module name %s\n",
		   ace_yyvsp[0].ACE_Static_Node_->name (), ((MT_Module *) mt->object ())->name ()));

      if (mt->init (args.argc (), args.argv ()) == -1
	  || ((ACE_Stream_Type *) (ace_yyvsp[-2].ACE_Static_Node_)->record ()->type ())->push (mt) == -1)
	{
	  ACE_ERROR ((LM_ERROR, "dynamic initialization failed for Module %s\n",
		     ace_yyvsp[0].ACE_Static_Node_->name ()));
	  ace_yyerrno++;
	}
    }
break;
case 26:
#line 150 "Svc_Conf.y"
{ 
      ACE_Module_Type *mt = get_module (ace_yyvsp[-2].ACE_Static_Node_, ace_yyvsp[0].ACE_Static_Node_->name ());
      if (::strcmp (ace_yyvsp[0].ACE_Static_Node_->name (), 
		    ((MT_Module *) mt->object ())->name ()) != 0)
	ACE_ERROR ((LM_ERROR, "warning, service name %s is different from Module name %s\n",
		   ace_yyvsp[0].ACE_Static_Node_->name (), ((MT_Module *) mt->object ())->name ()));
      if (((ACE_Stream_Type *) (ace_yyvsp[-2].ACE_Static_Node_)->record ()->type ())->push (mt) == -1)
	ace_yyerrno++;
    }
break;
case 27:
#line 160 "Svc_Conf.y"
{ 
      ACE_Module_Type *mt = get_module (ace_yyvsp[-2].ACE_Static_Node_, ace_yyvsp[0].ACE_Static_Node_->name ());
      if (mt != 0)
	mt->suspend ();
    }
break;
case 28:
#line 166 "Svc_Conf.y"
{
      ACE_Module_Type *mt = get_module (ace_yyvsp[-2].ACE_Static_Node_, ace_yyvsp[0].ACE_Static_Node_->name ());
      if (mt != 0)
	mt->resume ();
    }
break;
case 29:
#line 172 "Svc_Conf.y"
{ 
      ACE_Module_Type *mt = get_module (ace_yyvsp[-2].ACE_Static_Node_, ace_yyvsp[0].ACE_Static_Node_->name ());
      if (mt != 0 
	  && ((ACE_Stream_Type *) (ace_yyvsp[-2].ACE_Static_Node_)->record ()->type ())->remove (mt) == -1)
	{
	  ACE_ERROR ((LM_ERROR, "cannot remove Module_Type %s from STREAM_Type %s\n",
		     ace_yyvsp[0].ACE_Static_Node_->name (), (ace_yyvsp[-2].ACE_Static_Node_)->name ()));
	  ace_yyerrno++;
	}
    }
break;
case 30:
#line 186 "Svc_Conf.y"
{
      unsigned int flags 
	= ACE_Service_Type::DELETE_THIS | (ace_yyvsp[-1].location_node_->dispose () == 0 ? 0 : ACE_Service_Type::DELETE_OBJ);
      ACE_Service_Type *stp = ace_create_service_type (ace_yyvsp[-3].ident_, ace_yyvsp[-2].type_, ace_yyvsp[-1].location_node_->symbol (), flags);
      ace_yyval.svc_record_ = new ACE_Service_Record (ace_yyvsp[-3].ident_, stp, ace_yyvsp[-1].location_node_->handle (), ace_yyvsp[0].type_);
    }
break;
case 31:
#line 196 "Svc_Conf.y"
{
      ace_yyval.type_ = 1;
    }
break;
case 32:
#line 200 "Svc_Conf.y"
{
      ace_yyval.type_ = 0;
    }
break;
case 33:
#line 204 "Svc_Conf.y"
{
      ace_yyval.type_ = 1;
    }
break;
case 34:
#line 211 "Svc_Conf.y"
{
      ace_yyval.location_node_ = new ACE_Object_Node (ace_yyvsp[-2].ident_, ace_yyvsp[0].ident_);
    }
break;
case 35:
#line 215 "Svc_Conf.y"
{
      ace_yyval.location_node_ = new ACE_Function_Node (ace_yyvsp[-4].ident_, ace_yyvsp[-2].ident_);
    }
break;
case 36:
#line 222 "Svc_Conf.y"
{
      ace_yyval.type_ = ACE_MODULE_T;
    }
break;
case 37:
#line 226 "Svc_Conf.y"
{
      ace_yyval.type_ = ACE_SVC_OBJ_T;
    }
break;
case 38:
#line 230 "Svc_Conf.y"
{
      ace_yyval.type_ = ACE_STREAM_T;
    }
break;
case 40:
#line 237 "Svc_Conf.y"
{ ace_yyval.ident_ = 0; }
break;
#line 866 "y.tab.c"
    }
    ace_yyssp -= ace_yym;
    ace_yystate = *ace_yyssp;
    ace_yyvsp -= ace_yym;
    ace_yym = ace_yylhs[ace_yyn];
    if (ace_yystate == 0 && ace_yym == 0)
    {
#ifdef YYDEBUG
        if (5 < ace_yydebug)
            printf("ace_yydebug: after reduction, shifting from state 0 to\
 state %d\n", YYFINAL);
#endif
        ace_yystate = YYFINAL;
        *++ace_yyssp = YYFINAL;
        *++ace_yyvsp = ace_yyval;
        if (ace_yychar < 0)
        {
            if ((ace_yychar = ace_yylex()) < 0) ace_yychar = 0;
#if YYDEBUG
            if (ace_yydebug)
            {
                ace_yys = 0;
                if (ace_yychar <= YYMAXTOKEN) ace_yys = ace_yyname[ace_yychar];
                if (!ace_yys) ace_yys = "illegal-symbol";
                if (5 < ace_yydebug)
                    printf("ace_yydebug: state %d, reading %d (%s)\n",
                        YYFINAL, ace_yychar, ace_yys);
                else
                    YYDEBUG_LOOK_AHEAD(YYFINAL, ace_yychar, ace_yys, ace_yyssp-ace_yyss);
            }
#endif
        }
        if (ace_yychar == 0) goto ace_yyaccept;
        goto ace_yyloop;
    }
    if ((ace_yyn = ace_yygindex[ace_yym]) && (ace_yyn += ace_yystate) >= 0 &&
            ace_yyn <= YYTABLESIZE && ace_yycheck[ace_yyn] == ace_yystate)
        ace_yystate = ace_yytable[ace_yyn];
    else
        ace_yystate = ace_yydgoto[ace_yym];
#ifdef YYDEBUG
    if (5 < ace_yydebug)
        printf("ace_yydebug: after reduction, shifting from state %d \
to state %d\n", *ace_yyssp, ace_yystate);
#endif
    if (ace_yyssp >= ace_yyss + ace_yystacksize - 1)
    {
        goto ace_yyoverflow;
    }
    *++ace_yyssp = ace_yystate;
    *++ace_yyvsp = ace_yyval;
    goto ace_yyloop;
ace_yyoverflow:
    ace_yyerror("yacc stack overflow");
ace_yyabort:
    return (1);
ace_yyaccept:
    return (0);
}
