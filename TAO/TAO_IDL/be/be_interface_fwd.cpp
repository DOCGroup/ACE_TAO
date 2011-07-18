// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_interface_fwd.cpp
//
// = DESCRIPTION
//    Extension of class AST_InterfaceFwd that provides additional means for C++
//    mapping of an interface.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_interface_fwd.h"
#include "be_interface.h"
#include "be_visitor.h"
#include "ast_interface.h"

ACE_RCSID (be,
           be_interface_fwd,
           "$Id$")

be_interface_fwd::be_interface_fwd (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    AST_InterfaceFwd (),
    be_decl (),
    be_type ()
{
  // Always the case.
  this->size_type (AST_Type::VARIABLE);
}

be_interface_fwd::be_interface_fwd (AST_Interface *dummy,
                                    UTL_ScopedName *n)
  : COMMON_Base (dummy->is_local (),
                 dummy->is_abstract ()),
    AST_Decl (AST_Decl::NT_interface_fwd,
              n),
    AST_Type (AST_Decl::NT_interface_fwd,
              n),
    AST_InterfaceFwd (dummy,
                      n),
    be_decl (AST_Decl::NT_interface_fwd,
             n),
    be_type (AST_Decl::NT_interface_fwd,
             n)
{
  // Always the case.
  this->size_type (AST_Type::VARIABLE);
}

be_interface_fwd::~be_interface_fwd (void)
{
}

void
be_interface_fwd::seen_in_sequence (bool val)
{
  this->be_type::seen_in_sequence (val);
  be_interface *fd =
    be_interface::narrow_from_decl (this->full_definition ());
  fd->seen_in_sequence (val);
}

void
be_interface_fwd::seen_in_operation (bool val)
{
  this->be_type::seen_in_operation (val);
  be_interface *fd =
    be_interface::narrow_from_decl (this->full_definition ());
  fd->seen_in_operation (val);
}

void
be_interface_fwd::destroy (void)
{
  this->be_type::destroy ();
  this->AST_InterfaceFwd::destroy ();
}

int
be_interface_fwd::accept (be_visitor *visitor)
{
  return visitor->visit_interface_fwd (this);
}



IMPL_NARROW_FROM_DECL (be_interface_fwd)
