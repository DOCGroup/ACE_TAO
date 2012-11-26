
//=============================================================================
/**
 *  @file    ami_handler_reply_stub_operation_cs.h
 *
 *  $Id$
 *
 *  Visitor for generating code skelton code for IDL operations
 *  of AMI Reply Handlers.
 *
 *
 *  @author Michael Kircher
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_OPERATION_AMI_HANDLER_REPLY_STUB_OPERATION_CS_H_
#define _BE_VISITOR_OPERATION_OPERATION_AMI_HANDLER_REPLY_STUB_OPERATION_CS_H_

// ************************************************************
// Operation visitor for client stubs
// ************************************************************

/**
 * @class be_visitor_operation_ami_handler_reply_stub_operation_cs
 *
 * @brief be_visitor_operation_ami_handler_operation_cs
 *
 * Visitor for generating code skelton code for IDL operations
 * of AMI Reply Handlers.
 */
class be_visitor_operation_ami_handler_reply_stub_operation_cs : public be_visitor_operation
{
public:
  /// constructor
  be_visitor_operation_ami_handler_reply_stub_operation_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_ami_handler_reply_stub_operation_cs (void);

  /// visit operation.
  virtual int visit_operation (be_operation *node);

  /// visit argument to generate ParamData entries
  virtual int visit_argument (be_argument *node);

  // = template methods
  /// generate any info before the actual code for the stub is generated
  int gen_pre_stub_info (be_operation *node,
                         be_type *bt);

  /// generate code that marshals the arguments and transmits them
  virtual int gen_marshal_and_invoke (be_operation *node,
                                      be_type *bt);

  // =helper
  /// stuff to output after every member of the scope is handled
  virtual int post_process (be_decl *);
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_AMI_HANDLER_REPLY_STUB_OPERATION_CS_H_ */
