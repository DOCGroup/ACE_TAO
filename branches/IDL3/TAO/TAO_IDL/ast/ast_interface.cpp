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

#include "idl.h"
#include "idl_extern.h"

ACE_RCSID(ast, ast_interface, "$Id$")

// Constructor(s) and destructor.

AST_Interface::AST_Interface (void)
  : is_valuetype_ (0),
    pd_inherits (0),
    pd_n_inherits (0),
    pd_inherits_flat (0),
    pd_n_inherits_flat (0)
{
}

AST_Interface::AST_Interface (UTL_ScopedName *n,
                              AST_Interface **ih,
                              long nih,
                              AST_Interface **ih_flat,
                              long nih_flat,
                              idl_bool local,
                              idl_bool abstract)
  : AST_Type (AST_Decl::NT_interface,
              n),
    AST_Decl (AST_Decl::NT_interface,
              n),
    UTL_Scope (AST_Decl::NT_interface),
    COMMON_Base (local,
                 abstract),
    is_valuetype_ (0),
    pd_inherits (ih),
    pd_n_inherits (nih),
    pd_inherits_flat (ih_flat),
    pd_n_inherits_flat (nih_flat)
{
}

AST_Interface::~AST_Interface (void)
{
}

// Public operations.

idl_bool
AST_Interface::is_valuetype (void)
{
  return this->is_valuetype_;
}

void
AST_Interface::set_valuetype (void)
{
  this->is_valuetype_ = 1;
}

idl_bool
AST_Interface::is_abstract_valuetype (void)
{
  return this->is_valuetype_ && this->is_abstract_;
}

void
AST_Interface::set_abstract_valuetype (void)
{
  this->is_valuetype_ = 1;
  this->is_abstract_ = 1;
}

void
AST_Interface::be_replace_operation (AST_Decl *old_op,
                                     AST_Decl *new_op)
{
  replace_scope (old_op,
                 new_op);

  replace_referenced (old_op,
                      new_op);
}

void
AST_Interface::be_add_operation (AST_Operation *op)
{
  fe_add_operation (op);
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
                           I_FALSE,
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

// Add an AST_Structure (a struct declaration) to this scope.
AST_Structure *
AST_Interface::fe_add_structure (AST_Structure *t)
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

// Add an AST_Union (a union declaration) to this scope.
AST_Union *
AST_Interface::fe_add_union (AST_Union *t)
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

AST_Factory *
AST_Interface::fe_add_factory (AST_Factory *f)
{
  AST_Decl *d = 0;

  // Can't add to interface which was not yet defined.
  if (!this->is_defined ())
    {
      idl_global->err ()->error2 (UTL_Error::EIDL_DECL_NOT_DEFINED,
                                  this,
                                  f);
      return 0;
    }

  // Already defined and cannot be redefined? Or already used?
  if ((d = this->lookup_for_add (f, I_FALSE)) != 0)
    {
      if (!can_be_redefined (d))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_REDEF,
                                      f,
                                      this,
                                      d);
          return 0;
        }

      if (this->referenced (d, f->local_name ()))
        {
          idl_global->err ()->error3 (UTL_Error::EIDL_DEF_USE,
                                      f,
                                      this,
                                      d);
          return 0;
        }

      if (f->has_ancestor (d))
        {
          idl_global->err ()->redefinition_in_scope (f,
                                                     d);
          return 0;
        }
    }

  // Add it to scope.
  this->add_to_scope (f);

  // Add it to set of locally referenced symbols.
  this->add_to_referenced (f,
                           I_FALSE,
                           f->local_name ());

  return f;
}

// Dump this AST_Interface node to the ostream o.
void
AST_Interface::dump (ACE_OSTREAM_TYPE &o)
{
  if (this->is_valuetype ())
    {
      if (this->is_abstract_valuetype ())
        {
          o << "abstract ";
        }

      o << "valuetype ";
    }
  else
    {
      if (this->is_abstract ())
        {
          o << "abstract ";
        }
      else if (this->is_local ())
        {
          o << "local ";
        }

      o << "interface ";
    }

  this->local_name ()->dump (o);
  o << " ";

  if (this->pd_n_inherits > 0)
    {
      o << ": ";

      for (long i = 0; i < this->pd_n_inherits; i++)
        {
          this->pd_inherits[i]->local_name ()->dump (o);

          if (i < this->pd_n_inherits - 1)
            {
              o << ", ";
            }
        }
    }

  o << " {\n";
  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);
  o << "}";
}

