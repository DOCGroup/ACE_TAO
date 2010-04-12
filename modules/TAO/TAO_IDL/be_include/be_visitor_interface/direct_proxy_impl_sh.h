//=============================================================================
/**
 *  @file    direct_proxy_impl_sh.h
 *
 *  $Id$
 *
 *   Concrete visitor for the Interface node. This
 *   visitor handle the generation of the proxy
 *   implementations Direct clas.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================


#ifndef BE_DIRECT_PROXY_IMPL_SH_H_
#define BE_DIRECT_PROXY_IMPL_SH_H_

/**
 * @class be_visitor_interface_direct_proxy_impl_sh
 *
 * @brief Generates the declaration for the Direct proxy
 * implementation classes.
 */
class be_visitor_interface_direct_proxy_impl_sh : public be_visitor_interface
{

public:
  // -- Ctor/Dtor Decl. --
  be_visitor_interface_direct_proxy_impl_sh (be_visitor_context *ctx);

  virtual ~be_visitor_interface_direct_proxy_impl_sh (void);

  /// visit an interface
  virtual int visit_interface (be_interface *node);

  /// This will just call the above method - no need to create
  /// another set of visitors for this stuff.
  virtual int visit_component (be_component *node);

  /// This will just call the above method - no need to create
  /// another set of visitors for this stuff.
  virtual int visit_connector (be_connector *node);

  /// Helper to generate the declarations for the operations
  /// of any abstract parents we may have.
  static int gen_abstract_ops_helper (be_interface *node,
                                      be_interface *base,
                                      TAO_OutStream *os);
};

#endif /* BE_DIRECT_PROXY_IMPL_SH_H_ */
