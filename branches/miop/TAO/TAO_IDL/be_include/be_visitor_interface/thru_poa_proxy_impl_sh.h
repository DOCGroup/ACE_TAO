//$Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    thru_poa_proxy_impl_sh.h
//
// = DESCRIPTION
//     Concrete visitor for the Interface node. This
//     visitor handle the generation of the proxy
//     implementations ThruPOA clas.
//
// = AUTHOR
//     Angelo Corsaro <corsaro@cs.wustl.edu>
//
// ============================================================================

#ifndef BE_THRU_POA_PROXY_IMPL_SH_H_
#define BE_THRU_POA_PROXY_IMPL_SH_H_

class be_visitor_interface_thru_poa_proxy_impl_sh : public be_visitor_interface
{
  // = TITLE
  //   Generates the declaration for the ThruPOA proxy
  //   implementation classes.

public:
  // -- Ctor/Dtor Decl. --
  be_visitor_interface_thru_poa_proxy_impl_sh (be_visitor_context *ctx);

  virtual ~be_visitor_interface_thru_poa_proxy_impl_sh (void);

  virtual int visit_interface (be_interface *node);
  // visit an interface

};

#endif /* BE_THRU_POA_PROXY_IMPL_SH_H_ */
