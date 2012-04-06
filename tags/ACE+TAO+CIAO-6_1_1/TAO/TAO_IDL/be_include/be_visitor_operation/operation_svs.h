
//=============================================================================
/**
 *  @file    operation_svs.h
 *
 *  $Id$
 *
 *  Visitor for generating code for IDL operations in
 *  CIAO servant implementations
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_VISITOR_OPERATION_OPERATION_SVS_H_
#define _BE_VISITOR_OPERATION_OPERATION_SVS_H_

// ************************************************************
// Operation visitor for CIAO servant implementations
// ************************************************************

/**
 * @class be_visitor_operation_svs
 *
 * @brief be_visitor_operation_svs
 *
 * This is a concrete visitor to generate the CIAO servant
 * implementations for operation
 */
class be_visitor_operation_svs : public be_visitor_scope
{
public:
  be_visitor_operation_svs (be_visitor_context *ctx);

  ~be_visitor_operation_svs (void);

  virtual int visit_operation (be_operation *node);
  virtual int visit_argument (be_argument *node);

  virtual int post_process (be_decl *bd);

  void scope (be_decl *node);

private:
  int gen_op_body (be_operation *node);

private:
  be_decl *scope_;
};

#endif /* _BE_VISITOR_OPERATION_OPERATION_SVS_H_ */
