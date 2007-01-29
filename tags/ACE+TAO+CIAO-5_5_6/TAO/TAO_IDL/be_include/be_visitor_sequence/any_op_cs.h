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
//    any_op_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the Sequence class
//    This one provides code generation for the Sequence node in the client
//    header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_SEQUENCE_ANY_OP_CS_H_
#define _BE_VISITOR_SEQUENCE_ANY_OP_CS_H_

class be_visitor_sequence_any_op_cs : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_sequence_any_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for sequence that generates the Any operator
  //   implementations
  //

public:
  be_visitor_sequence_any_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_sequence_any_op_cs (void);
  // destructor

  virtual int visit_sequence (be_sequence *node);
  // visit sequence
};

#endif /* _BE_VISITOR_SEQUENCE_ANY_OP_CS_H_ */
