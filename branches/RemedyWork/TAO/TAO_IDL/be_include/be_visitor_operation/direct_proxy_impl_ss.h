//=============================================================================
/**
 *  @file    direct_proxy_impl_ss.h
 *
 *  $Id$
 *
 *  Visitor that generates the code for the operation
 *  in the Direct Proxy Implementation.
 *
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
  be_visitor_operation_direct_proxy_impl_ss (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_direct_proxy_impl_ss (void);

  /// visit operation node.
  virtual int visit_operation (be_operation *node);

  /// A helper method to generate operation invocation code.
  virtual int gen_invoke (be_visitor_context &ctx, be_operation *node);
};

#endif /* _BE_VISITOR_OPERATION_DIRECT_PROXY_IMPL_SS_H_ */
