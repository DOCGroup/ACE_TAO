/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    any_op_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the valuetype node.
 *  This one provides code generation for the Any operators
 *
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
  be_visitor_valuetype_any_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_any_op_ch (void);

  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);
};

#endif /* _BE_VALUETYPE_ANY_OP_CH_H_ */
