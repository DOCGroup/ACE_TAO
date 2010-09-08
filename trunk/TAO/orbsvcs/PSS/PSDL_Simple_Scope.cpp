// -*- C++ -*-
// $Id$

#include "PSDL_Simple_Scope.h"
#include "PSDL_Interface_Scope.h"



TAO_PSDL_Simple_Scope::TAO_PSDL_Simple_Scope (TAO_PSDL_Scope *parent_scope,
                                              ACE_CString identifier_type)
  : scope_map_ (),
    parent_scope_ (parent_scope),
    identifier_type_ (identifier_type)
{
}

TAO_PSDL_Simple_Scope::~TAO_PSDL_Simple_Scope (void)
{
}

int
TAO_PSDL_Simple_Scope::add_typedef (ACE_CString identifier,
                                    ACE_CString identifier_type)
{
  return TAO_PSDL_Scope::instance ()->add_typedef_to_scope (identifier,
                                                            identifier_type,
                                                            this);
}

int
TAO_PSDL_Simple_Scope::add_const_decl (ACE_CString identifier,
                                       ACE_CString identifier_type)
{
  return TAO_PSDL_Scope::instance ()->add_const_decl_to_scope (identifier,
                                                               identifier_type,
                                                               this);
}

int
TAO_PSDL_Simple_Scope::add_except_decl (ACE_CString identifier,
                                        ACE_CString identifier_type)
{
  return TAO_PSDL_Scope::instance ()->add_except_decl_to_scope (identifier,
                                                                identifier_type,
                                                                this);
}

int
TAO_PSDL_Simple_Scope::add_enum_decl (ACE_CString identifier,
                                      ACE_CString identifier_type)
{
  return TAO_PSDL_Scope::instance ()->add_enum_decl_to_scope (identifier,
                                                              identifier_type,
                                                              this);
}

int
TAO_PSDL_Simple_Scope::add_op_dcl (ACE_CString identifier)
{
  return TAO_PSDL_Scope::instance ()->add_op_dcl_to_scope (identifier,
                                                           this);
}

void
TAO_PSDL_Simple_Scope::dump (CORBA::ULong depth)
{
  depth = depth + 5;

  for (Scope_Map_Iterator i = this->scope_map_.begin ();
       i != this->scope_map_.end ();
       ++i)
    {
      this->print_depth (depth);
      ACE_DEBUG ((LM_DEBUG,
                  "%s (simple types)",
                  (*i).ext_id_.c_str ()));
      (*i).int_id_->dump (depth);
    }
}

int
TAO_PSDL_Simple_Scope::find (const ACE_CString &identifier_name,
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
TAO_PSDL_Simple_Scope::find (const ACE_CString &identifier_name)
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

ACE_CString
TAO_PSDL_Simple_Scope::identifier_type (void)
{
  return this->identifier_type_;
}

ACE_CString
TAO_PSDL_Simple_Scope::module_name (void)
{
  return this->module_name_;
}

ACE_CString
TAO_PSDL_Simple_Scope::interface_name (void)
{
  return this->interface_name_;
}

TAO_PSDL_Scope *
TAO_PSDL_Simple_Scope::parent_scope (void)
{
  return this->parent_scope_;
}

Scope_Map *
TAO_PSDL_Simple_Scope::scope_map (void)
{
  return &this->scope_map_;
}
