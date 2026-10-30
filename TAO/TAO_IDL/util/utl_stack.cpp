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

#include "utl_stack.h"
#include "utl_scoped_name.h"
#include "utl_scope.h"
#include "ast_decl.h"
#include "global_extern.h"

#include <new>
#include <stdexcept>

UTL_ScopeStack::UTL_ScopeStack ()
{
  this->pd_stack_data.reserve (increments);
}

// Push an element on the stack.
UTL_ScopeStack *
UTL_ScopeStack::push (UTL_Scope *el)
{
  try
    {
      this->pd_stack_data.push_back (el);
    }
  catch (std::bad_alloc const &)
    {
      return nullptr;
    }
  catch (std::length_error const &)
    {
      return nullptr;
    }

  return this;
}

// Pop an element from the stack.
void
UTL_ScopeStack::pop ()
{
  if (this->pd_stack_data.empty ())
    {
      return;
    }

  UTL_Scope *current = this->top ();

  // If our top scope has a #pragma prefix associated with it,
  // it goes away with the scope.
  if (current != nullptr && current->has_prefix ())
    {
      if (!idl_global->pragma_prefixes ().empty ())
        {
          idl_global->pragma_prefixes ().pop_back ();
        }
    }

  this->pd_stack_data.pop_back ();
}

// Return top element on stack.
UTL_Scope *
UTL_ScopeStack::top () const
{
  if (this->pd_stack_data.empty ())
    {
      return nullptr;
    }

  return this->pd_stack_data.back ();
}

// Return bottom element on stack.
UTL_Scope *
UTL_ScopeStack::bottom () const
{
  if (this->pd_stack_data.empty ())
    {
      return nullptr;
    }

  return this->pd_stack_data.front ();
}

// Clear entire stack.
void
UTL_ScopeStack::clear ()
{
  this->pd_stack_data.clear ();
}

// How deep is the stack?
unsigned long
UTL_ScopeStack::depth () const
{
  return static_cast<unsigned long> (this->pd_stack_data.size ());
}

// Return (top - 1) element on stack.
UTL_Scope *
UTL_ScopeStack::next_to_top () const
{
  if (this->pd_stack_data.size () < 2)
    {
      return nullptr;
    }

  return this->pd_stack_data[this->pd_stack_data.size () - 2];
}

// Return topmost non-NULL element.
UTL_Scope *
UTL_ScopeStack::top_non_null () const
{
  for (std::size_t i = this->pd_stack_data.size (); i > 0; --i)
    {
      UTL_Scope *scope = this->pd_stack_data[i - 1];

      if (scope != nullptr)
        {
          return scope;
        }
    }

  return nullptr;
}

AST_Decl *
UTL_ScopeStack::lookup_by_name (
  UTL_ScopedName *name, bool full_def_only, bool for_add)
{
  for (std::size_t i = this->pd_stack_data.size (); i > 0; --i)
    {
      UTL_Scope *scope = this->pd_stack_data[i - 1];
      if (scope != nullptr)
        {
          AST_Decl *node = scope->lookup_by_name (name, full_def_only, for_add);
          if (node != nullptr) return node;
        }
    }
  return nullptr;
}

UTL_ScopeStackActiveIterator::UTL_ScopeStackActiveIterator (UTL_ScopeStack &s)
  : source (s),
    il (static_cast<long> (s.pd_stack_data.size ()) - 1)
{
}

// Advance to next item
void
UTL_ScopeStackActiveIterator::next ()
{
  il--;
}

// Get current item.
UTL_Scope *
UTL_ScopeStackActiveIterator::item () const
{
  if (this->il >= 0)
    {
      return this->source.pd_stack_data[static_cast<std::size_t> (il)];
    }

  return nullptr;
}

// Is this iteration done?
bool
UTL_ScopeStackActiveIterator::is_done () const
{
  if (this->il >= 0)
    {
      return false;
    }

  return true;
}
