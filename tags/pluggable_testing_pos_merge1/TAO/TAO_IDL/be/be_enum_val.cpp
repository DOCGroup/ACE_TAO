// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_enum_val.cpp
//
// = DESCRIPTION
//    Extension of class AST_EnumVal that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

ACE_RCSID(be, be_enum_val, "$Id$")

be_enum_val::be_enum_val (void)
{
}

be_enum_val::be_enum_val (unsigned long v, UTL_ScopedName *n, UTL_StrList *p)
  : AST_Constant (AST_Expression::EV_ulong,
                  AST_Decl::NT_enum_val,
                  new AST_Expression(v),
                  n,
                  p),
    AST_Decl (AST_Decl::NT_enum_val, n, p)
{
}

int
be_enum_val::accept (be_visitor *visitor)
{
  return visitor->visit_enum_val (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_enum_val, AST_EnumVal, be_decl)
IMPL_NARROW_FROM_DECL (be_enum_val)
