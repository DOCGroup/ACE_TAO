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

#ifndef _UTL_STACK_UTL_STACK_HH
#define _UTL_STACK_UTL_STACK_HH

#include "TAO_IDL_FE_Export.h"

class UTL_Scope;

// UTL_ScopeStack implements scope nesting.

// Forward declaration of active iterator for UTL_ScopeStack.
class UTL_ScopeStackActiveIterator;

class TAO_IDL_FE_Export UTL_ScopeStack
{
public:
  UTL_ScopeStack (void);
  ~UTL_ScopeStack (void);

  // Return top element.
  UTL_Scope *top (void);

  // Pop top element.
  void pop (void);

  // Clear entire stack.
  void clear (void);

  // Push an element on the stack and return stack.
  UTL_ScopeStack *push (UTL_Scope *el);

  // How deep is the stack now?
  unsigned long depth (void);

  // Return bottom element.
  UTL_Scope *bottom (void);

  // Return (top - 1) element.
  UTL_Scope *next_to_top (void);

  // return topmost non-NULL element.
  UTL_Scope *top_non_null (void);

private:
  // Store scopes stack
  UTL_Scope **pd_stack_data;

  // How many allocated?
  unsigned long pd_stack_data_nalloced;

  // How many used?
  unsigned long pd_stack_top;

  // Friend active iterator class for UTL_ScopeStack.
  friend class UTL_ScopeStackActiveIterator;
};

// Active iterator for UTL_ScopeStack.
class TAO_IDL_FE_Export UTL_ScopeStackActiveIterator
{
public:
  UTL_ScopeStackActiveIterator (UTL_ScopeStack &s);

  // Advance to next element.
  void next (void);

  // Get current item.
  UTL_Scope*item (void);

  // Is the iteration finished?
  long is_done (void);

private:
  // On what to iterate?
  UTL_ScopeStack &source;

  // Where are we in iteration?
  long il;
};

#endif           // _UTL_STACK_UTL_STACK_HH
