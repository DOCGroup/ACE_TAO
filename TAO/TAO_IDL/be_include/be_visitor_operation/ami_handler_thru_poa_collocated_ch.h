//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    thru_poa_collocated_ch.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations for the thru_poa collocated case
//    inside of the servant header
//    Special code for the AMI Reply Handler
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_ami_handler_thru_poa_collocated_ch_H_
#define _BE_VISITOR_OPERATION_ami_handler_thru_poa_collocated_ch_H_

// ************************************************************
// Operation visitor for server header in the thru_poa collocated class
// ************************************************************

class be_visitor_operation_ami_handler_thru_poa_collocated_ch : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_ami_handler_thru_poa_collocated_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for operation
  //   in the thru_poa collocated class
  //
  //
public:
  be_visitor_operation_ami_handler_thru_poa_collocated_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_ami_handler_thru_poa_collocated_ch (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation node.

};

#endif /* _BE_VISITOR_OPERATION_ami_handler_thru_poa_collocated_ch_H_ */
