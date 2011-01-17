/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    field_ch.h
 *
 *  $Id$
 *
 *  Visitor generating code for valuetype fields in the valuetype class
 *  (header).
 *
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
  ~be_visitor_valuetype_field_ch (void);

  /// visit the field node
  virtual int visit_field (be_field *node);

  // =visit operations on all possible data types that a valuetype
  //  state member can be.

  /// visit array type
  virtual int visit_array (be_array *node);

  /// visit enum type
  virtual int visit_enum (be_enum *node);

  /// visit interface type
  virtual int visit_interface (be_interface *node);

  /// visit interface forward type
  virtual int visit_interface_fwd (be_interface_fwd *node);

  /// visit valuebox type
  virtual int visit_valuebox (be_valuebox *node);

  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);

  /// visit predefined type
  virtual int visit_predefined_type (be_predefined_type *node);

  /// visit sequence type
  virtual int visit_sequence (be_sequence *node);

  /// visit string type
  virtual int visit_string (be_string *node);

  /// visit structure type
  virtual int visit_structure (be_structure *node);

  /// visit typedefed type
  virtual int visit_typedef (be_typedef *node);

  /// visit union type
  virtual int visit_union (be_union *node);

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
