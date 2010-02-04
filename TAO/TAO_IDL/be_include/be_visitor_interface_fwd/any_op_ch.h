/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    any_op_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the forward declared Interface node.
 *  This one provides code generation for the Any operators.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_INTERFACE_FWD_ANY_OP_CH_H_
#define _BE_INTERFACE_FWD_ANY_OP_CH_H_

/**
 * @class be_visitor_interface_fwd_any_op_ch
 *
 * @brief be_visitor_interface_fwd_any_op_ch
 *
 * This is a concrete visitor for forward declared interfaces that
 * generates the Any operator declaration.
 */
class be_visitor_interface_fwd_any_op_ch : public be_visitor_decl
{

public:
  /// constructor
  be_visitor_interface_fwd_any_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_interface_fwd_any_op_ch (void);

  /// visit interface_fwd
  virtual int visit_interface_fwd (be_interface_fwd *node);
};

#endif /* _BE_INTERFACE_FWD_ANY_OP_CH_H_ */
