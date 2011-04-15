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
                AST_Connector *base_connector);

  virtual ~be_connector (void);

  /// Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL (be_connector);
  DEF_NARROW_FROM_SCOPE (be_connector);

  /// Visiting.
  virtual int accept (be_visitor *visitor);

  bool dds_connector (void);
  bool ami_connector (void);

private:
  void check_ancestors (void);

private:
  bool dds_connector_;
  bool ami_connector_;
};

#endif // BE_CONNECTOR_H
