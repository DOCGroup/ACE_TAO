// $Id$

/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.
2550 Garcia Avenue
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

*/

// AST_Union nodes represent IDL union declarations.
// AST_Union is a subclass of AST_ConcreteType and of UTL_Scope (the
// union branches are managed in a scope).
// AST_Union nodes have a discriminator type (a subclass of AST_ConcreteType),
// a name (an UTL_ScopedName) and a field denoting the discriminator type if
// it is a primitive type (the value of this field is from the union
// AST_Expression::ExprType and serves as a cache). This field is used
// to compute coercions for labels based on the expected discriminator type.

#include "idl.h"
#include "idl_extern.h"

ACE_RCSID(ast, ast_union, "$Id$")

// Constructor(s) and destructor.
AST_Union::AST_Union (void)
{
}

AST_Union::AST_Union (AST_ConcreteType *dt,
                      UTL_ScopedName *n,
                      UTL_StrList *p,
                      idl_bool local,
                      idl_bool abstract)
 : AST_Decl (AST_Decl::NT_union, 
             n, 
             p),
   UTL_Scope (AST_Decl::NT_union),
   COMMON_Base (local, 
                abstract)
{
  AST_PredefinedType *pdt = 0;

  if (dt == 0) 
    {
      this->pd_disc_type = 0;
      this->pd_udisc_type = AST_Expression::EV_none;
      return;
    }

  // If the discriminator type is a predefined type
  // then install the equivalent coercion target type in
  // the pd_udisc_type field.
  if (dt->node_type () == AST_Decl::NT_pre_defined) 
    {
      pdt = AST_PredefinedType::narrow_from_decl (dt);

      if (pdt == 0) 
        {
          this->pd_disc_type = 0;
          this->pd_udisc_type = AST_Expression::EV_none;
          return;
        }

      pd_disc_type = dt;

      switch (pdt->pt ()) 
        {
        case AST_PredefinedType::PT_long:
          this->pd_udisc_type = AST_Expression::EV_long;
          break;
        case AST_PredefinedType::PT_ulong:
          this->pd_udisc_type = AST_Expression::EV_ulong;
          break;
        case AST_PredefinedType::PT_short:
          this->pd_udisc_type = AST_Expression::EV_short;
          break;
        case AST_PredefinedType::PT_ushort:
          this->pd_udisc_type = AST_Expression::EV_ushort;
          break;
        case AST_PredefinedType::PT_char:
          this->pd_udisc_type = AST_Expression::EV_char;
          break;
        case AST_PredefinedType::PT_wchar:
          this->pd_udisc_type = AST_Expression::EV_wchar;
          break;
        case AST_PredefinedType::PT_octet:
          this->pd_udisc_type = AST_Expression::EV_octet;
          break;
        case AST_PredefinedType::PT_boolean:
          this->pd_udisc_type = AST_Expression::EV_bool;
          break;
        default:
          this->pd_udisc_type = AST_Expression::EV_none;
          this->pd_disc_type = 0;
          break;
        }
    } 
  else if (dt->node_type() == AST_Decl::NT_enum) 
    {
      this->pd_udisc_type = AST_Expression::EV_any;
      this->pd_disc_type = dt;
    } 
  else 
    {
      this->pd_udisc_type = AST_Expression::EV_none;
      this->pd_disc_type = 0;
    }

  if (this->pd_disc_type == 0)
    {
      idl_global->err ()->error2 (UTL_Error::EIDL_DISC_TYPE, 
                                  this, 
                                  dt);
    }
}

AST_Union::~AST_Union (void)
{
}

// Public operations.

