//=============================================================================
/**
 *  @file    smart_proxy_cs.h
 *
 *  $Id$
 *
 *  Visitor for generating code for IDL operations in client stubs
 *
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_OPERATION_SMART_PROXY_CS_H_
#define _BE_VISITOR_OPERATION_OPERATION_SMART_PROXY_CS_H_

// ************************************************************
// Operation visitor for client stubs
// ************************************************************

/**
 * @class be_visitor_operation_smart_proxy_cs
 *
 * @brief be_visitor_operation_smart_proxy_cs
 *
 * This is a concrete visitor to generate the client stubs for operation
 */
class be_visitor_operation_smart_proxy_cs : public be_visitor_operation
{
public:
  /// constructor
  be_visitor_operation_smart_proxy_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_smart_proxy_cs (void);

  /// visit operation.
  virtual int visit_operation (be_operation *node);

  /// A helper method to generate operation invocation code.
  virtual int gen_invoke (be_visitor_context &ctx, be_operation *node);
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_SMART_PROXY_CS_H_ */

