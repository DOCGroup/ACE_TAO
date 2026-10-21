/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    valuebox_ci.h
 *
 *  Concrete visitor for the Valuebox node.
 *  This provides code generation for valueboxes in the client inline.
 *
 *  @author Gary Maxey
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUEBOX_CI_H_
#define _BE_VISITOR_VALUEBOX_CI_H_

/**
 * @class be_visitor_valuebox_ci
 *
 * @brief be_visitor_valuebox_ci
 *
 * This is a concrete visitor to generate the client inline for valuebox
 */
class be_visitor_valuebox_ci : public be_visitor_valuebox
{
public:
  /// constructor
  be_visitor_valuebox_ci (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuebox_ci () override;

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
  int emit_for_predef_enum (be_type *node,
                            const char * type_suffix,
                            bool is_any);

  void emit_default_constructor ();

  void emit_default_constructor_alloc (be_decl *node);

  void emit_constructor_one_arg (be_decl *node,
                                const char * type_suffix);

  void emit_constructor_one_arg_alloc (be_decl *node);

  void emit_copy_constructor ();

  void emit_copy_constructor_alloc (be_decl *node);

  void emit_assignment (be_decl *node,
                        const char * type_suffix);

  void emit_assignment_alloc (be_decl *node);

  void emit_accessor_modifier (be_decl *node);

  void emit_boxed_access (be_decl *node, const char *out_ref_modifier);
};

#endif /* _BE_VISITOR_VALUEBOX_CI_H_ */
