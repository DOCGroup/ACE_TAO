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
 * utl_scope.cc - Implementation of class UTL_Scope
 */

#include "idl.h"
#include "idl_extern.h"

ACE_RCSID(util, utl_scope, "$Id$")

#undef  INCREMENT
#define INCREMENT 64

/*
 * Static functions
 */
static Identifier *_global_scope_name = NULL;
static Identifier *_global_scope_root_name = NULL;

/*
 * Determines if a name is global
 */
static long
is_global_name(Identifier *i)
{
  long comp_result = 0;

  if (i == NULL) return comp_result;

  if (_global_scope_name == NULL)
    _global_scope_name = new Identifier("::", 1, 0, I_FALSE);

  if (_global_scope_root_name == NULL)
    _global_scope_root_name = new Identifier("", 1, 0, I_FALSE);

  comp_result = i->compare(_global_scope_name);
  if (!comp_result)
    comp_result = i->compare(_global_scope_root_name);

  return comp_result;
}

// Helper function for lookup_by_name. Iterates doing local lookups of
// subsequent components of a scoped name.
static AST_Decl *
iter_lookup_by_name_local (AST_Decl *d, 
                           UTL_ScopedName *e,
                           idl_bool treat_as_ref,
                           long scope_offset,
                           UTL_Scope *caller)
{
  Identifier *s;
  AST_Typedef *td;
  UTL_IdListActiveIterator *i;
  UTL_Scope *sc;
  UTL_Scope *t = NULL;

  i = new UTL_IdListActiveIterator (e);

  for (i->next(); !(i->is_done ()); ) 
    {
      s = i->item ();

      // Update iterator before loop. This is needed for the check for
      // typedef, since we only want to look at the base type if there
      // actually are more components of the name to resolve.
      i->next ();
      scope_offset--;

      // Next component in name was not found
      if (d == NULL) 
        {
          delete i;
          return NULL;
        }

      // If this is a typedef and we're not done, we should get the
      // base type to get the scope it defines (if any)
      if (!i->is_done ()) 
        {
          while (d != NULL && d->node_type () == AST_Decl::NT_typedef) 
            {
              td = AST_Typedef::narrow_from_decl (d);

              if (td == NULL) 
                {
                  delete i;
                  return NULL;
                }

              d = td->base_type ();
            }

          if (d == NULL) 
            {
              delete i;
              return NULL;
            }
        }

      // Try to convert the AST_Decl to a UTL_Scope
      sc = DeclAsScope (d);
      if (sc == NULL) 
        {
          delete i;
          return NULL;
        }

      // Look up the next element
      t = d->defined_in ();

      d = sc->lookup_by_name_local (s, 
                                    treat_as_ref, 
                                    0, 
                                    scope_offset);

      // If there is a reopened module in a scope somewhat
      // removed from where we are, we may need to backtrack
      // to find the subsequent declaration(s) of that 
      // module's scope.
      while (d == NULL && t != NULL)
        {
          d = t->lookup_by_name ((UTL_ScopedName *) e->tail (), 
                                 treat_as_ref, 
                                 1, 
                                 1,
                                 scope_offset + 1);

          // If we have a complete match, d can't be NULL
          // and we're done, no sense in further iteration.
          caller->matched = t->matched;

          if (caller->matched == I_TRUE)
            {
              return d;
            }

          AST_Decl *tmp = ScopeAsDecl (t);

          t = tmp->defined_in ();
        }
    }

  // OK, done with the loop
  if (i->is_done () && d != NULL)
    {
      caller->matched = I_TRUE;
    }
  delete i;
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
           pd_referenced_used(0),
           pd_name_referenced(NULL),
           pd_name_referenced_allocated(0),
           pd_name_referenced_used(0)
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
           pd_referenced_used(0),
           pd_name_referenced(NULL),
           pd_name_referenced_allocated(0),
           pd_name_referenced_used(0)
{
}

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
    case AST_Decl::NT_wstring:
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
    return add_type(a->field_type()) ? a : 0 ;
  } else
    return a;
}

AST_Operation *UTL_Scope::add_operation(AST_Operation *o)
{
  if (o == NULL) return NULL;
  o->set_added(I_TRUE);
  if (!o->return_type()->added()) {
    return add_type(o->return_type()) ? o : 0 ;
  } else
    return o;
}

