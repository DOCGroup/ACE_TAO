/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    typedef_ch.h
 *
 *  Concrete visitor for the Typedef class
 *  This provides the code generation for the Typedef class in the client
 *  header.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_TYPEDEF_TYPEDEF_CH_H_
#define _BE_VISITOR_TYPEDEF_TYPEDEF_CH_H_

/**
 * @class be_visitor_typedef_ch
 *
 * @brief be_visitor_typedef_ch
 *
 * This is a concrete visitor to generate the client header for typedef
 */
class be_visitor_typedef_ch : public be_visitor_typedef
{
public:
  /// constructor
  be_visitor_typedef_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_typedef_ch ();

  /// visit typedef. We provide code for this method in the derived class
  int visit_typedef (be_typedef *node) override;

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  /// visit a array
  int visit_array (be_array *node) override;

  /// visit an enum
  int visit_enum (be_enum *node) override;

  /// visit an interface;
  int visit_interface (be_interface *node) override;

  /// visit an interface;
  int visit_interface_fwd (be_interface_fwd *node) override;

  /// visit predefined type node
  int visit_predefined_type (be_predefined_type *node) override;

  /// visit a typedef
  int visit_string (be_string *node) override;

  /// visit a sequence
  int visit_sequence (be_sequence *node) override;

  /// visit a structure
  int visit_structure (be_structure *node) override;

  /// visit a union
  int visit_union (be_union *node) override;

  /// visit a valuebox
  int visit_valuebox (be_valuebox *node) override;

  /// visit a valuetype
  int visit_valuetype (be_valuetype *node) override;
};

#endif /* _BE_VISITOR_TYPEDEF_TYPEDEF_CH_H_ */
