//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_sh.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations in server header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_OPERATION_SH_H_
#define _BE_VISITOR_OPERATION_OPERATION_SH_H_

// ************************************************************
// Operation visitor for server header
// ************************************************************

class be_visitor_operation_sh : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operation_sh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for operation
  //
  //
public:
  be_visitor_operation_sh (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_sh (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation.

};

#endif /* _BE_VISITOR_OPERATION_OPERATION_SH_H_ */
