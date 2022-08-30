/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    field_ch.h
 *
 *  Visitor generating code for valuetype fields in the valuetype class
 *  (header).
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de> derived from be_visitor_union_branch/public_ch.h
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUETYPE_FIELD_CH_H_
#define _BE_VISITOR_VALUETYPE_FIELD_CH_H_

/**
 * @class be_visitor_valuetype_field_ch
 *
 * @brief be_visitor_field_ch
 *
 * Visitor generating code for valuetype fields in the valuetype class
 * (header).
 */
class be_visitor_valuetype_field_ch : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_valuetype_field_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_field_ch ();

  /// visit the field node
  int visit_field (be_field *node) override;

  // =visit operations on all possible data types that a valuetype
  //  state member can be.

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
  int visit_eventtype (be_eventtype *node) override;

  int visit_valuetype_fwd (be_valuetype_fwd *node) override;
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

  /// print additional material around the field (e.g. virtual before)
  void setenclosings (const char *pre, const char *post);
  const char *pre_op ();
  const char *post_op ();

private:
  const char *pre_op_;
  const char *post_op_;
  AST_Field::Visibility visibility_;
};

#endif /* _BE_VISITOR_VALUETYPE_FIELD_CH_H_ */
