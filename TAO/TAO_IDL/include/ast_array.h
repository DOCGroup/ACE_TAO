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

#ifndef _AST_ARRAY_AST_ARRAY_HH
#define _AST_ARRAY_AST_ARRAY_HH

// Representation of array declaration:
//
// An array is a combination of a list of dimensions and a base type

/*
** DEPENDENCIES: ast_concrete_type.hh, utl_exprlist.hh, ast_type.hh,
**		 ast_decl.hh
**
** USE: Included from ast.hh
*/

class	AST_Array : public virtual AST_ConcreteType {
public:
  // Operations

  // Constructor(s)
  AST_Array();
  AST_Array(UTL_ScopedName *n, unsigned long ndims, UTL_ExprList *dims);
  virtual ~AST_Array() {}

  // Data Accessors
  const unsigned long n_dims();
  AST_Expression **dims();
  void		   set_dims(AST_Expression **, unsigned long);
  AST_Type	  *base_type();
  void		   set_base_type(AST_Type *nbt);

  // Narrowing
  DEF_NARROW_METHODS1(AST_Array, AST_ConcreteType);
  DEF_NARROW_FROM_DECL(AST_Array);

  // AST Dumping
  virtual void			dump(ostream &o);

private:
  // Data
  unsigned long			pd_n_dims;	// How many dimensions?
  AST_Expression		**pd_dims;	// Their expressions
  AST_Type			*pd_base_type;	// Base type of array

  // Operations

  // Compute how many dimensions
  AST_Expression	       **compute_dims(UTL_ExprList *dims,
					     unsigned long ndims);
};

#endif           // _AST_ARRAY_AST_ARRAY_HH
