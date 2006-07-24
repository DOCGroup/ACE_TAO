// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_remote_proxy_impl_cs.h
//
// = DESCRIPTION
//    Visitor for generating code for remote proxy impls
//    in client stubs.
//
//
// = AUTHOR
//    Aniruddha Gokhale & Angelo Corsaro
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_REMOTE_PROXY_IMPL_OPERATION_REMOTE_PROXY_IMPL_CS_H_
#define _BE_VISITOR_OPERATION_REMOTE_PROXY_IMPL_OPERATION_REMOTE_PROXY_IMPL_CS_H_

// ************************************************************
// Operation_Remote_Proxy_Impl visitor for client stubs
// ************************************************************

class be_visitor_operation_remote_proxy_impl_cs : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operation_remote_proxy_impl_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client invocation
  //   code for the Remote Proxy Implementation.
  //
public:
  be_visitor_operation_remote_proxy_impl_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_remote_proxy_impl_cs (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation_remote_proxy_impl.

  virtual int visit_argument (be_argument *node);
  // visit argument to generate ParamData entries

  // = template methods

  // =helper
  virtual int post_process (be_decl *);
  // stuff to output after every member of the scope is handled
};

#endif /* _BE_VISITOR_OPERATION_REMOTE_PROXY_IMPL_OPERATION_REMOTE_PROXY_IMPL_CS_H_ */
