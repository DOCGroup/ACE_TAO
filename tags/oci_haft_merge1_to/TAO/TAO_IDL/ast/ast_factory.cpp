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

// AST_Factory nodes denote OBV factory construct declarations
// AST_Factory is a subclass of AST_Decl (it is not a type!)
// and of UTL_Scope (the arguments are managed in a scope).

#include "ast_factory.h"
#include "ast_argument.h"
#include "ast_exception.h"
#include "ast_visitor.h"
#include "global_extern.h"
#include "utl_err.h"
#include "utl_identifier.h"
#include "utl_exceptlist.h"
#include "utl_namelist.h"

ACE_RCSID (ast,
           ast_factory,
           "$Id$")

AST_Factory::AST_Factory (void)
  : COMMON_Base (), 
    AST_Decl (),
    UTL_Scope (),
    pd_exceptions (0),
    pd_n_exceptions (0),
    argument_count_ (-1),
    has_native_ (0)
{
}

AST_Factory::AST_Factory (UTL_ScopedName *n)
  : COMMON_Base (1,
                 0), //@@ Always local, never abstract
    AST_Decl (AST_Decl::NT_factory,
              n),
    UTL_Scope (AST_Decl::NT_factory),
    pd_exceptions (0),
    pd_n_exceptions (0),
    argument_count_ (-1),
    has_native_ (0)
{
}

AST_Factory::~AST_Factory (void)
{
}

// Public operations.

UTL_ExceptList *
AST_Factory::exceptions (void)
{
  return this->pd_exceptions;
}

int
AST_Factory::n_exceptions (void)
{
  return this->pd_n_exceptions;
}

// Return the member count.
int
AST_Factory::argument_count (void)
{
  this->compute_argument_attr ();

  return this->argument_count_;
}

// Return if any argument or the return type is a <native> type.
int
AST_Factory::has_native (void)
{
  this->compute_argument_attr ();

  return this->has_native_;
}

void
AST_Factory::destroy (void)
{
}

// Private operations.

// Compute total number of members.
int
AST_Factory::compute_argument_attr (void)
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
      for (UTL_ScopeActiveIterator i (this, IK_decls);
           !i.is_done ();
           i.next ())
        {
          // Get the next AST decl node.
          d = i.item ();

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

  return 0;
}

// Add this AST_Argument node (an factory argument declaration)
// to this scope.
AST_Argument *
AST_Factory::fe_add_argument (AST_Argument *t)
{
  AST_Decl *d = 0;

  // Already defined and cannot be redefined? Or already used?
  if ((d = lookup_by_name_local (t->local_name(), 0)) != 0)
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

UTL_NameList *
AST_Factory::fe_add_exceptions (UTL_NameList *t)
{
  UTL_ScopedName *nl_n = 0;
  AST_Exception *fe = 0;
  AST_Decl *d = 0;

  this->pd_exceptions = 0;

  for (UTL_NamelistActiveIterator nl_i (t); !nl_i.is_done (); nl_i.next ())
    {
      nl_n = nl_i.item ();

      d = this->lookup_by_name (nl_n,
                                I_TRUE);

      if (d == 0 || d->node_type() != AST_Decl::NT_except)
        {
          idl_global->err ()->lookup_error (nl_n);
          return 0;
        }

      fe = AST_Exception::narrow_from_decl (d);

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

      this->pd_n_exceptions++;
    }

  return t;
}

// Dump this AST_Factory node (an OBV factory construct) to the ostream o.
void
AST_Factory::dump (ACE_OSTREAM_TYPE &o)
{
  AST_Decl *d = 0;

  o << "factory ";
  this->local_name ()->dump (o);
  o << "(";

  // Iterator must be explicitly advanced inside the loop.
  for (UTL_ScopeActiveIterator i (this, IK_decls);
       !i.is_done();)
    {
      d = i.item ();
      d->dump (o);
      i.next ();

      if (!i.is_done())
        {
          o << ", ";
        }
    }

  o << ")";

}

int
AST_Factory::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_factory (this);
}

// Data accessors

// Narrowing.
IMPL_NARROW_METHODS2(AST_Factory, AST_Decl, UTL_Scope)
IMPL_NARROW_FROM_DECL(AST_Factory)
IMPL_NARROW_FROM_SCOPE(AST_Factory)
