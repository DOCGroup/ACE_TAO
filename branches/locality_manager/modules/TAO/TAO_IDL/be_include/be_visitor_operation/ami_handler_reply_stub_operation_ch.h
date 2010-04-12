
//=============================================================================
/**
 *  @file    ami_handler_reply_stub_operation_ch.h
 *
 *  $Id$
 *
 *  Visitor for generating code for AMI Handler call back operations
 *  in client header.
 *
 *
 *  @author Aniruddha Gokhale and Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_ami_handler_reply_stub_operation_ch_H_
#define _BE_VISITOR_OPERATION_ami_handler_reply_stub_operation_ch_H_

// ************************************************************
// Operation visitor for server header
// ************************************************************

/**
 * @class be_visitor_operation_ami_handler_reply_stub_operation_ch
 *
 * @brief be_visitor_ami_handler_reply_stub_operation_ch
 *
 * This is a concrete visitor to generate the server header for operation
 */
class be_visitor_operation_ami_handler_reply_stub_operation_ch : public be_visitor_operation
{
public:
  /// constructor
  be_visitor_operation_ami_handler_reply_stub_operation_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_ami_handler_reply_stub_operation_ch (void);

  /// visit operation.
  virtual int visit_operation (be_operation *node);
};

#endif /* _BE_VISITOR_OPERATION_ami_handler_reply_stub_operation_ch_H_ */
