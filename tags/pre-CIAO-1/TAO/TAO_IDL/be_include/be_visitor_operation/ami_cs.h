//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_cs.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations in client stubs
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_AMI_CS_H_
#define _BE_VISITOR_OPERATION_AMI_CS_H_

// ************************************************************
// Operation visitor for client stubs
// ************************************************************

class be_visitor_operation_ami_cs : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operation_ami_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for operation
  //
  //
public:
  be_visitor_operation_ami_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_ami_cs (void);
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
};

// specialized visitors

class be_interpretive_visitor_operation_ami_cs : public be_visitor_operation_ami_cs
{
  //
  // = TITLE
  //   be_interpretive_visitor_operation_ami_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for operation
  //   using interpretive marshaling.
  //
  //
public:
  be_interpretive_visitor_operation_ami_cs (be_visitor_context *);
  // ctor

  ~be_interpretive_visitor_operation_ami_cs (void);
  // dtor

  virtual int gen_pre_stub_info (be_operation *, be_type *);
  // generate any info before the actual code for the stub is generated

  virtual int gen_marshal_and_invoke (be_operation *, be_type *);
  // generate code that marshals the arguments and transmits them

};

class be_compiled_visitor_operation_ami_cs : public be_visitor_operation_ami_cs
{
  //
  // = TITLE
  //   be_compiled_visitor_operation_ami_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for operation
  //   using compiled marshaling.
  //
  //
public:
  be_compiled_visitor_operation_ami_cs (be_visitor_context *);
  // ctor

  ~be_compiled_visitor_operation_ami_cs (void);
  // dtor

  virtual int gen_pre_stub_info (be_operation *, be_type *);
  // generate any info before the actual code for the stub is generated

  virtual int gen_marshal_and_invoke (be_operation *, be_type *);
  // generate code that marshals the arguments and transmits them
};

#endif /* _BE_VISITOR_OPERATION_AMI_CS_H_ */
