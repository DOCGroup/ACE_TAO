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

// AST_Type is the base class for all AST classes which represent
// IDL type constructs.

#include "ast_type.h"
#include "ast_visitor.h"
#include "ace/Log_Msg.h"

ACE_RCSID (ast, 
           ast_type, 
           "$Id$")

AST_Type::AST_Type (void)
  : ifr_added_ (0),
    ifr_fwd_added_ (0),
    size_type_ (AST_Type::SIZE_UNKNOWN),
    has_constructor_ (0)
{
}

AST_Type::AST_Type (AST_Decl::NodeType nt,
                    UTL_ScopedName *n)
  : AST_Decl (nt,
              n),
    ifr_added_ (0),
    ifr_fwd_added_ (0),
    size_type_ (AST_Type::SIZE_UNKNOWN),
    has_constructor_ (0)
{
}

AST_Type::~AST_Type (void)
{
}

// Public operations.

// Return our size type.
AST_Type::SIZE_TYPE
AST_Type::size_type (void)
{
  if (this->size_type_ == AST_Type::SIZE_UNKNOWN)
    {
      (void) this->compute_size_type ();
    }

  return this->size_type_;
}

// Set our size type and that of all our ancestors.
void
AST_Type::size_type (AST_Type::SIZE_TYPE st)
{
  // Precondition - you cannot set somebody's sizetype to unknown.
  ACE_ASSERT (st != AST_Type::SIZE_UNKNOWN);

  // Size type can be VARIABLE or FIXED.
  if (this->size_type_ == AST_Type::SIZE_UNKNOWN) // not set yet
    {
      this->size_type_ = st; // set it
    }
  else if ((this->size_type_ == AST_Type::FIXED)
           && (st == AST_Type::VARIABLE))
    {
      // Once we are VARIABLE, we cannot be FIXED. But if we were FIXED and then
      // get overwritten to VARIABLE, it is fine. Such a situation occurs only
      // when setting the sizes of structures and unions.
      this->size_type_ = st;
    }
}

// Compute the size type of the node in question
int
AST_Type::compute_size_type (void)
{
  return 0;
}

idl_bool
AST_Type::in_recursion (AST_Type *)
{
  // By default we are not involved in recursion.
  return 0;
}

idl_bool
AST_Type::is_defined (void)
{
  // AST_Interface, AST_Structure, and AST_Union will
  // override this, as will AST_InterfaceFwd, etc.
  return 1;
}

idl_bool
AST_Type::ifr_added (void)
{
  return this->ifr_added_;
}

void
AST_Type::ifr_added (idl_bool val)
{
  this->ifr_added_ = val;
}

idl_bool
AST_Type::ifr_fwd_added (void)
{
  return this->ifr_fwd_added_;
}

void
AST_Type::ifr_fwd_added (idl_bool val)
{
  this->ifr_fwd_added_ = val;
}

idl_bool
AST_Type::has_constructor (void)
{
  return this->has_constructor_;
}

void
AST_Type::has_constructor (idl_bool value)
{
  // Similarly to be_decl::size_type_, once this
  // gets set to I_TRUE, we don't want it to
  // change back.
  if (this->has_constructor_ == 0)
    {
      this->has_constructor_ = value;
    }
}

int
AST_Type::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_type (this);
}

void
AST_Type::destroy (void)
{
  this->AST_Decl::destroy ();
}

// Narrowing.
IMPL_NARROW_METHODS1(AST_Type, AST_Decl)
IMPL_NARROW_FROM_DECL(AST_Type)
