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

#ifndef _AST_GENERATOR_AST_GENERATOR_HH
#define _AST_GENERATOR_AST_GENERATOR_HH

#include "ast_operation.h"
#include "ast_argument.h"
#include "ast_predefined_type.h"
#include "ast_union_label.h"
#include "ast_porttype.h"

#include "fe_utils.h"

class UTL_LabelList;
class UTL_ExprList;
class AST_Root;
class AST_EventType;
class AST_EventTypeFwd;
class AST_Extended_Port;
class AST_Mirror_Port;
class AST_Connector;
class AST_Provides;
class AST_Uses;
class AST_Publishes;
class AST_Emits;
class AST_Consumes;
class AST_Template_Module;
class AST_Template_Module_Inst;
class AST_Template_Module_Ref;
class AST_Param_Holder;
class AST_Finder;

// Defines base class for node generators.

class TAO_IDL_FE_Export AST_Generator
{
public:
  AST_Generator (void) {}

  virtual ~AST_Generator (void) {}

  // Operations.

  // Create a node representing a predefined type.
  virtual AST_PredefinedType *create_predefined_type (
      AST_PredefinedType::PredefinedType t,
      UTL_ScopedName *n);

  // Create a node representing a module.
  virtual AST_Module *create_module (UTL_Scope *s,
                                     UTL_ScopedName *n);

  // Create a node representing a module which is the root of an AST.
  virtual AST_Root *create_root (UTL_ScopedName *n);

  // Create a node representing an interface.
  virtual AST_Interface *create_interface (
      UTL_ScopedName *n,
      AST_Type **inherits,
      long n_inherits,
      AST_Interface **inherits_flat,
      long n_inherits_flat,
      bool is_local,
      bool is_abstract);

  // Create a node representing a forward declaration of an interface.
  virtual AST_InterfaceFwd *create_interface_fwd (
      UTL_ScopedName *n,
      bool is_local,
      bool is_abstract);

  // Create a node representing a valuetype.
  virtual AST_ValueType *create_valuetype (
      UTL_ScopedName *n,
      AST_Type **inherits,
      long n_inherits,
      AST_Type *inherits_concrete,
      AST_Interface **inherits_flat,
      long n_inherits_flat,
      AST_Type **supports_list,
      long n_supports,
      AST_Type *supports_concrete,
      bool is_abstract,
      bool is_truncatable,
      bool is_custom);

  // Create a node representing a forward declaration of a valuetype.
  virtual AST_ValueTypeFwd *create_valuetype_fwd (
      UTL_ScopedName *n,
      bool is_abstract);

  // Create a node representing an eventtype.
  virtual AST_EventType *create_eventtype (
      UTL_ScopedName *n,
      AST_Type **inherits,
      long n_inherits,
      AST_Type *inherits_concrete,
      AST_Interface **inherits_flat,
      long n_inherits_flat,
      AST_Type **supports_list,
      long n_supports,
      AST_Type *supports_concrete,
      bool is_abstract,
      bool is_truncatable,
      bool is_custom);

  // Create a node representing a forward declaration of an eventtype.
  virtual AST_EventTypeFwd *create_eventtype_fwd (
      UTL_ScopedName *n,
      bool is_abstract);

  // Create a node representing a component.
  virtual AST_Component *create_component (
      UTL_ScopedName *n,
      AST_Component *base_component,
      AST_Type **supports_list,
      long n_supports,
      AST_Interface **supports_flat,
      long n_supports_flat);

  // Create a node representing a forward declaration of a component.
  virtual AST_ComponentFwd *create_component_fwd (
      UTL_ScopedName *n);

  // Create a node representing a component home.
  virtual AST_Home *create_home (
      UTL_ScopedName *n,
      AST_Home *base_home,
      AST_Component *managed_component,
      AST_Type *primary_key,
      AST_Type **support_lists,
      long n_supports,
      AST_Interface **supports_flat,
      long n_supports_flat);

  // Create a node representing an exception.
  virtual AST_Exception *create_exception (
      UTL_ScopedName *n,
      bool is_local,
      bool is_abstract);

  // Create a node representing a structure.
  virtual AST_Structure *create_structure (
      UTL_ScopedName *n,
      bool is_local,
      bool is_abstract);

  // Create a node representing a forward declaration of a structure.
  virtual AST_StructureFwd *create_structure_fwd (UTL_ScopedName *n);

  // Create a node representing an enum.
  virtual AST_Enum *create_enum (UTL_ScopedName *n,
                                 bool is_local,
                                 bool is_abstract);

  // Create a node representing an operation on an interface.
  virtual AST_Operation *create_operation (
      AST_Type *rt,
      AST_Operation::Flags fl,
      UTL_ScopedName *n,
      bool is_local,
      bool is_abstract);

  // Create a node representing a field in a structure, exception or
  // union.
  virtual AST_Field *create_field (AST_Type *ft,
                                   UTL_ScopedName *n,
                                   AST_Field::Visibility vis =
                                     AST_Field::vis_NA);

  // Create a node representing an argument to an operation.
  virtual AST_Argument *create_argument (
      AST_Argument::Direction d,
      AST_Type *ft,
      UTL_ScopedName *n);

  // Create a node representing an attribute.
  virtual AST_Attribute *create_attribute (
      bool ro,
      AST_Type *ft,
      UTL_ScopedName *n,
      bool is_local,
      bool is_abstract);

