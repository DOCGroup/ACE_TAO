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
//    Concrete visitor for the valuetype node.
//    This one provides code generation for the Any operators
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef _BE_VALUETYPE_ANY_OP_CH_H_
#define _BE_VALUETYPE_ANY_OP_CH_H_

class be_visitor_valuetype_any_op_ch : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_valuetype_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for valuetypes that generates the Any
  //   operator declarations.
  //

public:
  be_visitor_valuetype_any_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_any_op_ch (void);
  // destructor

  virtual int visit_valuetype (be_valuetype *node);
  // visit interface
};

#endif /* _BE_VALUETYPE_ANY_OP_CH_H_ */
