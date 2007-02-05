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
//    smart_proxy_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This provides code generation for smart proxy classes for an
//    interface in the client header.
//
// = AUTHOR
//    Kirthika Parameswaran
//
// ============================================================================

#ifndef _BE_INTERFACE_SMART_PROXY_CH_H_
#define _BE_INTERFACE_SMART_PROXY_CH_H_

class be_visitor_interface_smart_proxy_ch : public be_visitor_interface
{
  // = TITLE
  //   Generate the "smart proxy" class declaration.

public:
  be_visitor_interface_smart_proxy_ch (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_interface_smart_proxy_ch (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit an interface

  virtual int visit_component (be_component *node);
  // This will just call the above method - no need to create
  // another set of visitors for this stuff.
};

#endif /* _BE_INTERFACE_SMART_PROXY_CH_H_ */
