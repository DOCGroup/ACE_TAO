/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    sequence_base.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Sequence class
 *  This one provides code generation for the base typ of the Sequence node.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_SEQUENCE_SEQUENCE_BASE_H_
#define _BE_VISITOR_SEQUENCE_SEQUENCE_BASE_H_

/**
 * @class be_visitor_sequence_base
 *
 * @brief be_visitor_sequence_base
 *
 * This is a concrete visitor to generate the template parameters
 * for a TAO_*_Sequence instantiation.
 * = BUGS
 * The class name is misleading, eventually this class could be
 * used to generate code in other files, not only the client
 * header.
 * The visitor factory should provide a factory method that builds
 * this class on the fly.
 */
class be_visitor_sequence_base : public be_visitor_decl
{
public:
  /// Constructor
  be_visitor_sequence_base (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_sequence_base (void);

  // = Visitor methods.
  virtual int visit_predefined_type (be_predefined_type *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_interface_fwd (be_interface_fwd *node);
  virtual int visit_valuebox (be_valuebox *node);
  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_structure_fwd (be_structure_fwd *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_exception (be_exception *node);
  virtual int visit_union (be_union *node);
  virtual int visit_union_fwd (be_union_fwd *node);
  virtual int visit_array (be_array *node);
  virtual int visit_string (be_string *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_typedef (be_typedef *node);

protected:
  /// helper that does the common job
  int visit_node (be_type *);

};

/**
 * @class be_visitor_sequence_base_template_args:
 =TITLE
 * be_visitor_sequnce_base_template_args
 * =Description
 * This is a derived class just used to override the method
 * visit_interface that generates the arguments for the
 * templates. This is necessary as the same visitor was being used
 * by the idl compiler to generate the template arguments and the
 * return arguments for some of the methods.
 */
class be_visitor_sequence_base_template_args: public be_visitor_sequence_base
{
public:
  be_visitor_sequence_base_template_args (be_visitor_context *ctx,
                                          be_sequence *node );

  ///Dtor
  ~be_visitor_sequence_base_template_args (void);

  //= Visit methods
  virtual int visit_interface (be_interface *node);

  virtual int visit_interface_fwd (be_interface_fwd *node);

  virtual int visit_valuetype (be_valuetype *node);

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);

  virtual int visit_predefined_type (be_predefined_type *node);
private:
  be_sequence *beseq_;
};
#endif /* _BE_VISITOR_SEQUENCE_SEQUENCE_BASE_H_ */
