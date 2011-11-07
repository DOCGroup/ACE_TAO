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
 * idl.ll - Lexical scanner for IDL 3.1
 */

#include "global_extern.h"
#include "nr_extern.h"
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
#include "ast_root.h"
#include "fe_interface_header.h"
#include "fe_obv_header.h"
#include "fe_component_header.h"
#include "fe_home_header.h"
#include "fe_private.h"
#include "fe_extern.h"
#include "y.tab.h"

static char *               idl_wstring_escape_reader (char *);
static ACE_CDR::WChar       idl_wchar_escape_reader (char *);
static ACE_CDR::Char        idl_escape_reader (char *);
static ACE_CDR::Double      idl_atof (char *);
static ACE_CDR::LongLong    idl_atoi (char *, long);
static ACE_CDR::ULongLong   idl_atoui (char *, long);
static void                 idl_parse_line_and_file (char *);
static void                 idl_store_pragma (char *);
static char *               idl_get_pragma_string (char *);
static bool                 idl_valid_version (char *);
static AST_Decl *           idl_find_node (char *);

#define ace_yytext yytext

#ifdef USE_MCPP_BUFFER_LEXING
#include "mcpp_yyinput.h"
#endif /*USE_MCPP_BUFFER_LEXING*/

%}

/* SO we don't choke on files that use \r\n */
NL [\r?\n]

%array
%option never-interactive

%%

any             return IDL_ANY;
Object          return IDL_OBJECT;
module          return IDL_MODULE;
raises          return IDL_RAISES;
readonly        return IDL_READONLY;
attribute       return IDL_ATTRIBUTE;
exception       return IDL_EXCEPTION;
context         return IDL_CONTEXT;
interface       return IDL_INTERFACE;
const           return IDL_CONST;
typedef         return IDL_TYPEDEF;
struct          return IDL_STRUCT;
enum            return IDL_ENUM;
string          return IDL_STRING;
wstring         return IDL_WSTRING;
sequence        return IDL_SEQUENCE;
union           return IDL_UNION;
fixed           return IDL_FIXED;
switch          return IDL_SWITCH;
case            return IDL_CASE;
default         return IDL_DEFAULT;
float           return IDL_FLOAT;
double          return IDL_DOUBLE;
long            return IDL_LONG;
short           return IDL_SHORT;
unsigned        return IDL_UNSIGNED;
char            return IDL_CHAR;
wchar           return IDL_WCHAR;
boolean         return IDL_BOOLEAN;
octet           return IDL_OCTET;
void            return IDL_VOID;
native          return IDL_NATIVE;
local           return IDL_LOCAL;
abstract        return IDL_ABSTRACT;

custom          return IDL_CUSTOM;
factory         return IDL_FACTORY;
private         return IDL_PRIVATE;
public          return IDL_PUBLIC;
supports        return IDL_SUPPORTS;
truncatable     return IDL_TRUNCATABLE;
valuetype       return IDL_VALUETYPE;

component       return IDL_COMPONENT;
consumes        return IDL_CONSUMES;
emits           return IDL_EMITS;
eventtype       return IDL_EVENTTYPE;
finder          return IDL_FINDER;
getraises       return IDL_GETRAISES;
home            return IDL_HOME;
import          return IDL_IMPORT;
multiple        return IDL_MULTIPLE;
primarykey      return IDL_PRIMARYKEY;
provides        return IDL_PROVIDES;
publishes       return IDL_PUBLISHES;
setraises       return IDL_SETRAISES;
typeid          return IDL_TYPEID;
typeprefix      return IDL_TYPEPREFIX;
uses            return IDL_USES;
manages         return IDL_MANAGES;

typename        return IDL_TYPENAME;
port            return IDL_PORT;
mirrorport      return IDL_MIRRORPORT;
porttype        return IDL_PORTTYPE;
connector       return IDL_CONNECTOR;
alias           return IDL_ALIAS;

TRUE            return IDL_TRUETOK;
FALSE           return IDL_FALSETOK;

inout           return IDL_INOUT;
in              return IDL_IN;
out             return IDL_OUT;
oneway          return IDL_ONEWAY;

\<\<            return IDL_LEFT_SHIFT;
\>\>            return IDL_RIGHT_SHIFT;
\:\:            {
                  yylval.strval = ACE::strnew ("::");
                  return IDL_SCOPE_DELIMITOR;
                }

