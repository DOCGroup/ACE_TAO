// -*- C++ -*-
// $Id$

#include "PSDL_Scope.h"
#include "PSDL_Root_Scope.h"
#include "PSDL_Simple_Scope.h"
#include "PSDL_Module_Scope.h"
#include "PSDL_Interface_Scope.h"
#include "PSDL_Struct_Scope.h"
#include "PSDL_Exception_Scope.h"
#include "PSDL_Op_Dcl_Scope.h"
#include "PSDL_Stream.h"
#include "ace/OS_NS_ctype.h"



#undef INCREMENT
#define INCREMENT 40

TAO_PSDL_Scope::TAO_PSDL_Scope (void)
  : psdl_scope_ (new TAO_PSDL_Scope *[INCREMENT]),
    ast_scope_ (0),
    module_names_ (0),
    interface_names_ (0),
    psdl_scope_top_ (0),
    root_scope_ (0),
    identifier_ (),
    name_space_ (),
    interface_name_ (),
    ps_sh_ (0),
    ps_si_ (0),
    ps_sin_ (0)
{
}

TAO_PSDL_Scope::~TAO_PSDL_Scope (void)
{
}

void
TAO_PSDL_Scope::to_lower_case (ACE_CString &identifier)
{
  size_t len = identifier.length ();
  for (size_t i = 0; i <= len; ++i)
    {
      identifier[i] = ACE_OS::ace_tolower (identifier[i]);
    }
}

int
TAO_PSDL_Scope::check_identifier (ACE_CString identifier,
                                  TAO_PSDL_Scope *scope)
{
  int result = 0;

  this->to_lower_case (identifier);

  // Check in the ROOT_SCOPE
  if (scope->scope_map () != 0)
    result = scope->scope_map ()->find (identifier);

  if (result != 0)
    {
      // Didnt find in this scope. Check in the parents scope
      // since that also counts.
      if (scope->parent_scope () != 0)
        {
          result =
            this->check_identifier (identifier,
                                    scope->parent_scope ());
        }
    }

  return result;
}

void
TAO_PSDL_Scope::check_name_in_scope (ACE_CString identifier,
                                     TAO_PSDL_Scope *scope)
{
  // This method is to be used in the cases when the identifier should
  // have been declared before: for example, forward declarations.

  // Check if it is a user-defined type defined in this scope.
  int check_result = this->check_identifier (identifier,
                                             scope);

  if (check_result == -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Identifier %s not defined before in the scope..aborting..\n",
                  identifier.c_str ()));

      ACE_OS::exit (1);
    }
}

int
TAO_PSDL_Scope::add_module (ACE_CString)
{
  return 0;
}

int
TAO_PSDL_Scope::add_module_to_scope (ACE_CString identifier,
                                     TAO_PSDL_Scope *scope)
{
  ACE_CString lower_identifier = identifier;
  this->to_lower_case (lower_identifier);

  if (this->check_identifier (lower_identifier, scope) == -1)
    {
      TAO_PSDL_Module_Scope *psdl_module =
        new TAO_PSDL_Module_Scope (scope);

      scope->scope_map ()->bind (lower_identifier,
                                 psdl_module);

      size_t cur_size = this->module_names_.size ();

      /// @@ Rather than here .. you must have actually save the
      /// identifier name in the instance of the module itself .. or
      /// may be I should rather figure out a
      this->module_names_.size (cur_size + 1);
      this->module_names_[cur_size] = identifier;

      TAO_PSDL_Scope::instance ()->push_scope (psdl_module);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Error: module name re-defined: ABORTING\n"));
      return -1;
    }

  return 0;
}

int
TAO_PSDL_Scope::add_interface_dcl_to_scope (ACE_CString identifier,
                                            TAO_PSDL_Scope *scope)
{
  this->to_lower_case (identifier);

  if (this->check_identifier (identifier, scope) == -1)
    {
      TAO_PSDL_Simple_Scope *psdl_simple =
        new TAO_PSDL_Simple_Scope (scope,
                                   "forward_dcl");

      scope->scope_map ()->bind (identifier,
                                 psdl_simple);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Error: interface_name %s re-defined\n",
                  identifier.c_str ()));

      ACE_OS::exit (1);
      return -1;
    }

  return 0;
}

int
TAO_PSDL_Scope::add_interface (ACE_CString)
{
  return 0;
}

