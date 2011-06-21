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

#ifndef _UTL_ERR_UTL_ERR_HH
#define _UTL_ERR_UTL_ERR_HH

#include "ast_expression.h"
#include "idl_global.h"
#include "utl_scoped_name.h"

class AST_Decl;
class AST_Interface;
class AST_Enum;
class AST_Union;
class AST_UnionLabel;
class UTL_String;

class TAO_IDL_FE_Export UTL_Error
{
public:
  enum ErrorCode {
    EIDL_SYNTAX_ERROR,          // Syntax error in IDL input
                                // More details will be gleaned from examining
                                // idl_global->parse_state()
    EIDL_REDEF,                 // Redefinition
    EIDL_REDEF_SCOPE,           // Redefinition inside defining scope
    EIDL_DEF_USE,               // Definition after use
    EIDL_MULTIPLE_BRANCH,       // More than one union branch with this label
    EIDL_COERCION_FAILURE,      // Coercion failure
    EIDL_SCOPE_CONFLICT,        // Between fwd declare and full declare
    EIDL_ONEWAY_CONFLICT,       // Between op decl and argument direction
    EIDL_PREFIX_CONFLICT,       // Between prefixes at declaration and use
    EIDL_ILLEGAL_VERSION,       // Bad number in #pragma version statement
    EIDL_VERSION_RESET,         // Can't reset version, even to same number
    EIDL_ID_RESET,              // Tried to set id to a different string
    EIDL_TYPEID_RESET,          // Can't set repo id with 'typeid' twice
    EIDL_INVALID_TYPEID,        // Can't use 'typeid' on this type.
    EIDL_INVALID_TYPEPREFIX,    // Can't use 'typeprefix on this type
    EIDL_DISC_TYPE,             // Illegal discriminator type in union
    EIDL_LABEL_TYPE,            // Mismatch with discriminator type in union
    EIDL_ILLEGAL_ADD,           // Illegal add action
    EIDL_ILLEGAL_USE,           // Illegal type used in expression
    EIDL_ILLEGAL_RAISES,        // Error in "raises" clause
    EIDL_ILLEGAL_CONTEXT,       // Error in "context" clause
    EIDL_CANT_INHERIT,          // Cannot inherit from non-interface
    EIDL_CANT_SUPPORT,          // Cannot support a non-interface
    EIDL_LOOKUP_ERROR,          // Identifier not found
    EIDL_INHERIT_FWD_ERROR,     // Cannot inherit from fwd decl interface
    EIDL_SUPPORTS_FWD_ERROR,    // Cannot support a fwd decl interface
    EIDL_PRIMARY_KEY_ERROR,     // Does not inherit from primaryKeyBase
    EIDL_CONSTANT_EXPECTED,     // We got something else..
    EIDL_INTERFACE_EXPECTED,    // We got something else..
    EIDL_VALUETYPE_EXPECTED,    // We got something else..
    EIDL_CONCRETE_VT_EXPECTED,  // We got something else..
    EIDL_ABSTRACT_EXPECTED,     // We got something else..
    EIDL_EVENTTYPE_EXPECTED,    // We got something else..
    EIDL_TMPL_MODULE_EXPECTED,  // We got something else..
    EIDL_PORTTYPE_EXPECTED,     // We got something else..
    EIDL_CONNECTOR_EXPECTED,    // We got something else..
    EIDL_TYPEDEF_EXPECTED,      // We got something else..
    EIDL_NAME_CASE_ERROR,       // Identifier spellings differ only in case
    EIDL_NAME_CASE_WARNING,     // Same as above, but only a warning
    EIDL_KEYWORD_ERROR,         // Case-insensitive clash with IDL keyword
    EIDL_KEYWORD_WARNING,       // Same as above, but only a warning
    EIDL_ANONYMOUS_ERROR,       // Anonymous types are deprecated by spec
    EIDL_ANONYMOUS_WARNING,     // Same as above, but only a warning
    EIDL_ENUM_VAL_EXPECTED,     // Expected an enumerator
    EIDL_ENUM_VAL_NOT_FOUND,    // Didnt find an enumerator with that name
    EIDL_EVAL_ERROR,            // Error in evaluating expression
    EIDL_INCOMPATIBLE_TYPE,     // Assign floating pt. to int or vice versa
    EIDL_AMBIGUOUS,             // Ambiguous name definition
    EIDL_DECL_NOT_DEFINED,      // Forward declared but never defined
    EIDL_FWD_DECL_LOOKUP,       // Tried to lookup in fwd declared intf
    EIDL_RECURSIVE_TYPE,        // Illegal recursive use of type
    EIDL_NONVOID_ONEWAY,        // Non-void return type in oneway operation
    EIDL_NOT_A_TYPE,            // Not a type
    EIDL_UNDERSCORE,            // More than one leading underscore
    EIDL_EMPTY_MODULE,          // Empty modules not allowed in IDL.
    EIDL_BACK_END,              // Back end error
    EIDL_ILLEGAL_INFIX,         // Illegal infix operator in expression
    EIDL_LOCAL_REMOTE_MISMATCH, // Local type used in remote operation
    EIDL_IGNORE_IDL3_ERROR,     // -Sm option used with component or home decl
    EIDL_TC_SUPPRESSION_WARNING,// -St option used with exception decl
    EIDL_ILLEGAL_BOXED_TYPE,    // Valuetype not allowed for box value type
    EIDL_ILLEGAL_PRIMARY_KEY,   // Primary key doesn't meet spec constraints
    EIDL_MISMATCHED_T_PARAM,    // Between def'd & ref'd template interfaces
    EIDL_DUPLICATE_T_PARAM,     // A tmpl module's param ids must be unique
    EIDL_T_ARG_LENGTH,          // Wrong # of template args
    EIDL_MISMATCHED_SEQ_PARAM,  // 'sequence<T>' must match a previous param
    EIDL_TEMPLATE_NOT_ALIASED,  // ref to tmpl module scope must be via alias
    EIDL_OK                     // No error
  };

