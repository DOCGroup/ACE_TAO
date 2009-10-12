// $Id$

#include "be_instantiated_connector.h"
#include "be_visitor.h"

be_instantiated_connector::be_instantiated_connector (
      UTL_ScopedName *n,
      AST_Connector *connector_type,
      AST_Template_Common::T_ARGLIST *template_args)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_connector,
              n),
    AST_Type (AST_Decl::NT_connector,
              n),
    AST_ConcreteType (AST_Decl::NT_connector,
                      n),
    UTL_Scope (AST_Decl::NT_interface),
    AST_Instantiated_Connector (n,
                                connector_type,
                                template_args),
    be_type (AST_Decl::NT_connector,
             n),
    be_scope (AST_Decl::NT_connector)
{
}

be_instantiated_connector::~be_instantiated_connector (void)
{
}

void
be_instantiated_connector::destroy (void)
{
  this->AST_Instantiated_Connector::destroy ();
  this->be_type::destroy ();
  this->be_scope::destroy ();
}

int
be_instantiated_connector::accept (be_visitor *visitor)
{
  return visitor->visit_instantiated_connector (this);
}

IMPL_NARROW_FROM_DECL (be_instantiated_connector)
IMPL_NARROW_FROM_SCOPE (be_instantiated_connector)

