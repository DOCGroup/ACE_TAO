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
//    Concrete visitor for the Interface node.
//    This one provides code generation for the Any operators
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_INTERFACE_ANY_OP_CH_H_
#define _BE_INTERFACE_ANY_OP_CH_H_

class be_visitor_interface_any_op_ch : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for interface that generates the Any operator
  //   declarations
  //

public:
  be_visitor_interface_any_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_any_op_ch (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit interface
};

#endif /* _BE_INTERFACE_ANY_OP_CH_H_ */
