#ifndef lint
char ace_yysccsid[] = "@(#)yaccpar	1.4 (Berkeley) 02/25/90 \n\
 Modified 5/2/90 by J. Roskind to support graphic debugging modes";
#endif
#line 2 "Svc_Conf.y"
/* $Id$*/
#define ACE_BUILD_DLL
#include "ace/ARGV.h"
#include "ace/Svc_Conf.h"
#include "ace/Module.h"
#include "ace/Stream.h"

ACE_RCSID(ace, Svc_Conf_y, "$Id$")

/* Prototypes.*/
static ACE_Module_Type *ace_get_module (ACE_Static_Node *str_rec,
                                        ACE_Static_Node *svc_type);
static ACE_Module_Type *ace_get_module (ACE_Static_Node *str_rec,
                                        const char *svc_name);

#define ACE_YYDEBUG_LEXER_TEXT (ace_yytext[ace_yyleng] = '\0', ace_yytext)
/* Force the pretty debugging code to compile.*/
#define ACE_YYDEBUG 1

/* Efficient memory allocation technique.*/
ACE_Obstack *ace_obstack;

#line 29 "Svc_Conf_y.cpp"
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
#define ACE_YYERRCODE 256
short ace_yylhs[] = {                                        -1,
    0,    0,    0,   14,   14,   14,   14,   14,   14,    5,
    6,    7,    8,    9,   11,   18,   11,   15,   15,   19,
   12,   12,   10,   10,   13,   13,   13,   13,   13,   16,
    4,    4,    4,   17,   17,   17,    3,    3,    3,    2,
    2,    1,    1,
};
short ace_yylen[] = {                                         2,
    2,    2,    0,    1,    1,    1,    1,    1,    1,    3,
    3,    2,    2,    2,    3,    0,    4,    1,    1,    0,
    4,    0,    2,    0,    1,    1,    1,    1,    1,    4,
    1,    1,    0,    3,    5,    4,    2,    2,    2,    1,
    0,    1,    1,
};
short ace_yydefred[] = {                                      3,
    0,    2,    0,    0,    0,    0,    0,    0,    4,    5,
    6,    7,    8,    9,    1,    0,    0,    0,   12,   13,
   14,   16,   18,   19,    0,    0,    0,    0,    0,   40,
   10,   11,    0,   20,   15,   37,   39,   38,   42,   43,
    0,    0,    0,   17,   24,    0,    0,   31,   32,   30,
    0,    0,    0,   21,   25,   26,   27,   28,   29,   23,
   36,    0,   35,
};
short ace_yydgoto[] = {                                       1,
   42,   31,   29,   50,    9,   10,   11,   12,   13,   51,
   14,   35,   60,   15,   25,   17,   43,   33,   45,
};
short ace_yysindex[] = {                                      0,
 -228,    0, -266, -260, -254, -246, -244, -247,    0,    0,
    0,    0,    0,    0,    0, -251, -235, -235,    0,    0,
    0,    0,    0,    0, -237, -249, -238, -236, -252,    0,
    0,    0, -237,    0,    0,    0,    0,    0,    0,    0,
 -231, -234, -248,    0,    0, -230, -229,    0,    0,    0,
 -253, -225, -227,    0,    0,    0,    0,    0,    0,    0,
    0, -224,    0,
};
short ace_yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   27,   27,    0,    0,
    0,    0,    0,    0,   46,    0,    0,    0,    0,    0,
    0,    0,   46,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   20,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    1,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,
};
short ace_yygindex[] = {                                      0,
    0,   31,    0,    0,   -8,   -6,   -1,    2,    3,    0,
    0,   18,    0,    0,    0,    0,    0,    0,    0,
};
#define ACE_YYTABLESIZE 308
short ace_yytable[] = {                                      23,
   34,   24,   16,    3,    4,    5,    6,    7,   18,    3,
    4,   26,   27,   28,   19,   39,   40,   48,   49,   33,
   54,   22,   20,   41,   21,   36,   41,    2,    3,    4,
    5,    6,    7,    8,   30,   34,   37,   46,   38,   53,
   52,   47,   55,   62,   56,   22,   61,   63,   32,   57,
   44,    0,   58,   59,    0,    0,    0,    0,    0,    0,
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
   33,   33,   41,   41,   41,   41,   41,   41,   41,   33,
    0,    0,   33,   33,    0,    0,    0,    0,    0,   41,
   41,   22,   22,   22,   22,   22,   22,   22,
};
short ace_yycheck[] = {                                       8,
    0,    8,  269,  257,  258,  259,  260,  261,  269,  257,
  258,  263,  264,  265,  269,  268,  269,  266,  267,    0,
  274,  269,  269,  276,  269,  275,    0,  256,  257,  258,
  259,  260,  261,  262,  270,  273,  275,  269,  275,  269,
  271,  276,   51,  271,   51,    0,  272,  272,   18,   51,
   33,   -1,   51,   51,   -1,   -1,   -1,   -1,   -1,   -1,
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
#define ACE_YYFINAL 1
#ifndef ACE_YYDEBUG
#define ACE_YYDEBUG 0
#endif
#define ACE_YYMAXTOKEN 276
#if ACE_YYDEBUG
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
"svc_initializer : ACE_COLON ACE_IDENT ACE_LPAREN ACE_RPAREN",
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
#ifndef ACE_YYSTACKSIZE
#ifdef ACE_YYMAXDEPTH
#define ACE_YYSTACKSIZE ACE_YYMAXDEPTH
#else
#define ACE_YYSTACKSIZE 300
#endif
#endif
int ace_yydebug;
int ace_yynerrs;
int ace_yyerrflag;
int ace_yychar;
short *ace_yyssp;
ACE_YYSTYPE *ace_yyvsp;
ACE_YYSTYPE ace_yyval;
ACE_YYSTYPE ace_yylval;
#define ace_yystacksize ACE_YYSTACKSIZE
short ace_yyss[ACE_YYSTACKSIZE];
ACE_YYSTYPE ace_yyvs[ACE_YYSTACKSIZE];
#line 307 "Svc_Conf.y"
// Prints the error string to standard output.  Cleans up the error
// messages.

void
ace_yyerror (const char *s)
{
#if defined (ACE_NLOGGING)
  ACE_UNUSED_ARG (s);
#endif /* ACE_NLOGGING */

  ACE_ERROR ((LM_ERROR,
              ASYS_TEXT ("[error %d] on line %d: %s\n"),
              ++ace_yyerrno,
              ace_yylineno,
              ASYS_WIDE_STRING (s)));
}

// Note that SRC_REC represents left context, which is the STREAM *
// record.

static ACE_Module_Type *
ace_get_module (ACE_Static_Node *str_rec,
                const char *svc_name)
{
  const ACE_Service_Type *sr = str_rec->record ();
  const ACE_Service_Type_Impl *type = sr->type ();
  ACE_Stream_Type *st = sr == 0
    ? 0
    : ACE_dynamic_cast (ACE_Stream_Type *,
                        ACE_const_cast (ACE_Service_Type_Impl *,
                                        type));
  ACE_Module_Type *mt = st == 0 ? 0 : st->find (ASYS_WIDE_STRING (svc_name));

  if (sr == 0 || st == 0 || mt == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("cannot locate Module_Type %s in STREAM_Type %s\n"),
                  ASYS_WIDE_STRING (svc_name),
                  ASYS_WIDE_STRING (str_rec->name ())));
      ace_yyerrno++;
    }

  return mt;
}

