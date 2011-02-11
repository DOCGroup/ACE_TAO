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
  virtual ~ifr_visitor (void);

  virtual int visit_decl (AST_Decl *d);
  virtual int visit_scope (UTL_Scope *node);
  virtual int visit_type (AST_Type *node);
  virtual int visit_predefined_type (AST_PredefinedType *node);
  virtual int visit_module (AST_Module *node);
  virtual int visit_interface (AST_Interface *node);
  virtual int visit_interface_fwd (AST_InterfaceFwd *node);
  virtual int visit_valuebox (AST_ValueBox *node);
  virtual int visit_valuetype (AST_ValueType *node);
  virtual int visit_valuetype_fwd (AST_ValueTypeFwd *node);
  virtual int visit_component (AST_Component *node);
  virtual int visit_component_fwd (AST_ComponentFwd *node);
  virtual int visit_template_module (AST_Template_Module *node);
  virtual int visit_template_module_inst (AST_Template_Module_Inst *node);
  virtual int visit_template_module_ref (AST_Template_Module_Ref *node);
  virtual int visit_porttype (AST_PortType *node);
  virtual int visit_provides (AST_Provides *node);
  virtual int visit_uses (AST_Uses *node);
  virtual int visit_publishes (AST_Publishes *node);
  virtual int visit_emits (AST_Emits *node);
  virtual int visit_consumes (AST_Consumes *node);
  virtual int visit_extended_port (AST_Extended_Port *node);
  virtual int visit_mirror_port (AST_Mirror_Port *node);
  virtual int visit_connector (AST_Connector *node);
  virtual int visit_eventtype (AST_EventType *node);
  virtual int visit_eventtype_fwd (AST_EventTypeFwd *node);
  virtual int visit_home (AST_Home *node);
  virtual int visit_factory (AST_Factory *node);
  virtual int visit_finder (AST_Finder *node);
  virtual int visit_structure (AST_Structure *node);
  virtual int visit_structure_fwd (AST_StructureFwd *node);
  virtual int visit_exception (AST_Exception *node);
  virtual int visit_expression (AST_Expression *node);
  virtual int visit_enum (AST_Enum *node);
  virtual int visit_operation (AST_Operation *node);
  virtual int visit_field (AST_Field *node);
  virtual int visit_argument (AST_Argument *node);
  virtual int visit_attribute (AST_Attribute *node);
  virtual int visit_union (AST_Union *node);
  virtual int visit_union_fwd (AST_UnionFwd *node);
  virtual int visit_union_branch (AST_UnionBranch *node);
  virtual int visit_union_label (AST_UnionLabel *node);
  virtual int visit_constant (AST_Constant *node);
  virtual int visit_enum_val (AST_EnumVal *node);
  virtual int visit_array (AST_Array *node);
  virtual int visit_sequence (AST_Sequence *node);
  virtual int visit_string (AST_String *node);
  virtual int visit_typedef (AST_Typedef *node);
  virtual int visit_root (AST_Root *node);
  virtual int visit_native (AST_Native *node);
  virtual int visit_param_holder (AST_Param_Holder *node);

  ACE_Lock &lock (void) const;
  // Accessor for our lock.

protected:
  // Not abstract but we want to force subclassing anyway.
  ifr_visitor (void);

private:
  ACE_Lock *lock_;
  // Lock.
};

#endif /* TAO_IFR_VISITOR_H */
