
//=============================================================================
/**
 *  @file    be_native.cpp
 *
 *  $Id$
 *
 *  The native IDL type
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "be_native.h"
#include "be_visitor.h"

be_native::be_native (UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_native,
              n),
    AST_Type (AST_Decl::NT_native,
              n),
    AST_ConcreteType (AST_Decl::NT_native,
                      n),
    UTL_Scope (AST_Decl::NT_native),
    AST_Structure (AST_Decl::NT_native,
                   n,
                   true,
                   false),
    AST_Exception (n,
                   true,
                   false),
    AST_Native (n),
    be_decl (AST_Decl::NT_native,
             n),
    be_type (AST_Decl::NT_native,
             n),
    be_structure (n,
                  true,
                  false),
    be_exception (n,
                  true,
                  false)
{
}

int
be_native::gen_typecode (void)
{
  return 0;
}

long
be_native::tc_size (void)
{
  return 0;
}

void
be_native::destroy (void)
{
  this->be_exception::destroy ();
  this->AST_Native::destroy ();
}

int
be_native::accept (be_visitor *visitor)
{
  return visitor->visit_native (this);
}



IMPL_NARROW_FROM_DECL(be_native)
