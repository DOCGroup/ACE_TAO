// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    base_proxy_impl_ch.h
//
// = DESCRIPTION
//    Visitor for generating code for proxy implementations.
//
// = AUTHOR
//    Angelo Corsaro
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_BASE_PROXY_IMPL_CH_H_
#define _BE_VISITOR_OPERATION_BASE_PROXY_IMPL_CH_H_


class be_visitor_operation_base_proxy_impl_ch : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_base_proxy_impl_ch
  //
  // = DESCRIPTION
  //   Concrete visitor that generates the code for the base class
  //   of Proxy Impls.
  //
  //
public:
  be_visitor_operation_base_proxy_impl_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_base_proxy_impl_ch (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation.
};

#endif /* _BE_VISITOR_OPERATION_BASE_PROXY_IMPL_CH_H_ */
