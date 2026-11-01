%{
/*

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
 * idl.ll - Lexical scanner for IDL
 */

#include <string>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <optional>

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
#include "ast_annotation_appl.h"
#include "idl.tab.hpp"

#include <string_view>

static ACE_CDR::WChar       idl_wchar_escape_reader (char *);
static ACE_CDR::Char        idl_escape_reader (char *);
static ACE_CDR::Double      idl_atof (char const *);
static ACE_CDR::LongLong    idl_atoi (char const *, long);
static ACE_CDR::ULongLong   idl_atoui (char const *, long);
static void                 idl_parse_line_and_file (char *);
static void                 idl_store_pragma (char *);
static std::optional<std::string> idl_get_pragma_string (char *);
static bool                 idl_valid_version (std::string const&);
static AST_Decl *           idl_find_node (std::string const&);

#define ace_yytext yytext

#ifdef USE_MCPP_BUFFER_LEXING
#include "mcpp_yyinput.h"
#endif /*USE_MCPP_BUFFER_LEXING*/

#if defined __ANDROID__ && defined ECHO
#undef ECHO
#endif

#define IDL4_KEYWORD(TOKEN_NAME) if (idl_global->idl_version_ >= IDL_VERSION_4) return TOKEN_NAME;

#ifdef __GNUC__
#  pragma GCC diagnostic ignored "-Wmisleading-indentation"
#endif

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
                  tao_yylval.strval = ACE::strnew ("::");
                  return IDL_SCOPE_DELIMITOR;
                }

@annotation[^A-Za-z0-9_] return IDL_ANNOTATION_DECL; // Allow annotation names that start with "annotation"
@ return IDL_ANNOTATION_SYMBOL;

int8 IDL4_KEYWORD(IDL_INT8); REJECT;
uint8 IDL4_KEYWORD(IDL_UINT8); REJECT;
int16 IDL4_KEYWORD(IDL_INT16); REJECT;
uint16 IDL4_KEYWORD(IDL_UINT16); REJECT;
int32 IDL4_KEYWORD(IDL_INT32); REJECT;
uint32 IDL4_KEYWORD(IDL_UINT32); REJECT;
int64 IDL4_KEYWORD(IDL_INT64); REJECT;
uint64 IDL4_KEYWORD(IDL_UINT64); REJECT;

bitfield IDL4_KEYWORD(IDL_BITFIELD); REJECT;
bitmask IDL4_KEYWORD(IDL_BITMASK); REJECT;
bitset IDL4_KEYWORD(IDL_BITSET); REJECT;
map IDL4_KEYWORD(IDL_MAP); REJECT;

[a-ij-rs-zA-IJ-RS-Z_][a-ij-rs-zA-IJ-RS-Z0-9_]* {
  // Make sure that this identifier is not a C++ keyword. If it is,
  // prepend it with a _cxx_. Lookup in the perfect hash table for C++
  // keyword and grab the mapping.  BTW, the reason for the odd
  // regular expression is to handle EBCDIC, as well as ASCII.

  TAO_IDL_CPP_Keyword_Table cpp_key_tbl;
  TAO_IDL_CPP_Keyword_Entry const *entry = nullptr;
  if (!idl_global->preserve_cpp_keywords())
    {
      // This check will ensure that escaped C++ keywords will be
      // caught and prepended with '_cxx' as non-escaped keywords
      // are now prepended with '_cxx_'.
      const char *tmp =
        ace_yytext[0] == '_' ? ace_yytext + 1 : ace_yytext;

      entry =
        cpp_key_tbl.lookup (tmp,
                            static_cast<unsigned int> (std::strlen (tmp)));
    }

  if (entry)
    {
      tao_yylval.strval = ACE::strnew (entry->mapping_);
    }
  else
    {
      tao_yylval.strval = ACE::strnew (ace_yytext);
    }

  return IDENTIFIER;
}

"-"?(([0-9]+"."[0-9]*)|("."[0-9]+))([eE][+-]?[0-9]+)?[lLfF]?      {
                  tao_yylval.dval = idl_atof (ace_yytext);
                  return IDL_FLOATING_PT_LITERAL;
                }
"-"?[0-9]+[eE][+-]?[0-9]+[lLfF]?  {
                  tao_yylval.dval = idl_atof (ace_yytext);
                  return IDL_FLOATING_PT_LITERAL;
                }

