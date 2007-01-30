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
//    exception_ci.h
//
// = DESCRIPTION
//    Visitor for Exceptions.
//    For code generation in client inline.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_EXCEPTION_EXCEPTION_CI_H_
#define _BE_VISITOR_EXCEPTION_EXCEPTION_CI_H_

class be_visitor_exception_ci : public be_visitor_exception
{
  //
  // = TITLE
  //   be_visitor_exception_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client inline for exception
  //
  //
public:
  be_visitor_exception_ci (be_visitor_context *ctx);
  // ctor

  ~be_visitor_exception_ci (void);
  // dtor

  virtual int visit_exception (be_exception *node);
  // visit exception.

};

#endif /* _BE_VISITOR_EXCEPTION_EXCEPTION_CI_H_ */
