/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Module class
 *  This one provides visitors to generate the CDR operators for elements in
 *  the Module's scope.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_MODULE_CDR_OP_H_
#define _BE_VISITOR_MODULE_CDR_OP_H_

/**
 * @class be_visitor_module_cdr_op
 *
 * @brief be_visitor_module_cdr_op
 *
 * This is a concrete visitor for module that generates the CDR operator
 * declarations and implementations
 */
class be_visitor_module_cdr_op : public be_visitor_module
{

public:
  /// constructor
  be_visitor_module_cdr_op (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_module_cdr_op (void);

  /// visit module
  virtual int visit_module (be_module *node);
};

#endif /* _BE_VISITOR_MODULE_CDR_OP_H_ */
