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

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
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

#include "ace/Env_Value_T.h"

#include "utl_scoped_name.h"
#include "utl_identifier.h"
#include "global_extern.h"
#include "fe_extern.h"

#include "ast_module.h"
#include "ast_predefined_type.h"
#include "ast_generator.h"
#include "ast_root.h"

#include <string>
#include <unordered_set>

// Populate the global scope with all predefined entities.
void
fe_populate_global_scope ()
{
  // No need to create a scoped name for the basic types, the
  // AST_PredefinedType constructor will do that.

  AST_PredefinedType *pdt = nullptr;
  AST_Root *root =
    dynamic_cast<AST_Root*> (idl_global->root ());

  Identifier void_id ("void");
  UTL_ScopedName void_name (&void_id, nullptr);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_void,
      &void_name);

  root->fe_add_predefined_type (pdt);

  /// Put this prefix in force while we're creating the
  /// CORBA module and its contents.
  idl_global->pragma_prefixes ().push_back ("omg.org");

  Identifier corba_id ("CORBA");
  UTL_ScopedName sn (&corba_id, nullptr);

  AST_Module *m =
    idl_global->gen ()->create_module (root, &sn);

  root->fe_add_module (m);
  idl_global->corba_module (m);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_long,
      nullptr);

  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_ulong,
      nullptr);

  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_longlong,
      nullptr);

  m->fe_add_predefined_type(pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_ulonglong,
      nullptr);

  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_short,
      nullptr);

  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_ushort,
      nullptr);

  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_float,
      nullptr);

  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_double,
      nullptr);

  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_longdouble,
      nullptr);

  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_char,
      nullptr);

  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_wchar,
      nullptr);

  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_octet,
      nullptr);

  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_any,
      nullptr);

  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_boolean,
      nullptr);

  m->fe_add_predefined_type (pdt);

  m->fe_add_predefined_type (idl_global->gen ()->create_predefined_type (
    AST_PredefinedType::PT_uint8, nullptr));

  m->fe_add_predefined_type (idl_global->gen ()->create_predefined_type (
    AST_PredefinedType::PT_int8, nullptr));

  Identifier Object_id ("Object");
  UTL_ScopedName Object_name (&Object_id, nullptr);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_object,
      &Object_name);

  m->fe_add_predefined_type (pdt);

// Add these to make all keywords protected even in different spellings

  Identifier ValueBase_id ("ValueBase");
  UTL_ScopedName ValueBase_name (&ValueBase_id, nullptr);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_value,
      &ValueBase_name);

  m->fe_add_predefined_type (pdt);

  Identifier AbstractBase_id ("AbstractBase");
  UTL_ScopedName AbstractBase_name (&AbstractBase_id, nullptr);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_abstract,
      &AbstractBase_name);

  m->fe_add_predefined_type (pdt);

  Identifier TypeCode_id ("TypeCode");
  UTL_ScopedName TypeCode_name (&TypeCode_id, nullptr);

  pdt =
    idl_global->gen ()->create_predefined_type (
      AST_PredefinedType::PT_pseudo,
      &TypeCode_name);

  m->fe_add_predefined_type (pdt);

  if (!idl_global->pragma_prefixes ().empty ())
    {
      idl_global->pragma_prefixes ().pop_back ();
    }
}

// Populate idl_global's hash map with upper case versions of
// all the IDL keywords
void
fe_populate_idl_keywords ()
{
  static constexpr char const* keywords[] =
    {
      "ABSTRACT",
      "ALIAS",
      "ANY",
      "ATTRIBUTE",
      "BOOLEAN",
      "CASE",
      "CHAR",
      "COMPONENT",
      "CONNECTOR",
      "CONST",
      "CONSUMES",
      "CONTEXT",
      "CUSTOM",
      "DEFAULT",
      "DOUBLE",
      "EMITS",
      "EXCEPTION",
      "ENUM",
      "EVENTTYPE",
      "FACTORY",
      "FALSE",
      "FINDER",
      "FIXED",
      "FLOAT",
      "GETRAISES",
      "HOME",
      "IMPORT",
      "IN",
      "INOUT",
      "INTERFACE",
      "LOCAL",
      "LONG",
      "MIRRORPORT",
      "MODULE",
      "MULTIPLE",
      "NATIVE",
      "OBJECT",
      "OCTET",
      "ONEWAY",
      "OUT",
      "PORT",
      "PORTTYPE",
      "PRIMARYKEY",
      "PRIVATE",
      "PROVIDES",
      "PUBLIC",
      "PUBLISHES",
      "RAISES",
      "READONLY",
      "SEQUENCE",
      "SETRAISES",
      "SHORT",
      "STRING",
      "STRUCT",
      "SUPPORTS",
      "SWITCH",
      "TRUE",
      "TRUNCATABLE",
      "TYPEDEF",
      "TYPEID",
      "TYPEPREFIX",
      "UNION",
      "UNSIGNED",
      "USES",
      "VALUEBASE",
      "VALUETYPE",
      "VOID",
      "WCHAR",
      "WSTRING"
    };

  std::unordered_set<std::string> &keywords_set = idl_global->idl_keywords ();

  for (char const* const keyword : keywords)
    {
      keywords_set.emplace (keyword);
    }
}

