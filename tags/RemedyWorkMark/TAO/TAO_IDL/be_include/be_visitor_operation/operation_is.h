
//=============================================================================
/**
 *  @file    operation_is.h
 *
 *  $Id$
 *
 *  Visitor for generating code for IDL operations in implementation skeleton
 *
 *
 *  @author Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_OPERATION_IS_CPP_
#define _BE_VISITOR_OPERATION_OPERATION_IS_CPP_

// ************************************************************
// Operation visitor for implementation skeleton
// ************************************************************

/**
 * @class be_visitor_operation_is
 *
 * @brief be_visitor_operation_is
 *
 * This is a concrete visitor to generate the implementation skeleton for operation
 */
class be_visitor_operation_is : public be_visitor_operation
{
public:
  /// constructor
  be_visitor_operation_is (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_is (void);

  /// visit operation. We provide code for this method in the derived class
  virtual int visit_operation (be_operation *node);
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_IS_CPP_ */
