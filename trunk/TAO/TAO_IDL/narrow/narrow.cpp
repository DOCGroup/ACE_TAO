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

#include	"idl.h"
#include	"idl_extern.h"

// Narrowing functions for AST

/*
 * Convert a UTL_Scope node into an AST_Decl node if possible
 */
AST_Decl *
ScopeAsDecl(UTL_Scope *s)
{
  if (s == NULL) return NULL;

  switch (s->scope_node_type()) {
  case AST_Decl::NT_module:
    return AST_Module::narrow_from_scope(s);
  case AST_Decl::NT_root:
    return AST_Root::narrow_from_scope(s);
  case AST_Decl::NT_interface:
    return AST_Interface::narrow_from_scope(s);
  case AST_Decl::NT_op:
    return AST_Operation::narrow_from_scope(s);
  case AST_Decl::NT_except:
    return AST_Exception::narrow_from_scope(s);
  case AST_Decl::NT_union:
    return AST_Union::narrow_from_scope(s);
  case AST_Decl::NT_struct:
    return AST_Structure::narrow_from_scope(s);
  case AST_Decl::NT_enum:
    return AST_Enum::narrow_from_scope(s);
  default:
    return NULL;
  }
}

/*
 * Convert an AST_Decl to a UTL_Scope if possible
 */
UTL_Scope *
DeclAsScope(AST_Decl *d)
{
   if (d == NULL) return NULL;

   switch (d->node_type()) {
   case AST_Decl::NT_interface_fwd:
      /*
       * Resolve forward declared interface by looking at full_definition()
       * field and iterating
       */
      d = (AST_InterfaceFwd::narrow_from_decl(d))->full_definition();
      /*
       * Fall through
       */
   case AST_Decl::NT_interface:
      return AST_Interface::narrow_from_decl(d);
   case AST_Decl::NT_module:
      return AST_Module::narrow_from_decl(d);
   case AST_Decl::NT_root:
      return AST_Root::narrow_from_decl(d);
   case AST_Decl::NT_except:
      return AST_Exception::narrow_from_decl(d);
   case AST_Decl::NT_union:
      return AST_Union::narrow_from_decl(d);
   case AST_Decl::NT_struct:
      return AST_Structure::narrow_from_decl(d);
   case AST_Decl::NT_enum:
      return AST_Enum::narrow_from_decl(d);
   case AST_Decl::NT_op:
      return AST_Operation::narrow_from_decl(d);
   default:
      return NULL;
   }
}
