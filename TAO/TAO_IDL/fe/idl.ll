
%{
/*  $Id$

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

IDL_INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
IDL_ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

IDL_INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
IDL_ANY OBLIGATION ON THE PART OF Sun OR IDL_ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IDL_IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR IDL_ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR IDL_ANY PATENTS BY
IDL_INTERFACE DEFINITION LANGUAGE CFE OR IDL_ANY PART THEREOF.

IDL_IN NO EVENT WILL SUN OR IDL_ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
IDL_ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.
2550 Garcia Avenue
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

 */

/*
 * idl.ll - Lexical scanner for IDL 1.1
 */

#include "idl.h"
#include "idl_extern.h"
#include "fe_private.h"
#include "y.tab.h"

static char	idl_escape_reader(char *);
static double	idl_atof(char *);
static long	idl_atoi(char *, long);
static void	idl_parse_line_and_file(char *);
static void	idl_store_pragma(char *);

#if 0 /* defined (HPUX) */
// HPUX has yytext typed to unsigned char *. We make sure here that
// we'll always use char * static char* __yytext = (char*) yytext;
// NOTE: This will not work under flex(1) were yytext is not a fixed
// buffer, but a dynamically allocated one. This function should do
// the trick for us.
inline char *__yytext()
{
  return (char *) yytext;
}
#define ace_yytext __yytext()
#else
#define ace_yytext yytext
#endif /* 0 */

static int scan_obv_token (int token)
{
#ifdef IDL_HAS_VALUETYPE
    if (idl_global->obv_support ())
      {
        return token;
      }
#endif /* IDL_HAS_VALUETYPE */
    yylval.strval = ACE_OS::strdup (ace_yytext);
    return IDENTIFIER;
}

%}

/* SO we don't choke on files that use \r\n */
NL [\r?\n]

%array
%option never-interactive

%%

any             return IDL_ANY;
module		return IDL_MODULE;
raises		return IDL_RAISES;
readonly	return IDL_READONLY;
attribute	return IDL_ATTRIBUTE;
exception	return IDL_EXCEPTION;
context		return IDL_CONTEXT;
interface	return IDL_INTERFACE;
const		return IDL_CONST;
typedef		return IDL_TYPEDEF;
struct		return IDL_STRUCT;
enum		return IDL_ENUM;
string		return IDL_STRING;
wstring 	return IDL_WSTRING;
sequence	return IDL_SEQUENCE;
union		return IDL_UNION;
switch		return IDL_SWITCH;
case		return IDL_CASE;
default		return IDL_DEFAULT;
float		return IDL_FLOAT;
double		return IDL_DOUBLE;
long		return IDL_LONG;
short		return IDL_SHORT;
unsigned	return IDL_UNSIGNED;
char		return IDL_CHAR;
wchar		return IDL_WCHAR;
boolean		return IDL_BOOLEAN;
octet		return IDL_OCTET;
void		return IDL_VOID;
native          return IDL_NATIVE;
abstract        return scan_obv_token (IDL_ABSTRACT);
custom          return scan_obv_token (IDL_CUSTOM);
init            return scan_obv_token (IDL_INIT);
private         return scan_obv_token (IDL_PRIVATE);
public          return scan_obv_token (IDL_PUBLIC);
supports        return scan_obv_token (IDL_SUPPORTS);
truncatable     return scan_obv_token (IDL_TRUNCATABLE);
valuetype       return scan_obv_token (IDL_VALUETYPE);

TRUE		return IDL_TRUETOK;
FALSE		return IDL_FALSETOK;

inout		return IDL_INOUT;
in		return IDL_IN;
out		return IDL_OUT;
oneway		return IDL_ONEWAY;

\<\<		return IDL_LEFT_SHIFT;
\>\>		return IDL_RIGHT_SHIFT;
\:\:		{
		  yylval.strval = "::";
		  return IDL_SCOPE_DELIMITOR;
		}

[a-zA-Z][a-zA-Z0-9_]*	{
    /* make sure that this identifier is not a C++ keyword. If it is,
       prepend it with a _cxx_. Lookup in the perfect hash table for
       C++ keyword and grab the mapping*/

    TAO_IDL_CPP_Keyword_Table cpp_key_tbl;
    const TAO_IDL_CPP_Keyword_Entry *entry =
            cpp_key_tbl.lookup (ace_yytext,
                                ACE_OS::strlen (ace_yytext));
    if (entry)
      {
        yylval.strval = ACE_OS::strdup (entry->mapping_);
      }
    else
      {
        yylval.strval = ACE_OS::strdup (ace_yytext);
      }
    return IDENTIFIER;
}

-?[0-9]+"."[0-9]*([eE][+-]?[0-9]+)?[lLfF]?      {
                  yylval.dval = idl_atof(ace_yytext);
                  return IDL_FLOATING_PT_LITERAL;
                }
-?[0-9]+[eE][+-]?[0-9]+[lLfF]?  {
                  yylval.dval = idl_atof(ace_yytext);
                  return IDL_FLOATING_PT_LITERAL;
                }

