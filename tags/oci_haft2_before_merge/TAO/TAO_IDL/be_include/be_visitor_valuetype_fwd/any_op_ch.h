//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the forward declared Valuetype node.
//    This one provides code generation for the Any operators.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VALUETYPE_FWD_ANY_OP_CH_H_
#define _BE_VALUETYPE_FWD_ANY_OP_CH_H_

class be_visitor_valuetype_fwd_any_op_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_valuetype_fwd_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for forward declared valuetypes that
  //   generates the Any operator declaration.
  //

public:
  be_visitor_valuetype_fwd_any_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_fwd_any_op_ch (void);
  // destructor

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  // visit valuetype_fwd
};

#endif /* _BE_VALUETYPE_FWD_ANY_OP_CH_H_ */
