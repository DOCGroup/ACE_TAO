/* -*- C++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_ch.h
//
// = DESCRIPTION
//    Visitor for Exceptions.
//    For code generation of Any operator in client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_EXCEPTION_ANY_OP_CH_H_
#define _BE_VISITOR_EXCEPTION_ANY_OP_CH_H_

class be_visitor_exception_any_op_ch : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_exception_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for exception that generates the Any operator
  //   declarations
  //

public:
  be_visitor_exception_any_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_exception_any_op_ch (void);
  // destructor

  virtual int visit_exception (be_exception *node);
  // visit exception

  virtual int visit_field (be_field *node);
  // visit field
};

#endif /* _BE_VISITOR_EXCEPTION_ANY_OP_CH_H_ */
