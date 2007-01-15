%option noyywrap

%{
// $Id$
// ========================================================================
//
// = LIBRARY
//   orbsvcs/PSS
//
// = FILENAME
//   PSDL.ll
//
// = AUTHOR
//   Priyanka Gontla <gontla_p@ociweb.com>
// ========================================================================

#include "ace/OS.h"

#include "PSDL_Interpreter.h"
#include "PSDL_Node.h"
#include "PSDL_y.h"

static const char * extract_string(char*);

//#define TAO_YY_LEX_DEBUG

#define TAO_YY_LEX_DEBUG ACE_OS::fprintf(stderr, "%s\n", yytext)

#define YY_DECL int TAO_PSDL_yylex (TAO_PSDL_Node **lvalp, void* )

#define YY_BREAK
#define YY_NO_UNPUT

%}

white_space     \t
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

";"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_SEMI; }
":"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_COLON; }
"{"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_OPEN_CURLY_BRACE; }
"}"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_CLOSE_CURLY_BRACE; }
"("		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_OPEN_BRACE; }
")"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_CLOSE_BRACE; }
"<"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_OPEN_ANGULAR_BRACE; }
">"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_CLOSE_ANGULAR_BRACE; }
","		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_COMMA; }
"+"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_PLUS; }
"-"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_MINUS; }
"^"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_XOR; }
"&"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_AND; }
"*"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_MULT; }
">>"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_RIGHT_SHIFT; }
"<<"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_LEFT_SHIFT; }
"/"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_DIV; }
"%"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_PERCENT; }
"~"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_TILDA; }
"="		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_EQUAL; }
"|"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_OR; }
"::"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_DOUBLE_COLON; }
"["		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_OPEN_SQUARE_BRACE; }
"]"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_CLOSE_SQUARE_BRACE; }
"in"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_IN; }
"as"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_AS; }
"of"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_OF; }
"key"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_KEY; }
"ref"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_REF; }
"any"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_ANY; }
"out"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_OUT; }
"case"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_CASE; }
"inout"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_INOUT; }
"catalog"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_CATALOG; }
"provides"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_PROVIDES; }
"char"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_CHAR; }
"implements"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_IMPLEMENTS; }
"scope"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_SCOPE; }
"stores"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_STORES; }
"state"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_STATE; }
"abstract"      { TAO_YY_LEX_DEBUG; return TAO_PSDL_ABSTRACT; }
"storagetype"   { TAO_YY_LEX_DEBUG; return TAO_PSDL_STORAGETYPE; }
"storagehome"   { TAO_YY_LEX_DEBUG; return TAO_PSDL_STORAGEHOME; }
"strong"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_STRONG; }
"primary"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_PRIMARY; }
"readonly"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_READONLY; }
"union"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_UNION; }
"switch"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_SWITCH; }
"wchar"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_WCHAR; }
"boolean"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_BOOLEAN; }
"octet"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_OCTET; }
"Object"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_OBJECT; }
"struct"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_STRUCT; }
"default"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_DEFAULT; }
"float"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_FLOAT; }
"double"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_DOUBLE; }
"long"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_LONG; }
"short"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_SHORT; }
"unsigned"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_UNSIGNED; }
"typedef"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_TYPEDEF; }
"native"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_NATIVE; }
"module"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_MODULE; }
"oneway"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_ONEWAY; }
"const"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_CONST; }
"void"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_VOID; }
"raises"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_RAISES; }
"context"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_CONTEXT; }
"fixed"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_FIXED; }
"ValueBase"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_VALUEBASE; }
"exception"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_EXCEPTION; }
"interface"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_INTERFACE; }
"local"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_LOCAL; }
"valuetype"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_VALUETYPE; }
"custom"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_CUSTOM; }
"truncatable"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_TRUNCATABLE; }
"supports"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_SUPPORTS; }
"string"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_STRING; }
"wstring"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_WSTRING; }
"attribute"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_ATTRIBUTE; }
"enum"		{ TAO_YY_LEX_DEBUG; return TAO_PSDL_ENUM; }
"sequence" 	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_SEQUENCE; }
"private"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_PRIVATE; }
"public"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_PUBLIC; }
"factory"	{ TAO_YY_LEX_DEBUG; return TAO_PSDL_FACTORY; }
"TRUE"		{
		  *lvalp =
		    new TAO_PSDL_Literal ((CORBA::Boolean) 1);
		  TAO_YY_LEX_DEBUG; return TAO_PSDL_TRUE;
		}
"FALSE"		{
		  *lvalp =
		    new TAO_PSDL_Literal ((CORBA::Boolean) 0);
		  TAO_YY_LEX_DEBUG; return TAO_PSDL_FALSE;
		}
{integer}	{
		  *lvalp =
			new TAO_PSDL_Literal (ACE_OS::atoi (yytext));
		  TAO_YY_LEX_DEBUG; return TAO_PSDL_INTEGER;
		}
{float}		{
	  	  double v;
		  sscanf (yytext, "%lf", &v);
		  *lvalp =
		    new TAO_PSDL_Literal (v);
		  TAO_YY_LEX_DEBUG; return TAO_PSDL_FLOATING_PT;
		}
{ident}		{
		  *lvalp =
		    new TAO_PSDL_Identifier (yytext);
		  TAO_YY_LEX_DEBUG; return TAO_PSDL_IDENT;
		}
{string}	{
		  *lvalp =
		    new TAO_PSDL_Literal (extract_string (yytext));
		  TAO_YY_LEX_DEBUG; return TAO_PSDL_STRING;
		}
{white_space}   {
                  TAO_YY_LEX_DEBUG; break; // Ignore
                }
{newline}	{
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
yywrap ()
{
  return 1;
}
