// $Id$

#include "fe_porttype_header.h"

FE_PorttypeHeader::FE_PorttypeHeader (UTL_ScopedName *n,
                                      FE_Utils::T_PARAMLIST_INFO *params)
  : FE_TemplateHeader_Common (params),
    name_ (n)
{
}

FE_PorttypeHeader::~FE_PorttypeHeader (void)
{
}

UTL_ScopedName *
FE_PorttypeHeader::name (void) const
{
  return this->name_;
}

void
FE_PorttypeHeader::destroy (void)
{
  if (this->name_ != 0)
    {
      this->name_->destroy ();
      delete this->name_;
      this->name_ = 0;
    }

  this->FE_TemplateHeader_Common::destroy ();
}
