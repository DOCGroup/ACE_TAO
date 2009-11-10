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

// AST_Interfaces denote IDL interface definitions
// AST_Interfaces are subclasses of AST_Type and UTL_Scope
// AST_Interfaces have an array of inherited interfaces and
// a count of the number of inherited interfaces. This count
// represents the total number of unique (recursively) inherited
// interfaces.

#include "ast_interface.h"
#include "ast_interface_fwd.h"
#include "ast_valuetype.h"
#include "ast_component.h"
#include "ast_constant.h"
#include "ast_exception.h"
#include "ast_attribute.h"
#include "ast_operation.h"
#include "ast_field.h"
#include "ast_enum.h"
#include "ast_enum_val.h"
#include "ast_union.h"
#include "ast_union_fwd.h"
#include "ast_structure_fwd.h"
#include "ast_native.h"
#include "ast_visitor.h"
#include "ast_extern.h"
#include "utl_err.h"
#include "utl_identifier.h"
#include "utl_indenter.h"
#include "utl_string.h"
#include "global_extern.h"
#include "nr_extern.h"

#include "ace/streams.h"

ACE_RCSID (ast,
           ast_interface,
           "$Id$")

AST_Interface::AST_Interface (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    UTL_Scope (),
    pd_inherits (0),
    pd_n_inherits (0),
    pd_inherits_flat (0),
    pd_n_inherits_flat (0),
    home_equiv_ (false),
    fwd_decl_ (0)
{
  this->size_type (AST_Type::VARIABLE); // Always the case.
  this->has_constructor (true);      // Always the case.
}

AST_Interface::AST_Interface (UTL_ScopedName *n,
                              AST_Interface **ih,
                              long nih,
                              AST_Interface **ih_flat,
                              long nih_flat,
                              bool local,
                              bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (AST_Decl::NT_interface,
              n),
    AST_Type (AST_Decl::NT_interface,
              n),
    UTL_Scope (AST_Decl::NT_interface),
    pd_inherits (ih),
    pd_n_inherits (nih),
    pd_inherits_flat (ih_flat),
    pd_n_inherits_flat (nih_flat),
    home_equiv_ (false),
    fwd_decl_ (0)
{
  this->size_type (AST_Type::VARIABLE); // always the case
  this->has_constructor (true);      // always the case
}

AST_Interface::~AST_Interface (void)
{
}

void
AST_Interface::be_replace_operation (AST_Decl *old_op,
                                     AST_Decl *new_op)
{
  this->replace_scope (old_op,
                       new_op);

  this->replace_referenced (old_op,
                            new_op);
}

AST_Operation *
AST_Interface::be_add_operation (AST_Operation *op)
{
  return this->fe_add_operation (op);
}

bool
AST_Interface::is_defined (void)
{
  // Each instance of a forward declared interface no
  // longer has a redefined full definition, so we
  // have to backtrack to the fwd decl is_defined(),
  // which searches for the one that does. If one
  // is found, then we are defined for code generation
  // purposes. See AST_InterfaceFwd::destroy() to
  // see the difference for cleanup purposes.
  return (0 == this->fwd_decl_
          ? this->pd_n_inherits >= 0
          : this->fwd_decl_->is_defined ());
}

// Add an AST_Constant node (a constant declaration) to this scope.
AST_Constant *
AST_Interface::fe_add_constant (AST_Constant *t)
{
  AST_Decl *d = 0;

  // Can't add to interface which was not yet defined
  if (!this->is_defined ())
    {
      idl_global->err ()->error2 (UTL_Error::EIDL_DECL_NOT_DEFINED,
                                  this,
                                  t);
      return 0;
    }

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (t, false)) != 0)
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
                           false,
                           t->local_name ());

  return t;
}

