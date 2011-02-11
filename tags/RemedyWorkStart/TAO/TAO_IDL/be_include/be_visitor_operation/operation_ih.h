
//=============================================================================
/**
 *  @file    operation_ih.h
 *
 *  $Id$
 *
 *  Visitor for generating code for IDL operations in implementation header
 *
 *
 *  @author Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_OPERATION_IH_H_
#define _BE_VISITOR_OPERATION_OPERATION_IH_H_

// ************************************************************
// Operation visitor for implementation header
// ************************************************************

/**
 * @class be_visitor_operation_ih
 *
 * @brief be_visitor_operation_ih
 *
 * This is a concrete visitor to generate the implementation header for operation
 */
class be_visitor_operation_ih : public be_visitor_operation
{
public:
  /// constructor
  be_visitor_operation_ih (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_ih (void);

  /// visit operation. We provide code for this method in the derived class
  virtual int visit_operation (be_operation *node);
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_IH_H_ */
