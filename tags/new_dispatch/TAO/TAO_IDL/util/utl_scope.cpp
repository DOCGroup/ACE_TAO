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

#pragma ident "%@(#)utl_scope.cc	1.46% %92/06/10% Sun Microsystems"

/*
 * utl_scope.cc - Implementation of class UTL_Scope
 */

#include	"idl.h"
#include	"idl_extern.h"

#undef	INCREMENT
#define	INCREMENT	64

/*
 * Static functions
 */
static Identifier	*_global_scope_name = NULL;
static Identifier	*_global_scope_root_name = NULL;

/*
 * Determines if a name is global
 */
static long
is_global_name(Identifier *i)
{
  long comp_result = 0;
  char *s;

  if (i == NULL) return comp_result;
  s = i->get_string();

  if (_global_scope_name == NULL)
    _global_scope_name = new Identifier("::", 1, 0, I_FALSE);

  if (_global_scope_root_name == NULL)
    _global_scope_root_name = new Identifier("", 1, 0, I_FALSE);

  comp_result = i->compare(_global_scope_name);
  if (!comp_result)
    comp_result = i->compare(_global_scope_root_name);

  return comp_result;
}

/*
 * Helper function for lookup_by_name. Iterates doing local lookups of
 * subsequent components of a scoped name
 */
static AST_Decl *
iter_lookup_by_name_local(AST_Decl *d, UTL_ScopedName *e,
			  idl_bool treat_as_ref)
{
  Identifier			*s;
  AST_Typedef			*td;
  UTL_IdListActiveIterator	*i;
  UTL_Scope			*sc;

  i = new UTL_IdListActiveIterator(e);
  for (i->next(); !(i->is_done()); ) {
    s = i->item();
    /*
     * Update iterator before loop. This is needed for the check for
     * typedef, since we only want to look at the base type if there
     * actually are more components of the name to resolve.
     */
    i->next();
    /*
     * Next component in name was not found
     */
    if (d == NULL) {
      return NULL;
    }
    /*
     * If this is a typedef and we're not done, we should get the
     * base type to get the scope it defines (if any)
     */
    if (!(i->is_done())) {
      while (d != NULL && d->node_type() == AST_Decl::NT_typedef) {
        td = AST_Typedef::narrow_from_decl(d);
	if (td == NULL)
	  return NULL;
	d = td->base_type();
      }
      if (d == NULL)
        return NULL;
    }
    /*
     * Try to convert the AST_Decl to a UTL_Scope
     */
    sc = DeclAsScope(d);
    if (sc == NULL)
      return NULL;
    /*
     * Look up the next element
     */
    d = sc->lookup_by_name_local(s, treat_as_ref);
  }
  /*
   * OK, done with the loop
   */
  return d;
}

/*
 * Constructor(s)
 */
UTL_Scope::UTL_Scope()
	 : pd_scope_node_type(AST_Decl::NT_module),
	   pd_decls(NULL),
	   pd_decls_allocated(0),
	   pd_decls_used(0),
	   pd_local_types(NULL),
	   pd_locals_allocated(0),
	   pd_locals_used(0),
	   pd_referenced(NULL),
	   pd_referenced_allocated(0),
	   pd_referenced_used(0)
{
}

UTL_Scope::UTL_Scope(AST_Decl::NodeType nt)
	 : pd_scope_node_type(nt),
	   pd_decls(NULL),
	   pd_decls_allocated(0),
	   pd_decls_used(0),
	   pd_local_types(NULL),
	   pd_locals_allocated(0),
	   pd_locals_used(0),
	   pd_referenced(NULL),
	   pd_referenced_allocated(0),
	   pd_referenced_used(0)
{
};

/*
 * Private operations
 */

