// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_is.cpp
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations in implementation skeleton
//
// = AUTHOR
//   Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_OPERATION_IS_CPP_
#define _BE_VISITOR_OPERATION_OPERATION_IS_CPP_

// ************************************************************
// Operation visitor for implementation skeleton
// ************************************************************

class be_visitor_operation_is : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operation_is
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the implementation skeleton for operation
  //
  //
public:
  be_visitor_operation_is (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_is (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation. We provide code for this method in the derived class

  int gen_raise_exception (be_operation *node);

};

#endif /* _BE_VISITOR_OPERATION_OPERATION_IS_CPP_ */
