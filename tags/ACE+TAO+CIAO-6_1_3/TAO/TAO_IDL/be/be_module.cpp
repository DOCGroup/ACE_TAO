//=============================================================================
/**
 *  @file    be_module.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Module that provides additional means for C++
 *  mapping of a module
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#include "be_module.h"
#include "be_visitor.h"

be_module::be_module (UTL_ScopedName *n, AST_Module *previous)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_module, n),
    UTL_Scope (AST_Decl::NT_module),
    AST_Module (n, previous),
    be_scope (AST_Decl::NT_module),
    be_decl (AST_Decl::NT_module, n)
{
}

void
be_module::destroy (void)
{
  // Call the destroy methods of our base classes.
  this->be_scope::destroy ();
  this->be_decl::destroy ();
  this->AST_Module::destroy ();
}

int
be_module::accept (be_visitor *visitor)
{
  return visitor->visit_module (this);
}

IMPL_NARROW_FROM_DECL (be_module)
IMPL_NARROW_FROM_SCOPE (be_module)
