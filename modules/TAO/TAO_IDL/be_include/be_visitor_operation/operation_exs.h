//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    operation_exs.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations in
//    CIAO executor implementations
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_OPERATION_EXS_H_
#define _BE_VISITOR_OPERATION_OPERATION_EXS_H_

// ************************************************************
// Operation visitor for CIAO executor implementations
// ************************************************************

class be_visitor_operation_exs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_exs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the CIAO executor
  //   implementations for operation
  //
  //
public:
  be_visitor_operation_exs (be_visitor_context *ctx);

  ~be_visitor_operation_exs (void);

  virtual int visit_operation (be_operation *node);
  virtual int visit_argument (be_argument *node);
  
  virtual int post_process (be_decl *bd);
  
  void scope (be_interface *node);

private:
  int gen_op_body (be_type *return_type);
  
private:
  TAO_OutStream &os_;
  be_interface *scope_;
  const char *your_code_here_;
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_EXS_H_ */
