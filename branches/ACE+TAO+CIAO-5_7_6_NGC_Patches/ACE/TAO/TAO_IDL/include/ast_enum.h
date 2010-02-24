// This may look like C, but it's really -*- C++ -*-
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

#ifndef _AST_ENUM_AST_ENUM_HH
#define _AST_ENUM_AST_ENUM_HH

#include "ast_concrete_type.h"
#include "utl_scope.h"

class AST_EnumVal;

class TAO_IDL_FE_Export AST_Enum : public virtual AST_ConcreteType,
                                   public virtual UTL_Scope
{
public:
  AST_Enum (void);
  AST_Enum (UTL_ScopedName *n,
            bool local,
            bool abstract);

  virtual ~AST_Enum (void);

  // Narrowing

  DEF_NARROW_FROM_DECL(AST_Enum);
  DEF_NARROW_FROM_SCOPE(AST_Enum);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &);

  // Look up an AST_EnumVal by value
  AST_EnumVal *lookup_by_value (const AST_Expression *v);

  // Get value to be assigned to next enumerator.
  unsigned long next_enum_val (void);

  // Return the count of members
  virtual int member_count (void);

  // Convert a numeric value to the enum's string name for it.
  UTL_ScopedName *value_to_name (const unsigned long v);

  // Cleanup function.
  virtual void destroy (void);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

private:
  friend int tao_yyparse (void);

  unsigned long pd_enum_counter;
  // Value for next enumerator.

  int member_count_;
  // Number of members.

  int compute_member_count (void);
  // Count the number of members.

  virtual AST_EnumVal *fe_add_enum_val (AST_EnumVal *v);
  // Scope Management.
};

#endif           // _AST_ENUM_AST_ENUM_HH