[a-ij-rs-zA-IJ-RS-Z_][a-ij-rs-zA-IJ-RS-Z0-9_]* {
  // Make sure that this identifier is not a C++ keyword. If it is,
  // prepend it with a _cxx_. Lookup in the perfect hash table for C++
  // keyword and grab the mapping.  BTW, the reason for the odd
  // regular expression is to handle EBCDIC, as well as ASCII.

  TAO_IDL_CPP_Keyword_Table cpp_key_tbl;
  const TAO_IDL_CPP_Keyword_Entry *entry = 0;
  if (!idl_global->preserve_cpp_keywords())
    {
      // This check will ensure that escaped C++ keywords will be
      // caught and prepended with '_cxx' as non-escaped keywords
      // are now prepended with '_cxx_'.
      const char *tmp =
        ace_yytext[0] == '_' ? ace_yytext + 1 : ace_yytext;

      entry =
        cpp_key_tbl.lookup (tmp,
                            static_cast<unsigned int> (ACE_OS::strlen (tmp)));
    }

  if (entry)
    {
      yylval.strval = ACE::strnew (entry->mapping_);
    }
  else
    {
      yylval.strval = ACE::strnew (ace_yytext);
    }

  return IDENTIFIER;
}

"-"?(([0-9]+"."[0-9]*)|("."[0-9]+))([eE][+-]?[0-9]+)?[lLfF]?      {
                  yylval.dval = idl_atof (ace_yytext);
                  return IDL_FLOATING_PT_LITERAL;
                }
"-"?[0-9]+[eE][+-]?[0-9]+[lLfF]?  {
                  yylval.dval = idl_atof (ace_yytext);
                  return IDL_FLOATING_PT_LITERAL;
                }

"-"[1-9][0-9]*  {
                  yylval.ival = idl_atoi (ace_yytext, 10);
                  return IDL_INTEGER_LITERAL;
                }
[1-9][0-9]*     {
                  yylval.uival = idl_atoui (ace_yytext, 10);
                  return IDL_UINTEGER_LITERAL;
                }
"-"0[xX][a-fA-F0-9]+ {
                  yylval.ival = idl_atoi (ace_yytext, 16);
                  return IDL_INTEGER_LITERAL;
                }
0[xX][a-fA-F0-9]+ {
                  yylval.uival = idl_atoui (ace_yytext, 16);
                  return IDL_UINTEGER_LITERAL;
                }
"-"0[0-7]*      {
                  yylval.ival = idl_atoi (ace_yytext, 8);
                  return IDL_INTEGER_LITERAL;
                }
0[0-7]*         {
                  yylval.uival = idl_atoui (ace_yytext, 8);
                  return IDL_UINTEGER_LITERAL;
                }

(\"([^\\\"]*|\\[ntvbrfax\\\?\'\"]|\\[0-7]{1,3})*\"[ \t]*)+ {
                  /* Skip the quotes */
                  char * const tmp = ace_yytext;
                  for(size_t i = ACE_OS::strlen (tmp); i-- != 0; ) {
                    if (isspace(tmp[i])) {
                      tmp[i] = '\0';
                    }
                    else {
                      break;
                    }
                  }
                  tmp[ACE_OS::strlen (tmp) - 1] = '\0';
                  ACE_NEW_RETURN (yylval.sval,
                                  UTL_String (tmp + 1, true),
                                  IDL_STRING_LITERAL);
                  return IDL_STRING_LITERAL;
                }
(L\"([^\\\"]*|\\[ntvbrfax\\\?\'\"]|\\[0-7]{1,3}|\\u([0-9a-fA-F]{1,4}))*\"[ \t]*)+ {
                  /* Skip the bookends */
                  char * const tmp = ACE_OS::strdup (ace_yytext);
                  for(size_t i = ACE_OS::strlen (tmp); i-- != 0; ) {
                    if (isspace(tmp[i])) {
                      tmp[i] = '\0';
                    }
                    else {
                      break;
                    }
                  }
                  tmp[ACE_OS::strlen (tmp) - 1] = '\0';
                  yylval.wsval = idl_wstring_escape_reader(tmp + 2);
                  return IDL_WSTRING_LITERAL;
                }
