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
//    interceptors_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This provides code generation for interceptor classes for an
//    interface in the client header.
//
// = AUTHOR
//    Kirthika Parameswaran
//
// ============================================================================

#ifndef _BE_INTERFACE_INTERCEPTORS_CS_H_
#define _BE_INTERFACE_INTERCEPTORS_CS_H_

class be_visitor_interface_interceptors_cs : public be_visitor_interface
{
  // = TITLE
  //   Generate  the "interceptors" class declaration.

public:
  be_visitor_interface_interceptors_cs (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_interface_interceptors_cs (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit an interface

  virtual int visit_component (be_component *node);
  // visit a component

  static int gen_abstract_ops_helper (be_interface *node,
                                      be_interface *base,
                                      TAO_OutStream *os);
  // Helper to generate the declarations for the operations
  // of any abstract parents we may have.
};

#endif /* _BE_INTERFACE_INTERCEPTORS_CS_H_ */
