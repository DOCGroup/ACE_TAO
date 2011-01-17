//=============================================================================
/**
 *  @file    proxy_impl_xh.h
 *
 *  $Id$
 *
 *  Visitor for generating code for proxy implementations.
 *
 *
 *  @author Angelo Corsaro
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_PROXY_IMPL_XH_H_
#define _BE_VISITOR_OPERATION_PROXY_IMPL_XH_H_


/**
 * @class be_visitor_operation_proxy_impl_xh
 *
 * @brief be_visitor_operation_proxy_impl_xh
 *
 * Concrete visitor that generates the code for the base class
 * of Proxy Impls.
 */
class be_visitor_operation_proxy_impl_xh : public be_visitor_operation
{
public:
  /// constructor
  be_visitor_operation_proxy_impl_xh (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_proxy_impl_xh (void);

  /// visit operation.
  virtual int visit_operation (be_operation *node);
};

#endif /* _BE_VISITOR_OPERATION_PROXY_IMPL_XH_H_ */
