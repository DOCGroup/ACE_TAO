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

/*
 * ast_module.cc - Implementation of class AST_Module
 *
 * AST_Modules denote IDL module declarations
 * AST_Modules are subclasses of AST_Decl (they are not a type!) and
 * of UTL_Scope.
 */

#include	"idl.h"
#include	"idl_extern.h"

ACE_RCSID(ast, ast_module, "$Id$")

/*
 * Constructor(s) and destructor
 */
AST_Module::AST_Module()
{
}

AST_Module::AST_Module(UTL_ScopedName *n, UTL_StrList *p)
	 : AST_Decl(AST_Decl::NT_module, n, p),
	   UTL_Scope(AST_Decl::NT_module)
{
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
 * Add this AST_PredefinedType node (a predefined type declaration) to
 * this scope
 */
AST_PredefinedType *AST_Module::fe_add_predefined_type(AST_PredefinedType *t)
{
  AST_Decl *d;

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

  return t;
}

/*
 * Add this AST_Module node (a module declaration) to this scope
 */
AST_Module *AST_Module::fe_add_module(AST_Module *t)
{
  AST_Decl *d;

  /*
   * Already defined and cannot be redefined? Or already used?
   */
  if ((d = lookup_for_add(t, I_FALSE)) != NULL) {
    if (!can_be_redefined(d)) {
      idl_global->err()->error3(UTL_Error::EIDL_REDEF, t, this, d);
      return NULL;
    }
    // if our platform supports namespaces, we allow reopening
    // modules. However, if namespace support is not available, this is flagged
    // as an error

#ifndef ACE_HAS_USING_KEYWORD
    if (referenced(d)) {
      idl_global->err()->error3(UTL_Error::EIDL_DEF_USE, t, this, d);
      return NULL;
    }
#endif /* ACE_HAS_USING_KEYWORD */

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

  return t;
}

/*
 * Add this AST_Interface node (an interface declaration) to this scope
 */
AST_Interface *AST_Module::fe_add_interface(AST_Interface *t)
{
  AST_Decl		*predef;
  AST_Interface		*fwd;

  /*
   * Already defined?
   */
  if ((predef = lookup_for_add(t, I_FALSE)) != NULL) {
    /*
     * Treat fwd declared interfaces specially
     */
    if (predef->node_type() == AST_Decl::NT_interface) {
      fwd = AST_Interface::narrow_from_decl(predef);

      if (fwd == NULL)
          return NULL;

      if (!fwd->is_defined()) {	/* Forward declared and not defined yet */
	if (fwd->defined_in() != this) {
	  idl_global->err()
	     ->error3(UTL_Error::EIDL_SCOPE_CONFLICT, fwd, t, this);
	  return NULL;
        }
      }
      /*
       * OK, not illegal redef of forward declaration. Now check whether
       * it has been referenced already
       */
      else if (referenced(predef)) {
        idl_global->err()->error3(UTL_Error::EIDL_DEF_USE, t, this, predef);
	return NULL;
      }
    } else if (!can_be_redefined(predef)) {
      idl_global->err()->error3(UTL_Error::EIDL_REDEF, t, this, predef);
      return NULL;
    } else if (referenced(predef)) {
      idl_global->err()->error3(UTL_Error::EIDL_DEF_USE, t, this, predef);
      return NULL;
    } else if (t->has_ancestor(predef)) {
      idl_global->err()->redefinition_in_scope(t, predef);
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

  return t;
}

/*
 * Add this AST_InterfaceFwd node (a forward declaration of an IDL
 * interface) to this scope
 */
AST_InterfaceFwd *AST_Module::fe_add_interface_fwd(AST_InterfaceFwd *i)
{
  AST_Decl	*d;
  AST_Interface	*itf;

  /*
   * Already defined and cannot be redefined? Or already used?
   */
  if ((d = lookup_for_add(i, I_FALSE)) != NULL) {
    if (d->node_type() == AST_Decl::NT_interface &&
	d->defined_in() == this) {
      itf = AST_Interface::narrow_from_decl(d);
      if (itf == NULL)
          return NULL;

      i->set_full_definition(itf);
      return i;
    }
    if (!can_be_redefined(d)) {
      idl_global->err()->error3(UTL_Error::EIDL_REDEF, i, this, d);
      return NULL;
    }
    if (referenced(d)) {
      idl_global->err()->error3(UTL_Error::EIDL_DEF_USE, i, this, d);
      return NULL;
    }
    if (i->has_ancestor(d)) {
      idl_global->err()->redefinition_in_scope(i, d);
      return NULL;
    }
  }
  /*
   * Add it to scope
   */
  add_to_scope(i);
  /*
   * Add it to set of locally referenced symbols
   */
  add_to_referenced(i, I_FALSE);

  return i;
}

/*
 * Add this AST_Constant node (a constant declaration) to this scope
 */
AST_Constant *AST_Module::fe_add_constant(AST_Constant *t)
{
  AST_Decl *d;

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

  return t;
}

/*
 * Add this AST_Exception node (an exception declaration) to this scope
 */
AST_Exception *AST_Module::fe_add_exception(AST_Exception *t)
{
  AST_Decl *d;

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

  return t;
}

/*
 * Add this AST_Union node (a union declaration) to this scope
 */
AST_Union *AST_Module::fe_add_union(AST_Union *t)
{
  AST_Decl *d;

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

  return t;
}

/*
 * Add this AST_Structure node (a struct declaration) to this scope
 */
AST_Structure *AST_Module::fe_add_structure(AST_Structure *t)
{
  AST_Decl *d;

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

  return t;
}

/*
 * Add this AST_Enum node (an enum declaration) to this scope
 */
AST_Enum *AST_Module::fe_add_enum(AST_Enum *t)
{
  AST_Decl *d;

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

  return t;
}

/*
 * Add this AST_EnumVal node (an enumerator declaration) to this scope
 * This is done to conform to the C++ scoping rules which declare
 * enumerators in the enclosing scope (in addition to declaring them
 * in the enum itself)
 */
AST_EnumVal *AST_Module::fe_add_enum_val(AST_EnumVal *t)
{
  AST_Decl *d;

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

  return t;
}

/*
 * Add this AST_Typedef node (a typedef) to this scope
 */
AST_Typedef *AST_Module::fe_add_typedef(AST_Typedef *t)
{
  AST_Decl *d;

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

  return t;
}

/*
 * Add an AST_Native (a native declaration) to this scope
 */
AST_Native *AST_Module::fe_add_native (AST_Native *t)
{
  AST_Decl *d;

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

  return t;
}

/*
 * Dump this AST_Module node to the ostream o
 */
void
AST_Module::dump(ostream &o)
{
  o << "module ";
  local_name()->dump(o);
  o << " {\n";
  UTL_Scope::dump(o);
  idl_global->indent()->skip_to(o);
  o << "}";
}

/*
 * Narrowing methods
 */
IMPL_NARROW_METHODS2(AST_Module, AST_Decl, UTL_Scope)
IMPL_NARROW_FROM_DECL(AST_Module)
IMPL_NARROW_FROM_SCOPE(AST_Module)
