//$Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    remote_proxy_impl_cs.h
//
// = DESCRIPTION
//     Concrete visitor for the Interface node. This
//     visitor handle the generation of the proxy
//     implementations remote clas.
//
// = AUTHOR
//     Angelo Corsaro <corsaro@cs.wustl.edu>
//
// ============================================================================

#ifndef BE_REMOTE_PROXY_IMPL_CS_H_
#define BE_REMOTE_PROXY_IMPL_CS_H_

class be_visitor_interface_remote_proxy_impl_cs : public be_visitor_interface
{
  // = TITLE
  //   Generates the implementation for the remote proxy
  //   implementation classes.

public:
  // -- Ctor/Dtor Decl. --
  be_visitor_interface_remote_proxy_impl_cs (be_visitor_context *ctx);

  virtual ~be_visitor_interface_remote_proxy_impl_cs (void);

  virtual int visit_interface (be_interface *node);
  // visit an interface

  static int gen_abstract_ops_helper (be_interface *node,
                                      be_interface *base,
                                      TAO_OutStream *os);
  // Helper to generate the declarations for the operations
  // of any abstract parents we may have.
};

#endif /* BE_REMOTE_PROXY_IMPL_CS_H_ */
