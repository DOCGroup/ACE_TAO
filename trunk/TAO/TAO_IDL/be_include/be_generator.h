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

#ifndef _BE_GENERATOR_BE_GENERATOR_HH
#define _BE_GENERATOR_BE_GENERATOR_HH

// be_generator.hh
//
// Defines BE class for generator
//
// This defines the same protocol as the AST_Generator class but actually
// is implemented to create nodes of AST classes subclassed in this BE

/*
** DEPENDENCIES: AST_Generator.h
**
** USE: Included from be.h
*/

class	be_generator : public AST_Generator {
public:
  virtual AST_Root	*create_root(UTL_ScopedName *n,
				     UTL_StrList *p);
  // Create a node representing a module which is the root of an AST made up of
  // BE nodes

  // Operations
  virtual AST_PredefinedType
    *create_predefined_type(AST_PredefinedType::PredefinedType t,
			    UTL_ScopedName *n,
			    UTL_StrList *p);

  virtual AST_Module	*create_module(UTL_ScopedName *n, UTL_StrList *p);
  virtual AST_Interface	*create_interface(UTL_ScopedName *n,
					  AST_Interface **ih,
					  long nih,
					  UTL_StrList *p);
  virtual AST_InterfaceFwd *create_interface_fwd(UTL_ScopedName *n,
						 UTL_StrList *p);
  virtual AST_Exception	*create_exception(UTL_ScopedName *n, UTL_StrList *p);
  virtual AST_Structure	*create_structure(UTL_ScopedName *n, UTL_StrList *p);
  virtual AST_Enum	*create_enum(UTL_ScopedName *n, UTL_StrList *p);
  virtual AST_Operation	*create_operation(AST_Type *rt,
					  AST_Operation::Flags fl,
					  UTL_ScopedName *n,
					  UTL_StrList *p);
  virtual AST_Field	*create_field(AST_Type *ft, UTL_ScopedName *n,
				      UTL_StrList *p);
  virtual AST_Argument	*create_argument(AST_Argument::Direction d,
					 AST_Type *ft,
					 UTL_ScopedName *n,
					 UTL_StrList *p);
  virtual AST_Attribute	*create_attribute(idl_bool ro,
					  AST_Type *ft,
					  UTL_ScopedName *n,
					  UTL_StrList *p);
  virtual AST_Union	*create_union(AST_ConcreteType *dt,
				      UTL_ScopedName *n,
				      UTL_StrList *p);
  virtual AST_UnionBranch *create_union_branch(AST_UnionLabel *lab,
					       AST_Type *ft,
					       UTL_ScopedName *n,
					       UTL_StrList *p);
  virtual AST_UnionLabel *create_union_label(AST_UnionLabel::UnionLabel ul,
					     AST_Expression *lv);
  virtual AST_Constant	*create_constant(AST_Expression::ExprType et,
					 AST_Expression *ev,
					 UTL_ScopedName *n,
					 UTL_StrList *p);

  virtual AST_Expression	*create_expr(UTL_ScopedName *n);
  virtual AST_Expression	*create_expr(AST_Expression *v,
					     AST_Expression::ExprType t);
  virtual AST_Expression	*create_expr(AST_Expression::ExprComb c,
					     AST_Expression *v1,
					     AST_Expression *v2);
  virtual AST_Expression	*create_expr(long v);
  virtual AST_Expression	*create_expr(long v,
					     AST_Expression::ExprType t);
  virtual AST_Expression	*create_expr(unsigned long v);
  virtual AST_Expression	*create_expr(String *s);
  virtual AST_Expression	*create_expr(char c);
  virtual AST_Expression	*create_expr(double d);

  virtual AST_EnumVal	*create_enum_val(unsigned long v,
					 UTL_ScopedName *n,
					 UTL_StrList *p);
  virtual AST_Array	*create_array(UTL_ScopedName *n,
				      unsigned long ndims,
				      UTL_ExprList *dims);
  virtual AST_Sequence	*create_sequence(AST_Expression *v, AST_Type *bt);
  virtual AST_String	*create_string(AST_Expression *v);
  virtual AST_String	*create_wstring(AST_Expression *v);
  virtual AST_Typedef	*create_typedef(AST_Type *bt,
					UTL_ScopedName *n,
					UTL_StrList *p);
  // Create a node representing a native
  virtual AST_Native	*create_native (UTL_ScopedName *n,
                                        UTL_StrList *p);

};

#endif           // _BE_GENERATOR_BE_GENERATOR_HH