-?[1-9][0-9]*	{
		  yylval.ival = idl_atoi(ace_yytext, 10);
		  return IDL_INTEGER_LITERAL;
	        }
-?0[xX][a-fA-F0-9]+ {
		  yylval.ival = idl_atoi(ace_yytext, 16);
		  return IDL_INTEGER_LITERAL;
	        }
-?0[0-7]*	{
		  yylval.ival = idl_atoi(ace_yytext, 8);
		  return IDL_INTEGER_LITERAL;
	      	}

"\""[^\"]*"\""	{
		  /* Skip the quotes */
		  char *tmp = ace_yytext;
		  tmp[strlen(tmp)-1] = '\0';
		  yylval.sval = new String(tmp + 1);
		  return IDL_STRING_LITERAL;
	      	}
"'"."'"		{
		  yylval.cval = ace_yytext [1];
		  return IDL_CHARACTER_LITERAL;
	      	}
"'"\\([0-7]{1,3})"'"	{
		  // octal character constant
		  yylval.cval = idl_escape_reader(ace_yytext + 1);
		  return IDL_CHARACTER_LITERAL;
		}
"'"\\[xX]([0-9a-fA-F]{1,2})"'"	{
		  // hexadecimal character constant
		  yylval.cval = idl_escape_reader(ace_yytext + 1);
		  return IDL_CHARACTER_LITERAL;
		}
"'"\\."'"	{
		  yylval.cval = idl_escape_reader(ace_yytext + 1);
		  return IDL_CHARACTER_LITERAL;
		}
^#[ \t]*pragma[ \t].*{NL}	{/* remember pragma */
  		  idl_global->set_lineno(idl_global->lineno() + 1);
		  idl_store_pragma(ace_yytext);
		}
^#[ \t]*[0-9]*" ""\""[^\"]*"\""" "[0-9]*{NL}		{
		  idl_parse_line_and_file(ace_yytext);
		}
^#[ \t]*[0-9]*" ""\""[^\"]*"\""{NL}		{
		  idl_parse_line_and_file(ace_yytext);
		}
^#line[ \t]*[0-9]*" ""\""[^\"]*"\""{NL}		{
		  idl_parse_line_and_file(ace_yytext);
		}
^#[ \t]*[0-9]*{NL} {
		  idl_parse_line_and_file(ace_yytext);
	        }
^#[ \t]*ident.*{NL}	{
		  /* ignore cpp ident */
  		  idl_global->set_lineno(idl_global->lineno() + 1);
		}
\/\/.*{NL}	{
		  /* ignore comments */
  		  idl_global->set_lineno(idl_global->lineno() + 1);
		}
"/*"		{
		  for(;;) {
		    char c = yyinput();
		    if (c == '*') {
		      char next = yyinput();
		      if (next == '/')
			break;
		      else
			yyunput(c, NULL);
	              if (c == '\n')
		        idl_global->set_lineno(idl_global->lineno() + 1);
		    }
	          }
	        }
[ \t]*		;
{NL}		{
  		  idl_global->set_lineno(idl_global->lineno() + 1);
		}
.		return ace_yytext [0];

%%
	/* subroutines */

/*
 * Strip down a name to the last component, i.e. everything after the last
 * '/' character
 */
static char *
stripped_name(UTL_String *fn)
{
    char	*n = fn->get_string();
    long	l;

    if (n == NULL)
	return NULL;
    l = strlen(n);
    for (n += l; l > 0 && *n != '/'; l--, n--);
    if (*n == '/') n++;
    return n;
}

/*
 * Parse a #line statement generated by the C preprocessor
 */
static void
idl_parse_line_and_file(char *buf)
{
  char		*r = buf;
  char 		*h;
  UTL_String	*nm;

  /* Skip initial '#' */
  if (*r != '#')
    return;
  else
    r++;

  /* Check to see if we're running under the screwy Microsoft scheme */
  /* of putting #line num instead of #num. */

  if (ACE_OS::strncmp (r, "line", 4) == 0)
    r += 5;

  /* Find line number */
  while (*r == ' ' || *r == '\t')
    r++;
  h = r;
  for (; *r != '\0' && *r != ' ' && *r != '\t'; r++)
    continue;
  *r++ = 0;
  idl_global->set_lineno(idl_atoi(h, 10));

  /* Find file name, if present */
  for (; *r != '"'; r++)
    if (*r == '\n' || *r == '\0')
      return;

  h = ++r;
  for (; *r != '"'; r++)
    continue;
  *r = 0;
  if (*h == '\0')
    idl_global->set_filename(new String("standard input"));
  else
    {
      long i;
      long j;

      // Put Microsoft-style pathnames into a canonical form.
      for (i = 0, j = 0; h[j] != '\0'; i++, j++)
        {
          if (h[j] == '\\' && h[j + 1] == '\\')
	    j++;

	  h[i] = h[j];
        }
      h[i] = '\0';
      idl_global->set_filename(new String(h));
    }
  int in_main_file = I_FALSE;
  if (idl_global->filename()->compare(idl_global->real_filename())
      || idl_global->filename()->compare(idl_global->main_filename()))
    in_main_file = I_TRUE;
  idl_global->set_in_main_file(in_main_file);
  /*
   * If it's an import file store the stripped name for the BE to use
   */
  if (!(idl_global->in_main_file()) && idl_global->import()) {
    nm = new UTL_String(stripped_name(idl_global->filename()));
    idl_global->store_include_file_name(nm);
  }
}

