/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    any_op_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This one provides code generation for the Any operators
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_ANY_OP_CS_H_
#define _BE_COMPONENT_ANY_OP_CS_H_

/**
 * @class be_visitor_component_any_op_cs
 *
 * @brief be_visitor_interface_any_op_cs
 *
 * This is a concrete visitor for interface that generates the Any operator
 * implementations
 */
class be_visitor_component_any_op_cs : public be_visitor_component
{
public:
  be_visitor_component_any_op_cs (be_visitor_context *ctx);
  ~be_visitor_component_any_op_cs (void);

  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);
};

#endif /* _BE_COMPONENT_ANY_OP_CS_H_ */