void
AST_Interface::fwd_redefinition_helper (AST_Interface *&i,
                                        UTL_Scope *s)
{
  if (i == 0)
    {
      return;
    }

  // Fwd redefinition should be in the same scope, so local
  // lookup is all that's needed.
  AST_Decl *d = s->lookup_by_name_local (i->local_name (),
                                         0);

  AST_Interface *fd = 0;

  if (d != 0)
    {
      // Full definition must have the same prefix as the forward declaration.
      if (ACE_OS::strcmp (i->prefix (), d->prefix ()) != 0)
        {
          idl_global->err ()->error1 (UTL_Error::EIDL_PREFIX_CONFLICT,
                                      i);

          return;
        }

      // If this interface has been forward declared in a previous opening
      // of the module it's defined in, the lookup will find the
      // forward declaration.
      if (d->node_type () == AST_Decl::NT_interface_fwd)
        {
          AST_InterfaceFwd *fwd_def =
            AST_InterfaceFwd::narrow_from_decl (d);

          fd = fwd_def->full_definition ();
        }
      // In all other cases, the lookup will find an interface node.
      else if (d->node_type () == AST_Decl::NT_interface)
        {
          fd = AST_Interface::narrow_from_decl (d);
        }

      // Successful?
      if (fd == 0)
        {
          // Should we give an error here?
          // No, look in fe_add_interface.
        }
      // If it is a forward declared interface..
      else if (!fd->is_defined ())
        {
          // Check if redefining in same scope. If a module is reopened,
          // a new pointer in created, and the first term below will be
          // true. In that case, the scoped names must be compared.
          if (fd->defined_in () != s
              && !AST_Interface::compare_names (fd,
                                                i))
            {
              idl_global->err ()->error2 (UTL_Error::EIDL_SCOPE_CONFLICT,
                                          i,
                                          fd);
            }
          // All OK, do the redefinition.
          else
            {
              // Only redefinition of the same kind.
              if (i->is_local () != fd->is_local ()
                  || i->is_valuetype () != fd->is_valuetype ()
                  || i->is_abstract_valuetype () !=
                       fd->is_abstract_valuetype ()
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
              delete i;
              i = fd;
            }
        }
    }
}

// Data accessors.

void
AST_Interface::redefine (AST_Interface *from)
{
  // 'this' is the full_definition member of a forward
  // declared interface. 'from' is the actual full
  // definition, which may be in a different scope.
  // Since 'this' will replace 'from' upon returning
  // from here, we have to update the scope now.
  this->set_inherits (from->inherits ());
  this->set_n_inherits (from->n_inherits ());
  this->set_inherits_flat (from->inherits_flat ());
  this->set_n_inherits_flat (from->n_inherits_flat ());

  // We've already checked for inconsistent prefixes.
  this->prefix (ACE::strnew (from->prefix ()));

  this->set_defined_in (from->defined_in ());
  this->set_imported (idl_global->imported ());
  this->set_in_main_file (idl_global->in_main_file ());
  this->set_line (idl_global->lineno ());
  this->set_file_name (idl_global->filename ());
  this->ifr_added_ = from->ifr_added_;
  this->ifr_fwd_added_ = from->ifr_fwd_added_;
}

AST_Interface **
AST_Interface::inherits (void)
{
  return this->pd_inherits;
}

void
AST_Interface::set_inherits (AST_Interface **i)
{
  this->pd_inherits = i;
}

long
AST_Interface::n_inherits (void)
{
  return this->pd_n_inherits;
}

void
AST_Interface::set_n_inherits (long i)
{
  this->pd_n_inherits = i;
}

AST_Interface **
AST_Interface::inherits_flat (void)
{
  return this->pd_inherits_flat;
}

void
AST_Interface::set_inherits_flat (AST_Interface **i)
{
  this->pd_inherits_flat = i;
}

long
AST_Interface::n_inherits_flat (void)
{
  return pd_n_inherits_flat;
}

void
AST_Interface::set_n_inherits_flat (long i)
{
  this->pd_n_inherits_flat = i;
}

// Get the scoped names and, if they are the same
// length, iterate over them, comparing each term.
idl_bool
AST_Interface::compare_names (AST_Interface *that,
                              AST_Interface *other)
{
  UTL_ScopedName *that_name = that->name ();
  UTL_ScopedName *other_name = other->name ();

  long that_length = that_name->length ();

  if (that_length != other_name->length ())
    {
      return I_FALSE;
    }

  Identifier *that_id = 0;
  Identifier *other_id = 0;

  for (UTL_IdListActiveIterator that_iter (that_name), other_iter (other_name);
       !that_iter.is_done ();
       that_iter.next (), other_iter.next ())
    {
      that_id = that_iter.item ();
      other_id = other_iter.item ();

      if (ACE_OS::strcmp (that_id->get_string (),
                          other_id->get_string ()))
        {
          return I_FALSE;
        }
    }

  return I_TRUE;
}

void
AST_Interface::inherited_name_clash (void)
{
  AST_Decl *my_member = 0;
  AST_Decl *parent1_member = 0;
  AST_Decl  *parent2_member = 0;

  // Compare our members with those of each parent.

  for (UTL_ScopeActiveIterator my_members (DeclAsScope (this), IK_decls);
       !my_members.is_done ();
       my_members.next ())
    {
      my_member = my_members.item ();
      Identifier *id = my_member->local_name ();

      for (int i = 0; i < this->pd_n_inherits_flat; ++i)
        {
          for (UTL_ScopeActiveIterator parent_members (
                   DeclAsScope (this->pd_inherits_flat[i]), 
                   UTL_Scope::IK_decls
                 );
               !parent_members.is_done ();
               parent_members.next ())
            {
              parent1_member = parent_members.item ();
              AST_Decl::NodeType nt = parent1_member->node_type ();

              // All other member types but these may be redefined in
              // the child.
              if (nt == AST_Decl::NT_op || nt == AST_Decl::NT_attr)
                {
                  Identifier *pid = parent1_member->local_name ();

                  if (id->compare (pid) == I_TRUE)
                    {
                      idl_global->err ()->error2 (UTL_Error::EIDL_REDEF,
                                                  my_member,
                                                  parent1_member);
                    }
                  else if (id->case_compare_quiet (pid) == I_TRUE)
                    {
                      if (idl_global->case_diff_error ())
                        {
                          idl_global->err ()->error2 (
                              UTL_Error::EIDL_NAME_CASE_ERROR,
                              my_member,
                              parent1_member
                            );
                        }
                      else
                        {
                          idl_global->err ()->warning2 (
                              UTL_Error::EIDL_NAME_CASE_WARNING,
                              my_member,
                              parent1_member
                            );
                        }
                    }
                } // end of IF (nt == AST_Decl::NT_op ....)
            } // end of FOR (parent_members ...)
        } // end of FOR (i ...)
    } // end of FOR (my_members ...)

  // Now compare members of each parent with each other.

  for (int i = 0; i < this->pd_n_inherits_flat - 1; i++)
    {
      for (UTL_ScopeActiveIterator parent1_members (
               DeclAsScope (this->pd_inherits_flat[i]),
               UTL_Scope::IK_decls
             );
           !parent1_members.is_done ();
           parent1_members.next ())
        {
          parent1_member = parent1_members.item ();
          AST_Decl::NodeType nt1 = parent1_member->node_type ();

          // Only these member types may cause a clash with other
          // parents' member of the same type.
          if (nt1 == AST_Decl::NT_op || nt1 == AST_Decl::NT_attr)
            {
              Identifier *pid1 = parent1_member->local_name ();

              for (int j = i + 1; j < this->pd_n_inherits_flat; j++)
                {
                  for (UTL_ScopeActiveIterator parent2_members (
                           DeclAsScope (this->pd_inherits_flat[j]),
                           UTL_Scope::IK_decls
                         );
                       !parent2_members.is_done ();
                       parent2_members.next ())
                    {
                      parent2_member = parent2_members.item ();
                      AST_Decl::NodeType nt2 =
                        parent2_member->node_type ();

                      // Only these member types may cause a clash
                      // with other parents' member of the same type.
                      if (nt2 == AST_Decl::NT_op || nt2 == AST_Decl::NT_attr)
                        {
                          Identifier *pid2 = parent2_member->local_name ();

                          if (pid1->compare (pid2) == I_TRUE)
                            {
                              idl_global->err ()->error3 (
                                                      UTL_Error::EIDL_REDEF,
                                                      this,
                                                      parent1_member,
                                                      parent2_member
                                                    );
                            }
                          else if (pid1->case_compare_quiet (pid2) == I_TRUE)
                            {
                              if (idl_global->case_diff_error ())
                                {
                                  idl_global->err ()->error3 (
                                      UTL_Error::EIDL_NAME_CASE_ERROR,
                                      this,
                                      parent1_member,
                                      parent2_member
                                    );
                                }
                              else
                                {
                                  idl_global->err ()->warning3 (
                                      UTL_Error::EIDL_NAME_CASE_WARNING,
                                      this,
                                      parent1_member,
                                      parent2_member
                                    );
                                }
                            }
                        } // end of IF (nt2 == AST_Decl::NT_op ...)
                    } // end of FOR (parent2_members ...)
                } // end of FOR (j ...)
            } // end of IF (nt1 == AST_Decl::NT_op ..)
        } // end of FOR (parent1_members ...)
    } // end of FOR (i ...)
}

void
AST_Interface::destroy (void)
{
  delete [] this->pd_inherits;
  this->pd_inherits = 0;
  delete [] this->pd_inherits_flat;
  this->pd_inherits_flat = 0;
}

int
AST_Interface::ast_accept (ast_visitor *visitor)
{
  if (this->is_valuetype_)
    {
      return visitor->visit_valuetype (this);
    }
  else
    {
      return visitor->visit_interface (this);
    }
}

// Narrowing methods.
IMPL_NARROW_METHODS2(AST_Interface, AST_Type, UTL_Scope)
IMPL_NARROW_FROM_DECL(AST_Interface)
IMPL_NARROW_FROM_SCOPE(AST_Interface)