"'"."'"         {
                  yylval.cval = ace_yytext [1];
                  return IDL_CHARACTER_LITERAL;
                }
"'"\\([0-7]{1,3})"'" {
                  // octal character constant
                  yylval.cval = idl_escape_reader (ace_yytext + 1);
                  return IDL_CHARACTER_LITERAL;
                }
"'"\\[xX]([0-9a-fA-F]{1,2})"'" {
                  // hexadecimal character constant
                  yylval.cval = idl_escape_reader (ace_yytext + 1);
                  return IDL_CHARACTER_LITERAL;
                }
"'"\\."'"       {
                  yylval.cval = idl_escape_reader (ace_yytext + 1);
                  return IDL_CHARACTER_LITERAL;
                }
L"'"."'"        {
                  // wide character constant
                  yylval.wcval = ace_yytext [2];
                  return IDL_WCHAR_LITERAL;
                }
L"'"\\u([0-9a-fA-F]{1,4})"'" {
                  // hexadecimal wide character constant
                  yylval.wcval = idl_wchar_escape_reader(ace_yytext + 2);
                  return IDL_WCHAR_LITERAL;
                }
^[ \t]*#[ \t]*pragma[ \t].*{NL} |
^\?\?=[ \t]*pragma[ \t].*{NL} {/* remember pragma */
                  idl_global->set_lineno(idl_global->lineno() + 1);
                  idl_store_pragma(ace_yytext);
                }
^[ \t]*#file[ \t].*{NL} |
^\?\?=[ \t]*file[ \t].*{NL} {/* ignore file */
                  idl_global->set_lineno(idl_global->lineno() + 1);
                }
