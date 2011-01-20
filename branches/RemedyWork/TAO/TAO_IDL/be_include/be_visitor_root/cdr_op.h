/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for the CDR operators for elements of
 *  the Root scope.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_ROOT_CDR_OP_H_
#define _BE_VISITOR_ROOT_CDR_OP_H_

/**
 * @class be_visitor_root_cdr_op
 *
 * @brief be_visitor_root_cdr_op
 *
 * This is a concrete visitor for root that generates the CDR operator
 * declarations and implementations
 */
class be_visitor_root_cdr_op : public be_visitor_root
{
public:
  be_visitor_root_cdr_op (be_visitor_context *ctx);

  ~be_visitor_root_cdr_op (void);

  virtual int visit_root (be_root *node);
};

#endif /* _BE_VISITOR_ROOT_CDR_OP_H_ */
