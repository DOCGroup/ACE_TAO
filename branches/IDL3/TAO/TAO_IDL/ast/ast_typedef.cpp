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

// AST_Typedef nodes represent an IDL typedef statement.
// AST_Typedef is a subclass of AST_Decl (it is not a type,
// but instead is a type renaming).
// AST_Typedef nodes have a base type (a subclass of AST_Type)
// and a name (an UTL_ScopedName).

#include "ast_typedef.h"
#include "ast_visitor.h"
#include "utl_identifier.h"

ACE_RCSID (ast, 
           ast_typedef, 
           "$Id$")

AST_Typedef::AST_Typedef (void)
 : pd_base_type (0)
{
}

AST_Typedef::AST_Typedef (AST_Type *bt,
                          UTL_ScopedName *n,
                          idl_bool local,
                          idl_bool abstract)
 : AST_Type (AST_Decl::NT_typedef,
             n),
   AST_Decl (AST_Decl::NT_typedef,
             n),
   COMMON_Base (bt->is_local () || local,
                abstract),
   pd_base_type (bt)
{
}

AST_Typedef::~AST_Typedef (void)
{
}

// Given a typedef node, traverse the chain of base types until they are no
// more typedefs, and return that most primitive base type.
AST_Type *
AST_Typedef::primitive_base_type (void)
{
  AST_Type *d = this;
  AST_Typedef *temp = 0;

  while (d && d->node_type () == AST_Decl::NT_typedef)
    {
      temp = AST_Typedef::narrow_from_decl (d);
      d = AST_Type::narrow_from_decl (temp->base_type ());
    }

  return d;
}

// Redefinition of inherited virtual operations.

// Dump this AST_Typedef node to the ostream o.
void
AST_Typedef::dump (ACE_OSTREAM_TYPE&o)
{
  if (this->is_local ())
    {
      o << "(local) ";
    }
  else
    {
      o << "(abstract) ";
    }

  o << "typedef ";
  this->pd_base_type->dump (o);
  o << " ";
  this->local_name ()->dump (o);
}

int
AST_Typedef::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_typedef (this);
}

void
AST_Typedef::destroy (void)
{
}

// Data accessors.

AST_Type *
AST_Typedef::base_type (void)
{
  return this->pd_base_type;
}

// Narrowing.
IMPL_NARROW_METHODS1(AST_Typedef, AST_Type)
IMPL_NARROW_FROM_DECL(AST_Typedef)
