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

#include "utl_strlist.h"
#include "utl_exprlist.h"
#include "utl_labellist.h"
#include "utl_decllist.h"
#include "utl_string.h"
#include "utl_err.h"
#include "ast_union_label.h"
#include "ast_field.h"
#include "ast_expression.h"
#include "ast_argument.h"
#include "ast_operation.h"
#include "fe_interface_header.h"
#include "global_extern.h"
#include "fe_private.h"
#include "y.tab.h"

static char *           idl_wstring_escape_reader (char *);
static ACE_CDR::WChar   idl_wchar_escape_reader (char *);
static char             idl_escape_reader (char *);
static double           idl_atof (char *);
static long             idl_atoi (char *, long);
static unsigned long	idl_atoui (char *, long);
static void		idl_parse_line_and_file (char *);
static void		idl_store_pragma (char *);
static char *           idl_get_pragma_string (char *);
static idl_bool         idl_valid_version (char *);
static AST_Decl *       idl_find_node (char *);

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
    if (idl_global->obv_support ())
      {
        return token;
      }

    TAO_IDL_CPP_Keyword_Table cpp_key_tbl;
    const TAO_IDL_CPP_Keyword_Entry *entry =
      cpp_key_tbl.lookup (ace_yytext,
                          ACE_OS::strlen (ace_yytext));
    if (entry)
      yylval.strval = ACE_OS::strdup (entry->mapping_);
    else
      yylval.strval = ACE_OS::strdup (ace_yytext);
    return IDENTIFIER;
}

static int scan_ccm_token (int token)
{
  if (idl_global->component_support ())
    {
      return token;
    }
  else
    {
      yylval.strval = ACE_OS::strdup (ace_yytext);
      return IDENTIFIER;
    }
}

%}

/* SO we don't choke on files that use \r\n */
NL [\r?\n]

%array
%option never-interactive

%%

any             return IDL_ANY;
Object          return IDL_OBJECT;
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
fixed		return IDL_FIXED;
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
local           return IDL_LOCAL;
abstract        return scan_obv_token (IDL_ABSTRACT);
custom          return scan_obv_token (IDL_CUSTOM);
factory         return scan_obv_token (IDL_FACTORY);
private         return scan_obv_token (IDL_PRIVATE);
public          return scan_obv_token (IDL_PUBLIC);
supports        return scan_obv_token (IDL_SUPPORTS);
truncatable     return scan_obv_token (IDL_TRUNCATABLE);
valuetype       return scan_obv_token (IDL_VALUETYPE);
component       return scan_ccm_token (IDL_COMPONENT);
consumes        return scan_ccm_token (IDL_CONSUMES);
emits           return scan_ccm_token (IDL_EMITS);
eventtype       return scan_ccm_token (IDL_EVENTTYPE);
finder          return scan_ccm_token (IDL_FINDER);
getraises       return scan_ccm_token (IDL_GETRAISES);
home            return scan_ccm_token (IDL_HOME);
import          return scan_ccm_token (IDL_IMPORT);
multiple        return scan_ccm_token (IDL_MULTIPLE);
primarykey      return scan_ccm_token (IDL_PRIMARYKEY);
provides        return scan_ccm_token (IDL_PROVIDES);
publishes       return scan_ccm_token (IDL_PUBLISHES);
setraises       return scan_ccm_token (IDL_SETRAISES);
typeid          return scan_ccm_token (IDL_TYPEID);
typeprefix      return scan_ccm_token (IDL_TYPEPREFIX);
uses            return scan_ccm_token (IDL_USES);
manages         return scan_ccm_token (IDL_MANAGES);

TRUE		return IDL_TRUETOK;
FALSE		return IDL_FALSETOK;

inout		return IDL_INOUT;
in		return IDL_IN;
out		return IDL_OUT;
oneway		return IDL_ONEWAY;

\<\<		return IDL_LEFT_SHIFT;
\>\>		return IDL_RIGHT_SHIFT;
\:\:		{
		  yylval.strval = (char *) "::";
		  return IDL_SCOPE_DELIMITOR;
		}

[a-ij-rs-zA-IJ-RS-Z_][a-ij-rs-zA-IJ-RS-Z0-9_]*	{
  // Make sure that this identifier is not a C++ keyword. If it is,
  // prepend it with a _cxx_. Lookup in the perfect hash table for C++
  // keyword and grab the mapping.  BTW, the reason for the odd
  // regular expression is to handle EBCDIC, as well as ASCII.

  TAO_IDL_CPP_Keyword_Table cpp_key_tbl;
  const TAO_IDL_CPP_Keyword_Entry *entry =
    cpp_key_tbl.lookup (ace_yytext,
                        ACE_OS::strlen (ace_yytext));
  if (entry)
    yylval.strval = ACE_OS::strdup (entry->mapping_);
  else
    yylval.strval = ACE_OS::strdup (ace_yytext);

  return IDENTIFIER;
}

