//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    collocated_ss.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations for the collocated case
//    inside of the servant skeleton
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_COLLOCATED_SS_H_
#define _BE_VISITOR_OPERATION_COLLOCATED_SS_H_

// *****************************************************************
// Operation visitor for server skeletons for the collocated class
// *****************************************************************

class be_visitor_operation_collocated_ss : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_ss
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server skeletons for
  //   operation in the collocated class
  //
  //
public:
  be_visitor_operation_collocated_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_collocated_ss (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation node.

};

#endif /* _BE_VISITOR_OPERATION_COLLOCATED_SS_H_ */
