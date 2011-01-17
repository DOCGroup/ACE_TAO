
//=============================================================================
/**
 *  @file    be_constant.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Constant that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#include "be_constant.h"
#include "be_visitor.h"
#include "utl_scope.h"
#include "nr_extern.h"

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
