// -*- C++ -*-
// $Id$

#include "PSDL_Root_Scope.h"
#include "PSDL_Interface_Scope.h"



TAO_PSDL_Root_Scope::TAO_PSDL_Root_Scope ()
  : root_scope_map_ ()
{
}

TAO_PSDL_Root_Scope::~TAO_PSDL_Root_Scope ()
{
}

int
TAO_PSDL_Root_Scope::add_module (ACE_CString identifier)
{
  return TAO_PSDL_Scope::instance ()->add_module_to_scope (identifier,
                                                           this);
}

int
TAO_PSDL_Root_Scope::add_interface (ACE_CString interface_name)
{
  // Add the actual interface.
  return TAO_PSDL_Scope::instance ()->add_interface_to_scope (interface_name,
                                                              this);
}

int
TAO_PSDL_Root_Scope::add_struct (ACE_CString struct_name)
{
  return TAO_PSDL_Scope::instance ()->add_struct_to_scope (struct_name,
                                                           this);
}

int
TAO_PSDL_Root_Scope::add_typedef (ACE_CString identifier,
                                  ACE_CString identifier_type)
{
  return TAO_PSDL_Scope::instance ()->add_typedef_to_scope (identifier,
                                                            identifier_type,
                                                            this);
}

int
TAO_PSDL_Root_Scope::add_const_decl (ACE_CString identifier,
                                     ACE_CString identifier_type)
{
  return TAO_PSDL_Scope::instance ()->add_const_decl_to_scope (identifier,
                                                               identifier_type,
                                                               this);
}

int
TAO_PSDL_Root_Scope::add_except_decl (ACE_CString identifier,
                                      ACE_CString identifier_type)
{
  return TAO_PSDL_Scope::instance ()->add_except_decl_to_scope (identifier,
                                                                identifier_type,
                                                                this);
}

int
TAO_PSDL_Root_Scope::add_op_dcl (ACE_CString identifier)
{
  return TAO_PSDL_Scope::instance ()->add_op_dcl_to_scope (identifier,
                                                           this);
}

void
TAO_PSDL_Root_Scope::dump (CORBA::ULong depth)
{
  ACE_DEBUG ((LM_DEBUG,
              "ROOT_SCOPE"));

  depth = depth + 5;
  for (Scope_Map_Iterator i = this->root_scope_map_.begin ();
       i != this->root_scope_map_.end ();
       ++i)
    {
      TAO_PSDL_Scope::instance ()->print_depth (depth);
      ACE_DEBUG ((LM_DEBUG,
                  "%s (module)\n",
                  (*i).ext_id_.c_str ()));
      (*i).int_id_->dump (depth);
    }

}

int
TAO_PSDL_Root_Scope::find (const ACE_CString &identifier_name,
                           ACE_CString &identifier_type)
{
  for (Scope_Map_Iterator i = this->root_scope_map_.begin ();
       i != this->root_scope_map_.end ();
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
TAO_PSDL_Root_Scope::find (const ACE_CString &identifier_name)
{
  for (Scope_Map_Iterator i = this->root_scope_map_.begin ();
       i != this->root_scope_map_.end ();
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
              return 0;
            }
        }
    }

  return -1;
}

int
TAO_PSDL_Root_Scope::get_module_name (const ACE_CString &identifier_name,
                                      ACE_CString &module_name)
{
  for (Scope_Map_Iterator i = this->root_scope_map_.begin ();
       i != this->root_scope_map_.end ();
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
TAO_PSDL_Root_Scope::get_interface_name (const ACE_CString &identifier_name,
                                         ACE_CString &interface_name)
{
  for (Scope_Map_Iterator i = this->root_scope_map_.begin ();
       i != this->root_scope_map_.end ();
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
TAO_PSDL_Root_Scope::parent_scope (void)
{
  return 0;
}

Scope_Map *
TAO_PSDL_Root_Scope::scope_map (void)
{
  return &this->root_scope_map_;
}
