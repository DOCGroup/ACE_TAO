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

#pragma ident "%@(#)ast_interface_fwd.cc	1.44% %92/06/11% Sun Microsystems"

/*
 * ast_interface_fwd.cc - Implementation of class AST_InterfaceFwd
 *
 * AST_InterfaceFwd nodes denote forward declarations of IDL interfaces
 * AST_InterfaceFwd nodes have a field containing the full declaration
 * of the interface, which is initialized when that declaration is
 * encountered.
 */

#include	"idl.h"
#include	"idl_extern.h"

/*
 * Constructor(s) and destructor
 */
AST_InterfaceFwd::AST_InterfaceFwd()
		: pd_full_definition(NULL)
{
}

AST_InterfaceFwd::AST_InterfaceFwd(UTL_ScopedName *n, UTL_StrList *p)
		 : AST_Decl(AST_Decl::NT_interface_fwd, n, p)
{
  /*
   * Create a dummy placeholder for the forward declared interface. This
   * interface node is not yet defined (n_inherits < 0), so some operations
   * will fail
   */
  pd_full_definition = idl_global->gen()->create_interface(n, NULL, -1, p);
  /*
   * Record the node in a list to be checked after the entire AST has been
   * parsed. All nodes in the list must have n_inherits >= 0, else this
   * indicates that a full definition was not seen for this forward
   * delcared interface
   */
  AST_record_fwd_interface(this);
}

/*
 * Private operations
 */

/*
 * Public operations
 */

/*
 * Redefinition of inherited virtual operations
 */

/*
 * Dump this AST_InterfaceFwd node to the ostream o
 */
void
AST_InterfaceFwd::dump(ostream &o)
{
  o << "interface ";
  local_name()->dump(o);
}

/*
 * Data accessors
 */

AST_Interface	*
AST_InterfaceFwd::full_definition()
{
  return pd_full_definition;
}

void
AST_InterfaceFwd::set_full_definition(AST_Interface *nfd)
{
  pd_full_definition = nfd;
}    

/*
 * Narrowing methods 
 */
IMPL_NARROW_METHODS1 (AST_InterfaceFwd, AST_Type)
IMPL_NARROW_FROM_DECL (AST_InterfaceFwd)
