// $Id$

#include "ast_porttype.h"
#include "ast_provides.h"
#include "ast_uses.h"
#include "ast_publishes.h"
#include "ast_emits.h"
#include "ast_consumes.h"
#include "ast_visitor.h"

#include "utl_err.h"
#include "utl_indenter.h"
#include "global_extern.h"

AST_Decl::NodeType const
AST_PortType::NT = AST_Decl::NT_porttype;

AST_PortType::AST_PortType (UTL_ScopedName *n)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_porttype,
              n),
    AST_Type (AST_Decl::NT_porttype,
              n),
    UTL_Scope (AST_Decl::NT_porttype)
{
}

AST_PortType::~AST_PortType (void)
{
}

void
AST_PortType::destroy (void)
{
  this->UTL_Scope::destroy ();
  this->AST_Type::destroy ();
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

AST_Provides *
AST_PortType::fe_add_provides (AST_Provides *p)
{
  return
    AST_Provides::narrow_from_decl (
      this->fe_add_ref_decl (p));
}

AST_Uses *
AST_PortType::fe_add_uses (AST_Uses *u)
{
  return
    AST_Uses::narrow_from_decl (
      this->fe_add_ref_decl (u));
}

AST_Publishes *
AST_PortType::fe_add_publishes (AST_Publishes *p)
{
  return
    AST_Publishes::narrow_from_decl (
      this->fe_add_ref_decl (p));
}

AST_Emits *
AST_PortType::fe_add_emits (AST_Emits *e)
{
  return
    AST_Emits::narrow_from_decl (
      this->fe_add_ref_decl (e));
  return e;
}

AST_Consumes *
AST_PortType::fe_add_consumes (AST_Consumes *c)
{
  return
    AST_Consumes::narrow_from_decl (
      this->fe_add_ref_decl (c));
}

IMPL_NARROW_FROM_DECL (AST_PortType)
IMPL_NARROW_FROM_SCOPE (AST_PortType)

