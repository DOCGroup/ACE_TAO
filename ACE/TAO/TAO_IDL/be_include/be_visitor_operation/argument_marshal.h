//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    argument_marshal.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations. This is a visitor for
//    passing arguments to the CDR operators in a compiled marshaling
//    stub/skeleton
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_ARGUMENT_MARSHAL_H_
#define _BE_VISITOR_OPERATION_ARGUMENT_MARSHAL_H_

// ************************************************************
// generic operation visitor for docall, upcall, pre/post
// ************************************************************

class be_visitor_operation_argument_marshal
  : public be_visitor_operation_argument
{
  //
  // = TITLE
  //   be_compiled_visitor_operation_argument_marshal
  //
  // = DESCRIPTION
  //   This is a visitor to generate a variable declaration of an operation
  //   return type
  //
public:
  be_visitor_operation_argument_marshal (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_argument_marshal (void);
  // destructor

  virtual int pre_process (be_decl *bd);
  // stuff to o/p before each element of the scope is handled

  virtual int post_process (be_decl *bd);
  // stuff to o/p after each element of the scope is handled

  enum LAST_ARG_PRINTED
  {
    TAO_ARG_NONE,
    TAO_ARG_IN,
    TAO_ARG_INOUT,
    TAO_ARG_OUT
  };

private:
  LAST_ARG_PRINTED last_arg_printed_;
};

// ****************************************************************

class be_visitor_args_decl : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_compiled_args_decl
  //
  // = DESCRIPTION
  //   This is a visitor to generate variables declarations in the
  //   compiled marshaling stubs.
  //
public:
  be_visitor_args_decl (be_visitor_context *ctx);
  // constructor

  int visit_array (be_array *node);
  int visit_typedef (be_typedef *node);
  int visit_argument (be_argument *node);
  int visit_operation (be_operation *node);
};

#endif /* _BE_VISITOR_OPERATION_ARGUMENT_MARSHAL_H_ */
