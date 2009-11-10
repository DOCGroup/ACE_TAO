// -*- C++ -*-
// $Id$

#ifndef AST_INSTANTIATED_CONNECTOR_H
#define AST_INSTANTIATED_CONNECTOR_H

#include "ast_concrete_type.h"
#include "ast_template_common.h"
#include "utl_scope.h"

class AST_Connector;
class UTL_NameListl;

class TAO_IDL_FE_Export AST_Instantiated_Connector
  : public virtual AST_ConcreteType,
    public virtual UTL_Scope
{
public:
  AST_Instantiated_Connector (
    UTL_ScopedName *n,
    AST_Connector *connector_type,
    AST_Template_Common::T_ARGLIST *template_args);

  virtual ~AST_Instantiated_Connector (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL (AST_Instantiated_Connector);
  DEF_NARROW_FROM_SCOPE (AST_Instantiated_Connector);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

private:
  AST_Connector *connector_type_;
  AST_Template_Common::T_ARGLIST *template_args_;
};

#endif /* AST_INSTANTIATED_INTERFACE_H */