"-"?(([0-9]+"."[0-9]*)|("."?[0-9]+))[dD] {
                  tao_yylval.fixval = ACE_CDR::Fixed::from_string (ace_yytext);
                  return IDL_FIXED_PT_LITERAL;
                }

"-"[1-9][0-9]*  {
                  tao_yylval.ival = idl_atoi (ace_yytext, 10);
                  return IDL_INTEGER_LITERAL;
                }
[1-9][0-9]*     {
                  tao_yylval.uival = idl_atoui (ace_yytext, 10);
                  return IDL_UINTEGER_LITERAL;
                }
"-"0[xX][a-fA-F0-9]+ {
                  tao_yylval.ival = idl_atoi (ace_yytext, 16);
                  return IDL_INTEGER_LITERAL;
                }
0[xX][a-fA-F0-9]+ {
                  tao_yylval.uival = idl_atoui (ace_yytext, 16);
                  return IDL_UINTEGER_LITERAL;
                }
"-"0[0-7]*      {
                  tao_yylval.ival = idl_atoi (ace_yytext, 8);
                  return IDL_INTEGER_LITERAL;
                }
0[0-7]*         {
                  tao_yylval.uival = idl_atoui (ace_yytext, 8);
                  return IDL_UINTEGER_LITERAL;
                }

(\"([^\\\"]*|\\[ntvbrfax\\\?\'\"]|\\[0-7]{1,3})*\"[ \t]*)+ {
                  /* Skip the quotes */
                  char * const tmp = ace_yytext;
                  for (size_t i = std::strlen (tmp); i-- != 0; )
                    {
                      if (isspace (tmp[i]))
                        {
                          tmp[i] = '\0';
                        }
                      else
                        {
                          break;
                        }
                    }
                  tmp[std::strlen (tmp) - 1] = '\0';
                  ACE_NEW_RETURN (tao_yylval.sval,
                                  std::string (tmp + 1),
                                  IDL_STRING_LITERAL);
                  return IDL_STRING_LITERAL;
                }
(L\"([^\\\"]*|\\[ntvbrfax\\\?\'\"]|\\[0-7]{1,3}|\\u([0-9a-fA-F]{1,4}))*\"[ \t]*)+ {
                  /* Skip the bookends */
                  char * const tmp = ace_yytext;
                  for (size_t i = std::strlen (tmp); i-- != 0; )
                    {
                      if (isspace (tmp[i]))
                        {
                          tmp[i] = '\0';
                        }
                      else
                        {
                          break;
                        }
                    }
                  tmp[std::strlen (tmp) - 1] = '\0';
                  ACE_NEW_RETURN (tao_yylval.wsval,
                                  std::string (tmp + 2),
                                  IDL_WSTRING_LITERAL);
                  return IDL_WSTRING_LITERAL;
                }
"'"."'"         {
                  tao_yylval.cval = ace_yytext[1];
                  return IDL_CHARACTER_LITERAL;
                }
"'"\\([0-7]{1,3})"'" {
                  // octal character constant
                  tao_yylval.cval = idl_escape_reader (ace_yytext + 1);
                  return IDL_CHARACTER_LITERAL;
                }
"'"\\[xX]([0-9a-fA-F]{1,2})"'" {
                  // hexadecimal character constant
                  tao_yylval.cval = idl_escape_reader (ace_yytext + 1);
                  return IDL_CHARACTER_LITERAL;
                }
"'"\\."'"       {
                  tao_yylval.cval = idl_escape_reader (ace_yytext + 1);
                  return IDL_CHARACTER_LITERAL;
                }
L"'"."'"        {
                  // wide character constant
                  tao_yylval.wcval = ace_yytext[2];
                  return IDL_WCHAR_LITERAL;
                }
L"'"\\u([0-9a-fA-F]{1,4})"'" {
                  // hexadecimal wide character constant
                  tao_yylval.wcval = idl_wchar_escape_reader (ace_yytext + 2);
                  return IDL_WCHAR_LITERAL;
                }
^[ \t]*#[ \t]*pragma[ \t].*{NL} |
^\?\?=[ \t]*pragma[ \t].*{NL} {/* remember pragma */
                  idl_global->set_lineno (idl_global->lineno () + 1);
                  idl_store_pragma (ace_yytext);
                  break;
                }