AST_Argument *UTL_Scope::add_argument(AST_Argument *a)
{
  if (a == NULL) return NULL;
  a->set_added(I_TRUE);
  if (!a->field_type()->added()) {
    return add_type(a->field_type()) ? a : 0 ;
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
    return add_type(u->field_type()) ? u : 0 ;
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
    return add_type(f->field_type()) ? f : 0 ;
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
    return add_type(t->base_type()) ? t : 0 ;
  } else
    return t;
}

AST_Sequence *UTL_Scope::add_sequence(AST_Sequence *s)
{
  if (s == NULL) return NULL;
  s->set_added(I_TRUE);
  if (!s->base_type()->added()) {
    return add_type(s->base_type()) ? s : 0 ;
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
    return add_type(a->base_type()) ? a : 0 ;
  } else
    return a;
}

AST_Native *UTL_Scope::add_native (AST_Native *n)
{
  if (n == NULL) return NULL;
  n->set_added (I_TRUE);
  return n;
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

AST_Native *UTL_Scope::fe_add_native (AST_Native *)
{
  return NULL;
}

// This is the second pass of the front end
// It calls the public add protocol on everything in scope.
// It calls the add_xx functions of the most derived AST_Node.

AST_Decl *UTL_Scope::call_add()
{
  AST_Decl              *result = NULL;
  AST_Decl              *decl;

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
      case AST_Decl::NT_native:
        result = add_native (AST_Native::narrow_from_decl(decl));
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
      case AST_Decl::NT_wstring:
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
  AST_Decl                 *as_decl;
  UTL_Scope                *ancestor;
  AST_PredefinedType       *t;
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
  default:
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

// Look through inherited interfaces
AST_Decl *
UTL_Scope::look_in_inherited (UTL_ScopedName *e, 
                              idl_bool treat_as_ref)
{
  AST_Decl *d = NULL;
  AST_Decl *d_before = NULL;
  AST_Interface *i = AST_Interface::narrow_from_scope (this);
  AST_Interface **is;
  long nis;

  // This scope is not an interface..
  if (i == NULL)
    {
      return NULL;
    }

  // Can't look in an interface which was not yet defined
  if (!i->is_defined ()) 
    {
      idl_global->err ()->fwd_decl_lookup (i, 
                                           e);
      return NULL;
    }

  //OK, loop through inherited interfaces.

  // (Don't leave the inheritance hierarchy, no module or global ...)
  // Find all and report ambiguous results as error.

  for (nis = i->n_inherits (), is = i->inherits (); nis > 0; nis--, is++) 
    {
      d = (*is)->lookup_by_name (e, 
                                 treat_as_ref, 
                                 0 /* not in_parent */);
      if (d != NULL) 
        {
	        if (d_before == NULL) 
            {   // first result found
	            d_before = d;
	          }
        	else 
            {			  // conflict against further results ?
	            if (d != d_before)  
                {
		              cerr << "warning in " << idl_global->filename ()->get_string ()
		                   << " line " << idl_global->lineno () << ": " ;
		              e->dump (cerr);
		              cerr << " is ambiguous in scope.\nFound ";
	              	d->name ()->dump (cerr);
		              cerr << " and ";
		              d_before->name ()->dump (cerr);
		              cerr << ".\n";
	              }
	          }
        }
    }

  return d_before;
}

// Look up a String * in local scope only
AST_Decl *
UTL_Scope::lookup_by_name_local (Identifier *e, 
                                 idl_bool,
                                 long index,
                                 long scope_offset)
{
  UTL_ScopeActiveIterator *i = 
    new UTL_ScopeActiveIterator (this,
                                 UTL_Scope::IK_both);
  AST_Decl *d;
  AST_InterfaceFwd *fwd;

  // Iterate over this scope
  while (!(i->is_done ())) 
    {
      d = i->item ();

      Identifier *item_name = d->local_name ();

      if (item_name == NULL)
        {
          i->next ();
          continue;
        }

      long equal = 0;

      if (scope_offset == 0)
        {
          equal = e->case_compare (item_name);
        }
      else
        {
          // If d is an argument in some other scope,
          // whether or not it matches e is irrelevant,
          // and can only cause problems if it does.
          if (d->node_type () != AST_Decl::NT_argument)
            {
              equal = e->compare (item_name);
            }
        }

      if (equal) 
        {
          if (index == 0) 
            {
              delete i;

              // Special case for forward declared interfaces. 
              // Look through the forward declaration and retrieve 
              // the full definition.
              if (d->node_type () == AST_Decl::NT_interface_fwd) 
                {
                  fwd = AST_InterfaceFwd::narrow_from_decl (d);

                  if (fwd == NULL)
                    {
                      d = NULL;
                    }
                  else
                    {
                      d = fwd->full_definition ();
                    }
                }

              return d;
            }
          else 
            {
              index--;
            }
        }
      i->next ();
    }
  delete i;

  // OK, not found, return NULL
  return NULL;
}

/*
 * Implements lookup by name for scoped names
 */
AST_Decl *
UTL_Scope::lookup_by_name (UTL_ScopedName *e, 
                           idl_bool treat_as_ref,
                           idl_bool in_parent,
                           long start_index,
                           long scope_offset)
{
  AST_Decl *d;
  this->matched = I_FALSE;
  UTL_Scope *t = NULL;

  // Empty name? error
  if (e == NULL) 
    {
      return NULL;
    }

  // If name starts with "::" or "" start lookup in global scope
  if (is_global_name (e->head ())) 
    {
     // Get parent scope
      d = ScopeAsDecl (this);

      if (d == NULL)
        {
          return NULL;
        }

      t = d->defined_in();

      // If this is the global scope..
      if (t == NULL) 
        {
          // Look up tail of name starting here
          d = lookup_by_name ((UTL_ScopedName *) e->tail (), 
                              treat_as_ref,
                              in_parent,
                              start_index,
                              scope_offset);

          // Now return whatever we have
          return d;
        }

      // OK, not global scope yet, so simply iterate with parent scope
      d = t->lookup_by_name (e, 
                             treat_as_ref,
                             in_parent,
                             start_index,
                             scope_offset + 1);

      // If treat_as_ref is true and d is not NULL, add d to
      // set of nodes referenced here
      if (treat_as_ref && d != NULL)
        {
          add_to_referenced (d, 
                             I_FALSE,
                             0);
        }

      // Now return what we have
      return d;
    }

  // The name does not start with "::"
  // Is name defined here?
  long index = start_index;

  while (1) 
    {
      d = lookup_by_name_local (e->head (), 
                                treat_as_ref, 
                                index, 
                                scope_offset);

      // If we have popped up to a parent scope, we
      // must check the other children, if we haven't 
      // had any luck so far.
      if (d == NULL && scope_offset > 0)
        {
          UTL_ScopeActiveIterator *iter = 
            new UTL_ScopeActiveIterator (this,
                                         UTL_Scope::IK_both);

          while (!iter->is_done ()) 
            {
              d = iter->item ();
              UTL_Scope *t = DeclAsScope (d);
              if (t == NULL)
                {
                  d = NULL;
                }
              else
                {
                  AST_Interface *i = 
                    AST_Interface::narrow_from_scope (t);
                  if (i == NULL || i->is_defined ())
                    {
                      d = t->lookup_by_name (e,
                                             treat_as_ref,
                                             0,
                                             0,
                                             scope_offset - 1);
                    }
                  else
                    {
                      d = NULL;
                    }

                  if (d != NULL)
                    {
                      this->matched = t->matched;

                      if (this->matched == I_TRUE)
                        {
                          return d;
                        }
                      else
                        {
                          break;
                        }
                    }
                }

              iter->next ();
            }

          delete iter;
        }

      if (d == NULL) 
        {

          // Special case for scope which is an interface. We have to look
          // in the inherited interfaces as well..
	        // Look before parent scopes !
          if (pd_scope_node_type == AST_Decl::NT_interface)
            {
              d = look_in_inherited (e, 
                                     treat_as_ref);
            }

          if ((d == NULL) && in_parent) 
            {

              // OK, not found. Go down parent scope chain.
              d = ScopeAsDecl (this);

              if (d != NULL) 
                {
                  t = d->defined_in();

                  if (t == NULL)
                    {
                      d = NULL;
                    }
                  else
                    {
                      d = t->lookup_by_name (e, 
                                             treat_as_ref, 
                                             in_parent, 
                                             0, 
                                             scope_offset + 1);
                    }
                }
            }

          // If treat_as_ref is true and d is not NULL, add d to
          // set of nodes referenced here
          if (treat_as_ref && d != NULL)
            {
              Identifier *id = 0;
              AST_Decl::NodeType nt = d->node_type ();

              if (nt == AST_Decl::NT_typedef)
                {
                  AST_Typedef *td = AST_Typedef::narrow_from_decl (d);
                  nt = td->base_type ()->node_type ();
                }

              if (d->imported () == I_FALSE
                  && nt != AST_Decl::NT_except)
                {
                  id = e->head ();
                }

              // All the stuff below needs to be done while we are
              // in the scope we started with.  
              if (scope_offset == 0)
                {
                  // If we have stripped e to the last component, then
                  // add it - the last component of a scoped name is
                  // the only one that counts as a reference in a scope.
                  if (id == e->last_component ())
                    {
                      add_to_referenced (d, 
                                         I_FALSE,
                                         id);
                    }

                  AST_Type *t = AST_Type::narrow_from_decl (d);

                  // Are we a type, rather than an identifier?
                  if (t != NULL)
                    {
                      UTL_Scope *s = ScopeAsDecl (this)->defined_in ();

                      if (s != NULL)
                        {
                          AST_Decl *parent = ScopeAsDecl (s);

                          // If the scope we are defined in is itself inside
                          // an interface or valuetype, then we should also
                          // be exported to the interface (or valuetype) scope.
                          if (parent->node_type () == AST_Decl::NT_interface)
                            {
                              s->add_to_referenced (d,
                                                    I_FALSE,
                                                    d->local_name ());
                            }
                        }
                    }
                }
            }

          // OK, now return whatever we found
          return d;
        }

      // OK, start of name is defined. Now loop doing local lookups
      // of subsequent elements of the name.
      if (this->matched == I_FALSE)
        {
          d = iter_lookup_by_name_local (d, 
                                         e, 
                                         treat_as_ref, 
                                         scope_offset,
                                         this);
        }

      // If treat_as_ref is true and d is not NULL, add d to
      // set of nodes referenced here
      if (treat_as_ref && d != NULL)
        {
          add_to_referenced (d, 
                             I_FALSE,
                             0);
        }

      // All OK, name fully resolved
      if ( d != NULL ) 
        {
          return d;
        }

      else index++ ;
    }
}

// Add a node to set of nodes referenced in this scope
void
UTL_Scope::add_to_referenced (AST_Decl *e, 
                              idl_bool recursive,
                              Identifier *id,
                              AST_Decl *ex)
{
  UTL_Scope     *s;
  AST_Decl      **tmp;
  AST_Interface *itf;
  long          oreferenced_allocated;
  long          i;

  if (e == NULL) return;

  // Special case for forward declared interfaces in the
  // scope in which they're defined. Cannot add before full
  // definition is seen
  if (e->node_type() == AST_Decl::NT_interface) 
    {
      itf = AST_Interface::narrow_from_decl(e);

      if (itf != NULL && itf->defined_in() == this && !itf->is_defined())
        {
          return;
        }
    }
  // Only insert if it is not there already
  if (referenced (e, id))
    {
      return;
    }

  // Make sure there's space for one more decl
  if (pd_referenced_allocated == pd_referenced_used) 
    {
      oreferenced_allocated = pd_referenced_allocated;
      pd_referenced_allocated += INCREMENT;
      tmp = new AST_Decl *[pd_referenced_allocated];

      for (i = 0; i < oreferenced_allocated; i++)
        {
          tmp[i] = pd_referenced[i];
        }

      delete [] pd_referenced;

      pd_referenced = tmp;
    }

  // Insert new decl
  pd_referenced[pd_referenced_used++] = e;

  // Insert new reference
  if (ex == 0)
    pd_referenced[pd_referenced_used++] = e;
  else if (referenced (ex))
    {
      for (i = pd_referenced_used; i > 0; i--)
        {
          pd_referenced[i] = pd_referenced[i-1];
          if (pd_referenced[i-1] == ex)
            {
              pd_referenced[i] = e;
              break;
            }
        }
      ++pd_referenced_used;
    }

  // Now, if recursive is specified and "this" is not a common ancestor
  // of the referencing scope and the scope of definition of "e" then
  // add "e" to the set of referenced nodes in the parent of "this"
  if (recursive && !(e->has_ancestor (ScopeAsDecl (this)))) 
    {
      s = e->defined_in ();

      if (s != NULL)
        {
          s->add_to_referenced (e, 
                                recursive, 
                                id);
        }
    }

  // Add the identifier arg, if non-null, to the identifier list.
  if (id)
    {
      if (pd_name_referenced_allocated == pd_name_referenced_used)
        {
          long name_referenced_allocated = pd_name_referenced_allocated;
          pd_name_referenced_allocated += INCREMENT;
          Identifier **name_tmp = 
            new Identifier *[pd_name_referenced_allocated];

          for (i = 0; i < name_referenced_allocated; i++)
            {
              name_tmp[i] = pd_name_referenced[i];
            }

          delete [] pd_name_referenced;

          pd_name_referenced = name_tmp;
        }

      // Insert new identifier
      pd_name_referenced[pd_name_referenced_used++] = id;
    }
}

void 
UTL_Scope::replace_referenced (AST_Decl *old_decl,
                               AST_Decl *new_decl)
{
  int i;
  for (i = 0; i < pd_referenced_used; i++)
    if (pd_referenced[i] == old_decl)
      {
        pd_referenced[i] = new_decl;
        break;
      }
  
}


void 
UTL_Scope::replace_scope (AST_Decl *old_decl,
                          AST_Decl *new_decl)
{
  int i;
  for (i = 0; i < pd_decls_used; i++)
    if (pd_decls[i] == old_decl)
      {
        pd_decls[i] = new_decl;
        break;
      }
  
}


// Add a node to set of nodes declared in this scope
void
UTL_Scope::add_to_scope(AST_Decl *e, AST_Decl *ex)
{
  if (e == NULL) return;

  AST_Decl **tmp = this->pd_referenced;
  UTL_IdListActiveIterator *iter;
  long i = this->pd_referenced_used;
  long odecls_allocated;
  Identifier *decl_name = e->local_name ();

  // First, make sure there's no clash between e, that was
  // just declared, and some other identifier referenced
  // in this scope.
  for (; i > 0; i--, tmp++)
    {
      UTL_ScopedName *s = (*tmp)->name ();
      iter = new UTL_IdListActiveIterator (s);
      Identifier *ref_name = iter->item ();
      char *ref_string = ref_name->get_string ();

      // Get the top level compenent of the scoped
      // name. That's the only one that matters for
      // collision comparisons.
      while (!ACE_OS::strcmp (ref_string, "")
             || !ACE_OS::strcmp (ref_string, "::"))
        {
          iter->next ();
          ref_name = iter->item ();
          ref_string = ref_name->get_string ();
        }

      delete iter;

      // If the names compare exactly, it's a redefinition 
      // error, unless they're both modules (which can be
      // reopened) or we have a belated definition of a 
      // forward-declared interface.
      if (decl_name->compare (ref_name) == I_TRUE
          && e->node_type () != AST_Decl::NT_module
          && ((*tmp)->node_type () != AST_Decl::NT_interface_fwd
              || e->node_type () != AST_Decl::NT_interface))
        {
          idl_global->err ()->redef_error (decl_name->get_string (),
                                           ref_string);

          break;
        }

      // If the spellings differ only by case, it's also an error
      // (case_compare outputs its own error message).
      if (decl_name->case_compare_quiet (ref_name) == I_TRUE)
        {
          idl_global->err ()->name_case_error (decl_name->get_string (),
                                               ref_string);

          break;
        }
    }

  // Now make sure there's space for one more.
  if (pd_decls_allocated == pd_decls_used) 
    {

      odecls_allocated    = pd_decls_allocated;
      pd_decls_allocated  += INCREMENT;
      tmp                 = new AST_Decl *[pd_decls_allocated];

      for (i = 0; i < odecls_allocated; i++)
        {
          tmp[i] = this->pd_decls[i];
        }

      delete [] this->pd_decls;

      this->pd_decls = tmp;
    }


  // Insert new decl
  if (ex == 0)
    pd_decls[pd_decls_used++] = e;
  else
    {
      for (i = pd_decls_used; i > 0; i--)
        {
          pd_decls[i] = pd_decls[i-1];
          if (pd_decls[i-1] == ex)
            {
              pd_decls[i] = e;
              break;
            }
        }
      ++pd_decls_used;
    }
}

// Add a node to set of nodes representing manifest types defined in this scope
void
UTL_Scope::add_to_local_types(AST_Decl *e)
{
  AST_Decl      **tmp;
  long          olocals_allocated;
  long          i;

  if (e == NULL) return;

  // Make sure there's space for one more
  if (pd_locals_allocated == pd_locals_used) {

    olocals_allocated   = pd_locals_allocated;
    pd_locals_allocated         += INCREMENT;
    tmp                 = new AST_Decl *[pd_locals_allocated];

    for (i = 0; i < olocals_allocated; i++)
      tmp[i] = pd_local_types[i];

    delete []pd_local_types;

    pd_local_types = tmp;
  }

  // Insert new decl
  pd_local_types[pd_locals_used++] = e;
}

// Has this node been referenced here before?
idl_bool
UTL_Scope::referenced (AST_Decl *e, 
                       Identifier *id)
{
  long i = pd_referenced_used;
  AST_Decl **tmp = pd_referenced;
  Identifier *member = 0;
  Identifier *test = 0;

  for (; i > 0; i--, tmp++)
    {
      if (*tmp == e)      // Same node?
        return I_TRUE;
      if ((*tmp)->node_type () == AST_Decl::NT_interface_fwd
          && e->node_type () == AST_Decl::NT_interface)
        {
          member = (*tmp)->local_name ();
          test = e->local_name ();
          // If we're just defining a forward
          // declared interface, no need to go any further.
          if (member->compare (test) == I_TRUE)
            return I_FALSE;
        }
    }

  // pd_referenced is a list of decls, and so there's no
  // way of telling how much of its scoped name was used
  // when it was referenced in this scope. pd_name_referenced
  // is a list of Identifiers that store the identifier (or
  // the first segment of a scoped name) used in the reference,
  // so we can catch these name reolution clashes.
  if (id)
    {
      long j = pd_name_referenced_used;
      Identifier **name_tmp = pd_name_referenced;

      for (; j > 0; j--, name_tmp++)
        {
          // If we are a module, there is no clash, if we
          // are an interface, this is not the right place to
          // catch a clash, and if it wasnt' defined in this
          // scope, then it's a type name for something else
          // that was, and it can appear any number of times
          // in this scope without a clash.
          if (id->compare (*name_tmp) == I_TRUE
              && e->node_type () != AST_Decl::NT_module
              && e->defined_in () == this)
            {
              idl_global->err ()->redef_error (id->get_string (),
                                               (*name_tmp)->get_string ());

              return I_TRUE;
            }
          else if (id->case_compare_quiet (*name_tmp) == I_TRUE)
            {
              if (idl_global->case_diff_error ())
                {
                  idl_global->err ()->name_case_error (
                                          id->get_string (),
                                          (*name_tmp)->get_string ()
                                        );
                }
              else
                {
                  idl_global->err ()->name_case_warning (
                                          id->get_string (),
                                          (*name_tmp)->get_string ()
                                        );
                }

              return I_TRUE;
            }
        }
    }

  return I_FALSE;               // Not found
}

/*
 * Redefinition of inherited virtual operations
 */

// AST Dumping
void
UTL_Scope::dump (ostream &o)
{
  UTL_ScopeActiveIterator       *i;
  AST_Decl                      *d;

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
                          stage(i == UTL_Scope::IK_both
                                ? UTL_Scope::IK_decls
                                : i),
                          il(0)
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
AST_Decl        *
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
  long  limit;

  limit = (stage == UTL_Scope::IK_decls)
    ? iter_source->pd_decls_used
    : iter_source->pd_locals_used;

  for (;;) {
    if (il < limit)                             // Last element?
      return I_FALSE;
    if (stage == UTL_Scope::IK_localtypes)      // Already done local types?
      return I_TRUE;
    if (ik == UTL_Scope::IK_decls)              // Only want decls?
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
