/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    any_op_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the forward declared Valuetype node.
 *  This one provides code generation for the Any operators.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_VALUETYPE_FWD_ANY_OP_CH_H_
#define _BE_VALUETYPE_FWD_ANY_OP_CH_H_

/**
 * @class be_visitor_valuetype_fwd_any_op_ch
 *
 * @brief be_visitor_valuetype_fwd_any_op_ch
 *
 * This is a concrete visitor for forward declared valuetypes that
 * generates the Any operator declaration.
 */
class be_visitor_valuetype_fwd_any_op_ch : public be_visitor_decl
{

public:
  /// constructor
  be_visitor_valuetype_fwd_any_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_fwd_any_op_ch (void);

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);
};

#endif /* _BE_VALUETYPE_FWD_ANY_OP_CH_H_ */
