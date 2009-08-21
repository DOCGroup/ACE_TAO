// $Id$

#include "ast_instantiated_connector.h"
#include "ast_visitor.h"

#include "utl_identifier.h"
#include "utl_indenter.h"
#include "utl_namelist.h"
#include "global_extern.h"

ACE_RCSID (ast,
           ast_instantiated_connector,
           "$Id$")

AST_Instantiated_Connector::AST_Instantiated_Connector (
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
    UTL_Scope (AST_Decl::NT_connector),
    connector_type_ (connector_type),
    template_args_ (template_args)
{
}

AST_Instantiated_Connector::~AST_Instantiated_Connector (void)
{
}

void
AST_Instantiated_Connector::destroy (void)
{
  delete this->template_args_;
  this->template_args_ = 0;
  
  this->AST_ConcreteType::destroy ();
  this->UTL_Scope::destroy ();
}

void
AST_Instantiated_Connector::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "connector ");

  this->local_name ()->dump (o);
  this->dump_i (o, "<");
  unsigned long index = 0UL;
  
  // TODO - template args

  this->dump_i (o, " {\n");

  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);

  this->dump_i (o, "}\n");
}

int
AST_Instantiated_Connector::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_instantiated_connector (this);
}

IMPL_NARROW_FROM_DECL (AST_Instantiated_Connector)
IMPL_NARROW_FROM_SCOPE (AST_Instantiated_Connector)

