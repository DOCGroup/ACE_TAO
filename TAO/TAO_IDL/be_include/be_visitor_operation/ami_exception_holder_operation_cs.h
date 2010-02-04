
//=============================================================================
/**
 *  @file    ami_exception_holder_operation_cs.h
 *
 *  $Id$
 *
 *  Visitor for generating code for IDL operations in client stubs
 *
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de<
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_OPERATION_AMI_EXCEPTION_HOLDER_OPERATION_CS_H_
#define _BE_VISITOR_OPERATION_OPERATION_AMI_EXCEPTION_HOLDER_OPERATION_CS_H_


/**
 * @class be_visitor_operation_ami_exception_holder_operation_cs
 *
 * @brief be_visitor_operation_ami_exception_holder_operation_cs
 *
 * This is a concrete visitor to generate the client stubs for operation
 */
class be_visitor_operation_ami_exception_holder_operation_cs : public be_visitor_operation
{
public:
  /// constructor
  be_visitor_operation_ami_exception_holder_operation_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_ami_exception_holder_operation_cs (void);

  /// visit operation.
  virtual int visit_operation (be_operation *node);
};



#endif /* _BE_VISITOR_OPERATION_OPERATION_AMI_EXCEPTION_HOLDER_OPERATION_CS_H_ */