static 
AST_Decl * add_type(AST_Type *type)
{
  AST_Decl * result = 0;
  UTL_Scope * scope = 0;
  switch (type->node_type()) {
    case AST_Decl::NT_array:
      result =
        idl_global->root()->add_array(AST_Array::narrow_from_decl(type));
      break;
    case AST_Decl::NT_enum:
      result = type->defined_in()->add_enum(AST_Enum::narrow_from_decl(type));
      scope = AST_Enum::narrow_from_decl(type); 
      break;
    case AST_Decl::NT_sequence:
      result =
	idl_global->root()->add_sequence(AST_Sequence::narrow_from_decl(type));
      break;
    case AST_Decl::NT_string:
      result = 
	idl_global->root()->add_string(AST_String::narrow_from_decl(type));
      break;
    case AST_Decl::NT_struct:
      result = 
        type->defined_in()->
          add_structure(AST_Structure::narrow_from_decl(type));
      scope = AST_Structure::narrow_from_decl(type);
      break;
    case AST_Decl::NT_union:
      result =
	type->defined_in()->add_union(AST_Union::narrow_from_decl(type));
      scope = AST_Union::narrow_from_decl(type);
      break;
    default:
      // for non-complex types, like predefined types
      // no additional add needed, assume everything is ok
      result = (AST_Decl *) 1;
      break;
  }
  if (scope)
    result = scope->call_add();
  return result; 
}

/*
 * Protected operations
 */

/*
 * Special version of lookup which only looks at the local name instead of
 * the fully scoped name, when doing lookups. This version is intended to
 * be used only by the CFE add_xxx functions
 */
AST_Decl *
UTL_Scope::lookup_for_add(AST_Decl *d, idl_bool treat_as_ref)
{
  if (d == NULL)
    return NULL;
  return lookup_by_name_local(d->local_name(), treat_as_ref);
}

/*
 * Public operations
 */

// Narrowing
IMPL_NARROW_METHODS0(UTL_Scope)
IMPL_NARROW_FROM_SCOPE(UTL_Scope)

// Scope Management Protocol
//
// All members of the protocol defined in UTL_Scope simply return the node
// and don't do a thing. These members are simply dummies to retain
// compatibility with pre-two-pass compiler back-ends.

AST_PredefinedType *UTL_Scope::add_predefined_type(AST_PredefinedType *p)
{
  if (p == NULL) return NULL;
  p->set_added(I_TRUE);
  return p;
}

AST_Module *UTL_Scope::add_module(AST_Module *m)
{
  if (m == NULL) return NULL;
  m->set_added(I_TRUE);
  return m;
}

AST_Interface *UTL_Scope::add_interface(AST_Interface *i)
{
  if (i == NULL) return NULL;
  i->set_added(I_TRUE);
  return i;
}

AST_InterfaceFwd *UTL_Scope::add_interface_fwd(AST_InterfaceFwd *i)
{
  if (i == NULL) return NULL;
  i->set_added(I_TRUE);
  return i;
}

AST_Exception *UTL_Scope::add_exception(AST_Exception *e)
{
  if (e == NULL) return NULL;
  e->set_added(I_TRUE);
  return e;
}

AST_Constant *UTL_Scope::add_constant(AST_Constant *c)
{
  if (c == NULL) return NULL;
  c->set_added(I_TRUE);
  return c;
}

UTL_StrList *UTL_Scope::add_context(UTL_StrList *c)
{
  return c;
}

UTL_NameList *UTL_Scope::add_exceptions(UTL_NameList *e)
{
  return e;
}

AST_Attribute *UTL_Scope::add_attribute(AST_Attribute *a)
{
  if (a == NULL) return NULL;
  a->set_added(I_TRUE);
  if (!a->field_type()->added()) {
    return add_type(a->field_type()) ? a : NULL;
  } else
    return a;
}

AST_Operation *UTL_Scope::add_operation(AST_Operation *o)
{
  if (o == NULL) return NULL;
  o->set_added(I_TRUE);
  if (!o->return_type()->added()) {
    return add_type(o->return_type()) ? o : NULL;
  } else
    return o;
}

