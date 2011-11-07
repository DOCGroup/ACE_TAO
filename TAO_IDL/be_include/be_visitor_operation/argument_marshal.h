
//=============================================================================
/**
 *  @file    argument_marshal.h
 *
 *  $Id$
 *
 *  Visitor for generating code for IDL operations. This is a visitor for
 *  passing arguments to the CDR operators in a compiled marshaling
 *  stub/skeleton
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_ARGUMENT_MARSHAL_H_
#define _BE_VISITOR_OPERATION_ARGUMENT_MARSHAL_H_

// ************************************************************
// generic operation visitor for docall, upcall, pre/post
// ************************************************************

/**
 * This is a visitor to generate a variable declaration of an operation
 * return type
 */
class be_visitor_operation_argument_marshal
  : public be_visitor_operation_argument
{
public:
  /// constructor
  be_visitor_operation_argument_marshal (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_argument_marshal (void);

  /// stuff to o/p before each element of the scope is handled
  virtual int pre_process (be_decl *bd);

  /// stuff to o/p after each element of the scope is handled
  virtual int post_process (be_decl *bd);

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

/**
 * @class be_visitor_args_decl
 *
 * @brief be_visitor_compiled_args_decl
 *
 * This is a visitor to generate variables declarations in the
 * compiled marshaling stubs.
 */
class be_visitor_args_decl : public be_visitor_scope
{
public:
  /// constructor
  be_visitor_args_decl (be_visitor_context *ctx);

  int visit_array (be_array *node);
  int visit_typedef (be_typedef *node);
  int visit_argument (be_argument *node);
  int visit_operation (be_operation *node);
};

#endif /* _BE_VISITOR_OPERATION_ARGUMENT_MARSHAL_H_ */