// FE initialization
void
FE_init ()
{
  // Initialize FE global data object.
  ACE_NEW (idl_global,
           IDL_GlobalData);

  // Initialize some of its data.
  idl_global->set_root (nullptr);
  idl_global->set_gen (nullptr);
  idl_global->set_err (FE_new_UTL_Error ());
  idl_global->set_err_count (0);
  idl_global->set_indent (FE_new_UTL_Indenter ());
  idl_global->set_import (true);
  idl_global->set_in_main_file (false);
  idl_global->set_lineno (-1);
  idl_global->clear_prog_name ();

  idl_global->set_local_escapes ({});
  idl_global->set_compile_flags (0);
  idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
  idl_global->preserve_cpp_keywords (false);

  // Put an empty prefix on the stack for the global scope.
  idl_global->pragma_prefixes ().emplace_back ();
}

void
FE_populate ()
{
  AST_Root *r = nullptr;

  // Check that the BE init created a generator object
  if (idl_global->gen () == nullptr)
    {
      ACE_ERROR ((
          LM_ERROR,
          ACE_TEXT ("IDL: idl_global->gen() not initialized, exiting\n")
        ));

      throw Bailout ();
    }

  // Create a global root for the AST. Note that the AST root has no name.
  Identifier root_id ("");
  UTL_ScopedName root_name (&root_id, nullptr);
  r = idl_global->gen ()->create_root (&root_name);
  idl_global->set_root (r);

  if (r == nullptr)
    {
      ACE_ERROR ((
          LM_ERROR,
          ACE_TEXT ("IDL: FE init failed to create AST root, exiting\n")
        ));

      throw Bailout ();
    }

  // Push it on the stack
  idl_global->scopes ().push (idl_global->root ());

  // Populate it with nodes for predefined types.
  fe_populate_global_scope ();

  // Set flag to indicate we are processing the main file now.
  idl_global->set_in_main_file (true);

  // Populate the IDL keyword container, for checking local identifiers.
  fe_populate_idl_keywords ();
}

void
FE_extract_env_include_paths (std::vector<std::string> &list)
{
  ACE_Env_Value<char*> incl_paths (ACE_TEXT ("INCLUDE"),
                                   nullptr);
  char const * const env_paths = incl_paths;

  if (env_paths != nullptr)
    {
#if defined (ACE_WIN32)
      constexpr char separator = ';';
#else
      constexpr char separator = ':';
#endif
      std::string const paths (env_paths);
      std::string::size_type start = 0;

      do
        {
          std::string::size_type const end = paths.find (separator, start);
          if (end == std::string::npos)
            {
              list.emplace_back (paths.substr (start));
              break;
            }

          list.emplace_back (paths.substr (start, end - start));
          start = end + 1;
        }
      while (true);
    }
}

// Store include paths from the environment variable, if any.
void
FE_store_env_include_paths ()
{
  std::vector<std::string> list;
  FE_extract_env_include_paths (list);

  for (std::string const &path : list)
    {
      idl_global->add_include_path (path.c_str (), false);
    }
}

std::string
FE_get_cpp_loc_from_env ()
{
  // See if TAO_IDL_PREPROCESSOR is defined.
  ACE_Env_Value<char*> preprocessor (ACE_TEXT ("TAO_IDL_PREPROCESSOR"),
                                     nullptr);

  char const * const env_preprocessor = preprocessor;
  if (env_preprocessor != nullptr)
    {
      return env_preprocessor;
    }

  return idl_global->cpp_location ();
}

std::optional<std::string>
FE_get_cpp_args_from_env ()
{
  // Added some customizable preprocessor options
  ACE_Env_Value<char*> args1 (ACE_TEXT ("TAO_IDL_PREPROCESSOR_ARGS"),
                              nullptr);

  char const * const env_args = args1;
  if (env_args != nullptr)
    {
      return std::string (env_args);
    }

  return std::nullopt;
}
