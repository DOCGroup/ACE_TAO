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

// AST_Modules denote IDL module declarations
// AST_Modules are subclasses of AST_Decl (they are not a type!) and
// of UTL_Scope.

#include "ast_module.h"
#include "ast_predefined_type.h"
#include "ast_valuetype.h"
#include "ast_valuetype_fwd.h"
#include "ast_eventtype.h"
#include "ast_eventtype_fwd.h"
#include "ast_component.h"
#include "ast_component_fwd.h"
#include "ast_home.h"
#include "ast_constant.h"
#include "ast_exception.h"
#include "ast_union.h"
#include "ast_union_fwd.h"
#include "ast_structure_fwd.h"
#include "ast_enum.h"
#include "ast_enum_val.h"
#include "ast_native.h"
#include "ast_generator.h"
#include "ast_visitor.h"
#include "ast_extern.h"
#include "utl_err.h"
#include "utl_identifier.h"
#include "utl_indenter.h"
#include "global_extern.h"
#include "nr_extern.h"

ACE_RCSID (ast, 
           ast_module, 
           "$Id$")

AST_Module::AST_Module (void)
{
}

AST_Module::AST_Module (UTL_ScopedName *n)
 : AST_Decl (AST_Decl::NT_module,
             n),
   UTL_Scope (AST_Decl::NT_module),
   pd_has_nested_valuetype (0)
{
}

AST_Module::~AST_Module (void)
{
}

// Add this AST_PredefinedType node (a predefined type declaration) to
// this scope.

AST_PredefinedType *
AST_Module::fe_add_predefined_type (AST_PredefinedType *t)
{
  AST_Decl *d = 0;

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
          idl_global->err()->redefinition_in_scope (t,
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

  return t;
}

// Add this AST_Module node (a module declaration) to this scope.
AST_Module *
AST_Module::fe_add_module (AST_Module *t)
{
  AST_Decl *d;
  AST_Module *m = 0;

  UTL_Scope *scope = t->defined_in ();
  const char *prefix_holder = 0;

  // If our prefix is empty, we check to see if an ancestor has one.
  while (ACE_OS::strcmp (t->prefix (), "") == 0 && scope != 0)
    {
      AST_Decl *parent = ScopeAsDecl (scope);
      prefix_holder = parent->prefix ();

      // We have reached global scope.
      if (prefix_holder == 0)
        {
          break;
        }

      t->prefix (ACE_const_cast (char *, prefix_holder));
      scope = parent->defined_in ();
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

      m = AST_Module::narrow_from_decl (d);

      // has_ancestor() returns TRUE if both nodes are the same.
      if (t != m)
        {
          if (t->has_ancestor (d))
            {
              idl_global->err ()->redefinition_in_scope (t,
                                                         d);
              return 0;
            }
        }

      const char *prev_prefix = d->prefix ();
      const char *this_prefix = t->prefix ();

      if (ACE_OS::strcmp (this_prefix, "") == 0)
        {
          t->prefix (ACE_const_cast (char *, prev_prefix));
        }
      else
        {
          if (ACE_OS::strcmp (prev_prefix, "") == 0)
            {
              d->prefix (ACE_const_cast (char *, this_prefix));
            }
          else
            {
              if (ACE_OS::strcmp (this_prefix, prev_prefix) != 0)
                {
                  idl_global->err ()->error2 (UTL_Error::EIDL_PREFIX_CONFLICT,
                                              this,
                                              d);

                  return 0;
                }
            }
        }
    }

  // If this node is not a reopened module, add it to scope and referenced.
  if (m == 0 || t != m)
    {
      // Add it to scope.
      this->add_to_scope (t);

      // Add it to set of locally referenced symbols.
      this->add_to_referenced (t,
                               I_FALSE,
                               t->local_name ());
    }

  return t;
}

// Add this AST_Interface node (an interface declaration) to this scope.
AST_Interface *
AST_Module::fe_add_interface (AST_Interface *t)
{
  if (t->redef_clash ())
    {
      return 0;
    }

  AST_Decl *predef = 0;
  AST_Interface *fwd = 0;

  // Already defined?
  if ((predef = this->lookup_for_add (t, I_FALSE)) != 0)
    {
      // Treat fwd declared interfaces specially
      if (predef->node_type () == AST_Decl::NT_interface)
        {
          fwd = AST_Interface::narrow_from_decl (predef);

          if (fwd == 0)
            {
              return 0;
            }

          // Forward declared and not defined yet.
          if (!fwd->is_defined ())
            {
              if (fwd->defined_in () != this)
                {
                  idl_global->err ()->error3 (UTL_Error::EIDL_SCOPE_CONFLICT,
                                              fwd,
                                              t,
                                              this);

                  return 0;
                }
            }
          // OK, not illegal redef of forward declaration. Now check whether.
          // it has been referenced already.
          else if (this->referenced (predef, t->local_name ()))
            {
              idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                          t,
                                          this,
                                          predef);

              return 0;
            }
        }
      else if (!can_be_redefined (predef))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      t,
                                      this,
                                      predef);

          return 0;
        }
      else if (referenced (predef, t->local_name ()) && !t->is_defined ())
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      t,
                                      this,
                                      predef);

          return 0;
        }
      else if (t->has_ancestor (predef))
        {
          idl_global->err ()->redefinition_in_scope (t,
                                                     predef);

          return 0;
        }
    }

  // Add it to scope
  this->add_to_scope (t);

  // Add it to set of locally referenced symbols
  this->add_to_referenced (t,
                           I_FALSE,
                           t->local_name ());
  return t;
}

