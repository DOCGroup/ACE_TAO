// -*- C++ -*-
// $Id$

#include "PSDL_Exception_Scope.h"




TAO_PSDL_Exception_Scope::TAO_PSDL_Exception_Scope (TAO_PSDL_Scope *parent_scope)
  : scope_map_ (),
    parent_scope_ (parent_scope)
{
  // Constructor.
}

int
TAO_PSDL_Exception_Scope::add_member_decl (ACE_CString identifier,
                                           ACE_CString identifier_type)
{
  // Add member declarations to this scope.
  return TAO_PSDL_Scope::instance ()->add_member_decl_to_scope (identifier,
                                                                identifier_type,
                                                                this);
}

void
TAO_PSDL_Exception_Scope::dump (CORBA::ULong depth)
{
  depth = depth + 5;

  for (Scope_Map_Iterator i = this->scope_map_.begin ();
       i != this->scope_map_.end ();
       ++i)
    {
      this->print_depth (depth);
      ACE_DEBUG ((LM_DEBUG,
                  "%s (%s)\n",
                  (*i).ext_id_.c_str (), (*i).int_id_));
    }
}

int
TAO_PSDL_Exception_Scope::find (const ACE_CString &identifier_name,
                             ACE_CString &identifier_type)
{
  for (Scope_Map_Iterator i = this->scope_map_.begin ();
       i != this->scope_map_.end ();
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
TAO_PSDL_Exception_Scope::find (const ACE_CString &identifier_name)
{
  for (Scope_Map_Iterator i = this->scope_map_.begin ();
       i != this->scope_map_.end ();
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

TAO_PSDL_Scope *
TAO_PSDL_Exception_Scope::parent_scope (void)
{
  return this->parent_scope_;
}

Scope_Map *
TAO_PSDL_Exception_Scope::scope_map (void)
{
  return &this->scope_map_;
}

ACE_CString
TAO_PSDL_Exception_Scope::identifier_type (void)
{
  return this->identifier_type_;
}

ACE_CString
TAO_PSDL_Exception_Scope::module_name (void)
{
  return this->module_name_;
}

ACE_CString
TAO_PSDL_Exception_Scope::interface_name (void)
{
  return this->interface_name_;
}
