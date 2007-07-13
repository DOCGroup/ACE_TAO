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
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    AST_StructureFwd (),
    be_decl (),
    be_type ()
{
}

be_structure_fwd::be_structure_fwd (AST_Structure *dummy,
                                    UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_struct_fwd,
              n),
    AST_Type (AST_Decl::NT_struct_fwd,
              n),
    AST_StructureFwd (dummy,
                      n),
    be_decl (AST_Decl::NT_struct_fwd,
             n),
    be_type (AST_Decl::NT_struct_fwd,
             n)
{
}

be_structure_fwd::~be_structure_fwd (void)
{
}

void
be_structure_fwd::destroy (void)
{
  this->be_type::destroy ();
  this->AST_StructureFwd::destroy ();
}

int
be_structure_fwd::accept (be_visitor *visitor)
{
  return visitor->visit_structure_fwd (this);
}



IMPL_NARROW_FROM_DECL (be_structure_fwd)
