// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IDL_FE_DLL
//
// = FILENAME
//    ast_visitor.h
//
// = DESCRIPTION
//    The abstract AST visitor base class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_IDL_FE_DLL_AST_VISITOR_H
#define TAO_IDL_FE_DLL_AST_VISITOR_H

#include "TAO_IDL_FE_Export.h"

class AST_Decl;
class UTL_Scope;
class AST_Type;
class AST_PredefinedType;
class AST_Module;
class AST_Interface;
class AST_InterfaceFwd;

// These classes don't exist now, but they may someday.
//  class AST_Valuetype;
//  class AST_ValuetypeFwd;
class AST_Factory;

class AST_Structure;
class AST_Exception;
class AST_Expression;
class AST_Enum;
class AST_Operation;
class AST_Field;
class AST_Argument;
class AST_Attribute;
class AST_Union;
class AST_UnionBranch;
class AST_UnionLabel;
class AST_Constant;
class AST_EnumVal;
class AST_Array;
class AST_Sequence;
class AST_String;
class AST_Typedef;
class AST_Root;
class AST_Native;

class TAO_IDL_FE_Export ast_visitor
{
  //
  // = TITLE
  //    ast_visitor.
  //
  // = DESCRIPTION
  //    To implement BE processing we use the "Visitor Pattern", this
  //    is the abstract "Visitor". This class is the abstract base class
  //    for visitors that traverse the AST_<node> classes. Other abstract
  //    classes for C++ code generation, Java code generation, or IfR
  //    administration, for example, can be derived from this class and
  //    be the root of their own subtree of specialized visitor classes.
  //
public:
  ast_visitor (void);
  // Constructor.

  virtual ~ast_visitor (void);
  // Destructor.

  virtual int visit_decl (AST_Decl *d) = 0;
  // Visit a decl node.

  virtual int visit_scope (UTL_Scope *node) = 0;
  // Visit the scope

  virtual int visit_type (AST_Type *node) = 0;
  // Visit the type.

  virtual int visit_predefined_type (AST_PredefinedType *node) = 0;
  // Visit the predefined type.

  virtual int visit_module (AST_Module *node) = 0;
  // Visit module.

  virtual int visit_interface (AST_Interface *node) = 0;
  // Visit interface.

  virtual int visit_interface_fwd (AST_InterfaceFwd *node) = 0;
  // Visit interface_fwd

  virtual int visit_valuetype (AST_Interface *node) = 0;
  // Visit valuetype.

  virtual int visit_valuetype_fwd (AST_InterfaceFwd *node) = 0;
  // Visit valuetype_fwd

  virtual int visit_factory (AST_Factory *node) = 0;
  // Visit a OBV factory construct.

  virtual int visit_structure (AST_Structure *node) = 0;
  // Visit a structure.

  virtual int visit_exception (AST_Exception *node) = 0;
  // Visit exception

  virtual int visit_expression (AST_Expression *node) = 0;
  // Visit expression.

  virtual int visit_enum (AST_Enum *node) = 0;
  // Visit an enum.

  virtual int visit_operation (AST_Operation *node) = 0;
  // Visit an operation.

  virtual int visit_field (AST_Field *node) = 0;
  // Visit a field.

  virtual int visit_argument (AST_Argument *node) = 0;
  // Visit argument.

  virtual int visit_attribute (AST_Attribute *node) = 0;
  // Visit an attribute.

  virtual int visit_union (AST_Union *node) = 0;
  // Visit union.

  virtual int visit_union_branch (AST_UnionBranch *node) = 0;
  // Visit union branch.

  virtual int visit_union_label (AST_UnionLabel *node) = 0;
  // Visit union label.

  virtual int visit_constant (AST_Constant *node) = 0;
  // Visit a constant.

  virtual int visit_enum_val (AST_EnumVal *node) = 0;
  // Visit enum val.

  virtual int visit_array (AST_Array *node) = 0;
  // Visit an array.

  virtual int visit_sequence (AST_Sequence *node) = 0;
  // Visit a sequence.

  virtual int visit_string (AST_String *node) = 0;
  // Visit a string.

  virtual int visit_typedef (AST_Typedef *node) = 0;
  // Visit a typedef.

  virtual int visit_root (AST_Root *node) = 0;
  // Visit a root.

  virtual int visit_native (AST_Native *node) = 0;
  // Visit a native.
};

#endif /* TAO_IDL_FE_DLL_AST_VISITOR_H */
