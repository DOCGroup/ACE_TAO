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

#ifndef _IDL_FWD_IDL_FWD_HH
#define _IDL_FWD_IDL_FWD_HH

// idl_fwd.hh
//
// Forward declaration of all class names in CFE-BEs contract

/*
** DEPENDENCIES: NONE
**
** USE: Included from idl.hh
*/

// Class for global data representation:

class	IDL_GlobalData;			// Global data is stored in an
					// instance of this

// Classes for utility of the IDL compiler:

class	UTL_ScopeStack;			// Stack of scopes
class	UTL_Scope;			// A definition scope (mixin)

class	UTL_LongList;			// List of longs
class	UTL_StrList;			// List of strings
class	UTL_StrlistActiveIterator;	// Active iterator for list of strings
class	UTL_IdList;			// List of identifiers
class	UTL_IdListActiveIterator;	// Active iterator for list of id's
class	UTL_ExceptList;			// List of exceptions
class	UTL_NameList;			// List of scoped names
class	UTL_ExprList;			// List of expressions
class	UTL_InterfList;			// List of interfaces
class	UTL_LabelList;			// List of union branch labels
class	UTL_DeclList;			// List of declarators
class	UTL_String;			// String class
class	UTL_Identifier;			// Identifier class

class	UTL_Error;			// Error class
class	UTL_Indenter;			// Indenter class

// Classes for utility of the FE. These classes are used only
// by the FE but their forward declaration is needed because lists
// of them are constructed by classes in UTL

class	FE_InterfaceHeader;		// Interface header
class	FE_Declarator;			// Declarator

// Generator class for the AST

class	AST_Generator;			// Generate nodes of various types

// Classes for AST:

class	AST_Decl;			// The rock bottom base class

class	AST_PredefinedType;		// A predefined type
class	AST_Type;			// An IDL type
class	AST_ConcreteType;		// An IDL concrete type
class	AST_Module;			// A module
class	AST_Root;			// The root of an AST (a module)
class	AST_Interface;			// An interface
class	AST_InterfaceFwd;		// A forward interface decl
class	AST_Constant;			// A constant declaration
class	AST_Expression;			// Value of an expression
class	AST_Exception;			// An exception declaration
class	AST_Attribute;			// An attribute declaration
class	AST_Operation;			// An operation declaration
class	AST_Argument;			// An argument to an operation
class	AST_Union;			// A union declaration
class	AST_UnionBranch;		// A branch in a union
class	AST_UnionLabel;			// A union branch label
class	AST_Structure;			// A structure declaration
class	AST_Field;			// A field in a structure or union
class	AST_Enum;			// An enum declaration
class	AST_EnumVal;			// An enumerator in an enum
class	AST_Sequence;			// A sequence declaration
class	AST_String;			// A string declaration
class	AST_Array;			// An array declaration
class	AST_Typedef;			// A typedef declaration

#endif           // _IDL_FWD_IDL_FWD_HH
