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
//    exception_cs.h
//
// = DESCRIPTION
//    Visitor for Exceptions.
//    For code generation in client stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_EXCEPTION_EXCEPTION_CS_H_
#define _BE_VISITOR_EXCEPTION_EXCEPTION_CS_H_

class be_visitor_exception_cs : public be_visitor_exception
{
  //
  // = TITLE
  //   be_visitor_exception_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for exception
  //
  //
public:
  be_visitor_exception_cs (be_visitor_context *ctx);
  // ctor

  ~be_visitor_exception_cs (void);
  // dtor

  virtual int visit_exception (be_exception *node);
  // visit exception. We provide code for this method in the derived class

};

#endif /* _BE_VISITOR_EXCEPTION_EXCEPTION_CS_H_ */
