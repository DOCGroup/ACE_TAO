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
//    Visitor that generates the code for the operation
//    in the Direct Proxy Implementation.
//
// = AUTHOR
//    Angelo Corsaro
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_DIRECT_PROXY_IMPL_SS_H_
#define _BE_VISITOR_OPERATION_DIRECT_PROXY_IMPL_SS_H_

class be_visitor_operation_direct_proxy_impl_ss : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operation_direct_proxy_impl_ss
  //
  // = DESCRIPTION
  //   This visitor generates the code for the
  //   operation of the Direct Proxy Implementation.
  //
  //
public:
  be_visitor_operation_direct_proxy_impl_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_direct_proxy_impl_ss (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation node.

  virtual int gen_invoke (be_visitor_context &ctx, be_operation *node);
  // A helper method to generate operation invocation code.
};

#endif /* _BE_VISITOR_OPERATION_DIRECT_PROXY_IMPL_SS_H_ */
