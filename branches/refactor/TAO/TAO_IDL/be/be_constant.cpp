// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_constant.cpp
//
// = DESCRIPTION
//    Extension of class AST_Constant that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_constant.h"
#include "be_visitor.h"
#include "utl_scope.h"
#include "nr_extern.h"

ACE_RCSID (be, 
           be_constant, 
           "$Id$")

be_constant::be_constant (void)
{
}

be_constant::be_constant (AST_Expression::ExprType et,
			                    AST_Expression *v,
                          UTL_ScopedName *n)
  : AST_Constant (et,
                  v,
                  n),
    AST_Decl (AST_Decl::NT_const,
              n)
{
}

int
be_constant::accept (be_visitor *visitor)
{
  return visitor->visit_constant (this);
}

void
be_constant::destroy (void)
{
  this->AST_Constant::destroy ();
  this->be_decl::destroy ();
}

// Narrowing
IMPL_NARROW_METHODS2 (be_constant, AST_Constant, be_decl)
IMPL_NARROW_FROM_DECL (be_constant)