^[ \t]*#file[ \t].*{NL} |
^\?\?=[ \t]*file[ \t].*{NL} {/* ignore file */
                  idl_global->set_lineno(idl_global->lineno () + 1);
                  break;
                }
^[ \t]*#[ \t]*[0-9]*" ""\""[^\"]*"\""" "[0-9]*([ \t]*[0-9]*)*{NL} |
^\?\?=[ \t]*[0-9]*" ""\""[^\"]*"\""" "[0-9]*([ \t]*[0-9]*)?{NL} {
                  idl_parse_line_and_file (ace_yytext);
                  break;
                }
^[ \t]*#[ \t]*[0-9]*" ""\""[^\"]*"\""{NL} |
^\?\?=[ \t]*[0-9]*" ""\""[^\"]*"\""{NL} {
                  idl_parse_line_and_file (ace_yytext);
                  break;
                }
^[ \t]*#line[ \t]*[0-9]+[ \t]*("\""[^\"]*"\""([ \t]*[0-9]*([ \t]*[0-9]*)?)?)?{NL} |
^\?\?=line[ \t]*[0-9]*" ""\""[^\"]*"\""{NL} {
                  idl_parse_line_and_file (ace_yytext);
                  break;
                }
^[ \t]*#[ \t]*[0-9]*{NL} |
^\?\?=[ \t]*[0-9]*{NL} {
                  idl_parse_line_and_file (ace_yytext);
                  break;
                }
^[ \t]*#[ \t]*ident[ \t].*{NL} |
^\?\?=[ \t]*ident[ \t].*{NL} {
                  /* ignore cpp ident */
                  idl_global->set_lineno (idl_global->lineno () + 1);
                  break;
                }
\/\/.*{NL}      {
                  /* ignore comments */
                  idl_global->set_lineno(idl_global->lineno () + 1);
                  break;
                }
"/*"            {
                  for (;;)
                    {
                      char const c = static_cast<char> (yyinput ());
                      if (c == '*')
                        {
                          char const next = static_cast<char> (yyinput ());
                          if (next == '/')
                            break;
                          else
                            yyunput (c, 0);
                        }
                      else if (c == '\n')
                        {
                          idl_global->set_lineno (idl_global->lineno () + 1);
                        }
                    }
                  break;
                }
[ \t]*          break;
{NL}            {
                  idl_global->set_lineno (idl_global->lineno () + 1);
                  break;
                }
.               return ace_yytext[0];

%%
        /* subroutines */

static bool
same_file (std::string const& path1, std::string const& path2)
{
  char fullpath1 [MAXPATHLEN + 1] = {};
  char fullpath2 [MAXPATHLEN + 1] = {};

  char *fp1 = ACE_OS::realpath (path1.c_str (), fullpath1);
  char *fp2 = ACE_OS::realpath (path2.c_str (), fullpath2);

  return
    (fp1 == nullptr || fp2 == nullptr)
      ? false
      : FE_Utils::path_cmp (fullpath1, fullpath2) == 0;
}

// Parse a #line statement generated by the C preprocessor
static void
idl_parse_line_and_file (char *buf)
{
  std::string const line (buf);
  if (line.empty () || line[0] != '#')
    {
      return;
    }

  size_t position = 1;

  // Check to see if we're running under the screwy Microsoft scheme
  // of putting #line num instead of #num.
  if (line.compare (position, 4, "line") == 0)
    {
      position += 5;
      if (position > line.size ())
        {
          return;
        }
    }

  // Find line number.
  while (position < line.size ()
         && isspace (static_cast<unsigned char> (line[position])))
    {
      ++position;
    }

  size_t const line_number_start = position;

  while (position < line.size ()
         && isdigit (static_cast<unsigned char> (line[position])))
    {
      ++position;
    }

  std::string const line_number = line.substr (line_number_start,
                                               position - line_number_start);
  idl_global->set_lineno (static_cast<long> (idl_atoui (line_number.c_str (), 10)));

  // Find file name, if present.
  size_t const filename_start = line.find ('"', position);
  if (filename_start == std::string::npos)
    {
      return;
    }

  size_t const filename_end = line.find ('"', filename_start + 1);
  if (filename_end == std::string::npos)
    {
      return;
    }

  std::string const filename = line.substr (filename_start + 1,
                                            filename_end - filename_start - 1);
  if (filename.empty ())
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("IDL: No input files\n")));

      throw Bailout ();
    }
  else
    {
      // Put Microsoft-style pathnames into a canonical form.
      std::string canonical_filename;
      canonical_filename.reserve (filename.size ());
      for (size_t i = 0; i < filename.size (); ++i)
        {
          canonical_filename += filename[i];
          if (filename[i] == '\\'
              && i + 1 < filename.size ()
              && filename[i + 1] == '\\')
            {
              ++i;
            }
        }

      idl_global->update_prefix (canonical_filename);
      idl_global->set_filename (canonical_filename);
    }

  std::string const& fname = idl_global->filename ();
  bool in_main_file = false;

  bool is_real_filename =
    UTL_String::compare (fname, idl_global->real_filename ())
    || same_file (fname, idl_global->real_filename ());

  bool is_main_filename = false;

  if (!is_real_filename)
    {
      is_main_filename =
        UTL_String::compare (fname, idl_global->main_filename ())
        || same_file (fname, idl_global->main_filename ());
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
      std::string const nm =
        FE_Utils::stripped_preproc_include (fname);

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

  if (std::strstr (buf + 8, "import") != nullptr)
    {
      idl_global->set_import (true);
      return;
    }

  if (std::strstr (buf + 8, "include") != nullptr)
    {
      idl_global->set_import (false);
      return;
    }

  if (std::strncmp (buf + 8, "prefix", 6) == 0)
    {
      std::optional<std::string> prefix_string = idl_get_pragma_string (buf);

      if (prefix_string)
        {
          std::string const& new_prefix = *prefix_string;
          unsigned long depth = idl_global->scopes ().depth ();

          // We replace the prefix only if there is a prefix already
          // associated with this file, otherwise we add the prefix.
          std::string ext_id (idl_global->filename ());
          IDL_GlobalData::File_Prefix_Map &file_prefixes =
            idl_global->file_prefixes ();
          IDL_GlobalData::File_Prefix_Map::iterator const entry =
            file_prefixes.find (ext_id);
          bool const found = entry != file_prefixes.end ();

          if (found)
            {
              if (!entry->second.empty ())
                {
                  if (!idl_global->pragma_prefixes ().empty ())
                    {
                      idl_global->pragma_prefixes ().pop_back ();
                    }
                }
              else if (depth == 1)
                {
                  // Remove the default "" and bind the new prefix.
                  entry->second = new_prefix;
                }
            }

          UTL_Scope *top_scope = idl_global->scopes ().top ();

          if (depth > 1)
            {
              top_scope->has_prefix (true);
              ScopeAsDecl (top_scope)->prefix_scope (top_scope);
            }

          idl_global->pragma_prefixes ().push_back (new_prefix);

          if (depth == 1)
            {
              idl_global->root ()->prefix (new_prefix);
            }

          if (idl_global->in_main_file ())
            {
              idl_global->root ()->set_imported (false);
              top_scope->has_prefix (true);
            }

          if (!found)
            {
              file_prefixes.emplace (ext_id, new_prefix);
            }
        }
    }
  else if (std::strncmp (buf + 8, "version", 7) == 0)
    {
      std::string pragma_value (buf + 16);
      size_t const name_start = pragma_value.find_first_not_of (" \t");
      pragma_value.erase (0, name_start);
      size_t const separator = pragma_value.find_first_of (" \t");

      // Most likely this means there is only a version number
      // without an identifier to apply it to.
      if (separator == std::string::npos)
        {
          char const *msg = "no identifier or scoped name";
          idl_global->err ()->version_syntax_error (msg);
          return;
        }

      std::string const scoped_name (pragma_value.substr (0, separator));
      size_t const version_start = pragma_value.find_first_not_of (" \t",
                                                                   separator);
      std::string version = version_start == std::string::npos
        ? std::string ()
        : pragma_value.substr (version_start);

      // For some reason, the SunCC preprocessor adds a trailing space, which
      // messes with idl_valid_version() below, so we check and remove.
      while (!version.empty () && version.back () == ' ')
        {
          version.pop_back ();
        }

      AST_Decl *d = idl_find_node (scoped_name);

      if (d == nullptr)
        {
          return;
        }

      if (!idl_valid_version (version))
        {
          idl_global->err ()->version_number_error (version);
          return;
        }

      d->version (version);
    }
  else if (std::strncmp (buf + 8, "ident", 5) == 0)
    {
      idl_global->ident_string (buf + 8);
    }
  else if (std::strncmp (buf + 8, "ID", 2) == 0)
    {
      std::string work (buf + 11);
      size_t const name_start = work.find_first_not_of (' ');
      work.erase (0, name_start);
      size_t const end = work.find (' ');
      if (end != std::string::npos)
        {
          work.resize (end);
        }

      AST_Decl *d = idl_find_node (work);

      if (d == nullptr)
        {
          return;
        }

      std::optional<std::string> new_id = idl_get_pragma_string (buf);

      if (new_id)
        {
          if (d->typeid_set ())
            {
              idl_global->err ()->id_reset_error (d->repoID (),
                                                  *new_id);
              return;
            }

          d->repoID (*new_id);
          d->typeid_set (true);
        }
    }
  else if (std::strncmp (buf + 8, "DCPS_DATA_TYPE", 14) == 0)
    {
      std::optional<std::string> sample_type = idl_get_pragma_string (buf);
      if (sample_type)
        {
          idl_global->add_dcps_data_type (*sample_type);
        }
    }
  else if (std::strncmp (buf + 8, "DCPS_DATA_KEY", 13) == 0)
    {
      std::optional<std::string> tmp = idl_get_pragma_string (buf);
      if (tmp)
        {
          // Split up data type and key strings.
          size_t const separator = tmp->find_first_of (" \t\n\r\f\v");
          std::string const sample_type = tmp->substr (0, separator);
          size_t const key_start = tmp->find_first_not_of (" \t\n\r\f\v",
                                                           separator);
          std::string const key = key_start == std::string::npos
            ? std::string ()
            : tmp->substr (key_start);

          if (!idl_global->add_dcps_data_key (sample_type, key))
            {
              ACE_ERROR ((LM_ERROR,
                          "DCPS_DATA_TYPE \"%C\" not found for key \"%C\"\n",
                          sample_type.c_str (),
                          key.c_str ()));
            }
        }
    }
  else if (std::strncmp (buf + 8, "DCPS_DATA_SEQUENCE_TYPE", 23) == 0)
    {
      std::optional<std::string> seq_type = idl_get_pragma_string (buf);
      if (seq_type)
        {
          idl_global->set_dcps_sequence_type (*seq_type);
        }
    }
  else if (std::strncmp (buf + 8, "DCPS_SUPPORT_ZERO_COPY_READ", 27) == 0)
    {
      idl_global->dcps_support_zero_copy_read (true);
    }
  else if (std::strncmp (buf + 8, "DCPS_GEN_ZERO_COPY_READ", 23) == 0)
    {
      idl_global->dcps_gen_zero_copy_read (true);
    }
  else if (std::strncmp (buf + 8, "ciao lem", 8) == 0)
    {
      std::optional<std::string> tmp = idl_get_pragma_string (buf);
      if (tmp)
        {
          idl_global->add_ciao_lem_file_names (*tmp);
        }
    }
  else if (std::strncmp (buf + 8, "ndds typesupport", 16) == 0)
    {
      std::optional<std::string> tmp = idl_get_pragma_string (buf);
      if (tmp)
        {
          idl_global->add_ciao_rti_ts_file_names (*tmp);
        }
    }
  else if (std::strncmp (buf + 8, "coredx typesupport", 18) == 0)
    {
      std::optional<std::string> tmp = idl_get_pragma_string (buf);
      if (tmp)
        {
          idl_global->add_ciao_coredx_ts_file_names (*tmp);
        }
    }
  else if (std::strncmp (buf + 8, "opendds typesupport", 19) == 0)
    {
      std::optional<std::string> tmp = idl_get_pragma_string (buf);
      if (tmp)
        {
          idl_global->add_ciao_oci_ts_file_names (*tmp);
        }
    }
  else if (std::strncmp (buf + 8, "splice typesupport", 18) == 0)
    {
      std::optional<std::string> tmp = idl_get_pragma_string (buf);
      if (tmp)
        {
          idl_global->add_ciao_spl_ts_file_names (*tmp);
        }
    }
  else if ((std::strncmp (buf + 8, "ciao ami4ccm interface", 22) == 0) ||
           (std::strncmp (buf + 8, "ami4ccm interface", 17) == 0))
    {
      if (idl_global->in_main_file ())
        {
          std::optional<std::string> tmp = idl_get_pragma_string (buf);
          if (tmp)
            {
              idl_global->add_ciao_ami_iface_names (*tmp);
            }
        }
    }
  else if ((std::strncmp (buf + 8, "ciao ami4ccm receptacle", 23) == 0) ||
           (std::strncmp (buf + 8, "ami4ccm receptacle", 18) == 0))
    {
      std::optional<std::string> tmp = idl_get_pragma_string (buf);
      if (tmp && idl_global->in_main_file ())
        {
          idl_global->add_ciao_ami_recep_names (*tmp);
        }
      else if (tmp)
        {
          /// This is intended for the executor IDL file,
          /// when a pragma is seen in the main file, but
          /// it will do no harm in other cases.
          idl_global->add_included_ami_recep_names (*tmp);
        }
    }
  else if ((std::strncmp (buf + 8, "ciao ami4ccm idl", 16) == 0) ||
           (std::strncmp (buf + 8, "ami4ccm idl", 11) == 0))
    {
      std::optional<std::string> tmp = idl_get_pragma_string (buf);

      /// These pragmas are found in the file where the interface
      /// that has a AMI4CCM_* counterpart in the *A.idl file
      /// is declared. We add the filename to the list in all
      /// IDL files *except* the one where it is found, to
      /// eliminate a circular include of xxxC.h and xxxAC.h.
      if (tmp && !idl_global->in_main_file ())
        {
          idl_global->add_ciao_ami_idl_fnames (*tmp);
        }
    }
  else if (std::strncmp (buf + 8, "dds4ccm impl", 12) == 0)
    {
      std::optional<std::string> tmp = idl_get_pragma_string (buf);
      if (tmp)
        {
          idl_global->add_dds4ccm_impl_fnames (*tmp);
        }
    }
}

/*
 * idl_atoi - Convert a string of digits into a negative integer according to base b
 */
static ACE_CDR::LongLong
idl_atoi (char const *s, long b)
{
  ACE_CDR::LongLong r = 0;

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
idl_atoui (char const *s, long b)
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
idl_atof (char const *s)
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

static std::optional<std::string>
idl_get_pragma_string (char *pragma)
{
  // Get pointers to each end of the substring between the quotes.
  char const *firstquote = std::strchr (pragma, '"');

  if (!firstquote)
    {
      idl_global->err ()->syntax_error (IDL_GlobalData::PS_PragmaPrefixSyntax);

      return {};
    }

  char const *start = firstquote + 1;
  char const *end = std::strchr (start, '"');

  if (!end)
    {
      idl_global->err ()->syntax_error (IDL_GlobalData::PS_PragmaPrefixSyntax);

      return {};
    }

  return std::string (start, end);
}

static bool
idl_valid_version_part (std::string const& part)
{
  unsigned int value = 0;

  for (char c : part)
    {
      if (!std::isdigit (static_cast<unsigned char> (c)))
        {
          return false;
        }

      unsigned int const digit = static_cast<unsigned int> (c - '0');
      if (value > (ACE_UINT16_MAX - digit) / 10)
        {
          return false;
        }

      value = value * 10 + digit;
    }

  return !part.empty ();
}

static bool
idl_valid_version (std::string const& s)
{
  std::size_t const decimal = s.find ('.');

  if (decimal == std::string::npos
      || decimal == 0
      || decimal + 1 == s.size ()
      || s.find ('.', decimal + 1) != std::string::npos)
    {
      return false;
    }

  return idl_valid_version_part (s.substr (0, decimal))
      && idl_valid_version_part (s.substr (decimal + 1));
}

static AST_Decl *
idl_find_node (std::string const& s)
{
  FE_Utils::Scoped_Name_Owner node = FE_Utils::string_to_scoped_name_owner (s);
  AST_Decl * d = nullptr;

  if (node != nullptr)
    {
      d = idl_global->scopes ().top_non_null ()->lookup_by_name (node.get ());

      if (d == nullptr)
        {
          idl_global->err ()->lookup_error (node.get ());
        }
    }

  return d;
}
