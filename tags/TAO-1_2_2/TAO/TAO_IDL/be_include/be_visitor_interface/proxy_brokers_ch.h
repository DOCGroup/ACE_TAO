//$Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    proxy_brokers_ch.h
//
// = DESCRIPTION
//     Concrete visitor for the Interface node. This
//     visitor handle the generation of the proxy brokers
//     for the given interface.
//
// = AUTHOR
//     Angelo Corsaro <corsaro@cs.wustl.edu>
//
// ============================================================================

#ifndef BE_PROXY_BROKERS_CH_H_
#define BE_PROXY_BROKERS_CH_H_

class be_visitor_interface_proxy_brokers_ch : public be_visitor_interface
{
  // = TITLE
  //   Generates the declaration for the base proxy
  //   broker class.

public:
  // -- Ctor/Dtor Decl. --
  be_visitor_interface_proxy_brokers_ch (be_visitor_context *ctx);

  virtual ~be_visitor_interface_proxy_brokers_ch (void);

  virtual int visit_interface (be_interface *node);
  // visit an interface

};

#endif /* BE_PROXY_BROKERS_CH_H_ */
