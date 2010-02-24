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
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    AST_InterfaceFwd (),
    be_decl (),
    be_type (),
    be_interface_fwd (),
    AST_ValueTypeFwd ()
{
}

be_valuetype_fwd::be_valuetype_fwd (AST_Interface *dummy,
                                    UTL_ScopedName *n)
  : COMMON_Base (dummy->is_local (),
                 dummy->is_abstract ()),
    AST_Decl (AST_Decl::NT_valuetype_fwd,
              n),
    AST_Type (AST_Decl::NT_valuetype_fwd,
              n),
    AST_InterfaceFwd (dummy,
                      n),
    be_decl (AST_Decl::NT_valuetype_fwd,
             n),
    be_type (AST_Decl::NT_valuetype_fwd,
             n),
    be_interface_fwd (dummy,
                      n),
    AST_ValueTypeFwd (dummy,
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



IMPL_NARROW_FROM_DECL (be_valuetype_fwd)

