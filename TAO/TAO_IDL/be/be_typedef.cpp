// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_typedef.cpp
//
// = DESCRIPTION
//    Extension of class AST_Typedef that provides additional means for C++
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

ACE_RCSID (be,
           be_typedef,
           "$Id$")

be_typedef::be_typedef (void)
{
}

be_typedef::be_typedef (AST_Type *bt,
                        UTL_ScopedName *n,
                        idl_bool local,
                        idl_bool abstract)
  : AST_Typedef (bt,
                 n,
                 bt->is_local () || local,
                 abstract),
    AST_Decl (AST_Decl::NT_typedef,
              n),
    COMMON_Base (bt->is_local () || local,
                 abstract)
{
}

// Given a typedef node, traverse the chain of base types until they are no
// more typedefs, and return that most primitive base type.
be_type *
be_typedef::primitive_base_type (void)
{
  be_type *d = this;
  be_typedef *temp = 0;

  while (d && d->node_type () == AST_Decl::NT_typedef)
    {
      temp = be_typedef::narrow_from_decl (d);
      d = be_type::narrow_from_decl (temp->base_type ());
    }

  return d;
}

// Compute the size type of the node in question.
int
be_typedef::compute_size_type (void)
{
  be_type *type = be_type::narrow_from_decl (this->base_type ());

  if (type == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_typedef::compute_size_type - "
                         "bad base type\n"),
                        -1);
    }

  // Our size type is the same as our type.
  this->size_type (type->size_type ());

  //While we're here, take care of has_constructor.
  this->has_constructor (type->has_constructor ());

  return 0;
}

AST_Decl::NodeType
be_typedef::base_node_type (void) const
{
  be_typedef *td = ACE_const_cast (be_typedef *, this);

  be_type *base = be_type::narrow_from_decl (td->base_type ());

  return base->base_node_type ();
}

void
be_typedef::destroy (void)
{
  // Do nothing
}

int
be_typedef::accept (be_visitor *visitor)
{
  return visitor->visit_typedef (this);
}

// Narrowing.
IMPL_NARROW_METHODS2 (be_typedef, AST_Typedef, be_type)
IMPL_NARROW_FROM_DECL (be_typedef)
