
//=============================================================================
/**
 *  @file    exceptlist_cs.h
 *
 *  Visitor for generating code for IDL operations. This generates the tables
 *  for the exceptions used by the stubs.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_EXCEPTIONLIST_CS_H_
#define _BE_VISITOR_OPERATION_EXCEPTIONLIST_CS_H_

// ******************************************************************************
// Visitor for generating the exception list
// ******************************************************************************

/**
 * @class be_visitor_operation_exceptlist_cs
 *
 * @brief be_visitor_operaion_exceptlist_cs
 *
 * This is a visitor to generate operation exception list to be provided to
 * the do_static_call
 */
class be_visitor_operation_exceptlist_cs : public be_visitor_decl
{
public:
  /// constructor
  explicit be_visitor_operation_exceptlist_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_exceptlist_cs () override;
  be_visitor_operation_exceptlist_cs (be_visitor_operation_exceptlist_cs const &) = delete;
  be_visitor_operation_exceptlist_cs (be_visitor_operation_exceptlist_cs &&) = delete;
  be_visitor_operation_exceptlist_cs &operator= (be_visitor_operation_exceptlist_cs const &) = delete;
  be_visitor_operation_exceptlist_cs &operator= (be_visitor_operation_exceptlist_cs &&) = delete;

  /// visit the operation
  int visit_operation (be_operation *node) override;
};

#endif /* _BE_VISITOR_OPERATION_EXCEPTIONLIST_CS_H_ */
