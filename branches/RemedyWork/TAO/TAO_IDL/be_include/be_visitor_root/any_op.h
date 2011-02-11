/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    any_op.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for the Any operators for elements of
 *  the Root scope.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_ROOT_ANY_OP_H_
#define _BE_VISITOR_ROOT_ANY_OP_H_

/**
 * @class be_visitor_root_any_op
 *
 * @brief be_visitor_root_any_op
 *
 * This is a concrete visitor for root that generates the Any operator
 * declarations and implementations
 */
class be_visitor_root_any_op : public be_visitor_root
{
public:
  be_visitor_root_any_op (be_visitor_context *ctx);

  ~be_visitor_root_any_op (void);

  virtual int visit_root (be_root *node);
};

#endif /* _BE_VISITOR_ROOT_ANY_OP_H_ */
