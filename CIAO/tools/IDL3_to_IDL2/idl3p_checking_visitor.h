
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    idl3p_checking_visitor.h
 *
 *  $Id$
 *
 *  Header file for visitor that checks modules for the
 *  presence of declarations other than non-code-generating
 *  IDL3+ constructs. Prevents the generation of illegal
 *  empty modules in the *_IDL2 artifact.
 *
 *
 */
//=============================================================================


#ifndef TAO_IDL3P_CHECKING_VISITOR_H
#define TAO_IDL3P_CHECKING_VISITOR_H

#include "basic_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "TAO_IDL3_TO_IDL2_BE_Export.h"

class TAO_IDL3_TO_IDL2_BE_Export idl3p_checking_visitor
  : public basic_visitor
{
  //
  // = TITLE
  //    idl3p_checking_visitor
  //
  // = DESCRIPTION
  //    Checks for modules empty of anything except template
  //    module and porttype declarations.
  //
public:
  idl3p_checking_visitor (void);
  virtual ~idl3p_checking_visitor (void);

  virtual int visit_scope (UTL_Scope *node);
  virtual int visit_module (AST_Module *node);
  virtual int visit_interface (AST_Interface *node);
  virtual int visit_interface_fwd (AST_InterfaceFwd *node);
  virtual int visit_valuebox (AST_ValueBox *node);
  virtual int visit_valuetype (AST_ValueType *node);
  virtual int visit_valuetype_fwd (AST_ValueTypeFwd *node);
  virtual int visit_component (AST_Component *node);
  virtual int visit_component_fwd (AST_ComponentFwd *node);
  virtual int visit_template_module_inst (AST_Template_Module_Inst *node);
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
  virtual int visit_array (AST_Array *node);
  virtual int visit_sequence (AST_Sequence *node);
  virtual int visit_string (AST_String *node);
  virtual int visit_typedef (AST_Typedef *node);
  virtual int visit_root (AST_Root *node);
  virtual int visit_native (AST_Native *node);

  bool needs_codegen (void) const;

private:
  bool needs_codegen_;
};

#endif // TAO_IDL3P_CHECKING_VISITOR_H
