/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    component_is.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation in the implementation skeleton
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_COMPONENT_COMPONENT_IS_H_
#define _BE_COMPONENT_COMPONENT_IS_H_

/**
 * @class be_visitor_component_is
 *
 * @brief be_visitor_component_ih
 *
 * This is a concrete visitor to generate 
 * the implementation skeleton for component
 */
class be_visitor_component_is : public be_visitor_component
{
public:
  /// constructor
  be_visitor_component_is (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_component_is (void);

  /// set the right context and make a visitor
  virtual int visit_component (be_component *node);
};

#endif /* _BE_COMPONENT_COMPONENT_IS_H_ */
