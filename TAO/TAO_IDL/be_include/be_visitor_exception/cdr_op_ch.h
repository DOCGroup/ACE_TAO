/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_ch.h
 *
 *  Concrete visitor for the Exception class
 *  This one provides code generation for the CDR operators for the exception
 *  in the client header.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_EXCEPTION_CDR_OP_CH_H_
#define _BE_VISITOR_EXCEPTION_CDR_OP_CH_H_

/**
 * @class be_visitor_exception_cdr_op_ch
 *
 * @brief be_visitor_exception_cdr_op_ch
 *
 * This is a concrete visitor for exception that generates the CDR operator
 * declarations
 */
class be_visitor_exception_cdr_op_ch : public be_visitor_exception
{
public:
  be_visitor_exception_cdr_op_ch (be_visitor_context *ctx);
  ~be_visitor_exception_cdr_op_ch ();

  virtual int visit_exception (be_exception *node);
};

#endif /* _BE_VISITOR_EXCEPTION_CDR_OP_CH_H_ */