// Add an AST_Exception node (an exception declaration) to this scope.
AST_Exception *
AST_Interface::fe_add_exception (AST_Exception *t)
{
  AST_Decl *d = 0;

  // Can't add to interface which was not yet defined
  if (!this->is_defined ())
    {
      idl_global->err ()->error2 (UTL_Error::EIDL_DECL_NOT_DEFINED,
                                  this,
                                  t);
      return 0;
    }

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (t, false)) != 0)
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
                           false,
                           t->local_name ());

  return t;
}

// Add an AST_Attribute node (an attribute declaration) to this scope.
AST_Attribute *
AST_Interface::fe_add_attribute (AST_Attribute *t)
{
  AST_Decl *d = 0;

  // Can't add to interface which was not yet defined.
   if (!this->is_defined ())
    {
      idl_global->err ()->error2 (UTL_Error::EIDL_DECL_NOT_DEFINED,
                                  this,
                                  t);
      return 0;
    }
  /*
   * Already defined and cannot be redefined? Or already used?
   */
  if ((d = this->lookup_for_add (t, false)) != 0)
    {
      if (!can_be_redefined (d))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      t,
                                      this,
                                      d);
          return 0;
        }

      if (referenced (d, t->local_name ()))
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
                           false,
                           t->local_name ());

  return t;
}

// Add this AST_Field node (a field declaration) to this scope
// (only for valuetypes).
AST_Field *
AST_Interface::fe_add_field (AST_Field *t)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (t, false)) != 0)
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
                           false,
                           t->local_name ());

  AST_Type *ft = t->field_type ();
  UTL_ScopedName *mru = ft->last_referenced_as ();

  if (mru != 0)
    {
      this->add_to_referenced (ft,
                               false,
                               mru->first_component ());
    }

  return t;
}

// Add an AST_Operation node (an operation declaration) to this scope.
AST_Operation *
AST_Interface::fe_add_operation (AST_Operation *t)
{
  AST_Decl *d = 0;

  // Can't add to interface which was not yet defined.
  if (!this->is_defined ())
    {
      idl_global->err ()->error2 (UTL_Error::EIDL_DECL_NOT_DEFINED,
                                  this,
                                  t);
      return 0;
    }

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (t, false)) != 0)
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
  else if ((d = this->look_in_inherited (t->name (), false)) != 0)
    {
      if (d->node_type () == AST_Decl::NT_op)
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      t,
                                      this,
                                      d);
          return 0;
        }
    }

  // Add it to scope.
  this->add_to_scope (t);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (t,
                           false,
                           t->local_name ());

  return t;
}

// Add an AST_Structure (a struct declaration) to this scope.
AST_Structure *
AST_Interface::fe_add_structure (AST_Structure *t)
{
  AST_Decl *predef = 0;
  AST_StructureFwd *fwd = 0;

  if ((predef = this->lookup_for_add (t, false)) != 0)
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
              if (fwd->defined_in () == this)
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
                           false,
                           t->local_name ());

  return t;
}

// Add this AST_StructureFwd node (a forward declaration of an IDL
// struct) to this scope.
AST_StructureFwd *
AST_Interface::fe_add_structure_fwd (AST_StructureFwd *t)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (t, false)) != 0)
    {
      AST_Decl::NodeType nt = d->node_type ();

      // There used to be another check here ANDed with the one below:
      // d->defined_in () == this. But lookup_for_add() calls only
      // lookup_by_name_local(), which does not bump up the scope.
      if (nt == AST_Decl::NT_struct_fwd)
        {
          // It's legal to forward declare something more than once,
          // but we need only one entry in the scope for lookup.
          AST_StructureFwd *fd = AST_StructureFwd::narrow_from_decl (d);
          t->destroy ();
          delete t;
          t = 0;
          return fd;
        }
      else if (nt == AST_Decl::NT_struct)
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
                           false,
                           t->local_name ());

  // Must check later that all struct and union forward declarations
  // are defined in the same IDL file.
  AST_record_fwd_decl (t);
  return t;
}

