/**
 *  @file   be_visitor_tmpl_module_inst.cpp
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "be_visitor_tmpl_module_inst.h"
#include "be_visitor_reifying.h"
#include "be_visitor_context.h"

#include "be_root.h"
#include "be_module.h"
#include "be_template_module.h"
#include "be_template_module_inst.h"
#include "be_typedef.h"

#include "ast_param_holder.h"

be_visitor_tmpl_module_inst::be_visitor_tmpl_module_inst (
      be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_tmpl_module_inst::~be_visitor_tmpl_module_inst (void)
{
}

int
be_visitor_tmpl_module_inst::visit_root (be_root *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_root - ")
                         ACE_TEXT ("visit scope failed\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_tmpl_module_inst::visit_module (be_module *node)
{
  UTL_Scope *s = this->ctx_->template_module_inst_scope ();
  
  // We can conveniently check this member's value to tell
  // if we are (at some level )processing a template module
  // instantiation.
  // If so, we need to create a new module on the AST.
  // When processing of the instantiation is done, the member
  // is reset to 0.
  if (this->ctx_->template_args () != 0)
    {
      be_module *m = 0;
      
      ACE_NEW_RETURN (m,
                      be_module (node->name ()),
                      -1);
                      
      s->add_to_scope (m);
      this->ctx_->template_module_inst_scope (m);
    }
    
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_module - ")
                         ACE_TEXT ("visit scope failed\n")),
                        -1);
    }
    
  // Restore the outer adding scope.  
  this->ctx_->template_module_inst_scope (s);
  
  return 0;
}

// Not called from be_template_module::accept() but from
// visit_template_module_inst() in this visitor.
int
be_visitor_tmpl_module_inst::visit_template_module (
  be_template_module *node)
{
  this->ctx_->template_params (node->template_params ());
  
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_template_module - ")
                         ACE_TEXT ("visit scope failed\n")),
                        -1);
    }

  return 0;
}

int
be_visitor_tmpl_module_inst::visit_template_module_inst (
  be_template_module_inst *node)
{
  this->ctx_->template_args (node->template_args ());
  UTL_Scope *s = node->defined_in ();
  
  be_module *instance = 0;
  
  ACE_NEW_RETURN (instance,
                  be_module (node->name ()),
                  -1);
                  
  // Add the new module to the scope containing the template
  // module instantiation.
  s->add_to_scope (instance);
  
  // Everything we visit in the template module below will be
  // added to the module just created.
  this->ctx_->template_module_inst_scope (instance);
  
  be_template_module *tm =
    be_template_module::narrow_from_decl (node->ref ());
  
  if (this->visit_template_module (tm) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_template_module_inst - ")
                         ACE_TEXT ("visit_template_module failed\n")),
                        -1);
    }
    
  // By checking this member for a non-zero value, we can tell if
  // we are processing an instantiated template module or not. We
  // need to know this, for example while visiting an IDL module,
  // to decide whether to create an implied IDL module or just
  // visit its scope.  
  this->ctx_->template_args (0);

  // Restore the outer adding scope.  
  this->ctx_->template_module_inst_scope (s);
  
  return 0;
}

int
be_visitor_tmpl_module_inst::visit_typedef (be_typedef *node)
{
  if (this->ctx_->template_args () == 0)
    {
      return 0;
    }
    
  AST_Type *bt =
    AST_Type::narrow_from_decl (this->reify_type (node->base_type ()));
  
  be_typedef *td = 0;
  ACE_NEW_RETURN  (td,
                   be_typedef (bt,
                               node->name (),
                               false,
                               false),
                   -1);
                   
  this->ctx_->template_module_inst_scope ()->add_to_scope (td);

  return 0;
}

AST_Decl *
be_visitor_tmpl_module_inst::reify_type (AST_Decl *d)
{
  be_visitor_reifying rv (this->ctx_);
  be_decl *be_candidate =
    be_decl::narrow_from_decl (d);
  
  if (be_candidate->accept (&rv) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_tmpl_module_inst::")
                  ACE_TEXT ("reify_type() - reifying ")
                  ACE_TEXT ("visitor failed on %C\n"),
                  be_candidate->full_name ()));
                  
      return 0;
    }
    
  return rv.reified_node ();
}
