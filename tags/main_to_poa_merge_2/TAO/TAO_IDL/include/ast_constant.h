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

#ifndef _AST_CONSTANT_AST_CONSTANT_HH
#define _AST_CONSTANT_AST_CONSTANT_HH

// Representation of constant declaration:
//
// NOTE: Part of the job of the constructor is to convert
// the AST_Constant_val it received to the type denoted by
// the ConstantType it received.
//
// NOTE: There are two constructors here, one that gets a NodeType
// parameter. This constructor is used from AST_EnumVal, which is
// just a glorified AST_Constant

/*
** DEPENDENCIES: ast_decl.hh, ast_expression.hh, utl_scoped_name.hh,
**		 utl_strlist.hh
**
** USE: Included from ast.hh
*/

class	AST_Constant : public virtual AST_Decl
{
public:
  // Operations

  // Constructor(s)
  AST_Constant();
  AST_Constant(AST_Expression::ExprType et, AST_Decl::NodeType nt,
	       AST_Expression *ev, UTL_ScopedName *n, UTL_StrList *p);
  AST_Constant(AST_Expression::ExprType et, AST_Expression *ev,
	       UTL_ScopedName *n, UTL_StrList *p);
  virtual ~AST_Constant() {}

  // Data Accessors
  AST_Expression *constant_value();
  AST_Expression::ExprType et();

  // Narrowing
  DEF_NARROW_METHODS1(AST_Constant, AST_Decl);
  DEF_NARROW_FROM_DECL(AST_Constant);

  // AST Dumping
  virtual void			dump(ostream &o);

private:
  // Data
  AST_Expression		*pd_constant_value;	// The value
  AST_Expression::ExprType	pd_et;			// Its expr type
};

#endif           // _AST_CONSTANT_AST_CONSTANT_HH
