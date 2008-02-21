// $Id$

/* -*- C++ -*- */
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
#include "tao/IFR_Client/IFR_ComponentsC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class UTL_ExceptList;

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
  ifr_adding_visitor (AST_Decl *scope,
                      CORBA::Boolean in_reopened = 0,
                      bool allow_duplicate_typedefs = false);

  virtual ~ifr_adding_visitor (void);

  virtual int visit_scope (UTL_Scope *node);
  virtual int visit_predefined_type (AST_PredefinedType *node);
  virtual int visit_module (AST_Module *node);
  virtual int visit_interface (AST_Interface *node);
  virtual int visit_interface_fwd (AST_InterfaceFwd *node);
  virtual int visit_valuebox (AST_ValueBox *node);
  virtual int visit_valuetype (AST_ValueType *node);
  virtual int visit_valuetype_fwd (AST_ValueTypeFwd *node);
  virtual int visit_component (AST_Component *node);
  virtual int visit_component_fwd (AST_ComponentFwd *node);
  virtual int visit_eventtype (AST_EventType *node);
  virtual int visit_eventtype_fwd (AST_EventTypeFwd *node);
  virtual int visit_home (AST_Home *node);
  virtual int visit_factory (AST_Factory *node);
  virtual int visit_structure (AST_Structure *node);
  virtual int visit_structure_fwd (AST_StructureFwd *node);
  virtual int visit_exception (AST_Exception *node);
  virtual int visit_enum (AST_Enum *node);
  virtual int visit_operation (AST_Operation *node);
  virtual int visit_field (AST_Field *node);
  virtual int visit_attribute (AST_Attribute *node);
  virtual int visit_union (AST_Union *node);
  virtual int visit_union_fwd (AST_UnionFwd *node);
  virtual int visit_constant (AST_Constant *node);
  virtual int visit_array (AST_Array *node);
  virtual int visit_sequence (AST_Sequence *node);
  virtual int visit_string (AST_String *node);
  virtual int visit_typedef (AST_Typedef *node);
  virtual int visit_root (AST_Root *node);
  virtual int visit_native (AST_Native *node);

protected:
  CORBA::PrimitiveKind expr_type_to_pkind (AST_Expression::ExprType et);
  CORBA::PrimitiveKind predefined_type_to_pkind (AST_PredefinedType *node);
  // Conversion functions.

  void load_any (AST_Expression::AST_ExprValue *ev,
                 CORBA::Any &any);
  // Determine the primitive type and insert into the Any.

  void element_type (AST_Type *base_type, bool owned = false);
  // Creates or looks up the element type of an array or sequence,
  // and stores the result in ir_current_.

  int create_interface_def (AST_Interface *node);
  // Code encapsulated out of visit_interface().

  int create_value_def (AST_ValueType *node);
  // Code encapsulated out of visit_valuetype().

  int create_component_def (AST_Component *node);
  // Code encapsulated out of visit_component().

  int create_home_def (AST_Home *node);
  // Code encapsulated out of visit_home().

  int create_event_def (AST_EventType *node);
  // Code encapsulated out of visit_eventtype().

  int create_value_member (AST_Field *node);
  // Conditional call from visit_field().

  void get_referenced_type (AST_Type *node);
  // Utility method to update ir_current_ for struct members, union
  // members, operation parameters and operation return types.

  void fill_base_value (CORBA::ValueDef_ptr &result,
                        AST_ValueType *node);

  void fill_base_component (CORBA::ComponentIR::ComponentDef_ptr &result,
                            AST_Component *node);

  void fill_base_home (CORBA::ComponentIR::HomeDef_ptr &result,
                       AST_Home *node);

  void fill_managed_component (CORBA::ComponentIR::ComponentDef_ptr &result,
                               AST_Home *node);

  void fill_primary_key (CORBA::ValueDef_ptr &result,
                         AST_Home *node);

  void fill_abstract_base_values (CORBA::ValueDefSeq &result,
                                  AST_ValueType *node);

  void fill_inherited_interfaces (CORBA::InterfaceDefSeq &result,
                                  AST_Interface *node);

  void fill_supported_interfaces (CORBA::InterfaceDefSeq &result,
                                  AST_Interface *node);

  void fill_interfaces (CORBA::InterfaceDefSeq &result,
                        AST_Interface **list,
                        CORBA::Long length);

  void fill_initializers (CORBA::ExtInitializerSeq &result,
                          AST_ValueType *node);

  void fill_get_exceptions (CORBA::ExceptionDefSeq &result,
                            AST_Attribute *node);

  void fill_set_exceptions (CORBA::ExceptionDefSeq &result,
                            AST_Attribute *node);

  void fill_exceptions (CORBA::ExceptionDefSeq &result,
                        AST_Decl *node);

  void fill_exceptions (CORBA::ExceptionDefSeq &result,
                        UTL_ExceptList *list);

  void fill_params (CORBA::ParDescriptionSeq &result,
                    AST_Operation *node);

  void visit_all_provides (AST_Component *node,
                           CORBA::ComponentIR::ComponentDef_ptr c);

  void visit_all_uses (AST_Component *node,
                       CORBA::ComponentIR::ComponentDef_ptr c);

  void visit_all_emits (AST_Component *node,
                        CORBA::ComponentIR::ComponentDef_ptr c);

  void visit_all_publishes (AST_Component *node,
                            CORBA::ComponentIR::ComponentDef_ptr c);

  void visit_all_consumes (AST_Component *node,
                           CORBA::ComponentIR::ComponentDef_ptr c);

  void visit_all_factories (AST_Home *node,
                            CORBA::ComponentIR::HomeDef_ptr h);

  void visit_all_finders (AST_Home *node,
                          CORBA::ComponentIR::HomeDef_ptr h);

  void expand_id (ACE_CString &str,
                  const char *local_name);

protected:
  CORBA::IDLType_var ir_current_;
  // Holder for the IR object most recently created or looked up by
  // the visitor. This makes it accessible by visitor methods that
  // need the result of a call to another visitor method that
  // creates an IR object.

  AST_Decl *scope_;
  // Store the node whose scope (if any) we will be visiting.

  bool in_reopened_;
  // Are we traversing the scope of a reopened module?

  bool allow_duplicate_typedefs_;
  // Do we allow duplicate typedefs?
};

#endif /* TAO_IFR_ADDING_VISITOR_H */
