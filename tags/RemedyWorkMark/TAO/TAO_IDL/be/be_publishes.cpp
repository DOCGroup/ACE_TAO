// $Id$

#include "be_publishes.h"
#include "be_eventtype.h"
#include "be_visitor.h"

be_publishes::be_publishes (UTL_ScopedName *n,
                            AST_Type *publishes_type)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_publishes,
              n),
    AST_Field (AST_Decl::NT_publishes,
               publishes_type,
               n),
    AST_Publishes (n,
                  publishes_type),
    be_decl (AST_Decl::NT_publishes,
             n),
    be_field (publishes_type,
              n)
{
}

be_publishes::~be_publishes (void)
{
}

be_eventtype *
be_publishes::publishes_type (void) const
{
  return
    be_eventtype::narrow_from_decl (
      this->AST_Publishes::publishes_type ());
}

int
be_publishes::accept (be_visitor *visitor)
{
  return visitor->visit_publishes (this);
}

void
be_publishes::destroy (void)
{
  this->AST_Publishes::destroy ();
  this->be_field::destroy ();
}

IMPL_NARROW_FROM_DECL (be_publishes)
