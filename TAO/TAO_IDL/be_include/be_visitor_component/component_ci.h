/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_ci.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This one provides code generation for components in the client inline.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_COMPONENT_COMPONENT_CI_H_
#define _BE_COMPONENT_COMPONENT_CI_H_

/**
 * @class be_visitor_component_ci
 *
 * @brief be_visitor_interface_ci
 *
 * This is a concrete visitor to generate the client header for component.
 */
class be_visitor_component_ci : public be_visitor_component
{
public:
  /// constructor
  be_visitor_component_ci (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_component_ci (void);

  /// set the right context and make a visitor
  virtual int visit_component (be_component *node);
};

#endif /* _BE_COMPONENT_COMPONENT_CI_H_ */
