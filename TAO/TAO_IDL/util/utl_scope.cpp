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

#include "utl_scope.h"
#include "utl_identifier.h"
#include "utl_err.h"
#include "utl_indenter.h"
#include "utl_string.h"
#include "ast_interface.h"
#include "ast_interface_fwd.h"
#include "ast_typedef.h"
#include "ast_type.h"
#include "ast_root.h"
#include "ast_array.h"
#include "ast_enum.h"
#include "ast_concrete_type.h"
#include "ast_sequence.h"
#include "ast_string.h"
#include "ast_structure_fwd.h"
#include "ast_exception.h"
#include "ast_constant.h"
#include "ast_attribute.h"
#include "ast_operation.h"
#include "ast_argument.h"
#include "ast_union.h"
#include "ast_union_fwd.h"
#include "ast_union_branch.h"
#include "ast_field.h"
#include "ast_enum_val.h"
#include "ast_native.h"
#include "ast_factory.h"
#include "ast_visitor.h"
#include "nr_extern.h"
#include "global_extern.h"

ACE_RCSID (util, 
           utl_scope, 
           "$Id$")

#undef  INCREMENT
#define INCREMENT 64

// Static variables.
static Identifier *_global_scope_name = 0;
static Identifier *_global_scope_root_name = 0;

// Static functions.

// Determines if a name is global.
static long
is_global_name (Identifier *i)
{
  long comp_result = 0;

  if (i == 0)
    {
      return comp_result;
    }

  if (_global_scope_name == 0)
    {
      ACE_NEW_RETURN (_global_scope_name,
                      Identifier ("::"),
                      0);
    }

  if (_global_scope_root_name == 0)
    {
      ACE_NEW_RETURN (_global_scope_root_name,
                      Identifier (""),
                      0);
    }

  comp_result = i->compare (_global_scope_name);

  if (comp_result == 0)
    {
      comp_result = i->compare (_global_scope_root_name);
    }

  return comp_result;
}

// Helper function for lookup_by_name. Iterates doing local lookups of
// subsequent components of a scoped name.
static AST_Decl *
iter_lookup_by_name_local (AST_Decl *d,
                           UTL_ScopedName *e,
                           long index)
{
  AST_Typedef *td = AST_Typedef::narrow_from_decl (d);
  AST_Decl *result = 0;

  // Remove all the layers of typedefs.
  while (d != NULL && d->node_type () == AST_Decl::NT_typedef)
    {
      if (td == 0)
        {
          return 0;
        }

      d = td->base_type ();
    }

  if (d == 0)
    {
      return 0;
    }

  // Try to convert the AST_Decl to a UTL_Scope.
  UTL_Scope *sc = DeclAsScope (d);

  if (sc == 0)
    {
      return 0;
    }

  if (index < ACE_static_cast (long, sc->nmembers ()))
    {
      // Look up the first component of the scoped name.
      result = sc->lookup_by_name_local (e->head (),
                                         index);
    }
  else
    {
      return 0;
    }


  if (result == 0)
    {
      return 0;
    }
  else
    {
      UTL_ScopedName *sn = (UTL_ScopedName *) e->tail ();

      if (sn == 0)
        {
          // We're done.
          return result;
        }
      else
        {
          // Look up the next component of the scoped name.
          result = iter_lookup_by_name_local (result,
                                              sn,
                                              0);
        }

      if (result != 0)
        {
          // We're done.
          return result;
        }
      else
        {
          // Maybe we're on the wrong branch of reopened
          // and/or nested modules, so let's see if there's
          // another branch. If 'index' gets as high as the
          // number of members in the scope, the call above
          // to lookup_by_name_local will catch it and return 0.
          return iter_lookup_by_name_local (d,
                                            e,
                                            index + 1);
        }
    }
}

//  Constructors.

UTL_Scope::UTL_Scope (void)
  : pd_scope_node_type (AST_Decl::NT_module),
    pd_decls (0),
    pd_decls_allocated (0),
    pd_decls_used (0),
    pd_local_types (0),
    pd_locals_allocated (0),
    pd_locals_used (0),
    pd_referenced (0),
    pd_referenced_allocated (0),
    pd_referenced_used (0),
    pd_name_referenced (0),
    pd_name_referenced_allocated (0),
    pd_name_referenced_used (0),
    has_prefix_ (0)
{
}

UTL_Scope::UTL_Scope (AST_Decl::NodeType nt)
  : pd_scope_node_type (nt),
    pd_decls (0),
    pd_decls_allocated (0),
    pd_decls_used (0),
    pd_local_types (0),
    pd_locals_allocated (0),
    pd_locals_used (0),
    pd_referenced (0),
    pd_referenced_allocated (0),
    pd_referenced_used (0),
    pd_name_referenced (0),
    pd_name_referenced_allocated (0),
    pd_name_referenced_used (0),
    has_prefix_ (0)
{
}

// Destructor.
UTL_Scope::~UTL_Scope (void)
{
}

// Private operations.

static AST_Decl *
add_type (AST_Type *type)
{
  AST_Decl *result = 0;
  UTL_Scope *scope = 0;

  switch (type->node_type())
    {
    case AST_Decl::NT_array:
      result =
        idl_global->root ()->add_array (AST_Array::narrow_from_decl (type));
      break;
    case AST_Decl::NT_enum:
      result =
        type->defined_in ()->add_enum (AST_Enum::narrow_from_decl (type));
      scope = AST_Enum::narrow_from_decl (type);
      break;
    case AST_Decl::NT_sequence:
      result =
        idl_global->root ()->add_sequence (
                                 AST_Sequence::narrow_from_decl (type)
                               );
      break;
    case AST_Decl::NT_string:
    case AST_Decl::NT_wstring:
      result =
        idl_global->root ()->add_string (AST_String::narrow_from_decl (type));
      break;
    case AST_Decl::NT_struct:
      result =
        type->defined_in ()->add_structure (
                                 AST_Structure::narrow_from_decl (type)
                               );
      scope = AST_Structure::narrow_from_decl (type);
      break;
    case AST_Decl::NT_union:
      result =
        type->defined_in ()->add_union (AST_Union::narrow_from_decl (type));
      scope = AST_Union::narrow_from_decl (type);
      break;
    default:
      // For non-complex types, like predefined types
      // no additional add needed, assume everything is ok.
      result = (AST_Decl *) 1;
      break;
    }

  if (scope != 0)
    {
      result = scope->call_add ();
    }

  return result;
}

