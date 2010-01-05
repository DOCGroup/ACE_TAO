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
//    any_op_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    This one provides code generation for the Any operators
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_ANY_OP_CS_H_
#define _BE_COMPONENT_ANY_OP_CS_H_

class be_visitor_component_any_op_cs : public be_visitor_component
{
  //
  // = TITLE
  //   be_visitor_interface_any_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for interface that generates the Any operator
  //   implementations
  //

public:
  be_visitor_component_any_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_component_any_op_cs (void);
  // destructor

  virtual int visit_component (be_component *node);
  // visit interface
};

#endif /* _BE_COMPONENT_ANY_OP_CS_H_ */
