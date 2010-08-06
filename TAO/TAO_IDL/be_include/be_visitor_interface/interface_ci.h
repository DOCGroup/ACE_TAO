//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_ci.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This one provides code generation for interfaces in the client inline.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_INTERFACE_INTERFACE_CI_H_
#define _BE_INTERFACE_INTERFACE_CI_H_

class be_visitor_interface_ci : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client inline for interface
  //
  //
public:
  be_visitor_interface_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_ci (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor
};

#endif /* _BE_INTERFACE_INTERFACE_CI_H_ */
