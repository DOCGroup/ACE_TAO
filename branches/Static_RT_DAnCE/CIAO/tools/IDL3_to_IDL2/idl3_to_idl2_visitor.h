// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_PICML_BE_DLL
//
// = FILENAME
//    idl3_to_idl2_visitor.h
//
// = DESCRIPTION
//    The IDML importer visitor base class.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef TAO_PICML_VISITOR_H
#define TAO_PICML_VISITOR_H

#include "ast_visitor.h"
#include "utl_scoped_name.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_OutStream;
class UTL_ExceptList;

class idl3_to_idl2_visitor : public ast_visitor
{
  //
  // = TITLE
  //    idl3_to_idl2_visitor.
  //
  // = DESCRIPTION
  //    IDL3 to IDL2 conversion visitor. The methods are
  //    not pure virtual to facilitate the implementation of some
  //    derived visitors that override only a few.
  //
public:
  idl3_to_idl2_visitor (void);
  virtual ~idl3_to_idl2_visitor (void);

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
  virtual int visit_eventtype (AST_EventType *node);
  virtual int visit_eventtype_fwd (AST_EventTypeFwd *node);
  virtual int visit_home (AST_Home *node);
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
  virtual int visit_root (AST_Root *node);
  virtual int visit_native (AST_Native *node);

private:
  void check_prefix (AST_Decl *d);
  void check_id_and_version (AST_Decl *d);
  const char *type_name (AST_Type *t);
  void gen_anonymous_array (AST_Type *array, AST_Decl *wrapper);
  void gen_label_value (AST_UnionLabel *node);
  void gen_provides (AST_Component *node);
  void gen_uses (AST_Component *node);
  void gen_publishes (AST_Component *node);
  void gen_emits (AST_Component *node);
  void gen_consumes (AST_Component *node);
  UTL_ScopedName *create_scoped_name (const char *prefix,
                                      const char *local_name,
                                      const char *suffix,
                                      AST_Decl *parent);
  void tranfer_scope_elements (AST_Home *src, AST_Interface &dst);
  void gen_factories (AST_Home *node, AST_Interface &xplicit);
  void gen_finders (AST_Home *node, AST_Interface &xplicit);
  void gen_params (UTL_Scope *s, int arg_count);
  void gen_exception_list (UTL_ExceptList *exceptions,
                           const char *prefix = "",
                           bool closed = true);

private:
  TAO_OutStream *os;
  AST_Type *disc_type_;
};

#endif /* TAO_PICML_VISITOR_H */
