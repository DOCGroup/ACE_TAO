// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IDL3_TO_IDL2_BE_DLL
//
// = FILENAME
//    basic_visitor.h
//
// = DESCRIPTION
//    Base class for other visitors in this backend.
//
// ============================================================================

#ifndef TAO_BASIC_VISITOR_H
#define TAO_BASIC_VISITOR_H

#include "ast_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "TAO_IDL3_TO_IDL2_BE_Export.h"

#include "ace/SString.h"

class TAO_OutStream;
class UTL_ExceptList;

class TAO_IDL3_TO_IDL2_BE_Export basic_visitor : public ast_visitor
{
  //
  // = TITLE
  //    basic_visitor.
  //
  // = DESCRIPTION
  //    Base class for visitors in this backend and others.
  //
public:
  basic_visitor (void);
  virtual ~basic_visitor (void);

  virtual int visit_decl (AST_Decl *d);
  virtual int visit_scope (UTL_Scope *node);
  virtual int visit_type (AST_Type *node);
  virtual int visit_predefined_type (AST_PredefinedType *node);
  virtual int visit_module (AST_Module *node) = 0;
  virtual int visit_interface (AST_Interface *node) = 0;
  virtual int visit_interface_fwd (AST_InterfaceFwd *node);
  virtual int visit_valuebox (AST_ValueBox *node);
  virtual int visit_valuetype (AST_ValueType *node);
  virtual int visit_valuetype_fwd (AST_ValueTypeFwd *node);
  virtual int visit_component (AST_Component *node) = 0;
  virtual int visit_component_fwd (AST_ComponentFwd *node) = 0;
  virtual int visit_eventtype (AST_EventType *node) = 0;
  virtual int visit_eventtype_fwd (AST_EventTypeFwd *node) = 0;
  virtual int visit_home (AST_Home *node) = 0;
  virtual int visit_factory (AST_Factory *node);
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
  virtual int visit_root (AST_Root *node) = 0;
  virtual int visit_native (AST_Native *node);
  
protected:
  void check_prefix (AST_Decl *d);
  void check_id_and_version (AST_Decl *d);
  const char *type_name (AST_Type *t);
  void gen_anonymous_array (AST_Type *array, AST_Decl *wrapper);
  void gen_params (UTL_Scope *s, int arg_count);
  void gen_exception_list (UTL_ExceptList *exceptions,
                           const char *prefix = "",
                           bool closed = true);
  void gen_operation (AST_Operation *node);
  void gen_attribute (AST_Attribute *node);
  void gen_label_value (AST_UnionLabel *node);
  
  // Overrides allow common code for visit_scope().
  virtual bool scope_skip_type (AST_Decl *d);
  
  // Used by derived visitors to avoid generating an empty IDL module.
  bool can_skip_module (AST_Module *m);
  
  // Used by derived visitors to check for special include handling.
  bool match_excluded_file (const char *raw_filename);

protected:
  TAO_OutStream *os;
  AST_Type *disc_type_;
  
private:
  ACE_CString tmp_retval_;
};

#endif // TAO_BASIC_VISITOR_H
