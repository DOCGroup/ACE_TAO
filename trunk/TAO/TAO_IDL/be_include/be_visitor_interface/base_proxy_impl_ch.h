//=============================================================================
/**
 *  @file    base_proxy_impl_ch.h
 *
 *  $Id$
 *
 *   Concrete visitor for the Interface node. This
 *   visitor handle the generation of the proxy
 *   implementations base clas.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================


#ifndef BE_BASE_PROXY_IMPL_CH_H_
#define BE_BASE_PROXY_IMPL_CH_H_

/**
 * @class be_visitor_interface_base_proxy_impl_ch
 *
 * @brief Generates the declaration for the base proxy
 * implementation classes.
 */
class be_visitor_interface_base_proxy_impl_ch : public be_visitor_interface
{

public:
  // -- Ctor/Dtor Decl. --
  be_visitor_interface_base_proxy_impl_ch (be_visitor_context *ctx);

  virtual ~be_visitor_interface_base_proxy_impl_ch (void);

  /// visit an interface
  virtual int visit_interface (be_interface *node);

  /// This will just call the above method - no need to create
  /// another set of visitors for this stuff.
  virtual int visit_component (be_component *node);

  /// Helper to generate the declarations for the operations
  /// of any abstract parents we may have.
  static int gen_abstract_ops_helper (be_interface *node,
                                      be_interface *base,
                                      TAO_OutStream *os);
};

#endif /* BE_BASE_PROXY_IMPL_CH_H_ */
