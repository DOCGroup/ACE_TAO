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
//    root_ss.h
//
// = DESCRIPTION
//    Concrete visitor for the Root class
//    This one provides code generation for elements of the Root node in the
//    server skeletons.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_SS_H_
#define _BE_VISITOR_ROOT_ROOT_SS_H_

class be_visitor_root_ss : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_ss
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server skeletons for root
  //
  //
public:
  be_visitor_root_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_root_ss (void);
  // destructor

  virtual int init (void);
  // set the right context and make a visitor
};

#endif /* _BE_VISITOR_ROOT_ROOT_SS_H_ */
