// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_field.cpp
//
// = DESCRIPTION
//    Extension of class AST_Field that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

ACE_RCSID(be, be_field, "$Id$")

be_field::be_field (void)
{
}

be_field::be_field (AST_Type *ft, 
                    UTL_ScopedName *n, 
                    Visibility vis)
  : AST_Field (ft, 
               n, 
               vis),
    AST_Decl (AST_Decl::NT_field, 
              n),
    COMMON_Base (ft->is_local (), 
                 ft->is_abstract ())
{
}

// Compute the size type of the node in question.
int
be_field::compute_size_type (void)
{
  be_type *type = be_type::narrow_from_decl (this->field_type ());

  if (type == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_field::compute_size_type - "
                         "bad field type\n"), -1);
    }

  // Our size type is the same as our type.
  this->size_type (type->size_type ()); // As a side effect, will also update
                                        // the size type of parent.

  // While we're here, take care of has_constructor.
  this->has_constructor (type->has_constructor ());

  return 0;
}

int
be_field::accept (be_visitor *visitor)
{
  return visitor->visit_field (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_field, AST_Field, be_decl)
IMPL_NARROW_FROM_DECL (be_field)
