// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    proxy_impl_xh.h
//
// = DESCRIPTION
//    Visitor for generating code for proxy implementations.
//
// = AUTHOR
//    Angelo Corsaro
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_PROXY_IMPL_XH_H_
#define _BE_VISITOR_OPERATION_PROXY_IMPL_XH_H_


class be_visitor_operation_proxy_impl_xh : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_proxy_impl_xh
  //
  // = DESCRIPTION
  //   Concrete visitor that generates the code for the base class
  //   of Proxy Impls.
  //
  //
public:
  be_visitor_operation_proxy_impl_xh (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_proxy_impl_xh (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation.
};

#endif /* _BE_VISITOR_OPERATION_PROXY_IMPL_XH_H_ */
