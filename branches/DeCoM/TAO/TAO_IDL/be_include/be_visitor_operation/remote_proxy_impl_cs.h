//=============================================================================
/**
 *  @file    remote_proxy_impl_cs.h
 *
 *  $Id$
 *
 *  Visitor for generating code for remote proxy impls
 *  in client stubs.
 *
 *
 *
 *  @author Aniruddha Gokhale & Angelo Corsaro
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_REMOTE_PROXY_IMPL_OPERATION_REMOTE_PROXY_IMPL_CS_H_
#define _BE_VISITOR_OPERATION_REMOTE_PROXY_IMPL_OPERATION_REMOTE_PROXY_IMPL_CS_H_

// ************************************************************
// Operation_Remote_Proxy_Impl visitor for client stubs
// ************************************************************

/**
 * @class be_visitor_operation_remote_proxy_impl_cs
 *
 * @brief be_visitor_operation_remote_proxy_impl_cs
 *
 * This is a concrete visitor to generate the client invocation
 * code for the Remote Proxy Implementation.
 */
class be_visitor_operation_remote_proxy_impl_cs : public be_visitor_operation
{
public:
  /// constructor
  be_visitor_operation_remote_proxy_impl_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_remote_proxy_impl_cs (void);

  /// visit operation_remote_proxy_impl.
  virtual int visit_operation (be_operation *node);

  /// visit argument to generate ParamData entries
  virtual int visit_argument (be_argument *node);

  // = template methods

  // =helper
  /// stuff to output after every member of the scope is handled
  virtual int post_process (be_decl *);
};

#endif /* _BE_VISITOR_OPERATION_REMOTE_PROXY_IMPL_OPERATION_REMOTE_PROXY_IMPL_CS_H_ */
