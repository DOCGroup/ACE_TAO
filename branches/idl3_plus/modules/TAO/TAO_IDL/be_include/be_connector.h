// -*- C++ -*-
// $Id$

#ifndef BE_CONNECTOR_H
#define BE_CONNECTOR_H

#include "ast_connector.h"
#include "be_component.h"

class be_connector
  : public virtual AST_Connector,
    public virtual be_component
{
public:
  be_connector (UTL_ScopedName *n,
                AST_Connector *base_connector,
                FE_Utils::T_PARAMLIST_INFO *template_params);

  virtual ~be_connector (void);
  
  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL (be_connector);
  DEF_NARROW_FROM_SCOPE (be_connector);

  // Visiting.
  virtual int accept (be_visitor *visitor);
};

#endif // BE_CONNECTOR_H