  // Operations

  // Report errors with varying numbers of arguments
  void error0 (ErrorCode e);
  void error1 (ErrorCode e,
               AST_Decl *t);
  void error2 (ErrorCode e,
               AST_Decl *t1,
               AST_Decl *t2);
  void error3 (ErrorCode e,
               AST_Decl *t1,
               AST_Decl *t2,
               AST_Decl *t3);

  // Report warnings with varying numbers of arguments
  void warning0 (ErrorCode e);
  void warning1 (ErrorCode e,
                 AST_Decl *t);
  void warning2 (ErrorCode e,
                 AST_Decl *t1,
                 AST_Decl *t2);
  void warning3 (ErrorCode e,
                 AST_Decl *t1,
                 AST_Decl *t2,
                 AST_Decl *t3);

  // Report a syntax error in IDL input
  void syntax_error (IDL_GlobalData::ParseState ps);

  // Report clash of declared and referenced indentifiers
  void redef_error (const char *n, const char *t);

  // Report a name being used with different spellings
  void name_case_error (char *n,
                        char *t);

  // Same as above, but doesn't increment the error count.
  void name_case_warning (char *n,
                          char *t);

  // Differs from spelling of IDL keyword only by case.
  void idl_keyword_error (char *n);

  // Same as above, but doesn't increment the error count.
  void idl_keyword_warning (char *n);

  // Report an unsuccesful coercion attempt
  void coercion_error (AST_Expression *v,
                       AST_Expression::ExprType t);

  // Report a failed name lookup attempt.
  void lookup_error (UTL_ScopedName *n);

  // Report an illegal #pragma version.
  void version_number_error (char *n);

  // Report an illegal #pragma version syntax
  void version_syntax_error (const char *msg);

  // Repost an attempt to reset the version.
  void version_reset_error (void);

  // Report an attempt to change the id once set.
  void id_reset_error (const char *o,
                       const char *n);

  // Report an attempt to use a forward declared interface which
  // hasn't been defined yet in an inheritance spec
  void inheritance_fwd_error  (UTL_ScopedName *n,
                               AST_Interface *f);

  // Report an attempt to inherit from something other than an interface
  void inheritance_error (UTL_ScopedName *n,
                          AST_Decl *d);

  // Report an attempt to use a forward declared interface which
  // hasn't been defined yet in an inheritance spec
  void supports_fwd_error  (UTL_ScopedName *n,
                            AST_Interface *f);

  // Report an attempt to support something other than an interface
  void supports_error (UTL_ScopedName *n,
                       AST_Decl *d);

  // Report an attempt to illegally inherit from an abstract type.
  void abstract_inheritance_error (UTL_ScopedName *v,
                                   UTL_ScopedName *i);

  // Report an attempt to support more then one concrete type.
  void abstract_support_error (UTL_ScopedName *v,
                               UTL_ScopedName *i);

  // Report a situation where a non-local interface was expected but we got
  // something else instead. This most likely is a case in a supports
  // or inheritance list.
  void unconstrained_interface_expected (UTL_ScopedName *c,
                                         UTL_ScopedName *i);