// Are we or the parameter node involved in any recursion?
idl_bool
AST_Union::in_recursion (AST_Type *node)
{
  if (node == 0)
    {
      // We are determining the recursive status for ourselves.
      node = this;
    }

  // Proceed if the number of members in our scope is greater than 0.
  if (this->nmembers () > 0)
    {
      // Initialize an iterator to iterate thru our scope.
      UTL_ScopeActiveIterator *si = 0;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (this,
                                               UTL_Scope::IK_decls),
                      0);
      // Continue until each element is visited.
      while (!si->is_done ())
        {
          AST_UnionBranch *field = 
            AST_UnionBranch::narrow_from_decl (si->item ());

          if (field == 0)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("(%N:%l) AST_Union::")
                                 ACE_TEXT ("in_recursion - ")
                                 ACE_TEXT ("bad field node\n")),
                                0);
            }

          AST_Type *type = 
            AST_Type::narrow_from_decl (field->field_type ());

          if (type == 0)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("(%N:%l) AST_Union::")
                                 ACE_TEXT ("in_recursion - ")
                                 ACE_TEXT ("bad field type\n")),
                                0);
            }

          if (type->in_recursion (node))
            {
              delete si;
              return 1;
            }

          si->next ();
        }

      delete si;
    }

  // Not in recursion.
  return 0;
}

// Look up the default branch in union.
AST_UnionBranch *
AST_Union::lookup_default (void)
{
  UTL_ScopeActiveIterator *i = 0;
  ACE_NEW_RETURN (i,
                  UTL_ScopeActiveIterator (this, 
                                           IK_both),
                  0);

  AST_UnionBranch	*b = 0;
  AST_Decl *d = 0;

  while (!i->is_done()) 
    {
      d = i->item ();

      if (d->node_type () == AST_Decl::NT_union_branch) 
        {
          b = AST_UnionBranch::narrow_from_decl (d);

          if (b == 0) 
            {
              i->next ();
              continue;
            }

          if (b->label() != 0 
              && b->label ()->label_kind () == AST_UnionLabel::UL_default) 
            {
	            idl_global->err ()->error2 (UTL_Error::EIDL_MULTIPLE_BRANCH, 
                                          this, 
                                          b);
	            delete i;
	            return b;
            }
        }

      i->next ();
    }

  delete i;
  return NULL;
}

// Look up a branch by label.
AST_UnionBranch *
AST_Union::lookup_label (AST_UnionBranch *b)
{
  AST_UnionLabel *label = b->label ();
  AST_Expression *lv = label->label_val ();

  if (label->label_val () == 0)
    {
      return b;
    }

  AST_Decl *d = 0;
  AST_UnionBranch	*fb = 0;

  lv->set_ev (lv->coerce (this->pd_udisc_type));

  if (lv->ev () == 0) 
    {
      idl_global->err ()->eval_error (lv);
      return b;
    }

  UTL_ScopeActiveIterator	*i = 0;
  ACE_NEW_RETURN (i,
                  UTL_ScopeActiveIterator (this, 
                                           IK_decls),
                  0);

  while (!i->is_done()) 
    {
      d = i->item ();

      if (d->node_type () == AST_Decl::NT_union_branch) 
        {
          fb = AST_UnionBranch::narrow_from_decl (d);

          if (fb == 0) 
            {
              i->next ();
              continue;
            }

          if (fb->label() != 0 
              && fb->label ()->label_kind () == AST_UnionLabel::UL_label 
              && fb->label ()->label_val ()->compare (lv)) 
            {
	            idl_global->err ()->error2  (UTL_Error::EIDL_MULTIPLE_BRANCH, 
                                           this, 
                                           b);
	            delete i;
	            return b;
            }
        }

      i->next();
    }

  delete i;
  return 0;
}

