//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ci.h
//
// = DESCRIPTION
//    Visitors for generation of code for Sequences in client stubs. This one
//    generates the CDR operators.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_SEQUENCE_CDR_OP_CI_H_
#define _BE_VISITOR_SEQUENCE_CDR_OP_CI_H_

// ************************************************************
// class be_visitor_sequence_cs
// ************************************************************

class be_visitor_sequence_cdr_op_ci : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_sequence_cdr_op_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor for sequence that generates the CDR operator
  //   implementations
  //

public:
  be_visitor_sequence_cdr_op_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_sequence_cdr_op_ci (void);
  // destructor

  virtual int visit_sequence (be_sequence *node);
  // visit sequence
};

#endif /* _BE_VISITOR_SEQUENCE_CDR_OP_CI_H_ */
