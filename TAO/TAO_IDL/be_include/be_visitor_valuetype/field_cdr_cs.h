/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    field_cdr_cs.h
 *
 *  Visitor generating code for Field in the client stubs file.
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de> derived from be_visitor_field/cdr_op_cs.h
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUETYPE_FIELD_CDR_CS_H_
#define _BE_VISITOR_VALUETYPE_FIELD_CDR_CS_H_

#include "be_visitor_scope.h"

/**
 * @class be_visitor_valuetype_field_cdr_cs
 *
 * @brief be_visitor_valuetype_field_cdr_cs
 *
 * This is a concrete visitor for the be_field node for the client header.
 */
class be_visitor_valuetype_field_cdr_cs : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_valuetype_field_cdr_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_field_cdr_cs ();

  /// visit the field node
  int visit_field (be_field *node) override;

  // =visit operations on all possible data types that a field can be

  /// visit array type
  int visit_array (be_array *node) override;

  /// visit enum type
  int visit_enum (be_enum *node) override;

  /// visit interface type
  int visit_interface (be_interface *node) override;

  /// visit interface forward type
  int visit_interface_fwd (be_interface_fwd *node) override;

  /// visit valuebox type
  int visit_valuebox (be_valuebox *node) override;

  int visit_valuetype (be_valuetype *node) override;

  int visit_valuetype_fwd (be_valuetype_fwd *node) override;

  /// visit component type
  int visit_component (be_component *node) override;

  /// visit interface forward type
  int visit_component_fwd (be_component_fwd *node) override;

  /// visit eventtype type
  int visit_eventtype (be_eventtype *node) override;

  /// visit eventtype forward type
  int visit_eventtype_fwd (be_eventtype_fwd *node) override;

  /// visit predefined type
  int visit_predefined_type (be_predefined_type *node) override;

  /// visit sequence type
  int visit_sequence (be_sequence *node) override;

  /// visit string type
  int visit_string (be_string *node) override;

  /// visit structure type
  int visit_structure (be_structure *node) override;

  /// visit typedefed type
  int visit_typedef (be_typedef *node) override;

  /// visit union type
  int visit_union (be_union *node) override;

  const char *pre_, *post_;

private:
  int valuetype_common (be_type *);
};

/**
 * @class be_visitor_valuetype_field_cdr_decl
 *
 * @brief be_visitor_cdr_valuetype_field_cdr_decl
 *
 * When generating CDR operators for fields of structures and/or
 * unions it may be necessary to define some local variables (for
 * instance to hold temporary T_forany objects); this visitor
 * generates that code.
 */
class be_visitor_valuetype_field_cdr_decl : public be_visitor_scope
{
public:
  /// constructor
  be_visitor_valuetype_field_cdr_decl (be_visitor_context *ctx);

  /// visit the field node
  int visit_field (be_field *node) override;

  // = visit operations for data types that require a local variable.

  /// visit array type
  int visit_array (be_array *node) override;

  /// visit typedef type
  int visit_typedef (be_typedef *node) override;
};

#endif /*  _BE_VISITOR_VALUETYPE_FIELD_CDR_CS_H_ */