/*
 * Store a #pragma line into the list of pragmas
 */
static void
idl_store_pragma(char *buf)
{
  char *sp = buf + 1;
  // Remove all the blanks between the '#' and the 'pragma'
  if (*sp != 'p')
    {
      while (*sp != 'p')
        ++sp;
      char *tp = buf + 1;
      while (*sp != '\n')
        {
          *tp = *sp;
	  ++tp; ++sp;
        }
    }

  if (ACE_OS::strstr(buf + 8, "import") != 0) {
    idl_global->set_import(I_TRUE);
    return;
  }
  if (ACE_OS::strstr(buf + 8, "include") != 0) {
    idl_global->set_import(I_FALSE);
    return;
  }
  // Remove the final '\n'
  while (*sp != '\n')
    ++sp;
  *sp = '\0';
  UTL_StrList *p = idl_global->pragmas();
  if (p == NULL)
    idl_global->set_pragmas(new UTL_StrList(new String(buf), NULL));
  else {
    p->nconc(new UTL_StrList(new String(buf), NULL));
    idl_global->set_pragmas(p);
  }
}

/*
 * idl_atoi - Convert a string of digits into an integer according to base b
 */
static long
idl_atoi(char *s, long b)
{
	long	r = 0;
	long	negative = 0;

	if (*s == '-') {
	  negative = 1;
	  s++;
	}
	if (b == 8 && *s == '0')
	  s++;
	else if (b == 16 && *s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X'))
	  s += 2;

	for (; *s; s++)
	  if (*s <= '9' && *s >= '0')
	    r = (r * b) + (*s - '0');
	  else if (b > 10 && *s <= 'f' && *s >= 'a')
	    r = (r * b) + (*s - 'a' + 10);
	  else if (b > 10 && *s <= 'F' && *s >= 'A')
	    r = (r * b) + (*s - 'A' + 10);
	  else
	    break;

	if (negative)
	  r *= -1;

	return r;
}

/*
 * Convert a string to a float; atof doesn't seem to work, always.
 */
static double
idl_atof(char *s)
{
	char    *h = s;
	double	d = 0.0;
	double	f = 0.0;
	double	e, k;
	long	neg = 0, negexp = 0;

	ACE_UNUSED_ARG (f);
	ACE_UNUSED_ARG (h);

	if (*s == '-') {
	  neg = 1;
	  s++;
	}
	while (*s >= '0' && *s <= '9') {
		d = (d * 10) + *s - '0';
		s++;
	}
	if (*s == '.') {
		s++;
		e = 10;
		while (*s >= '0' && *s <= '9') {
			d += (*s - '0') / (e * 1.0);
			e *= 10;
			s++;
		}
	}
	if (*s == 'e' || *s == 'E') {
		s++;
		if (*s == '-') {
			negexp = 1;
			s++;
		} else if (*s == '+')
			s++;
		e = 0;
		while (*s >= '0' && *s <= '9') {
			e = (e * 10) + *s - '0';
			s++;
		}
		if (e > 0) {
			for (k = 1; e > 0; k *= 10, e--);
			if (negexp)
				d /= k;
			else
				d *= k;
		}
	}

	if (neg) d *= -1.0;

	return d;
}

/*
 * Convert (some) escaped characters into their ascii values
 */
static char
idl_escape_reader(
    char *str
)
{
    if (str[0] != '\\') {
	return str[0];
    }

    switch (str[1]) {
      case 'n':
	return '\n';
      case 't':
	return '\t';
      case 'v':
	return '\v';
      case 'b':
	return '\b';
      case 'r':
	return '\r';
      case 'f':
	return '\f';
      case 'a':
	return '\a';
      case '\\':
	return '\\';
      case '\?':
	return '?';
      case '\'':
	return '\'';
      case '"':
	return '"';
      case 'x':
	{
	    int i;
	    // hex value
	    for (i = 2; str[i] != '\0' && isxdigit(str[i]); i++) {
		continue;
	    }
	    char save = str[i];
	    str[i] = '\0';
	    char out = (char)idl_atoi(&str[2], 16);
	    str[i] = save;
	    return out;
	}
	ACE_NOTREACHED (break;)
      default:
	// check for octal value
	if (str[1] >= '0' && str[1] <= '7') {
 	    int i;
	    for (i = 1; str[i] >= '0' && str[i] <= '7'; i++) {
		continue;
	    }
	    char save = str[i];
	    str[i] = '\0';
	    char out = (char)idl_atoi(&str[1], 8);
	    str[i] = save;
	    return out;
	} else {
	  return str[1] - 'a';
	}
	ACE_NOTREACHED  (break;)
    }
}
