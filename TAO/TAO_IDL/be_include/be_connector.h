// -*- C++ -*-
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

  virtual ~be_connector ();

  /// Cleanup function.
  virtual void destroy ();

  /// Visiting.
  virtual int accept (be_visitor *visitor);

  bool dds_connector ();
  bool ami_connector ();

private:
  void check_ancestors ();

private:
  bool dds_connector_;
  bool ami_connector_;
};

#endif // BE_CONNECTOR_H
