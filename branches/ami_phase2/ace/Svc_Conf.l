%{
// $Id$
// Sample lexical analysis for regular expression subset.  Must be
// compiled with FLEX and an ANSI C++ compiler.

// Lexical tokens values defined by YACC.
#include "ace/Svc_Conf.h"
#include "ace/Svc_Conf_Tokens.h"

ACE_RCSID(ace, Svc_Conf_l, "$Id$")

// Keeps track of the current line for debugging output.
int yylineno = 1;

// Keeps track of the number of errors encountered so far.
int yyerrno = 0;

// Used to parse service configurator directives from a string rather
// than from a svc.conf file.
const ASYS_TCHAR *yydirective = 0;

#define token(x)  x
%}

%s PARAMETERS
%s NORMAL

letter	        [a-zA-Z_]
letter_or_digit	[a-zA-Z_0-9]
digit           [0-9]
ident           {letter}{letter_or_digit}*
pathname        ([A-Za-z\%]:)?[a-zA-Z_0-9/\%\.\\-]+
symbol		[ -~]
string		\"{symbol}*\"
white_space	[ \t]
newline         \n
other		.

%%

^#{other}*$   	      ; /* EMPTY */
dynamic	              { return token (ACE_DYNAMIC); }
static	              { return token (ACE_STATIC); }
suspend	              { return token (ACE_SUSPEND); }
resume	              { return token (ACE_RESUME); }
remove	              { return token (ACE_REMOVE); }
stream	              { return token (ACE_USTREAM); }
Module		      { return token (ACE_MODULE_T); }
Service_Object	      { return token (ACE_SVC_OBJ_T); }
STREAM		      { return token (ACE_STREAM_T); }
active	              { return token (ACE_ACTIVE); }
inactive	      { return token (ACE_INACTIVE); }
":"		      { return token (ACE_COLON); }
"*"		      { return token (ACE_STAR); }
"("                   { return token (ACE_LPAREN); }
")"                   { return token (ACE_RPAREN); }
"{"                   { return token (ACE_LBRACE); }
"}"                   { return token (ACE_RBRACE); }
{string}	      { // Eliminate the opening and closing double quotes
		        *strrchr (yytext, '"') = '\0';
			yyleng -= 1;
                        yylval.ident_ = ace_obstack->copy (yytext + 1, yyleng);
			return token (ACE_STRING); }
{ident}		      {
		        yylval.ident_ = ace_obstack->copy (yytext, yyleng);
			return token (ACE_IDENT);
		      }
{pathname}	      {
		        yylval.ident_ = ace_obstack->copy (yytext, yyleng);
			return token (ACE_PATHNAME);
		      }
{white_space}+	      ; /* EMPTY */
{newline}	      { yylineno++; }
{other}		      { ACE_ERROR ((LM_ERROR, "unknown char = %d\n", *yytext)); }
<<EOF>>               { YY_NEW_FILE; yyterminate(); }
%%
int
yywrap (void)
{
  ::fflush (yyin);
  yytext[0] = '#';
  yyleng = 0;

  // This needs to be freed to prevent a memory leak.
  yy_delete_parse_buffer ();

  return 1;
}

void
yy_delete_parse_buffer (void)
{
  if (yy_current_buffer != 0) 
    {
      yy_delete_buffer (yy_current_buffer);
      yy_current_buffer = 0;
    }
}
