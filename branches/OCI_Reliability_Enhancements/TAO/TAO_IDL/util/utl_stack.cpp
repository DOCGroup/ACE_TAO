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

#include "utl_stack.h"
#include "utl_scope.h"
#include "global_extern.h"

#undef  INCREMENT
#define  INCREMENT  64

UTL_ScopeStack::UTL_ScopeStack (void)
  : pd_stack_data_nalloced (INCREMENT),
    pd_stack_top (0)
{
  ACE_NEW (this->pd_stack_data,
           UTL_Scope *[INCREMENT]);
}

UTL_ScopeStack::~UTL_ScopeStack (void)
{
  if (this->pd_stack_data != 0)
    {
      delete [] this->pd_stack_data;
    }
}

// Push an element on the stack.
UTL_ScopeStack *
UTL_ScopeStack::push (UTL_Scope *el)
{
  UTL_Scope  **tmp = 0;
  long ostack_data_nalloced;
  long i;

  // Make sure there's space for one more.
  if (this->pd_stack_data_nalloced == this->pd_stack_top)
    {
      ostack_data_nalloced = this->pd_stack_data_nalloced;
      this->pd_stack_data_nalloced += INCREMENT;

      ACE_NEW_RETURN (tmp,
                      UTL_Scope *[this->pd_stack_data_nalloced],
                      0);

      for (i = 0; i < ostack_data_nalloced; ++i)
        {
          tmp[i] = this->pd_stack_data[i];
        }

      delete [] this->pd_stack_data;
      this->pd_stack_data = tmp;
    }

  // Insert new scope.
  this->pd_stack_data[this->pd_stack_top++] = el;

  return this;
}

// Pop an element from the stack.
void
UTL_ScopeStack::pop (void)
{
  if (this->pd_stack_top <= 0)
    {
      return;
    }

  UTL_Scope *current = this->top ();

  // If our top scope has a #pragma prefix associated with it,
  // it goes away with the scope.
  if (current != 0 && current->has_prefix ())
    {
      char *trash = 0;
      idl_global->pragma_prefixes ().pop (trash);
      delete [] trash;
    }

  --this->pd_stack_top;
}

// Return top element on stack.
UTL_Scope *
UTL_ScopeStack::top (void)
{
  if (this->pd_stack_top <= 0)
    {
      return 0;
    }

  return this->pd_stack_data[pd_stack_top - 1];
}

// Return bottom element on stack.
UTL_Scope *
UTL_ScopeStack::bottom (void)
{
  if (this->pd_stack_top == 0)
    {
      return 0;
    }

  return this->pd_stack_data[0];
}

// Clear entire stack.
void
UTL_ScopeStack::clear (void)
{
  this->pd_stack_top = 0;
}

// How deep is the stack?
unsigned long
UTL_ScopeStack::depth (void)
{
  return this->pd_stack_top;
}

// Return (top - 1) element on stack.
UTL_Scope *
UTL_ScopeStack::next_to_top (void)
{
  UTL_Scope  *tmp, *retval;

  if (this->depth () < 2)
    {
      return 0;
    }

  tmp = top ();    // Save top
  (void) pop ();    // Pop it
  retval = top ();  // Get next one down
  (void) push (tmp);  // Push top back
  return retval;  // Return next one down
}

// Return topmost non-NULL element.
UTL_Scope *
UTL_ScopeStack::top_non_null (void)
{
  for (long i = this->pd_stack_top - 1; i >= 0; --i)
    {
      if (this->pd_stack_data[i] != 0)
        {
          return this->pd_stack_data[i];
        }
    }

  return 0;
}

UTL_ScopeStackActiveIterator::UTL_ScopeStackActiveIterator (UTL_ScopeStack &s)
  : source (s),
    il (s.pd_stack_top - 1)
{
}

// Advance to next item
void
UTL_ScopeStackActiveIterator::next (void)
{
  il--;
}

// Get current item.
UTL_Scope *
UTL_ScopeStackActiveIterator::item (void)
{
  if (this->il >= 0)
    {
      return this->source.pd_stack_data[il];
    }

  return 0;
}

// Is this iteration done?
long
UTL_ScopeStackActiveIterator::is_done (void)
{
  if (this->il >= 0)
    {
      return false;
    }

  return true;
}

