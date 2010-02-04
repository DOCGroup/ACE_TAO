
//=============================================================================
/**
 *  @file    ami_ch.h
 *
 *  $Id$
 *
 *  Visitor for generating AMI stub code for IDL operations in
 *  client header.
 *
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_AMI_CH_H_
#define _BE_VISITOR_OPERATION_AMI_CH_H_

// ************************************************************
// Operation visitor to generate AMI stubs for client header
// ************************************************************

/**
 * @class be_visitor_operation_ami_ch
 *
 * @brief be_visitor_operation_ami_ch.
 *
 * This is a concrete visitor to generate the AMI stubs in the
 * client header for operation.
 */
class be_visitor_operation_ami_ch : public be_visitor_operation
{
public:
  /// constructor
  be_visitor_operation_ami_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_ami_ch (void);

  /// visit operation.
  virtual int visit_operation (be_operation *node);
};

#endif /* _BE_VISITOR_OPERATION_AMI_CH_H_ */
