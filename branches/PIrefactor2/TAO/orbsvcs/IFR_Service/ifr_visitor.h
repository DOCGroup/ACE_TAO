// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IFR_BE_DLL
//
// = FILENAME
//    ifr_visitor.h
//
// = DESCRIPTION
//    The Interface Repository administration visitor base class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_IFR_VISITOR_H
#define TAO_IFR_VISITOR_H

#include "ast_visitor.h"
#include "be_extern.h"
#include "tao/Environment.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ifr_visitor : public ast_visitor
{
  //
  // = TITLE
  //    ifr_visitor.
  //
  // = DESCRIPTION
  //    Base class for the IfR administration visitors. The methods are
  //    not pure virtual to facilitate the implementation of some
  //    derived visitors that override only a few.
  //
public:
  ifr_visitor (void);
  // Constructor.

  virtual ~ifr_visitor (void);
  // Destructor.

  virtual int visit_decl (AST_Decl *d);
  // Visit a decl node.

  virtual int visit_scope (UTL_Scope *node);
  // Visit the scope.

  virtual int visit_type (AST_Type *node);
  // Visit the type.

  virtual int visit_predefined_type (AST_PredefinedType *node);
  // Visit the predefined type.

  virtual int visit_module (AST_Module *node);
  // Visit module.

  virtual int visit_interface (AST_Interface *node);
  // Visit interface.

  virtual int visit_interface_fwd (AST_InterfaceFwd *node);
  // Visit a forward interface declaration.

  virtual int visit_valuetype (AST_ValueType *node);
  // Visit valuetype.

  virtual int visit_valuetype_fwd (AST_ValueTypeFwd *node);
  // Visit valuetype_fwd

  virtual int visit_component (AST_Component *node);
  // Visit component.

  virtual int visit_component_fwd (AST_ComponentFwd *node);
  // Visit component_fwd

  virtual int visit_eventtype (AST_EventType *node);
  // Visit valuetype.

  virtual int visit_eventtype_fwd (AST_EventTypeFwd *node);
  // Visit valuetype_fwd

  virtual int visit_home (AST_Home *node);
  // Visit component home.

  virtual int visit_factory (AST_Factory *node);
  // Visit a factory construct.

  virtual int visit_structure (AST_Structure *node);
  // Visit a structure.

  virtual int visit_structure_fwd (AST_StructureFwd *node);
  // Visit a forward structure declaration.

  virtual int visit_exception (AST_Exception *node);
  // Visit exception

  virtual int visit_expression (AST_Expression *node);
  // Visit expression.

  virtual int visit_enum (AST_Enum *node);
  // Visit an enum.

  virtual int visit_operation (AST_Operation *node);
  // Visit an operation.

  virtual int visit_field (AST_Field *node);
  // Visit a field.

  virtual int visit_argument (AST_Argument *node);
  // Visit argument.

  virtual int visit_attribute (AST_Attribute *node);
  // Visit an attribute.

  virtual int visit_union (AST_Union *node);
  // Visit union.

  virtual int visit_union_fwd (AST_UnionFwd *node);
  // Visit a forward union declaration.

  virtual int visit_union_branch (AST_UnionBranch *node);
  // Visit union branch.

  virtual int visit_union_label (AST_UnionLabel *node);
  // Visit union label.

  virtual int visit_constant (AST_Constant *node);
  // Visit a constant.

  virtual int visit_enum_val (AST_EnumVal *node);
  // Visit enum val.

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

  ACE_Lock &lock (void) const;
  // Accessor for our lock.

private:
  ACE_Lock *lock_;
  // Lock.
};

#endif /* TAO_IFR_VISITOR_H */
