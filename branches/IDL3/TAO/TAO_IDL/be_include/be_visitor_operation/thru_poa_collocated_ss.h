//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    thru_poa_collocated_ss.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations for the thru_poa collocated case
//    inside of the servant skeleton
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_THRU_POA_COLLOCATED_SS_H_
#define _BE_VISITOR_OPERATION_THRU_POA_COLLOCATED_SS_H_

// *****************************************************************
// Operation visitor for server skeletons for the thru_poa collocated class
// *****************************************************************

class be_visitor_operation_thru_poa_collocated_ss : public be_visitor_operation
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
  be_visitor_operation_thru_poa_collocated_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_thru_poa_collocated_ss (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation node.

  virtual int gen_invoke (be_visitor_context &ctx, be_operation *node);
  // A helper method to generate operation invocation code.
};

#endif /* _BE_VISITOR_OPERATION_THRU_POA_COLLOCATED_SS_H_ */