int
TAO_PSDL_Scope::add_interface_to_scope (ACE_CString identifier,
                                        TAO_PSDL_Scope *scope)
{
  this->to_lower_case (identifier);

  if (this->check_identifier (identifier, scope) == -1)
    {
      TAO_PSDL_Interface_Scope *psdl_interface =
        new TAO_PSDL_Interface_Scope (scope);

      scope->scope_map ()->bind (identifier,
                                  psdl_interface);

      TAO_PSDL_Scope::instance ()->push_scope (psdl_interface);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Error: interface_name %s re-defined\n",
                  identifier.c_str ()));

      ACE_OS::exit (1);
      return -1;
    }

  return 0;
}

int
TAO_PSDL_Scope::add_struct (ACE_CString)
{
  return 0;
}

int
TAO_PSDL_Scope::add_struct_to_scope (ACE_CString struct_name,
                                     TAO_PSDL_Scope *scope)
{
  this->to_lower_case (struct_name);

  if (this->check_identifier (struct_name, scope) == -1)
    {
      TAO_PSDL_Struct_Scope *psdl_struct =
        new TAO_PSDL_Struct_Scope (scope);

      scope->scope_map ()->bind (struct_name,
                                 psdl_struct);

      TAO_PSDL_Scope::instance ()->push_scope (psdl_struct);

    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Error: identifier re-defined: ABORTING\n"));

      ACE_OS::exit (1);

      return -1;
    }

  return 0;
}

int
TAO_PSDL_Scope::add_typedef (ACE_CString,
                             ACE_CString)
{
  return 0;
}

int
TAO_PSDL_Scope::add_typedef_to_scope (ACE_CString identifier,
                                      ACE_CString identifier_type,
                                      TAO_PSDL_Scope *scope)
{
  this->to_lower_case (identifier);
  this->to_lower_case (identifier_type);

  if (this->check_identifier (identifier, scope) == -1)
    {
      TAO_PSDL_Simple_Scope *psdl_simple =
        new TAO_PSDL_Simple_Scope (scope,
                                   identifier_type);

      scope->scope_map ()->bind (identifier,
                                 psdl_simple);
    }
  else
    {
      ACE_DEBUG ((LM_ERROR,
                  "Error: trying to redefine typedef %s\n",
                  identifier.c_str ()));

      ACE_OS::exit (1);

      return -1;
    }

  return 0;
}

int
TAO_PSDL_Scope::add_const_decl (ACE_CString,
                                ACE_CString)
{
  return 0;
}

int
TAO_PSDL_Scope::add_const_decl_to_scope (ACE_CString identifier,
                                         ACE_CString identifier_type,
                                         TAO_PSDL_Scope *scope)
{
  this->to_lower_case (identifier);
  this->to_lower_case (identifier_type);

  // First check if the identifier_type is either a predefined type
  // or if it is defined before. If it is defined before, we will
  // proceed.
  this->check_name_in_scope (identifier_type, scope);

  if (this->check_identifier (identifier, scope) == -1)
    {
      TAO_PSDL_Simple_Scope *psdl_simple =
            new TAO_PSDL_Simple_Scope (scope,
                                       identifier_type);

      scope->scope_map ()->bind (identifier,
                                 psdl_simple);

    }
  else
    {
      ACE_DEBUG ((LM_ERROR,
                  "Error: const_decl re-defined %s\n",
                  identifier.c_str ()));

      ACE_OS::exit (1);

      return -1;
    }

  return 0;
}

int
TAO_PSDL_Scope::add_except_decl (ACE_CString,
                                ACE_CString)
{
  return 0;
}

int
TAO_PSDL_Scope::add_except_decl_to_scope (ACE_CString identifier,
                                          ACE_CString identifier_type,
                                          TAO_PSDL_Scope *scope)
{
  this->to_lower_case (identifier);
  this->to_lower_case (identifier_type);

  if (this->check_identifier (identifier, scope) == -1)
    {
      TAO_PSDL_Simple_Scope *psdl_simple =
        new TAO_PSDL_Simple_Scope (scope,
                                   identifier_type);

      scope->scope_map ()->bind (identifier,
                                 psdl_simple);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Error: exception_name %s re-defined\n",
                  identifier.c_str ()));

      ACE_OS::exit (1);

      return -1;
    }

  return 0;
}

int
TAO_PSDL_Scope::add_op_dcl (ACE_CString)
{
  return 0;
}

int
TAO_PSDL_Scope::add_op_dcl_to_scope (ACE_CString op_name,
                                     TAO_PSDL_Scope *scope)
{
  this->to_lower_case (op_name);

  if (this->check_identifier (op_name, scope) == -1)
    {
      TAO_PSDL_Op_Dcl_Scope *psdl_op_dcl =
        new TAO_PSDL_Op_Dcl_Scope (scope);

      scope->scope_map ()->bind (op_name,
                                  psdl_op_dcl);

      TAO_PSDL_Scope::instance ()->push_scope (psdl_op_dcl);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Error: op_name %s re-defined\n",
                  op_name.c_str ()));

      ACE_OS::exit (1);

      return -1;
    }

  return 0;
}

