//=============================================================================
/**
 *  @file    remote_proxy_broker_cs.h
 *
 *  $Id$
 *
 *   Concrete visitor for the Interface node. This
 *   visitor generates the declaration for the remote
 *   ProxyBroker class.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================


#ifndef BE_REMOTE_PROXY_BROKER_CS_H_
#define BE_REMOTE_PROXY_BROKER_CS_H_

/**
 * @class be_visitor_interface_remote_proxy_broker_cs
 *
 * @brief Generates the implementation  for the base proxy
 * broker class.
 */
class be_visitor_interface_remote_proxy_broker_cs : public be_visitor_interface
{

public:
  // -- Ctor/Dtor Decl. --
  be_visitor_interface_remote_proxy_broker_cs (be_visitor_context *ctx);

  virtual ~be_visitor_interface_remote_proxy_broker_cs (void);

  /// visit an interface
  virtual int visit_interface (be_interface *node);

  /// This will just call the above method - no need to create
  /// another set of visitors for this stuff.
  virtual int visit_component (be_component *node);
};

#endif /* BE_REMOTE_PROXY_BROKER_CS_H_ */
