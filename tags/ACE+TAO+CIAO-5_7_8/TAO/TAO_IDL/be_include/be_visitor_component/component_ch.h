/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This one provides code generation for components in the client header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_COMPONENT_COMPONENT_CH_H_
#define _BE_COMPONENT_COMPONENT_CH_H_

/**
 * @class be_visitor_component_ch
 *
 * @brief be_visitor_component_ch
 *
 * This is a concrete visitor to generate the client header for component.
 */
class be_visitor_component_ch : public be_visitor_component
{
public:
  be_visitor_component_ch (be_visitor_context *ctx);
  ~be_visitor_component_ch (void);

  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);
};

#endif /* _BE_COMPONENT_COMPONENT_CH_H_ */
