// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_eventtype_fwd.h
//
// = DESCRIPTION
//    Extension of class be_valuetype_fwd that provides additional
//    means for C++ mapping of an eventtype.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================


#include "be_eventtype_fwd.h"
#include "be_visitor.h"
#include "ast_interface.h"

ACE_RCSID (be, 
           be_eventtype_fwd, 
           "$Id$")

be_eventtype_fwd::be_eventtype_fwd (void)
{
}

be_eventtype_fwd::be_eventtype_fwd (AST_Interface *dummy,
                                    UTL_ScopedName *n)
  : be_valuetype_fwd (dummy,
                      n),
    be_interface_fwd (dummy,
                      n),
    AST_EventTypeFwd (dummy,
                      n),
    AST_ValueTypeFwd (dummy,
                      n),
    AST_InterfaceFwd (dummy,
                      n),
    AST_Decl (AST_Decl::NT_valuetype_fwd,
              n),
    COMMON_Base (dummy->is_local (),
                 dummy->is_abstract ())
{
}

be_eventtype_fwd::~be_eventtype_fwd (void)
{
}

int
be_eventtype_fwd::accept (be_visitor *visitor)
{
  return visitor->visit_eventtype_fwd (this);
}

void
be_eventtype_fwd::destroy (void)
{
  this->be_valuetype_fwd::destroy ();
}

// Narrowing.
IMPL_NARROW_METHODS2 (be_eventtype_fwd, be_valuetype_fwd, AST_EventTypeFwd)
IMPL_NARROW_FROM_DECL (be_eventtype_fwd)

