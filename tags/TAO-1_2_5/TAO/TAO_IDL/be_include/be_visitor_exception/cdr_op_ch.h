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
//    cdr_op_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Exception class
//    This one provides code generation for the CDR operators for the exception
//    in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_EXCEPTION_CDR_OP_CH_H_
#define _BE_VISITOR_EXCEPTION_CDR_OP_CH_H_

class be_visitor_exception_cdr_op_ch : public be_visitor_exception
{
  //
  // = TITLE
  //   be_visitor_exception_cdr_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for exception that generates the CDR operator
  //   declarations
  //

public:
  be_visitor_exception_cdr_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_exception_cdr_op_ch (void);
  // destructor

  virtual int visit_exception (be_exception *node);
  // visit exception

};

#endif /* _BE_VISITOR_EXCEPTION_CDR_OP_CH_H_ */
