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

#pragma ident "%@(#)ast_enum.cc	1.46% %92/06/11% Sun Microsystems"

/*
 * ast_enum.cc - Implementation of class AST_Enum
 *
 * AST_Enum nodes denote IDL enum declarations.
 * AST_Enums are a subclass of AST_ConcreteType and UTL_Scope.
 * They have a counter denoting the next value to be assigned to
 * an enumerator, and a name (a UTL_ScopedName).
 */

#include	"idl.h"
#include	"idl_extern.h"

/*
 * Constructor(s) and destructor
 */
AST_Enum::AST_Enum()
	: pd_enum_counter(0)
{
}

AST_Enum::AST_Enum(UTL_ScopedName *n, UTL_StrList *p)
       : AST_Decl(AST_Decl::NT_enum, n, p),
	 UTL_Scope(AST_Decl::NT_enum),
	 pd_enum_counter(0)
{
}

/*
 * Private operations
 */

/*
 * Public operations
 */

/*
 * Look up an enumerator by the value of the supplied expression
 */
AST_EnumVal *
AST_Enum::lookup_by_value(const AST_Expression *v)
{
  UTL_ScopeActiveIterator *iter;
  AST_EnumVal		    *item;
  AST_Decl		  *i;

  iter = new UTL_ScopeActiveIterator(this, IK_decls);

  while (!iter->is_done()) {
    i = iter->item();
    item = AST_EnumVal::narrow_from_decl(i);
    if (item->constant_value() == v) {
      delete iter;
      return item;
    }
    iter->next();
  }
  delete iter;
  return NULL;
}

/*
 * Compute the value to be assigned to the next enumerator. Bump the
 * counter
 */
unsigned long
AST_Enum::next_enum_val()
{
  unsigned long i = pd_enum_counter++;

  return i;
}

/*
 * Static helper functions
 */

/*
 * Modify scoped name of an enumval so that it is scoped inside the scope
 * in which the enum is defined and not inside the enum itself
 */
static UTL_ScopedName *
munge_name_for_enumval(UTL_ScopedName *n, Identifier *last_component)
{
  long		  l	= n->length();
  UTL_ScopedName *hold	= n;

  /*
   * last three components are:
   * - scope in which enum is defined
   * - scope for enum
   * - name of enumval
   *
   * We want to stop cdr'ing down the list when the head of the
   * list is at the name for the scope in which the enum is defined
   */
  while (l > 3) {
    l--;
    n = (UTL_ScopedName *) n->tail();
  }

  n->set_tail(new UTL_IdList(last_component, NULL));

  return hold;
}
  
/*
 * Redefinition of inherited virtual operations
 */

/*
 * Add an AST_EnumVal node to this scope
 */
AST_EnumVal *AST_Enum::fe_add_enum_val(AST_EnumVal *t)
{
  AST_Decl *d;
  AST_EnumVal *t1 = NULL;

  if (t != NULL) {
    t1 = idl_global->gen()->create_enum_val
      (t->constant_value()->coerce(AST_Expression::EV_ulong)->u.ulval, 
       t->name(), t->pragmas());
    t->set_name(munge_name_for_enumval(t->name(), t->local_name()));
    t1->set_name(munge_name_for_enumval(t1->name(), t1->local_name()));
  }
  /*
   * Already defined and cannot be redefined? Or already used?
   */
  if ((d = lookup_for_add(t, I_FALSE)) != NULL) {
    if (!can_be_redefined(d)) {
      idl_global->err()->error3(UTL_Error::EIDL_REDEF, t, this, d);
      return NULL;
    }
    if (referenced(d)) {
      idl_global->err()->error3(UTL_Error::EIDL_DEF_USE, t, this, d);
      return NULL;
    }
    if (t->has_ancestor(d)) {
      idl_global->err()->redefinition_in_scope(t, d);
      return NULL;
    }
  }
  /*
   * Add it to scope
   */
  add_to_scope(t);
  /*
   * Add it to set of locally referenced symbols
   */
  add_to_referenced(t, I_FALSE);
  /*
   * Add it to enclosing scope
   */
  idl_global->scopes()->next_to_top()->fe_add_enum_val(t1);

  return t;
}

/*
 * Dump this AST_Enum to the ostream o
 */
void
AST_Enum::dump(ostream &o)
{
  UTL_ScopeActiveIterator *i = new UTL_ScopeActiveIterator(this, IK_decls);
  AST_Decl		    *d;

  o << "enum ";
  local_name()->dump(o);
  o << " {\n";

  while (!(i->is_done())) {
    d = i->item();
    d->local_name()->dump(o);
    i->next();
    if (!(i->is_done()))
      o << ", ";
  }
  delete i;

  idl_global->indent()->skip_to(o);
  o << "}";
}

/*
 * Narrowing methods
 */
IMPL_NARROW_METHODS2(AST_Enum, AST_ConcreteType, UTL_Scope)
IMPL_NARROW_FROM_DECL(AST_Enum)
IMPL_NARROW_FROM_SCOPE(AST_Enum)