AST_Argument *UTL_Scope::add_argument(AST_Argument *a)
{
  if (a == NULL) return NULL;
  a->set_added(I_TRUE);
  if (!a->field_type()->added()) {
    return add_type(a->field_type()) ? a : NULL;
  } else
    return a;
}

AST_Union *UTL_Scope::add_union(AST_Union *u)
{
  if (u == NULL) return NULL;
  u->set_added(I_TRUE);
  return u;
}

AST_UnionBranch *UTL_Scope::add_union_branch(AST_UnionBranch *u)
{
  if (u == NULL) return NULL;
  u->set_added(I_TRUE);
  if (!u->field_type()->added()) {
    return add_type(u->field_type()) ? u : NULL;
  } else 
    return u;
}

AST_Structure *UTL_Scope::add_structure(AST_Structure *s)
{
  if (s == NULL) return NULL;
  s->set_added(I_TRUE);
  return s;
}

AST_Field *UTL_Scope::add_field(AST_Field *f)
{
  if (f == NULL) return NULL;
  f->set_added(I_TRUE);
  if (!f->field_type()->added()) {
    return add_type(f->field_type()) ? f : NULL;
  } else
    return f;
}

AST_Enum *UTL_Scope::add_enum(AST_Enum *e)
{
  if (e == NULL) return NULL;
  e->set_added(I_TRUE);
  return e;
}

AST_EnumVal *UTL_Scope::add_enum_val(AST_EnumVal *e)
{
  if (e == NULL) return NULL;
  e->set_added(I_TRUE);
  return e;
}

AST_Typedef *UTL_Scope::add_typedef(AST_Typedef *t)
{
  if (t == NULL) return NULL;
  t->set_added(I_TRUE);
  if (!t->base_type()->added()) {
    return add_type(t->base_type()) ? t : NULL;
  } else 
    return t;
}

AST_Sequence *UTL_Scope::add_sequence(AST_Sequence *s)
{
  if (s == NULL) return NULL;
  s->set_added(I_TRUE);
  if (!s->base_type()->added()) {
    return add_type(s->base_type()) ? s : NULL;
  } else
    return s;
}

AST_String *UTL_Scope::add_string(AST_String *s)
{
  if (s == NULL) return NULL;
  s->set_added(I_TRUE);
  return s;
}

AST_Array *UTL_Scope::add_array(AST_Array *a)
{
  if (a == NULL) return NULL;
  a->set_added(I_TRUE);
  if (!a->base_type()->added()) {
    return add_type(a->base_type()) ? a : NULL;
  } else 
    return a;
}

// Protected Front End Scope Management Protocol
//
// All members of the protocol defined in UTL_Scope simply return NULL
// and don't do a thing. This ensures that runtime errors will discover
// operations which should have been redefined to allow certain kinds of
// AST nodes to appear in a given context

AST_PredefinedType *UTL_Scope::fe_add_predefined_type(AST_PredefinedType *)
{
  return NULL;
}

AST_Module *UTL_Scope::fe_add_module(AST_Module *)
{
  return NULL;
}

AST_Interface *UTL_Scope::fe_add_interface(AST_Interface *)
{
  return NULL;
}

AST_InterfaceFwd *UTL_Scope::fe_add_interface_fwd(AST_InterfaceFwd *)
{
  return NULL;
}

AST_Exception *UTL_Scope::fe_add_exception(AST_Exception *)
{
  return NULL;
}

AST_Constant *UTL_Scope::fe_add_constant(AST_Constant *)
{
  return NULL;
}

UTL_StrList *UTL_Scope::fe_add_context(UTL_StrList *)
{
  return NULL;
}

UTL_NameList *UTL_Scope::fe_add_exceptions(UTL_NameList *)
{
  return NULL;
}

