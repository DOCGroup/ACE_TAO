//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_ch.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations in client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_OPERATION_CH_H_
#define _BE_VISITOR_OPERATION_OPERATION_CH_H_

// ************************************************************
// Operation visitor for client header
// ************************************************************

class be_visitor_operation_ch : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operation_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for operation
  //
  //
public:
  be_visitor_operation_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_ch (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation.
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_CH_H_ */
