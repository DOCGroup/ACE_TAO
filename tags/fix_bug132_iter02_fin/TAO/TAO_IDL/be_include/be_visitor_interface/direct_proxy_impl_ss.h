//$Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    direct_proxy_impl_ss.h
//
// = DESCRIPTION
//    Visitor that takes care of generation the implementation
//    for the direct proxy implementation.
//
// = AUTHOR
//    Angelo Corsaro
//
// ============================================================================

#ifndef BE_DIRECT_PROXY_IMPL_SS_H_
#define BE_DIRECT_PROXY_IMPL_SS_H_

class be_visitor_interface_direct_proxy_impl_ss : public be_visitor_interface
{
  // = TITLE
  //   Generates the declaration for the Direct proxy
  //   implementation classes.

public:
  // -- Ctor/Dtor Decl. --
  be_visitor_interface_direct_proxy_impl_ss (be_visitor_context *ctx);

  virtual ~be_visitor_interface_direct_proxy_impl_ss (void);

  virtual int visit_interface (be_interface *node);
  // visit an interface

};

#endif /* BE_DIRECT_PROXY_IMPL_SS_H_ */
