//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    arglist_ami.h
//
// = DESCRIPTION
//    Visitor for generating IDL Stub code for IDL operations. This
//    generates the operation signature for the IDL stub. 
//
// = AUTHOR
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_ARGLIST_AMI_H_
#define _BE_VISITOR_OPERATION_ARGLIST_AMI_H_

// ******************************************************************************
// Operation visitor for argument list - generates parameters in the signature
// ******************************************************************************

class be_visitor_operation_arglist_ami : public be_visitor_scope
{
  //
  // = TITLE
  //     be_visitor_operaion_arglist_ami
  //
  // = DESCRIPTION
  //     This is a visitor to generate operation argument list for an
  //     AMI stub.
  //
  //
public:
  be_visitor_operation_arglist_ami (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_arglist_ami (void);
  // destructor

  int visit_operation (be_operation *node);
  // visit the operation

  int visit_argument (be_argument *node);
  // visit each argument
};

#endif /* _BE_VISITOR_OPERATION_ARGLIST_AMI_H_ */
