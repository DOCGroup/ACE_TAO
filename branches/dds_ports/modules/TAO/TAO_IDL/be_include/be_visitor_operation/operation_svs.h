//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_svs.h
//
// = DESCRIPTION
//    Visitor for generating code for IDL operations in
//    CIAO servant implementations
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_OPERATION_OPERATION_SVS_H_
#define _BE_VISITOR_OPERATION_OPERATION_SVS_H_

// ************************************************************
// Operation visitor for CIAO servant implementations
// ************************************************************

class be_visitor_operation_svs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_operation_svs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the CIAO servant
  //   implementations for operation
  //
  //
public:
  be_visitor_operation_svs (be_visitor_context *ctx);

  ~be_visitor_operation_svs (void);

  virtual int visit_operation (be_operation *node);
  virtual int visit_argument (be_argument *node);
  
  virtual int post_process (be_decl *bd);
  
  void for_facets (bool val);
  void scope (be_interface *node);
  
private:
  int gen_op_body (be_operation *node);
  
private:
  be_interface *scope_;
  bool for_facets_;
  bool swapping_;
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_SVS_H_ */
