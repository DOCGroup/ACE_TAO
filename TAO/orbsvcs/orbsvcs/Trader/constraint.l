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

#include "Constraint.h"
#include "Constraint_Nodes.h"
#include "Constraint_Tokens.h"

static ACE_Literal_Constraint* extract_string(const char*);

#define ACE_YY_LEX_DEBUG

#ifdef ACE_CONSTRAINT_DEBUG
#define ACE_YY_LEX_DEBUG ACE_OS::fprintf(stderr, "%s\n", yytext)
#endif /* ACE_CONSTRAINT_DEBUG */


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

min             { ACE_YY_LEX_DEBUG; return MIN; }
max             { ACE_YY_LEX_DEBUG; return MAX; }
first           { ACE_YY_LEX_DEBUG; return FIRST; }
random          { ACE_YY_LEX_DEBUG; return RANDOM; }
with            { ACE_YY_LEX_DEBUG; return WITH; }
exist		{ ACE_YY_LEX_DEBUG; return EXIST; }
not		{ ACE_YY_LEX_DEBUG; return NOT; }
and		{ ACE_YY_LEX_DEBUG; return AND; }
or		{ ACE_YY_LEX_DEBUG; return OR; }
in		{ ACE_YY_LEX_DEBUG; return IN; }
"~"             { ACE_YY_LEX_DEBUG; return TWIDDLE; }
"+"		{ ACE_YY_LEX_DEBUG; return PLUS; }
"-"		{ ACE_YY_LEX_DEBUG; return MINUS; }
"*"		{ ACE_YY_LEX_DEBUG; return MULT; }
"/"		{ ACE_YY_LEX_DEBUG; return DIV; }
"<"		{ ACE_YY_LEX_DEBUG; return LT; }
"<="		{ ACE_YY_LEX_DEBUG; return LE; }
">"		{ ACE_YY_LEX_DEBUG; return GT; }
">="		{ ACE_YY_LEX_DEBUG; return GE; }
"=="		{ ACE_YY_LEX_DEBUG; return EQ; }
"!="		{ ACE_YY_LEX_DEBUG; return NE; }
"("             { ACE_YY_LEX_DEBUG; return LPAREN; }
")"             { ACE_YY_LEX_DEBUG; return RPAREN; }
TRUE		{ 
		  yylval.constraint_ = 
		    new ACE_Literal_Constraint((CORBA::Boolean)TRUE_CON);
		  ACE_YY_LEX_DEBUG; return BOOLEAN;
		}
FALSE		{ 
		  yylval.constraint_ = 
		    new ACE_Literal_Constraint((CORBA::Boolean)FALSE_CON);
		  ACE_YY_LEX_DEBUG; return BOOLEAN;
		}
{integer}	{ 
		  yylval.constraint_ = 
		    new ACE_Literal_Constraint((CORBA::Long)atoi(yytext));
		  ACE_YY_LEX_DEBUG; return NUMBER; 
		}
{float}		{ 
		  yylval.constraint_ = 
		    new ACE_Literal_Constraint((CORBA::Double)atof(yytext));
		  ACE_YY_LEX_DEBUG; return NUMBER; 
		}
{string}	{ 
		  yylval.constraint_ = extract_string(yytext);
		  ACE_YY_LEX_DEBUG; return STRING; 
		}
{ident}		{ 
		  yylval.constraint_ = 
		    new ACE_Property_Constraint(yytext);
		  ACE_YY_LEX_DEBUG; return IDENT; 
		}
{unknown}       { 
                  ACE_YY_LEX_DEBUG; return UNKNOWN;
                }
%%

ACE_Literal_Constraint*
extract_string(const char* total)
{
  int prev_slash = 0, 
    ctr = 0;
  char str[BUFSIZ],
   *tmp = total + 1;

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
  return new ACE_Literal_Constraint(str);
}