"-"?(([0-9]+"."[0-9]*)|("."[0-9]+))([eE][+-]?[0-9]+)?[lLfF]?      {
                  yylval.dval = idl_atof(ace_yytext);
                  return IDL_FLOATING_PT_LITERAL;
                }
"-"?[0-9]+[eE][+-]?[0-9]+[lLfF]?  {
                  yylval.dval = idl_atof(ace_yytext);
                  return IDL_FLOATING_PT_LITERAL;
                }

"-"[1-9][0-9]*	{
		  yylval.ival = idl_atoi(ace_yytext, 10);
		  return IDL_INTEGER_LITERAL;
	        }
[1-9][0-9]*     {
		  yylval.uival = idl_atoui(ace_yytext, 10);
		  return IDL_UINTEGER_LITERAL;
		}
"-"0[xX][a-fA-F0-9]+ {
		  yylval.ival = idl_atoi(ace_yytext, 16);
		  return IDL_INTEGER_LITERAL;
	        }
0[xX][a-fA-F0-9]+    {
                  yylval.uival = idl_atoui(ace_yytext, 16);
		  return IDL_UINTEGER_LITERAL;
		}
"-"0[0-7]*	{
		  yylval.ival = idl_atoi(ace_yytext, 8);
		  return IDL_INTEGER_LITERAL;
	      	}
0[0-7]*	        {
		  yylval.uival = idl_atoui(ace_yytext, 8);
		  return IDL_UINTEGER_LITERAL;
	      	}

(\"([^\\\"]*|\\[ntvbrfax\\\?\'\"])*\"[ \t]*)+	{
		  /* Skip the quotes */
		  char *tmp = ace_yytext;
		  tmp[strlen(tmp)-1] = '\0';
		  yylval.sval = new UTL_String(tmp + 1);
		  return IDL_STRING_LITERAL;
	      	}
L\"([^\\\"]*|\\u([0-9a-fA-F]{1,4}))*\"	{
		  /* Skip the bookends */
		  char *tmp = ACE_OS::strdup (ace_yytext);
		  tmp[strlen (tmp) - 1] = '\0';
		  yylval.wsval = idl_wstring_escape_reader(tmp + 2);
		  return IDL_WSTRING_LITERAL;
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
L"'"."'"	{
		  // wide character constant
		  yylval.wcval = ace_yytext [2];
		  return IDL_WCHAR_LITERAL;
		}
L"'"\\u([0-9a-fA-F]{1,4})"'"	{
		  // hexadecimal wide character constant
		  yylval.wcval = idl_wchar_escape_reader(ace_yytext + 2);
		  return IDL_WCHAR_LITERAL;
		}
^#[ \t]*pragma[ \t].*{NL}	|
^\?\?=[ \t]*pragma[ \t].*{NL}	{/* remember pragma */
  		  idl_global->set_lineno(idl_global->lineno() + 1);
		  idl_store_pragma(ace_yytext);
		}
^#[ \t]*file[ \t].*{NL}	|
^\?\?=[ \t]*file[ \t].*{NL}	{/* ignore file */
  		  idl_global->set_lineno(idl_global->lineno() + 1);
		}
