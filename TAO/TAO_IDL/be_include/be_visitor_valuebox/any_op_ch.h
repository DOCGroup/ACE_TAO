/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    any_op_ch.h
 *
 *  Concrete visitor for the valuebox node.
 *  This one provides code generation for the Any operators
 *
 *  @author Gary Maxey
 */
//=============================================================================


#ifndef _BE_VALUEBOX_ANY_OP_CH_H_
#define _BE_VALUEBOX_ANY_OP_CH_H_

/**
 * @class be_visitor_valuebox_any_op_ch
 *
 * @brief be_visitor_valuebox_any_op_ch
 *
 * This is a concrete visitor for valueboxes that generates the Any
 * operator declarations.
 */
class be_visitor_valuebox_any_op_ch : public be_visitor_valuebox
{
public:
  /// constructor
  explicit be_visitor_valuebox_any_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuebox_any_op_ch () override;

  be_visitor_valuebox_any_op_ch (be_visitor_valuebox_any_op_ch const &) = delete;
  be_visitor_valuebox_any_op_ch (be_visitor_valuebox_any_op_ch &&) = delete;
  be_visitor_valuebox_any_op_ch &operator= (be_visitor_valuebox_any_op_ch const &) = delete;
  be_visitor_valuebox_any_op_ch &operator= (be_visitor_valuebox_any_op_ch &&) = delete;

  /// visit valuebox
  int visit_valuebox (be_valuebox *node) override;
};

#endif /* _BE_VALUEBOX_ANY_OP_CH_H_ */