// Add this AST_ValueType node (a value type declaration) to this scope.
AST_ValueType *
AST_Module::fe_add_valuetype (AST_ValueType *t)
{
  if (t->redef_clash ())
    {
      return 0;
    }

  AST_Decl *predef = 0;
  AST_ValueType *fwd = 0;

  // Already defined?
  if ((predef = this->lookup_for_add (t, I_FALSE)) != 0)
    {
      // Treat fwd declared interfaces specially
      if (predef->node_type () == AST_Decl::NT_valuetype)
        {
          fwd = AST_ValueType::narrow_from_decl (predef);

          if (fwd == 0)
            {
              return 0;
            }

          // Forward declared and not defined yet.
          if (!fwd->is_defined ())
            {
              if (fwd->defined_in () != this)
                {
                  idl_global->err ()->error3 (UTL_Error::EIDL_SCOPE_CONFLICT,
                                              fwd,
                                              t,
                                              this);

                  return 0;
                }
            }
          // OK, not illegal redef of forward declaration. Now check whether.
          // it has been referenced already.
          else if (this->referenced (predef, t->local_name ()))
            {
              idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                          t,
                                          this,
                                          predef);

              return 0;
            }
        }
      else if (!can_be_redefined (predef))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      t,
                                      this,
                                      predef);

          return 0;
        }
      else if (referenced (predef, t->local_name ()) && !t->is_defined ())
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      t,
                                      this,
                                      predef);

          return 0;
        }
      else if (t->has_ancestor (predef))
        {
          idl_global->err ()->redefinition_in_scope (t,
                                                     predef);

          return 0;
        }
    }

  // Add it to scope
  this->add_to_scope (t);

  // Add it to set of locally referenced symbols
  this->add_to_referenced (t,
                           I_FALSE,
                           t->local_name ());
  return t;
}