AST_Attribute *UTL_Scope::fe_add_attribute(AST_Attribute *)
{
  return NULL;
}

AST_Operation *UTL_Scope::fe_add_operation(AST_Operation *)
{
  return NULL;
}

AST_Argument *UTL_Scope::fe_add_argument(AST_Argument *)
{
  return NULL;
}

AST_Union *UTL_Scope::fe_add_union(AST_Union *)
{
  return NULL;
}

AST_UnionBranch *UTL_Scope::fe_add_union_branch(AST_UnionBranch *)
{
  return NULL;
}

AST_Structure *UTL_Scope::fe_add_structure(AST_Structure *)
{
  return NULL;
}

AST_Field *UTL_Scope::fe_add_field(AST_Field *)
{
  return NULL;
}

AST_Enum *UTL_Scope::fe_add_enum(AST_Enum *)
{
  return NULL;
}

AST_EnumVal *UTL_Scope::fe_add_enum_val(AST_EnumVal *)
{
  return NULL;
}

AST_Typedef *UTL_Scope::fe_add_typedef(AST_Typedef *)
{
  return NULL;
}

AST_Sequence *UTL_Scope::fe_add_sequence(AST_Sequence *)
{
  return NULL;
}

AST_String *UTL_Scope::fe_add_string(AST_String *)
{
  return NULL;
}

AST_Array *UTL_Scope::fe_add_array(AST_Array *)
{
  return NULL;
}

// This is the second pass of the front end
// It calls the public add protocol on everything in scope.
// It calls the add_xx functions of the most derived AST_Node.

AST_Decl *UTL_Scope::call_add()
{
  AST_Decl 		*result = NULL;
  AST_Decl 		*decl;

  UTL_ScopeActiveIterator *i;
  UTL_Scope *scope;

  i = new UTL_ScopeActiveIterator(this, UTL_Scope::IK_decls);
  while(!(i->is_done())) {
    decl = i->item();
    scope = 0;
    switch (decl->node_type()) {
      case AST_Decl::NT_argument: 
        result = add_argument(AST_Argument::narrow_from_decl(decl));
  	break;
      case AST_Decl::NT_array:
    	result = add_array(AST_Array::narrow_from_decl(decl));
	break;
      case AST_Decl::NT_attr: 
        result = add_attribute(AST_Attribute::narrow_from_decl(decl));
	break;
      case AST_Decl::NT_const:
        result = add_constant(AST_Constant::narrow_from_decl(decl));
	break;
      case AST_Decl::NT_enum:
        scope = AST_Enum::narrow_from_decl(decl);
        result = add_enum(AST_Enum::narrow_from_decl(decl));
	break;
      case AST_Decl::NT_enum_val:
        result = add_enum_val(AST_EnumVal::narrow_from_decl(decl)); 
	break;
      case AST_Decl::NT_except:
        scope = AST_Exception::narrow_from_decl(decl);
        result = add_exception(AST_Exception::narrow_from_decl(decl));
	break;
      case AST_Decl::NT_field:
        result = add_field(AST_Field::narrow_from_decl(decl));
	break;
      case AST_Decl::NT_interface:
        scope = AST_Interface::narrow_from_decl(decl);
        result = add_interface(AST_Interface::narrow_from_decl(decl));
	break;
      case AST_Decl::NT_interface_fwd:
        result = add_interface_fwd(AST_InterfaceFwd::narrow_from_decl(decl));
	break;
      case AST_Decl::NT_module:
      	scope = AST_Module::narrow_from_decl(decl);
  	result = add_module(AST_Module::narrow_from_decl(decl)); 
  	break;
      case AST_Decl::NT_op:
	result = add_operation(AST_Operation::narrow_from_decl(decl));
        scope = AST_Operation::narrow_from_decl(decl);
	break;
      case AST_Decl::NT_pre_defined:
        result =
	  add_predefined_type(AST_PredefinedType::narrow_from_decl(decl));
 	break;
      case AST_Decl::NT_sequence:
        result = add_sequence(AST_Sequence::narrow_from_decl(decl));
 	break;
      case AST_Decl::NT_string:
        result = add_string(AST_String::narrow_from_decl(decl));
	break;
      case AST_Decl::NT_struct:
        result = add_structure(AST_Structure::narrow_from_decl(decl));
        scope = AST_Structure::narrow_from_decl(decl);
  	break;
      case AST_Decl::NT_typedef:
        result = add_typedef(AST_Typedef::narrow_from_decl(decl));
	break;
      case AST_Decl::NT_union:
        result = add_union(AST_Union::narrow_from_decl(decl));
        scope = AST_Union::narrow_from_decl(decl);
	break;
      case AST_Decl::NT_union_branch:
        result = add_union_branch(AST_UnionBranch::narrow_from_decl(decl));
	break;
      default:
        return NULL;
    }
    if (scope)
      scope->call_add();
    i->next();
  }
  return result;
}

