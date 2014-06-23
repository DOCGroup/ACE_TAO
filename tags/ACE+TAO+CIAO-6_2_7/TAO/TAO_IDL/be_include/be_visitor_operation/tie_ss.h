
//=============================================================================
/**
 *  @file    tie_ss.h
 *
 *  $Id$
 *
 *  Visitor for generating code for IDL operations for the TIE class
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_TIE_SS_H_
#define _BE_VISITOR_OPERATION_TIE_SS_H_

// ***********************************************************************
// Operation visitor for operation declaration inside the TIE class
// ***********************************************************************

/**
 * @class be_visitor_operation_tie_ss
 *
 * @brief be_visitor_operation_tie_ss
 *
 * This is a concrete visitor to generate the operaton inside a TIE class
 */
class be_visitor_operation_tie_ss : public be_visitor_scope
{
public:
  /// constructor
  be_visitor_operation_tie_ss (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_operation_tie_ss (void);

  /// set the right context and make a visitor
  virtual int visit_operation (be_operation *node);
};

#endif /* _BE_VISITOR_OPERATION_TIE_SS_H_ */
