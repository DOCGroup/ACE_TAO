//$Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    remote_proxy_broker_cs.h
//
// = DESCRIPTION
//     Concrete visitor for the Interface node. This
//     visitor generates the declaration for the remote
//     ProxyBroker class.
//
// = AUTHOR
//     Angelo Corsaro <corsaro@cs.wustl.edu>
//
// ============================================================================

#ifndef BE_REMOTE_PROXY_BROKER_CS_H_
#define BE_REMOTE_PROXY_BROKER_CS_H_

class be_visitor_interface_remote_proxy_broker_cs : public be_visitor_interface
{
  // = TITLE
  //   Generates the implementation  for the base proxy
  //   broker class.

public:
  // -- Ctor/Dtor Decl. --
  be_visitor_interface_remote_proxy_broker_cs (be_visitor_context *ctx);

  virtual ~be_visitor_interface_remote_proxy_broker_cs (void);

  virtual int visit_interface (be_interface *node);
  // visit an interface

};

#endif /* BE_REMOTE_PROXY_BROKER_CS_H_ */
