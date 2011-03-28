
/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    ifr_adding_visitor.h
 *
 *  $Id$
 *
 *  Header file for class ifr_adding_visitor.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IFR_ADDING_VISITOR_H
#define TAO_IFR_ADDING_VISITOR_H

#include "ifr_visitor.h"
#include "tao/IFR_Client/IFR_ComponentsC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class UTL_ExceptList;

/**
 * @class ifr_adding_visitor
 *
 * @brief ifr_adding_visitor.
 *
 * This visitor adds items found in the IDL file being processed
 * to the Interface Repository.
 */
class ifr_adding_visitor : public ifr_visitor
{
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
  virtual int visit_provides (AST_Provides *node);
  virtual int visit_uses (AST_Uses *node);
  virtual int visit_publishes (AST_Publishes *node);
  virtual int visit_emits (AST_Emits *node);
  virtual int visit_consumes (AST_Consumes *node);
  virtual int visit_eventtype (AST_EventType *node);
  virtual int visit_eventtype_fwd (AST_EventTypeFwd *node);
  virtual int visit_home (AST_Home *node);
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
  /// Conversion functions.
  CORBA::PrimitiveKind expr_type_to_pkind (AST_Expression::ExprType et);
  CORBA::PrimitiveKind predefined_type_to_pkind (AST_PredefinedType *node);

  /// Determine the primitive type and insert into the Any.
  void load_any (AST_Expression::AST_ExprValue *ev,
                 CORBA::Any &any);

  /// Creates or looks up the element type of an array or sequence,
  /// and stores the result in ir_current_.
  void element_type (AST_Type *base_type, bool owned = false);

  /// Code encapsulated out of visit_interface().
  int create_interface_def (AST_Interface *node);

  /// Code encapsulated out of visit_valuetype().
  int create_value_def (AST_ValueType *node);

  /// Code encapsulated out of visit_component().
  int create_component_def (AST_Component *node);

  /// Code encapsulated out of visit_home().
  int create_home_def (AST_Home *node);

  /// Code encapsulated out of visit_eventtype().
  int create_event_def (AST_EventType *node);

  /// Conditional call from visit_field().
  int create_value_member (AST_Field *node);

  /// Utility method to update ir_current_ for struct members, union
  /// members, operation parameters and operation return types.
  void get_referenced_type (AST_Type *node);

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
                        AST_Type **list,
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
                    UTL_Scope *node);

  void visit_all_factories (AST_Home *node,
                            CORBA::ComponentIR::HomeDef_ptr h);

  void visit_all_finders (AST_Home *node,
                          CORBA::ComponentIR::HomeDef_ptr h);

  void expand_id (ACE_CString &str,
                  const char *local_name);

protected:
  /**
   * Holder for the IR object most recently created or looked up by
   * the visitor. This makes it accessible by visitor methods that
   * need the result of a call to another visitor method that
   * creates an IR object.
   */
  CORBA::IDLType_var ir_current_;

  /// Store the node whose scope (if any) we will be visiting.
  AST_Decl *scope_;

  /// Are we traversing the scope of a reopened module?
  bool in_reopened_;

  /// Do we allow duplicate typedefs?
  bool allow_duplicate_typedefs_;
};

#endif /* TAO_IFR_ADDING_VISITOR_H */