// Look up a branch in an enum which is the discriminator type for this
// union, based on the label value which must be an enumerator in that
// enum.
AST_UnionBranch *
AST_Union::lookup_enum (AST_UnionBranch *b)
{
  AST_UnionLabel *label = b->label();
  AST_Expression *lv = label->label_val ();
  AST_Enum *e = AST_Enum::narrow_from_decl (this->pd_disc_type);
  AST_Decl *d = 0;
  AST_UnionBranch	*fb = 0;

  if (e == 0) 
    {
      return 0;
    }

  if (lv == 0)
    {
      return b;
    }

  // Expecting a symbol label.
  if (lv->ec () != AST_Expression::EC_symbol) 
    {
      idl_global->err ()->enum_val_expected (this, 
                                             label);
      return b;
    }

  // See if the symbol defines a constant in the discriminator enum.
  UTL_ScopedName *sn = lv->n ();
  d = e->lookup_by_name (sn, 
                         I_TRUE);

  if (d == 0 || d->defined_in () != e) 
    {
      idl_global->err ()->enum_val_lookup_failure (this, 
                                                   e, 
                                                   sn);
      return b;
    }

  // OK, now see if this symbol is already used as the label of
  // some other branch.
  UTL_ScopeActiveIterator	*i = 0;
  ACE_NEW_RETURN (i,
                  UTL_ScopeActiveIterator (this, 
                                           IK_decls),
                  0);

  while (!i->is_done()) 
    {
      d = i->item ();

      if (d->node_type () == AST_Decl::NT_union_branch) 
        {
          fb = AST_UnionBranch::narrow_from_decl (d);

          if (fb == 0) 
            {
                i->next ();
                continue;
            }

          if (fb->label() != 0 
              && fb->label ()->label_kind () == AST_UnionLabel::UL_label 
              && fb->label ()->label_val ()->compare (lv)) 
            {
	            idl_global->err ()->error2 (UTL_Error::EIDL_MULTIPLE_BRANCH, 
                                          this, 
                                          b);
	            delete i;
	            return b;
            }
        }

      i->next();
    }

  delete i;
  return NULL;
}

// Look up a branch by value. This is the top level branch label resolution
// entry point. It dispatches to the right lookup function depending on the
// union discriminator type.
AST_UnionBranch *
AST_Union::lookup_branch (AST_UnionBranch *branch)
{
  AST_UnionLabel *label = 0;

  if (branch != 0)
    {
      label = branch->label ();
    }

  if (label != 0) 
    {
      if (label->label_kind () == AST_UnionLabel::UL_default)
        {
          return lookup_default ();
        }

      if (this->pd_udisc_type == AST_Expression::EV_any)
        {
          // CONVENTION: indicates enum discriminant.
          return lookup_enum(branch);
        }

      return lookup_label (branch);
    }

  return 0;
}

// Redefinition of inherited virtual operations

// Add this AST_UnionBranch node (a node representing one branch in a
// union declaration) to this scope
AST_UnionBranch *
AST_Union::fe_add_union_branch (AST_UnionBranch *t)
{
  AST_Decl *d = 0;

  // If this is a malformed branch, don't do anything with it.
  if (t == 0 || t->label() == 0)
    {
      return 0;
    }

  // If branch with that label already exists, complain.
  if (lookup_branch (t) != 0) 
    {
      idl_global->err ()->error2 (UTL_Error::EIDL_MULTIPLE_BRANCH, 
                                  this, 
                                  t);
      return 0;
    }

  // If branch with same field name exists, complain.
  if ((d = lookup_for_add (t, I_FALSE)) != 0) 
    {
      if (!can_be_redefined (d)) 
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF, 
                                      t, 
                                      this, 
                                      d);
          return 0;
        }

      if (this->referenced (d, t->local_name ())) 
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE, 
                                      t, 
                                      this, 
                                      d);
          return 0;
        }

      if (t->has_ancestor (d)) 
        {
          idl_global->err ()->redefinition_in_scope (t, 
                                                     d);
          return 0;
        }
    }

  // Add it to scope.
  this->add_to_scope (t);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (t, 
                           I_FALSE, 
                           t->local_name ());

  this->fields_.enqueue_tail (t);

  return t;
}

