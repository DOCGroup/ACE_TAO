//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_thru_poa_collocated_cs.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations for the thru_poa collocated case
//    inside of the servant skeleton
//
// = AUTHOR
//    Aniruddha Gokhale
//    Michael Kircher
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_AMI_HANDLER_THRU_POA_COLLOCATED_CS_H_
#define _BE_VISITOR_OPERATION_AMI_HANDLER_THRU_POA_COLLOCATED_CS_H_

// *****************************************************************
// Operation visitor for server skeletons for the thru_poa collocated class
// *****************************************************************

class be_visitor_operation_ami_handler_thru_poa_collocated_cs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_ss
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server skeletons for
  //   operation in the thru_poa collocated class
  //
  //
public:
  be_visitor_operation_ami_handler_thru_poa_collocated_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_ami_handler_thru_poa_collocated_cs (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation node.

  virtual int gen_invoke (be_visitor_context &ctx, be_operation *node);
  // A helper method to generate operation invocation code.

  virtual int void_return_type (be_type *bt);
  // is my return type void?
};

#endif /* _BE_VISITOR_OPERATION_AMI_HANDLER_THRU_POA_COLLOCATED_CS_H_ */