// Add this AST_EventType node (an event type declaration) to this scope.
AST_EventType *
AST_Module::fe_add_eventtype (AST_EventType *t)
{
  if (t->redef_clash ())
    {
      return 0;
    }

  AST_Decl *predef = 0;
  AST_EventType *fwd = 0;

  // Already defined?
  if ((predef = this->lookup_for_add (t, I_FALSE)) != 0)
    {
      // Treat fwd declared interfaces specially
      if (predef->node_type () == AST_Decl::NT_eventtype)
        {
          fwd = AST_EventType::narrow_from_decl (predef);

          if (fwd == 0)
            {
              return 0;
            }

          // Forward declared and not defined yet.
          if (!fwd->is_defined ())
            {
              if (fwd->defined_in () != this)
                {
                  idl_global->err ()->error3 (UTL_Error::EIDL_SCOPE_CONFLICT,
                                              fwd,
                                              t,
                                              this);

                  return 0;
                }
            }
          // OK, not illegal redef of forward declaration. Now check whether.
          // it has been referenced already.
          else if (this->referenced (predef, t->local_name ()))
            {
              idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                          t,
                                          this,
                                          predef);

              return 0;
            }
        }
      else if (!can_be_redefined (predef))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      t,
                                      this,
                                      predef);

          return 0;
        }
      else if (referenced (predef, t->local_name ()) && !t->is_defined ())
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      t,
                                      this,
                                      predef);

          return 0;
        }
      else if (t->has_ancestor (predef))
        {
          idl_global->err ()->redefinition_in_scope (t,
                                                     predef);

          return 0;
        }
    }

  // Add it to scope
  this->add_to_scope (t);

  // Add it to set of locally referenced symbols
  this->add_to_referenced (t,
                           I_FALSE,
                           t->local_name ());
  return t;
}

// Add this AST_Component node (a value type declaration) to this scope.
AST_Component *
AST_Module::fe_add_component (AST_Component *t)
{
  if (t->redef_clash ())
    {
      return 0;
    }

  AST_Decl *predef = 0;
  AST_Component *fwd = 0;

  // Already defined?
  if ((predef = this->lookup_for_add (t, I_FALSE)) != 0)
    {
      // Treat fwd declared interfaces specially
      if (predef->node_type () == AST_Decl::NT_component)
        {
          fwd = AST_Component::narrow_from_decl (predef);

          if (fwd == 0)
            {
              return 0;
            }

          // Forward declared and not defined yet.
          if (!fwd->is_defined ())
            {
              if (fwd->defined_in () != this)
                {
                  idl_global->err ()->error3 (UTL_Error::EIDL_SCOPE_CONFLICT,
                                              fwd,
                                              t,
                                              this);

                  return 0;
                }
            }
          // OK, not illegal redef of forward declaration. Now check whether.
          // it has been referenced already.
          else if (this->referenced (predef, t->local_name ()))
            {
              idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                          t,
                                          this,
                                          predef);

              return 0;
            }
        }
      else if (!can_be_redefined (predef))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      t,
                                      this,
                                      predef);

          return 0;
        }
      else if (referenced (predef, t->local_name ()) && !t->is_defined ())
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      t,
                                      this,
                                      predef);

          return 0;
        }
      else if (t->has_ancestor (predef))
        {
          idl_global->err ()->redefinition_in_scope (t,
                                                     predef);

          return 0;
        }
    }

  // Add it to scope
  this->add_to_scope (t);

  // Add it to set of locally referenced symbols
  this->add_to_referenced (t,
                           I_FALSE,
                           t->local_name ());
  return t;
}

// Add this AST_Home node (a value type declaration) to this scope.
AST_Home *
AST_Module::fe_add_home (AST_Home *t)
{
  AST_Decl *d = 0;

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

      if (this->referenced  (d, t->local_name ()))
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

  // The home's local name is not added to the referenced list, since
  // the name will later be mangled to allow a creation of an
  // equivalent interface with the original name.

  return t;
}

