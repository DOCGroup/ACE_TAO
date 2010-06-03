/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Exception class
 *  This one provides code generation for the CDR operators for the exception
 *  in the client stub.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_EXCEPTION_CDR_OP_CS_H_
#define _BE_VISITOR_EXCEPTION_CDR_OP_CS_H_

/**
 * @class be_visitor_exception_cdr_op_cs
 *
 * @brief be_visitor_exception_cdr_op_cs
 *
 * This is a concrete visitor for exception that generates the CDR operator
 * implementations
 */
class be_visitor_exception_cdr_op_cs : public be_visitor_exception
{

public:
  be_visitor_exception_cdr_op_cs (be_visitor_context *ctx);
  ~be_visitor_exception_cdr_op_cs (void);

  virtual int visit_exception (be_exception *node);

  virtual int post_process (be_decl *);
};

#endif /* _BE_VISITOR_EXCEPTION_CDR_OP_CS_H_ */
