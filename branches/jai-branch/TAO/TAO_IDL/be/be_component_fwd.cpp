// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_component_fwd.cpp
//
// = DESCRIPTION
//    Extension of class AST_ComponentFwd that provides additional means for C++
//    mapping of a forward declared component.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include "be_component_fwd.h"
#include "be_visitor.h"

ACE_RCSID (be, 
           be_component_fwd, 
           "$Id$")

be_component_fwd::be_component_fwd (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    AST_InterfaceFwd (),
    AST_ComponentFwd (),
    be_type (),
    be_interface_fwd ()
{
  this->size_type (AST_Type::VARIABLE);
}

be_component_fwd::be_component_fwd (AST_Interface *dummy,
                                    UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_component_fwd,
              n),
    AST_Type (AST_Decl::NT_component_fwd,
              n),
    AST_InterfaceFwd (dummy,
                      n),
    AST_ComponentFwd (dummy,
                      n),
    be_type (AST_Decl::NT_component_fwd,
             n),
    be_interface_fwd (dummy,
                      n)
{
  this->size_type (AST_Type::VARIABLE);
}

be_component_fwd::~be_component_fwd (void)
{
}

void
be_component_fwd::destroy (void)
{
  this->AST_ComponentFwd::destroy ();
}

int
be_component_fwd::accept (be_visitor *visitor)
{
  return visitor->visit_component_fwd (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_component_fwd, AST_ComponentFwd, be_interface_fwd)
IMPL_NARROW_FROM_DECL (be_component_fwd)
