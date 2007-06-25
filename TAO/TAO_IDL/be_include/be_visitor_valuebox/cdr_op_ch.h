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
//    Concrete visitor for valueboxes.
//    This one provides code generation for the CDR operators.
//
// = AUTHOR
//    Gary Maxey
//
// ============================================================================

#ifndef _BE_VISITOR_VALUEBOX_CDR_OP_CH_H_
#define _BE_VISITOR_VALUEBOX_CDR_OP_CH_H_

class be_visitor_valuebox_cdr_op_ch : public be_visitor_valuebox
{
  //
  // = TITLE
  //   be_visitor_valuebox_cdr_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for valuebox that generates the CDR operator
  //   declarations
  //

public:
  be_visitor_valuebox_cdr_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuebox_cdr_op_ch (void);
  // destructor

  virtual int visit_valuebox (be_valuebox *node);
  // visit valuebox
};

#endif /* _BE_VISITOR_VALUEBOX_CDR_OP_CH_H_ */
