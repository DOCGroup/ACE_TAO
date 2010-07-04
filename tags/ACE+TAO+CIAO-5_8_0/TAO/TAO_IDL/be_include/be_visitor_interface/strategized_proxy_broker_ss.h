//=============================================================================
/**
 *  @file    strategized_proxy_broker_ss.h
 *
 *  $Id$
 *
 *   Concrete visitor for the Interface node. This
 *   visitor generates the declaration for the strategized
 *   ProxyBroker class.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================

#ifndef BE_STRATEGIZED_PROXY_BROKER_SS_H_
#define BE_STRATEGIZED_PROXY_BROKER_SS_H_

/**
 * @class be_visitor_interface_direct_proxy_impl_ss
 *
 * @brief Generates the implementation for the base proxy
 * broker class.
 */
class be_visitor_interface_strategized_proxy_broker_ss
  : public be_visitor_interface
{
public:
  // -- Ctor/Dtor Decl. --
  be_visitor_interface_strategized_proxy_broker_ss (be_visitor_context *ctx);

  virtual ~be_visitor_interface_strategized_proxy_broker_ss (void);

  /// visit an interface
  virtual int visit_interface (be_interface *node);

  /// This will just call the above method - no need to create
  /// another set of visitors for this stuff.
  virtual int visit_component (be_component *node);

  /// This will just call the above method - no need to create
  /// another set of visitors for this stuff.
  virtual int visit_connector (be_connector *node);

private:
  void gen_direct_operations (be_interface *node, TAO_OutStream *os);
};

#endif /* BE_STRATEGIZED_PROXY_BROKER_SS_H_ */
