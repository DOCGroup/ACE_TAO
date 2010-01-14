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
#include "be_interface.h"
#include "be_attribute.h"
#include "be_operation.h"
#include "be_argument.h"
#include "be_typedef.h"
#include "be_constant.h"
#include "be_structure.h"
#include "be_param_holder.h"
#include "be_expression.h"

#include "ast_param_holder.h"

#include "utl_namelist.h"
#include "utl_identifier.h"
#include "utl_exceptlist.h"

#include "fe_interface_header.h"

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
be_visitor_tmpl_module_inst::visit_interface (be_interface *node)
{
  if (this->ctx_->template_args () == 0)
    {
      return 0;
    }
    
  UTL_Scope *s = node->defined_in ();
  UTL_NameList *parent_names = 0;
  
  // We're at global scope here so we need to fool the scope stack
  // for a minute so the correct repo id can be calculated at
  // interface construction time.
  idl_global->scopes ().push (s);

  for (long i = 0; i < node->n_inherits (); ++i)
    {
      AST_Type *parent =
        AST_Type::narrow_from_decl (this->reify_type (
          node->inherits ()[i]));
          
      // We copy each name added so we can call destroy() on the
      // list, which disposes of the contents as well as the
      // nested tail pointers.    
      UTL_NameList *parent_name = 0;    
      ACE_NEW_RETURN (parent_name,
                      UTL_NameList (parent->name ()->copy (), 0),
                      -1);

      if (parent_names == 0)
        {
          parent_names = parent_name;
        }
      else
        {
          parent_names->nconc (parent_name);
        }
    }
    
  // Back to reality.
  idl_global->scopes ().pop ();

  // Now set the scope to our adding scope.
  s = this->ctx_->template_module_inst_scope ();
  idl_global->scopes ().push (s);

  Identifier *node_id = 0;
  ACE_NEW_RETURN (node_id,
                  Identifier (node->local_name ()),
                  -1);
                  
  UTL_ScopedName *local_name = 0;
  ACE_NEW_RETURN (local_name,
                  UTL_ScopedName (node_id, 0),
                  -1);
                  
  FE_InterfaceHeader header (local_name,
                             parent_names,
                             node->is_local (),
                             node->is_abstract (),
                             true);
                             
  be_interface *added_iface = 0;
  ACE_NEW_RETURN (added_iface,
                  be_interface (header.name (),
                                header.inherits (),
                                header.n_inherits (),
                                header.inherits_flat (),
                                header.n_inherits_flat (),
                                header.is_local (),
                                header.is_abstract ()),
                  -1);
             
  if (parent_names)
    {
      parent_names->destroy ();                
    }
  delete parent_names;
  parent_names = 0;
  
  // Back to reality.
  idl_global->scopes ().pop ();
  
  added_iface->set_defined_in (s);
  added_iface->set_imported (node->imported ());
  
  // Set repo id to 0, so it will be recomputed on the next access,
  // and set the prefix to the eventtype's prefix. All this is
  // necessary in case the eventtype's prefix was modified after
  // its declaration. We assume 'implied IDL' means that the
  // derived event consumer interface should have the same prefix.
  added_iface->AST_Decl::repoID (0);
  added_iface->prefix (const_cast<char*> (node->prefix ()));
  
  const char *repo_id = added_iface->repoID ();

  // For interfaces, this should always be a module or root, and
  // AST_Root is a subclass of AST_Module.
  AST_Module *m = AST_Module::narrow_from_scope (s);
  
  m->be_add_interface (added_iface);
  
  // Save our containing scope for restoration later.
  be_scope *holder = this->ctx_->template_module_inst_scope ();
  
  // Update the adding scope for the interface contents.
  this->ctx_->template_module_inst_scope (added_iface);
  
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("visit_scope failed\n")),
                        -1);
    }
    
  // Restore the previous scope.  
  this->ctx_->template_module_inst_scope (holder);
    
  return 0;
}

