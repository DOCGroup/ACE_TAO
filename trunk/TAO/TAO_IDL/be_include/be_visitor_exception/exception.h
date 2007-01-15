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
//    exception.h
//
// = DESCRIPTION
//    Visitor for Exceptions.
//    This is the generic (common) visitor for all exceptions.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_EXCEPTION_EXCEPTION_H_
#define _BE_VISITOR_EXCEPTION_EXCEPTION_H_

class be_visitor_exception : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_exception
  //
  // = DESCRIPTION
  //   This is the base visitor for exception
  //
  //
public:
  be_visitor_exception (be_visitor_context *ctx);
  // ctor

  ~be_visitor_exception (void);
  // dtor

  virtual int visit_exception (be_exception *node);
  // visit exception. We provide code for this method in the derived class

  // =visit operations on syntactically valid elements in our scope

  virtual int visit_field (be_field *node);
  // visit field
};

#endif /* _BE_VISITOR_EXCEPTION_EXCEPTION_H_ */
