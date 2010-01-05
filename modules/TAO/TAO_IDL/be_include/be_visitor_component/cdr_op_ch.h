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
//    Concrete visitor for the Component class
//    This one provides code generation for the CDR operators for the
//    component in the client header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_COMPONENT_CDR_OP_CH_H_
#define _BE_VISITOR_COMPONENT_CDR_OP_CH_H_

class be_visitor_component_cdr_op_ch : public be_visitor_component
{
  //
  // = TITLE
  //   be_visitor_component_cdr_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for component that generates the CDR operator
  //   declarations
  //

public:
  be_visitor_component_cdr_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_component_cdr_op_ch (void);
  // destructor

  virtual int visit_component (be_component *node);
  // visit component
};

#endif /* _BE_VISITOR_COMPONENT_CDR_OP_CH_H_ */
