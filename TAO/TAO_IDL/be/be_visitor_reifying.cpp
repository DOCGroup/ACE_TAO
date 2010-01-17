/**
 *  @file   be_visitor_reifying.cpp
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "be_visitor_reifying.h"
#include "be_visitor_context.h"

#include "be_interface.h"
#include "be_valuetype.h"
#include "be_eventtype.h"
#include "be_component.h"
#include "be_exception.h"
#include "be_typedef.h"
#include "be_array.h"
#include "be_sequence.h"
#include "be_predefined_type.h"
#include "be_string.h"
#include "be_constant.h"
#include "be_param_holder.h"

#include "ast_template_module.h"

#include "utl_identifier.h"
#include "utl_exprlist.h"
#include "nr_extern.h"

be_visitor_reifying::be_visitor_reifying (
      be_visitor_context *ctx)
  : be_visitor_decl (ctx),
    reified_node_ (0)
{
}

be_visitor_reifying::~be_visitor_reifying (void)
{
}

AST_Decl *
be_visitor_reifying::reified_node (void) const
{
  return this->reified_node_;
}

int
be_visitor_reifying::visit_interface (be_interface *node)
{
  UTL_ScopedName *tmpl_tail =
    this->template_module_rel_name (node);
    
  if (tmpl_tail != 0)
    {
      AST_Decl *d =
        idl_global->scopes ().top ()->lookup_by_name (
          tmpl_tail,
          true);
    
      this->reified_node_ = d;
    }
  else
    {
      this->reified_node_ = node;
    }
    
  return 0;
}

int
be_visitor_reifying::visit_valuetype (be_valuetype *node)
{
  this->reified_node_ = node;
  return 0;
}

int
be_visitor_reifying::visit_eventtype (be_eventtype *node)
{
  this->reified_node_ = node;
  return 0;
}

int
be_visitor_reifying::visit_component (be_component *node)
{
  this->reified_node_ = node;
  return 0;
}

int
be_visitor_reifying::visit_exception (be_exception *node)
{
  this->reified_node_ = node;
  return 0;
}

int
be_visitor_reifying::visit_typedef (be_typedef *node)
{
  this->reified_node_ = node;
  return 0;
}

int
be_visitor_reifying::visit_array (be_array *node)
{
  be_type *bt =
    be_type::narrow_from_decl (node->base_type ());
    
  if (bt->accept (this) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_reifying::")
                         ACE_TEXT ("visit_array - ")
                         ACE_TEXT ("visit of base type failed\n")),
                        -1);
    }
    
  bt = be_type::narrow_from_decl (this->reified_node_);
  
  AST_Expression **dims = node->dims ();
  AST_Expression *v = 0;
  UTL_ExprList *v_list = 0;
  
  for (ACE_CDR::ULong i = 0; i < node->n_dims (); ++i)
    {
      be_param_holder *ph =
        be_param_holder::narrow_from_decl (dims[i]->param_holder ());
      
      if (ph != 0)
        {
          if (this->visit_param_holder (ph) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("be_visitor_reifying::")
                                 ACE_TEXT ("visit_array - ")
                                 ACE_TEXT ("visit_param_holder() ")
                                 ACE_TEXT ("failed\n")),
                                -1);
            }
      
          AST_Constant *c =
            AST_Constant::narrow_from_decl (this->reified_node_);
            
          ACE_NEW_RETURN (v,
                          AST_Expression (c->constant_value (),
                                          AST_Expression::EV_ulong),
                          -1);
        }
      else
        {
          ACE_NEW_RETURN (v,
                          AST_Expression (dims[i],
                                          AST_Expression::EV_ulong),
                          -1);
        }
        
      UTL_ExprList *el = 0;
      ACE_NEW_RETURN (el,
                      UTL_ExprList (v, 0),
                      -1);
                      
      if (v_list == 0)
        {
          v_list = el;
        }
      else
        {
          v_list->nconc (el);
        }
    }
    
  UTL_ScopedName sn (node->name ()->last_component ()->copy (),
                     0);
                     
  be_array *arr = 0;
    
  ACE_NEW_RETURN (arr,
                  be_array (&sn,
                            node->n_dims (),
                            v_list,
                            false,
                            false),
                  -1);
                  
 
  // No need to add this new node to any scope - it's anonymous
  // and owned by the node that references it.
                    
  sn.destroy ();
  v_list->destroy ();
  delete v_list;
  v_list = 0;
  
  arr->set_base_type (bt);
  this->reified_node_ = arr;
  
  return 0;
}

int
be_visitor_reifying::visit_sequence (be_sequence *node)
{
  be_type *bt =
    be_type::narrow_from_decl (node->base_type ());
    
  if (bt->accept (this) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_reifying::")
                         ACE_TEXT ("visit_sequence - ")
                         ACE_TEXT ("visit of base type failed\n")),
                        -1);
    }
    
  bt = be_type::narrow_from_decl (this->reified_node_);
  
  AST_Expression *v = node->max_size ();
  be_param_holder *ph =
    be_param_holder::narrow_from_decl (v->param_holder ());
  
  if (ph != 0)
    {
      if (this->visit_param_holder (ph) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_reifying::")
                             ACE_TEXT ("visit_sequence - ")
                             ACE_TEXT ("visit_param_holder() ")
                             ACE_TEXT ("failed\n")),
                            -1);
        }
      
      AST_Constant *c =
        AST_Constant::narrow_from_decl (this->reified_node_);
        
      v = c->constant_value ();
    }
  
  AST_Expression *bound = 0;
  ACE_NEW_RETURN (bound,
                  AST_Expression (v,
                                  AST_Expression::EV_ulong),
                  -1);
      
  Identifier id ("sequence");
  UTL_ScopedName sn (&id, 0);

  ACE_NEW_RETURN (this->reified_node_,
                  be_sequence (bound,
                               bt,
                               &sn,
                               false,
                               false),
                  -1);

  // No need to add this new node to any scope - it's anonymous
  // and owned by the node that references it.
                    
  return 0;
}

int
be_visitor_reifying::visit_predefined_type (be_predefined_type *node)
{
  this->reified_node_ = node;
  return 0;
}

int
be_visitor_reifying::visit_string (be_string *node)
{
  this->reified_node_ = node;
  return 0;
}

int
be_visitor_reifying::visit_constant (be_constant *node)
{
  // AFAICT, this is called only on the member constant of a param
  // holder, in which case nothing further is needed.
  this->reified_node_ = node;
  return 0;
}

int
be_visitor_reifying::visit_param_holder (be_param_holder *node)
{
  size_t i = 0;
  FE_Utils::T_ARGLIST *t_args =
    this->ctx_->template_args ();

  for (FE_Utils::T_PARAMLIST_INFO::ITERATOR iter (
         *this->ctx_->template_params ());
       !iter.done ();
       iter.advance (), ++i)
    {
      FE_Utils::T_Param_Info *item = 0;
      iter.next (item);
      
      if (item == node->info ())
        {
          AST_Decl **ret_ptr = 0;
          
          if (t_args->get (ret_ptr, i) == 0)
            {
              be_decl *candidate =
                be_decl::narrow_from_decl (*ret_ptr);
                
              return candidate->accept (this);
            }
          else
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("be_visitor_reifying::")
                                 ACE_TEXT ("visit_param_holder() - access of ")
                                 ACE_TEXT ("current template arglist failed - ")
                                 ACE_TEXT ("param=%C scope=%C index=%d\n"),
                                 item->name_.c_str (),
                                 this->ctx_->node ()->full_name (),
                                 i),
                                -1);
            }
        }
    }
    
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("be_visitor_reifying::")
                     ACE_TEXT ("visit_param_holder() - no match for ")
                     ACE_TEXT ("template param %C in %C\n"),
                     node->info ()->name_.c_str (),
                     this->ctx_->node ()->full_name ()),
                    -1);
}

UTL_ScopedName *
be_visitor_reifying::template_module_rel_name (AST_Decl *d)
{
  AST_Decl *tmp = d;
  ACE_CString name (d->full_name ());
  
  while (tmp != 0)
    {
      if (AST_Template_Module::narrow_from_decl (tmp) != 0)
        {
          ACE_CString head (tmp->local_name ()->get_string ());
          
          ACE_CString::size_type start = name.find (head) + 2;
            
          ACE_CString tail (name.substr (start + head.length ()));
            
          return idl_global->string_to_scoped_name (tail.c_str ());
        }
        
      tmp = ScopeAsDecl (tmp->defined_in ());
    }
    
  return 0;
}

