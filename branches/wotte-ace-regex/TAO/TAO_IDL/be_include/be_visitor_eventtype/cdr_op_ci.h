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
//    cdr_op_ci.h
//
// = DESCRIPTION
//    Concrete visitor for eventtypes.
//    This one provides code generation for the CDR operators.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_EVENTTYPE_CDR_OP_CI_H_
#define _BE_VISITOR_EVENTTYPE_CDR_OP_CI_H_

class be_visitor_eventtype_cdr_op_ci : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_eventtype_cdr_op_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor for eventtype that generates the CDR operator
  //   declarations
  //

public:
  be_visitor_eventtype_cdr_op_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_eventtype_cdr_op_ci (void);
  // destructor

  virtual int visit_eventtype (be_eventtype *node);
  // visit valuetype
};

#endif /* _BE_VISITOR_EVENTTYPE_CDR_OP_CI_H_ */
