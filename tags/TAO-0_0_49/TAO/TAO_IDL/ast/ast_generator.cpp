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

/*
 * ast_generator.cc - Implements the base AST node generator
 *
 * The generator protocol is explained in detail in the IDL CFE
 * design document.
 * The AST_Generator class provides operations to instantiate any
 * of the AST nodes. It contains an operation for every constructor
 * of every AST class.
 */

#include	"idl.h"
#include	"idl_extern.h"

/*
 * Constructors
 */

/*
 * Private operations
 */

/*
 * Public operations
 */

/*
 * Construct an AST_PredefinedType node (a predefined type)
 */
AST_PredefinedType *
AST_Generator::create_predefined_type(AST_PredefinedType::PredefinedType t,
				    UTL_ScopedName *n,
				    UTL_StrList *p)
{
  return new AST_PredefinedType(t, n, p);
}

/*
 * Construct an AST_Module node (a module)
 */
AST_Module	*
AST_Generator::create_module(UTL_ScopedName *n, UTL_StrList *p)
{
  return new AST_Module(n, p);
}

/*
 * Construct an AST_Root node (a node representing the root of an AST)
 */
AST_Root *
AST_Generator::create_root(UTL_ScopedName *n,
			  UTL_StrList *p)
{
    return new AST_Root(n, p);
}

/*
 * Construct an AST_Interface node (an interface)
 */
AST_Interface	*
AST_Generator::create_interface(UTL_ScopedName *n,
			       AST_Interface **ih,
			       long nih,
			       UTL_StrList *p)
{
  return new AST_Interface(n, ih, nih, p);
}

/*
 * Construct an AST_InterfaceFwd node (a node representing the forward
 * declaration of an interface)
 */
AST_InterfaceFwd *
AST_Generator::create_interface_fwd(UTL_ScopedName *n, UTL_StrList *p)
{
  return new AST_InterfaceFwd(n, p);
}

/*
 * Construct an AST_Exception node (an exception)
 */
AST_Exception	*
AST_Generator::create_exception(UTL_ScopedName *n, UTL_StrList *p)
{
  return new AST_Exception(n, p);
}

/*
 * Construct an AST_Structure node (a struct)
 */
AST_Structure	*
AST_Generator::create_structure(UTL_ScopedName *n, UTL_StrList *p)
{
  return new AST_Structure(n, p);
}

/*
 * Construct an AST_Enum node (an enum)
 */
AST_Enum	*
AST_Generator::create_enum(UTL_ScopedName *n, UTL_StrList *p)
{
  return new AST_Enum(n, p);
}

/*
 * Construct an AST_Operation node (an operation on an interface)
 */
AST_Operation	*
AST_Generator::create_operation(AST_Type *rt,
			       AST_Operation::Flags fl,
			       UTL_ScopedName *n,
			       UTL_StrList *p)
{
  return new AST_Operation(rt, fl, n, p);
}

/*
 * Construct an AST_Field node (a field in a struct, union or exception)
 */
AST_Field	*
AST_Generator::create_field(AST_Type *ft, UTL_ScopedName *n, UTL_StrList *p)
{
  return new AST_Field(ft, n, p);
}

/*
 * Construct an AST_Argument node (an argument to an operation)
 */
AST_Argument	*
AST_Generator::create_argument(AST_Argument::Direction d,
			      AST_Type *ft,
			      UTL_ScopedName *n,
			      UTL_StrList *p)
{
  return new AST_Argument(d, ft, n, p);
}

/*
 * Construct an AST_Attribute node (an attribute)
 */
AST_Attribute	*
AST_Generator::create_attribute(idl_bool ro,
			       AST_Type *ft,
			       UTL_ScopedName *n,
			       UTL_StrList *p)
{
  return new AST_Attribute(ro, ft, n, p);
}

/*
 * Construct an AST_Union node (a union)
 */
AST_Union	*
AST_Generator::create_union(AST_ConcreteType *dt,
			   UTL_ScopedName *n,
			   UTL_StrList *p)
{
  return new AST_Union(dt, n, p);
}

/*
 * Construct an AST_UnionBranch node (a branch in a union)
 */