static ACE_Module_Type *
ace_get_module (ACE_Static_Node *str_rec,
                ACE_Static_Node *svc_type)
{
  const ACE_Service_Type *sr = str_rec->record ();
  const ACE_Service_Type_Impl *type = sr->type ();
  ACE_Stream_Type *st = sr == 0 ? 0 : (ACE_Stream_Type *) type;
  const ACE_Service_Type *sv = svc_type->record ();
  type = sv->type ();
  ACE_Module_Type *mt = (ACE_Module_Type *) type;
  ASYS_TCHAR *module_type_name = ACE_const_cast (ASYS_TCHAR *, svc_type->name ());

  if (sr == 0 || st == 0 || mt == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("cannot locate Module_Type %s or STREAM_Type %s\n"),
                  ASYS_WIDE_STRING (module_type_name),
                  ASYS_WIDE_STRING (str_rec->name ())));
      ace_yyerrno++;
    }

  // Make sure that the Module has the same name as the
  // Module_Type object from the svc.conf file.
  ACE_Module<ACE_SYNCH> *mp = (ACE_Module<ACE_SYNCH> *) mt->object ();

  if (ACE_OS::strcmp (mp->name (), ASYS_WIDE_STRING (module_type_name)) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("warning: assigning Module_Type name %s to Module %s since names differ\n"),
                  ASYS_WIDE_STRING (module_type_name),
                  mp->name ()));
      mp->name (ASYS_WIDE_STRING (module_type_name));
    }

  return mt;
}

