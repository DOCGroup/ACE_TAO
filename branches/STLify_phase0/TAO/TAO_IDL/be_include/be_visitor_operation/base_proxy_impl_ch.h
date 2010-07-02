//=============================================================================
/**
 *  @file    base_proxy_impl_ch.h
 *
 *  $Id$
 *
 *  Visitor for generating code for proxy implementations.
 *
 *
 *  @author Angelo Corsaro
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_BASE_PROXY_IMPL_CH_H_
#define _BE_VISITOR_OPERATION_BASE_PROXY_IMPL_CH_H_


/**
 * @class be_visitor_operation_base_proxy_impl_ch
 *
 * @brief be_visitor_operation_base_proxy_impl_ch
 *
 * Concrete visitor that generates the code for the base class
 * of Proxy Impls.
 */
class be_visitor_operation_base_proxy_impl_ch : public be_visitor_scope
{
public:
  /// constructor
  be_visitor_operation_base_proxy_impl_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_base_proxy_impl_ch (void);

  /// visit operation.
  virtual int visit_operation (be_operation *node);
};

#endif /* _BE_VISITOR_OPERATION_BASE_PROXY_IMPL_CH_H_ */
