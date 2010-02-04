//=============================================================================
/**
 *  @file    proxy_impls_ch.h
 *
 *  $Id$
 *
 *   Concrete visitor for the Interface node. This
 *   visitor handle the generation of the proxy
 *   implementations  class.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================


#ifndef BE_PROXY_BROKER_IMPLS_CH_H_
#define BE_PROXY_BROKER_IMPLS_CH_H_

/**
 * @class be_visitor_interface_proxy_impls_ch
 *
 * @brief Generates the declaration for the base proxy
 * implementation classes.
 */
class be_visitor_interface_proxy_impls_ch : public be_visitor_interface
{

public:
  // -- Ctor/Dtor Decl. --
  be_visitor_interface_proxy_impls_ch (be_visitor_context *ctx);

  virtual ~be_visitor_interface_proxy_impls_ch (void);

  /// visit an interface
  virtual int visit_interface (be_interface *node);

  /// This will just call the above method - no need to create
  /// another set of visitors for this stuff.
  virtual int visit_component (be_component *node);
};

#endif /* BE_PROXY_BROKER_IMPLS_CH_H_ */
