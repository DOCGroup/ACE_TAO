//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    collocated_ch.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations for the collocated case
//    inside of the servant header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_COLLOCATED_SH_H_
#define _BE_VISITOR_OPERATION_COLLOCATED_SH_H_

// ************************************************************
// Operation visitor for server header in the collocated class
// ************************************************************

class be_visitor_operation_collocated_sh : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_collocated_sh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for operation
  //   in the collocated class
  //
  //
public:
  be_visitor_operation_collocated_sh (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_collocated_sh (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation node.

};

#endif /* _BE_VISITOR_OPERATION_COLLOCATED_SH_H_ */
