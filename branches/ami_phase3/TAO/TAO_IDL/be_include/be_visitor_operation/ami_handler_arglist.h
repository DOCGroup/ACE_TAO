//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_arglist.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations. This generates the
//    operation signature.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_AMI_HANDLER_ARGLIST_H_
#define _BE_VISITOR_OPERATION_AMI_HANDLER_ARGLIST_H_

// ******************************************************************************
// Operation visitor for argument list - generates parameters in the signature
// ******************************************************************************

class be_visitor_operation_ami_handler_arglist : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operaion_ami_handler_arglist
  //
  // = DESCRIPTION
  //   This is a visitor to generate operation argument list
  //
  //
public:
  be_visitor_operation_ami_handler_arglist (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_ami_handler_arglist (void);
  // destructor

  virtual int visit_scope (be_scope *node);
  // Visit the scope and its elements. 
  // This implementation is the same as
  // <be_visitor_scope::visit_scope>. The variation is that it calls the
  // <post_process>  only if the return value of the <accept> is 1. We
  // need to do this since we have to differentiate between an argument
  // that was printed and an argument that was skipped in the signature.
  
  virtual int visit_operation (be_operation *node);
  // visit the operation

  virtual int visit_argument (be_argument *node);
  // visit each argument

  virtual int post_process (be_decl *);
  // stuff to o/p after each element of the scope is handled

};

#endif /* _BE_VISITOR_OPERATION_AMI_HANDLER_ARGLIST_H_ */
