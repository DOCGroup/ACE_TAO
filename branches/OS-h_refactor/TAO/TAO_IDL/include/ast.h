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

#ifndef _AST_AST_HH
#define _AST_AST_HH

// ast.h
//
// Defines the classes which constitute the agreement between the CFE
// and BEs.

/*
** DEPENDENCIES: NONE
**
** USE: Included from idl.hh
*/

// This is to remove "inherits via dominance" warnings from MSVC.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
# pragma warning (disable : 4250)
#endif /* _MSC_VER */

#include        "ast_visitor.h"         // abstract class ast_visitor

#include        "utl_scoped_name.h"     // Define UTL_ScopedName

#include        "ast_decl.h"            // class AST_Decl

#include        "ast_expression.h"      // class AST_Expression

#include        "utl_scope.h"           // class UTL_Scope

#include        "ast_type.h"            // class AST_Type
#include        "ast_concrete_type.h"   // class AST_ConcreteType
#include        "ast_predefined_type.h" // class AST_PredefinedType
#include        "ast_module.h"          // class AST_Module
#include        "ast_root.h"            // class AST_Root
#include        "ast_interface.h"       // class AST_Interface
#include        "ast_interface_fwd.h"   // class AST_InterfaceFwd
#include        "ast_component.h"       // class AST_Component
#include        "ast_structure.h"       // class AST_Structure
#include        "ast_exception.h"       // class AST_Exception
#include        "ast_enum.h"            // class AST_Enum
#include        "ast_operation.h"       // class AST_Operation
#include        "ast_field.h"           // class AST_Field
#include        "ast_argument.h"        // class AST_Argument
#include        "ast_attribute.h"       // class AST_Attribute
#include        "ast_union.h"           // class AST_Union
#include        "ast_union_branch.h"    // class AST_UnionBranch
#include        "ast_union_label.h"     // class AST_UnionLabel
#include        "ast_constant.h"        // class AST_Constant
#include        "ast_enum_val.h"        // class AST_EnumVal
#include        "ast_array.h"           // class AST_Array
#include        "ast_sequence.h"        // class AST_Sequence
#include        "ast_string.h"          // class AST_String
#include        "ast_typedef.h"         // class AST_Typedef
#include        "ast_native.h"          // class AST_Native
#include        "ast_factory.h"         // class AST_Factory

#include        "utl_list.h"            // class UTL_List
#include        "utl_strlist.h"         // class UTL_StrList
#include        "utl_exprlist.h"        // class UTL_ExprList

#include        "ast_generator.h"       // class AST_Generator

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif           // _AST_AST_HH
