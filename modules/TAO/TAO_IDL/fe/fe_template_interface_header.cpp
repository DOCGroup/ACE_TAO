// $Id$

#include "fe_template_interface_header.h"

FE_Template_InterfaceHeader::FE_Template_InterfaceHeader (UTL_ScopedName *n)
  : pd_interface_name (n),
//    pd_inherits (0),
    pd_n_inherits (0),
//    pd_inherits_flat (0),
    pd_n_inherits_flat (0)
{
//  this->compile_inheritance (inherits, false);
}

FE_Template_InterfaceHeader::~FE_Template_InterfaceHeader (void)
{
}

void
FE_Template_InterfaceHeader::destroy (void)
{
  if (0 != this->pd_interface_name)
    {
      this->pd_interface_name->destroy ();
      delete this->pd_interface_name;
      this->pd_interface_name = 0;
    }
}

// Data accessors.

UTL_ScopedName *
FE_Template_InterfaceHeader::name (void) const
{
  return this->pd_interface_name;
}

AST_Template_Interface **
FE_Template_InterfaceHeader::inherits (void) const
{
  return this->pd_inherits;
}

long
FE_Template_InterfaceHeader::n_inherits (void) const
{
  return this->pd_n_inherits;
}

AST_Template_Interface **
FE_Template_InterfaceHeader::inherits_flat (void) const
{
  return this->pd_inherits_flat;
}

long
FE_Template_InterfaceHeader::n_inherits_flat (void) const
{
  return this->pd_n_inherits_flat;
}

