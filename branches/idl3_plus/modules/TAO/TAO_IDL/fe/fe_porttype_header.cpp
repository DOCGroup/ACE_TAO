// $Id$

#include "fe_porttype_header.h"

FE_PorttypeHeader::FE_PorttypeHeader (UTL_ScopedName *n,
                                      FE_Utils::T_PARAMLIST_INFO *params,
                                      FE_Utils::EXT_PORTLIST_INFO *ports)
  : FE_TemplateHeader_Common (n, params),
    port_info_ (ports)
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
  // Must destroy the queue element members (but not the elements
  // themselves) before we destroy the queue.
  if (this->port_info_ != 0)
    {
      for (FE_Utils::EXT_PORTLIST_INFO::CONST_ITERATOR i (*this->port_info_);
           !i.done ();
           i.advance ())
        {
          FE_Utils::Ext_Port_Description *info = 0;
          i.next (info);
          info->destroy ();
        }
    }

  delete this->port_info_;
  this->port_info_ = 0;

  this->FE_TemplateHeader_Common::destroy ();
}
