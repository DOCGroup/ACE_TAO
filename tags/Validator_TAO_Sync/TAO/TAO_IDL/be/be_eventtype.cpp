// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_eventtype.cpp
//
// = DESCRIPTION
//    Extension of class AST_EventType and be_valuetype that provides
//    additional means for C++ mapping of an eventtype.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include "be_eventtype.h"
#include "be_visitor.h"

ACE_RCSID (be,
           be_eventtype,
           "$Id$")

// Default constructor.
be_eventtype::be_eventtype (void)
{
}

// Constructor used to build the AST.
be_eventtype::be_eventtype (UTL_ScopedName *n,
                            AST_Interface **inherits,
                            long n_inherits,
                            AST_ValueType *inherits_concrete,
                            AST_Interface **inherits_flat,
                            long n_inherits_flat,
                            AST_Interface **supports,
                            long n_supports,
                            AST_Interface *supports_concrete,
                            idl_bool abstract,
                            idl_bool truncatable)
  : be_valuetype (n,
                  inherits,
                  n_inherits,
                  inherits_concrete,
                  inherits_flat,
                  n_inherits_flat,
                  supports,
                  n_supports,
                  supports_concrete,
                  abstract,
                  truncatable),
    be_interface (n,
                  inherits,
                  n_inherits,
                  inherits_flat,
                  n_inherits_flat,
                  0,
                  abstract),
    AST_ValueType (n,
                   inherits,
                   n_inherits,
                   inherits_concrete,
                   inherits_flat,
                   n_inherits_flat,
                   supports,
                   n_supports,
                   supports_concrete,
                   abstract,
                   truncatable),
    AST_Interface (n,
                   inherits,
                   n_inherits,
                   inherits_flat,
                   n_inherits_flat,
                   0,
                   abstract),
    AST_Decl (AST_Decl::NT_eventtype,
              n),
    UTL_Scope (AST_Decl::NT_eventtype),
    COMMON_Base (0,
                 abstract)
{
}

be_eventtype::~be_eventtype (void)
{
}

// Accept a visitor.
int
be_eventtype::accept (be_visitor *visitor)
{
  return visitor->visit_eventtype (this);
}

void
be_eventtype::destroy (void)
{
  this->be_valuetype::destroy ();
}

// Narrowing.
IMPL_NARROW_METHODS2 (be_eventtype, be_valuetype, AST_EventType)
IMPL_NARROW_FROM_DECL (be_eventtype)
IMPL_NARROW_FROM_SCOPE (be_eventtype)
