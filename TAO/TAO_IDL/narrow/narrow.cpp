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
#include "ast_porttype.h"
#include "ast_connector.h"
#include "ast_home.h"
#include "ast_operation.h"
#include "ast_factory.h"
#include "ast_finder.h"
#include "ast_exception.h"
#include "ast_union.h"
#include "ast_enum.h"
#include "ast_interface_fwd.h"
#include "utl_scope.h"
#include "ast_annotation_decl.h"
#include "ast_annotation_appl.h"

// Narrowing functions for AST

TAO_IDL_FE_Export AST_Decl *
ScopeAsDecl (UTL_Scope *s)
{
  if (s == nullptr)
    {
      return nullptr;
    }

  switch (s->scope_node_type ())
  {
    case AST_Decl::NT_module:
      return dynamic_cast<AST_Module*> (s);
    case AST_Decl::NT_root:
      return dynamic_cast<AST_Root*> (s);
    case AST_Decl::NT_interface:
      return dynamic_cast<AST_Interface*> (s);
    case AST_Decl::NT_valuetype:
      return dynamic_cast<AST_ValueType*> (s);
    case AST_Decl::NT_eventtype:
      return dynamic_cast<AST_EventType*> (s);
    case AST_Decl::NT_component:
      return dynamic_cast<AST_Component*> (s);
    case AST_Decl::NT_porttype:
      return dynamic_cast<AST_PortType*> (s);
    case AST_Decl::NT_connector:
      return dynamic_cast<AST_Connector*> (s);
    case AST_Decl::NT_home:
      return dynamic_cast<AST_Home*> (s);
    case AST_Decl::NT_op:
      return dynamic_cast<AST_Operation*> (s);
    case AST_Decl::NT_factory:
      return dynamic_cast<AST_Factory*> (s);
    case AST_Decl::NT_finder:
      return dynamic_cast<AST_Finder*> (s);
    case AST_Decl::NT_except:
      return dynamic_cast<AST_Exception*> (s);
    case AST_Decl::NT_union:
      return dynamic_cast<AST_Union*> (s);
    case AST_Decl::NT_struct:
      return dynamic_cast<AST_Structure*> (s);
    case AST_Decl::NT_enum:
      return dynamic_cast<AST_Enum*> (s);
    case AST_Decl::NT_annotation_decl:
      return dynamic_cast<AST_Annotation_Decl*> (s);
    case AST_Decl::NT_annotation_appl:
      return dynamic_cast<AST_Annotation_Appl*> (s);
    default:
      return nullptr;
  }
}

/*
 * Convert an AST_Decl to a UTL_Scope if possible
 */
TAO_IDL_FE_Export UTL_Scope *
DeclAsScope (AST_Decl *d)
{
  if (d == nullptr)
    {
      return nullptr;
    }

  switch (d->node_type ())
  {
    case AST_Decl::NT_interface_fwd:
      /*
       * Resolve forward declared interface by looking at full_definition()
       * field and iterating
       */
      d = (dynamic_cast<AST_InterfaceFwd*> (d))->full_definition ();
      ACE_FALLTHROUGH;
    case AST_Decl::NT_interface:
      return dynamic_cast<AST_Interface*> (d);
    case AST_Decl::NT_valuetype:
      return dynamic_cast<AST_ValueType*> (d);
    case AST_Decl::NT_eventtype:
      return dynamic_cast<AST_EventType*> (d);
    case AST_Decl::NT_component:
      return dynamic_cast<AST_Component*> (d);
    case AST_Decl::NT_porttype:
      return dynamic_cast<AST_PortType*> (d);
    case AST_Decl::NT_connector:
      return dynamic_cast<AST_Connector*> (d);
    case AST_Decl::NT_home:
      return dynamic_cast<AST_Home*> (d);
    case AST_Decl::NT_module:
      return dynamic_cast<AST_Module*> (d);
    case AST_Decl::NT_root:
      return dynamic_cast<AST_Root*> (d);
    case AST_Decl::NT_except:
      return dynamic_cast<AST_Exception*> (d);
    case AST_Decl::NT_union:
      return dynamic_cast<AST_Union*> (d);
    case AST_Decl::NT_struct:
      return dynamic_cast<AST_Structure*> (d);
    case AST_Decl::NT_enum:
      return dynamic_cast<AST_Enum*> (d);
    case AST_Decl::NT_op:
      return dynamic_cast<AST_Operation*> (d);
    case AST_Decl::NT_factory:
      return dynamic_cast<AST_Factory*> (d);
    case AST_Decl::NT_finder:
      return dynamic_cast<AST_Finder*> (d);
    default:
      return nullptr;
  }
}
