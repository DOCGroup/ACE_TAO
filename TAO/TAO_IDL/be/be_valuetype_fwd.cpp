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


#include "be_valuetype_fwd.h"
#include "be_visitor.h"
#include "ast_interface.h"

ACE_RCSID (be, 
           be_valuetype_fwd, 
           "$Id$")

be_valuetype_fwd::be_valuetype_fwd (void)
{
}

be_valuetype_fwd::be_valuetype_fwd (AST_Interface *dummy,
                                    UTL_ScopedName *n)
  : be_interface_fwd (dummy,
                      n),
    AST_ValueTypeFwd (dummy,
                      n),
    AST_InterfaceFwd (dummy,
                      n),
    AST_Decl (AST_Decl::NT_valuetype_fwd,
              n)
{
}

be_valuetype_fwd::~be_valuetype_fwd (void)
{
}

int
be_valuetype_fwd::accept (be_visitor *visitor)
{
  return visitor->visit_valuetype_fwd (this);
}

void
be_valuetype_fwd::destroy (void)
{
  this->be_interface_fwd::destroy ();
}

// Narrowing.
IMPL_NARROW_METHODS2 (be_valuetype_fwd, be_interface_fwd, AST_ValueTypeFwd)
IMPL_NARROW_FROM_DECL (be_valuetype_fwd)

