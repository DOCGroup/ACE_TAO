
//=============================================================================
/**
 *  @file    arglist.h
 *
 *  $Id$
 *
 *  Visitor for generating code for IDL (valuetype) operations.
 *  This generates the operation signature.
 *
 *
 *
 */
//=============================================================================


#ifndef _BE_VISITOR_OBV_OPERATION_ARGLIST_H_
#define _BE_VISITOR_OBV_OPERATION_ARGLIST_H_

// ******************************************************************************
// Operation visitor for argument list - generates parameters in the signature
// ******************************************************************************

/**
 * @class be_visitor_obv_operation_arglist
 *
 * @brief be_visitor_operaion_arglist
 *
 * This is a visitor to generate operation argument list
 */
class be_visitor_obv_operation_arglist : public be_visitor_scope
{
public:
  /// constructor
  be_visitor_obv_operation_arglist (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_obv_operation_arglist (void);

  /// special arglist generation for amh_exceptionholders
  bool is_amh_exception_holder (be_operation *node);

  /// visit the operation
  int visit_operation (be_operation *node);

  /// visit each argument
  int visit_argument (be_argument *node);

  /// set 'comma' only between arguments
  int post_process (be_decl *bd);
};

#endif /* _BE_VISITOR_OBV_OPERATION_ARGLIST_H_ */