// Private lookup mechanism

/*
 * Lookup the node for a primitive (built in) type
 */
AST_Decl *
UTL_Scope::lookup_primitive_type(AST_Expression::ExprType et)
{
  AST_Decl		   *as_decl;
  UTL_Scope		   *ancestor;
  AST_PredefinedType	   *t;
  UTL_ScopeActiveIterator *i;
  AST_PredefinedType::PredefinedType pdt;

  as_decl = ScopeAsDecl(this);
  if (as_decl == NULL)
    return NULL;
  ancestor = as_decl->defined_in();
  if (ancestor != NULL)
    return ancestor->lookup_primitive_type(et);

  switch (et) {
  case AST_Expression::EV_short:
    pdt = AST_PredefinedType::PT_short;
    break;
  case AST_Expression::EV_ushort:
    pdt = AST_PredefinedType::PT_ushort;
    break;
  case AST_Expression::EV_long:
    pdt = AST_PredefinedType::PT_long;
    break;
  case AST_Expression::EV_ulong:
    pdt = AST_PredefinedType::PT_ulong;
    break;
  case AST_Expression::EV_longlong:
    pdt = AST_PredefinedType::PT_longlong;
    break;
  case AST_Expression::EV_ulonglong:
    pdt = AST_PredefinedType::PT_ulonglong;
    break;
  case AST_Expression::EV_float:
    pdt = AST_PredefinedType::PT_float;
    break;
  case AST_Expression::EV_double:
    pdt = AST_PredefinedType::PT_double;
    break;
  case AST_Expression::EV_longdouble:
    pdt = AST_PredefinedType::PT_longdouble;
    break;
  case AST_Expression::EV_char:
    pdt = AST_PredefinedType::PT_char;
    break;
  case AST_Expression::EV_wchar:
    pdt = AST_PredefinedType::PT_wchar;
    break;
  case AST_Expression::EV_octet:
    pdt = AST_PredefinedType::PT_octet;
    break;
  case AST_Expression::EV_bool:
    pdt = AST_PredefinedType::PT_boolean;
    break;
  case AST_Expression::EV_any:
    pdt = AST_PredefinedType::PT_any;
    break;
  case AST_Expression::EV_void:
    pdt = AST_PredefinedType::PT_void;
    break;
  case AST_Expression::EV_string:
  case AST_Expression::EV_wstring:
  case AST_Expression::EV_none:
    return NULL;
  }
  i = new UTL_ScopeActiveIterator(this, UTL_Scope::IK_decls);
  while (!(i->is_done())) {
    as_decl = i->item();
    if (as_decl->node_type() == AST_Decl::NT_pre_defined) {
      t = AST_PredefinedType::narrow_from_decl(as_decl);
      if (t == NULL) {
        i->next();
        continue;
      }
      if (t->pt() == pdt) {
	delete i;
	return t;
      }
    }
    i->next();
  }
  delete i;
  return NULL;
}

