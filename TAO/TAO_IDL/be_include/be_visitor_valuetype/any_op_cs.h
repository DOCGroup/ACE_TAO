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
//    Concrete visitor for the valuetype node.
//    This one provides code generation for the Any operators
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef _BE_VALUETYPE_ANY_OP_CS_H_
#define _BE_VALUETYPE_ANY_OP_CS_H_

class be_visitor_valuetype_any_op_cs : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_valuetype_any_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for valuetype that generates the Any
  //   operator implementations
  //

public:
  be_visitor_valuetype_any_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_any_op_cs (void);
  // destructor

  virtual int visit_valuetype (be_valuetype *node);
  // visit interface
};

#endif /* _BE_VALUETYPE_ANY_OP_CS_H_ */
