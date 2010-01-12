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
#include "be_constant.h"
#include "be_structure.h"
#include "be_param_holder.h"
#include "be_expression.h"

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
    AST_Type::narrow_from_decl (
      this->reify_type (node->base_type ()));
  
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

int
be_visitor_tmpl_module_inst::visit_constant (be_constant *node)
{
  if (this->ctx_->template_args () == 0)
    {
      return 0;
    }
    
  be_param_holder *ph =
    be_param_holder::narrow_from_decl (
      node->constant_value ()->param_holder ());
      
  AST_Expression *v = 0;
  AST_Expression::ExprType et = AST_Expression::EV_none;
    
  if (ph != 0)
    {
      be_visitor_reifying rv (this->ctx_);
      
      if (rv.visit_param_holder (ph) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_tmpl_module_inst::")
                             ACE_TEXT ("visit_constant - ")
                             ACE_TEXT ("reification of param ")
                             ACE_TEXT ("holder failed\n")),
                            -1);
        }
        
      AST_Constant *c =
        AST_Constant::narrow_from_decl (rv.reified_node ());
        
      v = c->constant_value ();
      et = c->et ();
    }
  else
    {
      v = node->constant_value ();
      et = node->et ();
    }
    
  be_expression *new_v = 0;
  ACE_NEW_RETURN (new_v,
                  be_expression (v, et),
                  -1);
    
  be_constant *new_c = 0;
  ACE_NEW_RETURN (new_c,
                  be_constant (et, new_v, node->name ()),
                  -1);
    
  this->ctx_->template_module_inst_scope ()->add_to_scope (new_c);

  return 0;
}

int
be_visitor_tmpl_module_inst::visit_structure (be_structure *node)
{
  if (this->ctx_->template_args () == 0)
    {
      return 0;
    }
    
  be_structure *s = 0;
  ACE_NEW_RETURN (s,
                  be_structure (node->name (),
                                node->is_local (),
                                node->is_abstract ()),
                  -1);
      
  // Hold current scope for restoration later.    
  be_scope *holder = this->ctx_->template_module_inst_scope (); 
           
  holder->add_to_scope (s);
  
  // Store the new scope for traversal.
  this->ctx_->template_module_inst_scope (s);
  
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_structure - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }
    
  // Restore the outer scope.  
  this->ctx_->template_module_inst_scope (holder);

  return 0;
}

int
be_visitor_tmpl_module_inst::visit_field (be_field *node)
{
  AST_Type *t =
    AST_Type::narrow_from_decl (
      this->reify_type (node->field_type ()));
      
  be_field *f = 0;
  ACE_NEW_RETURN (f,
                  be_field (t,
                            node->name (),
                            node->visibility ()),
                  -1);
      
  this->ctx_->template_module_inst_scope ()->be_add_field (f);
  
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
