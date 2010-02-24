/* -*- c++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    root_exh.h
//
// = DESCRIPTION
//    Concrete visitor for the Root class
//    This one provides code generation for elements of the Root node in the
//    CIAO exec impl header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_EXH_H_
#define _BE_VISITOR_ROOT_ROOT_EXH_H_

class be_visitor_root_exh : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_exh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the CIAO servant
  //   header for root
  //
  //
public:
  be_visitor_root_exh (be_visitor_context *ctx);

  ~be_visitor_root_exh (void);

  virtual int init (void);
  // Set the right context and make a visitor.
};

#endif /* _BE_VISITOR_ROOT_ROOT_EXH_H_ */
