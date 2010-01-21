/**
 *  @file   ast_visitor_tmpl_module_inst.cpp
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "ast_visitor_tmpl_module_inst.h"
#include "ast_visitor_reifying.h"
#include "ast_visitor_context.h"
#include "ast_generator.h"

#include "ast_root.h"
#include "ast_module.h"
#include "ast_template_module.h"
#include "ast_template_module_inst.h"
#include "ast_eventtype.h"
#include "ast_interface.h"
#include "ast_attribute.h"
#include "ast_operation.h"
#include "ast_argument.h"
#include "ast_typedef.h"
#include "ast_constant.h"
#include "ast_structure.h"
#include "ast_factory.h"
#include "ast_param_holder.h"
#include "ast_expression.h"

#include "ast_param_holder.h"

#include "utl_namelist.h"
#include "utl_identifier.h"
#include "utl_exceptlist.h"

#include "fe_interface_header.h"
#include "fe_obv_header.h"

#include "nr_extern.h"

ast_visitor_tmpl_module_inst::ast_visitor_tmpl_module_inst (
      ast_visitor_context *ctx)
  : ast_visitor (),
    ctx_ (ctx),
    for_eventtype_ (false)
{
}

ast_visitor_tmpl_module_inst::~ast_visitor_tmpl_module_inst (void)
{
}

int ast_visitor_tmpl_module_inst::visit_decl (AST_Decl *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_scope (UTL_Scope *node)
{
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      if (d == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                             ACE_TEXT ("visit_scope - ")
                             ACE_TEXT ("bad node in this scope\n")),
                            -1);
        }
        
      // Send the visitor.
      if (d == 0 || d->ast_accept (this) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                             ACE_TEXT ("visit_scope - ")
                             ACE_TEXT ("codegen for scope failed\n")),
                            -1);
        }
    }

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_type (AST_Type *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_interface_fwd (AST_InterfaceFwd *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_valuebox (AST_ValueBox *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_valuetype_fwd (AST_ValueTypeFwd *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_eventtype_fwd (AST_EventTypeFwd *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_component_fwd (AST_ComponentFwd *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_template_module_ref (AST_Template_Module_Ref *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_porttype (AST_PortType *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_provides (AST_Provides *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_uses (AST_Uses *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_publishes (AST_Publishes *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_emits (AST_Emits *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_consumes (AST_Consumes *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_extended_port (AST_Extended_Port *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_mirror_port (AST_Mirror_Port *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_connector (AST_Connector *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_home (AST_Home *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_structure_fwd (AST_StructureFwd *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_expression (AST_Expression *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_enum (AST_Enum *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_union (AST_Union *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_union_fwd (AST_UnionFwd *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_union_branch (AST_UnionBranch *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_union_label (AST_UnionLabel *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_enum_val (AST_EnumVal *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_param_holder (AST_Param_Holder *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_root (AST_Root *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_native (AST_Native *)
{
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_module (AST_Module *node)
{
  // We can conveniently check this member's value to tell
  // if we are (at some level )processing a template module
  // instantiation.
  // If so, we need to create a new module on the AST.
  // When processing of the instantiation is done, the member
  // is reset to 0.
  bool in_template_module =
    this->ctx_->template_args () != 0;
  
  AST_Module *added_module = 0;
      
  // If we are traversing a template module as a result of its
  // instantiation, we want to create a corresponding module
  // in the instantiated module (at the top of the scope stack),
  // and push it on the scope stack. Otherwise, we just push
  // the module we are visiting.    
  if (in_template_module)
    {
      added_module =
        idl_global->gen ()->create_module (idl_global->scopes ().top (),
                                           node->name ());
                      
      idl_global->scopes ().top ()->add_to_scope (added_module);
    }
    
  idl_global->scopes ().push (
    in_template_module ? added_module : node);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_module - ")
                         ACE_TEXT ("visit scope failed\n")),
                        -1);
    }
    
  // Restore scope stack. 
  idl_global->scopes ().pop ();
  
  return 0;
}

// Not called from ast_template_module::accept() but from
// visit_template_module_inst() in this visitor.
int
ast_visitor_tmpl_module_inst::visit_template_module (
  AST_Template_Module *node)
{
  this->ctx_->template_params (node->template_params ());
  
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_template_module - ")
                         ACE_TEXT ("visit scope failed\n")),
                        -1);
    }

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_template_module_inst (
  AST_Template_Module_Inst *node)
{
  this->ctx_->template_args (node->template_args ());
  
  AST_Module *instance =
    idl_global->gen ()->create_module (idl_global->scopes ().top (),
                                       node->name ());
                  
  // Add the new module to the scope containing the template
  // module instantiation.
  idl_global->scopes ().top ()->add_to_scope (instance);
  
  // Update our scope management.
  idl_global->scopes ().push (instance);
  
  AST_Template_Module *tm = node->ref ();
  
  if (this->visit_template_module (tm) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
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
ast_visitor_tmpl_module_inst::visit_eventtype (AST_EventType *node)
{
  this->for_eventtype_ = true;
  return this->visit_valuetype (node);
}

int
ast_visitor_tmpl_module_inst::visit_valuetype (AST_ValueType *node)
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
                  Identifier (node->local_name ()->get_string ()),
                  -1);
                  
  UTL_ScopedName *local_name = 0;
  ACE_NEW_RETURN (local_name,
                  UTL_ScopedName (node_id, 0),
                  -1);
                  
  AST_ValueType *added_vtype = 0;

  FE_OBVHeader header (local_name,
                       parent_names,
                       supports_names,
                       (parent_names != 0
                          ? parent_names->truncatable ()
                          : false),
                       this->for_eventtype_);
  
  if (this->for_eventtype_)
    {
      added_vtype =
        idl_global->gen ()->create_eventtype (header.name (),
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
                                              false);
    }
  else
    {                           
      added_vtype =
        idl_global->gen ()->create_valuetype (header.name (),
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
                                              false);
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
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
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
ast_visitor_tmpl_module_inst::visit_interface (AST_Interface *node)
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
                  Identifier (node->local_name ()->get_string ()),
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
                             
  AST_Interface *added_iface =
    idl_global->gen ()->create_interface (header.name (),
                                          header.inherits (),
                                          header.n_inherits (),
                                          header.inherits_flat (),
                                          header.n_inherits_flat (),
                                          header.is_local (),
                                          header.is_abstract ());
             
  parent_names->destroy ();                
  delete parent_names;
  parent_names = 0;
  
  idl_global->scopes ().top ()->add_to_scope (added_iface);
  
  // Update the scope stack.
  idl_global->scopes ().push (added_iface);
  
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("visit_scope failed\n")),
                        -1);
    }
    
  // Through with this scope.  
  idl_global->scopes ().pop ();
    
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_attribute (AST_Attribute *node)
{
  AST_Type *ft =
    AST_Type::narrow_from_decl (
      this->reify_type (node->field_type ()));
      
  AST_Attribute *added_attr =
    idl_global->gen ()->create_attribute (node->readonly (),
                                          ft,
                                          node->name (),
                                          node->is_local (),
                                          node->is_abstract ());
                  
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
ast_visitor_tmpl_module_inst::visit_operation (AST_Operation *node)
{
  AST_Type *rt =
    AST_Type::narrow_from_decl (
      this->reify_type (node->return_type ()));
      
  Identifier id (node->local_name ()->get_string ());
  UTL_ScopedName sn (&id, 0);
      
  AST_Operation *added_op =
    idl_global->gen ()->create_operation (rt,
                                          node->flags (),
                                          &sn,
                                          node->is_local (),
                                          node->is_abstract ());
                  
  idl_global->scopes ().top ()->add_to_scope (added_op);
  
  // Update the scope stack.
  idl_global->scopes ().push (added_op);
  
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
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
ast_visitor_tmpl_module_inst::visit_argument (AST_Argument *node)
{
  AST_Type *t =
    AST_Type::narrow_from_decl (
      this->reify_type (node->field_type ()));
      
  AST_Argument *added_arg =
    idl_global->gen ()->create_argument (node->direction (),
                                         t,
                                         node->name ());
    
  idl_global->scopes ().top ()->add_to_scope (added_arg);
      
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_typedef (AST_Typedef *node)
{
  if (this->ctx_->template_args () == 0)
    {
      return 0;
    }
    
  AST_Type *bt =
    AST_Type::narrow_from_decl (
      this->reify_type (node->base_type ()));
  
  AST_Typedef *added_td =
    idl_global->gen ()->create_typedef (bt,
                                        node->name (),
                                        false,
                                        false);
                   
  idl_global->scopes ().top ()->add_to_scope (added_td);

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_constant (AST_Constant *node)
{
  if (this->ctx_->template_args () == 0)
    {
      return 0;
    }
    
  AST_Param_Holder *ph =
    node->constant_value ()->param_holder ();
      
  AST_Expression *v = 0;
  AST_Expression::ExprType et = AST_Expression::EV_none;
    
  if (ph != 0)
    {
      ast_visitor_reifying rv (this->ctx_);
      
      if (rv.visit_param_holder (ph) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("ast_visitor_tmpl_module_inst::")
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
    
  AST_Expression *new_v =
    idl_global->gen ()->create_expr (v, et);
    
  AST_Constant *added_const =
    idl_global->gen ()->create_constant (et,
                                         new_v,
                                         node->name ());
    
  idl_global->scopes ().top ()->add_to_scope (added_const);

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_structure (AST_Structure *node)
{
  if (this->ctx_->template_args () == 0)
    {
      return 0;
    }
    
  UTL_ScopedName sn (node->name ()->last_component (), 0);
    
  AST_Structure *added_struct =
    idl_global->gen ()->create_structure (&sn,
                                          node->is_local (),
                                          node->is_abstract ());
                        
  idl_global->scopes ().top ()->add_to_scope (added_struct);
  
  // Update our scope stack.
  idl_global->scopes ().push (added_struct);
  
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                         ACE_TEXT ("visit_structure - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }
    
  // Through with this scope.  
  idl_global->scopes ().pop ();

  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_field (AST_Field *node)
{
  AST_Type *t =
    AST_Type::narrow_from_decl (
      this->reify_type (node->field_type ()));
      
  AST_Field *added_field =
    idl_global->gen ()->create_field (t,
                                      node->name (),
                                      node->visibility ());
      
  idl_global->scopes ().top ()->add_to_scope (added_field);
  
  return 0;
}

int
ast_visitor_tmpl_module_inst::visit_factory (AST_Factory *node)
{
  Identifier id (node->local_name ()->get_string ());
  UTL_ScopedName sn (&id, 0);
  
  AST_Factory *added_factory =
    idl_global->gen ()->create_factory (&sn);
                  
  idl_global->scopes ().top ()->add_to_scope (added_factory);
  
  // Update the scope stack.
  idl_global->scopes ().push (added_factory);
  
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("ast_visitor_tmpl_module_inst::")
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
ast_visitor_tmpl_module_inst::reify_type (AST_Decl *d)
{
  ast_visitor_reifying rv (this->ctx_);
  
  if (d->ast_accept (&rv) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ast_visitor_tmpl_module_inst::")
                  ACE_TEXT ("reify_type() - reifying ")
                  ACE_TEXT ("visitor failed on %C\n"),
                  d->full_name ()));
                  
      return 0;
    }
    
  return rv.reified_node ();
}

UTL_ExceptList *
ast_visitor_tmpl_module_inst::reify_exception_list (
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
ast_visitor_tmpl_module_inst::create_name_list (AST_Type **list,
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
