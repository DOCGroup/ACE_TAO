
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    ifr_visitor.h
 *
 *  The Interface Repository administration visitor base class.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IFR_VISITOR_H
#define TAO_IFR_VISITOR_H

#include "ast_visitor.h"
#include "be_extern.h"
#include "tao/Environment.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ifr_visitor
 *
 * @brief ifr_visitor.
 *
 * Base class for the IfR administration visitors. The methods are
 * not pure virtual to facilitate the implementation of some
 * derived visitors that override only a few.
 */
class ifr_visitor : public ast_visitor
{
public:
  virtual ~ifr_visitor ();

  int visit_decl (AST_Decl *d) override;
  int visit_scope (UTL_Scope *node) override;
  int visit_type (AST_Type *node) override;
  int visit_predefined_type (AST_PredefinedType *node) override;
  int visit_module (AST_Module *node) override;
  int visit_interface (AST_Interface *node) override;
  int visit_interface_fwd (AST_InterfaceFwd *node) override;
  int visit_valuebox (AST_ValueBox *node) override;
  int visit_valuetype (AST_ValueType *node) override;
  int visit_valuetype_fwd (AST_ValueTypeFwd *node) override;
  int visit_component (AST_Component *node) override;
  int visit_component_fwd (AST_ComponentFwd *node) override;
  int visit_template_module (AST_Template_Module *node) override;
  int visit_template_module_inst (AST_Template_Module_Inst *node) override;
  int visit_template_module_ref (AST_Template_Module_Ref *node) override;
  int visit_porttype (AST_PortType *node) override;
  int visit_provides (AST_Provides *node) override;
  int visit_uses (AST_Uses *node) override;
  int visit_publishes (AST_Publishes *node) override;
  int visit_emits (AST_Emits *node) override;
  int visit_consumes (AST_Consumes *node) override;
  int visit_extended_port (AST_Extended_Port *node) override;
  int visit_mirror_port (AST_Mirror_Port *node) override;
  int visit_connector (AST_Connector *node) override;
  int visit_eventtype (AST_EventType *node) override;
  int visit_eventtype_fwd (AST_EventTypeFwd *node) override;
  int visit_home (AST_Home *node) override;
  int visit_factory (AST_Factory *node) override;
  int visit_finder (AST_Finder *node) override;
  int visit_structure (AST_Structure *node) override;
  int visit_structure_fwd (AST_StructureFwd *node) override;
  int visit_exception (AST_Exception *node) override;
  int visit_expression (AST_Expression *node) override;
  int visit_enum (AST_Enum *node) override;
  int visit_operation (AST_Operation *node) override;
  int visit_field (AST_Field *node) override;
  int visit_argument (AST_Argument *node) override;
  int visit_attribute (AST_Attribute *node) override;
  int visit_union (AST_Union *node) override;
  int visit_union_fwd (AST_UnionFwd *node) override;
  int visit_union_branch (AST_UnionBranch *node) override;
  int visit_union_label (AST_UnionLabel *node) override;
  int visit_constant (AST_Constant *node) override;
  int visit_enum_val (AST_EnumVal *node) override;
  int visit_array (AST_Array *node) override;
  int visit_sequence (AST_Sequence *node) override;
  int visit_string (AST_String *node) override;
  int visit_typedef (AST_Typedef *node) override;
  int visit_root (AST_Root *node) override;
  int visit_native (AST_Native *node) override;
  int visit_param_holder (AST_Param_Holder *node) override;

  /// Accessor for our lock.
  ACE_Lock &lock () const;

protected:
  // Not abstract but we want to force subclassing anyway.
  ifr_visitor ();

private:
  /// Lock.
  ACE_Lock *lock_;

  // Not supported, generate an error
  int visit_fixed (AST_Fixed *node);
};

#endif /* TAO_IFR_VISITOR_H */
