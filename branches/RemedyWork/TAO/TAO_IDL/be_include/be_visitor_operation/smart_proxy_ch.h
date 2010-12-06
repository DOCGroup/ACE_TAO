
//=============================================================================
/**
 *  @file    smart_proxy_ch.h
 *
 *  $Id$
 *
 *  Visitor for generating code for IDL operations for the smart proxy case
 *  inside of the client stubs header
 *
 *
 *  @author Kirthika Parameswaran  <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_SMART_PROXY_CH_H_
#define _BE_VISITOR_OPERATION_SMART_PROXY_CH_H_

// ************************************************************
// Operation visitor for client header in the smart proxy class
// ************************************************************

/**
 * @class be_visitor_operation_smart_proxy_ch
 *
 * @brief be_visitor_operation_smart_proxy_ch
 *
 * This is a concrete visitor to generate the client header for operation
 * in the smart proxy class
 */
class be_visitor_operation_smart_proxy_ch : public be_visitor_scope
{
public:
  /// constructor
  be_visitor_operation_smart_proxy_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_smart_proxy_ch (void);

  /// visit operation node.
  virtual int visit_operation (be_operation *node);
};

#endif /* _BE_VISITOR_OPERATION_SMART_PROXY_CH_H_ */
