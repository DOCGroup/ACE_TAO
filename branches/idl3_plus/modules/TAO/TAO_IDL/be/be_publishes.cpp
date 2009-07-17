// $Id$

#include "be_publishes.h"
#include "be_eventtype.h"
#include "be_visitor.h"

be_publishes::be_publishes (UTL_ScopedName *n,
                            be_eventtype *publishes_type)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_publishes,
              n),
    AST_Field (AST_Decl::NT_publishes,
               publishes_type,
               n),
    AST_Publishes (n,
                  publishes_type),
    be_decl (AST_Decl::NT_publishes,
             n)
{
}

be_publishes::~be_publishes (void)
{
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
  this->be_decl::destroy ();
}

IMPL_NARROW_FROM_DECL(be_publishes)
