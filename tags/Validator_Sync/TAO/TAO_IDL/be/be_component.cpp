// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_component.cpp
//
// = DESCRIPTION
//    Extension of class AST_Component that provides additional means for C++
//    mapping of an interface.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include "be_component.h"
#include "be_visitor.h"

ACE_RCSID (be, 
           be_component, 
           "$Id$")

be_component::be_component (void)
{
  this->size_type (AST_Type::VARIABLE);
  this->has_constructor (I_TRUE);
}

be_component::be_component (UTL_ScopedName *n,
                            AST_Component *base_component,
                            AST_Interface **supports,
                            long n_supports,
                            AST_Interface **supports_flat,
                            long n_supports_flat)
  : be_interface (n,
                  supports,
                  n_supports,
                  supports_flat,
                  n_supports_flat,
                  I_FALSE,
                  I_FALSE),
    AST_Component (n,
                   base_component,
                   supports,
                   n_supports,
                   supports_flat,
                   n_supports_flat),
    AST_Interface (n,
                   supports,
                   n_supports,
                   supports_flat,
                   n_supports_flat,
                   I_FALSE,
                   I_FALSE),
    AST_Type (AST_Decl::NT_component,
              n),
    AST_Decl (AST_Decl::NT_component,
              n),
    UTL_Scope (AST_Decl::NT_component),
    COMMON_Base (I_FALSE,
                 I_FALSE)
{
  this->size_type (AST_Type::VARIABLE);
  this->has_constructor (I_TRUE);
}

be_component::~be_component (void)
{
}

void
be_component::destroy ()
{
  this->be_interface::destroy ();
  this->AST_Component::destroy ();
}

int
be_component::accept (be_visitor *visitor)
{
  return visitor->visit_component (this);
}

AST_Structure *
be_component::be_add_structure (AST_Structure *t)
{
  return this->fe_add_structure (t);
}

AST_Typedef *
be_component::be_add_typedef (AST_Typedef *t)
{
  return this->fe_add_typedef (t);
}

// Narrowing.
IMPL_NARROW_METHODS2 (be_component, be_interface, AST_Component)
IMPL_NARROW_FROM_DECL (be_component)
IMPL_NARROW_FROM_SCOPE (be_component)
