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

#pragma ident "%@(#)ast_operation.cc	1.46% %92/06/11% Sun Microsystems"

/*
 * ast_operation.cc - Implementation of class AST_Operation
 *
 * AST_Operation nodes denote IDL operation declarations
 * AST_Operations are a subclass of AST_Decl (they are not a type!)
 * and of UTL_Scope (the arguments are managed in a scope).
 * AST_Operations have a return type (a subclass of AST_Type),
 * a bitfield for denoting various properties of the operation (the
 * values are ORed together from constants defined in the enum
 * AST_Operation::FLags), a name (a UTL_ScopedName), a context
 * (implemented as a list of Strings, a UTL_StrList), and a raises
 * clause (implemented as an array of AST_Exceptions).
 */

#include	"idl.h"
#include	"idl_extern.h"

/*
 * Constructor(s) and destructor
 */
AST_Operation::AST_Operation()
	     : pd_flags(OP_noflags),
	       pd_return_type(NULL),
	       pd_context(NULL),
	       pd_exceptions(NULL)
{
}

AST_Operation::AST_Operation(AST_Type *rt, Flags fl, UTL_ScopedName *n,
			   UTL_StrList *p)
	     : pd_return_type(rt),
	       pd_flags(fl),
	       pd_context(NULL),
	       pd_exceptions(NULL),
	       AST_Decl(AST_Decl::NT_op, n, p),
	       UTL_Scope(AST_Decl::NT_op)
{
  AST_PredefinedType *pdt;

  /*
   * Check that if the operation is oneway, the return type must be void
   */
  if (rt != NULL && pd_flags == OP_oneway) {
    if (rt->node_type() != AST_Decl::NT_pre_defined)
      idl_global->err()->error1(UTL_Error::EIDL_NONVOID_ONEWAY, this);
    else {
      pdt = AST_PredefinedType::narrow_from_decl(rt);
      if (pdt == NULL || pdt->pt() != AST_PredefinedType::PT_void)
        idl_global->err()->error1(UTL_Error::EIDL_NONVOID_ONEWAY, this);
    }
  }
}

/*
 * Private operations
 */

/*
 * Public operations
 */

/*
 * Redefinition of inherited virtual operations
 */

/*
 * Add this context (a UTL_StrList) to this scope
 */
UTL_StrList *
AST_Operation::fe_add_context(UTL_StrList *t)
{
  pd_context = t;

  return t;
}

/*
 * Add these exceptions (identified by name) to this scope.
 * This looks up each name to resolve it to the name of a known
 * exception, and then adds the referenced exception to the list
 * of exceptions that this operation can raise.
 *
 * NOTE: No attempt is made to ensure that exceptions are mentioned
 *       only once..
 */
UTL_NameList *
AST_Operation::fe_add_exceptions(UTL_NameList *t)
{
  UTL_NamelistActiveIterator *nl_i;
  UTL_ScopedName	     *nl_n;
  UTL_Scope		     *fs = idl_global->scopes()->top();
  AST_Exception		     *fe;
  AST_Decl		     *d;

  pd_exceptions = NULL;
  nl_i = new UTL_NamelistActiveIterator(t);
  while (!(nl_i->is_done())) {
    nl_n = nl_i->item();
    d = lookup_by_name(nl_n, I_TRUE);
    if (d == NULL || d->node_type() != AST_Decl::NT_except) {
      idl_global->err()->lookup_error(nl_n);
      delete nl_i;
      return NULL;
    }
    fe = AST_Exception::narrow_from_decl(d);
    if (fe == NULL) {
      idl_global->err()->error1(UTL_Error::EIDL_ILLEGAL_RAISES, this);
      return NULL;
    }
    if (pd_exceptions == NULL)
      pd_exceptions = new UTL_ExceptList(fe, NULL);
    else
      pd_exceptions->nconc(new UTL_ExceptList(fe, NULL));
    nl_i->next();
  }
  delete nl_i;

  return t;
}

/*
 * Add this AST_Argument node (an operation argument declaration)
 * to this scope
 */
AST_Argument *AST_Operation::fe_add_argument(AST_Argument *t)
{
  AST_Decl *d;

  /*
   * Already defined and cannot be redefined? Or already used?
   */
  if ((d = lookup_by_name_local(t->local_name(), I_FALSE)) != NULL) {
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
   * Cannot add OUT or INOUT argument to oneway operation
   */
  if ((t->direction() == AST_Argument::dir_OUT ||
       t->direction() == AST_Argument::dir_INOUT) &&
      pd_flags == OP_oneway) {
    idl_global->err()->error2(UTL_Error::EIDL_ONEWAY_CONFLICT, t, this);
    return NULL;
  }
  /*
   * Add it to scope
   */
  add_to_scope(t);
  /*
   * Add it to set of locally referenced symbols
   */
  add_to_referenced(t, I_FALSE);

  return t;
}

/*
 * Dump this AST_Operation node (an operation) to the ostream o
 */
void
AST_Operation::dump(ostream &o)
{
  UTL_ScopeActiveIterator   *i;
  UTL_StrlistActiveIterator *si;
  UTL_ExceptlistActiveIterator *ei;
  AST_Decl		    *d;
  AST_Exception		    *e;
  String		    *s;

  if (pd_flags == OP_oneway)
    o << "oneway ";
  else if (pd_flags == OP_idempotent)
    o << "idempotent ";

  i = new UTL_ScopeActiveIterator(this, IK_decls);
  pd_return_type->name()->dump(o);
  o << " ";
  local_name()->dump(o);
  o << "(";
  while (!(i->is_done())) {
    d = i->item();
    d->dump(o);
    i->next();
    if (!(i->is_done()))
      o << ", ";
  }
  delete i;
  o << ")";

  if (pd_exceptions != NULL) {
    o << " raises(";
    ei = new UTL_ExceptlistActiveIterator(pd_exceptions);
    while (!(ei->is_done())) {
      e = ei->item();
      ei->next();
      e->local_name()->dump(o);
      if (!(ei->is_done()))
	o << ", ";
    }
    delete ei;
    o << ")";
  }
  if (pd_context != NULL) {
    o << " context(";
    si = new UTL_StrlistActiveIterator(pd_context);
    while (!(si->is_done())) {
      s = si->item();
      si->next();
      o << s->get_string();
      if (!(si->is_done()))
	o << ", ";
    }
    delete si;
    o << ")";
  }    
}

/*
 * Data accessors
 */

AST_Type *
AST_Operation::return_type()
{
  return pd_return_type;
}

AST_Operation::Flags
AST_Operation::flags()
{
  return pd_flags;
}

UTL_StrList *
AST_Operation::context()
{
  return pd_context;
}

UTL_ExceptList *
AST_Operation::exceptions()
{
  return pd_exceptions;
}

// Narrowing
IMPL_NARROW_METHODS2(AST_Operation, AST_Decl, UTL_Scope)
IMPL_NARROW_FROM_DECL(AST_Operation)
IMPL_NARROW_FROM_SCOPE(AST_Operation)
