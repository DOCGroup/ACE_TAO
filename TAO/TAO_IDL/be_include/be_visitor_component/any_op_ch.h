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
//    Concrete visitor for the Component node.
//    This one provides code generation for the Any operators
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_ANY_OP_CH_H_
#define _BE_COMPONENT_ANY_OP_CH_H_

class be_visitor_component_any_op_ch : public be_visitor_component
{
  //
  // = TITLE
  //   be_visitor_component_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for component that generates the Any
  //   operator declarations
  //

public:
  be_visitor_component_any_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_component_any_op_ch (void);
  // destructor

  virtual int visit_component (be_component *node);
  // visit interface
};

#endif /* _BE_COMPONENT_ANY_OP_CH_H_ */
