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

// AST_Structure nodes denote IDL struct declarations.
// AST_Structure is a subclass of AST_ConcreteType and of UTL_Scope (the
// structure's fields are managed in a scope).

#include "ast_union.h"
#include "ast_structure_fwd.h"
#include "ast_field.h"
#include "ast_enum.h"
#include "ast_enum_val.h"
#include "ast_typedef.h"
#include "ast_visitor.h"

#include "utl_string.h"
#include "utl_err.h"
#include "utl_indenter.h"

AST_Structure::AST_Structure (UTL_ScopedName *n,
                              bool local,
                              bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (AST_Decl::NT_struct,
              n),
    AST_Type (AST_Decl::NT_struct,
              n),
    AST_ConcreteType (AST_Decl::NT_struct,
                      n),
    UTL_Scope (AST_Decl::NT_struct),
    member_count_ (-1),
    local_struct_ (-1),
    fwd_decl_ (0)
{
}

AST_Structure::AST_Structure (AST_Decl::NodeType nt,
                              UTL_ScopedName *n,
                              bool local,
                              bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (nt,
              n),
    AST_Type (nt,
              n),
    AST_ConcreteType (nt,
                      n),
    UTL_Scope (nt),
    member_count_ (-1),
    local_struct_ (-1),
    fwd_decl_ (0)
{
}

AST_Structure::~AST_Structure (void)
{
}

// Are we or the parameter node involved in any recursion?
bool
AST_Structure::in_recursion (ACE_Unbounded_Queue<AST_Type *> &list)
{
  bool self_test = (list.size () == 0);

  // We should calculate this only once. If it has already been
  // done, just return it.
  if (self_test && this->in_recursion_ != -1)
    {
      return (this->in_recursion_ == 1);
    }

  if (list.size () > 1)
  {
    if (match_names (this, list))
      {
        // We've found ourselves outside of a sequence.
        // This happens when we are not recursed ourselves but instead
        // are part of another recursed type which is part of us.
        // f.i. union containing sequence of struct containing the union as member.
        return false;
      }
  }

  list.enqueue_tail(this);

  // Proceed if the number of members in our scope is greater than 0.
  if (this->nmembers () > 0)
    {
      // Initialize an iterator to iterate over our scope.
      // Continue until each element is visited.
      for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          AST_Field *field = AST_Field::narrow_from_decl (si.item ());

          if (field == 0)
            // This will be an enum value or other legitimate non-field
            // member - in any case, no recursion.
            {
              continue;
            }

          AST_Type *type = field->field_type ();

          if (type->node_type () == AST_Decl::NT_typedef)
            {
              AST_Typedef *td = AST_Typedef::narrow_from_decl (type);
              type = td->primitive_base_type ();
            }

          if (type == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("(%N:%l) AST_Structure::")
                                 ACE_TEXT ("in_recursion - ")
                                 ACE_TEXT ("bad field type\n")),
                                0);
            }

          if (type->in_recursion (list))
            {
              if (self_test)
                this->in_recursion_ = 1;
              idl_global->recursive_type_seen_ = true;
              return true;
            }
        }
    }

  // Not in recursion.
  if (self_test)
    this->in_recursion_ = 0;
  return false;
}

int
AST_Structure::member_count (void)
{
  if (this->member_count_ == -1)
    {
      this->compute_member_count ();
    }

  return this->member_count_;
}

ACE_CDR::ULong
AST_Structure::nfields (void) const
{
  return this->fields_.size ();
}

int
AST_Structure::field (AST_Field **&result,
                      ACE_CDR::ULong slot) const
{
  return this->fields_.get (result, slot);
}

bool
AST_Structure::is_local (void)
{
  if (this->local_struct_ == -1)
    {
      if (this->is_local_)
        {
          this->local_struct_ = this->is_local_;
        }
      else
        {
          this->local_struct_ = 0;

          if (this->nmembers () > 0)
            {
              // Instantiate a scope iterator.
              for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
                   !si.is_done ();
                   si.next ())
                {
                  if (si.item ()->is_local ())
                    {
                      this->local_struct_ = true;
                      break;
                    }
                }
            }
        }
    }

  return this->local_struct_;
}

int
AST_Structure::contains_wstring (void)
{
  if (this->contains_wstring_ == -1)
    {
      for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          if (si.item ()->contains_wstring () == 1)
            {
              this->contains_wstring_ = 1;
              return this->contains_wstring_;
            }
        }

      this->contains_wstring_ = 0;
    }

  return this->contains_wstring_;
}

bool
AST_Structure::is_defined (void)
{
  return 0 == this->fwd_decl_ || this->fwd_decl_->is_defined ();
}

bool
AST_Structure::legal_for_primary_key (void) const
{
  bool retval = true;

  if (!this->recursing_in_legal_pk_)
    {
      this->recursing_in_legal_pk_ = true;

      for (UTL_ScopeActiveIterator si (const_cast<AST_Structure *> (this),
                                      UTL_Scope::IK_decls);
          !si.is_done ();
          si.next ())
        {
          AST_Field *f = AST_Field::narrow_from_decl (si.item ());

          if (f != 0 && !f->field_type ()->legal_for_primary_key ())
            {
              retval = false;
              break;
            }
        }

      this->recursing_in_legal_pk_ = false;
    }

  return retval;
}

AST_StructureFwd *
AST_Structure::fwd_decl (void) const
{
  return this->fwd_decl_;
}

void
AST_Structure::fwd_decl (AST_StructureFwd *node)
{
  this->fwd_decl_ = node;
}

