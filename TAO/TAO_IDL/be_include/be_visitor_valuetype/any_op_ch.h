/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    any_op_ch.h
 *
 *  Concrete visitor for the valuetype node.
 *  This one provides code generation for the Any operators
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef _BE_VALUETYPE_ANY_OP_CH_H_
#define _BE_VALUETYPE_ANY_OP_CH_H_

/**
 * @class be_visitor_valuetype_any_op_ch
 *
 * @brief be_visitor_valuetype_any_op_ch
 *
 * This is a concrete visitor for valuetypes that generates the Any
 * operator declarations.
 */
class be_visitor_valuetype_any_op_ch : public be_visitor_valuetype
{
public:
  /// constructor
  explicit be_visitor_valuetype_any_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_any_op_ch () override;
  be_visitor_valuetype_any_op_ch (be_visitor_valuetype_any_op_ch const &) = delete;
  be_visitor_valuetype_any_op_ch (be_visitor_valuetype_any_op_ch &&) = delete;
  be_visitor_valuetype_any_op_ch &operator= (be_visitor_valuetype_any_op_ch const &) = delete;
  be_visitor_valuetype_any_op_ch &operator= (be_visitor_valuetype_any_op_ch &&) = delete;

  int visit_valuetype (be_valuetype *node) override;
  int visit_eventtype (be_eventtype *node) override;
};

#endif /* _BE_VALUETYPE_ANY_OP_CH_H_ */
