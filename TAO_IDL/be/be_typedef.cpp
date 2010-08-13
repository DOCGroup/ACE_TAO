
//=============================================================================
/**
 *  @file    be_typedef.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Typedef that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#include "be_typedef.h"
#include "be_visitor.h"
#include "ace/Log_Msg.h"

be_typedef::be_typedef (AST_Type *bt,
                        UTL_ScopedName *n,
                        bool local,
                        bool abstract)
  : COMMON_Base (bt->is_local () || local,
                 abstract),
    AST_Decl (AST_Decl::NT_typedef,
              n),
    AST_Type (AST_Decl::NT_typedef,
              n),
    AST_Field (AST_Decl::NT_typedef,
               bt,
               n),
    AST_Typedef (bt,
                 n,
                 bt->is_local () || local,
                 abstract),
    be_decl (AST_Decl::NT_typedef,
             n),
    be_type (AST_Decl::NT_typedef,
             n)
{
  AST_Type *pbt = this->primitive_base_type ();
  AST_Decl::NodeType nt = pbt->node_type ();

  if (nt == AST_Decl::NT_sequence)
    {
      pbt->anonymous (false);
    }
}

void
be_typedef::seen_in_sequence (bool val)
{
  this->be_type::seen_in_sequence (val);
  this->primitive_base_type ()->seen_in_sequence (val);
}

// Some compilers seems to have a problem with a function
// that's both virtual and overloaded.
bool
be_typedef::seen_in_operation (void) const
{
  return this->be_type::seen_in_operation ();
}

void
be_typedef::seen_in_operation (bool val)
{
  this->be_type::seen_in_operation (val);
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
  be_typedef *td = const_cast<be_typedef *> (this);

  be_type *base = be_type::narrow_from_decl (td->base_type ());

  return base->base_node_type ();
}

void
be_typedef::gen_member_ostream_operator (TAO_OutStream *os,
                                         const char *instance_name,
                                         bool use_underscore,
                                         bool accessor)
{
  this->primitive_base_type ()->gen_member_ostream_operator (os,
                                                             instance_name,
                                                             use_underscore,
                                                             accessor);
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

IMPL_NARROW_FROM_DECL (be_typedef)
