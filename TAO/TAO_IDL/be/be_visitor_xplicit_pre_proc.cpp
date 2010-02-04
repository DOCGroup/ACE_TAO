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
  this->node_ = node;
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
                         ACE_TEXT ("be_visitor_ccm_pre_proc::")
                         ACE_TEXT ("create_explicit - code generation ")
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
  
  if (this->visit_scope (home_op) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_ccm_pre_proc::")
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

  be_operation *added_factory = 0;
  ACE_NEW_RETURN (added_factory,
                  be_operation (this->node_->managed_component (),
                                AST_Operation::OP_noflags,
                                &sn,
                                false,
                                false),
                  -1);
                  
  AST_Interface *d =
    AST_Interface::narrow_from_scope (
      idl_global->scopes ().top ());
                  
  idl_global->scopes ().top ()->add_to_scope (added_factory);
  idl_global->scopes ().push (added_factory);
  
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_ccm_pre_proc::")
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

  be_operation *added_finder = 0;
  ACE_NEW_RETURN (added_finder,
                  be_operation (this->node_->managed_component (),
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
                         ACE_TEXT ("be_visitor_ccm_pre_proc::")
                         ACE_TEXT ("visit_finder - code generation ")
                         ACE_TEXT ("for scope failed\n")),
                        -1);
    }
    
  idl_global->scopes ().pop ();
                    
  return 0;
}

be_interface *
be_visitor_xplicit_pre_proc::xplicit (void) const
{
  return this->xplicit_;
}