// Protected operations.

// Special version of lookup which only looks at the local name instead of
// the fully scoped name, when doing lookups. This version is intended to
// be used only by the CFE add_xxx functions.
AST_Decl *
UTL_Scope::lookup_for_add (AST_Decl *d,
                           idl_bool)
{
  if (d == 0)
    {
      return 0;
    }

  Identifier *id = d->local_name ();

  if (this->idl_keyword_clash (id) != 0)
    {
      return 0;
    }

  return lookup_by_name_local (id,
                               0);
}

int
UTL_Scope::idl_keyword_clash (Identifier *e)
{
  if (e->escaped ())
    {
      return 0;
    }

  char *tmp = e->get_string ();

  UTL_String utl_tmp (tmp);

  ACE_CString ext_id (utl_tmp.get_canonical_rep (),
                      0,
                      0);

  int status = idl_global->idl_keywords ().find (ext_id);

  utl_tmp.destroy ();

  if (status == 0)
    {
      if (idl_global->case_diff_error ())
        {
          idl_global->err ()->idl_keyword_error (tmp);
        }
      else
        {
          idl_global->err ()->idl_keyword_warning (tmp);
        }

      return -1;
    }

  return 0;
}

idl_bool
UTL_Scope::redef_clash (AST_Decl::NodeType new_nt,
                        AST_Decl::NodeType scope_elem_nt)
{
  switch (new_nt)
  {
    case AST_Decl::NT_module:
      return scope_elem_nt != AST_Decl::NT_module;
    case AST_Decl::NT_struct:
    case AST_Decl::NT_struct_fwd:
      return scope_elem_nt != AST_Decl::NT_struct_fwd;
    case AST_Decl::NT_union:
    case AST_Decl::NT_union_fwd:
      return scope_elem_nt != AST_Decl::NT_union_fwd;
    case AST_Decl::NT_interface:
      return scope_elem_nt != AST_Decl::NT_interface_fwd;
    case AST_Decl::NT_interface_fwd:
      return (scope_elem_nt != AST_Decl::NT_interface_fwd
              && scope_elem_nt != AST_Decl::NT_interface);
    default:
      return I_TRUE;
  }
}

// Public operations.

// Scope Management Protocol.
//
// All members of the protocol defined in UTL_Scope simply return the node
// and don't do a thing. These members are simply dummies to retain
// compatibility with pre-two-pass compiler back-ends.

AST_PredefinedType *
UTL_Scope::add_predefined_type (AST_PredefinedType *p)
{
  if (p == 0)
    {
      return 0;
    }

  p->set_added (I_TRUE);
  return p;
}

AST_Module *
UTL_Scope::add_module (AST_Module *m)
{
  if (m == 0)
    {
      return 0;
    }

  m->set_added (I_TRUE);
  return m;
}

AST_Interface *
UTL_Scope::add_interface (AST_Interface *i)
{
  if (i == 0)
    {
      return 0;
    }

  i->set_added (I_TRUE);
  return i;
}

AST_InterfaceFwd *
UTL_Scope::add_interface_fwd (AST_InterfaceFwd *i)
{
  if (i == 0)
    {
      return 0;
    }

  i->set_added (I_TRUE);
  return i;
}

AST_Exception *
UTL_Scope::add_exception (AST_Exception *e)
{
  if (e == 0)
    {
      return 0;
    }

  e->set_added (I_TRUE);
  return e;
}

AST_Constant *
UTL_Scope::add_constant (AST_Constant *c)
{
  if (c == 0)
    {
      return 0;
    }

  c->set_added (I_TRUE);
  return c;
}

UTL_StrList *
UTL_Scope::add_context (UTL_StrList *c)
{
  return c;
}

UTL_NameList *
UTL_Scope::add_exceptions (UTL_NameList *e)
{
  return e;
}

AST_Attribute *
UTL_Scope::add_attribute (AST_Attribute *a)
{
  if (a == 0)
    {
      return 0;
    }

  a->set_added (I_TRUE);

  if (!a->field_type ()->added ())
    {
      return add_type (a->field_type ()) ? a : 0;
    }
  else
    {
      return a;
    }
}

AST_Operation *
UTL_Scope::add_operation (AST_Operation *o)
{
  if (o == 0)
    {
      return 0;
    }

  o->set_added (I_TRUE);

  if (!o->return_type ()->added ())
    {
      return add_type (o->return_type ()) ? o : 0;
    }
  else
    {
      return o;
    }
}

AST_Argument *
UTL_Scope::add_argument (AST_Argument *a)
{
  if (a == 0)
    {
      return 0;
    }

  a->set_added (I_TRUE);

  if (!a->field_type ()->added ())
    {
      return add_type (a->field_type ()) ? a : 0;
    }
  else
    {
      return a;
    }
}

AST_Union *
UTL_Scope::add_union (AST_Union *u)
{
  if (u == 0)
    {
      return 0;
    }

  u->set_added (I_TRUE);
  return u;
}

