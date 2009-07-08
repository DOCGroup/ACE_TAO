// $Id$

#include "fe_template_interface_header.h"
#include "fe_extern.h"
#include "ast_template_interface.h"
#include "utl_err.h"
#include "global_extern.h"

FE_Template_InterfaceHeader::FE_Template_InterfaceHeader (
      UTL_ScopedName *n,
      FE_Utils::T_PARAMLIST_INFO *params,
      FE_Utils::T_REFLIST_INFO *inherited)
  : FE_InterfaceHeader (n,
                        0,
                        false,
                        false,
                        false),
    param_info_ (params),
    parent_info_ (inherited)
{
  this->compile_template_inheritance ();
  
  if (this->duplicate_param_id ())
    {
      idl_global->err ()->duplicate_param_id (this->interface_name_);
    }
}

FE_Template_InterfaceHeader::~FE_Template_InterfaceHeader (void)
{
}

void
FE_Template_InterfaceHeader::destroy (void)
{
  // Queue element members have self-managed memory.
  delete this->param_info_;
  this->param_info_ = 0;
  
  // Must destroy the queue element members (but not the elements
  // themselves) before we destroy the queue.
  if (this->parent_info_ != 0)
    {
      for (FE_Utils::T_REFLIST_INFO::CONST_ITERATOR i (*this->parent_info_);
           !i.done ();
           i.advance ())
        {
          FE_Utils::T_Ref_Info *info = 0;
          i.next (info);
          info->destroy ();
        }
    }

  delete this->parent_info_;
  this->parent_info_ = 0;
  
  this->FE_InterfaceHeader::destroy ();
}

// Data accessors.

FE_Utils::T_PARAMLIST_INFO *
FE_Template_InterfaceHeader::param_info (void) const
{
  return this->param_info_;
}

void
FE_Template_InterfaceHeader::compile_template_inheritance (void)
{
  if (this->parent_info_ == 0)
    {
      return;
    }
    
  for (FE_Utils::T_REFLIST_INFO::CONST_ITERATOR iter (*this->parent_info_);
       !iter.done ();
       iter.advance ())
    {
      FE_Utils::T_Ref_Info *item = 0;
      iter.next (item);
      
      UTL_Scope *s = idl_global->scopes ().top_non_null ();
      AST_Decl *d = s->lookup_by_name (item->name,
                                       true,
                                       true,
                                       true);
                                       
      if (d == 0)
        {
          idl_global->err ()->lookup_error (item->name);
          
          // Bad IDL, best to avoid a possible crash.
          throw Bailout ();
        }
        
      if (d->node_type () == AST_Decl::NT_typedef)
        {
          d = AST_Typedef::narrow_from_decl (d)->primitive_base_type ();
        }
        
      AST_Template_Interface *i =
        AST_Template_Interface::narrow_from_decl (d);
        
      if (i == 0)
        {
          idl_global->err ()->error1 (UTL_Error::EIDL_TMPL_IFACE_EXPECTED,
                                      i);
                                      
          // Keep going but don't process the bad parent further.                            
          continue;
        }
        
      // Each parent template parameter must match one in the child.  
      if (! this->match_params (i))
        {
          // Keep going but don't process the bad parent further.                            
          continue;
        }
        
      // This part is the same as for a non-template interface.  
      this->compile_one_inheritance (i);
    }
    
  // Same as for non-template interface.  
  this->install_in_header ();
}

bool
FE_Template_InterfaceHeader::match_params (AST_Template_Interface *ti)
{
  for (FE_Utils::T_PARAMLIST_INFO::CONST_ITERATOR i (ti->template_params ());
       !i.done ();
       i.advance ())
    {
      FE_Utils::T_Param_Info *item = 0;
      i.next (item);
      
      bool one_matched = false;

      for (FE_Utils::T_PARAMLIST_INFO::CONST_ITERATOR j (*this->param_info_);
           !j.done ();
           j.advance ())
        {
          FE_Utils::T_Param_Info *my_item = 0;
          j.next (my_item);
          
          if (item->type == my_item->type
              && item->name == my_item->name)
            {
              one_matched = true;
              break;
            }
        }
        
      if (! one_matched)
        {
          idl_global->err ()->error1 (UTL_Error::EIDL_MISMATCHED_T_PARAM,
                                      ti);
          return false;
        }
    }
    
  return true;
}

bool
FE_Template_InterfaceHeader::duplicate_param_id (void)
{
  size_t cur_pos = 0UL;
  size_t size = this->param_info_->size ();
  
  for (FE_Utils::T_PARAMLIST_INFO::CONST_ITERATOR i (*this->param_info_);
       !i.done ();
       i.advance (), ++cur_pos)
    {
      FE_Utils::T_Param_Info *this_one = 0;
      FE_Utils::T_Param_Info *that_one = 0;
      
      i.next (this_one);
      
      for (size_t j = cur_pos + 1; j < size; ++j)
        {
          this->param_info_->get (that_one, j);
          
          if (this_one->name == that_one->name)
            {
              return true;
            }
        }
    }
    
  return false;
}

