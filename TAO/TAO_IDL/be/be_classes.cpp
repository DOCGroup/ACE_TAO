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

#pragma ident "%@(#)be_classes.cc	1.20% %92/06/10% Sun Microsystems"

// be_classes.cc
//
// Implementation of all dummy BE classes

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_PredefinedType
 */
be_predefined_type::be_predefined_type()
{
}
be_predefined_type::be_predefined_type(AST_PredefinedType::PredefinedType t,
				       UTL_ScopedName *n, UTL_StrList *p)
		  : AST_PredefinedType(t, n, p),
		    AST_Decl(AST_Decl::NT_pre_defined, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_predefined_type, AST_PredefinedType)
IMPL_NARROW_FROM_DECL(be_predefined_type)

/*
 * BE_Module
 */
be_module::be_module()
{
}
be_module::be_module(UTL_ScopedName *n, UTL_StrList *p)
	 : AST_Decl(AST_Decl::NT_module, n, p),
	   UTL_Scope(AST_Decl::NT_module)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_module, AST_Module)
IMPL_NARROW_FROM_DECL(be_module)
IMPL_NARROW_FROM_SCOPE(be_module)

/*
 * BE_Interface
 */
be_interface::be_interface()
{
}
be_interface::be_interface(UTL_ScopedName *n, AST_Interface **ih, long nih,
			   UTL_StrList *p)
	    : AST_Interface(n, ih, nih, p),
	      AST_Decl(AST_Decl::NT_interface, n, p),
	      UTL_Scope(AST_Decl::NT_interface)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_interface, AST_Interface)
IMPL_NARROW_FROM_DECL(be_interface)
IMPL_NARROW_FROM_SCOPE(be_interface)

/*
 * BE_InterfaceFwd
 */
be_interface_fwd::be_interface_fwd()
{
}
be_interface_fwd::be_interface_fwd(UTL_ScopedName *n, UTL_StrList *p)
		: AST_InterfaceFwd(n, p),
		  AST_Decl(AST_Decl::NT_interface_fwd, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_interface_fwd, AST_InterfaceFwd)
IMPL_NARROW_FROM_DECL(be_interface_fwd)

/*
 * BE_Exception
 */
be_exception::be_exception()
{
}
be_exception::be_exception(UTL_ScopedName *n, UTL_StrList *p)
	    : AST_Decl(AST_Decl::NT_except, n, p),
              AST_Structure(AST_Decl::NT_except, n, p),
	      UTL_Scope(AST_Decl::NT_except)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_exception, AST_Exception)
IMPL_NARROW_FROM_DECL(be_exception)
IMPL_NARROW_FROM_SCOPE(be_exception)

/*
 * BE_Structure
 */
be_structure::be_structure()
{
}
be_structure::be_structure(UTL_ScopedName *n, UTL_StrList *p)
	    : AST_Decl(AST_Decl::NT_struct, n, p),
	      UTL_Scope(AST_Decl::NT_struct)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_structure, AST_Structure)
IMPL_NARROW_FROM_DECL(be_structure)
IMPL_NARROW_FROM_SCOPE(be_structure)

/*
 * BE_Enum
 */
be_enum::be_enum()
{
}
be_enum::be_enum(UTL_ScopedName *n, UTL_StrList *p)
       : AST_Enum(n, p),
	 AST_Decl(AST_Decl::NT_enum, n, p),
	 UTL_Scope(AST_Decl::NT_enum)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_enum, AST_Enum)
IMPL_NARROW_FROM_DECL(be_enum)
IMPL_NARROW_FROM_SCOPE(be_enum)

/*
 * BE_Operation
 */
be_operation::be_operation()
{
}
be_operation::be_operation(AST_Type *rt, AST_Operation::Flags fl,
			   UTL_ScopedName *n, UTL_StrList *p)
	    : AST_Operation(rt, fl, n, p),
	      AST_Decl(AST_Decl::NT_op, n, p),
	      UTL_Scope(AST_Decl::NT_op)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_operation, AST_Operation)
IMPL_NARROW_FROM_DECL(be_operation)
IMPL_NARROW_FROM_SCOPE(be_operation)

/*
 * BE_Field
 */
be_field::be_field()
{
}
be_field::be_field(AST_Type *ft, UTL_ScopedName *n, UTL_StrList *p)
	: AST_Field(ft, n, p),
	  AST_Decl(AST_Decl::NT_field, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_field, AST_Field)
IMPL_NARROW_FROM_DECL(be_field)

/*
 * BE_Argument
 */
be_argument::be_argument()
{
}
be_argument::be_argument(AST_Argument::Direction d, AST_Type *ft,
			 UTL_ScopedName *n, UTL_StrList *p)
	   : AST_Argument(d, ft, n, p),
	     AST_Field(AST_Decl::NT_argument, ft, n, p),
	     AST_Decl(AST_Decl::NT_argument, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_argument, AST_Argument)
IMPL_NARROW_FROM_DECL(be_argument)

/*
 * BE_Attribute
 */
be_attribute::be_attribute()
{
}
be_attribute::be_attribute(idl_bool ro, AST_Type *ft, UTL_ScopedName *n,
			   UTL_StrList *p)
	    : AST_Attribute(ro, ft, n, p),
	      AST_Field(AST_Decl::NT_attr, ft, n, p),
	      AST_Decl(AST_Decl::NT_attr, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_attribute, AST_Attribute)
