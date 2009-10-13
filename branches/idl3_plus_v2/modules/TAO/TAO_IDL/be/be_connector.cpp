// $Id$

#include "be_connector.h"
#include "be_visitor.h"

ACE_RCSID (be,
           be_connector,
           "$Id$")

be_connector::be_connector (
      UTL_ScopedName *n,
      AST_Connector *base_connector,
      FE_Utils::T_PARAMLIST_INFO *template_params)
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
                   0),
    AST_Template_Common (AST_Decl::NT_connector,
                         n,
                         template_params),
    AST_Connector (n,
                   base_connector,
                   template_params),
    be_scope (AST_Decl::NT_connector),
    be_decl (AST_Decl::NT_connector,
              n),
    be_type (AST_Decl::NT_connector,
             n),
    be_interface (n,
                  0,
                  0,
                  0,
                  0,
                  false,
                  false),
    be_component (n,
                  base_connector,
                  0,
                  0,
                  0,
                  0)
{
}

be_connector::~be_connector (void)
{
}

void
be_connector::destroy (void)
{
  this->AST_Connector::destroy ();
  this->be_component::destroy ();
}

int
be_connector::accept (be_visitor *visitor)
{
  return visitor->visit_connector (this);
}

IMPL_NARROW_FROM_DECL (be_connector)
IMPL_NARROW_FROM_SCOPE (be_connector)

