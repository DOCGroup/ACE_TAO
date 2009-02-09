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

#ifndef _BE_GENERATOR_BE_GENERATOR_HH
#define _BE_GENERATOR_BE_GENERATOR_HH

// Defines BE class for generator
//
// This defines the same protocol as the AST_Generator class but actually
// is implemented to create nodes of AST classes subclassed in this BE

#include "TAO_IDL_BE_Export.h"
#include "ast_generator.h"

class TAO_IDL_BE_Export be_generator : public AST_Generator
{
public:
  // Create a node representing a module which is the root of an AST made
  // up of BE nodes.
  virtual AST_Root *create_root (UTL_ScopedName *n);

  virtual AST_PredefinedType *create_predefined_type (
      AST_PredefinedType::PredefinedType t,
      UTL_ScopedName *n
    );

  virtual AST_Module *create_module (UTL_Scope *s,
                                     UTL_ScopedName *n);

  virtual AST_Interface *create_interface (UTL_ScopedName *n,
                                           AST_Interface **ih,
                                           long nih,
                                           AST_Interface **ih_flat,
                                           long nih_flat,
                                           bool is_local,
                                           bool is_abstract);

  virtual AST_InterfaceFwd *create_interface_fwd (UTL_ScopedName *n,
                                                  bool is_local,
                                                  bool is_abstract);

  // Create a node representing a boxed value type
  virtual AST_ValueBox *create_valuebox (UTL_ScopedName *n,
                                         AST_Type *boxed_type);

  virtual AST_ValueType *create_valuetype (UTL_ScopedName *n,
                                           AST_Interface **inherits,
                                           long n_inherits,
                                           AST_ValueType *inherits_concrete,
                                           AST_Interface **inherits_flat,
                                           long n_inherits_flat,
                                           AST_Interface **supports_list,
                                           long n_supports,
                                           AST_Interface *supports_concrete,
                                           bool is_abstract,
                                           bool is_truncatable,
                                           bool is_custom);

  virtual AST_ValueTypeFwd *create_valuetype_fwd (UTL_ScopedName *n,
                                                  bool abstract);

  virtual AST_EventType *create_eventtype (UTL_ScopedName *n,
                                           AST_Interface **inherits,
                                           long n_inherits,
                                           AST_ValueType *inherits_concrete,
                                           AST_Interface **inherits_flat,
                                           long n_inherits_flat,
                                           AST_Interface **supports_list,
                                           long n_supports,
                                           AST_Interface *supports_concrete,
                                           bool is_abstract,
                                           bool is_truncatable,
                                           bool is_custom);

  virtual AST_EventTypeFwd *create_eventtype_fwd (UTL_ScopedName *n,
                                                  bool abstract);

  virtual AST_Component *create_component (UTL_ScopedName *n,
                                           AST_Component *base_component,
                                           AST_Interface **supports_list,
                                           long n_supports,
                                           AST_Interface **supports_flat,
                                           long n_supports_flat);

  virtual AST_ComponentFwd *create_component_fwd (UTL_ScopedName *n);

  virtual AST_Home *create_home (UTL_ScopedName *n,
                                 AST_Home *base_home,
                                 AST_Component *managed_component,
                                 AST_ValueType *primary_key,
                                 AST_Interface **supports_list,
                                 long n_supports,
                                 AST_Interface **supports_flat,
                                 long n_supports_flat);

  virtual AST_Exception *create_exception (UTL_ScopedName *n,
                                           bool is_local,
                                           bool is_abstract);

  virtual AST_Structure *create_structure (UTL_ScopedName *n,
                                           bool is_local,
                                           bool is_abstract);

  virtual AST_StructureFwd *create_structure_fwd (UTL_ScopedName *n);

  virtual AST_Enum *create_enum (UTL_ScopedName *n,
                                 bool is_local,
                                 bool is_abstract);

  virtual AST_Operation *create_operation (AST_Type *rt,
                                           AST_Operation::Flags fl,
                                           UTL_ScopedName *n,
                                           bool is_local,
                                           bool is_abstract);

  virtual AST_Field *create_field (AST_Type *ft, UTL_ScopedName *n,
                                   AST_Field::Visibility vis =
                                     AST_Field::vis_NA);

  virtual AST_Argument *create_argument (AST_Argument::Direction d,
                                         AST_Type *ft,
                                         UTL_ScopedName *n);

  virtual AST_Attribute *create_attribute (bool ro,
                                           AST_Type *ft,
                                           UTL_ScopedName *n,
                                           bool is_local,
                                           bool is_abstract);

  virtual AST_Union *create_union (AST_ConcreteType *dt,
                                   UTL_ScopedName *n,
                                   bool is_local,
                                   bool is_abstract);

  virtual AST_UnionFwd *create_union_fwd (UTL_ScopedName *n);

  virtual AST_UnionBranch *create_union_branch (UTL_LabelList *ll,
                                                AST_Type *ft,
                                                UTL_ScopedName *n);

  virtual AST_UnionLabel *create_union_label (AST_UnionLabel::UnionLabel ul,
                                              AST_Expression *lv);

  virtual AST_Constant *create_constant (AST_Expression::ExprType et,
                                         AST_Expression *ev,
                                         UTL_ScopedName *n);

  virtual AST_Expression *create_expr (UTL_ScopedName *n);

  virtual AST_Expression *create_expr (AST_Expression *v,
                                       AST_Expression::ExprType t);

  virtual AST_Expression *create_expr (AST_Expression::ExprComb c,
                                       AST_Expression *v1,
                                       AST_Expression *v2);

  virtual AST_Expression *create_expr (ACE_CDR::Long v);

  virtual AST_Expression *create_expr (ACE_CDR::Boolean b);

  virtual AST_Expression *create_expr (ACE_CDR::ULong v,
                                       AST_Expression::ExprType t);

  virtual AST_Expression *create_expr( UTL_String *s);

  virtual AST_Expression *create_expr (char *s);

  virtual AST_Expression *create_expr (ACE_CDR::Char c);

  virtual AST_Expression *create_expr (ACE_OutputCDR::from_wchar wc);

  virtual AST_Expression *create_expr (ACE_CDR::Double d);

  virtual AST_EnumVal *create_enum_val (ACE_CDR::ULong v,
                                        UTL_ScopedName *n);

  virtual AST_Array *create_array (UTL_ScopedName *n,
                                   ACE_CDR::ULong ndims,
                                   UTL_ExprList *dims,
                                   bool is_local,
                                   bool is_abstract);

  virtual AST_Sequence *create_sequence (AST_Expression *v,
                                         AST_Type *bt,
                                         UTL_ScopedName *n,
                                         bool is_local,
                                         bool is_abstract);

  virtual AST_String *create_string (AST_Expression *v);

  virtual AST_String *create_wstring (AST_Expression *v);

  virtual AST_Typedef *create_typedef (AST_Type *bt,
                                       UTL_ScopedName *n,
                                       bool is_local,
                                       bool is_abstract);

  // Create a node representing a native
  virtual AST_Native *create_native (UTL_ScopedName *n);

  // Create a node representing a OBV factory construct
  virtual AST_Factory *create_factory (UTL_ScopedName *n);
};

#endif           // _BE_GENERATOR_BE_GENERATOR_HH