int
TAO_PSDL_Scope::add_member_decl (ACE_CString,
                                 ACE_CString)
{
  return 0;
}

int
TAO_PSDL_Scope::add_member_decl_to_scope (ACE_CString identifier,
                                          ACE_CString identifier_type,
                                          TAO_PSDL_Scope *scope)
{
  this->to_lower_case (identifier);
  this->to_lower_case (identifier_type);

  if (this->check_identifier (identifier, scope) == -1)
    {
      TAO_PSDL_Simple_Scope *psdl_simple =
        new TAO_PSDL_Simple_Scope (scope,
                                   identifier_type);

      scope->scope_map ()->bind (identifier,
                                 psdl_simple);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Error: member_decl %s re-defined\n",
                  identifier.c_str ()));

      ACE_OS::exit (1);

      return -1;
    }

  return 0;
}

int
TAO_PSDL_Scope::add_exception (ACE_CString)
{
  return 0;
}

int
TAO_PSDL_Scope::add_exception_to_scope (ACE_CString identifier,
                                        TAO_PSDL_Scope *scope)
{
  this->to_lower_case (identifier);

  if (this->check_identifier (identifier, scope) == -1)
    {
      TAO_PSDL_Exception_Scope *psdl_exception =
        new TAO_PSDL_Exception_Scope (scope);

      scope->scope_map ()->bind (identifier,
                                  psdl_exception);

      TAO_PSDL_Scope::instance ()->push_scope (psdl_exception);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Error: exception %s re-defined: aborting\n",
                  identifier.c_str ()));

      ACE_OS::exit (1);

      return -1;
    }

  return 0;
}

int
TAO_PSDL_Scope::add_scoped_decl_to_scope (ACE_CString identifier,
                                          ACE_CString identifier_type,
                                          TAO_PSDL_Scope *scope)
{
  this->to_lower_case (identifier);
  this->to_lower_case (identifier_type);

  TAO_PSDL_Simple_Scope *psdl_simple =
    new TAO_PSDL_Simple_Scope (scope,
                               identifier_type);

  scope->scope_map ()->bind (identifier,
                             psdl_simple);
  return 0;
}

int
TAO_PSDL_Scope::add_enum_decl_to_scope (ACE_CString identifier,
                                        ACE_CString identifier_type,
                                        TAO_PSDL_Scope *scope)
{
  this->to_lower_case (identifier);
  this->to_lower_case (identifier_type);

  TAO_PSDL_Simple_Scope *psdl_simple =
    new TAO_PSDL_Simple_Scope (scope,
                               identifier_type);

  scope->scope_map ()->bind (identifier,
                             psdl_simple);
  return 0;
}

void
TAO_PSDL_Scope::set_root_scope ()
{
  // This method is called to push the root_scope the first time.
  if (this->root_scope_ == 0)
    {
      this->root_scope_ = new TAO_PSDL_Root_Scope ();
      TAO_PSDL_Scope::instance ()->push_scope (this->root_scope_);
    }
}

void
TAO_PSDL_Scope::set_stub_prefix (const char *filename)
{
  this->stub_prefix_ = filename;
}

const ACE_CString &
TAO_PSDL_Scope::get_stub_prefix (void)
{
  return this->stub_prefix_;
}

TAO_PSDL_Stream *
TAO_PSDL_Scope::get_sh (void)
{
  if (this->ps_sh_ == 0)
    {
      ACE_NEW_RETURN (this->ps_sh_,
                      TAO_PSDL_Stream,
                      0);

      ACE_CString file_name =
        TAO_PSDL_Scope::instance ()->get_stub_prefix () + "C.h";

      this->ps_sh_->open (file_name.c_str ());

      this->header_initialization (this->ps_sh_);
    }

  return this->ps_sh_;
}

TAO_PSDL_Stream *
TAO_PSDL_Scope::get_si (void)
{
  if (this->ps_si_ == 0)
    {
      ACE_NEW_RETURN (this->ps_si_,
                      TAO_PSDL_Stream,
                      0);

      ACE_CString file_name =
        TAO_PSDL_Scope::instance ()->get_stub_prefix () + "C.cpp";

      this->ps_si_->open (file_name.c_str ());

      this->stub_initialization (this->ps_si_);
    }

  return this->ps_si_;
}

