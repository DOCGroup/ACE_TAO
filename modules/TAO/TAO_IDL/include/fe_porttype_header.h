// $Id$

#ifndef FE_PORTTYPE_HEADER_H
#define FE_PORTTYPE_HEADER_H

#include "fe_template_header_common.h"

class FE_PorttypeHeader : public FE_TemplateHeader_Common
{
public:
  FE_PorttypeHeader (UTL_ScopedName *n,
                     FE_Utils::T_PARAMLIST_INFO *params);

  virtual ~FE_PorttypeHeader (void);

  UTL_ScopedName *name (void) const;

  virtual void destroy (void);
  
private:
  UTL_ScopedName *name_;
};

#endif // FE_PORTTYPE_HEADER_H

