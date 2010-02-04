
//=============================================================================
/**
 *  @file    operation_ch.h
 *
 *  $Id$
 *
 *  Visitor for generating code for IDL operations in client header
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_OPERATION_CH_H_
#define _BE_VISITOR_OPERATION_OPERATION_CH_H_

// ************************************************************
// Operation visitor for client header
// ************************************************************

/**
 * @class be_visitor_operation_ch
 *
 * @brief be_visitor_operation_ch
 *
 * This is a concrete visitor to generate the client header for operation
 */
class be_visitor_operation_ch : public be_visitor_operation
{
public:
  /// constructor
  be_visitor_operation_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_ch (void);

  /// visit operation.
  virtual int visit_operation (be_operation *node);
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_CH_H_ */
