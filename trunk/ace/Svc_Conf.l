%{
// $Id$
// Sample lexical analysis for regular expression subset.  Must be
// compiled with FLEX and an ANSI C++ compiler.

// Lexical tokens values defined by YACC.
#include "ace/Svc_Conf.h"
#include "ace/Svc_Conf_Tokens.h"

ACE_RCSID (ace,
	   Svc_Conf_l,
	   "$Id$")

// Keeps track of the current line for debugging output.
int yylineno = 1;

// Array that implements the underlying lexer buffer stack.
YY_BUFFER_STATE yybuffer_stack[ACE_SERVICE_DIRECTIVE_STACK_DEPTH];

// Array index of the buffer currently in use.
int yy_stack_index = 0;

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
{string}	      { // Check for first type of string, i.e.,
                        // "double quotes" delimited.  
                        char *s = strrchr (yytext, '"');
                        if (s == 0)
                          // Check for second type of string, i.e.,
                          // 'single quotes' delimited.
                          s = strrchr (yytext, '\'');

                        ACE_ASSERT (s != 0);
                        // Eliminate the opening and closing double or
                        // single quotes.
                        *s = '\0';
			yyleng -= 1;
                        yylval->ident_ = ace_obstack->copy (yytext + 1, yyleng);
			return token (ACE_STRING); }
{ident}		      {
		        yylval->ident_ = ace_obstack->copy (yytext, yyleng);
			return token (ACE_IDENT);
		      }
{pathname}	      {
		        yylval->ident_ = ace_obstack->copy (yytext, yyleng);
			return token (ACE_PATHNAME);
		      }
{white_space}+	      ; /* EMPTY */
{newline}	      { yylineno++; }
{other}		      { ACE_ERROR ((LM_ERROR,
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
yy_push_buffer (FILE *file)
{
  // External synchronization is required.

  if (yy_stack_index >= ACE_SERVICE_DIRECTIVE_STACK_DEPTH)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Service Configurator directive nesting "
                  "is too deep.\n"
                  "(%P|%t) Consider increasing value of "
                  "ACE_SERVICE_DIRECTIVE_STACK_DEPTH.\n"));

      // Not much we can do, so resort to flushing the current buffer
      // and switch to the supplied stream.
      yyrestart (file);
    }
  else
    {
      yybuffer_stack[yy_stack_index++] = YY_CURRENT_BUFFER;
      yy_switch_to_buffer (yy_create_buffer (file, YY_BUF_SIZE));
    }
}

void
yy_push_buffer (const char *directive)
{
  // External synchronization is required.

  if (yy_stack_index >= ACE_SERVICE_DIRECTIVE_STACK_DEPTH)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) Service Configurator directive nesting "
                  "is too deep.\n"
                  "(%P|%t) Consider increasing value of "
                  "ACE_SERVICE_DIRECTIVE_STACK_DEPTH.\n"));

      // Not much we can do.
    }
  else
    {
      yybuffer_stack[yy_stack_index++] = YY_CURRENT_BUFFER;

      // yy_scan_string() already switches the buffer so setting
      // YY_CURRENT_BUFFER here is a bit redundant.  No biggy.
      YY_CURRENT_BUFFER = yy_scan_string (directive);
    }
}

void
yy_pop_buffer (void)
{
  // External synchronization is required.

  if (--yy_stack_index >= 0)
    {
      yy_delete_buffer (YY_CURRENT_BUFFER);
      yy_switch_to_buffer (yybuffer_stack[yy_stack_index]);
    }
}
