
//=============================================================================
/**
 *  @file    be_finder.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Finder that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#include "be_finder.h"
#include "be_visitor.h"

be_finder::be_finder (UTL_ScopedName *n)
  : COMMON_Base (1,
                 0), //@@ Always local, never abstract
    AST_Decl (AST_Decl::NT_finder,
              n),
    UTL_Scope (AST_Decl::NT_finder),
    AST_Factory (n),
    AST_Finder (n),
    be_scope (AST_Decl::NT_finder),
    be_decl (AST_Decl::NT_finder,
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
