// $Id$

#include "ast_connector.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "utl_indenter.h"
#include "utl_err.h"
#include "global_extern.h"

AST_Decl::NodeType const
AST_Connector::NT = AST_Decl::NT_connector;

AST_Connector::AST_Connector (
      UTL_ScopedName *n,
      AST_Connector *base_connector)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_connector,
              n),
    AST_Type (AST_Decl::NT_connector,
              n),
    UTL_Scope (AST_Decl::NT_connector),
    AST_Interface (n,
                   0,
                   0,
                   0,
                   0,
                   false,
                   false),
    AST_Component (n,
                   base_connector,
                   0,
                   0,
                   0,
                   0)
{
  if (!this->imported ())
    {
      idl_global->connector_seen_ = true;
    }
}

AST_Connector::~AST_Connector (void)
{
}

AST_Connector *
AST_Connector::base_connector (void) const
{
  return AST_Connector::narrow_from_decl (this->pd_base_component);
}

void
AST_Connector::destroy (void)
{
  this->AST_Component::destroy ();
}

void
AST_Connector::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "connector ");

  this->local_name ()->dump (o);

  this->dump_i (o, " ");

  if (this->pd_base_component != 0)
    {
      this->dump_i (o, ": ");
      this->pd_base_component->local_name ()->dump (o);
    }

  this->dump_i (o, " {\n");

  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);

  this->dump_i (o, "}");
}

int
AST_Connector::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_connector (this);
}

IMPL_NARROW_FROM_DECL (AST_Connector)
IMPL_NARROW_FROM_SCOPE (AST_Connector)
