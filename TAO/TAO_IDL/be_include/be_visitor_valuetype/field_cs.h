/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    field_cs.h
 *
 *   Visitor for the Valuetype class.
 *   This one generates code for accessor and modifier functions of
 *   valuetype state members (in the stub file).
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de> derived from be_visitor_union_branch/public_ci.h
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUETYPE_FIELD_CS_H_
#define _BE_VISITOR_VALUETYPE_FIELD_CS_H_

/**
 * @class be_visitor_valuetype_field_cs
 *
 * @brief be_visitor_valuetype_field_cs
 *
 * This visitor is used to generate the accessors and modifiers
 * for the members of valuetype
 */
class be_visitor_valuetype_field_cs : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_valuetype_field_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_field_cs ();

  /// visit the field node
  int visit_field (be_field *node) override;

  // =visit operations on all possible data types that a union_branch can be

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

  /// print additional string before op (e.g. ACE_INLINE)
  void setenclosings (const char *pre);
  const char *pre_op ();

  /// if (in_obv_space_) print OBV_name else print name
  void op_name (be_valuetype *node, TAO_OutStream *os);

  bool in_obv_space_;
private:
  const char *pre_op_;

  int valuetype_common (be_type *node);
};

#endif /* _BE_VISITOR_VALUETYPE_FIELD_CS_H_ */

