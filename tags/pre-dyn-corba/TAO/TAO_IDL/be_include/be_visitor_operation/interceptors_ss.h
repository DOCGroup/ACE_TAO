//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interceptors_ss.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations. This is a generic visitor
//    that calls visitors for the operation class.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_INTERCEPTORS_SS_H_
#define _BE_VISITOR_OPERATION_INTERCEPTORS_SS_H_

// ************************************************************
// generic operation visitor for docall, upcall, pre/post
// ************************************************************

class be_visitor_operation_interceptors_ss : public be_visitor_operation
{
  //
  // = TITLE
  //   be_visitor_operation_interceptors_ss
  //
  // = DESCRIPTION
  //   This is a visitor to generate a variable declaration ofr an operation
  //   return type
  //
public:
  be_visitor_operation_interceptors_ss (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation_interceptors_ss (void);
  // destructor

  virtual int visit_operation (be_operation *node);
  // visit the operation

private:
  int generate_class_declaration (TAO_OutStream *os,
                                  be_operation *node);
  // Generate the declaration for the helper RequestInfo class

  int generate_class_definition (TAO_OutStream *os,
                                 be_operation *node);
  // Generate the definition for the helper RequestInfo class
};

#endif /* _BE_VISITOR_OPERATION_INTERCEPTORS_SS_H_ */