/*
 * Look through inherited interfaces
 */
AST_Decl *
UTL_Scope::look_in_inherited(UTL_ScopedName *e, idl_bool treat_as_ref)
{
  AST_Decl		*d = NULL;
  AST_Interface		*i = AST_Interface::narrow_from_scope(this);
  AST_Interface		**is;
  long			nis;

  /*
   * This scope is not an interface..
   */
  if (i == NULL)
    return NULL;
  /*
   * Can't look in an interface which was not yet defined
   */
  if (!i->is_defined()) {
    idl_global->err()->fwd_decl_lookup(i, e);
    return NULL;
  }
  /*
   * OK, loop through inherited interfaces. Stop when you find it
   */
  for (nis = i->n_inherits(), is = i->inherits(); nis > 0; nis--, is++) {
    d = (*is)->lookup_by_name(e, treat_as_ref);
    if (d != NULL)
      return d;
  }
  /*
   * Not found
   */
  return NULL;
}    

/*
 * Look up a String * in local scope only
 */
AST_Decl *
UTL_Scope::lookup_by_name_local(Identifier *e, idl_bool)
{
  UTL_ScopeActiveIterator *i = new UTL_ScopeActiveIterator(this,
							   UTL_Scope::IK_both);
  AST_Decl		  *d;
  AST_InterfaceFwd	  *fwd;

  /*
   * Iterate over this scope
   */
  while (!(i->is_done())) {
    d = i->item();
    if (d->local_name() != NULL && d->local_name()->compare(e)) {
      delete i;
      /*
       * Special case for forward declared interfaces. Look through the
       * forward declaration and retrieve the full definition
       */
      if (d->node_type() == AST_Decl::NT_interface_fwd) {
        fwd = AST_InterfaceFwd::narrow_from_decl(d);
	if (fwd == NULL)
	  d = NULL;
	else
	  d = fwd->full_definition();
      }
      return d;
    }
    i->next();
  }
  /*
   * OK, not found, return NULL
   */
  return NULL;
}

/*
 * Implements lookup by name for scoped names
 */
AST_Decl *
UTL_Scope::lookup_by_name(UTL_ScopedName *e, idl_bool treat_as_ref)
{
  AST_Decl		     *d;
  UTL_Scope		     *t = NULL;

  /*
   * Empty name? error
   */
  if (e == NULL) {
    return NULL;
  }
  /*
   * If name starts with "::" or "" start look up in global scope
   */
  if (is_global_name(e->head())) {
    /*
     * Get parent scope
     */
    d = ScopeAsDecl(this);
    if (d == NULL)
      return NULL;
    t = d->defined_in();
    /*
     * If this is the global scope..
     */
    if (t == NULL) {
      /*
       * Look up tail of name starting here
       */
      d = lookup_by_name((UTL_ScopedName *) e->tail(), treat_as_ref);
      /*
       * Now return whatever we have
       */
      return d;
    }
    /*
     * OK, not global scope yet, so simply iterate with parent scope
     */
    d = t->lookup_by_name(e, treat_as_ref);
    /*
     * If treat_as_ref is true and d is not NULL, add d to
     * set of nodes referenced here
     */
    if (treat_as_ref && d != NULL)
      add_to_referenced(d, I_FALSE);
    /*
     * Now return what we have
     */
    return d;
  }
  /*
   * The name does not start with "::"
   *
   * Is name defined here?
   */
  d = lookup_by_name_local(e->head(), treat_as_ref);
  if (d == NULL) {
    /*
     * OK, not found. Go down parent scope chain.
     */
    d = ScopeAsDecl(this);
    if (d != NULL) {
      t = d->defined_in();
      if (t == NULL)
	d = NULL;
      else
	d = t->lookup_by_name(e, treat_as_ref);
    }
    /*
     * Special case for scope which is an interface. We have to look
     * in the inherited interfaces as well..
     */
    if (d == NULL) {
      if (pd_scope_node_type == AST_Decl::NT_interface)
	d = look_in_inherited(e, treat_as_ref);
    }
    /*
     * If treat_as_ref is true and d is not NULL, add d to
     * set of nodes referenced here
     */
    if (treat_as_ref && d != NULL)
      add_to_referenced(d, I_FALSE);
    /*
     * OK, now return whatever we found
     */
    return d;
  }
  /*
   * OK, start of name is defined. Now loop doing local lookups
   * of subsequent elements of the name
   */
  d = iter_lookup_by_name_local(d, e, treat_as_ref);
  /*
   * If treat_as_ref is true and d is not NULL, add d to set
   * of nodes referenced here.
   */
  if (treat_as_ref && d != NULL)
    add_to_referenced(d, I_FALSE);
  /*
   * All OK, name fully resolved
   */
  return d;
}

