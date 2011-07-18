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

// EXPLANATION: The CORBA spec says that the only legal use of recursive types is
// in a manifest sequence declared inside a struct or union whose base type is
// the struct or union.

// ALGORITHM FOR CHECK:
// Sequences push a NULL on the scope stack to mark where in the scope nesting
// they appear.

// - If the type is not a struct or union, return FALSE (legal use of type).
// - Otherwise check up the scope stack, looking for this base type. If we
//   find a NULL return FALSE (legal use of type, since it is inside some
//   sequence). If we find the type on the stack, return TRUE (illegal use
//   since it was not bracketed by a sequence). If we don't find the base
//   type nor a sequence, return FALSE (legal use, since we're not nested).

#include "ast_union.h"
#include "utl_stack.h"
#include "global_extern.h"
#include "nr_extern.h"

ACE_RCSID (ast,
           ast_recursive,
           "$Id$")

bool
AST_illegal_interface_recursion (AST_Decl *t)
{
  // Can't be 0 since we know we have an interface or valuetype.
  AST_Decl *d = 0;

  // If we encounter the argument in an enclosing scope, it's illegal.
  for (UTL_ScopeStackActiveIterator i (idl_global->scopes ());
       !i.is_done ();
       i.next ())
    {
      d = ScopeAsDecl (i.item ());

      // Exceptions cannot be recursive, but may contain a reference
      // to the interface they are defined in.
      if (d->node_type () == AST_Decl::NT_except)
        {
          return false;
        }

      if (d == t)
        {
          return true;
        }
    }

  return false;
}

bool
AST_illegal_recursive_type (AST_Decl *t)
{
  if (t == 0)
    {
      return false;
    }

  AST_Decl::NodeType nt;
  AST_Type *ut = AST_Type::narrow_from_decl (t);

  if (ut != 0)
    {
      ut = ut->unaliased_type ();
      nt = ut->node_type ();
    }
  else
    {
      nt = t->node_type ();
    }

  if (nt == AST_Decl::NT_interface)
    {
      // Check for interface->struct/union->....->interface nesting.
//      return AST_illegal_interface_recursion (t);
    }
  else if (nt != AST_Decl::NT_struct && nt != AST_Decl::NT_union)
    {
      // Structs and unions fall through to the check below.
      return false;  // NOT ILLEGAL.
    }

  bool check_for_struct = false;
  bool check_for_union = false;
  AST_Structure  *st1 = 0;
  AST_Union  *un1 = 0;

  // Narrow the type appropriately so comparison will work.
  if (t->node_type () == AST_Decl::NT_struct)
    {
      check_for_struct = true;
      st1 = AST_Structure::narrow_from_decl (t);

      if (st1 == 0)
        {
          return false;  // NOT ILLEGAL.
        }
    }
  else if (t->node_type () == AST_Decl::NT_union)
    {
      check_for_union = true;
      un1 = AST_Union::narrow_from_decl (t);

      if (un1 == 0)
        {
          return false;  // NOT ILLEGAL.
        }
    }

  UTL_Scope  *s = 0;
  AST_Structure *st2 = 0;
  AST_Union *un2 = 0;

  // OK, iterate up the stack.
  for (UTL_ScopeStackActiveIterator i (idl_global->scopes ());
       !i.is_done ();
       i.next ())
    {
      s = i.item ();

      // If we hit a NULL we're done since it means that we're nested inside
      // a sequence, where recursive types may be used.
      if (s == 0)
        {
          return false;  // NOT ILLEGAL.
        }

      // OK, must check this scope.
      if (s->scope_node_type () == AST_Decl::NT_struct
          && check_for_struct == true)
        {
          st2 = AST_Structure::narrow_from_scope (s);

          if (st2 != 0 && st2 == st1)
            {
              return true;  // ILLEGAL RECURSIVE TYPE USE.
            }
        }
      else if (s->scope_node_type () == AST_Decl::NT_union
               && check_for_union == true)
        {
          un2 = AST_Union::narrow_from_scope (s);

          if (un2 != 0 && un2 == un1)
            {
              return true;  // ILLEGAL RECURSIVE TYPE USE.
            }
        }
    }

  // No more scopes to check. This type was used legally.
  return false;    // NOT ILLEGAL.
}

