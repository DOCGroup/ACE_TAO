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

// AST_Fields denote fields in IDL structure, union and exception
// declarations. AST_Field is also used as a superclass of AST_Argument
// and AST_UnionBranch.
// AST_Fields have a field type (a subclass of AST_Type) and a name
// (a UTL_ScopedName).

// AST_Field supplies two constructors, one to be used in constructing
// AST_Field nodes, the other to be used in constructing AST_Argument
// nodes and AST_UnionBranch nodes.

#include "ast_field.h"
#include "ast_type.h"
#include "ast_visitor.h"
#include "utl_identifier.h"

ACE_RCSID (ast, ast_field, "$Id$")

AST_Field::AST_Field (void)
  : COMMON_Base (),
    AST_Decl (),
    pd_field_type (0),
    pd_visibility (vis_NA),
    anonymous_type_ (false)
{
}

// To be used when constructing an AST_Field node.
AST_Field::AST_Field (AST_Type *ft,
                      UTL_ScopedName *n,
                      Visibility vis)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_field,
              n),
    pd_field_type (ft),
    pd_visibility (vis),
    anonymous_type_ (false)
{
  AST_Decl::NodeType fnt = ft->node_type ();
  
  if (AST_Decl::NT_array == fnt || AST_Decl::NT_sequence == fnt)
    {
      this->anonymous_type_ = true;
    }
}

// To be used when constructing a node of a subclass of AST_Field.
AST_Field::AST_Field (AST_Decl::NodeType nt,
                      AST_Type *ft,
                      UTL_ScopedName *n,
                      Visibility vis)
  : COMMON_Base (),
    AST_Decl (nt,
              n),
    pd_field_type (ft),
    pd_visibility (vis),
    anonymous_type_ (false)
{
  AST_Decl::NodeType fnt = ft->node_type ();
  
  if (AST_Decl::NT_array == fnt || AST_Decl::NT_sequence == fnt)
    {
      this->anonymous_type_ = true;
    }
}

AST_Field::~AST_Field (void)
{
}

// Dump this AST_Field node to the ostream o.
void
AST_Field::dump (ACE_OSTREAM_TYPE &o)
{
  switch (this->pd_visibility)
    {
    case vis_PRIVATE:
      this->dump_i (o, "private ");

      break;
    case vis_PUBLIC:
      this->dump_i (o, "public ");

      break;
    case vis_NA:
      break;
    }

  this->pd_field_type->local_name ()->dump (o);

  this->dump_i (o, " ");

  this->local_name ()->dump (o);
}

int
AST_Field::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_field (this);
}

void
AST_Field::destroy (void)
{
  if (this->anonymous_type_)
    {
      this->pd_field_type->destroy ();
      delete this->pd_field_type;
      this->pd_field_type = 0;
    }

  this->AST_Decl::destroy ();
}

AST_Type *
AST_Field::field_type (void) const
{
  return this->pd_field_type;
}

AST_Field::Visibility
AST_Field::visibility (void)
{
  return this->pd_visibility;
}

int
AST_Field::contains_wstring (void)
{
  return this->pd_field_type->contains_wstring ();
}



IMPL_NARROW_FROM_DECL(AST_Field)
