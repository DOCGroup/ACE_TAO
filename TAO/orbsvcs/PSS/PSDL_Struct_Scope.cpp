// -*- C++ -*-
// $Id$

#include "PSDL_Struct_Scope.h"



TAO_PSDL_Struct_Scope::TAO_PSDL_Struct_Scope (TAO_PSDL_Scope *parent_scope)
  : struct_scope_ (),
    parent_scope_ (parent_scope),
    identifier_type_ ("struct")
{
}

int
TAO_PSDL_Struct_Scope::add_module (ACE_CString identifier)
{
  return TAO_PSDL_Scope::instance ()->add_module_to_scope (identifier,
                                                           this);
}

int
TAO_PSDL_Struct_Scope::add_struct (ACE_CString struct_name)
{
  return TAO_PSDL_Scope::instance ()->add_struct_to_scope (struct_name,
                                                           this);
}

int
TAO_PSDL_Struct_Scope::add_typedef (ACE_CString identifier,
                                    ACE_CString identifier_type)
{
  return TAO_PSDL_Scope::instance ()->add_typedef_to_scope (identifier,
                                                            identifier_type,
                                                            this);
}

int
TAO_PSDL_Struct_Scope::add_const_decl (ACE_CString identifier,
                                       ACE_CString identifier_type)
{
  return TAO_PSDL_Scope::instance ()->add_const_decl_to_scope (identifier,
                                                               identifier_type,
                                                               this);
}

int
TAO_PSDL_Struct_Scope::add_except_decl (ACE_CString identifier,
                                        ACE_CString identifier_type)
{
  return TAO_PSDL_Scope::instance ()->add_except_decl_to_scope (identifier,
                                                                identifier_type,
                                                                this);
}

void
TAO_PSDL_Struct_Scope::dump (CORBA::ULong depth)
{
  depth = depth + 5;

  for (Scope_Map_Iterator i = this->struct_scope_.begin ();
       i != this->struct_scope_.end ();
       ++i)
    {
      this->print_depth (depth);
      ACE_DEBUG ((LM_DEBUG,
                  "%s (struct)\n",
                  (*i).ext_id_.c_str ()));
      (*i).int_id_->dump (depth);
    }
}

int
TAO_PSDL_Struct_Scope::find (const ACE_CString &identifier_name,
                             ACE_CString &identifier_type)
{
  for (Scope_Map_Iterator i = this->struct_scope_.begin ();
       i != this->struct_scope_.end ();
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
TAO_PSDL_Struct_Scope::find (const ACE_CString &identifier_name)
{
  for (Scope_Map_Iterator i = this->struct_scope_.begin ();
       i != this->struct_scope_.end ();
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
TAO_PSDL_Struct_Scope::parent_scope (void)
{
  return this->parent_scope_;
}

Scope_Map *
TAO_PSDL_Struct_Scope::scope_map (void)
{
  return &this->struct_scope_;
}

ACE_CString
TAO_PSDL_Struct_Scope::identifier_type (void)
{
  return this->identifier_type_;
}

ACE_CString
TAO_PSDL_Struct_Scope::module_name (void)
{
  return this->module_name_;
}

ACE_CString
TAO_PSDL_Struct_Scope::interface_name (void)
{
  return this->interface_name_;
}
