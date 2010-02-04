/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    component_ih.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation in the implementation header
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_COMPONENT_COMPONENT_IH_H_
#define _BE_COMPONENT_COMPONENT_IH_H_

/**
 * @class be_visitor_component_ih
 *
 * @brief be_visitor_component_ih
 *
 * This is a concrete visitor to generate 
 * the implementation header for component
 */
class be_visitor_component_ih : public be_visitor_component
{
public:
  /// constructor
  be_visitor_component_ih (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_component_ih (void);

  /// set the right context and make a visitor
  virtual int visit_component (be_component *node);
};

#endif /* _BE_COMPONENT_COMPONENT_IH_H_ */
