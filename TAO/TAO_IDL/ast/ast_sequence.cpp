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

// AST_Sequence nodes represent IDL sequence declarations.
// AST_Sequence is a subclass of AST_ConcreteType.
// AST_Sequence nodes have a maximum size (an AST_Expression which
// must evaluate to a positive integer) and a base type (a subclass
// of AST_Type).

#include "ast_sequence.h"
#include "ast_expression.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "ace/Log_Msg.h"

ACE_RCSID (ast, 
           ast_sequence, 
           "$Id$")

AST_Sequence::AST_Sequence (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    AST_ConcreteType (),
    pd_max_size (0),
    pd_base_type (0)
{
  // A sequence data type is always VARIABLE.
  this->size_type (AST_Type::VARIABLE);
}

AST_Sequence::AST_Sequence (AST_Expression *ms,
                            AST_Type *bt,
                            UTL_ScopedName *n,
                            idl_bool local,
                            idl_bool abstract)
  : COMMON_Base (bt->is_local () || local,
                 abstract),
    AST_Decl (AST_Decl::NT_sequence,
              n,
              I_TRUE),
    AST_Type (AST_Decl::NT_sequence,
              n),
    AST_ConcreteType (AST_Decl::NT_sequence,
                      n),
    pd_max_size (ms),
    pd_base_type (bt)
{
  // Check if we are bounded or unbounded. An expression value of 0 means
  // unbounded.
  if (ms->ev ()->u.ulval == 0)
    {
      this->unbounded_ = I_TRUE;
    }
  else
    {
      this->unbounded_ = I_FALSE;
    }

  // A sequence data type is always VARIABLE.
  this->size_type (AST_Type::VARIABLE);
}

AST_Sequence::~AST_Sequence (void)
{
}

// Public operations.

idl_bool
AST_Sequence::in_recursion (AST_Type *node)
{
  if (node == 0)
    {
      // There has to be a parameter.
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) AST_Sequence::")
                         ACE_TEXT ("in_recursion - ")
                         ACE_TEXT ("bad parameter node\n")),
                        0);
    }

  AST_Type *type = AST_Type::narrow_from_decl (this->base_type ());

  if (!type)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) AST_Sequence::")
                         ACE_TEXT ("in_recursion - ")
                         ACE_TEXT ("bad base type\n")),
                        0);
    }

  if (!ACE_OS::strcmp (node->full_name (),
                       type->full_name ()))
    {
      // They match.
      return 1;
    }
  else
    {
      // Not in recursion.
      return 0;
    }
}

// Redefinition of inherited virtual operations.

// Dump this AST_Sequence node to the ostream o.
void
AST_Sequence::dump (ACE_OSTREAM_TYPE &o)
{
  o << "sequence <";
  this->pd_base_type->dump (o);
  o << ", ";
  this->pd_max_size->dump (o);
  o << ">";
}

int
AST_Sequence::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_sequence (this);
}

// Data accessors.

AST_Expression *
AST_Sequence::max_size (void)
{
  return this->pd_max_size;
}

AST_Type *
AST_Sequence::base_type (void)
{
  return this->pd_base_type;
}

idl_bool
AST_Sequence::unbounded (void) const
{
  return this->unbounded_;
}

// Narrowing.
IMPL_NARROW_METHODS1(AST_Sequence, AST_ConcreteType)
IMPL_NARROW_FROM_DECL(AST_Sequence)