// Add this AST_InterfaceFwd node (a forward declaration of an IDL
// interface) to this scope.
AST_InterfaceFwd *
AST_Module::fe_add_interface_fwd (AST_InterfaceFwd *i)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (i, I_FALSE)) != 0)
    {
      AST_Decl::NodeType nt = d->node_type ();

      if (nt == AST_Decl::NT_interface_fwd)
        {
          AST_InterfaceFwd *ifwd = AST_InterfaceFwd::narrow_from_decl (d);
          i->set_full_definition (ifwd->full_definition ());
        }

      // There used to be another check here ANDed with the one below:
      // d->defined_in () == this. But lookup_for_add calls only
      // lookup_by_name_local(), which does not bump up the scope,
      // and look_in_previous() for modules. If look_in_previous()
      // finds something, the scopes will NOT be the same pointer
      // value, but the result is what we want.
      if (nt == AST_Decl::NT_interface)
        {
          AST_Interface *itf = AST_Interface::narrow_from_decl (d);

          if (itf == 0)
            {
              return 0;
            }

          if (i->added () == 0)
            {
              i->set_added (1);
              this->add_to_scope (i);
            }

          // @@ Redefinition of forward. Type check not implemented.
          i->set_full_definition (itf);   // @@ Memory leak.
          return i;
        }

      if (!can_be_redefined (d)) {

          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      i,
                                      this,
                                      d);
          return 0;
        }

      if (this->referenced (d, i->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      i,
                                      this,
                                      d);
          return 0;
        }

      if (i->has_ancestor (d))
        {
          idl_global->err ()->redefinition_in_scope  (i,
                                                      d);
          return 0;
        }
    }

  // Add it to scope
  this->add_to_scope (i);

  // Add it to set of locally referenced symbols
  this->add_to_referenced (i,
                           I_FALSE,
                           i->local_name ());

  return i;
}

// Add this AST_ValueTypeFwd node (a forward declaration of an IDL
// value type) to this scope.
AST_ValueTypeFwd *
AST_Module::fe_add_valuetype_fwd (AST_ValueTypeFwd *v)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (v, I_FALSE)) != 0)
    {
      AST_Decl::NodeType nt = d->node_type ();

      if (nt == AST_Decl::NT_valuetype_fwd)
        {
          AST_ValueTypeFwd *vfwd = AST_ValueTypeFwd::narrow_from_decl (d);
          v->set_full_definition (vfwd->full_definition ());
        }

      // There used to be another check here ANDed with the one below:
      // d->defined_in () == this. But lookup_for_add calls only
      // lookup_by_name_local(), which does not bump up the scope,
      // and look_in_previous() for modules. If look_in_previous()
      // finds something, the scopes will NOT be the same pointer
      // value, but the result is what we want.
      if (nt == AST_Decl::NT_valuetype)
        {
          AST_ValueType *vtf = AST_ValueType::narrow_from_decl (d);

          if (vtf == 0)
            {
              return 0;
            }

          if (v->added () == 0)
            {
              v->set_added (1);
              this->add_to_scope (v);
            }

          // @@ Redefinition of forward. Type check not implemented.
          v->set_full_definition (vtf);   // @@ Memory leak.
          return v;
        }

      if (!can_be_redefined (d)) {

          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      v,
                                      this,
                                      d);
          return 0;
        }

      if (this->referenced (d, v->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      v,
                                      this,
                                      d);
          return 0;
        }

      if (v->has_ancestor (d))
        {
          idl_global->err ()->redefinition_in_scope  (v,
                                                      d);
          return 0;
        }
    }

  // Add it to scope
  this->add_to_scope (v);

  // Add it to set of locally referenced symbols
  this->add_to_referenced (v,
                           I_FALSE,
                           v->local_name ());

  return v;
}

