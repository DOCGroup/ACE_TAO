/* -*- c++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    sequence_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the Sequence class
//    This one provides code generation for the Sequence node in the client
//    stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_SEQUENCE_SEQUENCE_CS_H_
#define _BE_VISITOR_SEQUENCE_SEQUENCE_CS_H_

class be_visitor_sequence_cs : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_sequence_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for
  //   sequences
  //
public:
  be_visitor_sequence_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_sequence_cs (void);
  // destructor

  virtual int visit_sequence (be_sequence *node);
  // visit sequence node

  virtual int gen_unbounded_sequence (be_sequence *node);
  // generate a instance of the unbounded sequence

  virtual int gen_bounded_sequence (be_sequence *node);
  // generate a instance of the bounded sequence

  virtual int gen_unbounded_obj_sequence (be_sequence *node);
  // generate a instance of the unbounded object sequence

  virtual int gen_bounded_obj_sequence (be_sequence *node);
  // generate a instance of the bounded object sequence

  virtual int gen_bounded_str_sequence (be_sequence *node);
  // generate a instance of the bounded string sequence

  virtual int instantiate_sequence (be_sequence *node);
  // determine which sequence to instantiate

  virtual int gen_base_sequence_class (be_sequence *node);
  // generate the base sequence class
};

#endif /* _BE_VISITOR_SEQUENCE_SEQUENCE_CS_H_ */
