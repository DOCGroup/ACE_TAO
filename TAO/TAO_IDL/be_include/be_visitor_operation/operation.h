//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations in client stubs
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_OPERATION_H_
#define _BE_VISITOR_OPERATION_OPERATION_H_

// ************************************************************
// Generic Operation visitor with some helper methods
// ************************************************************

class be_visitor_operation : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation
  //
  // = DESCRIPTION
  //   This is a base helper visitor for operation node
  //
  //
public:
  be_visitor_operation (be_visitor_context *ctx);
  // constructor

  ~be_visitor_operation (void);
  // destructor

  // =helper

  virtual int void_return_type (be_type *);
  // is my return type void?

  virtual int has_param_type (be_operation *, AST_Argument::Direction);
  // does any of the argument node have the specified direction

};

#endif /* _BE_VISITOR_OPERATION_OPERATION_H_ */
