/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component class
 *  This one provides code generation for the CDR operators for the
 *  component in the client header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_VISITOR_COMPONENT_CDR_OP_CH_H_
#define _BE_VISITOR_COMPONENT_CDR_OP_CH_H_

/**
 * @class be_visitor_component_cdr_op_ch
 *
 * @brief be_visitor_component_cdr_op_ch
 *
 * This is a concrete visitor for component that generates the CDR operator
 * declarations
 */
class be_visitor_component_cdr_op_ch : public be_visitor_component
{
public:
  be_visitor_component_cdr_op_ch (be_visitor_context *ctx);
  ~be_visitor_component_cdr_op_ch (void);

  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);
};

#endif /* _BE_VISITOR_COMPONENT_CDR_OP_CH_H_ */