// Add this AST_EventTypeFwd node (a forward declaration of an IDL
// event type) to this scope.
AST_EventTypeFwd *
AST_Module::fe_add_eventtype_fwd (AST_EventTypeFwd *v)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (v, I_FALSE)) != 0)
    {
      AST_Decl::NodeType nt = d->node_type ();

      if (nt == AST_Decl::NT_eventtype_fwd)
        {
          AST_EventTypeFwd *efwd = AST_EventTypeFwd::narrow_from_decl (d);
          v->set_full_definition (efwd->full_definition ());
        }

      // There used to be another check here ANDed with the one below:
      // d->defined_in () == this. But lookup_for_add calls only
      // lookup_by_name_local(), which does not bump up the scope,
      // and look_in_previous() for modules. If look_in_previous()
      // finds something, the scopes will NOT be the same pointer
      // value, but the result is what we want.
      if (nt == AST_Decl::NT_eventtype)
        {
          AST_EventType *vtf = AST_EventType::narrow_from_decl (d);

          if (vtf == 0)
            {
              return 0;
            }

          if (v->added () == 0)
            {
              v->set_added (1);
              this->add_to_scope (v);
            }

          // @@ Redefinition of forward. Type check not implemented.
          v->set_full_definition (vtf);   // @@ Memory leak.
          return v;
        }

      if (!can_be_redefined (d)) {

          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      v,
                                      this,
                                      d);
          return 0;
        }

      if (this->referenced (d, v->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      v,
                                      this,
                                      d);
          return 0;
        }

      if (v->has_ancestor (d))
        {
          idl_global->err ()->redefinition_in_scope  (v,
                                                      d);
          return 0;
        }
    }

  // Add it to scope
  this->add_to_scope (v);

  // Add it to set of locally referenced symbols
  this->add_to_referenced (v,
                           I_FALSE,
                           v->local_name ());

  return v;
}

// Add this AST_ComponentFwd node (a forward declaration of an IDL
// value type) to this scope.
AST_ComponentFwd *
AST_Module::fe_add_component_fwd (AST_ComponentFwd *c)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (c, I_FALSE)) != 0)
    {
      AST_Decl::NodeType nt = d->node_type ();

      if (nt == AST_Decl::NT_component_fwd)
        {
          AST_ComponentFwd *cfwd = AST_ComponentFwd::narrow_from_decl (d);
          c->set_full_definition (cfwd->full_definition ());
        }

      // There used to be another check here ANDed with the one below:
      // d->defined_in () == this. But lookup_for_add calls only
      // lookup_by_name_local(), which does not bump up the scope,
      // and look_in_previous() for modules. If look_in_previous()
      // finds something, the scopes will NOT be the same pointer
      // value, but the result is what we want.
      if (nt == AST_Decl::NT_component)
        {
          AST_Component *cf = AST_Component::narrow_from_decl (d);

          if (cf == 0)
            {
              return 0;
            }

          if (c->added () == 0)
            {
              c->set_added (1);
              this->add_to_scope (c);
            }

          // @@ Redefinition of forward. Type check not implemented.
          c->set_full_definition (cf);   // @@ Memory leak.
          return c;
        }

      if (!can_be_redefined (d)) {

          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      c,
                                      this,
                                      d);
          return 0;
        }

      if (this->referenced (d, c->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      c,
                                      this,
                                      d);
          return 0;
        }

      if (c->has_ancestor (d))
        {
          idl_global->err ()->redefinition_in_scope  (c,
                                                      d);
          return 0;
        }
    }

  // Add it to scope
  this->add_to_scope (c);

  // Add it to set of locally referenced symbols
  this->add_to_referenced (c,
                           I_FALSE,
                           c->local_name ());

  return c;
}

// Add this AST_Constant node (a constant declaration) to this scope.
AST_Constant *
AST_Module::fe_add_constant (AST_Constant *t)
{
  AST_Decl *d = 0;

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
          return NULL;
        }
    }

  // Add it to scope.
  this->add_to_scope (t);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (t,
                           I_FALSE,
                           t->local_name ());

  return t;
}

// Add this AST_Exception node (an exception declaration) to this scope
AST_Exception *
AST_Module::fe_add_exception (AST_Exception *t)
{
  AST_Decl *d = 0;

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

      if (this->referenced  (d, t->local_name ()))
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

  return t;
}