ACE_Service_Type_Impl *
ace_create_service_type (const ASYS_TCHAR *name,
                         int type,
                         void *symbol,
                         u_int flags,
                         ACE_Service_Object_Exterminator gobbler)
{
  ACE_Service_Type_Impl *stp = 0;

  // Note, the only place we need to put a case statement.  This is
  // also the place where we'd put the RTTI tests, if the compiler
  // actually supported them!

  switch (type)
    {
    case ACE_SVC_OBJ_T:
      ACE_NEW_RETURN (stp,
                      ACE_Service_Object_Type ((ACE_Service_Object *) symbol,
                                               ASYS_WIDE_STRING (name), flags,
                                               gobbler),
                      0);
      break;
    case ACE_MODULE_T:
      ACE_NEW_RETURN (stp,
                      ACE_Module_Type (symbol, ASYS_WIDE_STRING (name), flags),
                      0);
      break;
    case ACE_STREAM_T:
      ACE_NEW_RETURN (stp,
                      ACE_Stream_Type (symbol, ASYS_WIDE_STRING (name), flags),
                      0);
      break;
    default:
      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("unknown case\n")));
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

  // Try to reopen any filename argument to use ACE_YYIN.
  if (argc > 1 && (ace_yyin = freopen (argv[1], "r", stdin)) == 0)
    (void) ::fprintf (stderr, "usage: %s [file]\n", argv[0]), exit (1);

  return ace_yyparse ();
}
#endif /* DEBUGGING */
#line 400 "Svc_Conf_y.cpp"
#define ACE_YYABORT goto ace_yyabort
#define ACE_YYACCEPT goto ace_yyaccept
#define ACE_YYERROR goto ace_yyerrlab
#ifdef ACE_YYDEBUG
#ifndef ACE_YYDEBUG_LEXER_TEXT /* pointer to the text isolated by the lexer*/
#define ACE_YYDEBUG_LEXER_TEXT "ACE_YYDEBUG_LEXER_TEXT not defined"
#endif
#ifndef ACE_YYDEBUG_INDENT_STRING
#define ACE_YYDEBUG_INDENT_STRING  "|       "
#endif
#ifndef ACE_YYDEBUG_REDUCE_STRING
#define ACE_YYDEBUG_REDUCE_STRING  "+-------"
#endif
#ifndef ACE_YYDEBUG_INDENT
#ifdef __cplusplus
void ACE_YYDEBUG_INDENT(int ace_yyindent)
#else
ACE_YYDEBUG_INDENT(ace_yyindent)
int ace_yyindent;
#endif
{
    while(ace_yyindent-- > 0)
        printf("%s", ACE_YYDEBUG_INDENT_STRING);
}
#endif /* ACE_YYDEBUG_INDENT */
#ifndef ACE_YYDEBUG_REDUCE
#ifdef __cplusplus
void ACE_YYDEBUG_REDUCE(int /* ace_yynew_state */, int /* ace_yyrule_num */, char *ace_yyrule_string, int ace_yynew_indent, int ace_yyrhs_count)
#else
ACE_YYDEBUG_REDUCE(ace_yynew_state, ace_yyrule_num, ace_yyrule_string, ace_yynew_indent, ace_yyrhs_count)
int ace_yynew_state;
int ace_yyrule_num;
char * ace_yyrule_string;
int ace_yynew_indent;
int ace_yyrhs_count;
#endif
{
    if (1 < ace_yyrhs_count)
    {  /* draw the graphics for the reduction */
        ACE_YYDEBUG_INDENT(ace_yynew_indent);
        while(1 < ace_yyrhs_count--)
            printf("%s", ACE_YYDEBUG_REDUCE_STRING);
        putchar('+'); /* left rotated L would look nice */
        putchar('\n');
        ACE_YYDEBUG_INDENT(ace_yynew_indent);
        putchar('|'); /* down arrow would look nice */
        putchar('\n');
    }
    ACE_YYDEBUG_INDENT(ace_yynew_indent);
    /* Only print the resulting token name */
    while (*ace_yyrule_string)
        putchar(*ace_yyrule_string++);
    putchar('\n');
}
#endif /* ACE_YYDEBUG_REDUCE */
#ifndef ACE_YYDEBUG_SHIFT_LEXEME
#ifdef __cplusplus
void ACE_YYDEBUG_SHIFT_LEXEME(int /* ace_yyold_state */, int /* ace_yynew_state*/, char *ace_yytoken_string, int ace_yynew_indent)
#else
ACE_YYDEBUG_SHIFT_LEXEME(ace_yyold_state, ace_yynew_state, ace_yytoken_string, ace_yynew_indent)
int ace_yyold_state;
int ace_yynew_state;
char * ace_yytoken_string;
int ace_yynew_indent;
#endif
{
    ACE_YYDEBUG_INDENT(ace_yynew_indent);
    printf("%s <-- `%s'\n", ace_yytoken_string, ACE_YYDEBUG_LEXER_TEXT);
}
#endif /*  ACE_YYDEBUG_SHIFT_LEXEME */
#ifndef ACE_YYDEBUG_LOOK_AHEAD
#ifdef __cplusplus
void ACE_YYDEBUG_LOOK_AHEAD(int /* ace_yynew_state */, int ace_yytoken_num, char *ace_yytoken_string, int ace_yyindent)
#else
ACE_YYDEBUG_LOOK_AHEAD(ace_yynew_state, ace_yytoken_num, ace_yytoken_string, ace_yyindent)
int ace_yynew_state;
int ace_yytoken_num;
char * ace_yytoken_string;
int ace_yyindent;
#endif
{
    ACE_YYDEBUG_INDENT(ace_yyindent);
    printf("          .... look ahead at %s   `%s'\n",
           ace_yytoken_string,
           (0 == ace_yytoken_num)? "\0": ACE_YYDEBUG_LEXER_TEXT);
}
#endif /* ACE_YYDEBUG_LOOK_AHEAD */
#ifndef ACE_YYDEBUG_DISCARD_STATE
#ifdef __cplusplus
void ACE_YYDEBUG_DISCARD_STATE(int /* ace_yynew_state */, int ace_yyindent)
#else
ACE_YYDEBUG_DISCARD_STATE(ace_yynew_state, ace_yyindent)
int ace_yynew_state;
int ace_yyindent;
#endif
{
    if (0 < ace_yyindent)
    {  /* draw the graphics for the reduction */
        ACE_YYDEBUG_INDENT(ace_yyindent-1);
        printf("%s", ACE_YYDEBUG_REDUCE_STRING);
        putchar('+'); /* left rotated L would look nice */
        printf("  discarding state\n");
        ACE_YYDEBUG_INDENT(ace_yyindent-1);
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
#endif /* ACE_YYDEBUG_DISCARD_STATE */
#ifndef ACE_YYDEBUG_DISCARD_TOKEN
#ifdef __cplusplus
void ACE_YYDEBUG_DISCARD_TOKEN(int /* ace_yynew_state */, int /* ace_yytoken_num */, char *ace_yytoken_string, int ace_yyindent)
#else
ACE_YYDEBUG_DISCARD_TOKEN(ace_yynew_state, ace_yytoken_num, ace_yytoken_string, ace_yyindent)
int ace_yynew_state;
int ace_yytoken_num;
char * ace_yytoken_string;
int ace_yyindent;
#endif
{
    ACE_YYDEBUG_INDENT(ace_yyindent);
    printf("discarding token %s\n", ace_yytoken_string);
}
#endif /* ACE_YYDEBUG_DISCARD_TOKEN */
#ifndef ACE_YYDEBUG_SHIFT_ERROR_LEXEME
#ifdef __cplusplus
void ACE_YYDEBUG_SHIFT_ERROR_LEXEME(int /* ace_yyold_state */, int /* ace_yynew_state */, int ace_yyindent)
#else
ACE_YYDEBUG_SHIFT_ERROR_LEXEME(ace_yyold_state, ace_yynew_state, ace_yyindent)
int ace_yyold_state;
int ace_yynew_state;
int ace_yyindent;
#endif
{
    ACE_YYDEBUG_INDENT(ace_yyindent);
    printf("error\n");
}
#endif /* ACE_YYDEBUG_SHIFT_ERROR_LEXEME */
#endif /* ACE_YYDEBUG */
#ifdef __cplusplus
extern "C" { extern char *ace_foo(const char *); }
#endif
int
ace_yyparse()
{
    register int ace_yym, ace_yyn, ace_yystate;
#if ACE_YYDEBUG
    register char *ace_yys;
#ifndef __cplusplus
    extern char *ace_foo();
#endif

    if ((ace_yys = ACE_OS::getenv("ACE_YYDEBUG")))
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
    if ((ace_yyn = ace_yydefred[ace_yystate])) goto ace_yyreduce;
    if (ace_yychar < 0)
    {
        if ((ace_yychar = ace_yylex()) < 0) ace_yychar = 0;
#if ACE_YYDEBUG
        if (ace_yydebug)
        {
            ace_yys = 0;
            if (ace_yychar <= ACE_YYMAXTOKEN) ace_yys = ace_yyname[ace_yychar];
            if (!ace_yys) ace_yys = "illegal-symbol";
            if (5 < ace_yydebug)
                printf("ace_yydebug: state %d, reading %d (%s)\n", ace_yystate,
                        ace_yychar, ace_yys);
            else
                ACE_YYDEBUG_LOOK_AHEAD(ace_yystate, ace_yychar, ace_yys, ace_yyssp-ace_yyss);
        }
#endif
    }
    if ((ace_yyn = ace_yysindex[ace_yystate]) && (ace_yyn += ace_yychar) >= 0 &&
            ace_yyn <= ACE_YYTABLESIZE && ace_yycheck[ace_yyn] == ace_yychar)
    {
#if ACE_YYDEBUG
        if (ace_yydebug)
            if (5 < ace_yydebug)
                printf("ace_yydebug: state %d, shifting to state %d\n",
                        ace_yystate, ace_yytable[ace_yyn]);
            else
                ACE_YYDEBUG_SHIFT_LEXEME(ace_yystate, ace_yytable[ace_yyn], ace_yys, ace_yyssp-ace_yyss);
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
            ace_yyn <= ACE_YYTABLESIZE && ace_yycheck[ace_yyn] == ace_yychar)
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
            if ((ace_yyn = ace_yysindex[*ace_yyssp]) && (ace_yyn += ACE_YYERRCODE) >= 0 &&
                    ace_yyn <= ACE_YYTABLESIZE && ace_yycheck[ace_yyn] == ACE_YYERRCODE)
            {
#if ACE_YYDEBUG
                if (ace_yydebug)
                    if (5 < ace_yydebug)
                        printf("ace_yydebug: state %d, error recovery shifting\
 to state %d\n", *ace_yyssp, ace_yytable[ace_yyn]);
                    else
                        ACE_YYDEBUG_SHIFT_ERROR_LEXEME(*ace_yyssp, ace_yytable[ace_yyn], ace_yyssp-ace_yyss);
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
#if ACE_YYDEBUG
                if (ace_yydebug)
                    if (5 < ace_yydebug)
                        printf("ace_yydebug: error recovery discarding state %d\
",
                            *ace_yyssp);
                    else
                        ACE_YYDEBUG_DISCARD_STATE(*ace_yyssp, ace_yyssp-ace_yyss-1);
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
#if ACE_YYDEBUG
        if (ace_yydebug)
        {
            ace_yys = 0;
            if (ace_yychar <= ACE_YYMAXTOKEN) ace_yys = ace_yyname[ace_yychar];
            if (!ace_yys) ace_yys = "illegal-symbol";
            if (5 < ace_yydebug)
                printf("ace_yydebug: state %d, error recovery discards token %d (%s)\n",
                    ace_yystate, ace_yychar, ace_yys);
            else
                ACE_YYDEBUG_DISCARD_TOKEN(ace_yystate, ace_yychar, ace_yys, ace_yyssp-ace_yyss);
        }
#endif
        ace_yychar = (-1);
        goto ace_yyloop;
    }
ace_yyreduce:
    ace_yym = ace_yylen[ace_yyn];
    ace_yyval = ace_yyvsp[1-ace_yym];
#if ACE_YYDEBUG
    if (ace_yydebug)
        if (5 < ace_yydebug)
            printf("ace_yydebug: state %d, reducing by rule %d (%s)\n",
                    ace_yystate, ace_yyn, ace_yyrule[ace_yyn]);
        else
            ACE_YYDEBUG_REDUCE(ace_yystate, ace_yyn, ace_yyrule[ace_yyn], ace_yyssp-ace_yyss-ace_yym, ace_yym);
#endif
    switch (ace_yyn)
    {
case 1:
#line 44 "Svc_Conf.y"
{
      if (ace_yyvsp[0].parse_node_ != 0)
      {
        ace_yyvsp[0].parse_node_->apply (); delete ace_yyvsp[0].parse_node_;
      }
      ace_obstack->release ();
    }
break;
case 2:
#line 52 "Svc_Conf.y"
{
      ace_obstack->release ();
    }
break;
case 10:
#line 69 "Svc_Conf.y"
{
      if (ace_yyvsp[-1].svc_record_ != 0)
        ace_yyval.parse_node_ = new ACE_Dynamic_Node (ace_yyvsp[-1].svc_record_, ace_yyvsp[0].ident_);
      else
        ace_yyval.parse_node_ = 0;
    }
break;
case 11:
#line 79 "Svc_Conf.y"
{
      ace_yyval.parse_node_ = new ACE_Static_Node (ace_yyvsp[-1].ident_, ace_yyvsp[0].ident_);
    }
break;
case 12:
#line 86 "Svc_Conf.y"
{
      ace_yyval.parse_node_ = new ACE_Suspend_Node (ace_yyvsp[0].ident_);
    }
break;
case 13:
#line 93 "Svc_Conf.y"
{
      ace_yyval.parse_node_ = new ACE_Resume_Node (ace_yyvsp[0].ident_);
    }
break;
case 14:
#line 100 "Svc_Conf.y"
{
      ace_yyval.parse_node_ = new ACE_Remove_Node (ace_yyvsp[0].ident_);
    }
break;
case 15:
#line 107 "Svc_Conf.y"
{
      ace_yyval.parse_node_ = new ACE_Stream_Node (ace_yyvsp[-1].static_node_, ace_yyvsp[0].parse_node_);
    }
break;
case 16:
#line 110 "Svc_Conf.y"
{ ace_yyval.static_node_ = new ACE_Static_Node (ace_yyvsp[0].ident_); }
break;
case 17:
#line 111 "Svc_Conf.y"
{
      ace_yyval.parse_node_ = new ACE_Dummy_Node (ace_yyvsp[-1].static_node_, ace_yyvsp[0].parse_node_);
    }
break;
case 18:
#line 118 "Svc_Conf.y"
{
    }
break;
case 19:
#line 121 "Svc_Conf.y"
{
    }
break;
case 20:
#line 127 "Svc_Conf.y"
{
      /* Initialize left context...*/
      ace_yyval.static_node_ = ace_yyvsp[-1].static_node_;
    }
break;
case 21:
#line 132 "Svc_Conf.y"
{
      ace_yyval.parse_node_ = ace_yyvsp[-1].parse_node_;
    }
break;
case 22:
#line 135 "Svc_Conf.y"
{ ace_yyval.parse_node_ = 0; }
break;
case 23:
#line 140 "Svc_Conf.y"
{
      if (ace_yyvsp[0].parse_node_ != 0)
        {
          ace_yyvsp[0].parse_node_->link (ace_yyvsp[-1].parse_node_);
          ace_yyval.parse_node_ = ace_yyvsp[0].parse_node_;
        }
    }
break;
case 24:
#line 147 "Svc_Conf.y"
{ ace_yyval.parse_node_ = 0; }
break;
case 25:
#line 152 "Svc_Conf.y"
{
      ACE_Static_Node *svc_type = ace_yyvsp[0].static_node_;

      if (svc_type != 0)
        {
          ACE_Static_Node *module = ace_yyvsp[-2].static_node_;

          ACE_ARGV args (ASYS_WIDE_STRING (svc_type->parameters ()));
          ACE_Module_Type *mt = ace_get_module (module,
                                                svc_type);
          ACE_Stream_Type *st =
            ACE_dynamic_cast (ACE_Stream_Type *,
                              ACE_const_cast (ACE_Service_Type_Impl *,
                                              module->record ()->type ()));

          if (mt->init (args.argc (), args.argv ()) == -1
              || st->push (mt) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          ASYS_TEXT ("dynamic initialization failed for Module %s\n"),
                          ASYS_WIDE_STRING (svc_type->name ())));
              ace_yyerrno++;
            }
        }
    }
break;
case 26:
#line 178 "Svc_Conf.y"
{
      ACE_Module_Type *mt = ace_get_module (ace_yyvsp[-2].static_node_, ace_yyvsp[0].static_node_->name ());

      if (((ACE_Stream_Type *) (ace_yyvsp[-2].static_node_)->record ()->type ())->push (mt) == -1)
        ace_yyerrno++;
    }
break;
case 27:
#line 185 "Svc_Conf.y"
{
      ACE_Module_Type *mt = ace_get_module (ace_yyvsp[-2].static_node_,
                                            ace_yyvsp[0].static_node_->name ());
      if (mt != 0)
        mt->suspend ();
    }
break;
case 28:
#line 192 "Svc_Conf.y"
{
      ACE_Module_Type *mt = ace_get_module (ace_yyvsp[-2].static_node_,
                                            ace_yyvsp[0].static_node_->name ());
      if (mt != 0)
        mt->resume ();
    }
break;
case 29:
#line 199 "Svc_Conf.y"
{
      ACE_Static_Node *stream = ace_yyvsp[-2].static_node_;
      ACE_Static_Node *module = ace_yyvsp[0].static_node_;
      ACE_Module_Type *mt = ace_get_module (stream,
                                            module->name ());

      ACE_Stream_Type *st =
        ACE_dynamic_cast (ACE_Stream_Type *,
                          ACE_const_cast (ACE_Service_Type_Impl *,
                                          stream->record ()->type ()));
      if (mt != 0 && st->remove (mt) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("cannot remove Module_Type %s from STREAM_Type %s\n"),
                      ASYS_WIDE_STRING (module->name ()),
                      ASYS_WIDE_STRING (stream->name ())));
          ace_yyerrno++;
        }
    }