// Add an AST_Enum node (an enum declaration) to this scope.
AST_Enum *
AST_Interface::fe_add_enum (AST_Enum *t)
{
  AST_Decl *d = 0;

  // Can't add to interface which was not yet defined.
  if (!this->is_defined ())
    {
      idl_global->err ()->error2 (UTL_Error::EIDL_DECL_NOT_DEFINED,
                                  this,
                                  t);
      return 0;
    }

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (t, false)) != 0)
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
                           false,
                           t->local_name ());

  return t;
}

// Add an AST_Union (a union declaration) to this scope.
AST_Union *
AST_Interface::fe_add_union (AST_Union *t)
{
  AST_Decl *predef = 0;
  AST_UnionFwd *fwd = 0;

  if ((predef = this->lookup_for_add (t, false)) != 0)
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
              if (fwd->defined_in () == this)
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
                           false,
                           t->local_name ());

  return t;
}

// Add this AST_UnionFwd node (a forward declaration of an IDL
// union) to this scope.
AST_UnionFwd *
AST_Interface::fe_add_union_fwd (AST_UnionFwd *t)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (t, false)) != 0)
    {
      AST_Decl::NodeType nt = d->node_type ();

      // There used to be another check here ANDed with the one below:
      // d->defined_in () == this. But lookup_for_add() calls only
      // lookup_by_name_local(), which does not bump up the scope.
      if (nt == AST_Decl::NT_union_fwd)
        {
          // It's legal to forward declare something more than once,
          // but we need only one entry in the scope for lookup.
          AST_UnionFwd *fd = AST_UnionFwd::narrow_from_decl (d);
          t->destroy ();
          delete t;
          t = 0;
          return fd;
        }
      else if (nt == AST_Decl::NT_union)
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
                           false,
                           t->local_name ());

  // Must check later that all struct and union forward declarations
  // are defined in the same IDL file.
  AST_record_fwd_decl (t);
  return t;
}

// Add an AST_EnumVal node (an enumerator) to this scope.
// This is done to conform to the C++ scoping rules which declare
// enumerators in the enclosing scope (in addition to declaring them
// in the enum itself).
AST_EnumVal *
AST_Interface::fe_add_enum_val (AST_EnumVal *t)
{
  AST_Decl *d = 0;

  // Can't add to interface which was not yet defined.
  if (!this->is_defined ())
    {
      idl_global->err ()->error2 (UTL_Error::EIDL_DECL_NOT_DEFINED,
                                  this,
                                  t);
      return 0;
    }

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (t, false)) != 0)
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
                           false,
                           t->local_name ());

  return t;
}

// Add an AST_Typedef (a typedef) to the current scope.
AST_Typedef *
AST_Interface::fe_add_typedef (AST_Typedef *t)
{
  AST_Decl *d = 0;

  // Can't add to interface which was not yet defined.
  if (!this->is_defined ())
    {
      idl_global->err ()->error2 (UTL_Error::EIDL_DECL_NOT_DEFINED,
                                  this,
                                  t);
      return 0;
    }

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (t, false)) != 0)
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
                           false,
                           t->local_name ());

  AST_Type *bt = t->base_type ();
  UTL_ScopedName *mru = bt->last_referenced_as ();

  if (mru != 0)
    {
      this->add_to_referenced (
          bt,
          false,
          mru->first_component ()
        );
    }

  return t;
}

// Add an AST_Native (a native declaration) to this scope.
AST_Native *
AST_Interface::fe_add_native (AST_Native *t)
{
  AST_Decl *d = 0;

  // Can't add to interface which was not yet defined.
  if (!this->is_defined ())
    {
      idl_global->err ()->error2 (UTL_Error::EIDL_DECL_NOT_DEFINED,
                                  this,
                                  t);
      return 0;
    }

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (t, false)) != 0)
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
                           false,
                           t->local_name ());

  return t;
}

