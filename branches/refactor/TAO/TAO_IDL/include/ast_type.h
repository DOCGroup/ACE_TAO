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

#ifndef _AST_TYPE_AST_TYPE_HH
#define _AST_TYPE_AST_TYPE_HH

#include "ast_decl.h"

// Class for all IDL types
//
// This is useful wherever any IDL type defining construct can appear
// such as the base type for a typedef or array.

class TAO_IDL_FE_Export AST_Type : public virtual AST_Decl
{
public:
  enum SIZE_TYPE
  {
    SIZE_UNKNOWN,
    FIXED,
    VARIABLE
  };
  // Indicates if we are fixed size or variable. Most useful for structs,
  // unions, and arrays.

  // Operations.

  AST_Type (void);

  AST_Type (AST_Decl::NodeType nt,
            UTL_ScopedName *n);

  virtual ~AST_Type (void);

  virtual idl_bool in_recursion (AST_Type *node = 0);
  // Determine if we are involved in some kind of limited recursion.
  // Most types cannot be involved except structs and unions.
  // If the parameter is 0, we are trying to determine this for ourselves.

  // To be overridden by the subclasses interface, struct, union, and
  // the corresponding forward declaration classes.
  virtual idl_bool is_defined (void);

  virtual void size_type (SIZE_TYPE);
  // Set the size type.

  virtual SIZE_TYPE size_type (void);
  // Return our size type.

  // Accessors/mutators for the private members.

  idl_bool has_constructor (void);
  // Accessor for protected member.

  void has_constructor (idl_bool value);
  // Mutator for protected member.

  idl_bool ifr_added (void);
  void ifr_added (idl_bool val);

  idl_bool ifr_fwd_added (void);
  void ifr_fwd_added (idl_bool val);

  const char *nested_type_name (AST_Decl *d,
                                const char *suffix = 0,
                                const char *prefix = 0);
  // Type name of a node used when generating declarations.

  idl_bool seq_cdr_hdr_gen (void) const;
  void seq_cdr_hdr_gen (idl_bool val);
  idl_bool seq_cdr_body_gen (void) const;
  void seq_cdr_body_gen (idl_bool val);
  // Accessors for the members.

  // Narrowing.
  DEF_NARROW_METHODS1(AST_Type, AST_Decl);
  DEF_NARROW_FROM_DECL(AST_Type);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  // Cleanup.
  virtual void destroy (void);

protected:
  virtual int compute_size_type (void);
  // Determine our size type and set it if it is unknown.

  const char *nested_name (const char *local_name,
                           const char *full_name,
                           AST_Decl *use_scope,
                           const char *suffix,
                           const char *prefix);
  // Type name of a node used when generating declarations.

  // Has the full definition been added to the Interface Repository?
  // Used for types which can have members and can be forward declared.
  idl_bool ifr_added_;

  // Has this node been forward declared in this IDL file?
  idl_bool ifr_fwd_added_;

  SIZE_TYPE size_type_;
  // Whether we are fixed or variable size (by default fixed).

  idl_bool has_constructor_;
  // Attribute that helps a union determine whether a member
  // should be included by value or by reference.

  char *nested_type_name_;
  // For the corresponding method.

  idl_bool seq_cdr_hdr_gen_;
  idl_bool seq_cdr_body_gen_;
  // Helps in avoiding duplicate generation of CDR stream operators
  // for sequences.
};

#endif           // _AST_TYPE_AST_TYPE_HH
