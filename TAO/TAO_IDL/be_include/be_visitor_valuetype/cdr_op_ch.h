/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for valuetypes.
 *  This one provides code generation for the CDR operators.
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUETYPE_CDR_OP_CH_H_
#define _BE_VISITOR_VALUETYPE_CDR_OP_CH_H_

/**
 * @class be_visitor_valuetype_cdr_op_ch
 *
 * @brief be_visitor_valuetype_cdr_op_ch
 *
 * This is a concrete visitor for valuetype that generates the CDR operator
 * declarations
 */
class be_visitor_valuetype_cdr_op_ch : public be_visitor_valuetype
{

public:
  /// constructor
  be_visitor_valuetype_cdr_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_cdr_op_ch (void);

  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);
};

#endif /* _BE_VISITOR_VALUETYPE_CDR_OP_CH_H_ */
