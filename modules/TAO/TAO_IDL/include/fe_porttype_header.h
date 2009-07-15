// $Id$

#ifndef FE_PORTTYPE_HEADER_H
#define FE_PORTTYPE_HEADER_H

#include "fe_template_header_common.h"

class FE_PorttypeHeader : public FE_TemplateHeader_Common
{
public:
  FE_PorttypeHeader (UTL_ScopedName *n,
                     FE_Utils::T_PARAMLIST_INFO *params,
                     FE_Utils::EXT_PORTLIST_INFO *ports);

  virtual ~FE_PorttypeHeader (void);

  UTL_ScopedName *name (void) const;

  virtual void destroy (void);

private:
  FE_Utils::EXT_PORTLIST_INFO *port_info_;
};

#endif // FE_PORTTYPE_HEADER_H

