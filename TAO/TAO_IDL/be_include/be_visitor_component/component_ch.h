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
  /// constructor
  be_visitor_component_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_component_ch (void);

  /// set the right context and make a visitor
  virtual int visit_component (be_component *node);
};

#endif /* _BE_COMPONENT_COMPONENT_CH_H_ */
