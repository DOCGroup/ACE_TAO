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
 * ast_structure.cc - Implementation of class AST_Structure
 *
 * AST_Structure nodes denote IDL struct declarations.
 * AST_Structure is a subclass of AST_ConcreteType and of UTL_Scope (the
 * structure's fields are managed in a scope).
 */

#include	"idl.h"
#include	"idl_extern.h"

ACE_RCSID(ast, ast_structure, "$Id$")

/*
 * Constructor(s) and destructor
 */
AST_Structure::AST_Structure()
{
}

AST_Structure::AST_Structure(UTL_ScopedName *n, UTL_StrList *p)
	     : AST_Decl(AST_Decl::NT_struct, n, p),
	       UTL_Scope(AST_Decl::NT_struct)
{
}

AST_Structure::AST_Structure(AST_Decl::NodeType nt,
			     UTL_ScopedName *n,
			     UTL_StrList *p)
	     : AST_Decl(nt, n, p),
	       UTL_Scope(nt)
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
 * Add this AST_Field node (a field declaration) to this scope
 */
AST_Field *AST_Structure::fe_add_field(AST_Field *t)
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
 * Add an AST_Structure node (a manifest struct type) to this scope
 */
AST_Structure *AST_Structure::fe_add_structure(AST_Structure *t)
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
   * Add it to local types
   */
  add_to_local_types(t);
  /*
   * Add it to set of locally referenced symbols
   */
  add_to_referenced(t, I_FALSE);

  return t;
}

/*
 * Add an AST_Union node (a manifest union type) to this scope
 */
AST_Union *AST_Structure::fe_add_union(AST_Union *t)
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
   * Add it to local types
   */
  add_to_local_types(t);
  /*
   * Add it to set of locally referenced symbols
   */
  add_to_referenced(t, I_FALSE);

  return t;
}

/*
 * Add this AST_Enum node (a manifest enum declaration) to this scope
 */
AST_Enum *AST_Structure::fe_add_enum(AST_Enum *t)
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
   * Add it to local types
   */
  add_to_local_types(t);
  /*
   * Add it to set of locally referenced symbols
   */
  add_to_referenced(t, I_FALSE);

  return t;
}

/*
 * Add this AST_EnumVal node (an enumerator declaration) to this scope.
 * This is done to conform to the C++ scoping rules which declare
 * enumerators in the enclosing scope (in addition to declaring them
 * in the enum itself)
 */
AST_EnumVal *AST_Structure::fe_add_enum_val(AST_EnumVal *t)
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
 * Dump this AST_Structure node to the ostream o
 */
void
AST_Structure::dump(ostream &o)
{
  o << "struct ";
  AST_Decl::dump(o);
  o << " {\n";
  UTL_Scope::dump(o);
  idl_global->indent()->skip_to(o);
  o << "}";
}

// Narrowing
IMPL_NARROW_METHODS2(AST_Structure, AST_ConcreteType, UTL_Scope)
IMPL_NARROW_FROM_DECL(AST_Structure)
IMPL_NARROW_FROM_SCOPE(AST_Structure)
