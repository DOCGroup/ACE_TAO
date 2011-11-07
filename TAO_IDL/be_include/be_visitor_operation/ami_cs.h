
//=============================================================================
/**
 *  @file    ami_cs.h
 *
 *  $Id$
 *
 *  Visitor for generating code for IDL operations in client stubs
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_AMI_CS_H_
#define _BE_VISITOR_OPERATION_AMI_CS_H_

// ************************************************************
// Operation visitor for client stubs
// ************************************************************

/**
 * @class be_visitor_operation_ami_cs
 *
 * @brief be_visitor_operation_ami_cs
 *
 * This is a concrete visitor to generate the client stubs for operation
 */
class be_visitor_operation_ami_cs : public be_visitor_operation
{
public:
  /// constructor
  be_visitor_operation_ami_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_ami_cs (void);

  /// visit operation.
  virtual int visit_operation (be_operation *node);

  /// visit argument to generate ParamData entries
  virtual int visit_argument (be_argument *node);

  // = template methods
  /// generate any info before the actual code for the stub is generated
  int gen_pre_stub_info (be_operation *node,
                         be_type *bt);

  // =helper
  /// stuff to output after every member of the scope is handled
  virtual int post_process (be_decl *);
};

#endif /* _BE_VISITOR_OPERATION_AMI_CS_H_ */
