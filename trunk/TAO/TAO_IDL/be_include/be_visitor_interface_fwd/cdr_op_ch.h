/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the InterfaceFwd class
 *  This one provides code generation for the CDR operators for the forward
 *  declared interface in the client header, if the node is not later defined
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_VISITOR_INTERFACE_FWD_CDR_OP_CH_H_
#define _BE_VISITOR_INTERFACE_FWD_CDR_OP_CH_H_

/**
 * @class be_visitor_interface_fwd_cdr_op_ch
 *
 * @brief be_visitor_interface_fwd_cdr_op_ch
 *
 * This is a concrete visitor for forward declared interface that generates
 * the CDR operator declarations, if the node is not later defined.
 */
class be_visitor_interface_fwd_cdr_op_ch : public be_visitor_decl
{

public:
  /// constructor
  be_visitor_interface_fwd_cdr_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_interface_fwd_cdr_op_ch (void);

  /// visit forward declared interface
  virtual int visit_interface_fwd (be_interface_fwd *node);
};

#endif /* _BE_VISITOR_INTERFACE_FWD_CDR_OP_CH_H_ */