// Dump this AST_Interface node to the ostream o.
void
AST_Interface::dump (ACE_OSTREAM_TYPE &o)
{
  if (this->is_abstract ())
    {
      this->dump_i (o, "abstract ");
    }
  else if (this->is_local ())
    {
      this->dump_i (o, "local ");
    }

  this->dump_i (o, "interface ");

  this->local_name ()->dump (o);
  this->dump_i (o, " ");

  if (this->pd_n_inherits > 0)
    {
      this->dump_i (o, ": ");

      for (long i = 0; i < this->pd_n_inherits; ++i)
        {
          this->pd_inherits[i]->local_name ()->dump (o);

          if (i < this->pd_n_inherits - 1)
            {
              this->dump_i (o, ", ");
            }
        }
    }

  this->dump_i (o, " {\n");

  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);

  this->dump_i (o, "}");
}

// This serves for interfaces, valuetypes, components and eventtypes.
void
AST_Interface::fwd_redefinition_helper (AST_Interface *&i,
                                        UTL_Scope *s)
{
  if (i == 0)
    {
      return;
    }

  UTL_Scope *scope = i->defined_in ();
  const char *prefix_holder = 0;

  // If our prefix is empty, we check to see if an ancestor has one.
  while (ACE_OS::strcmp (i->prefix (), "") == 0 && scope != 0)
    {
      AST_Decl *parent = ScopeAsDecl (scope);
      prefix_holder = parent->prefix ();

      // We have reached global scope.
      if (prefix_holder == 0)
        {
          break;
        }

      i->prefix (const_cast<char *> (prefix_holder));
      scope = parent->defined_in ();
    }

  // Fwd redefinition should be in the same scope, so local
  // lookup is all that's needed.
  AST_Decl *d = s->lookup_by_name_local (i->local_name (),
                                         0);
                                         
  AST_Interface *fd = 0;

  if (d != 0)
    {
      scope = d->defined_in ();

      // If the lookup prefix is empty, we check to see if an ancestor has one.
      while (ACE_OS::strcmp (d->prefix (), "") == 0 && scope != 0)
        {
          AST_Decl *parent = ScopeAsDecl (scope);
          prefix_holder = parent->prefix ();

          // We have reached global scope.
          if (prefix_holder == 0)
            {
              break;
            }

          d->prefix (const_cast<char *> (prefix_holder));
          scope = parent->defined_in ();
        }

      // (JP) This could give a bogus error, since typeprefix can
      // appear any time after the corresponding declaration.
      // The right way to do this is with a separate traversal
      // after the entire AST is built.
      /*
      if (ACE_OS::strcmp (i->prefix (), d->prefix ()) != 0)
        {
          idl_global->err ()->error1 (UTL_Error::EIDL_PREFIX_CONFLICT,
                                      i);

          return;
        }
      */

      fd = AST_Interface::narrow_from_decl (d);

      // Successful?
      if (fd == 0)
        {
          AST_Decl::NodeType nt = d->node_type ();
          
          if (nt == AST_Decl::NT_struct_fwd || nt == AST_Decl::NT_union_fwd)
            {
              idl_global->err ()->redef_error (i->full_name (),
                                               d->full_name ());
            }
        }
      // If it is a forward declared interface..
      else if (!fd->is_defined ())
        {
          // Check if redefining in same scope. If a module is reopened,
          // a new pointer in created, and the first term below will be
          // true. In that case, the scoped names must be compared.
          if (fd->defined_in () != s
              && i->name ()->compare (fd->name ()) != 0)
            {
              idl_global->err ()->error2 (UTL_Error::EIDL_SCOPE_CONFLICT,
                                          i,
                                          fd);
            }
          // All OK, do the redefinition.
          else
            {
              AST_Decl::NodeType fd_nt = fd->node_type ();
              AST_Decl::NodeType i_nt = i->node_type ();

              // Only redefinition of the same kind.
              if (i->is_local () != fd->is_local ()
                  || i_nt != fd_nt
                  || i->is_abstract () != fd->is_abstract ()
                  )
                {
                  idl_global->err ()->error2 (UTL_Error::EIDL_REDEF,
                                              i,
                                              fd);
                  return;
                }

              fd->redefine (i);

              // Use full definition node.
              i->destroy ();
              delete i;
              i = fd;
            }
        }
    }
}