// Add this AST_Union node (a union declaration) to this scope
AST_Union *
AST_Module::fe_add_union (AST_Union *t)
{
  AST_Decl *predef = 0;
  AST_UnionFwd *fwd = 0;

  if ((predef = this->lookup_for_add (t, I_FALSE)) != 0)
    {
      // Treat fwd declared interfaces specially
      if (predef->node_type () == AST_Decl::NT_union_fwd)
        {
          fwd = AST_UnionFwd::narrow_from_decl (predef);

          if (fwd == 0)
            {
              return 0;
            }

          // Forward declared and not defined yet.
          if (!fwd->is_defined ())
            {
              UTL_Scope *s = fwd->defined_in ();
              UTL_ScopedName *sn = ScopeAsDecl (s)->name ();

              if (fwd->defined_in () == this
                  || sn->compare (this->name ()) == 0)
                {
                  fwd->set_full_definition (t);
                }
              else
                {
                  idl_global->err ()->error3 (UTL_Error::EIDL_SCOPE_CONFLICT,
                                              fwd,
                                              t,
                                              this);

                  return 0;
                }
            }
          // OK, not illegal redef of forward declaration. Now check whether.
          // it has been referenced already.
          else if (this->referenced (predef, t->local_name ()))
            {
              idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                          t,
                                          this,
                                          predef);

              return 0;
            }
        }
      else if (!can_be_redefined (predef))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      t,
                                      this,
                                      predef);

          return 0;
        }
      else if (referenced (predef, t->local_name ()) && !t->is_defined ())
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      t,
                                      this,
                                      predef);

          return 0;
        }
    }

  // Add it to scope.
  this->add_to_scope (t);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (t,
                           I_FALSE,
                           t->local_name ());

  return t;
}

// Add this AST_UnionFwd node (a forward declaration of an IDL
// union) to this scope.
AST_UnionFwd *
AST_Module::fe_add_union_fwd (AST_UnionFwd *t)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (t, I_FALSE)) != 0)
    {
      AST_Decl::NodeType nt = d->node_type ();

      if (nt == AST_Decl::NT_union_fwd)
        {
          AST_UnionFwd *ufwd = AST_UnionFwd::narrow_from_decl (d);
          t->set_full_definition (ufwd->full_definition ());
        }

      // There used to be another check here ANDed with the one below:
      // d->defined_in () == this. But lookup_for_add calls only
      // lookup_by_name_local(), which does not bump up the scope,
      // and look_in_previous() for modules. If look_in_previous()
      // finds something, the scopes will NOT be the same pointer
      // value, but the result is what we want.
      if (nt == AST_Decl::NT_union)
        {
          AST_Union *s = AST_Union::narrow_from_decl (d);
          t->set_full_definition (s);

          if (t->added () == 0)
            {
              t->set_added (1);
              this->add_to_scope (t);

              // Must check later that all struct and union forward declarations
              // are defined in the same IDL file.
              AST_record_fwd_decl (t);
            }

          return t;
        }
      else
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
        }
    }

  // Add it to scope
  this->add_to_scope (t);

  // Add it to set of locally referenced symbols
  this->add_to_referenced (t,
                           I_FALSE,
                           t->local_name ());

  // Must check later that all struct and union forward declarations
  // are defined in the same IDL file.
  AST_record_fwd_decl (t);
  return t;
}