TAO_PSDL_Stream *
TAO_PSDL_Scope::get_sinline (void)
{
  if (this->ps_sin_ == 0)
    {
      ACE_NEW_RETURN (this->ps_sin_,
                      TAO_PSDL_Stream,
                      0);

      ACE_CString file_name =
        TAO_PSDL_Scope::instance ()->get_stub_prefix () + "C.i";

      this->ps_sin_->open (file_name.c_str ());
    }

  return this->ps_sin_;
}

void
TAO_PSDL_Scope::header_initialization (TAO_PSDL_Stream *ps_sh)
{
  ps_sh->reset ();

  *ps_sh << "#ifndef _TAO_PSDL_" << this->get_stub_prefix () << "C_H_";
  ps_sh->nl ();

  *ps_sh << "#define _TAO_PSDL_" << this->get_stub_prefix () << "C_H_";
  ps_sh->nl ();

  *ps_sh << "#include \"tao/corba.h\"";
  ps_sh->nl ();

  *ps_sh << "#if !defined (ACE_LACKS_PRAGMA_ONCE)";
  ps_sh->nl ();
  *ps_sh << "# pragma once";
  ps_sh->nl ();
  *ps_sh << "#endif /* ACE_LACKS_PRAGMA_ONCE */";
  ps_sh->nl ();

  ps_sh->nl ();

  *ps_sh << "#if defined (TAO_EXPORT_MACRO)";
  ps_sh->nl ();
  *ps_sh << "#undef TAO_EXPORT_MACRO";
  ps_sh->nl ();
  *ps_sh << "#endif";
  ps_sh->nl ();
  *ps_sh << "#define TAO_EXPORT_MACRO";
  ps_sh->nl ();

  ps_sh->nl ();

  *ps_sh << "#if defined (TAO_EXPORT_NESTED_CLASSES)";
  ps_sh->nl ();
  *ps_sh << "#  if defined (TAO_EXPORT_NESTED_MACRO)";
  ps_sh->nl ();
  *ps_sh << "#    undef TAO_EXPORT_NESTED_MACRO";
  ps_sh->nl ();
  *ps_sh << "#  endif /* defined (TAO_EXPORT_NESTED_MACRO) */";
  ps_sh->nl ();
  *ps_sh << "#  define TAO_EXPORT_NESTED_MACRO";
  ps_sh->nl ();
  *ps_sh << "#endif /* TAO_EXPORT_NESTED_CLASSES */";
  ps_sh->nl ();

  ps_sh->nl ();

  *ps_sh << "#if defined(_MSC_VER)";
  ps_sh->nl ();
  *ps_sh << "#pragma warning(push)";
  ps_sh->nl ();
  *ps_sh << "#pragma warning(disable:4250)";
  ps_sh->nl ();
  *ps_sh << "#endif /* _MSC_VER */";
  ps_sh->nl ();

  ps_sh->nl ();

  ps_sh->nl ();

}

void
TAO_PSDL_Scope::stub_initialization (TAO_PSDL_Stream *ps_si)
{

  ps_si->reset ();

  *ps_si << "#include \"" << this->get_stub_prefix () << "C.h\"";
  ps_si->nl ();
  *ps_si << "#include \"tao/Stub.h\"";
  ps_si->nl ();
  *ps_si << "#include \"tao/PortableInterceptor.h\"";
  ps_si->nl ();

  ps_si->nl ();

  *ps_si << "#if TAO_HAS_INTERCEPTORS == 1";
  ps_si->nl ();
  *ps_si << "#include \"tao/RequestInfo_Util.h\"";
  ps_si->nl ();
  *ps_si << "#include \"tao/ClientRequestInfo_i.h\"";
  ps_si->nl ();
  *ps_si << "#endif  /* TAO_HAS_INTERCEPTORS == 1 */";
  ps_si->nl ();

  ps_si->nl ();

  ps_si->nl ();

  *ps_si << "#if !defined (__ACE_INLINE__)";
  ps_si->nl ();
  *ps_si << "#include \"" << this->get_stub_prefix () << "C.i\"";
  ps_si->nl ();
  *ps_si << "#endif /* !defined INLINE */";
  ps_si->nl ();
  ps_si->nl ();

}

TAO_PSDL_Scope *
TAO_PSDL_Scope::pop_top_scope ()
{
  // Return the top Scope
  if (this->psdl_scope_top_ <= 0)
    return 0;

  return this->psdl_scope_ [this->psdl_scope_top_ - 1];
}

