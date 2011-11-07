
//=============================================================================
/**
 *  @file    operation_sh.h
 *
 *  $Id$
 *
 *  Visitor for generating code for IDL operations in server header
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_OPERATION_SH_H_
#define _BE_VISITOR_OPERATION_OPERATION_SH_H_

// ************************************************************
// Operation visitor for server header
// ************************************************************

/**
 * @class be_visitor_operation_sh
 *
 * @brief be_visitor_operation_sh
 *
 * This is a concrete visitor to generate the server header for operation
 */
class be_visitor_operation_sh : public be_visitor_operation
{
public:
  /// constructor
  be_visitor_operation_sh (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_sh (void);

  /// visit operation.
  virtual int visit_operation (be_operation *node);
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_SH_H_ */
