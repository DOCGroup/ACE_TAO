%{
TAO_END_VERSIONED_NAMESPACE_DECL

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

#include "orbsvcs/Trader/Constraint_Interpreter.h"
#include "orbsvcs/Trader/Constraint_Nodes.h"
#include "orbsvcs/Trader/Constraint_Tokens.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

static TAO_Literal_Constraint* extract_string(const char*);
static CORBA::LongLong trader_strtoll(const char* str, unsigned int base);
static CORBA::ULongLong trader_strtoull(const char* str, unsigned int base);

#define TAO_YY_LEX_DEBUG

#ifdef TAO_CONSTRAINT_DEBUG
#define TAO_YY_LEX_DEBUG ACE_OS::fprintf(stderr, "%s\n", yytext)
#endif /* TAO_CONSTRAINT_DEBUG */


%}

white_space     [ \t]
letter		[a-zA-Z]
digit		[0-9]
alpha_num	({letter}|{digit})
integer		[-+]?{digit}+
float		[-+]?({digit}*\.{digit}+)([eE][-+]?{digit}+)?
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
                    new TAO_Literal_Constraint(true);
		  TAO_YY_LEX_DEBUG; return TAO_BOOLEAN;
		}
FALSE		{
		  yylval.constraint_ =
                    new TAO_Literal_Constraint(false);
		  TAO_YY_LEX_DEBUG; return TAO_BOOLEAN;
		}
{integer}	{
                  if (yytext[0] == '-')
                    {
                      yylval.constraint_ =
		        new TAO_Literal_Constraint(
                              trader_strtoll(yytext, 0));
                    }
                  else
                    {
                       yylval.constraint_ =
                         new TAO_Literal_Constraint(
                               trader_strtoull(yytext, 0));
                    }
		  TAO_YY_LEX_DEBUG; return TAO_NUMBER;
		}
{float}		{
		  yylval.constraint_ =
		    new TAO_Literal_Constraint(ACE_OS::strtod(yytext, 0));
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
{white_space}   {}
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

CORBA::ULongLong
trader_strtoull(const char* s, unsigned int base)
{
  CORBA::ULongLong result = 0;
  CORBA::ULongLong previous = 0;

  // Check for a valid base
  if (!(base == 0 || base == 8 || base == 10 || base == 16))
    {
      errno = EINVAL;
      return result;
    }

  if (*s == '+')
    {
      s++;
    }

  if ((base == 0 || base == 16) &&
      *s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X'))
    {
      s += 2;
      base = 16;
    }
  else if ((base == 0 || base == 8) && *s == '0')
    {
      s++;
      base = 8;
    }
  else
    {
      base = 10;
    }

  for (; *s; ++s)
    {
      if (base == 8 && *s <= '7' && *s >= '0')
        {
          result = (result * base) + (*s - '0');
        }
      else if (base > 8 && *s <= '9' && *s >= '0')
        {
          result = (result * base) + (*s - '0');
        }
      else if (base > 10 && *s <= 'f' && *s >= 'a')
        {
          result = (result * base) + (*s - 'a' + 10);
        }
      else if (base > 10 && *s <= 'F' && *s >= 'A')
        {
          result = (result * base) + (*s - 'A' + 10);
        }
      else
        { 
          break;
        }

      // If the previous value is greater than result, then we have
      // exceeded the size of a CORBA::ULongLong and the result
      // will be ACE_UINT64_MAX.  However, errno will be set to ERANGE
      if (previous > result)
        {
          errno = ERANGE;
          result = ACE_UINT64_MAX;
          break;
        }
      previous = result;
    }

  return result;
}

CORBA::LongLong
trader_strtoll(const char* s, unsigned int base)
{
  // Check for the negative sign
  bool negative = false;
  if (*s == '-')
    {
      negative = true;
      s++;
    }

  // Convert the raw text into a CORBA::LongLong
  CORBA::LongLong result = static_cast<CORBA::LongLong> (
                                         trader_strtoull(s, base));

  // If the result that comes back is negative, then the value exceeded
  // the maximum for CORBA::LongLong
  if (result < 0)
    {
      errno = ERANGE;
      return (negative ? ACE_INT64_MIN : ACE_INT64_MAX);
    }

  return (negative ? -result : result);
}

int
yywrap (void)
{
  return 1;
}

TAO_END_VERSIONED_NAMESPACE_DECL
