// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_structure_fwd.cpp
//
// = DESCRIPTION
//    Extension of class AST_StructureFwd that provides additional means for C++
//    mapping of a struct.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include "be_structure_fwd.h"
#include "be_visitor.h"

ACE_RCSID (be, 
           be_structure_fwd, 
           "$Id$")

be_structure_fwd::be_structure_fwd (void)
{
}

be_structure_fwd::be_structure_fwd (AST_Structure *dummy,
                                    UTL_ScopedName *n)
  : be_type (AST_Decl::NT_struct_fwd,
             n),
    AST_StructureFwd (dummy,
                      n),
    AST_Type (AST_Decl::NT_struct_fwd,
              n),
    AST_Decl (AST_Decl::NT_struct_fwd,
              n)
{
}

be_structure_fwd::~be_structure_fwd (void)
{
}

void
be_structure_fwd::destroy (void)
{
  // Do nothing.
}

int
be_structure_fwd::accept (be_visitor *visitor)
{
  return visitor->visit_structure_fwd (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_structure_fwd, AST_StructureFwd, be_type)
IMPL_NARROW_FROM_DECL (be_structure_fwd)
