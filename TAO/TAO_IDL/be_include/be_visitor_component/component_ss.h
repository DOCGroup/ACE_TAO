/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_ss.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation in the server skeleton
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_COMPONENT_COMPONENT_SS_H_
#define _BE_COMPONENT_COMPONENT_SS_H_

/**
 * @class be_visitor_component_ss
 *
 * @brief be_visitor_component_ss
 *
 * This is a concrete visitor to generate 
 * the server skeleton for component.
 */
class be_visitor_component_ss : public be_visitor_component
{
public:
  /// constructor
  be_visitor_component_ss (be_visitor_context *ctx);
  
  /// destructor
  ~be_visitor_component_ss (void);
  
  /// set the right context and make a visitor
  virtual int visit_component (be_component *node);
};

#endif /* _BE_COMPONENT_COMPONENT_SS_H_ */
