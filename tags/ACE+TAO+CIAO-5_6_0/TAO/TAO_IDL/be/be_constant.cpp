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
  : COMMON_Base (),
    AST_Decl (),
    AST_Constant (),
    be_decl ()
{
}

be_constant::be_constant (AST_Expression::ExprType et,
                          AST_Expression *v,
                          UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_const, n),
    AST_Constant (et, v, n),
    be_decl (AST_Decl::NT_const, n)
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
  this->be_decl::destroy ();
  this->AST_Constant::destroy ();
}



IMPL_NARROW_FROM_DECL (be_constant)