IMPL_NARROW_FROM_DECL(be_attribute)

/*
 * BE_Union
 */
be_union::be_union()
{
}
be_union::be_union(AST_ConcreteType *dt, UTL_ScopedName *n, UTL_StrList *p)
	: AST_Union(dt, n, p),
	  AST_Decl(AST_Decl::NT_union, n, p),
          AST_Structure(AST_Decl::NT_union, n, p),
	  UTL_Scope(AST_Decl::NT_union)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_union, AST_Union)
IMPL_NARROW_FROM_DECL(be_union)
IMPL_NARROW_FROM_SCOPE(be_union)

/*
 * BE_UnionBranch
 */
be_union_branch::be_union_branch()
{
}
be_union_branch::be_union_branch(AST_UnionLabel *lab, AST_Type *ft,
				 UTL_ScopedName *n, UTL_StrList *p)
	       : AST_UnionBranch(lab, ft, n, p),
		 AST_Field(AST_Decl::NT_union_branch, ft, n, p),
		 AST_Decl(AST_Decl::NT_union_branch, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_union_branch, AST_UnionBranch)
IMPL_NARROW_FROM_DECL(be_union_branch)

/*
 * BE_UnionLabel
 */
be_union_label::be_union_label()
{
}
be_union_label::be_union_label(AST_UnionLabel::UnionLabel ul,
			       AST_Expression *v)
	      : AST_UnionLabel(ul, v)
{
}

/*
 * BE_Constant
 */
be_constant::be_constant()
{
}
be_constant::be_constant(AST_Expression::ExprType et,
			 AST_Expression *v,
			 UTL_ScopedName *n,
			 UTL_StrList *p)
	   : AST_Constant(et, v, n, p),
	     AST_Decl(AST_Decl::NT_const, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_constant, AST_Constant)
IMPL_NARROW_FROM_DECL(be_constant)

/*
 * BE_Expression
 */
be_expr_val::be_expr_val(UTL_ScopedName *n) : AST_Expression(n) {}
be_expr_val::be_expr_val(AST_Expression *b, AST_Expression::ExprType t)
	   : AST_Expression(b, t) 
{
}
be_expr_val::be_expr_val(AST_Expression::ExprComb c,
			 AST_Expression *v1,
			 AST_Expression *v2)
	   : AST_Expression(c, v1, v2)
{
}
be_expr_val::be_expr_val(long l) : AST_Expression(l) {}
be_expr_val::be_expr_val(long l, AST_Expression::ExprType t)
	   : AST_Expression(l, t)
{
}
be_expr_val::be_expr_val(unsigned long l) : AST_Expression(l) {}
be_expr_val::be_expr_val(String *s) : AST_Expression(s) {}
be_expr_val::be_expr_val(char c) : AST_Expression(c) {}
be_expr_val::be_expr_val(double d) : AST_Expression(d) {}

be_enum_val::be_enum_val()
{
}
be_enum_val::be_enum_val(unsigned long v, UTL_ScopedName *n, UTL_StrList *p)
	   : AST_Constant(AST_Expression::EV_ulong,
			  AST_Decl::NT_enum_val,
			  new AST_Expression(v),
			  n,
			  p),
	     AST_Decl(AST_Decl::NT_enum_val, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_enum_val, AST_EnumVal)
IMPL_NARROW_FROM_DECL(be_enum_val)

/*
 * BE_Array
 */
be_array::be_array()
{
}
be_array::be_array(UTL_ScopedName *n, unsigned long ndims, UTL_ExprList *dims)
	: AST_Array(n, ndims, dims),
	  AST_Decl(AST_Decl::NT_array, n, NULL)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_array, AST_Array)
IMPL_NARROW_FROM_DECL(be_array)

/*
 * BE_Sequence
 */
be_sequence::be_sequence()
{
}
be_sequence::be_sequence(AST_Expression *v, AST_Type *t)
	   : AST_Sequence(v, t),
    	     AST_Decl(AST_Decl::NT_sequence,
		      new UTL_ScopedName(
				new Identifier("sequence", 1, 0, I_FALSE),
				NULL),
		      NULL)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_sequence, AST_Sequence)
IMPL_NARROW_FROM_DECL(be_sequence)

/*
 * BE_String
 */
be_string::be_string()
{
}
be_string::be_string(AST_Expression *v)
	 : AST_String(v),
	   AST_Decl(AST_Decl::NT_string,
		    new UTL_ScopedName(new Identifier("string", 1, 0, I_FALSE),
				       NULL),
		    NULL)
{
}
be_string::be_string(AST_Expression *v, long wide)
	 : AST_String(v, wide),
	   AST_Decl(AST_Decl::NT_string,
		    wide == 1
		    ? new UTL_ScopedName(new Identifier("string",1,0,I_FALSE),
					 NULL)
		    : new UTL_ScopedName(new Identifier("wstring_t",
                                                        1,
                                                        0,
                                                        I_FALSE),
					 NULL),
		    NULL)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_string, AST_String)
IMPL_NARROW_FROM_DECL(be_string)

be_typedef::be_typedef()
{
}
be_typedef::be_typedef(AST_Type *bt, UTL_ScopedName *n, UTL_StrList *p)
	  : AST_Typedef(bt, n, p),
	    AST_Decl(AST_Decl::NT_typedef, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_typedef, AST_Typedef)
IMPL_NARROW_FROM_DECL(be_typedef)