break;
case 30:
#line 222 "Svc_Conf.y"
{
      u_int flags
        = ACE_Service_Type::DELETE_THIS
        | (ace_yyvsp[-1].location_node_->dispose () == 0 ? 0 : ACE_Service_Type::DELETE_OBJ);
      ACE_Service_Object_Exterminator gobbler = 0;
      void *sym = ace_yyvsp[-1].location_node_->symbol (&gobbler);

      if (sym != 0)
        {
          ACE_Service_Type_Impl *stp
            = ace_create_service_type (ASYS_WIDE_STRING (ace_yyvsp[-3].ident_),
                                       ace_yyvsp[-2].type_,
                                       sym,
                                       flags,
                                       gobbler);
          ace_yyval.svc_record_ = new ACE_Service_Type (ASYS_WIDE_STRING (ace_yyvsp[-3].ident_),
                                     stp,
                                     ace_yyvsp[-1].location_node_->handle (),
                                     ace_yyvsp[0].type_);
        }
      else
        {
          ++ace_yyerrno;
          ace_yyval.svc_record_ = 0;
        }
      delete ace_yyvsp[-1].location_node_;
    }
break;
case 31:
#line 253 "Svc_Conf.y"
{
      ace_yyval.type_ = 1;
    }
break;
case 32:
#line 257 "Svc_Conf.y"
{
      ace_yyval.type_ = 0;
    }
