/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_ch.h
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
  /// constructor
  be_visitor_exception_cdr_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_exception_cdr_op_cs (void);

  /// visit exception
  virtual int visit_exception (be_exception *node);

  /// any post processing that needs to be done after a scope element is handled
  virtual int post_process (be_decl *);
};

#endif /* _BE_VISITOR_EXCEPTION_CDR_OP_CS_H_ */
