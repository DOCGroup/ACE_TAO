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

// AST_InterfaceFwd nodes denote forward declarations of IDL interfaces
// AST_InterfaceFwd nodes have a field containing the full declaration
// of the interface, which is initialized when that declaration is
// encountered.

#include "idl.h"
#include  "idl_extern.h"

ACE_RCSID(ast, ast_interface_fwd, "$Id$")

// Constructor(s) and destructor.
AST_InterfaceFwd::AST_InterfaceFwd (void)
  : pd_full_definition (0)
{
}

AST_InterfaceFwd::AST_InterfaceFwd (AST_Interface *dummy,
                                    UTL_ScopedName *n)
  : AST_Decl (AST_Decl::NT_interface_fwd, 
              n)
{
  // Create a dummy placeholder for the forward declared interface. This
  // interface node is not yet defined (n_inherits < 0), so some operations
  // will fail.
  pd_full_definition = dummy;
}

AST_InterfaceFwd::~AST_InterfaceFwd (void)
{
}

// Private operations.

idl_bool
AST_InterfaceFwd::is_local (void)
{
  return this->full_definition ()->is_local ();
}

idl_bool AST_InterfaceFwd::is_valuetype (void)
{
  return this->full_definition ()->is_valuetype ();
}

idl_bool
AST_InterfaceFwd::is_abstract_valuetype (void)
{
  return this->full_definition ()->is_abstract_valuetype ();
}

void
AST_InterfaceFwd::set_abstract_valuetype (void)
{
  this->full_definition ()->set_abstract_valuetype ();
}

// Redefinition of inherited virtual operations.

// Dump this AST_InterfaceFwd node to the ostream o.
void
AST_InterfaceFwd::dump (ACE_OSTREAM_TYPE &o)
{
  if (this->is_valuetype ())
    {
      if (this->is_abstract_valuetype ())
        {
          o << "abstract ";
        }

      o << "valuetype ";
    }
  else
    {
      if (this->is_abstract ())
        {
          o << "abstract ";
        }
      else if (this->is_local ())
        {
          o << "local ";
        }

      o << "interface ";
    }

  this->local_name ()->dump (o);
}

int
AST_InterfaceFwd::ast_accept (ast_visitor *visitor)
{
  if (this->is_valuetype ())
    {
      return visitor->visit_valuetype_fwd (this);
    }
  else
    {
      return visitor->visit_interface_fwd (this);
    }
}

// Data accessors.

AST_Interface *
AST_InterfaceFwd::full_definition (void)
{
  return this->pd_full_definition;
}

void
AST_InterfaceFwd::set_full_definition (AST_Interface *nfd)
{
  this->pd_full_definition = nfd;
}

idl_bool
AST_InterfaceFwd::is_defined (void)
{
  return this->pd_full_definition->is_defined ();
}

// Narrowing methods.
IMPL_NARROW_METHODS1 (AST_InterfaceFwd, AST_Type)
IMPL_NARROW_FROM_DECL (AST_InterfaceFwd)
