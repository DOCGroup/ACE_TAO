//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_ami_handler_reply_stub_operation_cs.h
//
// = DESCRIPTION
//    Visitor for generating code skelton code for IDL operations
//    of AMI Reply Handlers.
//
// = AUTHOR
//    Michael Kircher
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_OPERATION_AMI_HANDLER_REPLY_STUB_OPERATION_CS_H_
#define _BE_VISITOR_OPERATION_OPERATION_AMI_HANDLER_REPLY_STUB_OPERATION_CS_H_

// ************************************************************
// Operation visitor for client stubs
// ************************************************************

class be_visitor_operation_ami_handler_reply_stub_operation_cs : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operation_ami_handler_operation_cs
  //
  // = DESCRIPTION
  //    Visitor for generating code skelton code for IDL operations
  //    of AMI Reply Handlers.
  //
  //
public:
  be_visitor_operation_ami_handler_reply_stub_operation_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_ami_handler_reply_stub_operation_cs (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation.

  virtual int visit_argument (be_argument *node);
  // visit argument to generate ParamData entries

  // = template methods
  int gen_pre_stub_info (be_operation *node, 
                         be_type *bt);
  // generate any info before the actual code for the stub is generated

  virtual int gen_marshal_and_invoke (be_operation *node, 
                                      be_type *bt);
  // generate code that marshals the arguments and transmits them

  // =helper
  virtual int post_process (be_decl *);
  // stuff to output after every member of the scope is handled
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_AMI_HANDLER_REPLY_STUB_OPERATION_CS_H_ */
