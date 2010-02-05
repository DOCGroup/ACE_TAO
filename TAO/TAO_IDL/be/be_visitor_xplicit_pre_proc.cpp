/**
 *  @file   be_visitor_xplicit_pre_proc.cpp
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "be_visitor_xplicit_pre_proc.h"
#include "be_home.h"
#include "be_component.h"
#include "be_interface.h"
#include "be_operation.h"
#include "be_argument.h"
#include "be_finder.h"
#include "be_exception.h"
#include "be_field.h"

#include "ast_module.h"

#include "utl_namelist.h"

#include "fe_interface_header.h"
#include "nr_extern.h"
#include "global_extern.h"

be_visitor_xplicit_pre_proc::be_visitor_xplicit_pre_proc (
      be_visitor_context *ctx)
  : be_visitor_ccm_pre_proc (ctx),
    xplicit_ (0)
{
}

be_visitor_xplicit_pre_proc::~be_visitor_xplicit_pre_proc (void)
{
}

int
be_visitor_xplicit_pre_proc::visit_home (be_home *node)
{
  UTL_NameList *parent_list = this->compute_inheritance (node);

  FE_InterfaceHeader header (0,
                             parent_list,
                             false,
                             false,
                             true);

  // We're at global scope here so we need to fool the scope stack
  // for a minute so the correct repo id can be calculated at
  // interface construction time.
  AST_Module *m =
    AST_Module::narrow_from_scope (node->defined_in ());

  idl_global->scopes ().push (m);

  UTL_ScopedName *explicit_name =
    this->create_scoped_name ("",
                              node->local_name (),
                              "Explicit",
                              m);

  be_interface *i = 0;
  ACE_NEW_RETURN (i,
                  be_interface (explicit_name,
                                header.inherits (),
                                header.n_inherits (),
                                header.inherits_flat (),
                                header.n_inherits_flat (),
                                false,
                                false),
                  0);

  (void) m->be_add_interface (i);

  i->original_interface (node);

  idl_global->scopes ().push (i);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_home - code generation ")
                         ACE_TEXT ("for home scope failed\n")),
                        0);
    }

  // Through with the explicit interface scope
  idl_global->scopes ().pop ();

  explicit_name->destroy ();
  delete explicit_name;
  explicit_name = 0;

  header.destroy ();

  parent_list->destroy ();
  delete parent_list;
  parent_list = 0;

  // Through with the scope containing the home.
  idl_global->scopes ().pop ();

  this->xplicit_ = i;

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_operation (be_operation *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  be_operation *home_op = 0;
  ACE_NEW_RETURN (home_op,
                  be_operation (node->return_type (),
                                node->flags (),
                                &sn,
                                false,
                                false),
                  -1);

  home_op->be_add_exceptions (node->exceptions ());

  idl_global->scopes ().top ()->add_to_scope (home_op);
  idl_global->scopes ().push (home_op);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_operation - code generation ")
                         ACE_TEXT ("for scope failed\n")),
                        -1);
    }

  idl_global->scopes ().pop ();

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_argument (be_argument *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  be_argument *added_arg = 0;
  ACE_NEW_RETURN (added_arg,
                  be_argument (node->direction (),
                               node->field_type (),
                               &sn),
                  -1);

  idl_global->scopes ().top ()->add_to_scope (added_arg);

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_factory (be_factory *node)
{
  UTL_ScopedName sn (node->local_name (), 0);
  
  AST_Home *f_home =
    AST_Home::narrow_from_scope (node->defined_in ());

  be_operation *added_factory = 0;
  ACE_NEW_RETURN (added_factory,
                  be_operation (f_home->managed_component (),
                                AST_Operation::OP_noflags,
                                &sn,
                                false,
                                false),
                  -1);

  idl_global->scopes ().top ()->add_to_scope (added_factory);
  idl_global->scopes ().push (added_factory);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_factory - code generation ")
                         ACE_TEXT ("for scope failed\n")),
                        -1);
    }

  idl_global->scopes ().pop ();

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_finder (be_finder *node)
{
  UTL_ScopedName sn (node->local_name (), 0);

  AST_Home *f_home =
    AST_Home::narrow_from_scope (node->defined_in ());

  be_operation *added_finder = 0;
  ACE_NEW_RETURN (added_finder,
                  be_operation (f_home->managed_component (),
                                AST_Operation::OP_noflags,
                                &sn,
                                false,
                                false),
                  -1);

  idl_global->scopes ().top ()->add_to_scope (added_finder);
  idl_global->scopes ().push (added_finder);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_finder - code generation ")
                         ACE_TEXT ("for scope failed\n")),
                        -1);
    }

  idl_global->scopes ().pop ();

  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_valuebox (be_valuebox *)
{
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_valuetype (be_valuetype *)
{
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_structure (be_structure *)
{
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_structure_fwd (be_structure_fwd *)
{
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_exception (be_exception *node)
{
  UTL_ScopedName sn (node->local_name (), 0);
  
  be_exception *added_excep = 0;
  ACE_NEW_RETURN (added_excep,
                  be_exception (&sn,
                                false,
                                false),
                  -1);
                                          
  idl_global->scopes ().top ()->add_to_scope (added_excep);
  idl_global->scopes ().push (added_excep);
  
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_xplicit_pre_proc::")
                         ACE_TEXT ("visit_exception - code generation ")
                         ACE_TEXT ("for scope failed\n")),
                        -1);
    }
    
  idl_global->scopes ().pop ();
    
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_enum (be_enum *)
{
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_field (be_field *node)
{
  UTL_ScopedName sn (node->local_name (), 0);
  
  be_field *added_field = 0;
  ACE_NEW_RETURN (added_field,
                  be_field (node->field_type (),
                            &sn,
                            node->visibility ()),
                  -1);
                                    
  idl_global->scopes ().top ()->add_to_scope (added_field);
  
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_attribute (be_attribute *)
{
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_union (be_union *)
{
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_union_fwd (be_union_fwd *)
{
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_union_branch (be_union_branch *)
{
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_union_label (be_union_label *)
{
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_constant (be_constant *)
{
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_enum_val (be_enum_val *)
{
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_array (be_array *)
{
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_sequence (be_sequence *)
{
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_string (be_string *)
{
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_typedef (be_typedef *)
{
  return 0;
}

int
be_visitor_xplicit_pre_proc::visit_native (be_native *)
{
  return 0;
}

be_interface *
be_visitor_xplicit_pre_proc::xplicit (void) const
{
  return this->xplicit_;
}

