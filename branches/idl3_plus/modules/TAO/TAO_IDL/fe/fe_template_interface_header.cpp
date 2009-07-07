// $Id$

#include "fe_template_interface_header.h"

FE_Template_InterfaceHeader::FE_Template_InterfaceHeader (
      UTL_ScopedName *n,
      FE_Utils::T_PARAMLIST_INFO *params,
      FE_Utils::T_REFLIST_INFO *inherited)
  : interface_name_ (n),
    param_info_ (params),
    parent_info_ (inherited),
    inherits_ (0),
    n_inherits_ (0),
    inherits_flat_ (0),
    n_inherits_flat_ (0)
{
//  this->compile_inheritance (inherits, false);
}

FE_Template_InterfaceHeader::~FE_Template_InterfaceHeader (void)
{
}

void
FE_Template_InterfaceHeader::destroy (void)
{
  if (0 != this->interface_name_)
    {
      this->interface_name_->destroy ();
      delete this->interface_name_;
      this->interface_name_ = 0;
    }

  // Queue element members have self-managed memory.
  delete this->param_info_;
  this->param_info_ = 0;

  // Queue element destructor cleans up element members.
  delete this->parent_info_;
  this->parent_info_ = 0;
}

// Data accessors.

UTL_ScopedName *
FE_Template_InterfaceHeader::name (void) const
{
  return this->interface_name_;
}

AST_Template_Interface **
FE_Template_InterfaceHeader::inherits (void) const
{
  return this->inherits_;
}

long
FE_Template_InterfaceHeader::n_inherits (void) const
{
  return this->n_inherits_;
}

AST_Template_Interface **
FE_Template_InterfaceHeader::inherits_flat (void) const
{
  return this->inherits_flat_;
}

long
FE_Template_InterfaceHeader::n_inherits_flat (void) const
{
  return this->n_inherits_flat_;
}

FE_Utils::T_PARAMLIST_INFO *
FE_Template_InterfaceHeader::param_info (void) const
{
  return this->param_info_;
}