// Add this AST_Union (manifest union type) to this scope.
AST_Union *
AST_Union::fe_add_union (AST_Union *t)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = lookup_for_add (t, I_FALSE)) != 0) 
    {
      if (!can_be_redefined (d)) 
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF, 
                                      t, 
                                      this, 
                                      d);
          return 0;
        }

      if (this->referenced (d, t->local_name ())) 
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE, 
                                      t, 
                                      this, 
                                      d);
          return 0;
        }

      if (t->has_ancestor (d)) 
        {
          idl_global->err ()->redefinition_in_scope (t, 
                                                     d);
          return 0;
        }
    }

  // Add it to local types.
  this->add_to_local_types (t);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (t, 
                           I_FALSE, 
                           t->local_name ());

  return t;
}

// Add this AST_Structure node (manifest struct type) to this scope.
AST_Structure *
AST_Union::fe_add_structure (AST_Structure *t)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = lookup_for_add (t, I_FALSE)) != 0) 
    {
      if (!can_be_redefined (d)) 
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF, 
                                      t, 
                                      this, 
                                      d);
          return 0;
        }

      if (this->referenced (d, t->local_name ())) 
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE, 
                                      t, 
                                      this, 
                                      d);
          return 0;
        }

      if (t->has_ancestor (d)) 
        {
          idl_global->err ()->redefinition_in_scope (t, 
                                                     d);
          return 0;
        }
    }

  // Add it to local types.
  this->add_to_local_types (t);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (t, 
                           I_FALSE, 
                           t->local_name ());

  return t;
}

// Add this AST_Enum node (manifest enum type) to this scope.
AST_Enum *
AST_Union::fe_add_enum (AST_Enum *t)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = lookup_for_add (t, I_FALSE)) != 0) 
    {
      if (!can_be_redefined (d)) 
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF, 
                                      t, 
                                      this, 
                                      d);
          return 0;
        }

      if (this->referenced (d, t->local_name ())) 
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE, 
                                      t, 
                                      this, 
                                      d);
          return 0;
        }

      if (t->has_ancestor (d)) 
        {
          idl_global->err ()->redefinition_in_scope (t, 
                                                     d);
          return 0;
        }
    }

  // Add it to local types.
  this->add_to_local_types (t);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (t, 
                           I_FALSE, 
                           t->local_name ());

  return t;
}

// Add this AST_EnumVal node (enumerator declaration) to this scope.
// This is done to conform to the C++ scoping rules which declare
// enumerators in the enclosing scope (in addition to declaring them
// in the enum itself).
AST_EnumVal *
AST_Union::fe_add_enum_val (AST_EnumVal *t)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = lookup_for_add (t, I_FALSE)) != 0) 
    {
      if (!can_be_redefined (d)) 
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF, 
                                      t, 
                                      this, 
                                      d);
          return 0;
        }

      if (this->referenced (d, t->local_name ())) 
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE, 
                                      t, 
                                      this, 
                                      d);
          return 0;
        }

      if (t->has_ancestor (d)) 
        {
          idl_global->err ()->redefinition_in_scope (t, 
                                                     d);
          return 0;
        }
    }

  // Add it to scope.
  this->add_to_scope (t);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (t, 
                           I_FALSE, 
                           t->local_name ());

  return t;
}

// Dump this AST_Union node to the ostream o.
void
AST_Union::dump (ostream &o)
{
  o << "union ";
  this->local_name ()->dump (o);
  o << " switch (";
  this->pd_disc_type->local_name ()->dump (o);
  o << ") {\n";
  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);
  o << "}";
}

int
AST_Union::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_union (this);
}

// Data accessors.

AST_ConcreteType *
AST_Union::disc_type (void)
{
  return this->pd_disc_type;
}

AST_Expression::ExprType
AST_Union::udisc_type (void)
{
  return this->pd_udisc_type;
}

// Narrowing.
IMPL_NARROW_METHODS2(AST_Union, AST_ConcreteType, UTL_Scope)
IMPL_NARROW_FROM_DECL(AST_Union)
IMPL_NARROW_FROM_SCOPE(AST_Union)