int
be_visitor_tmpl_module_inst::visit_attribute (be_attribute *node)
{
  AST_Type *ft =
    AST_Type::narrow_from_decl (
      this->reify_type (node->field_type ()));
      
  be_attribute *added_attr = 0;
  ACE_NEW_RETURN (added_attr,
                  be_attribute (node->readonly (),
                                ft,
                                node->name (),
                                node->is_local (),
                                node->is_abstract ()),
                  -1);
                  
  be_scope *s = this->ctx_->template_module_inst_scope ();
  added_attr->set_defined_in (s);               
  s->add_to_scope (added_attr);
  
  // Force recalculation of our repo ID based on the new scope.
  added_attr->repoID (0);
  const char *dummy = added_attr->repoID ();
  
  // These will work even if the exception lists are null.
  
  UTL_ExceptList *old_ex = node->get_get_exceptions ();
  UTL_ExceptList *new_ex = this->reify_exception_list (old_ex);
  added_attr->be_add_get_exceptions (new_ex);
  
  old_ex = node->get_set_exceptions ();
  new_ex = this->reify_exception_list (old_ex);
  added_attr->be_add_set_exceptions (new_ex);
  
  return 0;
}

int
be_visitor_tmpl_module_inst::visit_operation (be_operation *node)
{
  AST_Type *rt =
    AST_Type::narrow_from_decl (
      this->reify_type (node->return_type ()));
      
  Identifier id (node->local_name ()->get_string ());
  UTL_ScopedName sn (&id, 0);
      
  be_operation *added_op = 0;
  ACE_NEW_RETURN (added_op,
                  be_operation (rt,
                                node->flags (),
                                &sn,
                                node->is_local (),
                                node->is_abstract ()),
                  -1);
                  
  be_scope *s = this->ctx_->template_module_inst_scope ();
  s->add_to_scope (added_op);
  added_op->set_defined_in (s);
  
  this->ctx_->template_module_inst_scope (added_op);
  
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("visit_scope failed\n")),
                        -1);
    }
    
  this->ctx_->template_module_inst_scope (s);
  
  UTL_ExceptList *new_ex =
    this->reify_exception_list (node->exceptions ());
    
  added_op->be_add_exceptions (new_ex);
  
  return 0;
}

int
be_visitor_tmpl_module_inst::visit_argument (be_argument *node)
{
  AST_Type *t =
    AST_Type::narrow_from_decl (
      this->reify_type (node->field_type ()));
      
  be_argument *arg = 0;
  ACE_NEW_RETURN (arg,
                  be_argument (node->direction (),
                               t,
                               node->name ()),
                  -1);
      
  be_operation *op =
    be_operation::narrow_from_scope (
      this->ctx_->template_module_inst_scope ());
      
  op->be_add_argument (arg);
      
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
  
  be_typedef *added_td = 0;
  ACE_NEW_RETURN  (added_td,
                   be_typedef (bt,
                               node->name (),
                               false,
                               false),
                   -1);
                   
  be_scope *s = this->ctx_->template_module_inst_scope ();
  added_td->set_defined_in (s);               
  s->add_to_scope (added_td);

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
    
  be_constant *added_const = 0;
  ACE_NEW_RETURN (added_const,
                  be_constant (et, new_v, node->name ()),
                  -1);
    
  be_scope *s = this->ctx_->template_module_inst_scope ();
  added_const->set_defined_in (s);               
  s->add_to_scope (added_const);

  return 0;
}

int
be_visitor_tmpl_module_inst::visit_structure (be_structure *node)
{
  if (this->ctx_->template_args () == 0)
    {
      return 0;
    }
    
  be_structure *added_struct = 0;
  ACE_NEW_RETURN (added_struct,
                  be_structure (node->name (),
                                node->is_local (),
                                node->is_abstract ()),
                  -1);
      
  // Hold current scope for restoration later.    
  be_scope *holder = this->ctx_->template_module_inst_scope (); 
           
  holder->add_to_scope (added_struct);
  added_struct->set_defined_in (holder);
  
  
  // Store the new scope for traversal.
  this->ctx_->template_module_inst_scope (added_struct);
  
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

UTL_ExceptList *
be_visitor_tmpl_module_inst::reify_exception_list (
  UTL_ExceptList *orig)
{
  if (orig == 0)
    {
      return 0;
    }
    
  UTL_ExceptList *retval = 0;
  
  for (UTL_ExceptlistActiveIterator i (orig);
       !i.is_done ();
       i.next ())
    {
      AST_Type *ex =
        AST_Type::narrow_from_decl (this->reify_type (i.item ()));
      
      UTL_ExceptList *ex_list = 0;
      ACE_NEW_RETURN (ex_list,
                      UTL_ExceptList (ex, 0),
                      0);
                      
      if (retval == 0)
        {
          retval = ex_list;
        }
      else
        {
          retval->nconc (ex_list);
        }
    }
    
  return retval;
}