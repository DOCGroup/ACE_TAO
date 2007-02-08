// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations in client stubs
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_OPERATION_H_
#define _BE_VISITOR_OPERATION_OPERATION_H_

// ************************************************************
// Generic Operation visitor with some helper methods
// ************************************************************

class be_visitor_operation : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation
  //
  // = DESCRIPTION
  //   This is a base helper visitor for operation node
  //
  //
public:
  be_visitor_operation (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation (void);
  // destructor

  // =helper

  virtual int void_return_type (be_type *);
  // is my return type void?

  virtual int has_param_type (be_operation *,
                              AST_Argument::Direction);
  // does any of the argument node have the specified direction

  /// Count the number of "in" and "inout" parameters.
  virtual size_t count_non_out_parameters (be_operation *node);

  /// Special generation of throw_spec if it is an AMH ExceptionHolder
  /// 0:false, 1:true
  virtual int is_amh_exception_holder (be_interface *node);

  virtual int gen_raise_exception (const char *exception_name,
                                   const char *exception_arguments);
  // helper that generates code for raising an exception

  virtual int gen_stub_operation_body (be_operation *node,
                                       be_type *return_type);
  // Generates the code from opening brace to closing brace for
  // stub operations. Also called (for now)
  // from be_visitor_operation_cs, to generate operation stub
  // bodies for abstract interfaces (unless and until abstract
  // interfaces are made to interoperate with collocatino).

  int gen_pre_stub_info (be_operation *node);
  // generate any info before the actual code for the stub is generated

  virtual int gen_raise_interceptor_exception (
      be_type *return_type,
      const char *exception_name,
      const char *exception_arguments
    );
  // helper that generates code for raising an exception within
  // interceptor's try block

  void gen_stub_body_arglist (be_operation *node,
                              TAO_OutStream *os,
                              bool ami = false);

  void gen_arg_template_param_name (AST_Decl *scope,
                                    AST_Type *bt,
                                    TAO_OutStream *os);
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_H_ */
