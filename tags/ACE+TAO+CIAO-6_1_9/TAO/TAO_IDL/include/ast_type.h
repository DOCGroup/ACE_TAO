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

#include "ace/Unbounded_Queue.h"

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

  AST_Type (AST_Decl::NodeType nt,
            UTL_ScopedName *n);

  virtual ~AST_Type (void);

  virtual bool in_recursion (ACE_Unbounded_Queue<AST_Type *> &list);
  // Determine if we are involved in some kind of limited recursion.
  // Most types cannot be involved except structs and unions.
  // The head element of the list the possibly recursed type we are checking for.
  // If the list is empty, we are trying to determine this for ourselves.

  virtual void size_type (SIZE_TYPE);
  // Set the size type.

  virtual SIZE_TYPE size_type (void);
  // Return our size type.

  // Accessors/mutators for the private members.

  bool has_constructor (void);
  // Accessor for protected member.

  void has_constructor (bool value);
  // Mutator for protected member.

  bool ifr_added (void);
  void ifr_added (bool val);

  bool ifr_fwd_added (void);
  void ifr_fwd_added (bool val);

  const char *nested_type_name (AST_Decl *d,
                                const char *suffix = 0,
                                const char *prefix = 0);
  // Type name of a node used when generating declarations.

  AST_Type *unaliased_type (void);
  // Utility function to make sure we are using the unaliased type.

  virtual bool legal_for_primary_key (void) const;
  // Recursively called on valuetype to check for legal use as
  // a primary key. Overridden for valuetype, struct, sequence,
  // union, array, typedef, and interface.

  // Narrowing.

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

  bool match_names (AST_Type *t, ACE_Unbounded_Queue<AST_Type *> &list);

protected:
  // Has the full definition been added to the Interface Repository?
  // Used for types which can have members and can be forward declared.
  bool ifr_added_;

  // Has this node been forward declared in this IDL file?
  bool ifr_fwd_added_;

  SIZE_TYPE size_type_;
  // Whether we are fixed or variable size (by default fixed).

  bool has_constructor_;
  // Attribute that helps a union determine whether a member
  // should be included by value or by reference.

  char *nested_type_name_;
  // For the corresponding method.

  long in_recursion_;
  // Storage once the value has been computed.

  mutable bool recursing_in_legal_pk_;
  // Node-specific flag to abort recursion in legal_for_primary_key().
};

#endif           // _AST_TYPE_AST_TYPE_HH