AST_UnionFwd *
UTL_Scope::add_union_fwd (AST_UnionFwd *u)
{
  if (u == 0)
    {
      return 0;
    }

  u->set_added (I_TRUE);
  return u;
}

AST_UnionBranch *
UTL_Scope::add_union_branch (AST_UnionBranch *u)
{
  if (u == 0)
    {
      return 0;
    }

  u->set_added (I_TRUE);

  if (!u->field_type ()->added ())
    {
      return add_type (u->field_type ()) ? u : 0;
    }
  else
    {
      return u;
    }
}

AST_Structure *
UTL_Scope::add_structure (AST_Structure *s)
{
  if (s == 0)
    {
      return 0;
    }

  s->set_added (I_TRUE);
  return s;
}

AST_StructureFwd *
UTL_Scope::add_structure_fwd (AST_StructureFwd *s)
{
  if (s == 0)
    {
      return 0;
    }

  s->set_added (I_TRUE);
  return s;
}

AST_Field *
UTL_Scope::add_field (AST_Field *f)
{
  if (f == 0)
    {
      return 0;
    }

  f->set_added (I_TRUE);

  if (!f->field_type ()->added ())
    {
      return add_type (f->field_type ()) ? f : 0;
    }
  else
    {
      return f;
    }
}

AST_Enum *
UTL_Scope::add_enum (AST_Enum *e)
{
  if (e == 0)
    {
      return 0;
    }

  e->set_added (I_TRUE);
  return e;
}

AST_EnumVal *
UTL_Scope::add_enum_val (AST_EnumVal *e)
{
  if (e == 0)
    {
      return 0;
    }

  e->set_added (I_TRUE);
  return e;
}

AST_Typedef *
UTL_Scope::add_typedef (AST_Typedef *t)
{
  if (t == 0)
    {
      return 0;
    }

  t->set_added (I_TRUE);
  if (!t->base_type ()->added ())
    {
      return add_type (t->base_type ()) ? t : 0;
    }
  else
    {
      return t;
    }
}

AST_Sequence *
UTL_Scope::add_sequence (AST_Sequence *s)
{
  if (s == 0)
    {
      return 0;
    }

  s->set_added (I_TRUE);

  if (!s->base_type ()->added ())
    {
      return add_type (s->base_type ()) ? s : 0;
    }
  else
    {
      return s;
    }
}

AST_String *
UTL_Scope::add_string (AST_String *s)
{
  if (s == 0)
    {
      return 0;
    }

  s->set_added (I_TRUE);
  return s;
}

AST_Array *
UTL_Scope::add_array (AST_Array *a)
{
  if (a == 0)
    {
      return 0;
    }

  a->set_added (I_TRUE);

  if (!a->base_type ()->added ())
    {
      return add_type (a->base_type ()) ? a : 0;
    }
  else
    {
      return a;
    }
}

AST_Native *
UTL_Scope::add_native (AST_Native *n)
{
  if (n == 0)
    {
      return 0;
    }

  n->set_added (I_TRUE);
  return n;
}

AST_Factory *
UTL_Scope::add_factory (AST_Factory *f)
{
  //We don't invite any new types so there is nothing actually to add
  if (f == 0)
    {
      return 0;
    }

  f->set_added (I_TRUE);
  return f;
}

// Protected Front End Scope Management Protocol.
//
// All members of the protocol defined in UTL_Scope simply return NULL
// and don't do a thing. This ensures that runtime errors will discover
// operations which should have been redefined to allow certain kinds of
// AST nodes to appear in a given context.

AST_PredefinedType *
UTL_Scope::fe_add_predefined_type (AST_PredefinedType *)
{
  return 0;
}

AST_Module *
UTL_Scope::fe_add_module (AST_Module *)
{
  return NULL;
}

AST_Interface *
UTL_Scope::fe_add_interface (AST_Interface *)
{
  return 0;
}

AST_InterfaceFwd *
UTL_Scope::fe_add_interface_fwd (AST_InterfaceFwd *)
{
  return 0;
}

AST_Exception *
UTL_Scope::fe_add_exception (AST_Exception *)
{
  return 0;
}

AST_Constant *
UTL_Scope::fe_add_constant (AST_Constant *)
{
  return 0;
}

UTL_StrList *
UTL_Scope::fe_add_context (UTL_StrList *)
{
  return 0;
}

UTL_NameList *
UTL_Scope::fe_add_exceptions (UTL_NameList *)
{
  return 0;
}

AST_Attribute *
UTL_Scope::fe_add_attribute (AST_Attribute *)
{
  return 0;
}

AST_Operation *
UTL_Scope::fe_add_operation (AST_Operation *)
{
  return 0;
}

AST_Argument *
UTL_Scope::fe_add_argument (AST_Argument *)
{
  return 0;
}

AST_Union *
UTL_Scope::fe_add_union (AST_Union *)
{
  return 0;
}

AST_UnionFwd *
UTL_Scope::fe_add_union_fwd (AST_UnionFwd *)
{
  return 0;
}

AST_UnionBranch *
UTL_Scope::fe_add_union_branch (AST_UnionBranch *)
{
  return 0;
}

AST_Structure *
UTL_Scope::fe_add_structure (AST_Structure *)
{
  return 0;
}

AST_StructureFwd *
UTL_Scope::fe_add_structure_fwd (AST_StructureFwd *)
{
  return 0;
}

AST_Field *
UTL_Scope::fe_add_field (AST_Field *)
{
  return 0;
}

AST_Enum *
UTL_Scope::fe_add_enum (AST_Enum *)
{
  return 0;
}

AST_EnumVal *
UTL_Scope::fe_add_enum_val (AST_EnumVal *)
{
  return 0;
}

AST_Typedef *
UTL_Scope::fe_add_typedef (AST_Typedef *)
{
  return 0;
}

