// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_ih.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations in implementation header
//
// = AUTHOR
//   Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_OPERATION_IH_H_
#define _BE_VISITOR_OPERATION_OPERATION_IH_H_

// ************************************************************
// Operation visitor for implementation header
// ************************************************************

class be_visitor_operation_ih : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operation_ih
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the implementation header for operation
  //
  //
public:
  be_visitor_operation_ih (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_ih (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation. We provide code for this method in the derived class

  int gen_raise_exception (be_operation *node);

};

#endif /* _BE_VISITOR_OPERATION_OPERATION_IH_H_ */
