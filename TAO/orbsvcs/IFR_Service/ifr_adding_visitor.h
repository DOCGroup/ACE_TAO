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
#include "tao/IFR_CLient/IFR_ComponentsC.h"

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
                      CORBA::Boolean in_reopened = 0);
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

  virtual int visit_valuetype (AST_ValueType *node);
  // Visit valuetype.

  virtual int visit_valuetype_fwd (AST_ValueTypeFwd *node);
  // Visit valuetype_fwd.

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
  CORBA::PrimitiveKind expr_type_to_pkind (AST_Expression::ExprType et);
  CORBA::PrimitiveKind predefined_type_to_pkind (AST_PredefinedType *node);
  // Conversion functions.

  void load_any (AST_Expression::AST_ExprValue *ev,
                 CORBA::Any &any);
  // Determine the primitive type and insert into the Any.

  void element_type (AST_Type *base_type
                     ACE_ENV_ARG_DECL);
  // Creates or looks up the element type of an array or sequence,
  // and stores the result in ir_current_.

  int create_interface_def (AST_Interface *node
                            ACE_ENV_ARG_DECL);
  // Code encapsulated out of visit_interface().

  int create_value_def (AST_ValueType *node
                        ACE_ENV_ARG_DECL);
  // Code encapsulated out of visit_valuetype().

  int create_component_def (AST_Component *node
                            ACE_ENV_ARG_DECL);
  // Code encapsulated out of visit_component().

  int create_home_def (AST_Home *node
                       ACE_ENV_ARG_DECL);
  // Code encapsulated out of visit_home().

  int create_event_def (AST_EventType *node
                        ACE_ENV_ARG_DECL);
  // Code encapsulated out of visit_eventtype().

  void get_referenced_type (AST_Type *node
                            ACE_ENV_ARG_DECL);
  // Utility method to update ir_current_ for struct members, union
  // members, operation parameters and operation return types.

  void fill_base_value (CORBA::ValueDef_ptr &result,
                        AST_ValueType *node
                        ACE_ENV_ARG_DECL);

  void fill_base_component (CORBA::ComponentIR::ComponentDef_ptr &result,
                            AST_Component *node
                            ACE_ENV_ARG_DECL);

  void fill_base_home (CORBA::ComponentIR::HomeDef_ptr &result,
                       AST_Home *node
                       ACE_ENV_ARG_DECL);

  void fill_managed_component (CORBA::ComponentIR::ComponentDef_ptr &result,
                               AST_Home *node
                               ACE_ENV_ARG_DECL);

  void fill_primary_key (CORBA::ValueDef_ptr &result,
                         AST_Home *node
                         ACE_ENV_ARG_DECL);

  void fill_abstract_base_values (CORBA::ValueDefSeq &result,
                                  AST_ValueType *node
                                  ACE_ENV_ARG_DECL);

  void fill_inherited_interfaces (CORBA::InterfaceDefSeq &result,
                                  AST_Interface *node
                                  ACE_ENV_ARG_DECL);

  void fill_supported_interfaces (CORBA::InterfaceDefSeq &result,
                                  AST_Interface *node
                                  ACE_ENV_ARG_DECL);

  void fill_interfaces (CORBA::InterfaceDefSeq &result,
                        AST_Interface **list,
                        CORBA::Long length
                        ACE_ENV_ARG_DECL);

  void fill_initializers (CORBA::ExtInitializerSeq &result,
                          AST_ValueType *node
                          ACE_ENV_ARG_DECL);

  void fill_get_exceptions (CORBA::ExceptionDefSeq &result,
                            AST_Attribute *node
                            ACE_ENV_ARG_DECL);

  void fill_set_exceptions (CORBA::ExceptionDefSeq &result,
                            AST_Attribute *node
                            ACE_ENV_ARG_DECL);

  void fill_exceptions (CORBA::ExceptionDefSeq &result,
                        AST_Decl *node
                        ACE_ENV_ARG_DECL);

  void fill_exceptions (CORBA::ExceptionDefSeq &result,
                        UTL_ExceptList *list
                        ACE_ENV_ARG_DECL);

  void fill_params (CORBA::ParDescriptionSeq &result,
                    AST_Operation *node
                    ACE_ENV_ARG_DECL);

  void visit_all_provides (AST_Component *node,
                           CORBA::ComponentIR::ComponentDef_ptr c
                           ACE_ENV_ARG_DECL);

  void visit_all_uses (AST_Component *node,
                       CORBA::ComponentIR::ComponentDef_ptr c
                       ACE_ENV_ARG_DECL);

  void visit_all_emits (AST_Component *node,
                        CORBA::ComponentIR::ComponentDef_ptr c
                        ACE_ENV_ARG_DECL);

  void visit_all_publishes (AST_Component *node,
                            CORBA::ComponentIR::ComponentDef_ptr c
                            ACE_ENV_ARG_DECL);

  void visit_all_consumes (AST_Component *node,
                           CORBA::ComponentIR::ComponentDef_ptr c
                           ACE_ENV_ARG_DECL);

  void visit_all_factories (AST_Home *node,
                            CORBA::ComponentIR::HomeDef_ptr h
                            ACE_ENV_ARG_DECL);

  void visit_all_finders (AST_Home *node,
                          CORBA::ComponentIR::HomeDef_ptr h
                          ACE_ENV_ARG_DECL);

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

  CORBA::Boolean in_reopened_;
  // Are we traversing the scope of a reopened module?
};

#endif /* TAO_IFR_ADDING_VISITOR_H */
