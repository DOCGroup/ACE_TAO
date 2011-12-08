
//=============================================================================
/**
 *  @file    be_root.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Root that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#include "be_root.h"
#include "be_visitor.h"
#include "ast_sequence.h"
#include "ast_string.h"
#include "ast_array.h"

be_root::be_root (UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_root,
              n),
    UTL_Scope (AST_Decl::NT_root),
    AST_Module (n),
    AST_Root (n),
    be_scope (AST_Decl::NT_root),
    be_decl (AST_Decl::NT_root,
             n),
    be_module (n)
{
}

be_root::~be_root (void)
{
}

void
be_root::destroy (void)
{
  // Call the destroy methods of our base classes.

// The scope of the root is handled specially, since we may
// be processing multiple IDL files and we want to keep
// the predefined types around until we are all done.
//  be_scope::destroy ();
//  be_decl::destroy ();

  AST_Root::destroy ();
}

int
be_root::accept (be_visitor *visitor)
{
  return visitor->visit_root (this);
}



IMPL_NARROW_FROM_DECL (be_root)
IMPL_NARROW_FROM_SCOPE (be_root)
