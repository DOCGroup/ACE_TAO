// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_thru_poa_proxy_impl_ss.h
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

#ifndef _BE_VISITOR_OPERATION_THRU_POA_PROXY_IMPL_OPERATION_THRU_POA_PROXY_IMPL_SS_H_
#define _BE_VISITOR_OPERATION_THRU_POA_PROXY_IMPL_OPERATION_THRU_POA_PROXY_IMPL_SS_H_

// ************************************************************
// Operation_Thru_Poa_Proxy_Impl visitor for client stubs
// ************************************************************

class be_visitor_operation_thru_poa_proxy_impl_ss : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operation_thru_poa_proxy_impl_ss
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client invocation
  //   code for the Remote Proxy Implementation.
  //
public:
  be_visitor_operation_thru_poa_proxy_impl_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_thru_poa_proxy_impl_ss (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation_thru_poa_proxy_impl.


  virtual int gen_invoke (be_visitor_context &ctx, be_operation *node);
  // A helper method to generate operation invocation code.

  virtual int gen_check_exception (be_type *bt);
  // ACE_CHECK macros generator.

  virtual int void_return_type (be_type *bt);
  // is my return type void?
};

#endif /* _BE_VISITOR_OPERATION_THRU_POA_PROXY_IMPL_OPERATION_THRU_POA_PROXY_IMPL_SS_H_ */
