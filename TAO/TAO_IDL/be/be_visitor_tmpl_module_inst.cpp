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
#include "be_eventtype.h"
#include "be_interface.h"
#include "be_attribute.h"
#include "be_operation.h"
#include "be_argument.h"
#include "be_typedef.h"
#include "be_constant.h"
#include "be_structure.h"
#include "be_factory.h"
#include "be_param_holder.h"
#include "be_expression.h"

#include "ast_param_holder.h"

#include "utl_namelist.h"
#include "utl_identifier.h"
#include "utl_exceptlist.h"

#include "fe_interface_header.h"
#include "fe_obv_header.h"

#include "nr_extern.h"

be_visitor_tmpl_module_inst::be_visitor_tmpl_module_inst (
      be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    for_eventtype_ (false)
{
}

be_visitor_tmpl_module_inst::~be_visitor_tmpl_module_inst (void)
{
}

int
be_visitor_tmpl_module_inst::visit_root (be_root *node)
{
  // We will be updating the global scope stack as required for
  // correct implied IDL creation. The root node is already on
  // the stack so we needn't do anything here.

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
  // We can conveniently check this member's value to tell
  // if we are (at some level )processing a template module
  // instantiation.
  // If so, we need to create a new module on the AST.
  // When processing of the instantiation is done, the member
  // is reset to 0.
  bool in_template_module =
    this->ctx_->template_args () != 0;
  
  be_module *added_module = 0;
      
  // If we are traversing a template module as a result of its
  // instantiation, we want to create a corresponding module
  // in the instantiated module (at the top of the scope stack),
  // and push it on the scope stack. Otherwise, we just push
  // the module we are visiting.    
  if (in_template_module)
    {
      ACE_NEW_RETURN (added_module,
                      be_module (node->name ()),
                      -1);
                      
      idl_global->scopes ().top ()->add_to_scope (added_module);
    }
    
  idl_global->scopes ().push (
    in_template_module ? added_module : node);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_module - ")
                         ACE_TEXT ("visit scope failed\n")),
                        -1);
    }
    
  // Restore scope stack. 
  idl_global->scopes ().pop ();
  
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
  
  be_module *instance = 0;
  ACE_NEW_RETURN (instance,
                  be_module (node->name ()),
                  -1);
                  
  // Add the new module to the scope containing the template
  // module instantiation.
  idl_global->scopes ().top ()->add_to_scope (instance);
  
  // Update our scope management.
  idl_global->scopes ().push (instance);
  
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

  // Restore the scope stack.  
  idl_global->scopes ().pop ();
  
  return 0;
}

int
be_visitor_tmpl_module_inst::visit_eventtype (be_eventtype *node)
{
  this->for_eventtype_ = true;
  return this->visit_valuetype (node);
}

int
be_visitor_tmpl_module_inst::visit_valuetype (be_valuetype *node)
{
  if (this->ctx_->template_args () == 0)
    {
      return 0;
    }
    
  UTL_NameList *parent_names =
    this->create_name_list (node->inherits (),
                            node->n_inherits ());
                            
  UTL_NameList *supports_names =
    this->create_name_list (node->supports (),
                            node->n_supports ());
                            
  Identifier *node_id = 0;
  ACE_NEW_RETURN (node_id,
                  Identifier (node->local_name ()),
                  -1);
                  
  UTL_ScopedName *local_name = 0;
  ACE_NEW_RETURN (local_name,
                  UTL_ScopedName (node_id, 0),
                  -1);
                  
  be_valuetype *added_vtype = 0;

  FE_OBVHeader header (local_name,
                       parent_names,
                       supports_names,
                       (parent_names != 0
                          ? parent_names->truncatable ()
                          : false),
                       this->for_eventtype_);
  
  if (this->for_eventtype_)
    {
      ACE_NEW_RETURN (added_vtype,
                      be_eventtype (header.name (),
                                    header.inherits (),
                                    header.n_inherits (),
                                    header.inherits_concrete (),
                                    header.inherits_flat (),
                                    header.n_inherits_flat (),
                                    header.supports (),
                                    header.n_supports (),
                                    header.supports_concrete (),
                                    false,
                                    header.truncatable (),
                                    false),
                      -1);
    }
  else
    {                           
      ACE_NEW_RETURN (added_vtype,
                      be_valuetype (header.name (),
                                    header.inherits (),
                                    header.n_inherits (),
                                    header.inherits_concrete (),
                                    header.inherits_flat (),
                                    header.n_inherits_flat (),
                                    header.supports (),
                                    header.n_supports (),
                                    header.supports_concrete (),
                                    false,
                                    header.truncatable (),
                                    false),
                      -1);
    }
             
  parent_names->destroy ();                
  delete parent_names;
  parent_names = 0;
  
  supports_names->destroy ();
  delete supports_names;
  supports_names = 0;
  
  idl_global->scopes ().top ()->add_to_scope (added_vtype);
  
  // Update the scope management.
  idl_global->scopes ().push (added_vtype);
  
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_valuetype - ")
                         ACE_TEXT ("visit_scope failed\n")),
                        -1);
    }
    
  // Through with this scope.
  idl_global->scopes ().pop ();
  
  // Reset the flag.
  this->for_eventtype_ = false;
    
  return 0;
}

