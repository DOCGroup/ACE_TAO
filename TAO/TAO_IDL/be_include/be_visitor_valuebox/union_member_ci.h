/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    union_member_ci.h
 *
 *   Visitor for the Valuebox class.
 *   This one generates code for accessor and modifier functions of
 *   union members.
 *
 *  @author Gary Maxey
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUEBOX_UNION_MEMBER_CI_H_
#define _BE_VISITOR_VALUEBOX_UNION_MEMBER_CI_H_

/**
 * @class be_visitor_valuebox_union_member_ci
 *
 * @brief be_visitor_valuebox_union_member_ci
 *
 * This visitor is used to generate the accessors and modifiers
 * for valuebox union members.
 */
class be_visitor_valuebox_union_member_ci : public be_visitor_decl
{
public:
  /// constructor
  explicit be_visitor_valuebox_union_member_ci (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuebox_union_member_ci () override;

  be_visitor_valuebox_union_member_ci (be_visitor_valuebox_union_member_ci const &) = delete;
  be_visitor_valuebox_union_member_ci (be_visitor_valuebox_union_member_ci &&) = delete;
  be_visitor_valuebox_union_member_ci &operator= (be_visitor_valuebox_union_member_ci const &) = delete;
  be_visitor_valuebox_union_member_ci &operator= (be_visitor_valuebox_union_member_ci &&) = delete;

  /// visit the field node
  virtual int visit_union_member (be_union_branch *node);

  // =visit operations on all possible data types that a union_branch can be

  /// visit array type
  int visit_array (be_array *node) override;

  /// visit enum type
  int visit_enum (be_enum *node) override;

  /// visit interface type
  int visit_interface (be_interface *node) override;

  /// visit interface forward type
  int visit_interface_fwd (be_interface_fwd *node) override;

  int visit_valuetype (be_valuetype *node) override;

  int visit_valuetype_fwd (be_valuetype_fwd *node) override;

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

private:
  void emit_member_set (be_decl *field,
                        be_type *field_type,
                        char const *const_arg,
                        char const *arg_modifier);

  void emit_member_get (be_decl *field,
                        be_type *field_type,
                        char const *const_prefix,
                        char const *type_suffix,
                        char const *const_method);

private:
  be_valuebox *vb_node_;
};

#endif /* _BE_VISITOR_VALUEBOX_UNION_MEMBER_CI_H_ */

