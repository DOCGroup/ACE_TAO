// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_argument.cpp
//
// = DESCRIPTION
//    Extension of class AST_Argument that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Argument
 */

be_argument::be_argument (void)
{
}

be_argument::be_argument (AST_Argument::Direction d, AST_Type *ft,
                          UTL_ScopedName *n, UTL_StrList *p)
  : AST_Argument (d, ft, n, p),
    AST_Field (AST_Decl::NT_argument, ft, n, p),
    AST_Decl (AST_Decl::NT_argument, n, p)
{
}


int
be_argument::accept (be_visitor *visitor)
{
  return visitor->visit_argument (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_argument, AST_Argument, be_decl)
IMPL_NARROW_FROM_DECL (be_argument)