AST_Sequence *
UTL_Scope::fe_add_sequence (AST_Sequence *)
{
  return 0;
}

AST_String *
UTL_Scope::fe_add_string (AST_String *)
{
  return 0;
}

AST_Array *
UTL_Scope::fe_add_array (AST_Array *)
{
  return 0;
}

AST_Native *
UTL_Scope::fe_add_native (AST_Native *)
{
  return 0;
}

AST_Factory *
UTL_Scope::fe_add_factory (AST_Factory *)
{
  return 0;
}

// This is the second pass of the front end
// It calls the public add protocol on everything in scope.
// It calls the add_xx functions of the most derived AST_Node.

AST_Decl *
UTL_Scope::call_add (void)
{
  AST_Decl *result = 0;
  AST_Decl *decl = 0;

  UTL_ScopeActiveIterator i (this,
                             UTL_Scope::IK_decls);
  UTL_Scope *scope = 0;

  while (!i.is_done ())
    {
      decl = i.item ();
      scope = 0;

      switch (decl->node_type ())
        {
        case AST_Decl::NT_argument:
          result = add_argument (AST_Argument::narrow_from_decl (decl));
          break;
        case AST_Decl::NT_array:
          result = add_array (AST_Array::narrow_from_decl (decl));
          break;
        case AST_Decl::NT_attr:
          result = add_attribute (AST_Attribute::narrow_from_decl (decl));
          break;
        case AST_Decl::NT_const:
          result = add_constant (AST_Constant::narrow_from_decl (decl));
          break;
        case AST_Decl::NT_enum:
          scope = AST_Enum::narrow_from_decl (decl);
          result = add_enum (AST_Enum::narrow_from_decl (decl));
          break;
        case AST_Decl::NT_enum_val:
          result = add_enum_val (AST_EnumVal::narrow_from_decl (decl));
          break;
        case AST_Decl::NT_except:
          scope = AST_Exception::narrow_from_decl (decl);
          result = add_exception (AST_Exception::narrow_from_decl (decl));
          break;
        case AST_Decl::NT_field:
          result = add_field (AST_Field::narrow_from_decl (decl));
          break;
        case AST_Decl::NT_interface:
          scope = AST_Interface::narrow_from_decl (decl);
          result = add_interface (AST_Interface::narrow_from_decl (decl));
          break;
        case AST_Decl::NT_interface_fwd:
          result =
            add_interface_fwd (AST_InterfaceFwd::narrow_from_decl (decl));
          break;
        case AST_Decl::NT_module:
          scope = AST_Module::narrow_from_decl (decl);
          result = add_module (AST_Module::narrow_from_decl (decl));
          break;
        case AST_Decl::NT_native:
          result = add_native (AST_Native::narrow_from_decl (decl));
          break;
        case AST_Decl::NT_op:
          result = add_operation (AST_Operation::narrow_from_decl (decl));
          scope = AST_Operation::narrow_from_decl (decl);
          break;
        case AST_Decl::NT_pre_defined:
          result =
            add_predefined_type (AST_PredefinedType::narrow_from_decl (decl));
          break;
        case AST_Decl::NT_sequence:
          result = add_sequence (AST_Sequence::narrow_from_decl (decl));
          break;
        case AST_Decl::NT_string:
        case AST_Decl::NT_wstring:
          result = add_string (AST_String::narrow_from_decl (decl));
          break;
        case AST_Decl::NT_struct:
          result = add_structure (AST_Structure::narrow_from_decl (decl));
          scope = AST_Structure::narrow_from_decl (decl);
          break;
        case AST_Decl::NT_typedef:
          result = add_typedef (AST_Typedef::narrow_from_decl (decl));
          break;
        case AST_Decl::NT_union:
          result = add_union (AST_Union::narrow_from_decl (decl));
          scope = AST_Union::narrow_from_decl (decl);
          break;
        case AST_Decl::NT_union_branch:
          result =
            add_union_branch (AST_UnionBranch::narrow_from_decl (decl));
          break;
        case AST_Decl::NT_factory:
          result = add_factory (AST_Factory::narrow_from_decl (decl));
          scope = AST_Factory::narrow_from_decl (decl);
          break;
        default:
          return 0;
        }

      if (scope != 0)
        {
          scope->call_add ();
        }

      i.next ();
    }

  return result;
}

// Private lookup mechanism.

// For dealing with lookups of 'Object' and 'TypeCode'.
AST_Decl *
UTL_Scope::lookup_pseudo (Identifier *e)
{
  Identifier *item_name = 0;
  AST_Decl *d = 0;
  UTL_ScopeActiveIterator *i = 0;
  char *name_string = e->get_string ();

  if (ACE_OS::strcmp (name_string, "Object") == 0
      || ACE_OS::strcmp (name_string, "ValueBase") == 0)
    {
      // Iterate over the global scope.
      UTL_ScopeActiveIterator global_iter (idl_global->scopes ().bottom (),
                                           UTL_Scope::IK_decls);

      i = &global_iter;
    }
  else if (ACE_OS::strcmp (name_string, "TypeCode") == 0
           || ACE_OS::strcmp (name_string, "TCKind") == 0)
    {
      // Occurrences of "TypeCode" or TCKind in IDL files must be
      // scoped with "CORBA" so we know we'll be in the CORBA module
      // if we get this far, and we can use "this" for the scope of
      // the iterator.
      UTL_ScopeActiveIterator corba_iter (this,
                                          UTL_Scope::IK_decls);

      i = &corba_iter;
    }
  else
    {
      return 0;
    }

  for (;!i->is_done (); i->next ())
    {
      d = i->item ();

      item_name = d->local_name ();

      if (e->compare (item_name))
        {
          return d;
        }
    }

  return 0;
}

