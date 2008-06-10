// $Id$

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

#include "ast_module.h"
#include "ast_predefined_type.h"
#include "ast_generator.h"
#include "ast_root.h"
#include "utl_scoped_name.h"
#include "utl_identifier.h"
#include "global_extern.h"
#include "fe_extern.h"
#include "ace/Env_Value_T.h"
#include "ace/UUID.h"

ACE_RCSID (fe,
           fe_init,
           "$Id$")

const size_t LOCAL_ESCAPES_BUFFER_SIZE = 1024;

// Populate the global scope with all predefined entities.
void
fe_populate_global_scope (AST_Module *m)
{
  // No need to created a scoped name for the basic types, the
  // AST_PredefinedType constructor will do that.

  AST_PredefinedType *pdt = 0;

  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_long,
                            0
                          );
  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_ulong,
                            0
                          );
  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_longlong,
                            0
                          );
  m->fe_add_predefined_type(pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_ulonglong,
                            0
                          );
  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_short,
                            0
                          );
  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_ushort,
                            0
                          );
  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_float,
                            0
                          );
  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_double,
                            0
                          );
  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_longdouble,
                            0
                          );
  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_char,
                            0
                          );
  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_wchar,
                            0
                          );
  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_octet,
                            0
                          );
  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_any,
                            0
                          );
  m->fe_add_predefined_type (pdt);

  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_boolean,
                            0
                          );
  m->fe_add_predefined_type (pdt);

  Identifier void_id ("void");
  UTL_ScopedName void_name (&void_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_void,
                            &void_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier Object_id ("Object");
  UTL_ScopedName Object_name (&Object_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_object,
                            &Object_name
                          );
  m->fe_add_predefined_type (pdt);

// Add these to make all keywords protected even in different spellings

  Identifier attribute_id ("attribute");
  UTL_ScopedName attribute_name (&attribute_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &attribute_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier case_id ("case");
  UTL_ScopedName case_name (&case_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &case_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier const_id ("const");
  UTL_ScopedName const_name (&const_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &const_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier context_id ("context");
  UTL_ScopedName context_name (&context_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &context_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier default_id ("default");
  UTL_ScopedName default_name (&default_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &default_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier enum_id ("enum");
  UTL_ScopedName enum_name (&enum_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &enum_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier exception_id ("exception");
  UTL_ScopedName exception_name (&exception_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &exception_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier in_id ("in");
  UTL_ScopedName in_name (&in_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &in_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier out_id ("out");
  UTL_ScopedName out_name (&out_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &out_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier inout_id ("inout");
  UTL_ScopedName inout_name (&inout_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &inout_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier interface_id ("interface");
  UTL_ScopedName interface_name (&interface_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &interface_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier module_id ("module");
  UTL_ScopedName module_name (&module_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &module_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier oneway_id ("oneway");
  UTL_ScopedName oneway_name (&oneway_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &oneway_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier raises_id ("raises");
  UTL_ScopedName raises_name (&raises_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &raises_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier readonly_id ("readonly");
  UTL_ScopedName readonly_name (&readonly_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &readonly_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier sequence_id ("sequence");
  UTL_ScopedName sequence_name (&sequence_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &sequence_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier string_id ("string");
  UTL_ScopedName string_name (&string_id, 0);
  pdt =
      idl_global->gen ()->create_predefined_type (
                              AST_PredefinedType::PT_pseudo,
                              &string_name
                            );
  m->fe_add_predefined_type (pdt);

  Identifier wstring_id ("wstring");
  UTL_ScopedName wstring_name (&wstring_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &wstring_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier struct_id ("struct");
  UTL_ScopedName struct_name (&struct_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &struct_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier switch_id ("switch");
  UTL_ScopedName switch_name (&switch_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &switch_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier typedef_id ("typedef");
  UTL_ScopedName typedef_name (&typedef_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &typedef_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier union_id ("union");
  UTL_ScopedName union_name (&union_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &union_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier unsigned_id ("unsigned");
  UTL_ScopedName unsigned_name (&unsigned_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &unsigned_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier TRUE_id ("TRUE");
  UTL_ScopedName TRUE_name (&TRUE_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &TRUE_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier FALSE_id ("FALSE");
  UTL_ScopedName FALSE_name (&FALSE_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &FALSE_name
                         );
  m->fe_add_predefined_type (pdt);

  Identifier abstract_id ("abstract");
  UTL_ScopedName abstract_name (&abstract_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &abstract_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier custom_id ("custom");
  UTL_ScopedName custom_name (&custom_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &custom_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier private_id ("private");
  UTL_ScopedName private_name (&private_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &private_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier public_id ("public");
  UTL_ScopedName public_name (&public_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &public_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier supports_id ("supports");
  UTL_ScopedName supports_name (&supports_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &supports_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier truncatable_id ("truncatable");
  UTL_ScopedName truncatable_name (&truncatable_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &truncatable_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier valuetype_id ("valuetype");
  UTL_ScopedName valuetype_name (&valuetype_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            &valuetype_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier ValueBase_id ("ValueBase");
  UTL_ScopedName ValueBase_name (&ValueBase_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_value,
                            &ValueBase_name
                          );
  m->fe_add_predefined_type (pdt);

  Identifier AbstractBase_id ("AbstractBase");
  UTL_ScopedName AbstractBase_name (&AbstractBase_id, 0);
  pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_abstract,
                            &AbstractBase_name
                          );
  m->fe_add_predefined_type (pdt);
}

// Populate idl_global's hash map with upper case versions of
// all the IDL keywords
void
fe_populate_idl_keywords (void)
{
  static const char *keywords[] =
    {
      "ABSTRACT",
      "ANY",
      "ATTRIBUTE",
      "BOOLEAN",
      "CASE",
      "CHAR",
      "COMPONENT",
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
      "MODULE",
      "MULTIPLE",
      "NATIVE",
      "OBJECT",
      "OCTET",
      "ONEWAY",
      "OUT",
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

  ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex> &map =
    idl_global->idl_keywords ();

  u_long length = sizeof (keywords) / sizeof (char *);
  ACE_CString ext_id;
  int int_id = 0;

  for (u_long i = 0; i < length; ++i)
    {
      ext_id.set (keywords[i],
                  false);
      (void) map.bind (ext_id,
                       int_id);
    }
}

// FE initialization
void
FE_init (void)
{
  // Initialize FE global data object.
  ACE_NEW (idl_global,
           IDL_GlobalData);

  // Initialize some of its data.
  idl_global->set_root (0);
  idl_global->set_gen (0);
  idl_global->set_err (FE_new_UTL_Error ());
  idl_global->set_err_count (0);
  idl_global->set_indent (FE_new_UTL_Indenter ());
  idl_global->set_filename (0);
  idl_global->set_main_filename (0);
  idl_global->set_real_filename (0);
  idl_global->set_stripped_filename (0);
  idl_global->set_import (true);
  idl_global->set_in_main_file (false);
  idl_global->set_lineno (-1);
  idl_global->set_prog_name (0);

  char local_escapes[LOCAL_ESCAPES_BUFFER_SIZE];
  ACE_OS::memset (&local_escapes,
                  0,
                  LOCAL_ESCAPES_BUFFER_SIZE);

  idl_global->set_local_escapes (local_escapes);
  idl_global->set_compile_flags (0);
  idl_global->set_include_file_names (0);
  idl_global->set_n_include_file_names (0);
  idl_global->set_parse_state (IDL_GlobalData::PS_NoState);
  idl_global->preserve_cpp_keywords (false);

  // Put an empty prefix on the stack for the global scope.
  idl_global->pragma_prefixes ().push (ACE::strnew (""));
}

void
FE_populate (void)
{
  AST_Root *r = 0;

  // Check that the BE init created a generator object
  if (idl_global->gen () == 0)
    {
      ACE_ERROR ((
          LM_ERROR,
          ACE_TEXT ("IDL: idl_global->gen() not initialized, exiting\n")
        ));

      throw Bailout ();
    }

  // Create a global root for the AST. Note that the AST root has no name.
  Identifier root_id ("");
  UTL_ScopedName root_name (&root_id, 0);
  r = idl_global->gen ()->create_root (&root_name);
  idl_global->set_root (r);

  if (r == 0)
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
  fe_populate_global_scope (idl_global->root ());

  // Set flag to indicate we are processing the main file now.
  idl_global->set_in_main_file (true);

  // Populate the IDL keyword container, for checking local identifiers.
  fe_populate_idl_keywords ();
}

// Store include paths from the environment variable, if any.
void
FE_store_env_include_paths (void)
{
  // If this method have to change then don't forget to update
  // util/utl_global.cpp:IDL_GlobalData::validate_orb_include (...).
  ACE_Env_Value<char*> incl_paths (ACE_TEXT ("INCLUDE"),
                                   (char *) 0);
  const char *aggr_str = incl_paths;

  if (aggr_str != 0)
    {
      char separator;
#if defined (ACE_WIN32)
      separator = ';';
#else
      separator = ':';
#endif
      ACE_CString aggr_cstr (aggr_str);
      ACE_CString::size_type pos;

      do
        {
          pos = aggr_cstr.find (separator);
          idl_global->add_include_path (aggr_cstr.substr (0, pos).fast_rep ());
          aggr_cstr = aggr_cstr.substr (pos + 1);
        } while (pos != ACE_CString::npos);
    }
}

const char *
FE_get_cpp_loc_from_env (void)
{
  const char *cpp_loc = 0;

  // See if TAO_IDL_PREPROCESSOR is defined.
  ACE_Env_Value<char*> preprocessor (ACE_TEXT ("TAO_IDL_PREPROCESSOR"),
                                     (char *) 0);

  // Set cpp_loc to the built in location, unless it has been overriden by
  // environment variables.
  if (preprocessor != 0)
    {
      cpp_loc = preprocessor;
    }
  else
    {
      // Check for the deprecated CPP_LOCATION environment variable
      ACE_Env_Value<char*> cpp_path (ACE_TEXT ("CPP_LOCATION"),
                                     (char *) 0);

      if (cpp_path != 0)
        {
          ACE_ERROR ((LM_WARNING,
                      "WARNING: The environment variable "
                      "CPP_LOCATION has been deprecated.\n"
                      "         Please use TAO_IDL_PREPROCESSOR "
                      "instead.\n"));

          cpp_loc = cpp_path;
        }
      else
        {
          cpp_loc = idl_global->cpp_location ();
        }
    }

  return cpp_loc;
}

const char *
FE_get_cpp_args_from_env (void)
{
  const char *cpp_args = 0;

  // Added some customizable preprocessor options
  ACE_Env_Value<char*> args1 (ACE_TEXT ("TAO_IDL_PREPROCESSOR_ARGS"),
                              (char *) 0);

  if (args1 != 0)
    {
      cpp_args = args1;
    }
  else
    {
      // Check for the deprecated TAO_IDL_DEFAULT_CPP_FLAGS environment
      // variable.
      ACE_Env_Value<char*> args2 (ACE_TEXT ("TAO_IDL_DEFAULT_CPP_FLAGS"),
                                  (char *) 0);

      if (args2 != 0)
        {
          ACE_ERROR ((LM_WARNING,
                      "Warning: The environment variable "
                      "TAO_IDL_DEFAULT_CPP_FLAGS has been "
                      "deprecated.\n"
                      "         Please use "
                      "TAO_IDL_PREPROCESSOR_ARGS instead.\n"));

          cpp_args = args2;
        }
    }

  return cpp_args;
}

