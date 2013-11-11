/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    cdr_op_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the base "BE_Field" node
 *  This generates code for structure members in the client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_FIELD_CDR_OP_CS_H_
#define _BE_VISITOR_FIELD_CDR_OP_CS_H_

/**
 * @class be_visitor_field_cdr_op_cs
 *
 * @brief be_visitor_field_cdr_op_cs
 *
 * This is a concrete visitor for the be_field node for the client header.
 */
class be_visitor_field_cdr_op_cs : public be_visitor_decl
{
public:
  be_visitor_field_cdr_op_cs (be_visitor_context *ctx);
  ~be_visitor_field_cdr_op_cs (void);

  /// visit the field node
  virtual int visit_field (be_field *node);

  // =visit operations on all possible data types that a field can be

  virtual int visit_array (be_array *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_interface_fwd (be_interface_fwd *node);
  virtual int visit_component (be_component *node);
  virtual int visit_component_fwd (be_component_fwd *node);
  virtual int visit_valuebox (be_valuebox *node);
  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  virtual int visit_eventtype (be_eventtype *node);
  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);
  virtual int visit_predefined_type (be_predefined_type *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_string (be_string *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_structure_fwd (be_structure_fwd *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_union (be_union *node);
  virtual int visit_union_fwd (be_union_fwd *node);

 private:
  int emit_valuetype_common (void);
};

/**
 * @class be_visitor_cdr_op_field_decl
 *
 * @brief be_visitor_cdr_op_ci_field_decl
 *
 * When generating CDR operators for fields of structures and/or
 * unions it may be necessary to define some local variables (for
 * instance to hold temporary T_forany objects); this visitor
 * generates that code.
 */
class be_visitor_cdr_op_field_decl : public be_visitor_scope
{
public:
  /// constructor
  be_visitor_cdr_op_field_decl (be_visitor_context *ctx);

  /// visit the field node
  virtual int visit_field (be_field *node);

  // = visit operations for data types that require a local variable.

  /// visit array type
  virtual int visit_array (be_array *node);

  /// visit typedef type
  virtual int visit_typedef (be_typedef *node);
};

#endif /*  _BE_VISITOR_FIELD_CDR_OP_CS_H_ */
