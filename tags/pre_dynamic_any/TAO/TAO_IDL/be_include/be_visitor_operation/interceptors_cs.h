//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interceptors_cs.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations. This is a generic visitor
//    that calls visitors for the operation class.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_INTERCEPTORS_CS_H_
#define _BE_VISITOR_OPERATION_INTERCEPTORS_CS_H_

// ************************************************************
// generic operation visitor for docall, upcall, pre/post
// ************************************************************

class be_visitor_operation_interceptors_cs : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operation_interceptors_cs
  //
  // = DESCRIPTION
  //   This is a visitor to generate a variable declaration ofr an operation
  //   return type
  //
public:
  be_visitor_operation_interceptors_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_interceptors_cs (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit the operation

};

#endif /* _BE_VISITOR_OPERATION_INTERCEPTORS_H_ */
