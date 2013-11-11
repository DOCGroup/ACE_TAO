/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for valueboxes.
 *  This one provides code generation for the CDR operators.
 *
 *
 *  @author Gary Maxey
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUEBOX_CDR_OP_CH_H_
#define _BE_VISITOR_VALUEBOX_CDR_OP_CH_H_

/**
 * @class be_visitor_valuebox_cdr_op_ch
 *
 * @brief be_visitor_valuebox_cdr_op_ch
 *
 * This is a concrete visitor for valuebox that generates the CDR operator
 * declarations
 */
class be_visitor_valuebox_cdr_op_ch : public be_visitor_valuebox
{

public:
  /// constructor
  be_visitor_valuebox_cdr_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuebox_cdr_op_ch (void);

  /// visit valuebox
  virtual int visit_valuebox (be_valuebox *node);
};

#endif /* _BE_VISITOR_VALUEBOX_CDR_OP_CH_H_ */