void
TAO_PSDL_Scope::set_module_scope (void)
{
  size_t cur_size = this->module_names_.size ();

  // Resetting the module_names_
  this->module_names_.size (cur_size -1);
  this->set_scope ();
}

void
TAO_PSDL_Scope::set_interface_scope (void)
{
  size_t cur_size = this->interface_names_.size ();

  // Resetting the module_names_
  this->interface_names_.size (cur_size -1);
  this->set_scope ();
}

void
TAO_PSDL_Scope::set_scope (void)
{
  // Remove the top scope. Its no longer needed.
  if (this->psdl_scope_top_ <= 0)
    {
      return;
    }

  --this->psdl_scope_top_;
}

void
TAO_PSDL_Scope::push_scope (TAO_PSDL_Scope *scope)
{
  // Push a new scope.
  ++this->psdl_scope_top_;
  this->psdl_scope_ [this->psdl_scope_top_ - 1] = scope;

  // save for later.
  size_t cur_size = this->ast_scope_.size ();
  this->ast_scope_.size (cur_size + 1);
  this->ast_scope_[cur_size] = scope;
}

void
TAO_PSDL_Scope::save_identifier (ACE_CString identifier)
{
  this->identifier_ = identifier;
}

ACE_CString
TAO_PSDL_Scope::get_identifier (void)
{
  return this->identifier_;
}

void
TAO_PSDL_Scope::dump (CORBA::ULong depth)
{
  this->psdl_scope_[0]->dump (depth);
}

int
TAO_PSDL_Scope::find (const ACE_CString &identifier_name,
                      ACE_CString &identifier_type)
{
  int result = this->root_scope_->find (identifier_name,
                                        identifier_type);

  return result;
}

int
TAO_PSDL_Scope::find (const ACE_CString &identifier_name)
{
  int result = this->root_scope_->find (identifier_name);

  return result;
}

int
TAO_PSDL_Scope::get_module_name (const ACE_CString &identifier_name,
                                 ACE_CString &module_name)
{
  return this->root_scope_->get_module_name (identifier_name,
                                             module_name);
}

int
TAO_PSDL_Scope::get_interface_name (const ACE_CString &identifier_name,
                                    ACE_CString &interface_name)
{
  return this->root_scope_->get_interface_name (identifier_name,
                                                interface_name);
}

ACE_CString
TAO_PSDL_Scope::identifier_type (void)
{
  return "null";
}

ACE_CString
TAO_PSDL_Scope::module_name (void)
{
  return "null";
}

ACE_CString
TAO_PSDL_Scope::interface_name (void)
{
  return "null";
}

ACE_CString
TAO_PSDL_Scope::convert_str (int identifier_type)
{
  switch (identifier_type)
    {
    case 258:
      return "module";
    case 264:
      return "abstract";
    case 288:
      return "octet";
    case 290:
      return "object";
    case 291:
      return "struct";
    case 295:
      return "long";
    case 296:
      return "short";
    case 298:
      return "char";
    case 300:
      return "typedef";
    case 301:
      return "native";
    case 316:
      return "void";
    case 317:
      return "in";
    case 324:
      return "exception";
    case 325:
      return "interface";
    case 326:
      return "local";
    case 332:
      return "char *";
    case 337:
      return "enum";
    case 338:
      return "sequence";
    default:
      return "unknown";
    };
}


void
TAO_PSDL_Scope::print_depth (CORBA::ULong depth)
{
  for (CORBA::ULong i=0; i != depth; ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "-"));
    }

  ACE_DEBUG ((LM_DEBUG,
              ">"));
}

TAO_PSDL_Scope *
TAO_PSDL_Scope::instance (void)
{
  return TAO_Singleton<TAO_PSDL_Scope, TAO_SYNCH_MUTEX>::instance ();
}

void
TAO_PSDL_Scope::set_name_space (ACE_CString name_space)
{
  this->name_space_ = name_space;
}

ACE_CString
TAO_PSDL_Scope::get_name_space (void)
{
  return this->name_space_;
}

void
TAO_PSDL_Scope::set_interface_name (ACE_CString interface_name)
{
  this->interface_name_ = interface_name;
}

ACE_CString
TAO_PSDL_Scope::get_interface_name (void)
{
  return this->interface_name_;
}

TAO_PSDL_Scope *
TAO_PSDL_Scope::parent_scope (void)
{
  return 0;
}

Scope_Map *
TAO_PSDL_Scope::scope_map (void)
{
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template TAO_Singleton<TAO_PSDL_Scope, TAO_SYNCH_MUTEX> *TAO_Singleton<TAO_PSDL_Scope, TAO_SYNCH_MUTEX>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
