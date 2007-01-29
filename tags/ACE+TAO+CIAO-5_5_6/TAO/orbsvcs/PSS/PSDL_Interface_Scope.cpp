// -*- C++ -*-
// $Id$

#include "PSDL_Module_Scope.h"
#include "PSDL_Interface_Scope.h"

ACE_RCSID (PSS, PSDL_Interface_Scope, "$Id$")

TAO_PSDL_Interface_Scope::TAO_PSDL_Interface_Scope (TAO_PSDL_Scope *parent_scope)
  : interface_scope_ (),
    parent_scope_ (parent_scope)
{
  // Constructor
}

TAO_PSDL_Interface_Scope::~TAO_PSDL_Interface_Scope (void)
{
  // destructor
}

int
TAO_PSDL_Interface_Scope::add_interface (ACE_CString interface_name)
{
  return TAO_PSDL_Scope::instance ()->add_interface_to_scope (interface_name,
                                                              this);
}

int
TAO_PSDL_Interface_Scope::add_struct (ACE_CString struct_name)
{
  return TAO_PSDL_Scope::instance ()->add_struct_to_scope (struct_name,
                                                           this);
}

int
TAO_PSDL_Interface_Scope::add_typedef (ACE_CString identifier,
                                       ACE_CString identifier_type)
{
  return TAO_PSDL_Scope::instance ()->add_typedef_to_scope (identifier,
                                                            identifier_type,
                                                            this);
}

int
TAO_PSDL_Interface_Scope::add_const_decl (ACE_CString identifier,
                                       ACE_CString identifier_type)
{
  return TAO_PSDL_Scope::instance ()->add_const_decl_to_scope (identifier,
                                                               identifier_type,
                                                               this);
}

int
TAO_PSDL_Interface_Scope::add_except_decl (ACE_CString identifier,
                                        ACE_CString identifier_type)
{
  return TAO_PSDL_Scope::instance ()->add_except_decl_to_scope (identifier,
                                                                identifier_type,
                                                                this);
}

int
TAO_PSDL_Interface_Scope::add_exception (ACE_CString identifier)
{
  return TAO_PSDL_Scope::instance ()->add_exception_to_scope (identifier,
                                                              this);
}

int
TAO_PSDL_Interface_Scope::add_op_dcl (ACE_CString identifier)
{
  return TAO_PSDL_Scope::instance ()->add_op_dcl_to_scope (identifier,
                                                           this);
}

void
TAO_PSDL_Interface_Scope::dump (CORBA::ULong depth)
{
  depth = depth + 5;

  for (Scope_Map_Iterator i = this->interface_scope_.begin ();
       i != this->interface_scope_.end ();
       ++i)
    {
      this->print_depth (depth);
      ACE_DEBUG ((LM_DEBUG,
                  "%s (interface/exception/struct)\n",
                  (*i).ext_id_.c_str ()));
      (*i).int_id_->dump (depth);
    }
}

int
TAO_PSDL_Interface_Scope::find (const ACE_CString &identifier_name,
                                ACE_CString &identifier_type)
{
  for (Scope_Map_Iterator i = this->interface_scope_.begin ();
       i != this->interface_scope_.end ();
       ++i)
    {
      if (ACE_OS::strcmp (identifier_name.c_str (), (*i).ext_id_.c_str ()) == 0)
        {
          identifier_type = (*i).int_id_->identifier_type ();
          return 0;
        }
      else
        {
          if ((*i).int_id_->find (identifier_name,
                                  identifier_type) == 0)
            {
              return 0;
            }
        }
    }

  return -1;
}

int
TAO_PSDL_Interface_Scope::find (const ACE_CString &identifier_name)
{
  for (Scope_Map_Iterator i = this->interface_scope_.begin ();
       i != this->interface_scope_.end ();
       ++i)
    {
      if (ACE_OS::strcmp (identifier_name.c_str (), (*i).ext_id_.c_str ()) == 0)
        {
          return 0;
        }
      else
        {
          if ((*i).int_id_->find (identifier_name) == 0)
            {
              // #@@@@ deal
              //      identifier_type = identifier_info->type;
              return 0;
            }
        }
    }

  return -1;
}

int
TAO_PSDL_Interface_Scope::get_module_name (const ACE_CString &identifier_name,
                                           ACE_CString &module_name)
{
  for (Scope_Map_Iterator i = this->interface_scope_.begin ();
       i != this->interface_scope_.end ();
       ++i)
    {
      if (ACE_OS::strcmp (identifier_name.c_str (), (*i).ext_id_.c_str ()) == 0)
        {
          module_name = (*i).int_id_->module_name ();
          return 0;
        }
      else
        {
          if ((*i).int_id_->get_module_name (identifier_name,
                                  module_name) == 0)
            {
              return 0;
            }
        }
    }

  return -1;
}

int
TAO_PSDL_Interface_Scope::get_interface_name (const ACE_CString &identifier_name,
                                              ACE_CString &interface_name)
{
  for (Scope_Map_Iterator i = this->interface_scope_.begin ();
       i != this->interface_scope_.end ();
       ++i)
    {
      if (ACE_OS::strcmp (identifier_name.c_str (), (*i).ext_id_.c_str ()) == 0)
        {
          interface_name = (*i).int_id_->interface_name ();
          return 0;
        }
      else
        {
          if ((*i).int_id_->get_interface_name (identifier_name,
                                                interface_name) == 0)
            {
              return 0;
            }
        }
    }

  return -1;
}

TAO_PSDL_Scope *
TAO_PSDL_Interface_Scope::parent_scope (void)
{
  return this->parent_scope_;
}

Scope_Map *
TAO_PSDL_Interface_Scope::scope_map (void)
{
  return &this->interface_scope_;
}

ACE_CString
TAO_PSDL_Interface_Scope::identifier_type (void)
{
  return this->identifier_type_;
}

ACE_CString
TAO_PSDL_Interface_Scope::module_name (void)
{
  return this->module_name_;
}

ACE_CString
TAO_PSDL_Interface_Scope::interface_name (void)
{
  return this->interface_name_;
}
