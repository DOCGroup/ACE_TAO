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

// AST_Operation nodes denote IDL operation declarations
// AST_Operations are a subclass of AST_Decl (they are not a type!)
// and of UTL_Scope (the arguments are managed in a scope).
// AST_Operations have a return type (a subclass of AST_Type),
// a bitfield for denoting various properties of the operation (the
// values are ORed together from constants defined in the enum
// AST_Operation::FLags), a name (a UTL_ScopedName), a context
// (implemented as a list of Strings, a UTL_StrList), and a raises
// clause (implemented as an array of AST_Exceptions).

#include "ast_operation.h"
#include "ast_predefined_type.h"
#include "ast_argument.h"
#include "ast_exception.h"
#include "ast_param_holder.h"
#include "ast_typedef.h"
#include "ast_visitor.h"

#include "utl_err.h"
#include "utl_namelist.h"
#include "utl_exceptlist.h"
#include "utl_identifier.h"
#include "utl_string.h"
#include "utl_strlist.h"

#include "global_extern.h"

AST_Decl::NodeType const
AST_Operation::NT = AST_Decl::NT_op;

AST_Operation::AST_Operation (AST_Type *rt,
                              Flags fl,
                              UTL_ScopedName *n,
                              bool local,
                              bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl(AST_Decl::NT_op,
             n),
    UTL_Scope(AST_Decl::NT_op),
    pd_return_type (rt),
    pd_flags (fl),
    pd_context (0),
    pd_exceptions (0),
    argument_count_ (-1),
    has_native_ (0)
{
  AST_PredefinedType *pdt = 0;

  // Check that if the operation is oneway, the return type must be void.
  if (rt != 0 && pd_flags == OP_oneway)
    {
      if (rt->node_type () != AST_Decl::NT_pre_defined)
        {
          idl_global->err ()->error1 (UTL_Error::EIDL_NONVOID_ONEWAY,
                                      this);
        }
      else
        {
          pdt = AST_PredefinedType::narrow_from_decl (rt);

          if (pdt == 0 || pdt->pt () != AST_PredefinedType::PT_void)
            {
              idl_global->err ()->error1 (UTL_Error::EIDL_NONVOID_ONEWAY,
                                          this);
            }
        }
    }
}

AST_Operation::~AST_Operation (void)
{
}

// Public operations.

bool
AST_Operation::void_return_type (void)
{
  AST_Type* type = this->return_type ();

  return (type->node_type () == AST_Decl::NT_pre_defined
          && (AST_PredefinedType::narrow_from_decl (type)->pt ()
                == AST_PredefinedType::PT_void));
}

// Return the member count.
int
AST_Operation::argument_count (void)
{
  this->compute_argument_attr ();

  return this->argument_count_;
}

int
AST_Operation::count_arguments_with_direction (int direction_mask)
{
  int count = 0;

  for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
       !si.is_done ();
       si.next ())
    {
      AST_Argument *arg =
        AST_Argument::narrow_from_decl (si.item ());

      if ((arg->direction () & direction_mask) != 0)
        {
          ++count;
        }
    }

  return count;
}


// Return if any argument or the return type is a <native> type.
int
AST_Operation::has_native (void)
{
  this->compute_argument_attr ();

  return this->has_native_;
}

void
AST_Operation::destroy (void)
{
  // No need to delete our exception list, the
  // destroy() method does it. The UTL_ExceptList
  // destroy() method does NOT delete the contained
  // exception nodes.

  if (this->pd_exceptions != 0)
    {
      this->pd_exceptions->destroy ();
      this->pd_exceptions = 0;
    }

  this->UTL_Scope::destroy ();
  this->AST_Decl::destroy ();
}

UTL_ExceptList *
AST_Operation::be_add_exceptions (UTL_ExceptList *t)
{
  if (this->pd_exceptions != 0)
    {
      idl_global->err ()->error1 (UTL_Error::EIDL_ILLEGAL_RAISES,
                                  this);
    }
  else
    {
      this->pd_exceptions = t;
    }

  return this->pd_exceptions;
}

// Private operations.

// Compute total number of members.
int
AST_Operation::compute_argument_attr (void)
{
  if (this->argument_count_ != -1)
    {
      return 0;
    }

  AST_Decl *d = 0;
  AST_Type *type = 0;
  AST_Argument *arg = 0;

  this->argument_count_ = 0;

  // If there are elements in this scope.
  if (this->nmembers () > 0)
    {
      // Instantiate a scope iterator.
      for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
           !si.is_done ();
           si.next ())
        {
          // Get the next AST decl node.
          d = si.item ();

          if (d->node_type () == AST_Decl::NT_argument)
            {
              this->argument_count_++;

              arg = AST_Argument::narrow_from_decl (d);

              type = AST_Type::narrow_from_decl (arg->field_type ());

              if (type->node_type () == AST_Decl::NT_native)
                {
                  this->has_native_ = 1;
                }
            }
        }
    }

  type = AST_Type::narrow_from_decl (this->return_type ());

  if (type->node_type () == AST_Decl::NT_native)
    {
      this->has_native_ = 1;
    }

  return 0;
}

// Add this context (a UTL_StrList) to this scope.
UTL_StrList *
AST_Operation::fe_add_context (UTL_StrList *t)
{
  this->pd_context = t;

  return t;
}

// Add these exceptions (identified by name) to this scope.
// This looks up each name to resolve it to the name of a known
// exception, and then adds the referenced exception to the list
// exceptions that this operation can raise.

