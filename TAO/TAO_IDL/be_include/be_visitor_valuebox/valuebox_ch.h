/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    valuebox_ch.h
 *
 *  Concrete visitor for the Valuebox node.
 *  This provides code generation for valueboxes in the client header.
 *
 *  @author Gary Maxey
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUEBOX_CH_H_
#define _BE_VISITOR_VALUEBOX_CH_H_

#include <string_view>

/**
 * @class be_visitor_valuebox_ch
 *
 * @brief be_visitor_valuebox_ch
 *
 * This is a concrete visitor to generate the client header for valuebox
 */
class be_visitor_valuebox_ch : public be_visitor_valuebox
{
public:
  /// constructor
  explicit be_visitor_valuebox_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuebox_ch () override  = default;

  be_visitor_valuebox_ch (be_visitor_valuebox_ch const &) = delete;
  be_visitor_valuebox_ch (be_visitor_valuebox_ch &&) = delete;
  be_visitor_valuebox_ch &operator= (be_visitor_valuebox_ch const &) = delete;
  be_visitor_valuebox_ch &operator= (be_visitor_valuebox_ch &&) = delete;

  /// visit valuebox.
  int visit_valuebox (be_valuebox *node) override;

  /// visit array.
  int visit_array (be_array *node) override;

  /// visit enum.
  int visit_enum (be_enum *node) override;

  /// visit interface.
  int visit_interface (be_interface *node) override;

  /// visit predefined type.
  int visit_predefined_type (be_predefined_type *node) override;

  /// visit sequence.
  int visit_sequence (be_sequence *node) override;

  /// visit string.
  int visit_string (be_string *node) override;

  /// visit structure.
  int visit_structure (be_structure *node) override;

  /// visit typedef.
  int visit_typedef (be_typedef *node) override;

  /// visit union.
  int visit_union (be_union *node) override;

private:
  int emit_for_predef_enum(be_type *node,
                           std::string_view type_suffix,
                           bool is_any);

  void emit_default_constructor () const;

  void emit_constructor_one_arg (be_decl *node,
                                 std::string_view type_suffix,
                                 std::string_view const_prefix,
                                 std::string_view ref_modifier);

  void emit_copy_constructor () const;

  void emit_assignment (be_decl *node,
                        std::string_view type_suffix,
                        std::string_view const_prefix,
                        std::string_view ref_modifier);

  void emit_boxed_access (be_decl *node,
                          std::string_view type_suffix,
                          std::string_view const_prefix,
                          std::string_view in_ref_modifier,
                          std::string_view out_ref_modifier);

  void emit_accessor_modifier (be_decl *node) const;

  void emit_boxed_member_var (be_decl *node,
                              std::string_view type_suffix);
};

#endif /* _BE_VISITOR_VALUEBOX_CH_H_ */
