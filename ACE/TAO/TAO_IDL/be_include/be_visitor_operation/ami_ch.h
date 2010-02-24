//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_ch.h
//
// = DESCRIPTION
//    Visitor for generating AMI stub code for IDL operations in
//    client header.
//
// = AUTHOR
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_AMI_CH_H_
#define _BE_VISITOR_OPERATION_AMI_CH_H_

// ************************************************************
// Operation visitor to generate AMI stubs for client header
// ************************************************************

class be_visitor_operation_ami_ch : public be_visitor_operation
{
  //
  // = TITLE
  //     be_visitor_operation_ami_ch.
  //
  // = DESCRIPTION
  //     This is a concrete visitor to generate the AMI stubs in the
  //     client header for operation.
  //
  //
public:
  be_visitor_operation_ami_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_ami_ch (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation.
};

#endif /* _BE_VISITOR_OPERATION_AMI_CH_H_ */
