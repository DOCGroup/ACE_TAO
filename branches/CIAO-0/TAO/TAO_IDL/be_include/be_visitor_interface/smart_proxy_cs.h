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
//    smart_proxy_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This provides code generation for smart proxy classes in the client stub.
//
// = AUTHOR
//    Kirthika Parameswaran
//
// ============================================================================

#ifndef _BE_INTERFACE_SMART_PROXY_CS_H_
#define _BE_INTERFACE_SMART_PROXY_CS_H_

class be_visitor_interface_smart_proxy_cs : public be_visitor_interface
{
  // = TITLE
  //   Generate the "thru_poa collocated" class declaration.

public:
  be_visitor_interface_smart_proxy_cs (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_interface_smart_proxy_cs (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit interface
};

#endif /* _BE_INTERFACE_SMART_PROXY_CS_H_ */
