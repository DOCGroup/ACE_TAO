#include "ast_porttype.h"
#include "ast_provides.h"
#include "ast_uses.h"
#include "ast_attribute.h"
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

AST_PortType::~AST_PortType ()
{
}

void
AST_PortType::destroy ()
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
  return dynamic_cast<AST_Provides*> (this->fe_add_ref_decl (p));
}

AST_Uses *
AST_PortType::fe_add_uses (AST_Uses *u)
{
  return dynamic_cast<AST_Uses*> (this->fe_add_ref_decl (u));
}

AST_Attribute *
AST_PortType::fe_add_attribute (AST_Attribute *t)
{
  return dynamic_cast<AST_Attribute*> (this->fe_add_decl (t));
}
