// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_smart_proxy_cs.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations in client stubs
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_OPERATION_SMART_PROXY_CS_H_
#define _BE_VISITOR_OPERATION_OPERATION_SMART_PROXY_CS_H_

// ************************************************************
// Operation visitor for client stubs
// ************************************************************

class be_visitor_operation_smart_proxy_cs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_smart_proxy_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for operation
  //
  //
public:
  be_visitor_operation_smart_proxy_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_smart_proxy_cs (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation.

  virtual int gen_invoke (be_visitor_context &ctx, be_operation *node);
  // A helper method to generate operation invocation code.

  virtual int void_return_type (be_type *bt);
  // is my return type void?
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_SMART_PROXY_CS_H_ */

