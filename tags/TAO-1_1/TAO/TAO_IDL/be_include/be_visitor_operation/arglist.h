//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    arglist.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations. This generates the
//    operation signature.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_ARGLIST_H_
#define _BE_VISITOR_OPERATION_ARGLIST_H_

// ******************************************************************************
// Operation visitor for argument list - generates parameters in the signature
// ******************************************************************************

class be_visitor_operation_arglist : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operaion_arglist
  //
  // = DESCRIPTION
  //   This is a visitor to generate operation argument list
  //
  //
public:
  be_visitor_operation_arglist (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_arglist (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit the operation

  virtual int visit_argument (be_argument *node);
  // visit each argument

  virtual int post_process (be_decl *);
  // stuff to o/p after each element of the scope is handled

};

#endif /* _BE_VISITOR_OPERATION_ARGLIST_H_ */
