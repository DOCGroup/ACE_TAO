
//=============================================================================
/**
 *  @file    be_eventtype.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_EventType and be_valuetype that provides
 *  additional means for C++ mapping of an eventtype.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "be_eventtype.h"
#include "be_visitor.h"

#include "global_extern.h"

be_eventtype::be_eventtype (UTL_ScopedName *n,
                            AST_Type **inherits,
                            long n_inherits,
                            AST_Type *inherits_concrete,
                            AST_Interface **inherits_flat,
                            long n_inherits_flat,
                            AST_Type **supports,
                            long n_supports,
                            AST_Type *supports_concrete,
                            bool abstract,
                            bool truncatable,
                            bool custom)
  : COMMON_Base (0,
                 abstract),
    AST_Decl (AST_Decl::NT_eventtype,
              n),
    AST_Type (AST_Decl::NT_eventtype,
              n),
    UTL_Scope (AST_Decl::NT_eventtype),
    AST_Interface (n,
                   inherits,
                   n_inherits,
                   inherits_flat,
                   n_inherits_flat,
                   0,
                   abstract),
    be_scope (AST_Decl::NT_eventtype),
    be_decl (AST_Decl::NT_eventtype,
             n),
    be_type (AST_Decl::NT_eventtype,
             n),
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
                   truncatable,
                   custom),
    be_valuetype (n,
                  inherits,
                  n_inherits,
                  inherits_concrete,
                  inherits_flat,
                  n_inherits_flat,
                  supports,
                  n_supports,
                  supports_concrete,
                  abstract,
                  truncatable,
                  custom),
    AST_EventType (n,
                   inherits,
                   n_inherits,
                   inherits_concrete,
                   inherits_flat,
                   n_inherits_flat,
                   supports,
                   n_supports,
                   supports_concrete,
                   abstract,
                   truncatable,
                   custom)
{
}

be_eventtype::~be_eventtype (void)
{
}

// The differences between valuetype and eventtype are too small
// to warrant a separate set of visitors.
int
be_eventtype::accept (be_visitor *visitor)
{
  return (idl_global->ignore_idl3 ()
            ? 0
            : visitor->visit_eventtype (this));
}

void
be_eventtype::destroy (void)
{
  this->be_valuetype::destroy ();
}



IMPL_NARROW_FROM_DECL (be_eventtype)
IMPL_NARROW_FROM_SCOPE (be_eventtype)
