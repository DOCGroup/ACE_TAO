/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    buffer_type.h
 *
 *  Concrete visitor for the Sequence class
 *  This one provides code generation for the buffer type of the Sequence
 *  node.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_SEQUENCE_BUFFER_TYPE_H_
#define _BE_VISITOR_SEQUENCE_BUFFER_TYPE_H_

/**
 * @class be_visitor_sequence_buffer_type
 *
 * @brief be_visitor_sequence_buffer_type
 *
 * This is a concrete visitor to generate the buffer type
 * for a TAO_*_Sequence instantiation.
 */
class be_visitor_sequence_buffer_type : public be_visitor_decl
{
public:
  /// Constructor
  be_visitor_sequence_buffer_type (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_sequence_buffer_type ();

  // = Visitor methods.
  int visit_predefined_type (be_predefined_type *node) override;
  int visit_interface (be_interface *node) override;
  int visit_interface_fwd (be_interface_fwd *node) override;
  int visit_component (be_component *node) override;
  int visit_component_fwd (be_component_fwd *node) override;
  int visit_valuebox (be_valuebox *node) override;
  int visit_valuetype (be_valuetype *node) override;
  int visit_valuetype_fwd (be_valuetype_fwd *node) override;
  int visit_eventtype (be_eventtype *node) override;
  int visit_eventtype_fwd (be_eventtype_fwd *node) override;
  int visit_structure (be_structure *node) override;
  int visit_structure_fwd (be_structure_fwd *node) override;
  int visit_enum (be_enum *node) override;
  int visit_exception (be_exception *node) override;
  int visit_union (be_union *node) override;
  int visit_union_fwd (be_union_fwd *node) override;
  int visit_array (be_array *node) override;
  int visit_string (be_string *node) override;
  int visit_sequence (be_sequence *node) override;
  int visit_typedef (be_typedef *node) override;

protected:
  /// helper that does the common job
  int visit_node (be_type *);
};

#endif /* _BE_VISITOR_SEQUENCE_BUFFER_TYPE_H_ */
