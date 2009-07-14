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
//    root_svh.h
//
// = DESCRIPTION
//    Concrete visitor for the Root class
//    This one provides code generation for elements of the Root node in the
//    CIAO servant header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_SVH_H_
#define _BE_VISITOR_ROOT_ROOT_SVH_H_

class be_visitor_root_svh : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_svh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the CIAO servant
  //   header for root
  //
  //
public:
  be_visitor_root_svh (be_visitor_context *ctx);

  ~be_visitor_root_svh (void);

  virtual int init (void);
  // Set the right context and make a visitor.
};

#endif /* _BE_VISITOR_ROOT_ROOT_SVH_H_ */
