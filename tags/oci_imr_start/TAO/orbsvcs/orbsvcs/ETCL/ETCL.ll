%option noyywrap

%{
// ETCL.ll,v 1.3 2002/01/14 19:52:28 parsons Exp
// ========================================================================
//
// = LIBRARY
//   orbsvcs/ECTL
// 
// = FILENAME
//   ETCL.ll
//
// = AUTHOR
//   Carlos O'Ryan <coryan@uci.edu> based on previous work by
//   Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

#include "ace/OS.h"

#include "ETCL_Constraint.h"
#include "ETCL_y.h"

static const char * extract_string(char*);

#define TAO_YY_LEX_DEBUG

#ifdef TAO_CONSTRAINT_DEBUG
#define TAO_YY_LEX_DEBUG TAO_OS::fprintf(stderr, "%s\n", yytext)
#endif /* TAO_CONSTRAINT_DEBUG */

#define YY_DECL int TAO_ETCL_yylex (TAO_ETCL_YYSTYPE *lvalp, void* state)

#define YY_BREAK
#define YY_NO_UNPUT

%}

white_space     [ \t]
letter		[a-zA-Z]
digit		[0-9]
alpha_num	({letter}|{digit})
integer		{digit}+ 
float		({digit}*\.{digit}+)([eE][-+]?{digit}+)? 
string		'(([^'\\]*)|([^'\\]*\\')|([^'\\]*\\\\))*'
base		{letter}({alpha_num}|[_])*
ident		{base}|\\{base}
newline		\n

%%

min             { TAO_YY_LEX_DEBUG; return TAO_ETCL_MIN; }
max             { TAO_YY_LEX_DEBUG; return TAO_ETCL_MAX; }
first           { TAO_YY_LEX_DEBUG; return TAO_ETCL_FIRST; }
random          { TAO_YY_LEX_DEBUG; return TAO_ETCL_RANDOM; }
with            { TAO_YY_LEX_DEBUG; return TAO_ETCL_WITH; }
exist		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_EXIST; }
not		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_NOT; }
and		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_AND; }
or		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_OR; }
in		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_IN; }
"~"             { TAO_YY_LEX_DEBUG; return TAO_ETCL_TWIDDLE; }
"+"		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_PLUS; }
"-"		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_MINUS; }
"*"		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_MULT; }
"/"		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_DIV; }
"<"		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_LT; }
"<="		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_LE; }
">"		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_GT; }
">="		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_GE; }
"=="		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_EQ; }
"!="		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_NE; }
"("             { TAO_YY_LEX_DEBUG; return TAO_ETCL_LPAREN; }
")"             { TAO_YY_LEX_DEBUG; return TAO_ETCL_RPAREN; }
"$"		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_DOLLAR; }
"."		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_DOT; }
"default"	{ TAO_YY_LEX_DEBUG; return TAO_ETCL_DEFAULT; }
"_d"		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_DISCRIMINANT; }
"_type_id"	{ TAO_YY_LEX_DEBUG; return TAO_ETCL_TYPE_ID; }
"_repos_id"	{ TAO_YY_LEX_DEBUG; return TAO_ETCL_REPOS_ID; }
"_length"	{ TAO_YY_LEX_DEBUG; return TAO_ETCL_LENGTH; }
"["		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_LBRA; }
"]"		{ TAO_YY_LEX_DEBUG; return TAO_ETCL_RBRA; }
TRUE		{ 
		  lvalp->constraint = 
		    new TAO_ETCL_Literal_Constraint ((CORBA::Boolean) 1);
		  TAO_YY_LEX_DEBUG; return TAO_ETCL_BOOLEAN;
		}
FALSE		{ 
		  lvalp->constraint = 
		    new TAO_ETCL_Literal_Constraint ((CORBA::Boolean) 0);
		  TAO_YY_LEX_DEBUG; return TAO_ETCL_BOOLEAN;
		}
{integer}	{ 
		  lvalp->constraint = 
		    new TAO_ETCL_Literal_Constraint (ACE_OS::atoi (yytext));
		  TAO_YY_LEX_DEBUG; return TAO_ETCL_INTEGER; 
		}
{float}		{
		  double v;
		  sscanf (yytext, "%lf", &v); 
		  lvalp->constraint = 
		    new TAO_ETCL_Literal_Constraint (v);
		  TAO_YY_LEX_DEBUG; return TAO_ETCL_FLOAT; 
		}
{string}	{ 
		  lvalp->constraint =
		    new TAO_ETCL_Literal_Constraint (extract_string (yytext));
		  TAO_YY_LEX_DEBUG; return TAO_ETCL_STRING; 
		}
{ident}		{ 
		  lvalp->constraint = 
		    new TAO_ETCL_Identifier (yytext);
		  TAO_YY_LEX_DEBUG; return TAO_ETCL_IDENT; 
		}
{white_space}   { 
                  TAO_YY_LEX_DEBUG; break; // Ignore
                }
.               { 
                  TAO_YY_LEX_DEBUG; break; // @@ TODO
                }
%%

const char*
extract_string(char* str)
{
  char *t = str;
  for (char * i = str + 1; *i != '\''; ++i, ++t)
    {
      if (*i == '\\')
        {
          ++i;
          if (*i == 0)
            return 0;
          else if (*i == 't')
            *t = '\t';
          else if (*i == 'n')
            *t = '\n';
          else if (*i == '\\')
            *t = '\\';
          else
            *t = *i;
          continue;
        }

      *t = *i;     
    }

  *t = '\0';
  return str;
}

int
yywrap (void)
{
  return 1;
}
