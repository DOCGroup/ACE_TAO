/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    typedef_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Typedef class
 *  This provides the code generation for the Typedef class in the client
 *  header.
 *
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
  ~be_visitor_typedef_ch (void);

  /// visit typedef. We provide code for this method in the derived class
  virtual int visit_typedef (be_typedef *node);

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  /// visit a array
  virtual int visit_array (be_array *node);

  /// visit an enum
  virtual int visit_enum (be_enum *node);

  /// visit an interface;
  virtual int visit_interface (be_interface *node);

  /// visit an interface;
  virtual int visit_interface_fwd (be_interface_fwd *node);

  /// visit predefined type node
  virtual int visit_predefined_type (be_predefined_type *node);

  /// visit a typedef
  virtual int visit_string (be_string *node);

  /// visit a sequence
  virtual int visit_sequence (be_sequence *node);

  /// visit a structure
  virtual int visit_structure (be_structure *node);

  /// visit a union
  virtual int visit_union (be_union *node);

  /// visit a valuebox
  virtual int visit_valuebox (be_valuebox *node);

  /// visit a valuetype
  virtual int visit_valuetype (be_valuetype *node);
};

#endif /* _BE_VISITOR_TYPEDEF_TYPEDEF_CH_H_ */