// Add a node to set of nodes referenced in this scope
void
UTL_Scope::add_to_referenced(AST_Decl *e, idl_bool recursive)
{
  UTL_Scope	*s;
  AST_Decl	**tmp;
  AST_Interface *itf;
  long		oreferenced_allocated;
  long		i;

  if (e == NULL) return;
  
  // Special case for forward declared interfaces in the
  // scope in which they're defined. Cannot add before full
  // definition is seen
  if (e->node_type() == AST_Decl::NT_interface) {
    itf = AST_Interface::narrow_from_decl(e);
    if (itf != NULL && itf->defined_in() == this && !itf->is_defined())
      return;
  }
  // Only insert if it is not there already
  if (referenced(e))
    return;

  // Make sure there's space for one more
  if (pd_referenced_allocated == pd_referenced_used) {

    oreferenced_allocated	= pd_referenced_allocated;
    pd_referenced_allocated	+= INCREMENT;
    tmp				= new AST_Decl *[pd_referenced_allocated];

    for (i = 0; i < oreferenced_allocated; i++)
      tmp[i] = pd_referenced[i];

    delete pd_referenced;

    pd_referenced = tmp;
  }

  // Insert new reference
  pd_referenced[pd_referenced_used++] = e;

  // Now, if recursive is specified and "this" is not a common ancestor
  // of the referencing scope and the scope of definition of "e" then
  // add "e" to the set of referenced nodes in the parent of "this"
  if (recursive && !(e->has_ancestor(ScopeAsDecl(this)))) {
    s = e->defined_in();
    if (s != NULL)
      s->add_to_referenced(e, recursive);
  }
}
  
// Add a node to set of nodes declared in this scope
void
UTL_Scope::add_to_scope(AST_Decl *e)
{
  AST_Decl	**tmp;
  long		odecls_allocated;
  long		i;

  if (e == NULL) return;
  
  // Make sure there's space for one more
  if (pd_decls_allocated == pd_decls_used) {

    odecls_allocated	= pd_decls_allocated;
    pd_decls_allocated	+= INCREMENT;
    tmp			= new AST_Decl *[pd_decls_allocated];

    for (i = 0; i < odecls_allocated; i++)
      tmp[i] = pd_decls[i];

    delete pd_decls;

    pd_decls = tmp;
  }

  // Insert new decl
  pd_decls[pd_decls_used++] = e;
}

// Add a node to set of nodes representing manifest types defined in this scope
void
UTL_Scope::add_to_local_types(AST_Decl *e)
{
  AST_Decl	**tmp;
  long		olocals_allocated;
  long		i;

  if (e == NULL) return;

  // Make sure there's space for one more
  if (pd_locals_allocated == pd_locals_used) {

    olocals_allocated	= pd_locals_allocated;
    pd_locals_allocated 	+= INCREMENT;
    tmp			= new AST_Decl *[pd_locals_allocated];

    for (i = 0; i < olocals_allocated; i++)
      tmp[i] = pd_local_types[i];

    delete pd_local_types;

    pd_local_types = tmp;
  }

  // Insert new decl
  pd_local_types[pd_locals_used++] = e;
}

