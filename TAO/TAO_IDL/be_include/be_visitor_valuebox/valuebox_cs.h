/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    valuebox_cs.h
 *
 *  Concrete visitor for the Valuebox node.
 *  This provides code generation for valueboxes in the client stubs.
 *
 *  @author Gary Maxey
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUEBOX_CS_H_
#define _BE_VISITOR_VALUEBOX_CS_H_

/**
 * @class be_visitor_valuebox_cs
 *
 * @brief be_visitor_valuebox_cs
 *
 * This is a concrete visitor to generate the client stubs for valuebox
 */
class be_visitor_valuebox_cs : public be_visitor_valuebox
{
public:
  /// constructor
  be_visitor_valuebox_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuebox_cs () override;

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

  /// Generate virtual destructor.
  void emit_destructor ();

  int emit_for_predef_enum (char const * marshal_arg);

};

#endif /* _BE_VISITOR_VALUEBOX_CS_H_ */