// Add this AST_Structure node (a struct declaration) to this scope.
AST_Structure *
AST_Module::fe_add_structure (AST_Structure *t)
{
  AST_Decl *predef = 0;
  AST_StructureFwd *fwd = 0;

  if ((predef = this->lookup_for_add (t, I_FALSE)) != 0)
    {
      // Treat fwd declared interfaces specially
      if (predef->node_type () == AST_Decl::NT_struct_fwd)
        {
          fwd = AST_StructureFwd::narrow_from_decl (predef);

          if (fwd == 0)
            {
              return 0;
            }

          // Forward declared and not defined yet.
          if (!fwd->is_defined ())
            {
              UTL_Scope *s = fwd->defined_in ();
              UTL_ScopedName *sn = ScopeAsDecl (s)->name ();

              if (fwd->defined_in () == this
                  || sn->compare (this->name ()) == 0)
                {
                  fwd->set_full_definition (t);
                }
              else
                {
                  idl_global->err ()->error3 (UTL_Error::EIDL_SCOPE_CONFLICT,
                                              fwd,
                                              t,
                                              this);

                  return 0;
                }
            }
          // OK, not illegal redef of forward declaration. Now check whether.
          // it has been referenced already.
          else if (this->referenced (predef, t->local_name ()))
            {
              idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                          t,
                                          this,
                                          predef);

              return 0;
            }
        }
      else if (!can_be_redefined (predef))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      t,
                                      this,
                                      predef);

          return 0;
        }
      else if (referenced (predef, t->local_name ()) && !t->is_defined ())
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      t,
                                      this,
                                      predef);

          return 0;
        }
    }

  // Add it to scope.
  this->add_to_scope (t);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (t,
                           I_FALSE,
                           t->local_name ());

  return t;
}

// Add this AST_StructureFwd node (a forward declaration of an IDL
// struct) to this scope.
AST_StructureFwd *
AST_Module::fe_add_structure_fwd (AST_StructureFwd *t)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (t, I_FALSE)) != 0)
    {
      AST_Decl::NodeType nt = d->node_type ();

      if (nt == AST_Decl::NT_struct_fwd)
        {
          AST_StructureFwd *sfwd = AST_StructureFwd::narrow_from_decl (d);
          t->set_full_definition (sfwd->full_definition ());
        }

      // There used to be another check here ANDed with the one below:
      // d->defined_in () == this. But lookup_for_add calls only
      // lookup_by_name_local(), which does not bump up the scope,
      // and look_in_previous() for modules. If look_in_previous()
      // finds something, the scopes will NOT be the same pointer
      // value, but the result is what we want.
      if (nt == AST_Decl::NT_struct)
        {
          AST_Structure *s = AST_Structure::narrow_from_decl (d);
          t->set_full_definition (s);

          if (t->added () == 0)
            {
              t->set_added (1);
              this->add_to_scope (t);

              // Must check later that all struct and union forward declarations
              // are defined in the same IDL file.
              AST_record_fwd_decl (t);
            }

          return t;
        }
      else
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
        }
    }

  // Add it to scope
  this->add_to_scope (t);

  // Add it to set of locally referenced symbols
  this->add_to_referenced (t,
                           I_FALSE,
                           t->local_name ());

  // Must check later that all struct and union forward declarations
  // are defined in the same IDL file.
  AST_record_fwd_decl (t);
  return t;
}

// Add this AST_Enum node (an enum declaration) to this scope.
AST_Enum *
AST_Module::fe_add_enum (AST_Enum *t)
{
  AST_Decl *d = 0;

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

      if (t->has_ancestor(d))
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

  return t;
}

// Add this AST_EnumVal node (an enumerator declaration) to this scope
// This is done to conform to the C++ scoping rules which declare
// enumerators in the enclosing scope (in addition to declaring them
// in the enum itself).
AST_EnumVal *
AST_Module::fe_add_enum_val (AST_EnumVal *t)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add(t, I_FALSE)) != 0)
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

  return t;
}

// Add this AST_Typedef node (a typedef) to this scope.
AST_Typedef *
AST_Module::fe_add_typedef (AST_Typedef *t)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add(t, I_FALSE)) != 0)
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

  AST_Type *bt = t->base_type ();
  UTL_ScopedName *mru = bt->last_referenced_as ();

  if (mru != 0)
    {
      this->add_to_referenced (bt,
                               I_FALSE,
                               mru->first_component ());
    }

  return t;
}

// Add an AST_Native (a native declaration) to this scope.
AST_Native *
AST_Module::fe_add_native (AST_Native *t)
{
  AST_Decl *d = 0;

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

  return t;
}

