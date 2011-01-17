
//=============================================================================
/**
 *  @file    argument.h
 *
 *  $Id$
 *
 *  Visitor for generating code for IDL operations. This is a generic visitor
 *  that calls visitors for the Argument class.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_ARGUMENT_H_
#define _BE_VISITOR_OPERATION_ARGUMENT_H_

// ************************************************************
// generic operation visitor for docall, upcall, pre/post
// ************************************************************

/**
 * @class be_visitor_operation_argument
 *
 * @brief be_visitor_operation_argument
 *
 * This is a visitor to generate a variable declaration ofr an operation
 * return type
 */
class be_visitor_operation_argument : public be_visitor_operation
{
public:
  /// constructor
  be_visitor_operation_argument (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_argument (void);

  /// stuff to o/p after each element of the scope is handled
  virtual int post_process (be_decl *);

  /// visit the operation
  virtual int visit_operation (be_operation *node);

  /// visit argument
  virtual int visit_argument (be_argument *node);
};

#endif /* _BE_VISITOR_OPERATION_ARGUMENT_H_ */
