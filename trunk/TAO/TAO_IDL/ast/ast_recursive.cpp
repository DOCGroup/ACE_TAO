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

/*
 * ast_recursive.cc - Check whether a type is used recursively in a situation
 *		      where this use would be illegal.
 *
 * EXPLANATION: The CORBAS says that the only legal use of recursive types is
 * in a manifest sequence declared inside a struct or union whose base type is
 * the struct or union
 *
 * ALGORITH FOR CHECK:
 * Sequences push a NULL on the scope stack to mark where in the scope nesting
 * they appear.
 *
 * - If the type is not a struct or union, return FALSE (legal use of type)
 * - Otherwise check up the scope stack, looking for this base type. If we
 *   find a NULL return FALSE (legal use of type, since it is inside some
 *   sequence). If we find the type on the stack, return TRUE (illegal use
 *   since it was not bracketed by a sequence). If we don't find the base
 *   type nor a sequence, return FALSE (legal use, since we're not nested).
 */

#include	"idl.h"
#include	"idl_extern.h"

/*
 * FE_illegal_recursive_type() - Implement the algorithm described above
 */
idl_bool
AST_illegal_recursive_type(AST_Decl *t)
{
  UTL_ScopeStackActiveIterator *i = 0;
  UTL_Scope		       *s = 0;
  AST_Structure		       *st1 = 0, *st2 = 0;
  AST_Union		       *un1 = 0, *un2 = 0;
  idl_bool		       check_for_struct = I_FALSE,
			       check_for_union = I_FALSE;

  if (t == NULL) return I_FALSE;
  
  /*
   * We only care about structs and unions
   */
  if (t->node_type() != AST_Decl::NT_struct &&
      t->node_type() != AST_Decl::NT_union)
    return I_FALSE;	// NOT ILLEGAL
  /*
   * Narrow the type appropriately so comparison will work
   */
  if (t->node_type() == AST_Decl::NT_struct) {
    check_for_struct = I_TRUE;
    st1 = AST_Structure::narrow_from_decl(t);
    if (st1 == NULL)
      return I_FALSE;	// NOT ILLEGAL
  } else if (t->node_type() == AST_Decl::NT_union) {
    check_for_union = I_TRUE;
    un1 = AST_Union::narrow_from_decl(t);
    if (un1 == NULL)
      return I_FALSE;	// NOT ILLEGAL
  }
  /*
   * OK, iterate up the stack
   */
  i = new UTL_ScopeStackActiveIterator(idl_global->scopes());
  while (!(i->is_done())) {
    s = i->item();
    /*
     * If we hit a NULL we're done since it means that we're nested inside
     * a sequence, where recursive types may be used
     */
    if (s == NULL) {
      delete i;
      return I_FALSE;	// NOT ILLEGAL
    }
    /*
     * OK, must check this scope
     */
    if (s->scope_node_type() == AST_Decl::NT_struct && check_for_struct) {
      st2 = AST_Structure::narrow_from_scope(s);
      if (st2 != NULL && st2 == st1) {
	delete i;
        return I_TRUE;	// ILLEGAL RECURSIVE TYPE USE
      }
    } else if (s->scope_node_type() == AST_Decl::NT_union &&
	       check_for_union) {
      un2 = AST_Union::narrow_from_scope(s);
      if (un2 != NULL && un2 == un1) {
        delete i;
	return I_TRUE;	// ILLEGAL RECURSIVE TYPE USE
      }
    }
    /*
     * This one is fine, get next item
     */
    i->next();
  }
  /*
   * No more scopes to check. This type was used legally
   */
  delete i;
  return I_FALSE;		// NOT ILLEGAL
}