// Has this node been referenced here before?
idl_bool
UTL_Scope::referenced(AST_Decl *e)
{
  long		i = pd_referenced_used;
  AST_Decl	**tmp = pd_referenced;

  for (; i > 0; i--, tmp++)
    if (*tmp == e)	// Same node?
      return I_TRUE;

  return I_FALSE;		// Not found
}

/*
 * Redefinition of inherited virtual operations
 */

// AST Dumping
void
UTL_Scope::dump(ostream &o)
{
  UTL_ScopeActiveIterator	*i;
  AST_Decl			*d;

  if (idl_global->indent() == NULL)
    idl_global->set_indent(new UTL_Indenter());

  idl_global->indent()->increase();

  if (pd_locals_used > 0) {
    i = new UTL_ScopeActiveIterator(this, UTL_Scope::IK_localtypes);

    o << GTDEVEL("\n/* Locally defined types: */\n");
    while (!(i->is_done())) {
      d = i->item();
      if (!d->imported()) {
	idl_global->indent()->skip_to(o);
	d->dump(o);
	o << "\n";
      }
      i->next();
    }
    delete i;
  }

  if (pd_decls_used > 0) {
    i = new UTL_ScopeActiveIterator(this, UTL_Scope::IK_decls);

    o << GTDEVEL("\n/* Declarations: */\n");
    while (!(i->is_done())) {
      d = i->item();
      if (!d->imported()) {
	idl_global->indent()->skip_to(o);
	d->dump(o);
	o << ";\n";
      }
      i->next();
    }
    delete i;
  }

  idl_global->indent()->decrease();
}

// How many entries are defined?
unsigned long
UTL_Scope::nmembers()
{
   return pd_decls_used;
}

// UTL_SCOPE_ACTIVE_ITERATOR

/*
 * Constructor
 */

UTL_ScopeActiveIterator::UTL_ScopeActiveIterator(
				       UTL_Scope *s,
				       UTL_Scope::ScopeIterationKind i)
			: iter_source(s),
			  ik(i),
			  il(0),
			  stage(i == UTL_Scope::IK_both
				? UTL_Scope::IK_decls
				: i)
{
}

/*
 * Private operations
 */

/*
 * Public operations
 */

// Advance to next iterm
void
UTL_ScopeActiveIterator::next()
{
  il++;
}

// Get current item
AST_Decl	*
UTL_ScopeActiveIterator::item()
{
  if (is_done())
    return NULL;
  if (stage == UTL_Scope::IK_decls)
    return iter_source->pd_decls[il];
  if (stage == UTL_Scope::IK_localtypes)
    return iter_source->pd_local_types[il];

  return NULL;
}

// Is this iteration done?
idl_bool
UTL_ScopeActiveIterator::is_done()
{
  long	limit;

  limit = (stage == UTL_Scope::IK_decls) 
    ? iter_source->pd_decls_used 
    : iter_source->pd_locals_used;

  for (;;) {
    if (il < limit)				// Last element?
      return I_FALSE;
    if (stage == UTL_Scope::IK_localtypes)	// Already done local types?
      return I_TRUE;
    if (ik == UTL_Scope::IK_decls)		// Only want decls?
      return I_TRUE;

    // Switch to next stage
    stage = UTL_Scope::IK_localtypes;
    il = 0;
    limit = iter_source->pd_locals_used;
  }
}

// What kind of iterator is this?
UTL_Scope::ScopeIterationKind
UTL_ScopeActiveIterator::iteration_kind()
{
  return ik;
}

// And where are we in the iteration?
UTL_Scope::ScopeIterationKind
UTL_ScopeActiveIterator::iteration_stage()
{
  return stage;
}
