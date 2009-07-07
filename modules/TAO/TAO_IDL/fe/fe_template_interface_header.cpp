// $Id$

#include "fe_template_interface_header.h"

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
      for (FE_Utils::T_REFLIST_INFO::ITERATOR i (*this->parent_info_);
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
    
  for (FE_Utils::T_REFLIST_INFO::ITERATOR i (*this->parent_info_);
       !i.done ();
       i.advance ())
    {
    }
}

