// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    IDL_TO_WSDL_BE_DLL
//
// = FILENAME
//    type_visitor.h
//
// = DESCRIPTION
//    Generates elements in <types></types> in the WSDL document.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef TYPE_VISITOR_H
#define TYPE_VISITOR_H

#include "idl_to_wsdl_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ast_expression.h"

class UTL_IdList;

class type_visitor : public idl_to_wsdl_visitor
{
  //
  // = TITLE
  //    type_visitor.
  //
  // = DESCRIPTION
  //    Generates elements in <types></types> in the WSDL document.
  //
public:
  type_visitor (DOMElement *sub_tree);
  virtual ~type_visitor (void);

  virtual int visit_interface (AST_Interface *node);
  virtual int visit_valuetype (AST_ValueType *node);
  virtual int visit_eventtype (AST_EventType *node);
  virtual int visit_structure (AST_Structure *node);
  virtual int visit_exception (AST_Exception *node);
  virtual int visit_enum (AST_Enum *node);
  virtual int visit_field (AST_Field *node);
  virtual int visit_union (AST_Union *node);
  virtual int visit_union_branch (AST_UnionBranch *node);
  virtual int visit_union_label (AST_UnionLabel *node);
  virtual int visit_enum_val (AST_EnumVal *node);
  virtual int visit_array (AST_Array *node);
  virtual int visit_sequence (AST_Sequence *node);
  virtual int visit_typedef (AST_Typedef *node);

private:
  void gen_seq_array_common (DOMElement *elem,
                             AST_Type *base_type,
                             AST_Expression *size,
                             bool is_array,
                             const char *insert);
  void gen_inherited_vt_members (AST_ValueType *node,
                                 type_visitor &visitor);
  ACE_CString print_scoped_name (UTL_IdList *sn);
  ACE_CString expr_val_to_string (AST_Expression::AST_ExprValue *ev);

private:
  ACE_CString label_list_;
  AST_Union *current_union_;
  AST_Typedef *alias_;
};

#endif /* TYPE_VISITOR_H */
