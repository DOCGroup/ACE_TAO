//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_ami.h
//
// = DESCRIPTION
//    Visitor for generating AMI stub code for IDL operations in
//    client header.  
//
// = AUTHOR
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_OPERATION_AMI_H_
#define _BE_VISITOR_OPERATION_OPERATION_AMI_H_

// ************************************************************
// Operation visitor to generate AMI stubs for client header
// ************************************************************

class be_visitor_operation_ami : public be_visitor_operation
{
  //
  // = TITLE
  //     be_visitor_operation_ami.
  //
  // = DESCRIPTION
  //     This is a concrete visitor to generate the AMI stubs in the
  //     client header for operation.
  //
  //
public:
  be_visitor_operation_ami (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_ami (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation.
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_AMI_H_ */
