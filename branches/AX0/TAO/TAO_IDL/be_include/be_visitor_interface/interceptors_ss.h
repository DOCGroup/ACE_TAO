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
//    interceptors_ss.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This provides code generation for interceptor classes for an
//    interface in the server source.
//
// = AUTHOR
//    Kirthika Parameswaran
//
// ============================================================================

#ifndef _BE_INTERFACE_INTERCEPTORS_SS_H_
#define _BE_INTERFACE_INTERCEPTORS_SS_H_

class be_visitor_interface_interceptors_ss : public be_visitor_interface
{
  // = TITLE
  //   Generate  the "interceptors" class declaration.

public:
  be_visitor_interface_interceptors_ss (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_interface_interceptors_ss (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit an interface

};

#endif /* _BE_INTERFACE_INTERCEPTORS_SS_H_ */
