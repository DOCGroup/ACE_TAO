//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interceptors_ch.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations for the interceptor case
//    inside of the client stubs header
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_INTERCEPTORS_CH_H_
#define _BE_VISITOR_OPERATION_INTERCEPTORS_CH_H_

// ************************************************************
// Operation visitor for client header in the interceptor class
// ************************************************************

class be_visitor_operation_interceptors_ch : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operation_interceptors_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client request info
  //   in header for operation in the interceptor class
  //
  //
public:
  be_visitor_operation_interceptors_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_interceptors_ch (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit operation node.
};

#endif /* _BE_VISITOR_OPERATION_INTERCEPTORS_CH_H_ */
