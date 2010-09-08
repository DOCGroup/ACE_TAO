// -*- C++ -*-
// $Id$

#include "PSDL_Module_Scope.h"
#include "PSDL_Op_Dcl_Scope.h"



TAO_PSDL_Op_Dcl_Scope::TAO_PSDL_Op_Dcl_Scope (TAO_PSDL_Scope *parent_scope)
  : scope_map_ (),
    parent_scope_ (parent_scope)
{
}

int
TAO_PSDL_Op_Dcl_Scope::add_const_decl (ACE_CString identifier,
                                       ACE_CString identifier_type)
{
  return TAO_PSDL_Scope::instance ()->add_const_decl_to_scope (identifier,
                                                               identifier_type,
                                                               this);
}

int
TAO_PSDL_Op_Dcl_Scope::add_member_decl (ACE_CString identifier,
                                        ACE_CString identifier_type)
{
  return TAO_PSDL_Scope::instance ()->add_member_decl_to_scope (identifier,
                                                                identifier_type,
                                                                this);
}

void
TAO_PSDL_Op_Dcl_Scope::dump (CORBA::ULong depth)
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

TAO_PSDL_Scope *
TAO_PSDL_Op_Dcl_Scope::parent_scope (void)
{
  return this->parent_scope_;
}

Scope_Map *
TAO_PSDL_Op_Dcl_Scope::scope_map (void)
{
  return &this->scope_map_;
}
