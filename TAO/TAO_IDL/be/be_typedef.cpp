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

#include "be_typedef.h"
#include "be_visitor.h"
#include "ace/Log_Msg.h"

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
  : be_type (AST_Decl::NT_typedef,
             n),
    be_decl (AST_Decl::NT_typedef,
             n),
    AST_Typedef (bt,
                 n,
                 bt->is_local () || local,
                 abstract),
    AST_Type (AST_Decl::NT_typedef,
              n),
    AST_Decl (AST_Decl::NT_typedef,
              n),
    COMMON_Base (bt->is_local () || local,
                 abstract)
{
}

void 
be_typedef::seq_elem_tmplinst (idl_bool val)
{
  this->be_type::seq_elem_tmplinst (val);
  this->primitive_base_type ()->seq_elem_tmplinst (val);
}

void 
be_typedef::seen_in_sequence (idl_bool val)
{
  this->be_type::seen_in_sequence (val);
  this->primitive_base_type ()->seen_in_sequence (val);
}

void 
be_typedef::seen_in_operation (idl_bool val)
{
  this->be_type::seen_in_operation (val);
  this->primitive_base_type ()->seen_in_operation (val);
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
  this->AST_Typedef::destroy ();
  this->be_type::destroy ();
}

int
be_typedef::accept (be_visitor *visitor)
{
  return visitor->visit_typedef (this);
}

// Narrowing.
IMPL_NARROW_METHODS2 (be_typedef, AST_Typedef, be_type)
IMPL_NARROW_FROM_DECL (be_typedef)
