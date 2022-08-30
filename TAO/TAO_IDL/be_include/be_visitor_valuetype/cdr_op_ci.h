/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_ci.h
 *
 *  Concrete visitor for valuetypes.
 *  This one provides code generation for the CDR operators.
 *
 *  @author Torsten Kuepper <kuepper2@lfa.uni-wuppertal.de>
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUETYPE_CDR_OP_CI_H_
#define _BE_VISITOR_VALUETYPE_CDR_OP_CI_H_

/**
 * @class be_visitor_valuetype_cdr_op_ci
 *
 * @brief be_visitor_valuetype_cdr_op_ci
 *
 * This is a concrete visitor for valuetype that generates the CDR operator
 * implementations
 */
class be_visitor_valuetype_cdr_op_ci : public be_visitor_valuetype
{

public:
  /// constructor
  be_visitor_valuetype_cdr_op_ci (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_cdr_op_ci (void);

  /// visit valuetype
  int visit_valuetype (be_valuetype *node) override;

  /// visit field
  int visit_field (be_field *node) override;

  /// visit array
  int visit_array (be_array *node) override;

  /// visit sequence
  int visit_sequence (be_sequence *node) override;
};

#endif /* _BE_VISITOR_VALUETYPE_CDR_OP_CI_H_ */
