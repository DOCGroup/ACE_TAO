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

// be_generator.cc
//
// Implementation of BE generator class
//
// This implements the same protocol as AST_Generator but creates instances
// of the BE-subclassed classes instead of of AST classes

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "ace/inc_user_config.h"

ACE_RCSID(be, be_generator, "$Id$")

/*
 * Constructor
 */

/*
 * Private operations
 */

/*
 * Public operations
 */

/*
 * Inherited operations redefined here
 */

/*
 * Construct a be_root node (a node representing the root of an AST of BE nodes)
 */
AST_Root *
be_generator::create_root(UTL_ScopedName *n,
                          UTL_StrList *p)
{
    return new be_root(n, p);
}

/*
 * Create a BE_PredefinedType node
 */
AST_PredefinedType *
be_generator::create_predefined_type(AST_PredefinedType::PredefinedType t,
                                     UTL_ScopedName *n,
                                     UTL_StrList *p)
{
  return (AST_PredefinedType *) new be_predefined_type(t, n, p);
}

/*
 * Create a BE_Module node
 */
AST_Module *
be_generator::create_module(UTL_ScopedName *n, UTL_StrList *p)
{
  return (AST_Module *) new be_module(n, p);
}

/*
 * Create a BE_Interface node
 */
AST_Interface *
be_generator::create_interface(UTL_ScopedName *n,
                               AST_Interface **ih,
                               long nih,
                               UTL_StrList *p)
{
  return (AST_Interface *) new be_interface(n, ih, nih, p);
}

/*
 * Create a BE_InterfaceFwd node
 */
AST_InterfaceFwd *
be_generator::create_interface_fwd(UTL_ScopedName *n, UTL_StrList *p)
{
  return (AST_InterfaceFwd *) new be_interface_fwd(n, p);
}

/*
 * Create a BE_Exception node
 */
AST_Exception *
be_generator::create_exception(UTL_ScopedName *n, UTL_StrList *p)
{
  return (AST_Exception *) new be_exception(n, p);
}

/*
 * Create a BE_Structure node
 */
AST_Structure *
be_generator::create_structure(UTL_ScopedName *n, UTL_StrList *p)
{
  return (AST_Structure *) new be_structure(n, p);
}

/*
 * Create a BE_Enum node
 */
AST_Enum *
be_generator::create_enum(UTL_ScopedName *n, UTL_StrList *p)
{
  return (AST_Enum *) new be_enum(n, p);
}

/*
 * Create a BE_Operation node
 */
AST_Operation *
be_generator::create_operation(AST_Type *rt,
                               AST_Operation::Flags fl,
                               UTL_ScopedName *n,
                               UTL_StrList *p)
{
  return (AST_Operation *) new be_operation(rt, fl, n, p);
}

/*
 * Create a BE_Field node
 */
AST_Field *
be_generator::create_field(AST_Type *ft, UTL_ScopedName *n, UTL_StrList *p)
{
  return (AST_Field *) new be_field(ft, n, p);
}

/*
 * Create a BE_Argument node
 */
AST_Argument *
be_generator::create_argument(AST_Argument::Direction d,
                              AST_Type *ft,
                              UTL_ScopedName *n,
                              UTL_StrList *p)
{
  return (AST_Argument *) new be_argument(d, ft, n, p);
}

/*
 * Create a BE_Attribute node
 */
AST_Attribute *
be_generator::create_attribute(idl_bool ro,
                               AST_Type *ft,
                               UTL_ScopedName *n,
                               UTL_StrList *p)
{
  return (AST_Attribute *) new be_attribute(ro, ft, n, p);
}

/*
 * Create a BE_Union node
 */
AST_Union *
be_generator::create_union(AST_ConcreteType *dt,
                           UTL_ScopedName *n,
                           UTL_StrList *p)
{
  return (AST_Union *) new be_union(dt, n, p);
}

/*
 * Create a BE_UnionBranch node
 */
AST_UnionBranch *
be_generator::create_union_branch(AST_UnionLabel *lab,
                                  AST_Type *ft,
                                  UTL_ScopedName *n,
                                  UTL_StrList *p)
{
  return (AST_UnionBranch *) new be_union_branch(lab, ft, n, p);
}

