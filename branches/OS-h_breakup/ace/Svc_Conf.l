%{
// $Id$
// Sample lexical analysis for regular expression subset.  Must be
// compiled with FLEX and an ANSI C++ compiler.

// Lexical tokens values defined by YACC.
#include "ace/Svc_Conf.h"
#include "ace/Svc_Conf_Tokens.h"
#include "ace/Svc_Conf_Lexer_Guard.h"

ACE_RCSID (ace,
	   Svc_Conf_l,
	   "$Id$")

// Keeps track of the current line for debugging output.
int yylineno = 1;

#define token(x)  x
%}

%s PARAMETERS
%s NORMAL

letter	        [a-zA-Z_]
letter_or_digit	[a-zA-Z_0-9]
digit           [0-9]
ident           {letter}{letter_or_digit}*
pathname        ([A-Za-z\%]:)?[a-zA-Z_0-9/\%\.\\~-]+
symbol		[ -~]
string		(\"{symbol}*\"|\'{symbol}*\')
white_space	[ \t]
newline         \n
carrage_retn    \r
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
":"		      { return token (':'); }
"*"		      { return token ('*'); }
"("                   { return token ('('); }
")"                   { return token (')'); }
"{"                   { return token ('{'); }
"}"                   { return token ('}'); }
{string}	      {
                        // Check for first type of string, i.e.,
                        // "double quotes" delimited.  
                        ACE_TCHAR *s = ACE_OS::strrchr (yytext, '"');
                        if (s == 0)
                          // Check for second type of string, i.e.,
                          // 'single quotes' delimited.
                          s = ACE_OS::strrchr (yytext, '\'');

                        ACE_ASSERT (s != 0);
                        // Eliminate the opening and closing double or
                        // single quotes.
                        *s = '\0';
			yyleng -= 1;
                        yylval->ident_ = ACE_SVC_CONF_PARAM->obstack.copy (yytext + 1, yyleng);
			return token (ACE_STRING); }
{ident}		      {
		        yylval->ident_ = ACE_SVC_CONF_PARAM->obstack.copy (yytext, yyleng);
			return token (ACE_IDENT);
		      }
{pathname}	      {
		        yylval->ident_ = ACE_SVC_CONF_PARAM->obstack.copy (yytext, yyleng);
			return token (ACE_PATHNAME);
		      }
{white_space}+	      ; /* EMPTY */
{carrage_retn}        ; /* EMPTY */
{newline}	      { ACE_SVC_CONF_PARAM->yylineno++; ace_yylineno++; }
{other}		      {
                         ACE_ERROR ((LM_ERROR,
                                     ACE_LIB_TEXT ("unknown character = (%d"), 
                                                   *yytext));
                        if (ACE_OS::ace_isprint (*yytext))
                          ACE_ERROR ((LM_ERROR, ACE_LIB_TEXT ("|%c"), *yytext));
                        ACE_ERROR ((LM_ERROR, ACE_LIB_TEXT (")\n")));
                      }
<<EOF>>               { yyterminate(); }
%%
int
yywrap (void)
{
  ::fflush (yyin);
  yytext[0] = '#';
  yyleng = 0;

  return 1;
}

void
yy_push_buffer (FILE *file, yy_buffer_state *&buffer)
{
  // External synchronization is required.

  if (buffer == 0)
    buffer = yy_create_buffer (file, YY_BUF_SIZE);

  yy_switch_to_buffer (buffer);
}

void
yy_push_buffer (const ACE_TCHAR *directive, yy_buffer_state *&buffer)
{
  // External synchronization is required.

  // yyparse() may invoke yylex() multiple times when parsing
  // a single directive.  Prevent a new buffer from created during
  // each call to yylex().
  if (YY_CURRENT_BUFFER != 0
      && directive == YY_CURRENT_BUFFER->yy_ch_buf)
    return;

  if (buffer == 0)
    {
      // yy_scan_string() already switches the buffer so there is
      // no need to explicitly make the switch.
      buffer = yy_scan_string (directive);
    }
  else
    yy_switch_to_buffer (buffer);
}

void
yy_pop_buffer (yy_buffer_state *buffer)
{
  // External synchronization is required.

  yy_switch_to_buffer (buffer);
}
