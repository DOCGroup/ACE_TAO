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
  be_visitor_valuebox_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuebox_ch ();

  /// visit valuebox.
  virtual int visit_valuebox (be_valuebox *node);

  /// visit array.
  virtual int visit_array (be_array *node);

  /// visit enum.
  virtual int visit_enum (be_enum *node);

  /// visit interface.
  virtual int visit_interface (be_interface *node);

  /// visit predefined type.
  virtual int visit_predefined_type (be_predefined_type *node);

  /// visit sequence.
  virtual int visit_sequence (be_sequence *node);

  /// visit string.
  virtual int visit_string (be_string *node);

  /// visit structure.
  virtual int visit_structure (be_structure *node);

  /// visit typedef.
  virtual int visit_typedef (be_typedef *node);

  /// visit union.
  virtual int visit_union (be_union *node);

private:
  int emit_for_predef_enum(be_type *node,
                           const char * type_suffix,
                           bool is_any);

  void emit_default_constructor ();

  void emit_constructor_one_arg (be_decl *node,
                                 const char * type_suffix,
                                 const char * const_prefix,
                                 const char * ref_modifier);

  void emit_copy_constructor ();

  void emit_assignment (be_decl *node,
                        const char * type_suffix,
                        const char * const_prefix,
                        const char * ref_modifier);

  void emit_boxed_access (be_decl *node,
                          const char * type_suffix,
                          const char *const_prefix,
                          const char *in_ref_modifier,
                          const char *out_ref_modifier);

  void emit_accessor_modifier (be_decl *node);

  void emit_boxed_member_var (be_decl *node,
                              const char * type_suffix);
};

#endif /* _BE_VISITOR_VALUEBOX_CH_H_ */