/*
 * Create a BE_UnionLabel node
 */
AST_UnionLabel *
be_generator::create_union_label(AST_UnionLabel::UnionLabel ul,
                                 AST_Expression *lv)
{
  return (AST_UnionLabel *) new be_union_label(ul, lv);
}

/*
 * Create a BE_Constant node
 */
AST_Constant *
be_generator::create_constant(AST_Expression::ExprType et,
                              AST_Expression *ev,
                              UTL_ScopedName *n,
                              UTL_StrList *p)
{
  return (AST_Constant *) new be_constant (et, ev, n, p);
}

/*
 * Create a symbolic BE_Expression node
 */
AST_Expression *
be_generator::create_expr(UTL_ScopedName *n)
{
  return (AST_Expression *) new be_expression (n);
}

/*
 * Create a BE_Expression node denoting a coercion
 */
AST_Expression *
be_generator::create_expr(AST_Expression *b, AST_Expression::ExprType t)
{
  return (AST_Expression *) new be_expression (b, t);
}

/*
 * Create a BE_Expression node combining two other AST_Expression nodes
 */
AST_Expression *
be_generator::create_expr(AST_Expression::ExprComb c,
                          AST_Expression *v1,
                          AST_Expression *v2)
{
  return (AST_Expression *) new be_expression (c, v1, v2);
}

/*
 * Create a BE_Expression node denoting a long integer
 */
AST_Expression *
be_generator::create_expr(long l)
{
  return (AST_Expression *) new be_expression (l);
}

/*
 * Create a BE_Expression node denoting a long integer being used as a boolean
 */
AST_Expression *
be_generator::create_expr(long l, AST_Expression::ExprType t)
{
  return (AST_Expression *) new be_expression (l, t);
}

/*
 * Create a BE_Expression node denoting an unsigned long integer
 */
AST_Expression *
be_generator::create_expr(unsigned long l)
{
  return (AST_Expression *) new be_expression(l);
}

/*
 * Create a BE_Expression node denoting a char * (encapsulated as a String)
 */
AST_Expression *
be_generator::create_expr (String *s)
{
  return (AST_Expression *) new be_expression (s);
}

/*
 * Create a BE_Expression node denoting a character
 */
AST_Expression *
be_generator::create_expr(char c)
{
  return (AST_Expression *) new be_expression (c);
}

/*
 * Create a BE_Expression node denoting a 64-bit floating point number
 */
AST_Expression *
be_generator::create_expr (double d)
{
  return (AST_Expression *) new be_expression (d);
}

/*
 * Create a BE_EnumVal node
 */
AST_EnumVal *
be_generator::create_enum_val(unsigned long v,
                              UTL_ScopedName *n,
                              UTL_StrList *p)
{
  return (AST_EnumVal *) new be_enum_val(v, n, p);
}

/*
 * Create a BE_Array node
 */
AST_Array *
be_generator::create_array(UTL_ScopedName *n,
                           unsigned long ndims,
                           UTL_ExprList *dims)
{
  return (AST_Array *) new be_array(n, ndims, dims);
}

/*
 * Create a BE_Sequence node
 */
AST_Sequence *
be_generator::create_sequence(AST_Expression *v, AST_Type *bt)
{
  return (AST_Sequence *) new be_sequence(v, bt);
}

/*
 * Create a BE_String node
 */
AST_String *
be_generator::create_string(AST_Expression *v)
{
  return (AST_String *) new be_string(v);
}

/*
 * Create a BE_string node for a wide string
 */
AST_String *
be_generator::create_wstring(AST_Expression *v)
{
  return (AST_String *) new be_string(v, sizeof(ACE_OS::WChar));
}

/*
 * Create a BE_Typedef node
 */
AST_Typedef *
be_generator::create_typedef(AST_Type *bt, UTL_ScopedName *n, UTL_StrList *p)
{
  return (AST_Typedef *) new be_typedef(bt, n, p);
}

/*
 * Construct an AST_Native node
 */
AST_Native *
be_generator::create_native (UTL_ScopedName *n,
                             UTL_StrList *p)
{
  return (AST_Native *) new be_native (n, p);
}
