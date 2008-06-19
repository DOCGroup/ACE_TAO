// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    exceptlist_ss.h
//
// = DESCRIPTION
//    Generates the array of user exception TypeCodes used by the
//    skeletons.
//
// = AUTHOR
//    Ossama Othman
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_EXCEPTIONLIST_SS_H_
#define _BE_VISITOR_OPERATION_EXCEPTIONLIST_SS_H_

// ***************************************************************************
// Visitor for generating the user exception TypeCode list used by the
// skeletons.
// ***************************************************************************

class be_visitor_operation_exceptlist_ss : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_operaion_exceptlist_ss
  //
  // = DESCRIPTION
  //   This is a visitor to generate operation exception TypeCode list
  //   to be used by the skeletons.
  //
  //
public:

  /// constructor
  be_visitor_operation_exceptlist_ss (be_visitor_context * ctx);

  /// destructor
  ~be_visitor_operation_exceptlist_ss (void);

  /// visit the operation
  int visit_operation (be_operation * node);

};

#endif /* _BE_VISITOR_OPERATION_EXCEPTIONLIST_SS_H_ */
