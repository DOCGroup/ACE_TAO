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

#include "idl.h"
#include "idl_extern.h"

ACE_RCSID(ast, ast_enum, "$Id$")

// Constructor(s) and destructor,

AST_Enum::AST_Enum (void)
        : pd_enum_counter (0),
    member_count_ (-1)
{
}

AST_Enum::AST_Enum (UTL_ScopedName *n,
                    UTL_StrList *p,
                    idl_bool local,
                    idl_bool abstract)
 : AST_Decl (AST_Decl::NT_enum,
             n,
             p),
         UTL_Scope (AST_Decl::NT_enum),
   COMMON_Base (local,
                abstract),
         pd_enum_counter (0),
   member_count_ (-1)
{
}

AST_Enum::~AST_Enum (void)
{
}

// Public operations.

// Return the member count.
int
AST_Enum::member_count (void)
{
  if (this->member_count_ == -1)
    {
      this->compute_member_count ();
    }

  return this->member_count_;
}

// Convert a numeric value to the string name
UTL_ScopedName *
AST_Enum::value_to_name (const unsigned long v)
{
  UTL_ScopeActiveIterator *iter = 0;
  AST_EnumVal *item = 0;
  AST_Decl *i = 0;

  ACE_NEW_RETURN (iter,
                  UTL_ScopeActiveIterator (this,
                                           IK_decls),
                  0);

  while (!iter->is_done ())
    {
      i = iter->item  ();
      item = AST_EnumVal::narrow_from_decl (i);

      if (item->constant_value ()->ev ()->u.ulval == v)
        {
          delete iter;
          return item->name ();
        }

      iter->next ();
    }

  delete iter;
  return 0;
}

// Look up an enumerator by the value of the supplied expression.
AST_EnumVal *
AST_Enum::lookup_by_value (const AST_Expression *v)
{
  AST_EnumVal *item = 0;
  AST_Decl *i = 0;

  UTL_ScopeActiveIterator *iter = 0;
  ACE_NEW_RETURN (iter,
                  UTL_ScopeActiveIterator (this,
                                           IK_decls),
                  0);

  while (!iter->is_done ())
    {
      i = iter->item ();
      item = AST_EnumVal::narrow_from_decl (i);

      if (item->constant_value () == v)
        {
          delete iter;
          return item;
        }

      iter->next ();
    }

  delete iter;
  return 0;
}

// Compute the value to be assigned to the next enumerator. Bump the
// counter.
unsigned long
AST_Enum::next_enum_val (void)
{
  unsigned long i = pd_enum_counter++;

  return i;
}

// Static helper functions

// Modify scoped name of an enumval so that it is scoped inside the scope
// in which the enum is defined and not inside the enum itself
static UTL_ScopedName *
munge_name_for_enumval (UTL_ScopedName *n,
                        Identifier *last_component)
{
  long len = n->length ();
  UTL_ScopedName *hold = n;

  // Last three components are:
  // - scope in which enum is defined
  // - scope for enum
  // - name of enumval

  // We want to stop cdr'ing down the list when the head of the
  // list is at the name for the scope in which the enum is defined.
  while (len > 3)
    {
      len--;
      n = (UTL_ScopedName *) n->tail ();
    }

  UTL_IdList *id = 0;
  ACE_NEW_RETURN (id,
                  UTL_IdList (last_component,
                              0),
                  0);

  n->set_tail (id);

  return hold;
}

// Private operations.

// Compute total number of members.
int
AST_Enum::compute_member_count (void)
{
  UTL_ScopeActiveIterator *si = 0;

  this->member_count_ = 0;

  // If there are elements in this scope
  if (this->nmembers () > 0)
    {
      // Instantiate a scope iterator.
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (this,
                                               UTL_Scope::IK_decls),
                      -1);

      while (!(si->is_done ()))
              {
                // Get the next AST decl node.
          this->member_count_++;
          si->next ();
        }

      delete si;
    }

  return 0;
}

// Add an AST_EnumVal node to this scope
AST_EnumVal *
AST_Enum::fe_add_enum_val (AST_EnumVal *t)
{
  AST_Decl *d = 0;
  AST_EnumVal *t1 = 0;

  if (t != 0)
    {
      unsigned long tmp =
        t->constant_value ()->coerce (AST_Expression::EV_ulong)->u.ulval;

      t1 = idl_global->gen ()->create_enum_val (tmp,
                                                t->name (),
                                                t->pragmas ());

      t->set_name (munge_name_for_enumval (t->name (),
                                           t->local_name ()));

      t1->set_name (munge_name_for_enumval (t1->name (),
                                            t1->local_name ()));
    }

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (t, I_FALSE)) != 0)
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

  // Add it to enclosing scope.
  idl_global->scopes ()->next_to_top ()->fe_add_enum_val (t1);

  return t;
}

// Redefinition of inherited virtual operations.

// Dump this AST_Enum to the ostream o
void
AST_Enum::dump (ACE_OSTREAM_TYPE &o)
{
  UTL_ScopeActiveIterator *i = 0;
  ACE_NEW (i,
           UTL_ScopeActiveIterator (this,
                                    IK_decls));

  AST_Decl *d = 0;

  if (this->is_local ())
    {
      o << "(local) ";
    }
  else if (this->is_abstract ())
    {
      o << "(abstract) ";
    }

  o << "enum ";
  this->local_name ()->dump (o);
  o << " {\n";

  while (!i->is_done ())
    {
      d = i->item ();
      d->local_name ()->dump (o);
      i->next ();

      if (!i->is_done ())
        {
          o << ", ";
        }
    }

  delete i;

  idl_global->indent ()->skip_to (o);
  o << "}";
}

int
AST_Enum::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_enum (this);
}

void
AST_Enum::destroy (void)
{
}


// Narrowing methods
IMPL_NARROW_METHODS2(AST_Enum, AST_ConcreteType, UTL_Scope)
IMPL_NARROW_FROM_DECL(AST_Enum)
IMPL_NARROW_FROM_SCOPE(AST_Enum)
