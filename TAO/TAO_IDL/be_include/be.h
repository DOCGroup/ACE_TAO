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

#ifndef _BE_BE_HH
#define _BE_BE_HH

#pragma ident "%@(#)be.h	1.19% %92/06/10% Sun Microsystems"

// be.h
//

// ACE includes

#include        "ace/ACE.h"
#include        "ace/OS.h"
#include        "ace/Singleton.h"
#include        "ace/Synch.h"
#include        "ace/Log_Msg.h"

/*
 * BE includes
 */

#include        "be_decl.h"             // class BE_Decl
#include        "be_scope.h"            // class BE_Scope
#include	"be_generator.h"	// BE generator class
#include        "be_type.h"
#include	"be_predefined_type.h"  // class BE_PredefinedType
#include	"be_module.h"		// class BE_Module
#include	"be_interface.h"	// class BE_Interface
#include	"be_interface_fwd.h"	// class BE_InterfaceFwd
#include	"be_structure.h"	// class BE_Structure
#include	"be_exception.h"	// class BE_Exception
#include        "be_expression.h"       // class BE_Expr_val
#include	"be_enum.h"		// class BE_Enum
#include	"be_operation.h"	// class BE_Operation
#include	"be_field.h"		// class BE_Field
#include	"be_argument.h"	        // class BE_Argument
#include	"be_attribute.h"	// class BE_Attribute
#include	"be_union.h"		// class BE_Union
#include	"be_union_branch.h"	// class BE_UnionBranch
#include	"be_union_label.h"	// class BE_UnionLabel
#include	"be_constant.h"	        // class BE_Constant
#include	"be_enum_val.h"	        // class BE_EnumVal
#include	"be_array.h"		// class BE_Array
#include	"be_sequence.h"	        // class BE_Sequence
#include	"be_string.h"		// class BE_String
#include	"be_typedef.h"	        // class BE_Typedef 
#include        "be_root.h"             // class BE_Root

#include        "be_helper.h"           // helper functions
#include        "be_state.h"            // code generation states
#include        "be_codegen.h"          // code generator
#include        "be_factory.h"          // factory

#endif           // _BE_BE_HH
