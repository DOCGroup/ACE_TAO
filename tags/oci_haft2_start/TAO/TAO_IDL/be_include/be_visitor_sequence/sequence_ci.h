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
//    sequence_ci.h
//
// = DESCRIPTION
//    Concrete visitor for the Sequence class
//    This one provides code generation for the Sequence node in the client
//    inline.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_SEQUENCE_SEQUENCE_CI_H_
#define _BE_VISITOR_SEQUENCE_SEQUENCE_CI_H_

class be_visitor_sequence_ci : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_sequence_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client inline for
  //   sequences. The only purpose is to generate the implementation for the
  //   _var and _out classes
  //
public:
  be_visitor_sequence_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_sequence_ci (void);
  // destructor

  virtual int visit_sequence (be_sequence *node);
  // visit sequence node
};

#endif /* _BE_VISITOR_SEQUENCE_SEQUENCE_CI_H_ */
