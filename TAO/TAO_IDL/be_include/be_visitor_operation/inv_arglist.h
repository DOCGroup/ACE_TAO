// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    inv_arglist.h
//
// = DESCRIPTION
//    Visitor for generating signature needed to invoke a given
//    operation.
//
// = AUTHOR
//    Angelo Corsaro
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_INV_ARGLIST_H_
#define _BE_VISITOR_OPERATION_INV_ARGLIST_H_

// ******************************************************************************
// Operation visitor for argument list - generates parameters in the signature
// ******************************************************************************

class be_visitor_operation_inv_arglist : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operaion_inv_arglist
  //
  // = DESCRIPTION
  //   This visitor generates the argument list needed to
  //   invoke a given operation. No parentesys are generated
  //   to give the caller code the abity to stick in other
  //   parameter that might be needed.
  //
  //
public:
  be_visitor_operation_inv_arglist (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_inv_arglist (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit the operation

  virtual int visit_argument (be_argument *node);
  // visit each argument

};

#endif /* _BE_VISITOR_OPERATION_INV_ARGLIST_H_ */
