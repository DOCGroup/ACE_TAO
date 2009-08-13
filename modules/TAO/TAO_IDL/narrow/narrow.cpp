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

#include "ast_module.h"
#include "ast_root.h"
#include "ast_valuetype.h"
#include "ast_eventtype.h"
#include "ast_component.h"
#include "ast_home.h"
#include "ast_operation.h"
#include "ast_factory.h"
#include "ast_exception.h"
#include "ast_union.h"
#include "ast_enum.h"
#include "ast_interface_fwd.h"
#include "utl_scope.h"

ACE_RCSID (narrow, 
           narrow, 
           "$Id$")

// Narrowing functions for AST

TAO_IDL_FE_Export AST_Decl *
ScopeAsDecl (UTL_Scope *s)
{
  if (s == 0) return 0;

  switch (s->scope_node_type ())
  {
    case AST_Decl::NT_module:
      return AST_Module::narrow_from_scope (s);
    case AST_Decl::NT_root:
      return AST_Root::narrow_from_scope (s);
    case AST_Decl::NT_interface:
      return AST_Interface::narrow_from_scope (s);
    case AST_Decl::NT_valuetype:
      return AST_ValueType::narrow_from_scope (s);
    case AST_Decl::NT_eventtype:
      return AST_EventType::narrow_from_scope (s);
    case AST_Decl::NT_component:
      return AST_Component::narrow_from_scope (s);
    case AST_Decl::NT_home:
      return AST_Home::narrow_from_scope (s);
    case AST_Decl::NT_op:
      return AST_Operation::narrow_from_scope (s);
    case AST_Decl::NT_factory:
      return AST_Factory::narrow_from_scope (s);
    case AST_Decl::NT_except:
      return AST_Exception::narrow_from_scope (s);
    case AST_Decl::NT_union:
      return AST_Union::narrow_from_scope (s);
    case AST_Decl::NT_struct:
      return AST_Structure::narrow_from_scope (s);
    case AST_Decl::NT_enum:
      return AST_Enum::narrow_from_scope (s);
    default:
      return 0;
  }
}

/*
 * Convert an AST_Decl to a UTL_Scope if possible
 */
TAO_IDL_FE_Export UTL_Scope *
DeclAsScope (AST_Decl *d)
{
   if (d == 0) return 0;

   switch (d->node_type ())
   {
     case AST_Decl::NT_interface_fwd:
        /*
         * Resolve forward declared interface by looking at full_definition()
         * field and iterating
         */
        d = (AST_InterfaceFwd::narrow_from_decl (d))->full_definition ();
        /*
         * Fall through
         */
     case AST_Decl::NT_interface:
        return AST_Interface::narrow_from_decl (d);
     case AST_Decl::NT_valuetype:
        return AST_ValueType::narrow_from_decl (d);
     case AST_Decl::NT_eventtype:
        return AST_EventType::narrow_from_decl (d);
     case AST_Decl::NT_component:
        return AST_Component::narrow_from_decl (d);
     case AST_Decl::NT_home:
        return AST_Home::narrow_from_decl (d);
     case AST_Decl::NT_module:
        return AST_Module::narrow_from_decl (d);
     case AST_Decl::NT_root:
        return AST_Root::narrow_from_decl (d);
     case AST_Decl::NT_except:
        return AST_Exception::narrow_from_decl (d);
     case AST_Decl::NT_union:
        return AST_Union::narrow_from_decl (d);
     case AST_Decl::NT_struct:
        return AST_Structure::narrow_from_decl (d);
     case AST_Decl::NT_enum:
        return AST_Enum::narrow_from_decl (d);
     case AST_Decl::NT_op:
        return AST_Operation::narrow_from_decl (d);
     case AST_Decl::NT_factory:
        return AST_Factory::narrow_from_decl (d);
     default:
        return 0;
   }
}