void
AST_Interface::redef_clash_populate_r (AST_Interface *t)
{
  if (this->insert_non_dup (t, 0) == 0)
    {
      return;
    }

  AST_Interface **parents = t->inherits ();
  long n_parents = t->n_inherits ();
  long i;

  for (i = 0; i < n_parents; ++i)
    {
      this->redef_clash_populate_r (parents[i]);
    }

  AST_Decl::NodeType nt = t->node_type ();

  if (nt == AST_Decl::NT_valuetype || nt == AST_Decl::NT_eventtype)
    {
      AST_ValueType *v = AST_ValueType::narrow_from_decl (t);
      AST_Interface **supports = v->supports ();
      long n_supports = v->n_supports ();

      for (i = 0; i < n_supports; ++i)
        {
          this->redef_clash_populate_r (supports[i]);
        }
    }
  else if (nt == AST_Decl::NT_component)
    {
      AST_Component *c = AST_Component::narrow_from_decl (t);
      AST_Interface **supports = c->supports ();
      long n_supports = c->n_supports ();

      for (i = 0; i < n_supports; ++i)
        {
          this->redef_clash_populate_r (supports[i]);
        }
    }
}

bool
AST_Interface::home_equiv (void) const
{
  return this->home_equiv_;
}

void
AST_Interface::home_equiv (bool val)
{
  this->home_equiv_ = val;
}

AST_InterfaceFwd *
AST_Interface::fwd_decl (void) const
{
  return this->fwd_decl_;
}

void
AST_Interface::fwd_decl (AST_InterfaceFwd *node)
{
  this->fwd_decl_ = node;
}

int
AST_Interface::insert_non_dup (AST_Interface *t,
                               bool abstract_paths_only)
{
  // Now check if the dequeued element has any ancestors. If yes, insert
  // them inside the queue making sure that there are no duplicates.
  // If we are doing a component, the inheritance list is actually a
  // supports list.
  for (long i = 0; i < t->n_inherits (); ++i)
    {
      // Retrieve the next parent from which the dequeued element inherits.
      AST_Interface *parent = t->inherits ()[i];

      if (abstract_paths_only && ! parent->is_abstract ())
        {
          continue;
        }

      (void) this->insert_non_dup (parent, abstract_paths_only);
    } // end of for loop

  const char *full_name = t->full_name ();

  // Initialize an iterator to search the queue for duplicates.
  for (ACE_Unbounded_Queue_Iterator<AST_Interface *> q_iter (
           this->insert_queue
         );
       !q_iter.done ();
       (void) q_iter.advance ())
    {
      // Queue element.
      AST_Interface **temp = 0;

      (void) q_iter.next (temp);

      if (!ACE_OS::strcmp (full_name,
                           (*temp)->full_name ()))
        {
          // We exist in this queue and cannot be inserted.
          return 0;
        }
    }

  // Initialize an iterator to search the del_queue for duplicates.
  for (ACE_Unbounded_Queue_Iterator<AST_Interface *> del_q_iter (
           this->del_queue
         );
       !del_q_iter.done ();
       (void) del_q_iter.advance ())
    {
      // Queue element.
      AST_Interface **temp = 0;

      (void) del_q_iter.next (temp);

      if (!ACE_OS::strcmp (full_name,
                           (*temp)->full_name ()))
        {
          // We exist in this del_queue and cannot be inserted.
          return 0;
        }
    }

  // Insert the parent in the queue.
  if (this->insert_queue.enqueue_tail (t) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        "(%N:%l) be_interface::insert_non_dup - "
                        "enqueue failed\n"),
                        0);
    }

  return 1;
}

