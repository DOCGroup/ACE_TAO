/* -*- C++ -*- */
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
//    Visitor for Exceptions.
//    For code generation of Any operator in client stubs
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_EXCEPTION_ANY_OP_CS_H_
#define _BE_VISITOR_EXCEPTION_ANY_OP_CS_H_

class be_visitor_exception_any_op_cs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_exception_any_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for exception that generates the Any operator
  //   implementations
  //

public:
  be_visitor_exception_any_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_exception_any_op_cs (void);
  // destructor

  virtual int visit_exception (be_exception *node);
  // visit exception

  virtual int visit_field (be_field *node);
  // visit field
};

#endif /* _BE_VISITOR_EXCEPTION_ANY_OP_CS_H_ */