  // Create a node representing a union.
  virtual AST_Union *create_union (AST_ConcreteType *dt,
                                   UTL_ScopedName *n,
                                   bool is_local,
                                   bool is_abstract);

  // Create a node representing a forward declaration of a union.
  virtual AST_UnionFwd *create_union_fwd (UTL_ScopedName *n);

  // Create a node representing one branch in a union.
  virtual AST_UnionBranch *create_union_branch (
      UTL_LabelList *ll,
      AST_Type *ft,
      UTL_ScopedName *n);

  // Create a node representing a label on a union branch.
  virtual AST_UnionLabel *create_union_label (AST_UnionLabel::UnionLabel ul,
                                              AST_Expression *lv);

  // Create a node representing a constant
  virtual AST_Constant *create_constant (
      AST_Expression::ExprType et,
      AST_Expression *ev,
      UTL_ScopedName *n);

  // Create various kinds of nodes representing expressions.

  virtual AST_Expression *create_expr (UTL_ScopedName *n);

  virtual AST_Expression *create_expr (AST_Expression *v,
                                       AST_Expression::ExprType t);

  virtual AST_Expression *create_expr (AST_Expression::ExprComb c,
                                       AST_Expression *v1,
                                       AST_Expression *v2);

  virtual AST_Expression *create_expr (ACE_CDR::Long v);

  virtual AST_Expression *create_expr (ACE_CDR::LongLong v);

  virtual AST_Expression *create_expr (ACE_CDR::Boolean b);

  virtual AST_Expression *create_expr (ACE_CDR::ULong v);

  virtual AST_Expression *create_expr (ACE_CDR::ULongLong v);

  virtual AST_Expression *create_expr (ACE_CDR::ULong v,
                                       AST_Expression::ExprType t);

  virtual AST_Expression *create_expr (UTL_String *s);

  virtual AST_Expression *create_expr (char *s);

  virtual AST_Expression *create_expr (ACE_CDR::Char c);

  virtual AST_Expression *create_expr (ACE_OutputCDR::from_wchar wc);

  virtual AST_Expression *create_expr (ACE_CDR::Double d);

  // Create a node representing an enumerator.
  virtual AST_EnumVal *create_enum_val (ACE_CDR::ULong v,
                                        UTL_ScopedName *n);

  // Create a node representing an array type.
  virtual AST_Array *create_array (UTL_ScopedName *n,
                                   ACE_CDR::ULong ndims,
                                   UTL_ExprList *dims,
                                   bool is_local,
                                   bool is_abstract);

  // Create a node representing a sequence type.
  virtual AST_Sequence *create_sequence (AST_Expression *v,
                                         AST_Type *bt,
                                         UTL_ScopedName *n,
                                         bool is_local,
                                         bool is_abstract);

  // Create a node representing a string type.
  virtual AST_String *create_string (AST_Expression *v);

  // Create a node representing a wide string type.
  virtual AST_String *create_wstring (AST_Expression *v);

  // Create a node representing a type renaming (typedef).
  virtual AST_Typedef *create_typedef (AST_Type *bt,
                                       UTL_ScopedName *n,
                                       bool is_local,
                                       bool is_abstract);

  // Create a node representing a native.
  virtual AST_Native *create_native (UTL_ScopedName *n);

  // Create a node representing an OBV or home factory construct.
  virtual AST_Factory *create_factory (UTL_ScopedName *n);

  // Create a node representing an home finder construct.
  virtual AST_Finder *create_finder (UTL_ScopedName *n);

  // Create a node representing a valuebox.
  virtual AST_ValueBox *create_valuebox (UTL_ScopedName *n,
                                         AST_Type *boxed_type);

  virtual
  AST_PortType *create_porttype (UTL_ScopedName *n);

  virtual
  AST_Provides *create_provides (UTL_ScopedName *n,
                                 AST_Type *provides_type);

  virtual
  AST_Uses *create_uses (UTL_ScopedName *n,
                         AST_Type *uses_type,
                         bool is_multiple);

  virtual
  AST_Publishes *create_publishes (UTL_ScopedName *n,
                                   AST_Type *publishes_type);

  virtual
  AST_Emits *create_emits (UTL_ScopedName *n,
                           AST_Type *emits_type);

  virtual
  AST_Consumes *create_consumes (UTL_ScopedName *n,
                                 AST_Type *consumes_type);

  virtual
  AST_Extended_Port *create_extended_port (
    UTL_ScopedName *n,
    AST_PortType *porttype_ref);

  virtual
  AST_Mirror_Port *create_mirror_port (
    UTL_ScopedName *n,
    AST_PortType *porttype_ref);

  virtual
  AST_Connector *create_connector (
    UTL_ScopedName  *n,
    AST_Connector *base_connector);

  virtual
  AST_Template_Module *create_template_module (
    UTL_ScopedName *n,
    FE_Utils::T_PARAMLIST_INFO *template_params);

  virtual
  AST_Template_Module_Inst *create_template_module_inst (
    UTL_ScopedName *n,
    AST_Template_Module *ref,
    FE_Utils::T_ARGLIST *template_args);

  virtual
  AST_Template_Module_Ref *create_template_module_ref (
    UTL_ScopedName *n,
    AST_Template_Module *ref,
    UTL_StrList *param_refs);

  virtual
  AST_Param_Holder *create_param_holder (
    UTL_ScopedName *parameter_name,
    FE_Utils::T_Param_Info *info);
};

#endif           // _AST_GENERATOR_AST_GENERATOR_HH
