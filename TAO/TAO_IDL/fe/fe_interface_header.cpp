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

#pragma ident "%@(#)fe_interface_header.cc	1.30% %92/06/10% Sun Microsystems"

/*
 * fe_interface_header.cc - Implements the FE private class FE_InterfaceHeader
 *
 * FE_InterfaceHeader instances are used to store information about an
 * interface header as the interface is being parsed and before the
 * AST_Interface node used to represent this interface is created.
 */

#include	"idl.h"
#include	"idl_extern.h"

#include	"fe_private.h"

/*
 * Constructor(s) and destructor
 */

FE_InterfaceHeader::FE_InterfaceHeader(UTL_ScopedName *n, UTL_NameList *nl)
		  : pd_interface_name(n)
{
  compile_inheritance(nl);
}

/*
 * Private operations
 */

// Compute flattened, non-repeating list of inherited interfaces

#undef	INCREMENT
#define	INCREMENT	512

/*
 * Private storage used to store interfaces seen already in the
 * computation of the unique, flattened inheritance list
 */
static AST_Interface **iseen = NULL;
static long	    iallocated = 0;
static long	    iused = 0;

/*
 * Add an interface to an inheritance spec
 */
static void
add_inheritance(AST_Interface *i)
{
  long			oiallocated;
  long			k;
  AST_Interface	      **oiseen;

  /*
   * Make sure there's space for one more
   */
  if (iallocated == iused) {
    if (iallocated == 0) {
      iallocated = INCREMENT;
      iseen = new AST_Interface *[iallocated];
    } else {
      oiallocated = iallocated;
      oiseen = iseen;
      iallocated += INCREMENT;
      iseen = new AST_Interface *[iallocated];
      for (k = 0; k < iused; k++)
	iseen[k] = oiseen[k];
      delete oiseen;
    }
  }
  /*
   * OK, now insert it
   */
  iseen[iused] = i;
  iused++;
}

/*
 * Have we already seen this interface?
 */
static long
already_seen(AST_Interface *ip)
{
  long	i;

  for (i = 0; i < iused; i++) {
    if (iseen[i] == ip)
      return I_TRUE;
  }
  return I_FALSE;
}

/*
 * Add this interface to the list of inherited if not already there
 */
void
FE_InterfaceHeader::compile_one_inheritance(AST_Interface *i)
{
  /*
   * Check for badly formed interface
   */
  if (i == NULL)
    return;
  /*
   * If we've seen it already then don't expand again
   */
  if (already_seen(i))
    return;
  /*
   * OK, add i to the list of inherited interfaces
   */
  add_inheritance(i);
}

/*
 * Compute the list of top-level interfaces this one inherits from
 */
void
FE_InterfaceHeader::compile_inheritance(UTL_NameList *nl)
{
  UTL_NamelistActiveIterator *l = new UTL_NamelistActiveIterator(nl);
  AST_Decl		       *d;
  AST_Interface		       *i;
  long			        k;

  iused = 0;
  /*
   * Compute expanded flattened non-repeating list of interfaces
   * which this one inherits from
   */
  while (!(l->is_done())) {
    /*
     * Check that scope stack is valid
     */
    if (idl_global->scopes()->top() == NULL) {
      idl_global->err()->lookup_error(l->item());
      return;
    }
    /*
     * Look it up
     */
    d = idl_global->scopes()->top()->lookup_by_name(l->item(), I_TRUE);
    /*
     * Not found?
     */
    if (d == NULL) {
      idl_global->err()->lookup_error(l->item());
      return;
    }
    /*
     * Not an interface?
     */
    while(d->node_type() == AST_Decl::NT_typedef) 
      d = AST_Typedef::narrow_from_decl(d)->base_type();
    if (d->node_type() != AST_Decl::NT_interface) {
      idl_global->err()->inheritance_error(pd_interface_name, d);
      return;
    }
    /*
     * OK, cast to an interface
     */
    i = AST_Interface::narrow_from_decl(d);
    if (i == NULL)
       idl_global->err()->inheritance_error(pd_interface_name, d);
    /*
     * Forward declared interface?
     */
    if (!i->is_defined()) {
      idl_global->err()->inheritance_fwd_error(pd_interface_name, i);
      return;
    }
    /*
     * OK, see if we have to add this to the list of interfaces
     * inherited from
     */
    compile_one_inheritance(i);

    /*
     * Next element in header list
     */
    l->next();
  }
  delete l;
  /*
   * OK, install in interface header
   */
  pd_inherits = new AST_Interface *[iused];
  for (k = 0; k < iused; k++)
    pd_inherits[k] = iseen[k];
  pd_n_inherits = iused;
}

/*
 * Public operations
 */

/*
 * Redefinition of inherited virtual operations
 */

/*
 * Data accessors
 */

UTL_ScopedName *
FE_InterfaceHeader::interface_name()
{
  return pd_interface_name;
}

AST_Interface **
FE_InterfaceHeader::inherits()
{
  return pd_inherits;
}

long
FE_InterfaceHeader::n_inherits()
{
  return pd_n_inherits;
}
