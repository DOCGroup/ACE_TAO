// -*- C++ -*-

//=============================================================================
/**
 *  @file    exceptlist_ss.h
 *
 *  $Id$
 *
 *  Generates the array of user exception TypeCodes used by the
 *  skeletons.
 *
 *
 *  @author Ossama Othman
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_EXCEPTIONLIST_SS_H_
#define _BE_VISITOR_OPERATION_EXCEPTIONLIST_SS_H_

// ***************************************************************************
// Visitor for generating the user exception TypeCode list used by the
// skeletons.
// ***************************************************************************

/**
 * @class be_visitor_operation_exceptlist_ss
 *
 * @brief be_visitor_operaion_exceptlist_ss
 *
 * This is a visitor to generate operation exception TypeCode list
 * to be used by the skeletons.
 */
class be_visitor_operation_exceptlist_ss : public be_visitor_decl
{
public:

  /// constructor
  be_visitor_operation_exceptlist_ss (be_visitor_context * ctx);

  /// destructor
  ~be_visitor_operation_exceptlist_ss (void);

  /// visit the operation
  int visit_operation (be_operation * node);

};

#endif /* _BE_VISITOR_OPERATION_EXCEPTIONLIST_SS_H_ */
