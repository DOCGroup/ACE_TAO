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

#ifndef _UTL_ERROR_UTL_ERROR_HH
#define _UTL_ERROR_UTL_ERROR_HH

#pragma ident "%@(#)utl_error.hh	1.41% %92/06/10% Sun Microsystems"

// utl_error.hh
//
// Defines error codes and error handling class for UTL
// This will change as a result of internationalization

/*
** DEPENDENCIES: ast.hh
**
** USE: Included from util.hh
*/

class	UTL_Error
{
public:
  enum ErrorCode {
    EIDL_SYNTAX_ERROR,		// Syntax error in IDL input
				// More details will be gleaned from examining
				// idl_global->parse_state()
    EIDL_REDEF,			// Redefinition
    EIDL_REDEF_SCOPE,		// Redefinition inside defining scope
    EIDL_DEF_USE,		// Definition after use
    EIDL_MULTIPLE_BRANCH,	// More than one union branch with this
				// label
    EIDL_COERCION_FAILURE,	// Coercion failure
    EIDL_SCOPE_CONFLICT,	// Between fwd declare and full declare
    EIDL_ONEWAY_CONFLICT,	// Between op decl and argument direction
    EIDL_DISC_TYPE,		// Illegal discriminator type in union
    EIDL_LABEL_TYPE,		// Mismatch with discriminator type in union
    EIDL_ILLEGAL_ADD,		// Illegal add action
    EIDL_ILLEGAL_USE,		// Illegal type used in expression
    EIDL_ILLEGAL_RAISES,	// Error in "raises" clause
    EIDL_ILLEGAL_CONTEXT,	// Error in "context" clause
    EIDL_CANT_INHERIT,		// Cannot inherit from non-interface
    EIDL_LOOKUP_ERROR,		// Identifier not found
    EIDL_INHERIT_FWD_ERROR,	// Cannot inherit from fwd decl interface
    EIDL_CONSTANT_EXPECTED,	// We got something else..
    EIDL_NAME_CASE_ERROR,	// Spelling differences found
    EIDL_ENUM_VAL_EXPECTED,	// Expected an enumerator
    EIDL_ENUM_VAL_NOT_FOUND,	// Didnt find an enumerator with that name
    EIDL_EVAL_ERROR,		// Error in evaluating expression
    EIDL_AMBIGUOUS,		// Ambiguous name definition
    EIDL_DECL_NOT_DEFINED,	// Forward declared but never defined
    EIDL_FWD_DECL_LOOKUP,	// Tried to lookup in fwd declared intf
    EIDL_RECURSIVE_TYPE,	// Illegal recursive use of type
    EIDL_NONVOID_ONEWAY,	// Non-void return type in oneway operation
    EIDL_NOT_A_TYPE,		// Not a type
    EIDL_BACK_END,		// Back end error
    EIDL_OK			// No error
  };

  // Operations

  // Report errors with varying numbers of arguments
  void	error0(ErrorCode e);
  void	error1(ErrorCode e, AST_Decl *t);
  void	error2(ErrorCode e, AST_Decl *t1, AST_Decl *t2);
  void	error3(ErrorCode e, AST_Decl *t1, AST_Decl *t2, AST_Decl *t3);

  // Report a syntax error in IDL input
  void	syntax_error(IDL_GlobalData::ParseState ps);

  // Report a name being used with different spellings
  void	name_case_error(char *n, char *t);

  // Report an unsuccesful coercion attempt
  void	coercion_error(AST_Expression *v, AST_Expression::ExprType t);

  // Report a failed name lookup attempt
  void	lookup_error(UTL_ScopedName *n);

  // Report an attempt to use a forward declared interface which
  // hasn't been defined yet in an inheritance spec
  void	inheritance_fwd_error(UTL_ScopedName *n, AST_Interface *f);

  // Report an attempt to inherit from something other than an interface
  void	inheritance_error(UTL_ScopedName *n, AST_Decl *d);

  // Report an error while evaluating an expression (division by zero, etc.)
  void	eval_error(AST_Expression *d);

  // Report a situation where a constant was expected but we got
  // something else instead. This most likely is a case where a union
  // label did not evaluate to a constant
  void	constant_expected(UTL_ScopedName *n, AST_Decl *d);

  // Report a situation where an enumerator was expected but we got
  // something else instead. This occurs when a union with an enum
  // discriminator is being parsed and one of the branch labels is
  // not an enumerator in that enum
  void	enum_val_expected(AST_Union *t, AST_UnionLabel *l);

  // Report a failed enumerator lookup in an enum
  void	enum_val_lookup_failure(AST_Union *t, AST_Enum *e, UTL_ScopedName *n);

  // Report an ambiguous name definition
  void	ambiguous(UTL_Scope *s, AST_Decl *l, AST_Decl *d);

  // Report a forward declared interface which was never defined
  void	fwd_decl_not_defined(AST_Interface *d);

  // Report attempt to lookup in forward declared interface
  void	fwd_decl_lookup(AST_Interface *d, UTL_ScopedName *n);

  // Report a redefinition in scope
  void	redefinition_in_scope(AST_Decl *t, AST_Decl *s);

  // Report not a type error
  void	not_a_type(AST_Decl *d);

  // Report back-end error
  void  back_end(long lineno, String *s);
};

#endif           // _UTL_ERROR_UTL_ERROR_HH
