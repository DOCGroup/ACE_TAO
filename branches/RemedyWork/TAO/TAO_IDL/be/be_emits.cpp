// $Id$

#include "be_emits.h"
#include "be_eventtype.h"
#include "be_visitor.h"

be_emits::be_emits (UTL_ScopedName *n,
                    AST_Type *emits_type)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_emits,
              n),
    AST_Field (AST_Decl::NT_emits,
               emits_type,
               n),
    AST_Emits (n,
               emits_type),
    be_decl (AST_Decl::NT_emits,
             n),
    be_field (emits_type,
              n)
{
}

be_emits::~be_emits (void)
{
}

be_eventtype *
be_emits::emits_type (void) const
{
  return
    be_eventtype::narrow_from_decl (
      this->AST_Emits::emits_type ());
}

int
be_emits::accept (be_visitor *visitor)
{
  return visitor->visit_emits (this);
}

void
be_emits::destroy (void)
{
  this->AST_Emits::destroy ();
  this->be_field::destroy ();
}

IMPL_NARROW_FROM_DECL (be_emits)
