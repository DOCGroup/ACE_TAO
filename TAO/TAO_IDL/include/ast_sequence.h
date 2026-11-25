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

#ifndef _AST_SEQUENCE_AST_SEQUENCE_HH
#define _AST_SEQUENCE_AST_SEQUENCE_HH

#include "ast_concrete_type.h"

#include <memory>

class AST_Expression;
class AST_Type;

class TAO_IDL_FE_Export AST_Sequence : public virtual AST_ConcreteType
{
public:
  AST_Sequence (AST_Expression *max_size,
                AST_Type *bt,
                UTL_ScopedName *n,
                bool local,
                bool abstract);

  ~AST_Sequence () override;
  AST_Sequence (AST_Sequence const &) = delete;
  AST_Sequence (AST_Sequence &&) = delete;
  AST_Sequence &operator= (AST_Sequence const &) = delete;
  AST_Sequence &operator= (AST_Sequence &&) = delete;

  /// Check whether this type or a type in the list is involved in recursion.
  bool in_recursion (std::vector<AST_Type *> &list) override;

  // Data Accessors.
  AST_Expression *max_size () const;

  AST_Type *base_type () const;

  /**
   * Returns the fully dealiased base type if it's a typedef. If it's not a
   * typedef, it returns the same value as base_type().
   */
  AST_Type *primitive_base_type () const;

  /// Whether this sequence is unbounded.
  virtual bool unbounded () const;

  /// Recursively checks whether the type is legal as a primary key.
  /// Overridden for valuetypes, structs, sequences, unions, arrays,
  /// typedefs and interfaces.
  bool legal_for_primary_key () const override;

  /// Whether the element type is fully defined.
  bool is_defined () override;

  // Cleanup method.
  void destroy () override;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &o) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  static AST_Decl::NodeType const NT;

  /// @name Base type annotations
  /// @{
  AST_Annotation_Appls &base_type_annotations ();
  void base_type_annotations (AST_Annotation_Appls const &annotations);
  /// @}

private:
  // Data.
  /// Maximum sequence size.
  std::unique_ptr<AST_Expression> pd_max_size;

  /// Sequence base type.
  AST_Type *pd_base_type;

  /// Whether the sequence is unbounded.
  bool unbounded_ {true};

  std::unique_ptr<AST_Type> owned_base_type_;

  /// Annotations on the base type.
  AST_Annotation_Appls base_type_annotations_;
};

#endif           // _AST_SEQUENCE_AST_SEQUENCE_HH
