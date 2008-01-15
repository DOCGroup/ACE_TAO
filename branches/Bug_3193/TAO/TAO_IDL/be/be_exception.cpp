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


#include "be_exception.h"
#include "be_visitor.h"

#include "global_extern.h"

ACE_RCSID (be,
           be_exception,
           "$Id$")

be_exception::be_exception (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    AST_ConcreteType (),
    UTL_Scope (),
    AST_Structure (),
    AST_Exception (),
    be_scope (),
    be_decl (),
    be_type (),
    be_structure ()
{
  // Always the case.
  this->size_type (AST_Type::VARIABLE);
}

be_exception::be_exception (UTL_ScopedName *n,
                            bool local,
                            bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (AST_Decl::NT_except,
              n),
    AST_Type (AST_Decl::NT_except,
              n),
    AST_ConcreteType (AST_Decl::NT_except,
                      n),
    UTL_Scope (AST_Decl::NT_except),
    AST_Structure (AST_Decl::NT_except,
                   n,
                   local,
                   abstract),
    AST_Exception (n,
                   local,
                   abstract),
    be_scope (AST_Decl::NT_except),
    be_decl (AST_Decl::NT_except,
             n),
    be_type (AST_Decl::NT_except,
             n),
    be_structure (n,
                  local,
                  abstract)
{
  // Always the case.
  this->size_type (AST_Type::VARIABLE);

  if (!this->imported ())
    {
      idl_global->exception_seen_ = true;
    }
}

void
be_exception::destroy (void)
{
  // Call the destroy methods of our base classes.
  this->be_scope::destroy ();
  this->be_type::destroy ();
  this->AST_Exception::destroy ();
}

int
be_exception::accept (be_visitor *visitor)
{
  return visitor->visit_exception (this);
}



IMPL_NARROW_FROM_DECL (be_exception)
IMPL_NARROW_FROM_SCOPE (be_exception)