  // A concrete supported interface must inherit from all concrete
  // interfaces supported by the valuetype's ancestors, and all of
  // those conrete interfaces' ancestors.
  void concrete_supported_inheritance_error (UTL_ScopedName *v,
                                             UTL_ScopedName *i);

  // Report an error while evaluating an expression (division by zero, etc.)
  void eval_error (AST_Expression *d);

  // Report incompatible types in constant assignment
  void incompatible_type_error (AST_Expression *d);

  // Report incompatible types in discriminator vs label.
  void incompatible_disc_error (AST_Decl *d,
                                AST_Expression *e);

  // Report a situation where a constant was expected but we got
  // something else instead. This most likely is a case where a union
  // label did not evaluate to a constant
  void constant_expected (UTL_ScopedName *n,
                          AST_Decl *d);

  // Report a situation where an interface was expected but we got
  // something else instead. This most likely is a case in a supports
  // or inheritance list.
  void interface_expected (AST_Decl *d);

  // Report a situation where a template module was expected but we got
  // something else instead. This most likely is a case in a template
  // module instantiation or alias.
  void template_module_expected (AST_Decl *d);

  // Report a situation where a value type was expected but we got
  // something else instead. This most likely is a case in a primary
  // key, emits, publishes or consumes declaration.
  void valuetype_expected (AST_Decl *d);

  // Report a situation where a concrete value type was expected but we got
  // something else instead. This most likely is a case where a valuetype
  // inherits from something other than a concrete valuetype.
  void concrete_valuetype_expected (AST_Decl *d);

  // Report a situation where an abstract type was expected but we got
  // something else instead. This is the case in an inheritance
  // list where a concrete type appears after an abstract type, or
  // where a valuetype inherits more than one concrete valuetype.
  void abstract_expected (AST_Decl *d);

  // Report a situation where an event type was expected but we got
  // something else instead. This is the case in an 'emits', 'publishes',
  // or 'consumes' declaration in a component and it's not an event type.
  void eventtype_expected (AST_Decl *d);

  // Report a situation where a valuetype used as a primary key for a
  // component home does not inherit directly or indirectly from
  // Components::primaryKeyBase.
  void primary_key_error (AST_Decl *d);

  // Report a situation where an enumerator was expected but we got
  // something else instead. This occurs when a union with an enum
  // discriminator is being parsed and one of the branch labels is
  // not an enumerator in that enum
  void enum_val_expected (AST_Union *t,
                          AST_UnionLabel *l);

  // Report a failed enumerator lookup in an enum
  void enum_val_lookup_failure (AST_Union *t,
                                AST_Enum *e,
                                UTL_ScopedName *n);

  // Report an ambiguous name definition
  void ambiguous (UTL_Scope *s,
                  AST_Decl *l,
                  AST_Decl *d);

  // Report a forward declared interface which was never defined
  void fwd_decl_not_defined (AST_Type *d);

  // Report attempt to lookup in forward declared interface
  void fwd_decl_lookup (AST_Interface *d,
                        UTL_ScopedName *n);

  // Report a redefinition in scope
  void redefinition_in_scope (AST_Decl *t,
                              AST_Decl *s);

  // Report not a type error
  void not_a_type (AST_Decl *d);

  // Report back-end error
  void back_end (long lineno,
                 UTL_String *s);

  // Report illegal infix operator error (they can be used
  // only with integer, floating point or fixed point expressions.
  void illegal_infix (void);

  // Report local type used in remote operation.
  void local_remote_mismatch (AST_Decl *l,
                              UTL_Scope *s);

  // Improper use of -Sm option.
  void ignore_idl3_error (AST_Decl *d);

  // Valuetype violates a spec-defined constraint on a
  // valuetype used as a primary key.
  void illegal_primary_key (AST_Decl *d);

  // Template parameter was repeated.
  void duplicate_param_id (UTL_ScopedName *n);

  // Referenced template parameter not matched in param list.
  void mismatched_template_param (const char *name);

  // Given a template param of the form 'sequence<T>', the
  // 'T' must match a previous param in the list.
  void mismatch_seq_of_param (const char *param_id);

  // Informative message when a lookup fails because a
  // masking scope has not been correctly differentiated
  // in the scoped name from the scope it masks.
  void scope_masking_error (AST_Decl *masked,
                            AST_Decl *loc);

  // Error (default) or warning (set by command line option).
  void anonymous_type_diagnostic (void);

  // Reference to an item in the scope of a template
  // module was not via an alias.
  void template_scope_ref_not_aliased (AST_Decl *d);
};

#endif           // _UTL_ERR_UTL_ERR_HH
