//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    direct_collocated_ss.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations for the direct_collocated case
//    inside of the servant skeleton
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_DIRECT_COLLOCATED_SS_H_
#define _BE_VISITOR_OPERATION_DIRECT_COLLOCATED_SS_H_

// *****************************************************************
// Operation visitor for server skeletons for the direct collocated class
// *****************************************************************

class be_visitor_operation_direct_collocated_ss : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_ss
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server skeletons for
  //   operation in the direct collocated class
  //
  //
public:
  be_visitor_operation_direct_collocated_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_direct_collocated_ss (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation node.

  virtual int gen_invoke (be_visitor_context &ctx, be_operation *node);
  // A helper method to generate operation invocation code.

  virtual int void_return_type (be_type *bt);
  // is my return type void?
};

#endif /* _BE_VISITOR_OPERATION_DIRECT_COLLOCATED_SS_H_ */
