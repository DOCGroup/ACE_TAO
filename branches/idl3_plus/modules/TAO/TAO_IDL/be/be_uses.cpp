// $Id$

#include "be_uses.h"
#include "be_interface.h"
#include "be_visitor.h"

be_uses::be_uses (UTL_ScopedName *n,
                  AST_Interface *uses_type,
                  bool is_multiple)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_uses,
              n),
    AST_Field (AST_Decl::NT_uses,
               uses_type,
               n),
    AST_Uses (n,
              uses_type,
              is_multiple),
    be_decl (AST_Decl::NT_uses,
             n)
{
}

be_uses::~be_uses (void)
{
}

int
be_uses::accept (be_visitor *visitor)
{
  return visitor->visit_uses (this);
}

void
be_uses::destroy (void)
{
  this->AST_Uses::destroy ();
  this->be_decl::destroy ();
}

IMPL_NARROW_FROM_DECL(be_uses)
