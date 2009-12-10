// $Id$

#include "be_uses.h"
#include "be_interface.h"
#include "be_visitor.h"

be_uses::be_uses (UTL_ScopedName *n,
                  AST_Type *uses_type,
                  bool is_multiple)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_uses,
              n),
    AST_Field (AST_Decl::NT_uses,
               uses_type,
               n),
    AST_Uses (n,
              uses_type,
              is_multiple),
    be_decl (AST_Decl::NT_uses,
             n),
    be_field (uses_type,
              n)
{
}

be_uses::~be_uses (void)
{
}

be_type *
be_uses::uses_type (void) const
{
  return
    be_type::narrow_from_decl (
      this->AST_Uses::uses_type ());
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
  this->be_field::destroy ();
}

IMPL_NARROW_FROM_DECL (be_uses)
