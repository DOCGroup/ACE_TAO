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

#pragma ident "%@(#)utl_stack.cc	1.36% %92/06/10% Sun Microsystems"

/*
 * utl_stack.cc - Implementation of class UTL_ScopeStack
 */

#include	"idl.h"
#include	"idl_extern.h"

// Class UTL_ScopeStack

#undef	INCREMENT
#define	INCREMENT	64

/*
 * Constructor(s) and destructor
 */

UTL_ScopeStack::UTL_ScopeStack()
	      : pd_stack_data_nalloced(INCREMENT),
		pd_stack_data(new UTL_Scope *[INCREMENT]),
		pd_stack_top(0)
{
}

UTL_ScopeStack::~UTL_ScopeStack()
{
  if (pd_stack_data != NULL)
    delete pd_stack_data;
}

/*
 * Private operations
 */

/*
 * Public operations
 */

// Push an element on the stack
UTL_ScopeStack *
UTL_ScopeStack::push(UTL_Scope *el)
{
  UTL_Scope	**tmp;
  AST_Decl	*d = ScopeAsDecl(el);
  long		ostack_data_nalloced;
  long		i;

  // Make sure there's space for one more
  if (pd_stack_data_nalloced == pd_stack_top) {
    ostack_data_nalloced = pd_stack_data_nalloced;
    pd_stack_data_nalloced += INCREMENT;
    tmp			 = new UTL_Scope *[pd_stack_data_nalloced];

    for (i = 0; i < ostack_data_nalloced; i++)
      tmp[i] = pd_stack_data[i];

    delete []pd_stack_data;
    pd_stack_data = tmp;
  }

  // Insert new scope
  pd_stack_data[pd_stack_top++] = el;

  return this;
}

// Pop an element from the stack
void
UTL_ScopeStack::pop()
{
  UTL_Scope	*s;

  if (pd_stack_top <= 0)
    return;
  s = pd_stack_data[--pd_stack_top];
}

// Return top element on stack
UTL_Scope *
UTL_ScopeStack::top()
{
  if (pd_stack_top <= 0)
    return NULL;
  return pd_stack_data[pd_stack_top - 1];
}

// Return bottom element on stack
UTL_Scope *
UTL_ScopeStack::bottom()
{
  if (pd_stack_top == 0)
    return NULL;
  return pd_stack_data[0];
}

// Clear entire stack
void
UTL_ScopeStack::clear()
{
  pd_stack_top = 0;
}

// How deep is the stack?
unsigned long
UTL_ScopeStack::depth()
{
  return pd_stack_top;
}

// Return (top - 1) element on stack
UTL_Scope *
UTL_ScopeStack::next_to_top()
{
  UTL_Scope	*tmp, *retval;

  if (depth() < 2)
    return NULL;

  tmp = top();		// Save top
  (void) pop();		// Pop it
  retval = top();	// Get next one down
  (void) push(tmp);	// Push top back
  return retval;	// Return next one down
}

// Return topmost non-NULL element
UTL_Scope *
UTL_ScopeStack::top_non_null()
{
  long		i;

  for (i = pd_stack_top - 1; i >= 0; i--)
    if (pd_stack_data[i] != NULL)
      return pd_stack_data[i];
  return NULL;
}

/*
 * Redefinition of inherited virtual operations
 */

// Class UTL_ScopeStackActiveIterator

/*
 * Constructor(s)
 */

UTL_ScopeStackActiveIterator::UTL_ScopeStackActiveIterator(UTL_ScopeStack *s)
			    : source(s),
			      il(s->pd_stack_top - 1)
{
}

/*
 * Private operations
 */

/*
 * Public operations
 */

// Advance to next item
void
UTL_ScopeStackActiveIterator::next()
{
  il--;
}

// Get current item
UTL_Scope *
UTL_ScopeStackActiveIterator::item()
{
  if (il >= 0)
    return source->pd_stack_data[il];
  return NULL;
}

// Is this iteration done?
long
UTL_ScopeStackActiveIterator::is_done()
{
  if (il >= 0)
    return I_FALSE;
  return I_TRUE;
}

/*
 * Redefinition of inherited virtual operations
 */