// This serves only for interfaces. It is overridden for valuetypes,
// components and eventtypes.
void
AST_Interface::redefine (AST_Interface *from)
{
  // 'this' is the full_definition member of a forward
  // declared interface. 'from' is the actual full
  // definition, which may be in a different scope.
  // Since 'this' will replace 'from' upon returning
  // from here, we have to update the scope now.
  this->pd_n_inherits = from->pd_n_inherits;
  unsigned long i = 0;

  unsigned long array_size =
    static_cast<unsigned long> (from->pd_n_inherits);
  ACE_NEW (this->pd_inherits,
           AST_Interface *[array_size]);

  for (i = 0; i < array_size; ++i)
    {
      this->pd_inherits[i] = from->pd_inherits[i];
    }

  this->pd_n_inherits_flat = from->pd_n_inherits_flat;
  array_size =
    static_cast<unsigned long> (from->pd_n_inherits_flat);
  ACE_NEW (this->pd_inherits_flat,
           AST_Interface *[array_size]);

  for (i = 0; i < array_size; ++i)
    {
      this->pd_inherits_flat[i] = from->pd_inherits_flat[i];
    }

  // We've already checked for inconsistent prefixes.
  this->prefix (from->prefix ());

  this->set_defined_in (from->defined_in ());
  this->set_imported (idl_global->imported ());
  this->set_in_main_file (idl_global->in_main_file ());
  this->set_line (idl_global->lineno ());
  this->set_file_name (idl_global->filename ()->get_string ());
  this->ifr_added_ = from->ifr_added_;
  this->ifr_fwd_added_ = from->ifr_fwd_added_;
  this->fwd_decl_->set_as_defined ();
}

// Data accessors.

AST_Interface **
AST_Interface::inherits (void) const
{
  return this->pd_inherits;
}

long
AST_Interface::n_inherits (void) const
{
  return this->pd_n_inherits;
}

AST_Interface **
AST_Interface::inherits_flat (void) const
{
  return this->pd_inherits_flat;
}

long
AST_Interface::n_inherits_flat (void) const
{
  return pd_n_inherits_flat;
}

ACE_Unbounded_Queue<AST_Interface *> &
AST_Interface::get_insert_queue (void)
{
  return this->insert_queue;
}

ACE_Unbounded_Queue<AST_Interface *> &
AST_Interface::get_del_queue (void)
{
  return this->del_queue;
}

bool
AST_Interface::redef_clash (void)
{
  this->insert_queue.reset ();
  this->redef_clash_populate_r (this);

  AST_Interface **group1_member = 0;
  AST_Interface **group2_member = 0;
  AST_Decl *group1_member_item = 0;
  AST_Decl *group2_member_item = 0;

  int i = 1;

  // Now compare all pairs.
  for (ACE_Unbounded_Queue_Iterator<AST_Interface *> group1_iter (
           this->insert_queue
         );
       !group1_iter.done ();
       (void) group1_iter.advance (), ++i)
    {
      // Queue element.
      (void) group1_iter.next (group1_member);

      for (UTL_ScopeActiveIterator group1_member_items (
               DeclAsScope (*group1_member),
               UTL_Scope::IK_decls
             );
           !group1_member_items.is_done ();
           group1_member_items.next ())
        {
          group1_member_item = group1_member_items.item ();
          AST_Decl::NodeType nt1 = group1_member_item->node_type ();

          // Only these member types may cause a clash because
          // they can't be redefined.
          if (nt1 != AST_Decl::NT_op && nt1 != AST_Decl::NT_attr)
            {
              continue;
            }

          Identifier *pid1 = group1_member_item->local_name ();
          int j = 0;

          for (ACE_Unbounded_Queue_Iterator<AST_Interface *> group2_iter (
                   this->insert_queue
                 );
               !group2_iter.done ();
               (void) group2_iter.advance ())
            {
              // Since group1 and group2 are the same list, we can start this
              // iterator from where the outer one is.
              while (j++ < i)
                {
                  group2_iter.advance ();
                }

              if (group2_iter.done ())
                {
                  break;
                }

              // Queue element.
              (void) group2_iter.next (group2_member);

              for (UTL_ScopeActiveIterator group2_member_items (
                       DeclAsScope (*group2_member),
                       UTL_Scope::IK_decls
                     );
                   !group2_member_items.is_done ();
                   group2_member_items.next ())
                {
                  group2_member_item = group2_member_items.item ();
                  AST_Decl::NodeType nt2 = group2_member_item->node_type ();

                  // Only these member types may cause a clash
                  // with other parents' member of the same type.
                  if (nt2 != AST_Decl::NT_op && nt2 != AST_Decl::NT_attr)
                    {
                      continue;
                    }

                  Identifier *pid2 = group2_member_item->local_name ();

                  if (pid1->compare (pid2) == true)
                    {
                      idl_global->err ()->error3 (
                                              UTL_Error::EIDL_REDEF,
                                              *group1_member,
                                              *group2_member,
                                              group2_member_item
                                            );
                      return true;
                    }
                  else if (pid1->case_compare_quiet (pid2) == true)
                    {
                      if (idl_global->case_diff_error ())
                        {
                          idl_global->err ()->error3 (
                              UTL_Error::EIDL_NAME_CASE_ERROR,
                              *group1_member,
                              group1_member_item,
                              group2_member_item
                            );
                          return true;
                        }
                      else
                        {
                          idl_global->err ()->warning3 (
                              UTL_Error::EIDL_NAME_CASE_WARNING,
                              *group1_member,
                              group1_member_item,
                              group2_member_item
                            );
                        }
                    }
                } // end of FOR (group2_member_items)
            } // end of FOR (group2_iter)
        } // end of FOR (group1_member_items)
    } // end of FOR (group1_iter)

  return false;
}

