// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_string.cpp
//
// = DESCRIPTION
//    Extension of class AST_String that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_string.h"
#include "be_visitor.h"
#include "utl_identifier.h"

ACE_RCSID (be, 
           be_string, 
           "$Id$")

be_string::be_string (void)
{
  // Always the case.
  this->size_type (AST_Type::VARIABLE);
}

be_string::be_string (AST_Decl::NodeType nt,
                      UTL_ScopedName *n,
                      AST_Expression *v,
                      long width)
  : AST_String (nt,
                n,
                v,
                width),
    AST_Decl (nt,
              n,
              I_TRUE)
{
}

// Overriden method.
void
be_string::compute_tc_name (void)
{
  // Start with the head as the CORBA namespace.
  Identifier *corba_id = 0;
  ACE_NEW (corba_id,
           Identifier ("CORBA"));

  ACE_NEW (this->tc_name_,
           UTL_ScopedName (corba_id,
                           0));

  Identifier *id = 0;
  ACE_NEW (id,
           Identifier ("_tc_string"));

  UTL_ScopedName *conc_name = 0;
  ACE_NEW (conc_name,
           UTL_ScopedName (id,
                           0));

  this->tc_name_->nconc (conc_name);
}

int
be_string::accept (be_visitor *visitor)
{
  return visitor->visit_string (this);
}

void
be_string::destroy (void)
{
  this->be_type::destroy ();
  this->AST_String::destroy ();
}

// Narrowing.
IMPL_NARROW_METHODS2 (be_string, AST_String, be_type)
IMPL_NARROW_FROM_DECL (be_string)
