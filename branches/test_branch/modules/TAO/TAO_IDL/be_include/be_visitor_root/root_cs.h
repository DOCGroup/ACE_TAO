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
//    root_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the Root class
//    This one provides code generation for elements of the Root node in the
//    client stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_CS_H_
#define _BE_VISITOR_ROOT_ROOT_CS_H_

class be_visitor_root_cs : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for root
  //
  //
public:
  be_visitor_root_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_root_cs (void);
  // destructor

  virtual int init (void);
  // set the right context and make a visitor
};

#endif /* _BE_VISITOR_ROOT_ROOT_CS_H_ */
