//
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interceptors_arglist.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations. This generates the
//    operation signature.
//
// = AUTHOR
//    Kirthika Parameswaran
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_INTERCEPTORS_ARGLIST_H_
#define _BE_VISITOR_OPERATION_INTERCEPTORS_ARGLIST_H_

// ******************************************************************************
// Operation visitor for argument list - generates parameters in the signature
// ******************************************************************************

class be_visitor_operation_interceptors_arglist : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operaion_interceptors_arglist
  //
  // = DESCRIPTION
  //   This is a visitor to generate operation argument list
  //
  //
public:
  be_visitor_operation_interceptors_arglist (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_interceptors_arglist (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit the operation

  virtual int visit_argument (be_argument *node);
  // visit each argument

  virtual int pre_process (be_decl *);
  // stuff to o/p before each element of the scope is handled

  virtual int post_process (be_decl *);
  // stuff to o/p after each element of the scope is handled

};

#endif /* _BE_VISITOR_OPERATION_INTERCEPTORS_ARGLIST_H_ */
