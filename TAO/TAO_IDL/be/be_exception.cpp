// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_exception.cpp
//
// = DESCRIPTION
//    Extension of class AST_Exception that provides additional means for C++
//    mapping of an interface.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================


#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

ACE_RCSID(be, be_exception, "$Id$")

be_exception::be_exception (void)
{
  this->size_type (be_decl::VARIABLE); // always the case
}

be_exception::be_exception (UTL_ScopedName *n,
                            idl_bool local,
                            idl_bool abstract)
  : AST_Decl (AST_Decl::NT_except, 
              n),
    AST_Structure (AST_Decl::NT_except, 
                   n, 
                   local, 
                   abstract),
    UTL_Scope (AST_Decl::NT_except),
    COMMON_Base (local, 
                 abstract)
{
  // Always the case.
  this->size_type (be_decl::VARIABLE);
}

void
be_exception::destroy (void)
{
  // Call the destroy methods of our base classes.
  be_scope::destroy ();
  be_type::destroy ();
}

int
be_exception::accept (be_visitor *visitor)
{
  return visitor->visit_exception (this);
}

// Narrowing
IMPL_NARROW_METHODS3 (be_exception, AST_Exception, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_exception)
IMPL_NARROW_FROM_SCOPE (be_exception)
