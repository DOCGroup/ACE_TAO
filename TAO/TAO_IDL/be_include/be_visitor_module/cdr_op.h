/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op.h
 *
 *  Concrete visitor for the Module class
 *  This one provides visitors to generate the CDR operators for elements in
 *  the Module's scope.
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
  explicit be_visitor_module_cdr_op (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_module_cdr_op () override = default;

  be_visitor_module_cdr_op (const be_visitor_module_cdr_op &) = delete;
  be_visitor_module_cdr_op (be_visitor_module_cdr_op &&) = delete;
  be_visitor_module_cdr_op &operator= (const be_visitor_module_cdr_op &) = delete;
  be_visitor_module_cdr_op &operator= (be_visitor_module_cdr_op &&) = delete;

  /// visit module
  int visit_module (be_module *node) override;
};

#endif /* _BE_VISITOR_MODULE_CDR_OP_H_ */
