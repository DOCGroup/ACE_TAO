// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_native.cpp
//
// = DESCRIPTION
//    The native IDL type
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_native.h"
#include "be_visitor.h"

ACE_RCSID (be, 
           be_native, 
           "$Id$")


be_native::be_native (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    AST_Native (),
    be_decl (),
    be_type ()
{
}

be_native::be_native (UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_native,
              n),
    AST_Type (AST_Decl::NT_native,
              n),
    AST_Native (n),
    be_decl (AST_Decl::NT_native,
             n),
    be_type (AST_Decl::NT_native,
             n)
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

int
be_native::accept (be_visitor *visitor)
{
  return visitor->visit_native (this);
}

// Narrowing
IMPL_NARROW_METHODS2(be_native, AST_Native, be_type)
IMPL_NARROW_FROM_DECL(be_native)
