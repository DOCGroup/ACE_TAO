// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_module.cpp
//
// = DESCRIPTION
//    Extension of class AST_Module that provides additional means for C++
//    mapping of a module
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

ACE_RCSID(be, be_module, "$Id$")

/*
 * BE_Module
 */
be_module::be_module (void)
{
}

be_module::be_module (UTL_ScopedName *n, UTL_StrList *p)
  : AST_Decl (AST_Decl::NT_module, n, p),
    UTL_Scope (AST_Decl::NT_module)
{
}

// compute the size type of the node in question
int
be_module::compute_size_type (void)
{
  // our size does not matter
  return 0;
}

int
be_module::accept (be_visitor *visitor)
{
  return visitor->visit_module (this);
}

// Narrowing
IMPL_NARROW_METHODS3 (be_module, AST_Module, be_scope, be_decl)
IMPL_NARROW_FROM_DECL (be_module)
IMPL_NARROW_FROM_SCOPE (be_module)
