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
//    Concrete visitor for the forward declared Interface node.
//    This one provides code generation for the Any operators.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_INTERFACE_FWD_ANY_OP_CH_H_
#define _BE_INTERFACE_FWD_ANY_OP_CH_H_

class be_visitor_interface_fwd_any_op_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_interface_fwd_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for forward declared interfaces that
  //   generates the Any operator declaration.
  //

public:
  be_visitor_interface_fwd_any_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_fwd_any_op_ch (void);
  // destructor

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface_fwd
};

#endif /* _BE_INTERFACE_FWD_ANY_OP_CH_H_ */
