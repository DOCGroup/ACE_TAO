%{
// $Id$
// ========================================================================
//
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    constraint.l
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

#include "ace/OS.h"
#include "Constraint_Interpreter.h"
#include "Constraint_Nodes.h"
#include "Constraint_Tokens.h"

static TAO_Literal_Constraint* extract_string(const char*);

#define TAO_YY_LEX_DEBUG

#ifdef TAO_CONSTRAINT_DEBUG
#define TAO_YY_LEX_DEBUG TAO_OS::fprintf(stderr, "%s\n", yytext)
#endif /* TAO_CONSTRAINT_DEBUG */


%}

white_space     [ \t]
letter		[a-zA-Z]
digit		[0-9]
alpha_num	({letter}|{digit})
integer		{digit}+ 
float		({digit}*\.{digit}+)([eE][-+]?{digit}+)? 
string		'(([^'\\]*)|([^'\\]*\\')|([^'\\]*\\\\))*'
ident		{letter}({alpha_num}|[_])*
newline		\n
unknown         [^ \t]

%%

min             { TAO_YY_LEX_DEBUG; return TAO_MIN; }
max             { TAO_YY_LEX_DEBUG; return TAO_MAX; }
first           { TAO_YY_LEX_DEBUG; return TAO_FIRST; }
random          { TAO_YY_LEX_DEBUG; return TAO_RANDOM; }
with            { TAO_YY_LEX_DEBUG; return TAO_WITH; }
exist		{ TAO_YY_LEX_DEBUG; return TAO_EXIST; }
not		{ TAO_YY_LEX_DEBUG; return TAO_NOT; }
and		{ TAO_YY_LEX_DEBUG; return TAO_AND; }
or		{ TAO_YY_LEX_DEBUG; return TAO_OR; }
in		{ TAO_YY_LEX_DEBUG; return TAO_IN; }
"~"             { TAO_YY_LEX_DEBUG; return TAO_TWIDDLE; }
"+"		{ TAO_YY_LEX_DEBUG; return TAO_PLUS; }
"-"		{ TAO_YY_LEX_DEBUG; return TAO_MINUS; }
"*"		{ TAO_YY_LEX_DEBUG; return TAO_MULT; }
"/"		{ TAO_YY_LEX_DEBUG; return TAO_DIV; }
"<"		{ TAO_YY_LEX_DEBUG; return TAO_LT; }
"<="		{ TAO_YY_LEX_DEBUG; return TAO_LE; }
">"		{ TAO_YY_LEX_DEBUG; return TAO_GT; }
">="		{ TAO_YY_LEX_DEBUG; return TAO_GE; }
"=="		{ TAO_YY_LEX_DEBUG; return TAO_EQ; }
"!="		{ TAO_YY_LEX_DEBUG; return TAO_NE; }
"("             { TAO_YY_LEX_DEBUG; return TAO_LPAREN; }
")"             { TAO_YY_LEX_DEBUG; return TAO_RPAREN; }
TRUE		{ 
		  yylval.constraint_ = 
		    new TAO_Literal_Constraint((CORBA::Boolean) 1);
		  TAO_YY_LEX_DEBUG; return TAO_BOOLEAN;
		}
FALSE		{ 
		  yylval.constraint_ = 
		    new TAO_Literal_Constraint((CORBA::Boolean) 0);
		  TAO_YY_LEX_DEBUG; return TAO_BOOLEAN;
		}
{integer}	{ 
		  yylval.constraint_ = 
		    new TAO_Literal_Constraint((CORBA::Long)atoi(yytext));
		  TAO_YY_LEX_DEBUG; return TAO_NUMBER; 
		}
{float}		{ 
		  yylval.constraint_ = 
		    new TAO_Literal_Constraint((CORBA::Double)atof(yytext));
		  TAO_YY_LEX_DEBUG; return TAO_NUMBER; 
		}
{string}	{ 
		  yylval.constraint_ = extract_string(yytext);
		  TAO_YY_LEX_DEBUG; return TAO_STRING; 
		}
{ident}		{ 
		  yylval.constraint_ = 
		    new TAO_Property_Constraint(yytext);
		  TAO_YY_LEX_DEBUG; return TAO_IDENT; 
		}
{unknown}       { 
                  TAO_YY_LEX_DEBUG; return TAO_UNKNOWN;
                }
%%

TAO_Literal_Constraint*
extract_string(const char* total)
{
  int prev_slash = 0, 
    ctr = 0;
  char str[BUFSIZ],
   *tmp = (char*) total + 1;

  while (*tmp != '\0')
    {
      if (*tmp == '\\')
        {
          if (prev_slash)
            prev_slash = 0;
          else
            {
              prev_slash = 1;
              continue;
            }
        }
      else if (*tmp == '\'')
        prev_slash = 0;

      str[ctr++] = *tmp;
      tmp++;
    }

  str[ctr - 1] = '\0';
  return new TAO_Literal_Constraint(str);
}