^[ \t]*#[ \t]*[0-9]*" ""\""[^\"]*"\""" "[0-9]*([ \t]*[0-9]*)*{NL} |
^\?\?=[ \t]*[0-9]*" ""\""[^\"]*"\""" "[0-9]*([ \t]*[0-9]*)?{NL} {
                  idl_parse_line_and_file(ace_yytext);
                }
^[ \t]*#[ \t]*[0-9]*" ""\""[^\"]*"\""{NL} |
^\?\?=[ \t]*[0-9]*" ""\""[^\"]*"\""{NL} {
                  idl_parse_line_and_file(ace_yytext);
                }
^[ \t]*#line[ \t]*[0-9]+[ \t]*("\""[^\"]*"\""([ \t]*[0-9]*([ \t]*[0-9]*)?)?)?{NL} |
^\?\?=line[ \t]*[0-9]*" ""\""[^\"]*"\""{NL} {
                  idl_parse_line_and_file(ace_yytext);
                }
^[ \t]*#[ \t]*[0-9]*{NL} |
^\?\?=[ \t]*[0-9]*{NL} {
                  idl_parse_line_and_file(ace_yytext);
                }
^[ \t]*#[ \t]*ident[ \t].*{NL} |
^\?\?=[ \t]*ident[ \t].*{NL} {
                  /* ignore cpp ident */
                  idl_global->set_lineno(idl_global->lineno() + 1);
                }
\/\/.*{NL}      {
                  /* ignore comments */
                  idl_global->set_lineno(idl_global->lineno() + 1);
                }
"/*"            {
                  for(;;) {
                    char const c = yyinput();
                    if (c == '*') {
                      char const next = yyinput();
                      if (next == '/')
                        break;
                      else
                        yyunput(c, NULL);
                    }
                    else if (c == '\n') {
                      idl_global->set_lineno(idl_global->lineno() + 1);
                    }
                  }
                }
[ \t]*          ;
{NL}            {
                  idl_global->set_lineno(idl_global->lineno() + 1);
                }
.               return ace_yytext [0];

%%
        /* subroutines */

static bool
same_file (char *path1, char *path2)
{
  char fullpath1 [MAXPATHLEN + 1] = {0};
  char fullpath2 [MAXPATHLEN + 1] = {0};

  char * fp1 = ACE_OS::realpath(path1, fullpath1);
  char * fp2 = ACE_OS::realpath(path2, fullpath2);

  return
    (fp1 == 0 || fp2 == 0)
      ? false
      : FE_Utils::path_cmp (fullpath1,fullpath2) == 0;
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
  idl_global->set_lineno ((long) idl_atoui (h, 10));

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
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("IDL: No input files\n")));

      throw Bailout ();
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
#if defined (ACE_OPENVMS)
      // translate this into *nix format as the OpenVMS preprocessor
      // possibly produced VMS-style paths here.
      char trans_path[MAXPATHLEN] = "";
      char *temp_h = IDL_GlobalData::translateName (h, trans_path);
      if (temp_h) h = temp_h;
#endif
      ACE_NEW (tmp,
               UTL_String (h, true));
      idl_global->update_prefix (tmp->get_string ());
      idl_global->set_filename (tmp);
    }

  UTL_String *fname = idl_global->filename ();
  bool in_main_file = false;

  bool is_real_filename =
    fname->compare (idl_global->real_filename ())
    || same_file (fname->get_string(),
                  idl_global->real_filename ()->get_string());

  bool is_main_filename = false;

  if (!is_real_filename)
    {
#if defined (ACE_OPENVMS)
      char full_path[MAXPATHLEN] = "";
      char *full_fname = ACE_OS::realpath (fname->get_string (), full_path);
      // I don't see the benefit of using ->compare since this is targeted at IDL identifiers
      // not at filenames and in the case of OpenVMS (case-insensitive filesystem) gets really
      // problematic as filenames retrieved through different mechanisms may give different
      // casing.
      is_main_filename = FE_Utils::path_cmp (idl_global->main_filename ()->get_string (),
                                             full_fname) == 0;
#else
      is_main_filename =
        fname->compare (idl_global->main_filename ())
        || same_file (fname->get_string(),
                      idl_global->main_filename ()->get_string());
#endif
    }

  if (is_real_filename || is_main_filename)
    {
      in_main_file = true;
    }

  idl_global->set_in_main_file (in_main_file);

  // Strip off any command line -I prefix that may have been added
  // by the preprocessor.
  if (!(idl_global->in_main_file ()) && idl_global->import ())
    {
      ACE_NEW (nm,
               UTL_String (
                 FE_Utils::stripped_preproc_include (
                   fname->get_string ()), true));

      // This call also manages the #pragma prefix.
      idl_global->store_include_file_name (nm);
    }
}

// Store a #pragma line into the list of pragmas
static void
idl_store_pragma (char *buf)
{
  char *sp = buf + 1;
  int crunched = 0;

  // Remove all the blanks between the '#' and the 'pragma'.
  while (*sp != 'p')
    {
      ++sp;
      ++crunched;
    }

  char *tp = buf + 1;

  // This copies the crunched string back to the original, and
  // also compensates for the behavior of the Sun preprocessor,
  // which put spaces around the double colons of a non-quoted
  // scoped name, a case which is possible in #pragma version.
  // Also eats whitespace, which some preprocessors (for example
  // Intel) don't automatically do.
  while (*sp != '\n')
    {
      if (*sp == ' ' && (*(sp + 1) == ':' || *(sp + 1) == ' '))
        {
          ++crunched;
        }
      else if (*sp == ':' && *(sp + 1) == ' ')
        {
          *tp = *sp;
          ++crunched;
          ++sp;
          ++tp;
        }
      else
        {
          *tp = *sp;
          ++tp;
        }

      ++sp;
    }

  // The '\n' is still <crunched> spaces too far away, with
  // garbage characters in between.
  sp -= crunched;
  *sp = '\0';

  if (ACE_OS::strstr (buf + 8, "import") != 0)
    {
      idl_global->set_import (true);
      return;
    }

  if (ACE_OS::strstr (buf + 8, "include") != 0)
    {
      idl_global->set_import (false);
      return;
    }

  if (ACE_OS::strncmp (buf + 8, "prefix", 6) == 0)
    {
      char *new_prefix = idl_get_pragma_string (buf);

      if (new_prefix != 0)
        {
          unsigned long depth = idl_global->scopes ().depth ();

          // We replace the prefix only if there is a prefix already
          // associated with this file, otherwise we add the prefix.
          char *ext_id = idl_global->filename ()->get_string ();
          ACE_Hash_Map_Entry<char *, char *> *entry = 0;
          int const status =
            idl_global->file_prefixes ().find (ext_id, entry);

          if (status == 0)
            {
              if (ACE_OS::strcmp (entry->int_id_, "") != 0)
                {
                  char *trash = 0;
                  idl_global->pragma_prefixes ().pop (trash);
                  delete [] trash;
                }
              else if (depth == 1)
                {
                  // Remove the default "" and bind the new prefix.
                  ACE::strdelete (entry->ext_id_);
                  ACE::strdelete (entry->int_id_);
                  (void) idl_global->file_prefixes ().unbind (entry);
                  (void) idl_global->file_prefixes ().bind (ACE::strnew (ext_id),
                                                            ACE::strnew (new_prefix));
                }
            }

          UTL_Scope *top_scope = idl_global->scopes ().top ();

          if (depth > 1)
            {
              top_scope->has_prefix (true);
              ScopeAsDecl (top_scope)->prefix_scope (top_scope);
            }

          idl_global->pragma_prefixes ().push (new_prefix);

          if (depth == 1)
            {
              idl_global->root ()->prefix (new_prefix);
            }

          if (idl_global->in_main_file ())
            {
              idl_global->root ()->set_imported (false);
              top_scope->has_prefix (true);
            }

          if (status != 0)
            {
              (void) idl_global->file_prefixes ().bind (ACE::strnew (ext_id),
                                                        ACE::strnew (new_prefix));
            }
        }
    }
  else if (ACE_OS::strncmp (buf + 8, "version", 7) == 0)
    {
      char *tmp = buf + 16;

      while (*tmp == ' ' || *tmp == '\t')
        {
          ++tmp;
        }

      char *number = ACE_OS::strchr (tmp, ' ');

      if (number == 0)
        {
          number = ACE_OS::strchr (tmp, '\t');
        }

      // Used later to trim the type string.
      ptrdiff_t cutoff = number - tmp;

      // Most likely this means there is only a version number
      // without an identifier to apply it to.
      if (number == 0)
        {
          const char *msg = "no identifier or scoped name";
          idl_global->err ()->version_syntax_error (msg);
          return;
        }

      while (*number == ' ' || *number == '\t')
        {
          ++number;
        }

      size_t len = ACE_OS::strlen (number);

      // For some reason, the SunCC preprocessor adds a trailing space, which
      // messes with idl_valid_version() below, so we check and remove.
      while (number[len - 1] == ' ')
        {
          number[len - 1] = '\0';
          len = ACE_OS::strlen (number);
        }

      // Trim the type string after trimming the number string.
      tmp[cutoff] = '\0';

      // This call adds a proper null terminator to tmp, so no need to
      // do it here.
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
          if (d->typeid_set ())
            {
              idl_global->err ()->id_reset_error (d->repoID (),
                                                  new_id);
              return;
            }

          d->repoID (new_id);
          d->typeid_set (true);
        }
    }
  else if (ACE_OS::strncmp (buf + 8, "DCPS_DATA_TYPE", 14) == 0)
    {
      char *sample_type = idl_get_pragma_string (buf);
      idl_global->add_dcps_data_type (sample_type);

      // Delete sample_type since add_dcps_data_type() doesn't take its ownership.
      delete [] sample_type;
    }
  else if (ACE_OS::strncmp (buf + 8, "DCPS_DATA_KEY", 13) == 0)
    {
      char *tmp = idl_get_pragma_string (buf);

      // Split up data type and key strings
      char *sample_type = tmp;

      while (*tmp && !isspace (*tmp))
        {
          ++tmp;
        }

      while (isspace (*tmp))
        {
          *tmp = '\0';
          tmp++;
        }

      char *key = tmp;

      if (!idl_global->add_dcps_data_key (sample_type, key))
        {
          ACE_ERROR((LM_ERROR, "DCPS_DATA_TYPE \"%C\" not found for key \"%C\"\n",
            sample_type, key));
        }

      // Delete sample_type since add_dcps_data_key() doesn't take its ownership.
      delete [] sample_type;
    }
  else if (ACE_OS::strncmp (buf + 8, "DCPS_SUPPORT_ZERO_COPY_READ", 27) == 0)
    {
      idl_global->dcps_support_zero_copy_read (true);
    }
  else if (ACE_OS::strncmp (buf + 8, "DCPS_GEN_ZERO_COPY_READ", 23) == 0)
    {
      idl_global->dcps_gen_zero_copy_read (true);
    }
  else if (ACE_OS::strncmp (buf + 8, "ciao lem", 8) == 0)
    {
      char *tmp = idl_get_pragma_string (buf);
      idl_global->add_ciao_lem_file_names (tmp);

      // Delete tmp since add_ciao_lem_file_names() doesn't take its ownership.
      delete [] tmp;
    }
  else if (ACE_OS::strncmp (buf + 8, "ndds typesupport", 16) == 0)
    {
      char *tmp = idl_get_pragma_string (buf);
      idl_global->add_ciao_rti_ts_file_names (tmp);

      // Delete tmp since add_ciao_rti_ts_file_names() doesn't take its ownership.
      delete [] tmp;
    }
  else if (ACE_OS::strncmp (buf + 8, "opendds typesupport", 19) == 0)
    {
      char *tmp = idl_get_pragma_string (buf);
      idl_global->add_ciao_oci_ts_file_names (tmp);

      // Delete tmp since add_ciao_oci_ts_file_names() doesn't take its ownership.
      delete [] tmp;
    }
  else if (ACE_OS::strncmp (buf + 8, "splice typesupport", 18) == 0)
    {
      char *tmp = idl_get_pragma_string (buf);
      idl_global->add_ciao_spl_ts_file_names (tmp);

      // Delete tmp since add_ciao_spl_ts_file_names() doesn't take its ownership.
      delete [] tmp;
    }
  else if ((ACE_OS::strncmp (buf + 8, "ciao ami4ccm interface", 22) == 0) ||
           (ACE_OS::strncmp (buf + 8, "ami4ccm interface", 17) == 0))
    {
      if (idl_global->in_main_file ())
        {
          char *tmp = idl_get_pragma_string (buf);
          idl_global->add_ciao_ami_iface_names (tmp);

          // Delete tmp since add_ciao_ami_iface_names() doesn't take its ownership.
          delete [] tmp;
        }
    }
  else if ((ACE_OS::strncmp (buf + 8, "ciao ami4ccm receptacle", 23) == 0)||
           (ACE_OS::strncmp (buf + 8, "ami4ccm receptacle", 18) == 0))
    {
      char *tmp = idl_get_pragma_string (buf);

      if (idl_global->in_main_file ())
        {
          idl_global->add_ciao_ami_recep_names (tmp);
        }
      else
        {
          /// This is intended for the executor IDL file,
          /// when a pragma is seen in the main file, but
          /// it will do no harm in other cases.
          idl_global->add_included_ami_recep_names (tmp);
        }

      // Delete tmp since add_ciao_spl_ts_file_names() doesn't take its ownership.
      delete [] tmp;
    }
  else if ((ACE_OS::strncmp (buf + 8, "ciao ami4ccm idl", 16) == 0) ||
           (ACE_OS::strncmp (buf + 8, "ami4ccm idl", 11) == 0))
    {
      char *tmp = idl_get_pragma_string (buf);

      /// These pragmas are found in the file where the interface
      /// that has a AMI4CCM_* counterpart in the *A.idl file
      /// is declared. We add the filename to the list in all
      /// IDL files *except* the one where it is found, to
      /// eliminate a circular include of xxxC.h and xxxAC.h.
      if (!idl_global->in_main_file ())
        {
          idl_global->add_ciao_ami_idl_fnames (tmp);
        }

      // Delete tmp since add_ciao_ami_idl_fnames() doesn't take its ownership.
      delete [] tmp;
    }
  else if (ACE_OS::strncmp (buf + 8, "dds4ccm impl", 12) == 0)
    {
      char *tmp = idl_get_pragma_string (buf);

      idl_global->add_dds4ccm_impl_fnames (tmp);

      // Delete tmp since add_dds4ccm_impl_fnames() doesn't take its ownership.
      delete [] tmp;
    }
}

/*
 * idl_atoi - Convert a string of digits into a negative integer according to base b
 */
static ACE_CDR::LongLong
idl_atoi (char *s, long b)
{
  ACE_CDR::LongLong r = ACE_CDR_LONGLONG_INITIALIZER;

  // Skip over the dash and possibly spaces after the dash
  while (*s == '-' || *s == ' ' || *s == '\t')
    {
      ++s;
    }

  if (b == 8 && *s == '0')
    {
      ++s;
    }
  else if (b == 16 && *s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X'))
    {
      s += 2;
    }

  for (; *s; ++s)
    {
      if (*s <= '9' && *s >= '0')
        {
          r = (r * b) + (*s - '0');
        }
      else if (b > 10 && *s <= 'f' && *s >= 'a')
        {
          r = (r * b) + (*s - 'a' + 10);
        }
      else if (b > 10 && *s <= 'F' && *s >= 'A')
        {
          r = (r * b) + (*s - 'A' + 10);
        }
      else
        {
          break;
        }
    }

  return -r;
}

/*
 * idl_atoui - Convert a string of digits into an unsigned integer according to base b
 */
static ACE_CDR::ULongLong
idl_atoui(char *s, long b)
{
  ACE_CDR::ULongLong r = 0;

  if (b == 8 && *s == '0')
    {
      ++s;
    }
  else if (b == 16 && *s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X'))
    {
      s += 2;
    }

  for (; *s; ++s)
    {
      if (*s <= '9' && *s >= '0')
        {
          r = (r * b) + (*s - '0');
        }
      else if (b > 10 && *s <= 'f' && *s >= 'a')
        {
          r = (r * b) + (*s - 'a' + 10);
        }
      else if (b > 10 && *s <= 'F' && *s >= 'A')
        {
          r = (r * b) + (*s - 'A' + 10);
        }
      else
        {
          break;
        }
    }

  return r;
}

/*
 * Convert a string to a float; atof doesn't seem to work, always.
 */
static ACE_CDR::Double
idl_atof (char *s)
{
  double d = 0.0;
  double e, k;
  long neg = 0, negexp = 0;

  if (*s == '-')
    {
      neg = 1;

      // Skip over the dash and possibly spaces after the dash
      while (*s == '-' || *s == ' ' || *s == '\t')
        {
          ++s;
        }
    }

  while (*s >= '0' && *s <= '9')
    {
      d = (d * 10) + *s - '0';
      ++s;
    }

  if (*s == '.')
    {
      ++s;
      e = 10;

      while (*s >= '0' && *s <= '9')
        {
          d += (*s - '0') / (e * 1.0);
          e *= 10;
          ++s;
        }
    }

  if (*s == 'e' || *s == 'E')
    {
      ++s;

      if (*s == '-')
        {
            negexp = 1;
            s++;
        }
      else if (*s == '+')
        {
          ++s;
        }

      e = 0;

      while (*s >= '0' && *s <= '9')
        {
          e = (e * 10) + *s - '0';
          ++s;
        }

      if (e > 0)
        {
          for (k = 1; e > 0; k *= 10, e--) ;

          if (negexp)
            {
              d /= k;
            }
          else
            {
              d *= k;
            }
        }
    }

  if (neg)
    {
      d *= -1.0;
    }

  return d;
}

/*
 * Convert (some) escaped characters into their ascii values
 */
static char
idl_escape_reader (char *str)
{
  if (str[0] != '\\')
    {
      return str[0];
    }

  switch (str[1])
  {
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
        for (i = 2; str[i] != '\0' && isxdigit (str[i]); ++i)
          {
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
      if (str[1] >= '0' && str[1] <= '7')
        {
          int i;

          for (i = 1; str[i] >= '0' && str[i] <= '7'; ++i)
            {
              continue;
            }

          char save = str[i];
          str[i] = '\0';
          char out = (char)idl_atoui(&str[1], 8);
          str[i] = save;
          return out;
        }
      else
        {
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
  const char *firstquote = ACE_OS::strchr (pragma, '"');

  if (firstquote == 0)
    {
      idl_global->err ()->syntax_error (
          IDL_GlobalData::PS_PragmaPrefixSyntax
        );

      return 0;
    }

  const char *start = firstquote + 1;
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

static bool
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

  ptrdiff_t const len = minor - s;

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
  UTL_ScopedName * node = FE_Utils::string_to_scoped_name (s);
  AST_Decl * d = 0;

  if (node != 0)
    {
      d = idl_global->scopes ().top_non_null ()->lookup_by_name (node);

      if (d == 0)
        {
          idl_global->err ()->lookup_error (node);
        }

      node->destroy ();
      delete node;
    }

  return d;
}
