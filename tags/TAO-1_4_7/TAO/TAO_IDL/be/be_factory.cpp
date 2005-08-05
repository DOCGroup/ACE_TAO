// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_factory.cpp
//
// = DESCRIPTION
//    Extension of class AST_Factory that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Boris Kolpackov <bosk@ipmce.ru>
//
// ============================================================================

#include "be_factory.h"
#include "be_visitor.h"

ACE_RCSID (be, 
           be_factory, 
           "$Id$")

be_factory::be_factory (void)
  : COMMON_Base (),
    AST_Decl (),
    UTL_Scope (),
    AST_Factory (),
    be_scope (),
    be_decl ()
{
}

be_factory::be_factory (UTL_ScopedName *n)
  : COMMON_Base (1,
                 0), //@@ Always local, never abstract
    AST_Decl (AST_Decl::NT_factory,
              n),
    UTL_Scope (AST_Decl::NT_factory),
    AST_Factory (n),
    be_scope (AST_Decl::NT_factory),
    be_decl (AST_Decl::NT_factory,
             n)
{
}

be_factory::~be_factory (void)
{
}

void
be_factory::destroy (void)
{
  // Call the destroy methods of our base classes.
  be_scope::destroy ();
  be_decl::destroy ();
}

int
be_factory::accept (be_visitor *visitor)
{
  return visitor->visit_factory (this);
}

// Narrowing
IMPL_NARROW_METHODS3 (be_factory, AST_Factory, be_scope, be_decl)
IMPL_NARROW_FROM_DECL (be_factory)
IMPL_NARROW_FROM_SCOPE (be_factory)
