// $Id$

#include "ast_template_common.h"
#include "ast_field.h"
#include "ast_constant.h"

#include "utl_identifier.h"
#include "utl_namelist.h"
#include "utl_strlist.h"
#include "utl_string.h"
#include "utl_err.h"
#include "global_extern.h"

ACE_RCSID (ast,
           ast_template_common,
           "$Id$")

AST_Template_Common::AST_Template_Common (
      AST_Decl::NodeType nt,
      UTL_ScopedName *n,
      FE_Utils::T_PARAMLIST_INFO *template_params)
  : AST_Decl (nt,
              n),
    UTL_Scope (nt),
    template_params_ (template_params)
{
}

AST_Template_Common::~AST_Template_Common (void)
{
}

FE_Utils::T_PARAMLIST_INFO *
AST_Template_Common::template_params (void)
{
  return this->template_params_;
}

void
AST_Template_Common::destroy (void)
{
  delete this->template_params_;
  this->template_params_ = 0;
}

AST_Template_Common::T_ARGLIST *
AST_Template_Common::match_arg_names (UTL_NameList *arg_names)
{
  size_t names_len = static_cast<size_t> (arg_names->length ());
  
  if (names_len != this->template_params_->size ())
    {
      idl_global->err ()->error1 (UTL_Error::EIDL_T_ARG_LENGTH,
                                  this);
      return 0;
    }
    
  size_t slot = 0UL;
  bool good = true;
  T_ARGLIST *retval = 0;
  ACE_NEW_RETURN (retval,
                  T_ARGLIST,
                  0);
  
  for (UTL_NamelistActiveIterator iter (arg_names);
       !iter.is_done ();
       iter.next (), ++slot)
    {
      UTL_ScopedName *name = iter.item ();
      
      AST_Decl *d = this->lookup_by_name (name, true);
      
      if (d == 0)
        {
          idl_global->err ()->lookup_error (name);
          good = false;
          break;
        }
      else if (d->node_type () == AST_Decl::NT_typedef)
        {
          AST_Typedef *td =
            AST_Typedef::narrow_from_decl (d);
            
          d = td->primitive_base_type ();
        }
      
      FE_Utils::T_Param_Info *param = 0;
      (void) this->template_params_->get (param, slot);
        
      if (! this->match_param_type (param->type_, d))
        {
          idl_global->err ()->mismatched_template_param (d->name ());
          good = false;
          break;
        }
        
      retval->enqueue_tail (d);
    }
    
  if (! good)
    {
      delete retval;
      retval = 0;
    }
    
  return retval;
}

bool
AST_Template_Common::match_params (UTL_StrList *param_names)
{
  size_t names_len = static_cast<size_t> (param_names->length ());
  
  if (names_len != this->template_params_->size ())
    {
      idl_global->err ()->error1 (UTL_Error::EIDL_T_ARG_LENGTH,
                                  this);
      return false;
    }
    
  size_t slot = 0UL;

  for (UTL_StrlistActiveIterator i (param_names);
       !i.is_done ();
       i.next (), ++slot)
    { 
      UTL_String *s = i.item ();
      
      FE_Utils::T_Param_Info *param = 0;
      (void) this->template_params_->get (param, slot);
      
      if (param->name_ != s->get_string ())
        {
          idl_global->err ()->mismatched_template_param (this->name ());
        
          return false;
        }
    }
    
  return true;
}

bool
AST_Template_Common::match_param_type (AST_Decl::NodeType my_type,
                                       AST_Decl *d)
{
  if (my_type == AST_Decl::NT_type)
    {
      return true;
    }
    
  AST_Decl::NodeType other_type = d->node_type ();
    
  if (other_type == AST_Decl::NT_field)
    {
      other_type =
        AST_Field::narrow_from_decl (d)->field_type ()->node_type ();
    }
    
  if (other_type == AST_Decl::NT_const)
    {
      AST_Constant *c =
        AST_Constant::narrow_from_decl (d);
        
      switch (c->et ())
        {
          case AST_Expression::EV_enum:
            other_type = AST_Decl::NT_enum;
            break;
          case AST_Expression::EV_string:
            other_type = AST_Decl::NT_string;
            break;
          case AST_Expression::EV_wstring:
            other_type = AST_Decl::NT_wstring;
            break;
          case AST_Expression::EV_any:
          case AST_Expression::EV_object:
            return false;
          default:
            other_type = AST_Decl::NT_pre_defined;
            break;
        }
    }
    
  // Mapping 'primitive' keyword to NT_pre_defined isn't
  // clearly correct - need to raise an issue about what the
  // keyword means.  
  switch (my_type)
    {
      case AST_Decl::NT_pre_defined:
        switch (other_type)
          {
            case AST_Decl::NT_string:
            case AST_Decl::NT_wstring:
            case AST_Decl::NT_pre_defined:
              return true;
            default:
              return false;
          }
      default:
        return my_type == other_type;
    }
}

