/* -*- c++ -*- */
// $Id$

#include "ifr_visitor.h"

ACE_RCSID(IFR_Service, ifr_visitor, "$Id$")

ifr_visitor::ifr_visitor (void)
  : lock_ (0)
{
  int argc = 0;
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      be_global->orb (CORBA::ORB_init (argc,
                                       0,
                                       0,
                                       ACE_TRY_ENV));
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        be_global->orb ()->resolve_initial_references ("InterfaceRepository",
                                                       ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (object.in ()))
        {
          ACE_ERROR ((
              LM_ERROR,
              ACE_TEXT ("Null objref from resolve_initial_references\n")
            ));
        }

      CORBA_Repository_var repo = CORBA_Repository::_narrow (object.in (),
                                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (repo.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("CORBA::Repository::_narrow failed\n")));
        }

      be_global->repository (repo._retn ());

      // Create the appropriate lock.
      if (be_global->enable_locking ())
        {
          ACE_NEW_THROW_EX (this->lock_,
                            ACE_Lock_Adapter<TAO_SYNCH_MUTEX> (),
                            CORBA::NO_MEMORY ());
        }
      else
        {
          ACE_NEW_THROW_EX (this->lock_,
                            ACE_Lock_Adapter<ACE_SYNCH_NULL_MUTEX> (),
                            CORBA::NO_MEMORY ());
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("ifr_visitor"));
    }
  ACE_ENDTRY;
}

ifr_visitor::~ifr_visitor (void)
{
  delete this->lock_;
}

// All the visit methods. Each concrete ifr visitors will 
// override some subset of these.

int 
ifr_visitor::visit_decl (AST_Decl *)
{
  return 0;
}

int 
ifr_visitor::visit_scope (UTL_Scope *)
{
  return 0;
}

int 
ifr_visitor::visit_type (AST_Type *)
{
  return 0;
}

int 
ifr_visitor::visit_predefined_type (AST_PredefinedType *)
{
  return 0;
}

int 
ifr_visitor::visit_module (AST_Module *)
{
  return 0;
}

int 
ifr_visitor::visit_interface (AST_Interface *)
{
  return 0;
}

int 
ifr_visitor::visit_interface_fwd (AST_InterfaceFwd *)
{
  return 0;
}

int 
ifr_visitor::visit_valuetype (AST_Interface *)
{
  return 0;
}

int 
ifr_visitor::visit_valuetype_fwd (AST_InterfaceFwd *)
{
  return 0;
}

int 
ifr_visitor::visit_structure (AST_Structure *)
{
  return 0;
}

int 
ifr_visitor::visit_exception (AST_Exception *)
{
  return 0;
}

int 
ifr_visitor::visit_expression (AST_Expression *)
{
  return 0;
}

int 
ifr_visitor::visit_enum (AST_Enum *)
{
  return 0;
}

int 
ifr_visitor::visit_operation (AST_Operation *)
{
  return 0;
}

int 
ifr_visitor::visit_field (AST_Field *)
{
  return 0;
}

int 
ifr_visitor::visit_argument (AST_Argument *)
{
  return 0;
}

int 
ifr_visitor::visit_attribute (AST_Attribute *)
{
  return 0;
}

int 
ifr_visitor::visit_union (AST_Union *)
{
  return 0;
}

int 
ifr_visitor::visit_union_branch (AST_UnionBranch *)
{
  return 0;
}

int 
ifr_visitor::visit_union_label (AST_UnionLabel *)
{
  return 0;
}

int 
ifr_visitor::visit_constant (AST_Constant *)
{
  return 0;
}

int 
ifr_visitor::visit_enum_val (AST_EnumVal *)
{
  return 0;
}

int 
ifr_visitor::visit_array (AST_Array *)
{
  return 0;
}

int 
ifr_visitor::visit_sequence (AST_Sequence *)
{
  return 0;
}

int 
ifr_visitor::visit_string (AST_String *)
{
  return 0;
}

int 
ifr_visitor::visit_typedef (AST_Typedef *)
{
  return 0;
}

int 
ifr_visitor::visit_root (AST_Root *)
{
  return 0;
}

int 
ifr_visitor::visit_native (AST_Native *)
{
  return 0;
}

ACE_Lock &
ifr_visitor::lock (void) const
{
  return *this->lock_;
}

