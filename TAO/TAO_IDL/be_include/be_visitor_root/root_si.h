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
//    root_si.h
//
// = DESCRIPTION
//    Concrete visitor for the Root class
//    This one provides code generation for elements of the Root node in the
//    server inline.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_SI_H_
#define _BE_VISITOR_ROOT_ROOT_SI_H_

class be_visitor_root_si : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_si
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server inline for root
  //
  //
public:
  be_visitor_root_si (be_visitor_context *ctx);
  // constructor

  ~be_visitor_root_si (void);
  // destructor

  virtual int init (void);
  // set the right context and make a visitor
};

#endif /* _BE_VISITOR_ROOT_ROOT_SI_H_ */
