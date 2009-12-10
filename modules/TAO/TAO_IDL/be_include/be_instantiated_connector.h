// -*- C++ -*-
// $Id$

#ifndef BE_INSTANTIATED_CONNECTOR_H
#define BE_INSTANTIATED_CONNECTOR_H

#include "ast_instantiated_connector.h"

#include "be_type.h"
#include "be_scope.h"

class be_instantiated_connector
  : public virtual AST_Instantiated_Connector,
    public virtual be_type,
    public virtual be_scope
{
public:
  be_instantiated_connector (
    UTL_ScopedName *n,
    AST_Connector *connector_type,
    AST_Template_Common::T_ARGLIST *template_args);

  virtual ~be_instantiated_connector (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL (be_instantiated_connector);
  DEF_NARROW_FROM_SCOPE (be_instantiated_connector);

  // Visiting.
  virtual int accept (be_visitor *visitor);
};

#endif /* BE_INSTANTIATED_CONNECTOR_H */

