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

// AST_Argument nodes denote arguments to an operation. They
// are AST_Fields with directions. Thus, they have a name
// (an UTL_ScopedName) and a type (a subclass of AST_Type).
// Additionally they have a direction field which takes values
// from the enum AST_Argument::Direction.

#include "ast_argument.h"
#include "ast_visitor.h"

ACE_RCSID (ast, 
           ast_argument, 
           "$Id$")

// Static functions.

// Convert an enum Direction value to a char *.
static const char *
direction_to_string (AST_Argument::Direction d)
{
  switch (d)
    {
    case AST_Argument::dir_IN:
      return "in";
    case AST_Argument::dir_OUT:
      return "out";
    case AST_Argument::dir_INOUT:
      return "inout";
    }

  return 0;
}

AST_Argument::AST_Argument (void)
  : pd_direction (dir_IN)
{
}

AST_Argument::AST_Argument (Direction d,
                            AST_Type *ft,
                            UTL_ScopedName *n)
  : AST_Field (AST_Decl::NT_argument,
               ft,
               n),
	  AST_Decl (AST_Decl::NT_argument,
              n),
	  pd_direction (d)
{
}

AST_Argument::~AST_Argument (void)
{
}

// Dump this AST_Argument node to the ostream o.
void
AST_Argument::dump (ostream &o)
{
  o << direction_to_string (pd_direction) << " ";
  AST_Field::dump (o);
}

int
AST_Argument::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_argument (this);
}

AST_Argument::Direction
AST_Argument::direction (void)
{
  return this->pd_direction;
}

// Narrowing operations.
IMPL_NARROW_METHODS1(AST_Argument, AST_Field)
IMPL_NARROW_FROM_DECL(AST_Argument)
