// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    be_finder.cpp
//
// = DESCRIPTION
//    Extension of class AST_Finder that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include "be_finder.h"
#include "be_visitor.h"

be_finder::be_finder (UTL_ScopedName *n)
  : COMMON_Base (1,
                 0), //@@ Always local, never abstract
    AST_Decl (AST_Decl::NT_factory,
              n),
    UTL_Scope (AST_Decl::NT_factory),
    AST_Factory (n),
    AST_Finder (n),
    be_scope (AST_Decl::NT_factory),
    be_decl (AST_Decl::NT_factory,
             n),
    be_factory (n)
{
}

be_finder::~be_finder (void)
{
}

void
be_finder::destroy (void)
{
  // Call the destroy methods of our base classes.
  this->be_factory::destroy ();
  this->AST_Finder::destroy ();
}

int
be_finder::accept (be_visitor *visitor)
{
  return visitor->visit_finder (this);
}

IMPL_NARROW_FROM_DECL (be_finder)
IMPL_NARROW_FROM_SCOPE (be_finder)
