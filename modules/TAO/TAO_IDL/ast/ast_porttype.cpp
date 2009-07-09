// $Id$

#include "ast_porttype.h"
#include "ast_visitor.h"

#include "utl_identifier.h"
#include "utl_indenter.h"
#include "global_extern.h"

ACE_RCSID (ast,
           ast_porttype,
           "$Id$")

AST_PortType::AST_PortType (
      UTL_ScopedName *n,
      const FE_Utils::T_PARAMLIST_INFO *template_params)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_porttype,
              n),
    template_params_ (*template_params)
{
}

AST_PortType::~AST_PortType (void)
{
}

FE_Utils::T_PARAMLIST_INFO &
AST_PortType::template_params (void)
{
  return this->template_params_;
}

void
AST_PortType::destroy (void)
{
  this->AST_Decl::destroy ();
}

void
AST_PortType::dump (ACE_OSTREAM_TYPE & /* o */)
{
}

int
AST_PortType::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_porttype (this);
}

IMPL_NARROW_FROM_DECL(AST_PortType)
IMPL_NARROW_FROM_SCOPE(AST_PortType)