AST_UnionBranch *
AST_Generator::create_union_branch(AST_UnionLabel *lab,
				 AST_Type *ft,
				 UTL_ScopedName *n,
				 UTL_StrList *p)
{
  return new AST_UnionBranch(lab, ft, n, p);
}

/*
 * Construct an AST_UnionLabel node (a label in a union)
 */
AST_UnionLabel	*
AST_Generator::create_union_label(AST_UnionLabel::UnionLabel ul,
				AST_Expression *v)
{
  return new AST_UnionLabel(ul, v);
}

/*
 * Construct an AST_Constant node (a constant)
 */
AST_Constant	*
AST_Generator::create_constant(AST_Expression::ExprType et,
			      AST_Expression *ev,
			      UTL_ScopedName *n,
			      UTL_StrList *p)
{
  return new AST_Constant(et, ev, n, p);
}

/*
 * Construct an AST_Expression node denoting a symbolic name
 */
AST_Expression	*
AST_Generator::create_expr(UTL_ScopedName *n)
{
  return new AST_Expression(n);
}

/*
 * Construct an AST_Expression denoting a coercion
 */
AST_Expression	*
AST_Generator::create_expr(AST_Expression *v, AST_Expression::ExprType t)
{
  return new AST_Expression(v, t);
}

/*
 * Construct an AST_Expression node denoting a binary combinator
 * of two other AST_Expression nodes
 */
AST_Expression	*
AST_Generator::create_expr(AST_Expression::ExprComb c,
			   AST_Expression *v1,
			   AST_Expression *v2)
{
  return new AST_Expression(c, v1, v2);
}

/*
 * Construct an AST_Expression node denoting a long integer
 */
AST_Expression	*
AST_Generator::create_expr(long v)
{
  return new AST_Expression(v);
}

/*
 * Construct an AST_Expression node denoting a long integer being used
 * as a boolean
 */
AST_Expression	*
AST_Generator::create_expr(long v, AST_Expression::ExprType t)
{
  return new AST_Expression(v, t);
}

/*
 * Construct an AST_Expression node denoting an unsigned long integer
 */
AST_Expression	*
AST_Generator::create_expr(unsigned long v)
{
  return new AST_Expression(v);
}

/*
 * Construct an AST_Expression node denoting a string (a char *
 * encapsulated as String)
 */
AST_Expression	*
AST_Generator::create_expr(String *s)
{
  return new AST_Expression(s);
}

/*
 * Construct an AST_Expression node denoting a character
 */
AST_Expression	*
AST_Generator::create_expr(char c)
{
  return new AST_Expression(c);
}

/*
 * Construct an AST_Expression node denoting a 64-bit floating point number
 */
AST_Expression	*
AST_Generator::create_expr(double d)
{
  return new AST_Expression(d);
}

/*
 * Construct an AST_EnumVal node (an enumerator)
 */
AST_EnumVal	*
AST_Generator::create_enum_val(unsigned long v,
			     UTL_ScopedName *n,
			     UTL_StrList *p)
{
  return new AST_EnumVal(v, n, p);
}

/*
 * Construct an AST_Array node (an array type or field modifier)
 */
AST_Array	*
AST_Generator::create_array(UTL_ScopedName *n,
			   unsigned long ndims,
			   UTL_ExprList *dims)
{
  return new AST_Array(n, ndims, dims);
}

/*
 * Construct an AST_Sequence node (a sequence type definition)
 */
AST_Sequence	*
AST_Generator::create_sequence(AST_Expression *ms, AST_Type *bt)
{
  return new AST_Sequence(ms, bt);
}

/*
 * Construct an AST_String node (a string type definition)
 */
AST_String	*
AST_Generator::create_string(AST_Expression *ms)
{
  return new AST_String(ms);
}

/*
 * Construct an AST_String node denoting a wide string type definition
 */
AST_String	*
AST_Generator::create_wstring(AST_Expression *ms)
{
  return new AST_String(ms, sizeof(wchar_t));
}

/*
 * Construct an AST_Typedef node (a typedef)
 */
AST_Typedef	*
AST_Generator::create_typedef(AST_Type *bt, UTL_ScopedName *n, UTL_StrList *p)
{
  return new AST_Typedef(bt, n, p);
}
