// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_union_fwd.cpp
//
// = DESCRIPTION
//    Extension of class AST_UnionFwd that provides additional means for C++
//    mapping of a union.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include "be_union_fwd.h"
#include "be_visitor.h"

ACE_RCSID (be, 
           be_union_fwd, 
           "$Id$")

be_union_fwd::be_union_fwd (void)
{
}

be_union_fwd::be_union_fwd (UTL_ScopedName *n)
  : be_type (AST_Decl::NT_union_fwd,
             n),
    AST_UnionFwd (n),
    AST_Type (AST_Decl::NT_union_fwd,
              n),
    AST_Decl (AST_Decl::NT_union_fwd,
              n)
{
}

be_union_fwd::~be_union_fwd (void)
{
}

void
be_union_fwd::destroy (void)
{
  // Do nothing.
}

int
be_union_fwd::accept (be_visitor *visitor)
{
  return visitor->visit_union_fwd (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_union_fwd, AST_UnionFwd, be_type)
IMPL_NARROW_FROM_DECL (be_union_fwd)
