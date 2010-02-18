// $Id$

#include "be_porttype.h"
#include "be_visitor.h"

be_porttype::be_porttype (UTL_ScopedName *n)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_porttype,
              n),
    AST_Type (AST_Decl::NT_porttype,
              n),
    UTL_Scope (AST_Decl::NT_porttype),
    AST_PortType (n),
    be_scope (AST_Decl::NT_porttype),
    be_decl (AST_Decl::NT_porttype,
             n),
    be_type (AST_Decl::NT_porttype,
             n)
{
}

be_porttype::~be_porttype (void)
{
}

void
be_porttype::destroy (void)
{
  this->AST_PortType::destroy ();
}

int
be_porttype::accept (be_visitor *visitor)
{
  return visitor->visit_porttype (this);
}

IMPL_NARROW_FROM_DECL (be_porttype)
IMPL_NARROW_FROM_SCOPE (be_porttype)
