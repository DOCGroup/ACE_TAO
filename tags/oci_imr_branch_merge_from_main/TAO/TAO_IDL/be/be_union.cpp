// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_union.cpp
//
// = DESCRIPTION
//    Extension of class AST_Union that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_union.h"
#include "be_visitor.h"
#include "be_codegen.h"
#include "be_helper.h"
#include "be_extern.h"

#include "ast_union_branch.h"
#include "utl_identifier.h"
#include "idl_defines.h"
#include "global_extern.h"

ACE_RCSID (be,
           be_union,
           "$Id$")


be_union::be_union (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    AST_ConcreteType (),
    UTL_Scope (),
    AST_Structure (),
    AST_Union (),
    be_scope (),
    be_decl (),
    be_type ()
{
  // Always the case.
  this->has_constructor (I_TRUE);
}

be_union::be_union (AST_ConcreteType *dt,
                    UTL_ScopedName *n,
                    idl_bool local,
                    idl_bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (AST_Decl::NT_union,
              n),
    AST_Type (AST_Decl::NT_union,
              n),
    AST_ConcreteType (AST_Decl::NT_union,
                      n),
    UTL_Scope (AST_Decl::NT_union),
    AST_Structure (AST_Decl::NT_union,
                   n,
                   local,
                   abstract),
    AST_Union (dt,
               n,
               local,
               abstract),
    be_scope (AST_Decl::NT_union),
    be_decl (AST_Decl::NT_union,
             n),
    be_type (AST_Decl::NT_union,
             n)
{
  // Always the case.
  this->has_constructor (I_TRUE);

  if (!this->imported ())
    {
      ACE_SET_BITS (idl_global->decls_seen_info_,
                    idl_global->decls_seen_masks.aggregate_seen_);

      ACE_SET_BITS (idl_global->decls_seen_info_,
                    idl_global->decls_seen_masks.union_seen_);
    }
}

void
be_union::redefine (AST_Structure *from)
{
  be_union *bu = be_union::narrow_from_decl (from);
  this->common_varout_gen_ = bu->common_varout_gen_;
  AST_Union::redefine (from);
}

idl_bool
be_union::has_duplicate_case_labels (void)
{
  for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();
      AST_UnionBranch *ub =
        AST_UnionBranch::narrow_from_decl (d);

      if (ub->label_list_length () > 1)
        {
          return I_TRUE;
        }
    }

  return I_FALSE;
}

void
be_union::destroy (void)
{
  // Call the destroy methods of our base classes.
  be_scope::destroy ();
  be_type::destroy ();
}

// Visitor method.
int
be_union::accept (be_visitor *visitor)
{
  return visitor->visit_union (this);
}

// Narrowing.
IMPL_NARROW_METHODS3 (be_union, AST_Union, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_union)
IMPL_NARROW_FROM_SCOPE (be_union)