// Look through inherited interfaces.
AST_Decl *
AST_Interface::look_in_inherited (UTL_ScopedName *e,
                                  bool treat_as_ref)
{
  AST_Decl *d = 0;
  AST_Decl *d_before = 0;
  AST_Interface **is = 0;
  long nis = -1;

  // Can't look in an interface which was not yet defined.
  if (!this->is_defined ())
    {
      return 0;
    }

  // OK, loop through inherited interfaces.

  // (Don't leave the inheritance hierarchy, no module or global ...)
  // Find all and report ambiguous results as error.

  for (nis = this->n_inherits (), is = this->inherits ();
       nis > 0;
       nis--, is++)
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
                              "warning in %C line %d: ",
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

AST_Decl *
AST_Interface::lookup_for_add (AST_Decl *d,
                               bool /* treat_as_ref */)
{
  if (d == 0)
    {
      return 0;
    }

  Identifier *id = d->local_name ();
  AST_Decl *prev = 0;
  AST_Decl::NodeType nt = NT_root;
  long nis = -1;
  AST_Interface **is = 0;

  if (this->idl_keyword_clash (id) != 0)
    {
      return 0;
    }

  prev = this->lookup_by_name_local (id,
                                     0);

  if (prev != 0)
    {
      nt = prev->node_type ();

      if (nt == AST_Decl::NT_op || nt == AST_Decl::NT_attr)
        {
          return prev;
        }
    }

  for (nis = this->n_inherits_flat (), is = this->inherits_flat ();
       nis > 0;
       nis--, is++)
    {
      prev = (*is)->lookup_by_name_local (id,
                                          0);

      if (prev != 0)
        {
          nt = prev->node_type ();

          if (nt == AST_Decl::NT_op || nt == AST_Decl::NT_attr)
            {
              return prev;
            }
        }
    }

  return 0;
}

bool
AST_Interface::legal_for_primary_key (void) const
{
  return false;
}

void
AST_Interface::destroy (void)
{
  delete [] this->pd_inherits;
  this->pd_inherits = 0;
  this->pd_n_inherits = 0;

  delete [] this->pd_inherits_flat;
  this->pd_inherits_flat = 0;
  this->pd_n_inherits_flat = 0;

  this->UTL_Scope::destroy ();
  this->AST_Type::destroy ();
}

int
AST_Interface::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_interface (this);
}

IMPL_NARROW_FROM_DECL(AST_Interface)
IMPL_NARROW_FROM_SCOPE(AST_Interface)
