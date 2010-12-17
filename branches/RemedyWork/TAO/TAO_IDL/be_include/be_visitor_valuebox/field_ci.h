/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    field_ci.h
 *
 *  $Id$
 *
 *   Visitor for the Valuebox class.
 *   This one generates code for accessor and modifier functions of
 *   struct field members.
 *
 *
 *  @author Gary Maxey
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUEBOX_FIELD_CI_H_
#define _BE_VISITOR_VALUEBOX_FIELD_CI_H_

/**
 * @class be_visitor_valuebox_field_ci
 *
 * @brief be_visitor_valuebox_field_ci
 *
 * This visitor is used to generate the accessors and modifiers
 * for valuebox struct members.
 */
class be_visitor_valuebox_field_ci : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_valuebox_field_ci (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuebox_field_ci (void);

  /// visit the field node
  virtual int visit_field (be_field *node);

  // =visit operations on all possible data types that a union_branch can be

  /// visit array type
  virtual int visit_array (be_array *node);

  /// visit enum type
  virtual int visit_enum (be_enum *node);

  /// visit interface type
  virtual int visit_interface (be_interface *node);

  /// visit interface forward type
  virtual int visit_interface_fwd (be_interface_fwd *node);

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

private:
  void emit_member_set (be_decl *field,
                        be_type *field_type,
                        const char *const_arg,
                        const char *arg_modifier);

  void emit_member_get (be_decl *field,
                        be_type *field_type,
                        const char *const_prefix,
                        const char *type_suffix,
                        const char *const_method,
                        const char *access_modifier);

private:
  be_valuebox *vb_node_;
};

#endif /* _BE_VISITOR_VALUEBOX_FIELD_CI_H_ */

