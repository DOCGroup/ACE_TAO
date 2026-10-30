
//=============================================================================
/**
 *  @file    smart_proxy_ch.h
 *
 *  Visitor for generating code for IDL operations for the smart proxy case
 *  inside of the client stubs header
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
  explicit be_visitor_operation_smart_proxy_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_smart_proxy_ch () override;
  be_visitor_operation_smart_proxy_ch (be_visitor_operation_smart_proxy_ch const &) = delete;
  be_visitor_operation_smart_proxy_ch (be_visitor_operation_smart_proxy_ch &&) = delete;
  be_visitor_operation_smart_proxy_ch &operator= (be_visitor_operation_smart_proxy_ch const &) = delete;
  be_visitor_operation_smart_proxy_ch &operator= (be_visitor_operation_smart_proxy_ch &&) = delete;

  /// visit operation node.
  int visit_operation (be_operation *node) override;
};

#endif /* _BE_VISITOR_OPERATION_SMART_PROXY_CH_H_ */
