//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_ami_handler_skeleton_cs.h
//
// = DESCRIPTION
//    Visitor for generating code skelton code for IDL operations 
//    of AMI Reply Handlers.
//
// = AUTHOR
//    Michael Kircher
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_OPERATION_AMI_HANDLER_SKELETON_CS_H_
#define _BE_VISITOR_OPERATION_OPERATION_AMI_HANDLER_SKELETON_CS_H_

// ************************************************************
// Operation visitor for client stubs
// ************************************************************

class be_visitor_operation_ami_handler_skeleton_cs : public be_visitor_operation
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
  be_visitor_operation_ami_handler_skeleton_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_ami_handler_skeleton_cs (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation.

  virtual int visit_argument (be_argument *node);
  // visit argument to generate ParamData entries

  // = template methods
  virtual int gen_pre_stub_info (be_operation *, be_type *) = 0;
  // generate any info before the actual code for the stub is generated

  virtual int gen_marshal_and_invoke (be_operation *, be_type *) = 0;
  // generate code that marshals the arguments and transmits them

  // =helper
  virtual int post_process (be_decl *);
  // stuff to output after every member of the scope is handled

  virtual int gen_raise_exception (be_type *,
                                   const char * excep,
                                   const char * status);
  // helper that generates code for raising an exception

  virtual int gen_check_exception (be_type *);
  // helper that generates code for checking for an exception

};

// specialized visitors

class be_interpretive_visitor_operation_ami_handler_skeleton_cs : public be_visitor_operation_ami_handler_skeleton_cs
{
  //
  // = TITLE
  //   be_interpretive_visitor_operation_ami_skeleton_operation_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client skeletons 
  //   for AMI reply handlers of IDL operations using interpretive marshaling.
  //
  //
public:
  be_interpretive_visitor_operation_ami_handler_skeleton_cs (be_visitor_context *);
  // ctor

  ~be_interpretive_visitor_operation_ami_handler_skeleton_cs (void);
  // dtor

  virtual int gen_pre_stub_info (be_operation *, be_type *);
  // generate any info before the actual code for the stub is generated

  virtual int gen_marshal_and_invoke (be_operation *, be_type *);
  // generate code that marshals the arguments and transmits them

};

class be_compiled_visitor_operation_ami_handler_skeleton_cs : public be_visitor_operation_ami_handler_skeleton_cs
{
  //
  // = TITLE
  //   be_compiled_visitor_operation_ami_handler_skeleton_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client skeletons 
  //   for AMI reply handlers of IDL operations using compiled marshaling.
  //
  //
public:
  be_compiled_visitor_operation_ami_handler_skeleton_cs (be_visitor_context *);
  // ctor

  ~be_compiled_visitor_operation_ami_handler_skeleton_cs (void);
  // dtor

  virtual int gen_pre_stub_info (be_operation *, be_type *);
  // generate any info before the actual code for the stub is generated

  virtual int gen_marshal_and_invoke (be_operation *, be_type *);
  // generate code that marshals the arguments and transmits them

};


#endif /* _BE_VISITOR_OPERATION_OPERATION_AMI_HANDLER_SKELETON_CS_H_ */
