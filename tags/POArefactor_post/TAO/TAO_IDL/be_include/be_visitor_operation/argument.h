//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    argument.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations. This is a generic visitor
//    that calls visitors for the Argument class.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_ARGUMENT_H_
#define _BE_VISITOR_OPERATION_ARGUMENT_H_

// ************************************************************
// generic operation visitor for docall, upcall, pre/post
// ************************************************************

class be_visitor_operation_argument : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operation_argument
  //
  // = DESCRIPTION
  //   This is a visitor to generate a variable declaration ofr an operation
  //   return type
  //
public:
  be_visitor_operation_argument (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_argument (void);
  // destructor

  virtual int post_process (be_decl *);
  // stuff to o/p after each element of the scope is handled

  virtual int visit_operation (be_operation *node);
  // visit the operation

  virtual int visit_argument (be_argument *node);
  // visit argument
};

#endif /* _BE_VISITOR_OPERATION_ARGUMENT_H_ */
