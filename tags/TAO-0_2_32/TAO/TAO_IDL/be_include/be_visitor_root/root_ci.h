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
//    root_ci.h
//
// = DESCRIPTION
//    Concrete visitor for the Root class
//    This one provides code generation for elements of the Root node in the
//    client inline.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_CI_H_
#define _BE_VISITOR_ROOT_ROOT_CI_H_

class be_visitor_root_ci : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client inline for root
  //
  //
public:
  be_visitor_root_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_root_ci (void);
  // destructor

  virtual int init (void);
  // set the right context and make a visitor
};

#endif /* _BE_VISITOR_ROOT_ROOT_CI_H_ */
