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
//    root_svs.h
//
// = DESCRIPTION
//    Concrete visitor for the Root class
//    This one provides code generation for elements of the Root node in the
//    CIAO servant source.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_SVS_H_
#define _BE_VISITOR_ROOT_ROOT_SVS_H_

class be_visitor_root_svs : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_svs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the CIAO servant
  //   source for root
  //
  //
public:
  be_visitor_root_svs (be_visitor_context *ctx);

  ~be_visitor_root_svs (void);

  virtual int init (void);
  // Set the right context and make a visitor.
};

#endif /* _BE_VISITOR_ROOT_ROOT_SVS_H_ */
