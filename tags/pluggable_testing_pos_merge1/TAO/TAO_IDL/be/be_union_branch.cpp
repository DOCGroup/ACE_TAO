// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_union_branch.cpp
//
// = DESCRIPTION
//    Extension of class AST_UnionBranch that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

ACE_RCSID(be, be_union_branch, "$Id$")


/*
 * BE_UnionBranch
 */
be_union_branch::be_union_branch (void)
{
}

be_union_branch::be_union_branch (UTL_LabelList *ll, AST_Type *ft,
                                  UTL_ScopedName *n, UTL_StrList *p)
  : AST_UnionBranch (ll, ft, n, p),
    AST_Field (AST_Decl::NT_union_branch, ft, n, p),
    AST_Decl (AST_Decl::NT_union_branch, n, p)
{
}

// compute the size type of the node in question
int
be_union_branch::compute_size_type (void)
{
  be_type *type = be_type::narrow_from_decl (this->field_type ());
  if (!type)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_field::compute_size_type - "
                         "bad field type\n"), -1);
    }

  // our size type is the same as our type
  this->size_type (type->size_type ()); // as a side effect will also update
                                        // the size type of parent
  return 0;
}

int
be_union_branch::gen_label_value (TAO_OutStream *os, unsigned long index)
{
  AST_Expression *e = this->label (index)->label_val ();
  if (e->ec () != AST_Expression::EC_symbol)
    {
      // Easy, just a number...
      *os << e;
      return 0;
    }

  // If the enum is not in the global scope we have to prefix it.
  be_union *u =
    be_union::narrow_from_scope (this->defined_in ());
  if (u == 0)
    return -1;

  be_type* dt =
    be_type::narrow_from_decl (u->disc_type ());
  if (dt == 0)
    return -1;

  // Find where was the enum defined, if it was defined in the globa
  // scope, then it is easy to generate the enum values....
  be_scope* scope =
    be_scope::narrow_from_scope (dt->defined_in ());
  if (scope == 0)
    {
      *os << e->n ();
      return 0;
    }

  // But if it was generated inside a module or something similar then
  // we must prefix the enum value with something...
  be_decl* decl =
    scope->decl ();
  *os << decl->fullname () << "::" << e->n ();
  return 0;
}

int
be_union_branch::accept (be_visitor *visitor)
{
  return visitor->visit_union_branch (this);
}

// Narrowing
IMPL_NARROW_METHODS2 (be_union_branch, AST_UnionBranch, be_decl)
IMPL_NARROW_FROM_DECL (be_union_branch)