// NOTE: No attempt is made to ensure that exceptions are mentioned
//       only once..
UTL_NameList *
AST_Operation::fe_add_exceptions (UTL_NameList *t)
{
  if (0 == t)
    {
      return 0;
    }

  UTL_ScopedName *nl_n = 0;
  AST_Type *fe = 0;
  AST_Decl *d = 0;

  this->pd_exceptions = 0;

  for (UTL_NamelistActiveIterator nl_i (t); !nl_i.is_done (); nl_i.next ())
    {
      nl_n = nl_i.item ();
      d = this->lookup_by_name (nl_n, true);

      if (d == 0)
        {
          idl_global->err ()->lookup_error (nl_n);
          return 0;
        }

      AST_Decl::NodeType nt = d->node_type ();

      switch (nt)
        {
          case AST_Decl::NT_except:
            break;
          case AST_Decl::NT_param_holder:
            {
              AST_Param_Holder *ph =
                AST_Param_Holder::narrow_from_decl (d);

              nt = ph->info ()->type_;

              if (nt != AST_Decl::NT_except
                  && nt != AST_Decl::NT_type)
                {
                  idl_global->err ()->mismatched_template_param (
                    ph->info ()->name_.c_str ());
                }

              break;
            }
          case AST_Decl::NT_typedef:
            {
              AST_Typedef *td =
                AST_Typedef::narrow_from_decl (d);

              nt = td->primitive_base_type ()->node_type ();

              if (nt != AST_Decl::NT_except)
                {
                  idl_global->err ()->error1 (
                    UTL_Error::EIDL_ILLEGAL_RAISES,
                    this);
                }

              break;
            }
          case AST_Decl::NT_native:
            {
              // This is the only use case for this node type.
              int compare =
                ACE_OS::strcmp (d->local_name ()->get_string (),
                                "UserExceptionBase");

              if (compare != 0)
                {
                  idl_global->err ()->error1 (
                    UTL_Error::EIDL_ILLEGAL_RAISES,
                    this);
                }

              break;
            }
          default:
            idl_global->err ()->error1 (
              UTL_Error::EIDL_ILLEGAL_RAISES,
              this);

            break;
        };

      bool oneway_op =
        (this->flags () == AST_Operation::OP_oneway);

      fe = AST_Type::narrow_from_decl (d);

      if (oneway_op && fe != 0)
        {
          idl_global->err ()->error1 (UTL_Error::EIDL_ILLEGAL_RAISES,
                                      this);
        }

      if (fe == 0)
        {
          idl_global->err ()->error1 (UTL_Error::EIDL_ILLEGAL_RAISES,
                                      this);
          return 0;
        }

      if (this->pd_exceptions == 0)
        {
          ACE_NEW_RETURN (this->pd_exceptions,
                          UTL_ExceptList (fe,
                                          0),
                          0);
        }
      else
        {
          UTL_ExceptList *el = 0;
          ACE_NEW_RETURN (el,
                          UTL_ExceptList (fe,
                                          0),
                          0);

          this->pd_exceptions->nconc (el);
        }
    }

  // This return value is never used, it's easier to
  // destroy it here and return 0 than to destroy it
  // each place it is passed in.
  t->destroy ();
  delete t;
  t = 0;
  return 0;
}

AST_Argument *
AST_Operation::fe_add_argument (AST_Argument *t)
{
  return
    AST_Argument::narrow_from_decl (
      this->fe_add_decl (t));
}

// Dump this AST_Operation node (an operation) to the ostream o.
void
AST_Operation::dump (ACE_OSTREAM_TYPE &o)
{
  AST_Decl *d = 0;
  AST_Type *e = 0;
  UTL_String *s = 0;

  if (this->pd_flags == OP_oneway)
    {
      this->dump_i (o, "oneway ");
    }
  else if (this->pd_flags == OP_idempotent)
    {
      this->dump_i (o, "idempotent ");
    }

  this->pd_return_type->name ()->dump (o);
  this->dump_i (o, " ");
  this->local_name ()->dump (o);
  this->dump_i (o, "(");

  // Must advance the iterator explicity inside the loop.
  for (UTL_ScopeActiveIterator i (this, IK_decls); !i.is_done ();)
    {
      d = i.item ();
      d->dump (o);
      i.next ();

      if (!i.is_done())
        {
          this->dump_i (o, ", ");
        }
    }

  this->dump_i (o, ")");

  if (this->pd_exceptions != 0)
    {
      this->dump_i (o, " raises(");

      // Must advance the iterator explicity inside the loop.
      for (UTL_ExceptlistActiveIterator ei (this->pd_exceptions);
           !ei.is_done ();)
        {
          e = ei.item ();
          ei.next ();
          e->local_name ()->dump (o);

          if (!ei.is_done())
            {
             this->dump_i (o, ", ");
            }
        }

      this->dump_i (o, ")");
    }

  if (this->pd_context != 0)
    {
      this->dump_i (o, " context(");

      // Must advance the iterator explicity inside the loop.
      for (UTL_StrlistActiveIterator si (this->pd_context); !si.is_done();)
        {
          s = si.item ();
          si.next ();
          this->dump_i (o, s->get_string ());

          if (!si.is_done())
            {
              this->dump_i (o, ", ");
            }
        }

      this->dump_i (o, ")");
    }
}

int
AST_Operation::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_operation (this);
}

// Data accessors

AST_Type *
AST_Operation::return_type (void)
{
  return this->pd_return_type;
}

AST_Operation::Flags
AST_Operation::flags (void)
{
  return this->pd_flags;
}

UTL_StrList *
AST_Operation::context (void)
{
  return this->pd_context;
}

UTL_ExceptList *
AST_Operation::exceptions (void)
{
  return this->pd_exceptions;
}

IMPL_NARROW_FROM_DECL(AST_Operation)
IMPL_NARROW_FROM_SCOPE(AST_Operation)
