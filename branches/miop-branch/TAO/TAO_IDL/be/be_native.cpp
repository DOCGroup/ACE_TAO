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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

ACE_RCSID(be, be_native, "$Id$")


be_native::be_native (void)
{
}

be_native::be_native (UTL_ScopedName *n)
  : AST_Native (n),
    AST_Decl (AST_Decl::NT_native,
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
