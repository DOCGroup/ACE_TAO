//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_reply_stub_operation_ch.h
//
// = DESCRIPTION
//    Visitor for generating code for AMI Handler call back operations
//    in client header.
//
// = AUTHOR
//    Aniruddha Gokhale and Alexander Babu Arulanthu
//    <alex@cs.wustl.edu>
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_ami_handler_reply_stub_operation_ch_H_
#define _BE_VISITOR_OPERATION_ami_handler_reply_stub_operation_ch_H_

// ************************************************************
// Operation visitor for server header
// ************************************************************

class be_visitor_operation_ami_handler_reply_stub_operation_ch : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_ami_handler_reply_stub_operation_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for operation
  //
  //
public:
  be_visitor_operation_ami_handler_reply_stub_operation_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_ami_handler_reply_stub_operation_ch (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation.
};

#endif /* _BE_VISITOR_OPERATION_ami_handler_reply_stub_operation_ch_H_ */
