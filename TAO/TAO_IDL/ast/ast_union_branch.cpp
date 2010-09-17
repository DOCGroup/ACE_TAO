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

// AST_UnionBranch nodes represent a single branch of an IDL union
// declaration.
// AST_UnionBranch is a subclass of AST_Field, adding a label (which
// is a subclass of AST_UnionLabel).

#include "ast_union_branch.h"
#include "ast_union_label.h"
#include "ast_union.h"
#include "ast_enum.h"
#include "ast_enum_val.h"
#include "ast_visitor.h"

#include "utl_labellist.h"
#include "fe_extern.h"

AST_Decl::NodeType const
AST_UnionBranch::NT = AST_Decl::NT_union_branch;

AST_UnionBranch::AST_UnionBranch (UTL_LabelList *ll,
                                  AST_Type *ft,
                                  UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_union_branch,
              n),
    AST_Field (AST_Decl::NT_union_branch,
               ft,
               n),
    pd_ll (ll)
{
}

AST_UnionBranch::~AST_UnionBranch (void)
{
}

// Redefinition of inherited virtual operations.

// Dump this AST_UnionBranch node to the ostream o.
void
AST_UnionBranch::dump (ACE_OSTREAM_TYPE &o)
{
  for (unsigned long i = 0; i < this->label_list_length (); ++i)
    {
      this->dump_i (o, "case ");

      AST_UnionLabel *ul = this->label (i);
      ul->dump (o);

      this->dump_i (o, ":\n");
    }

  AST_Field::dump (o);
}

int
AST_UnionBranch::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_union_branch (this);
}

void
AST_UnionBranch::destroy (void)
{
  this->pd_ll->destroy ();
  delete this->pd_ll;
  this->pd_ll = 0;

  this->AST_Field::destroy ();
}

UTL_LabelList *
AST_UnionBranch::labels (void) const
{
  return this->pd_ll;
}

AST_UnionLabel *
AST_UnionBranch::label (unsigned long index)
{
  unsigned long i = 0;

  for (UTL_LabellistActiveIterator iter (this->pd_ll);
       !iter.is_done ();
       iter.next ())
    {
      if (i == index)
        {
          return iter.item ();
        }

      ++i;
    }

  return 0;
}

unsigned long
AST_UnionBranch::label_list_length (void)
{
  if (this->pd_ll)
    {
      return this->pd_ll->length ();
    }
  else
    {
      return 0;
    }
}

void
AST_UnionBranch::add_labels (AST_Union *u)
{
  for (UTL_LabellistActiveIterator i (this->pd_ll);
       !i.is_done ();
       i.next ())
    {
      if (AST_UnionLabel::UL_default == i.item ()->label_kind ())
        {
          return;
        }
    }

  const bool enum_labels = (u->udisc_type () == AST_Expression::EV_enum);

  for (UTL_LabellistActiveIterator i (this->pd_ll);
       !i.is_done ();
       i.next ())
    {
      AST_Expression *ex = i.item ()->label_val ();
      UTL_ScopedName *n = ex->n ();

      if (n)
        {
          u->add_to_name_referenced (n->first_component ());
        }

      // If we have enum val labels, we need to set the type and
      // evaluate here, so the value will be available when the
      // default index in calculated.
      if (enum_labels)
        {
          ex->ev ()->et = AST_Expression::EV_enum;
          AST_Enum *disc = AST_Enum::narrow_from_decl (u->disc_type ());
          AST_EnumVal *dval = disc->lookup_by_value (ex);

          if (dval == 0)
            {
              idl_global->err ()->incompatible_disc_error (disc, ex);
              throw Bailout ();
            }

          ex->ev ()->u.eval = dval->constant_value ()->ev ()->u.ulval;
        }
    }
}

void
AST_UnionBranch::coerce_labels (AST_Union *u)
{
  for (unsigned long i = 0; i < this->label_list_length (); ++i)
    {
      AST_UnionLabel *ul = this->label (i);

      if (ul->label_kind () == AST_UnionLabel::UL_default)
        {
          continue;
        }

      AST_Expression *lv = ul->label_val ();
      lv->set_ev (lv->coerce (u->udisc_type ()));
    }
}

IMPL_NARROW_FROM_DECL(AST_UnionBranch)