int
be_visitor_tmpl_module_inst::visit_interface (be_interface *node)
{
  if (this->ctx_->template_args () == 0)
    {
      return 0;
    }
    
  UTL_NameList *parent_names =
    this->create_name_list (node->inherits (),
                            node->n_inherits ());
                            
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
  
  idl_global->scopes ().top ()->add_to_scope (added_iface);
  
  // Update the scope stack.
  idl_global->scopes ().push (added_iface);
  
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("visit_scope failed\n")),
                        -1);
    }
    
  // Through with this scope.  
  idl_global->scopes ().pop ();
    
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
                  
  idl_global->scopes ().top ()->add_to_scope (added_attr);
  
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
                  
  idl_global->scopes ().top ()->add_to_scope (added_op);
  
  // Update the scope stack.
  idl_global->scopes ().push (added_op);
  
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_operation - ")
                         ACE_TEXT ("visit_scope failed\n")),
                        -1);
    }
    
  // Through with this scope.  
  idl_global->scopes ().pop ();
  
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
      
  be_argument *added_arg = 0;
  ACE_NEW_RETURN (added_arg,
                  be_argument (node->direction (),
                               t,
                               node->name ()),
                  -1);
    
  idl_global->scopes ().top ()->add_to_scope (added_arg);
      
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
                   
  idl_global->scopes ().top ()->add_to_scope (added_td);

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
    
  idl_global->scopes ().top ()->add_to_scope (added_const);

  return 0;
}

int
be_visitor_tmpl_module_inst::visit_structure (be_structure *node)
{
  if (this->ctx_->template_args () == 0)
    {
      return 0;
    }
    
  UTL_ScopedName sn (node->name ()->last_component (), 0);
    
  be_structure *added_struct = 0;
  ACE_NEW_RETURN (added_struct,
                  be_structure (&sn,
                                node->is_local (),
                                node->is_abstract ()),
                  -1);
                        
  idl_global->scopes ().top ()->add_to_scope (added_struct);
  
  // Update our scope stack.
  idl_global->scopes ().push (added_struct);
  
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_structure - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }
    
  // Through with this scope.  
  idl_global->scopes ().pop ();

  return 0;
}

int
be_visitor_tmpl_module_inst::visit_field (be_field *node)
{
  AST_Type *t =
    AST_Type::narrow_from_decl (
      this->reify_type (node->field_type ()));
      
  be_field *added_field = 0;
  ACE_NEW_RETURN (added_field,
                  be_field (t,
                            node->name (),
                            node->visibility ()),
                  -1);
      
  idl_global->scopes ().top ()->add_to_scope (added_field);
  
  return 0;
}

int
be_visitor_tmpl_module_inst::visit_factory (be_factory *node)
{
  Identifier id (node->local_name ()->get_string ());
  UTL_ScopedName sn (&id, 0);
  
  be_factory *added_factory = 0;
  ACE_NEW_RETURN (added_factory,
                  be_factory (&sn),
                  -1);
                  
  idl_global->scopes ().top ()->add_to_scope (added_factory);
  
  // Update the scope stack.
  idl_global->scopes ().push (added_factory);
  
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_factory - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }
    
  // Through with this scope.
  idl_global->scopes ().pop ();
  
  UTL_ExceptList *reified_exceps =
    this->reify_exception_list (node->exceptions ());
    
  added_factory->be_add_exceptions (reified_exceps);
  
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

UTL_NameList *
be_visitor_tmpl_module_inst::create_name_list (AST_Type **list,
                                               long length)
{
  UTL_NameList *retval = 0;

  for (long i = 0; i < length; ++i)
    {
      AST_Type *item =
        AST_Type::narrow_from_decl (this->reify_type (list[i]));
          
      // We copy each name added so we can call destroy() on the
      // list, which disposes of the contents as well as the
      // nested tail pointers.    
      UTL_NameList *name_item = 0;    
      ACE_NEW_RETURN (name_item,
                      UTL_NameList (item->name ()->copy (), 0),
                      0);

      if (retval == 0)
        {
          retval = name_item;
        }
      else
        {
          retval->nconc (name_item);
        }
    }
    
  return retval;
}
