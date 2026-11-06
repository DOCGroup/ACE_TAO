//=============================================================================
/**
 *  @file    direct_proxy_impl_ss.h
 *
 *  Visitor that generates the code for the operation
 *  in the Direct Proxy Implementation.
 *
 *  @author Angelo Corsaro
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_DIRECT_PROXY_IMPL_SS_H_
#define _BE_VISITOR_OPERATION_DIRECT_PROXY_IMPL_SS_H_

/**
 * @class be_visitor_operation_direct_proxy_impl_ss
 *
 * @brief be_visitor_operation_direct_proxy_impl_ss
 *
 * This visitor generates the code for the
 * operation of the Direct Proxy Implementation.
 */
class be_visitor_operation_direct_proxy_impl_ss : public be_visitor_operation
{
public:
  /// constructor
  explicit be_visitor_operation_direct_proxy_impl_ss (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_direct_proxy_impl_ss () override;
  be_visitor_operation_direct_proxy_impl_ss (be_visitor_operation_direct_proxy_impl_ss const &) = delete;
  be_visitor_operation_direct_proxy_impl_ss (be_visitor_operation_direct_proxy_impl_ss &&) = delete;
  be_visitor_operation_direct_proxy_impl_ss &operator= (be_visitor_operation_direct_proxy_impl_ss const &) = delete;
  be_visitor_operation_direct_proxy_impl_ss &operator= (be_visitor_operation_direct_proxy_impl_ss &&) = delete;

  /// visit operation node.
  int visit_operation (be_operation *node) override;

  /// A helper method to generate operation invocation code.
  virtual int gen_invoke (be_visitor_context &ctx, be_operation *node);
};

#endif /* _BE_VISITOR_OPERATION_DIRECT_PROXY_IMPL_SS_H_ */
