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
//    Concrete visitor for eventtypes.
//    This one provides code generation for the CDR operators.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_EVENTTYPE_CDR_OP_CH_H_
#define _BE_VISITOR_EVENTTYPE_CDR_OP_CH_H_

class be_visitor_eventtype_cdr_op_ch : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_eventtype_cdr_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for eventtype that generates the CDR operator
  //   declarations
  //

public:
  be_visitor_eventtype_cdr_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_eventtype_cdr_op_ch (void);
  // destructor

  virtual int visit_eventtype (be_eventtype *node);
  // visit valuetype
};

#endif /* _BE_VISITOR_EVENTTYPE_CDR_OP_CH_H_ */
