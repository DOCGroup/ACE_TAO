// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_enum.cpp
//
// = DESCRIPTION
//    Extension of class AST_Enum that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_enum.h"
#include "be_visitor.h"

#include "global_extern.h"

ACE_RCSID (be,
           be_enum,
           "$Id$")

be_enum::be_enum (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    AST_ConcreteType (),
    UTL_Scope (),
    AST_Enum (),
    be_scope (),
    be_decl (),
    be_type ()
{
}

be_enum::be_enum (UTL_ScopedName *n,
                  bool local,
                  bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (AST_Decl::NT_enum,
              n),
    AST_Type (AST_Decl::NT_enum,
              n),
    AST_ConcreteType (AST_Decl::NT_enum,
                      n),
    UTL_Scope (AST_Decl::NT_enum),
    AST_Enum (n,
              local,
              abstract),
    be_scope (AST_Decl::NT_enum),
    be_decl (AST_Decl::NT_enum,
             n),
    be_type (AST_Decl::NT_enum,
             n)
{
  if (!this->imported ())
    {
      idl_global->enum_seen_ = true;
    }
}

void
be_enum::destroy (void)
{
  // Call the destroy methods of our base classes.
  // No need to call be_scope::destroy(). It has no
  // allocated members, and AST_Enum::destroy() will
  // call UTL_Scope::destroy().
  this->be_type::destroy ();
  this->be_scope::destroy ();
  this->AST_Enum::destroy ();
}

int
be_enum::accept (be_visitor *visitor)
{
  return visitor->visit_enum (this);
}

// Narrowing
IMPL_NARROW_METHODS3 (be_enum, AST_Enum, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_enum)
IMPL_NARROW_FROM_SCOPE (be_enum)