// Lookup the node for a primitive (built in) type.
AST_Decl *
UTL_Scope::lookup_primitive_type (AST_Expression::ExprType et)
{
  AST_Decl *as_decl = 0;
  UTL_Scope *ancestor = 0;
  AST_PredefinedType *t = 0;
  AST_PredefinedType::PredefinedType pdt;

  as_decl = ScopeAsDecl (this);

  if (as_decl == 0)
    {
      return 0;
    }
  ancestor = as_decl->defined_in ();

  if (ancestor != 0)
    {
      return ancestor->lookup_primitive_type (et);
    }

  switch (et)
    {
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
      return 0;
    }

  for (UTL_ScopeActiveIterator i (this, UTL_Scope::IK_decls);
       !i.is_done();
       i.next ())
    {
      as_decl = i.item ();

      if (as_decl->node_type () == AST_Decl::NT_pre_defined)
        {
          t = AST_PredefinedType::narrow_from_decl (as_decl);

          if (t == NULL)
            {
              continue;
            }

          if (t->pt () == pdt)
            {
              return t;
            }
        }
    }

  return 0;
}

// Look through inherited interfaces.
AST_Decl *
UTL_Scope::look_in_inherited (UTL_ScopedName *e,
                              idl_bool treat_as_ref)
{
  AST_Decl *d = 0;
  AST_Decl *d_before = 0;
  AST_Interface *i = AST_Interface::narrow_from_scope (this);
  AST_Interface **is = 0;
  long nis = -1;

  // This scope is not an interface.
  if (i == 0)
    {
      return 0;
    }

  // Can't look in an interface which was not yet defined.
  if (!i->is_defined ())
    {
      idl_global->err ()->fwd_decl_lookup (i,
                                           e);
      return 0;
    }

  // OK, loop through inherited interfaces.

  // (Don't leave the inheritance hierarchy, no module or global ...)
  // Find all and report ambiguous results as error.

  for (nis = i->n_inherits (), is = i->inherits (); nis > 0; nis--, is++)
    {
      d = (*is)->lookup_by_name (e,
                                 treat_as_ref,
                                 0 /* not in parent */);
      if (d != 0)
        {
          if (d_before == 0)
            {
              // First result found.
              d_before = d;
            }
          else
            {
              // Conflict against further results?
              if (d != d_before)
                {
                  ACE_ERROR ((LM_ERROR,
                              "warning in %s line %d: ",
                              idl_global->filename ()->get_string (),
                              idl_global->lineno ()));

                  e->dump (*ACE_DEFAULT_LOG_STREAM);

                  ACE_ERROR ((LM_ERROR,
                              " is ambiguous in scope.\n"
                              "Found "));

                  d->name ()->dump (*ACE_DEFAULT_LOG_STREAM);

                  ACE_ERROR ((LM_ERROR,
                              " and "));

                  d_before->name ()->dump (*ACE_DEFAULT_LOG_STREAM);

                  ACE_ERROR ((LM_ERROR,
                              ".\n"));
                }
            }
        }
    }

  return d_before;
}

// Look up a String * in local scope only.
AST_Decl *
UTL_Scope::lookup_by_name_local (Identifier *e,
                                 long index)
{
  if (index > 0 && index == (long) this->nmembers ())
    {
      return 0;
    }

  // Will return 0 unless looking up 'Object' or 'TypeCode'.
  AST_Decl *d = this->lookup_pseudo (e);

  if (d != 0)
    {
      return d;
    }

  if (this->idl_keyword_clash (e) != 0)
    {
      return 0;
    }

  Identifier *item_name = 0;

  idl_bool in_corba =
    ACE_OS::strcmp (e->get_string (), "CORBA") == 0;

  // Iterate over this scope.
  for (UTL_ScopeActiveIterator i (this, UTL_Scope::IK_both);
       !i.is_done ();
       i.next ())
    {
      d = i.item ();

      item_name = d->local_name ();

      if (item_name == 0)
        {
          continue;
        }

      // Right now we populate the global scope with all the CORBA basic
      // types, so something like 'ULong' in an IDL file will find a
      // match, unless we skip over these items. This is a workaround until
      // there's time to fix the code generation for CORBA basic types.
      if (!in_corba
          && ACE_OS::strcmp (d->name ()->head ()->get_string (), "CORBA") == 0)
        {
          continue;
        }

      if (e->case_compare (item_name))
        {
          if (index == 0)
            {
              AST_Decl::NodeType nt = d->node_type ();

              // Special case for forward declared interfaces,
              // In this case, we want to return
              // the full definition member, whether defined yet or not
              if (d->node_type () == AST_Decl::NT_interface_fwd)
                {
                  d = AST_InterfaceFwd::narrow_from_decl (d)->full_definition ();
                }

              return d;
            }
          else
          // If the index has been incremented, it means the identifier
          // matched on a previous call to this function, but after
          // returning, the rest of the scoped name didn't match. So we
          // see if there's another matching identifier to the 'head' of
          // the scoped name we're working with.
            {
              index--;
              continue;
            }
        }
    }

  // OK, not found, check if this scope is a module, and if so,
  // look in previous openings, if any.
  d = ScopeAsDecl (this);

  if (d->node_type () == AST_Decl::NT_module)
    {
      AST_Module *m = AST_Module::narrow_from_decl (d);

      return m->look_in_previous (e);
    }
  else
    {
      return 0;
    }
}

