// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_valuetype_fwd.h
//
// = DESCRIPTION
//    Extension of class be_interface_fwd that provides additional
//    means for C++ mapping of an valuetype.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//    changed for valuetypes by Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//
// ============================================================================


#include "idl.h"
#include "idl_extern.h"
#include "be.h"

ACE_RCSID(be, be_valuetype_fwd, "$Id$")

be_valuetype_fwd::be_valuetype_fwd (void)
{
}

be_valuetype_fwd::be_valuetype_fwd (AST_Interface *dummy,
                                    UTL_ScopedName *n)
  : be_interface_fwd (dummy, 
                      n),
    AST_InterfaceFwd (dummy, 
                      n),
    AST_Decl (AST_Decl::NT_interface_fwd, 
              n)
{
}

be_valuetype_fwd::~be_valuetype_fwd (void)
{
}


void
be_valuetype_fwd::set_abstract_valuetype (void)
{
  this->full_definition ()->set_abstract_valuetype ();
}

int
be_valuetype_fwd::accept (be_visitor *visitor)
{
  return visitor->visit_valuetype_fwd (this);
}

// Narrowing.
IMPL_NARROW_METHODS1 (be_valuetype_fwd, be_interface_fwd)
IMPL_NARROW_FROM_DECL (be_valuetype_fwd)

