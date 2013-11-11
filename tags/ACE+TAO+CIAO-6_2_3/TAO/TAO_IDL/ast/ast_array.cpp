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

// AST_Array nodes denote array type and field modifiers.
// AST_Array nodes have a list of dimensions (a UTL_ExprList)
// a count of the number of dimensions and a base type (a
// subtype of AST_ConcreteType. This means that we cannot have
// arrays of AST_Interfaces???

#include "ast_array.h"
#include "ast_expression.h"
#include "ast_param_holder.h"
#include "ast_visitor.h"

#include "utl_exprlist.h"
#include "utl_identifier.h"
#include "ace/Log_Msg.h"
#include "ace/OS_Memory.h"

AST_Decl::NodeType const
AST_Array::NT = AST_Decl::NT_array;

AST_Array::AST_Array (UTL_ScopedName *n,
                      ACE_CDR::ULong nd,
                      UTL_ExprList *ds,
                      bool local,
                      bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (AST_Decl::NT_array,
              n,
              true),
    AST_Type (AST_Decl::NT_array,
              n),
    AST_ConcreteType (AST_Decl::NT_array,
                      n),
    pd_n_dims (nd),
    pd_base_type (0),
    owns_base_type_ (false)
{
  this->pd_dims = this->compute_dims (ds,
                                      nd);
}

AST_Array::~AST_Array (void)
{
}

// Private operations.

// Compute how many dimensions there are and collect their expressions
// into an array.
AST_Expression **
AST_Array::compute_dims (UTL_ExprList *ds,
                         ACE_CDR::ULong nds)
{
  if (ds == 0)
    {
      return 0;
    }

  AST_Expression **result = 0;
  ACE_NEW_RETURN (result,
                  AST_Expression *[nds],
                  0);

  UTL_ExprlistActiveIterator iter (ds);

  for (ACE_CDR::ULong i = 0;
       !iter.is_done () && i < nds;
       iter.next (), i++)
    {
      AST_Expression *orig = iter.item ();
      AST_Param_Holder *ph = orig->param_holder ();

      AST_Expression::ExprType ex_type =
        (ph == 0 ? orig->ev ()->et : ph->info ()->const_type_);

      AST_Expression *copy = 0;
      ACE_NEW_RETURN (copy,
                      AST_Expression (orig,
                                      ex_type),
                      0);

      result[i] = copy;
    }

  return result;
}

// Redefinition of inherited virtual operations.

// Dump this AST_Array node to the ostream o.
void
AST_Array::dump (ACE_OSTREAM_TYPE &o)
{
  pd_base_type->dump (o);

  this->dump_i (o, " ");

  this->local_name ()->dump (o);

  for (ACE_CDR::ULong i = 0; i < this->pd_n_dims; i++)
    {
      this->dump_i (o, "[");

      pd_dims[i]->dump (o);

      this->dump_i (o, "]");
    }
}

int
AST_Array::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_array (this);
}

// Compute the size type of the node in question.
int
AST_Array::compute_size_type (void)
{
  AST_Type *type = this->base_type ();

  if (!type)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_array::compute_size_type - "
                         "bad base type\n"),
                        -1);
    }

  // Our size type is the same as our type.
  this->size_type (type->size_type ());

  this->has_constructor (type->has_constructor ());

  return 0;
}

// Data accessors.
ACE_CDR::ULong
AST_Array::n_dims (void)
{
  return this->pd_n_dims;
}

AST_Expression **
AST_Array::dims (void)
{
  return this->pd_dims;
}

AST_Type *
AST_Array::base_type (void) const
{
  return this->pd_base_type;
}

void
AST_Array::set_base_type (AST_Type *nbt)
{
  this->pd_base_type = nbt;
  this->is_local_ = nbt->is_local ();
  AST_Decl::NodeType bnt = nbt->node_type ();

  if (bnt == AST_Decl::NT_sequence
      || bnt == AST_Decl::NT_param_holder)
    {
      this->owns_base_type_ = true;
    }
}

bool
AST_Array::legal_for_primary_key (void) const
{
  return this->base_type ()->legal_for_primary_key ();
}

void
AST_Array::destroy (void)
{
  if (this->owns_base_type_)
    {
      this->pd_base_type->destroy ();
      delete this->pd_base_type;
      this->pd_base_type = 0;
    }

  for (ACE_CDR::ULong i = 0; i < this->pd_n_dims; ++i)
    {
      this->pd_dims[i]->destroy ();
      delete this->pd_dims[i];
      this->pd_dims[i] = 0;
    }

  delete [] this->pd_dims;
  this->pd_dims = 0;
  this->pd_n_dims = 0;

  this->AST_ConcreteType::destroy ();
}

void
AST_Array::set_dims (AST_Expression **ds,
                     ACE_CDR::ULong nds)
{
  this->pd_dims = ds;
  this->pd_n_dims = nds;
}

IMPL_NARROW_FROM_DECL(AST_Array)
