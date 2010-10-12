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
#include "ast_template_module.h"
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
#include "ast_typedef.h"
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

AST_Decl::NodeType const
AST_Interface::NT = AST_Decl::NT_interface;

AST_Interface::AST_Interface (UTL_ScopedName *n,
                              AST_Type **ih,
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
    fwd_decl_ (0),
    has_mixed_parentage_ (-1),
    ami_handler_ (0),
    ami4ccm_uses_ (0)
{
  this->size_type (AST_Type::VARIABLE); // always the case
  this->has_constructor (true);      // always the case

  // Enqueue the param holders (if any) for later destruction.
  // By the time our destroy() is called, it will be too late
  // to iterate over pd_inherits.
  for (long i = 0; i < nih; ++i)
    {
      if (ih[i]->node_type () == AST_Decl::NT_param_holder)
        {
          this->param_holders_.enqueue_tail (ih[i]);
        }
    }
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

AST_Constant *
AST_Interface::fe_add_constant (AST_Constant *t)
{
  return
    AST_Constant::narrow_from_decl (
      this->fe_add_decl (t));
}

AST_Exception *
AST_Interface::fe_add_exception (AST_Exception *t)
{
  return
    AST_Exception::narrow_from_decl (
      this->fe_add_decl (t));
}

AST_Attribute *
AST_Interface::fe_add_attribute (AST_Attribute *t)
{
  return
    AST_Attribute::narrow_from_decl (
      this->fe_add_decl (t));
}

AST_Operation *
AST_Interface::fe_add_operation (AST_Operation *t)
{
  return
    AST_Operation::narrow_from_decl (
      this->fe_add_decl (t));
}

AST_Structure *
AST_Interface::fe_add_structure (AST_Structure *t)
{
  return this->fe_add_full_struct_type (t);
}

AST_StructureFwd *
AST_Interface::fe_add_structure_fwd (AST_StructureFwd *t)
{
  return this->fe_add_fwd_struct_type (t);
}

AST_Enum *
AST_Interface::fe_add_enum (AST_Enum *t)
{
  return
    AST_Enum::narrow_from_decl (
      this->fe_add_decl (t));
}

AST_Union *
AST_Interface::fe_add_union (AST_Union *t)
{
  return
    AST_Union::narrow_from_decl (
      this->fe_add_full_struct_type (t));
}

AST_UnionFwd *
AST_Interface::fe_add_union_fwd (AST_UnionFwd *t)
{
  return
    AST_UnionFwd::narrow_from_decl (
      this->fe_add_fwd_struct_type (t));
}

// Add an AST_EnumVal node (an enumerator) to this scope.
// This is done to conform to the C++ scoping rules which declare
// enumerators in the enclosing scope (in addition to declaring them
// in the enum itself).
AST_EnumVal *
AST_Interface::fe_add_enum_val (AST_EnumVal *t)
{
  return
    AST_EnumVal::narrow_from_decl (
      this->fe_add_decl (t));
}

// Add an AST_Typedef (a typedef) to the current scope.
AST_Typedef *
AST_Interface::fe_add_typedef (AST_Typedef *t)
{
  return
    AST_Typedef::narrow_from_decl (
      this->fe_add_ref_decl (t));
}

AST_Native *
AST_Interface::fe_add_native (AST_Native *t)
{
  return
    AST_Native::narrow_from_decl (
      this->fe_add_decl (t));
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
                                         false);

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

              AST_InterfaceFwd *fwd = fd->fwd_decl ();

              if (fwd != 0)
                {
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

void
AST_Interface::redef_clash_populate_r (AST_Type *t)
{
  if (this->insert_non_dup (t, 0) == 0)
    {
      return;
    }

  AST_Decl::NodeType nt = t->node_type ();
  long n = 0;

  if (nt != AST_Decl::NT_param_holder)
    {
      AST_Interface *i =
        AST_Interface::narrow_from_decl (t);

      AST_Type **parents = i->inherits ();
      long n_parents = i->n_inherits ();

      for (n = 0; n < n_parents; ++n)
        {
          this->redef_clash_populate_r (parents[n]);
        }
    }

  if (nt == AST_Decl::NT_valuetype || nt == AST_Decl::NT_eventtype)
    {
      AST_ValueType *v = AST_ValueType::narrow_from_decl (t);
      AST_Type **supports = v->supports ();
      long n_supports = v->n_supports ();

      for (n = 0; n < n_supports; ++n)
        {
          this->redef_clash_populate_r (supports[n]);
        }
    }
  else if (nt == AST_Decl::NT_component)
    {
      AST_Component *c = AST_Component::narrow_from_decl (t);
      AST_Type **supports = c->supports ();
      long n_supports = c->n_supports ();

      for (n = 0; n < n_supports; ++n)
        {
          this->redef_clash_populate_r (supports[n]);
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
AST_Interface::insert_non_dup (AST_Type *t,
                               bool abstract_paths_only)
{
  AST_Interface *f =
    AST_Interface::narrow_from_decl (t);

  // Now check if the dequeued element has any ancestors. If yes, insert
  // them inside the queue making sure that there are no duplicates.
  // If we are doing a component, the inheritance list is actually a
  // supports list.
  if (f != 0)
    {
      for (long i = 0; i < f->n_inherits (); ++i)
        {
          // Retrieve the next parent from which
          // the dequeued element inherits.
          AST_Type *parent = f->inherits ()[i];

          if (abstract_paths_only && ! parent->is_abstract ())
            {
              continue;
            }

          (void) this->insert_non_dup (parent,
                                       abstract_paths_only);
        }
    }

  const char *full_name = t->full_name ();

  // Initialize an iterator to search the queue for duplicates.
  for (ACE_Unbounded_Queue_Iterator<AST_Type *> q_iter (
           this->insert_queue
         );
       !q_iter.done ();
       (void) q_iter.advance ())
    {
      // Queue element.
      AST_Type **temp = 0;

      (void) q_iter.next (temp);

      if (!ACE_OS::strcmp (full_name,
                           (*temp)->full_name ()))
        {
          // We exist in this queue and cannot be inserted.
          return 0;
        }
    }

  // Initialize an iterator to search the del_queue for duplicates.
  for (ACE_Unbounded_Queue_Iterator<AST_Type *> del_q_iter (
           this->del_queue
         );
       !del_q_iter.done ();
       (void) del_q_iter.advance ())
    {
      // Queue element.
      AST_Type **temp = 0;

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
           AST_Type *[array_size]);

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

AST_Type **
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

ACE_Unbounded_Queue<AST_Type *> &
AST_Interface::get_insert_queue (void)
{
  return this->insert_queue;
}

ACE_Unbounded_Queue<AST_Type *> &
AST_Interface::get_del_queue (void)
{
  return this->del_queue;
}

bool
AST_Interface::redef_clash (void)
{
  this->insert_queue.reset ();
  this->redef_clash_populate_r (this);

  AST_Type **group1_member = 0;
  AST_Type **group2_member = 0;
  AST_Decl *group1_member_item = 0;
  AST_Decl *group2_member_item = 0;

  int i = 1;

  // Now compare all pairs.
  for (ACE_Unbounded_Queue_Iterator<AST_Type *> group1_iter (
           this->insert_queue
         );
       !group1_iter.done ();
       (void) group1_iter.advance (), ++i)
    {
      // Queue element.
      (void) group1_iter.next (group1_member);
      UTL_Scope *s = DeclAsScope (*group1_member);

      if (s != 0)
        {
          for (UTL_ScopeActiveIterator group1_member_items (
                 s,
                 UTL_Scope::IK_decls);
               !group1_member_items.is_done ();
               group1_member_items.next ())
            {
              group1_member_item = group1_member_items.item ();
              AST_Decl::NodeType nt1 =
                group1_member_item->node_type ();

              // Only these member types may cause a clash because
              // they can't be redefined.
              if (nt1 != AST_Decl::NT_op
                  && nt1 != AST_Decl::NT_attr)
                {
                  continue;
                }

              Identifier *pid1 = group1_member_item->local_name ();
              int j = 0;

              for (ACE_Unbounded_Queue_Iterator<AST_Type *> group2_iter (
                     this->insert_queue);
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
                  UTL_Scope *ss = DeclAsScope (*group2_member);

                  if (ss != 0)
                    {
                      for (UTL_ScopeActiveIterator group2_member_items (
                             ss,
                             UTL_Scope::IK_decls);
                           !group2_member_items.is_done ();
                           group2_member_items.next ())
                        {
                          group2_member_item =
                            group2_member_items.item ();

                          AST_Decl::NodeType nt2 =
                            group2_member_item->node_type ();

                          // Only these member types may cause a clash
                          // with other parents' member of the same type.
                          if (nt2 != AST_Decl::NT_op
                              && nt2 != AST_Decl::NT_attr)
                            {
                              continue;
                            }

                          Identifier *pid2 =
                            group2_member_item->local_name ();

                          if (pid1->compare (pid2) == true)
                            {
                              idl_global->err ()->error3 (
                                UTL_Error::EIDL_REDEF,
                                *group1_member,
                                *group2_member,
                                group2_member_item);

                              return true;
                            }
                          else if (pid1->case_compare_quiet (pid2))
                            {
                              if (idl_global->case_diff_error ())
                                {
                                  idl_global->err ()->error3 (
                                    UTL_Error::EIDL_NAME_CASE_ERROR,
                                    *group1_member,
                                    group1_member_item,
                                    group2_member_item);

                                  return true;
                                }
                              else
                                {
                                  idl_global->err ()->warning3 (
                                    UTL_Error::EIDL_NAME_CASE_WARNING,
                                    *group1_member,
                                    group1_member_item,
                                    group2_member_item);
                                }
                            }
                        } // end of FOR (group2_member_items)
                    } // end of IF ss != 0
                } // end of FOR (group2_iter)
            } // end of FOR (group1_member_items)
        } // end of IF s != 0
    } // end of FOR (group1_iter)

  return false;
}

// Look through inherited interfaces.
AST_Decl *
AST_Interface::look_in_inherited (UTL_ScopedName *e,
                                  bool full_def_only)
{
  AST_Decl *d = 0;
  AST_Decl *d_before = 0;
  AST_Type **is = 0;
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
      AST_Interface *i =
        AST_Interface::narrow_from_decl (*is);

      if (i == 0)
        {
          continue;
        }

      d = (i)->lookup_by_name_r (e, full_def_only);
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
AST_Interface::look_in_inherited_local (Identifier *e,
                                        bool full_def_only)
{
  // Can't look in an interface which was not yet defined.
  if (!this->is_defined ())
    {
      return 0;
    }

  AST_Decl *d = 0;
  AST_Type **is = 0;
  long nis = -1;

  /// OK, loop through inherited interfaces.
  for (nis = this->n_inherits (), is = this->inherits ();
       nis > 0;
       nis--, is++)
    {
      AST_Interface *i =
        AST_Interface::narrow_from_decl (*is);

      if (i == 0)
        {
          continue;
        }

      d = i->lookup_by_name_local (e, full_def_only);

      if (d != 0)
        {
          break;
        }
    }

  return d;
}

AST_Decl *
AST_Interface::lookup_for_add (AST_Decl *d)
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

  prev = this->lookup_by_name_local (id, false);

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
      prev = (*is)->lookup_by_name_local (id, false);

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

int
AST_Interface::has_mixed_parentage (void)
{
  if (this->is_abstract_)
    {
      return 0;
    }

  AST_Decl::NodeType nt = this->node_type ();

  if (AST_Decl::NT_component == nt
      || AST_Decl::NT_home == nt
      || AST_Decl::NT_connector == nt)
    {
      return 0;
    }

  if (this->has_mixed_parentage_ == -1)
    {
      this->analyze_parentage ();
    }

  return this->has_mixed_parentage_;
}

void
AST_Interface::analyze_parentage (void)
{
  if (this->has_mixed_parentage_ != -1)
    {
      return;
    }

  this->has_mixed_parentage_ = 0;

  // Only interfaces may have mixed parentage.
  if (this->node_type () != AST_Decl::NT_interface)
    {
      return;
    }

  for (long i = 0; i < this->pd_n_inherits; ++i)
    {
      AST_Interface *parent =
         AST_Interface::narrow_from_decl (this->pd_inherits[i]);

      if (parent == 0)
        {
          // The item is a template param holder.
          continue;
        }

      if (parent->is_abstract ()
          || parent->has_mixed_parentage ())
        {
          this->has_mixed_parentage_ = 1;
          break;
        }
    }

  // Must check if we are declared in a template module, in
  // which case no code will be generated, so we should not
  // be enqueued.
  bool in_tmpl_module = false;
  UTL_Scope *s = this->defined_in ();

  while (s != 0)
    {
      AST_Template_Module *m =
        AST_Template_Module::narrow_from_scope (s);

      if (m != 0)
        {
          in_tmpl_module = true;
          break;
        }

      s = ScopeAsDecl (s)->defined_in ();
    }

  if (this->has_mixed_parentage_ == 1
      && this->is_defined ()
      && !this->imported ()
      && !in_tmpl_module)
    {
      idl_global->mixed_parentage_interfaces ().enqueue_tail (this);
    }
}

bool
AST_Interface::legal_for_primary_key (void) const
{
  return false;
}

AST_Decl *
AST_Interface::special_lookup (UTL_ScopedName *e,
                               bool full_def_only,
                               AST_Decl *&final_parent_decl)
{
  AST_Decl *d = this->look_in_inherited_local (e->head (),
                                               full_def_only);

  if (d != 0)
    {
      UTL_Scope *s = DeclAsScope (d);
      UTL_ScopedName *sn =
        static_cast<UTL_ScopedName *> (e->tail ());

      return (s != 0 && sn != 0
                ? s->lookup_by_name_r (sn, full_def_only, final_parent_decl)
                : d);
    }

  return 0;
}

AST_Interface *
AST_Interface::ami_handler (void) const
{
  return this->ami_handler_;
}

void
AST_Interface::ami_handler (AST_Interface *handler)
{
  this->ami_handler_ = handler;
}

AST_Interface *
AST_Interface::ami4ccm_uses (void) const
{
  return this->ami4ccm_uses_;
}

void
AST_Interface::ami4ccm_uses (AST_Interface *implied)
{
  this->ami4ccm_uses_ = implied;
}

void
AST_Interface::destroy (void)
{
  for (ACE_Unbounded_Queue_Iterator<AST_Type *> i (
         this->param_holders_);
       !i.done ();
       (void) i.advance ())
    {
      AST_Type **tt = 0;
      i.next (tt);
      AST_Type *t = *tt;
      t->destroy ();
      delete t;
      t = 0;
    }

  // The destroy() we are in gets called twice if we start from
  // be_valuetype or be_eventtype. This line keeps us from
  // iterating over null pointers the 2nd time.
  this->param_holders_.reset ();

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
