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

#include "be_union_branch.h"
#include "be_union.h"
#include "be_type.h"
#include "be_enum.h"
#include "be_visitor.h"
#include "be_helper.h"
#include "ast_union_label.h"

ACE_RCSID (be, 
           be_union_branch, 
           "$Id$")

be_union_branch::be_union_branch (void)
{
}

be_union_branch::be_union_branch (UTL_LabelList *ll,
                                  AST_Type *ft,
                                  UTL_ScopedName *n)
  : AST_UnionBranch (ll,
                     ft,
                     n),
    AST_Field (AST_Decl::NT_union_branch,
               ft,
               n),
    AST_Decl (AST_Decl::NT_union_branch,
              n),
    COMMON_Base (ft->is_local (),
                 ft->is_abstract ())
{
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
    {
      return -1;
    }

  be_type* dt =
    be_type::narrow_from_decl (u->disc_type ());

  if (dt == 0)
    {
      return -1;
    }

  // Check if discriminator is a typedef of an integer. If so, and the
  // first IF block in this function didn't catch it, then we
  // are a constant of this type. We can't generate the constant's name,
  // we must generate the underlying integer value for the
  // label value.
  if (dt->node_type () == AST_Decl::NT_pre_defined)
    {
      *os << e;
      return 0;
    }

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
  be_decl* decl = scope->decl ();

  *os << decl->full_name () << "::" << e->n ()->last_component ();

  return 0;
}

int
be_union_branch::gen_default_label_value (TAO_OutStream *os,
                                          be_union *bu)
{
  be_union::DefaultValue dv;

  if (bu->default_value (dv) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch::"
                         "gen_default_label_value - "
                         "computing default value failed\n"),
                        -1);
    }

  switch (bu->udisc_type ())
    {
      case AST_Expression::EV_short:
        *os << dv.u.short_val;
        break;
      case AST_Expression::EV_ushort:
        *os << dv.u.ushort_val;
        break;
      case AST_Expression::EV_long:
        *os << dv.u.long_val;
        break;
      case AST_Expression::EV_ulong:
        *os << dv.u.ulong_val;
        break;
      case AST_Expression::EV_char:
        os->print ("%d", dv.u.char_val);
        break;
      case AST_Expression::EV_bool:
        *os << dv.u.bool_val;
        break;
      case AST_Expression::EV_any:
        // The discriminant is an enum. Some compilers will
        // not accept a numeric value assigned to this
        // discriminant, so we must generate the string name.
        {
          AST_ConcreteType *act = bu->disc_type ();
          be_enum *be = be_enum::narrow_from_decl (act);

          // The function value_to_name() takes care of adding
          // any necessary scoping to the output.
          *os << be->value_to_name (dv.u.enum_val);
          break;
        }
      case AST_Expression::EV_longlong:
      case AST_Expression::EV_ulonglong:
        // Unimplemented.
      default:
        // Error caught earlier.
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_union_branch::"
                           "gen_default_label_value - "
                           "bad or unimplemented discriminant type\n"),
                          -1);
    }

  return 0;
}

int
be_union_branch::accept (be_visitor *visitor)
{
  return visitor->visit_union_branch (this);
}

// Narrowing.
IMPL_NARROW_METHODS2 (be_union_branch, AST_UnionBranch, be_decl)
IMPL_NARROW_FROM_DECL (be_union_branch)