break;
case 33:
#line 261 "Svc_Conf.y"
{
      ace_yyval.type_ = 1;
    }
break;
case 34:
#line 268 "Svc_Conf.y"
{
      ace_yyval.location_node_ = new ACE_Object_Node (ace_yyvsp[-2].ident_, ace_yyvsp[0].ident_);
    }
break;
case 35:
#line 272 "Svc_Conf.y"
{
      ace_yyval.location_node_ = new ACE_Function_Node (ace_yyvsp[-4].ident_, ace_yyvsp[-2].ident_);
    }
break;
case 36:
#line 276 "Svc_Conf.y"
{
      ace_yyval.location_node_ = new ACE_Static_Function_Node (ace_yyvsp[-2].ident_);
    }
break;
case 37:
#line 283 "Svc_Conf.y"
{
      ace_yyval.type_ = ACE_MODULE_T;
    }
break;
case 38:
#line 287 "Svc_Conf.y"
{
      ace_yyval.type_ = ACE_SVC_OBJ_T;
    }
break;
case 39:
#line 291 "Svc_Conf.y"
{
      ace_yyval.type_ = ACE_STREAM_T;
    }
break;
case 41:
#line 298 "Svc_Conf.y"
{ ace_yyval.ident_ = 0; }
break;
#line 976 "Svc_Conf_y.cpp"
    }
    ace_yyssp -= ace_yym;
    ace_yystate = *ace_yyssp;
    ace_yyvsp -= ace_yym;
    ace_yym = ace_yylhs[ace_yyn];
    if (ace_yystate == 0 && ace_yym == 0)
    {
#ifdef ACE_YYDEBUG
        if (5 < ace_yydebug)
            printf("ace_yydebug: after reduction, shifting from state 0 to\
 state %d\n", ACE_YYFINAL);
#endif
        ace_yystate = ACE_YYFINAL;
        *++ace_yyssp = ACE_YYFINAL;
        *++ace_yyvsp = ace_yyval;
        if (ace_yychar < 0)
        {
            if ((ace_yychar = ace_yylex()) < 0) ace_yychar = 0;
#if ACE_YYDEBUG
            if (ace_yydebug)
            {
                ace_yys = 0;
                if (ace_yychar <= ACE_YYMAXTOKEN) ace_yys = ace_yyname[ace_yychar];
                if (!ace_yys) ace_yys = "illegal-symbol";
                if (5 < ace_yydebug)
                    printf("ace_yydebug: state %d, reading %d (%s)\n",
                        ACE_YYFINAL, ace_yychar, ace_yys);
                else
                    ACE_YYDEBUG_LOOK_AHEAD(ACE_YYFINAL, ace_yychar, ace_yys, ace_yyssp-ace_yyss);
            }
#endif
        }
        if (ace_yychar == 0) goto ace_yyaccept;
        goto ace_yyloop;
    }
    if ((ace_yyn = ace_yygindex[ace_yym]) && (ace_yyn += ace_yystate) >= 0 &&
            ace_yyn <= ACE_YYTABLESIZE && ace_yycheck[ace_yyn] == ace_yystate)
        ace_yystate = ace_yytable[ace_yyn];
    else
        ace_yystate = ace_yydgoto[ace_yym];
#ifdef ACE_YYDEBUG
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