^#[ \t]*[0-9]*" ""\""[^\"]*"\""" "[0-9]*([ \t]*[0-9]*)?{NL}		|
^\?\?=[ \t]*[0-9]*" ""\""[^\"]*"\""" "[0-9]*([ \t]*[0-9]*)?{NL}		{
		  idl_parse_line_and_file(ace_yytext);
		}
^#[ \t]*[0-9]*" ""\""[^\"]*"\""{NL}		|
^\?\?=[ \t]*[0-9]*" ""\""[^\"]*"\""{NL}		{
		  idl_parse_line_and_file(ace_yytext);
		}
^#line[ \t]*[0-9]+[ \t]*("\""[^\"]*"\"")?{NL}		|
^\?\?=line[ \t]*[0-9]*" ""\""[^\"]*"\""{NL}		{
		  idl_parse_line_and_file(ace_yytext);
		}
^#[ \t]*[0-9]*{NL} |
^\?\?=[ \t]*[0-9]*{NL} {
		  idl_parse_line_and_file(ace_yytext);
	        }
^#[ \t]*ident[ \t].*{NL}	|
^\?\?=[ \t]*ident[ \t].*{NL}	{
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

// Parse a #line statement generated by the C preprocessor
static void
idl_parse_line_and_file (char *buf)
{
  char *r = buf;
  char *h = 0;
  UTL_String *nm = 0;

  // Skip initial '#'.
  if (*r != '#')
    {
      return;
    }
  else
    {
      r++;
    }

  // Check to see if we're running under the screwy Microsoft scheme
  // of putting #line num instead of #num.

  if (ACE_OS::strncmp (r, "line", 4) == 0)
    {
      r += 5;
    }

  // Find line number.
  while (isspace (*r))
    {
      r++;
    }

  h = r;

  for (; isdigit (*r); r++)
    {
      continue;
    }

  *r++ = 0;
  idl_global->set_lineno (idl_atoui (h, 10));

  // Find file name, if present.
  for (; *r != '"'; r++)
    {
      if (*r == '\n' || *r == '\0')
        {
          return;
        }
    }

  h = ++r;

  for (; *r != '"'; r++)
    {
      continue;
    }

  *r = 0;
  UTL_String *tmp = 0;

  if (*h == '\0')
    {
      ACE_NEW (tmp,
               UTL_String ("standard input"));
      idl_global->set_filename (tmp);
      return;
    }
  else
    {
      long i;
      long j;

      // Put Microsoft-style pathnames into a canonical form.
      for (i = 0, j = 0; h[j] != '\0'; i++, j++)
        {
          if (h[j] == '\\' && h[j + 1] == '\\')
            {
	            j++;
            }

	        h[i] = h[j];
        }

      h[i] = '\0';
      ACE_NEW (tmp,
               UTL_String (h));
      idl_global->set_filename (tmp);
    }

  UTL_String *fname = idl_global->filename ();
  idl_bool in_main_file = I_FALSE;
  idl_bool is_real_filename = fname->compare (idl_global->real_filename ());
  idl_bool is_main_filename = I_FALSE;

  if (!is_real_filename)
    {
      is_main_filename = fname->compare (idl_global->main_filename ());
    }

  if (is_real_filename || is_main_filename)
    {
      in_main_file = I_TRUE;
    }

  idl_global->set_in_main_file (in_main_file);

  // If it's an import file store the stripped name for the BE to use
  if (!(idl_global->in_main_file ()) && idl_global->import ()) 
    {
      ACE_NEW (nm,
               UTL_String (stripped_name (fname)));

      // This call also manages the #pragma prefix.
      idl_global->store_include_file_name (nm);
    }
  else if (is_main_filename 
           && idl_global->pragma_prefixes ().size () > 1
           && idl_global->scopes ().depth () == 1)
    {
      // If we're here, we have come to the end of an included file, so we
      // pop its prefix.
      char *trash = 0;
      idl_global->pragma_prefixes ().pop (trash);
      delete [] trash;
    }
}

// Store a #pragma line into the list of pragmas
static void
idl_store_pragma (char *buf)
{
  char *sp = buf + 1;
  int crunched = 0;

  // Remove all the blanks between the '#' and the 'pragma'.
  if (*sp != 'p')
    {
      while (*sp != 'p')
        {
          ++sp;
          ++crunched;
        }

      char *tp = buf + 1;

      while (*sp != '\n')
        {
          *tp = *sp;
          ++tp;
          ++sp;
        }
    }

  // Remove the final '\n'.
  while (*sp != '\n')
    {
      ++sp;
    }

  // The '\n' is still <crunched> spaces too far away, with
  // garbage characters in between.
  sp -= crunched;
  *sp = '\0';

  if (ACE_OS::strstr (buf + 8, "import") != 0)
    {
      idl_global->set_import (I_TRUE);
      return;
    }

  if (ACE_OS::strstr (buf + 8, "include") != 0)
    {
      idl_global->set_import (I_FALSE);
      return;
    }

  if (ACE_OS::strncmp (buf + 8, "prefix", 6) == 0)
    {
      char *new_prefix = idl_get_pragma_string (buf);

      if (new_prefix != 0)
        {
          unsigned long depth = idl_global->scopes ().depth ();

          // At global scope, we always replace the prefix. For all
          // other scopes, we replace only if there is a prefix already
          // associated with that scope, otherwise we add the prefix.
          if (depth == 1 || idl_global->scopes ().top ()->has_prefix ())
            {
              char *trash = 0;
              idl_global->pragma_prefixes ().pop (trash);
              delete [] trash;
            }

          if (depth > 1)
            {
              idl_global->scopes ().top ()->has_prefix (1);
            }

          idl_global->pragma_prefixes ().push (new_prefix);
        }
    }
  else if (ACE_OS::strncmp (buf + 8, "version", 7) == 0)
    {
      char *tmp = buf + 16;

      while (*tmp == ' ')
        {
          ++tmp;
        }

      char *number = ACE_OS::strchr (tmp, ' ');

      while (*number == ' ')
        {
          ++number;
        }

      AST_Decl *d = idl_find_node (tmp);

      if (d == 0)
        {
          return;
        }

      if (!idl_valid_version (number))
        {
          idl_global->err ()->version_number_error (number);
          return;
        }

      d->version (ACE::strnew (number));
    }
  else if (ACE_OS::strncmp (buf + 8, "ident", 5) == 0) 
    {
      idl_global->ident_string (buf + 8);
    }
  else if (ACE_OS::strncmp (buf + 8, "ID", 2) == 0)
    {
      char *tmp = buf + 11;

      while (*tmp == ' ')
        {
          ++tmp;
        }

      AST_Decl *d = idl_find_node (tmp);

      if (d == 0)
        {
          return;
        }

      char *new_id = idl_get_pragma_string (buf);

      if (new_id != 0)
        {
          d->repoID (new_id);
          d->typeid_set (1);
        }
    }
}

/*
 * idl_atoi - Convert a string of digits into a negative integer according to base b
 */
static long
idl_atoi(char *s, long b)
{
	long	r = 0;
	s++;

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

	return -r;
}

/*
 * idl_atoui - Convert a string of digits into an unsigned integer according to base b
 */
static unsigned long
idl_atoui(char *s, long b)
{
	long	r = 0;

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
	    char out = (char)idl_atoui(&str[2], 16);
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
	    char out = (char)idl_atoui(&str[1], 8);
	    str[i] = save;
	    return out;
	} else {
	  return str[1] - 'a';
	}
	ACE_NOTREACHED  (break;)
    }
}
/*
 * Convert escaped hex digits into a wchar
 */
static ACE_CDR::WChar
idl_wchar_escape_reader (char *str)
{
  if (str[0] != '\\' || str[1] != 'u')
    {
      return 0;
    }

  int i;
  // get the hex digits
  for (i = 2; str[i] != '\0' && isxdigit (str[i]); i++)
    {
      continue;
    }
  char save = str[i];
  str[i] = '\0';
  ACE_CDR::WChar out = (ACE_CDR::WChar) idl_atoui (&str[2], 16);
  str[i] = save;
  return out;
}

/*
 * Checks wstring for validity
 */
static char *
idl_wstring_escape_reader (char *str)
{
  return str;
}

static char *
idl_get_pragma_string (char *pragma)
{
  // Get pointers to each end of the substring between the quotes.
  const char *start = ACE_OS::strchr (pragma, '"') + 1;
  const char *end = ACE_OS::strchr (start, '"');

  if (end == 0)
    {
      idl_global->err ()->syntax_error (
          IDL_GlobalData::PS_PragmaPrefixSyntax
        );

      return 0;
    }

  int len = end - start;
  char *retval = 0;

  ACE_NEW_RETURN (retval,
                  char[len + 1],
                  0);

  ACE_OS::strncpy (retval,
                   start,
                   len);

  retval[len] = '\0';
  return retval;
}

static idl_bool
idl_valid_version (char *s)
{
  // Nothing preceding decimal point.
  if (*s == '.')
    {
      return 0;
    }

  char *minor = ACE_OS::strchr (s, '.');
  int i;

  if (minor == 0)
    {
      // No decimal point.
      return 0;
    }

  if (*(minor + 1) == '\0')
    {
      // Nothing following decimal point.
      return 0;
    }

  char *tmp = minor + 1;

  for (i = 0; tmp[i] != '\0'; ++i)
    {
      if (!isdigit (tmp[i]))
        {
          return 0;
        }
    }

  int len = minor - s;

  for (i = 0; i < len; ++i)
    {
      if (!isdigit (s[i]))
        {
          return 0;
        }
    }

  // Major and minor version numbers must be unsigned shorts.
  if (ACE_OS::atoi (minor + 1) > ACE_UINT16_MAX
      || ACE_OS::atoi (s) > ACE_UINT16_MAX)
    {
      return 0;
    }

  return 1;
}

static AST_Decl *
idl_find_node (char *s)
{
  UTL_ScopedName *node = idl_global->string_to_scoped_name (s);
  AST_Decl *d = 0;

  if (node != 0)
    {
      d = idl_global->scopes ().top_non_null ()->lookup_by_name (node,
                                                                  I_TRUE);
    }

  if (d == 0)
    {
      idl_global->err ()->lookup_error (node);
      node->destroy ();
      delete node;
      node = 0;
    }

  return d;
}
