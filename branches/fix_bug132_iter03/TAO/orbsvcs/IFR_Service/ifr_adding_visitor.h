// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IFR_BE_DLL
//
// = FILENAME
//    ifr_adding_visitor.h
//
// = DESCRIPTION
//    Header file for class ifr_adding_visitor.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_IFR_ADDING_VISITOR_H
#define TAO_IFR_ADDING_VISITOR_H

#include "ifr_visitor.h"
#include "idl.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ifr_adding_visitor : public ifr_visitor
{
  //
  // = TITLE
  //    ifr_adding_visitor.
  //
  // = DESCRIPTION
  //    This visitor adds items found in the IDL file being processed
  //    to the Interface Repository.
  //
public:
  ifr_adding_visitor (void);
  // Constructor.

  virtual ~ifr_adding_visitor (void);
  // Destructor.

  virtual int visit_scope (UTL_Scope *node);
  // Visit the scope.

  virtual int visit_predefined_type (AST_PredefinedType *node);
  // Visit the predefined type.

  virtual int visit_module (AST_Module *node);
  // Visit module.

  virtual int visit_interface (AST_Interface *node);
  // Visit interface.

  virtual int visit_interface_fwd (AST_InterfaceFwd *node);
  // Visit interface_fwd.

  virtual int visit_valuetype (AST_Interface *node);
  // Visit valuetype.

  virtual int visit_valuetype_fwd (AST_InterfaceFwd *node);
  // Visit valuetype_fwd.

  virtual int visit_structure (AST_Structure *node);
  // Visit a structure.

  virtual int visit_exception (AST_Exception *node);
  // Visit exception

  virtual int visit_enum (AST_Enum *node);
  // Visit an enum.

  virtual int visit_operation (AST_Operation *node);
  // Visit an operation.

  virtual int visit_field (AST_Field *node);
  // Visit a field.

  virtual int visit_attribute (AST_Attribute *node);
  // Visit an attribute.

  virtual int visit_union (AST_Union *node);
  // Visit union.

  virtual int visit_union_branch (AST_UnionBranch *node);
  // Visit union branch.

  virtual int visit_constant (AST_Constant *node);
  // Visit a constant.

  virtual int visit_array (AST_Array *node);
  // Visit an array.

  virtual int visit_sequence (AST_Sequence *node);
  // Visit a sequence.

  virtual int visit_string (AST_String *node);
  // Visit a string.

  virtual int visit_typedef (AST_Typedef *node);
  // Visit a typedef.

  virtual int visit_root (AST_Root *node);
  // Visit the root.

  virtual int visit_native (AST_Native *node);
  // Visit a native.

protected:
  IR_PrimitiveKind expr_type_to_pkind (AST_Expression::ExprType et);
  IR_PrimitiveKind predefined_type_to_pkind (AST_PredefinedType *node);
  // Conversion functions.

  void load_any (AST_Expression::AST_ExprValue *ev,
                 CORBA::Any &any);
  // Determine the primitive type and insert into the Any.

  const char *gen_version (AST_Decl *node);
  // Defaults to "1.0" for now.

  void element_type (AST_Type *base_type,
                     CORBA::Environment &ACE_TRY_ENV);
  // Creates or looks up the element type of an array or sequence,
  // and stores the result in ir_current_.

  void redef_warning (AST_Decl *node);
  // If a type that can be forward declared is seen in an IDL file,
  // and there is already a repository entry for a declaration of
  // the same type, it may be an error. This warning lets the user
  // decide.

  IR_IDLType_var ir_current_;
  // Holder for the IR object most recently created or looked up by
  // the visitor. This makes it accessible by visitor methods that
  // need the result of a call to another visitor method that
  // creates an IR object.
};

#endif /* TAO_IFR_ADDING_VISITOR_H */

