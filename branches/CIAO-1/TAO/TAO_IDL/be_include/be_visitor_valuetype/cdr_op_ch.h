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
//    valuetype_cdr_op_ch.h
//
// = DESCRIPTION
//    Concrete visitor for valuetypes.
//    This one provides code generation for the CDR operators.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//
// ============================================================================

#ifndef _BE_VISITOR_VALUETYPE_CDR_OP_CH_H_
#define _BE_VISITOR_VALUETYPE_CDR_OP_CH_H_

class be_visitor_valuetype_cdr_op_ch : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_valuetype_cdr_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for interface that generates the CDR operator
  //   declarations
  //

public:
  be_visitor_valuetype_cdr_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_cdr_op_ch (void);
  // destructor

  virtual int visit_valuetype (be_valuetype *node);
  // visit valuetype
};

#endif /* _BE_VISITOR_VALUETYPE_CDR_OP_CH_H_ */