// Dump this AST_Module node to the ostream o.
void
AST_Module::dump (ACE_OSTREAM_TYPE &o)
{
  o << "module ";
  this->local_name ()->dump (o);
  o << " {\n";
  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);
  o << "}";
}


// Involved in OBV_ namespace generation.
void
AST_Module::set_has_nested_valuetype (void)
{
  UTL_Scope *parent = this->defined_in ();

  if (!this->pd_has_nested_valuetype && parent)
    {
      AST_Module *pm = AST_Module::narrow_from_scope (parent);

      if (pm != 0)
        {
          pm->set_has_nested_valuetype ();
        }
    }

  this->pd_has_nested_valuetype = 1;
}

idl_bool
AST_Module::has_nested_valuetype (void)
{
  return this->pd_has_nested_valuetype;
}

int
AST_Module::be_add_interface (AST_Interface *i,
                              AST_Interface *ix)
{
  // Add it to scope.
  this->add_to_scope (i,
                      ix);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (i,
                           I_FALSE,
                           i->local_name (),
                           ix);

  return 0;
}

void
AST_Module::add_CORBA_members (void)
{
 UTL_ScopedName *sn = 0;
  Identifier *id = 0;

  ACE_NEW (id,
           Identifier ("TypeCode"));

  ACE_NEW (sn,
           UTL_ScopedName (id,
                           0));

  AST_PredefinedType *pdt =
    idl_global->gen ()->create_predefined_type (
                            AST_PredefinedType::PT_pseudo,
                            sn
                          );

  this->fe_add_predefined_type (pdt);
}

void
AST_Module::add_to_previous (AST_Module *m)
{
  // Here, we depend on the scope iterator in
  // be_generator::create_module (which calls this function)
  // to return items in the order they were declared or included.
  // That means that the last module returned that matches the name
  // of this one will have all the decls from all previous
  // reopenings in its previous_ member.
  this->previous_ = m->previous_;

  AST_Decl *d = 0;

  for (UTL_ScopeActiveIterator iter (DeclAsScope (m), IK_decls);
       !iter.is_done ();
       iter.next ())
    {
      d = iter.item ();

      // Add all the previous opening's decls (except
      // for the predefined types) to the 'previous' list
      // of this one.
      if (d->node_type () == AST_Decl::NT_pre_defined)
        {
          continue;
        }
      else if (d->node_type () == AST_Decl::NT_interface_fwd)
        {
          AST_InterfaceFwd *f = AST_InterfaceFwd::narrow_from_decl (d);
          AST_Interface *i = f->full_definition ();

          // If i is defined, it means that the interface was forward
          // declared AFTER it was defined, perhaps in a subsequent
          // opening of the same module - legal, but superfluous.
          // Adding d to previous_ in that case can only bung up the
          // results of look_in_previous() later, so we skip it.
          if (i->is_defined ())
            {
              continue;
            }
        }

      this->previous_.insert (d);
    }
}

AST_Decl *
AST_Module::look_in_previous (Identifier *e)
{
  AST_Decl *d = 0;
  AST_Decl *retval = 0;
  ACE_Unbounded_Set_Iterator<AST_Decl *> iter (this->previous_);

  // If there are more than two openings of this module, we want
  // to get the last one - the one that will have the decls from
  // all the previous openings added to previous_.
  while (!iter.done ())
    {
      d = *iter;

      if (e->case_compare (d->local_name ()))
        {
          retval = d;
        }

      iter++;
    }

  return retval;
}

ACE_Unbounded_Set<AST_Decl *> &
AST_Module::previous (void)
{
  return this->previous_;
}

void
AST_Module::destroy (void)
{
  this->AST_Decl::destroy ();
}

int
AST_Module::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_module (this);
}

// Narrowing methods
IMPL_NARROW_METHODS2(AST_Module, AST_Decl, UTL_Scope)
IMPL_NARROW_FROM_DECL(AST_Module)
IMPL_NARROW_FROM_SCOPE(AST_Module)
