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
//    root_sh.h
//
// = DESCRIPTION
//    Concrete visitor for the Root class
//    This one provides code generation for elements of the Root node in the
//    server header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_SH_H_
#define _BE_VISITOR_ROOT_ROOT_SH_H_

class be_visitor_root_sh : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_sh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for root
  //
  //
public:
  be_visitor_root_sh (be_visitor_context *ctx);
  // constructor

  ~be_visitor_root_sh (void);
  // destructor

  virtual int init (void);
  // set the right context and make a visitor
};

#endif /* _BE_VISITOR_ROOT_ROOT_SH_H_ */
