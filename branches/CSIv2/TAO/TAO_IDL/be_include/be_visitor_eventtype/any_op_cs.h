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
//    any_op_cs.h
//
// = DESCRIPTION
//    Concrete visitor for eventtypes.
//    This one provides code generation for the any operators.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_EVENTTYPE_ANY_OP_CS_H_
#define _BE_VISITOR_EVENTTYPE_ANY_OP_CS_H_

class be_visitor_eventtype_any_op_cs : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_eventtype_any_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for eventtype that generates the Any operator
  //   declarations
  //

public:
  be_visitor_eventtype_any_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_eventtype_any_op_cs (void);
  // destructor

  virtual int visit_eventtype (be_eventtype *node);
  // visit valuetype
};

#endif /* _BE_VISITOR_EVENTTYPE_ANY_OP_CS_H_ */
