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
//    Extension of class AST_Array that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"

ACE_RCSID(be, be_string, "$Id$")

be_string::be_string (void)
{
  // Always the case.
  this->size_type (be_decl::VARIABLE);
}

be_string::be_string (AST_Expression *v)
  : AST_String (v),
    AST_Decl (AST_Decl::NT_string,
              new UTL_ScopedName (new Identifier ("string"),
                                  0),
              I_TRUE)
{
  // Always the case.
  this->size_type (be_decl::VARIABLE);
}

be_string::be_string (AST_Expression *v,
                      long wide)
  : AST_String (v, wide),
    AST_Decl (wide == (long) sizeof (char)
                ? AST_Decl::NT_string
                : AST_Decl::NT_wstring,
              wide == (long) sizeof (char)
                ? new UTL_ScopedName (new Identifier ("string"),
                                      0)
                : new UTL_ScopedName (new Identifier ("wstring"),
                                      0),
              I_TRUE)
{
  // Always the case.
  this->size_type (be_decl::VARIABLE);
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

// Narrowing.
IMPL_NARROW_METHODS2 (be_string, AST_String, be_type)
IMPL_NARROW_FROM_DECL (be_string)
