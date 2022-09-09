/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    field_ch.h
 *
 *   Visitor for the structure fields in valuebox class.
 *   This one generates code for access to structure members
 *   in the client header.
 *
 *   Based on be_visitor_union_branch/public_ch.  In general we
 *   generate the same method signatures as that visitor but cannot
 *   use it directly because a valuebox is not a scope and because
 *   the structure is visited prior to visiting the valuebox.
 *
 *  @author Gary Maxey
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUEBOX_FIELD_CH_H_
#define _BE_VISITOR_VALUEBOX_FIELD_CH_H_

/**
 * @class be_visitor_valuebox_field_ch
 *
 * @brief be_visitor_valuebox_field_ch
 *
 * inside a valuebox for the client header. This generates the code
 * for the public section of the "valuebox" class
 */
class be_visitor_valuebox_field_ch : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_valuebox_field_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuebox_field_ch () override;

  /// visit the union_branch node
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

  /// visit valuetype type
  int visit_valuetype (be_valuetype *node) override;

  /// visit valuetype forward type
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
                        const char *const_arg,
                        const char *arg_modifier);

  void emit_member_get (be_decl *field,
                        be_type *field_type,
                        const char *const_prefix,
                        const char *type_suffix,
                        const char *const_method);
};

#endif /* _BE_VISITOR_VALUEBOX_FIELD_CH_H_ */