ACE_Unbounded_Queue<AST_Field *> &
AST_Structure::fields (void)
{
  return this->fields_;
}

// Private operations.

AST_Field *
AST_Structure::fe_add_field (AST_Field *t)
{
  return this->fe_add_ref_decl (t);
}

AST_Structure *
AST_Structure::fe_add_structure (AST_Structure *t)
{
  return this->fe_add_full_struct_type (t);
}

AST_Union *
AST_Structure::fe_add_union (AST_Union *t)
{
  return
    AST_Union::narrow_from_decl (
      this->fe_add_full_struct_type (t));
}

AST_Enum *
AST_Structure::fe_add_enum (AST_Enum *t)
{
  return
    AST_Enum::narrow_from_decl (
      this->fe_add_decl (t));
}

// Add this AST_EnumVal node (an enumerator declaration) to this scope.
// This is done to conform to the C++ scoping rules which declare
// enumerators in the enclosing scope (in addition to declaring them
// in the enum itself).
AST_EnumVal *
AST_Structure::fe_add_enum_val (AST_EnumVal *t)
{
  return
    AST_EnumVal::narrow_from_decl (
      this->fe_add_decl (t));
}

// Compute total number of members.
int
AST_Structure::compute_member_count (void)
{
  this->member_count_ = 0;

  // If there are elements in this scope.
  if (this->nmembers () > 0)
    {
      // Instantiate a scope iterator.
      for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          ++this->member_count_;
        }
    }

  return 0;
}

// Dump this AST_Structure node to the ostream o.
void
AST_Structure::dump (ACE_OSTREAM_TYPE &o)
{
  if (this->is_local ())
    {
      this->dump_i (o, "(local) ");
    }

  this->dump_i (o, "struct ");
  AST_Decl::dump (o);
  this->dump_i (o, " {\n");
  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);
  this->dump_i (o, "}");
}

// This serves for structs and unions.
void
AST_Structure::fwd_redefinition_helper (AST_Structure *&i,
                                        UTL_Scope *s)
{
  if (i == 0)
    {
      return;
    }

  // Fwd redefinition should be in the same scope, so local
  // lookup is all that's needed.
  AST_Decl *d =
    s->lookup_by_name_local (i->local_name (), false);

  AST_Structure *fd = 0;

  if (d != 0)
    {
      // Full definition must have the same prefix as the forward declaration.
      if (ACE_OS::strcmp (i->prefix (), d->prefix ()) != 0)
        {
          idl_global->err ()->error1 (UTL_Error::EIDL_PREFIX_CONFLICT,
                                      i);

          return;
        }

      AST_Decl::NodeType nt = d->node_type ();

      // If this interface has been forward declared in a previous opening
      // of the module it's defined in, the lookup will find the
      // forward declaration.
      if (nt == AST_Decl::NT_struct_fwd
          || nt == AST_Decl::NT_union_fwd)
        {
          AST_StructureFwd *fwd_def =
            AST_StructureFwd::narrow_from_decl (d);

          fd = fwd_def->full_definition ();
        }
      // In all other cases, the lookup will find an interface node.
      else if (nt == AST_Decl::NT_struct
               || nt == AST_Decl::NT_union)
        {
          fd = AST_Structure::narrow_from_decl (d);
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
              if (i_nt != fd_nt)
                {
                  idl_global->err ()->error2 (UTL_Error::EIDL_REDEF,
                                              i,
                                              fd);
                  return;
                }

              fd->redefine (i);
              AST_StructureFwd *fwd = fd->fwd_decl ();

              if (0 != fwd)
                {
                  // So the fwd decl won't destroy us at cleanup time.
                  // Unlike interfaces, valuetypes and components, it's
                  // ok to do this here, since fwd declared structs
                  // and unions must be defined in the same translation
                  // unit.
                  fwd->set_as_defined ();
                }

              // Use full definition node.
              i->destroy ();
              delete i;
              i = fd;
            }
        }
    }
}

// This serves only for structs. It is overridden for unions.
void
AST_Structure::redefine (AST_Structure *from)
{
  // We've already checked for inconsistent prefixes.
  this->prefix (from->prefix ());

  this->set_defined_in (from->defined_in ());
  this->set_imported (idl_global->imported ());
  this->set_in_main_file (idl_global->in_main_file ());
  this->set_line (idl_global->lineno ());
  this->set_file_name (idl_global->filename ()->get_string ());
  this->ifr_added_ = from->ifr_added_;
  this->ifr_fwd_added_ = from->ifr_fwd_added_;
  this->fields_ = from->fields_;
  this->member_count_ = from->member_count_;
  this->local_struct_ = from->local_struct_;
}

// Compute the size type of the node in question.
int
AST_Structure::compute_size_type (void)
{
  for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      // Get the next AST decl node.
      AST_Decl *d = si.item ();

      if (d->node_type () == AST_Decl::NT_enum_val)
        {
          continue;
        }

      AST_Field *f = AST_Field::narrow_from_decl (d);
      AST_Type *t = f->field_type ();

      if (t != 0)
        {
          this->size_type (t->size_type ());

          // While we're iterating, we might as well do this one too.
          this->has_constructor (t->has_constructor ());
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      "WARNING (%N:%l) be_structure::compute_size_type - "
                      "narrow_from_decl returned 0\n"));
        }
    }

  return 0;
}

int
AST_Structure::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_structure (this);
}

void
AST_Structure::destroy (void)
{
  this->AST_ConcreteType::destroy ();
  this->UTL_Scope::destroy ();
}

IMPL_NARROW_FROM_DECL(AST_Structure)
IMPL_NARROW_FROM_SCOPE(AST_Structure)