// Implements lookup by name for scoped names.
AST_Decl *
UTL_Scope::lookup_by_name (UTL_ScopedName *e,
                           idl_bool treat_as_ref,
                           idl_bool in_parent)
{
  AST_Decl *d = 0;
  UTL_Scope *t = 0;

  // Empty name? Error.
  if (e == 0)
    {
      return 0;
    }

  // If name starts with "::" or "" start lookup in global scope.
  if (is_global_name (e->head ()))
    {
     // Get parent scope.
      d = ScopeAsDecl (this);

      if (d == 0)
        {
          return 0;
        }

      t = d->defined_in ();

      // If this is the global scope..
      if (t == 0)
        {
          // Look up tail of name starting here.
          d = lookup_by_name ((UTL_ScopedName *) e->tail (),
                              treat_as_ref,
                              in_parent);

          // Now return whatever we have.
          return d;
        }

      // OK, not global scope yet, so simply iterate with parent scope.
      d = t->lookup_by_name (e,
                             treat_as_ref,
                             in_parent);

      // If treat_as_ref is true and d is not NULL, add d to
      // set of nodes referenced here.
      if (treat_as_ref && d != 0)
        {
          add_to_referenced (d,
                             I_FALSE,
                             0);
        }

      // Now return what we have.
      return d;
    }

  // The name does not start with "::"
  // Is name defined here?
  long index = 0;

  while (1)
    {
      d = lookup_by_name_local (e->head (),
                                index);

      if (d == 0)
        {

          // Special case for scope which is an interface. We have to look
          // in the inherited interfaces as well.
                // Look before parent scopes.
          if (pd_scope_node_type == AST_Decl::NT_interface)
            {
              d = look_in_inherited (e,
                                     treat_as_ref);
            }

          if ((d == 0) && in_parent && idl_global->err_count () == 0)
            {

              // OK, not found. Go down parent scope chain.
              d = ScopeAsDecl (this);

              if (d != 0)
                {
                  t = d->defined_in ();

                  if (t == 0)
                    {
                      d = 0;
                    }
                  else
                    {
                      d = t->lookup_by_name (e,
                                             treat_as_ref,
                                             in_parent);
                    }
                }
            }

          // If treat_as_ref is true and d is not NULL, add d to
          // set of nodes referenced here.
          if (treat_as_ref && d != 0)
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
              if (t != 0)
                {
                  // Are we defined in this scope or just referenced?
                  if (d->defined_in () == this)
                    {
                      UTL_Scope *s = ScopeAsDecl (this)->defined_in ();

                      if (s != 0)
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

          // OK, now return whatever we found.
          return d;
        }

      // For the possible call to look_in_inherited() below.
      AST_Decl::NodeType nt = d->node_type ();
      t = DeclAsScope (d);

      // OK, start of name is defined. Now loop doing local lookups
      // of subsequent elements of the name, if any.
      UTL_ScopedName *sn = (UTL_ScopedName *) e->tail ();

      if (sn != 0)
        {
          d = iter_lookup_by_name_local (d,
                                         sn,
                                         0);
        }

      // If the start of the scoped name is an interface, and the
      // above lookup failed, it's possible that what we're looking
      // up was inherited into that interface. The first call to
      // look_in_inherited() is this function only checks base classes
      // of the scope (interface) we started the lookup from.
      if (d == 0 && nt == AST_Decl::NT_interface)
        {
          d = t->look_in_inherited (sn,
                                    treat_as_ref);
        }

      // If treat_as_ref is true and d is not 0, add d to
      // set of nodes referenced here.
      if (treat_as_ref && d != 0)
        {
          add_to_referenced (d,
                             I_FALSE,
                             0);
        }

      // All OK, name fully resolved.
      if (d != 0)
        {
          return d;
        }
      else
        {
          index++;
        }
    }
}

// Add a node to set of nodes referenced in this scope.
void
UTL_Scope::add_to_referenced (AST_Decl *e,
                              idl_bool recursive,
                              Identifier *id,
                              AST_Decl *ex)
{
  UTL_Scope *s = 0;
  AST_Decl **tmp;
  AST_Interface *itf = 0;
  long oreferenced_allocated;
  long i;

  if (e == 0)
    {
      return;
    }

  // Special case for forward declared interfaces in the
  // scope in which they're defined. Cannot add before full
  // definition is seen.
  if (e->node_type() == AST_Decl::NT_interface)
    {
      itf = AST_Interface::narrow_from_decl(e);

      if (itf != 0
          && itf->defined_in () == this
          && !itf->is_defined ())
        {
          return;
        }
    }

  // Only insert if it is not there already.
  if (this->referenced (e, id))
    {
      return;
    }

  // Make sure there's space for one more decl.
  if (this->pd_referenced_allocated == this->pd_referenced_used)
    {
      oreferenced_allocated = this->pd_referenced_allocated;
      pd_referenced_allocated += INCREMENT;

      ACE_NEW (tmp,
               AST_Decl *[this->pd_referenced_allocated]);

      for (i = 0; i < oreferenced_allocated; i++)
        {
          tmp[i] = this->pd_referenced[i];
        }

      delete [] this->pd_referenced;

      this->pd_referenced = tmp;
    }

  // Insert new decl.
  this->pd_referenced[this->pd_referenced_used++] = e;

  // Insert new reference.
  if (ex == 0)
    {
      this->pd_referenced[this->pd_referenced_used++] = e;
    }
  else if (this->referenced (ex))
    {
      for (i = this->pd_referenced_used; i > 0; i--)
        {
          this->pd_referenced[i] = this->pd_referenced[i - 1];

          if (this->pd_referenced[i-1] == ex)
            {
              this->pd_referenced[i] = e;
              break;
            }
        }

      ++this->pd_referenced_used;
    }

  // Now, if recursive is specified and "this" is not a common ancestor
  // of the referencing scope and the scope of definition of "e" then
  // add "e" to the set of referenced nodes in the parent of "this".
  if (recursive && !(e->has_ancestor (ScopeAsDecl (this))))
    {
      s = e->defined_in ();

      if (s != 0)
        {
          s->add_to_referenced (e,
                                recursive,
                                id);
        }
    }

  // Add the identifier arg, if non-null, to the identifier list.
  if (id)
    {
      if (this->pd_name_referenced_allocated == this->pd_name_referenced_used)
        {
          long name_referenced_allocated = this->pd_name_referenced_allocated;
          pd_name_referenced_allocated += INCREMENT;

          Identifier **name_tmp = 0;
          ACE_NEW (name_tmp,
                   Identifier *[this->pd_name_referenced_allocated]);

          for (i = 0; i < name_referenced_allocated; i++)
            {
              name_tmp[i] = this->pd_name_referenced[i];
            }

          delete [] this->pd_name_referenced;

          this->pd_name_referenced = name_tmp;
        }

      // Insert new identifier.
      this->pd_name_referenced[this->pd_name_referenced_used++] = id;
    }
}

void
UTL_Scope::replace_referenced (AST_Decl *old_decl,
                               AST_Decl *new_decl)
{
  for (int i = 0; i < this->pd_referenced_used; i++)
    {
      if (this->pd_referenced[i] == old_decl)
        {
          this->pd_referenced[i] = new_decl;
          break;
        }
    }

}


void
UTL_Scope::replace_scope (AST_Decl *old_decl,
                          AST_Decl *new_decl)
{
  for (int i = 0; i < pd_decls_used; i++)
    {
      if (this->pd_decls[i] == old_decl)
        {
          this->pd_decls[i] = new_decl;
          break;
        }
    }

}


// Add a node to set of nodes declared in this scope.
void
UTL_Scope::add_to_scope (AST_Decl *e,
                         AST_Decl *ex)
{
  if (e == 0)
    {
      return;
    }

  AST_Decl **tmp = this->pd_decls;
  long i = this->pd_decls_used;

  Identifier *decl_name = e->local_name ();
  char *decl_string = decl_name->get_string ();

  Identifier *ref_name = 0;
  char *ref_string = 0;

  // First, make sure there's no clash between e, that was
  // just declared, and some other identifier referenced
  // in this scope.
  for (; i > 0; i--, tmp++)
    {
      // A local declaration doesn't use a scoped name.
      ref_name = (*tmp)->local_name ();
      ref_string = ref_name->get_string ();

      // If the names compare exactly, it's a redefini8tion
      // error, unless they're both modules (which can be
      // reopened) or we have a belated definition of a
      // forward-declared interface.
      AST_Decl::NodeType new_nt = e->node_type ();
      AST_Decl::NodeType scope_elem_nt = (*tmp)->node_type ();

      if (decl_name->compare (ref_name) == I_TRUE
          && this->redef_clash (new_nt, scope_elem_nt) == I_TRUE)
        {
          idl_global->err ()->redef_error (decl_string,
                                           ref_string);

          return;
        }
      // If the spellings differ only by case, it's also an error,
      // unless one, but not both of the identifiers were escaped.
      else if (decl_name->case_compare_quiet (ref_name) == I_TRUE
               && !(decl_name->escaped () ^ ref_name->escaped ()))
        {
          if (idl_global->case_diff_error ())
            {
              idl_global->err ()->name_case_error (decl_string,
                                                   ref_string);

              return;
            }
          else
            {
              idl_global->err ()->name_case_warning (decl_string,
                                                     ref_string);
            }
        }
    }

  AST_Decl *d = ScopeAsDecl (this);
  AST_Decl::NodeType nt = d->node_type ();

  // The name of any scope except the unnamed scope formed by an operation
  // may not be redefined immediately within (and the root scope has no name).
  // As well as OBV factory construct.
  if (nt != AST_Decl::NT_root
      && nt != AST_Decl::NT_op
      && nt != AST_Decl::NT_factory)
    {
      Identifier *parent_name = d->local_name ();

      if (decl_name->compare (parent_name) == I_TRUE)
        {
          idl_global->err ()->redef_error (
                                  decl_name->get_string (),
                                  parent_name->get_string ()
                                );
        }
      else if (decl_name->case_compare_quiet (parent_name) == I_TRUE)
        {
          if (idl_global->case_diff_error ())
            {
              idl_global->err ()->name_case_error (
                                      decl_name->get_string (),
                                      parent_name->get_string ()
                                    );
            }
          else
            {
              idl_global->err ()->name_case_warning (
                                      decl_name->get_string (),
                                      parent_name->get_string ()
                                    );
            }
        }
    }

  // Now make sure there's space for one more.
  if (this->pd_decls_allocated == this->pd_decls_used)
    {
      long odecls_allocated = this->pd_decls_allocated;
      this->pd_decls_allocated += INCREMENT;

      ACE_NEW (tmp,
               AST_Decl *[pd_decls_allocated]);

      for (i = 0; i < odecls_allocated; i++)
        {
          tmp[i] = this->pd_decls[i];
        }

      delete [] this->pd_decls;

      this->pd_decls = tmp;
    }


  // Insert new decl.
  if (ex == 0)
    {
      this->pd_decls[this->pd_decls_used++] = e;
    }
  else
    {
      for (i = this->pd_decls_used; i > 0; i--)
        {
          this->pd_decls[i] = this->pd_decls[i - 1];

          if (this->pd_decls[i - 1] == ex)
            {
              this->pd_decls[i] = e;
              break;
            }
        }

      ++this->pd_decls_used;
    }
}

// Add a node to set of nodes representing manifest
// types defined in this scope.
void
UTL_Scope::add_to_local_types (AST_Decl *e)
{
  if (e == 0)
    {
      return;
    }

  // Make sure there's space for one more.
  if (this->pd_locals_allocated == this->pd_locals_used)
    {
      long olocals_allocated = pd_locals_allocated;
      pd_locals_allocated += INCREMENT;

      AST_Decl **tmp = 0;
      ACE_NEW (tmp,
               AST_Decl *[this->pd_locals_allocated]);

      for (long i = 0; i < olocals_allocated; i++)
        {
          tmp[i] = this->pd_local_types[i];
        }

      delete [] this->pd_local_types;

      this->pd_local_types = tmp;
    }

  // Insert new decl.
  this->pd_local_types[this->pd_locals_used++] = e;
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
      // Same node?
      if (*tmp == e)
        {
          return I_TRUE;
        }

      // Are we definging a forward declared struct, union, or interface,
      // or reopening a module?
      idl_bool forward_redef = this->redef_clash (e->node_type (),
                                                  (*tmp)->node_type ());

      if (forward_redef == I_FALSE)
        {
          member = (*tmp)->local_name ();
          test = e->local_name ();

          if (member->compare (test) == I_TRUE)
            {
              return I_FALSE;
            }
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
          // catch a clash, and if it wasn't defined in this
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
          // No clash if one or the other of the identifiers was escaped.
          else if (id->case_compare_quiet (*name_tmp) == I_TRUE
                   && !(id->escaped () ^ (*name_tmp)->escaped ()))
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

  // Not found
  return I_FALSE;
}

idl_bool
UTL_Scope::has_prefix (void)
{
  return this->has_prefix_;
}

void
UTL_Scope::has_prefix (idl_bool val)
{
  this->has_prefix_ = val;
}

// Redefinition of inherited virtual operations.

// AST Dumping.
void
UTL_Scope::dump (ACE_OSTREAM_TYPE &o)
{
  AST_Decl *d = 0;

  if (idl_global->indent () == 0)
    {
      UTL_Indenter *idnt = 0;
      ACE_NEW (idnt,
               UTL_Indenter);

      idl_global->set_indent (idnt);
    }

  idl_global->indent ()->increase ();

  if (pd_locals_used > 0)
    {
      o << ("\n/* Locally defined types: */\n");

      for (UTL_ScopeActiveIterator i (this, UTL_Scope::IK_localtypes);
           !i.is_done ();
           i.next ())
        {
          d = i.item ();

          if (!d->imported ())
            {
              idl_global->indent ()->skip_to (o);
              d->dump (o);
              o << "\n";
            }
        }
    }

  if (pd_decls_used > 0)
    {
      o << ACE_TEXT ("\n/* Declarations: */\n");

      for (UTL_ScopeActiveIterator j (this, UTL_Scope::IK_decls);
           !j.is_done ();
           j.next ())
        {
          d = j.item ();

          if (!d->imported ())
            {
              idl_global->indent ()->skip_to (o);
              d->dump (o);
              o << ";\n";
            }
        }
    }

  idl_global->indent ()->decrease ();
}

int
UTL_Scope::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_scope (this);
}

// How many entries are defined?
unsigned long
UTL_Scope::nmembers (void)
{
  return this->pd_decls_used;
}

void
UTL_Scope::destroy (void)
{
  for (UTL_ScopeActiveIterator iter (this, IK_both);
       !iter.is_done ();
       iter.next ())
    {
      AST_Decl *i = iter.item ();
      i->destroy ();
      delete i;
      i = 0;
    }

  delete [] this->pd_decls;
  this->pd_decls = 0;

  delete [] this->pd_local_types;
  this->pd_local_types = 0;

  delete [] this->pd_referenced;
  this->pd_referenced = 0;

  delete [] this->pd_name_referenced;
  this->pd_name_referenced = 0;
}

// Narrowing.
IMPL_NARROW_METHODS0(UTL_Scope)
IMPL_NARROW_FROM_SCOPE(UTL_Scope)

// UTL_SCOPE_ACTIVE_ITERATOR

// Constructor.
UTL_ScopeActiveIterator::UTL_ScopeActiveIterator (
    UTL_Scope *s,
    UTL_Scope::ScopeIterationKind i
  )
  : iter_source (s),
    ik (i),
    stage (i == UTL_Scope::IK_both ? UTL_Scope::IK_localtypes : i),
    il (0)
{
}

// Public operations.

// Advance to next item.
void
UTL_ScopeActiveIterator::next (void)
{
  this->il++;
}

// Get current item.
AST_Decl *
UTL_ScopeActiveIterator::item (void)
{
  if (this->is_done ())
    {
      return 0;
    }

  if (stage == UTL_Scope::IK_decls)
    {
      return this->iter_source->pd_decls[il];
    }

  if (stage == UTL_Scope::IK_localtypes)
    {
      return this->iter_source->pd_local_types[il];
    }

  return 0;
}

// Is this iteration done?
idl_bool
UTL_ScopeActiveIterator::is_done (void)
{
  long limit =
    (stage == UTL_Scope::IK_decls)
      ? iter_source->pd_decls_used
      : iter_source->pd_locals_used;

  for (;;)
    {
      // Last element?
      if (this->il < limit)
        {
          return I_FALSE;
        }

      // Only want decls?
      if (this->stage == UTL_Scope::IK_decls)
        {
          return I_TRUE;
        }

      // Already done local types?
      if (this->ik == UTL_Scope::IK_localtypes)
        {
          return I_TRUE;
        }

      // Switch to next stage.
      this->stage = UTL_Scope::IK_decls;
      this->il = 0;
      limit = this->iter_source->pd_decls_used;
    }
}

// What kind of iterator is this?
UTL_Scope::ScopeIterationKind
UTL_ScopeActiveIterator::iteration_kind (void)
{
  return this->ik;
}

// And where are we in the iteration?
UTL_Scope::ScopeIterationKind
UTL_ScopeActiveIterator::iteration_stage (void)
{
  return this->stage;
}
